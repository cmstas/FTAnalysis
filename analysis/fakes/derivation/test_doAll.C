{

    gROOT->ProcessLine(".L ../../misc/common_utils.h");

    gROOT->ProcessLine(".L ScanChain_fast.C+");
    // gROOT->ProcessLine(".L ScanChain.C+");

    TChain *ch = new TChain("t");

    ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_QCD_MuEnriched-2017v1_LEPTON_v15/output_*.root");
    // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_TTJets-2017v1_LEPTON_v15/output*.root");

    TString suffix = "";
    // TString options = "useLooseEMVA,IsoTrigs";
    // TString options = "useLooseEMVA,IsoTrigs,overrideMCMatch,doHighMET";
    TString options = "useLooseEMVA,IsoTrigs";
    ScanChain(ch,"rate_test.root", options);

}
