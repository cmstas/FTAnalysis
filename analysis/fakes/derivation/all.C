{

    gROOT->ProcessLine(".L ../../misc/common_utils.h");
    gROOT->ProcessLine(".L make1DplotFR.C");
    gROOT->ProcessLine(".L plotEWKCorFR.C");
    gROOT->ProcessLine(".L plotFR.C");
    gROOT->ProcessLine(".L dumpFunctions.C");

    // // nominal
    // TString dir = "plotting/histos_nominal/";
    // float mt_sf_el_iso =    1.0302;
    // float mt_sf_el_noniso = 1.0375;
    // float mt_sf_mu_iso =    0.9956;
    // float mt_sf_mu_noniso = 0.9976;

    // TString dir = "outputs_ptthresh/";
    // float mt_sf_el_iso =    1.070;
    // float mt_sf_el_noniso = 1.070;
    // float mt_sf_mu_iso =    0.9956;
    // float mt_sf_mu_noniso = 0.9976;

    // TString dir = "outputs_nmissleq1/";
    // float mt_sf_el_iso =    0.993;
    // float mt_sf_el_noniso = 0.993;
    // float mt_sf_mu_iso =    0.995;
    // float mt_sf_mu_noniso = 0.995;

    // TString dir = "outputs_extrapt/";
    // float mt_sf_el_iso =    0.993;
    // float mt_sf_el_noniso = 0.993;
    // float mt_sf_mu_iso =    0.995;
    // float mt_sf_mu_noniso = 0.995;

    // TString dir = "outputs_latestid/";
    // TString dir = "outputs_extrapt/";
    // float mt_sf_el_iso =    1.219;
    // float mt_sf_el_noniso = 1.221;
    // float mt_sf_mu_iso =    1.178;
    // float mt_sf_mu_noniso = 1.182;

    // TString dir = "outputs/";
    // float mt_sf_el_iso =    1.0;
    // float mt_sf_el_noniso = 1.0;
    // float mt_sf_mu_iso =    1.0;
    // float mt_sf_mu_noniso = 1.0;

    // TString dir = "outputs/";
    // float mt_sf_el_iso =    1.268;
    // float mt_sf_el_noniso = 1.270;
    // float mt_sf_mu_iso =    1.112;
    // float mt_sf_mu_noniso = 1.117;

    // TString dir = "outputs_2017_Oct31/";
    // float mt_sf_el_iso =    1.332;
    // float mt_sf_el_noniso = 1.338;
    // float mt_sf_mu_iso =    1.204;
    // float mt_sf_mu_noniso = 1.203;
    // int year = 2017;


// TString dir = "v25jec6/outputs_SS_2017/";
// float mt_sf_el_iso =    1.024;
// float mt_sf_el_noniso = 1.021;
// float mt_sf_mu_iso =    1.216;
// float mt_sf_mu_noniso = 1.207;
// int year = 2017;

// TString dir = "v25jec32/outputs_SS_2017/";
// float mt_sf_el_iso =    1.007;
// float mt_sf_el_noniso = 1.013;
// float mt_sf_mu_iso =    1.214;
// float mt_sf_mu_noniso = 1.197;
// int year = 2017;

// TString dir = "outputs_FT_2018/";
// float mt_sf_el_iso =    1.082;
// float mt_sf_el_noniso = 0.664;
// float mt_sf_mu_iso =    0.904;
// float mt_sf_mu_noniso = 0.561;
// int year = 2018;

// TString dir = "outputs_FT_2017/";
// float mt_sf_el_iso =    1.013;
// float mt_sf_el_noniso = 1.018;
// float mt_sf_mu_iso =    1.208;
// float mt_sf_mu_noniso = 1.208;
// int year = 2017;

    // // 2018 with 2017 MC
    // TString dir("outputs_2018_Oct31/");
    // float mt_sf_el_iso =    1.084;
    // float mt_sf_el_noniso = 1.076;
    // float mt_sf_mu_iso =    0.904;
    // float mt_sf_mu_noniso = 0.912;
    // int year = 2018;

    TString dir = "outputs_test/";
    float mt_sf_el_iso =    1.098;
    float mt_sf_el_noniso = 1.100;
    float mt_sf_mu_iso =    1.220;
    float mt_sf_mu_noniso = 1.203;
    int year = 2017;

    plotFR(dir);
    make1DplotFR(dir, 1.0, 1.0, mt_sf_el_noniso, mt_sf_mu_noniso, false, year);
    plotEWKCorFR(dir, 1.0, 1.0, mt_sf_el_noniso, mt_sf_mu_noniso, false, year);
    make1DplotFR(dir, 1.0, 1.0, mt_sf_el_iso, mt_sf_mu_iso, true, year);
    plotEWKCorFR(dir, 1.0, 1.0, mt_sf_el_iso, mt_sf_mu_iso, true, year);
    // dumpFunctions(dir);

    gSystem->Exec("cp -r pdfs/ "+dir);
    gSystem->Exec("cp ewkCorFR_*.root "+dir);
    gSystem->Exec("cp hists_1DFR*.root "+dir);

}
