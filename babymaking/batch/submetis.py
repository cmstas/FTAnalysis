import time
import itertools
import json
import traceback

from metis.Sample import SNTSample
from metis.LocalMergeTask import LocalMergeTask
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser
from metis.Utils import send_email


if __name__ == "__main__":

    tag = "v0_PMtest"
    merged_dir = "/nfs-7/userdata/namin/tupler_babies/merged/FT/{}/output/".format(tag)
    tarfile = "inputs/package.tar.xz"
    input_executable = "inputs/condor_executable_metis.sh"
    dsname = "/TTTW_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"
    shortname = "TTTW"
    task = CondorTask(
            sample = SNTSample(
                dataset=dsname,
                ),
            files_per_output = 2,
            output_name = "output.root",
            tag = tag,
            condor_submit_params = {"use_xrootd":True},
            cmssw_version = "CMSSW_9_2_8",
            input_executable = input_executable,
            tarfile = tarfile,
    )

    merge_task = LocalMergeTask(
            input_filenames=task.get_sample().get_files(),
            output_filename="{}/{}.root".format(merged_dir,shortname)
            )

    if not task.complete():
        task.process()
    else:
        if not merge_task.complete():
            merge_task.process()
