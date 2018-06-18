{
    gROOT->ProcessLine(".L ../misc/class_files/v8.02/SS.cc+");
    gROOT->ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc+");
    gROOT->ProcessLine(".L ScanChain.C+");

    int year = 2017;

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
    TString options = "useInclusiveSFs ";
    TString outputdir = Form("outputs%d", year);

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
    TChain ch_data("t", "data");
    ch_data.Add(basedir+"Data*.root");
    ScanChain(&ch_data, year, options, outputdir);

    // Data-Driven Fakes
    TChain ch_fakes("t", "fakes");
    ch_fakes.Add(basedir+"Data*.root");
    ScanChain(&ch_fakes, year, options + "doFakes", outputdir);

    // Data-Driven Flips
    TChain ch_flips("t", "flips");
    ch_flips.Add(basedir+"Data*.root");
    ScanChain(&ch_flips, year, options + "doFlips", outputdir);

    // Monte-Carlo Backgrounds
    TChain ch_ttw("t", "ttw");
    ch_ttw.Add(basedir+"TTWnlo.root");
    ScanChain(&ch_ttw, year, options, outputdir);

    TChain ch_ttz("t", "ttz");
    ch_ttz.Add(basedir+"TTZnlo.root");
    ScanChain(&ch_ttz, year, options, outputdir);

    TChain ch_tth("t", "tth");
    ch_tth.Add(basedir+"TTHtoNonBB.root");
    ScanChain(&ch_tth, year, options, outputdir);

    TChain ch_dy("t", "dy");
    ch_dy.Add(basedir+"DY_low.root");
    ch_dy.Add(basedir+"DY_high.root");
    ScanChain(&ch_dy, year, options, outputdir);

    TChain ch_wjets("t", "wjets");
    ch_wjets.Add(basedir+"WJets*.root");
    ScanChain(&ch_wjets, year, options, outputdir);

    TChain ch_tt("t", "tt");
    ch_tt.Add(basedir+"TTBAR*.root");
    ScanChain(&ch_tt, year, options, outputdir);

    TChain ch_vv("t", "vv");
    ch_vv.Add(basedir+"WZ.root");
    ch_vv.Add(basedir+"WW.root");
    ch_vv.Add(basedir+"WWDPS.root");
    ch_vv.Add(basedir+"ZZ.root");
    ScanChain(&ch_vv, year, options, outputdir);

    TChain ch_rares("t", "rares");
    ch_rares.Add(basedir+"WZG.root");
    ch_rares.Add(basedir+"WZZ.root");
    ch_rares.Add(basedir+"ZZZ.root");
    ch_rares.Add(basedir+"WWZ.root");
    ch_rares.Add(basedir+"WWW.root");
    ch_rares.Add(basedir+"TZQ.root");
    ch_rares.Add(basedir+"TTZZ.root");
    ch_rares.Add(basedir+"TTWZ.root");
    ch_rares.Add(basedir+"TTWW.root");
    ch_rares.Add(basedir+"TTWH.root");
    ch_rares.Add(basedir+"TTHH.root");
    ch_rares.Add(basedir+"TTTW.root");
    ch_rares.Add(basedir+"TTTJ.root");
    ScanChain(&ch_rares, year, options, outputdir);

    TChain ch_singletop("t", "singletop");
    ch_singletop.Add(basedir+"STtop.root");
    ch_singletop.Add(basedir+"STantitop.root");
    ScanChain(&ch_singletop, year, options, outputdir);

}

