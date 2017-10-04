{
    gSystem->Exec("mkdir -p plots");

    gROOT->ProcessLine(".L Software/dataMCplotMaker/dataMCplotMaker.cc+");
    gROOT->ProcessLine(".L CMS3.cc+");
    gROOT->ProcessLine(".L ScanChain.C+");

    TChain *ch = new TChain("Events");
    ch->Add("/hadoop/cms/store/group/snt/run2_moriond17/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V08-00-16/merged_ntuple_1.root");

    ScanChain(ch);

}

