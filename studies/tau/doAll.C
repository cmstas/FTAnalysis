{

    gSystem->Load("CORE/CMS3_CORE.so");

    gROOT->ProcessLine(".L Yield.C+");


    auto getEnv = [](const char* name, int default_){
        const char* env = gSystem->Getenv(name);
        if (env != nullptr) return atoi(env);
        else return default_;
    };

    int tau_selection = getEnv("tau_selection", 0);
    int datasetIdx = getEnv("datasetIdx", 0);

    cout << "Finding yields with tau_selection=" << tau_selection << " and datasetIdx=" << datasetIdx << endl;

    auto fname = [tau_selection](const string& category) {
        stringstream res;
        switch(tau_selection) {
            case 0:
                res << "output/yield_" << category << "_notau.root";
                break;
            case 1:
                res << "output/yield_" << category << ".root";
                break;
            case 2:
                res << "output/yield_" << category << "_matchtau.root";
                break;
        }
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
            ScanChain(ch_ft, &out_ft, tau_selection);
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
            ScanChain(ch_ttw, &out_ttw, tau_selection);
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
            ScanChain(ch_ttz, &out_ttz, tau_selection);
            out_ttz.Close();
            break;
        } case 3: {
            TChain *ch_tth = new TChain("Events");
            TFile out_tth(fname("tth"), "RECREATE");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_1.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_3.root");
            ch_tth->Add("/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-01/merged_ntuple_4.root");
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
            ScanChain(ch_tth, &out_tth, tau_selection);
            out_tth.Close();
            break;
        }
    }
}
