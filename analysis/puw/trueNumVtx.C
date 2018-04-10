{

    TChain *ch = new TChain("t");
    ch->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v1.00_94x_baseline_withos_v3/output/DY_high.root");
    TH1F* hnumints = new TH1F("hnumints","hnumints",90,0,90);
    hnumints->Sumw2();
    ch->Draw("trueNumInt[0]>>hnumints");

    TFile *fout = new TFile("truenumint_mc2017.root", "RECREATE");
    hnumints->Write();

    fout->Write();
    fout->Close();

}
