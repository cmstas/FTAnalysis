{

    gSystem->Exec("mkdir -p outputs");

    gROOT->ProcessLine(".L ../misc/class_files/v8.02/SS.cc+"); 
    // gSystem->Load("../../common/CORE/CMS3_CORE.so");
    // gSystem->Load("../../common/CORE/CMS3_CORE.so");
    gROOT->ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc++");
    gROOT->ProcessLine(".L ScanChain.C+");

    TString tag = "v1.00_94x_baseline_v1";
    TString basedir = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s/output/",tag.Data());

    TChain *ch_data = new TChain("t","data");
    ch_data->Add(basedir+"Data*.root");
    ScanChain(ch_data);

    TChain *ch_ttw = new TChain("t","ttw");
    ch_ttw->Add(basedir+"TTWnlo.root");
    ScanChain(ch_ttw);

    TChain *ch_ttz = new TChain("t","ttz");
    ch_ttz->Add(basedir+"TTZnlo.root");
    ScanChain(ch_ttz);

    TChain *ch_dy = new TChain("t","dy");
    ch_dy->Add(basedir+"DY*.root");
    ScanChain(ch_dy);

    TChain *ch_wjets = new TChain("t","wjets");
    ch_wjets->Add(basedir+"WJets*.root");
    ScanChain(ch_wjets);

    TChain *ch_tt = new TChain("t","tt");
    ch_tt->Add(basedir+"TTBAR*.root");
    ScanChain(ch_tt);

}

