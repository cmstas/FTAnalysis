{
    gROOT->ProcessLine(".L ../misc/class_files/v8.02/SS.cc+");
    gROOT->ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc+");
    gROOT->ProcessLine(".L ScanChain.C+");


    /* TString tag = "v1.00_94x_baseline_withos_v3"; */
    /* TString tag = "v1.02_94x_withos_relaxhitsmore"; */
    // TString tag = "v1.04_v1";
    TString tag = "v1.05_v1";
    TString basedir_2017 = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s/output/", tag.Data());

    /* Options:
     *     useInclusiveSFs: use inclusive (ie not era-dependent) lepton scale factors
     *     doFakes: Run data-driven fake estimate
     *     doFlips: Run charge mis-id estimate
     *     zeroMissingInnerHits: Require exactly zero missing inner hits
     */

    // TString options = "useInclusiveSFs evaluateBDT";
    TString options = "useInclusiveSFs ";
    TString outputdir = "outputs";

    // by default, iso triggers for 2017 and noniso for 2016

    // // FIXME FIXME no met cut
    // iso iso
    options += " useIsoTriggers2016 ";
    outputdir = "outputs_iso";

    // // noniso noniso
    // options += " useNonIsoTriggers2017 useNonIsoTriggers2016 ";
    // outputdir = "outputs_noniso";

    // // iso iso high ht
    // options += " useIsoTriggers2016 doHighHT ";
    // outputdir = "outputs_iso_ht";

    // // noniso noniso high ht
    // options += " useNonIsoTriggers2017 useNonIsoTriggers2016 doHighHT ";
    // outputdir = "outputs_noniso_ht";

    // gSystem->Exec(Form("rm -rf %s", outputdir.Data()));
    gSystem->Exec(Form("mkdir -p %s", outputdir.Data()));

    gSystem->Exec(Form("git show HEAD > %s/git.diff", outputdir.Data()));
    gSystem->Exec(Form("git diff >> %s/git.diff", outputdir.Data()));

    // Data
    TChain ch_data_2017("t", "data_2017");
    ch_data_2017.Add(basedir_2017+"Data*.root");

    // Data-Driven Fakes
    TChain ch_fakes_2017("t", "fakes_2017");
    ch_fakes_2017.Add(basedir_2017+"Data*.root");

    // Data-Driven Flips
    TChain ch_flips_2017("t", "flips_2017");
    ch_flips_2017.Add(basedir_2017+"Data*.root");

    // Monte-Carlo Backgrounds
    TChain ch_ttw_2017("t", "ttw_2017");
    ch_ttw_2017.Add(basedir_2017+"TTWnlo.root");

    TChain ch_ttz_2017("t", "ttz_2017");
    ch_ttz_2017.Add(basedir_2017+"TTZnlo.root");

    TChain ch_tth_2017("t", "tth_2017");
    ch_tth_2017.Add(basedir_2017+"TTHtoNonBB.root");

    TChain ch_dy_2017("t", "dy_2017");
    ch_dy_2017.Add(basedir_2017+"DY_low.root");
    ch_dy_2017.Add(basedir_2017+"DY_high.root");

    TChain ch_wjets_2017("t", "wjets_2017");
    ch_wjets_2017.Add(basedir_2017+"WJets*.root");

    TChain ch_tt_2017("t", "tt_2017");
    ch_tt_2017.Add(basedir_2017+"TTBAR*.root");

    TChain ch_ttfake_2017("t", "ttfake_2017");
    ch_ttfake_2017.Add(basedir_2017+"TTBAR*.root");

    TChain ch_vv_2017("t", "vv_2017");
    ch_vv_2017.Add(basedir_2017+"WZ.root");
    ch_vv_2017.Add(basedir_2017+"WW.root");
    ch_vv_2017.Add(basedir_2017+"WWDPS.root");
    ch_vv_2017.Add(basedir_2017+"ZZ.root");

    TChain ch_rares_2017("t", "rares_2017");
    ch_rares_2017.Add(basedir_2017+"WZG.root");
    ch_rares_2017.Add(basedir_2017+"WZZ.root");
    ch_rares_2017.Add(basedir_2017+"ZZZ.root");
    ch_rares_2017.Add(basedir_2017+"WWZ.root");
    ch_rares_2017.Add(basedir_2017+"WWW.root");
    ch_rares_2017.Add(basedir_2017+"TZQ.root");
    ch_rares_2017.Add(basedir_2017+"TTZZ.root");
    ch_rares_2017.Add(basedir_2017+"TTWZ.root");
    ch_rares_2017.Add(basedir_2017+"TTWW.root");
    ch_rares_2017.Add(basedir_2017+"TTWH.root");
    ch_rares_2017.Add(basedir_2017+"TTHH.root");
    ch_rares_2017.Add(basedir_2017+"TTTW.root");
    ch_rares_2017.Add(basedir_2017+"TTTJ.root");

    TChain ch_singletop_2017("t", "singletop_2017");
    ch_singletop_2017.Add(basedir_2017+"STtop.root");
    ch_singletop_2017.Add(basedir_2017+"STantitop.root");


    // TString basedir_2016_data = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mll/output/skim/";
    // TString basedir_2016_mc = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v1.00_80x_baseline_full_v2/output/";
    TString basedir_2016_data = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v1.00_80x_baseline_full_v3/output/";
    TString basedir_2016_mc = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v1.00_80x_baseline_full_v3/output/";

    // Data
    TChain ch_data_2016("t", "data_2016");
    ch_data_2016.Add(basedir_2016_data+"Data*.root");

    // Data-Driven Fakes
    TChain ch_fakes_2016("t", "fakes_2016");
    ch_fakes_2016.Add(basedir_2016_data+"Data*.root");

    // Data-Driven Flips
    TChain ch_flips_2016("t", "flips_2016");
    ch_flips_2016.Add(basedir_2016_data+"Data*.root");

    // Monte-Carlo Backgrounds
    TChain ch_ttw_2016("t", "ttw_2016");
    ch_ttw_2016.Add(basedir_2016_mc+"TTWnlo.root");

    TChain ch_ttz_2016("t", "ttz_2016");
    ch_ttz_2016.Add(basedir_2016_mc+"TTZnlo.root");

    TChain ch_tth_2016("t", "tth_2016");
    ch_tth_2016.Add(basedir_2016_mc+"TTHtoNonBB.root");

    TChain ch_dy_2016("t", "dy_2016");
    // ch_dy_2016.Add(basedir_2016_mc+"DY_low.root");
    ch_dy_2016.Add(basedir_2016_mc+"DY_high.root");

    TChain ch_wjets_2016("t", "wjets_2016");
    ch_wjets_2016.Add(basedir_2016_mc+"WJets*.root");

    TChain ch_tt_2016("t", "tt_2016");
    ch_tt_2016.Add(basedir_2016_mc+"TTBAR*.root");

    TChain ch_ttfake_2016("t", "ttfake_2016");
    ch_ttfake_2016.Add(basedir_2016_mc+"TTBAR*.root");

    TChain ch_vv_2016("t", "vv_2016");
    ch_vv_2016.Add(basedir_2016_mc+"WZ.root");
    ch_vv_2016.Add(basedir_2016_mc+"QQWW.root");
    ch_vv_2016.Add(basedir_2016_mc+"WWDPS.root");
    ch_vv_2016.Add(basedir_2016_mc+"ZZ.root");

    TChain ch_rares_2016("t", "rares_2016");
    ch_rares_2016.Add(basedir_2016_mc+"WZG.root");
    ch_rares_2016.Add(basedir_2016_mc+"WZZ.root");
    ch_rares_2016.Add(basedir_2016_mc+"ZZZ.root");
    ch_rares_2016.Add(basedir_2016_mc+"WWZ.root");
    ch_rares_2016.Add(basedir_2016_mc+"WWW.root");
    ch_rares_2016.Add(basedir_2016_mc+"TZQ.root");
    ch_rares_2016.Add(basedir_2016_mc+"TTZZ.root");
    ch_rares_2016.Add(basedir_2016_mc+"TTWZ.root");
    ch_rares_2016.Add(basedir_2016_mc+"TTWW.root");
    ch_rares_2016.Add(basedir_2016_mc+"TTWH.root");
    ch_rares_2016.Add(basedir_2016_mc+"TTHH.root");
    ch_rares_2016.Add(basedir_2016_mc+"TTTW.root");
    ch_rares_2016.Add(basedir_2016_mc+"TTTJ.root");
    ch_rares_2016.Add(basedir_2016_mc+"ZG.root");
    ch_rares_2016.Add(basedir_2016_mc+"TTGdilep.root");
    ch_rares_2016.Add(basedir_2016_mc+"WGToLNuGext.root");

    TChain ch_singletop_2016("t", "singletop_2016");
    ch_singletop_2016.Add(basedir_2016_mc+"ST*.root");

    ScanChain(&ch_data_2017, options, outputdir);
    ScanChain(&ch_fakes_2017, options + "doFakes", outputdir);
    ScanChain(&ch_flips_2017, options + "doFlips", outputdir);
    ScanChain(&ch_ttw_2017, options, outputdir);
    ScanChain(&ch_ttz_2017, options, outputdir);
    ScanChain(&ch_tth_2017, options, outputdir);
    ScanChain(&ch_dy_2017, options, outputdir);
    ScanChain(&ch_wjets_2017, options, outputdir);
    ScanChain(&ch_tt_2017, options, outputdir);
    ScanChain(&ch_ttfake_2017, options + "doTruthFake", outputdir);
    ScanChain(&ch_vv_2017, options, outputdir);
    ScanChain(&ch_rares_2017, options, outputdir);
    ScanChain(&ch_singletop_2017, options, outputdir);
    options += " Data2016 ";
    ScanChain(&ch_data_2016, options, outputdir);
    ScanChain(&ch_fakes_2016, options + "doFakes", outputdir);
    ScanChain(&ch_flips_2016, options + "doFlips", outputdir);
    ScanChain(&ch_ttw_2016, options, outputdir);
    ScanChain(&ch_ttz_2016, options, outputdir);
    ScanChain(&ch_tth_2016, options, outputdir);
    ScanChain(&ch_dy_2016, options, outputdir);
    ScanChain(&ch_wjets_2016, options, outputdir);
    ScanChain(&ch_tt_2016, options, outputdir);
    ScanChain(&ch_ttfake_2016, options + "doTruthFake", outputdir);
    ScanChain(&ch_vv_2016, options, outputdir);
    ScanChain(&ch_rares_2016, options, outputdir);
    ScanChain(&ch_singletop_2016, options, outputdir);

}

