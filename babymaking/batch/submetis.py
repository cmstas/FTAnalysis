import sys
import glob
import os
import time
import itertools
import json
import traceback

from metis.Sample import SNTSample,DirectorySample
from metis.LocalMergeTask import LocalMergeTask
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser
from metis.Utils import send_email

def get_event_rate(fname):
    fname = fname.replace(".err", ".out")
    rate = -1
    if not os.path.exists(fname): return rate
    with open(fname, "r") as fhin:
        for line in fhin:
            if line.startswith("Processed "):
                parts = line.split()
                rate = int(parts[1])/float(parts[4])
                break
    return rate

def split_func(dsname):
    if any(x in dsname for x in [
        "/W","/Z","/TTJets","/DY","/ST",
        ]):
        return 5
    elif "/EGamma" in dsname:
        return 12
    elif "Run201" in dsname:
        return 7
    else:
        return 3

if __name__ == "__main__":

    from samples import data_2016, mc_2016, data_2017, mc_2017, data_2018, mc_2018, data_2016_94x, mc_2016_94x

    year_sample_map = [("2016",data_2016+mc_2016)] + [("2017",data_2017+mc_2017)] + [("2018",data_2018)] + [("2016_94x",data_2016_94x+mc_2016_94x)]
    # year_sample_map = [("2016_94x",data_2016_94x+mc_2016_94x)]

    # # FIXME FIXME
    # year_sample_map = [(y,sm[-1:]) for y,sm in year_sample_map]

    # tag = "v3.05_all"
    # tag = "v3.08_all"
    tag = "v3.09_all"


    # year_sample_map = [
    #         (2016, [
    #             ["/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "TTDL"],
    #             ["/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "TTSLtop"],
    #             ["/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "TTSLtopbar"],
    #             ]),
    #         (2017, [
    #             ["/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTDL"],
    #             ["/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTSLtop"],
    #             ["/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTSLtopbar"],
    #             ]),
    #         ]

    # year_sample_map = [(2017,mc_2017[:4])] # FIXME

    skip_tail = False
    
    # Loop 'til we get dizzy
    for i in range(1000):
        ndone = 0
        total_summary = {}
        nsamples = 0
        for year, sample_map in year_sample_map:
            merged_dir = "/nfs-7/userdata/{}/tupler_babies/merged/FT/{}/output/year_{}".format(os.getenv("USER"),tag,year)
            for dsname,shortname in sample_map:
                if "/hadoop/" in dsname:
                    sample = DirectorySample(
                        dataset=dsname.split("|",1)[0].strip(),
                        location=dsname.split("|",1)[1].strip(),
                        # tag="CMS4_V09-04-13", # if not specified, get latest tag
                        )
                else:
                    sample = SNTSample(
                        dataset=dsname,
                        # tag="CMS4_V09-04-13", # if not specified, get latest tag
                        )
                task = CondorTask(
                        sample = sample,
                        files_per_output = split_func(dsname),
                        output_name = "output.root",
                        tag = tag,
                        min_completion_fraction = 0.5 if skip_tail else 1.0,
                        # min_completion_fraction = 1.0 if not "/SMS" in dsname else 0.9,
                        # condor_submit_params = {"use_xrootd":True},
                        condor_submit_params = {"sites":"T2_US_UCSD"}, # I/O is hella faster
                        # condor_submit_params = {"use_xrootd":True},
                        # condor_submit_params = {"sites":"T2_US_UCSD,UCSB"}, # I/O is hella faster
                        cmssw_version = "CMSSW_9_2_8",
                        input_executable = "inputs/condor_executable_metis.sh",
                        tarfile = "inputs/package.tar.xz",
                        special_dir = "FTbabies/",
                )
                merge_task = LocalMergeTask(
                        input_filenames=task.get_outputs(),
                        output_filename="{}/{}.root".format(merged_dir,shortname),
                        ignore_bad = skip_tail,
                        # ignore_bad = "/SMS" in dsname,
                        )
                nsamples += 1
                if not task.complete():
                    task.process()
                else:
                    if not merge_task.complete():
                        merge_task.process()
                    else:
                        ndone += 1
                        
                total_summary[dsname.split("|")[0]] = task.get_task_summary()

        StatsParser(data=total_summary, webdir="~/public_html/dump/FTmetis/").do(
                custom_event_rate_parser=get_event_rate,
                )

        if ndone == nsamples:
            print "All done!"
            sys.exit()

        time.sleep(1800 if i < 10 else 3*60*60)

