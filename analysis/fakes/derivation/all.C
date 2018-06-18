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

    TString dir = "outputs/";
    float mt_sf_el_iso =    1.377;
    float mt_sf_el_noniso = 1.381;
    float mt_sf_mu_iso =    1.209;
    float mt_sf_mu_noniso = 1.209;


    plotFR(dir);

    make1DplotFR(dir, 1.0, 1.0, mt_sf_el_noniso, mt_sf_mu_noniso, false);
    plotEWKCorFR(dir, 1.0, 1.0, mt_sf_el_noniso, mt_sf_mu_noniso, false);
    make1DplotFR(dir, 1.0, 1.0, mt_sf_el_iso, mt_sf_mu_iso, true);
    plotEWKCorFR(dir, 1.0, 1.0, mt_sf_el_iso, mt_sf_mu_iso, true);
    dumpFunctions(dir);

}
