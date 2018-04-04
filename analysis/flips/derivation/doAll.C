{

    gSystem->Exec("mkdir -p outputs");

    gROOT->ProcessLine(".L ../misc/class_files/v8.02/SS.cc+"); 
    gROOT->ProcessLine(".L ScanChain.C+");

    TString tag = "v1.00_94x_baseline_v1";
    TString basedir = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s/output/",tag.Data());

    TChain *ch_dy = new TChain("t","dy");
    ch_dy->Add(basedir+"DY*.root");

    flip(ch_dy);

}

