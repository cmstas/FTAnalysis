{
    TString outputdir = "outputs/";
    gSystem->Exec(Form("mkdir -p %s", outputdir.Data()));

    gROOT->ProcessLine(".L ../../misc/class_files/v8.02/SS.cc+");
    gROOT->ProcessLine(".L ScanChain.C+");

    TString basedir_2016 = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.07_all/output/year_2016/";
    TString basedir_2017 = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.07_all/output/year_2017/";


    TChain *ch_tttt = new TChain("t","tttt");
    ch_tttt->Add(basedir_2016+"/TTTTnew.root");
    ch_tttt->Add(basedir_2017+"/TTTTnew.root");
    ScanChain(ch_tttt, "", outputdir);

    TChain *ch_ttw = new TChain("t","ttw");
    ch_ttw->Add(basedir_2016+"/TTWnlo.root");
    ch_ttw->Add(basedir_2017+"/TTWnlo.root");
    ScanChain(ch_ttw, "", outputdir);

    TChain *ch_tth = new TChain("t","tth");
    ch_tth->Add(basedir_2016+"/TTHtoNonBB.root");
    ch_tth->Add(basedir_2017+"/TTHtoNonBB.root");
    ScanChain(ch_tth, "", outputdir);

    TChain *ch_ttz = new TChain("t","ttz");
    ch_ttz->Add(basedir_2016+"/TTZnlo.root");
    ch_ttz->Add(basedir_2016+"/TTZLOW.root");
    ch_ttz->Add(basedir_2017+"/TTZnlo.root");
    ch_ttz->Add(basedir_2017+"/TTZLOW.root");
    ScanChain(ch_ttz, "", outputdir);

    TChain *ch_xg = new TChain("t","xg");
    ch_xg->Add(basedir_2016+"/TGext.root");
    ch_xg->Add(basedir_2016+"/TTGdilep.root");
    ch_xg->Add(basedir_2016+"/TTGsinglelepbar.root");
    ch_xg->Add(basedir_2016+"/TTGsinglelep.root");
    ch_xg->Add(basedir_2016+"/WGToLNuGext.root");
    ch_xg->Add(basedir_2016+"/ZG.root");
    ch_xg->Add(basedir_2017+"/TGext.root");
    ch_xg->Add(basedir_2017+"/TTGsinglelepbar.root");
    ch_xg->Add(basedir_2017+"/TTGsinglelep.root");
    ScanChain(ch_xg, "", outputdir);

    TChain *ch_rares = new TChain("t","rares");
    ch_rares->Add(basedir_2016+"/GGHtoZZto4L.root");
    ch_rares->Add(basedir_2016+"/QQWW.root");
    ch_rares->Add(basedir_2016+"/TWZ.root");
    ch_rares->Add(basedir_2016+"/TZQ.root");
    ch_rares->Add(basedir_2016+"/VHtoNonBB.root");
    ch_rares->Add(basedir_2016+"/WWDPS.root");
    ch_rares->Add(basedir_2016+"/WWW.root");
    ch_rares->Add(basedir_2016+"/WWZ.root");
    ch_rares->Add(basedir_2016+"/WZ.root");
    ch_rares->Add(basedir_2016+"/WZG.root");
    ch_rares->Add(basedir_2016+"/WZZ.root");
    ch_rares->Add(basedir_2016+"/ZZ.root");
    ch_rares->Add(basedir_2016+"/ZZZ.root");
    ch_rares->Add(basedir_2016+"/TTTJ.root");
    ch_rares->Add(basedir_2016+"/TTTW.root");
    // ch_rares->Add(basedir_2017+"/GGHtoZZto4L.root");
    ch_rares->Add(basedir_2017+"/QQWW.root");
    // ch_rares->Add(basedir_2017+"/TWZ.root");
    // ch_rares->Add(basedir_2017+"/TZQ.root");
    ch_rares->Add(basedir_2017+"/VHtoNonBB.root");
    ch_rares->Add(basedir_2017+"/WWDPS.root");
    ch_rares->Add(basedir_2017+"/WWW.root");
    ch_rares->Add(basedir_2017+"/WWZ.root");
    ch_rares->Add(basedir_2017+"/WZ.root");
    ch_rares->Add(basedir_2017+"/WZG.root");
    ch_rares->Add(basedir_2017+"/WZZ.root");
    ch_rares->Add(basedir_2017+"/ZZ.root");
    ch_rares->Add(basedir_2017+"/ZZZ.root");
    ch_rares->Add(basedir_2017+"/TTTJ.root");
    ch_rares->Add(basedir_2017+"/TTTW.root");
    ScanChain(ch_rares, "", outputdir);

    TChain *ch_ttvv = new TChain("t","ttvv");
    ch_ttvv->Add(basedir_2016+"/TTHH.root");
    ch_ttvv->Add(basedir_2016+"/TTWH.root");
    ch_ttvv->Add(basedir_2016+"/TTWW.root");
    ch_ttvv->Add(basedir_2016+"/TTWZ.root");
    ch_ttvv->Add(basedir_2016+"/TTZZ.root");
    ch_ttvv->Add(basedir_2017+"/TTHH.root");
    ch_ttvv->Add(basedir_2017+"/TTWH.root");
    ch_ttvv->Add(basedir_2017+"/TTWW.root");
    ch_ttvv->Add(basedir_2017+"/TTWZ.root");
    ch_ttvv->Add(basedir_2017+"/TTZZ.root");
    ScanChain(ch_ttvv, "", outputdir);

    TChain *ch_flips = new TChain("t","flips");
    ch_flips->Add(basedir_2016+"/DataDoubleEG*.root");
    ch_flips->Add(basedir_2017+"/DataDoubleEG*.root");
    ScanChain(ch_flips, "", outputdir);

    TChain *ch_fakes = new TChain("t","fakes");
    ch_fakes->Add(basedir_2016+"/TTBAR_PH.root");
    ch_fakes->Add(basedir_2017+"/TTBAR_PH.root");
    // ch_fakes->Add(basedir_2016+"/Data*.root");
    // ch_fakes->Add(basedir_2017+"/Data*.root");
    ScanChain(ch_fakes, "", outputdir);

    // gSystem->Exec(Form("hadd -f output_total.root %s/output_*.root",outputdir.Data()));
    gSystem->Exec(Form("hadd -f output_total_withtmva.root %s/output_*.root",outputdir.Data()));
    // gSystem->Exec(Form("hadd -f output_nnvec.root %s/output_*.root",outputdir.Data()));
    // gSystem->Exec(Form("hadd -f output_total_mcfakes.root %s/output_*.root",outputdir.Data()));
    // gSystem->Exec(Form("hadd -f output_total_sigssos.root %s/output_*.root",outputdir.Data()));
    // gSystem->Exec(Form("hadd -f output_total_nnvec.root %s/output_*.root",outputdir.Data()));
    // gSystem->Exec(Form("hadd -f output_total_missm1.root %s/output_*.root",outputdir.Data()));

}

