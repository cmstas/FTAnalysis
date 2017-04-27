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
//
#define WRITE(var) { p_data.var->Write(); p_ttw.var->Write(); p_ttz.var->Write(); p_tth.var->Write(); p_wz.var->Write(); p_ww.var->Write(); p_xg.var->Write(); p_rares.var->Write(); p_flips.var->Write(); p_fakes.var->Write(); p_tttt.var->Write(); p_fakes_mc.var->Write(); }

float lumiAG = getLumiUnblind();
string tag = getTag().Data();
string tagData = getTagData().Data();

float bloose = 0.5426;
float bmed = 0.8484;
float btight = 0.9535;

int nsr = getNsrsTTTT();
// int nsrdisc = getNsrsTTTTBDT();
int nsrdisc = getNsrsTTTT(); // FIXME
int nCR = 2;

bool doCustomSelection = false;
float scaleLumi = 1.;

bool doTTWISR = false; // FIXME
bool doTTZISR = false; // FIXME
bool doBDT = false; // FIXME

bool makeRootFiles = true;

TString dir = "v0.02_Apr27_test"; // tttt regions

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
    SR_t SRDISC;
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
    triple_t h_mu_l1pt;
    triple_t h_el_l1pt;
    triple_t h_mu_l2pt;
    triple_t h_mu_l3pt;
    triple_t h_el_l2pt;
    triple_t h_el_l3pt;
    triple_t h_mbb;
    triple_t h_disc;
    triple_t h_mid1;
    triple_t h_mid2;
    triple_t h_mid3;
};


//Total
yields_t total;

//function declaration
pair<yields_t, plots_t> run(TChain *chain, bool isData = 0, bool doFlips = 0, int doFakes = 0, int exclude = 0, bool isSignal = 0, bool isGamma = 0);
void avoidNegativeYields(TH1F* plot);
void avoidZeroIntegrals(TH1F* central,TH1F* up,TH1F* down);
void fillDownMirrorUp(TH1F* central,TH1F* up,TH1F* down);
void writeStatUpDown(TH1F* central,string name,bool down);
void writeStat(TH1F* central,string name);
void writeHTHltSyst(TH1F* central,string name,TString kine);
void initHistError(bool usePoisson, TH1F* plot);

void getyields(){

    gSystem->Exec(Form("mkdir -p ../limits/%s/", dir.Data()));

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

    // TString pfx  = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.04/output/";
    TString pfx  = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.02/output/";
    TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.06/output/";

    //Fill chains
    tttt_chain   ->Add(Form("%s/TTTT.root"           , pfx.Data()));

    ttbar_chain  ->Add(Form("%s/TTBAR_PH*.root"       , pfx.Data()));
    wjets_chain  ->Add(Form("%s/WJets.root"       , pfx.Data()));
    dy_chain     ->Add(Form("%s/DY_high*.root"        , pfx.Data()));
    dy_chain     ->Add(Form("%s/DY_low*.root"         , pfx.Data()));
    ttw_chain    ->Add(Form("%s/TTWnlo.root"            , pfx.Data()));
    ttz_chain   ->Add(Form("%s/TTZnlo.root"           , pfx.Data()));
    ttz_chain   ->Add(Form("%s/TTZLOW.root"         , pfx.Data()));
    tth_chain   ->Add(Form("%s/TTHtoNonBB.root"     , pfx.Data()));
    wz_chain     ->Add(Form("%s/WZ.root"             , pfx.Data()));
    ww_chain     ->Add(Form("%s/QQWW.root"           , pfx.Data()));
    qqww_chain     ->Add(Form("%s/QQWW.root"           , pfx.Data()));
    // xg_chain     ->Add(Form("%s/TG.root"             , pfx.Data()));
    // xg_chain     ->Add(Form("%s/WGToLNuG.root"           , pfx.Data()));
    xg_chain     ->Add(Form("%s/TGext.root"             , pfx.Data()));
    xg_chain     ->Add(Form("%s/WGToLNuGext.root"           , pfx.Data()));
    xg_chain     ->Add(Form("%s/TTG.root"            , pfx.Data()));
    xg_chain     ->Add(Form("%s/ZG.root"             , pfx.Data()));
    rares_chain  ->Add(Form("%s/ZZ.root"             , pfx.Data()));
    rares_chain  ->Add(Form("%s/GGHtoZZto4L.root"    , pfx.Data()));
    rares_chain  ->Add(Form("%s/WWZ.root"            , pfx.Data()));
    rares_chain  ->Add(Form("%s/WZZ.root"            , pfx.Data()));
    rares_chain  ->Add(Form("%s/WWW.root"            , pfx.Data()));
    rares_chain  ->Add(Form("%s/WWG.root"            , pfx.Data()));
    rares_chain  ->Add(Form("%s/WZG.root"            , pfx.Data()));
    rares_chain  ->Add(Form("%s/VHtoNonBB.root"      , pfx.Data()));
    rares_chain  ->Add(Form("%s/TZQ.root"            , pfx.Data()));
    rares_chain  ->Add(Form("%s/TWZ.root"            , pfx.Data()));
    rares_chain  ->Add(Form("%s/WWDPS.root"          , pfx.Data()));
    //data
    data_chain   ->Add(Form("%s/DataDoubleMuon*.root"    , pfxData.Data()));
    data_chain   ->Add(Form("%s/DataDoubleEG*.root"  , pfxData.Data()));
    data_chain   ->Add(Form("%s/DataMuonEG*.root"      , pfxData.Data()));
    data_chain   ->Add(Form("%s/JetHT*.root"      , pfxData.Data()));
    //flips
    flips_chain  ->Add(Form("%s/DataMuonEG*.root"     , pfxData.Data()));
    flips_chain  ->Add(Form("%s/DataDoubleEG*.root"      , pfxData.Data()));
    //fakes
    fakes_chain  ->Add(Form("%s/DataDoubleMuon*.root"    , pfxData.Data()));
    fakes_chain  ->Add(Form("%s/DataDoubleEG*.root"  , pfxData.Data()));
    fakes_chain  ->Add(Form("%s/DataMuonEG*.root"      , pfxData.Data()));
    fakes_chain  ->Add(Form("%s/JetHT*.root"      , pfxData.Data()));
    fakes_chain  ->Add(Form("%s/TTWnlo.root"                   , pfx.Data()));
    fakes_chain  ->Add(Form("%s/TTZnlo.root"                  , pfx.Data()));
    fakes_chain  ->Add(Form("%s/WZ.root"                    , pfx.Data()));
    fakes_chain  ->Add(Form("%s/TTHtoNonBB.root"            , pfx.Data()));
    fakes_chain  ->Add(Form("%s/QQWW.root"                  , pfx.Data()));

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
    ttbar_chain->SetTitle("fakes_mc");
    pair<yields_t, plots_t> results_fakes_mc = run(ttbar_chain, 0, 0, 1);
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
    plots_t& p_fakes_mc = results_fakes_mc.second;

    TFile *fout = new TFile("histos.root", "RECREATE");

    WRITE(h_ht.sr);          WRITE(h_ht.ttzcr);           WRITE(h_ht.ttwcr);        WRITE(h_ht.br);
    WRITE(h_met.sr);         WRITE(h_met.ttzcr);          WRITE(h_met.ttwcr);       WRITE(h_met.br);
    WRITE(h_mll.sr);         WRITE(h_mll.ttzcr);          WRITE(h_mll.ttwcr);       WRITE(h_mll.br);
    WRITE(h_mtmin.sr);       WRITE(h_mtmin.ttzcr);        WRITE(h_mtmin.ttwcr);     WRITE(h_mtmin.br);
    WRITE(h_njets.sr);       WRITE(h_njets.ttzcr);        WRITE(h_njets.ttwcr);     WRITE(h_njets.br);
    WRITE(h_nleps.sr);       WRITE(h_nleps.ttzcr);        WRITE(h_nleps.ttwcr);     WRITE(h_nleps.br);
    WRITE(h_nbtags.sr);      WRITE(h_nbtags.ttzcr);       WRITE(h_nbtags.ttwcr);    WRITE(h_nbtags.br);
    WRITE(h_type.sr);        WRITE(h_type.ttzcr);         WRITE(h_type.ttwcr);      WRITE(h_type.br);
    WRITE(h_charge.sr);      WRITE(h_charge.ttzcr);       WRITE(h_charge.ttwcr);    WRITE(h_charge.br);
    WRITE(h_l1pt.sr);        WRITE(h_l1pt.ttzcr);         WRITE(h_l1pt.ttwcr);      WRITE(h_l1pt.br);
    WRITE(h_l2pt.sr);        WRITE(h_l2pt.ttzcr);         WRITE(h_l2pt.ttwcr);      WRITE(h_l2pt.br);
    WRITE(h_l3pt.sr);        WRITE(h_l3pt.ttzcr);         WRITE(h_l3pt.ttwcr);      WRITE(h_l3pt.br);
    WRITE(h_el_l1pt.sr);     WRITE(h_el_l1pt.ttzcr);      WRITE(h_el_l1pt.ttwcr);   WRITE(h_el_l1pt.br);
    WRITE(h_el_l2pt.sr);     WRITE(h_el_l2pt.ttzcr);      WRITE(h_el_l2pt.ttwcr);   WRITE(h_el_l2pt.br);
    WRITE(h_el_l3pt.sr);     WRITE(h_el_l3pt.ttzcr);      WRITE(h_el_l3pt.ttwcr);   WRITE(h_el_l3pt.br);
    WRITE(h_mu_l1pt.sr);     WRITE(h_mu_l1pt.ttzcr);      WRITE(h_mu_l1pt.ttwcr);   WRITE(h_mu_l1pt.br);
    WRITE(h_mu_l2pt.sr);     WRITE(h_mu_l2pt.ttzcr);      WRITE(h_mu_l2pt.ttwcr);   WRITE(h_mu_l2pt.br);
    WRITE(h_mu_l3pt.sr);     WRITE(h_mu_l3pt.ttzcr);      WRITE(h_mu_l3pt.ttwcr);   WRITE(h_mu_l3pt.br);

    WRITE(h_mid1.br);
    WRITE(h_mid2.br);
    WRITE(h_mid3.br);

    WRITE(h_disc.br);
    WRITE(SRCR.TOTAL);
    WRITE(SRDISC.TOTAL);
    WRITE(SR.TOTAL);

    TNamed metadata(TString("metadata"),dir);
    metadata.Write();

    // fout->Write();
    fout->Close();

    // // Write output tree
    // out_file->cd();
    // out_tree->Write();
    // out_file->Close();

    // copy the root file with kinematic plots to live with the cards
    gSystem->Exec(Form("cp histos.root ../limits/%s/", dir.Data()));

}

//doFakes = 1 for QCD, 2 for inSitu
pair<yields_t, plots_t> run(TChain *chain, bool isData, bool doFlips, int doFakes, int exclude, bool isSignal, bool isGamma){

    // out_file->cd();


    TString chainTitle = chain->GetTitle();
    const char* chainTitleCh = chainTitle.Data();
    std::cout << "Working on " << chainTitle << std::endl;

    bool isWZ = (chainTitle=="wz");
    bool isttZ = (chainTitle=="ttz");
    bool isttW = (chainTitle=="ttw");

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

    p_result.h_mid1.br     = new TH1F(Form("br_mid1_%s"       , chainTitleCh) , Form("mid1_%s"       , chainTitleCh) , 5       , -2    , 3);
    p_result.h_mid2.br     = new TH1F(Form("br_mid2_%s"       , chainTitleCh) , Form("mid2_%s"       , chainTitleCh) , 5       , -2    , 3);
    p_result.h_mid3.br     = new TH1F(Form("br_mid3_%s"       , chainTitleCh) , Form("mid3_%s"       , chainTitleCh) , 5       , -2    , 3);

    p_result.h_disc.br     = new TH1F(Form("br_disc_%s"       , chainTitleCh) , Form("disc_%s"       , chainTitleCh) , 8       , 0    , 1.0);
    p_result.SRCR.TOTAL = new TH1F(Form("SRCR_TOTAL_%s" , chainTitleCh) , Form("SRCR_TOTAL_%s" , chainTitleCh) , nsr     , 0.5  , nsr+0.5);
    p_result.SRDISC.TOTAL = new TH1F(Form("SRDISC_TOTAL_%s" , chainTitleCh) , Form("SRDISC_TOTAL_%s" , chainTitleCh) , nsrdisc     , 0.5  , nsrdisc+0.5);
    p_result.SR.TOTAL   = new TH1F(Form("SR_TOTAL_%s"   , chainTitleCh) , Form("SR_TOTAL_%s"   , chainTitleCh) , nsr-nCR , 0.5  , nsr-nCR+0.5);



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

    bool doPoisson = isData && !doFlips && !doFakes;
    initHistError(doPoisson, p_result.h_disc.br);
    initHistError(doPoisson, p_result.SRCR.TOTAL   );
    initHistError(doPoisson, p_result.SRDISC.TOTAL   );
    initHistError(doPoisson, p_result.SR.TOTAL   );
    initHistError(doPoisson, p_jes_alt_up.SRCR.TOTAL   );
    initHistError(doPoisson, p_jes_alt_up.SRDISC.TOTAL   );
    initHistError(doPoisson, p_jes_alt_dn.SRCR.TOTAL   );
    initHistError(doPoisson, p_jes_alt_dn.SRDISC.TOTAL   );
    initHistError(doPoisson, p_btagSF_alt_up.SRCR.TOTAL);
    initHistError(doPoisson, p_btagSF_alt_up.SRDISC.TOTAL);
    initHistError(doPoisson, p_btagSF_alt_dn.SRCR.TOTAL);
    initHistError(doPoisson, p_btagSF_alt_dn.SRDISC.TOTAL);
    initHistError(doPoisson, p_pu_alt_up.SRCR.TOTAL    );
    initHistError(doPoisson, p_pu_alt_up.SRDISC.TOTAL    );
    initHistError(doPoisson, p_pu_alt_dn.SRCR.TOTAL    );
    initHistError(doPoisson, p_pu_alt_dn.SRDISC.TOTAL    );
    initHistError(doPoisson, p_fake_alt_up.SRCR.TOTAL    );
    initHistError(doPoisson, p_fake_alt_up.SRDISC.TOTAL    );


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

            if (!ss::is_real_data()) {
                weight *= getTruePUw_Moriond(ss::trueNumInt()[0]);
            }
            weight*=ss::weight_btagsf();
            // if (isWZ) weight*=getWZSF();
            // if (isttZ && applyttZSF) weight*=getttZSF();
            //apply lepton scale factors
            // if (ss::is_real_data()==0 && (!isWZ) && (!isttZ || !applyttZSF)) {
              if (ss::is_real_data()==0 && (!isttW) && (!isttZ)) {
                weight*=eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
            }

              float weight_alt_FR = weight;
            float weight_btag_up_alt = weight;
            float weight_btag_dn_alt = weight;
            float weight_pu_up_alt = weight;
            float weight_pu_dn_alt = weight;
            if (ss::is_real_data()==0) {
                weight_btag_up_alt = ss::weight_btagsf()>0 ? weight*ss::weight_btagsf_UP()/ss::weight_btagsf() : weight;
                weight_btag_dn_alt = ss::weight_btagsf()>0 ? weight*ss::weight_btagsf_DN()/ss::weight_btagsf() : weight;
                weight_pu_up_alt = getTruePUw_Moriond(ss::trueNumInt()[0])>0 ? weight*getTruePUwUp(ss::trueNumInt()[0])/getTruePUw_Moriond(ss::trueNumInt()[0]) : weight;
                weight_pu_dn_alt = getTruePUw_Moriond(ss::trueNumInt()[0])>0 ? weight*getTruePUwDn(ss::trueNumInt()[0])/getTruePUw_Moriond(ss::trueNumInt()[0]) : weight;
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
                        float fra = alternativeFakeRate(ss::lep1_id(),ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), ss::ht());
                        weight *= fr/(1.-fr);
                        weight_alt_FR *= fra/(1.-fra);
                    }
                    if (ss::lep2_passes_id()==0) {
                        float fr = fakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), ss::ht());
                        float fra = alternativeFakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), ss::ht());
                        weight *= fr/(1.-fr);
                        weight_alt_FR *= fra/(1.-fra);
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
                        weight *= fr / (1-fr);
                        weight_alt_FR *= fra/(1.-fra);
                    }
                    if(ss::is_real_data() && (ss::lep2_fo() && !ss::lep2_tight()) && ss::lep1_passes_id() && ss::lep3_passes_id()) {  // lep2 fake
                        float fr = fakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(),ss::lep2_p4().eta(),ss::ht());
                        float fra = alternativeFakeRate(ss::lep2_id(),ss::lep2_coneCorrPt(), ss::lep2_p4().eta(), ss::ht());
                        isClass6Fake = true;
                        weight *= fr / (1-fr);
                        weight_alt_FR *= fra/(1.-fra);
                    }
                    if(ss::is_real_data() && (ss::lep1_fo() && !ss::lep1_tight()) && ss::lep2_passes_id() && ss::lep3_passes_id()) {  // lep1 fake
                        float fr = fakeRate(ss::lep1_id(),ss::lep1_coneCorrPt(),ss::lep1_p4().eta(),ss::ht());
                        float fra = alternativeFakeRate(ss::lep1_id(),ss::lep1_coneCorrPt(), ss::lep1_p4().eta(), ss::ht());
                        isClass6Fake = true;
                        weight *= fr / (1-fr);
                        weight_alt_FR *= fra/(1.-fra);
                    }
                }
            }

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

            // if (!doFlips && !doFakes && exclude == 0) {
            //     if (nleps > 2) {
            //     if (!isData && !isGamma && ss::lep3_motherID()==2) continue;
            //     }
            // }

            //Reject duplicates (after selection otherwise flips are ignored...)
            if (isData && ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)){ continue; }
            }


            if (doBDT) {
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
              // // fill discriminant before requiring SR
              // float pred = get_prediction((float)tree_njets,(float)tree_nbtags,(float)tree_mt1,(float)tree_mt2,(float)tree_met,(float)tree_ml1l2,(float)tree_htb,(float)tree_nleps,(float)tree_ht,(float)tree_mj1j2,(float)tree_dphij1j2,(float)tree_ptj1,(float)tree_ptj2,(float)tree_ml1j2,(float)tree_ml1j1,(float)tree_wcands,(float)tree_dphil1j1,(float)tree_detal1l2,(float)tree_nlb40,(float)tree_nmb40,(float)tree_ntb40,(float)tree_q1,(float)tree_q2,(float)tree_ht4ratio);
              // p_result.h_disc.br->Fill(pred,weight);
            }

            if (!isData) {
              p_result.h_mid1.br->Fill(ss::lep1_motherID(), weight);
              p_result.h_mid2.br->Fill(ss::lep2_motherID(), weight);
              if (ss::lep3_passes_id()) p_result.h_mid3.br->Fill(ss::lep3_motherID(), weight);
            }


            float pt1 = lep1_pt;
            float pt2 = lep2_pt;
            float pt3 = ss::lep3_p4().pt();
            float pto1 = max(max(pt1,pt2),pt3);
            float pto3 = min(min(pt1,pt2),pt3);
            float pto2 = pt1+pt2+pt3-pto1-pto3;
            int mytype = ss::hyp_type();
            if (mytype==2 && abs(ss::lep1_id())==13) mytype = 1;
            float mll = (ss::lep1_p4()*lep1_pt/ss::lep1_p4().pt()+ss::lep2_p4()*lep2_pt/ss::lep2_p4().pt()).M();


            float mtl1_unc_up = MT(lep1_pt, ss::lep1_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
            float mtl2_unc_up = MT(lep2_pt, ss::lep2_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
            float mtmin_unc_up = mtl1_unc_up > mtl2_unc_up ? mtl2_unc_up : mtl1_unc_up;
            float mtl1_unc_dn = MT(lep1_pt, ss::lep1_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
            float mtl2_unc_dn = MT(lep2_pt, ss::lep2_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
            float mtmin_unc_dn = mtl1_unc_dn > mtl2_unc_dn ? mtl2_unc_dn : mtl1_unc_dn;
            int SR = signalRegionTest(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, nleps, isClass6);
            int SR_unc_up = signalRegionTest(ss::njets_unc_up(), ss::nbtags_unc_up(), ss::met_unc_up(), ss::ht_unc_up(), mtmin_unc_up, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, nleps, isClass6);
            int SR_unc_dn = signalRegionTest(ss::njets_unc_dn(), ss::nbtags_unc_dn(), ss::met_unc_dn(), ss::ht_unc_dn(), mtmin_unc_dn, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, nleps, isClass6);

            // FIXME
            int SRdisc = SR;
            int SRdisc_unc_up = SR_unc_up;
            int SRdisc_unc_dn = SR_unc_dn;

            if (isData  == 0 && SR_unc_up > 0)            p_jes_alt_up.SRCR.TOTAL->Fill(SR_unc_up, weight);
            if (isData  == 0 && SR_unc_dn > 0)            p_jes_alt_dn.SRCR.TOTAL->Fill(SR_unc_dn, weight);

            if (isData  == 0 && SR_unc_up > 0)            p_jes_alt_up.SRDISC.TOTAL->Fill(SRdisc_unc_up, weight);
            if (isData  == 0 && SR_unc_dn > 0)            p_jes_alt_dn.SRDISC.TOTAL->Fill(SRdisc_unc_dn, weight);

            // require SRs+CR (= ttZ CR + ttW CR + SRs)
            if (SR < 0) continue;

            p_result.SRCR.TOTAL->Fill(SR, weight);

            p_result.SRDISC.TOTAL->Fill(SR, weight);

            // fill systematics hists
            if (doFakes == 1 )            p_fake_alt_up.SRCR.TOTAL->Fill(SR, weight_alt_FR);
            if (isData  == 0 )            p_btagSF_alt_up.SRCR.TOTAL->Fill(SR, weight_btag_up_alt);
            if (isData  == 0 )            p_btagSF_alt_dn.SRCR.TOTAL->Fill(SR, weight_btag_dn_alt);
            if (isData  == 0 )            p_pu_alt_up.SRCR.TOTAL->Fill(SR, weight_pu_up_alt);
            if (isData  == 0 )            p_pu_alt_dn.SRCR.TOTAL->Fill(SR, weight_pu_dn_alt);

            if (doFakes == 1 )            p_fake_alt_up.SRDISC.TOTAL->Fill(SRdisc, weight_alt_FR);
            if (isData  == 0 )            p_btagSF_alt_up.SRDISC.TOTAL->Fill(SRdisc, weight_btag_up_alt);
            if (isData  == 0 )            p_btagSF_alt_dn.SRDISC.TOTAL->Fill(SRdisc, weight_btag_dn_alt);
            if (isData  == 0 )            p_pu_alt_up.SRDISC.TOTAL->Fill(SRdisc, weight_pu_up_alt);
            if (isData  == 0 )            p_pu_alt_dn.SRDISC.TOTAL->Fill(SRdisc, weight_pu_dn_alt);

            if (SR > 1) { // non ttZ CR
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
