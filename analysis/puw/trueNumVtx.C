{

    TChain *ch = new TChain("t");
    ch->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.02_nmiss0/output/DY_high.root"); // 2017 MC
    TH1F* hnumints = new TH1F("hnumints","hnumints",100,0,100);
    hnumints->Sumw2();
    ch->Draw("trueNumInt[0]>>hnumints");

    TFile *fout = new TFile("truenumint_mc2017.root", "RECREATE");
    hnumints->Write();

    fout->Write();
    fout->Close();

}
