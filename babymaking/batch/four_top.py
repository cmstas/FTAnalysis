import os
import sys

d_ds2name = {
        "backgrounds": {

            "/SM-TTWW/SM-TTWW_Private80X-TEST-v1/USER": "TTWW",
            "/TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTTT",
            "/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "TTZnlo",
            "/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/MINIAODSIM": "TTWnlo",

            # "/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZ",
            # "/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "DY_low",
            # "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/MINIAODSIM": "DY_high",
            # "/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "ZG",
            # "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WJets",
            # "/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTG",
            # "/WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "QQWW",
            # "/WWTo2L2Nu_DoubleScattering_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WWDPS",
            # "/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTBAR_PH",
            # "/ZZTo4L_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "ZZ",
            # "/TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TG",
            # "/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZZ",
            # "/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WWZ",
            # "/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WGToLNuG",
            # "/tZq_ll_4f_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "TZQ",
            # "/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTHtoNonBB",
            # "/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "VHtoNonBB",
            # "/TTZToLL_M-1to10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTZLOW",
            # "/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUgenV6_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "GGHtoZZto4L",
            # "/ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "ZZZ",
            # "/WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WWW",
            # "/WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "WWG",
            # "/WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZG",
            # "/ST_tWll_5f_LO_13TeV-MadGraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TWZ",

            },
        "data": {

            "/DoubleEG/Run2016H-03Feb2017_ver2-v1/CMS3": "DataDoubleEGH",
            "/DoubleMuon/Run2016H-03Feb2017_ver2-v1/CMS3": "DataDoubleMuonH",
            "/MuonEG/Run2016H-03Feb2017_ver2-v1/CMS3": "DataMuonEGH",
            "/DoubleEG/Run2016H-03Feb2017_ver3-v1/CMS3": "DataDoubleEGHv3",
            "/DoubleMuon/Run2016H-03Feb2017_ver3-v1/CMS3": "DataDoubleMuonHv3",
            "/MuonEG/Run2016H-03Feb2017_ver3-v1/CMS3": "DataMuonEGHv3",
            "/JetHT/Run2016H-03Feb2017_ver2-v1/CMS3": "DataJetHTH",
            "/JetHT/Run2016H-03Feb2017_ver3-v1/CMS3": "DataJetHTHv3",
            "/DoubleEG/Run2016B-03Feb2017_ver2-v2/CMS3": "DataDoubleEGRerecoB",
            "/DoubleMuon/Run2016B-03Feb2017_ver2-v2/CMS3": "DataDoubleMuonRerecoB",
            "/MuonEG/Run2016B-03Feb2017_ver2-v2/CMS3": "DataMuonEGRerecoB",
            "/DoubleEG/Run2016C-03Feb2017-v1/CMS3": "DataDoubleEGRerecoC",
            "/DoubleMuon/Run2016C-03Feb2017-v1/CMS3": "DataDoubleMuonRerecoC",
            "/MuonEG/Run2016C-03Feb2017-v1/CMS3": "DataMuonEGRerecoC",
            "/DoubleEG/Run2016D-03Feb2017-v1/CMS3": "DataDoubleEGRerecoD",
            "/DoubleMuon/Run2016D-03Feb2017-v1/CMS3": "DataDoubleMuonRerecoD",
            "/MuonEG/Run2016D-03Feb2017-v1/CMS3": "DataMuonEGRerecoD",
            "/DoubleEG/Run2016E-03Feb2017-v1/CMS3": "DataDoubleEGRerecoE",
            "/DoubleMuon/Run2016E-03Feb2017-v1/CMS3": "DataDoubleMuonRerecoE",
            "/MuonEG/Run2016E-03Feb2017-v1/CMS3": "DataMuonEGRerecoE",
            "/DoubleEG/Run2016F-03Feb2017-v1/CMS3": "DataDoubleEGRerecoF",
            "/DoubleMuon/Run2016F-03Feb2017-v1/CMS3": "DataDoubleMuonRerecoF",
            "/MuonEG/Run2016F-03Feb2017-v1/CMS3": "DataMuonEGRerecoF",
            "/DoubleEG/Run2016G-03Feb2017-v1/CMS3": "DataDoubleEGRerecoG",
            "/DoubleMuon/Run2016G-03Feb2017-v1/CMS3": "DataDoubleMuonRerecoG",
            "/MuonEG/Run2016G-03Feb2017-v1/CMS3": "DataMuonEGRerecoG",

            },

        }

def dataset_to_shortname(ds):
    for typ_values in d_ds2name.values():
        for dname in typ_values:
            if dname == ds:
                return typ_values[dname]
    return "unknown"

inputs_path = os.path.abspath(__file__).rsplit("/",1)[0]+"/inputs/"
sweepRoot_scripts = [inputs_path+"sweeproot.sh", inputs_path+"sweeproot_macro.C"]
merging_scripts = [inputs_path+"merge_script.sh", inputs_path+"merge_macro.C"]
baby_merged_dir = "/nfs-7/userdata/${USER}/tupler_babies/merged/"

tag = "v0.01"
package = inputs_path+"package.tar.gz"
executable = inputs_path+"condor_executable.sh"
dashboard_name = "AutoTwopler_FTbabies"

if not os.path.isfile(package):
    raise Exception("Woah! Did you run make_tar.sh? I don't see the package.tar.gz file in %s" % inputs_path)


