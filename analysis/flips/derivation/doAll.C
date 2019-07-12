{

    gSystem->Exec("mkdir -p outputs");

    gROOT->ProcessLine(".L ../../misc/class_files/v8.02/SS.cc+"); 
    gROOT->ProcessLine(".L ScanChain.C+");


    // int which = 11; // consider electrons

    // TString basedir("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.23/output/");

    // TChain *ch_2016 = new TChain("t","2016");
    // ch_2016->Add(basedir+"/year_2016/DY*.root");
    // ch_2016->Add(basedir+"/year_2016/TTBAR*.root");
    // flip(ch_2016, 2016, which);

    // TChain *ch_2017 = new TChain("t","2017");
    // ch_2017->Add(basedir+"/year_2017/DY*.root");
    // ch_2017->Add(basedir+"/year_2017/TTBAR*.root");
    // flip(ch_2017, 2017, which);

    // TChain *ch_2018 = new TChain("t","2018");
    // ch_2018->Add(basedir+"/year_2018/DY*.root");
    // ch_2018->Add(basedir+"/year_2018/TTBAR*.root");
    // flip(ch_2018, 2018, which);



    // which = 13; // consider muons

    // TChain *ch_2016_mu = new TChain("t","2016_mu");
    // ch_2016_mu->Add(basedir+"/year_2016/DY*.root");
    // ch_2016_mu->Add(basedir+"/year_2016/TTBAR*.root");
    // flip(ch_2016_mu, 2016, which);

    // TChain *ch_2017_mu = new TChain("t","2017_mu");
    // ch_2017_mu->Add(basedir+"/year_2017/DY*.root");
    // ch_2017_mu->Add(basedir+"/year_2017/TTBAR*.root");
    // flip(ch_2017_mu, 2017, which);

    // TChain *ch_2018_mu = new TChain("t","2018_mu");
    // ch_2018_mu->Add(basedir+"/year_2018/DY*.root");
    // ch_2018_mu->Add(basedir+"/year_2018/TTBAR*.root");
    // flip(ch_2018_mu, 2018, which);


    int which = 11; // consider electrons
    TString basedir("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.31/output/");
    TChain *ch_2016 = new TChain("t","2016");
    ch_2016->Add(basedir+"/year_2016_94x/DY*.root");
    ch_2016->Add(basedir+"/year_2016_94x/TTBAR*.root");
    flip(ch_2016, 2016, which);

}

