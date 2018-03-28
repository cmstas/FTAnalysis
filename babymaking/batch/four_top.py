import os
import sys

d_ds2name = {
        "backgrounds": {

            "/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "TTZnlo",
            "/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/MINIAODSIM": "TTWnlo",
            "/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTHtoNonBB",
            "/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZ",
            "/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "ZG",
            "/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTG",
            "/WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "QQWW",
            "/WWTo2L2Nu_DoubleScattering_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WWDPS",
            "/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTBAR_PH",
            "/ZZTo4L_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "ZZ",
            "/TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "TGext",
            "/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZZ",
            "/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WWZ",
            "/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext3-v1/MINIAODSIM": "WGToLNuGext",
            "/tZq_ll_4f_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "TZQ",
            "/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "VHtoNonBB",
            "/TTZToLL_M-1to10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTZLOW",
            "/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUgenV6_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "GGHtoZZto4L",
            "/ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "ZZZ",
            "/WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WWW",
            "/WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM": "WWG",
            "/WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WZG",
            "/ST_tWll_5f_LO_13TeV-MadGraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TWZ",
            "/TTWW_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTWW",
            "/TTTW_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTTW",
            "/TTWZ_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTWZ",
            "/TTTJ_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTTJ",
            "/TTWH_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTWH",
            "/TTHH_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTHH",
            "/TTZH_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTZH",
            "/TTZZ_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTZZ",
            "/TTGamma_SingleLeptFromT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTGsinglelep",
            "/TTGamma_SingleLeptFromTbar_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTGsinglelepbar",
            "/TTGamma_Dilept_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM": "TTGdilep",
            "/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTTTnew",
            "/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-isrdown-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTTTisrdown",
            "/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-fsrdown-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTTTfsrdown",
            "/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-fsrup-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTTTfsrup",
            "/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-isrup-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTTTisrup",

            # "/TT_MSDecays_dileptonic_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTpythia",

            # "/tHW_HToTT_0J_mH-350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THW_scan350",
            # "/tHW_HToTT_0J_mH-370_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THW_scan370",
            # "/tHW_HToTT_0J_mH-390_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THW_scan390",
            # "/tHW_HToTT_0J_mH-410_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THW_scan410",
            # "/tHW_HToTT_0J_mH-430_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THW_scan430",
            # "/tHW_HToTT_0J_mH-450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THW_scan450",
            # "/tHW_HToTT_0J_mH-470_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THW_scan470",
            # "/tHW_HToTT_0J_mH-490_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THW_scan490",
            # "/tHW_HToTT_0J_mH-510_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THW_scan510",
            # "/tHW_HToTT_0J_mH-530_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THW_scan530",
            # "/tHW_HToTT_0J_mH-550_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THW_scan550",
            # "/tHq_HToTT_0J_mH-350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THQ_scan350",
            # "/tHq_HToTT_0J_mH-370_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THQ_scan370",
            # "/tHq_HToTT_0J_mH-390_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THQ_scan390",
            # "/tHq_HToTT_0J_mH-410_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THQ_scan410",
            # "/tHq_HToTT_0J_mH-430_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THQ_scan430",
            # "/tHq_HToTT_0J_mH-450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THQ_scan450",
            # "/tHq_HToTT_0J_mH-470_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THQ_scan470",
            # "/tHq_HToTT_0J_mH-490_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THQ_scan490",
            # "/tHq_HToTT_0J_mH-510_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THQ_scan510",
            # "/tHq_HToTT_0J_mH-530_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THQ_scan530",
            # "/tHq_HToTT_0J_mH-550_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "THQ_scan550",
            # "/ttH_HToTT_1J_mH-350_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "TTH_scan350",
            # "/ttH_HToTT_1J_mH-370_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "TTH_scan370",
            # "/ttH_HToTT_1J_mH-390_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "TTH_scan390",
            # "/ttH_HToTT_1J_mH-410_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "TTH_scan410",
            # "/ttH_HToTT_1J_mH-430_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "TTH_scan430",
            # "/ttH_HToTT_1J_mH-450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "TTH_scan450",
            # "/ttH_HToTT_1J_mH-470_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "TTH_scan470",
            # "/ttH_HToTT_1J_mH-490_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "TTH_scan490",
            # "/ttH_HToTT_1J_mH-510_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "TTH_scan510",
            # "/ttH_HToTT_1J_mH-530_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "TTH_scan530",
            # "/ttH_HToTT_1J_mH-550_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM":    "TTH_scan550",

            # "/ttH_HToTT_1J_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "TTH_scan",
            # "/tHW_HToTT_0J_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "THW_scan",
            # "/tHq_HToTT_0J_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "THQ_scan",

            # "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "WJets",
            # "/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM": "DY_low",
            # "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/MINIAODSIM": "DY_high",

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

# tag = "v0.04" # deepCSV, era SFs
# tag = "v0.04_norundep" # deepCSV, integrated bsfs
# tag = "v0.05" # deepCSV, integrated bsfs, triple RA7 class8
# tag = "v0.04_ttzwhscales" # deepCSV, integrated bsfs, triple RA7 class8
# tag = "v0.06" # deepCSV, era SFs, fourth lepton - bugged?
# tag = "v0.07" # deepCSV, era SFs, fourth lepton
# tag = "v0.08" # deepCSV, era SFs, fourth lepton - 30 GeV jets, 25GeV btag
# tag = "v0.09" # deepCSV, era SFs, fourth lepton - 30 GeV jets, 25GeV btag, JER, MC ONLY
# tag = "v0.10" # deepCSV, era SFs, fourth lepton - 40 GeV jets, 25GeV btag, JER, MC ONLY
# tag = "v0.10_osft" # same as v0.10 but tttt only and OS is in babies too
# tag = "v0.10_fix" # same as v0.10 but JER/JEC unc fixed?
# tag = "v0.10_raresxg" # same as v0.10 but tttt only and OS is in babies too
# tag = "v0.10_data" # same as previous but data only
# tag = "v0.10_mcosv2" # same as previous but MC only (and with OS)
# tag = "v0.10_WSF" # same as v0.10_fix, no OS, but also has pdgW{leptonic,hadronic} SFs -- bugged, didn't have MC met filters
# tag = "v0.10_WSFv2" # same as non v2, but with fixed met filters
# tag = "v0.10_mll" # same as previous line, but with mll<12 veto for ee or OSSF
# tag = "v1.00" # new baseline
tag = "v1.0.6" # bJetCut40, jetCut20, 3rd lep cut 10
tag = "v1.0.6_fixed" # fixes reversed jet/bjet cuts from v1.0.6

package = inputs_path+"package.tar.gz"
executable = inputs_path+"condor_executable.sh"
dashboard_name = "AutoTwopler_FTbabies" + tag

if not os.path.isfile(package):
    raise Exception("Woah! Did you run make_tar.sh? I don't see the package.tar.gz file in %s" % inputs_path)
elif ((os.path.getmtime(package) < os.path.getmtime("main.exe")) or (os.path.getmtime(package) < os.path.getmtime("helper_babymaker.cc"))):
    raise Exception("Woah! Did you run make_job_inputs.sh? It looks like main.exe or helper_babymaker.cc might be newer than the tarball in %s" % inputs_path)
