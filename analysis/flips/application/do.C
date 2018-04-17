{

    gSystem->Exec("mkdir -p outputs");

    gROOT->ProcessLine(".L ../../misc/class_files/v8.02/SS.cc+"); 
    gROOT->ProcessLine(".L ../../../common/CORE/Tools/dorky/dorky.cc++");
    gROOT->ProcessLine(".L ScanChain.C+");

    // TString tag = "v1.00_94x_baseline_withos_v3";
    TString tag = "v1.02_94x_withos_relaxhitsmore";
    TString basedir = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s/output/",tag.Data());

    TChain *ch = new TChain("t");
    ch->Add(basedir+"DY_high.root");
    ch->Add(basedir+"DY_low.root");
    ch->Add(basedir+"TTBAR*.root");
    // ch->Add(basedir+"DataDoubleEG*.root");
    ch->Add(basedir+"rereco_DataDoubleEG*.root"); // NOTE RERECO!!! XXX

    TString flipfname = "../derivation/outputs/histos_both.root";

    // TChain *ch2016 = new TChain("t");
    // ch2016->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/DY_high*.root");
    // ch2016->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/DY_low*.root");
    // ch2016->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/TTBAR*.root");
    // ch2016->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/DataDoubleEG*.root");
    // closure(ch2016, flipfname, "outputs/histos_2016.root", -1, -1, 35.9); // Run2016

    closure(ch, flipfname, "outputs/histos_B.root", 297046, 299329, 4.767); // Run2017B
    closure(ch, flipfname, "outputs/histos_C.root", 299368, 302029, 9.583); // Run2017C
    closure(ch, flipfname, "outputs/histos_D.root", 302030, 303434, 4.224); // Run2017D
    closure(ch, flipfname, "outputs/histos_E.root", 303824, 304797, 9.261); // Run2017E
    closure(ch, flipfname, "outputs/histos_F.root", 305040, 306462, 13.463); // Run2017F
    closure(ch, flipfname, "outputs/histos.root");


}
