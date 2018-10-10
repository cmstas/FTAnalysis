import os
import sys
import ROOT as r
from multiprocessing import Pool as ThreadPool
import glob
import random
import time

header = """
  gROOT->ProcessLine(".L ../../misc/common_utils.h");
  // gSystem->Load("../../../common/CORE/CMS3_CORE.so");

  // gROOT->ProcessLine(".L ScanChain.C+");
  gROOT->ProcessLine(".L ScanChain_fast.C+");

  TChain *ch = new TChain("t");
"""

d_chain_strs = {
        "data_el": """
  // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017B-31Mar2018-v1_LEPTON_{tag_data}/*.root");
  // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017C-31Mar2018-v1_LEPTON_{tag_data}/*.root");
  // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017D-31Mar2018-v1_LEPTON_{tag_data}/*.root");
  // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017E-31Mar2018-v1_LEPTON_{tag_data}/*.root");
  // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017F-31Mar2018-v1_LEPTON_{tag_data}/*.root");

    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018A-PromptReco-v1_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018A-PromptReco-v2_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018A-PromptReco-v3_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018B-PromptReco-v1_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018B-PromptReco-v2_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018C-PromptReco-v1_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018C-PromptReco-v2_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018C-PromptReco-v3_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018D-PromptReco-v1_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018D-PromptReco-v2_LEPTON_{tag_data}/*.root");
  """,

      "data_mu": """
  // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017B-31Mar2018-v1_LEPTON_{tag_data}/*.root");
  // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017C-31Mar2018-v1_LEPTON_{tag_data}/*.root");
  // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017D-31Mar2018-v1_LEPTON_{tag_data}/*.root");
  // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017E-31Mar2018-v1_LEPTON_{tag_data}/*.root");
  // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017F-31Mar2018-v1_LEPTON_{tag_data}/*.root");

    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018A-PromptReco-v1_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018A-PromptReco-v2_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018A-PromptReco-v3_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018B-PromptReco-v1_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018B-PromptReco-v2_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018C-PromptReco-v1_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018C-PromptReco-v2_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018C-PromptReco-v3_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018D-PromptReco-v1_LEPTON_{tag_data}/*.root");
    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018D-PromptReco-v2_LEPTON_{tag_data}/*.root");
  """,

  "wjets": """
  ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_WJets-2017v1_LEPTON_{tag_mc}/*.root");
  """,

  "dy": """
  ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DYJets-2017v1_LEPTON_{tag_mc}/*.root");
  """,

  "qcd_el": """
  ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_QCD_EMEnriched-2017v1_LEPTON_{tag_mc}/*.root");
  ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_QCD_bcToE-2017v1_LEPTON_{tag_mc}/*.root");
  """,

  "qcd_mu": """
  ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_QCD_MuEnriched-2017v1_LEPTON_{tag_mc}/*.root");
  """,

  "ttjets": """
  ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_TTJets-2017v1_LEPTON_{tag_mc}/*.root");
  """,

}


# tag_data = "v8"
# tag_mc = "v8"
# tag_data = "v9"
# tag_mc = "v9"
# tag_data = "v13"
# tag_mc = "v13"
# tag_data = "v15"
# tag_mc = "v15"
tag_data = "v23"
tag_mc = "v23"

for proc in d_chain_strs.keys():
    for iso in [True,False]:

        if iso:
            footer = """
          ScanChain(ch,"./outputs/rate_histos_{proc}_LooseEMVA_IsoTrigs.root","doAbove18,useLooseEMVA,IsoTrigs");
            """.format(proc=proc)
        else:
            footer = """
          ScanChain(ch,"./outputs/rate_histos_{proc}_LooseEMVA.root","doAbove18,useLooseEMVA");
            """.format(proc=proc)

        full = "{\n" + header + d_chain_strs[proc].format(tag_data=tag_data,tag_mc=tag_mc) + footer + "\n}"
        iso_str = "_iso" if iso else ""
        with open(".parallel_doAll_{}{}.C".format(proc,iso_str), "w") as fhout:
            fhout.write(full)
