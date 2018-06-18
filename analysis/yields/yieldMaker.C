#include <bitset>
#include <iostream>
#include <algorithm>
#include "TChain.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TKey.h"
#include "TSystem.h"
#include "TMVA/Reader.h"
#include "../misc/class_files/v8.02/SS.h"
#include "../../common/CORE/SSSelections.h"
#include "../../common/CORE/Tools/dorky/dorky.h"
#include "../../common/CORE/Tools/utils.h"
#include "../misc/common_utils.h"
#include "../misc/signal_regions.h"
#include "xgbheader.h"
#include "c2numpy.h"
#define WRITE(var) { p_data.var->Write(); p_ttw.var->Write(); p_ttz.var->Write(); p_tth.var->Write(); p_xg.var->Write(); p_rares.var->Write(); p_flips.var->Write(); p_fakes.var->Write(); p_tttt.var->Write(); p_ttvv.var->Write(); p_fakes_mc.var->Write(); p_tightfake.var->Write(); p_extra.var->Write(); p_fakes_unw.var->Write(); p_fakes_mc_unw.var->Write(); }

float lumiAG = getLumiUnblind();

float bloose = 	0.1522;
float bmed = 0.4941;
float btight = 0.8001;


int nsr = getNsrsTTTT();
int nCR = 2;
// int nCR = 1;
int nsrdisc = 13;
// int nsrdisc = 19;
// int nsrdisc = 17;
// int nCR = 1;

bool doCustomSelection = false;
float scaleLumi = 1; // 75/35.87
// float scaleLumi = 1.869; // (35.9+41.3)/41.3 // FIXME
// float scaleLumi = 1;

bool doTTWISR = true;
bool doTTZISR = true;
bool doBDT = false;
bool outputTrainingBDT = false;
bool includeDataInBDTTree = false;
bool doISRFSRsyst = true;
bool doTTBB = true;
bool doJER = true;

bool makeRootFiles = true;
bool makeGenVariationsMC = true;

// TString dir = "v1.00_baseline";
// TString tag = "v1.00";
// TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mll/output/skim/";

// TString dir = "v1.00_baseline_caleb";
// TString tag = "v1.02_JetPtCut40";
// // TString tag = "v1.00";
// TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mll/output/skim/";

// float metcut = 50;
// bool doData = false;
// // TString dir = "v1.00_baseline_full_bdtcrz13bins";
// TString dir = "v1.00_baseline_full_resolvedtop_40_25";
// TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mll/output/skim/";

float metcut = 50;
bool doData = true;
// TString dir = "v1.00_baseline_full_bdtcrz13bins";
// /nfs-7/userdata/namin/tupler_babies/merged/FT/v1.00_80x_baseline_full_v2/output/

// // 20,20
// TString dir = "v1.00_baseline_full_resolvedtop_20_20";
// TString tag = "v1.0.4_bJetPtCut20JetPtCut20"; // from caleb's dir

// // 40,25
// TString dir = "v1.00_baseline_full_resolvedtop_40_25";
// TString tag = "v1.0.4_JetPtCut40"; // from caleb's dir

// // 40,25
// TString tag = "v1.03_os_nmiss1_dileptrig_v1";
// TString tag2 = "v1.00_80x_baseline_full_v2";
// TString dir = "v1.03_baseline";

// 40,25
TString tag = "v1.05_v1";
TString tag2 = "v1.00_80x_baseline_full_v2";
TString dir = "v1.05_baseline_v2_41p3_18bins";

// // 20,20
// TString tag = "v1.0.5_bJetPtCut20JetPtCut20";
// TString dir = "v1.00_baseline_2020_toptag_bdtout";

TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v1.05_v1/output/";
bool doTTTTisrfsr = true;
bool doRares = true;


// TString dir = "v0.10_sync";
bool doSync = false;

// // 30 gev jets
// bool doJER = true;
// TString dir = "v0.09_May30";
// TString tag = "v0.09"; // data is in v0.08
// TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.08/output/skim/";

bool suppressWarns = true;

c2numpy_writer writer;

// For output tree
float tree_weight = -1;
int tree_event = -1, tree_lumi = -1, tree_run = -1;
int tree_l1id = -1;
int tree_l2id = -1;
int tree_nbtags = -1;
int tree_njets = -1;
int tree_nleps = -1;
int tree_wcands = -1;
float tree_met = -1;
float tree_ht = -1;
float tree_htb = -1;
float tree_mt1 = -1;
float tree_mt2 = -1;
float tree_dphil1j1 = -1;
float tree_dphil2j2 = -1;
float tree_dphil1j2 = -1;
float tree_dphil2j1 = -1;
float tree_dphil1l2 = -1;
float tree_dphij1j2 = -1;
float tree_dphil1met = -1;
float tree_dphil2met = -1;
float tree_dphij1met = -1;
float tree_dphij2met = -1;
float tree_detal1j1 = -1;
float tree_detal2j2 = -1;
float tree_detal1j2 = -1;
float tree_detal2j1 = -1;
float tree_detal1l2 = -1;
float tree_detaj1j2 = -1;
float tree_ml1j1 = -1;
float tree_ml2j2 = -1;
float tree_ml1j2 = -1;
float tree_ml2j1 = -1;
float tree_ml1l2 = -1;
float tree_mj1j2 = -1;
float tree_ptl1 = -1;
float tree_ptl2 = -1;
float tree_ptl3 = -1;
float tree_ptj1 = -1;
float tree_ptj2 = -1;
float tree_ptj3 = -1;
float tree_ptj4 = -1;
float tree_ptj5 = -1;
float tree_ptj6 = -1;
float tree_ptj7 = -1;
float tree_ptj8 = -1;
float tree_maxmjoverpt = -1;
// float tree_stb = -1;
int tree_nlb40 = -1;
int tree_nmb40 = -1;
int tree_ntb40 = -1;
int tree_q1 = 0;
int tree_q2 = 0;
int tree_q3 = 0;
float tree_ht4ratio = 0;
int tree_SR = -1;
float tree_disc = -1;

float tree_topdisc1 = -1;
float tree_topdisc2 = -1;

float tree_f_njets = -1.;
float tree_f_nbtags = -1.;
float tree_f_nleps = -1.;
float tree_f_wcands = -1.;
float tree_f_nlb40 = -1.;
float tree_f_nmb40 = -1.;
float tree_f_ntb40 = -1.;
float tree_f_q1 = -1.;
float tree_f_q2 = -1.;

TString tree_name = "";
int tree_stype = -1;
TFile *out_file;
TTree *out_tree;

struct yields_t { float EE; float EM; float MM; float TOTAL; };
struct SR_t     { TH1F* EE; TH1F* EM; TH1F* MM; TH1F* TOTAL; };
struct triple_t { TH1F* ttzcr; TH1F* ttwcr; TH1F* sr; TH1F* br; };
struct plots_t  {
    SR_t SRCR;
    SR_t SRDISC;
    SR_t SR;
    triple_t h_ht;
    triple_t h_met;
    triple_t h_metnm1;
    triple_t h_mll;
    triple_t h_mllos;
    triple_t h_mtmin;
    triple_t h_mt1;
    triple_t h_mt2;
    triple_t h_njets;
    triple_t h_nbtags;
    triple_t h_l1pt;
    triple_t h_l2pt;
    triple_t h_l3pt;
    triple_t h_type;
    triple_t h_nvtx;
    triple_t h_charge;
    triple_t h_charge3;
    triple_t h_nleps;
    triple_t h_mu_l1pt;
    triple_t h_el_l1pt;
    triple_t h_mu_l2pt;
    triple_t h_mu_l3pt;
    triple_t h_el_l2pt;
    triple_t h_el_l3pt;
    triple_t h_el_nmiss;
    triple_t h_disc;
    triple_t h_disc2;
    triple_t h_mid1;
    triple_t h_mid2;
    triple_t h_mid3;
    triple_t h_mvis;
    triple_t h_mtvis;
    triple_t h_ntops;
    triple_t h_mtop1;
    triple_t h_mtop2;
    triple_t h_topdisc1;
    triple_t h_topdisc2;
    triple_t h_ntopness;
    triple_t h_bjetpt;
    triple_t h_jetpt;
    triple_t h_mva;
    triple_t h_mu_sip3d_lep1;
    triple_t h_mu_l1eta;
    triple_t h_mu_lep1_miniIso;
    triple_t h_mu_lep1_ptRel;
    triple_t h_mu_lep1_ptRelfail;
    triple_t h_mu_lep1_ptRatio;
    triple_t h_el_sip3d_lep1;
    triple_t h_el_l1eta;
    triple_t h_el_lep1_miniIso;
    triple_t h_el_lep1_ptRel;
    triple_t h_el_lep1_ptRelfail;
    triple_t h_el_lep1_ptRatio;
    triple_t h_mu_sip3d_lep2;
    triple_t h_mu_l2eta;
    triple_t h_mu_lep2_miniIso;
    triple_t h_mu_lep2_ptRel;
    triple_t h_mu_lep2_ptRelfail;
    triple_t h_mu_lep2_ptRatio;
    triple_t h_el_sip3d_lep2;
    triple_t h_el_l2eta;
    triple_t h_el_lep2_miniIso;
    triple_t h_el_lep2_ptRel;
    triple_t h_el_lep2_ptRelfail;
    triple_t h_el_lep2_ptRatio;
    triple_t h_mu_sip3d_lep3;
    triple_t h_mu_l3eta;
    triple_t h_mu_lep3_miniIso;
    triple_t h_mu_lep3_ptRel;
    triple_t h_mu_lep3_ptRelfail;
    triple_t h_mu_lep3_ptRatio;
    triple_t h_el_sip3d_lep3;
    triple_t h_el_l3eta;
    triple_t h_el_lep3_miniIso;
    triple_t h_el_lep3_ptRel;
    triple_t h_el_lep3_ptRelfail;
    triple_t h_el_lep3_ptRatio;
};


//Total
yields_t total;

//function declaration
pair<yields_t, plots_t> run(TChain *chain, bool isData = 0, bool doFlips = 0, int doFakes = 0, int exclude = 0, bool isSignal = 0, bool isGamma = 0, bool tightfake = 0, float yt = 0., bool ignoreFakeFactor = 0);
void avoidNegativeYields(TH1F* plot);
void avoidZeroIntegrals(TH1F* central,TH1F* up,TH1F* down);
void fillDownMirrorUp(TH1F* central,TH1F* up,TH1F* down);
void writeStatUpDown(TH1F* central,string name,bool down);
void writeStat(TH1F* central,string name);
void writeHTHltSyst(TH1F* central,string name,TString kine);
void initHistError(bool usePoisson, TH1F* plot);
std::pair<int,float> getNtops();
std::pair<float,float> getFirstTwoTops();
std::pair<float,float> getTrijetDiscs(TMVA::Reader* reader);
int getBDTBin(float disc, bool isClass6);


void getyields(){

    c2numpy_init(&writer, "testout", 10000000);
    c2numpy_addcolumn(&writer, "one", C2NUMPY_INTC);
    c2numpy_addcolumn(&writer, "two", C2NUMPY_FLOAT64);
    c2numpy_addcolumn(&writer, "three", (c2numpy_type)((int)C2NUMPY_STRING + 5));
    c2numpy_open(&writer);

    if (outputTrainingBDT) {
        out_file = new TFile("output.root", "RECREATE");
        out_file->cd();
        out_tree = new TTree("t", "From yieldMaker");
        out_tree->Branch("event" , &tree_event );
        out_tree->Branch("lumi" , &tree_lumi );
        out_tree->Branch("run" , &tree_run );
        out_tree->Branch("weight" , &tree_weight );
        out_tree->Branch("name" , &tree_name );
        out_tree->Branch("stype" , &tree_stype );
        out_tree->Branch("SR" , &tree_SR );
        out_tree->Branch("l1id", &tree_l1id );
        out_tree->Branch("l2id", &tree_l2id );
        out_tree->Branch("nbtags", &tree_nbtags );
        out_tree->Branch("njets", &tree_njets );
        out_tree->Branch("nleps", &tree_nleps );
        out_tree->Branch("wcands", &tree_wcands );
        out_tree->Branch("met", &tree_met );
        out_tree->Branch("ht", &tree_ht );
        out_tree->Branch("htb", &tree_htb );
        out_tree->Branch("mt1", &tree_mt1 );
        out_tree->Branch("mt2", &tree_mt2 );
        out_tree->Branch("dphil1j1", &tree_dphil1j1 );
        out_tree->Branch("dphil2j2", &tree_dphil2j2 );
        out_tree->Branch("dphil1j2", &tree_dphil1j2 );
        out_tree->Branch("dphil2j1", &tree_dphil2j1 );
        out_tree->Branch("dphil1l2", &tree_dphil1l2 );
        out_tree->Branch("dphij1j2", &tree_dphij1j2 );
        out_tree->Branch("dphil1met", &tree_dphil1met );
        out_tree->Branch("dphil2met", &tree_dphil2met );
        out_tree->Branch("dphij1met", &tree_dphij1met );
        out_tree->Branch("dphij2met", &tree_dphij2met );
        out_tree->Branch("detal1j1", &tree_detal1j1 );
        out_tree->Branch("detal2j2", &tree_detal2j2 );
        out_tree->Branch("detal1j2", &tree_detal1j2 );
        out_tree->Branch("detal2j1", &tree_detal2j1 );
        out_tree->Branch("detal1l2", &tree_detal1l2 );
        out_tree->Branch("detaj1j2", &tree_detaj1j2 );
        out_tree->Branch("ml1j1", &tree_ml1j1 );
        out_tree->Branch("ml2j2", &tree_ml2j2 );
        out_tree->Branch("ml1j2", &tree_ml1j2 );
        out_tree->Branch("ml2j1", &tree_ml2j1 );
        out_tree->Branch("ml1l2", &tree_ml1l2 );
        out_tree->Branch("mj1j2", &tree_mj1j2 );
        out_tree->Branch("ptl1", &tree_ptl1 );
        out_tree->Branch("ptl2", &tree_ptl2 );
        out_tree->Branch("ptl3", &tree_ptl3 );
        out_tree->Branch("ptj1", &tree_ptj1 );
        out_tree->Branch("ptj2", &tree_ptj2 );
        out_tree->Branch("ptj3", &tree_ptj3 );
        out_tree->Branch("ptj4", &tree_ptj4 );
        out_tree->Branch("ptj5", &tree_ptj5 );
        out_tree->Branch("ptj6", &tree_ptj6 );
        out_tree->Branch("ptj7", &tree_ptj7 );
        out_tree->Branch("ptj8", &tree_ptj8 );
        out_tree->Branch("nlb40", &tree_nlb40 );
        out_tree->Branch("nmb40", &tree_nmb40 );
        out_tree->Branch("ntb40", &tree_ntb40 );
        out_tree->Branch("topdisc1", &tree_topdisc1 );
        out_tree->Branch("topdisc2", &tree_topdisc2 );
        out_tree->Branch("q1", &tree_q1 );
        out_tree->Branch("q2", &tree_q2 );
        out_tree->Branch("q3", &tree_q3 );
        out_tree->Branch("ht4ratio", &tree_ht4ratio );
        out_tree->Branch("maxmjoverpt", &tree_maxmjoverpt );
        out_tree->Branch("disc", &tree_disc );
    }


    gSystem->Exec(Form("mkdir -p ../limits/%s/", dir.Data()));

    cout << "Running with lumi=" << lumiAG*scaleLumi << endl;

    //Chains
    //fakes&flips in mc
    TChain *ttbar_chain   = new TChain("t","ttbar");
    TChain *st_chain      = new TChain("t","st");
    // TChain *wjets_chain   = new TChain("t","wjets");
    // TChain *dy_chain      = new TChain("t","dy");
    //rares
    TChain *ttw_chain     = new TChain("t","ttw");
    TChain *ttz_chain    = new TChain("t","ttz");
    TChain *tth_chain    = new TChain("t","tth");
    TChain *xg_chain      = new TChain("t","xg");
    TChain *rares_chain   = new TChain("t","rares");
    TChain *ttttisrup_chain    = new TChain("t","ttttisrup");
    TChain *ttttfsrup_chain    = new TChain("t","ttttfsrup");
    TChain *ttttisrdn_chain    = new TChain("t","ttttisrdn");
    TChain *ttttfsrdn_chain    = new TChain("t","ttttfsrdn");
    TChain *tttt_chain    = new TChain("t","tttt");
    TChain *ttvv_chain    = new TChain("t","ttvv");
    // TChain *qqww_chain    = new TChain("t","qqww"); // separate this out to check shape uncertainties
    //data
    TChain *data_chain    = new TChain("t","data");
    TChain *flips_chain   = new TChain("t","flips");
    TChain *fakes_chain   = new TChain("t","fakes");
    TChain *tightfake_chain   = new TChain("t","tightfake");
    TChain *extra_chain   = new TChain("t","extra");
    //signals full sim
    
  // #include "yt_scan.h"
  
  // #include "higgs_scan_v2.h" // fullsim
  // #include "higgs_scan_ps_v2.h"

  // #include "higgs_scan.h" // fastsim
  // #include "higgs_scan_ps.h"

    // TString pfx  = Form("/nfs-7/userdata/cfangmei/tupler_babies/merged/FT/%s//output/",tag.Data());
    TString pfx  = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s//output/",tag.Data());
    TString pfx2  = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s//output/",tag2.Data());
    // TString pfx  = Form("/nfs-7/userdata/cfangmei/tupler_babies/merged/FT/%s//output/",tag.Data());

    //Fill chains
    tttt_chain   ->Add(Form("%s/TTTTnew.root"           , pfx.Data()));

    if (doTTTTisrfsr) {
        ttttisrup_chain   ->Add(Form("%s/TTTTisrup.root"           , pfx2.Data()));
        ttttfsrup_chain   ->Add(Form("%s/TTTTfsrup.root"           , pfx2.Data()));
        ttttisrdn_chain   ->Add(Form("%s/TTTTisrdown.root"           , pfx2.Data()));
        ttttfsrdn_chain   ->Add(Form("%s/TTTTfsrdown.root"           , pfx2.Data()));
    }

    if (doSync) {
        ttw_chain    ->Add("/nfs-7/userdata/namin/tupler_babies/merged/test/FT/test_synch_btagcsv_v1/output/TTWnlo.root");
    } else {
        ttw_chain    ->Add(Form("%s/TTWnlo.root"            , pfx.Data()));
    }
    ttz_chain   ->Add(Form("%s/TTZnlo.root"           , pfx.Data()));
    ttz_chain   ->Add(Form("%s/TTZLOW.root"         , pfx.Data()));
    tth_chain   ->Add(Form("%s/TTHtoNonBB.root"     , pfx.Data()));
    ttbar_chain  ->Add(Form("%s/TTBAR_*.root"       , pfx.Data()));

    if (doRares) {
        ttvv_chain->Add(Form("%s/TTHH.root",pfx.Data()));
        ttvv_chain->Add(Form("%s/TTWH.root",pfx.Data()));
        ttvv_chain->Add(Form("%s/TTWW.root",pfx.Data()));
        ttvv_chain->Add(Form("%s/TTWZ.root",pfx.Data()));
        ttvv_chain->Add(Form("%s/TTZH.root",pfx2.Data()));
        ttvv_chain->Add(Form("%s/TTZZ.root",pfx.Data()));
        xg_chain     ->Add(Form("%s/TGext.root"             , pfx2.Data()));
        xg_chain     ->Add(Form("%s/WGToLNuGext.root"           , pfx2.Data()));
        xg_chain     ->Add(Form("%s/TTGdilep.root"             , pfx2.Data()));
        xg_chain     ->Add(Form("%s/TTGsinglelep.root"             , pfx2.Data()));
        xg_chain     ->Add(Form("%s/TTGsinglelepbar.root"             , pfx2.Data()));
        xg_chain     ->Add(Form("%s/ZG.root"             , pfx2.Data()));
        rares_chain  ->Add(Form("%s/ZZ.root"             , pfx.Data()));
        rares_chain  ->Add(Form("%s/GGHtoZZto4L.root"    , pfx2.Data()));
        rares_chain  ->Add(Form("%s/WWZ.root"            , pfx.Data()));
        rares_chain  ->Add(Form("%s/WZZ.root"            , pfx.Data()));
        rares_chain  ->Add(Form("%s/WWW.root"            , pfx.Data()));
        rares_chain  ->Add(Form("%s/WWG.root"            , pfx2.Data()));
        rares_chain  ->Add(Form("%s/WZG.root"            , pfx.Data()));
        rares_chain  ->Add(Form("%s/VHtoNonBB.root"      , pfx2.Data()));
        rares_chain  ->Add(Form("%s/TZQ.root"            , pfx.Data()));
        rares_chain  ->Add(Form("%s/TWZ.root"            , pfx2.Data()));
        rares_chain  ->Add(Form("%s/WWDPS.root"          , pfx.Data()));
        rares_chain     ->Add(Form("%s/WZ.root"             , pfx.Data()));
        rares_chain     ->Add(Form("%s/QQWW.root"           , pfx2.Data()));
        rares_chain->Add(Form("%s/TTTJ.root",pfx.Data()));
        rares_chain->Add(Form("%s/TTTW.root",pfx.Data()));
    }
    //data
    
    if (doData) {
        data_chain   ->Add(Form("%s/DataDoubleMuon*.root"    , pfxData.Data()));
        data_chain   ->Add(Form("%s/DataDoubleEG*.root"  , pfxData.Data()));
        data_chain   ->Add(Form("%s/DataMuonEG*.root"      , pfxData.Data()));
    }

    // //flips
    flips_chain  ->Add(Form("%s/DataMuonEG*.root"     , pfxData.Data()));
    flips_chain  ->Add(Form("%s/DataDoubleEG*.root"      , pfxData.Data()));
    // //fakes
    fakes_chain  ->Add(Form("%s/DataDoubleMuon*.root"    , pfxData.Data()));
    fakes_chain  ->Add(Form("%s/DataDoubleEG*.root"  , pfxData.Data()));
    fakes_chain  ->Add(Form("%s/DataMuonEG*.root"      , pfxData.Data()));
    fakes_chain  ->Add(Form("%s/TTWnlo.root"                   , pfx.Data()));
    fakes_chain  ->Add(Form("%s/TTZnlo.root"                  , pfx.Data()));
    fakes_chain  ->Add(Form("%s/TTHtoNonBB.root"            , pfx.Data()));
    if (doRares) {
        fakes_chain  ->Add(Form("%s/WZ.root"                    , pfx.Data()));
        fakes_chain  ->Add(Form("%s/QQWW.root"                  , pfx2.Data()));
    }
    //tightfake
    tightfake_chain  ->Add(Form("%s/TTHtoNonBB.root"                   , pfx.Data()));
    tightfake_chain  ->Add(Form("%s/TTWnlo.root"                   , pfx.Data()));
    tightfake_chain  ->Add(Form("%s/TTZnlo.root"                  , pfx.Data()));
    tightfake_chain  ->Add(Form("%s/TTBAR_*.root"            , pfx.Data()));
    
    extra_chain  ->Add(Form("%s/TWZ.root"            , pfx2.Data()));

    pair<yields_t, plots_t> results_extra       = run(extra_chain);
    pair<yields_t, plots_t> results_tightfake       = run(tightfake_chain, 0, 0, 0, 0, 0, 0, 1);
    pair<yields_t, plots_t> results_ttw      = run(ttw_chain);
    pair<yields_t, plots_t> results_ttz     = run(ttz_chain);
    pair<yields_t, plots_t> results_tth     = run(tth_chain);
    // pair<yields_t, plots_t> results_qqww     = run(qqww_chain);
    pair<yields_t, plots_t> results_xg       = run(xg_chain, 0, 0, 0, 0, 0, 1);
    pair<yields_t, plots_t> results_rares    = run(rares_chain);
    pair<yields_t, plots_t> results_ttttisrup     = run(ttttisrup_chain);
    pair<yields_t, plots_t> results_ttttfsrup     = run(ttttfsrup_chain);
    pair<yields_t, plots_t> results_ttttisrdn     = run(ttttisrdn_chain);
    pair<yields_t, plots_t> results_ttttfsrdn     = run(ttttfsrdn_chain);
    pair<yields_t, plots_t> results_tttt     = run(tttt_chain); // [!] run after the variations
    pair<yields_t, plots_t> results_ttvv     = run(ttvv_chain);
    pair<yields_t, plots_t> results_data     = run(data_chain, 1);
    duplicate_removal::clear_list();
    pair<yields_t, plots_t> results_flips    = run(flips_chain, 1, 1);

    ttbar_chain->SetTitle("fakes_mc");
    pair<yields_t, plots_t> results_fakes_mc = run(ttbar_chain, 0, 0, 1);
    duplicate_removal::clear_list();
    pair<yields_t, plots_t> results_fakes    = run(fakes_chain, 1, 0, 1);
    duplicate_removal::clear_list();

    ttbar_chain->SetTitle("fakes_mc_unw");
    pair<yields_t, plots_t> results_fakes_mc_unw = run(ttbar_chain, 0, 0, 1, 0, 0, 0, 0, 0.0, 1);
    duplicate_removal::clear_list();
    fakes_chain->SetTitle("fakes_unw");
    pair<yields_t, plots_t> results_fakes_unw    = run(fakes_chain, 1, 0, 1, 0, 0, 0, 0, 0.0, 1);
    duplicate_removal::clear_list();

    if (outputTrainingBDT) {
        // Write output tree
        out_file->cd();
        out_tree->Write();
        out_file->Close();
    }

    plots_t& p_tttt       = results_tttt.second;
    plots_t& p_ttvv       = results_ttvv.second;

    plots_t& p_ttw      = results_ttw.second;
    plots_t& p_ttz     = results_ttz.second;
    plots_t& p_tth     = results_tth.second;
    plots_t& p_xg       = results_xg.second;
    plots_t& p_rares    = results_rares.second;
    plots_t& p_data     = results_data.second;
    plots_t& p_flips    = results_flips.second;
    plots_t& p_fakes    = results_fakes.second;
    plots_t& p_fakes_mc = results_fakes_mc.second;
    plots_t& p_fakes_unw    = results_fakes_unw.second;
    plots_t& p_fakes_mc_unw = results_fakes_mc_unw.second;
    plots_t& p_tightfake = results_tightfake.second;
    plots_t& p_extra = results_extra.second;

    std::cout << "Writing out histos.root" << std::endl;
    TFile *fout = new TFile("histos.root", "RECREATE");

    WRITE(h_ht.sr)      ; WRITE(h_ht.ttzcr)      ; WRITE(h_ht.ttwcr)      ; WRITE(h_ht.br)      ;
    WRITE(h_met.sr)     ; WRITE(h_met.ttzcr)     ; WRITE(h_met.ttwcr)     ; WRITE(h_met.br)     ;
    WRITE(h_mll.sr)     ; WRITE(h_mll.ttzcr)     ; WRITE(h_mll.ttwcr)     ; WRITE(h_mll.br)     ;
    WRITE(h_mtmin.sr)   ; WRITE(h_mtmin.ttzcr)   ; WRITE(h_mtmin.ttwcr)   ; WRITE(h_mtmin.br)   ;
    WRITE(h_njets.sr)   ; WRITE(h_njets.ttzcr)   ; WRITE(h_njets.ttwcr)   ; WRITE(h_njets.br)   ;
    WRITE(h_nleps.sr)   ; WRITE(h_nleps.ttzcr)   ; WRITE(h_nleps.ttwcr)   ; WRITE(h_nleps.br)   ;
    WRITE(h_nbtags.sr)  ; WRITE(h_nbtags.ttzcr)  ; WRITE(h_nbtags.ttwcr)  ; WRITE(h_nbtags.br)  ;
    WRITE(h_type.sr)    ; WRITE(h_type.ttzcr)    ; WRITE(h_type.ttwcr)    ; WRITE(h_type.br)    ;
    WRITE(h_charge.sr)  ; WRITE(h_charge.ttzcr)  ; WRITE(h_charge.ttwcr)  ; WRITE(h_charge.br)  ;
    WRITE(h_l1pt.sr)    ; WRITE(h_l1pt.ttzcr)    ; WRITE(h_l1pt.ttwcr)    ; WRITE(h_l1pt.br)    ;
    WRITE(h_l2pt.sr)    ; WRITE(h_l2pt.ttzcr)    ; WRITE(h_l2pt.ttwcr)    ; WRITE(h_l2pt.br)    ;
    WRITE(h_l3pt.sr)    ; WRITE(h_l3pt.ttzcr)    ; WRITE(h_l3pt.ttwcr)    ; WRITE(h_l3pt.br)    ;
    WRITE(h_el_l1pt.sr) ; WRITE(h_el_l1pt.ttzcr) ; WRITE(h_el_l1pt.ttwcr) ; WRITE(h_el_l1pt.br) ;
    WRITE(h_el_l2pt.sr) ; WRITE(h_el_l2pt.ttzcr) ; WRITE(h_el_l2pt.ttwcr) ; WRITE(h_el_l2pt.br) ;
    WRITE(h_el_l3pt.sr) ; WRITE(h_el_l3pt.ttzcr) ; WRITE(h_el_l3pt.ttwcr) ; WRITE(h_el_l3pt.br) ;
    WRITE(h_mu_l1pt.sr) ; WRITE(h_mu_l1pt.ttzcr) ; WRITE(h_mu_l1pt.ttwcr) ; WRITE(h_mu_l1pt.br) ;
    WRITE(h_mu_l2pt.sr) ; WRITE(h_mu_l2pt.ttzcr) ; WRITE(h_mu_l2pt.ttwcr) ; WRITE(h_mu_l2pt.br) ;
    WRITE(h_mu_l3pt.sr) ; WRITE(h_mu_l3pt.ttzcr) ; WRITE(h_mu_l3pt.ttwcr) ; WRITE(h_mu_l3pt.br) ;
    WRITE(h_mvis.sr)    ; WRITE(h_mvis.ttzcr)    ; WRITE(h_mvis.ttwcr)    ; WRITE(h_mvis.br)    ;
    WRITE(h_mtvis.sr)  ; WRITE(h_mtvis.ttzcr)  ; WRITE(h_mtvis.ttwcr)  ; WRITE(h_mtvis.br)  ;
    WRITE(h_ntops.sr)  ; 
    WRITE(h_mtop1.sr)  ; 
    WRITE(h_mtop2.sr)  ; 
    WRITE(h_topdisc1.sr)  ; 
    WRITE(h_topdisc2.sr)  ; 
    WRITE(h_ntopness.sr)  ; 
    WRITE(h_nvtx.br)  ; 

    WRITE(h_bjetpt.br)  ; WRITE(h_bjetpt.sr)  ; 
    WRITE(h_jetpt.br)  ; WRITE(h_jetpt.sr)  ; 
    WRITE(h_el_nmiss.br)  ;

    WRITE(h_mva.br)               ;   WRITE(h_mva.sr);
    WRITE(h_mu_sip3d_lep1.br)     ;   WRITE(h_mu_sip3d_lep1.sr);
    WRITE(h_mu_sip3d_lep2.br)     ;   WRITE(h_mu_sip3d_lep2.sr);
    WRITE(h_mu_sip3d_lep3.br)     ;   WRITE(h_mu_sip3d_lep3.sr);
    WRITE(h_el_sip3d_lep1.br)     ;   WRITE(h_el_sip3d_lep1.sr);
    WRITE(h_el_sip3d_lep2.br)     ;   WRITE(h_el_sip3d_lep2.sr);
    WRITE(h_el_sip3d_lep3.br)     ;   WRITE(h_el_sip3d_lep3.sr);
    WRITE(h_mu_l1eta.br)          ;   WRITE(h_mu_l1eta.sr);
    WRITE(h_mu_l2eta.br)          ;   WRITE(h_mu_l2eta.sr);
    WRITE(h_mu_l3eta.br)          ;   WRITE(h_mu_l3eta.sr);
    WRITE(h_el_l1eta.br)          ;   WRITE(h_el_l1eta.sr);
    WRITE(h_el_l2eta.br)          ;   WRITE(h_el_l2eta.sr);
    WRITE(h_el_l3eta.br)          ;   WRITE(h_el_l3eta.sr);
    WRITE(h_mu_lep1_miniIso.br)   ;   WRITE(h_mu_lep1_miniIso.sr);
    WRITE(h_mu_lep2_miniIso.br)   ;   WRITE(h_mu_lep2_miniIso.sr);
    WRITE(h_mu_lep3_miniIso.br)   ;   WRITE(h_mu_lep3_miniIso.sr);
    WRITE(h_mu_lep1_ptRatio.br)   ;   WRITE(h_mu_lep1_ptRatio.sr);
    WRITE(h_mu_lep2_ptRatio.br)   ;   WRITE(h_mu_lep2_ptRatio.sr);
    WRITE(h_mu_lep3_ptRatio.br)   ;   WRITE(h_mu_lep3_ptRatio.sr);
    WRITE(h_mu_lep1_ptRel.br)     ;   WRITE(h_mu_lep1_ptRel.sr);
    WRITE(h_mu_lep2_ptRel.br)     ;   WRITE(h_mu_lep2_ptRel.sr);
    WRITE(h_mu_lep3_ptRel.br)     ;   WRITE(h_mu_lep3_ptRel.sr);
    WRITE(h_mu_lep1_ptRelfail.br)     ;   WRITE(h_mu_lep1_ptRelfail.sr);
    WRITE(h_mu_lep2_ptRelfail.br)     ;   WRITE(h_mu_lep2_ptRelfail.sr);
    WRITE(h_mu_lep3_ptRelfail.br)     ;   WRITE(h_mu_lep3_ptRelfail.sr);
    WRITE(h_el_lep1_miniIso.br)   ;   WRITE(h_el_lep1_miniIso.sr);
    WRITE(h_el_lep2_miniIso.br)   ;   WRITE(h_el_lep2_miniIso.sr);
    WRITE(h_el_lep3_miniIso.br)   ;   WRITE(h_el_lep3_miniIso.sr);
    WRITE(h_el_lep1_ptRatio.br)   ;   WRITE(h_el_lep1_ptRatio.sr);
    WRITE(h_el_lep2_ptRatio.br)   ;   WRITE(h_el_lep2_ptRatio.sr);
    WRITE(h_el_lep3_ptRatio.br)   ;   WRITE(h_el_lep3_ptRatio.sr);
    WRITE(h_el_lep1_ptRel.br)     ;   WRITE(h_el_lep1_ptRel.sr);
    WRITE(h_el_lep2_ptRel.br)     ;   WRITE(h_el_lep2_ptRel.sr);
    WRITE(h_el_lep3_ptRel.br)     ;   WRITE(h_el_lep3_ptRel.sr);
    WRITE(h_el_lep1_ptRelfail.br)     ;   WRITE(h_el_lep1_ptRelfail.sr);
    WRITE(h_el_lep2_ptRelfail.br)     ;   WRITE(h_el_lep2_ptRelfail.sr);
    WRITE(h_el_lep3_ptRelfail.br)     ;   WRITE(h_el_lep3_ptRelfail.sr);

    WRITE(h_metnm1.br)     ;

    WRITE(h_mid1.br);
    WRITE(h_mid2.br);
    WRITE(h_mid3.br);

    WRITE(h_charge3.sr);

     WRITE(h_mllos.ttzcr)     ; 

    WRITE(h_disc.br);
    // WRITE(h_disc2.br);
    WRITE(SRCR.TOTAL);
    WRITE(SRDISC.TOTAL);
    WRITE(SR.TOTAL);

    TNamed metadata(TString("metadata"),dir);
    metadata.Write();

    c2numpy_close(&writer);

    // fout->Write();
    fout->Close();
    std::cout << "Done writing out histos.root" << std::endl;


    // copy the root file with kinematic plots to live with the cards
    gSystem->Exec(Form("cp histos.root ../limits/%s/", dir.Data()));


}

//doFakes = 1 for QCD, 2 for inSitu
pair<yields_t, plots_t> run(TChain *chain, bool isData, bool doFlips, int doFakes, int exclude, bool isSignal, bool isGamma, bool tightfake, float yt, bool ignoreFakeFactor){


    TMVA::Reader* reader = new TMVA::Reader("Silent");

    // 19
    reader->AddVariable("nbtags",&tree_f_nbtags);
    reader->AddVariable("njets",&tree_f_njets);
    reader->AddVariable("met",&tree_met);
    reader->AddVariable("ptl2",&tree_ptl2);
    reader->AddVariable("nlb40",&tree_f_nlb40);
    reader->AddVariable("ntb40",&tree_f_ntb40);
    reader->AddVariable("nleps",&tree_f_nleps);
    reader->AddVariable("htb",&tree_htb);
    reader->AddVariable("q1",&tree_f_q1);
    reader->AddVariable("ptj1",&tree_ptj1);
    reader->AddVariable("ptj6",&tree_ptj6);
    reader->AddVariable("ptj7",&tree_ptj7);
    reader->AddVariable("ml1j1",&tree_ml1j1);
    reader->AddVariable("dphil1l2",&tree_dphil1l2);
    reader->AddVariable("maxmjoverpt",&tree_maxmjoverpt);
    reader->AddVariable("ptl1",&tree_ptl1);
    reader->AddVariable("detal1l2",&tree_detal1l2);
    reader->AddVariable("ptj8",&tree_ptj8);
    reader->AddVariable("ptl3",&tree_ptl3);

    reader->AddSpectator("weight",&tree_weight);
    reader->AddSpectator("ptl1",&tree_ptl1);
    reader->AddSpectator("ptl2",&tree_ptl2);
    reader->AddSpectator("SR",&tree_SR);

    // reader->BookMVA("BDT","TMVAClassification_BDT.weights.xml");
    // reader->BookMVA("BDT","TMVAClassification_BDT_500trees.weights.xml");
    reader->BookMVA("BDT","TMVAClassification_BDT_19vars.xml");
    // reader->BookMVA("BDT","TMVAClassification_BDT_bgttw.weights.xml");

    float dummy = 0;
    TMVA::Reader* reader_toptag = new TMVA::Reader("Silent");
    reader_toptag->AddVariable("btag", &dummy);
    reader_toptag->AddVariable("ThPtOverSumPt", &dummy);
    reader_toptag->AddVariable("AngleThWh", &dummy);
    reader_toptag->AddVariable("AngleThBh", &dummy);
    reader_toptag->AddVariable("HadrWmass", &dummy);
    reader_toptag->AddVariable("TopMass", &dummy);
    reader_toptag->BookMVA("BDT","JetCombTrainer_BDT.weights.xml");

    // out_file->cd();


    TString chainTitle = chain->GetTitle();
    const char* chainTitleCh = chainTitle.Data();
    std::cout << "Working on " << chainTitle << std::endl;

    // bool isWZ = (chainTitle=="wz");
    bool istttt = (chainTitle=="tttt");
    bool isttZ = (chainTitle=="ttz");
    bool istt = (chainTitle=="ttbar") || (chainTitle=="fakes_mc") || (chainTitle=="fakes_mc_unw");
    bool isttW = (chainTitle=="ttw");
    bool isttH = (chainTitle=="tth");
    bool isXgamma = (chainTitle=="xg");

    bool isHiggsScan = false;
    bool isHiggsPseudoscalar = false;
    vector<float> mysparms;
    if (isSignal && (
                chainTitle.Contains("higgs_scan") ||  chainTitle.Contains("higgs_ps_scan") 
                || chainTitle.Contains("tth_scan") || chainTitle.Contains("thq_scan") || chainTitle.Contains("thw_scan")
                || chainTitle.Contains("tta_scan") || chainTitle.Contains("taq_scan") || chainTitle.Contains("taw_scan")
                ) ) {
        isHiggsScan = true;
        isHiggsPseudoscalar = chainTitle.Contains("tta_scan") || chainTitle.Contains("taq_scan") || chainTitle.Contains("taw_scan") || chainTitle.Contains("higgs_ps_scan");
        TObjArray *tx = chainTitle.Tokenize("_");
        for (int i = 0; i < tx->GetEntries(); i++) {
            if (((TObjString *)(tx->At(i)))->String().BeginsWith("m")) {
                float asparm = ((TObjString *)(tx->At(i)))->String().ReplaceAll("m","").Atof();
                mysparms.push_back(asparm);
            }
        }
    }

        
    // Calculate and store the normalization factors for the ISR reweighting
    float isr_norm_dy = 1.;
    float isr_norm_tt = 1.;
    if ((isttZ || isttW) && (doTTWISR || doTTZISR)) {
        TH1D* htemp = new TH1D("htemp","htemp",1,0,1);;
        chain->Draw("0.5>>htemp","weight_isr_dy");
        isr_norm_dy = chain->GetEntries()/htemp->GetBinContent(1);
        htemp->Reset();
        chain->Draw("0.5>>htemp","weight_isr_tt");
        isr_norm_tt = chain->GetEntries()/htemp->GetBinContent(1);
    }


    yields_t y_result;
    plots_t  p_result;

    //Initialize
    y_result.EE    = 0;
    y_result.EM    = 0;
    y_result.MM    = 0;
    y_result.TOTAL = 0;


    p_result.h_ht.sr       = new TH1F(Form("sr_ht_%s"         , chainTitleCh) , Form("ht_%s"         , chainTitleCh) , 15      , 100   , 1600);
    p_result.h_met.sr      = new TH1F(Form("sr_met_%s"        , chainTitleCh) , Form("met_%s"        , chainTitleCh) , 15      , 0    , 600);
    p_result.h_mll.sr      = new TH1F(Form("sr_mll_%s"        , chainTitleCh) , Form("mll_%s"        , chainTitleCh) , 20      , 0    , 200);
    p_result.h_mtmin.sr    = new TH1F(Form("sr_mtmin_%s"      , chainTitleCh) , Form("mtmin_%s"      , chainTitleCh) , 10      , 0    , 200);
    p_result.h_njets.sr    = new TH1F(Form("sr_njets_%s"      , chainTitleCh) , Form("njets_%s"      , chainTitleCh) , 10      , 1.5  , 11.5);
    p_result.h_nleps.sr    = new TH1F(Form("sr_nleps_%s"      , chainTitleCh) , Form("nleps_%s"      , chainTitleCh) , 6       , 0    , 6);
    p_result.h_nbtags.sr   = new TH1F(Form("sr_nbtags_%s"     , chainTitleCh) , Form("nbtags_%s"     , chainTitleCh) , 7       , -0.5 , 6.5);
    p_result.h_type.sr     = new TH1F(Form("sr_type_%s"       , chainTitleCh) , Form("type_%s"       , chainTitleCh) , 4       , 0    , 4);
    p_result.h_charge.sr   = new TH1F(Form("sr_charge_%s"     , chainTitleCh) , Form("charge_%s"     , chainTitleCh) , 3       , -1   , 2);
    p_result.h_l1pt.sr     = new TH1F(Form("sr_l1pt_%s"       , chainTitleCh) , Form("l1pt_%s"       , chainTitleCh) , 15      , 0    , 150);
    p_result.h_l2pt.sr     = new TH1F(Form("sr_l2pt_%s"       , chainTitleCh) , Form("l2pt_%s"       , chainTitleCh) , 15      , 0    , 150);
    p_result.h_l3pt.sr     = new TH1F(Form("sr_l3pt_%s"       , chainTitleCh) , Form("l3pt_%s"       , chainTitleCh) , 7       , 0    , 140);
    p_result.h_el_l1pt.sr  = new TH1F(Form("sr_el_l1pt_%s"    , chainTitleCh) , Form("el_l1pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_el_l2pt.sr  = new TH1F(Form("sr_el_l2pt_%s"    , chainTitleCh) , Form("el_l2pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_el_l3pt.sr  = new TH1F(Form("sr_el_l3pt_%s"    , chainTitleCh) , Form("el_l3pt_%s"    , chainTitleCh) , 7       , 0    , 140);
    p_result.h_mu_l1pt.sr  = new TH1F(Form("sr_mu_l1pt_%s"    , chainTitleCh) , Form("mu_l1pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_mu_l2pt.sr  = new TH1F(Form("sr_mu_l2pt_%s"    , chainTitleCh) , Form("mu_l2pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_mu_l3pt.sr  = new TH1F(Form("sr_mu_l3pt_%s"    , chainTitleCh) , Form("mu_l3pt_%s"    , chainTitleCh) , 7       , 0    , 140);
    p_result.h_mvis.sr      = new TH1F(Form("sr_mvis_%s"        , chainTitleCh) , Form("mvis_%s"        , chainTitleCh) , 15      , 250    , 2750);
    p_result.h_mtvis.sr      = new TH1F(Form("sr_mtvis_%s"        , chainTitleCh) , Form("mtvis_%s"        , chainTitleCh) , 20, 300 , 2500);
    p_result.h_ntops.sr      = new TH1F(Form("sr_ntops_%s"        , chainTitleCh) , Form("ntops_%s"        , chainTitleCh) , 5      , -0.5    , 4.5);
    p_result.h_mtop1.sr      = new TH1F(Form("sr_mtop1_%s"        , chainTitleCh) , Form("mtop1_%s"        , chainTitleCh) , 15      , 100    , 250);
    p_result.h_mtop2.sr      = new TH1F(Form("sr_mtop2_%s"        , chainTitleCh) , Form("mtop2_%s"        , chainTitleCh) , 15      , 100    , 250);
    p_result.h_topdisc1.sr      = new TH1F(Form("sr_topdisc1_%s"        , chainTitleCh) , Form("topdisc1_%s"        , chainTitleCh) , 30      , -0.6    , 0.4);
    p_result.h_topdisc2.sr      = new TH1F(Form("sr_topdisc2_%s"        , chainTitleCh) , Form("topdisc2_%s"        , chainTitleCh) , 30      , -0.6    , 0.4);
    p_result.h_ntopness.sr      = new TH1F(Form("sr_ntopness_%s"        , chainTitleCh) , Form("ntopness_%s"        , chainTitleCh) , 15      , 0., 0.75);
    p_result.h_bjetpt.sr      = new TH1F(Form("sr_bjetpt_%s"        , chainTitleCh) , Form("bjetpt_%s"        , chainTitleCh) , 16      , 0., 200.);
    p_result.h_jetpt.sr      = new TH1F(Form("sr_jetpt_%s"        , chainTitleCh) , Form("jetpt_%s"        , chainTitleCh) , 20      , 0., 200.);

    p_result.h_ht.ttzcr       = new TH1F(Form("ttzcr_ht_%s"         , chainTitleCh) , Form("ht_%s"         , chainTitleCh) , 15      , 100   , 1600);
    p_result.h_met.ttzcr      = new TH1F(Form("ttzcr_met_%s"        , chainTitleCh) , Form("met_%s"        , chainTitleCh) , 15      , 0    , 600);
    p_result.h_mll.ttzcr      = new TH1F(Form("ttzcr_mll_%s"        , chainTitleCh) , Form("mll_%s"        , chainTitleCh) , 20      , 0    , 200);
    p_result.h_mllos.ttzcr      = new TH1F(Form("ttzcr_mllos_%s"        , chainTitleCh) , Form("mllos_%s"        , chainTitleCh) , 20      , 0    , 200);
    p_result.h_mtmin.ttzcr    = new TH1F(Form("ttzcr_mtmin_%s"      , chainTitleCh) , Form("mtmin_%s"      , chainTitleCh) , 10      , 0    , 200);
    p_result.h_njets.ttzcr    = new TH1F(Form("ttzcr_njets_%s"      , chainTitleCh) , Form("njets_%s"      , chainTitleCh) , 10      , 1.5  , 11.5);
    p_result.h_nleps.ttzcr    = new TH1F(Form("ttzcr_nleps_%s"      , chainTitleCh) , Form("nleps_%s"      , chainTitleCh) , 6       , 0    , 6);
    p_result.h_nbtags.ttzcr   = new TH1F(Form("ttzcr_nbtags_%s"     , chainTitleCh) , Form("nbtags_%s"     , chainTitleCh) , 7       , -0.5 , 6.5);
    p_result.h_type.ttzcr     = new TH1F(Form("ttzcr_type_%s"       , chainTitleCh) , Form("type_%s"       , chainTitleCh) , 4       , 0    , 4);
    p_result.h_charge.ttzcr   = new TH1F(Form("ttzcr_charge_%s"     , chainTitleCh) , Form("charge_%s"     , chainTitleCh) , 3       , -1   , 2);
    p_result.h_l1pt.ttzcr     = new TH1F(Form("ttzcr_l1pt_%s"       , chainTitleCh) , Form("l1pt_%s"       , chainTitleCh) , 15      , 0    , 150);
    p_result.h_l2pt.ttzcr     = new TH1F(Form("ttzcr_l2pt_%s"       , chainTitleCh) , Form("l2pt_%s"       , chainTitleCh) , 15      , 0    , 150);
    p_result.h_l3pt.ttzcr     = new TH1F(Form("ttzcr_l3pt_%s"       , chainTitleCh) , Form("l3pt_%s"       , chainTitleCh) , 7       , 0    , 140);
    p_result.h_el_l1pt.ttzcr  = new TH1F(Form("ttzcr_el_l1pt_%s"    , chainTitleCh) , Form("el_l1pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_el_l2pt.ttzcr  = new TH1F(Form("ttzcr_el_l2pt_%s"    , chainTitleCh) , Form("el_l2pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_el_l3pt.ttzcr  = new TH1F(Form("ttzcr_el_l3pt_%s"    , chainTitleCh) , Form("el_l3pt_%s"    , chainTitleCh) , 7       , 0    , 140);
    p_result.h_mu_l1pt.ttzcr  = new TH1F(Form("ttzcr_mu_l1pt_%s"    , chainTitleCh) , Form("mu_l1pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_mu_l2pt.ttzcr  = new TH1F(Form("ttzcr_mu_l2pt_%s"    , chainTitleCh) , Form("mu_l2pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_mu_l3pt.ttzcr  = new TH1F(Form("ttzcr_mu_l3pt_%s"    , chainTitleCh) , Form("mu_l3pt_%s"    , chainTitleCh) , 7       , 0    , 140);
    p_result.h_mvis.ttzcr      = new TH1F(Form("ttzcr_mvis_%s"        , chainTitleCh) , Form("mvis_%s"        , chainTitleCh) , 25      , 0    , 2500);
    p_result.h_mtvis.ttzcr      = new TH1F(Form("ttzcr_mtvis_%s"        , chainTitleCh) , Form("mtvis_%s"        , chainTitleCh) , 20, 300 , 2500);

    p_result.h_ht.ttwcr       = new TH1F(Form("ttwcr_ht_%s"         , chainTitleCh) , Form("ht_%s"         , chainTitleCh) , 15      , 100   , 1600);
    p_result.h_met.ttwcr      = new TH1F(Form("ttwcr_met_%s"        , chainTitleCh) , Form("met_%s"        , chainTitleCh) , 15      , 0    , 600);
    p_result.h_mll.ttwcr      = new TH1F(Form("ttwcr_mll_%s"        , chainTitleCh) , Form("mll_%s"        , chainTitleCh) , 20      , 0    , 200);
    p_result.h_mtmin.ttwcr    = new TH1F(Form("ttwcr_mtmin_%s"      , chainTitleCh) , Form("mtmin_%s"      , chainTitleCh) , 10      , 0    , 200);
    p_result.h_njets.ttwcr    = new TH1F(Form("ttwcr_njets_%s"      , chainTitleCh) , Form("njets_%s"      , chainTitleCh) , 10      , 1.5  , 11.5);
    p_result.h_nleps.ttwcr    = new TH1F(Form("ttwcr_nleps_%s"      , chainTitleCh) , Form("nleps_%s"      , chainTitleCh) , 6       , 0    , 6);
    p_result.h_nbtags.ttwcr   = new TH1F(Form("ttwcr_nbtags_%s"     , chainTitleCh) , Form("nbtags_%s"     , chainTitleCh) , 7       , -0.5 , 6.5);
    p_result.h_type.ttwcr     = new TH1F(Form("ttwcr_type_%s"       , chainTitleCh) , Form("type_%s"       , chainTitleCh) , 4       , 0    , 4);
    p_result.h_charge.ttwcr   = new TH1F(Form("ttwcr_charge_%s"     , chainTitleCh) , Form("charge_%s"     , chainTitleCh) , 3       , -1   , 2);
    p_result.h_l1pt.ttwcr     = new TH1F(Form("ttwcr_l1pt_%s"       , chainTitleCh) , Form("l1pt_%s"       , chainTitleCh) , 15      , 0    , 150);
    p_result.h_l2pt.ttwcr     = new TH1F(Form("ttwcr_l2pt_%s"       , chainTitleCh) , Form("l2pt_%s"       , chainTitleCh) , 15      , 0    , 150);
    p_result.h_l3pt.ttwcr     = new TH1F(Form("ttwcr_l3pt_%s"       , chainTitleCh) , Form("l3pt_%s"       , chainTitleCh) , 7       , 0    , 140);
    p_result.h_el_l1pt.ttwcr  = new TH1F(Form("ttwcr_el_l1pt_%s"    , chainTitleCh) , Form("el_l1pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_el_l2pt.ttwcr  = new TH1F(Form("ttwcr_el_l2pt_%s"    , chainTitleCh) , Form("el_l2pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_el_l3pt.ttwcr  = new TH1F(Form("ttwcr_el_l3pt_%s"    , chainTitleCh) , Form("el_l3pt_%s"    , chainTitleCh) , 7       , 0    , 140);
    p_result.h_mu_l1pt.ttwcr  = new TH1F(Form("ttwcr_mu_l1pt_%s"    , chainTitleCh) , Form("mu_l1pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_mu_l2pt.ttwcr  = new TH1F(Form("ttwcr_mu_l2pt_%s"    , chainTitleCh) , Form("mu_l2pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_mu_l3pt.ttwcr  = new TH1F(Form("ttwcr_mu_l3pt_%s"    , chainTitleCh) , Form("mu_l3pt_%s"    , chainTitleCh) , 7       , 0    , 140);
    p_result.h_mvis.ttwcr      = new TH1F(Form("ttwcr_mvis_%s"        , chainTitleCh) , Form("mvis_%s"        , chainTitleCh) , 25      , 0    , 2500);
    p_result.h_mtvis.ttwcr      = new TH1F(Form("ttwcr_mtvis_%s"        , chainTitleCh) , Form("mtvis_%s"        , chainTitleCh) , 20, 300 , 2500);

    p_result.h_ht.br       = new TH1F(Form("br_ht_%s"         , chainTitleCh) , Form("ht_%s"         , chainTitleCh) , 15      , 100   , 1600);
    p_result.h_met.br      = new TH1F(Form("br_met_%s"        , chainTitleCh) , Form("met_%s"        , chainTitleCh) , 15      , 0    , 600);
    p_result.h_mll.br      = new TH1F(Form("br_mll_%s"        , chainTitleCh) , Form("mll_%s"        , chainTitleCh) , 20      , 0    , 200);
    p_result.h_mtmin.br    = new TH1F(Form("br_mtmin_%s"      , chainTitleCh) , Form("mtmin_%s"      , chainTitleCh) , 10      , 0    , 200);
    p_result.h_njets.br    = new TH1F(Form("br_njets_%s"      , chainTitleCh) , Form("njets_%s"      , chainTitleCh) , 10      , 1.5  , 11.5);
    p_result.h_nleps.br    = new TH1F(Form("br_nleps_%s"      , chainTitleCh) , Form("nleps_%s"      , chainTitleCh) , 6       , 0    , 6);
    p_result.h_nbtags.br   = new TH1F(Form("br_nbtags_%s"     , chainTitleCh) , Form("nbtags_%s"     , chainTitleCh) , 7       , -0.5 , 6.5);
    p_result.h_type.br     = new TH1F(Form("br_type_%s"       , chainTitleCh) , Form("type_%s"       , chainTitleCh) , 4       , 0    , 4);
    p_result.h_charge.br   = new TH1F(Form("br_charge_%s"     , chainTitleCh) , Form("charge_%s"     , chainTitleCh) , 3       , -1   , 2);
    p_result.h_l1pt.br     = new TH1F(Form("br_l1pt_%s"       , chainTitleCh) , Form("l1pt_%s"       , chainTitleCh) , 15      , 0    , 150);
    p_result.h_l2pt.br     = new TH1F(Form("br_l2pt_%s"       , chainTitleCh) , Form("l2pt_%s"       , chainTitleCh) , 15      , 0    , 150);
    p_result.h_l3pt.br     = new TH1F(Form("br_l3pt_%s"       , chainTitleCh) , Form("l3pt_%s"       , chainTitleCh) , 7       , 0    , 140);
    p_result.h_el_l1pt.br  = new TH1F(Form("br_el_l1pt_%s"    , chainTitleCh) , Form("el_l1pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_el_l2pt.br  = new TH1F(Form("br_el_l2pt_%s"    , chainTitleCh) , Form("el_l2pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_el_l3pt.br  = new TH1F(Form("br_el_l3pt_%s"    , chainTitleCh) , Form("el_l3pt_%s"    , chainTitleCh) , 7       , 0    , 140);
    p_result.h_mu_l1pt.br  = new TH1F(Form("br_mu_l1pt_%s"    , chainTitleCh) , Form("mu_l1pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_mu_l2pt.br  = new TH1F(Form("br_mu_l2pt_%s"    , chainTitleCh) , Form("mu_l2pt_%s"    , chainTitleCh) , 15      , 0    , 150);
    p_result.h_mu_l3pt.br  = new TH1F(Form("br_mu_l3pt_%s"    , chainTitleCh) , Form("mu_l3pt_%s"    , chainTitleCh) , 7       , 0    , 140);
    p_result.h_mvis.br      = new TH1F(Form("br_mvis_%s"        , chainTitleCh) , Form("mvis_%s"        , chainTitleCh) , 25      , 0    , 2500);
    p_result.h_mtvis.br      = new TH1F(Form("br_mtvis_%s"        , chainTitleCh) , Form("mtvis_%s"        , chainTitleCh) , 20, 300 , 2500);
    p_result.h_bjetpt.br      = new TH1F(Form("br_bjetpt_%s"        , chainTitleCh) , Form("bjetpt_%s"        , chainTitleCh) , 16      , 0., 200.);
    p_result.h_jetpt.br      = new TH1F(Form("br_jetpt_%s"        , chainTitleCh) , Form("jetpt_%s"        , chainTitleCh) , 20      , 0., 200.);
    p_result.h_el_nmiss.br  = new TH1F(Form("br_el_nmiss_%s"    , chainTitleCh) , Form("el_nmiss_%s"    , chainTitleCh) , 2      , 0    , 2);
    p_result.h_nvtx.br     = new TH1F(Form("br_nvtx_%s"       , chainTitleCh) , Form("nvtx_%s"       , chainTitleCh) , 20       , 0    , 80);

    // p_result.h_metnm1.br      = new TH1F(Form("br_metnm1_%s"        , chainTitleCh) , Form("metnm1_%s"        , chainTitleCh) , 80      , 0    , 400);
    p_result.h_metnm1.br      = new TH1F(Form("br_metnm1_%s"        , chainTitleCh) , Form("metnm1_%s"        , chainTitleCh) , 40      , 0    , 400);
    p_result.h_mid1.br     = new TH1F(Form("br_mid1_%s"       , chainTitleCh) , Form("mid1_%s"       , chainTitleCh) , 5       , -2    , 3);
    p_result.h_mid2.br     = new TH1F(Form("br_mid2_%s"       , chainTitleCh) , Form("mid2_%s"       , chainTitleCh) , 5       , -2    , 3);
    p_result.h_mid3.br     = new TH1F(Form("br_mid3_%s"       , chainTitleCh) , Form("mid3_%s"       , chainTitleCh) , 5       , -2    , 3);
    p_result.h_charge3.sr   = new TH1F(Form("sr_charge3_%s"     , chainTitleCh) , Form("charge3_%s"     , chainTitleCh) , 3       , 0   , 3);

    // p_result.h_disc.br     = new TH1F(Form("br_disc_%s"       , chainTitleCh) , Form("disc_%s"       , chainTitleCh) , 16       , -1.0    , 1.0);
    p_result.h_disc.br     = new TH1F(Form("br_disc_%s"       , chainTitleCh) , Form("disc_%s"       , chainTitleCh) , 20       , -1.0    , 1.0);
    // p_result.h_disc2.br     = new TH1F(Form("br_disc2_%s"       , chainTitleCh) , Form("disc2_%s"       , chainTitleCh) , 16       ,0., 1.0);
    p_result.h_disc2.br     = new TH1F(Form("br_disc2_%s"       , chainTitleCh) , Form("disc2_%s"       , chainTitleCh) , 4       ,0., 0.8);
    p_result.SRCR.TOTAL = new TH1F(Form("SRCR_TOTAL_%s" , chainTitleCh) , Form("SRCR_TOTAL_%s" , chainTitleCh) , nsr     , 0.5  , nsr+0.5);
    p_result.SRDISC.TOTAL = new TH1F(Form("SRDISC_TOTAL_%s" , chainTitleCh) , Form("SRDISC_TOTAL_%s" , chainTitleCh) , nsrdisc     , 0.5  , nsrdisc+0.5);
    p_result.SR.TOTAL   = new TH1F(Form("SR_TOTAL_%s"   , chainTitleCh) , Form("SR_TOTAL_%s"   , chainTitleCh) , nsr-nCR , 0.5  , nsr-nCR+0.5);

    p_result.h_mva.br             = new TH1F(Form("br_mva_%s"             , chainTitleCh), Form("mva_%s"             , chainTitleCh), 10, 0    , 1.5);
    p_result.h_mu_sip3d_lep1.br   = new TH1F(Form("br_sip3d_mu_lep1_%s"   , chainTitleCh), Form("sip3d_mu_lep1_%s"   , chainTitleCh), 20, 0    , 5);
    p_result.h_mu_sip3d_lep2.br   = new TH1F(Form("br_sip3d_mu_lep2_%s"   , chainTitleCh), Form("sip3d_mu_lep2_%s"   , chainTitleCh), 20, 0    , 5);
    p_result.h_mu_sip3d_lep3.br   = new TH1F(Form("br_sip3d_mu_lep3_%s"   , chainTitleCh), Form("sip3d_mu_lep3_%s"   , chainTitleCh), 20, 0    , 5);
    p_result.h_el_sip3d_lep1.br   = new TH1F(Form("br_sip3d_el_lep1_%s"   , chainTitleCh), Form("sip3d_el_lep1_%s"   , chainTitleCh), 20, 0    , 5);
    p_result.h_el_sip3d_lep2.br   = new TH1F(Form("br_sip3d_el_lep2_%s"   , chainTitleCh), Form("sip3d_el_lep2_%s"   , chainTitleCh), 20, 0    , 5);
    p_result.h_el_sip3d_lep3.br   = new TH1F(Form("br_sip3d_el_lep3_%s"   , chainTitleCh), Form("sip3d_el_lep3_%s"   , chainTitleCh), 20, 0    , 5);
    p_result.h_mu_l1eta.br        = new TH1F(Form("br_l1eta_mu_%s"        , chainTitleCh), Form("l1eta_mu_%s"        , chainTitleCh), 12, -2.5 , 2.5);
    p_result.h_mu_l2eta.br        = new TH1F(Form("br_l2eta_mu_%s"        , chainTitleCh), Form("l2eta_mu_%s"        , chainTitleCh), 12, -2.5 , 2.5);
    p_result.h_mu_l3eta.br        = new TH1F(Form("br_l3eta_mu_%s"        , chainTitleCh), Form("l3eta_mu_%s"        , chainTitleCh), 8, -2.5 , 2.5);
    p_result.h_el_l1eta.br        = new TH1F(Form("br_l1eta_el_%s"        , chainTitleCh), Form("l1eta_el_%s"        , chainTitleCh), 12, -2.5 , 2.5);
    p_result.h_el_l2eta.br        = new TH1F(Form("br_l2eta_el_%s"        , chainTitleCh), Form("l2eta_el_%s"        , chainTitleCh), 12, -2.5 , 2.5);
    p_result.h_el_l3eta.br        = new TH1F(Form("br_l3eta_el_%s"        , chainTitleCh), Form("l3eta_el_%s"        , chainTitleCh), 8, -2.5 , 2.5);
    p_result.h_mu_lep1_miniIso.br = new TH1F(Form("br_lep1_mu_miniIso_%s" , chainTitleCh), Form("lep1_mu_miniIso_%s" , chainTitleCh), 15, 0    , 0.2);
    p_result.h_mu_lep2_miniIso.br = new TH1F(Form("br_lep2_mu_miniIso_%s" , chainTitleCh), Form("lep2_mu_miniIso_%s" , chainTitleCh), 15, 0    , 0.2);
    p_result.h_mu_lep3_miniIso.br = new TH1F(Form("br_lep3_mu_miniIso_%s" , chainTitleCh), Form("lep3_mu_miniIso_%s" , chainTitleCh), 15, 0    , 0.2);
    p_result.h_mu_lep1_ptRel.br   = new TH1F(Form("br_lep1_mu_ptRel_%s"   , chainTitleCh), Form("lep1_mu_ptRel_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_mu_lep2_ptRel.br   = new TH1F(Form("br_lep2_mu_ptRel_%s"   , chainTitleCh), Form("lep2_mu_ptRel_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_mu_lep3_ptRel.br   = new TH1F(Form("br_lep3_mu_ptRel_%s"   , chainTitleCh), Form("lep3_mu_ptRel_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_mu_lep1_ptRelfail.br   = new TH1F(Form("br_lep1_mu_ptRelfail_%s"   , chainTitleCh), Form("lep1_mu_ptRelfail_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_mu_lep2_ptRelfail.br   = new TH1F(Form("br_lep2_mu_ptRelfail_%s"   , chainTitleCh), Form("lep2_mu_ptRelfail_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_mu_lep3_ptRelfail.br   = new TH1F(Form("br_lep3_mu_ptRelfail_%s"   , chainTitleCh), Form("lep3_mu_ptRelfail_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_el_lep1_miniIso.br = new TH1F(Form("br_lep1_el_miniIso_%s" , chainTitleCh), Form("lep1_el_miniIso_%s" , chainTitleCh), 15, 0    , 0.2);
    p_result.h_el_lep2_miniIso.br = new TH1F(Form("br_lep2_el_miniIso_%s" , chainTitleCh), Form("lep2_el_miniIso_%s" , chainTitleCh), 15, 0    , 0.2);
    p_result.h_el_lep3_miniIso.br = new TH1F(Form("br_lep3_el_miniIso_%s" , chainTitleCh), Form("lep3_el_miniIso_%s" , chainTitleCh), 15, 0    , 0.2);
    p_result.h_el_lep1_ptRel.br   = new TH1F(Form("br_lep1_el_ptRel_%s"   , chainTitleCh), Form("lep1_el_ptRel_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_el_lep2_ptRel.br   = new TH1F(Form("br_lep2_el_ptRel_%s"   , chainTitleCh), Form("lep2_el_ptRel_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_el_lep3_ptRel.br   = new TH1F(Form("br_lep3_el_ptRel_%s"   , chainTitleCh), Form("lep3_el_ptRel_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_el_lep1_ptRelfail.br   = new TH1F(Form("br_lep1_el_ptRelfail_%s"   , chainTitleCh), Form("lep1_el_ptRelfail_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_el_lep2_ptRelfail.br   = new TH1F(Form("br_lep2_el_ptRelfail_%s"   , chainTitleCh), Form("lep2_el_ptRelfail_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_el_lep3_ptRelfail.br   = new TH1F(Form("br_lep3_el_ptRelfail_%s"   , chainTitleCh), Form("lep3_el_ptRelfail_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_mu_lep1_ptRatio.br = new TH1F(Form("br_lep1_mu_ptRatio_%s" , chainTitleCh), Form("lep1_mu_ptRatio_%s" , chainTitleCh), 30, -0.5    , 1.5);
    p_result.h_mu_lep2_ptRatio.br = new TH1F(Form("br_lep2_mu_ptRatio_%s" , chainTitleCh), Form("lep2_mu_ptRatio_%s" , chainTitleCh), 30, -0.5    , 1.5);
    p_result.h_mu_lep3_ptRatio.br = new TH1F(Form("br_lep3_mu_ptRatio_%s" , chainTitleCh), Form("lep3_mu_ptRatio_%s" , chainTitleCh), 30, -0.5    , 1.5);
    p_result.h_el_lep1_ptRatio.br = new TH1F(Form("br_lep1_el_ptRatio_%s" , chainTitleCh), Form("lep1_el_ptRatio_%s" , chainTitleCh), 30, -0.5    , 1.5);
    p_result.h_el_lep2_ptRatio.br = new TH1F(Form("br_lep2_el_ptRatio_%s" , chainTitleCh), Form("lep2_el_ptRatio_%s" , chainTitleCh), 30, -0.5    , 1.5);
    p_result.h_el_lep3_ptRatio.br = new TH1F(Form("br_lep3_el_ptRatio_%s" , chainTitleCh), Form("lep3_el_ptRatio_%s" , chainTitleCh), 30, -0.5    , 1.5);

    p_result.h_mva.sr             = new TH1F(Form("sr_mva_%s"             , chainTitleCh), Form("mva_%s"             , chainTitleCh), 10, 0    , 1.5);
    p_result.h_mu_sip3d_lep1.sr   = new TH1F(Form("sr_sip3d_mu_lep1_%s"   , chainTitleCh), Form("sip3d_mu_lep1_%s"   , chainTitleCh), 20, 0    , 5);
    p_result.h_mu_sip3d_lep2.sr   = new TH1F(Form("sr_sip3d_mu_lep2_%s"   , chainTitleCh), Form("sip3d_mu_lep2_%s"   , chainTitleCh), 20, 0    , 5);
    p_result.h_mu_sip3d_lep3.sr   = new TH1F(Form("sr_sip3d_mu_lep3_%s"   , chainTitleCh), Form("sip3d_mu_lep3_%s"   , chainTitleCh), 20, 0    , 5);
    p_result.h_el_sip3d_lep1.sr   = new TH1F(Form("sr_sip3d_el_lep1_%s"   , chainTitleCh), Form("sip3d_el_lep1_%s"   , chainTitleCh), 20, 0    , 5);
    p_result.h_el_sip3d_lep2.sr   = new TH1F(Form("sr_sip3d_el_lep2_%s"   , chainTitleCh), Form("sip3d_el_lep2_%s"   , chainTitleCh), 20, 0    , 5);
    p_result.h_el_sip3d_lep3.sr   = new TH1F(Form("sr_sip3d_el_lep3_%s"   , chainTitleCh), Form("sip3d_el_lep3_%s"   , chainTitleCh), 20, 0    , 5);
    p_result.h_mu_l1eta.sr        = new TH1F(Form("sr_l1eta_mu_%s"        , chainTitleCh), Form("l1eta_mu_%s"        , chainTitleCh), 12, -2.5 , 2.5);
    p_result.h_mu_l2eta.sr        = new TH1F(Form("sr_l2eta_mu_%s"        , chainTitleCh), Form("l2eta_mu_%s"        , chainTitleCh), 12, -2.5 , 2.5);
    p_result.h_mu_l3eta.sr        = new TH1F(Form("sr_l3eta_mu_%s"        , chainTitleCh), Form("l3eta_mu_%s"        , chainTitleCh), 8, -2.5 , 2.5);
    p_result.h_el_l1eta.sr        = new TH1F(Form("sr_l1eta_el_%s"        , chainTitleCh), Form("l1eta_el_%s"        , chainTitleCh), 12, -2.5 , 2.5);
    p_result.h_el_l2eta.sr        = new TH1F(Form("sr_l2eta_el_%s"        , chainTitleCh), Form("l2eta_el_%s"        , chainTitleCh), 12, -2.5 , 2.5);
    p_result.h_el_l3eta.sr        = new TH1F(Form("sr_l3eta_el_%s"        , chainTitleCh), Form("l3eta_el_%s"        , chainTitleCh), 8, -2.5 , 2.5);
    p_result.h_mu_lep1_miniIso.sr = new TH1F(Form("sr_lep1_mu_miniIso_%s" , chainTitleCh), Form("lep1_mu_miniIso_%s" , chainTitleCh), 15, 0    , 0.2);
    p_result.h_mu_lep2_miniIso.sr = new TH1F(Form("sr_lep2_mu_miniIso_%s" , chainTitleCh), Form("lep2_mu_miniIso_%s" , chainTitleCh), 15, 0    , 0.2);
    p_result.h_mu_lep3_miniIso.sr = new TH1F(Form("sr_lep3_mu_miniIso_%s" , chainTitleCh), Form("lep3_mu_miniIso_%s" , chainTitleCh), 15, 0    , 0.2);
    p_result.h_mu_lep1_ptRel.sr   = new TH1F(Form("sr_lep1_mu_ptRel_%s"   , chainTitleCh), Form("lep1_mu_ptRel_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_mu_lep2_ptRel.sr   = new TH1F(Form("sr_lep2_mu_ptRel_%s"   , chainTitleCh), Form("lep2_mu_ptRel_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_mu_lep3_ptRel.sr   = new TH1F(Form("sr_lep3_mu_ptRel_%s"   , chainTitleCh), Form("lep3_mu_ptRel_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_mu_lep1_ptRelfail.sr   = new TH1F(Form("sr_lep1_mu_ptRelfail_%s"   , chainTitleCh), Form("lep1_mu_ptRelfail_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_mu_lep2_ptRelfail.sr   = new TH1F(Form("sr_lep2_mu_ptRelfail_%s"   , chainTitleCh), Form("lep2_mu_ptRelfail_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_mu_lep3_ptRelfail.sr   = new TH1F(Form("sr_lep3_mu_ptRelfail_%s"   , chainTitleCh), Form("lep3_mu_ptRelfail_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_el_lep1_miniIso.sr = new TH1F(Form("sr_lep1_el_miniIso_%s" , chainTitleCh), Form("lep1_el_miniIso_%s" , chainTitleCh), 15, 0    , 0.2);
    p_result.h_el_lep2_miniIso.sr = new TH1F(Form("sr_lep2_el_miniIso_%s" , chainTitleCh), Form("lep2_el_miniIso_%s" , chainTitleCh), 15, 0    , 0.2);
    p_result.h_el_lep3_miniIso.sr = new TH1F(Form("sr_lep3_el_miniIso_%s" , chainTitleCh), Form("lep3_el_miniIso_%s" , chainTitleCh), 15, 0    , 0.2);
    p_result.h_el_lep1_ptRel.sr   = new TH1F(Form("sr_lep1_el_ptRel_%s"   , chainTitleCh), Form("lep1_el_ptRel_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_el_lep2_ptRel.sr   = new TH1F(Form("sr_lep2_el_ptRel_%s"   , chainTitleCh), Form("lep2_el_ptRel_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_el_lep3_ptRel.sr   = new TH1F(Form("sr_lep3_el_ptRel_%s"   , chainTitleCh), Form("lep3_el_ptRel_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_el_lep1_ptRelfail.sr   = new TH1F(Form("sr_lep1_el_ptRelfail_%s"   , chainTitleCh), Form("lep1_el_ptRelfail_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_el_lep2_ptRelfail.sr   = new TH1F(Form("sr_lep2_el_ptRelfail_%s"   , chainTitleCh), Form("lep2_el_ptRelfail_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_el_lep3_ptRelfail.sr   = new TH1F(Form("sr_lep3_el_ptRelfail_%s"   , chainTitleCh), Form("lep3_el_ptRelfail_%s"   , chainTitleCh), 15, 0    , 50);
    p_result.h_mu_lep1_ptRatio.sr = new TH1F(Form("sr_lep1_mu_ptRatio_%s" , chainTitleCh), Form("lep1_mu_ptRatio_%s" , chainTitleCh), 30, -0.5    , 1.5);
    p_result.h_mu_lep2_ptRatio.sr = new TH1F(Form("sr_lep2_mu_ptRatio_%s" , chainTitleCh), Form("lep2_mu_ptRatio_%s" , chainTitleCh), 30, -0.5    , 1.5);
    p_result.h_mu_lep3_ptRatio.sr = new TH1F(Form("sr_lep3_mu_ptRatio_%s" , chainTitleCh), Form("lep3_mu_ptRatio_%s" , chainTitleCh), 30, -0.5    , 1.5);
    p_result.h_el_lep1_ptRatio.sr = new TH1F(Form("sr_lep1_el_ptRatio_%s" , chainTitleCh), Form("lep1_el_ptRatio_%s" , chainTitleCh), 30, -0.5    , 1.5);
    p_result.h_el_lep2_ptRatio.sr = new TH1F(Form("sr_lep2_el_ptRatio_%s" , chainTitleCh), Form("lep2_el_ptRatio_%s" , chainTitleCh), 30, -0.5    , 1.5);
    p_result.h_el_lep3_ptRatio.sr = new TH1F(Form("sr_lep3_el_ptRatio_%s" , chainTitleCh), Form("lep3_el_ptRatio_%s" , chainTitleCh), 30, -0.5    , 1.5);


    //For FR variations
    plots_t p_fake_alt_up;
    if (doFakes == 1) {
        p_fake_alt_up.SRCR.TOTAL        = new TH1F(Form("SRCR_FR_TOTAL_%s"      , chainTitleCh) , Form("SRCR_FR_TOTAL_%s"      , chainTitleCh) , nsr   , 0.5, nsr+0.5 );
        p_fake_alt_up.SRDISC.TOTAL        = new TH1F(Form("SRDISC_FR_TOTAL_%s"      , chainTitleCh) , Form("SRDISC_FR_TOTAL_%s"      , chainTitleCh) , nsrdisc   , 0.5, nsrdisc+0.5 );
    } else {
        p_fake_alt_up.SRCR.TOTAL        = 0;
        p_fake_alt_up.SRDISC.TOTAL        = 0;
    }
    //For JES variations
    plots_t p_jes_alt_up;
    plots_t p_jes_alt_dn;
    if (doFakes == 1 || isData==0) {
        p_jes_alt_up.SRCR.TOTAL     = new TH1F(Form("SRCR_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JES_UP_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 );
        p_jes_alt_dn.SRCR.TOTAL     = new TH1F(Form("SRCR_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JES_DN_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 );
        p_jes_alt_up.SRDISC.TOTAL     = new TH1F(Form("SRDISC_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JES_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 );
        p_jes_alt_dn.SRDISC.TOTAL     = new TH1F(Form("SRDISC_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JES_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 );
    } else {
        p_jes_alt_up.SRCR.TOTAL     = 0;
        p_jes_alt_dn.SRCR.TOTAL     = 0;
        p_jes_alt_up.SRDISC.TOTAL     = 0;
        p_jes_alt_dn.SRDISC.TOTAL     = 0;
    }
    //For JER variations
    plots_t p_jer_alt_up;
    plots_t p_jer_alt_dn;
    if (doFakes == 1 || isData==0) {
        p_jer_alt_up.SRCR.TOTAL     = new TH1F(Form("SRCR_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JER_UP_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 );
        p_jer_alt_dn.SRCR.TOTAL     = new TH1F(Form("SRCR_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JER_DN_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 );
        p_jer_alt_up.SRDISC.TOTAL     = new TH1F(Form("SRDISC_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JER_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 );
        p_jer_alt_dn.SRDISC.TOTAL     = new TH1F(Form("SRDISC_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JER_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 );
    } else {
        p_jer_alt_up.SRCR.TOTAL     = 0;
        p_jer_alt_dn.SRCR.TOTAL     = 0;
        p_jer_alt_up.SRDISC.TOTAL     = 0;
        p_jer_alt_dn.SRDISC.TOTAL     = 0;
    }
    //For btag SF variations
    plots_t p_btagSF_alt_up;
    plots_t p_btagSF_alt_dn;
    if (isData==0){
        p_btagSF_alt_up.SRCR.TOTAL = new TH1F(Form("SRCR_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRCR_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nsr,  0.5, nsr+0.5 );
        p_btagSF_alt_dn.SRCR.TOTAL = new TH1F(Form("SRCR_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRCR_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nsr,  0.5, nsr+0.5 );
        p_btagSF_alt_up.SRDISC.TOTAL = new TH1F(Form("SRDISC_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRDISC_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nsrdisc,  0.5, nsrdisc+0.5 );
        p_btagSF_alt_dn.SRDISC.TOTAL = new TH1F(Form("SRDISC_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRDISC_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nsrdisc,  0.5, nsrdisc+0.5 );
    }
    else {
        p_btagSF_alt_up.SRCR.TOTAL = 0;
        p_btagSF_alt_dn.SRCR.TOTAL = 0;
        p_btagSF_alt_up.SRDISC.TOTAL = 0;
        p_btagSF_alt_dn.SRDISC.TOTAL = 0;
    }
    //For PU variations
    plots_t p_pu_alt_up;
    plots_t p_pu_alt_dn;
    if (doFakes == 1 || isData==0) {
        p_pu_alt_up.SRCR.TOTAL     = new TH1F(Form("SRCR_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PU_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_pu_alt_dn.SRCR.TOTAL     = new TH1F(Form("SRCR_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PU_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_pu_alt_up.SRDISC.TOTAL     = new TH1F(Form("SRDISC_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PU_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_pu_alt_dn.SRDISC.TOTAL     = new TH1F(Form("SRDISC_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PU_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    } else {
        p_pu_alt_up.SRCR.TOTAL     = 0;
        p_pu_alt_dn.SRCR.TOTAL     = 0;
        p_pu_alt_up.SRDISC.TOTAL     = 0;
        p_pu_alt_dn.SRDISC.TOTAL     = 0;
    }
    //For PU variations
    plots_t p_lep_alt_up;
    plots_t p_lep_alt_dn;
    if (doFakes == 1 || isData==0) {
        p_lep_alt_up.SRCR.TOTAL     = new TH1F(Form("SRCR_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_LEP_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_lep_alt_dn.SRCR.TOTAL     = new TH1F(Form("SRCR_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_LEP_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_lep_alt_up.SRDISC.TOTAL     = new TH1F(Form("SRDISC_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_LEP_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_lep_alt_dn.SRDISC.TOTAL     = new TH1F(Form("SRDISC_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_LEP_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    } else {
        p_lep_alt_up.SRCR.TOTAL     = 0;
        p_lep_alt_dn.SRCR.TOTAL     = 0;
        p_lep_alt_up.SRDISC.TOTAL     = 0;
        p_lep_alt_dn.SRDISC.TOTAL     = 0;
    }
    //For bb variations
    plots_t p_bb_alt_up;
    plots_t p_bb_alt_dn;
    if (doFakes == 1 || isData==0) {
        p_bb_alt_up.SRCR.TOTAL     = new TH1F(Form("SRCR_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_BB_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_bb_alt_dn.SRCR.TOTAL     = new TH1F(Form("SRCR_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_BB_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_bb_alt_up.SRDISC.TOTAL     = new TH1F(Form("SRDISC_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_BB_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_bb_alt_dn.SRDISC.TOTAL     = new TH1F(Form("SRDISC_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_BB_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    } else {
        p_bb_alt_up.SRCR.TOTAL     = 0;
        p_bb_alt_dn.SRCR.TOTAL     = 0;
        p_bb_alt_up.SRDISC.TOTAL     = 0;
        p_bb_alt_dn.SRDISC.TOTAL     = 0;
    }
    //For PU variations
    plots_t p_isr_alt_up;
    plots_t p_isr_alt_dn;
    if (doFakes == 1 || isData==0) {
        p_isr_alt_up.SRCR.TOTAL     = new TH1F(Form("SRCR_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISR_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_isr_alt_dn.SRCR.TOTAL     = new TH1F(Form("SRCR_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISR_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_isr_alt_up.SRDISC.TOTAL     = new TH1F(Form("SRDISC_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISR_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_isr_alt_dn.SRDISC.TOTAL     = new TH1F(Form("SRDISC_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISR_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    } else {
        p_isr_alt_up.SRCR.TOTAL     = 0;
        p_isr_alt_dn.SRCR.TOTAL     = 0;
        p_isr_alt_up.SRDISC.TOTAL     = 0;
        p_isr_alt_dn.SRDISC.TOTAL     = 0;
    }
    //For theory variations
    plots_t p_pdf_alt_up;
    plots_t p_pdf_alt_dn;
    plots_t p_scale_alt_up;
    plots_t p_scale_alt_dn;
    plots_t p_alphas_alt_up;
    plots_t p_alphas_alt_dn;
    if (isData==0) {
        p_pdf_alt_up.SRCR.TOTAL     = new TH1F(Form("SRCR_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PDF_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_pdf_alt_dn.SRCR.TOTAL     = new TH1F(Form("SRCR_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PDF_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_pdf_alt_up.SRDISC.TOTAL     = new TH1F(Form("SRDISC_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PDF_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_pdf_alt_dn.SRDISC.TOTAL     = new TH1F(Form("SRDISC_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PDF_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_scale_alt_up.SRCR.TOTAL     = new TH1F(Form("SRCR_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_scale_alt_dn.SRCR.TOTAL     = new TH1F(Form("SRCR_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_scale_alt_up.SRDISC.TOTAL     = new TH1F(Form("SRDISC_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_scale_alt_dn.SRDISC.TOTAL     = new TH1F(Form("SRDISC_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_alphas_alt_up.SRCR.TOTAL     = new TH1F(Form("SRCR_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_alphas_alt_dn.SRCR.TOTAL     = new TH1F(Form("SRCR_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_alphas_alt_up.SRDISC.TOTAL     = new TH1F(Form("SRDISC_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_alphas_alt_dn.SRDISC.TOTAL     = new TH1F(Form("SRDISC_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    } else {
        p_pdf_alt_up.SRCR.TOTAL     = 0;
        p_pdf_alt_dn.SRCR.TOTAL     = 0;
        p_pdf_alt_up.SRDISC.TOTAL     = 0;
        p_pdf_alt_dn.SRDISC.TOTAL     = 0;
        p_scale_alt_up.SRCR.TOTAL     = 0;
        p_scale_alt_dn.SRCR.TOTAL     = 0;
        p_scale_alt_up.SRDISC.TOTAL     = 0;
        p_scale_alt_dn.SRDISC.TOTAL     = 0;
        p_alphas_alt_up.SRCR.TOTAL     = 0;
        p_alphas_alt_dn.SRCR.TOTAL     = 0;
        p_alphas_alt_up.SRDISC.TOTAL     = 0;
        p_alphas_alt_dn.SRDISC.TOTAL     = 0;
    }


    bool doPoisson = isData && !doFlips && !doFakes;
    initHistError(doPoisson, p_result.h_disc.br);
    initHistError(doPoisson, p_result.h_disc2.br);
    initHistError(doPoisson, p_result.SRCR.TOTAL   );
    initHistError(doPoisson, p_result.SRDISC.TOTAL   );
    initHistError(doPoisson, p_result.SR.TOTAL   );
    initHistError(doPoisson, p_jes_alt_up.SRCR.TOTAL   );
    initHistError(doPoisson, p_jes_alt_up.SRDISC.TOTAL   );
    initHistError(doPoisson, p_jes_alt_dn.SRCR.TOTAL   );
    initHistError(doPoisson, p_jes_alt_dn.SRDISC.TOTAL   );
    initHistError(doPoisson, p_jer_alt_up.SRCR.TOTAL   );
    initHistError(doPoisson, p_jer_alt_up.SRDISC.TOTAL   );
    initHistError(doPoisson, p_jer_alt_dn.SRCR.TOTAL   );
    initHistError(doPoisson, p_jer_alt_dn.SRDISC.TOTAL   );
    initHistError(doPoisson, p_btagSF_alt_up.SRCR.TOTAL);
    initHistError(doPoisson, p_btagSF_alt_up.SRDISC.TOTAL);
    initHistError(doPoisson, p_btagSF_alt_dn.SRCR.TOTAL);
    initHistError(doPoisson, p_btagSF_alt_dn.SRDISC.TOTAL);
    initHistError(doPoisson, p_pu_alt_up.SRCR.TOTAL    );
    initHistError(doPoisson, p_pu_alt_up.SRDISC.TOTAL    );
    initHistError(doPoisson, p_lep_alt_up.SRCR.TOTAL    );
    initHistError(doPoisson, p_lep_alt_up.SRDISC.TOTAL    );
    initHistError(doPoisson, p_isr_alt_up.SRCR.TOTAL    );
    initHistError(doPoisson, p_isr_alt_up.SRDISC.TOTAL    );
    initHistError(doPoisson, p_bb_alt_up.SRCR.TOTAL    );
    initHistError(doPoisson, p_bb_alt_up.SRDISC.TOTAL    );
    initHistError(doPoisson, p_pu_alt_dn.SRCR.TOTAL    );
    initHistError(doPoisson, p_pu_alt_dn.SRDISC.TOTAL    );
    initHistError(doPoisson, p_fake_alt_up.SRCR.TOTAL    );
    initHistError(doPoisson, p_fake_alt_up.SRDISC.TOTAL    );
    initHistError(doPoisson, p_pdf_alt_up.SRCR.TOTAL    );
    initHistError(doPoisson, p_pdf_alt_up.SRDISC.TOTAL    );
    initHistError(doPoisson, p_pdf_alt_dn.SRCR.TOTAL    );
    initHistError(doPoisson, p_pdf_alt_dn.SRDISC.TOTAL    );
    initHistError(doPoisson, p_scale_alt_up.SRCR.TOTAL    );
    initHistError(doPoisson, p_scale_alt_up.SRDISC.TOTAL    );
    initHistError(doPoisson, p_scale_alt_dn.SRCR.TOTAL    );
    initHistError(doPoisson, p_scale_alt_dn.SRDISC.TOTAL    );
    initHistError(doPoisson, p_alphas_alt_up.SRCR.TOTAL    );
    initHistError(doPoisson, p_alphas_alt_up.SRDISC.TOTAL    );
    initHistError(doPoisson, p_alphas_alt_dn.SRCR.TOTAL    );
    initHistError(doPoisson, p_alphas_alt_dn.SRDISC.TOTAL    );


    //nEvents in chain
    unsigned int nEventsTotal = 0;
    unsigned int nEventsChain = chain->GetEntries();

    //Set up iterator
    TObjArray *listOfFiles = chain->GetListOfFiles();
    TIter fileIter(listOfFiles);
    TFile *currentFile = 0;

    //Number of selected events
    //int nSelected = 0;

    //File Loop
    while ( (currentFile = (TFile*)fileIter.Next()) ){

        // Get File Content
        TFile *file = new TFile(currentFile->GetTitle());
        TString filename = currentFile->GetTitle();
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        float fudgefactor = 1.0;
        bool isFastSim = false;
        if (filename.Contains("_scan.root")) {
            isFastSim = true;
        }
        if (isSignal && isHiggsScan && filename.Contains("TTH_scan.root")) {
            if (fabs(mysparms.at(0)-350)<1.) fudgefactor = 103778. /  106860;
            if (fabs(mysparms.at(0)-370)<1.) fudgefactor = 91175. /   95639;
            if (fabs(mysparms.at(0)-390)<1.) fudgefactor = 93998. /   98685;
            if (fabs(mysparms.at(0)-410)<1.) fudgefactor = 98687. /   103643;
            if (fabs(mysparms.at(0)-430)<1.) fudgefactor = 102421. /  105441;
            if (fabs(mysparms.at(0)-450)<1.) fudgefactor = 87667. /   91375;
            if (fabs(mysparms.at(0)-470)<1.) fudgefactor = 96639. /   103045;
            if (fabs(mysparms.at(0)-490)<1.) fudgefactor = 93382. /   98446;
            if (fabs(mysparms.at(0)-510)<1.) fudgefactor = 101248. /  104742;
            if (fabs(mysparms.at(0)-530)<1.) fudgefactor = 86954. /   89999;
            if (fabs(mysparms.at(0)-550)<1.) fudgefactor = 91983. /   95851;
            std::cout <<  " mysparms.at(0): " << mysparms.at(0) <<  " fudgefactor: " << fudgefactor <<  std::endl;
        }

        if ((istttt||(isHiggsScan && !isFastSim)) && makeGenVariationsMC) {
            TH1F *cs_hist = 0;
            for (auto&& keyAsObj : *file->GetListOfKeys()){
                auto key = (TKey*) keyAsObj;
                TH1F *hist = (TH1F*)key->ReadObj();
                string title = hist->GetTitle();
                if (title == "SS2015 Baby Ntuple") continue;
                cs_hist = (TH1F*)hist->Clone("cs_hist");
            }
            //Next figure out scale, PDF, and alpha_s variations and fill the histograms
            for (int i = 1; i < nsr+1; i++){
                float nominal  = std::max(cs_hist->GetBinContent(200+i),0.); // NJA must match main.cc.norun
                float scale_up = nominal > 0 ? fabs(1-std::max(cs_hist->GetBinContent(400+i),0.)/nominal)  : 0;  // NJA must match main.cc.norun
                float scale_dn = nominal > 0 ? fabs(1-std::max(cs_hist->GetBinContent(600+i),0.)/nominal)  : 0;  // NJA must match main.cc.norun
                if (scale_up >= 1.0 && scale_dn >= 1.0) { // sometimes have crazy values
                    scale_up = 0.1;
                    scale_dn = 0.1;
                }
                TH1F *h_pdf = new TH1F("pdf", "pdf", 10000, 0, 1000);
                for (int j = 0; j < 100; j++){
                    h_pdf->Fill(cs_hist->GetBinContent(1000+100*(i-1)+j));
                }
                float pdf = h_pdf->GetMean() > 0 ? h_pdf->GetRMS()/nominal : 0;
                delete h_pdf;
                // std::cout << " i: " << i << " nominal: " << nominal << " scale_up: " << scale_up << " scale_dn: " << scale_dn << " pdf: " << pdf << std::endl;
                // p_scale_alt_up.SRCR.TOTAL->Fill(i,    scale_up);
                // p_scale_alt_dn.SRCR.TOTAL->Fill(i,    scale_dn);
                p_pdf_alt_up.SRCR.TOTAL->Fill(i,    pdf);
            }
        }

    

        // Loop over Events in current file
        for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){

            //Get Event Content
            samesign.GetEntry(event);
            nEventsTotal++;

            if (ss::is_real_data()){
                if (false && !doFlips && !doFakes) continue;
                else if (!isUnblindRun(ss::run())) continue;
            }

            //Reject not triggered
            if (!isFastSim) {
                if (!ss::fired_trigger()) continue;
            }
            if (!ss::passes_met_filters()) continue;

            if (doCustomSelection) {

            }

            if (isHiggsScan) {
                // make sure the higgs mass point we are considering is the same as chain title
                if (fabs(mysparms[0]-ss::higgs_mass()) > 0.1) continue;
            }


            float weight =  ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG;

            // if (isttH) {
            //     weight *= 1.26;
            // }
            if (istttt) {
                // new xsec is 11.97
                weight *= 11.97 / fabs(1000.0*ss::xsec());
            }
            weight*=scaleLumi;

            if (yt > 0.) weight *= yt*yt;

            if (isHiggsScan && isFastSim) {
                weight *= fudgefactor;
            }

            // // FIXME FIXME
            // if (chainTitle=="fakes") weight *= 1.09;
            // if (chainTitle=="flips") weight *= 1.01;
            // if (chainTitle=="rares") weight *= 1.08;
            // if (chainTitle=="tth") weight   *= 1.10;
            // if (chainTitle=="ttw") weight   *= 1.21;
            // if (chainTitle=="ttvv") weight  *= 1.03;
            // if (chainTitle=="ttz") weight   *= 1.44;
            // if (chainTitle=="xg") weight    *= 1.03;


            if (isHiggsPseudoscalar) weight *= ss::xsec_ps()/ss::xsec();

            if (!ss::is_real_data() && !isFastSim) weight *= ss::decayWSF();

            if (!ss::is_real_data()) {
                weight *= getTruePUw(ss::trueNumInt()[0]);
            }
            weight*=ss::weight_btagsf();
            // if (isWZ) weight*=getWZSF();
            // if (isttZ && applyttZSF) weight*=getttZSF();
            //apply lepton scale factors
            if (ss::is_real_data()==0 && (!isttW) && (!isttZ)) {
                // weight*=eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
                weight *= leptonScaleFactor(ss::lep1_id(), ss::lep1_p4().pt(), ss::lep1_p4().eta(), ss::ht(), -1);
                weight *= leptonScaleFactor(ss::lep2_id(), ss::lep2_p4().pt(), ss::lep2_p4().eta(), ss::ht(), -1);
            }
            if (isFastSim) {
                weight*=eventScaleFactorFastSim(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht(), ss::trueNumInt()[0]);
            }

            float weight_isr_up_alt = weight;
            if (doTTZISR && isttZ) {
                // want to take 50% of the effect as unc, and since we scale
                // nominal weight, scale up twice
                weight_isr_up_alt *= pow(ss::weight_isr_tt()*isr_norm_tt,2);
                weight *= ss::weight_isr_tt()*isr_norm_tt;
            }
            if (doTTWISR && isttW) {
                weight_isr_up_alt *= pow(ss::weight_isr_tt()*isr_norm_tt,2);
                // weight *= ss::weight_isr_dy()*isr_norm_dy;
                weight *= ss::weight_isr_tt()*isr_norm_tt;
            }

            float weight_bb_up_alt = weight;
            // if (doTTBB && (isttZ || isttW || isttH)) {
            if (doTTBB && (isttZ || isttW)) {
                // want to take full effect as unc, and since we scale
                // nominal weight, scale up twice
                if (ss::extragenb() == 2) {
                    float scaleamt = 0.7;
                    weight_bb_up_alt = (0.5*scaleamt+1)*weight;
                    // weight_bb_up_alt = (1.0*scaleamt+1)*weight;
                    weight *= 1.0;
                    // float scaleamt = 1.7;
                    // weight_bb_up_alt = weight*scaleamt*scaleamt;
                    // weight *= scaleamt;
                }
            }

            float weight_alt_FR = weight;
            float weight_btag_up_alt = weight;
            float weight_btag_dn_alt = weight;
            float weight_pu_up_alt = weight;
            float weight_pu_dn_alt = weight;
            float weight_lep_up_alt = weight;
            if (ss::is_real_data()==0) {
                weight_btag_up_alt = ss::weight_btagsf()>0 ? weight*ss::weight_btagsf_UP()/ss::weight_btagsf() : weight;
                weight_btag_dn_alt = ss::weight_btagsf()>0 ? weight*ss::weight_btagsf_DN()/ss::weight_btagsf() : weight;
                weight_pu_up_alt = getTruePUw(ss::trueNumInt()[0])>0 ? weight*getTruePUwUp(ss::trueNumInt()[0])/getTruePUw(ss::trueNumInt()[0]) : weight;
                weight_pu_dn_alt = getTruePUw(ss::trueNumInt()[0])>0 ? weight*getTruePUwDn(ss::trueNumInt()[0])/getTruePUw(ss::trueNumInt()[0]) : weight;
                weight_lep_up_alt = weight*(1.0+leptonScaleFactor_err(ss::lep1_id(),  ss::lep1_p4().pt(),  ss::lep1_p4().eta(),  ss::ht())+leptonScaleFactor_err(ss::lep2_id(),  ss::lep2_p4().pt(),  ss::lep2_p4().eta(),  ss::ht()));
                // std::cout << ss::lep1_id() << " " << ss::lep1_p4().pt() << " " << ss::lep1_p4().eta() << " " << ss::lep2_id() << " " << ss::lep2_p4().pt() << " " << ss::lep2_p4().eta() << " " << 1.0+leptonScaleFactor_err(ss::lep1_id(),  ss::lep1_p4().pt(),  ss::lep1_p4().eta(),  ss::ht())+leptonScaleFactor_err(ss::lep2_id(),  ss::lep2_p4().pt(),  ss::lep2_p4().eta(),  ss::ht()) << std::endl;
            }


            float lep1_pt = ss::lep1_coneCorrPt();
            float lep2_pt = ss::lep2_coneCorrPt();
            float lep3_pt = ss::lep3_coneCorrPt();
            //now recompute mtmin
            float mtl1 = MT(lep1_pt, ss::lep1_p4().phi(), ss::met(), ss::metPhi());
            float mtl2 = MT(lep2_pt, ss::lep2_p4().phi(), ss::met(), ss::metPhi());
            float mtmin = mtl1 > mtl2 ? mtl2 : mtl1;


            //drop electrons below 15 GeV
            if (abs(ss::lep1_id())==11 && lep1_pt<15) continue;
            if (abs(ss::lep2_id())==11 && lep2_pt<15) continue;

            //Progress
            SSAG::progress(nEventsTotal, nEventsChain);

            //Only keep good events
            int ssclass = 3;
            bool isClass6 = ss::hyp_class() == 6;
            if (!doFlips && !doFakes && exclude == 0 && !tightfake) {
                // if (ss::hyp_class() != ssclass && !isClass6) continue;
                if (ss::hyp_class() != ssclass && !isClass6 && ss::hyp_class() != 8) continue;
                if (!isData && !isGamma && ss::lep1_motherID()==2) continue;
                if (!isData && !isGamma && ss::lep2_motherID()==2) continue;
                if (!isData && !( (ss::lep1_motherID()==1 && ss::lep2_motherID()==1) || (ss::lep1_motherID()==-3 || ss::lep2_motherID()==-3)) ) continue;
            }

            if (!doFlips && !doFakes && exclude == 1){
                // if (ss::hyp_class() != ssclass && !isClass6) continue;
                if (ss::hyp_class() != ssclass && !isClass6 && ss::hyp_class() != 8) continue;
                if (!isData && !((ss::lep1_motherID() == 1 && ss::lep2_motherID() < 0) || (ss::lep1_motherID() < 0 && ss::lep2_motherID() == 1))) continue;
            }

            if (!doFlips && !doFakes && exclude == 2){
                // if (ss::hyp_class() != ssclass && !isClass6) continue;
                if (ss::hyp_class() != ssclass && !isClass6 && ss::hyp_class() != 8) continue;
                if (!isData && !((ss::lep1_motherID() == 1 && ss::lep2_motherID() == 2) || (ss::lep1_motherID() == 2 && ss::lep2_motherID() == 1))) continue;
            }

            if (!doFlips && !doFakes && exclude == 3){
                // if (ss::hyp_class() != ssclass && !isClass6) continue;
                if (ss::hyp_class() != ssclass && !isClass6 && ss::hyp_class() != 8) continue;
                if (!isData && !((ss::lep1_motherID() == 1 || ss::lep2_motherID() == 1))) continue;
            }

            if (doFlips) {
                if (ss::hyp_class() != 4 && !isClass6) continue;
                float flipFact = 0.;
                if (abs(ss::lep1_id())==11){
                    float flr = flipRate(ss::lep1_p4().pt(), ss::lep1_p4().eta());
                    flipFact += (flr/(1-flr));
                }
                if (abs(ss::lep2_id())==11){
                    float flr = flipRate(ss::lep2_p4().pt(), ss::lep2_p4().eta());
                    flipFact += (flr/(1-flr));
                }
                weight *= flipFact;
            }

            if (tightfake) {
                if (ss::hyp_class() != ssclass) continue;
                int nbadlegs = (ss::lep1_motherID() <= 0) + (ss::lep2_motherID() <= 0);
                int ngoodlegs = (ss::lep1_motherID() == 1) + (ss::lep2_motherID() == 1);
                // if it's MC and we're tightfake, then skip the event if it's 
                // truth matched to be prompt prompt. we only want reco tight-tight
                // events that are prompt-nonprompt (or nonprompt nonprompt)
                if (!isData && (nbadlegs != 1 || ngoodlegs != 1)) continue;
            }

            //QCD Fakes
            bool isClass6Fake = false;
            if (doFakes == 1){
                if (!isClass6) {
                    if (ss::hyp_class() != 2 && ss::hyp_class() != 1) continue;
                    if (ss::lep1_passes_id()==0) {
                        float fr = fakeRate(ss::lep1_id(),ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), ss::ht());
                        float fra = alternativeFakeRate(ss::lep1_id(),ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), ss::ht());
                        if (!ignoreFakeFactor) weight *= fr/(1.-fr);
                        if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                    }
                    if (ss::lep2_passes_id()==0) {
                        float fr = fakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), ss::ht());
                        float fra = alternativeFakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), ss::ht());
                        if (!ignoreFakeFactor) weight *= fr/(1.-fr);
                        if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                    }
                    //subtract double FO
                    if (ss::hyp_class() == 1) weight *= -1.;
                    if (!ss::is_real_data() && isData) {
                        weight *= -1.;
                    }
                } else {
                    if(ss::is_real_data() && (ss::lep3_fo() && !ss::lep3_tight()) && ss::lep1_passes_id() && ss::lep2_passes_id()) {  // lep3 fake
                        float fr = fakeRate(ss::lep3_id(),ss::lep3_p4().pt(),ss::lep3_p4().eta(),ss::ht());
                        float fra = alternativeFakeRate(ss::lep3_id(),ss::lep3_p4().pt(), ss::lep3_p4().eta(), ss::ht());
                        isClass6Fake = true;
                        if (!ignoreFakeFactor) weight *= fr / (1-fr);
                        if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                    }
                    if(ss::is_real_data() && (ss::lep2_fo() && !ss::lep2_tight()) && ss::lep1_passes_id() && ss::lep3_passes_id()) {  // lep2 fake
                        float fr = fakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(),ss::lep2_p4().eta(),ss::ht());
                        float fra = alternativeFakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), ss::ht());
                        isClass6Fake = true;
                        if (!ignoreFakeFactor) weight *= fr / (1-fr);
                        if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                    }
                    if(ss::is_real_data() && (ss::lep1_fo() && !ss::lep1_tight()) && ss::lep2_passes_id() && ss::lep3_passes_id()) {  // lep1 fake
                        float fr = fakeRate(ss::lep1_id(),ss::lep1_coneCorrPt(),ss::lep1_p4().eta(),ss::ht());
                        float fra = alternativeFakeRate(ss::lep1_id(),ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), ss::ht());
                        isClass6Fake = true;
                        if (!ignoreFakeFactor) weight *= fr / (1-fr);
                        if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                    }
                }
            }

            // // Before any real continue statements
            //     int nleps_temp = 2;
            //     if (ss::lep3_passes_id()) nleps_temp++;
            //     int SR_temp = signalRegionTest(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt,  nleps_temp, isClass6);
            //     if (doSync && isttW) {
            //         std::cout << Form("%1d %9d %llu\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%1d\t%1d\t%1d\n",
            //                 ss::run(), ss::lumi(), ss::event(),
            //                 ss::lep1_id(), lep1_pt,
            //                 ss::lep2_id(), lep2_pt,
            //                 ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::hyp_class(), SR_temp, nleps_temp) << std::endl;
            //     }

            if (isClass6) {
                float zmass23 = ss::lep2_id() == -ss::lep3_id() ? (ss::lep2_p4()+ss::lep3_p4()).mass() : -999.0;
                float zmass31 = ss::lep3_id() == -ss::lep1_id() ? (ss::lep3_p4()+ss::lep1_p4()).mass() : -999.0;
                float zmass = ( fabs(zmass31 - 91.2) < fabs(zmass23 - 91.2) ) ? zmass31 : zmass23;
                if (fabs(zmass-91.2) > 15) continue;
                if (doFakes && isData) {
                    if (!ss::is_real_data()) continue;
                    if (!isClass6Fake) continue;
                }
                if( !( isClass6Fake || (ss::lep1_passes_id() && ss::lep2_passes_id() && ss::lep3_passes_id()) ) ) continue;
            }

            int nleps = 2;
            bool lep3_pid = ss::lep3_passes_id();
            if (ss::lep3_passes_id()) nleps++;
            // if (ss::is_real_data()) {
            //     if (ss::lep3_passes_id()) nleps++;
            // } else {
            //     if (ss::lep3_passes_RA7v2() && ss::lep3_p4().pt() > 20.) nleps++;
            // }
            if (ss::lep4_passes_id() && (ss::lep4_p4().pt() > 20.) ) nleps++;

            if (!doFlips && !doFakes && exclude == 0) {
                if (nleps > 2) {
                if (!isData && !isGamma && ss::lep3_motherID()==2) continue;
                }
            }

            //Reject duplicates (after selection otherwise flips are ignored...)
            if (isData && ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)){ continue; }
            }

            // if all 3 charges are the same, throw the event away
            if (nleps > 2) {
              int q1 = 2*(ss::lep1_id() > 0) - 1;
              int q3 = 2*(ss::lep3_id() > 0) - 1;
              if (q3==q1) continue;
            }

            if (signalRegionTest(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, lep3_pt,  nleps, isClass6) > 1) {
                p_result.h_metnm1.br->Fill(ss::met() , weight);
            }

            //Require nominal baseline selections
            int BR = baseline_region(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, metcut);
            if (BR < 0) continue;

            if (!outputTrainingBDT) {
                // tttt baseline selections
                if (ss::njets() < 2) continue;
                if (ss::nbtags() < 2) continue;
                if (ss::met() < metcut) continue;
                if (ss::ht() < 300) continue;
                if (lep1_pt < 25.) continue;
                if (lep2_pt < 20.) continue;
            } else {
                // looser selections when training BDT
                if (lep1_pt < 15.) continue;
                if (lep2_pt < 15.) continue;
                if (ss::ht() < 250) continue;
                if (ss::met() < 30) continue;
                if (ss::njets() < 2) continue;
                if (ss::nbtags() < 1) continue;
            }

            // if (ss::ht() < 400) continue; // FIXME


                // if (istttt) {
                //     float blah = eventScaleFactor_trigonly(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
                //     std::cout <<  " blah: " << blah <<  std::endl;
                // }

        // c2numpy_intc(&writer, 3);
        // c2numpy_float64(&writer, 3.3);
        // c2numpy_string(&writer, "THREE");

            LorentzVector visible;
            for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
                if (ss::btags()[ijet].pt() > 40.) continue;
                visible += ss::btags()[ijet];
            }
            for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
                visible += ss::jets()[ijet];
            }
            visible += ss::lep1_p4();
            visible += ss::lep2_p4();
            if (nleps > 2) visible += ss::lep3_p4();
            float mvis = visible.M();
            // float mtvis = MT(visible.pt(), visible.phi(), ss::met(), ss::metPhi());
            float mtvis = visible.Mt();

            float pt1 = lep1_pt;
            float pt2 = lep2_pt;
            float pt3 = lep3_pt;
            float pto1 = max(max(pt1,pt2),pt3);
            float pto3 = min(min(pt1,pt2),pt3);
            float pto2 = pt1+pt2+pt3-pto1-pto3;
            int mytype = ss::hyp_type();
            if (mytype==2 && abs(ss::lep1_id())==13) mytype = 1;

            float ptratio_1 = -1.;
            float ptratio_2 = -1.;
            if (
                    (istttt || isttW || isttH || isttZ || istt || tightfake || ss::is_real_data())
                    && !(isttZ && ss::scale1fb() < 0.0001998 && ss::scale1fb() > 0.0001996) // skip 80x ttzlow sample by scale1fb
                    ) {
                ptratio_1 = ss::lep1_closeJet().pt() > 0 ? ss::lep1_p4().pt()/ss::lep1_closeJet().pt() : 1.0;
                ptratio_2 = ss::lep2_closeJet().pt() > 0 ? ss::lep2_p4().pt()/ss::lep2_closeJet().pt() : 1.0;
            }
            bool lep1_fail_ptratio = (ptratio_1 >= 0.) && (ptratio_1 < (abs(ss::lep1_id()) == 11 ? 0.80 : 0.76));
            bool lep2_fail_ptratio = (ptratio_2 >= 0.) && (ptratio_2 < (abs(ss::lep2_id()) == 11 ? 0.80 : 0.76));

            int SR = signalRegionTest(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, lep3_pt, nleps, isClass6);

            // JEC
            float mll = (ss::lep1_p4()*lep1_pt/ss::lep1_p4().pt()+ss::lep2_p4()*lep2_pt/ss::lep2_p4().pt()).M();
            float mtl1_unc_up = MT(lep1_pt, ss::lep1_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
            float mtl2_unc_up = MT(lep2_pt, ss::lep2_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
            float mtmin_unc_up = mtl1_unc_up > mtl2_unc_up ? mtl2_unc_up : mtl1_unc_up;
            float mtl1_unc_dn = MT(lep1_pt, ss::lep1_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
            float mtl2_unc_dn = MT(lep2_pt, ss::lep2_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
            float mtmin_unc_dn = mtl1_unc_dn > mtl2_unc_dn ? mtl2_unc_dn : mtl1_unc_dn;
            int SR_unc_up = signalRegionTest(ss::njets_unc_up(), ss::nbtags_unc_up(), ss::met_unc_up(), ss::ht_unc_up(), mtmin_unc_up, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, lep3_pt,  nleps, isClass6);
            int SR_unc_dn = signalRegionTest(ss::njets_unc_dn(), ss::nbtags_unc_dn(), ss::met_unc_dn(), ss::ht_unc_dn(), mtmin_unc_dn, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, lep3_pt,  nleps, isClass6);

            // JER
            float mtl1_JER_up = 0.;
            float mtl2_JER_up = 0.;
            float mtmin_JER_up = 0.;
            float mtl1_JER_dn = 0.;
            float mtl2_JER_dn = 0.;
            float mtmin_JER_dn = 0.;
            int SR_JER_up = SR;
            int SR_JER_dn = SR;
            if (doJER && !ss::is_real_data()) {
                mtl1_JER_up = MT(lep1_pt, ss::lep1_p4().phi(), ss::met_JER_up(), ss::metPhi_JER_up());
                mtl2_JER_up = MT(lep2_pt, ss::lep2_p4().phi(), ss::met_JER_up(), ss::metPhi_JER_up());
                mtmin_JER_up = mtl1_JER_up > mtl2_JER_up ? mtl2_JER_up : mtl1_JER_up;
                mtl1_JER_dn = MT(lep1_pt, ss::lep1_p4().phi(), ss::met_JER_dn(), ss::metPhi_JER_dn());
                mtl2_JER_dn = MT(lep2_pt, ss::lep2_p4().phi(), ss::met_JER_dn(), ss::metPhi_JER_dn());
                mtmin_JER_dn = mtl1_JER_dn > mtl2_JER_dn ? mtl2_JER_dn : mtl1_JER_dn;
                SR_JER_up = signalRegionTest(ss::njets_JER_up(), ss::nbtags_JER_up(), ss::met_JER_up(), ss::ht_JER_up(), mtmin_JER_up, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, lep3_pt,  nleps, isClass6);
                SR_JER_dn = signalRegionTest(ss::njets_JER_dn(), ss::nbtags_JER_dn(), ss::met_JER_dn(), ss::ht_JER_dn(), mtmin_JER_dn, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, lep3_pt,  nleps, isClass6);
            }


            float mvavalue = -10.;
            float mvavalueup = -10.;
            float mvavaluedn = -10.;
            float mvavalueJERup = -10.;
            float mvavalueJERdn = -10.;
            float pred = -1.;
            // if ((doBDT || outputTrainingBDT) && (!isData)) {
            if (doBDT || outputTrainingBDT) {
                // for ttW
                int ngenb = 0;
                int ngenc = 0;
                int ngenlight = 0;
                std::vector<LorentzVector> bjets;
                std::vector<LorentzVector> jets;
                float htb = -1.;
                for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
                    htb += ss::btags()[ijet].pt();
                    bjets.push_back(ss::btags()[ijet]);
                }
                int nlb40 = 0;
                int nmb40 = 0;
                int ntb40 = 0;
                float ht_first4 = 0.;
                float ht_rest = 0.;
                float maxmjoverpt = 0.;
                float maxmjoverpt_high = 0.;
                for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
                    float mjoverpt = ss::jets()[ijet].M()/ss::jets()[ijet].pt();
                    if (mjoverpt > maxmjoverpt) {
                        maxmjoverpt = mjoverpt;
                    }
                    if (ss::jets_disc().at(ijet) > bloose) nlb40++;
                    if (ss::jets_disc().at(ijet) > bmed)   nmb40++;
                    if (ss::jets_disc().at(ijet) > btight) ntb40++;
                    if (ijet < 4) ht_first4 += ss::jets()[ijet].pt();
                    else ht_rest += ss::jets()[ijet].pt();
                    // skip b-tagged jets
                    if (ss::jets_disc().at(ijet) > bmed) continue;
                    jets.push_back(ss::jets()[ijet]);
                }
                int wcands = 0;
                // int topcands = 0;
                for (unsigned int i = 0; i < jets.size(); i++) {
                    for (unsigned int j = i+1; j < jets.size(); j++) {
                        float wmass = (jets[i]+jets[j]).M();
                        if (fabs(wmass-80.4) < 20.0) {
                            wcands++;
                        }
                    }
                }
                tree_event = ss::event();
                tree_run = ss::run();
                tree_lumi = ss::lumi();
                tree_name = chainTitle;

                tree_stype = -1;
                if (isttW) tree_stype = 0;
                if (isttZ) tree_stype = 1;
                if (isttH) tree_stype = 2;
                if (tree_name == "fakes_mc") tree_stype = 3;
                if (tree_name == "xg") tree_stype = 4;
                if (tree_name == "ttvv") tree_stype = 5;
                if (tree_name == "rares") tree_stype = 6;
                if (tree_name == "tttt") tree_stype = 7;

                tree_weight = weight;
                tree_SR = SR;
                tree_l1id = ss::lep1_id();
                tree_l2id = ss::lep2_id();
                tree_nbtags = ss::nbtags();
                tree_njets = ss::njets();
                tree_nleps = nleps;
                tree_wcands = wcands;
                tree_met = ss::met();
                tree_ht = ss::ht();
                tree_htb = htb;
                tree_mt1 = mtl1;
                tree_mt2 = mtl2;
                tree_dphil1j1 = DeltaPhi(ss::lep1_p4().phi(),ss::jets().at(0).phi());
                tree_dphil2j2 = DeltaPhi(ss::lep2_p4().phi(),ss::jets().at(1).phi());
                tree_dphil1j2 = DeltaPhi(ss::lep1_p4().phi(),ss::jets().at(1).phi());
                tree_dphil2j1 = DeltaPhi(ss::lep2_p4().phi(),ss::jets().at(0).phi());
                tree_dphil1l2 = DeltaPhi(ss::lep1_p4().phi(),ss::lep2_p4().phi());
                tree_dphij1j2 = DeltaPhi(ss::jets().at(0).phi(),ss::jets().at(1).phi());
                tree_dphil1met = DeltaPhi(ss::lep1_p4().phi(),ss::metPhi());
                tree_dphil2met = DeltaPhi(ss::lep2_p4().phi(),ss::metPhi());
                tree_dphij1met = DeltaPhi(ss::jets().at(0).phi(),ss::metPhi());
                tree_dphij2met = DeltaPhi(ss::jets().at(1).phi(),ss::metPhi());
                tree_detal1j1 = fabs(ss::lep1_p4().eta()-ss::jets().at(0).eta());
                tree_detal2j2 = fabs(ss::lep2_p4().eta()-ss::jets().at(1).eta());
                tree_detal1j2 = fabs(ss::lep1_p4().eta()-ss::jets().at(1).eta());
                tree_detal2j1 = fabs(ss::lep2_p4().eta()-ss::jets().at(0).eta());
                tree_detal1l2 = fabs(ss::lep1_p4().eta()-ss::lep2_p4().eta());
                tree_detaj1j2 = fabs(ss::jets().at(0).eta()-ss::jets().at(1).eta());
                tree_ml1j1 = (ss::lep1_p4()+ss::jets().at(0)).M();
                tree_ml2j2 = (ss::lep2_p4()+ss::jets().at(1)).M();
                tree_ml1j2 = (ss::lep1_p4()+ss::jets().at(1)).M();
                tree_ml2j1 = (ss::lep2_p4()+ss::jets().at(0)).M();
                tree_ml1l2 = (ss::lep1_p4()+ss::lep2_p4()).M();
                tree_mj1j2 = (ss::jets().at(0)+ss::jets().at(1)).M();
                tree_ptl1 = ss::lep1_p4().pt();
                tree_ptl2 = ss::lep2_p4().pt();
                tree_ptl3 = (nleps >= 3) ? ss::lep3_p4().pt() : 0.;
                tree_ptj1 = ss::jets().at(0).pt();
                tree_ptj2 = ss::jets().at(1).pt();
                tree_ptj3 = (ss::jets().size() > 2) ? ss::jets().at(2).pt() : 0.;
                tree_ptj4 = (ss::jets().size() > 3) ? ss::jets().at(3).pt() : 0.;
                tree_ptj5 = (ss::jets().size() > 4) ? ss::jets().at(4).pt() : 0.;
                tree_ptj6 = (ss::jets().size() > 5) ? ss::jets().at(5).pt() : 0.;
                tree_ptj7 = (ss::jets().size() > 6) ? ss::jets().at(6).pt() : 0.;
                tree_ptj8 = (ss::jets().size() > 7) ? ss::jets().at(7).pt() : 0.;
                // tree_stb = stb;
                tree_q1 = (ss::lep1_id()<0)-(ss::lep1_id()>0);
                tree_q2 = (ss::lep2_id()<0)-(ss::lep2_id()>0);
                tree_q3 = nleps >= 3 ? ((ss::lep3_id()<0)-(ss::lep3_id()>0)) : 0;
                tree_ht4ratio = ss::njets()>4 ? ht_first4/ht_rest : 0;
                tree_maxmjoverpt = maxmjoverpt;
                tree_nlb40 = nlb40;
                tree_nmb40 = nmb40;
                tree_ntb40 = ntb40;
                // float variants for MVA
                tree_f_njets = tree_njets;
                tree_f_nbtags = tree_nbtags;
                tree_f_nleps = tree_nleps;
                tree_f_wcands = tree_wcands;
                tree_f_nlb40 = tree_nlb40;
                tree_f_nmb40 = tree_nmb40;
                tree_f_ntb40 = tree_ntb40;
                tree_f_q1 = tree_q1;
                tree_f_q2 = tree_q2;
                // // fill discriminant before requiring SR
                pred = get_prediction((float)tree_njets,(float)tree_nbtags,(float)tree_mt1,(float)tree_mt2,(float)tree_met,(float)tree_ml1l2,(float)tree_htb,(float)tree_nleps,(float)tree_ht,(float)tree_mj1j2,(float)tree_dphij1j2,(float)tree_ptj1,(float)tree_ptj2,(float)tree_ml1j2,(float)tree_ml1j1,(float)tree_wcands,(float)tree_dphil1j1,(float)tree_detal1l2,(float)tree_nlb40,(float)tree_nmb40,(float)tree_ntb40,(float)tree_q1,(float)tree_q2,(float)tree_ht4ratio);
                if (outputTrainingBDT && (!isData || includeDataInBDTTree)) out_tree->Fill();
                mvavalue = reader->EvaluateMVA("BDT");
                tree_disc = mvavalue;

                tree_njets = ss::njets_unc_up();
                tree_nbtags = ss::nbtags_unc_up();
                tree_met = ss::met_unc_up();
                tree_ht = ss::ht_unc_up();
                tree_mt1 = MT(lep1_pt, ss::lep1_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
                tree_mt2 = MT(lep2_pt, ss::lep2_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
                mvavalueup = reader->EvaluateMVA("BDT");

                tree_njets = ss::njets_unc_dn();
                tree_nbtags = ss::nbtags_unc_dn();
                tree_met = ss::met_unc_dn();
                tree_ht = ss::ht_unc_dn();
                tree_mt1 = MT(lep1_pt, ss::lep1_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
                tree_mt2 = MT(lep2_pt, ss::lep2_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
                mvavaluedn = reader->EvaluateMVA("BDT");


                tree_njets = ss::njets_JER_up();
                tree_nbtags = ss::nbtags_JER_up();
                tree_met = ss::met_JER_up();
                tree_ht = ss::ht_JER_up();
                mvavalueJERup = reader->EvaluateMVA("BDT");

                tree_njets = ss::njets_JER_dn();
                tree_nbtags = ss::nbtags_JER_dn();
                tree_met = ss::met_JER_dn();
                tree_ht = ss::ht_JER_dn();
                mvavalueJERdn = reader->EvaluateMVA("BDT");

                // transform preds
                pred = min(pred,0.79F);
                p_result.h_disc2.br->Fill(pred,weight); //  0. 0.25 0.35 0.65
                // float upper = 0.8F;
                // float lower = -0.4F;
                // mvavalue = ((max(min((float)mvavalue,upper-0.01F),lower)-lower)/(upper-lower));
                p_result.h_disc.br->Fill(mvavalue,weight);
            }

            // int SRdisc = mvavalue*10+1;
            int SRdisc = 1;
            int SRdiscup = 1;
            int SRdiscdn = 1;
            int SRdiscJERup = 1;
            int SRdiscJERdn = 1;

            SRdisc = getBDTBin(mvavalue, isClass6);
            SRdiscup = getBDTBin(mvavalueup, isClass6);
            SRdiscdn = getBDTBin(mvavaluedn, isClass6);
            SRdiscJERup = getBDTBin(mvavalueJERup, isClass6);
            SRdiscJERdn = getBDTBin(mvavalueJERdn, isClass6);

            int SRdisc_unc_up = SRdiscup;
            int SRdisc_unc_dn = SRdiscdn;
            int SRdisc_JER_up = SRdiscJERup;
            int SRdisc_JER_dn = SRdiscJERdn;

            if (isData  == 0 && SR >= 0 && SR_unc_up > 0)            p_jes_alt_up.SRCR.TOTAL->Fill(SR_unc_up, weight);
            if (isData  == 0 && SR >= 0 && SR_unc_dn > 0)            p_jes_alt_dn.SRCR.TOTAL->Fill(SR_unc_dn, weight);

            if (isData  == 0 && SR >= 0 && SR_JER_up > 0)            p_jer_alt_up.SRCR.TOTAL->Fill(SR_JER_up, weight);
            if (isData  == 0 && SR >= 0 && SR_JER_dn > 0)            p_jer_alt_dn.SRCR.TOTAL->Fill(SR_JER_dn, weight);

            if (isData  == 0 && SR >= 0 && SRdisc_unc_up > 0)            p_jes_alt_up.SRDISC.TOTAL->Fill(SRdisc_unc_up, weight);
            if (isData  == 0 && SR >= 0 && SRdisc_unc_dn > 0)            p_jes_alt_dn.SRDISC.TOTAL->Fill(SRdisc_unc_dn, weight);

            if (isData  == 0 && SR >= 0 && SRdisc_JER_up > 0)            p_jer_alt_up.SRDISC.TOTAL->Fill(SRdisc_JER_up, weight);
            if (isData  == 0 && SR >= 0 && SRdisc_JER_dn > 0)            p_jer_alt_dn.SRDISC.TOTAL->Fill(SRdisc_JER_dn, weight);

            // require SRs+CR (= ttZ CR + ttW CR + SRs)
            if (!outputTrainingBDT) {
                if (SR < 0) continue;
            }


            // if (doSync && SR == 2 && isttW && ss::hyp_class() == 3) {
            //     std::cout << Form("%1d %9d %llu\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\n",
            //             ss::run(), ss::lumi(), ss::event(),
            //             ss::lep1_id(), lep1_pt,
            //             ss::lep2_id(), lep2_pt,
            //             ss::njets(), ss::nbtags(), ss::met(), ss::ht()) << std::endl;
            // }

            // if (doSync && SR == 2 && ss::is_real_data() && isData && ss::hyp_class() == 3) { // signal leptons
            //     std::cout << Form("%1d %9d %llu\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\n",
            //             ss::run(), ss::lumi(), ss::event(),
            //             ss::lep1_id(), lep1_pt,
            //             ss::lep2_id(), lep2_pt,
            //             ss::njets(), ss::nbtags(), ss::met(), ss::ht()) << std::endl;
            // }

            // // fake app
            // if (doSync && SR == 2 && ss::is_real_data() && doFakes && (ss::hyp_class() == 1 || ss::hyp_class() == 2 || (ss::hyp_class()==6 && isClass6Fake))) {
            //     std::cout << Form("%1d %9d %llu\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%1d\t%1d\t%1d\n",
            //             ss::run(), ss::lumi(), ss::event(),
            //             ss::lep1_id(), lep1_pt,
            //             ss::lep2_id(), lep2_pt,
            //             ss::njets(), ss::nbtags(), ss::met(), ss::ht(),
            //             ss::lep1_passes_id(), ss::lep2_passes_id(), ss::lep3_passes_id()) << std::endl;
            // }

            // if (isXgamma && SR > 2) {
            //     std::cout << " filename: " << filename << " chainTitle: " << chainTitle << " weight: " << weight << " SR: " << SR << std::endl;
            // }


            // truth match the third lepton
            if (!isData) {
              p_result.h_mid1.br->Fill(ss::lep1_motherID(), weight);
              p_result.h_mid2.br->Fill(ss::lep2_motherID(), weight);
              if (ss::lep3_passes_id()) p_result.h_mid3.br->Fill(ss::lep3_motherID(), weight);
            }



            //These c-s errors
            if ((istttt || isHiggsScan) && makeGenVariationsMC && ss::genweights().size()>110) {
                float nom = ss::genweights().at(0);
                // float pdfrms = 0.;
                float scale_up = ss::genweights().at(4)/nom;
                float scale_dn = ss::genweights().at(8)/nom;
                float alphas_up = ss::genweights().at(110)/nom;
                float alphas_dn = ss::genweights().at(109)/nom;
                // TH1F *h_pdf = new TH1F("pdf", "pdf", 1000, 0, 10);
                // for (int i = 0; i < 100; i++) pdf += ss::genweights().at(i+10);
                // pdf /= (100*nom);
                // for (int i = 0; i < 100; i++) h_pdf->Fill(ss::genweights().at(i+10)*weight);
                // for (int i = 0; i < 100; i++) h_pdf->Fill(ss::genweights().at(i+10)*weight);
                // pdfrms = h_pdf->GetRMS()*sqrt(h_pdf->GetEntries());
                // delete h_pdf;
                p_scale_alt_up.SRCR.TOTAL->Fill(SR,    scale_up*weight);
                p_scale_alt_dn.SRCR.TOTAL->Fill(SR,    scale_dn*weight);
                p_scale_alt_up.SRDISC.TOTAL->Fill(SRdisc,    scale_up*weight);
                p_scale_alt_dn.SRDISC.TOTAL->Fill(SRdisc,    scale_dn*weight);
                p_alphas_alt_up.SRCR.TOTAL->Fill(SR,    alphas_up*weight);
                p_alphas_alt_dn.SRCR.TOTAL->Fill(SR,    alphas_dn*weight);
                p_alphas_alt_up.SRDISC.TOTAL->Fill(SRdisc,    alphas_up*weight);
                p_alphas_alt_dn.SRDISC.TOTAL->Fill(SRdisc,    alphas_dn*weight);
                // p_pdf_alt_up.SRCR.TOTAL->Fill(SR,      pdfrms);
            }

            p_result.SRCR.TOTAL->Fill(SR, weight);

            p_result.SRDISC.TOTAL->Fill(SRdisc, weight);

            // fill systematics hists
            if (doFakes == 1 )            p_fake_alt_up.SRCR.TOTAL->Fill(SR, weight_alt_FR);
            if (isData  == 0 )            p_btagSF_alt_up.SRCR.TOTAL->Fill(SR, weight_btag_up_alt);
            if (isData  == 0 )            p_btagSF_alt_dn.SRCR.TOTAL->Fill(SR, weight_btag_dn_alt);
            if (isData  == 0 )            p_pu_alt_up.SRCR.TOTAL->Fill(SR, weight_pu_up_alt);
            if (isData  == 0 )            p_pu_alt_dn.SRCR.TOTAL->Fill(SR, weight_pu_dn_alt);
            if (isData  == 0 )            p_isr_alt_up.SRCR.TOTAL->Fill(SR, weight_isr_up_alt);
            if (isData  == 0 )            p_bb_alt_up.SRCR.TOTAL->Fill(SR, weight_bb_up_alt);
            if (isData  == 0 )            p_lep_alt_up.SRCR.TOTAL->Fill(SR, weight_lep_up_alt);

            if (doFakes == 1 )            p_fake_alt_up.SRDISC.TOTAL->Fill(SRdisc, weight_alt_FR);
            if (isData  == 0 )            p_btagSF_alt_up.SRDISC.TOTAL->Fill(SRdisc, weight_btag_up_alt);
            if (isData  == 0 )            p_btagSF_alt_dn.SRDISC.TOTAL->Fill(SRdisc, weight_btag_dn_alt);
            if (isData  == 0 )            p_pu_alt_up.SRDISC.TOTAL->Fill(SRdisc, weight_pu_up_alt);
            if (isData  == 0 )            p_pu_alt_dn.SRDISC.TOTAL->Fill(SRdisc, weight_pu_dn_alt);
            if (isData  == 0 )            p_isr_alt_up.SRDISC.TOTAL->Fill(SRdisc, weight_isr_up_alt);
            if (isData  == 0 )            p_bb_alt_up.SRDISC.TOTAL->Fill(SRdisc, weight_bb_up_alt);
            if (isData  == 0 )            p_lep_alt_up.SRDISC.TOTAL->Fill(SRdisc, weight_lep_up_alt);

            bool plotlep3 = ss::lep3_passes_id() && lep3_pt > 20.;

            if (SR > 1) { // non ttZ CR
                p_result.h_njets.br->Fill(ss::njets() , weight);
                p_result.h_nbtags.br->Fill(ss::nbtags() , weight);
                p_result.h_type.br->Fill(mytype , weight);
                p_result.h_nvtx.br->Fill(ss::nGoodVertices() , weight);
                p_result.h_charge.br->Fill(ss::lep1_id() > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.br->Fill(nleps , weight);
                p_result.h_ht.br->Fill(ss::ht() , weight);
                p_result.h_met.br->Fill(ss::met() , weight);
                p_result.h_mvis.br->Fill(mvis , weight);
                p_result.h_mtvis.br->Fill(mtvis , weight);
                p_result.h_mll.br->Fill(mll , weight);
                p_result.h_mtmin.br->Fill(mtmin , weight);
                p_result.h_l1pt.br->Fill(pto1, weight);
                p_result.h_l2pt.br->Fill(pto2, weight);
                (abs(ss::lep1_id()) == 11) ? p_result.h_el_l1pt.br->Fill(pto1, weight) : p_result.h_mu_l1pt.br->Fill(pto1, weight);
                (abs(ss::lep2_id()) == 11) ? p_result.h_el_l2pt.br->Fill(pto2, weight) : p_result.h_mu_l2pt.br->Fill(pto2, weight);
                if (abs(ss::lep1_id()) == 11) p_result.h_el_nmiss.br->Fill(ss::lep1_el_exp_innerlayers(), weight);
                if (abs(ss::lep2_id()) == 11) p_result.h_el_nmiss.br->Fill(ss::lep2_el_exp_innerlayers(), weight);
                if (plotlep3) {
                    p_result.h_l3pt.br->Fill(pto3, weight);
                    (abs(ss::lep3_id()) == 11) ? p_result.h_el_l3pt.br->Fill(pto3, weight) : p_result.h_mu_l3pt.br->Fill(pto3, weight);
                }

                for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
                    p_result.h_bjetpt.br->Fill(ss::btags()[ijet].pt(),weight);
                }
                for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
                    // if (ss::jets_disc()[ijet] > bmed) continue;
                    p_result.h_jetpt.br->Fill(ss::jets()[ijet].pt(),weight);
                }

                if (abs(ss::lep1_id()) == 11) p_result.h_mva.br->Fill(ss::lep1_MVA()                                                     , weight);
                if(abs(ss::lep1_id()) == 13) {
                    p_result.h_mu_sip3d_lep1.br  ->Fill(ss::lep1_sip()                                                                          , weight);
                    p_result.h_mu_l1eta.br       ->Fill(ss::lep1_p4().eta()                                                                     , weight);
                    p_result.h_mu_lep1_miniIso.br->Fill(ss::lep1_miniIso()                                                                      , weight);
                    p_result.h_mu_lep1_ptRel.br  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    if (lep1_fail_ptratio) p_result.h_mu_lep1_ptRelfail.br  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    p_result.h_mu_lep1_ptRatio.br->Fill(ptratio_1                                                                               , weight);
                } else {
                    p_result.h_el_sip3d_lep1.br  ->Fill(ss::lep1_sip()                                                                          , weight);
                    p_result.h_el_l1pt.br        ->Fill(lep1_pt                                                                                 , weight);
                    p_result.h_el_l1eta.br       ->Fill(ss::lep1_p4().eta()                                                                     , weight);
                    p_result.h_el_lep1_miniIso.br->Fill(ss::lep1_miniIso()                                                                      , weight);
                    p_result.h_el_lep1_ptRel.br  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    if (lep1_fail_ptratio) p_result.h_el_lep1_ptRelfail.br  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    p_result.h_el_lep1_ptRatio.br->Fill(ptratio_1                                                                               , weight);
                }
                if (abs(ss::lep2_id()) == 11) p_result.h_mva.br->Fill(ss::lep2_MVA()                                                     , weight);
                if(abs(ss::lep2_id()) == 13) {
                    p_result.h_mu_sip3d_lep2.br  ->Fill(ss::lep2_sip()                                                                          , weight);
                    p_result.h_mu_l2eta.br       ->Fill(ss::lep2_p4().eta()                                                                     , weight);
                    p_result.h_mu_lep2_miniIso.br->Fill(ss::lep2_miniIso()                                                                      , weight);
                    p_result.h_mu_lep2_ptRel.br  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    if (lep2_fail_ptratio) p_result.h_mu_lep2_ptRelfail.br  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    p_result.h_mu_lep2_ptRatio.br->Fill(ptratio_2                                                                               , weight);
                } else {
                    p_result.h_el_sip3d_lep2.br  ->Fill(ss::lep2_sip()                                                                          , weight);
                    p_result.h_el_l2eta.br       ->Fill(ss::lep2_p4().eta()                                                                     , weight);
                    p_result.h_el_lep2_miniIso.br->Fill(ss::lep2_miniIso()                                                                      , weight);
                    p_result.h_el_lep2_ptRel.br  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    if (lep2_fail_ptratio) p_result.h_el_lep2_ptRelfail.br  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    p_result.h_el_lep2_ptRatio.br->Fill(ptratio_2                                                                               , weight);
                }
                if (plotlep3) {
                    if(abs(ss::lep3_id()) == 13) {
                        p_result.h_mu_l3eta.br       ->Fill(ss::lep3_p4().eta()                                                                     , weight);
                    } else {
                        p_result.h_el_l3eta.br       ->Fill(ss::lep3_p4().eta()                                                                     , weight);
                    }
                }

            }

            if (SR == 1) { // ttZ CR
                p_result.h_njets.ttzcr->Fill(ss::njets() , weight);
                p_result.h_nbtags.ttzcr->Fill(ss::nbtags() , weight);
                p_result.h_type.ttzcr->Fill(mytype , weight);
                p_result.h_charge.ttzcr->Fill(ss::lep1_id() > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.ttzcr->Fill(nleps , weight);
                p_result.h_ht.ttzcr->Fill(ss::ht() , weight);
                p_result.h_met.ttzcr->Fill(ss::met() , weight);
                p_result.h_mvis.ttzcr->Fill(mvis , weight);
                p_result.h_mtvis.ttzcr->Fill(mtvis , weight);
                p_result.h_mll.ttzcr->Fill(mll , weight);
                p_result.h_mtmin.ttzcr->Fill(mtmin , weight);
                p_result.h_l1pt.ttzcr->Fill(pto1, weight);
                p_result.h_l2pt.ttzcr->Fill(pto2, weight);
                (abs(ss::lep1_id()) == 11) ? p_result.h_el_l1pt.ttzcr->Fill(pto1, weight) : p_result.h_mu_l1pt.ttzcr->Fill(pto1, weight);
                (abs(ss::lep2_id()) == 11) ? p_result.h_el_l2pt.ttzcr->Fill(pto2, weight) : p_result.h_mu_l2pt.ttzcr->Fill(pto2, weight);
                if (plotlep3) {
                    float mllos1 = (ss::lep1_p4()*lep1_pt/ss::lep1_p4().pt()+ss::lep3_p4()*lep3_pt/ss::lep3_p4().pt()).M();
                    float mllos2 = (ss::lep2_p4()*lep2_pt/ss::lep2_p4().pt()+ss::lep3_p4()*lep3_pt/ss::lep3_p4().pt()).M();
                    float mllos = fabs(mllos1 - 91.2) < fabs(mllos2 - 91.2) ? mllos1 : mllos2;
                    p_result.h_mllos.ttzcr->Fill(mllos, weight);
                    p_result.h_l3pt.ttzcr->Fill(pto3, weight);
                    (abs(ss::lep3_id()) == 11) ? p_result.h_el_l3pt.ttzcr->Fill(pto3, weight) : p_result.h_mu_l3pt.ttzcr->Fill(pto3, weight);
                }
            }

            if (SR == 2) { // ttW CR
                p_result.h_njets.ttwcr->Fill(ss::njets() , weight);
                p_result.h_nbtags.ttwcr->Fill(ss::nbtags() , weight);
                p_result.h_type.ttwcr->Fill(mytype , weight);
                p_result.h_charge.ttwcr->Fill(ss::lep1_id() > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.ttwcr->Fill(nleps , weight);
                p_result.h_ht.ttwcr->Fill(ss::ht() , weight);
                p_result.h_met.ttwcr->Fill(ss::met() , weight);
                p_result.h_mvis.ttwcr->Fill(mvis , weight);
                p_result.h_mtvis.ttwcr->Fill(mtvis , weight);
                p_result.h_mll.ttwcr->Fill(mll , weight);
                p_result.h_mtmin.ttwcr->Fill(mtmin , weight);
                p_result.h_l1pt.ttwcr->Fill(pto1, weight);
                p_result.h_l2pt.ttwcr->Fill(pto2, weight);
                (abs(ss::lep1_id()) == 11) ? p_result.h_el_l1pt.ttwcr->Fill(pto1, weight) : p_result.h_mu_l1pt.ttwcr->Fill(pto1, weight);
                (abs(ss::lep2_id()) == 11) ? p_result.h_el_l2pt.ttwcr->Fill(pto2, weight) : p_result.h_mu_l2pt.ttwcr->Fill(pto2, weight);
                if (plotlep3) {
                    p_result.h_l3pt.ttwcr->Fill(pto3, weight);
                    (abs(ss::lep3_id()) == 11) ? p_result.h_el_l3pt.ttwcr->Fill(pto3, weight) : p_result.h_mu_l3pt.ttwcr->Fill(pto3, weight);
                }
            }

            if (SR > nCR) {
            // if (SR-nCR == 4) { // FIXME FIXME
                p_result.h_njets.sr->Fill(ss::njets() , weight);
                p_result.h_nbtags.sr->Fill(ss::nbtags() , weight);
                p_result.h_type.sr->Fill(mytype , weight);
                p_result.h_charge.sr->Fill(ss::lep1_id() > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.sr->Fill(nleps , weight);
                p_result.h_ht.sr->Fill(ss::ht() , weight);
                p_result.h_met.sr->Fill(ss::met() , weight);
                p_result.h_mvis.sr->Fill(mvis , weight);
                p_result.h_mtvis.sr->Fill(mtvis , weight);
                auto firsttwotops = getFirstTwoTops();

                if (false) {
                    auto trijet_discpair = getTrijetDiscs(reader_toptag);
                    p_result.h_topdisc1.sr->Fill(trijet_discpair.first, weight);
                    p_result.h_topdisc2.sr->Fill(trijet_discpair.second, weight);
                    tree_topdisc1 = trijet_discpair.first;
                    tree_topdisc2 = trijet_discpair.second;
                }

                // auto ntopinfo = getNtops();
                // p_result.h_ntops.sr->Fill(ntopinfo.first , weight);
                // p_result.h_ntopness.sr->Fill(ntopinfo.second , weight);
                if (firsttwotops.first > 0.) p_result.h_mtop1.sr->Fill(firsttwotops.first, weight);
                if (firsttwotops.second > 0.) p_result.h_mtop2.sr->Fill(firsttwotops.second, weight);
                p_result.h_mll.sr->Fill(mll , weight);
                p_result.h_mtmin.sr->Fill(mtmin , weight);
                p_result.h_l1pt.sr->Fill(pto1, weight);
                p_result.h_l2pt.sr->Fill(pto2, weight);
                (abs(ss::lep1_id()) == 11) ? p_result.h_el_l1pt.sr->Fill(pto1, weight) : p_result.h_mu_l1pt.sr->Fill(pto1, weight);
                (abs(ss::lep2_id()) == 11) ? p_result.h_el_l2pt.sr->Fill(pto2, weight) : p_result.h_mu_l2pt.sr->Fill(pto2, weight);
                if (plotlep3) {
                    p_result.h_l3pt.sr->Fill(pto3, weight);
                    (abs(ss::lep3_id()) == 11) ? p_result.h_el_l3pt.sr->Fill(pto3, weight) : p_result.h_mu_l3pt.sr->Fill(pto3, weight);
                }
                //Fill SR plot
                p_result.SR.TOTAL->Fill(SR-nCR, weight);

                for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
                    p_result.h_bjetpt.sr->Fill(ss::btags()[ijet].pt(),weight);
                }
                for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
                    // if (ss::jets_disc()[ijet] > bmed) continue;
                    p_result.h_jetpt.sr->Fill(ss::jets()[ijet].pt(),weight);
                }

                if (abs(ss::lep1_id()) == 11) p_result.h_mva.sr->Fill(ss::lep1_MVA()                                                     , weight);
                if(abs(ss::lep1_id()) == 13) {
                    p_result.h_mu_sip3d_lep1.sr  ->Fill(ss::lep1_sip()                                                                          , weight);
                    p_result.h_mu_l1eta.sr       ->Fill(ss::lep1_p4().eta()                                                                     , weight);
                    p_result.h_mu_lep1_miniIso.sr->Fill(ss::lep1_miniIso()                                                                      , weight);
                    p_result.h_mu_lep1_ptRel.sr  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    if (lep1_fail_ptratio) p_result.h_mu_lep1_ptRelfail.sr  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    p_result.h_mu_lep1_ptRatio.sr->Fill(ptratio_1                                                                               , weight);
                } else {
                    p_result.h_el_sip3d_lep1.sr  ->Fill(ss::lep1_sip()                                                                          , weight);
                    p_result.h_el_l1eta.sr       ->Fill(ss::lep1_p4().eta()                                                                     , weight);
                    p_result.h_el_lep1_miniIso.sr->Fill(ss::lep1_miniIso()                                                                      , weight);
                    p_result.h_el_lep1_ptRel.sr  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    if (lep1_fail_ptratio) p_result.h_el_lep1_ptRelfail.sr  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    p_result.h_el_lep1_ptRatio.sr->Fill(ptratio_1                                                                               , weight);
                }
                if (abs(ss::lep2_id()) == 11) p_result.h_mva.sr->Fill(ss::lep2_MVA()                                                     , weight);
                if(abs(ss::lep2_id()) == 13) {
                    p_result.h_mu_sip3d_lep2.sr  ->Fill(ss::lep2_sip()                                                                          , weight);
                    p_result.h_mu_l2eta.sr       ->Fill(ss::lep2_p4().eta()                                                                     , weight);
                    p_result.h_mu_lep2_miniIso.sr->Fill(ss::lep2_miniIso()                                                                      , weight);
                    p_result.h_mu_lep2_ptRel.sr  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    if (lep2_fail_ptratio) p_result.h_mu_lep2_ptRelfail.sr  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    p_result.h_mu_lep2_ptRatio.sr->Fill(ptratio_2                                                                               , weight);
                } else {
                    p_result.h_el_sip3d_lep2.sr  ->Fill(ss::lep2_sip()                                                                          , weight);
                    p_result.h_el_l2eta.sr       ->Fill(ss::lep2_p4().eta()                                                                     , weight);
                    p_result.h_el_lep2_miniIso.sr->Fill(ss::lep2_miniIso()                                                                      , weight);
                    p_result.h_el_lep2_ptRel.sr  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    if (lep2_fail_ptratio) p_result.h_el_lep2_ptRelfail.sr  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    p_result.h_el_lep2_ptRatio.sr->Fill(ptratio_2                                                                               , weight);
                }
                if (plotlep3) {
                    if(abs(ss::lep3_id()) == 13) {
                        p_result.h_mu_l3eta.sr       ->Fill(ss::lep3_p4().eta()                                                                     , weight);
                    } else {
                        p_result.h_el_l3eta.sr       ->Fill(ss::lep3_p4().eta()                                                                     , weight);
                    }
                }

                // if (nleps > 2) {
                //     int q1 = 2*(ss::lep1_id() > 0) - 1;
                //     int q3 = 2*(ss::lep3_id() > 0) - 1;
                //     p_result.h_charge3.sr->Fill(q3==q1 ? 1.5: 0.5 , weight);
                // }

            }


        }//event loop
        file->Close();
    }//file loop

        if (!isData) {
            avoidNegativeYields(p_btagSF_alt_up.SRCR.TOTAL);
            avoidNegativeYields(p_btagSF_alt_dn.SRCR.TOTAL);
            avoidNegativeYields(p_btagSF_alt_up.SRDISC.TOTAL);
            avoidNegativeYields(p_btagSF_alt_dn.SRDISC.TOTAL);
            avoidNegativeYields(p_pu_alt_up.SRCR.TOTAL);
            avoidNegativeYields(p_pu_alt_dn.SRCR.TOTAL);
            avoidNegativeYields(p_pu_alt_up.SRDISC.TOTAL);
            avoidNegativeYields(p_pu_alt_dn.SRDISC.TOTAL);
            avoidNegativeYields(p_isr_alt_up.SRCR.TOTAL);
            avoidNegativeYields(p_isr_alt_dn.SRCR.TOTAL);
            avoidNegativeYields(p_isr_alt_up.SRDISC.TOTAL);
            avoidNegativeYields(p_isr_alt_dn.SRDISC.TOTAL);
            avoidNegativeYields(p_lep_alt_up.SRCR.TOTAL);
            avoidNegativeYields(p_lep_alt_dn.SRCR.TOTAL);
            avoidNegativeYields(p_lep_alt_up.SRDISC.TOTAL);
            avoidNegativeYields(p_lep_alt_dn.SRDISC.TOTAL);
            avoidNegativeYields(p_result.SRCR.TOTAL);
            avoidNegativeYields(p_result.SRDISC.TOTAL);
            avoidNegativeYields(p_result.SR.TOTAL);
            avoidNegativeYields(p_jes_alt_up.SRCR.TOTAL);
            avoidNegativeYields(p_jes_alt_dn.SRCR.TOTAL);
            avoidNegativeYields(p_jes_alt_up.SRDISC.TOTAL);
            avoidNegativeYields(p_jes_alt_dn.SRDISC.TOTAL);
            avoidNegativeYields(p_jer_alt_up.SRCR.TOTAL);
            avoidNegativeYields(p_jer_alt_dn.SRCR.TOTAL);
            avoidNegativeYields(p_jer_alt_up.SRDISC.TOTAL);
            avoidNegativeYields(p_jer_alt_dn.SRDISC.TOTAL);
        }

    if (makeRootFiles) {

        TString kinRegs[] = {"srcr", "srdisc"};
        int nk = 2;

        for (int kr = 0; kr<nk;kr++) {

            string name = chainTitle.Data();

            TFile *fileOut = TFile::Open(Form("../limits/%s/%s_histos_%s_%.1fifb.root",dir.Data(),name.c_str(),kinRegs[kr].Data(),lumiAG),"RECREATE");
            TH1F* plot = 0;
            if (kinRegs[kr]=="srcr") plot=p_result.SRCR.TOTAL;
            else if (kinRegs[kr]=="srdisc") plot=p_result.SRDISC.TOTAL;
            else exit(1);

            //hack to mitigate statistical uncertainty due to empty control regions for fakes
            if (name=="fakes") {
                //in case prediction is zero, take statistical upper limit from fakes_mc
                TFile *file_fakes_mc = TFile::Open(Form("../limits/%s/fakes_mc_histos_%s_%.1fifb.root",dir.Data(),kinRegs[kr].Data(),lumiAG),"OPEN");
                if (file_fakes_mc==0) {
                    if (!suppressWarns) cout << "warning, need fakes_mc to set stat for fakes" << endl;
                    assert(0);
                }
                TH1F* h_mc = (TH1F*) file_fakes_mc->Get("sr");
                float scaleFact = 1.2;//from T+L control region
                for (int bin=1;bin<=plot->GetNbinsX();++bin) {
                    if (plot->GetBinContent(bin)<=2E-6) {
                        if (!suppressWarns) cout << "warning: plot " << plot->GetName() << " has zero stat unc in bin " << bin << " value=" << plot->GetBinContent(bin) << "; setting to MC pred=" << scaleFact*h_mc->GetBinContent(bin) << endl;
                        plot->SetBinError(bin,scaleFact*h_mc->GetBinContent(bin));

                        // // NJA: this was only for testing on Jun23
                        // plot->SetBinError(bin,h_mc->GetBinError(bin)); 
                        // plot->SetBinContent(bin,h_mc->GetBinContent(bin)); 

                    }
                }
                file_fakes_mc->Close();
                fileOut->cd();
            }

            //write the central sr plot
            TH1F* h_sr = (TH1F*) plot->Clone("sr");
            h_sr->Write();

            //now do systematics

            //stats
            writeStat(plot,name);

            //fakes ewk
            if (doFakes == 1) {
                TH1F* plot_alt = 0;
                if      (kinRegs[kr] == "srcr")   plot_alt = p_fake_alt_up.SRCR.TOTAL;
                else if      (kinRegs[kr] == "srdisc")   plot_alt = p_fake_alt_up.SRDISC.TOTAL;
                else exit(1);
                TH1F* fakes_EWKUp    = (TH1F*) plot_alt->Clone("fakes_EWKUp");
                TH1F* fakes_EWKDown  = (TH1F*) plot_alt->Clone("fakes_EWKDown");
                fillDownMirrorUp(h_sr,fakes_EWKUp,fakes_EWKDown);

                fakes_EWKUp->Write();
                fakes_EWKDown->Write();
            }

            // signal scale
            if ((istttt || isHiggsScan) && makeGenVariationsMC) {
                TH1F* plot_alt = 0;
                if      (kinRegs[kr] == "srcr")   plot_alt = p_scale_alt_up.SRCR.TOTAL;
                else if (kinRegs[kr] == "srdisc")  plot_alt = p_scale_alt_up.SRDISC.TOTAL;
                else exit(1);
                for (int bin = 0; bin < plot_alt->GetNbinsX(); bin++){
                    float vnom = h_sr->GetBinContent(bin);
                    float vup = plot_alt->GetBinContent(bin);
                }
                TH1F* scaleUp = (TH1F*) plot_alt->Clone("scaleUp");
                avoidNegativeYields(scaleUp);
                scaleUp->Scale(h_sr->Integral()/scaleUp->Integral());
                scaleUp->Write();
                if      (kinRegs[kr] == "srcr")   plot_alt = p_scale_alt_dn.SRCR.TOTAL;
                else if      (kinRegs[kr] == "srdisc")   plot_alt = p_scale_alt_dn.SRDISC.TOTAL;
                else exit(1);
                for (int bin = 0; bin < plot_alt->GetNbinsX(); bin++){
                }
                TH1F* scaleDn = (TH1F*) plot_alt->Clone("scaleDown");
                avoidNegativeYields(scaleDn);
                scaleDn->Scale(h_sr->Integral()/scaleDn->Integral());
                scaleDn->Write();
                // signal pdf
                if      (kinRegs[kr] == "srcr")   plot_alt = p_pdf_alt_up.SRCR.TOTAL;
                else if (kinRegs[kr] == "srdisc")  plot_alt = p_pdf_alt_up.SRDISC.TOTAL;
                else exit(1);
                for (int bin = 1; bin <= plot_alt->GetNbinsX(); bin++){
                    plot_alt->SetBinContent(bin, h_sr->GetBinContent(bin)*(1+plot_alt->GetBinContent(bin)));
                }
                TH1F* pdfUp = (TH1F*) plot_alt->Clone("pdfUp");
                avoidNegativeYields(pdfUp);
                pdfUp->Scale(h_sr->Integral()/pdfUp->Integral());
                pdfUp->Write();
                TH1F* pdfDn = (TH1F*) plot_alt->Clone("pdfDown");
                fillDownMirrorUp(h_sr,pdfUp,pdfDn);
                avoidNegativeYields(pdfDn);
                pdfDn->Write();
                // alphas
                if      (kinRegs[kr] == "srcr")   plot_alt = p_alphas_alt_up.SRCR.TOTAL;
                else if (kinRegs[kr] == "srdisc")  plot_alt = p_alphas_alt_up.SRDISC.TOTAL;
                else exit(1);
                for (int bin = 0; bin < plot_alt->GetNbinsX(); bin++){
                    float vnom = h_sr->GetBinContent(bin);
                    float vup = plot_alt->GetBinContent(bin);
                }
                TH1F* alphasUp = (TH1F*) plot_alt->Clone("alphasUp");
                avoidNegativeYields(alphasUp);
                alphasUp->Scale(h_sr->Integral()/alphasUp->Integral());
                alphasUp->Write();
                if      (kinRegs[kr] == "srcr")   plot_alt = p_alphas_alt_dn.SRCR.TOTAL;
                else if      (kinRegs[kr] == "srdisc")   plot_alt = p_alphas_alt_dn.SRDISC.TOTAL;
                else exit(1);
                for (int bin = 0; bin < plot_alt->GetNbinsX(); bin++){
                }
                TH1F* alphasDn = (TH1F*) plot_alt->Clone("alphasDown");
                avoidNegativeYields(alphasDn);
                alphasDn->Scale(h_sr->Integral()/alphasDn->Integral());
                alphasDn->Write();
            }

            if (istttt && doISRFSRsyst) {
                TH1F* h_isrup = (TH1F*) h_sr->Clone("isrvarUp");
                TH1F* h_fsrup = (TH1F*) h_sr->Clone("fsrvarUp");
                TH1F* h_isrdn = (TH1F*) h_sr->Clone("isrvarDown");
                TH1F* h_fsrdn = (TH1F*) h_sr->Clone("fsrvarDown");

                TFile *file_isrup = TFile::Open(Form("../limits/%s/ttttisrup_histos_%s_%.1fifb.root",dir.Data(),kinRegs[kr].Data(),lumiAG),"OPEN");
                TFile *file_fsrup = TFile::Open(Form("../limits/%s/ttttfsrup_histos_%s_%.1fifb.root",dir.Data(),kinRegs[kr].Data(),lumiAG),"OPEN");
                TFile *file_isrdn = TFile::Open(Form("../limits/%s/ttttisrdn_histos_%s_%.1fifb.root",dir.Data(),kinRegs[kr].Data(),lumiAG),"OPEN");
                TFile *file_fsrdn = TFile::Open(Form("../limits/%s/ttttfsrdn_histos_%s_%.1fifb.root",dir.Data(),kinRegs[kr].Data(),lumiAG),"OPEN");
                if (file_isrup==0 || file_fsrup==0 || file_isrdn==0 || file_fsrdn==0) {
                    cout << "error! need isr and fsr files for systematic" << endl;
                    assert(0);
                }
                TH1F* hinput_isrup = (TH1F*) file_isrup->Get("sr");
                TH1F* hinput_fsrup = (TH1F*) file_fsrup->Get("sr");
                TH1F* hinput_isrdn = (TH1F*) file_isrdn->Get("sr");
                TH1F* hinput_fsrdn = (TH1F*) file_fsrdn->Get("sr");
                
                hinput_isrup->Scale((h_sr->Integral())/(hinput_isrup->Integral()));
                hinput_fsrup->Scale((h_sr->Integral())/(hinput_fsrup->Integral()));
                hinput_isrdn->Scale((h_sr->Integral())/(hinput_isrdn->Integral()));
                hinput_fsrdn->Scale((h_sr->Integral())/(hinput_fsrdn->Integral()));

                for (int bin=1;bin<=h_isrup->GetNbinsX();++bin) {
                    h_isrup->SetBinContent(bin,hinput_isrup->GetBinContent(bin));
                    h_isrdn->SetBinContent(bin,hinput_isrdn->GetBinContent(bin));
                    // scale fsr variations closer to central by sqrt(2) according to
                    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/TopSystematics
                    float varup = hinput_fsrup->GetBinContent(bin);
                    float vardn = hinput_fsrdn->GetBinContent(bin);
                    float nom = h_sr->GetBinContent(bin);
                    // float sgnup = (varup-nom)/fabs(varup-nom);
                    // float sgndn = (vardn-nom)/fabs(vardn-nom);
                    // h_fsrup->SetBinContent(bin,varup);
                    // h_fsrdn->SetBinContent(bin,vardn);
                    // float newup = nom+sgnup*sqrt(abs(varup-nom));
                    // float newdn = nom+sgndn*sqrt(abs(vardn-nom));
                    float newup = nom+(varup-nom)/sqrt(2.);
                    float newdn = nom+(vardn-nom)/sqrt(2.);
                    if (nom == 0.) continue;
                    h_fsrup->SetBinContent(bin,newup);
                    h_fsrdn->SetBinContent(bin,newdn);
                }
                file_isrup->Close();
                file_fsrup->Close();
                file_isrdn->Close();
                file_fsrdn->Close();
                fileOut->cd();
                h_isrup->Write();
                h_fsrup->Write();
                h_isrdn->Write();
                h_fsrdn->Write();
            }


            if (!isData && !doFlips && !doFakes) {
                //btag: init
                TH1F* plot_btagSF_up_alt = 0;
                TH1F* plot_btagSF_ct_alt = 0;
                TH1F* plot_btagSF_dn_alt = 0;
                if      (kinRegs[kr]=="srcr")   plot_btagSF_up_alt = p_btagSF_alt_up.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_btagSF_up_alt = p_btagSF_alt_up.SRDISC.TOTAL;
                else exit(1);
                if      (kinRegs[kr]=="srcr")   plot_btagSF_dn_alt = p_btagSF_alt_dn.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_btagSF_dn_alt = p_btagSF_alt_dn.SRDISC.TOTAL;
                else exit(1);
                if      (kinRegs[kr]=="srcr")   plot_btagSF_ct_alt = p_result.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_btagSF_ct_alt = p_result.SRDISC.TOTAL;
                else exit(1);
                //btag: normalize (be careful with bins inclusive in btags, they are not affected by this syst)
                int nBins = plot_btagSF_ct_alt->GetNbinsX();
                int nInclBins = 0;

                if (plot_btagSF_up_alt->Integral(1,nBins-nInclBins)>0) {
                    float scaleBtag_up = plot_btagSF_ct_alt->Integral(1,nBins-nInclBins)/plot_btagSF_up_alt->Integral(1,nBins-nInclBins);
                    plot_btagSF_up_alt->Scale(scaleBtag_up);
                }
                if (plot_btagSF_dn_alt->Integral(1,nBins-nInclBins)) {
                    float scaleBtag_dn = plot_btagSF_ct_alt->Integral(1,nBins-nInclBins)/plot_btagSF_dn_alt->Integral(1,nBins-nInclBins);
                    plot_btagSF_dn_alt->Scale(scaleBtag_dn);
                }
                for (int ibin=1;ibin<=nInclBins;ibin++) {
                    plot_btagSF_up_alt->SetBinContent(nBins-nInclBins+ibin,plot_btagSF_ct_alt->GetBinContent(nBins-nInclBins+ibin));
                    plot_btagSF_dn_alt->SetBinContent(nBins-nInclBins+ibin,plot_btagSF_ct_alt->GetBinContent(nBins-nInclBins+ibin));
                }

                //btag: now save the plot
                TH1F* btagSFUp   = (TH1F*)plot_btagSF_up_alt->Clone("btagUp");
                TH1F* btagSFDown = (TH1F*)plot_btagSF_dn_alt->Clone("btagDown");
                btagSFUp  ->SetTitle("btagUp");
                btagSFDown->SetTitle("btagDown");
                btagSFUp  ->Write();
                btagSFDown->Write();

                //jes
                TH1F* plot_alt_jes_up = 0;
                TH1F* plot_alt_jes_dn = 0;
                if      (kinRegs[kr]=="srcr")   plot_alt_jes_up=p_jes_alt_up.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_alt_jes_up=p_jes_alt_up.SRDISC.TOTAL;
                else exit(1);
                if      (kinRegs[kr]=="srcr")   plot_alt_jes_dn=p_jes_alt_dn.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_alt_jes_dn=p_jes_alt_dn.SRDISC.TOTAL;
                else exit(1);
                TH1F* jesUp   = (TH1F*) plot_alt_jes_up->Clone("jesUp");
                TH1F* jesDown = (TH1F*) plot_alt_jes_dn->Clone("jesDown");
                if (name=="ttw" || name=="ttz") {
                    //wz is normalized in data, so we want only the sr migration
                    jesUp->Scale(h_sr->Integral()/jesUp->Integral());
                    jesDown->Scale(h_sr->Integral()/jesDown->Integral());
                }
                avoidZeroIntegrals(h_sr,jesUp,jesDown);
                jesUp->Write();
                jesDown->Write();

                //jer
                TH1F* plot_alt_jer_up = 0;
                TH1F* plot_alt_jer_dn = 0;
                if      (kinRegs[kr]=="srcr")   plot_alt_jer_up=p_jer_alt_up.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_alt_jer_up=p_jer_alt_up.SRDISC.TOTAL;
                else exit(1);
                if      (kinRegs[kr]=="srcr")   plot_alt_jer_dn=p_jer_alt_dn.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_alt_jer_dn=p_jer_alt_dn.SRDISC.TOTAL;
                else exit(1);
                TH1F* jerUp   = (TH1F*) plot_alt_jer_up->Clone("jerUp");
                TH1F* jerDown = (TH1F*) plot_alt_jer_dn->Clone("jerDown");
                if (name=="ttw" || name=="ttz") {
                    //wz is normalized in data, so we want only the sr migration
                    jerUp->Scale(h_sr->Integral()/jerUp->Integral());
                    jerDown->Scale(h_sr->Integral()/jerDown->Integral());
                }
                avoidZeroIntegrals(h_sr,jerUp,jerDown);
                jerUp->Write();
                jerDown->Write();

                //lep
                TH1F* plot_alt_lep_up = 0;
                TH1F* plot_alt_lep_dn = 0;
                if      (kinRegs[kr]=="srcr")   plot_alt_lep_up=p_lep_alt_up.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_alt_lep_up=p_lep_alt_up.SRDISC.TOTAL;
                else exit(1);
                if      (kinRegs[kr]=="srcr")   plot_alt_lep_dn=p_lep_alt_dn.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_alt_lep_dn=p_lep_alt_dn.SRDISC.TOTAL;
                else exit(1);
                TH1F* lepUp   = (TH1F*) plot_alt_lep_up->Clone("lepUp");
                TH1F* lepDown = (TH1F*) plot_alt_lep_dn->Clone("lepDown");
                fillDownMirrorUp(h_sr,lepUp,lepDown);
                lepUp->Write();
                lepDown->Write();

                // ttbb/ttjj ~ 1.7 uncertainty
                TH1F* plot_alt_bb_up = 0;
                TH1F* plot_alt_bb_dn = 0;
                if      (kinRegs[kr]=="srcr")   plot_alt_bb_up=p_bb_alt_up.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_alt_bb_up=p_bb_alt_up.SRDISC.TOTAL;
                else exit(1);
                if      (kinRegs[kr]=="srcr")   plot_alt_bb_dn=p_bb_alt_dn.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_alt_bb_dn=p_bb_alt_dn.SRDISC.TOTAL;
                else exit(1);
                TH1F* bbUp   = (TH1F*) plot_alt_bb_up->Clone("bbUp");
                TH1F* bbDown = (TH1F*) plot_alt_bb_dn->Clone("bbDown");
                for (int bin=1;bin<=bbUp->GetNbinsX();++bin) {
                    float nomval = h_sr->GetBinContent(bin);
                    float upval = bbUp->GetBinContent(bin);
                    // bbUp->SetBinContent(bin,0.5*(nomval+upval));
                    bbUp->SetBinContent(bin,nomval+(0.6/0.7)*(upval-nomval));
                }
                fillDownMirrorUp(h_sr,bbUp,bbDown);
                // if (name=="ttw" || name=="ttz") {
                //     bbUp->Scale(h_sr->Integral()/bbUp->Integral());
                //     bbDown->Scale(h_sr->Integral()/bbDown->Integral());
                // }
                bbUp->Write();
                bbDown->Write();

                //isr
                TH1F* plot_alt_isr_up = 0;
                TH1F* plot_alt_isr_dn = 0;
                if      (kinRegs[kr]=="srcr")   plot_alt_isr_up=p_isr_alt_up.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_alt_isr_up=p_isr_alt_up.SRDISC.TOTAL;
                else exit(1);
                if      (kinRegs[kr]=="srcr")   plot_alt_isr_dn=p_isr_alt_dn.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_alt_isr_dn=p_isr_alt_dn.SRDISC.TOTAL;
                else exit(1);
                TH1F* isrUp   = (TH1F*) plot_alt_isr_up->Clone("isrUp");
                TH1F* isrDown = (TH1F*) plot_alt_isr_dn->Clone("isrDown");
                for (int bin=1;bin<=isrUp->GetNbinsX();++bin) {
                    float nomval = h_sr->GetBinContent(bin);
                    float upval = isrUp->GetBinContent(bin);
                    isrUp->SetBinContent(bin,0.5*(nomval+upval));
                }
                fillDownMirrorUp(h_sr,isrUp,isrDown);
                // if (name=="ttw" || name=="ttz") {
                //     //wz is normalized in data, so we want only the sr migration
                //     isrUp->Scale(h_sr->Integral()/isrUp->Integral());
                //     isrDown->Scale(h_sr->Integral()/isrDown->Integral());
                // }
                isrUp->Write();
                isrDown->Write();

                //pu
                TH1F* plot_alt_pu_up = 0;
                TH1F* plot_alt_pu_dn = 0;
                if      (kinRegs[kr]=="srcr")   plot_alt_pu_up=p_pu_alt_up.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_alt_pu_up=p_pu_alt_up.SRDISC.TOTAL;
                else exit(1);
                if      (kinRegs[kr]=="srcr")   plot_alt_pu_dn=p_pu_alt_dn.SRCR.TOTAL;
                else if      (kinRegs[kr]=="srdisc")   plot_alt_pu_dn=p_pu_alt_dn.SRDISC.TOTAL;
                else exit(1);
                TH1F* puUp   = (TH1F*) plot_alt_pu_up->Clone("puUp");
                TH1F* puDown = (TH1F*) plot_alt_pu_dn->Clone("puDown");
                if (name=="ttw" || name=="ttz") {
                    //wz is normalized in data, so we want only the sr migration
                    puUp->Scale(h_sr->Integral()/puUp->Integral());
                    puDown->Scale(h_sr->Integral()/puDown->Integral());
                }
                puUp->Write();
                puDown->Write();

                //leptons
                if (!(name=="ttw" || name=="ttz")) {
                    writeHTHltSyst(h_sr,name,kinRegs[kr]);
                }
            }

            //end systematics
            fileOut->Close();
        }

    }

    //Return yield
    return pair<yields_t, plots_t>(y_result,p_result);

}

void avoidNegativeYields(TH1F* plot) {
  for (int bin=1;bin<=plot->GetNbinsX();++bin) {
    if (plot->GetBinContent(bin)<0) {
      if (!suppressWarns) cout << "warning: plot " << plot->GetName() << " has negative yield in bin " << bin << " value=" << plot->GetBinContent(bin) << " error=" << plot->GetBinError(bin) << "; setting to 1E-6."<< endl;
      plot->SetBinContent(bin,1E-6);
    }
  }
}


void writeStatUpDown(TH1F* central,string name,bool down) {
  TString updown = "Up";
  if (down) updown = "Down";
  TH1F* statUpDown = new TH1F(Form("%s_stat%s",name.c_str(),updown.Data()),
            Form("%s_stat%s",name.c_str(),updown.Data()),
            central->GetNbinsX(),central->GetXaxis()->GetXmin(),central->GetXaxis()->GetXmax());
  for (int bin=1;bin<=statUpDown->GetNbinsX();++bin) {
    float val = down ? (central->GetBinContent(bin)-central->GetBinError(bin)) : (central->GetBinContent(bin)+central->GetBinError(bin));
    //if (name=="fakes") cout << bin << " val=" << val << " c=" << central->GetBinContent(bin) << " e=" << central->GetBinError(bin) << endl;
    if (val>0) statUpDown->SetBinContent(bin,val);
    else statUpDown->SetBinContent(bin,1E-6);
  }
  statUpDown->Write();
}

void writeStat(TH1F* central,string name) {
  writeStatUpDown(central,name,0);
  writeStatUpDown(central,name,1);
}

void writeHTHltSyst(TH1F* central,string name,TString kine) {
  TString up = "Up";
  TH1F* systUp = (TH1F*) central->Clone(Form("hthlt%s",up.Data()));
  systUp->SetTitle(Form("hthlt%s",up.Data()));
  float systValue = 1.02;
  for (int bin=1;bin<=systUp->GetNbinsX();++bin) {
    float val = central->GetBinContent(bin)*systValue;
    if (val>0) systUp->SetBinContent(bin,val);
  }
  TString down = "Down";
  TH1F* systDown = (TH1F*) central->Clone(Form("hthlt%s",down.Data()));
  systDown->SetTitle(Form("hthlt%s",down.Data()));
  fillDownMirrorUp(central,systUp,systDown);
  systUp->Write();
  systDown->Write();
}

void fillDownMirrorUp(TH1F* central,TH1F* up,TH1F* down) {
  down->Reset();
  down->Add(up);
  down->Scale(-1);
  down->Add(central);
  down->Add(central);
  //need to avoid negative values...
  avoidNegativeYields(down);
}

void avoidZeroIntegrals(TH1F* central,TH1F* up,TH1F* down) {
  if (down->Integral()<=0) {
    down->Reset();
    down->Add(central);
    down->Scale(-1);
    avoidNegativeYields(down);
  }
  if (up->Integral()<=0) {
    up->Reset();
    up->Add(central);
    up->Scale(-1);
    avoidNegativeYields(up);
  }
}

void initHistError(bool usePoisson, TH1F* plot) {
  if (plot==0) return;
  if (usePoisson) plot->SetBinErrorOption(TH1::kPoisson);
  else  plot->Sumw2();
}

std::pair<float,float> getTrijetDiscs(TMVA::Reader* reader) {
    // Separate out b-tagged jets from non-btagged jets
    std::vector<LorentzVector> bjets;
    std::vector<float> bjets_disc;
    std::vector<LorentzVector> jets;
    for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
        bjets.push_back(ss::btags()[ijet]);
        bjets_disc.push_back(ss::btags_disc()[ijet]);
    }
    for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
        if (ss::jets_disc()[ijet] > bmed) continue;
        jets.push_back(ss::jets()[ijet]);
    }

    // std::vector<float> discs;
    float disc_best = -2;
    float disc_secondbest = -2;
    for (unsigned int ib = 0; ib < bjets.size(); ib++) {
        LorentzVector bjet = (bjets[ib]);
        float disc = bjets_disc[ib];
        for (unsigned int i = 0; i < jets.size(); i++) {
            for (unsigned int j = i+1; j < jets.size(); j++) {

                LorentzVector w = (jets[i]+jets[j]);
                float HadrWmass = w.M();
                if (HadrWmass > 1650. || HadrWmass < 15.) continue;

                LorentzVector top = (jets[i]+jets[j]+bjet);
                float TopMass = top.M();
                if (TopMass > 3900 || TopMass < 30) continue;

                float sumpt = jets[i].pt() + jets[j].pt() + bjet.pt();

                float ThPtOverSumPt = top.pt()/sumpt;

                float AngleThWh = fabs(DeltaPhi(top.phi(),w.phi()));
                float AngleThBh = fabs(DeltaPhi(top.phi(),bjet.phi()));

                std::vector<float> inputs = {disc,ThPtOverSumPt,AngleThWh,AngleThBh,HadrWmass,TopMass};
                float disc = reader->EvaluateMVA(inputs,"BDT");

                if (disc > disc_best) disc_best = disc;
                else if (disc > disc_secondbest) disc_secondbest = disc;

                // discs.push_back(disc);

            }
        }
    }
    // return discs;
    return std::pair<float,float>(disc_best,disc_secondbest);
}

std::pair<int,float> getNtops() {
    // Separate out b-tagged jets from non-btagged jets
    std::vector<LorentzVector> bjets;
    std::vector<LorentzVector> jets;
    for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
        bjets.push_back(ss::btags()[ijet]);
    }
    for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
        if (ss::jets_disc()[ijet] > bmed) continue;
        jets.push_back(ss::jets()[ijet]);
    }
    // std::cout << "begin getNtops() -->" << std::endl;
    // std::cout << "   found " << jets.size() << " jets and " << bjets.size() << " bjets" << std::endl;
    

    // Try to divide out combinatorics
    float ncomb = 0.5 * bjets.size() * jets.size();

    float cutw = 30.;
    float cutt = 50.;
    int ntops = 0;
    std::vector<int> used_ijets;
    // for each bjet, increment ntops if there is a unique pair
    // of quarks q1,q2 that satisfies
    //      |m(q1,q2)-mW| < 30
    //      |m(b,q1,q2)-mtop| < 50
    for (auto bjet : bjets) {
        int best_ijet1 = -1;
        int best_ijet2 = -1;
        float best_dmt = cutt;
        // std::cout << "   considering bjet with pt " << bjet.pt() << std::endl;
        for (unsigned int i = 0; i < jets.size(); i++) {
            for (unsigned int j = i+1; j < jets.size(); j++) {
                float dmw = fabs((jets[i]+jets[j]).M()-80.4);
                float dmt = fabs((jets[i]+jets[j]+bjet).M()-172.5);
                if (dmw > cutw) continue;
                if (dmt > cutt) continue;
                // bool already_used = std::find(used_pairs.begin(), used_pairs.end(), std::make_pair<int,int>(i,j)) != used_pairs.end();
                bool already_used = (std::find(used_ijets.begin(), used_ijets.end(), i) != used_ijets.end()
                                    || std::find(used_ijets.begin(), used_ijets.end(), j) != used_ijets.end());
                // std::cout << "       pair " << i << "," << j << " already used? " << already_used << std::endl;
                if (already_used) continue;
                if (dmt < best_dmt) {
                    // std::cout << "       pair " << i << "," << j << " has better dmt of " << dmt << std::endl;
                    best_dmt = dmt;
                    best_ijet1 = i;
                    best_ijet2 = j;
                }
            }
        }
        if (best_ijet1 < 0 || best_ijet2 < 0) continue;
        used_ijets.push_back(best_ijet1);
        used_ijets.push_back(best_ijet2);
        // std::cout << "   incrementing ntops" << std::endl;
        ntops++;
    }
    // std::cout << "end getNtops() -->" << std::endl;
    return std::pair<int,float>(ntops, 1.0*ntops/ncomb);
}

std::pair<float,float> getFirstTwoTops() {
    // Separate out b-tagged jets from non-btagged jets
    std::vector<LorentzVector> bjets;
    std::vector<LorentzVector> jets;
    for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
        bjets.push_back(ss::btags()[ijet]);
    }
    for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
        if (ss::jets_disc()[ijet] > bmed) continue;
        jets.push_back(ss::jets()[ijet]);
    }

    float mtop = 172.5;
    float mw = 80.4;
    float atop = 15.;
    float aw = 5.;


    float bestchisq1 = 1e6;
    float bestchisq2 = 1e6;
    float bestmt1 = -1.;
    float bestmt2 = -1.;

    for (auto bjet : bjets) {
        for (unsigned int i = 0; i < jets.size(); i++) {
            for (unsigned int j = i+1; j < jets.size(); j++) {

                float thismtop = (bjet+jets[i]+jets[j]).M();
                float chisq = pow((mw*mw - pow((jets[i]+jets[j]).M(),2)),2)/pow(aw*aw,2);
                chisq += pow((mtop*mtop - pow(thismtop,2)),2)/pow(atop*atop,2);

                if (chisq < bestchisq1) {
                    bestchisq1 = chisq;
                    bestmt1 = thismtop;
                } else if (chisq < bestchisq2) {
                    bestchisq2 = chisq;
                    bestmt2 = thismtop;
                }
            }
        }
    }
    // std::cout <<  " bjets.size(): " << bjets.size() <<  " jets.size(): " << jets.size() <<  " bestmt1: " << bestmt1 <<  " bestmt2: " << bestmt2 <<  " bestchisq1: " << bestchisq1 <<  " bestchisq2: " << bestchisq2 <<  std::endl;
    return std::pair<float,float>(bestmt1,bestmt2);
}

int getBDTBin(float disc, bool isClass6) {
    int ibin = 1;

    if (isClass6) return ibin;

    // 13 bins
    if (disc > -0.48) ibin = 2;
    if (disc > -0.37) ibin = 3;
    if (disc > -0.25) ibin = 4;
    if (disc > -0.14) ibin = 5;
    if (disc > -0.02) ibin = 6;
    if (disc >  0.08) ibin = 7;
    if (disc >  0.2) ibin = 8;
    if (disc >  0.31) ibin = 9;
    if (disc >  0.42) ibin = 10;
    if (disc >  0.54) ibin = 11;
    if (disc >  0.65) ibin = 12;
    if (disc >  0.77) ibin = 13;

    return ibin;
}

