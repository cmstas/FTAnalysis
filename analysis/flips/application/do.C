{

    gSystem->Exec("mkdir -p outputs");

    gROOT->ProcessLine(".L ../../misc/common_utils.h"); 
    gROOT->ProcessLine(".L ../../misc/class_files/v8.02/SS.cc+"); 
    gROOT->ProcessLine(".L ScanChain.C+");

    TString basedir("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.21/output/");

    TChain *ch_2016 = new TChain("t","2016");
    ch_2016->Add(basedir+"/year_2016/DY_high.root");
    ch_2016->Add(basedir+"/year_2016/DY_low.root");
    ch_2016->Add(basedir+"/year_2016/TTBAR*.root");
    ch_2016->Add(basedir+"/year_2016/DataDoubleEG*.root");
    closure(ch_2016, "../derivation/outputs/histos_2016.root", "outputs/histos_2016.root", 2016);

    basedir = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.21_fix2017/output/";

    TChain *ch_2017 = new TChain("t","2017");
    ch_2017->Add(basedir+"/year_2017/DY_high.root");
    ch_2017->Add(basedir+"/year_2017/DY_low.root");
    ch_2017->Add(basedir+"/year_2017/TTBAR*.root");
    ch_2017->Add(basedir+"/year_2017/DataDoubleEG*.root");
    closure(ch_2017, "../derivation/outputs/histos_2017.root", "outputs/histos_2017.root", 2017);

    basedir = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.22/output/";

    TChain *ch_2018 = new TChain("t","2018");
    ch_2018->Add(basedir+"/year_2018/DY_high.root");
    ch_2018->Add(basedir+"/year_2018/DY_low.root");
    ch_2018->Add(basedir+"/year_2018/TTBAR*.root");
    ch_2018->Add(basedir+"/year_2018/DataDoubleEG*.root");
    closure(ch_2018, "../derivation/outputs/histos_2018.root", "outputs/histos_2018.root", 2018);
    
    // TChain *ch_2016_94x = new TChain("t","2016_94x");
    // ch_2016_94x->Add(basedir+"/year_2016_94x/DY_high.root");
    // // ch_2016_94x->Add(basedir+"/year_2016_94x/DY_low.root");
    // ch_2016_94x->Add(basedir+"/year_2016_94x/TTBAR*.root");
    // ch_2016_94x->Add(basedir+"/year_2016_94x/DataDoubleEG*.root");
    // closure(ch_2016_94x, "../derivation/outputs/histos_2016_94x.root", "outputs/histos_2016_94x.root", 2016);
    
    // TChain *ch_2016_tail = new TChain("t","2016_tail");
    // ch_2016_tail->Add(basedir+"/year_2016/DY_high.root");
    // ch_2016_tail->Add(basedir+"/year_2016/DY_low.root");
    // ch_2016_tail->Add(basedir+"/year_2016/TTBAR*.root");
    // ch_2016_tail->Add(basedir+"/year_2016/DataDoubleEG*.root");
    // closure(ch_2016_tail, "../derivation/outputs/histos_2016_tail.root", "outputs/histos_2016_tail.root", 2016);

    // TChain *ch_2016 = new TChain("t","2016");
    // ch_2016->Add(basedir+"/year_2016/DY_high.root");
    // ch_2016->Add(basedir+"/year_2016/DY_low.root");
    // ch_2016->Add(basedir+"/year_2016/TTBAR*.root");
    // ch_2016->Add(basedir+"/year_2016/DataDoubleMu*.root");
    // closure(ch_2016, "../derivation/outputs/histos_2016_mu.root", "outputs/histos_2016_mu.root", 2016, -1, -1, -1, 13);

    // TChain *ch_2017 = new TChain("t","2017");
    // ch_2017->Add(basedir+"/year_2017/DY_high.root");
    // ch_2017->Add(basedir+"/year_2017/DY_low.root");
    // ch_2017->Add(basedir+"/year_2017/TTBAR*.root");
    // ch_2017->Add(basedir+"/year_2017/DataDoubleMu*.root");
    // closure(ch_2017, "../derivation/outputs/histos_2017_mu.root", "outputs/histos_2017_mu.root", 2017, -1, -1, -1, 13);

}
