{
    gSystem->Load("CORE/CMS3_CORE.so");
    gROOT->ProcessLine(".L CORE/CMS3.cc+");
    gROOT->ProcessLine(".L ScanChain.C+");

    TChain *ch = new TChain("Events");
    // ch->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/merged_ntuple_*.root");
    ch->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/merged_ntuple_1.root");

    ScanChain(ch);

}

