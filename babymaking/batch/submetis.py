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
    if any(x in dsname for x in [
        "/W","/Z","/TTJets","/DY","/ST",
        ]):
        return 5
    elif "/EGamma" in dsname:
        return 12
    elif "Run201" in dsname:
        return 7
    elif "/SMS-" in dsname:
        return 1
    else:
        return 3

if __name__ == "__main__":

    from samples import data_2016, mc_2016, data_2017, mc_2017, data_2018, mc_2018, data_2016_94x, mc_2016_94x

    # year_sample_map = [("2016",data_2016+mc_2016)] + [("2017",data_2017+mc_2017)] + [("2018",data_2018)] + [("2016_94x",data_2016_94x+mc_2016_94x)]
    year_sample_map = [("2016",data_2016+mc_2016)] + [("2017",data_2017+mc_2017)] + [("2018",data_2018+mc_2018)]
    # year_sample_map = [("2016_94x",data_2016_94x+mc_2016_94x)]
    # tag = "v3.16_all" # back to old V6 JECs
    # tag = "v3.17_all" # V6 JECs, pdf weight clipping, fixed JetID for jec/jer up/down
    # tag = "v3.19_all" # same as v17 with top tagging vars and 2 more potential bdt variables
    # tag = "v3.19_newdata" # to validate event counts with new prompt 2018Dv2
    # tag = "v3.20_jec32" # try JECv32 and new WPs with 2017, 2018 only
    # tag = "v3.21" # v32 JECs. all years, new 10-02-04 MC, including some 2018 MC -- bugged 2017 missing 2 ele iso branches
    # year_sample_map =  [("2017",data_2017+mc_2017)] # NOTE
    # tag = "v3.21_fix2017" # 2017 only. fixing iso issues
    year_sample_map =  [("2016",data_2016+mc_2016)]+[("2018",data_2018+mc_2018)] # NOTE
    # tag = "v3.22" # 2018 only. latest ID (v2), remade corruptions, all MC samples included I think
    tag = "v3.23" # 2016,2018 only. for 2018, latest ID (v2), remade corruptions, all MC samples included I think -- latest 102X btag WPs
    extra_args = ""
    tag_match = ""
    # extra_args = "--ignorebadfiles" # FIXME

    # year_sample_map[0][1][1:] = []
    # tag = "v3.21_test"
    # print year_sample_map

    # year_sample_map = [
    #         ("2017",[
    #     ["/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM","WWW"],
    #     ]),
    #         ]

    # year_sample_map = [
    #         ("2018",[
    #     ["/EGamma/Run2018D-PromptReco-v2/MINIAOD", "DataDoubleEGDv2"],
    #     ["/DoubleMuon/Run2018D-PromptReco-v2/MINIAOD", "DataDoubleMuonDv2"],
    #     ["/MuonEG/Run2018D-PromptReco-v2/MINIAOD", "DataMuonEGDv2"],
    #     ]),
    #         ]

    # year_sample_map = [("2017",data_2017+mc_2017)] + [("2018",data_2018)]


    # tag = "v3.09_newdeepflavv2"
    # year_sample_map = [
    #         ("2017",[
    #     # ["/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM|/hadoop/cms/store/group/snt/run2_mc2017//TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-18_newdeepflav/", "TTSL"],
    #     # ["/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTSL"],
    #     ["/TTTT_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM","TTTTnew"],
    #     ["/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTHtoNonBB"],
    #     ["/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTWnlo"],
    #     ["/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTZnlo"],
    #     ]),
    #         ("2018",[
    #     # ["/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIIFall18MiniAOD-102X_upgrade2018_realistic_v12-v1/MINIAODSIM", "TTSL"],
    #     ]),
    #         ]
    # tag_match = "CMS4*newdeep*"
    # # extra_args = "--ignorescale1fb --ignorebadfiles"
    # # split_func = lambda x: 5
    # extra_args = ""

    # tag = "v3.11_scalepdfunc"
    # year_sample_map = [("2016",mc_2016)] + [("2017",mc_2017)]
    # tag_match = ""
    # extra_args = ""
    # # extra_args = "--ignorebadfiles"

    # tag = "v3.11_all"
    # year_sample_map = [("2018",data_2018)]
    # tag_match = ""
    # extra_args = ""
    # # extra_args = "--ignorebadfiles"

    # tag = "v3.11_all"
    # year_sample_map = [("2016",mc_2016)]
    # tag_match = ""
    # extra_args = ""
    # # extra_args = "--ignorebadfiles"

    # # s = SNTSample(dataset=year_sample_map[0][1][2][0],tag="CMS4*newdeep*")
    # print s
    # print s.get_files()
    # sys.exit()

    # # FIXME FIXME
    # year_sample_map = [(y,sm[-1:]) for y,sm in year_sample_map]

    # tag = "v3.05_all"
    # tag = "v3.08_all"
    # tag = "v3.09_all"
    # year_sample_map = [("2016",data_2016+mc_2016)]
    # tag = "v3.09_lowpt3" # pt>10,15 for lep3, 2016 only

    # year_sample_map = [("2018",mc_2018)]
    # year_sample_map = [("2016_94x",mc_2016_94x)]
    # tag = "v3.09_all"

    # year_sample_map = [("2017",mc_2017)]
    # tag = "v3.09_deepflav"

    # year_sample_map = [("2017",[
    #     # ["/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "TTBAR_PH"],
    #     # # ["/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "TTDL"],
    #     # # ["/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "TTSLtop"],
    #     # # ["/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "TTSLtopbar"],
    #     # ["/TTTo2L2Nu_HT500Njet7_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "TTDLht500"],
    #     # ["/SMS-T6ttWW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM", "T6TTWW"],
    #     # ["/SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM", "T1TTTT"],
    #     ["/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM", "WJets_HT100To200"],
    #     ["/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "WJets_HT200To400"],
    #     ["/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "WJets_HT400To600"],
    #     ["/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "WJets_HT600To800"],
    #     ["/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "WJets_HT800To1200"],
    #     ["/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "WJets_HT1200To2500"],
    #     ])]
    # # tag = "v3.09_fakesv4"
    # tag = "v3.10_prefire"


    # year_sample_map = [("2018",data_2018+mc_2018)]
    # tag = "v3.10_all"

    # year_sample_map = [("2016",data_2016+mc_2016)]
    # tag = "v3.10_prefire"

    # year_sample_map = [
    #         (2016, [
    #             # ["/SMS-T6ttWW_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM", "T6TTWW"],
    #             # ["/SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM", "T1TTTT"],
    #             # ["/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "TTDL"],
    #             # ["/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "TTSLtop"],
    #             # ["/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "TTSLtopbar"],
    #             ]),
    #         (2017, [
    #             # ["/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTDL"],
    #             # ["/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTSLtop"],
    #             # ["/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM", "TTSLtopbar"],
    #             ["/TTPlus0Jet_DiLept_TuneCP5_amcatnlo/RunIIFall17MiniAODv2-Private-v1/CMS4", "TTdilep0jetext1"],
    #             ["/TTPlus0Jet_DiLept_TuneCP5_amcatnlo/RunIIFall17MiniAODv2-Private-v2/CMS4", "TTdilep0jetext2"],
    #             ["/TTPlus1Jet_DiLept_TuneCP5_amcatnlo/RunIIFall17MiniAODv2-Private-v1/CMS4", "TTdilep1jetext1"],
    #             ["/TTPlus1Jet_DiLept_TuneCP5_amcatnlo/RunIIFall17MiniAODv2-Private-v2/CMS4", "TTdilep1jetext2"],
    #             ]),
    #         ]

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
                        )
                else:
                    sample = SNTSample(
                        dataset=dsname,
                        exclude_tag_pattern="CMS4_V08-*", # ignore new samples by sicheng for 2016
                        tag=tag_match,
                        # tag="CMS4_V09-04-13", # if not specified, get latest tag
                        )
                skip_tail = "/SMS" in dsname
                # skip_tail = False
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
                                ["JobBatchName","FT_{}_{}".format(year,shortname)],
                                ],
                            # "classads": [ ["metis_extraargs","--ignorebadfiles"], ],
                            },
                        cmssw_version = "CMSSW_9_4_9",
                        scram_arch = "slc6_amd64_gcc630",
                        input_executable = "inputs/condor_executable_metis.sh",
                        tarfile = "inputs/package.tar.xz",
                        special_dir = "FTbabies/{}/".format(tag),
                        # recopy_inputs = True,
                )
                merge_task = LocalMergeTask(
                        input_filenames=task.get_outputs(),
                        output_filename="{}/{}.root".format(merged_dir,shortname),
                        ignore_bad = skip_tail,
                        )
                nsamples += 1
                if not task.complete():
                    task.additional_input_files = ["/home/users/namin/2018/fourtop/all/FTAnalysis/babymaking/batch/condor_chirp"]
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

        time.sleep(600 if i < 10 else 1.5*60*60)

