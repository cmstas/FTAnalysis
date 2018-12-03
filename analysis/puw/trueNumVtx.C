{

    // TChain *ch = new TChain("t");
    // ch->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.02_nmiss0/output/DY_high.root"); // 2017 MC
    // TH1F* hnumints = new TH1F("hnumints","hnumints",100,0,100);
    // hnumints->Sumw2();
    // ch->Draw("trueNumInt[0]>>hnumints");

    TChain *ch = new TChain("Events");
    ch->Add("/hadoop/cms/store/group/snt/run2_mc2018//DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall18MiniAOD-102X_upgrade2018_realistic_v12_ext1-v1_MINIAODSIM_CMS4_V10-02-00/merged_ntuple_1*.root");
    TH1F* hnumints = new TH1F("hnumints","hnumints",100,0,100);
    hnumints->Sumw2();
    ch->Draw("puInfo_trueNumInteractions[0]>>hnumints");

    TFile *fout = new TFile("truenumint_mc2017.root", "RECREATE");
    hnumints->Write();

    fout->Write();
    fout->Close();

}
