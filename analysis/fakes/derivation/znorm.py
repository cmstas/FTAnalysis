import sys
import ROOT as r
import os

r.gROOT.ProcessLine(".L ../../misc/common_utils.h")

import checks.TMultiDrawTreePlayer.api

# version ="v23"
version ="v25jec32"
d_lumis = {
        2017: 41.5,
        2018: 35.5,
        }

ele_trigs = [
        "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30",
        "HLT_Ele17_CaloIdM_TrackIdM_PFJet30",
        "HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30",
        "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30",
        "HLT_Ele23_CaloIdM_TrackIdM_PFJet30",
        "HLT_Ele8_CaloIdM_TrackIdM_PFJet30",
        "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30",
        ]

mu_trigs = [
        "HLT_Mu17_TrkIsoVVL",
        "HLT_Mu17",
        "HLT_Mu8_TrkIsoVVL",
        "HLT_Mu8",
        "HLT_IsoMu27",  # unprescaled
        ]


d_fpatts = {}
d_fpatts[2017] = {
        "dy": "/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DYJets-2017v1_LEPTON_{version}/output*.root".format(version=version),
        "mu": """
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017B-31Mar2018-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017C-31Mar2018-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017D-31Mar2018-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017E-31Mar2018-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017F-31Mar2018-v1_LEPTON_{version}//output*.root,
""".format(version=version),
        "e": """
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017B-31Mar2018-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017C-31Mar2018-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017D-31Mar2018-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017E-31Mar2018-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017F-31Mar2018-v1_LEPTON_{version}//output*.root,
""".format(version=version),
        }

d_fpatts[2018] = {
        "dy": "/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DYJets-2017v1_LEPTON_{version}/output*.root".format(version=version),
        "mu": """
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018A-PromptReco-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018A-PromptReco-v2_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018A-PromptReco-v3_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018B-PromptReco-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018B-PromptReco-v2_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018C-PromptReco-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018C-PromptReco-v2_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018C-PromptReco-v3_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018D-PromptReco-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2018D-PromptReco-v2_LEPTON_{version}//output*.root,
""".format(version=version),
        "e": """
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018A-PromptReco-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018A-PromptReco-v2_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018A-PromptReco-v3_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018B-PromptReco-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018B-PromptReco-v2_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018C-PromptReco-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018C-PromptReco-v2_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018C-PromptReco-v3_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018D-PromptReco-v1_LEPTON_{version}//output*.root,
/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_EGamma_Run2018D-PromptReco-v2_LEPTON_{version}//output*.root,
""".format(version=version),
        }


# fpatts = sys.argv[-1]


# for year in [2017, 2018]:
for year in [2017]:

    for proc,fpatts in d_fpatts[year].items():

        ch = r.TChain("t")
        for fpatt in fpatts.split(","):
            if not fpatt.strip(): continue
            print "Adding {}".format(fpatt.strip())
            ch.Add(fpatt.strip())

        trigs = []
        ismc = False
        if "SingleElectron" in fpatts:
            ismc = False
            trigs.extend(ele_trigs)
        elif "EGamma" in fpatts:
            ismc = False
            trigs.extend(ele_trigs)
        elif "DoubleMu" in fpatts:
            ismc = False
            trigs.extend(mu_trigs)
        else:
            ismc = True
            trigs.extend(ele_trigs)
            trigs.extend(mu_trigs)

        hists = []
        for trigname in trigs:
            idlep = 11
            if "_Ele" in trigname or "_IsoE" in trigname:
                idlep = 11
            elif "_Mu" in trigname or "_IsoMu" in trigname:
                idlep = 13

            hname = "mll_{}".format(trigname)
            mult = "1"
            if ismc:
                sf = "leptonScaleFactor({year}, {idlep}, p4.pt(), p4.eta(), 100.)*leptonScaleFactor({year}, {idlep}, tag_p4.pt(), tag_p4.eta(), 100.)".format(year=year,idlep=idlep)
                # sf = "leptonScaleFactor({year}, {idlep}, max(p4.pt(),20.), p4.eta(), 100.)*leptonScaleFactor({year}, {idlep}, max(tag_p4.pt(),20), tag_p4.eta(), 100.)".format(year=year,idlep=idlep)
                # sf = "1"
                mult = "{}*scale1fb*{}".format(d_lumis[year],sf)
            varexp = "dilep_mass>>{}(80,0,200)".format(hname)
            sel = "{}*{}*(abs(id)=={} && passes_SS_tight_v6 && {}>0 && tag_p4.pt()>30. && p4.pt()>25.)".format(mult,trigname,idlep,trigname)
            opt = "e"
            print varexp, sel, opt
            ch.Draw(varexp,sel,opt)

        # hists = ch.GetHists(N=15, file_cache="cachez.pkl")
        hists = ch.GetHists(N=15)
        for hist in hists.values():
            print hist.Integral()

        # f1 = r.TFile("hists_{}.root".format(proc),"RECREATE")
        outdir = "zpeaks/{}/".format(version)
        os.system("mkdir -p {}".format(outdir))
        f1 = r.TFile("{}/hists_{}_{}.root".format(outdir,proc,year),"RECREATE")
        [h.Write() for h in hists.values()]
        f1.Write()
        f1.Close()
