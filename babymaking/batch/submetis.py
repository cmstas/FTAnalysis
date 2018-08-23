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
    elif "Run201" in dsname:
        return 6
    else:
        return 2

if __name__ == "__main__":

    from samples import data_2016, mc_2016, data_2017, mc_2017, data_2018, mc_2018

    # sample_map = [
    #         ["/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"    , "TTBAR_PH"],
    #         # ["/TTTT_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTTTnew"],
    #         ["/TTTW_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"          , "TTTW"],
    #         ]
    sample_map = data_2017+mc_2017

    # sample_map = [
    #         ["/SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM", "T1TTTT"],
    #         ]

    # tag = "v3.01"

    # sample_map = [
    #         ["/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v2/MINIAODSIM", "DY_low"],
    #         ["/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017RECOSIMstep_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM", "DY_highext1"],
    #         ["/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTBAR_PH"],
    #         ["/TTTT_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM","TTTTnew"],
    #         ["/DoubleEG/Run2017B-31Mar2018-v1/MINIAOD", "DataDoubleEGB"],
    #         ["/DoubleEG/Run2017C-31Mar2018-v1/MINIAOD", "DataDoubleEGC"],
    #         ["/DoubleEG/Run2017D-31Mar2018-v1/MINIAOD", "DataDoubleEGD"],
    #         ["/DoubleEG/Run2017E-31Mar2018-v1/MINIAOD", "DataDoubleEGE"],
    #         ["/DoubleEG/Run2017F-31Mar2018-v1/MINIAOD", "DataDoubleEGF"],
    #         ]
    # tag = "v3.01_nmiss0"

    # sample_map = [
    #         ["/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v2/MINIAODSIM", "DY_low"],
    #         ["/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017RECOSIMstep_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "DY_high"],
    #         ["/DoubleEG/Run2017B-31Mar2018-v1/MINIAOD|/hadoop/cms/store/group/snt/run2_data2017//DoubleEG_Run2017B-31Mar2018-v1_MINIAOD_CMS4_V09-04-18/", "DataDoubleEGB"],
    #         ["/DoubleEG/Run2017C-31Mar2018-v1/MINIAOD|/hadoop/cms/store/group/snt/run2_data2017//DoubleEG_Run2017C-31Mar2018-v1_MINIAOD_CMS4_V09-04-18/", "DataDoubleEGC"],
    #         ["/DoubleEG/Run2017D-31Mar2018-v1/MINIAOD|/hadoop/cms/store/group/snt/run2_data2017//DoubleEG_Run2017D-31Mar2018-v1_MINIAOD_CMS4_V09-04-18/", "DataDoubleEGD"],
    #         ["/DoubleEG/Run2017E-31Mar2018-v1/MINIAOD|/hadoop/cms/store/group/snt/run2_data2017//DoubleEG_Run2017E-31Mar2018-v1_MINIAOD_CMS4_V09-04-18/", "DataDoubleEGE"],
    #         ["/DoubleEG/Run2017F-31Mar2018-v1/MINIAOD|/hadoop/cms/store/group/snt/run2_data2017//DoubleEG_Run2017F-31Mar2018-v1_MINIAOD_CMS4_V09-04-18/", "DataDoubleEGF"],
    #         ]
    # tag = "v3.02_nmiss0_newmet"

    sample_map = [
            ["/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "STantitop"],
            ["/ST_tW_top_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "STtop"],
            ["/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTWnlo"],
            ["/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTZnlo"],
            ["/TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTZLOW"],
            ["/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTHtoNonBB"],
            ["/WW_DoubleScattering_13TeV-pythia8_TuneCP5/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "WWDPS"],
            ["/WW_TuneCP5_13TeV-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM", "WW"],
            ["/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "WZ"],
            ["/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "ZZ"],
            ["/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "WWW"],
            ["/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "WWZ"],
            ["/WZG_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "WZG"],
            ["/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "WZZ"],
            ["/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "ZZZ"],
            ["/tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TZQ"],
            ["/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM", "VHtoNonBB"],
            ]
    tag = "v3.02_nmiss0_osv3"

    # FIXME: want to add 2016,2017,2018 to merged_dir eventually
    merged_dir = "/nfs-7/userdata/{}/tupler_babies/merged/FT/{}/output/".format(os.getenv("USER"),tag)
    
    # Loop 'til we get dizzy
    for i in range(1000):
        ndone = 0
        total_summary = {}
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
                    # min_completion_fraction = 1.0 if not "/SMS" in dsname else 0.9,
                    # condor_submit_params = {"use_xrootd":True},
                    condor_submit_params = {"sites":"T2_US_UCSD"}, # I/O is hella faster
                    cmssw_version = "CMSSW_9_2_8",
                    input_executable = "inputs/condor_executable_metis.sh",
                    tarfile = "inputs/package.tar.xz",
                    special_dir = "FTbabies/",
            )
            merge_task = LocalMergeTask(
                    input_filenames=task.get_outputs(),
                    output_filename="{}/{}.root".format(merged_dir,shortname),
                    # ignore_bad = "/SMS" in dsname,
                    )
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

        if ndone == len(sample_map):
            print "All done!"
            sys.exit()

        time.sleep(10*60)

