{
    gROOT->ProcessLine(".L ../misc/class_files/v8.02/SS.cc+");
    gROOT->ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc+");
    gROOT->ProcessLine(".L ScanChain.C+");

    /* TString tag = "v1.00_94x_baseline_withos_v3"; */
    /* TString tag = "v1.02_94x_withos_relaxhitsmore"; */
    TString tag = "v1.03_os_nmiss1_dileptrig_v1";
    TString basedir = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s/output/", tag.Data());

    /* Options:
     *     useInclusiveSFs: use inclusive (ie not era-dependent) lepton scale factors
     *     doFakes: Run data-driven fake estimate
     *     doFlips: Run charge mis-id estimate
     *     zeroMissingInnerHits: Require exactly zero missing inner hits
     */
    TString options = "useInclusiveSFs";
    TString outputdir = "outputs";

    gSystem->Exec(Form("mkdir -p %s", outputdir.Data()));

    // Data
    TChain ch_data("t", "data");
    ch_data.Add(basedir+"Data*.root");
    ScanChain(&ch_data, options);

    // Data-Driven Fakes
    TChain ch_fakes("t", "fakes");
    ch_fakes.Add(basedir+"Data*.root");
    ScanChain(&ch_fakes, options + "doFakes");

    // Data-Driven Flips
    TChain ch_flips("t", "flips");
    ch_flips.Add(basedir+"Data*.root");
    ScanChain(&ch_flips, options + "doFlips");

    // Monte-Carlo Backgrounds
    TChain ch_ttw("t", "ttw");
    ch_ttw.Add(basedir+"TTWnlo.root");
    ScanChain(&ch_ttw, options);

    TChain ch_ttz("t", "ttz");
    ch_ttz.Add(basedir+"TTZnlo.root");
    ScanChain(&ch_ttz, options);

    TChain ch_tth("t", "tth");
    ch_tth.Add(basedir+"TTHtoNonBB.root");
    ScanChain(&ch_tth, options);

    TChain ch_dy("t", "dy");
    ch_dy.Add(basedir+"DY_low.root");
    ch_dy.Add(basedir+"DY_high.root");
    ScanChain(&ch_dy, options);

    TChain ch_wjets("t", "wjets");
    ch_wjets.Add(basedir+"WJets*.root");
    ScanChain(&ch_wjets, options);

    TChain ch_tt("t", "tt");
    ch_tt.Add(basedir+"TTBAR*.root");
    ScanChain(&ch_tt, options);
}

