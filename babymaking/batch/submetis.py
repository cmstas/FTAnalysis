import sys
import glob
import os
import time
import itertools
import json
import traceback

from metis.Sample import SNTSample
from metis.LocalMergeTask import LocalMergeTask
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser
from metis.Utils import send_email

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

    sample_map = [

            ["/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"    , "TTBAR_PH"],
            # ["/TTTT_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTTTnew"],
            ["/TTTW_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"          , "TTTW"],

            ]

    tag = "v1_PMtest"

    # FIXME: want to add 2016,2017,2018 to merged_dir eventually
    merged_dir = "/nfs-7/userdata/{}/tupler_babies/merged/FT/{}/output/".format(os.getenv("USER"),tag)
    
    for i in range(100):
        ndone = 0
        total_summary = {}
        for dsname,shortname in sample_map:
            task = CondorTask(
                    sample = SNTSample(
                        dataset=dsname,
                        # tag="CMS4_V09-04-13", # if not specified, get latest tag
                        ),
                    files_per_output = split_func(dsname),
                    output_name = "output.root",
                    tag = tag,
                    # condor_submit_params = {"use_xrootd":True},
                    condor_submit_params = {"sites":"T2_US_UCSD"}, # I/O is hella faster
                    cmssw_version = "CMSSW_9_2_8",
                    input_executable = "inputs/condor_executable_metis.sh",
                    tarfile = "inputs/package.tar.xz",
                    special_dir = "FTbabies/",
            )
            merge_task = LocalMergeTask(
                    input_filenames=task.get_outputs(),
                    output_filename="{}/{}.root".format(merged_dir,shortname)
                    )
            if not task.complete():
                task.process()
            else:
                if not merge_task.complete():
                    merge_task.process()
                else:
                    ndone += 1
                    
            total_summary[dsname] = task.get_task_summary()

        StatsParser(data=total_summary, webdir="~/public_html/dump/FTmetis/").do()

        if ndone == len(sample_map):
            print "All done!"
            sys.exit()

        time.sleep(10*60)

