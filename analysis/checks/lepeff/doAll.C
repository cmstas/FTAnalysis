{

    gROOT->ProcessLine(".L CORE/CMS3_CORE.so");
    gROOT->ProcessLine(".L ScanChain.C+");

    // TChain *ch2016 = new TChain("Events");
    // ch2016->Add("/hadoop/cms/store/group/snt/run2_mc2016_94x//TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1_MINIAODSIM_CMS4_V10-02-05/merged_ntuple_1.root");
    // ch2016->Add("/hadoop/cms/store/group/snt/run2_mc2016_94x//TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1_MINIAODSIM_CMS4_V10-02-05/merged_ntuple_2.root");
    // ch2016->Add("/hadoop/cms/store/group/snt/run2_mc2016_94x//TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1_MINIAODSIM_CMS4_V10-02-05/merged_ntuple_3.root");
    // ch2016->Add("/hadoop/cms/store/group/snt/run2_mc2016_94x//TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1_MINIAODSIM_CMS4_V10-02-05/merged_ntuple_4.root");
    // ch2016->Add("/hadoop/cms/store/group/snt/run2_mc2016_94x//TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1_MINIAODSIM_CMS4_V10-02-05/merged_ntuple_5.root");
    // ScanChain(ch2016, "outputs/output_2016_tt.root", "2016");

    // Using inclusive amcatnlo one has issues matching muons (we get 1/3rd the efficiency!)
    TChain *ch2017 = new TChain("Events");
    ch2017->Add("/hadoop/cms/store/group/snt/run2_mc2017//TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V10-02-05//merged_ntuple_1.root");
    ch2017->Add("/hadoop/cms/store/group/snt/run2_mc2017//TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V10-02-05//merged_ntuple_2.root");
    ch2017->Add("/hadoop/cms/store/group/snt/run2_mc2017//TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V10-02-05///merged_ntuple_1.root");
    ch2017->Add("/hadoop/cms/store/group/snt/run2_mc2017//TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V10-02-05///merged_ntuple_2.root");
    ScanChain(ch2017, "outputs/output_2017_tt.root", "2017");


    // Using inclusive amcatnlo one has issues matching muons (we get 1/3rd the efficiency!)
    TChain *ch2018 = new TChain("Events");
    ch2018->Add("/hadoop/cms/store/group/snt/run2_mc2018//TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1_MINIAODSIM_CMS4_V10-02-04/merged_ntuple_1.root");
    ch2018->Add("/hadoop/cms/store/group/snt/run2_mc2018//TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1_MINIAODSIM_CMS4_V10-02-04/merged_ntuple_2.root");
    ch2018->Add("/hadoop/cms/store/group/snt/run2_mc2018//TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1_MINIAODSIM_CMS4_V10-02-04//merged_ntuple_1.root");
    ch2018->Add("/hadoop/cms/store/group/snt/run2_mc2018//TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1_MINIAODSIM_CMS4_V10-02-04//merged_ntuple_2.root");
    ScanChain(ch2018, "outputs/output_2018_tt.root", "2018");

}

