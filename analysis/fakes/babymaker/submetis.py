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
                den = float(parts[4])
                if den < 1e-6: break
                rate = 1.0*int(parts[1])/den
                break
    return rate

def split_func(dsname):
    if "/TTW" in dsname: return 3
    if "/TTZ" in dsname: return 3
    if "/W" in dsname: return 12
    if "/TT" in dsname: return 10
    if "/QCD" in dsname: return 15
    if "/DY" in dsname: return 8
    if "/EGamma" in dsname: return 15
    if "/Single" in dsname: return 15
    if "/DoubleMu" in dsname: return 11
    return 10

if __name__ == "__main__":

    from samples import *

    # year_sample_map = [("2016",data_2016+mc_2016)] + [("2017",data_2017+mc_2017)] + [("2018",data_2018+mc_2018)] + [("2016_94x",data_2016_94x+mc_2016_94x)]
    year_sample_map = [("2016",data_2016+mc_2016)] + [("2017",data_2017+mc_2017)] + [("2018",data_2018+mc_2018)]
    # tag = "v1.0_leptontree" # 2017, 2018 only
    # tag = "v2.0_leptontree" # 2017, 2018 only
    extra_args = ""
    tag_match = ""

    # # year_sample_map = [("2018",mc_2018)]
    # year_sample_map = [("2018",mc_2018+data_2018)]
    # # year_sample_map = [("2016",data_2016)]
    # # tag = "v2.0_leptontree"
    # tag = "v4.0_leptontree_2018resid17"

    year_sample_map = [("2018",
        [
        ["/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM", "DY_high"],
        ["/WW_TuneCP5_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM", "WW"],
        ["/WZ_TuneCP5_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v3/MINIAODSIM", "WZ"],
        ["/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM", "WJets_incl"],
        ["/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM", "WJets_HT100To200"],
        ["/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM", "WJets_HT200To400"],
        ["/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM", "WJets_HT400To600"],
        ["/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM", "WJets_HT600To800"],
        ["/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM", "WJets_HT800To1200"],
        ["/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM", "TTBAR_PH"],
        ["/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v3/MINIAODSIM","STantitop"],
        ["/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v3/MINIAODSIM", "STtop"],
        ["/EGamma/Run2018D-PromptReco-v2/MINIAOD", "DataDoubleEGDv2"],
        ["/SingleMuon/Run2018D-PromptReco-v2/MINIAOD", "DataDoubleMuonDv2"],
        ["/EGamma/Run2018A-17Sep2018-v2/MINIAOD", "ReRecoDataDoubleEGAv2"],
        ["/EGamma/Run2018B-17Sep2018-v1/MINIAOD", "ReRecoDataDoubleEGBv1"],
        ["/EGamma/Run2018C-17Sep2018-v1/MINIAOD", "ReRecoDataDoubleEGCv1"],
        ["/SingleMuon/Run2018A-17Sep2018-v2/MINIAOD", "ReRecoDataDoubleMuonAv2"],
        ["/SingleMuon/Run2018B-17Sep2018-v1/MINIAOD", "ReRecoDataDoubleMuonBv1"],
        ["/SingleMuon/Run2018C-17Sep2018-v1/MINIAOD", "ReRecoDataDoubleMuonCv1"],
        ["/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM", "TTWnlo"],
        ["/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM", "TTZnlo"],
            ]
        )]
    tag = "v3.0_isrbabies"
    extra_args = "--ignorebadfiles"
    tag_match = ""

    # extra_args = "--ignorebadfiles" # FIXME
    # year_sample_map = [("2016",data_2016)]
    # tag = "v2.1_test2016gpmva" # 2016 testing GP mva instead of nontrig branch

    # Loop 'til we get dizzy
    for i in range(1000):
        ndone = 0
        total_summary = {}
        nsamples = 0
        for year, sample_map in year_sample_map:
            merged_dir = "/nfs-7/userdata/{}/tupler_babies/merged/LeptonTree/{}/output/year_{}".format(os.getenv("USER"),tag,year)
            for dsname,shortname in sample_map:
                if "/hadoop/" in dsname:
                    sample = DirectorySample(
                        dataset=dsname.split("|",1)[0].strip(),
                        location=dsname.split("|",1)[1].strip(),
                        )
                else:
                    sample = SNTSample(
                        dataset=dsname,
                        exclude_tag_pattern="CMS4_V08-*", # ignore new samples by sicheng for 2016
                        tag=tag_match,
                        # tag="CMS4_V09-04-13", # if not specified, get latest tag
                        )
                skip_tail = "/SMS" in dsname
                # skip_tail = True
                task = CondorTask(
                        sample = sample,
                        files_per_output = split_func(dsname),
                        output_name = "output.root",
                        tag = tag,
                        min_completion_fraction = 0.93 if skip_tail else 1.0,
                        condor_submit_params = {
                            # "sites":"T2_US_UCSD,UCSB",  # I/O is hella faster
                            "sites":"T2_US_UCSD",  # I/O is hella faster
                            # "sites":"UAF",  # I/O is hella faster
                            "classads": [ 
                                ["metis_extraargs",extra_args],
                                ["JobBatchName","LT_{}_{}".format(year,shortname)],
                                ],
                            },
                        additional_input_files = ["/home/users/namin/2018/fourtop/all/FTAnalysis/babymaking/batch/condor_chirp"],
                        cmssw_version = "CMSSW_9_4_9",
                        scram_arch = "slc6_amd64_gcc630",
                        input_executable = "inputs/condor_executable_metis.sh",
                        tarfile = "inputs/package.tar.xz",
                        special_dir = "LTbabies/{}/".format(tag),
                        # recopy_inputs = True,
                )
                merge_task = LocalMergeTask(
                        input_filenames=task.get_outputs(),
                        output_filename="{}/{}.root".format(merged_dir,shortname),
                        ignore_bad = skip_tail,
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

        StatsParser(data=total_summary, webdir="~/public_html/dump/LTmetis/").do(
                custom_event_rate_parser=get_event_rate,
                )

        if ndone == nsamples:
            print "All done!"
            sys.exit()

        time.sleep(600 if i < 10 else 1.0*60*60)

