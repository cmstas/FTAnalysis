{

    gSystem->Exec("mkdir -p outputs");

    gROOT->ProcessLine(".L ../../misc/class_files/v8.02/SS.cc+"); 
    // gROOT->ProcessLine(".L ../../../common/CORE/Tools/dorky/dorky.cc++");
    gROOT->ProcessLine(".L ScanChain.C+");

    TString tag_2017 = "v3.02_nmiss0";
    TString basedir_2017 = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s/output/",tag_2017.Data());

    // TChain *ch = new TChain("t");
    // ch->Add(basedir_2017+"DY_high.root");
    // ch->Add(basedir_2017+"DY_low.root");
    // ch->Add(basedir_2017+"TTBAR*.root");
    // ch->Add(basedir_2017+"DataDoubleEG*.root");

    TString tag_2018 = "v3.03_nmiss0_data2018";
    TString basedir_2018 = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s/output/",tag_2018.Data());
    TChain *ch = new TChain("t");
    ch->Add(basedir_2017+"DY_high.root"); // TODO still using 2017 MC
    ch->Add(basedir_2017+"DY_low.root");
    ch->Add(basedir_2017+"TTBAR*.root");
    ch->Add(basedir_2018+"DataDoubleEG*.root");

    TString flipfname = "../derivation/outputs/histos_both.root";

    closure(ch, flipfname, "outputs/histos.root");


}
