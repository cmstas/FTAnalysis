{

    gSystem->Exec("mkdir -p outputs");

    gROOT->ProcessLine(".L ../../misc/class_files/v8.02/SS.cc+"); 
    gROOT->ProcessLine(".L ScanChain.C+");

    TString basedir("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.13_all/output/");

    // TChain *ch_2016 = new TChain("t","2016_tail");
    // ch_2016->Add(basedir+"/year_2016/DY*.root");
    // ch_2016->Add(basedir+"/year_2016/TTBAR*.root");
    // ch_2016->Add("/home/users/namin/2018/fourtop/all/FTAnalysis/babymaking/batch/output_DYzpt150.root");
    // flip(ch_2016, 2016);

    TChain *ch_2016 = new TChain("t","2016");
    ch_2016->Add(basedir+"/year_2016/DY*.root");
    ch_2016->Add(basedir+"/year_2016/TTBAR*.root");
    flip(ch_2016, 2016);

    TChain *ch_2017 = new TChain("t","2017");
    ch_2017->Add(basedir+"/year_2017/DY*.root");
    ch_2017->Add(basedir+"/year_2017/TTBAR*.root");
    flip(ch_2017, 2017);

    // FIXME
    basedir = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_all/output/";

    TChain *ch_2018 = new TChain("t","2018");
    ch_2018->Add(basedir+"/year_2018/DY*.root");
    ch_2018->Add(basedir+"/year_2018/TTBAR*.root");
    flip(ch_2018, 2018);

    // TChain *ch_2016_94x = new TChain("t","2016_94x");
    // ch_2016_94x->Add(basedir+"/year_2016_94x/DY*.root");
    // ch_2016_94x->Add(basedir+"/year_2016_94x/TTBAR*.root");
    // flip(ch_2016_94x, 2016);

    // TChain *ch_2016 = new TChain("t","2016_mu");
    // ch_2016->Add(basedir+"/year_2016/DY*.root");
    // ch_2016->Add(basedir+"/year_2016/TTBAR*.root");
    // flip(ch_2016, 2016, 13);

    // TChain *ch_2017 = new TChain("t","2017_mu");
    // ch_2017->Add(basedir+"/year_2017/DY*.root");
    // ch_2017->Add(basedir+"/year_2017/TTBAR*.root");
    // flip(ch_2017, 2017, 13);

}

