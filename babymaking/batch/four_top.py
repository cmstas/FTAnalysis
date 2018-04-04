import os
import sys

d_ds2name = {
        "backgrounds": {

            "/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM": "TTZnlo",
            "/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM": "TTWnlo",
            "/TTTT_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM": "TTTTnew",
            "/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM": "TTBAR_PH",
            "/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1/MINIAODSIM": "TTHtoNonBB",

            "/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v2/MINIAODSIM": "DY_low",
            "/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAOD-RECOSIMstep_94X_mc2017_realistic_v10-v1/MINIAODSIM": "DY_high",

            # note: 92X
            "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v1/MINIAODSIM": "WJets",

            # "/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZ",
            # "/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "ZG",
            # "/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTG",
            # "/WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "QQWW",
            # "/WWTo2L2Nu_DoubleScattering_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WWDPS",
            # "/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTBAR_PH",
            # "/ZZTo4L_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "ZZ",
            # "/TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "TGext",
            # "/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZZ",
            # "/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WWZ",
            # "/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext3-v1/MINIAODSIM": "WGToLNuGext",
            # "/tZq_ll_4f_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "TZQ",
            # "/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "VHtoNonBB",
            # "/TTZToLL_M-1to10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTZLOW",
            # "/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUgenV6_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "GGHtoZZto4L",
            # "/ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "ZZZ",
            # "/WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WWW",
            # "/WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "WWG",
            # "/WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZG",
            # "/ST_tWll_5f_LO_13TeV-MadGraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TWZ",
            # "/TTWW_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTWW",
            # "/TTTW_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTTW",
            # "/TTWZ_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTWZ",
            # "/TTTJ_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTTJ",
            # "/TTWH_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTWH",
            # "/TTHH_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTHH",
            # "/TTZH_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTZH",
            # "/TTZZ_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTZZ",
            # "/TTGamma_SingleLeptFromT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTGsinglelep",
            # "/TTGamma_SingleLeptFromTbar_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTGsinglelepbar",
            # "/TTGamma_Dilept_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTGdilep",
            # "/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-isrdown-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTTTisrdown",
            # "/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-fsrdown-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTTTfsrdown",
            # "/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-fsrup-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTTTfsrup",
            # "/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-isrup-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTTTisrup",

            # "/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "ST1",
            # "/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "ST2",
            # "/TT_MSDecays_dileptonic_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTpythia",

            },
        "data": {

                "/DoubleEG/Run2017B-PromptReco-v1/MINIAOD": "DataDoubleEGB",
                "/DoubleEG/Run2017B-PromptReco-v1/MINIAOD": "DataDoubleEGB",
                "/DoubleEG/Run2017B-PromptReco-v2/MINIAOD": "DataDoubleEGB",
                "/DoubleEG/Run2017C-PromptReco-v1/MINIAOD": "DataDoubleEGC",
                "/DoubleEG/Run2017C-PromptReco-v2/MINIAOD": "DataDoubleEGC",
                "/DoubleEG/Run2017C-PromptReco-v3/MINIAOD": "DataDoubleEGC",
                "/DoubleEG/Run2017D-PromptReco-v1/MINIAOD": "DataDoubleEGD",
                "/DoubleEG/Run2017E-PromptReco-v1/MINIAOD": "DataDoubleEGE",
                "/DoubleEG/Run2017F-PromptReco-v1/MINIAOD": "DataDoubleEGF",

                "/MuonEG/Run2017B-PromptReco-v1/MINIAOD": "DataMuonEGB",
                "/MuonEG/Run2017B-PromptReco-v1/MINIAOD": "DataMuonEGB",
                "/MuonEG/Run2017B-PromptReco-v2/MINIAOD": "DataMuonEGB",
                "/MuonEG/Run2017C-PromptReco-v1/MINIAOD": "DataMuonEGC",
                "/MuonEG/Run2017C-PromptReco-v2/MINIAOD": "DataMuonEGC",
                "/MuonEG/Run2017C-PromptReco-v3/MINIAOD": "DataMuonEGC",
                "/MuonEG/Run2017D-PromptReco-v1/MINIAOD": "DataMuonEGD",
                "/MuonEG/Run2017E-PromptReco-v1/MINIAOD": "DataMuonEGE",
                "/MuonEG/Run2017F-PromptReco-v1/MINIAOD": "DataMuonEGF",

                "/DoubleMuon/Run2017B-PromptReco-v1/MINIAOD": "DataDoubleMuonB",
                "/DoubleMuon/Run2017B-PromptReco-v1/MINIAOD": "DataDoubleMuonB",
                "/DoubleMuon/Run2017B-PromptReco-v2/MINIAOD": "DataDoubleMuonB",
                "/DoubleMuon/Run2017C-PromptReco-v1/MINIAOD": "DataDoubleMuonC",
                "/DoubleMuon/Run2017C-PromptReco-v2/MINIAOD": "DataDoubleMuonC",
                "/DoubleMuon/Run2017C-PromptReco-v3/MINIAOD": "DataDoubleMuonC",
                "/DoubleMuon/Run2017D-PromptReco-v1/MINIAOD": "DataDoubleMuonD",
                "/DoubleMuon/Run2017E-PromptReco-v1/MINIAOD": "DataDoubleMuonE",
                "/DoubleMuon/Run2017F-PromptReco-v1/MINIAOD": "DataDoubleMuonF",

            },

        }

def dataset_to_shortname(ds):
    for typ_values in d_ds2name.values():
        for dname in typ_values:
            if dname.split("|")[0] == ds:
                return typ_values[dname]
    return "unknown"

inputs_path = os.path.abspath(__file__).rsplit("/",1)[0]+"/inputs/"
sweepRoot_scripts = [inputs_path+"sweeproot.sh", inputs_path+"sweeproot_macro.C"]
merging_scripts = [inputs_path+"merge_script.sh", inputs_path+"merge_macro.C"]
baby_merged_dir = "/nfs-7/userdata/${USER}/tupler_babies/merged/"

tag = "v1.00_94x_baseline_v1"
package = inputs_path+"package.tar.gz"
executable = inputs_path+"condor_executable.sh"
dashboard_name = "AutoTwopler_FTbabies"

if not os.path.isfile(package):
    raise Exception("Woah! Did you run make_tar.sh? I don't see the package.tar.gz file in %s" % inputs_path)
elif ((os.path.getmtime(package) < os.path.getmtime("main.exe")) or (os.path.getmtime(package) < os.path.getmtime("helper_babymaker.cc"))):
    raise Exception("Woah! Did you run make_job_inputs.sh? It looks like main.exe or helper_babymaker.cc might be newer than the tarball in %s" % inputs_path)



