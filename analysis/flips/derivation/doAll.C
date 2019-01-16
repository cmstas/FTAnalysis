{

    gSystem->Exec("mkdir -p outputs");

    gROOT->ProcessLine(".L ../../misc/class_files/v8.02/SS.cc+"); 
    gROOT->ProcessLine(".L ScanChain.C+");

    TString basedir("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.21/output/");

    TChain *ch_2016 = new TChain("t","2016");
    ch_2016->Add(basedir+"/year_2016/DY*.root");
    ch_2016->Add(basedir+"/year_2016/TTBAR*.root");
    flip(ch_2016, 2016);

    basedir = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.21_fix2017/output/";
    TChain *ch_2017 = new TChain("t","2017");
    ch_2017->Add(basedir+"/year_2017/DY*.root");
    ch_2017->Add(basedir+"/year_2017/TTBAR*.root");
    flip(ch_2017, 2017);

    basedir = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.22/output/";
    TChain *ch_2018 = new TChain("t","2018");
    ch_2018->Add(basedir+"/year_2018/DY*.root");
    ch_2018->Add(basedir+"/year_2018/TTBAR*.root");
    flip(ch_2018, 2018);

}

