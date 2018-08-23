{

    gSystem->Exec("mkdir -p outputs");

    gROOT->ProcessLine(".L ../../misc/class_files/v8.02/SS.cc+"); 
    gROOT->ProcessLine(".L ScanChain.C+");

    // TString tag = "v3.01";
    // TString tag = "v3.01_nmiss0";
    TString tag = "v3.02_nmiss0";
    TString basedir = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s/output/",tag.Data());

    TChain *ch_both = new TChain("t","both");
    ch_both->Add(basedir+"DY*.root");
    ch_both->Add(basedir+"DY_highext1.root");
    ch_both->Add(basedir+"TTBAR*.root");
    flip(ch_both);

    // TChain *ch_tt = new TChain("t","tt");
    // ch_tt->Add(basedir+"TTBAR*.root");
    // flip(ch_tt);

    // TChain *ch_dy = new TChain("t","dy");
    // ch_dy->Add(basedir+"DY*.root");
    // flip(ch_dy);


}

