{

    gSystem->Load("CORE/CMS3_CORE.so");

    int error;
    gROOT->ProcessLine(".L Yield.C+", &error);
    if (error != 0) exit(error);


    auto getEnv = [](const char* name, int default_){
        const char* env = gSystem->Getenv(name);
        if (env != nullptr) return atoi(env);
        else return default_;
    };

    int datasetIdx = getEnv("datasetIdx", 0);
    int events_max = getEnv("events_max", 0);
    bool logging_enabled = false;

    if (datasetIdx < 0) exit(0);

    cout << "Finding yields with datasetIdx=" << datasetIdx << endl;

    auto fname = [](const string& category) {
        stringstream res;
        res << "output/yield_" << category << ".root";
        return TString(res.str());
    };
    switch(datasetIdx){
        case 0: {
            TChain *ch_ft = new TChain("Events");
            TFile out_ft(fname("ft"), "RECREATE");
            ch_ft->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/merged_ntuple_1.root");
            ch_ft->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/merged_ntuple_10.root");
            ch_ft->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/merged_ntuple_2.root");
            ch_ft->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/merged_ntuple_3.root");
            ch_ft->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/merged_ntuple_4.root");
            ch_ft->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/merged_ntuple_5.root");
            ch_ft->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/merged_ntuple_6.root");
            ch_ft->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/merged_ntuple_7.root");
            ch_ft->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/merged_ntuple_8.root");
            ScanChain(ch_ft, &out_ft, events_max, (logging_enabled) ? "ft" : "");
            out_ft.Close();
            break;
        } case 1: {
            TChain *ch_ttw = new TChain("Events");
            TFile out_ttw(fname("ttw"), "RECREATE");
            ch_ttw->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/V00-00-01/merged_ntuple_1.root");
            ch_ttw->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/V00-00-01/merged_ntuple_2.root");
            ch_ttw->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/V00-00-01/merged_ntuple_3.root");
            ch_ttw->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/V00-00-01/merged_ntuple_4.root");
            ch_ttw->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/V00-00-01/merged_ntuple_5.root");
            ch_ttw->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/V00-00-01/merged_ntuple_6.root");
            ch_ttw->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/V00-00-01/merged_ntuple_7.root");
            ScanChain(ch_ttw, &out_ttw, events_max, (logging_enabled) ? "ttw" : "");
            out_ttw.Close();
            break;
        } case 2: {
            TChain *ch_ttz = new TChain("Events");
            TFile out_ttz(fname("ttz"), "RECREATE");
            ch_ttz->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/V00-00-01/merged_ntuple_1.root");
            ch_ttz->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/V00-00-01/merged_ntuple_2.root");
            ch_ttz->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/V00-00-01/merged_ntuple_3.root");
            ch_ttz->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/V00-00-01/merged_ntuple_4.root");
            ch_ttz->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/V00-00-01/merged_ntuple_5.root");
            ch_ttz->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/V00-00-01/merged_ntuple_6.root");
            ch_ttz->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/V00-00-01/merged_ntuple_7.root");
            ch_ttz->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/V00-00-01/merged_ntuple_8.root");
            ScanChain(ch_ttz, &out_ttz, events_max, (logging_enabled) ? "ttz" : "");
            out_ttz.Close();
            break;
        } case 3: {
            TChain *ch_tth = new TChain("Events");
            TFile out_tth(fname("tth"), "RECREATE");
            /* CORRUPT ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_1.root"); */
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_3.root");
            /* CORRUPT ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_4.root"); */
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_5.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_6.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_7.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_8.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_10.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_11.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_12.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_13.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_14.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_15.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_16.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_17.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_18.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_19.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_20.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_21.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_22.root");
            ScanChain(ch_tth, &out_tth, events_max, (logging_enabled) ? "tth" : "");
            out_tth.Close();
            break;
        }
    }
}
