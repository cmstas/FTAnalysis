{
    gROOT->ProcessLine(".L ../misc/class_files/v8.02/SS.cc+");
    gROOT->ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc+");
    gROOT->ProcessLine(".L ScanChain.C+");

    int year = 2016;


    TString tag_data = "v0.10_mll";
    TString tag_mc = "v1.00_80x_baseline_full_v2";

    TString basedir_data = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s/output/skim/", tag_data.Data());
    TString basedir_mc =   Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s/output/", tag_mc.Data());

    /* Options:
     *     useInclusiveSFs: use inclusive (ie not era-dependent) lepton scale factors
     *     doFakes: Run data-driven fake estimate
     *     doFlips: Run charge mis-id estimate
     *     zeroMissingInnerHits: Require exactly zero missing inner hits
     */
    TString options = "useInclusiveSFs ";
    TString outputdir = Form("outputs%d", year);

    gSystem->Exec(Form("rm -rf %s", outputdir.Data()));
    gSystem->Exec(Form("mkdir -p %s", outputdir.Data()));

    gSystem->Exec(Form("git show HEAD > %s/git.diff", outputdir.Data()));
    gSystem->Exec(Form("git diff >> %s/git.diff", outputdir.Data()));

    // Data
    TChain ch_data("t", "data");
    ch_data.Add(basedir_data+"Data*.root");
    ScanChain(&ch_data, year, options, outputdir);

    // Data-Driven Fakes
    TChain ch_fakes("t", "fakes");
    ch_fakes.Add(basedir_data+"Data*.root");
    ScanChain(&ch_fakes, year, options + "doFakes", outputdir);

    // Data-Driven Flips
    TChain ch_flips("t", "flips");
    ch_flips.Add(basedir_data+"Data*.root");
    ScanChain(&ch_flips, year, options + "doFlips", outputdir);

    // Monte-Carlo Backgrounds
    TChain ch_ttw("t", "ttw");
    ch_ttw.Add(basedir_mc+"TTWnlo.root");
    ScanChain(&ch_ttw, year, options, outputdir);

    TChain ch_ttz("t", "ttz");
    ch_ttz.Add(basedir_mc+"TTZnlo.root");
    ScanChain(&ch_ttz, year, options, outputdir);

    TChain ch_tth("t", "tth");
    ch_tth.Add(basedir_mc+"TTHtoNonBB.root");
    ScanChain(&ch_tth, year, options, outputdir);

    TChain ch_dy("t", "dy");
    /* ch_dy.Add(basedir_mc+"DY_low.root"); */
    ch_dy.Add(basedir_mc+"DY_high.root");
    ScanChain(&ch_dy, year, options, outputdir);

    TChain ch_wjets("t", "wjets");
    ch_wjets.Add(basedir_mc+"WJets*.root");
    ScanChain(&ch_wjets, year, options, outputdir);

    TChain ch_tt("t", "tt");
    ch_tt.Add(basedir_mc+"TTBAR*.root");
    ScanChain(&ch_tt, year, options, outputdir);

    TChain ch_vv("t", "vv");
    ch_vv.Add(basedir_mc+"WZ.root");
    /* ch_vv.Add(basedir_mc+"WW.root"); */
    ch_vv.Add(basedir_mc+"WWDPS.root");
    ch_vv.Add(basedir_mc+"ZZ.root");
    ScanChain(&ch_vv, year, options, outputdir);

    TChain ch_rares("t", "rares");
    ch_rares.Add(basedir_mc+"WZG.root");
    ch_rares.Add(basedir_mc+"WZZ.root");
    ch_rares.Add(basedir_mc+"ZZZ.root");
    ch_rares.Add(basedir_mc+"WWZ.root");
    ch_rares.Add(basedir_mc+"WWW.root");
    ch_rares.Add(basedir_mc+"TZQ.root");
    ch_rares.Add(basedir_mc+"TTZZ.root");
    ch_rares.Add(basedir_mc+"TTWZ.root");
    ch_rares.Add(basedir_mc+"TTWW.root");
    ch_rares.Add(basedir_mc+"TTWH.root");
    ch_rares.Add(basedir_mc+"TTHH.root");
    ch_rares.Add(basedir_mc+"TTTW.root");
    ch_rares.Add(basedir_mc+"TTTJ.root");
    ScanChain(&ch_rares, year, options, outputdir);

    TChain ch_singletop("t", "singletop");
    ch_singletop.Add(basedir_mc+"ST1.root");
    ch_singletop.Add(basedir_mc+"ST2.root");
    ScanChain(&ch_singletop, year, options, outputdir);

}

