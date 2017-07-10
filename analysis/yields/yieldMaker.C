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
#define WRITE(var) { p_data.var->Write(); p_ttw.var->Write(); p_ttz.var->Write(); p_tth.var->Write(); p_xg.var->Write(); p_rares.var->Write(); p_flips.var->Write(); p_fakes.var->Write(); p_tttt.var->Write(); p_ttvv.var->Write(); p_fakes_mc.var->Write(); }

float lumiAG = getLumiUnblind();

float bloose = 	0.2219;
float bmed = 0.6324;
float btight = 0.8958;

int nsr = getNsrsTTTT();
int nsrdisc = 6; //getNsrsTTTT();
int nCR = 2;

bool doCustomSelection = false;
float scaleLumi = 1.;

bool doTTWISR = true;
bool doTTZISR = true;
bool doBDT = false; // FIXME
bool outputTrainingBDT = false; // FIXME
bool doISRFSRsyst = true;
bool doTTBB = true;
bool doJER = true;

bool makeRootFiles = true;
bool makeGenVariationsMC = true;

// TString dir = "v0.04_May10_test";
// TString tag = "v0.04";

// TString dir = "v0.07_May28";
// TString tag = "v0.07";

// TString dir = "v0.08_May28";
// TString tag = "v0.08";

// TString dir = "v0.09_May30";
// TString tag = "v0.09";

// // 40 gev jets
// bool doJER = false;
// TString dir = "v0.07_May30";
// TString tag = "v0.07"; // data is in v0.07
// TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.07/output/skim/";

// // 40 gev jets
// bool doJER = true;
// TString dir = "v0.10_Jun5";
// TString tag = "v0.10_fix"; // data is in v0.07 still
// TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.07/output/skim/";

// TString dir = "v0.10_Jun13";
// TString tag = "v0.10_fix"; // data is in v0.07 still
// TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.07/output/skim/";

// // preapproval
// TString dir = "v0.10_Jun19";
// TString tag = "v0.10_fix";
// TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.07/output/skim/";

// TString dir = "v0.10_Jun21_sync";
// TString tag = "v0.10_fix";
// TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_data/output/skim/";


// latest
// TString dir = "v0.10_Jun22_crfit";
// TString dir = "v0.10_Jun22_mcfakecent";
// TString dir = "v0.10_Jun22_sens18bins";
// TString dir = "v0.10_Jun22_sens8bins";
// TString dir = "v0.10_Jun22_unblind";
// TString dir = "v0.10_Jul6_test";
TString dir = "v0.10_Jul6_WSF";
TString tag = "v0.10_WSFv2"; // data is in v0.07 still
// TString tag = "v0.10_fix"; // data is in v0.07 still
TString pfxData = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_data/output/skim/";


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
float tree_ptj1 = -1;
float tree_ptj2 = -1;
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
    triple_t h_charge3;
    triple_t h_nleps;
    triple_t h_mu_l1pt;
    triple_t h_el_l1pt;
    triple_t h_mu_l2pt;
    triple_t h_mu_l3pt;
    triple_t h_el_l2pt;
    triple_t h_el_l3pt;
    triple_t h_disc;
    triple_t h_disc2;
    triple_t h_mid1;
    triple_t h_mid2;
    triple_t h_mid3;
    triple_t h_mvis;
    triple_t h_mtvis;
    triple_t h_ntops;
    triple_t h_ntopness;
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
std::pair<int,float> getNtops();

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
        out_tree->Branch("ptj1", &tree_ptj1 );
        out_tree->Branch("ptj2", &tree_ptj2 );
        out_tree->Branch("nlb40", &tree_nlb40 );
        out_tree->Branch("nmb40", &tree_nmb40 );
        out_tree->Branch("ntb40", &tree_ntb40 );
        out_tree->Branch("q1", &tree_q1 );
        out_tree->Branch("q2", &tree_q2 );
        out_tree->Branch("q3", &tree_q3 );
        out_tree->Branch("ht4ratio", &tree_ht4ratio );
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
    //signals full sim
    
  #include "higgs_scan_v2.h"
  #include "higgs_scan_ps_v2.h"

    TString pfx  = Form("/nfs-7/userdata/namin/tupler_babies/merged/FT/%s//output/skim/",tag.Data());

    //Fill chains
    tttt_chain   ->Add(Form("%s/TTTTnew.root"           , pfx.Data()));

    ttttisrup_chain   ->Add(Form("%s/TTTTisrup.root"           , pfx.Data()));
    ttttfsrup_chain   ->Add(Form("%s/TTTTfsrup.root"           , pfx.Data()));
    ttttisrdn_chain   ->Add(Form("%s/TTTTisrdown.root"           , pfx.Data()));
    ttttfsrdn_chain   ->Add(Form("%s/TTTTfsrdown.root"           , pfx.Data()));
    ttvv_chain->Add(Form("%s/TTHH.root",pfx.Data()));
    ttvv_chain->Add(Form("%s/TTWH.root",pfx.Data()));
    ttvv_chain->Add(Form("%s/TTWW.root",pfx.Data()));
    ttvv_chain->Add(Form("%s/TTWZ.root",pfx.Data()));
    ttvv_chain->Add(Form("%s/TTZH.root",pfx.Data()));
    ttvv_chain->Add(Form("%s/TTZZ.root",pfx.Data()));
    if (doSync) {
        ttw_chain    ->Add("/nfs-7/userdata/namin/tupler_babies/merged/test/FT/test_synch_btagcsv_v1/output/TTWnlo.root");
    } else {
        ttw_chain    ->Add(Form("%s/TTWnlo.root"            , pfx.Data()));
    }
    ttz_chain   ->Add(Form("%s/TTZnlo.root"           , pfx.Data()));
    ttz_chain   ->Add(Form("%s/TTZLOW.root"         , pfx.Data()));
    tth_chain   ->Add(Form("%s/TTHtoNonBB.root"     , pfx.Data()));
    ttbar_chain  ->Add(Form("%s/TTBAR_PH*.root"       , pfx.Data()));
    // wjets_chain  ->Add(Form("%s/WJets.root"       , pfx.Data()));
    // dy_chain     ->Add(Form("%s/DY_high*.root"        , pfx.Data()));
    // dy_chain     ->Add(Form("%s/DY_low*.root"         , pfx.Data()));
    // qqww_chain     ->Add(Form("%s/QQWW.root"           , pfx.Data()));
    // xg_chain     ->Add(Form("%s/TG.root"             , pfx.Data()));
    // xg_chain     ->Add(Form("%s/WGToLNuG.root"           , pfx.Data()));
    xg_chain     ->Add(Form("%s/TGext.root"             , pfx.Data()));
    xg_chain     ->Add(Form("%s/WGToLNuGext.root"           , pfx.Data()));
    xg_chain     ->Add(Form("%s/TTGdilep.root"             , pfx.Data()));
    xg_chain     ->Add(Form("%s/TTGsinglelep.root"             , pfx.Data()));
    xg_chain     ->Add(Form("%s/TTGsinglelepbar.root"             , pfx.Data()));
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
    rares_chain     ->Add(Form("%s/WZ.root"             , pfx.Data()));
    rares_chain     ->Add(Form("%s/QQWW.root"           , pfx.Data()));
    rares_chain->Add(Form("%s/TTTJ.root",pfx.Data()));
    rares_chain->Add(Form("%s/TTTW.root",pfx.Data()));
    //data
    if (doSync) {
        data_chain   ->Add("/nfs-7/userdata/namin/tupler_babies/merged/test/FT/test_synch_btagcsv_v1/output/DataMuonEG.root");
    } else {
        data_chain   ->Add(Form("%s/DataDoubleMuon*.root"    , pfxData.Data()));
        data_chain   ->Add(Form("%s/DataDoubleEG*.root"  , pfxData.Data()));
        data_chain   ->Add(Form("%s/DataMuonEG*.root"      , pfxData.Data()));
        data_chain   ->Add(Form("%s/JetHT*.root"      , pfxData.Data()));
    }
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
    WRITE(h_ntopness.sr)  ; 

    WRITE(h_mid1.br);
    WRITE(h_mid2.br);
    WRITE(h_mid3.br);

    WRITE(h_charge3.sr);

    WRITE(h_disc.br);
    WRITE(h_disc2.br);
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
pair<yields_t, plots_t> run(TChain *chain, bool isData, bool doFlips, int doFakes, int exclude, bool isSignal, bool isGamma){


    TMVA::Reader* reader = new TMVA::Reader("Silent");
    reader->AddVariable("njets",&tree_f_njets);
    reader->AddVariable("nbtags",&tree_f_nbtags);
    reader->AddVariable("mt1",&tree_mt1);
    reader->AddVariable("mt2",&tree_mt2);
    reader->AddVariable("met",&tree_met);
    reader->AddVariable("ml1l2",&tree_ml1l2);
    reader->AddVariable("htb",&tree_htb);
    reader->AddVariable("nleps",&tree_f_nleps);
    reader->AddVariable("ht",&tree_ht);
    reader->AddVariable("mj1j2",&tree_mj1j2);
    reader->AddVariable("dphij1j2",&tree_dphij1j2);
    reader->AddVariable("ptj1",&tree_ptj1);
    reader->AddVariable("ptj2",&tree_ptj2);
    reader->AddVariable("ml1j2",&tree_ml1j2);
    reader->AddVariable("ml1j1",&tree_ml1j1);
    reader->AddVariable("wcands",&tree_f_wcands);
    reader->AddVariable("dphil1j1",&tree_dphil1j1);
    reader->AddVariable("detal1l2",&tree_detal1l2);
    reader->AddVariable("nlb40",&tree_f_nlb40);
    reader->AddVariable("nmb40",&tree_f_nmb40);
    reader->AddVariable("ntb40",&tree_f_ntb40);
    reader->AddVariable("q1",&tree_f_q1);
    // reader->AddVariable("q2",&tree_f_q2);
    reader->AddVariable("ht4ratio",&tree_ht4ratio);
    reader->AddSpectator("weight",&tree_weight);
    reader->AddSpectator("ptl1",&tree_ptl1);
    reader->AddSpectator("ptl2",&tree_ptl2);
    reader->AddSpectator("SR",&tree_SR);

    // reader->BookMVA("BDT","TMVAClassification_BDT.weights.xml");
    reader->BookMVA("BDT","TMVAClassification_BDT_500trees.weights.xml");
    // reader->BookMVA("BDT","TMVAClassification_BDT_bgttw.weights.xml");

    // out_file->cd();


    TString chainTitle = chain->GetTitle();
    const char* chainTitleCh = chainTitle.Data();
    std::cout << "Working on " << chainTitle << std::endl;

    // bool isWZ = (chainTitle=="wz");
    bool istttt = (chainTitle=="tttt");
    bool isttZ = (chainTitle=="ttz");
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
    p_result.h_ntopness.sr      = new TH1F(Form("sr_ntopness_%s"        , chainTitleCh) , Form("ntopness_%s"        , chainTitleCh) , 15      , 0., 0.75);

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

    p_result.h_mid1.br     = new TH1F(Form("br_mid1_%s"       , chainTitleCh) , Form("mid1_%s"       , chainTitleCh) , 5       , -2    , 3);
    p_result.h_mid2.br     = new TH1F(Form("br_mid2_%s"       , chainTitleCh) , Form("mid2_%s"       , chainTitleCh) , 5       , -2    , 3);
    p_result.h_mid3.br     = new TH1F(Form("br_mid3_%s"       , chainTitleCh) , Form("mid3_%s"       , chainTitleCh) , 5       , -2    , 3);
    p_result.h_charge3.sr   = new TH1F(Form("sr_charge3_%s"     , chainTitleCh) , Form("charge3_%s"     , chainTitleCh) , 3       , 0   , 3);

    // p_result.h_disc.br     = new TH1F(Form("br_disc_%s"       , chainTitleCh) , Form("disc_%s"       , chainTitleCh) , 16       , -1.0    , 1.0);
    p_result.h_disc.br     = new TH1F(Form("br_disc_%s"       , chainTitleCh) , Form("disc_%s"       , chainTitleCh) , 20       , 0.    , 1.0);
    // p_result.h_disc2.br     = new TH1F(Form("br_disc2_%s"       , chainTitleCh) , Form("disc2_%s"       , chainTitleCh) , 16       ,0., 1.0);
    p_result.h_disc2.br     = new TH1F(Form("br_disc2_%s"       , chainTitleCh) , Form("disc2_%s"       , chainTitleCh) , 4       ,0., 0.8);
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


        if ((istttt||isHiggsScan) && makeGenVariationsMC) {
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
            if (!ss::fired_trigger()) continue;
            if (!ss::passes_met_filters()) continue;

            if (doCustomSelection) {

            }

            if (isHiggsScan) {
                // make sure the higgs mass point we are considering is the same as chain title
                if (fabs(mysparms[0]-ss::higgs_mass()) > 0.1) continue;
            }

            float weight =  ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG;
            weight*=scaleLumi;

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

            if (!ss::is_real_data()) weight *= ss::decayWSF();

            if (!ss::is_real_data()) {
                weight *= getTruePUw_Moriond(ss::trueNumInt()[0]);
            }
            weight*=ss::weight_btagsf();
            // if (isWZ) weight*=getWZSF();
            // if (isttZ && applyttZSF) weight*=getttZSF();
            //apply lepton scale factors
              if (ss::is_real_data()==0 && (!isttW) && (!isttZ)) {
                weight*=eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
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
                    float scaleamt = 1.7;
                    weight_bb_up_alt = weight*scaleamt*scaleamt;
                    weight *= scaleamt;
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
                weight_pu_up_alt = getTruePUw_Moriond(ss::trueNumInt()[0])>0 ? weight*getTruePUwUp(ss::trueNumInt()[0])/getTruePUw_Moriond(ss::trueNumInt()[0]) : weight;
                weight_pu_dn_alt = getTruePUw_Moriond(ss::trueNumInt()[0])>0 ? weight*getTruePUwDn(ss::trueNumInt()[0])/getTruePUw_Moriond(ss::trueNumInt()[0]) : weight;
                weight_lep_up_alt = weight*(1.0+leptonScaleFactor_err(ss::lep1_id(),  ss::lep1_p4().pt(),  ss::lep1_p4().eta(),  ss::ht())+leptonScaleFactor_err(ss::lep2_id(),  ss::lep2_p4().pt(),  ss::lep2_p4().eta(),  ss::ht()));
            }


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

            // // Before any real continue statements
            //     if ( ss::event()==1005336 || ss::event()==1006314 || ss::event()==1008479 || ss::event()==1009254 || ss::event()==1010876 || ss::event()==101671 || ss::event()==1016768 || ss::event()==1019408 || ss::event()==1022705 || ss::event()==1028170 || ss::event()==1030143 || ss::event()==1032198 || ss::event()==1032389 || ss::event()==1032455 || ss::event()==1036135 || ss::event()==1037830 || ss::event()==1037956 || ss::event()==1039352 || ss::event()==1042593 || ss::event()==1042741 || ss::event()==1043427 || ss::event()==104549 || ss::event()==104553 || ss::event()==1047421 || ss::event()==1054276 || ss::event()==1063334 || ss::event()==1063763 || ss::event()==1068999 || ss::event()==1070192 || ss::event()==1074142 || ss::event()==1075459 || ss::event()==1075840 || ss::event()==1076337 || ss::event()==1076462 || ss::event()==1078925 || ss::event()==1079158 || ss::event()==1079244 || ss::event()==1079634 || ss::event()==1081267 || ss::event()==1081557 || ss::event()==1082285 || ss::event()==1083335 || ss::event()==1083649 || ss::event()==1084089 || ss::event()==1091511 || ss::event()==1092166 || ss::event()==110387 || ss::event()==1109435 || ss::event()==110957 || ss::event()==1110582 || ss::event()==1111580 || ss::event()==1111817 || ss::event()==1112757 || ss::event()==1112980 || ss::event()==1114212 || ss::event()==1117607 || ss::event()==1120281 || ss::event()==112149 || ss::event()==1122094 || ss::event()==1126564 || ss::event()==1126637 || ss::event()==1130057 || ss::event()==1131439 || ss::event()==1131694 || ss::event()==1132669 || ss::event()==1134589 || ss::event()==1134967 || ss::event()==1135665 || ss::event()==1135941 || ss::event()==1136036 || ss::event()==1136143 || ss::event()==1137489 || ss::event()==1143724 || ss::event()==1145006 || ss::event()==1146851 || ss::event()==1147852 || ss::event()==1155272 || ss::event()==1162988 || ss::event()==1164374 || ss::event()==1166037 || ss::event()==1171445 || ss::event()==1178948 || ss::event()==1179241 || ss::event()==1180272 || ss::event()==1180966 || ss::event()==1184423 || ss::event()==1184813 || ss::event()==1185696 || ss::event()==1186504 || ss::event()==1189719 || ss::event()==1193795 || ss::event()==1194809 || ss::event()==1194993 || ss::event()==1199012 || ss::event()==1200084 || ss::event()==1202161 || ss::event()==1203320 || ss::event()==1203665 || ss::event()==120677 || ss::event()==1208453 || ss::event()==1212152 
            //     || ss::event()==1213341 || ss::event()==1219043 || ss::event()==121951 || ss::event()==1220544 || ss::event()==1222605 || ss::event()==1225234 || ss::event()==1226430 || ss::event()==1228769 || ss::event()==122976 || ss::event()==1231038 || ss::event()==1232346 || ss::event()==1235637 || ss::event()==1240160 || ss::event()==124137 || ss::event()==1242117 || ss::event()==1242603 || ss::event()==1246484 || ss::event()==1248770 || ss::event()==1252490 || ss::event()==1255197 || ss::event()==1258521 || ss::event()==1258588 || ss::event()==1259800 || ss::event()==1260177 || ss::event()==1261793 || ss::event()==1265086 || ss::event()==1265710 || ss::event()==1267035 || ss::event()==1267257 || ss::event()==1267325 || ss::event()==1267552 || ss::event()==1269065 || ss::event()==1270632 || ss::event()==1273472 || ss::event()==1276433 || ss::event()==1276473 || ss::event()==1278634 || ss::event()==1279435 || ss::event()==1280577 || ss::event()==1286369 || ss::event()==1286778 || ss::event()==1286781 || ss::event()==1288293 || ss::event()==1289757 || ss::event()==1290160 || ss::event()==1291118 || ss::event()==1291551 || ss::event()==1292509 || ss::event()==1295238 || ss::event()==1298014 || ss::event()==1299129 || ss::event()==1299474 || ss::event()==1299516 || ss::event()==1304710 || ss::event()==1312217 || ss::event()==1317717 || ss::event()==1321726 || ss::event()==1329026 || ss::event()==1331730 || ss::event()==133342 || ss::event()==1334552 || ss::event()==1335679 || ss::event()==1341232 || ss::event()==1346500 || ss::event()==1346873 || ss::event()==1348086 || ss::event()==1349052 || ss::event()==1349173 || ss::event()==1349294 || ss::event()==1350155 || ss::event()==1350605 || ss::event()==1351070 || ss::event()==1352128 || ss::event()==1353832 || ss::event()==135529 || ss::event()==1356135 || ss::event()==1357509 || ss::event()==1361728 || ss::event()==1363564 || ss::event()==1364719 || ss::event()==1365287 || ss::event()==1369677 || ss::event()==1371989 || ss::event()==1373478 || ss::event()==1375476 || ss::event()==1379515 || ss::event()==1379744 || ss::event()==1380415 || ss::event()==1380761 || ss::event()==1381705 || ss::event()==1389175 || ss::event()==1389200 || ss::event()==1390228 || ss::event()==1393899 || ss::event()==1396335 || ss::event()==1396617 || ss::event()==1397849 || ss::event()==1400438 || ss::event()==1410583 || ss::event()==1411054 || ss::event()==1412640 
            //     || ss::event()==1414106 || ss::event()==1414630 || ss::event()==1416141 || ss::event()==1417057 || ss::event()==1417723 || ss::event()==1419192 || ss::event()==1431962 || ss::event()==1438291 || ss::event()==1442166 || ss::event()==1447489 || ss::event()==1449986 || ss::event()==1450658 || ss::event()==1451553 || ss::event()==1457142 || ss::event()==1458604 || ss::event()==1458834 || ss::event()==1461914 || ss::event()==1461996 || ss::event()==1462159 || ss::event()==1462708 || ss::event()==1464990 || ss::event()==146579 || ss::event()==1469627 || ss::event()==1472626 || ss::event()==1472830 || ss::event()==1473819 || ss::event()==1474074 || ss::event()==1475398 || ss::event()==147620 || ss::event()==1476875 || ss::event()==147930 || ss::event()==1479343 || ss::event()==1480651 || ss::event()==1481178 || ss::event()==1483013 || ss::event()==1484356 || ss::event()==1485570 || ss::event()==1488659 || ss::event()==1490932 || ss::event()==1492830 || ss::event()==149290 || ss::event()==1493639 || ss::event()==1496953 || ss::event()==1499707 || ss::event()==1502717 || ss::event()==1504029 || ss::event()==1505312 || ss::event()==1506759 || ss::event()==1510387 || ss::event()==1510507 || ss::event()==1512030 || ss::event()==1512795 || ss::event()==1514874 || ss::event()==1515706 || ss::event()==1518394 || ss::event()==1520621 || ss::event()==1521886 || ss::event()==1521917 || ss::event()==1523992 || ss::event()==1528784 || ss::event()==1530483 || ss::event()==1530711 || ss::event()==1531974 || ss::event()==1537115 || ss::event()==1537431 || ss::event()==1537461 || ss::event()==1537535 || ss::event()==153904 || ss::event()==1541084 || ss::event()==1541646 || ss::event()==1542093 || ss::event()==1544543 || ss::event()==1546922 || ss::event()==1547585 || ss::event()==1548514 || ss::event()==1551622 || ss::event()==155211 || ss::event()==1557179 || ss::event()==1558218 || ss::event()==1561448 || ss::event()==1565898 || ss::event()==1569042 || ss::event()==1570072 || ss::event()==1570801 || ss::event()==157144 || ss::event()==1571809 || ss::event()==1572236 || ss::event()==1574938 || ss::event()==1575365 || ss::event()==1578770 || ss::event()==1582788 || ss::event()==1584855 || ss::event()==1586405 || ss::event()==1588227 || ss::event()==1588845 || ss::event()==1592621 || ss::event()==1596889 || ss::event()==1598738 || ss::event()==1603545 || ss::event()==160592 || ss::event()==1606309 
            //     || ss::event()==1608070 || ss::event()==1608245 || ss::event()==1609402 || ss::event()==1609719 || ss::event()==1609959 || ss::event()==1610930 || ss::event()==1611187 || ss::event()==1611344 || ss::event()==1613546 || ss::event()==1614566 || ss::event()==1615702 || ss::event()==1620026 || ss::event()==1632856 || ss::event()==1633090 || ss::event()==1637212 || ss::event()==1641802 || ss::event()==1646594 || ss::event()==1652373 || ss::event()==1652403 || ss::event()==1658362 || ss::event()==1661890 || ss::event()==1662276 || ss::event()==1664111 || ss::event()==1665635 || ss::event()==1666355 || ss::event()==1667309 || ss::event()==1672202 || ss::event()==167512 || ss::event()==1675389 || ss::event()==1675949 || ss::event()==1685554 || ss::event()==1687110 || ss::event()==1688282 || ss::event()==1693048 || ss::event()==169916 || ss::event()==1700371 || ss::event()==1701095 || ss::event()==1706020 || ss::event()==1708627 || ss::event()==1708649 || ss::event()==1710652 || ss::event()==1710981 || ss::event()==1715076 || ss::event()==1716446 || ss::event()==1720016 || ss::event()==1722235 || ss::event()==1725412 || ss::event()==1730850 || ss::event()==1734654 || ss::event()==173495 || ss::event()==1736391 || ss::event()==1737368 || ss::event()==1738153 || ss::event()==1739626 || ss::event()==1742140 || ss::event()==1742439 || ss::event()==1743724 || ss::event()==1748478 || ss::event()==1752576 || ss::event()==1756479 || ss::event()==1756898 || ss::event()==1757421 || ss::event()==1759247 || ss::event()==1766544 || ss::event()==1768224 || ss::event()==1771110 || ss::event()==1772073 || ss::event()==1773925 || ss::event()==1774674 || ss::event()==1779360 || ss::event()==1781602 || ss::event()==1781732 || ss::event()==1783472 || ss::event()==1784754 || ss::event()==178527 || ss::event()==1786858 || ss::event()==1786962 || ss::event()==1791930 || ss::event()==179246 || ss::event()==1795327 || ss::event()==1796058 || ss::event()==1796845 || ss::event()==179719 || ss::event()==1797317 || ss::event()==1800573 || ss::event()==1801234 || ss::event()==1804520 || ss::event()==1804765 || ss::event()==1806897 || ss::event()==1810307 || ss::event()==1810454 || ss::event()==1810730 || ss::event()==1811788 || ss::event()==1813363 || ss::event()==1816226 || ss::event()==1816667 || ss::event()==1816797 || ss::event()==1817016 || ss::event()==1818340 || ss::event()==1818437 || ss::event()==1820057 
            //     || ss::event()==1820249 || ss::event()==1820752 || ss::event()==1824029 || ss::event()==1825114 || ss::event()==1825789 || ss::event()==1830666 || ss::event()==1832384 || ss::event()==1833298 || ss::event()==1836896 || ss::event()==1837752 || ss::event()==1840800 || ss::event()==1841349 || ss::event()==1843467 || ss::event()==1848622 || ss::event()==1848845 || ss::event()==1850714 || ss::event()==1851409 || ss::event()==1851651 || ss::event()==1853037 || ss::event()==1855390 || ss::event()==1856485 || ss::event()==1857221 || ss::event()==1858749 || ss::event()==1859143 || ss::event()==185921 || ss::event()==186470 || ss::event()==186475 || ss::event()==1865175 || ss::event()==1865394 || ss::event()==186752 || ss::event()==186867 || ss::event()==187283 || ss::event()==1875022 || ss::event()==1876734 || ss::event()==1878514 || ss::event()==188142 || ss::event()==1885123 || ss::event()==1888992 || ss::event()==1891092 || ss::event()==1892617 || ss::event()==1893634 || ss::event()==1897318 || ss::event()==1901067 || ss::event()==1901471 || ss::event()==1903220 || ss::event()==1907114 || ss::event()==1907954 || ss::event()==1909481 || ss::event()==1910009 || ss::event()==1913025 || ss::event()==1913621 || ss::event()==1914562 || ss::event()==1916092 || ss::event()==1920042 || ss::event()==1920391 || ss::event()==1921495 || ss::event()==1923332 || ss::event()==1925063 || ss::event()==1929520 || ss::event()==1935437 || ss::event()==193578 || ss::event()==1939422 || ss::event()==1944552 || ss::event()==194471 || ss::event()==1945259 || ss::event()==1945420 || ss::event()==1948335 || ss::event()==1950543 || ss::event()==1951227 || ss::event()==1952929 || ss::event()==1955545 || ss::event()==195917 || ss::event()==1959853 || ss::event()==1960878 || ss::event()==1964617 || ss::event()==1964982 || ss::event()==1967944 || ss::event()==1969104 || ss::event()==1969522 || ss::event()==1970496 || ss::event()==1971358 || ss::event()==1974656 || ss::event()==1974889 || ss::event()==1978110 || ss::event()==1979371 || ss::event()==1982368 || ss::event()==1985354 || ss::event()==1988805 || ss::event()==1989890 || ss::event()==1992974 || ss::event()==1993254 || ss::event()==1996415 || ss::event()==199710 || ss::event()==2003175 || ss::event()==2003512 || ss::event()==200678 || ss::event()==2011469 || ss::event()==201515 || ss::event()==2015582 || ss::event()==2016005 || ss::event()==2020006 
            //     || ss::event()==2021745 || ss::event()==2023132 || ss::event()==2024750 || ss::event()==2034912 || ss::event()==2036490 || ss::event()==2037051 || ss::event()==2037251 || ss::event()==2037649 || ss::event()==2039922 || ss::event()==2041189 || ss::event()==2041360 || ss::event()==2044639 || ss::event()==2045175 || ss::event()==2045557 || ss::event()==205086 || ss::event()==2050940 || ss::event()==2051186 || ss::event()==2052205 || ss::event()==2053244 || ss::event()==2053861 || ss::event()==205635 || ss::event()==2056435 || ss::event()==2057492 || ss::event()==2057596 || ss::event()==2058748 || ss::event()==2060262 || ss::event()==2062732 || ss::event()==2063794 || ss::event()==2063843 || ss::event()==2074265 || ss::event()==2074731 || ss::event()==2076570 || ss::event()==2084640 || ss::event()==2089153 || ss::event()==2090123 || ss::event()==2090176 || ss::event()==2092518 || ss::event()==2096956 || ss::event()==2098542 || ss::event()==2100104 || ss::event()==2101505 || ss::event()==2101788 || ss::event()==2101967 || ss::event()==2103959 || ss::event()==2104203 || ss::event()==2105770 || ss::event()==2105816 || ss::event()==2106922 || ss::event()==2115583 || ss::event()==2116355 || ss::event()==2118558 || ss::event()==2119180 || ss::event()==2120328 || ss::event()==2120381 || ss::event()==212217 || ss::event()==2124482 || ss::event()==2124950 || ss::event()==2127470 || ss::event()==2129642 || ss::event()==2130899 || ss::event()==2131070 || ss::event()==2137098 || ss::event()==2141102 || ss::event()==2142183 || ss::event()==2143604 || ss::event()==2143675 || ss::event()==2144420 || ss::event()==2145197 || ss::event()==2145208 || ss::event()==2146199 || ss::event()==2147820 || ss::event()==2148327 || ss::event()==2151613 || ss::event()==2152471 || ss::event()==2152843 || ss::event()==2153785 || ss::event()==2155529 || ss::event()==2156056 || ss::event()==2158283 || ss::event()==2161687 || ss::event()==2161756 || ss::event()==2164702 || ss::event()==2164956 || ss::event()==2173465 || ss::event()==2175029 || ss::event()==217706 || ss::event()==2183315 || ss::event()==2183789 || ss::event()==2184632 || ss::event()==2185979 || ss::event()==2193397 || ss::event()==2193887 || ss::event()==2196654 || ss::event()==2202591 || ss::event()==220546 || ss::event()==22108 || ss::event()==2226279 || ss::event()==2227827 || ss::event()==2231734 || ss::event()==2233734 || ss::event()==2233823 
            //     || ss::event()==2234415 || ss::event()==2236336 || ss::event()==2236463 || ss::event()==2238776 || ss::event()==2239767 || ss::event()==2240295 || ss::event()==2242826 || ss::event()==2242847 || ss::event()==2251102 || ss::event()==2258363 || ss::event()==226181 || ss::event()==2261890 || ss::event()==2264435 || ss::event()==2264784 || ss::event()==2265085 || ss::event()==2268600 || ss::event()==2276640 || ss::event()==2278223 || ss::event()==2278258 || ss::event()==2280802 || ss::event()==228121 || ss::event()==2284114 || ss::event()==2284358 || ss::event()==2284497 || ss::event()==2285924 || ss::event()==2287318 || ss::event()==2287649 || ss::event()==2287895 || ss::event()==2289372 || ss::event()==2293722 || ss::event()==2297766 || ss::event()==2299846 || ss::event()==230001 || ss::event()==230801 || ss::event()==2308107 || ss::event()==2308999 || ss::event()==2309384 || ss::event()==2310472 || ss::event()==2312788 || ss::event()==2314161 || ss::event()==2314324 || ss::event()==231608 || ss::event()==2317422 || ss::event()==2320832 || ss::event()==2321462 || ss::event()==2330668 || ss::event()==2332225 || ss::event()==2332849 || ss::event()==2334330 || ss::event()==2336591 || ss::event()==2336768 || ss::event()==2337542 || ss::event()==2337908 || ss::event()==234106 || ss::event()==2341430 || ss::event()==2342518 || ss::event()==2342868 || ss::event()==2344127 || ss::event()==2346180 || ss::event()==2347132 || ss::event()==2348716 || ss::event()==2350144 || ss::event()==2353030 || ss::event()==2355016 || ss::event()==2357002 || ss::event()==2357344 || ss::event()==2362165 || ss::event()==2363744 || ss::event()==2364823 || ss::event()==2365779 || ss::event()==2368346 || ss::event()==2370910 || ss::event()==2372727 || ss::event()==2374836 || ss::event()==2376506 || ss::event()==2378602 || ss::event()==2381321 || ss::event()==2381481 || ss::event()==2383325 || ss::event()==2386007 || ss::event()==2386021 || ss::event()==2386181 || ss::event()==2387700 || ss::event()==239030 || ss::event()==2390872 || ss::event()==2391737 || ss::event()==2393600 || ss::event()==2394567 || ss::event()==2395350 || ss::event()==2397035 || ss::event()==2397604 || ss::event()==2404732 || ss::event()==2407541 || ss::event()==2409848 || ss::event()==2413683 || ss::event()==2415749 || ss::event()==2416880 || ss::event()==2418117 || ss::event()==2420803 || ss::event()==2425035 || ss::event()==2432124 
            //     || ss::event()==2433092 || ss::event()==2435842 || ss::event()==243627 || ss::event()==243669 || ss::event()==2440975 || ss::event()==2444015 || ss::event()==2444216 || ss::event()==2444335 || ss::event()==2445019 || ss::event()==2447883 || ss::event()==2455186 || ss::event()==2456008 || ss::event()==2458655 || ss::event()==2460385 || ss::event()==2466363 || ss::event()==2469186 || ss::event()==2470641 || ss::event()==2472962 || ss::event()==2475038 || ss::event()==2480450 || ss::event()==2494634 || ss::event()==2496000 || ss::event()==249680 || ss::event()==2497200 || ss::event()==249767 || ss::event()==250031 || ss::event()==2501827 || ss::event()==2502928 || ss::event()==2503428 || ss::event()==2509488 || ss::event()==2512951 || ss::event()==2513157 || ss::event()==2514101 || ss::event()==2514679 || ss::event()==2519125 || ss::event()==2525577 || ss::event()==252604 || ss::event()==2532505 || ss::event()==254029 || ss::event()==2544059 || ss::event()==2544395 || ss::event()==2544769 || ss::event()==2545646 || ss::event()==2555809 || ss::event()==2556587 || ss::event()==2562042 || ss::event()==2565525 || ss::event()==2573466 || ss::event()==2577651 || ss::event()==2580854 || ss::event()==2582640 || ss::event()==2588344 || ss::event()==2589776 || ss::event()==2593702 || ss::event()==2594842 || ss::event()==2595876 || ss::event()==2597901 || ss::event()==2600294 || ss::event()==260109 || ss::event()==2601793 || ss::event()==2605980 || ss::event()==2609768 || ss::event()==2612310 || ss::event()==2615325 || ss::event()==2621156 || ss::event()==2621521 || ss::event()==2626552 || ss::event()==2628006 || ss::event()==2630931 || ss::event()==2634644 || ss::event()==263497 || ss::event()==2635590 || ss::event()==2635995 || ss::event()==263646 || ss::event()==263660 || ss::event()==2639335 || ss::event()==2640309 || ss::event()==264138 || ss::event()==2643007 || ss::event()==2643341 || ss::event()==2644633 || ss::event()==2645762 || ss::event()==26469 || ss::event()==2647907 || ss::event()==2649160 || ss::event()==2653662 || ss::event()==265719 || ss::event()==2658502 || ss::event()==2658565 || ss::event()==2661258 || ss::event()==266262 || ss::event()==26635 || ss::event()==266613 || ss::event()==2668374 || ss::event()==2668605 || ss::event()==2669009 || ss::event()==2672138 || ss::event()==2674475 || ss::event()==2675479 || ss::event()==2675512 || ss::event()==2686161 
            //     || ss::event()==2690930 || ss::event()==2692191 || ss::event()==2692843 || ss::event()==2693555 || ss::event()==2697644 || ss::event()==2698916 || ss::event()==2700646 || ss::event()==2701211 || ss::event()==2709545 || ss::event()==2710409 || ss::event()==2712015 || ss::event()==2712607 || ss::event()==2724025 || ss::event()==2730623 || ss::event()==2731047 || ss::event()==2731455 || ss::event()==2734471 || ss::event()==2738680 || ss::event()==2738888 || ss::event()==2742947 || ss::event()==274340 || ss::event()==274357 || ss::event()==2744870 || ss::event()==2745463 || ss::event()==2746968 || ss::event()==2747306 || ss::event()==274865 || ss::event()==2749762 || ss::event()==2751107 || ss::event()==2752883 || ss::event()==2754781 || ss::event()==2758423 || ss::event()==2764439 || ss::event()==2766497 || ss::event()==2767823 || ss::event()==2772493 || ss::event()==2773181 || ss::event()==2774609 || ss::event()==2774855 || ss::event()==2775238 || ss::event()==27755 || ss::event()==2777311 || ss::event()==2780178 || ss::event()==2780394 || ss::event()==2782949 || ss::event()==278605 || ss::event()==2790431 || ss::event()==2790591 || ss::event()==27906 || ss::event()==2791822 || ss::event()==2795720 || ss::event()==2796122 || ss::event()==2796778 || ss::event()==2797617 || ss::event()==2798636 || ss::event()==2800031 || ss::event()==2802057 || ss::event()==2802623 || ss::event()==2805006 || ss::event()==2805463 || ss::event()==2807235 || ss::event()==280794 || ss::event()==2808026 || ss::event()==2812731 || ss::event()==2814783 || ss::event()==2815434 || ss::event()==281562 || ss::event()==2817158 || ss::event()==2817906 || ss::event()==282252 || ss::event()==2825283 || ss::event()==2829241 || ss::event()==283003 || ss::event()==2832399 || ss::event()==2837619 || ss::event()==2840584 || ss::event()==2851501 || ss::event()==2853015 || ss::event()==2854658 || ss::event()==2855637 || ss::event()==285580 || ss::event()==2855927 || ss::event()==285835 || ss::event()==2858553 || ss::event()==2858985 || ss::event()==2860775 || ss::event()==2864462 || ss::event()==2867069 || ss::event()==2867317 || ss::event()==2867365 || ss::event()==2867957 || ss::event()==2870275 || ss::event()==2871245 || ss::event()==2871428 || ss::event()==2877242 || ss::event()==2877562 || ss::event()==2880585 || ss::event()==2883636 || ss::event()==2884146 || ss::event()==2885416 || ss::event()==2891037 
            //     || ss::event()==2891750 || ss::event()==2892607 || ss::event()==2893937 || ss::event()==2896089 || ss::event()==2896629 || ss::event()==2897504 || ss::event()==2898754 || ss::event()==2899568 || ss::event()==2902662 || ss::event()==2903284 || ss::event()==2904132 || ss::event()==2904199 || ss::event()==2905021 || ss::event()==2906054 || ss::event()==2907496 || ss::event()==2907911 || ss::event()==2909490 || ss::event()==2910141 || ss::event()==2910612 || ss::event()==2913545 || ss::event()==2917949 || ss::event()==2920649 || ss::event()==292242 || ss::event()==2927358 || ss::event()==292754 || ss::event()==2931928 || ss::event()==29331 || ss::event()==2933414 || ss::event()==2937026 || ss::event()==2938155 || ss::event()==294054 || ss::event()==2940995 || ss::event()==2945044 || ss::event()==2951060 || ss::event()==2952648 || ss::event()==2954902 || ss::event()==2956186 || ss::event()==2962952 || ss::event()==2963302 || ss::event()==2967612 || ss::event()==2968268 || ss::event()==2971783 || ss::event()==2973258 || ss::event()==2973888 || ss::event()==2975467 || ss::event()==2983072 || ss::event()==2996339 || ss::event()==2997717 || ss::event()==3000354 || ss::event()==3002624 || ss::event()==3004929 || ss::event()==3006743 || ss::event()==3007133 || ss::event()==3007560 || ss::event()==3009702 || ss::event()==3009963 || ss::event()==301001 || ss::event()==3012494 || ss::event()==3014710 || ss::event()==3015889 || ss::event()==301613 || ss::event()==3018415 || ss::event()==3019201 || ss::event()==302073 || ss::event()==3023277 || ss::event()==3025052 || ss::event()==3027062 || ss::event()==3027758 || ss::event()==3029199 || ss::event()==3029249 || ss::event()==3031281 || ss::event()==3032969 || ss::event()==303308 || ss::event()==3036490 || ss::event()==3036670 || ss::event()==304080 || ss::event()==3043421 || ss::event()==3044519 || ss::event()==3048213 || ss::event()==3057483 || ss::event()==305753 || ss::event()==305759 || ss::event()==305997 || ss::event()==3061724 || ss::event()==3065886 || ss::event()==3066130 || ss::event()==3066681 || ss::event()==3067773 || ss::event()==3070507 || ss::event()==3076588 || ss::event()==3077193 || ss::event()==3079123 || ss::event()==3083719 || ss::event()==3084608 || ss::event()==3085964 || ss::event()==3086635 || ss::event()==3090263 || ss::event()==3091415 || ss::event()==3091751 || ss::event()==3094200 || ss::event()==3094696 
            //     || ss::event()==3096065 || ss::event()==309833 || ss::event()==3099295 || ss::event()==3103201 || ss::event()==3108433 || ss::event()==3112208 || ss::event()==3114656 || ss::event()==3120611 || ss::event()==3126611 || ss::event()==3128101 || ss::event()==3128270 || ss::event()==3128584 || ss::event()==313463 || ss::event()==3136267 || ss::event()==3136886 || ss::event()==3138589 || ss::event()==3140435 || ss::event()==3140677 || ss::event()==3142982 || ss::event()==314662 || ss::event()==3148233 || ss::event()==3149809 || ss::event()==3152523 || ss::event()==3153602 || ss::event()==3157703 || ss::event()==3158790 || ss::event()==3159295 || ss::event()==3165416 || ss::event()==3165606 || ss::event()==3166994 || ss::event()==3167850 || ss::event()==3170573 || ss::event()==3173578 || ss::event()==317632 || ss::event()==3176879 || ss::event()==3180525 || ss::event()==3183230 || ss::event()==3183735 || ss::event()==3184809 || ss::event()==3193706 || ss::event()==3194237 || ss::event()==3194351 || ss::event()==3195334 || ss::event()==319768 || ss::event()==3198625 || ss::event()==3200456 || ss::event()==3200968 || ss::event()==3202756 || ss::event()==3207977 || ss::event()==3209221 || ss::event()==3211745 || ss::event()==3213635 || ss::event()==3215743 || ss::event()==3216053 || ss::event()==3216352 || ss::event()==3219768 || ss::event()==3220619 || ss::event()==3224713 || ss::event()==3229921 || ss::event()==3231843 || ss::event()==3234653 || ss::event()==323643 || ss::event()==3242551 || ss::event()==3244068 || ss::event()==3245570 || ss::event()==3247215 || ss::event()==3247564 || ss::event()==3248590 || ss::event()==324965 || ss::event()==3253088 || ss::event()==3253545 || ss::event()==3257829 || ss::event()==3260903 || ss::event()==3263544 || ss::event()==3266872 || ss::event()==3273285 || ss::event()==3274273 || ss::event()==3281123 || ss::event()==3285111 || ss::event()==3285449 || ss::event()==3289963 || ss::event()==3294712 || ss::event()==3295805 || ss::event()==3298095 || ss::event()==329872 || ss::event()==3300311 || ss::event()==3301258 || ss::event()==3303983 || ss::event()==3305455 || ss::event()==3306135 || ss::event()==3306288 || ss::event()==3307064 || ss::event()==3312422 || ss::event()==3313633 || ss::event()==3314519 || ss::event()==3315249 || ss::event()==3318358 || ss::event()==3322574 || ss::event()==3322585 || ss::event()==3322888 || ss::event()==3322891 
            //     || ss::event()==3326765 || ss::event()==3327322 || ss::event()==3330508 || ss::event()==333290 || ss::event()==333432 || ss::event()==3336316 || ss::event()==3337483 || ss::event()==3337942 || ss::event()==3340086 || ss::event()==3341545 || ss::event()==3341850 || ss::event()==3343996 || ss::event()==3348562 || ss::event()==3351901 || ss::event()==3356547 || ss::event()==3357259 || ss::event()==3358094 || ss::event()==3358828 || ss::event()==335902 || ss::event()==3360312 || ss::event()==336232 || ss::event()==3364492 || ss::event()==336596 || ss::event()==3368023 || ss::event()==3375080 || ss::event()==3376654 || ss::event()==3376986 || ss::event()==3380651 || ss::event()==338276 || ss::event()==3385469 || ss::event()==3386252 || ss::event()==3386730 || ss::event()==3388349 || ss::event()==3389101 || ss::event()==3389388 || ss::event()==3392643 || ss::event()==3394592 || ss::event()==3397543 || ss::event()==3398670 || ss::event()==3399231 || ss::event()==3399635 || ss::event()==3400606 || ss::event()==3405935 || ss::event()==3406532 || ss::event()==341199 || ss::event()==3418829 || ss::event()==3420033 || ss::event()==3421024 || ss::event()==3433750 || ss::event()==3435414 || ss::event()==3435450 || ss::event()==3435457 || ss::event()==3437998 || ss::event()==3440200 || ss::event()==3445236 || ss::event()==3446429 || ss::event()==3447812 || ss::event()==3451677 || ss::event()==3452895 || ss::event()==3454384 || ss::event()==3454635 || ss::event()==3454826 || ss::event()==3457630 || ss::event()==3457786 || ss::event()==3458618 || ss::event()==3464924 || ss::event()==3466881 || ss::event()==3468839 || ss::event()==3478986 || ss::event()==3480190 || ss::event()==3483506 || ss::event()==3484579 || ss::event()==3489014 || ss::event()==3493803 || ss::event()==3495768 || ss::event()==3496844 || ss::event()==3500464 || ss::event()==3500601 || ss::event()==3503065 || ss::event()==3504407 || ss::event()==3508323 || ss::event()==3511799 || ss::event()==351214 || ss::event()==351397 || ss::event()==3515083 || ss::event()==3517630 || ss::event()==352087 || ss::event()==3525044 || ss::event()==352722 || ss::event()==3527900 || ss::event()==3529709 || ss::event()==3531679 || ss::event()==3537080 || ss::event()==3537767 || ss::event()==3542009 || ss::event()==3558733 || ss::event()==3562888 || ss::event()==3563826 || ss::event()==3564731 || ss::event()==3566244 || ss::event()==3567057 
            //     || ss::event()==3568505 || ss::event()==3568596 || ss::event()==3570105 || ss::event()==3573508 || ss::event()==3574564 || ss::event()==3577691 || ss::event()==3577907 || ss::event()==3578084 || ss::event()==3584208 || ss::event()==3586533 || ss::event()==3591707 || ss::event()==3596925 || ss::event()==3598423 || ss::event()==3599753 || ss::event()==3602881 || ss::event()==3606934 || ss::event()==3609220 || ss::event()==3611703 || ss::event()==3611851 || ss::event()==3613663 || ss::event()==3613729 || ss::event()==3616694 || ss::event()==361928 || ss::event()==3619413 || ss::event()==3624984 || ss::event()==3625078 || ss::event()==3627750 || ss::event()==3628069 || ss::event()==3634414 || ss::event()==3635185 || ss::event()==366385 || ss::event()==368288 || ss::event()==370165 || ss::event()==379886 || ss::event()==380143 || ss::event()==384610 || ss::event()==386529 || ss::event()==38741 || ss::event()==39168 || ss::event()==393186 || ss::event()==395084 || ss::event()==399694 || ss::event()==400860 || ss::event()==401034 || ss::event()==404834 || ss::event()==405703 || ss::event()==406591 || ss::event()==416020 || ss::event()==416443 || ss::event()==418182 || ss::event()==420222 || ss::event()==420254 || ss::event()==423653 || ss::event()==424760 || ss::event()==429636 || ss::event()==433225 || ss::event()==434521 || ss::event()==437183 || ss::event()==43891 || ss::event()==440781 || ss::event()==44303 || ss::event()==446496 || ss::event()==451489 || ss::event()==451669 || ss::event()==454786 || ss::event()==459665 || ss::event()==460032 || ss::event()==461193 || ss::event()==462098 || ss::event()==463361 || ss::event()==464381 || ss::event()==468664 || ss::event()==469516 || ss::event()==471215 || ss::event()==471711 || ss::event()==474862 || ss::event()==477389 || ss::event()==48013 || ss::event()==482715 || ss::event()==483662 || ss::event()==486609 || ss::event()==487607 || ss::event()==489781 || ss::event()==490231 || ss::event()==490550 || ss::event()==500181 || ss::event()==501006 || ss::event()==503353 || ss::event()==503868 || ss::event()==505874 || ss::event()==506992 || ss::event()==510475 || ss::event()==510998 || ss::event()==521669 || ss::event()==529985 || ss::event()==532451 || ss::event()==535609 || ss::event()==537829 || ss::event()==53864 || ss::event()==544817 || ss::event()==545970 
            //     || ss::event()==549285 || ss::event()==551432 || ss::event()==554248 || ss::event()==555586 || ss::event()==558708 || ss::event()==562211 || ss::event()==563005 || ss::event()==563711 || ss::event()==565622 || ss::event()==568350 || ss::event()==569580 || ss::event()==570482 || ss::event()==571661 || ss::event()==572476 || ss::event()==573482 || ss::event()==575161 || ss::event()==57815 || ss::event()==578626 || ss::event()==581052 || ss::event()==583445 || ss::event()==584384 || ss::event()==586594 || ss::event()==587121 || ss::event()==588845 || ss::event()==590394 || ss::event()==592889 || ss::event()==598078 || ss::event()==601355 || ss::event()==601883 || ss::event()==603635 || ss::event()==606382 || ss::event()==606932 || ss::event()==607129 || ss::event()==61094 || ss::event()==613061 || ss::event()==613120 || ss::event()==615667 || ss::event()==618561 || ss::event()==618841 || ss::event()==620914 || ss::event()==625187 || ss::event()==625814 || ss::event()==627783 || ss::event()==63209 || ss::event()==632290 || ss::event()==633343 || ss::event()==635994 || ss::event()==63726 || ss::event()==640499 || ss::event()==64124 || ss::event()==642549 || ss::event()==64409 || ss::event()==644530 || ss::event()==644574 || ss::event()==645677 || ss::event()==646622 || ss::event()==64722 || ss::event()==653344 || ss::event()==657044 || ss::event()==657865 || ss::event()==66016 || ss::event()==660594 || ss::event()==66163 || ss::event()==662387 || ss::event()==663598 || ss::event()==66423 || ss::event()==669053 || ss::event()==672036 || ss::event()==67326 || ss::event()==674792 || ss::event()==677945 || ss::event()==679227 || ss::event()==680373 || ss::event()==684795 || ss::event()==685382 || ss::event()==685966 || ss::event()==699830 || ss::event()==700020 || ss::event()==700340 || ss::event()==700717 || ss::event()==703526 || ss::event()==708151 || ss::event()==709286 || ss::event()==710619 || ss::event()==713904 || ss::event()==716313 || ss::event()==721803 || ss::event()==723274 || ss::event()==724702 || ss::event()==72509 || ss::event()==726611 || ss::event()==727386 || ss::event()==728226 || ss::event()==728681 || ss::event()==733884 || ss::event()==737211 || ss::event()==737676 || ss::event()==739752 || ss::event()==741134 || ss::event()==742981 || ss::event()==74299 
            //     || ss::event()==743426 || ss::event()==749563 || ss::event()==751618 || ss::event()==753129 || ss::event()==754922 || ss::event()==755762 || ss::event()==755868 || ss::event()==756143 || ss::event()==756512 || ss::event()==757278 || ss::event()==759758 || ss::event()==760550 || ss::event()==763712 || ss::event()==76624 || ss::event()==767584 || ss::event()==769243 || ss::event()==77552 || ss::event()==777080 || ss::event()==781717 || ss::event()==782158 || ss::event()==783099 || ss::event()==786132 || ss::event()==786166 || ss::event()==792377 || ss::event()==794166 || ss::event()==795866 || ss::event()==796706 || ss::event()==803727 || ss::event()==804240 || ss::event()==804909 || ss::event()==805186 || ss::event()==806093 || ss::event()==80744 || ss::event()==807804 || ss::event()==809998 || ss::event()==811174 || ss::event()==811179 || ss::event()==812437 || ss::event()==813194 || ss::event()==814909 || ss::event()==815777 || ss::event()==816777 || ss::event()==818395 || ss::event()==818561 || ss::event()==824020 || ss::event()==826757 || ss::event()==827488 || ss::event()==83511 || ss::event()==837993 || ss::event()==838393 || ss::event()==841052 || ss::event()==843601 || ss::event()==845158 || ss::event()==85022 || ss::event()==851997 || ss::event()==859146 || ss::event()==861078 || ss::event()==862699 || ss::event()==865169 || ss::event()==871276 || ss::event()==873081 || ss::event()==87340 || ss::event()==876155 || ss::event()==877877 || ss::event()==878611 || ss::event()==879647 || ss::event()==883806 || ss::event()==884919 || ss::event()==885094 || ss::event()==895395 || ss::event()==895498 || ss::event()==896979 || ss::event()==900910 || ss::event()==902126 || ss::event()==904005 || ss::event()==904348 || ss::event()==905575 || ss::event()==906989 || ss::event()==909093 || ss::event()==91222 || ss::event()==915344 || ss::event()==915641 || ss::event()==917837 || ss::event()==920854 || ss::event()==92124 || ss::event()==923206 || ss::event()==924610 || ss::event()==924825 || ss::event()==925142 || ss::event()==925221 || ss::event()==92912 || ss::event()==929888 || ss::event()==932009 || ss::event()==932655 || ss::event()==937375 || ss::event()==938387 || ss::event()==939359 || ss::event()==941660 || ss::event()==943306 || ss::event()==944534 || ss::event()==946893 
            //     || ss::event()==949676 || ss::event()==952599 || ss::event()==953730 || ss::event()==955047 || ss::event()==956457 || ss::event()==956689 || ss::event()==957452 || ss::event()==959156 || ss::event()==959439 || ss::event()==961174 || ss::event()==962096 || ss::event()==964836 || ss::event()==966308 || ss::event()==975412 || ss::event()==976135 || ss::event()==977432 || ss::event()==977526 || ss::event()==982623 || ss::event()==98587 || ss::event()==989622 || ss::event()==992344 || ss::event()==994201 || ss::event()==994710 || ss::event()==995201 || ss::event()==997435 || ss::event()==997456 ) {
            //     int nleps_temp = 2;
            //     if (ss::lep3_passes_id()) nleps_temp++;
            //     int SR_temp = signalRegionTest(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, nleps_temp, isClass6);
            //     if (doSync && isttW) {
            //         std::cout << Form("%1d %9d %llu\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%1d\t%1d\t%1d\n",
            //                 ss::run(), ss::lumi(), ss::event(),
            //                 ss::lep1_id(), lep1_pt,
            //                 ss::lep2_id(), lep2_pt,
            //                 ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::hyp_class(), SR_temp, nleps_temp) << std::endl;
            //     }
            // }

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
            // if (ss::is_real_data()) {
            //     if (ss::lep3_passes_id()) nleps++;
            // } else {
            //     if (ss::lep3_passes_RA7v2() && ss::lep3_p4().pt() > 20.) nleps++;
            // }
            if (ss::lep4_passes_id() && (ss::lep4_p4().pt() > 20.) ) nleps++;

            //Require nominal baseline selections
            int BR = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, true);
            if (BR < 0) continue;

            if (!outputTrainingBDT) {
                // tttt baseline selections
                if (ss::njets() < 2) continue;
                if (ss::nbtags() < 2) continue;
                if (ss::met() < 50) continue;
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
            float pt3 = ss::lep3_p4().pt();
            float pto1 = max(max(pt1,pt2),pt3);
            float pto3 = min(min(pt1,pt2),pt3);
            float pto2 = pt1+pt2+pt3-pto1-pto3;
            int mytype = ss::hyp_type();
            if (mytype==2 && abs(ss::lep1_id())==13) mytype = 1;

            int SR = signalRegionTest(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, nleps, isClass6);

            // JEC
            float mll = (ss::lep1_p4()*lep1_pt/ss::lep1_p4().pt()+ss::lep2_p4()*lep2_pt/ss::lep2_p4().pt()).M();
            float mtl1_unc_up = MT(lep1_pt, ss::lep1_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
            float mtl2_unc_up = MT(lep2_pt, ss::lep2_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
            float mtmin_unc_up = mtl1_unc_up > mtl2_unc_up ? mtl2_unc_up : mtl1_unc_up;
            float mtl1_unc_dn = MT(lep1_pt, ss::lep1_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
            float mtl2_unc_dn = MT(lep2_pt, ss::lep2_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
            float mtmin_unc_dn = mtl1_unc_dn > mtl2_unc_dn ? mtl2_unc_dn : mtl1_unc_dn;
            int SR_unc_up = signalRegionTest(ss::njets_unc_up(), ss::nbtags_unc_up(), ss::met_unc_up(), ss::ht_unc_up(), mtmin_unc_up, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, nleps, isClass6);
            int SR_unc_dn = signalRegionTest(ss::njets_unc_dn(), ss::nbtags_unc_dn(), ss::met_unc_dn(), ss::ht_unc_dn(), mtmin_unc_dn, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, nleps, isClass6);

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
                SR_JER_up = signalRegionTest(ss::njets_JER_up(), ss::nbtags_JER_up(), ss::met_JER_up(), ss::ht_JER_up(), mtmin_JER_up, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, nleps, isClass6);
                SR_JER_dn = signalRegionTest(ss::njets_JER_dn(), ss::nbtags_JER_dn(), ss::met_JER_dn(), ss::ht_JER_dn(), mtmin_JER_dn, ss::lep1_id(), ss::lep2_id(), lep1_pt, lep2_pt, nleps, isClass6);
            }


            float mvavalue = -2.;
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
                for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
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
                tree_ptl2 = ss::lep1_p4().pt();
                tree_ptj1 = ss::jets().at(0).pt();
                tree_ptj2 = ss::jets().at(1).pt();
                // tree_stb = stb;
                tree_q1 = (ss::lep1_id()<0)-(ss::lep1_id()>0);
                tree_q2 = (ss::lep2_id()<0)-(ss::lep2_id()>0);
                tree_q3 = nleps >= 3 ? ((ss::lep3_id()<0)-(ss::lep3_id()>0)) : 0;
                tree_ht4ratio = ss::njets()>4 ? ht_first4/ht_rest : 0;
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
                if (outputTrainingBDT && !isData) out_tree->Fill();
                mvavalue = reader->EvaluateMVA("BDT");
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
            // if (mvavalue > -0.217) SRdisc = 2;
            // if (mvavalue > -0.164) SRdisc = 3;
            // if (mvavalue > -0.044) SRdisc = 4;
            // if (mvavalue >  0.055) SRdisc = 5;
            // if (mvavalue >  0.135) SRdisc = 6;
            // if (mvavalue >  0.205) SRdisc = 7;
            // if (mvavalue >  0.283) SRdisc = 8;
            // if (mvavalue >  0.368) SRdisc = 9;
            // if (mvavalue >  0.456) SRdisc = 10;

            // if (mvavalue > -0.217) SRdisc = 2;
            // if (mvavalue > -0.044) SRdisc = 3;
            // if (mvavalue >  0.135) SRdisc = 4;
            // if (mvavalue >  0.283) SRdisc = 5;
            // if (mvavalue >  0.456) SRdisc = 6;
            
            // if (mvavalue > -0.4) SRdisc = 2;
            // if (mvavalue > -0.2) SRdisc = 3;
            // if (mvavalue >  0.0) SRdisc = 4;
            // if (mvavalue >  0.2) SRdisc = 5;
            // if (mvavalue >  0.4) SRdisc = 6;

            if (mvavalue > -0.4) SRdisc = 2;
            if (mvavalue > -0.2) SRdisc = 3;
            if (mvavalue >  0.0) SRdisc = 4;
            if (mvavalue >  0.21) SRdisc = 5;
            if (mvavalue >  0.42) SRdisc = 6;

            int SRdisc_unc_up = SRdisc;
            int SRdisc_unc_dn = SRdisc;
            int SRdisc_JER_up = SRdisc;
            int SRdisc_JER_dn = SRdisc;

            if (isData  == 0 && SR_unc_up > 0)            p_jes_alt_up.SRCR.TOTAL->Fill(SR_unc_up, weight);
            if (isData  == 0 && SR_unc_dn > 0)            p_jes_alt_dn.SRCR.TOTAL->Fill(SR_unc_dn, weight);
            if (isData  == 0 && SRdisc_unc_up > 0)            p_jes_alt_up.SRDISC.TOTAL->Fill(SRdisc_unc_up, weight);
            if (isData  == 0 && SRdisc_unc_dn > 0)            p_jes_alt_dn.SRDISC.TOTAL->Fill(SRdisc_unc_dn, weight);

            if (isData  == 0 && SR_JER_up > 0)            p_jer_alt_up.SRCR.TOTAL->Fill(SR_JER_up, weight);
            if (isData  == 0 && SR_JER_dn > 0)            p_jer_alt_dn.SRCR.TOTAL->Fill(SR_JER_dn, weight);
            if (isData  == 0 && SRdisc_JER_up > 0)            p_jer_alt_up.SRDISC.TOTAL->Fill(SRdisc_JER_up, weight);
            if (isData  == 0 && SRdisc_JER_dn > 0)            p_jer_alt_dn.SRDISC.TOTAL->Fill(SRdisc_JER_dn, weight);

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
            // if all 3 charges are the same, throw the event away
            if (nleps > 2) {
              int q1 = 2*(ss::lep1_id() > 0) - 1;
              int q3 = 2*(ss::lep3_id() > 0) - 1;
              if (q3==q1) continue;
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

            if (SR > 1) { // non ttZ CR
                p_result.h_njets.br->Fill(ss::njets() , weight);
                p_result.h_nbtags.br->Fill(ss::nbtags() , weight);
                p_result.h_type.br->Fill(mytype , weight);
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
                p_result.h_mvis.ttzcr->Fill(mvis , weight);
                p_result.h_mtvis.ttzcr->Fill(mtvis , weight);
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
                p_result.h_mvis.ttwcr->Fill(mvis , weight);
                p_result.h_mtvis.ttwcr->Fill(mtvis , weight);
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
                p_result.h_mvis.sr->Fill(mvis , weight);
                p_result.h_mtvis.sr->Fill(mtvis , weight);
                auto ntopinfo = getNtops();
                p_result.h_ntops.sr->Fill(ntopinfo.first , weight);
                p_result.h_ntopness.sr->Fill(ntopinfo.second , weight);
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

                if (nleps > 2) {
                    int q1 = 2*(ss::lep1_id() > 0) - 1;
                    int q3 = 2*(ss::lep3_id() > 0) - 1;
                    p_result.h_charge3.sr->Fill(q3==q1 ? 1.5: 0.5 , weight);
                }
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
