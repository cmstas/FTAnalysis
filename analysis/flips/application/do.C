{

    gSystem->Exec("mkdir -p outputs");

    gROOT->ProcessLine(".L ../../misc/common_utils.h"); 
    gROOT->ProcessLine(".L ../../misc/class_files/v8.02/SS.cc+"); 
    // gROOT->ProcessLine(".L ../../../common/CORE/Tools/dorky/dorky.cc++");
    gROOT->ProcessLine(".L ScanChain.C+");

    // // TString tag = "v3.01";
    // // TString tag = "v3.01_nmiss0";
    // TString tag = "v3.02_nmiss0";
    // // TString tag = "v3.02_nmiss0_newmet";
    // TString basedir = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s/output/",tag.Data());
    // TChain *ch = new TChain("t");
    // ch->Add(basedir+"DY_high.root");
    // // ch->Add(basedir+"DY_highext1.root");
    // ch->Add(basedir+"DY_low.root");
    // ch->Add(basedir+"TTBAR*.root");
    // ch->Add(basedir+"DataDoubleEG*.root");
    // TString flipfname = "../derivation/outputs/histos_both.root";

    // TChain *ch2016 = new TChain("t");
    // ch2016->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/DY_high*.root");
    // ch2016->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/DY_low*.root");
    // ch2016->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/TTBAR*.root");
    // ch2016->Add("/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/DataDoubleEG*.root");
    // closure(ch2016, flipfname, "outputs/histos_2016.root", -1, -1, 35.9); // Run2016

    // closure(ch, flipfname, "outputs/histos_B.root", 297046, 299329, 4.767); // Run2017B
    // closure(ch, flipfname, "outputs/histos_C.root", 299368, 302029, 9.583); // Run2017C
    // closure(ch, flipfname, "outputs/histos_D.root", 302030, 303434, 4.224); // Run2017D
    // closure(ch, flipfname, "outputs/histos_E.root", 303824, 304797, 9.261); // Run2017E
    // closure(ch, flipfname, "outputs/histos_F.root", 305040, 306462, 13.463); // Run2017F
    // closure(ch, flipfname, "outputs/histos.root");

    TString basedir("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_all/output/");

    TChain *ch_2016 = new TChain("t","2016");
    ch_2016->Add(basedir+"/year_2016/DY_high.root");
    ch_2016->Add(basedir+"/year_2016/DY_low.root");
    ch_2016->Add(basedir+"/year_2016/TTBAR*.root");
    ch_2016->Add(basedir+"/year_2016/DataDoubleEG*.root");
    closure(ch_2016, "../derivation/outputs/histos_2016.root", "outputs/histos_2016.root", 2016);

    TChain *ch_2017 = new TChain("t","2017");
    ch_2017->Add(basedir+"/year_2017/DY_high.root");
    ch_2017->Add(basedir+"/year_2017/DY_low.root");
    ch_2017->Add(basedir+"/year_2017/TTBAR*.root");
    ch_2017->Add(basedir+"/year_2017/DataDoubleEG*.root");
    closure(ch_2017, "../derivation/outputs/histos_2017.root", "outputs/histos_2017.root", 2017);

    TChain *ch_2018 = new TChain("t","2018");
    ch_2018->Add(basedir+"/year_2018/DY_high.root");
    // ch_2018->Add(basedir+"/year_2018/DY_low.root");
    // ch_2018->Add(basedir+"/year_2018/TTBAR*.root");
    ch_2018->Add(basedir+"/year_2017/TTBAR*.root"); // FIXME this is 2017
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
