{

    gROOT->ProcessLine(".L ../../misc/common_utils.h");

    gROOT->ProcessLine(".L ScanChain_fast.C+");

    // TChain *ch = new TChain("t");
    // // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_QCD_MuEnriched-2017v1_LEPTON_v15/output_*.root");
    // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_TTJets-2017v1_LEPTON_v15/output*.root");
    // // ch->Add("/home/users/namin/2017/ProjectMetis/LeptonBabyMaker/output_tt_100k.root");
    // // ch->Add("/home/users/namin/2017/ProjectMetis/LeptonBabyMaker/output_tt_100k_v2.root");
    // // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_TTJets-2017v1_LEPTON_v15/output_1.root");
    // TString suffix = "";
    // // TString options = "useLooseEMVA,IsoTrigs";
    // // TString options = "useLooseEMVA,IsoTrigs,requireMCMatch,doHighMET,noPUweight";
    // // TString options = "useLooseEMVA,IsoTrigs,requireMCMatch,doHighMET";
    // // TString options = "useLooseEMVA,IsoTrigs,requireMCMatch,doHighMET,allowMoreFO";
    // // TString options = "useLooseEMVA,IsoTrigs,requireMCMatch,doHighMET,twoFO,requireTwoJets,requireTag";
    // TString options = "useLooseEMVA,IsoTrigs,requireMCMatch,doHighMET,twoFO,requireTwoJets";
    // // TString options = "useLooseEMVA,IsoTrigs";
    // ScanChain(ch,"rate_test.root", options);
    // // ScanChain(ch,"rate_test_absweights.root", options);
    // //


    TChain *ch1 = new TChain("t"); ch1->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_QCD_EMEnriched-2017v1_LEPTON_v18/*.root");
                                   ch1->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_QCD_bcToE-2017v1_LEPTON_v18/*.root");
    TChain *ch2 = new TChain("t"); ch2->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_QCD_MuEnriched-2017v1_LEPTON_v17/*.root");

    TChain *ch3 = new TChain("t"); ch3->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_QCD_EMEnriched-2017v1_LEPTON_v18_oldiso/*.root");
                                   ch3->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_QCD_bcToE-2017v1_LEPTON_v18_oldiso/*.root");
    TChain *ch4 = new TChain("t"); ch4->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_QCD_MuEnriched-2017v1_LEPTON_v17_oldid/*.root");

    TChain *ch5 = new TChain("t"); ch5->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_TTJets-2017v1_LEPTON_v18/*.root");
    TChain *ch6 = new TChain("t"); ch6->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_TTJets-2017v1_LEPTON_v18_oldiso/*.root");

    TChain *ch7 = new TChain("t"); ch7->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_TTJets-2017v1_LEPTON_v19/*.root");
    TChain *ch8 = new TChain("t"); ch8->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_TTJets-2017v1_LEPTON_v20/*.root");

    // ScanChain(ch1,"outputs_test/rate_qcd_el_new.root", "requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch2,"outputs_test/rate_qcd_mu_new.root", "requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch3,"outputs_test/rate_qcd_el_old.root", "requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch4,"outputs_test/rate_qcd_mu_old.root", "requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch5,"outputs_test/rate_tthad_new.root", "requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch6,"outputs_test/rate_tthad_old.root", "requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch5,"outputs_test/rate_tt_new.root", "requireTwoJets,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch6,"outputs_test/rate_tt_old.root", "requireTwoJets,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");

    // ScanChain(ch1,"outputs_testabove25/rate_qcd_el_new.root", "requireMCMatch,doAbove25,useLooseEMVA,IsoTrigs");
    // ScanChain(ch2,"outputs_testabove25/rate_qcd_mu_new.root", "requireMCMatch,doAbove25,useLooseEMVA,IsoTrigs");
    // ScanChain(ch3,"outputs_testabove25/rate_qcd_el_old.root", "requireMCMatch,doAbove25,useLooseEMVA,IsoTrigs");
    // ScanChain(ch4,"outputs_testabove25/rate_qcd_mu_old.root", "requireMCMatch,doAbove25,useLooseEMVA,IsoTrigs");
    // ScanChain(ch5,"outputs_testabove25/rate_tthad_new.root", "requireTwoJets,doAbove25,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch6,"outputs_testabove25/rate_tthad_old.root", "requireTwoJets,doAbove25,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch5,"outputs_testabove25/rate_tt_new.root", "requireTwoJets,doAbove25,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch6,"outputs_testabove25/rate_tt_old.root", "requireTwoJets,doAbove25,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");

    // ScanChain(ch1,"outputs_testnoconecorr/rate_qcd_el_new.root", "noConeCorr,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch2,"outputs_testnoconecorr/rate_qcd_mu_new.root", "noConeCorr,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch3,"outputs_testnoconecorr/rate_qcd_el_old.root", "noConeCorr,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch4,"outputs_testnoconecorr/rate_qcd_mu_old.root", "noConeCorr,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch5,"outputs_testnoconecorr/rate_tthad_new.root", "noConeCorr,requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch6,"outputs_testnoconecorr/rate_tthad_old.root", "noConeCorr,requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch5,"outputs_testnoconecorr/rate_tt_new.root", "noConeCorr,requireTwoJets,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch6,"outputs_testnoconecorr/rate_tt_old.root", "noConeCorr,requireTwoJets,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");

    // ScanChain(ch1,"outputs_testmu18/rate_qcd_el_new.root", "doMu18Only,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch2,"outputs_testmu18/rate_qcd_mu_new.root", "doMu18Only,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch3,"outputs_testmu18/rate_qcd_el_old.root", "doMu18Only,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch4,"outputs_testmu18/rate_qcd_mu_old.root", "doMu18Only,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch5,"outputs_testmu18/rate_tthad_new.root", "doMu18Only,requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch6,"outputs_testmu18/rate_tthad_old.root", "doMu18Only,requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch5,"outputs_testmu18/rate_tt_new.root", "doMu18Only,requireTwoJets,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch6,"outputs_testmu18/rate_tt_old.root", "doMu18Only,requireTwoJets,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");

    // ScanChain(ch1,"outputs_testnewp/rate_qcd_el_new.root", "newPrescription,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch2,"outputs_testnewp/rate_qcd_mu_new.root", "newPrescription,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch3,"outputs_testnewp/rate_qcd_el_old.root", "newPrescription,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch4,"outputs_testnewp/rate_qcd_mu_old.root", "newPrescription,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch5,"outputs_testnewp/rate_tthad_new.root", "newPrescription,requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch6,"outputs_testnewp/rate_tthad_old.root", "newPrescription,requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch5,"outputs_testnewp/rate_tt_new.root", "newPrescription,requireTwoJets,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch6,"outputs_testnewp/rate_tt_old.root", "newPrescription,requireTwoJets,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");

    // ScanChain(ch7,"outputs_testttbar25/rate_tt_new.root", "doAbove25,requireTwoJets,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch7,"outputs_testttbar25/rate_tt_bonly_new.root", "doBonly,doAbove25,requireTwoJets,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch7,"outputs_testttbar25/rate_tthad1_new.root", "doAbove25,requireTwoJets,doHighMET,singleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch7,"outputs_testttbar25/rate_tthad1bonly_new.root", "doBonly,doAbove25,requireTwoJets,doHighMET,singleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch7,"outputs_testttbar25/rate_tthad2_new.root", "doAbove25,requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch7,"outputs_testttbar25/rate_tthad2_bonly_new.root", "doBonly,doAbove25,requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch7,"outputs_testttbar25/rate_tthad1_fogeq2_new.root", "twoFO,doAbove25,requireTwoJets,doHighMET,singleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch7,"outputs_testttbar25/rate_tthad1_fogeq2_bonly_new.root", "doBonly,twoFO,doAbove25,requireTwoJets,doHighMET,singleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch7,"outputs_testttbar25/rate_tthad1tag_new.root", "doAbove25,requireTag,requireTwoJets,doHighMET,singleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch7,"outputs_testttbar25/rate_tthad2_fogeq2_new.root", "twoFO,doAbove25,requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");

    // ScanChain(ch8,"outputs_testttbar25/rate_tthad2_abs_new.root", "absweight,doAbove25,requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch8,"outputs_testttbar25/rate_tthad2_abs_bonly_new.root", "absweight,doBonly,doAbove25,requireTwoJets,doHighMET,doubleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch8,"outputs_testttbar25/rate_tthad1_abs_fogeq2_new.root", "absweight,twoFO,doAbove25,requireTwoJets,doHighMET,singleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch8,"outputs_testttbar25/rate_tthad1_abs_fogeq2_bonly_new.root", "absweight,doBonly,twoFO,doAbove25,requireTwoJets,doHighMET,singleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");

    // ScanChain(ch8,"outputs_testttbar25/rate_tthad1_fogeq2_new.root", "twoFO,doAbove25,requireTwoJets,doHighMET,singleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch8,"outputs_testttbar25/rate_tthad1_fogeq2_notrig_new.root", "bypassTriggers,twoFO,doAbove25,requireTwoJets,doHighMET,singleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");

    // ScanChain(ch7,"outputs_testttbar25/rate_tthad1_fix_new.root", "doAbove25,requireTwoJets,doHighMET,singleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch7,"outputs_testttbar25/rate_tthad1_fogeq2_fix_new.root", "twoFO,doAbove25,requireTwoJets,doHighMET,singleHadronic,requireMCMatch,useLooseEMVA,IsoTrigs");

    // ScanChain(ch1,"outputs_testallmetmt/rate_qcd_el_new.root", "bypassLowMET,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch2,"outputs_testallmetmt/rate_qcd_mu_new.root", "bypassLowMET,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch3,"outputs_testallmetmt/rate_qcd_el_old.root", "bypassLowMET,requireMCMatch,useLooseEMVA,IsoTrigs");
    // ScanChain(ch4,"outputs_testallmetmt/rate_qcd_mu_old.root", "bypassLowMET,requireMCMatch,useLooseEMVA,IsoTrigs");
    
    // ScanChain(ch5,"outputs_test/rate_test.root", "requireTwoJets,doHighMET,requireMCMatch,useLooseEMVA,IsoTrigs");

    // TChain* chtest = new TChain("t");
    // chtest->Add("/home/users/namin/2017/ProjectMetis/LeptonBabyMaker/output_DoubleMuon_Run2017D_before.root");
    // ScanChain(chtest,"outputs_test/rate_test_before.root", 2017, "doAbove18,useLooseEMVA,IsoTrigs");

    // TChain* chtest = new TChain("t");
    // // chtest->Add("/home/users/namin/2017/ProjectMetis/LeptonBabyMaker/output_DoubleEG_Run2017D_after.root");
    // chtest->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017C-31Mar2018-v1_LEPTON_v24jec6//output_57.root");
    // // chtest->Add("/home/users/namin/2017/ProjectMetis/LeptonBabyMaker/output_DoubleMuon_Run2017D_after3.root");
    // // chtest->Add("/home/users/namin/2017/ProjectMetis/LeptonBabyMaker/output_DYJets_after.root");
    // // ScanChain(chtest,"outputs_test/rate_test_after.root", 2017, "doAbove18,useLooseEMVA,IsoTrigs");
    // ScanChain(chtest,"outputs_test/rate_test_after.root", 2017, "doAbove18,useLooseEMVA,IsoTrigs");

    // TChain* chtest = new TChain("t");
    // chtest->Add("/home/users/namin/2017/ProjectMetis/LeptonBabyMaker/output_qcdmu_synch.root");
    // ScanChain(chtest,"outputs_test/rate_test_before.root", 2017, "doAbove18,useLooseEMVA,IsoTrigs");
    // // chtest->Add("/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/babymaker/output_qcdmu_synch.root");
    // // ScanChain(chtest,"outputs_test/rate_test_after.root", 2017, "doAbove18,useLooseEMVA,IsoTrigs");

    TChain* chtest1 = new TChain("t");
    chtest1->Add("/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v1.0_leptontree/output/year_2017/QCD_MuEnriched_*.root");
    // ScanChain(chtest1,"outputs_test/rate_test_before.root", 2017, "doAbove18,useLooseEMVA,IsoTrigs");
    ScanChain(chtest1,"outputs_test/rate_test_qcd_mu.root", 2017, "useLooseEMVA,IsoTrigs");
    TChain* chtest2 = new TChain("t");
    chtest2->Add("/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v1.0_leptontree/output/year_2017/QCD_EMEnriched_*.root");
    chtest2->Add("/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v1.0_leptontree/output/year_2017/QCD_bcToE_*.root");
    ScanChain(chtest2,"outputs_test/rate_test_qcd_el.root", 2017, "useLooseEMVA,IsoTrigs");
    TChain* chtest3 = new TChain("t");
    chtest3->Add("/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v1.0_leptontree/output/year_2017/WJets.root");
    ScanChain(chtest3,"outputs_test/rate_test_wjets.root", 2017, "useLooseEMVA,IsoTrigs");
    TChain* chtest4 = new TChain("t");
    chtest4->Add("/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v1.0_leptontree/output/year_2017/DY_high.root");
    ScanChain(chtest4,"outputs_test/rate_test_dy.root", 2017, "useLooseEMVA,IsoTrigs");
    TChain* chtest5 = new TChain("t");
    chtest5->Add("/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v1.0_leptontree/output/year_2017/DataDoubleEG*.root");
    ScanChain(chtest5,"outputs_test/rate_test_data_el.root", 2017, "useLooseEMVA,IsoTrigs");
    TChain* chtest6 = new TChain("t");
    chtest6->Add("/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v1.0_leptontree/output/year_2017/DataDoubleMu*.root");
    ScanChain(chtest6,"outputs_test/rate_test_data_mu.root", 2017, "useLooseEMVA,IsoTrigs");
    TChain* chtest7 = new TChain("t");
    chtest7->Add("/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v1.0_leptontree/output/year_2017/TT*.root");
    ScanChain(chtest7,"outputs_test/rate_test_ttjets.root", 2017, "useLooseEMVA,IsoTrigs");

    // TChain *ch = new TChain("t");
    // // ch->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_WJets-2017v1_LEPTON_v25jec32/*.root");
    // ch->Add("output_WJets.root");
    // ScanChain(ch,"outputs_test/rate_test_wjets_before.root",2017,"useLooseEMVA,IsoTrigs");


    // chtest->Add("/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/babymaker/output_qcdmu_synch.root");
    // ScanChain(chtest,"outputs_test/rate_test_after.root", 2017, "doAbove18,useLooseEMVA,IsoTrigs");


}
