{
    gROOT->ProcessLine(".L ../misc/class_files/v8.02/SS.cc+");
    gROOT->ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc+");
    gROOT->ProcessLine(".L ScanChain.C+");

    /* TString tag = "v1.00_94x_baseline_withos_v3"; */
    /* TString tag = "v1.02_94x_withos_relaxhitsmore"; */
    TString tag = "v1.03_os_nmiss1_dileptrig_v1";
    TString basedir = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s/output/",tag.Data());

    /* Options:
     *     useInclusiveSFs: use inclusive (ie not era-dependent) lepton scale factors
     *     doFakes: Run data-driven fake estimate
     */
    TString options = "useInclusiveSFs";

    gSystem->Exec(Form("mkdir -p outputs%s", options.Data()));

    TChain *ch_data = new TChain("t","data");
    ch_data->Add(basedir+"Data*.root");
    ScanChain(ch_data, options);

    TChain *ch_fakes = new TChain("t","fakes");
    ch_fakes->Add(basedir+"Data*.root");
    ScanChain(ch_fakes, options + "doFakes");

    TChain *ch_ttw = new TChain("t","ttw");
    ch_ttw->Add(basedir+"TTWnlo.root");
    ScanChain(ch_ttw, options);

    TChain *ch_ttz = new TChain("t","ttz");
    ch_ttz->Add(basedir+"TTZnlo.root");
    ScanChain(ch_ttz, options);

    TChain *ch_tth = new TChain("t","tth");
    ch_tth->Add(basedir+"TTHtoNonBB.root");
    ScanChain(ch_tth, options);

    TChain *ch_dy = new TChain("t","dy");
    ch_dy->Add(basedir+"DY_low.root");
    ch_dy->Add(basedir+"DY_high.root");
    ScanChain(ch_dy, options);

    TChain *ch_wjets = new TChain("t","wjets");
    ch_wjets->Add(basedir+"WJets*.root");
    ScanChain(ch_wjets, options);

    TChain *ch_tt = new TChain("t","tt");
    ch_tt->Add(basedir+"TTBAR*.root");
    ScanChain(ch_tt, options);

}

