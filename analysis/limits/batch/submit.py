import os
import glob
import sys

from metis.Utils import get_proxy_file, do_cmd, get_timestamp, file_chunker
from metis.Task import Task
from metis.File import File, MutableFile
import metis.Utils as Utils
from metis.Sample import DirectorySample

import time


def condor_submit(**kwargs): # pragma: no cover
    """
    Takes in various keyword arguments to submit a condor job.
    Returns (succeeded:bool, cluster_id:str)
    fake=True kwarg returns (True, -1)
    multiple=True will let `arguments` and `selection_pairs` be lists (of lists)
    and will queue up one job for each element
    """

    if kwargs.get("fake",False):
        return True, -1

    for needed in ["executable","arguments","inputfiles","logdir"]:
        if needed not in kwargs:
            raise RuntimeError("To submit a proper condor job, please specify: {0}".format(needed))

    params = {}

    queue_multiple = kwargs.get("multiple",False)

    params["universe"] = kwargs.get("universe", "Vanilla")
    params["executable"] = kwargs["executable"]
    # params["inputfiles"] = ",".join(kwargs["inputfiles"])
    params["logdir"] = kwargs["logdir"]
    params["proxy"] = get_proxy_file()
    params["timestamp"] = get_timestamp()


    exe_dir = params["executable"].rsplit("/",1)[0]
    if "/" not in os.path.normpath(params["executable"]):
        exe_dir = "."

    # http://uaf-10.t2.ucsd.edu/~namin/dump/badsites.html
    good_sites = [

                "T2_US_Caltech",
                "T2_US_UCSD",
                "T2_US_MIT",
                "T2_US_Nebraska",
                "T2_US_Purdue",
                "T2_US_Vanderbilt",
                "UCSB",

                # "UAF", # bad (don't spam uafs!!)

            ]

    # if kwargs.get("use_xrootd", False): params["sites"] = kwargs.get("sites","T2_US_UCSD,T2_US_Wisconsin,T2_US_Florida,T2_US_Nebraska,T2_US_Caltech,T2_US_MIT,T2_US_Purdue")
    # if kwargs.get("use_xrootd", False): params["sites"] = kwargs.get("sites","T2_US_UCSD,T2_US_Caltech,T2_US_Wisconsin,T2_US_MIT")
    params["sites"] = kwargs.get("sites",",".join(good_sites))
    # if kwargs.get("use_xrootd", False): params["sites"] = kwargs.get("sites",",".join(good_sites))
    # else: params["sites"] = kwargs.get("sites","T2_US_UCSD")
    # if os.getenv("USER") in ["namin"] and "T2_US_UCSD" in params["sites"]:
    #     params["sites"] += ",UAF,UCSB"

    if queue_multiple:
        if len(kwargs["arguments"]) and (type(kwargs["arguments"][0]) not in [tuple,list]):
            raise RuntimeError("If queueing multiple jobs in one cluster_id, arguments must be a list of lists")
        params["arguments"] = map(lambda x: " ".join(map(str,x)), kwargs["arguments"])
        params["inputfiles"] = map(lambda x: ",".join(map(str,x)), kwargs["inputfiles"])
        params["extra"] = []
        if "selection_pairs" in kwargs:
            sps = kwargs["selection_pairs"]
            if len(sps) != len(kwargs["arguments"]):
                raise RuntimeError("Selection pairs must match argument list in length")
            for sel_pairs in sps:
                extra = ""
                for sel_pair in sel_pairs:
                    if len(sel_pair) != 2:
                        raise RuntimeError("This selection pair is not a 2-tuple: {0}".format(str(sel_pair)))
                    extra += '+{0}="{1}"\n'.format(*sel_pair)
                params["extra"].append(extra)
    else:
        params["arguments"] = " ".join(map(str,kwargs["arguments"]))
        params["inputfiles"] = ",".join(map(str,kwargs["inputfiles"]))
        params["extra"] = ""
        if "selection_pairs" in kwargs:
            for sel_pair in kwargs["selection_pairs"]:
                if len(sel_pair) != 2:
                    raise RuntimeError("This selection pair is not a 2-tuple: {0}".format(str(sel_pair)))
                params["extra"] += '+{0}="{1}"\n'.format(*sel_pair)

    params["proxyline"] = "x509userproxy={proxy}".format(proxy=params["proxy"])

    # Require singularity+cvmfs unless machine is uaf-*. or uafino.
    # NOTE, double {{ and }} because this gets str.format'ted later on
    # Must have singularity&cvmfs. Or, (it must be uaf or uafino computer AND if a uaf computer must not have too high of slotID number
    # so that we don't take all the cores of a uaf
    requirements_line = 'Requirements = ((HAS_SINGULARITY=?=True) && (HAS_CVMFS_cms_cern_ch =?= true)) || (regexp("(uaf-[0-9]{{1,2}}|uafino)\.", TARGET.Machine) && !(TARGET.SlotID>(TotalSlots<14 ? 3:7) && regexp("uaf-[0-9]", TARGET.machine)))'
    if kwargs.get("universe","").strip().lower() in ["local"]:
        kwargs["requirements_line"] = "Requirements = "
    if kwargs.get("requirements_line","").strip():
        requirements_line = kwargs["requirements_line"]

    template = """
universe={universe}
+DESIRED_Sites="{sites}"
executable={executable}
transfer_executable=True
transfer_output_files = ""
+Owner = undefined
+project_Name = \"cmssurfandturf\"
log={logdir}/{timestamp}.log
output={logdir}/std_logs/1e.$(Cluster).$(Process).out
error={logdir}/std_logs/1e.$(Cluster).$(Process).err
notification=Never
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
"""
    template += "{0}\n".format(params["proxyline"])
    template += "{0}\n".format(requirements_line)
    if kwargs.get("stream_logs",False):
        template += "StreamOut=True\nstream_error=True\nTransferOut=True\nTransferErr=True\n"
    for ad in kwargs.get("classads",[]):
        if len(ad) != 2:
            raise RuntimeError("This classad pair is not a 2-tuple: {0}".format(str(ad)))
        template += '+{0}="{1}"\n'.format(*ad)
    do_extra = len(params["extra"]) == len(params["arguments"])
    if queue_multiple:
        template += "\n"
        for ijob,(args,inp) in enumerate(zip(params["arguments"],params["inputfiles"])):
            template += "arguments={0}\n".format(args)
            template += "transfer_input_files={0}\n".format(inp)
            if do_extra:
                template += "{0}\n".format(params["extra"][ijob])
            template += "queue\n"
            template += "\n"
    else:
        template += "arguments={0}\n".format(params["arguments"])
        template += "transfer_input_files={0}\n".format(params["inputfiles"])
        template += "{0}\n".format(params["extra"])
        template += "queue\n"

    if kwargs.get("return_template",False):
        return template.format(**params)

    buff = template.format(**params)

    with open("{0}/submit.cmd".format(exe_dir),"w") as fhout:
        fhout.write(buff)

    extra_cli = ""
    schedd = kwargs.get("schedd","") # see note in condor_q about `schedd`
    if schedd:
        extra_cli += " -name {} ".format(schedd)
    _ = do_cmd("mkdir -p {}/std_logs/ ".format(params["logdir"]))

    # print buff
    # # FIXME
    print "Wrote the file, and now exiting"
    sys.exit()

    out = do_cmd("condor_submit {}/submit.cmd {}".format(exe_dir,extra_cli))


    succeeded = False
    cluster_id = -1
    if "job(s) submitted to cluster" in out:
        succeeded = True
        cluster_id = out.split("submitted to cluster ")[-1].split(".",1)[0].strip()
    else:
        raise RuntimeError("Couldn't submit job to cluster because:\n----\n{0}\n----".format(out))

    return succeeded, cluster_id

if __name__ == "__main__":

    # tag="v0"
    # outputdir = "/hadoop/cms/store/user/namin/batch_limits/{tag}/".format(tag=tag)
    # s = DirectorySample(dataset="/SS/batchlimits/LIM",location="../v3.27_mar4_t5tttt_v1/",globber="card_*_all_run2.root")
    # chunks,_ = file_chunker(sorted(s.get_files()), files_per_output=5, flush=True)
    # arguments = []
    # inputfiles = []
    # for i,chunk in enumerate(chunks):
    #     arguments.append([outputdir, tag, i+1])
    #     inputfiles.append(["package.tar.xz"]+[x.get_name() for x in chunk])
    # condor_submit(
    #         multiple=True,
    #         executable="executable.sh",
    #         arguments=arguments,
    #         inputfiles=inputfiles,
    #         logdir="logs/{}".format(tag),
    #         )

    # tag="v1"
    # outputdir = "/hadoop/cms/store/user/namin/batch_limits/{tag}/".format(tag=tag)
    # s = DirectorySample(dataset="/SS/batchlimits/LIM",location="../v3.28_ss_mar9_allsignals_v1/",globber="card_*_all_run2.root")
    # chunks,_ = file_chunker(sorted(s.get_files()), files_per_output=5, flush=True)
    # arguments = []
    # inputfiles = []
    # done_indices = set(map(lambda x:int(x.rsplit("_",1)[-1].split(".",1)[0]),glob.glob(outputdir+"/*.txt")))
    # for i,chunk in enumerate(chunks):
    #     idx = i+1
    #     if idx in done_indices: continue
    #     arguments.append([outputdir, tag, idx])
    #     inputfiles.append(["package.tar.xz"]+[x.get_name() for x in chunk])
    # print "Submitting {} jobs".format(len(arguments))
    # condor_submit(
    #         multiple=True,
    #         executable="executable.sh",
    #         arguments=arguments,
    #         inputfiles=inputfiles,
    #         logdir="logs/{}".format(tag),
    #         )

    # tag="v2"
    # outputdir = "/hadoop/cms/store/user/namin/batch_limits/{tag}/".format(tag=tag)
    # s = DirectorySample(dataset="/SS/batchlimits/LIM",location="../v3.28_ss_mar12allsigs_v1/",globber="card_*_all_run2.root")
    # chunks,_ = file_chunker(sorted(s.get_files()), files_per_output=10, flush=True)
    # arguments = []
    # inputfiles = []
    # done_indices = set(map(lambda x:int(x.rsplit("_",1)[-1].split(".",1)[0]),glob.glob(outputdir+"/*.txt")))
    # for i,chunk in enumerate(chunks):
    #     idx = i+1
    #     if idx in done_indices: continue
    #     arguments.append([outputdir, tag, idx])
    #     inputfiles.append(["package.tar.xz"]+[x.get_name() for x in chunk])
    # print "Submitting {} jobs".format(len(arguments))
    # condor_submit(
    #         multiple=True,
    #         executable="executable.sh",
    #         arguments=arguments,
    #         inputfiles=inputfiles,
    #         logdir="logs/{}".format(tag),
    #         )

#     tag="v3"
#     outputdir = "/hadoop/cms/store/user/namin/batch_limits/{tag}/".format(tag=tag)
#     s = DirectorySample(dataset="/SS/batchlimits/LIM",location="../v3.28_ss_mar13allsigs_v1/",globber="card_*_all_run2.root")
#     chunks,_ = file_chunker(sorted(s.get_files()), files_per_output=10, flush=True)
#     arguments = []
#     inputfiles = []
#     done_indices = set(map(lambda x:int(x.rsplit("_",1)[-1].split(".",1)[0]),glob.glob(outputdir+"/*.txt")))
#     for i,chunk in enumerate(chunks):
#         idx = i+1
#         if idx in done_indices: continue
#         arguments.append([outputdir, tag, idx])
#         inputfiles.append(["package.tar.xz"]+[x.get_name() for x in chunk])
#     print "Submitting {} jobs".format(len(arguments))
#     condor_submit(
#             multiple=True,
#             executable="executable.sh",
#             arguments=arguments,
#             inputfiles=inputfiles,
#             logdir="logs/{}".format(tag),
#             )

    tag="v6c"
    outputdir = "/hadoop/cms/store/user/namin/batch_limits/{tag}/".format(tag=tag)
    s = DirectorySample(dataset="/SS/batchlimits/LIM",location="../v3.31_ss_Jun26_v1c/",globber="card_*_all_run2.root")
    chunks,_ = file_chunker(sorted(s.get_files()), files_per_output=6, flush=True)
    arguments = []
    inputfiles = []
    done_indices = set(map(lambda x:int(x.rsplit("_",1)[-1].split(".",1)[0]),glob.glob(outputdir+"/*.txt")))
    for i,chunk in enumerate(chunks):
        idx = i+1
        if idx in done_indices: continue
        arguments.append([outputdir, tag, idx])
        inputfiles.append(["package.tar.xz"]+[x.get_name() for x in chunk])
    print "Submitting {} jobs".format(len(arguments))
    condor_submit(
            multiple=True,
            executable="executable.sh",
            arguments=arguments,
            inputfiles=inputfiles,
            logdir="logs/{}".format(tag),
            )
