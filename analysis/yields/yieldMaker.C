#include <bitset>
#include <iostream>
#include <algorithm>
#include "TChain.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TKey.h"
#include "TSystem.h"
#include "../misc/class_files/v8.02/SS.h"
#include "../../common/CORE/SSSelections.h"
#include "../../common/CORE/Tools/dorky/dorky.h"
#include "../../common/CORE/Tools/utils.h"
#include "../misc/common_utils.h"
#include "../misc/signal_regions.h"
// #include "bdt/function.h"
// #include "bdt/function_Apr18.h"
// #include "bdt/function_Apr22.h"

// #include "../../../../batch/points/isr_norm_TTWnlo.h"
// #include "../../../../batch/points/isr_norm_TTZnlo.h"
// #include "../../../../batch/points/isr_norm_TTZLOW.h"

float lumiAG = getLumiUnblind();
string tag = getTag().Data();  
string tagData = getTagData().Data();  

float bloose = 0.5426;
float bmed = 0.8484;
float btight = 0.9535;

int nsr = getNsrsTTTT();
int nCR = 2;

bool doCustomSelection = false; // use custom selection // FIXME
bool applyttZSF = true; // true if we want to apply the data driven scale factor for ttZ in addition to WZ
float scaleLumi = 1.;//3.0/1.264;//careful!!!

bool DOWEIGHTS = false; // FIXME
bool doTTWISR = false; // FIXME
bool doTTZISR = false; // FIXME

bool doLatex = 0;


bool suppressWarns = true;


// For output tree
float tree_met = -1, tree_ht = -1, tree_mtmin = -1, tree_weight = -1;
int tree_l1id = -1, tree_l2id = -1, tree_njets = -1, tree_nbtags = -1, tree_kine = -1, tree_SR = -1;
int tree_event = -1, tree_lumi = -1, tree_run = -1;
TString tree_name = "";
// TFile *out_file;
// TTree *out_tree;

struct yields_t { float EE; float EM; float MM; float TOTAL; }; 
struct SR_t     { TH1F* EE; TH1F* EM; TH1F* MM; TH1F* TOTAL; }; 
struct triple_t { TH1F* ttzcr; TH1F* ttwcr; TH1F* sr; TH1F* br; };
struct plots_t  {
    SR_t SRCR;
    SR_t SR;
    triple_t h_ht;
    triple_t h_met;
    triple_t h_mll;
    triple_t h_mtmin;
    triple_t h_mt1;
    triple_t h_mt2;
    triple_t h_njets;
    triple_t h_nbtags;
    triple_t h_l1pt;
    triple_t h_l2pt;
    triple_t h_l3pt;
    triple_t h_type;
    triple_t h_charge;
    triple_t h_nleps;
    triple_t h_wcands;
    triple_t h_mu_l1pt;
    triple_t h_el_l1pt;
    triple_t h_mu_l2pt;
    triple_t h_mu_l3pt;
    triple_t h_el_l2pt;
    triple_t h_el_l3pt;
    triple_t h_mbb;
    triple_t h_disc;
};


//Total
yields_t total; 

//function declaration
pair<yields_t, plots_t> run(TChain *chain, bool isData = 0, bool doFlips = 0, int doFakes = 0, int exclude = 0, bool isSignal = 0, bool isGamma = 0);
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt);
void initHistError(bool usePoisson, TH1F* plot);

static float roughSystTTW   = 0.2;
static float roughSystTTZH  = 0.2;
static float roughSystWZ    = 0.3;
static float roughSystWW    = 0.3;
static float roughSystXG    = 0.5;
static float roughSystRARES = 0.5;
static float roughSystFLIPS = 0.3;
static float roughSystFAKES = 0.35;
static float roughSystFAKESHH = 0.40;
static float roughSystFAKESXL = 0.30;

void getyields(){

    cout << "Running with lumi=" << lumiAG*scaleLumi << endl;

    //Chains
    //fakes&flips in mc
    TChain *ttbar_chain   = new TChain("t","ttbar");
    TChain *st_chain      = new TChain("t","st");
    TChain *wjets_chain   = new TChain("t","wjets");
    TChain *dy_chain      = new TChain("t","dy");
    //rares
    TChain *ttw_chain     = new TChain("t","ttw");
    TChain *ttz_chain    = new TChain("t","ttz");
    TChain *tth_chain    = new TChain("t","tth");
    TChain *wz_chain      = new TChain("t","wz");
    TChain *ww_chain      = new TChain("t","ww");
    TChain *xg_chain      = new TChain("t","xg");
    TChain *rares_chain   = new TChain("t","rares");
    TChain *tttt_chain    = new TChain("t","tttt");
    TChain *qqww_chain    = new TChain("t","qqww"); // separate this out to check shape uncertainties
    //data
    TChain *data_chain    = new TChain("t","data"); 
    TChain *flips_chain   = new TChain("t","flips"); 
    TChain *fakes_chain   = new TChain("t","fakes");
    //signals full sim

    TString pfx  = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/";
    TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.06/output/";

    //Fill chains
    tttt_chain   ->Add(Form("%s/TTTT.root"           , pfx.Data()));

    // ttbar_chain  ->Add(Form("%s/TTBAR_PH*.root"       , pfx.Data())); 
    // wjets_chain  ->Add(Form("%s/WJets.root"       , pfx.Data()));
    // dy_chain     ->Add(Form("%s/DY_high*.root"        , pfx.Data()));
    // dy_chain     ->Add(Form("%s/DY_low*.root"         , pfx.Data()));
    // ttw_chain    ->Add(Form("%s/TTWnlo.root"            , pfx.Data())); 
    // ttz_chain   ->Add(Form("%s/TTZnlo.root"           , pfx.Data())); 
    // ttz_chain   ->Add(Form("%s/TTZLOW.root"         , pfx.Data())); 
    // tth_chain   ->Add(Form("%s/TTHtoNonBB.root"     , pfx.Data()));
    // wz_chain     ->Add(Form("%s/WZ.root"             , pfx.Data()));
    // ww_chain     ->Add(Form("%s/QQWW.root"           , pfx.Data()));
    // qqww_chain     ->Add(Form("%s/QQWW.root"           , pfx.Data()));
    // xg_chain     ->Add(Form("%s/TG.root"             , pfx.Data()));
    // xg_chain     ->Add(Form("%s/TTG.root"            , pfx.Data()));
    // xg_chain     ->Add(Form("%s/WGToLNuG.root"           , pfx.Data()));
    // xg_chain     ->Add(Form("%s/ZG.root"             , pfx.Data()));
    // rares_chain  ->Add(Form("%s/ZZ.root"             , pfx.Data()));
    // rares_chain  ->Add(Form("%s/GGHtoZZto4L.root"    , pfx.Data()));
    // rares_chain  ->Add(Form("%s/WWZ.root"            , pfx.Data()));
    // rares_chain  ->Add(Form("%s/WZZ.root"            , pfx.Data()));
    // rares_chain  ->Add(Form("%s/WWW.root"            , pfx.Data()));
    // rares_chain  ->Add(Form("%s/WWG.root"            , pfx.Data()));
    // rares_chain  ->Add(Form("%s/WZG.root"            , pfx.Data()));
    // rares_chain  ->Add(Form("%s/VHtoNonBB.root"      , pfx.Data()));
    // rares_chain  ->Add(Form("%s/TZQ.root"            , pfx.Data()));
    // rares_chain  ->Add(Form("%s/TWZ.root"            , pfx.Data()));
    // rares_chain  ->Add(Form("%s/WWDPS.root"          , pfx.Data()));
    // //data
    // data_chain   ->Add(Form("%s/DataDoubleMuon*.root"    , pfxData.Data()));
    // data_chain   ->Add(Form("%s/DataDoubleEG*.root"  , pfxData.Data()));
    // data_chain   ->Add(Form("%s/DataMuonEG*.root"      , pfxData.Data()));
    // data_chain   ->Add(Form("%s/JetHT*.root"      , pfxData.Data()));
    // //flips
    // flips_chain  ->Add(Form("%s/DataMuonEG*.root"     , pfxData.Data()));
    // flips_chain  ->Add(Form("%s/DataDoubleEG*.root"      , pfxData.Data()));
    // //fakes
    // fakes_chain  ->Add(Form("%s/DataDoubleMuon*.root"    , pfxData.Data()));
    // fakes_chain  ->Add(Form("%s/DataDoubleEG*.root"  , pfxData.Data()));
    // fakes_chain  ->Add(Form("%s/DataMuonEG*.root"      , pfxData.Data()));
    // fakes_chain  ->Add(Form("%s/JetHT*.root"      , pfxData.Data()));
    // fakes_chain  ->Add(Form("%s/TTWnlo.root"                   , pfx.Data()));
    // fakes_chain  ->Add(Form("%s/TTZnlo.root"                  , pfx.Data()));
    // fakes_chain  ->Add(Form("%s/WZ.root"                    , pfx.Data()));
    // fakes_chain  ->Add(Form("%s/TTHtoNonBB.root"            , pfx.Data()));
    // fakes_chain  ->Add(Form("%s/QQWW.root"                  , pfx.Data()));


    pair<yields_t, plots_t> results_ttw      = run(ttw_chain);
    pair<yields_t, plots_t> results_ttz     = run(ttz_chain);
    pair<yields_t, plots_t> results_tth     = run(tth_chain);
    pair<yields_t, plots_t> results_qqww     = run(qqww_chain);
    pair<yields_t, plots_t> results_wz       = run(wz_chain);
    pair<yields_t, plots_t> results_ww       = run(ww_chain);
    pair<yields_t, plots_t> results_xg       = run(xg_chain, 0, 0, 0, 0, 0, 1);
    pair<yields_t, plots_t> results_rares    = run(rares_chain);
    pair<yields_t, plots_t> results_tttt     = run(tttt_chain);
    pair<yields_t, plots_t> results_data     = run(data_chain, 1);
    duplicate_removal::clear_list();
    pair<yields_t, plots_t> results_flips    = run(flips_chain, 1, 1);
    duplicate_removal::clear_list();
    pair<yields_t, plots_t> results_fakes    = run(fakes_chain, 1, 0, 1);
    duplicate_removal::clear_list();

    plots_t& p_tttt       = results_tttt.second;

    plots_t& p_ttw      = results_ttw.second;
    plots_t& p_ttz     = results_ttz.second;
    plots_t& p_tth     = results_tth.second;
    plots_t& p_wz       = results_wz.second;
    plots_t& p_ww       = results_ww.second;
    plots_t& p_xg       = results_xg.second;
    plots_t& p_rares    = results_rares.second;
    plots_t& p_data     = results_data.second;
    plots_t& p_flips    = results_flips.second;
    plots_t& p_fakes    = results_fakes.second;

    TFile *fout = new TFile("histos.root", "RECREATE");

    p_data.h_ht.sr->Write(); p_ttw.h_ht.sr->Write(); p_ttz.h_ht.sr->Write(); p_tth.h_ht.sr->Write(); p_wz.h_ht.sr->Write(); p_ww.h_ht.sr->Write(); p_xg.h_ht.sr->Write(); p_rares.h_ht.sr->Write(); p_flips.h_ht.sr->Write(); p_fakes.h_ht.sr->Write(); p_tttt.h_ht.sr->Write();
    p_data.h_met.sr->Write(); p_ttw.h_met.sr->Write(); p_ttz.h_met.sr->Write(); p_tth.h_met.sr->Write(); p_wz.h_met.sr->Write(); p_ww.h_met.sr->Write(); p_xg.h_met.sr->Write(); p_rares.h_met.sr->Write(); p_flips.h_met.sr->Write(); p_fakes.h_met.sr->Write(); p_tttt.h_met.sr->Write();
    p_data.h_mll.sr->Write(); p_ttw.h_mll.sr->Write(); p_ttz.h_mll.sr->Write(); p_tth.h_mll.sr->Write(); p_wz.h_mll.sr->Write(); p_ww.h_mll.sr->Write(); p_xg.h_mll.sr->Write(); p_rares.h_mll.sr->Write(); p_flips.h_mll.sr->Write(); p_fakes.h_mll.sr->Write(); p_tttt.h_mll.sr->Write();
    p_data.h_mtmin.sr->Write(); p_ttw.h_mtmin.sr->Write(); p_ttz.h_mtmin.sr->Write(); p_tth.h_mtmin.sr->Write(); p_wz.h_mtmin.sr->Write(); p_ww.h_mtmin.sr->Write(); p_xg.h_mtmin.sr->Write(); p_rares.h_mtmin.sr->Write(); p_flips.h_mtmin.sr->Write(); p_fakes.h_mtmin.sr->Write(); p_tttt.h_mtmin.sr->Write();
    p_data.h_njets.sr->Write(); p_ttw.h_njets.sr->Write(); p_ttz.h_njets.sr->Write(); p_tth.h_njets.sr->Write(); p_wz.h_njets.sr->Write(); p_ww.h_njets.sr->Write(); p_xg.h_njets.sr->Write(); p_rares.h_njets.sr->Write(); p_flips.h_njets.sr->Write(); p_fakes.h_njets.sr->Write(); p_tttt.h_njets.sr->Write();
    p_data.h_nleps.sr->Write(); p_ttw.h_nleps.sr->Write(); p_ttz.h_nleps.sr->Write(); p_tth.h_nleps.sr->Write(); p_wz.h_nleps.sr->Write(); p_ww.h_nleps.sr->Write(); p_xg.h_nleps.sr->Write(); p_rares.h_nleps.sr->Write(); p_flips.h_nleps.sr->Write(); p_fakes.h_nleps.sr->Write(); p_tttt.h_nleps.sr->Write();
    p_data.h_wcands.sr->Write(); p_ttw.h_wcands.sr->Write(); p_ttz.h_wcands.sr->Write(); p_tth.h_wcands.sr->Write(); p_wz.h_wcands.sr->Write(); p_ww.h_wcands.sr->Write(); p_xg.h_wcands.sr->Write(); p_rares.h_wcands.sr->Write(); p_flips.h_wcands.sr->Write(); p_fakes.h_wcands.sr->Write(); p_tttt.h_wcands.sr->Write();
    p_data.h_nbtags.sr->Write(); p_ttw.h_nbtags.sr->Write(); p_ttz.h_nbtags.sr->Write(); p_tth.h_nbtags.sr->Write(); p_wz.h_nbtags.sr->Write(); p_ww.h_nbtags.sr->Write(); p_xg.h_nbtags.sr->Write(); p_rares.h_nbtags.sr->Write(); p_flips.h_nbtags.sr->Write(); p_fakes.h_nbtags.sr->Write(); p_tttt.h_nbtags.sr->Write();
    p_data.h_type.sr->Write(); p_ttw.h_type.sr->Write(); p_ttz.h_type.sr->Write(); p_tth.h_type.sr->Write(); p_wz.h_type.sr->Write(); p_ww.h_type.sr->Write(); p_xg.h_type.sr->Write(); p_rares.h_type.sr->Write(); p_flips.h_type.sr->Write(); p_fakes.h_type.sr->Write(); p_tttt.h_type.sr->Write();
    p_data.h_charge.sr->Write(); p_ttw.h_charge.sr->Write(); p_ttz.h_charge.sr->Write(); p_tth.h_charge.sr->Write(); p_wz.h_charge.sr->Write(); p_ww.h_charge.sr->Write(); p_xg.h_charge.sr->Write(); p_rares.h_charge.sr->Write(); p_flips.h_charge.sr->Write(); p_fakes.h_charge.sr->Write(); p_tttt.h_charge.sr->Write();
    p_data.h_l1pt.sr->Write(); p_ttw.h_l1pt.sr->Write(); p_ttz.h_l1pt.sr->Write(); p_tth.h_l1pt.sr->Write(); p_wz.h_l1pt.sr->Write(); p_ww.h_l1pt.sr->Write(); p_xg.h_l1pt.sr->Write(); p_rares.h_l1pt.sr->Write(); p_flips.h_l1pt.sr->Write(); p_fakes.h_l1pt.sr->Write(); p_tttt.h_l1pt.sr->Write();
    p_data.h_l2pt.sr->Write(); p_ttw.h_l2pt.sr->Write(); p_ttz.h_l2pt.sr->Write(); p_tth.h_l2pt.sr->Write(); p_wz.h_l2pt.sr->Write(); p_ww.h_l2pt.sr->Write(); p_xg.h_l2pt.sr->Write(); p_rares.h_l2pt.sr->Write(); p_flips.h_l2pt.sr->Write(); p_fakes.h_l2pt.sr->Write(); p_tttt.h_l2pt.sr->Write();
    p_data.h_l3pt.sr->Write(); p_ttw.h_l3pt.sr->Write(); p_ttz.h_l3pt.sr->Write(); p_tth.h_l3pt.sr->Write(); p_wz.h_l3pt.sr->Write(); p_ww.h_l3pt.sr->Write(); p_xg.h_l3pt.sr->Write(); p_rares.h_l3pt.sr->Write(); p_flips.h_l3pt.sr->Write(); p_fakes.h_l3pt.sr->Write(); p_tttt.h_l3pt.sr->Write();
    p_data.h_el_l1pt.sr->Write(); p_ttw.h_el_l1pt.sr->Write(); p_ttz.h_el_l1pt.sr->Write(); p_tth.h_el_l1pt.sr->Write(); p_wz.h_el_l1pt.sr->Write(); p_ww.h_el_l1pt.sr->Write(); p_xg.h_el_l1pt.sr->Write(); p_rares.h_el_l1pt.sr->Write(); p_flips.h_el_l1pt.sr->Write(); p_fakes.h_el_l1pt.sr->Write(); p_tttt.h_el_l1pt.sr->Write();
    p_data.h_el_l2pt.sr->Write(); p_ttw.h_el_l2pt.sr->Write(); p_ttz.h_el_l2pt.sr->Write(); p_tth.h_el_l2pt.sr->Write(); p_wz.h_el_l2pt.sr->Write(); p_ww.h_el_l2pt.sr->Write(); p_xg.h_el_l2pt.sr->Write(); p_rares.h_el_l2pt.sr->Write(); p_flips.h_el_l2pt.sr->Write(); p_fakes.h_el_l2pt.sr->Write(); p_tttt.h_el_l2pt.sr->Write();
    p_data.h_el_l3pt.sr->Write(); p_ttw.h_el_l3pt.sr->Write(); p_ttz.h_el_l3pt.sr->Write(); p_tth.h_el_l3pt.sr->Write(); p_wz.h_el_l3pt.sr->Write(); p_ww.h_el_l3pt.sr->Write(); p_xg.h_el_l3pt.sr->Write(); p_rares.h_el_l3pt.sr->Write(); p_flips.h_el_l3pt.sr->Write(); p_fakes.h_el_l3pt.sr->Write(); p_tttt.h_el_l3pt.sr->Write();
    p_data.h_mu_l1pt.sr->Write(); p_ttw.h_mu_l1pt.sr->Write(); p_ttz.h_mu_l1pt.sr->Write(); p_tth.h_mu_l1pt.sr->Write(); p_wz.h_mu_l1pt.sr->Write(); p_ww.h_mu_l1pt.sr->Write(); p_xg.h_mu_l1pt.sr->Write(); p_rares.h_mu_l1pt.sr->Write(); p_flips.h_mu_l1pt.sr->Write(); p_fakes.h_mu_l1pt.sr->Write(); p_tttt.h_mu_l1pt.sr->Write();
    p_data.h_mu_l2pt.sr->Write(); p_ttw.h_mu_l2pt.sr->Write(); p_ttz.h_mu_l2pt.sr->Write(); p_tth.h_mu_l2pt.sr->Write(); p_wz.h_mu_l2pt.sr->Write(); p_ww.h_mu_l2pt.sr->Write(); p_xg.h_mu_l2pt.sr->Write(); p_rares.h_mu_l2pt.sr->Write(); p_flips.h_mu_l2pt.sr->Write(); p_fakes.h_mu_l2pt.sr->Write(); p_tttt.h_mu_l2pt.sr->Write();
    p_data.h_mu_l3pt.sr->Write(); p_ttw.h_mu_l3pt.sr->Write(); p_ttz.h_mu_l3pt.sr->Write(); p_tth.h_mu_l3pt.sr->Write(); p_wz.h_mu_l3pt.sr->Write(); p_ww.h_mu_l3pt.sr->Write(); p_xg.h_mu_l3pt.sr->Write(); p_rares.h_mu_l3pt.sr->Write(); p_flips.h_mu_l3pt.sr->Write(); p_fakes.h_mu_l3pt.sr->Write(); p_tttt.h_mu_l3pt.sr->Write();

    p_data.h_ht.ttzcr->Write(); p_ttw.h_ht.ttzcr->Write(); p_ttz.h_ht.ttzcr->Write(); p_tth.h_ht.ttzcr->Write(); p_wz.h_ht.ttzcr->Write(); p_ww.h_ht.ttzcr->Write(); p_xg.h_ht.ttzcr->Write(); p_rares.h_ht.ttzcr->Write(); p_flips.h_ht.ttzcr->Write(); p_fakes.h_ht.ttzcr->Write(); p_tttt.h_ht.ttzcr->Write();
    p_data.h_met.ttzcr->Write(); p_ttw.h_met.ttzcr->Write(); p_ttz.h_met.ttzcr->Write(); p_tth.h_met.ttzcr->Write(); p_wz.h_met.ttzcr->Write(); p_ww.h_met.ttzcr->Write(); p_xg.h_met.ttzcr->Write(); p_rares.h_met.ttzcr->Write(); p_flips.h_met.ttzcr->Write(); p_fakes.h_met.ttzcr->Write(); p_tttt.h_met.ttzcr->Write();
    p_data.h_mll.ttzcr->Write(); p_ttw.h_mll.ttzcr->Write(); p_ttz.h_mll.ttzcr->Write(); p_tth.h_mll.ttzcr->Write(); p_wz.h_mll.ttzcr->Write(); p_ww.h_mll.ttzcr->Write(); p_xg.h_mll.ttzcr->Write(); p_rares.h_mll.ttzcr->Write(); p_flips.h_mll.ttzcr->Write(); p_fakes.h_mll.ttzcr->Write(); p_tttt.h_mll.ttzcr->Write();
    p_data.h_mtmin.ttzcr->Write(); p_ttw.h_mtmin.ttzcr->Write(); p_ttz.h_mtmin.ttzcr->Write(); p_tth.h_mtmin.ttzcr->Write(); p_wz.h_mtmin.ttzcr->Write(); p_ww.h_mtmin.ttzcr->Write(); p_xg.h_mtmin.ttzcr->Write(); p_rares.h_mtmin.ttzcr->Write(); p_flips.h_mtmin.ttzcr->Write(); p_fakes.h_mtmin.ttzcr->Write(); p_tttt.h_mtmin.ttzcr->Write();
    p_data.h_njets.ttzcr->Write(); p_ttw.h_njets.ttzcr->Write(); p_ttz.h_njets.ttzcr->Write(); p_tth.h_njets.ttzcr->Write(); p_wz.h_njets.ttzcr->Write(); p_ww.h_njets.ttzcr->Write(); p_xg.h_njets.ttzcr->Write(); p_rares.h_njets.ttzcr->Write(); p_flips.h_njets.ttzcr->Write(); p_fakes.h_njets.ttzcr->Write(); p_tttt.h_njets.ttzcr->Write();
    p_data.h_nleps.ttzcr->Write(); p_ttw.h_nleps.ttzcr->Write(); p_ttz.h_nleps.ttzcr->Write(); p_tth.h_nleps.ttzcr->Write(); p_wz.h_nleps.ttzcr->Write(); p_ww.h_nleps.ttzcr->Write(); p_xg.h_nleps.ttzcr->Write(); p_rares.h_nleps.ttzcr->Write(); p_flips.h_nleps.ttzcr->Write(); p_fakes.h_nleps.ttzcr->Write(); p_tttt.h_nleps.ttzcr->Write();
    p_data.h_wcands.ttzcr->Write(); p_ttw.h_wcands.ttzcr->Write(); p_ttz.h_wcands.ttzcr->Write(); p_tth.h_wcands.ttzcr->Write(); p_wz.h_wcands.ttzcr->Write(); p_ww.h_wcands.ttzcr->Write(); p_xg.h_wcands.ttzcr->Write(); p_rares.h_wcands.ttzcr->Write(); p_flips.h_wcands.ttzcr->Write(); p_fakes.h_wcands.ttzcr->Write(); p_tttt.h_wcands.ttzcr->Write();
    p_data.h_nbtags.ttzcr->Write(); p_ttw.h_nbtags.ttzcr->Write(); p_ttz.h_nbtags.ttzcr->Write(); p_tth.h_nbtags.ttzcr->Write(); p_wz.h_nbtags.ttzcr->Write(); p_ww.h_nbtags.ttzcr->Write(); p_xg.h_nbtags.ttzcr->Write(); p_rares.h_nbtags.ttzcr->Write(); p_flips.h_nbtags.ttzcr->Write(); p_fakes.h_nbtags.ttzcr->Write(); p_tttt.h_nbtags.ttzcr->Write();
    p_data.h_type.ttzcr->Write(); p_ttw.h_type.ttzcr->Write(); p_ttz.h_type.ttzcr->Write(); p_tth.h_type.ttzcr->Write(); p_wz.h_type.ttzcr->Write(); p_ww.h_type.ttzcr->Write(); p_xg.h_type.ttzcr->Write(); p_rares.h_type.ttzcr->Write(); p_flips.h_type.ttzcr->Write(); p_fakes.h_type.ttzcr->Write(); p_tttt.h_type.ttzcr->Write();
    p_data.h_charge.ttzcr->Write(); p_ttw.h_charge.ttzcr->Write(); p_ttz.h_charge.ttzcr->Write(); p_tth.h_charge.ttzcr->Write(); p_wz.h_charge.ttzcr->Write(); p_ww.h_charge.ttzcr->Write(); p_xg.h_charge.ttzcr->Write(); p_rares.h_charge.ttzcr->Write(); p_flips.h_charge.ttzcr->Write(); p_fakes.h_charge.ttzcr->Write(); p_tttt.h_charge.ttzcr->Write();
    p_data.h_l1pt.ttzcr->Write(); p_ttw.h_l1pt.ttzcr->Write(); p_ttz.h_l1pt.ttzcr->Write(); p_tth.h_l1pt.ttzcr->Write(); p_wz.h_l1pt.ttzcr->Write(); p_ww.h_l1pt.ttzcr->Write(); p_xg.h_l1pt.ttzcr->Write(); p_rares.h_l1pt.ttzcr->Write(); p_flips.h_l1pt.ttzcr->Write(); p_fakes.h_l1pt.ttzcr->Write(); p_tttt.h_l1pt.ttzcr->Write();
    p_data.h_l2pt.ttzcr->Write(); p_ttw.h_l2pt.ttzcr->Write(); p_ttz.h_l2pt.ttzcr->Write(); p_tth.h_l2pt.ttzcr->Write(); p_wz.h_l2pt.ttzcr->Write(); p_ww.h_l2pt.ttzcr->Write(); p_xg.h_l2pt.ttzcr->Write(); p_rares.h_l2pt.ttzcr->Write(); p_flips.h_l2pt.ttzcr->Write(); p_fakes.h_l2pt.ttzcr->Write(); p_tttt.h_l2pt.ttzcr->Write();
    p_data.h_l3pt.ttzcr->Write(); p_ttw.h_l3pt.ttzcr->Write(); p_ttz.h_l3pt.ttzcr->Write(); p_tth.h_l3pt.ttzcr->Write(); p_wz.h_l3pt.ttzcr->Write(); p_ww.h_l3pt.ttzcr->Write(); p_xg.h_l3pt.ttzcr->Write(); p_rares.h_l3pt.ttzcr->Write(); p_flips.h_l3pt.ttzcr->Write(); p_fakes.h_l3pt.ttzcr->Write(); p_tttt.h_l3pt.ttzcr->Write();
    p_data.h_el_l1pt.ttzcr->Write(); p_ttw.h_el_l1pt.ttzcr->Write(); p_ttz.h_el_l1pt.ttzcr->Write(); p_tth.h_el_l1pt.ttzcr->Write(); p_wz.h_el_l1pt.ttzcr->Write(); p_ww.h_el_l1pt.ttzcr->Write(); p_xg.h_el_l1pt.ttzcr->Write(); p_rares.h_el_l1pt.ttzcr->Write(); p_flips.h_el_l1pt.ttzcr->Write(); p_fakes.h_el_l1pt.ttzcr->Write(); p_tttt.h_el_l1pt.ttzcr->Write();
    p_data.h_el_l2pt.ttzcr->Write(); p_ttw.h_el_l2pt.ttzcr->Write(); p_ttz.h_el_l2pt.ttzcr->Write(); p_tth.h_el_l2pt.ttzcr->Write(); p_wz.h_el_l2pt.ttzcr->Write(); p_ww.h_el_l2pt.ttzcr->Write(); p_xg.h_el_l2pt.ttzcr->Write(); p_rares.h_el_l2pt.ttzcr->Write(); p_flips.h_el_l2pt.ttzcr->Write(); p_fakes.h_el_l2pt.ttzcr->Write(); p_tttt.h_el_l2pt.ttzcr->Write();
    p_data.h_el_l3pt.ttzcr->Write(); p_ttw.h_el_l3pt.ttzcr->Write(); p_ttz.h_el_l3pt.ttzcr->Write(); p_tth.h_el_l3pt.ttzcr->Write(); p_wz.h_el_l3pt.ttzcr->Write(); p_ww.h_el_l3pt.ttzcr->Write(); p_xg.h_el_l3pt.ttzcr->Write(); p_rares.h_el_l3pt.ttzcr->Write(); p_flips.h_el_l3pt.ttzcr->Write(); p_fakes.h_el_l3pt.ttzcr->Write(); p_tttt.h_el_l3pt.ttzcr->Write();
    p_data.h_mu_l1pt.ttzcr->Write(); p_ttw.h_mu_l1pt.ttzcr->Write(); p_ttz.h_mu_l1pt.ttzcr->Write(); p_tth.h_mu_l1pt.ttzcr->Write(); p_wz.h_mu_l1pt.ttzcr->Write(); p_ww.h_mu_l1pt.ttzcr->Write(); p_xg.h_mu_l1pt.ttzcr->Write(); p_rares.h_mu_l1pt.ttzcr->Write(); p_flips.h_mu_l1pt.ttzcr->Write(); p_fakes.h_mu_l1pt.ttzcr->Write(); p_tttt.h_mu_l1pt.ttzcr->Write();
    p_data.h_mu_l2pt.ttzcr->Write(); p_ttw.h_mu_l2pt.ttzcr->Write(); p_ttz.h_mu_l2pt.ttzcr->Write(); p_tth.h_mu_l2pt.ttzcr->Write(); p_wz.h_mu_l2pt.ttzcr->Write(); p_ww.h_mu_l2pt.ttzcr->Write(); p_xg.h_mu_l2pt.ttzcr->Write(); p_rares.h_mu_l2pt.ttzcr->Write(); p_flips.h_mu_l2pt.ttzcr->Write(); p_fakes.h_mu_l2pt.ttzcr->Write(); p_tttt.h_mu_l2pt.ttzcr->Write();
    p_data.h_mu_l3pt.ttzcr->Write(); p_ttw.h_mu_l3pt.ttzcr->Write(); p_ttz.h_mu_l3pt.ttzcr->Write(); p_tth.h_mu_l3pt.ttzcr->Write(); p_wz.h_mu_l3pt.ttzcr->Write(); p_ww.h_mu_l3pt.ttzcr->Write(); p_xg.h_mu_l3pt.ttzcr->Write(); p_rares.h_mu_l3pt.ttzcr->Write(); p_flips.h_mu_l3pt.ttzcr->Write(); p_fakes.h_mu_l3pt.ttzcr->Write(); p_tttt.h_mu_l3pt.ttzcr->Write();

    p_data.h_ht.ttwcr->Write(); p_ttw.h_ht.ttwcr->Write(); p_ttz.h_ht.ttwcr->Write(); p_tth.h_ht.ttwcr->Write(); p_wz.h_ht.ttwcr->Write(); p_ww.h_ht.ttwcr->Write(); p_xg.h_ht.ttwcr->Write(); p_rares.h_ht.ttwcr->Write(); p_flips.h_ht.ttwcr->Write(); p_fakes.h_ht.ttwcr->Write(); p_tttt.h_ht.ttwcr->Write();
    p_data.h_met.ttwcr->Write(); p_ttw.h_met.ttwcr->Write(); p_ttz.h_met.ttwcr->Write(); p_tth.h_met.ttwcr->Write(); p_wz.h_met.ttwcr->Write(); p_ww.h_met.ttwcr->Write(); p_xg.h_met.ttwcr->Write(); p_rares.h_met.ttwcr->Write(); p_flips.h_met.ttwcr->Write(); p_fakes.h_met.ttwcr->Write(); p_tttt.h_met.ttwcr->Write();
    p_data.h_mll.ttwcr->Write(); p_ttw.h_mll.ttwcr->Write(); p_ttz.h_mll.ttwcr->Write(); p_tth.h_mll.ttwcr->Write(); p_wz.h_mll.ttwcr->Write(); p_ww.h_mll.ttwcr->Write(); p_xg.h_mll.ttwcr->Write(); p_rares.h_mll.ttwcr->Write(); p_flips.h_mll.ttwcr->Write(); p_fakes.h_mll.ttwcr->Write(); p_tttt.h_mll.ttwcr->Write();
    p_data.h_mtmin.ttwcr->Write(); p_ttw.h_mtmin.ttwcr->Write(); p_ttz.h_mtmin.ttwcr->Write(); p_tth.h_mtmin.ttwcr->Write(); p_wz.h_mtmin.ttwcr->Write(); p_ww.h_mtmin.ttwcr->Write(); p_xg.h_mtmin.ttwcr->Write(); p_rares.h_mtmin.ttwcr->Write(); p_flips.h_mtmin.ttwcr->Write(); p_fakes.h_mtmin.ttwcr->Write(); p_tttt.h_mtmin.ttwcr->Write();
    p_data.h_njets.ttwcr->Write(); p_ttw.h_njets.ttwcr->Write(); p_ttz.h_njets.ttwcr->Write(); p_tth.h_njets.ttwcr->Write(); p_wz.h_njets.ttwcr->Write(); p_ww.h_njets.ttwcr->Write(); p_xg.h_njets.ttwcr->Write(); p_rares.h_njets.ttwcr->Write(); p_flips.h_njets.ttwcr->Write(); p_fakes.h_njets.ttwcr->Write(); p_tttt.h_njets.ttwcr->Write();
    p_data.h_nleps.ttwcr->Write(); p_ttw.h_nleps.ttwcr->Write(); p_ttz.h_nleps.ttwcr->Write(); p_tth.h_nleps.ttwcr->Write(); p_wz.h_nleps.ttwcr->Write(); p_ww.h_nleps.ttwcr->Write(); p_xg.h_nleps.ttwcr->Write(); p_rares.h_nleps.ttwcr->Write(); p_flips.h_nleps.ttwcr->Write(); p_fakes.h_nleps.ttwcr->Write(); p_tttt.h_nleps.ttwcr->Write();
    p_data.h_wcands.ttwcr->Write(); p_ttw.h_wcands.ttwcr->Write(); p_ttz.h_wcands.ttwcr->Write(); p_tth.h_wcands.ttwcr->Write(); p_wz.h_wcands.ttwcr->Write(); p_ww.h_wcands.ttwcr->Write(); p_xg.h_wcands.ttwcr->Write(); p_rares.h_wcands.ttwcr->Write(); p_flips.h_wcands.ttwcr->Write(); p_fakes.h_wcands.ttwcr->Write(); p_tttt.h_wcands.ttwcr->Write();
    p_data.h_nbtags.ttwcr->Write(); p_ttw.h_nbtags.ttwcr->Write(); p_ttz.h_nbtags.ttwcr->Write(); p_tth.h_nbtags.ttwcr->Write(); p_wz.h_nbtags.ttwcr->Write(); p_ww.h_nbtags.ttwcr->Write(); p_xg.h_nbtags.ttwcr->Write(); p_rares.h_nbtags.ttwcr->Write(); p_flips.h_nbtags.ttwcr->Write(); p_fakes.h_nbtags.ttwcr->Write(); p_tttt.h_nbtags.ttwcr->Write();
    p_data.h_type.ttwcr->Write(); p_ttw.h_type.ttwcr->Write(); p_ttz.h_type.ttwcr->Write(); p_tth.h_type.ttwcr->Write(); p_wz.h_type.ttwcr->Write(); p_ww.h_type.ttwcr->Write(); p_xg.h_type.ttwcr->Write(); p_rares.h_type.ttwcr->Write(); p_flips.h_type.ttwcr->Write(); p_fakes.h_type.ttwcr->Write(); p_tttt.h_type.ttwcr->Write();
    p_data.h_charge.ttwcr->Write(); p_ttw.h_charge.ttwcr->Write(); p_ttz.h_charge.ttwcr->Write(); p_tth.h_charge.ttwcr->Write(); p_wz.h_charge.ttwcr->Write(); p_ww.h_charge.ttwcr->Write(); p_xg.h_charge.ttwcr->Write(); p_rares.h_charge.ttwcr->Write(); p_flips.h_charge.ttwcr->Write(); p_fakes.h_charge.ttwcr->Write(); p_tttt.h_charge.ttwcr->Write();
    p_data.h_l1pt.ttwcr->Write(); p_ttw.h_l1pt.ttwcr->Write(); p_ttz.h_l1pt.ttwcr->Write(); p_tth.h_l1pt.ttwcr->Write(); p_wz.h_l1pt.ttwcr->Write(); p_ww.h_l1pt.ttwcr->Write(); p_xg.h_l1pt.ttwcr->Write(); p_rares.h_l1pt.ttwcr->Write(); p_flips.h_l1pt.ttwcr->Write(); p_fakes.h_l1pt.ttwcr->Write(); p_tttt.h_l1pt.ttwcr->Write();
    p_data.h_l2pt.ttwcr->Write(); p_ttw.h_l2pt.ttwcr->Write(); p_ttz.h_l2pt.ttwcr->Write(); p_tth.h_l2pt.ttwcr->Write(); p_wz.h_l2pt.ttwcr->Write(); p_ww.h_l2pt.ttwcr->Write(); p_xg.h_l2pt.ttwcr->Write(); p_rares.h_l2pt.ttwcr->Write(); p_flips.h_l2pt.ttwcr->Write(); p_fakes.h_l2pt.ttwcr->Write(); p_tttt.h_l2pt.ttwcr->Write();
    p_data.h_l3pt.ttwcr->Write(); p_ttw.h_l3pt.ttwcr->Write(); p_ttz.h_l3pt.ttwcr->Write(); p_tth.h_l3pt.ttwcr->Write(); p_wz.h_l3pt.ttwcr->Write(); p_ww.h_l3pt.ttwcr->Write(); p_xg.h_l3pt.ttwcr->Write(); p_rares.h_l3pt.ttwcr->Write(); p_flips.h_l3pt.ttwcr->Write(); p_fakes.h_l3pt.ttwcr->Write(); p_tttt.h_l3pt.ttwcr->Write();
    p_data.h_el_l1pt.ttwcr->Write(); p_ttw.h_el_l1pt.ttwcr->Write(); p_ttz.h_el_l1pt.ttwcr->Write(); p_tth.h_el_l1pt.ttwcr->Write(); p_wz.h_el_l1pt.ttwcr->Write(); p_ww.h_el_l1pt.ttwcr->Write(); p_xg.h_el_l1pt.ttwcr->Write(); p_rares.h_el_l1pt.ttwcr->Write(); p_flips.h_el_l1pt.ttwcr->Write(); p_fakes.h_el_l1pt.ttwcr->Write(); p_tttt.h_el_l1pt.ttwcr->Write();
    p_data.h_el_l2pt.ttwcr->Write(); p_ttw.h_el_l2pt.ttwcr->Write(); p_ttz.h_el_l2pt.ttwcr->Write(); p_tth.h_el_l2pt.ttwcr->Write(); p_wz.h_el_l2pt.ttwcr->Write(); p_ww.h_el_l2pt.ttwcr->Write(); p_xg.h_el_l2pt.ttwcr->Write(); p_rares.h_el_l2pt.ttwcr->Write(); p_flips.h_el_l2pt.ttwcr->Write(); p_fakes.h_el_l2pt.ttwcr->Write(); p_tttt.h_el_l2pt.ttwcr->Write();
    p_data.h_el_l3pt.ttwcr->Write(); p_ttw.h_el_l3pt.ttwcr->Write(); p_ttz.h_el_l3pt.ttwcr->Write(); p_tth.h_el_l3pt.ttwcr->Write(); p_wz.h_el_l3pt.ttwcr->Write(); p_ww.h_el_l3pt.ttwcr->Write(); p_xg.h_el_l3pt.ttwcr->Write(); p_rares.h_el_l3pt.ttwcr->Write(); p_flips.h_el_l3pt.ttwcr->Write(); p_fakes.h_el_l3pt.ttwcr->Write(); p_tttt.h_el_l3pt.ttwcr->Write();
    p_data.h_mu_l1pt.ttwcr->Write(); p_ttw.h_mu_l1pt.ttwcr->Write(); p_ttz.h_mu_l1pt.ttwcr->Write(); p_tth.h_mu_l1pt.ttwcr->Write(); p_wz.h_mu_l1pt.ttwcr->Write(); p_ww.h_mu_l1pt.ttwcr->Write(); p_xg.h_mu_l1pt.ttwcr->Write(); p_rares.h_mu_l1pt.ttwcr->Write(); p_flips.h_mu_l1pt.ttwcr->Write(); p_fakes.h_mu_l1pt.ttwcr->Write(); p_tttt.h_mu_l1pt.ttwcr->Write();
    p_data.h_mu_l2pt.ttwcr->Write(); p_ttw.h_mu_l2pt.ttwcr->Write(); p_ttz.h_mu_l2pt.ttwcr->Write(); p_tth.h_mu_l2pt.ttwcr->Write(); p_wz.h_mu_l2pt.ttwcr->Write(); p_ww.h_mu_l2pt.ttwcr->Write(); p_xg.h_mu_l2pt.ttwcr->Write(); p_rares.h_mu_l2pt.ttwcr->Write(); p_flips.h_mu_l2pt.ttwcr->Write(); p_fakes.h_mu_l2pt.ttwcr->Write(); p_tttt.h_mu_l2pt.ttwcr->Write();
    p_data.h_mu_l3pt.ttwcr->Write(); p_ttw.h_mu_l3pt.ttwcr->Write(); p_ttz.h_mu_l3pt.ttwcr->Write(); p_tth.h_mu_l3pt.ttwcr->Write(); p_wz.h_mu_l3pt.ttwcr->Write(); p_ww.h_mu_l3pt.ttwcr->Write(); p_xg.h_mu_l3pt.ttwcr->Write(); p_rares.h_mu_l3pt.ttwcr->Write(); p_flips.h_mu_l3pt.ttwcr->Write(); p_fakes.h_mu_l3pt.ttwcr->Write(); p_tttt.h_mu_l3pt.ttwcr->Write();

    p_data.h_ht.br->Write(); p_ttw.h_ht.br->Write(); p_ttz.h_ht.br->Write(); p_tth.h_ht.br->Write(); p_wz.h_ht.br->Write(); p_ww.h_ht.br->Write(); p_xg.h_ht.br->Write(); p_rares.h_ht.br->Write(); p_flips.h_ht.br->Write(); p_fakes.h_ht.br->Write(); p_tttt.h_ht.br->Write();
    p_data.h_met.br->Write(); p_ttw.h_met.br->Write(); p_ttz.h_met.br->Write(); p_tth.h_met.br->Write(); p_wz.h_met.br->Write(); p_ww.h_met.br->Write(); p_xg.h_met.br->Write(); p_rares.h_met.br->Write(); p_flips.h_met.br->Write(); p_fakes.h_met.br->Write(); p_tttt.h_met.br->Write();
    p_data.h_mll.br->Write(); p_ttw.h_mll.br->Write(); p_ttz.h_mll.br->Write(); p_tth.h_mll.br->Write(); p_wz.h_mll.br->Write(); p_ww.h_mll.br->Write(); p_xg.h_mll.br->Write(); p_rares.h_mll.br->Write(); p_flips.h_mll.br->Write(); p_fakes.h_mll.br->Write(); p_tttt.h_mll.br->Write();
    p_data.h_mtmin.br->Write(); p_ttw.h_mtmin.br->Write(); p_ttz.h_mtmin.br->Write(); p_tth.h_mtmin.br->Write(); p_wz.h_mtmin.br->Write(); p_ww.h_mtmin.br->Write(); p_xg.h_mtmin.br->Write(); p_rares.h_mtmin.br->Write(); p_flips.h_mtmin.br->Write(); p_fakes.h_mtmin.br->Write(); p_tttt.h_mtmin.br->Write();
    p_data.h_njets.br->Write(); p_ttw.h_njets.br->Write(); p_ttz.h_njets.br->Write(); p_tth.h_njets.br->Write(); p_wz.h_njets.br->Write(); p_ww.h_njets.br->Write(); p_xg.h_njets.br->Write(); p_rares.h_njets.br->Write(); p_flips.h_njets.br->Write(); p_fakes.h_njets.br->Write(); p_tttt.h_njets.br->Write();
    p_data.h_nleps.br->Write(); p_ttw.h_nleps.br->Write(); p_ttz.h_nleps.br->Write(); p_tth.h_nleps.br->Write(); p_wz.h_nleps.br->Write(); p_ww.h_nleps.br->Write(); p_xg.h_nleps.br->Write(); p_rares.h_nleps.br->Write(); p_flips.h_nleps.br->Write(); p_fakes.h_nleps.br->Write(); p_tttt.h_nleps.br->Write();
    p_data.h_wcands.br->Write(); p_ttw.h_wcands.br->Write(); p_ttz.h_wcands.br->Write(); p_tth.h_wcands.br->Write(); p_wz.h_wcands.br->Write(); p_ww.h_wcands.br->Write(); p_xg.h_wcands.br->Write(); p_rares.h_wcands.br->Write(); p_flips.h_wcands.br->Write(); p_fakes.h_wcands.br->Write(); p_tttt.h_wcands.br->Write();
    p_data.h_nbtags.br->Write(); p_ttw.h_nbtags.br->Write(); p_ttz.h_nbtags.br->Write(); p_tth.h_nbtags.br->Write(); p_wz.h_nbtags.br->Write(); p_ww.h_nbtags.br->Write(); p_xg.h_nbtags.br->Write(); p_rares.h_nbtags.br->Write(); p_flips.h_nbtags.br->Write(); p_fakes.h_nbtags.br->Write(); p_tttt.h_nbtags.br->Write();
    p_data.h_type.br->Write(); p_ttw.h_type.br->Write(); p_ttz.h_type.br->Write(); p_tth.h_type.br->Write(); p_wz.h_type.br->Write(); p_ww.h_type.br->Write(); p_xg.h_type.br->Write(); p_rares.h_type.br->Write(); p_flips.h_type.br->Write(); p_fakes.h_type.br->Write(); p_tttt.h_type.br->Write();
    p_data.h_charge.br->Write(); p_ttw.h_charge.br->Write(); p_ttz.h_charge.br->Write(); p_tth.h_charge.br->Write(); p_wz.h_charge.br->Write(); p_ww.h_charge.br->Write(); p_xg.h_charge.br->Write(); p_rares.h_charge.br->Write(); p_flips.h_charge.br->Write(); p_fakes.h_charge.br->Write(); p_tttt.h_charge.br->Write();
    p_data.h_l1pt.br->Write(); p_ttw.h_l1pt.br->Write(); p_ttz.h_l1pt.br->Write(); p_tth.h_l1pt.br->Write(); p_wz.h_l1pt.br->Write(); p_ww.h_l1pt.br->Write(); p_xg.h_l1pt.br->Write(); p_rares.h_l1pt.br->Write(); p_flips.h_l1pt.br->Write(); p_fakes.h_l1pt.br->Write(); p_tttt.h_l1pt.br->Write();
    p_data.h_l2pt.br->Write(); p_ttw.h_l2pt.br->Write(); p_ttz.h_l2pt.br->Write(); p_tth.h_l2pt.br->Write(); p_wz.h_l2pt.br->Write(); p_ww.h_l2pt.br->Write(); p_xg.h_l2pt.br->Write(); p_rares.h_l2pt.br->Write(); p_flips.h_l2pt.br->Write(); p_fakes.h_l2pt.br->Write(); p_tttt.h_l2pt.br->Write();
    p_data.h_l3pt.br->Write(); p_ttw.h_l3pt.br->Write(); p_ttz.h_l3pt.br->Write(); p_tth.h_l3pt.br->Write(); p_wz.h_l3pt.br->Write(); p_ww.h_l3pt.br->Write(); p_xg.h_l3pt.br->Write(); p_rares.h_l3pt.br->Write(); p_flips.h_l3pt.br->Write(); p_fakes.h_l3pt.br->Write(); p_tttt.h_l3pt.br->Write();
    p_data.h_el_l1pt.br->Write(); p_ttw.h_el_l1pt.br->Write(); p_ttz.h_el_l1pt.br->Write(); p_tth.h_el_l1pt.br->Write(); p_wz.h_el_l1pt.br->Write(); p_ww.h_el_l1pt.br->Write(); p_xg.h_el_l1pt.br->Write(); p_rares.h_el_l1pt.br->Write(); p_flips.h_el_l1pt.br->Write(); p_fakes.h_el_l1pt.br->Write(); p_tttt.h_el_l1pt.br->Write();
    p_data.h_el_l2pt.br->Write(); p_ttw.h_el_l2pt.br->Write(); p_ttz.h_el_l2pt.br->Write(); p_tth.h_el_l2pt.br->Write(); p_wz.h_el_l2pt.br->Write(); p_ww.h_el_l2pt.br->Write(); p_xg.h_el_l2pt.br->Write(); p_rares.h_el_l2pt.br->Write(); p_flips.h_el_l2pt.br->Write(); p_fakes.h_el_l2pt.br->Write(); p_tttt.h_el_l2pt.br->Write();
    p_data.h_el_l3pt.br->Write(); p_ttw.h_el_l3pt.br->Write(); p_ttz.h_el_l3pt.br->Write(); p_tth.h_el_l3pt.br->Write(); p_wz.h_el_l3pt.br->Write(); p_ww.h_el_l3pt.br->Write(); p_xg.h_el_l3pt.br->Write(); p_rares.h_el_l3pt.br->Write(); p_flips.h_el_l3pt.br->Write(); p_fakes.h_el_l3pt.br->Write(); p_tttt.h_el_l3pt.br->Write();
    p_data.h_mu_l1pt.br->Write(); p_ttw.h_mu_l1pt.br->Write(); p_ttz.h_mu_l1pt.br->Write(); p_tth.h_mu_l1pt.br->Write(); p_wz.h_mu_l1pt.br->Write(); p_ww.h_mu_l1pt.br->Write(); p_xg.h_mu_l1pt.br->Write(); p_rares.h_mu_l1pt.br->Write(); p_flips.h_mu_l1pt.br->Write(); p_fakes.h_mu_l1pt.br->Write(); p_tttt.h_mu_l1pt.br->Write();
    p_data.h_mu_l2pt.br->Write(); p_ttw.h_mu_l2pt.br->Write(); p_ttz.h_mu_l2pt.br->Write(); p_tth.h_mu_l2pt.br->Write(); p_wz.h_mu_l2pt.br->Write(); p_ww.h_mu_l2pt.br->Write(); p_xg.h_mu_l2pt.br->Write(); p_rares.h_mu_l2pt.br->Write(); p_flips.h_mu_l2pt.br->Write(); p_fakes.h_mu_l2pt.br->Write(); p_tttt.h_mu_l2pt.br->Write();
    p_data.h_mu_l3pt.br->Write(); p_ttw.h_mu_l3pt.br->Write(); p_ttz.h_mu_l3pt.br->Write(); p_tth.h_mu_l3pt.br->Write(); p_wz.h_mu_l3pt.br->Write(); p_ww.h_mu_l3pt.br->Write(); p_xg.h_mu_l3pt.br->Write(); p_rares.h_mu_l3pt.br->Write(); p_flips.h_mu_l3pt.br->Write(); p_fakes.h_mu_l3pt.br->Write(); p_tttt.h_mu_l3pt.br->Write();

    p_data.h_disc.br->Write(); p_ttw.h_disc.br->Write(); p_ttz.h_disc.br->Write(); p_tth.h_disc.br->Write(); p_wz.h_disc.br->Write(); p_ww.h_disc.br->Write(); p_xg.h_disc.br->Write(); p_rares.h_disc.br->Write(); p_flips.h_disc.br->Write(); p_fakes.h_disc.br->Write(); p_tttt.h_disc.br->Write();
    p_data.SRCR.TOTAL->Write(); p_ttw.SRCR.TOTAL->Write(); p_ttz.SRCR.TOTAL->Write(); p_tth.SRCR.TOTAL->Write(); p_wz.SRCR.TOTAL->Write(); p_ww.SRCR.TOTAL->Write(); p_xg.SRCR.TOTAL->Write(); p_rares.SRCR.TOTAL->Write(); p_flips.SRCR.TOTAL->Write(); p_fakes.SRCR.TOTAL->Write(); p_tttt.SRCR.TOTAL->Write();
    p_data.SR.TOTAL->Write(); p_ttw.SR.TOTAL->Write(); p_ttz.SR.TOTAL->Write(); p_tth.SR.TOTAL->Write(); p_wz.SR.TOTAL->Write(); p_ww.SR.TOTAL->Write(); p_xg.SR.TOTAL->Write(); p_rares.SR.TOTAL->Write(); p_flips.SR.TOTAL->Write(); p_fakes.SR.TOTAL->Write(); p_tttt.SR.TOTAL->Write();

    fout->Write();
    fout->Close();

    // // Write output tree
    // out_file->cd();
    // out_tree->Write();
    // out_file->Close();

}

//doFakes = 1 for QCD, 2 for inSitu
pair<yields_t, plots_t> run(TChain *chain, bool isData, bool doFlips, int doFakes, int exclude, bool isSignal, bool isGamma){

    // out_file->cd();


    TString chainTitle = chain->GetTitle();
    const char* chainTitleCh = chainTitle.Data();
    std::cout << "Working on " << chainTitle << std::endl;

    bool isWZ = (chainTitle=="wz");
    // bool isttZ = (chainTitle=="ttzh");
    bool isttZ = (chainTitle=="ttz");
    bool isttW = (chainTitle=="ttw");
    bool isOS = (chainTitle=="ttbar_os");

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
    p_result.h_wcands.sr   = new TH1F(Form("sr_wcands_%s"     , chainTitleCh) , Form("wcands_%s"     , chainTitleCh) , 10      , 0    , 10);

    p_result.h_ht.ttzcr       = new TH1F(Form("ttzcr_ht_%s"         , chainTitleCh) , Form("ht_%s"         , chainTitleCh) , 15      , 100   , 1600);
    p_result.h_met.ttzcr      = new TH1F(Form("ttzcr_met_%s"        , chainTitleCh) , Form("met_%s"        , chainTitleCh) , 15      , 0    , 600);
    p_result.h_mll.ttzcr      = new TH1F(Form("ttzcr_mll_%s"        , chainTitleCh) , Form("mll_%s"        , chainTitleCh) , 20      , 0    , 200);
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
    p_result.h_wcands.ttzcr   = new TH1F(Form("ttzcr_wcands_%s"     , chainTitleCh) , Form("wcands_%s"     , chainTitleCh) , 10      , 0    , 10);

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
    p_result.h_wcands.ttwcr   = new TH1F(Form("ttwcr_wcands_%s"     , chainTitleCh) , Form("wcands_%s"     , chainTitleCh) , 10      , 0    , 10);

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
    p_result.h_wcands.br   = new TH1F(Form("br_wcands_%s"     , chainTitleCh) , Form("wcands_%s"     , chainTitleCh) , 10      , 0    , 10);

    p_result.h_disc.br     = new TH1F(Form("br_disc_%s"       , chainTitleCh) , Form("disc_%s"       , chainTitleCh) , 8       , 0    , 1.0);
    p_result.SRCR.TOTAL = new TH1F(Form("SRCR_TOTAL_%s" , chainTitleCh) , Form("SRCR_TOTAL_%s" , chainTitleCh) , nsr     , 0.5  , nsr+0.5);
    p_result.SR.TOTAL   = new TH1F(Form("SR_TOTAL_%s"   , chainTitleCh) , Form("SR_TOTAL_%s"   , chainTitleCh) , nsr-nCR , 0.5  , nsr-nCR+0.5);


    bool doPoisson = isData && !doFlips && !doFakes;

    initHistError(doPoisson, p_result.h_ht.sr         );
    initHistError(doPoisson, p_result.h_met.sr        );
    initHistError(doPoisson, p_result.h_mll.sr        );
    initHistError(doPoisson, p_result.h_mtmin.sr      );
    initHistError(doPoisson, p_result.h_njets.sr      );
    initHistError(doPoisson, p_result.h_nleps.sr      );
    initHistError(doPoisson, p_result.h_nbtags.sr     );
    initHistError(doPoisson, p_result.h_type.sr       );
    initHistError(doPoisson, p_result.h_charge.sr     );
    initHistError(doPoisson, p_result.h_l1pt.sr);
    initHistError(doPoisson, p_result.h_l2pt.sr);
    initHistError(doPoisson, p_result.h_l3pt.sr);
    initHistError(doPoisson, p_result.h_el_l1pt.sr);
    initHistError(doPoisson, p_result.h_el_l2pt.sr);
    initHistError(doPoisson, p_result.h_el_l3pt.sr);
    initHistError(doPoisson, p_result.h_mu_l1pt.sr);
    initHistError(doPoisson, p_result.h_mu_l2pt.sr);
    initHistError(doPoisson, p_result.h_mu_l3pt.sr);

    initHistError(doPoisson, p_result.h_ht.ttzcr         );
    initHistError(doPoisson, p_result.h_met.ttzcr        );
    initHistError(doPoisson, p_result.h_mll.ttzcr        );
    initHistError(doPoisson, p_result.h_mtmin.ttzcr      );
    initHistError(doPoisson, p_result.h_njets.ttzcr      );
    initHistError(doPoisson, p_result.h_nleps.ttzcr      );
    initHistError(doPoisson, p_result.h_nbtags.ttzcr     );
    initHistError(doPoisson, p_result.h_type.ttzcr       );
    initHistError(doPoisson, p_result.h_charge.ttzcr     );
    initHistError(doPoisson, p_result.h_l1pt.ttzcr);
    initHistError(doPoisson, p_result.h_l2pt.ttzcr);
    initHistError(doPoisson, p_result.h_l3pt.ttzcr);
    initHistError(doPoisson, p_result.h_el_l1pt.ttzcr);
    initHistError(doPoisson, p_result.h_el_l2pt.ttzcr);
    initHistError(doPoisson, p_result.h_el_l3pt.ttzcr);
    initHistError(doPoisson, p_result.h_mu_l1pt.ttzcr);
    initHistError(doPoisson, p_result.h_mu_l2pt.ttzcr);
    initHistError(doPoisson, p_result.h_mu_l3pt.ttzcr);

    initHistError(doPoisson, p_result.h_ht.ttwcr         );
    initHistError(doPoisson, p_result.h_met.ttwcr        );
    initHistError(doPoisson, p_result.h_mll.ttwcr        );
    initHistError(doPoisson, p_result.h_mtmin.ttwcr      );
    initHistError(doPoisson, p_result.h_njets.ttwcr      );
    initHistError(doPoisson, p_result.h_nleps.ttwcr      );
    initHistError(doPoisson, p_result.h_nbtags.ttwcr     );
    initHistError(doPoisson, p_result.h_type.ttwcr       );
    initHistError(doPoisson, p_result.h_charge.ttwcr     );
    initHistError(doPoisson, p_result.h_l1pt.ttwcr);
    initHistError(doPoisson, p_result.h_l2pt.ttwcr);
    initHistError(doPoisson, p_result.h_l3pt.ttwcr);
    initHistError(doPoisson, p_result.h_el_l1pt.ttwcr);
    initHistError(doPoisson, p_result.h_el_l2pt.ttwcr);
    initHistError(doPoisson, p_result.h_el_l3pt.ttwcr);
    initHistError(doPoisson, p_result.h_mu_l1pt.ttwcr);
    initHistError(doPoisson, p_result.h_mu_l2pt.ttwcr);
    initHistError(doPoisson, p_result.h_mu_l3pt.ttwcr);

    initHistError(doPoisson, p_result.h_ht.br         );
    initHistError(doPoisson, p_result.h_met.br        );
    initHistError(doPoisson, p_result.h_mll.br        );
    initHistError(doPoisson, p_result.h_mtmin.br      );
    initHistError(doPoisson, p_result.h_njets.br      );
    initHistError(doPoisson, p_result.h_nleps.br      );
    initHistError(doPoisson, p_result.h_nbtags.br     );
    initHistError(doPoisson, p_result.h_type.br       );
    initHistError(doPoisson, p_result.h_charge.br     );
    initHistError(doPoisson, p_result.h_l1pt.br);
    initHistError(doPoisson, p_result.h_l2pt.br);
    initHistError(doPoisson, p_result.h_l3pt.br);
    initHistError(doPoisson, p_result.h_el_l1pt.br);
    initHistError(doPoisson, p_result.h_el_l2pt.br);
    initHistError(doPoisson, p_result.h_el_l3pt.br);
    initHistError(doPoisson, p_result.h_mu_l1pt.br);
    initHistError(doPoisson, p_result.h_mu_l2pt.br);
    initHistError(doPoisson, p_result.h_mu_l3pt.br);

    initHistError(doPoisson, p_result.h_disc.br);
    initHistError(doPoisson, p_result.SRCR.TOTAL   );
    initHistError(doPoisson, p_result.SR.TOTAL   );


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
            if (!ss::fired_trigger()) continue;
            if (!ss::passes_met_filters()) continue;

            if (doCustomSelection) {

            }

            float weight =  ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG;
            weight*=scaleLumi;

            if (DOWEIGHTS) {
                if (!ss::is_real_data()) {
                    weight *= getTruePUw_Moriond(ss::trueNumInt()[0]);
                }
                weight*=ss::weight_btagsf();
                // if (isWZ) weight*=getWZSF();
                // if (isttZ && applyttZSF) weight*=getttZSF();
                //apply lepton scale factors
                if (ss::is_real_data()==0 && (!isWZ) && (!isttZ || !applyttZSF)) {
                    weight*=eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
                }

            }

            // if (doTTZISR && isttZ) {
            //     if (filename.Contains("TTZnlo")) weight *= ss::weight_isr()*isr_norm_TTZnlo(1,1,  0);
            //     if (filename.Contains("TTZLOW")) weight *= ss::weight_isr()*isr_norm_TTZnlo(1,1,  0);
            // }
            // if (doTTWISR && isttW) {
            //     if (filename.Contains("TTWnlo")) weight *= ss::weight_isr()*isr_norm_TTWnlo(1,1,  0);
            // }



            float lep1_pt = ss::lep1_coneCorrPt();
            float lep2_pt = ss::lep2_coneCorrPt();
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
            if (isOS) {
                ssclass = 4;
                weight *= 0.001;
            }
            bool isClass6 = ss::hyp_class() == 6;
            if (!doFlips && !doFakes && exclude == 0) {
                if (ss::hyp_class() != ssclass && !isClass6) continue;
                //first check the charge (since isPrompt does not know about it)
                if (!isData && !isGamma && ss::lep1_motherID()==2) continue;
                if (!isData && !isGamma && ss::lep2_motherID()==2) continue;
                //then allow events with two prompt leptons or with prompt photons
                //photons can give fakes from conversions (not accounted by data-driven method) 
                if (!isData && !( (ss::lep1_motherID()==1 && ss::lep2_motherID()==1) || (ss::lep1_motherID()==-3 || ss::lep2_motherID()==-3)) ) continue;
                //if (!isData && !isGamma && (ss::lep1_motherID()!=1 || ss::lep2_motherID()!=1)) continue;
                //if (isGamma && (ss::lep1_motherID()!=-3 && ss::lep2_motherID()!=-3)) continue;
            }

            if (!doFlips && !doFakes && exclude == 1){
                if (ss::hyp_class() != 3 && !isClass6) continue;
                if (!isData && !((ss::lep1_motherID() == 1 && ss::lep2_motherID() < 0) || (ss::lep1_motherID() < 0 && ss::lep2_motherID() == 1))) continue;
            }

            if (!doFlips && !doFakes && exclude == 2){
                if (ss::hyp_class() != 3 && !isClass6) continue;
                if (!isData && !((ss::lep1_motherID() == 1 && ss::lep2_motherID() == 2) || (ss::lep1_motherID() == 2 && ss::lep2_motherID() == 1))) continue;
            }

            if (!doFlips && !doFakes && exclude == 3){
                if (ss::hyp_class() != 3 && !isClass6) continue;
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

            //QCD Fakes
            bool isClass6Fake = false;
            if (doFakes == 1){
                if (!isClass6) {
                    if (ss::hyp_class() != 2 && ss::hyp_class() != 1) continue;
                    if (ss::lep1_passes_id()==0) {
                        float fr = fakeRate(ss::lep1_id(),ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), ss::ht());
                        weight *= fr/(1.-fr);
                    }
                    if (ss::lep2_passes_id()==0) {
                        float fr = fakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), ss::ht());
                        weight *= fr/(1.-fr);
                    }
                    //subtract double FO
                    if (ss::hyp_class() == 1) weight *= -1.;
                    if (!ss::is_real_data() && isData) {
                        weight *= -1.;
                    }
                } else {
                    if(ss::is_real_data() && (ss::lep3_fo() && !ss::lep3_tight()) && ss::lep1_passes_id() && ss::lep2_passes_id()) {  // lep3 fake
                        float fr = fakeRate(ss::lep3_id(),ss::lep3_p4().pt(),ss::lep3_p4().eta(),ss::ht());
                        isClass6Fake = true;
                        weight *= fr / (1-fr);
                    }
                    if(ss::is_real_data() && (ss::lep2_fo() && !ss::lep2_tight()) && ss::lep1_passes_id() && ss::lep3_passes_id()) {  // lep2 fake
                        float fr = fakeRate(ss::lep2_id(),ss::lep2_p4().pt(),ss::lep2_p4().eta(),ss::ht());
                        isClass6Fake = true;
                        weight *= fr / (1-fr);
                    }
                    if(ss::is_real_data() && (ss::lep1_fo() && !ss::lep1_tight()) && ss::lep2_passes_id() && ss::lep3_passes_id()) {  // lep1 fake
                        float fr = fakeRate(ss::lep1_id(),ss::lep1_p4().pt(),ss::lep1_p4().eta(),ss::ht());
                        isClass6Fake = true;
                        weight *= fr / (1-fr);
                    }
                }
            }

            if (isClass6) {
                float zmass;
                float zmass23 = ss::lep2_id() == -ss::lep3_id() ? (ss::lep2_p4()+ss::lep3_p4()).mass() : -999.0;
                float zmass31 = ss::lep3_id() == -ss::lep1_id() ? (ss::lep3_p4()+ss::lep1_p4()).mass() : -999.0;
                int iLepFromW = -1;
                if( fabs(zmass31 - 91.2) < fabs(zmass23 - 91.2) ) {
                    zmass = zmass31;
                    iLepFromW = 2;
                } else {
                    zmass = zmass23;
                    iLepFromW = 1;
                }
                if (fabs(zmass-91.2) > 15) continue;
                if (doFakes && isData) {
                    if (!ss::is_real_data()) continue;
                    if (!isClass6Fake) continue;
                }
                if( !( isClass6Fake || (ss::lep1_passes_id() && ss::lep2_passes_id() && ss::lep3_passes_id()) ) ) continue;
            }

            int nleps = 2;
            if (ss::lep3_passes_id()) nleps++;
            if (ss::lep4_passes_id()) nleps++;

            //Require nominal baseline selections
            int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, true);
            if (BR < 0) continue;

            // tttt baseline selections
            if (ss::njets() < 2) continue;
            if (ss::nbtags() < 2) continue;
            if (ss::met() < 50) continue;
            if (ss::ht() < 300) continue;
            if (lep1_pt < 25.) continue;
            if (lep2_pt < 20.) continue;


            //Reject duplicates (after selection otherwise flips are ignored...)
            if (isData && ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)){ continue; }
            }


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
                // if (isttW && ss::btags()[ijet].pt()) {
                //     if (abs(ss::btags_flavor()[ijet])==5) ngenb++;
                //     else if (abs(ss::btags_flavor()[ijet])==4) ngenc++;
                //     else ngenlight++;
                // }
            }
            int nlb40 = 0;
            int nmb40 = 0;
            int ntb40 = 0;
            float ht_first4 = 0.;
            float ht_rest = 0.;
            for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
                if (ss::jets_disc_ivf().at(ijet) > bloose) nlb40++;
                if (ss::jets_disc_ivf().at(ijet) > bmed)   nmb40++;
                if (ss::jets_disc_ivf().at(ijet) > btight) ntb40++;

                if (ijet < 4) ht_first4 += ss::jets()[ijet].pt();
                else ht_rest += ss::jets()[ijet].pt();

                // skip b-tagged jets
                if (ss::jets_disc_ivf().at(ijet) > 0.8484) continue;
                jets.push_back(ss::jets()[ijet]);
            }
            if (isttW) {
                // std::cout << " ngenb: " << ngenb << " ngenc: " << ngenc << " ngenlight: " << ngenlight << " ss::nbtags(): " << ss::nbtags() << std::endl;
            }
            int wcands = 0;
            // int topcands = 0;
            for (unsigned int i = 0; i < jets.size(); i++) {
                // p_result.h_jetmass.br->Fill(jets[i].M(), weight);
                for (unsigned int j = i+1; j < jets.size(); j++) {
                    float wmass = (jets[i]+jets[j]).M();
                    // p_result.h_wmass.br->Fill(wmass, weight);
                    if (fabs(wmass-80.4) < 20.0) {
                        wcands++;
                        // for (unsigned int k = 0; k < bjets.size(); k++) {
                        //     float topmass = (jets[i]+jets[j]+bjets[k]).M();
                        //     p_result.h_topmass.br->Fill(topmass, weight);
                        //     if (fabs(topmass-173) < 45.0) {
                        //         // topcands++;
                        //     }
                        // }

                    }
                }
            }

            float tree_nbtags = ss::nbtags();
            float tree_njets = ss::njets();
            float tree_mt1 = mtl1;
            float tree_mt2 = mtl2;
            float tree_met = ss::met();
            float tree_ml1l2 = (ss::lep1_p4()+ss::lep2_p4()).M();
            float tree_htb = htb;
            float tree_nleps = nleps;
            float tree_ht = ss::ht();
            float tree_mj1j2 = (ss::jets().at(0)+ss::jets().at(1)).M();
            float tree_dphij1j2 = DeltaPhi(ss::jets().at(0).phi(),ss::jets().at(1).phi());
            float tree_ptj1 = ss::jets().at(0).pt();
            float tree_ptj2 = ss::jets().at(1).pt();
            float tree_ml1j2 = (ss::lep1_p4()+ss::jets().at(1)).M();
            float tree_ml1j1 = (ss::lep1_p4()+ss::jets().at(0)).M();
            float tree_wcands = wcands;
            float tree_dphil1j1 = DeltaPhi(ss::lep1_p4().phi(),ss::jets().at(0).phi());
            float tree_detal1l2 = fabs(ss::lep1_p4().eta()-ss::lep2_p4().eta());
            int tree_q1 = (ss::lep1_id()<0)-(ss::lep1_id()>0);
            int tree_q2 = (ss::lep2_id()<0)-(ss::lep2_id()>0);
            int tree_q3 = nleps >= 3 ? ((ss::lep3_id()<0)-(ss::lep3_id()>0)) : 0;
            float tree_ht4ratio = ss::njets()>4 ? ht_first4/ht_rest : 0;
            int tree_nlb40 = nlb40;
            int tree_nmb40 = nmb40;
            int tree_ntb40 = ntb40;

            float pt1 = lep1_pt;
            float pt2 = lep2_pt;
            float pt3 = ss::lep3_p4().pt();
            float pto1 = max(max(pt1,pt2),pt3);
            float pto3 = min(min(pt1,pt2),pt3);
            float pto2 = pt1+pt2+pt3-pto1-pto3;
            int mytype = ss::hyp_type();
            if (mytype==2 && abs(ss::lep1_id())==13) mytype = 1;
            float mll = (ss::lep1_p4()*lep1_pt/ss::lep1_p4().pt()+ss::lep2_p4()*lep2_pt/ss::lep2_p4().pt()).M();

            // // fill discriminant before requiring SR
            // float pred = get_prediction((float)tree_njets,(float)tree_nbtags,(float)tree_mt1,(float)tree_mt2,(float)tree_met,(float)tree_ml1l2,(float)tree_htb,(float)tree_nleps,(float)tree_ht,(float)tree_mj1j2,(float)tree_dphij1j2,(float)tree_ptj1,(float)tree_ptj2,(float)tree_ml1j2,(float)tree_ml1j1,(float)tree_wcands,(float)tree_dphil1j1,(float)tree_detal1l2,(float)tree_nlb40,(float)tree_nmb40,(float)tree_ntb40,(float)tree_q1,(float)tree_q2,(float)tree_ht4ratio);
            // p_result.h_disc.br->Fill(pred,weight);

            // if (isttW) std::cout << weight << std::endl;


            // require SRs+CR (= ttZ CR + ttW CR + SRs)
            int SR = signalRegionTest(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, nleps, isClass6);
            if (SR < 0) continue;

            // if (isttW) std::cout << weight << " " << SR << std::endl;

            p_result.SRCR.TOTAL->Fill(SR, weight); 

            if (SR > 1) { // non ttZ CR
                p_result.h_wcands.br->Fill(wcands, weight);
                p_result.h_njets.br->Fill(ss::njets() , weight);
                p_result.h_nbtags.br->Fill(ss::nbtags() , weight);
                p_result.h_type.br->Fill(mytype , weight);
                p_result.h_charge.br->Fill(ss::lep1_id() > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.br->Fill(nleps , weight);
                p_result.h_ht.br->Fill(ss::ht() , weight);
                p_result.h_met.br->Fill(ss::met() , weight);
                p_result.h_mll.br->Fill(mll , weight);
                p_result.h_mtmin.br->Fill(mtmin , weight);
                p_result.h_l1pt.br->Fill(pto1, weight);
                p_result.h_l2pt.br->Fill(pto2, weight);
                (abs(ss::lep1_id()) == 11) ? p_result.h_el_l1pt.br->Fill(pto1, weight) : p_result.h_mu_l1pt.br->Fill(pto1, weight);
                (abs(ss::lep2_id()) == 11) ? p_result.h_el_l2pt.br->Fill(pto2, weight) : p_result.h_mu_l2pt.br->Fill(pto2, weight);
                if (nleps > 2) {
                    p_result.h_l3pt.br->Fill(pto3, weight);
                    (abs(ss::lep3_id()) == 11) ? p_result.h_el_l3pt.br->Fill(pto3, weight) : p_result.h_mu_l3pt.br->Fill(pto3, weight);
                }
            }

            if (SR == 1) { // ttZ CR
                p_result.h_wcands.ttzcr->Fill(wcands, weight);
                p_result.h_njets.ttzcr->Fill(ss::njets() , weight);
                p_result.h_nbtags.ttzcr->Fill(ss::nbtags() , weight);
                p_result.h_type.ttzcr->Fill(mytype , weight);
                p_result.h_charge.ttzcr->Fill(ss::lep1_id() > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.ttzcr->Fill(nleps , weight);
                p_result.h_ht.ttzcr->Fill(ss::ht() , weight);
                p_result.h_met.ttzcr->Fill(ss::met() , weight);
                p_result.h_mll.ttzcr->Fill(mll , weight);
                p_result.h_mtmin.ttzcr->Fill(mtmin , weight);
                p_result.h_l1pt.ttzcr->Fill(pto1, weight);
                p_result.h_l2pt.ttzcr->Fill(pto2, weight);
                (abs(ss::lep1_id()) == 11) ? p_result.h_el_l1pt.ttzcr->Fill(pto1, weight) : p_result.h_mu_l1pt.ttzcr->Fill(pto1, weight);
                (abs(ss::lep2_id()) == 11) ? p_result.h_el_l2pt.ttzcr->Fill(pto2, weight) : p_result.h_mu_l2pt.ttzcr->Fill(pto2, weight);
                if (nleps > 2) {
                    p_result.h_l3pt.ttzcr->Fill(pto3, weight);
                    (abs(ss::lep3_id()) == 11) ? p_result.h_el_l3pt.ttzcr->Fill(pto3, weight) : p_result.h_mu_l3pt.ttzcr->Fill(pto3, weight);
                }
            }

            if (SR == 2) { // ttW CR
                p_result.h_wcands.ttwcr->Fill(wcands, weight);
                p_result.h_njets.ttwcr->Fill(ss::njets() , weight);
                p_result.h_nbtags.ttwcr->Fill(ss::nbtags() , weight);
                p_result.h_type.ttwcr->Fill(mytype , weight);
                p_result.h_charge.ttwcr->Fill(ss::lep1_id() > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.ttwcr->Fill(nleps , weight);
                p_result.h_ht.ttwcr->Fill(ss::ht() , weight);
                p_result.h_met.ttwcr->Fill(ss::met() , weight);
                p_result.h_mll.ttwcr->Fill(mll , weight);
                p_result.h_mtmin.ttwcr->Fill(mtmin , weight);
                p_result.h_l1pt.ttwcr->Fill(pto1, weight);
                p_result.h_l2pt.ttwcr->Fill(pto2, weight);
                (abs(ss::lep1_id()) == 11) ? p_result.h_el_l1pt.ttwcr->Fill(pto1, weight) : p_result.h_mu_l1pt.ttwcr->Fill(pto1, weight);
                (abs(ss::lep2_id()) == 11) ? p_result.h_el_l2pt.ttwcr->Fill(pto2, weight) : p_result.h_mu_l2pt.ttwcr->Fill(pto2, weight);
                if (nleps > 2) {
                    p_result.h_l3pt.ttwcr->Fill(pto3, weight);
                    (abs(ss::lep3_id()) == 11) ? p_result.h_el_l3pt.ttwcr->Fill(pto3, weight) : p_result.h_mu_l3pt.ttwcr->Fill(pto3, weight);
                }
            }

            if (SR > nCR) {
                p_result.h_wcands.sr->Fill(wcands, weight);
                p_result.h_njets.sr->Fill(ss::njets() , weight);
                p_result.h_nbtags.sr->Fill(ss::nbtags() , weight);
                p_result.h_type.sr->Fill(mytype , weight);
                p_result.h_charge.sr->Fill(ss::lep1_id() > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.sr->Fill(nleps , weight);
                p_result.h_ht.sr->Fill(ss::ht() , weight);
                p_result.h_met.sr->Fill(ss::met() , weight);
                p_result.h_mll.sr->Fill(mll , weight);
                p_result.h_mtmin.sr->Fill(mtmin , weight);
                p_result.h_l1pt.sr->Fill(pto1, weight);
                p_result.h_l2pt.sr->Fill(pto2, weight);
                (abs(ss::lep1_id()) == 11) ? p_result.h_el_l1pt.sr->Fill(pto1, weight) : p_result.h_mu_l1pt.sr->Fill(pto1, weight);
                (abs(ss::lep2_id()) == 11) ? p_result.h_el_l2pt.sr->Fill(pto2, weight) : p_result.h_mu_l2pt.sr->Fill(pto2, weight);
                if (nleps > 2) {
                    p_result.h_l3pt.sr->Fill(pto3, weight);
                    (abs(ss::lep3_id()) == 11) ? p_result.h_el_l3pt.sr->Fill(pto3, weight) : p_result.h_mu_l3pt.sr->Fill(pto3, weight);
                }
                //Fill SR plot
                p_result.SR.TOTAL->Fill(SR-nCR, weight); 
            }


        }//event loop
        file->Close(); 
    }//file loop

    //Return yield
    return pair<yields_t, plots_t>(y_result,p_result); 

}

void initHistError(bool usePoisson, TH1F* plot) {
    if (plot==0) return;
    if (usePoisson) plot->SetBinErrorOption(TH1::kPoisson);
    else  plot->Sumw2();
}
