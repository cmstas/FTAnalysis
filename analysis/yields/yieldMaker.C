// #define SSLOOP 1
// #undef SSLOOP

#include <bitset>
#include <iostream>
#include <algorithm>
#include "TChain.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TMath.h"
#include "TH2F.h"
#include "Math/LorentzVector.h"
#include "TKey.h"
#include "TSystem.h"
#include "TMVA/Reader.h"
#include "../misc/class_files/v8.02/SS.h"
// #include "../../common/CORE/SSSelections.h"
#include "../../common/CORE/Tools/dorky/dorky.h"
#include "../../common/CORE/Tools/goodrun.h"
#include "../../common/CORE/Tools/MT2/MT2Utility.h"
#include "../../common/CORE/Tools/MT2/MT2.h"
#include "../misc/common_utils.h"
#include "../misc/signal_regions.h"
// #include "../misc/bdt.h"
// namespace testbdt {
#include "../misc/bdt_run2.h"
// }
#include "../../babymaking/batch/nPoints.cc"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "../misc/tqdm.h"

#define WRITE(var) { if (plots.var) plots.var->Write(); }

bool STOP_REQUESTED = false;

float XSEC_TTTT = 11.97;
float scaleLumi = 1; // 75/35.87
// float scaleLumi = 400.0/137.2; // 75/35.87
TH2D *h_counts = 0;
TH1D *h_weights = 0;

void avoidNegativeYields(TH1F* plot);

struct SR_t     {
    TH1F* SR = 0;
    TH1F* SRCR = 0;
    TH1F* SRDISC = 0;
    TH1F* HH = 0;
    TH1F* HL = 0;
    TH1F* LL = 0;
    TH1F* ML = 0;
    TH1F* LM = 0;
    TH1F* MI1 = 0; // Scan over MET
    TH1F* MI2 = 0; // Scan over HT
    TH1F* INCL = 0;
    void Write() {
        AvoidNegativeYields();
        if (SR) SR->Write();
        if (SRCR) SRCR->Write();
        if (SRDISC) SRDISC->Write();
        if (HH) HH->Write();
        if (HL) HL->Write();
        if (LL) LL->Write();
        if (ML) ML->Write();
        if (LM) LM->Write();
        if (MI1) MI1->Write();
        if (MI2) MI2->Write();
        if (INCL) INCL->Write();
    }
#ifdef SSLOOP
    void CatFill(region_t category, float value, float weight, int SRmi1, int SRmi2, std::vector<int> SRincl) {
        if ((category == HighHigh   ) && HH) HH->Fill(value, weight);
        if ((category == HighLow    ) && HL) HL->Fill(value, weight);
        if ((category == LowLow     ) && LL) LL->Fill(value, weight);
        if ((category == Multilepton) && ML) ML->Fill(value, weight);
        if ((category == LowMet) && LM) LM->Fill(value, weight);

        if ((category == HighHigh) && MI1 && (value > 0) && (SRmi1 > 0)) { // MET
            for (int i = 1; i <= SRmi1; i++) {
                MI1->Fill(i,weight);
            }
        }
        if (((category == HighHigh) || (category == LowMet)) && MI2 && (value > 0) && (SRmi2 > 0)) { // HT
            for (int i = 1; i <= SRmi2; i++) {
                MI2->Fill(i,weight);
            }
        }
        if (INCL && (value > 0) && (SRincl.size() > 0)) {
            for (unsigned int i = 0; i < SRincl.size(); i++) {
                if (SRincl[i] > 0) {
                    INCL->Fill(SRincl[i], weight);
                }
            }
        }
    }
#else
    void CatFill(int which, float value, float weight) {
        if ((which == 0) && SR    ) SR->Fill(value, weight    );
        if ((which == 1) && SRCR  ) SRCR->Fill(value, weight  );
        if ((which == 2) && SRDISC) SRDISC->Fill(value, weight);
    }
#endif
    void AvoidNegativeYields() {
        if (SR) avoidNegativeYields(SR);
        if (SRCR) avoidNegativeYields(SRCR);
        if (SRDISC) avoidNegativeYields(SRDISC);
        if (HH) avoidNegativeYields(HH);
        if (HL) avoidNegativeYields(HL);
        if (LL) avoidNegativeYields(LL);
        if (ML) avoidNegativeYields(ML);
        if (LM) avoidNegativeYields(LM);
        if (MI1) avoidNegativeYields(MI1);
        if (MI2) avoidNegativeYields(MI2);
        if (INCL) avoidNegativeYields(INCL);
    }
    void InitHistError(bool poisson) {
        if (poisson) {
            if (SR) SR->SetBinErrorOption(TH1::kPoisson);
            if (SRCR) SRCR->SetBinErrorOption(TH1::kPoisson);
            if (SRDISC) SRDISC->SetBinErrorOption(TH1::kPoisson);
            if (HH) HH->SetBinErrorOption(TH1::kPoisson);
            if (HL) HL->SetBinErrorOption(TH1::kPoisson);
            if (LL) LL->SetBinErrorOption(TH1::kPoisson);
            if (ML) ML->SetBinErrorOption(TH1::kPoisson);
            if (LM) LM->SetBinErrorOption(TH1::kPoisson);
            if (MI1) MI1->SetBinErrorOption(TH1::kPoisson);
            if (MI2) MI2->SetBinErrorOption(TH1::kPoisson);
            if (INCL) INCL->SetBinErrorOption(TH1::kPoisson);
        } else {
            if (SR) SR->Sumw2();
            if (SRCR) SRCR->Sumw2();
            if (SRDISC) SRDISC->Sumw2();
            if (HH) HH->Sumw2();
            if (HL) HL->Sumw2();
            if (LL) LL->Sumw2();
            if (ML) ML->Sumw2();
            if (LM) LM->Sumw2();
            if (MI1) MI1->Sumw2();
            if (MI2) MI2->Sumw2();
            if (INCL) INCL->Sumw2();
        }
    }
};


enum categbit_t { 
    kBaseline        = 1<<0,
    kSignalRegion    = 1<<1,
    kHighHigh        = 1<<2,
    kHighLow         = 1<<3,
    kLowLow          = 1<<4,
    kLowMet          = 1<<5,
    kMultilepton     = 1<<6,
    kMultileptonOnZ  = 1<<7,
    kMultileptonOffZ = 1<<8
};

struct triple_t {
    TH1F* br = 0; 
    TH1F* ssbr = 0;  // HH+HL+LL
    TH1F* sr = 0;
    TH1F* hh = 0;
    TH1F* hl = 0;
    TH1F* ll = 0;
    TH1F* ml = 0;
    TH1F* lm = 0;
    TH1F* mloffz = 0;
    TH1F* mlonz = 0;
    TH1F* ttzcr = 0;
    TH1F* ttwcr = 0;
    void Init(TString label, TString chainTitle, int nbins, float xlow, float xhigh) {
        ttwcr = new TH1F(Form("ttwcr_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); ttwcr->SetDirectory(0);
        ttzcr = new TH1F(Form("ttzcr_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); ttzcr->SetDirectory(0);
        sr = new TH1F(Form("sr_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); sr->SetDirectory(0);
        br = new TH1F(Form("br_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); br->SetDirectory(0);
        ssbr = new TH1F(Form("ssbr_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); ssbr->SetDirectory(0);
        hh = new TH1F(Form("hh_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); hh->SetDirectory(0);
        hl = new TH1F(Form("hl_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); hl->SetDirectory(0);
        ll = new TH1F(Form("ll_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); ll->SetDirectory(0);
        ml = new TH1F(Form("ml_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); ml->SetDirectory(0);
        lm = new TH1F(Form("lm_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); lm->SetDirectory(0);
        mloffz = new TH1F(Form("mloffz_%s_%s",label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); mloffz->SetDirectory(0);
        mlonz = new TH1F(Form("mlonz_%s_%s"  ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); mlonz->SetDirectory(0);
        sr = new TH1F(Form("sr_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); sr->SetDirectory(0);
    }


    void CatFill(int kcategs, float value, float weight) {
        if ((kcategs & kBaseline   ) && br) br->Fill(value, weight);
        if (((kcategs & kHighHigh) | (kcategs & kHighLow) | (kcategs & kLowLow)) && ssbr) ssbr->Fill(value, weight);
        if ((kcategs & kSignalRegion   ) && sr) sr->Fill(value, weight);
        if ((kcategs & kHighHigh   ) && hh) hh->Fill(value, weight);
        if ((kcategs & kHighLow    ) && hl) hl->Fill(value, weight);
        if ((kcategs & kLowLow     ) && ll) ll->Fill(value, weight);
        if ((kcategs & kLowMet) && lm) lm->Fill(value, weight);
        if ((kcategs & kMultilepton) && ml) ml->Fill(value, weight);
        if ((kcategs & kMultileptonOnZ) && mlonz) mlonz->Fill(value, weight);
        if ((kcategs & kMultileptonOffZ) && mloffz) mloffz->Fill(value, weight);
    }
    void Write() {
        if (br) { br->Write(); }
        if (ssbr) { ssbr->Write(); }
        if (ttzcr) { ttzcr->Write(); }
        if (ttwcr) { ttwcr->Write(); }
        if (hh) { hh->Write(); }
        if (hl) { hl->Write(); }
        if (ll) { ll->Write(); }
        if (ml) { ml->Write(); }
        if (lm) { lm->Write(); }
        if (mloffz) { mloffz->Write(); }
        if (mlonz) { mlonz->Write(); }
        if (sr) { sr->Write(); }
    }
};
struct tree_t {
    TFile* out_file;
    TTree* out_tree;
    float tree_weight = -1.;
    TString tree_name = "";
    int tree_year = -1;
    int tree_stype = -1;
    int tree_SR = -1;
    int tree_SRdisc = -1;
    int tree_hyp_class = -1;
    int tree_njets = -1;
    int tree_nbtags = -1;
    int tree_categ = -1;
    int tree_event = 0;
    int tree_run = 0;
    int tree_lumi = 0;
    bool tree_isdata = 0;
    float tree_mtmin = -1.;
    float tree_ht = -1.;
    float tree_met = -1.;
    int tree_lep1id = 0;
    int tree_lep2id = 0;
    int tree_lep3id = 0;
    float tree_lep1pt = -1.;
    float tree_lep2pt = -1.;
    float tree_lep3pt = -1.;
    float tree_lep1eta = -1.;
    float tree_lep2eta = -1.;
    float tree_lep3eta = -1.;
    float tree_lep1phi = -1.;
    float tree_lep2phi = -1.;
    float tree_lep3phi = -1.;
    int tree_iloose = -1;
    void Init(int year, TString chainTitle) {
        out_file = new TFile(Form("trees/tree_%i_%s.root",year,chainTitle.Data()),"RECREATE");
        out_file->cd();
        out_tree = new TTree("t", "tree_yieldMaker");
        out_tree->Branch("name" , &tree_name );
        out_tree->Branch("year" , &tree_year );
        out_tree->Branch("stype" , &tree_stype );
        out_tree->Branch("weight" , &tree_weight );
        out_tree->Branch("SR" , &tree_SR );
        out_tree->Branch("SRdisc" , &tree_SRdisc );
        out_tree->Branch("hyp_class" , &tree_hyp_class );
        out_tree->Branch("njets" , &tree_njets );
        out_tree->Branch("nbtags" , &tree_nbtags );
        out_tree->Branch("categ" , &tree_categ );
        out_tree->Branch("mtmin" , &tree_mtmin );
        out_tree->Branch("ht" , &tree_ht );
        out_tree->Branch("met" , &tree_met );
        out_tree->Branch("isdata" , &tree_isdata );
        out_tree->Branch("lep1id", &tree_lep1id );
        out_tree->Branch("lep2id", &tree_lep2id );
        out_tree->Branch("lep3id", &tree_lep3id );
        out_tree->Branch("lep1pt", &tree_lep1pt );
        out_tree->Branch("lep2pt", &tree_lep2pt );
        out_tree->Branch("lep3pt", &tree_lep3pt );
        out_tree->Branch("lep1eta", &tree_lep1eta );
        out_tree->Branch("lep2eta", &tree_lep2eta );
        out_tree->Branch("lep3eta", &tree_lep3eta );
        out_tree->Branch("lep1phi", &tree_lep1phi );
        out_tree->Branch("lep2phi", &tree_lep2phi );
        out_tree->Branch("lep3phi", &tree_lep3phi );
        out_tree->Branch("iloose", &tree_iloose );
        out_tree->Branch("event", &tree_event );
        out_tree->Branch("run", &tree_run );
        out_tree->Branch("lumi", &tree_lumi );
        // macro: 0dwiout_tree->Branch("kjldf_ea", &);kjlD0f"lviwyf&atree_kjpa kj
        tree_name = chainTitle;
        tree_year = year;
        if (chainTitle == "tttt") tree_stype = 0;
        if (chainTitle == "fakes") tree_stype = 1;
        if (chainTitle == "flips") tree_stype = 2;
        if (chainTitle == "ttw") tree_stype = 3;
        if (chainTitle == "ttz") tree_stype = 4;
        if (chainTitle == "tth") tree_stype = 5;
        if (chainTitle == "xg") tree_stype = 6;
        if (chainTitle == "ttvv") tree_stype = 7;
        if (chainTitle == "rares") tree_stype = 8;
        if (chainTitle == "ww") tree_stype = 9;
        if (chainTitle == "wz") tree_stype = 10;
        // if (chainTitle == "data") tree_stype = -1;
    }
    void FillVars(
            float weight, int SR, int SRdisc, int hyp_class
            ) {
        tree_weight = weight;
        tree_SR = SR;
        tree_SRdisc = SRdisc;
        tree_hyp_class = hyp_class;
        out_tree->Fill();
    }
    void FillVars(
            float weight, int SR, int categ, int hyp_class,
            int njets, int nbtags, float mtmin, float ht, float met,
            bool isdata, int lep1id, int lep2id, int lep3id,
            float lep1pt, float lep2pt, float lep3pt
            ) {
        tree_weight = weight;
        tree_SR = SR;
        tree_hyp_class = hyp_class;
        tree_njets = njets;
        tree_nbtags = nbtags;
        tree_categ = categ;
        tree_isdata = isdata;
        tree_mtmin = mtmin;
        tree_ht = ht;
        tree_met = met;
        tree_lep1id = lep1id;
        tree_lep2id = lep2id;
        tree_lep3id = lep3id;
        tree_lep1pt = lep1pt;
        tree_lep2pt = lep2pt;
        tree_lep3pt = lep3pt;
        if (tree_isdata) {
            tree_event = ss::event();
            tree_run = ss::run();
            tree_lumi = ss::lumi();
        }
        if (tree_isdata and hyp_class == 2) {
            tree_iloose = 1*(!ss::lep1_passes_id()) + 2*(!ss::lep2_passes_id());
        }
        // if (hyp_class == 1 or (hyp_class == 6 and (ss::lep1_passes_id()+ss::lep2_passes_id()+ss::lep3_passes_id())==1)) {
        //     // Two loose leptons, to calculate deltaR
        //     tree_lep1eta = ss::lep1_p4().eta();
        //     tree_lep2eta = ss::lep2_p4().eta();
        //     tree_lep3eta = ss::lep3_p4().eta();
        //     tree_lep1phi = ss::lep1_p4().phi();
        //     tree_lep2phi = ss::lep2_p4().phi();
        //     tree_lep3phi = ss::lep3_p4().phi();
        // }
        out_tree->Fill();
    }
    void Write() {
        out_file->cd();
        out_tree->Write();
        out_file->Close();
    }
};

struct plots_t  {
    SR_t SR;

    SR_t p_alphas_alt_dn_SR;
    SR_t p_alphas_alt_up_SR;
    SR_t p_bb_alt_up_SR    ;
    SR_t p_bb_alt_dn_SR    ;
    SR_t p_prefire_alt_up_SR    ;
    SR_t p_prefire_alt_dn_SR    ;
    SR_t p_trigger_alt_up_SR    ;
    SR_t p_trigger_alt_dn_SR    ;
    SR_t p_btagSF_alt_up_SR;
    SR_t p_btagSF_alt_dn_SR;
    SR_t p_btagSFheavy_alt_up_SR;
    SR_t p_btagSFheavy_alt_dn_SR;
    SR_t p_btagSFlight_alt_up_SR;
    SR_t p_btagSFlight_alt_dn_SR;
    SR_t p_fake_alt_up_SR  ;
    SR_t p_fake_nb0_up_SR  ;
    SR_t p_fake_nb1_up_SR  ;
    SR_t p_fake_nb2_up_SR  ;
    SR_t p_fake_nb3_up_SR  ;
    SR_t p_fake_unw_up_SR  ;
    SR_t p_fsrvar_alt_dn_SR;
    SR_t p_fsrvar_alt_up_SR;
    SR_t p_isr_alt_up_SR   ;
    SR_t p_isr_alt_dn_SR   ;
    SR_t p_isrvar_alt_dn_SR;
    SR_t p_isrvar_alt_up_SR;
    SR_t p_jer_alt_dn_SR   ;
    SR_t p_jer_alt_up_SR   ;
    SR_t p_jes_alt_dn_SR   ;
    SR_t p_jes_alt_up_SR   ;
    SR_t p_met_alt_up_SR   ;
    SR_t p_met_alt_dn_SR   ;
    SR_t p_lep_alt_up_SR   ;
    SR_t p_lep_alt_dn_SR   ;
    SR_t p_pdf_alt_dn_SR   ;
    SR_t p_pdf_alt_up_SR   ;
    SR_t p_pu_alt_dn_SR    ;
    SR_t p_pu_alt_up_SR    ;
    SR_t p_scale_alt_dn_SR ;
    SR_t p_scale_alt_up_SR ;
    SR_t p_unw_raw_SR   ;
    SR_t p_unw_sgn_SR   ;

    triple_t h_ht;
    triple_t h_met;
    triple_t h_metnm1;
    triple_t h_mll;
    triple_t h_mllbig;
    triple_t h_mllos;
    triple_t h_mtmin;
    triple_t h_mtmax;
    triple_t h_mt1;
    triple_t h_mt2;
    triple_t h_mt2min;
    triple_t h_nleptonicW;
    triple_t h_njets;
    triple_t h_nbtags;
    triple_t h_l1pt;
    triple_t h_l2pt;
    triple_t h_l3pt;
    triple_t h_type;
    triple_t h_class;
    triple_t h_type3;
    triple_t h_category;
    triple_t h_lumiblock;
    triple_t h_run;
    triple_t h_nvtx;
    triple_t h_charge;
    triple_t h_dphi;
    triple_t h_el_charge;
    triple_t h_mu_charge;
    triple_t h_q3;
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
    triple_t h_mu_l1phi;
    triple_t h_mu_lep1_miniIso;
    triple_t h_mu_lep1_ptRel;
    triple_t h_mu_lep1_ptRelfail;
    triple_t h_mu_lep1_ptRatio;
    triple_t h_el_sip3d_lep1;
    triple_t h_el_l1eta;
    triple_t h_el_l1phi;
    triple_t h_el_lep1_miniIso;
    triple_t h_el_lep1_ptRel;
    triple_t h_el_lep1_ptRelfail;
    triple_t h_el_lep1_ptRatio;
    triple_t h_mu_sip3d_lep2;
    triple_t h_mu_l2eta;
    triple_t h_mu_l2phi;
    triple_t h_mu_lep2_miniIso;
    triple_t h_mu_lep2_ptRel;
    triple_t h_mu_lep2_ptRelfail;
    triple_t h_mu_lep2_ptRatio;
    triple_t h_el_sip3d_lep2;
    triple_t h_el_l2eta;
    triple_t h_el_l2phi;
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

    triple_t h_lepsf;
    triple_t h_btagsf;
    triple_t h_wsf;
    triple_t h_puw;
    triple_t h_trigsf;
    triple_t h_isrw;

    triple_t h_dphil1l2;
    triple_t h_htb;
    triple_t h_nlb40;
    triple_t h_ntb40;
    triple_t h_detal1l2;
    triple_t h_maxmjoverpt;
    triple_t h_ml1j1;
    triple_t h_ptj1;
    triple_t h_ptj6;
    triple_t h_ptj7;
    triple_t h_ptj8;


};


//function declaration
plots_t run(TChain *chain, int year, TString options);
float calcDeltaPhi(float phi1, float phi2);
float calcMT(float pt1, float phi1, float pt2, float phi2);
float getSMSscale1fb(bool is1d);
float getRPVscale1fb();
float getWeightNormalization(int idx);
std::pair<float,float> getTrijetDiscs(TMVA::Reader* reader);

void getyields(TChain* ch, TString options="", TString outputdir="outputs/"){
    // TFile f1(Form("%s/histos_%s.root", outputdir.Data(), ch->GetTitle()), "RECREATE");

    signal(SIGINT, [](int){
            cout << "SIGINT Caught, stopping after current event" << endl;
            STOP_REQUESTED=true;
            });
    STOP_REQUESTED=false;


    bool quiet = options.Contains("quiet");

    int year = -1;
    TString extra("");
    if (options.Contains("Data2016")) {
        year = 2016;
        if (options.Contains("94x")) extra = "_94x";
    } else if (options.Contains("Data2017")) {
        year = 2017;
    } else if (options.Contains("Data2018")) {
        year = 2018;
    } else {
        cout << "Need to specify year!\n";
        assert(year > 0);
    }

    int year_for_output = year;
    if (options.Contains("FakeLumi2017")) {
        if (!quiet) std::cout << "Faking 2017 in the output name" << std::endl;
        year_for_output = 2017;
    } else if (options.Contains("FakeLumi2018")) {
        if (!quiet) std::cout << "Faking 2018 in the output name" << std::endl;
        year_for_output = 2018;
    }


    plots_t plots = run(ch, year, options);


    TString chainTitle = ch->GetTitle();
    TString outname = Form("%s/output_%d%s_%s.root", outputdir.Data(), year_for_output, extra.Data(), chainTitle.Data());

    if (!quiet) std::cout << "Writing out " << outname << std::endl;
    TFile *fout = new TFile(outname, "RECREATE");
    fout->cd();

    plots.SR.Write();
    plots.h_bjetpt.Write();
    plots.h_btagsf.Write();
    plots.h_charge.Write();
    plots.h_dphi.Write();
    plots.h_el_charge.Write();
    plots.h_mu_charge.Write();
    plots.h_q3.Write();
    plots.h_charge3.Write();
    plots.h_disc.Write();
    plots.h_el_l1eta.Write();
    plots.h_el_l1pt.Write();
    plots.h_el_l1phi.Write();
    plots.h_el_l2eta.Write();
    plots.h_el_l2pt.Write();
    plots.h_el_l2phi.Write();
    plots.h_el_l3eta.Write();
    plots.h_el_l3pt.Write();
    plots.h_el_lep1_miniIso.Write();
    plots.h_el_lep1_ptRatio.Write();
    plots.h_el_lep1_ptRel.Write();
    plots.h_el_lep1_ptRelfail.Write();
    plots.h_el_lep2_miniIso.Write();
    plots.h_el_lep2_ptRatio.Write();
    plots.h_el_lep2_ptRel.Write();
    plots.h_el_lep2_ptRelfail.Write();
    plots.h_el_lep3_miniIso.Write();
    plots.h_el_lep3_ptRatio.Write();
    plots.h_el_lep3_ptRel.Write();
    plots.h_el_lep3_ptRelfail.Write();
    plots.h_el_nmiss.Write();
    plots.h_el_sip3d_lep1.Write();
    plots.h_el_sip3d_lep2.Write();
    plots.h_el_sip3d_lep3.Write();
    plots.h_ht.Write();
    plots.h_isrw.Write();
    plots.h_jetpt.Write();
    plots.h_l1pt.Write();
    plots.h_l2pt.Write();
    plots.h_l3pt.Write();
    plots.h_lepsf.Write();
    plots.h_met.Write();
    plots.h_metnm1.Write();
    plots.h_mid1.Write();
    plots.h_mll.Write();
    plots.h_mllbig.Write();
    plots.h_mllos.Write();
    plots.h_mtmin.Write();
    plots.h_mtmax.Write();
    plots.h_mt2.Write();
    plots.h_mt2min.Write();
    plots.h_nleptonicW.Write();
    plots.h_mtop1.Write();
    plots.h_mtop2.Write();
    plots.h_mtvis.Write();
    plots.h_mu_l1eta.Write();
    plots.h_mu_l1pt.Write();
    plots.h_mu_l1phi.Write();
    plots.h_mu_l2eta.Write();
    plots.h_mu_l2pt.Write();
    plots.h_mu_l2phi.Write();
    plots.h_mu_l3eta.Write();
    plots.h_mu_l3pt.Write();
    plots.h_mu_lep1_miniIso.Write();
    plots.h_mu_lep1_ptRatio.Write();
    plots.h_mu_lep1_ptRel.Write();
    plots.h_mu_lep1_ptRelfail.Write();
    plots.h_mu_lep2_miniIso.Write();
    plots.h_mu_lep2_ptRatio.Write();
    plots.h_mu_lep2_ptRel.Write();
    plots.h_mu_lep2_ptRelfail.Write();
    plots.h_mu_lep3_miniIso.Write();
    plots.h_mu_lep3_ptRatio.Write();
    plots.h_mu_lep3_ptRel.Write();
    plots.h_mu_lep3_ptRelfail.Write();
    plots.h_mu_sip3d_lep1.Write();
    plots.h_mu_sip3d_lep2.Write();
    plots.h_mu_sip3d_lep3.Write();
    plots.h_mva.Write();
    plots.h_mvis.Write();
    plots.h_nbtags.Write();
    plots.h_njets.Write();
    plots.h_nleps.Write();
    plots.h_ntopness.Write();
    plots.h_ntops.Write();
    plots.h_nvtx.Write();
    plots.h_puw.Write();
    plots.h_topdisc1.Write();
    plots.h_topdisc2.Write();
    plots.h_trigsf.Write();
    plots.h_type.Write();
    plots.h_class.Write();
    plots.h_type3.Write();
    plots.h_category.Write();
    plots.h_lumiblock.Write();
    plots.h_run.Write();
    plots.h_wsf.Write();
    plots.h_dphil1l2.Write();
    plots.h_htb.Write();
    plots.h_nlb40.Write();
    plots.h_ntb40.Write();
    plots.h_detal1l2.Write();
    plots.h_maxmjoverpt.Write();
    plots.h_ml1j1.Write();
    plots.h_ptj1.Write();
    plots.h_ptj6.Write();
    plots.h_ptj7.Write();
    plots.h_ptj8.Write();
    plots.p_alphas_alt_dn_SR.Write();
    plots.p_alphas_alt_up_SR.Write();
    plots.p_bb_alt_dn_SR.Write();
    plots.p_bb_alt_up_SR.Write();
    plots.p_prefire_alt_dn_SR.Write();
    plots.p_prefire_alt_up_SR.Write();
    plots.p_trigger_alt_dn_SR.Write();
    plots.p_trigger_alt_up_SR.Write();
    plots.p_btagSF_alt_up_SR.Write();
    plots.p_btagSF_alt_dn_SR.Write();
    plots.p_btagSFheavy_alt_up_SR.Write();
    plots.p_btagSFheavy_alt_dn_SR.Write();
    plots.p_btagSFlight_alt_up_SR.Write();
    plots.p_btagSFlight_alt_dn_SR.Write();
    plots.p_fake_alt_up_SR.Write();
    plots.p_fake_nb0_up_SR.Write();
    plots.p_fake_nb1_up_SR.Write();
    plots.p_fake_nb2_up_SR.Write();
    plots.p_fake_nb3_up_SR.Write();
    plots.p_fake_unw_up_SR.Write();
    plots.p_fsrvar_alt_dn_SR.Write();
    plots.p_fsrvar_alt_up_SR.Write();
    plots.p_isr_alt_dn_SR.Write();
    plots.p_isr_alt_up_SR.Write();
    plots.p_isrvar_alt_dn_SR.Write();
    plots.p_isrvar_alt_up_SR.Write();
    plots.p_jer_alt_dn_SR.Write();
    plots.p_jer_alt_up_SR.Write();
    plots.p_jes_alt_dn_SR.Write();
    plots.p_jes_alt_up_SR.Write();
    plots.p_lep_alt_dn_SR.Write();
    plots.p_lep_alt_up_SR.Write();
    plots.p_met_alt_dn_SR.Write();
    plots.p_met_alt_up_SR.Write();
    plots.p_pdf_alt_dn_SR.Write();
    plots.p_pdf_alt_up_SR.Write();
    plots.p_pu_alt_dn_SR.Write();
    plots.p_pu_alt_up_SR.Write();
    plots.p_scale_alt_dn_SR.Write();
    plots.p_scale_alt_up_SR.Write();
    plots.p_unw_raw_SR.Write();
    plots.p_unw_sgn_SR.Write();

    // TNamed metadata(TString("metadata"),dir);
    // metadata.Write();

    // fout->Write();
    fout->Close();

    // std::cout <<  " plots.h_ht.sr->GetEntries(): " << plots.h_ht.sr->GetEntries() <<  std::endl;

    if (!quiet) std::cout << "Done writing out " << outname << std::endl;

    // // copy the root file with kinematic plots to live with the cards
    // gSystem->Exec(Form("cp histos.root ../limits/%s/", dir.Data()));


}

//doFakes = 1 for QCD, 2 for inSitu
plots_t run(TChain *chain, int year, TString options){

#ifdef SSLOOP
    ana_t analysis = SSANA;
    const bool isSS = true;
#else
    ana_t analysis = FTANA;
    const bool isSS = false;
#endif
    // bool isSS = analysis == SSANA;

    float lumiAG = getLumi(year);
    int nsr = getNsrs();
    int nCR = getNCRs();
    int nbdtbins = 17;

    int nHHsr = getNsrsHH();
    int nHLsr = getNsrsHL();
    int nLLsr = getNsrsLL();
    int nMLsr = getNsrsML();
    int nLMsr = getNsrsLM();
    int nMI1sr = getNsrsMI1();
    int nMI2sr = getNsrsMI2();
    int nINCLsr = getNsrsINCL();

    bool isData = 0;
    bool doFlips = 0;
    int doFakes = 0;
    int exclude = 0;
    bool isFastsim = 0;
    bool isRPV = 0;
    bool isRPVfullsim = 0;
    bool isGamma = 0;
    bool truthfake = 0;
    bool skipmatching = 0;
    bool ignoreFakeFactor = 0;

    bool evaluateBDT = options.Contains("evaluateBDT") && not options.Contains("noBDT");
    bool quiet = options.Contains("quiet");
    bool write_tree = options.Contains("writeTree");
    bool minPtFake18 = options.Contains("minPtFake18");
    bool new2016FRBins = options.Contains("new2016FRBins");
    bool doStitch = options.Contains("doStitch");
    if (options.Contains("BDT10")) { nbdtbins = 10; }
    if (options.Contains("BDT11")) { nbdtbins = 11; }
    if (options.Contains("BDT12")) { nbdtbins = 12; }
    if (options.Contains("BDT13")) { nbdtbins = 13; }
    if (options.Contains("BDT14")) { nbdtbins = 14; }
    if (options.Contains("BDT15")) { nbdtbins = 15; }
    if (options.Contains("BDT16")) { nbdtbins = 16; }
    if (options.Contains("BDT17")) { nbdtbins = 17; }
    if (options.Contains("BDT18")) { nbdtbins = 18; }
    if (options.Contains("BDT19")) { nbdtbins = 19; }
    if (options.Contains("BDT20")) { nbdtbins = 20; }

    // int nsrdisc = getNsrsDisc(nbdtbins); // this is supposed to be 1 more than nbdtbins (we add in CRZ as a "bin")
    int nsrdisc = nbdtbins+1; // this is supposed to be 1 more than nbdtbins (we add in CRZ as a "bin")

    float min_pt_fake = minPtFake18 ? 18. : -1;

    if (options.Contains("FakeLumi2017")) {
        if (!quiet) std::cout << "Faking 2017 luminosity" << std::endl;
        lumiAG = getLumi(2017);
    } else if (options.Contains("FakeLumi2018")) {
        if (!quiet) std::cout << "Faking 2018 luminosity" << std::endl;
        lumiAG = getLumi(2018);
    }

    bool partialUnblind = false;
    if (options.Contains("partialUnblind")) {
        set_goodrun_file("unblindjson/allsnt.txt");
        if (year == 2017 or options.Contains("FakeLumi2017")) {
            lumiAG = 10.;
        }
        if (year == 2018 or options.Contains("FakeLumi2018")) {
            lumiAG = 15.415;
        }
        partialUnblind = true;
    }

    // Note the blocks and else/ifs
    if (options.Contains("doData")) {
        if (!quiet) std::cout << "Doing data" << std::endl;
        isData = 1;
    }

    // --------
    if (options.Contains("doFakesMCUnw")) {
        if (!quiet) std::cout << "Doing fakesMC unweighted" << std::endl;
        doFakes = 1;
        ignoreFakeFactor = 1;
    }
    else if (options.Contains("doFakesUnw")) {
        if (!quiet) std::cout << "Doing fakes unweighted" << std::endl;
        isData = 1;
        doFakes = 1;
        ignoreFakeFactor = 1;
    }
    else if (options.Contains("doFakesMC")) {
        if (!quiet) std::cout << "Doing fakesMC" << std::endl;
        doFakes = 1;
    }
    else if (options.Contains("doFakes")) {
        if (!quiet) std::cout << "Doing fakes" << std::endl;
        isData = 1;
        doFakes = 1;
    }

    // --------
    if (options.Contains("doFlips")) {
        if (!quiet) std::cout << "Doing flips" << std::endl;
        isData = 1;
        doFlips = 1;
    }

    // --------
    if (options.Contains("doTruthFake")) {
        if (!quiet) std::cout << "Doing truthfake" << std::endl;
        truthfake = 1;
    }
    
    // --------
    if (options.Contains("doSkipMatching")) {
        if (!quiet) std::cout << "Skipping gen matching" << std::endl;
        skipmatching = 1;
    }

    // --------
    if (options.Contains("doXgamma")) {
        if (!quiet) std::cout << "Doing x+gamma" << std::endl;
        isGamma = 1;
    }

    if (!quiet) cout << "Running with lumi=" << lumiAG*scaleLumi << endl;

    // out_file->cd();


    TString chainTitle = chain->GetTitle();
    const char* chainTitleCh = chainTitle.Data();
    if (!quiet) std::cout << "Working on " << chainTitle << std::endl;

    plots_t plots;
    tree_t output_tree;

    if (write_tree) {
        if (!quiet) std::cout << "Initializing output tree" << std::endl;
        output_tree.Init(year, chainTitle);
    }


    if (chainTitle.Contains("rpv_")) {
        if (!quiet) std::cout << "RPV fullsim sample detected" << std::endl;
        isRPVfullsim = true;
    }

    if (chainTitle.Contains("t1qqqql")) {
        if (!quiet) std::cout << "RPV sample detected" << std::endl;
        isRPV = true;
    }

    if (chainTitle.Contains("fs_")) {
        if (!quiet) std::cout << "Fastsim sample detected" << std::endl;
        isFastsim = true;
        evaluateBDT = false;
    }

    bool isFakes = (chainTitle=="fakes") || (chainTitle=="fakes_mc");
    bool isFlips = (chainTitle=="flips");
    bool isRares = (chainTitle=="rares");
    bool istt = (chainTitle=="ttbar") || (chainTitle=="fakes_mc") || (chainTitle=="fakes_mc_unw");
    bool isttH = (chainTitle=="tth");
    bool istttt = (chainTitle=="tttt");
    bool isttVV = (chainTitle=="ttvv");
    bool isttW = (chainTitle=="ttw");
    bool isWW = (chainTitle=="ww");
    bool isttZ = (chainTitle=="ttz");
    bool isWZ = (chainTitle=="wz");
    bool isXgamma = (chainTitle=="xg");
    bool isT5qqqqWW = false;
    bool isT6ttHZ = false;
    bool isT6ttHZ_both = false;
    bool isT6ttHZ_z = false;
    bool isT6ttHZ_h = false;

    float invFilterEff = 1.;
    // float weightScale = 1.;
    if (chainTitle.Contains("fs_t5qqqqvv")) {
        // http://uaf-8.t2.ucsd.edu/~namin/dis/?query=%2FSMS-T5qqqqVV_TuneCUETP8M1_13TeV-madgraphMLM-pythia8%2FRunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1%2FMINIAODSIM&type=mcm&short=short
        // Do we need the line below still? I think that was for filtering ncharginos (W's, so we would write it as T5qqqqWW, but now we're ok with WW,WZ,ZZ (VV))
      // invFilterEff = 9./4.;
    }
    if (chainTitle.Contains("fs_t5qqqqvvdm20")) {
        // http://uaf-8.t2.ucsd.edu/~namin/dis/?query=%2FSMS-T5qqqqVV_dM20_TuneCUETP8M1_13TeV-madgraphMLM-pythia8%2FRunIISpring16MiniAODv2-PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1%2FMINIAODSIM&type=mcm&short=short
        // Do we need the line below still? I think that was for filtering ncharginos (W's, so we would write it as T5qqqqWW, but now we're ok with WW,WZ,ZZ (VV))
      // invFilterEff = 9./4.;
      invFilterEff *= 0.446; // efficiency of 1 lepton filter according to MCM for this sample
    }
    if (chainTitle.Contains("fs_t5qqqqww")) {
        isT5qqqqWW = true;
      invFilterEff = 9./4.;
    }
    if (chainTitle.Contains("fs_t5qqqqwwdm20")) {
        isT5qqqqWW = true;
      invFilterEff = 9./4.;
      invFilterEff *= 0.446; // efficiency of 1 lepton filter according to MCM for this sample
    }
    // BR to higgs is 0.6 (/SMS-T6ttHZ_BR-H_0p6_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM)
    if (chainTitle.Contains("fs_t6tthzbrb")) { // both (50-50)
        isT6ttHZ = true;
        isT6ttHZ_both = true;
      invFilterEff = 1/(2*0.4*0.6);
    }
    if (chainTitle.Contains("fs_t6tthzbrh")) { // higgs only
        isT6ttHZ = true;
        isT6ttHZ_h = true;
      invFilterEff = 1/(0.6*0.6);
    }
    if (chainTitle.Contains("fs_t6tthzbrz")) { // z only
        isT6ttHZ = true;
        isT6ttHZ_z = true;
      invFilterEff = 1/(0.4*0.4);
    }
    // if (chainTitle.Contains("rpv_")) {
    //     // FIXME this is just an adhoc scaling since I don't have xsecs
    //   weightScale = 0.000002;
    // }

    bool isHiggsScan = false;
    bool isHiggsPseudoscalar = false;
    bool isHiggsBoth = false;
    int higgs_mass = -1;
    float fastsim_isr_norm_central = 1.;
    float fastsim_isr_norm_up = 1.;
    float fastsim_isr_norm_down = 1.;
    vector<float> mysparms;
    if (chainTitle.Contains("higgsh")) {
        isHiggsScan = true;
        higgs_mass = chainTitle.Copy().ReplaceAll("higgsh","").Atoi();
        mysparms.push_back((float)higgs_mass);
    } else if (chainTitle.Contains("higgsa")) {
        isHiggsScan = true;
        isHiggsPseudoscalar = true;
        higgs_mass = chainTitle.Copy().ReplaceAll("higgsa","").Atoi();
        mysparms.push_back((float)higgs_mass);
    } else if (chainTitle.Contains("higgsb")) {
        isHiggsScan = true;
        isHiggsBoth = true;
        higgs_mass = chainTitle.Copy().ReplaceAll("higgsb","").Atoi();
        mysparms.push_back((float)higgs_mass);
    }
    if (isFastsim) {
        if ( chainTitle.Contains("higgs_scan") ||  chainTitle.Contains("higgs_ps_scan") 
                || chainTitle.Contains("tth_scan") || chainTitle.Contains("thq_scan") || chainTitle.Contains("thw_scan")
                || chainTitle.Contains("tta_scan") || chainTitle.Contains("taq_scan") || chainTitle.Contains("taw_scan")) {
            isHiggsScan = true;
            isHiggsPseudoscalar = chainTitle.Contains("tta_scan") || chainTitle.Contains("taq_scan") || chainTitle.Contains("taw_scan") || chainTitle.Contains("higgs_ps_scan");
        }
        TObjArray *tx = chainTitle.Tokenize("_");
        for (int i = 0; i < tx->GetEntries(); i++) {
            if (((TObjString *)(tx->At(i)))->String().BeginsWith("m")) {
                float asparm = ((TObjString *)(tx->At(i)))->String().ReplaceAll("m","").Atof();
                mysparms.push_back(asparm);
            }
        }
        if (!quiet) {
            std::cout << "Parsed " << mysparms.size() << " sparms:";
            if (mysparms.size() > 0) std::cout << " " << mysparms[0];
            if (mysparms.size() > 1) std::cout << " " << mysparms[1];
            std::cout << std::endl;
        }
        // if (year == 2016 and (mysparms.size() == 1 or isRPV)) {
        if (mysparms.size() == 1 or isRPV) {
            if (!quiet) {
                std::cout << "Calculating normalization factors for NISR jet reweighting..." << std::endl;
            }
            TH1D* htemp = new TH1D("htemp","htemp",1,0,2);
            int N = chain->Draw("1>>htemp",Form("(sparms[0] == %f)", mysparms[0]));
            htemp->Reset();
            chain->Draw("1>>htemp",Form("weight_isr * ((sparms[0] == %f))", mysparms[0]));
            fastsim_isr_norm_central = 1.0*N/htemp->GetBinContent(1); htemp->Reset();
            chain->Draw("1>>htemp",Form("weight_isr_UP * ((sparms[0] == %f))", mysparms[0]));
            fastsim_isr_norm_up = 1.0*N/htemp->GetBinContent(1); htemp->Reset();
            chain->Draw("1>>htemp",Form("weight_isr_DN * ((sparms[0] == %f))", mysparms[0]));
            fastsim_isr_norm_down = 1.0*N/htemp->GetBinContent(1); htemp->Reset();
            if (!quiet) {
                std::cout << "Calculated normalization factors for (central,up,down) ISR weights to be: " << fastsim_isr_norm_central << "," << fastsim_isr_norm_up << "," << fastsim_isr_norm_down << std::endl;
            }
        } else if (year == 2016 and (mysparms.size() == 2)) {
            if (!quiet) {
                std::cout << "Calculating normalization factors for NISR jet reweighting..." << std::endl;
            }
            TH1D* htemp = new TH1D("htemp","htemp",1,0,2);
            int N = chain->Draw("1>>htemp",Form("(sparms[0] == %f) && (sparms[1] == %f)", mysparms[0], mysparms[1]));
            htemp->Reset();
            chain->Draw("1>>htemp",Form("weight_isr * ((sparms[0] == %f) && (sparms[1] == %f))", mysparms[0], mysparms[1]));
            fastsim_isr_norm_central = 1.0*N/htemp->GetBinContent(1); htemp->Reset();
            chain->Draw("1>>htemp",Form("weight_isr_UP * ((sparms[0] == %f) && (sparms[1] == %f))", mysparms[0], mysparms[1]));
            fastsim_isr_norm_up = 1.0*N/htemp->GetBinContent(1); htemp->Reset();
            chain->Draw("1>>htemp",Form("weight_isr_DN * ((sparms[0] == %f) && (sparms[1] == %f))", mysparms[0], mysparms[1]));
            fastsim_isr_norm_down = 1.0*N/htemp->GetBinContent(1); htemp->Reset();
            if (!quiet) {
                std::cout << "Calculated normalization factors for (central,up,down) ISR weights to be: " << fastsim_isr_norm_central << "," << fastsim_isr_norm_up << "," << fastsim_isr_norm_down << std::endl;
            }
        }
    }

    plots_t  p_result;

    p_result.h_bjetpt.Init("bjetpt"     , chainTitle , 16 , 0.   , 200.);
    p_result.h_category.Init("category", chainTitle, 8 , 0.5 , 8.5);
    p_result.h_charge3.Init("charge3", chainTitle, 3 , -1 , 2);
    p_result.h_charge.Init("charge", chainTitle, 3 , -1 , 2);
    p_result.h_dphi.Init("dphi", chainTitle, 30 , -3.2 , 3.2);
    p_result.h_el_charge.Init("el_charge", chainTitle, 3 , -1 , 2);
    p_result.h_mu_charge.Init("mu_charge", chainTitle, 3 , -1 , 2);
    p_result.h_ht.Init("ht", chainTitle, 15, 100, 1600);
    p_result.h_isrw.Init("isrw"         , chainTitle , 50 , 0.7  , 1.3);
    p_result.h_jetpt.Init("jetpt"       , chainTitle , 20 , 0.   , 200.);
    p_result.h_l1pt.Init("l1pt"         , chainTitle , 15 , 0    , 150);
    p_result.h_l2pt.Init("l2pt"         , chainTitle , 15 , 0    , 150);
    p_result.h_l3pt.Init("l3pt"         , chainTitle , 7  , 0    , 140);
    p_result.h_met.Init("met", chainTitle, 16 , 0 , 400);
    p_result.h_metnm1.Init("metnm1"     , chainTitle , 40 , 0    , 400);
    p_result.h_mid1.Init("mid1"         , chainTitle , 5  , -2   , 3);
    p_result.h_mid2.Init("mid2"         , chainTitle , 5  , -2   , 3);
    p_result.h_mid3.Init("mid3"         , chainTitle , 5  , -2   , 3);
    p_result.h_mll.Init("mll", chainTitle, 20 , 0 , 200);
    p_result.h_mllbig.Init("mllbig", chainTitle, 20 , 0 , 400);
    p_result.h_mllos.Init("mllos", chainTitle, 20 , 0 , 200);
    p_result.h_mt2.Init("mt2", chainTitle, 10 , 0 , 200);
    p_result.h_mt2min.Init("mt2min", chainTitle, 10 , 0 , 200);
    p_result.h_mtmax.Init("mtmax", chainTitle, 75 , 0 , 200);
    p_result.h_mtmin.Init("mtmin"       , chainTitle , 10 , 0    , 200);
    p_result.h_mtop1.Init("mtop1"       , chainTitle , 15 , 100  , 250);
    p_result.h_mtop2.Init("mtop2"       , chainTitle , 15 , 100  , 250);
    p_result.h_mtvis.Init("mtvis"       , chainTitle , 20 , 300  , 2500);
    p_result.h_mva.Init("mva"           , chainTitle , 10 , 0    , 1.5);
    p_result.h_mvis.Init("mvis"         , chainTitle , 15 , 250  , 2750);
    // p_result.h_mvis.Init("mvis"         , chainTitle , 25 , 0    , 2500);
    p_result.h_nbtags.Init("nbtags"     , chainTitle , 7  , -0.5 , 6.5);
    p_result.h_njets.Init("njets"       , chainTitle , 10 , 1.5  , 11.5);
    p_result.h_nleps.Init("nleps"       , chainTitle , 6  , 0    , 6);
    p_result.h_nleptonicW.Init("nleptonicW", chainTitle, 5 , -0.5 , 4.5);
    p_result.h_ntopness.Init("ntopness" , chainTitle , 15 , 0.   , 0.75);
    p_result.h_ntops.Init("ntops"       , chainTitle , 5  , -0.5 , 4.5);
    p_result.h_q3.Init("q3", chainTitle, 3 , -1 , 2);
    p_result.h_nvtx.Init("nvtx"         , chainTitle , 20 , 0    , 80);
    p_result.h_puw.Init("puw"           , chainTitle , 50 , 0.7  , 1.3);
    p_result.h_topdisc1.Init("topdisc1" , chainTitle , 30 , -0.6 , 0.4);
    p_result.h_topdisc2.Init("topdisc2" , chainTitle , 30 , -0.6 , 0.4);
    p_result.h_type3.Init("type3", chainTitle, 4 , 0 , 4);
    p_result.h_type.Init("type", chainTitle, 4 , 0 , 4);
    p_result.h_class.Init("class", chainTitle, 6 , 0.5 , 6.5);

    p_result.h_el_l1eta.Init("el_l1eta"                   , chainTitle , 12 , -2.5 , 2.5);
    p_result.h_el_l1pt.Init("el_l1pt"                     , chainTitle , 15 , 0    , 150);
    p_result.h_el_l1phi.Init("el_l1phi"                     , chainTitle , 15 , -3.2    , 3.2);
    p_result.h_el_l2eta.Init("el_l2eta"                   , chainTitle , 12 , -2.5 , 2.5);
    p_result.h_el_l2pt.Init("el_l2pt"                     , chainTitle , 15 , 0    , 150);
    p_result.h_el_l2phi.Init("el_l2phi"                     , chainTitle , 15 , -3.2, 3.2);
    p_result.h_el_l3eta.Init("el_l3eta"                   , chainTitle , 8  , -2.5 , 2.5);
    p_result.h_el_l3pt.Init("el_l3pt"                     , chainTitle , 7  , 0    , 140);
    p_result.h_el_lep1_miniIso.Init("lep1_el_miniIso"     , chainTitle , 15 , 0    , 0.2);
    p_result.h_el_lep1_ptRatio.Init("lep1_el_ptRatio"     , chainTitle , 30 , -0.5 , 1.5);
    p_result.h_el_lep1_ptRelfail.Init("lep1_el_ptRelfail" , chainTitle , 15 , 0    , 50);
    p_result.h_el_lep1_ptRel.Init("lep1_el_ptRel"         , chainTitle , 15 , 0    , 50);
    p_result.h_el_lep2_miniIso.Init("lep2_el_miniIso"     , chainTitle , 15 , 0    , 0.2);
    p_result.h_el_lep2_ptRatio.Init("lep2_el_ptRatio"     , chainTitle , 30 , -0.5 , 1.5);
    p_result.h_el_lep2_ptRelfail.Init("lep2_el_ptRelfail" , chainTitle , 15 , 0    , 50);
    p_result.h_el_lep2_ptRel.Init("lep2_el_ptRel"         , chainTitle , 15 , 0    , 50);
    p_result.h_el_lep3_miniIso.Init("lep3_el_miniIso"     , chainTitle , 15 , 0    , 0.2);
    p_result.h_el_lep3_ptRatio.Init("lep3_el_ptRatio"     , chainTitle , 30 , -0.5 , 1.5);
    p_result.h_el_lep3_ptRelfail.Init("lep3_el_ptRelfail" , chainTitle , 15 , 0    , 50);
    p_result.h_el_lep3_ptRel.Init("lep3_el_ptRel"         , chainTitle , 15 , 0    , 50);
    p_result.h_el_nmiss.Init("el_nmiss"                   , chainTitle , 2  , 0    , 2);
    p_result.h_el_sip3d_lep1.Init("sip3d_el_lep1"         , chainTitle , 20 , 0    , 5);
    p_result.h_el_sip3d_lep2.Init("sip3d_el_lep2"         , chainTitle , 20 , 0    , 5);
    p_result.h_el_sip3d_lep3.Init("sip3d_el_lep3"         , chainTitle , 20 , 0    , 5);

    p_result.h_mu_l1pt.Init("mu_l1pt"                     , chainTitle , 15 , 0    , 150);
    p_result.h_mu_l1eta.Init("mu_l1eta"                   , chainTitle , 12 , -2.5 , 2.5);
    p_result.h_mu_l1phi.Init("mu_l1phi"                   , chainTitle , 15 , -3.2, 3.2);
    p_result.h_mu_l2pt.Init("mu_l2pt"                     , chainTitle , 15 , 0    , 150);
    p_result.h_mu_l2eta.Init("mu_l2eta"                   , chainTitle , 12 , -2.5 , 2.5);
    p_result.h_mu_l2phi.Init("mu_l2phi"                     , chainTitle , 15 , -3.2, 3.2);
    // p_result.h_mu_l2pt.Init("mu_l2pt"                     , chainTitle , 50 , 10    , 160);
    // p_result.h_mu_l2eta.Init("mu_l2eta"                   , chainTitle , 24 , -2.5 , 2.5);
    // p_result.h_mu_l2phi.Init("mu_l2phi"                     , chainTitle , 30 , -3.2, 3.2);
    p_result.h_mu_l3pt.Init("mu_l3pt"                     , chainTitle , 7  , 0    , 140);
    p_result.h_mu_l3eta.Init("mu_l3eta"                   , chainTitle , 8  , -2.5 , 2.5);
    p_result.h_mu_lep1_miniIso.Init("lep1_mu_miniIso"     , chainTitle , 15 , 0    , 0.2);
    p_result.h_mu_lep1_ptRatio.Init("lep1_mu_ptRatio"     , chainTitle , 30 , -0.5 , 1.5);
    p_result.h_mu_lep1_ptRelfail.Init("lep1_mu_ptRelfail" , chainTitle , 15 , 0    , 50);
    p_result.h_mu_lep1_ptRel.Init("lep1_mu_ptRel"         , chainTitle , 15 , 0    , 50);
    p_result.h_mu_lep2_miniIso.Init("lep2_mu_miniIso"     , chainTitle , 15 , 0    , 0.2);
    p_result.h_mu_lep2_ptRatio.Init("lep2_mu_ptRatio"     , chainTitle , 30 , -0.5 , 1.5);
    p_result.h_mu_lep2_ptRelfail.Init("lep2_mu_ptRelfail" , chainTitle , 15 , 0    , 50);
    p_result.h_mu_lep2_ptRel.Init("lep2_mu_ptRel"         , chainTitle , 15 , 0    , 50);
    p_result.h_mu_lep3_miniIso.Init("lep3_mu_miniIso"     , chainTitle , 15 , 0    , 0.2);
    p_result.h_mu_lep3_ptRatio.Init("lep3_mu_ptRatio"     , chainTitle , 30 , -0.5 , 1.5);
    p_result.h_mu_lep3_ptRelfail.Init("lep3_mu_ptRelfail" , chainTitle , 15 , 0    , 50);
    p_result.h_mu_lep3_ptRel.Init("lep3_mu_ptRel"         , chainTitle , 15 , 0    , 50);
    p_result.h_mu_sip3d_lep1.Init("sip3d_mu_lep1"         , chainTitle , 20 , 0    , 5);
    p_result.h_mu_sip3d_lep2.Init("sip3d_mu_lep2"         , chainTitle , 20 , 0    , 5);
    p_result.h_mu_sip3d_lep3.Init("sip3d_mu_lep3"         , chainTitle , 20 , 0    , 5);

    p_result.h_lumiblock.Init("lumiblock"         , chainTitle , 30 , 0    , 1500);
    p_result.h_run.Init("run"         , chainTitle , 54 , 272000 , 326000);

    p_result.h_disc.Init("disc"         , chainTitle , 20 , 0    , 1);

    p_result.h_wsf.Init("wsf"       , chainTitle , 50 , 0.7 , 1.3);
    p_result.h_btagsf.Init("btagsf" , chainTitle , 50 , 0.7 , 1.3);
    p_result.h_lepsf.Init("lepsf"   , chainTitle , 50 , 0.7 , 1.3);
    p_result.h_trigsf.Init("trigsf" , chainTitle , 50 , 0.7 , 1.3);

    p_result.h_detal1l2.Init("detal1l2"      , chainTitle, 30, -4  , 4 );
    p_result.h_dphil1l2.Init("dphil1l2"      , chainTitle, 15, 0   , 4 );
    p_result.h_htb.Init("htb"                , chainTitle, 16, 0   , 1600);
    p_result.h_maxmjoverpt.Init("maxmjoverpt", chainTitle, 50, 0   , 0.35);
    p_result.h_ml1j1.Init("ml1j1"            , chainTitle, 30, 0   , 1000 );
    p_result.h_nlb40.Init("nlb40"            , chainTitle, 5 , 0   , 5 );
    p_result.h_ntb40.Init("ntb40"            , chainTitle, 8 , 0   , 8 );
    p_result.h_ptj1.Init("ptj1"              , chainTitle, 50, 0   , 1000);
    p_result.h_ptj6.Init("ptj6"              , chainTitle, 30, 0   , 300 );
    p_result.h_ptj7.Init("ptj7"              , chainTitle, 30, 0   , 300 );
    p_result.h_ptj8.Init("ptj8"              , chainTitle, 30, 0   , 300 );

    // p_result.h_disc.br = new TH1F(Form("br_disc_%s"      , chainTitleCh) , Form("disc_%s"         , chainTitleCh) , 16      , -1.0 , 1.0);
    // p_result.h_disc.br    = new TH1F(Form("br_disc_%s"      , chainTitleCh) , Form("disc_%s"         , chainTitleCh) , 20      , 0.0  , 1.0);
    p_result.SR.SRCR      = new TH1F(Form("SRCR_TOTAL_%s"   , chainTitleCh) , Form("SRCR_TOTAL_%s"   , chainTitleCh) , nsr     , 0.5  , nsr+0.5); p_result.SR.SRCR->SetDirectory(0);
    p_result.SR.SRDISC    = new TH1F(Form("SRDISC_TOTAL_%s" , chainTitleCh) , Form("SRDISC_TOTAL_%s" , chainTitleCh) , nsrdisc , 0.5  , nsrdisc+0.5); p_result.SR.SRDISC->SetDirectory(0);
    p_result.SR.SR        = new TH1F(Form("SR_TOTAL_%s"     , chainTitleCh) , Form("SR_TOTAL_%s"     , chainTitleCh) , nsr-nCR , 0.5  , nsr-nCR+0.5); p_result.SR.SR->SetDirectory(0);
    p_result.SR.HH = new TH1F(Form("SRHH_TOTAL_%s" , chainTitleCh) , Form("SRHH_TOTAL_%s" , chainTitleCh) , nHHsr     , 0.5  , nHHsr+0.5); p_result.SR.HH->SetDirectory(0);
    p_result.SR.HL = new TH1F(Form("SRHL_TOTAL_%s" , chainTitleCh) , Form("SRHL_TOTAL_%s" , chainTitleCh) , nHLsr     , 0.5  , nHLsr+0.5); p_result.SR.HL->SetDirectory(0);
    p_result.SR.LL = new TH1F(Form("SRLL_TOTAL_%s" , chainTitleCh) , Form("SRLL_TOTAL_%s" , chainTitleCh) , nLLsr     , 0.5  , nLLsr+0.5); p_result.SR.LL->SetDirectory(0);
    p_result.SR.ML = new TH1F(Form("SRML_TOTAL_%s" , chainTitleCh) , Form("SRML_TOTAL_%s" , chainTitleCh) , nMLsr     , 0.5  , nMLsr+0.5); p_result.SR.ML->SetDirectory(0);
    p_result.SR.LM = new TH1F(Form("SRLM_TOTAL_%s" , chainTitleCh) , Form("SRLM_TOTAL_%s" , chainTitleCh) , nLMsr     , 0.5  , nLMsr+0.5); p_result.SR.LM->SetDirectory(0);
    p_result.SR.INCL = new TH1F(Form("SRINCL_TOTAL_%s" , chainTitleCh) , Form("SRINCL_TOTAL_%s" , chainTitleCh) , nINCLsr     , 0.5  , nINCLsr+0.5); p_result.SR.INCL->SetDirectory(0);
    p_result.SR.MI1 = new TH1F(Form("SRMI1_TOTAL_%s" , chainTitleCh) , Form("SRMI1_TOTAL_%s" , chainTitleCh) , nMI1sr     , 0.5  , nMI1sr+0.5); p_result.SR.MI1->SetDirectory(0);
    p_result.SR.MI2 = new TH1F(Form("SRMI2_TOTAL_%s" , chainTitleCh) , Form("SRMI2_TOTAL_%s" , chainTitleCh) , nMI2sr     , 0.5  , nMI2sr+0.5); p_result.SR.MI2->SetDirectory(0);

    //For FR variations
    if (doFakes == 1) {
        p_result.p_fake_alt_up_SR.SRCR        = new TH1F(Form("SRCR_FR_TOTAL_%s"      , chainTitleCh) , Form("SRCR_FR_TOTAL_%s"      , chainTitleCh) , nsr   , 0.5, nsr+0.5 ); p_result.p_fake_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_fake_alt_up_SR.SRDISC        = new TH1F(Form("SRDISC_FR_TOTAL_%s"      , chainTitleCh) , Form("SRDISC_FR_TOTAL_%s"      , chainTitleCh) , nsrdisc   , 0.5, nsrdisc+0.5 ); p_result.p_fake_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_fake_alt_up_SR.HH        = new TH1F(Form("SRHH_FR_TOTAL_%s"      , chainTitleCh) , Form("SRHH_FR_TOTAL_%s"      , chainTitleCh) , nHHsr   , 0.5, nHHsr+0.5 ); p_result.p_fake_alt_up_SR.HH->SetDirectory(0);
        p_result.p_fake_alt_up_SR.HL        = new TH1F(Form("SRHL_FR_TOTAL_%s"      , chainTitleCh) , Form("SRHL_FR_TOTAL_%s"      , chainTitleCh) , nHLsr   , 0.5, nHLsr+0.5 ); p_result.p_fake_alt_up_SR.HL->SetDirectory(0);
        p_result.p_fake_alt_up_SR.LL        = new TH1F(Form("SRLL_FR_TOTAL_%s"      , chainTitleCh) , Form("SRLL_FR_TOTAL_%s"      , chainTitleCh) , nLLsr   , 0.5, nLLsr+0.5 ); p_result.p_fake_alt_up_SR.LL->SetDirectory(0);
        p_result.p_fake_alt_up_SR.ML        = new TH1F(Form("SRML_FR_TOTAL_%s"      , chainTitleCh) , Form("SRML_FR_TOTAL_%s"      , chainTitleCh) , nMLsr   , 0.5, nMLsr+0.5 ); p_result.p_fake_alt_up_SR.ML->SetDirectory(0);
        p_result.p_fake_alt_up_SR.LM        = new TH1F(Form("SRLM_FR_TOTAL_%s"      , chainTitleCh) , Form("SRLM_FR_TOTAL_%s"      , chainTitleCh) , nLMsr   , 0.5, nLMsr+0.5 ); p_result.p_fake_alt_up_SR.LM->SetDirectory(0);
        p_result.p_fake_alt_up_SR.INCL        = new TH1F(Form("SRINCL_FR_TOTAL_%s"      , chainTitleCh) , Form("SRINCL_FR_TOTAL_%s"      , chainTitleCh) , nINCLsr   , 0.5, nINCLsr+0.5 ); p_result.p_fake_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_fake_alt_up_SR.MI1        = new TH1F(Form("SRMI1_FR_TOTAL_%s"      , chainTitleCh) , Form("SRMI1_FR_TOTAL_%s"      , chainTitleCh) , nMI1sr   , 0.5, nMI1sr+0.5 ); p_result.p_fake_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_fake_alt_up_SR.MI2        = new TH1F(Form("SRMI2_FR_TOTAL_%s"      , chainTitleCh) , Form("SRMI2_FR_TOTAL_%s"      , chainTitleCh) , nMI2sr   , 0.5, nMI2sr+0.5 ); p_result.p_fake_alt_up_SR.MI2->SetDirectory(0);
    }
    //For FR lnN1.3 separately for Nb0,1,2,3+
    if (doFakes == 1) {
        p_result.p_fake_nb0_up_SR.SRCR      = new TH1F(Form("SRCR_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , Form("SRCR_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , nsr     , 0.5, nsr+0.5 );     p_result.p_fake_nb0_up_SR.SRCR->SetDirectory(0);
        p_result.p_fake_nb0_up_SR.SRDISC    = new TH1F(Form("SRDISC_FRNORMNB0_TOTAL_%s"    , chainTitleCh) , Form("SRDISC_FRNORMNB0_TOTAL_%s"    , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 ); p_result.p_fake_nb0_up_SR.SRDISC->SetDirectory(0);
        p_result.p_fake_nb0_up_SR.HH        = new TH1F(Form("SRHH_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , Form("SRHH_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , nHHsr   , 0.5, nHHsr+0.5 );   p_result.p_fake_nb0_up_SR.HH->SetDirectory(0);
        p_result.p_fake_nb0_up_SR.HL        = new TH1F(Form("SRHL_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , Form("SRHL_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , nHLsr   , 0.5, nHLsr+0.5 );   p_result.p_fake_nb0_up_SR.HL->SetDirectory(0);
        p_result.p_fake_nb0_up_SR.LL        = new TH1F(Form("SRLL_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , Form("SRLL_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , nLLsr   , 0.5, nLLsr+0.5 );   p_result.p_fake_nb0_up_SR.LL->SetDirectory(0);
        p_result.p_fake_nb0_up_SR.ML        = new TH1F(Form("SRML_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , Form("SRML_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , nMLsr   , 0.5, nMLsr+0.5 );   p_result.p_fake_nb0_up_SR.ML->SetDirectory(0);
        p_result.p_fake_nb0_up_SR.LM        = new TH1F(Form("SRLM_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , Form("SRLM_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , nLMsr   , 0.5, nLMsr+0.5 );   p_result.p_fake_nb0_up_SR.LM->SetDirectory(0);
        p_result.p_fake_nb0_up_SR.INCL        = new TH1F(Form("SRINCL_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , Form("SRINCL_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , nINCLsr   , 0.5, nINCLsr+0.5 );   p_result.p_fake_nb0_up_SR.INCL->SetDirectory(0);
        p_result.p_fake_nb0_up_SR.MI1        = new TH1F(Form("SRMI1_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , Form("SRMI1_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , nMI1sr   , 0.5, nMI1sr+0.5 );   p_result.p_fake_nb0_up_SR.MI1->SetDirectory(0);
        p_result.p_fake_nb0_up_SR.MI2        = new TH1F(Form("SRMI2_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , Form("SRMI2_FRNORMNB0_TOTAL_%s"      , chainTitleCh) , nMI2sr   , 0.5, nMI2sr+0.5 );   p_result.p_fake_nb0_up_SR.MI2->SetDirectory(0);
        p_result.p_fake_nb1_up_SR.SRCR      = new TH1F(Form("SRCR_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , Form("SRCR_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , nsr     , 0.5, nsr+0.5 );     p_result.p_fake_nb1_up_SR.SRCR->SetDirectory(0);
        p_result.p_fake_nb1_up_SR.SRDISC    = new TH1F(Form("SRDISC_FRNORMNB1_TOTAL_%s"    , chainTitleCh) , Form("SRDISC_FRNORMNB1_TOTAL_%s"    , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 ); p_result.p_fake_nb1_up_SR.SRDISC->SetDirectory(0);
        p_result.p_fake_nb1_up_SR.HH        = new TH1F(Form("SRHH_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , Form("SRHH_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , nHHsr   , 0.5, nHHsr+0.5 );   p_result.p_fake_nb1_up_SR.HH->SetDirectory(0);
        p_result.p_fake_nb1_up_SR.HL        = new TH1F(Form("SRHL_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , Form("SRHL_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , nHLsr   , 0.5, nHLsr+0.5 );   p_result.p_fake_nb1_up_SR.HL->SetDirectory(0);
        p_result.p_fake_nb1_up_SR.LL        = new TH1F(Form("SRLL_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , Form("SRLL_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , nLLsr   , 0.5, nLLsr+0.5 );   p_result.p_fake_nb1_up_SR.LL->SetDirectory(0);
        p_result.p_fake_nb1_up_SR.ML        = new TH1F(Form("SRML_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , Form("SRML_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , nMLsr   , 0.5, nMLsr+0.5 );   p_result.p_fake_nb1_up_SR.ML->SetDirectory(0);
        p_result.p_fake_nb1_up_SR.LM        = new TH1F(Form("SRLM_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , Form("SRLM_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , nLMsr   , 0.5, nLMsr+0.5 );   p_result.p_fake_nb1_up_SR.LM->SetDirectory(0);
        p_result.p_fake_nb1_up_SR.INCL        = new TH1F(Form("SRINCL_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , Form("SRINCL_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , nINCLsr   , 0.5, nINCLsr+0.5 );   p_result.p_fake_nb1_up_SR.INCL->SetDirectory(0);
        p_result.p_fake_nb1_up_SR.MI1        = new TH1F(Form("SRMI1_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , Form("SRMI1_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , nMI1sr   , 0.5, nMI1sr+0.5 );   p_result.p_fake_nb1_up_SR.MI1->SetDirectory(0);
        p_result.p_fake_nb1_up_SR.MI2        = new TH1F(Form("SRMI2_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , Form("SRMI2_FRNORMNB1_TOTAL_%s"      , chainTitleCh) , nMI2sr   , 0.5, nMI2sr+0.5 );   p_result.p_fake_nb1_up_SR.MI2->SetDirectory(0);
        p_result.p_fake_nb2_up_SR.SRCR      = new TH1F(Form("SRCR_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , Form("SRCR_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , nsr     , 0.5, nsr+0.5 );     p_result.p_fake_nb2_up_SR.SRCR->SetDirectory(0);
        p_result.p_fake_nb2_up_SR.SRDISC    = new TH1F(Form("SRDISC_FRNORMNB2_TOTAL_%s"    , chainTitleCh) , Form("SRDISC_FRNORMNB2_TOTAL_%s"    , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 ); p_result.p_fake_nb2_up_SR.SRDISC->SetDirectory(0);
        p_result.p_fake_nb2_up_SR.HH        = new TH1F(Form("SRHH_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , Form("SRHH_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , nHHsr   , 0.5, nHHsr+0.5 );   p_result.p_fake_nb2_up_SR.HH->SetDirectory(0);
        p_result.p_fake_nb2_up_SR.HL        = new TH1F(Form("SRHL_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , Form("SRHL_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , nHLsr   , 0.5, nHLsr+0.5 );   p_result.p_fake_nb2_up_SR.HL->SetDirectory(0);
        p_result.p_fake_nb2_up_SR.LL        = new TH1F(Form("SRLL_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , Form("SRLL_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , nLLsr   , 0.5, nLLsr+0.5 );   p_result.p_fake_nb2_up_SR.LL->SetDirectory(0);
        p_result.p_fake_nb2_up_SR.ML        = new TH1F(Form("SRML_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , Form("SRML_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , nMLsr   , 0.5, nMLsr+0.5 );   p_result.p_fake_nb2_up_SR.ML->SetDirectory(0);
        p_result.p_fake_nb2_up_SR.LM        = new TH1F(Form("SRLM_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , Form("SRLM_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , nLMsr   , 0.5, nLMsr+0.5 );   p_result.p_fake_nb2_up_SR.LM->SetDirectory(0);
        p_result.p_fake_nb2_up_SR.INCL        = new TH1F(Form("SRINCL_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , Form("SRINCL_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , nINCLsr   , 0.5, nINCLsr+0.5 );   p_result.p_fake_nb2_up_SR.INCL->SetDirectory(0);
        p_result.p_fake_nb2_up_SR.MI1        = new TH1F(Form("SRMI1_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , Form("SRMI1_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , nMI1sr   , 0.5, nMI1sr+0.5 );   p_result.p_fake_nb2_up_SR.MI1->SetDirectory(0);
        p_result.p_fake_nb2_up_SR.MI2        = new TH1F(Form("SRMI2_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , Form("SRMI2_FRNORMNB2_TOTAL_%s"      , chainTitleCh) , nMI2sr   , 0.5, nMI2sr+0.5 );   p_result.p_fake_nb2_up_SR.MI2->SetDirectory(0);
        p_result.p_fake_nb3_up_SR.SRCR      = new TH1F(Form("SRCR_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , Form("SRCR_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , nsr     , 0.5, nsr+0.5 );     p_result.p_fake_nb3_up_SR.SRCR->SetDirectory(0);
        p_result.p_fake_nb3_up_SR.SRDISC    = new TH1F(Form("SRDISC_FRNORMNB3_TOTAL_%s"    , chainTitleCh) , Form("SRDISC_FRNORMNB3_TOTAL_%s"    , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 ); p_result.p_fake_nb3_up_SR.SRDISC->SetDirectory(0);
        p_result.p_fake_nb3_up_SR.HH        = new TH1F(Form("SRHH_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , Form("SRHH_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , nHHsr   , 0.5, nHHsr+0.5 );   p_result.p_fake_nb3_up_SR.HH->SetDirectory(0);
        p_result.p_fake_nb3_up_SR.HL        = new TH1F(Form("SRHL_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , Form("SRHL_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , nHLsr   , 0.5, nHLsr+0.5 );   p_result.p_fake_nb3_up_SR.HL->SetDirectory(0);
        p_result.p_fake_nb3_up_SR.LL        = new TH1F(Form("SRLL_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , Form("SRLL_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , nLLsr   , 0.5, nLLsr+0.5 );   p_result.p_fake_nb3_up_SR.LL->SetDirectory(0);
        p_result.p_fake_nb3_up_SR.ML        = new TH1F(Form("SRML_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , Form("SRML_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , nMLsr   , 0.5, nMLsr+0.5 );   p_result.p_fake_nb3_up_SR.ML->SetDirectory(0);
        p_result.p_fake_nb3_up_SR.LM        = new TH1F(Form("SRLM_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , Form("SRLM_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , nLMsr   , 0.5, nLMsr+0.5 );   p_result.p_fake_nb3_up_SR.LM->SetDirectory(0);
        p_result.p_fake_nb3_up_SR.INCL        = new TH1F(Form("SRINCL_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , Form("SRINCL_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , nINCLsr   , 0.5, nINCLsr+0.5 );   p_result.p_fake_nb3_up_SR.INCL->SetDirectory(0);
        p_result.p_fake_nb3_up_SR.MI1        = new TH1F(Form("SRMI1_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , Form("SRMI1_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , nMI1sr   , 0.5, nMI1sr+0.5 );   p_result.p_fake_nb3_up_SR.MI1->SetDirectory(0);
        p_result.p_fake_nb3_up_SR.MI2        = new TH1F(Form("SRMI2_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , Form("SRMI2_FRNORMNB3_TOTAL_%s"      , chainTitleCh) , nMI2sr   , 0.5, nMI2sr+0.5 );   p_result.p_fake_nb3_up_SR.MI2->SetDirectory(0);
    }
    //For unw FR application counts
    if (doFakes == 1) {
        p_result.p_fake_unw_up_SR.SRCR        = new TH1F(Form("SRCR_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRCR_UNWFR_TOTAL_%s"      , chainTitleCh) , nsr   , 0.5, nsr+0.5 ); p_result.p_fake_unw_up_SR.SRCR->SetDirectory(0);
        p_result.p_fake_unw_up_SR.SRDISC        = new TH1F(Form("SRDISC_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRDISC_UNWFR_TOTAL_%s"      , chainTitleCh) , nsrdisc   , 0.5, nsrdisc+0.5 ); p_result.p_fake_unw_up_SR.SRDISC->SetDirectory(0);
        p_result.p_fake_unw_up_SR.HH        = new TH1F(Form("SRHH_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRHH_UNWFR_TOTAL_%s"      , chainTitleCh) , nHHsr   , 0.5, nHHsr+0.5 ); p_result.p_fake_unw_up_SR.HH->SetDirectory(0);
        p_result.p_fake_unw_up_SR.HL        = new TH1F(Form("SRHL_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRHL_UNWFR_TOTAL_%s"      , chainTitleCh) , nHLsr   , 0.5, nHLsr+0.5 ); p_result.p_fake_unw_up_SR.HL->SetDirectory(0);
        p_result.p_fake_unw_up_SR.LL        = new TH1F(Form("SRLL_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRLL_UNWFR_TOTAL_%s"      , chainTitleCh) , nLLsr   , 0.5, nLLsr+0.5 ); p_result.p_fake_unw_up_SR.LL->SetDirectory(0);
        p_result.p_fake_unw_up_SR.ML        = new TH1F(Form("SRML_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRML_UNWFR_TOTAL_%s"      , chainTitleCh) , nMLsr   , 0.5, nMLsr+0.5 ); p_result.p_fake_unw_up_SR.ML->SetDirectory(0);
        p_result.p_fake_unw_up_SR.LM        = new TH1F(Form("SRLM_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRLM_UNWFR_TOTAL_%s"      , chainTitleCh) , nLMsr   , 0.5, nLMsr+0.5 ); p_result.p_fake_unw_up_SR.LM->SetDirectory(0);
        p_result.p_fake_unw_up_SR.INCL        = new TH1F(Form("SRINCL_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRINCL_UNWFR_TOTAL_%s"      , chainTitleCh) , nINCLsr   , 0.5, nINCLsr+0.5 ); p_result.p_fake_unw_up_SR.INCL->SetDirectory(0);
        p_result.p_fake_unw_up_SR.MI1        = new TH1F(Form("SRMI1_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRMI1_UNWFR_TOTAL_%s"      , chainTitleCh) , nMI1sr   , 0.5, nMI1sr+0.5 ); p_result.p_fake_unw_up_SR.MI1->SetDirectory(0);
        p_result.p_fake_unw_up_SR.MI2        = new TH1F(Form("SRMI2_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRMI2_UNWFR_TOTAL_%s"      , chainTitleCh) , nMI2sr   , 0.5, nMI2sr+0.5 ); p_result.p_fake_unw_up_SR.MI2->SetDirectory(0);
    }
    //For JES variations
    if (doFakes == 1 || isData==0) {
        p_result.p_jes_alt_up_SR.SRCR     = new TH1F(Form("SRCR_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JES_UP_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 ); p_result.p_jes_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JES_DN_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 ); p_result.p_jes_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_jes_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JES_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 ); p_result.p_jes_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JES_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 ); p_result.p_jes_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_jes_alt_up_SR.HH     = new TH1F(Form("SRHH_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_JES_UP_TOTAL_%s"   , chainTitleCh) , nHHsr , 0.5, nHHsr+0.5 ); p_result.p_jes_alt_up_SR.HH->SetDirectory(0);
        p_result.p_jes_alt_up_SR.HL     = new TH1F(Form("SRHL_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_JES_UP_TOTAL_%s"   , chainTitleCh) , nHLsr , 0.5, nHLsr+0.5 ); p_result.p_jes_alt_up_SR.HL->SetDirectory(0);
        p_result.p_jes_alt_up_SR.LL     = new TH1F(Form("SRLL_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_JES_UP_TOTAL_%s"   , chainTitleCh) , nLLsr , 0.5, nLLsr+0.5 ); p_result.p_jes_alt_up_SR.LL->SetDirectory(0);
        p_result.p_jes_alt_up_SR.ML     = new TH1F(Form("SRML_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_JES_UP_TOTAL_%s"   , chainTitleCh) , nMLsr , 0.5, nMLsr+0.5 ); p_result.p_jes_alt_up_SR.ML->SetDirectory(0);
        p_result.p_jes_alt_up_SR.LM     = new TH1F(Form("SRLM_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_JES_UP_TOTAL_%s"   , chainTitleCh) , nLMsr , 0.5, nLMsr+0.5 ); p_result.p_jes_alt_up_SR.LM->SetDirectory(0);
        p_result.p_jes_alt_up_SR.INCL     = new TH1F(Form("SRINCL_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_JES_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr , 0.5, nINCLsr+0.5 ); p_result.p_jes_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_jes_alt_up_SR.MI1     = new TH1F(Form("SRMI1_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_JES_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr , 0.5, nMI1sr+0.5 ); p_result.p_jes_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_jes_alt_up_SR.MI2     = new TH1F(Form("SRMI2_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_JES_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr , 0.5, nMI2sr+0.5 ); p_result.p_jes_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.HH     = new TH1F(Form("SRHH_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_JES_DN_TOTAL_%s"   , chainTitleCh) , nHHsr , 0.5, nHHsr+0.5 ); p_result.p_jes_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.HL     = new TH1F(Form("SRHL_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_JES_DN_TOTAL_%s"   , chainTitleCh) , nHLsr , 0.5, nHLsr+0.5 ); p_result.p_jes_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.LL     = new TH1F(Form("SRLL_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_JES_DN_TOTAL_%s"   , chainTitleCh) , nLLsr , 0.5, nLLsr+0.5 ); p_result.p_jes_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.ML     = new TH1F(Form("SRML_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_JES_DN_TOTAL_%s"   , chainTitleCh) , nMLsr , 0.5, nMLsr+0.5 ); p_result.p_jes_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.LM     = new TH1F(Form("SRLM_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_JES_DN_TOTAL_%s"   , chainTitleCh) , nLMsr , 0.5, nLMsr+0.5 ); p_result.p_jes_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_JES_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr , 0.5, nINCLsr+0.5 ); p_result.p_jes_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_JES_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr , 0.5, nMI1sr+0.5 ); p_result.p_jes_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_JES_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr , 0.5, nMI2sr+0.5 ); p_result.p_jes_alt_dn_SR.MI2->SetDirectory(0);
    }
    //For JER variations
    if (doFakes == 1 || isData==0) {
        p_result.p_jer_alt_up_SR.SRCR     = new TH1F(Form("SRCR_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JER_UP_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 ); p_result.p_jer_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JER_DN_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 ); p_result.p_jer_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_jer_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JER_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 ); p_result.p_jer_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JER_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 ); p_result.p_jer_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_jer_alt_up_SR.HH     = new TH1F(Form("SRHH_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_JER_UP_TOTAL_%s"   , chainTitleCh) , nHHsr , 0.5, nHHsr+0.5 ); p_result.p_jer_alt_up_SR.HH->SetDirectory(0);
        p_result.p_jer_alt_up_SR.HL     = new TH1F(Form("SRHL_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_JER_UP_TOTAL_%s"   , chainTitleCh) , nHLsr , 0.5, nHLsr+0.5 ); p_result.p_jer_alt_up_SR.HL->SetDirectory(0);
        p_result.p_jer_alt_up_SR.LL     = new TH1F(Form("SRLL_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_JER_UP_TOTAL_%s"   , chainTitleCh) , nLLsr , 0.5, nLLsr+0.5 ); p_result.p_jer_alt_up_SR.LL->SetDirectory(0);
        p_result.p_jer_alt_up_SR.ML     = new TH1F(Form("SRML_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_JER_UP_TOTAL_%s"   , chainTitleCh) , nMLsr , 0.5, nMLsr+0.5 ); p_result.p_jer_alt_up_SR.ML->SetDirectory(0);
        p_result.p_jer_alt_up_SR.LM     = new TH1F(Form("SRLM_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_JER_UP_TOTAL_%s"   , chainTitleCh) , nLMsr , 0.5, nLMsr+0.5 ); p_result.p_jer_alt_up_SR.LM->SetDirectory(0);
        p_result.p_jer_alt_up_SR.INCL     = new TH1F(Form("SRINCL_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_JER_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr , 0.5, nINCLsr+0.5 ); p_result.p_jer_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_jer_alt_up_SR.MI1     = new TH1F(Form("SRMI1_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_JER_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr , 0.5, nMI1sr+0.5 ); p_result.p_jer_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_jer_alt_up_SR.MI2     = new TH1F(Form("SRMI2_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_JER_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr , 0.5, nMI2sr+0.5 ); p_result.p_jer_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.HH     = new TH1F(Form("SRHH_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_JER_DN_TOTAL_%s"   , chainTitleCh) , nHHsr , 0.5, nHHsr+0.5 ); p_result.p_jer_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.HL     = new TH1F(Form("SRHL_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_JER_DN_TOTAL_%s"   , chainTitleCh) , nHLsr , 0.5, nHLsr+0.5 ); p_result.p_jer_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.LL     = new TH1F(Form("SRLL_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_JER_DN_TOTAL_%s"   , chainTitleCh) , nLLsr , 0.5, nLLsr+0.5 ); p_result.p_jer_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.ML     = new TH1F(Form("SRML_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_JER_DN_TOTAL_%s"   , chainTitleCh) , nMLsr , 0.5, nMLsr+0.5 ); p_result.p_jer_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.LM     = new TH1F(Form("SRLM_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_JER_DN_TOTAL_%s"   , chainTitleCh) , nLMsr , 0.5, nLMsr+0.5 ); p_result.p_jer_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_JER_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr , 0.5, nINCLsr+0.5 ); p_result.p_jer_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_JER_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr , 0.5, nMI1sr+0.5 ); p_result.p_jer_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_JER_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr , 0.5, nMI2sr+0.5 ); p_result.p_jer_alt_dn_SR.MI2->SetDirectory(0);
    }
    //For btag SF variations
    if (isData==0){
        p_result.p_btagSF_alt_up_SR.SRCR = new TH1F(Form("SRCR_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRCR_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nsr,  0.5, nsr+0.5 ); p_result.p_btagSF_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.SRCR = new TH1F(Form("SRCR_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRCR_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nsr,  0.5, nsr+0.5 ); p_result.p_btagSF_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_btagSF_alt_up_SR.SRDISC = new TH1F(Form("SRDISC_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRDISC_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_btagSF_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.SRDISC = new TH1F(Form("SRDISC_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRDISC_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_btagSF_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_btagSF_alt_up_SR.HH = new TH1F(Form("SRHH_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRHH_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nHHsr,  0.5, nHHsr+0.5 ); p_result.p_btagSF_alt_up_SR.HH->SetDirectory(0);
        p_result.p_btagSF_alt_up_SR.HL = new TH1F(Form("SRHL_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRHL_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nHLsr,  0.5, nHLsr+0.5 ); p_result.p_btagSF_alt_up_SR.HL->SetDirectory(0);
        p_result.p_btagSF_alt_up_SR.LL = new TH1F(Form("SRLL_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRLL_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nLLsr,  0.5, nLLsr+0.5 ); p_result.p_btagSF_alt_up_SR.LL->SetDirectory(0);
        p_result.p_btagSF_alt_up_SR.ML = new TH1F(Form("SRML_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRML_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nMLsr,  0.5, nMLsr+0.5 ); p_result.p_btagSF_alt_up_SR.ML->SetDirectory(0);
        p_result.p_btagSF_alt_up_SR.LM = new TH1F(Form("SRLM_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRLM_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nLMsr,  0.5, nLMsr+0.5 ); p_result.p_btagSF_alt_up_SR.LM->SetDirectory(0);
        p_result.p_btagSF_alt_up_SR.INCL = new TH1F(Form("SRINCL_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRINCL_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_btagSF_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_btagSF_alt_up_SR.MI1 = new TH1F(Form("SRMI1_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRMI1_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_btagSF_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_btagSF_alt_up_SR.MI2 = new TH1F(Form("SRMI2_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRMI2_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_btagSF_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.HH = new TH1F(Form("SRHH_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRHH_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nHHsr,  0.5, nHHsr+0.5 ); p_result.p_btagSF_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.HL = new TH1F(Form("SRHL_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRHL_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nHLsr,  0.5, nHLsr+0.5 ); p_result.p_btagSF_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.LL = new TH1F(Form("SRLL_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRLL_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nLLsr,  0.5, nLLsr+0.5 ); p_result.p_btagSF_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.ML = new TH1F(Form("SRML_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRML_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nMLsr,  0.5, nMLsr+0.5 ); p_result.p_btagSF_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.LM = new TH1F(Form("SRLM_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRLM_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nLMsr,  0.5, nLMsr+0.5 ); p_result.p_btagSF_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.INCL = new TH1F(Form("SRINCL_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRINCL_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_btagSF_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.MI1 = new TH1F(Form("SRMI1_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRMI1_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_btagSF_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.MI2 = new TH1F(Form("SRMI2_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRMI2_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_btagSF_alt_dn_SR.MI2->SetDirectory(0);

        p_result.p_btagSFheavy_alt_up_SR.SRCR = new TH1F(Form("SRCR_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh), Form("SRCR_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh),  nsr,  0.5, nsr+0.5 ); p_result.p_btagSFheavy_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_btagSFheavy_alt_dn_SR.SRCR = new TH1F(Form("SRCR_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh), Form("SRCR_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh),  nsr,  0.5, nsr+0.5 ); p_result.p_btagSFheavy_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_btagSFheavy_alt_up_SR.SRDISC = new TH1F(Form("SRDISC_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh), Form("SRDISC_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh),  nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_btagSFheavy_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_btagSFheavy_alt_dn_SR.SRDISC = new TH1F(Form("SRDISC_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh), Form("SRDISC_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh),  nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_btagSFheavy_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_btagSFheavy_alt_up_SR.HH = new TH1F(Form("SRHH_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh), Form("SRHH_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh),  nHHsr,  0.5, nHHsr+0.5 ); p_result.p_btagSFheavy_alt_up_SR.HH->SetDirectory(0);
        p_result.p_btagSFheavy_alt_up_SR.HL = new TH1F(Form("SRHL_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh), Form("SRHL_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh),  nHLsr,  0.5, nHLsr+0.5 ); p_result.p_btagSFheavy_alt_up_SR.HL->SetDirectory(0);
        p_result.p_btagSFheavy_alt_up_SR.LL = new TH1F(Form("SRLL_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh), Form("SRLL_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh),  nLLsr,  0.5, nLLsr+0.5 ); p_result.p_btagSFheavy_alt_up_SR.LL->SetDirectory(0);
        p_result.p_btagSFheavy_alt_up_SR.ML = new TH1F(Form("SRML_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh), Form("SRML_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh),  nMLsr,  0.5, nMLsr+0.5 ); p_result.p_btagSFheavy_alt_up_SR.ML->SetDirectory(0);
        p_result.p_btagSFheavy_alt_up_SR.LM = new TH1F(Form("SRLM_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh), Form("SRLM_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh),  nLMsr,  0.5, nLMsr+0.5 ); p_result.p_btagSFheavy_alt_up_SR.LM->SetDirectory(0);
        p_result.p_btagSFheavy_alt_up_SR.INCL = new TH1F(Form("SRINCL_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh), Form("SRINCL_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh),  nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_btagSFheavy_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_btagSFheavy_alt_up_SR.MI1 = new TH1F(Form("SRMI1_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh), Form("SRMI1_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh),  nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_btagSFheavy_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_btagSFheavy_alt_up_SR.MI2 = new TH1F(Form("SRMI2_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh), Form("SRMI2_BTAGSFHEAVY_UP_TOTAL_%s", chainTitleCh),  nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_btagSFheavy_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_btagSFheavy_alt_dn_SR.HH = new TH1F(Form("SRHH_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh), Form("SRHH_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh),  nHHsr,  0.5, nHHsr+0.5 ); p_result.p_btagSFheavy_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_btagSFheavy_alt_dn_SR.HL = new TH1F(Form("SRHL_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh), Form("SRHL_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh),  nHLsr,  0.5, nHLsr+0.5 ); p_result.p_btagSFheavy_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_btagSFheavy_alt_dn_SR.LL = new TH1F(Form("SRLL_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh), Form("SRLL_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh),  nLLsr,  0.5, nLLsr+0.5 ); p_result.p_btagSFheavy_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_btagSFheavy_alt_dn_SR.ML = new TH1F(Form("SRML_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh), Form("SRML_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh),  nMLsr,  0.5, nMLsr+0.5 ); p_result.p_btagSFheavy_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_btagSFheavy_alt_dn_SR.LM = new TH1F(Form("SRLM_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh), Form("SRLM_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh),  nLMsr,  0.5, nLMsr+0.5 ); p_result.p_btagSFheavy_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_btagSFheavy_alt_dn_SR.INCL = new TH1F(Form("SRINCL_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh), Form("SRINCL_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh),  nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_btagSFheavy_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_btagSFheavy_alt_dn_SR.MI1 = new TH1F(Form("SRMI1_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh), Form("SRMI1_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh),  nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_btagSFheavy_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_btagSFheavy_alt_dn_SR.MI2 = new TH1F(Form("SRMI2_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh), Form("SRMI2_BTAGSFHEAVY_DN_TOTAL_%s", chainTitleCh),  nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_btagSFheavy_alt_dn_SR.MI2->SetDirectory(0);

        p_result.p_btagSFlight_alt_up_SR.SRCR = new TH1F(Form("SRCR_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh), Form("SRCR_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh),  nsr,  0.5, nsr+0.5 ); p_result.p_btagSFlight_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_btagSFlight_alt_dn_SR.SRCR = new TH1F(Form("SRCR_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh), Form("SRCR_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh),  nsr,  0.5, nsr+0.5 ); p_result.p_btagSFlight_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_btagSFlight_alt_up_SR.SRDISC = new TH1F(Form("SRDISC_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh), Form("SRDISC_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh),  nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_btagSFlight_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_btagSFlight_alt_dn_SR.SRDISC = new TH1F(Form("SRDISC_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh), Form("SRDISC_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh),  nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_btagSFlight_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_btagSFlight_alt_up_SR.HH = new TH1F(Form("SRHH_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh), Form("SRHH_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh),  nHHsr,  0.5, nHHsr+0.5 ); p_result.p_btagSFlight_alt_up_SR.HH->SetDirectory(0);
        p_result.p_btagSFlight_alt_up_SR.HL = new TH1F(Form("SRHL_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh), Form("SRHL_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh),  nHLsr,  0.5, nHLsr+0.5 ); p_result.p_btagSFlight_alt_up_SR.HL->SetDirectory(0);
        p_result.p_btagSFlight_alt_up_SR.LL = new TH1F(Form("SRLL_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh), Form("SRLL_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh),  nLLsr,  0.5, nLLsr+0.5 ); p_result.p_btagSFlight_alt_up_SR.LL->SetDirectory(0);
        p_result.p_btagSFlight_alt_up_SR.ML = new TH1F(Form("SRML_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh), Form("SRML_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh),  nMLsr,  0.5, nMLsr+0.5 ); p_result.p_btagSFlight_alt_up_SR.ML->SetDirectory(0);
        p_result.p_btagSFlight_alt_up_SR.LM = new TH1F(Form("SRLM_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh), Form("SRLM_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh),  nLMsr,  0.5, nLMsr+0.5 ); p_result.p_btagSFlight_alt_up_SR.LM->SetDirectory(0);
        p_result.p_btagSFlight_alt_up_SR.INCL = new TH1F(Form("SRINCL_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh), Form("SRINCL_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh),  nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_btagSFlight_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_btagSFlight_alt_up_SR.MI1 = new TH1F(Form("SRMI1_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh), Form("SRMI1_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh),  nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_btagSFlight_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_btagSFlight_alt_up_SR.MI2 = new TH1F(Form("SRMI2_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh), Form("SRMI2_BTAGSFLIGHT_UP_TOTAL_%s", chainTitleCh),  nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_btagSFlight_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_btagSFlight_alt_dn_SR.HH = new TH1F(Form("SRHH_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh), Form("SRHH_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh),  nHHsr,  0.5, nHHsr+0.5 ); p_result.p_btagSFlight_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_btagSFlight_alt_dn_SR.HL = new TH1F(Form("SRHL_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh), Form("SRHL_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh),  nHLsr,  0.5, nHLsr+0.5 ); p_result.p_btagSFlight_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_btagSFlight_alt_dn_SR.LL = new TH1F(Form("SRLL_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh), Form("SRLL_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh),  nLLsr,  0.5, nLLsr+0.5 ); p_result.p_btagSFlight_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_btagSFlight_alt_dn_SR.ML = new TH1F(Form("SRML_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh), Form("SRML_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh),  nMLsr,  0.5, nMLsr+0.5 ); p_result.p_btagSFlight_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_btagSFlight_alt_dn_SR.LM = new TH1F(Form("SRLM_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh), Form("SRLM_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh),  nLMsr,  0.5, nLMsr+0.5 ); p_result.p_btagSFlight_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_btagSFlight_alt_dn_SR.INCL = new TH1F(Form("SRINCL_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh), Form("SRINCL_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh),  nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_btagSFlight_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_btagSFlight_alt_dn_SR.MI1 = new TH1F(Form("SRMI1_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh), Form("SRMI1_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh),  nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_btagSFlight_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_btagSFlight_alt_dn_SR.MI2 = new TH1F(Form("SRMI2_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh), Form("SRMI2_BTAGSFLIGHT_DN_TOTAL_%s", chainTitleCh),  nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_btagSFlight_alt_dn_SR.MI2->SetDirectory(0);

    }
    //For PU variations
    if (doFakes == 1 || isData==0) {
        p_result.p_pu_alt_up_SR.SRCR     = new TH1F(Form("SRCR_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PU_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_pu_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PU_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_pu_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_pu_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PU_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_pu_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PU_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_pu_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_pu_alt_up_SR.HH     = new TH1F(Form("SRHH_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_PU_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_pu_alt_up_SR.HH->SetDirectory(0);
        p_result.p_pu_alt_up_SR.HL     = new TH1F(Form("SRHL_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_PU_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_pu_alt_up_SR.HL->SetDirectory(0);
        p_result.p_pu_alt_up_SR.LL     = new TH1F(Form("SRLL_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_PU_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_pu_alt_up_SR.LL->SetDirectory(0);
        p_result.p_pu_alt_up_SR.ML     = new TH1F(Form("SRML_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_PU_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_pu_alt_up_SR.ML->SetDirectory(0);
        p_result.p_pu_alt_up_SR.LM     = new TH1F(Form("SRLM_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_PU_UP_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_pu_alt_up_SR.LM->SetDirectory(0);
        p_result.p_pu_alt_up_SR.INCL     = new TH1F(Form("SRINCL_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_PU_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_pu_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_pu_alt_up_SR.MI1     = new TH1F(Form("SRMI1_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_PU_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_pu_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_pu_alt_up_SR.MI2     = new TH1F(Form("SRMI2_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_PU_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_pu_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.HH     = new TH1F(Form("SRHH_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_PU_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_pu_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.HL     = new TH1F(Form("SRHL_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_PU_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_pu_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.LL     = new TH1F(Form("SRLL_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_PU_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_pu_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.ML     = new TH1F(Form("SRML_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_PU_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_pu_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.LM     = new TH1F(Form("SRLM_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_PU_DN_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_pu_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_PU_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_pu_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_PU_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_pu_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_PU_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_pu_alt_dn_SR.MI2->SetDirectory(0);
    }
    //For fastsim MET variations
    if (isData==0) {
        p_result.p_met_alt_up_SR.SRCR     = new TH1F(Form("SRCR_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_MET_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_met_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_met_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_MET_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_met_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_met_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_MET_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_met_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_met_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_MET_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_met_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_met_alt_up_SR.HH     = new TH1F(Form("SRHH_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_MET_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_met_alt_up_SR.HH->SetDirectory(0);
        p_result.p_met_alt_up_SR.HL     = new TH1F(Form("SRHL_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_MET_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_met_alt_up_SR.HL->SetDirectory(0);
        p_result.p_met_alt_up_SR.LL     = new TH1F(Form("SRLL_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_MET_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_met_alt_up_SR.LL->SetDirectory(0);
        p_result.p_met_alt_up_SR.ML     = new TH1F(Form("SRML_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_MET_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_met_alt_up_SR.ML->SetDirectory(0);
        p_result.p_met_alt_up_SR.LM     = new TH1F(Form("SRLM_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_MET_UP_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_met_alt_up_SR.LM->SetDirectory(0);
        p_result.p_met_alt_up_SR.INCL     = new TH1F(Form("SRINCL_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_MET_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_met_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_met_alt_up_SR.MI1     = new TH1F(Form("SRMI1_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_MET_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_met_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_met_alt_up_SR.MI2     = new TH1F(Form("SRMI2_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_MET_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_met_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_met_alt_dn_SR.HH     = new TH1F(Form("SRHH_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_MET_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_met_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_met_alt_dn_SR.HL     = new TH1F(Form("SRHL_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_MET_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_met_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_met_alt_dn_SR.LL     = new TH1F(Form("SRLL_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_MET_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_met_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_met_alt_dn_SR.ML     = new TH1F(Form("SRML_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_MET_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_met_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_met_alt_dn_SR.LM     = new TH1F(Form("SRLM_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_MET_DN_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_met_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_met_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_MET_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_met_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_met_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_MET_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_met_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_met_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_MET_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_met_alt_dn_SR.MI2->SetDirectory(0);
    }
    //For PU variations
    if (doFakes == 1 || isData==0) {
        p_result.p_lep_alt_up_SR.SRCR     = new TH1F(Form("SRCR_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_LEP_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_lep_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_LEP_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_lep_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_lep_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_LEP_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_lep_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_LEP_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_lep_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_lep_alt_up_SR.HH     = new TH1F(Form("SRHH_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_LEP_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_lep_alt_up_SR.HH->SetDirectory(0);
        p_result.p_lep_alt_up_SR.HL     = new TH1F(Form("SRHL_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_LEP_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_lep_alt_up_SR.HL->SetDirectory(0);
        p_result.p_lep_alt_up_SR.LL     = new TH1F(Form("SRLL_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_LEP_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_lep_alt_up_SR.LL->SetDirectory(0);
        p_result.p_lep_alt_up_SR.ML     = new TH1F(Form("SRML_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_LEP_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_lep_alt_up_SR.ML->SetDirectory(0);
        p_result.p_lep_alt_up_SR.LM     = new TH1F(Form("SRLM_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_LEP_UP_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_lep_alt_up_SR.LM->SetDirectory(0);
        p_result.p_lep_alt_up_SR.INCL     = new TH1F(Form("SRINCL_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_LEP_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_lep_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_lep_alt_up_SR.MI1     = new TH1F(Form("SRMI1_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_LEP_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_lep_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_lep_alt_up_SR.MI2     = new TH1F(Form("SRMI2_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_LEP_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_lep_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.HH     = new TH1F(Form("SRHH_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_LEP_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_lep_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.HL     = new TH1F(Form("SRHL_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_LEP_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_lep_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.LL     = new TH1F(Form("SRLL_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_LEP_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_lep_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.ML     = new TH1F(Form("SRML_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_LEP_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_lep_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.LM     = new TH1F(Form("SRLM_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_LEP_DN_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_lep_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_LEP_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_lep_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_LEP_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_lep_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_LEP_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_lep_alt_dn_SR.MI2->SetDirectory(0);
    }
    //For bb variations
    if (doFakes == 1 || isData==0) {
        p_result.p_bb_alt_up_SR.SRCR     = new TH1F(Form("SRCR_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_BB_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_bb_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_BB_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_bb_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_bb_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_BB_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_bb_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_BB_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_bb_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_bb_alt_up_SR.HH     = new TH1F(Form("SRHH_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_BB_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_bb_alt_up_SR.HH->SetDirectory(0);
        p_result.p_bb_alt_up_SR.HL     = new TH1F(Form("SRHL_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_BB_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_bb_alt_up_SR.HL->SetDirectory(0);
        p_result.p_bb_alt_up_SR.LL     = new TH1F(Form("SRLL_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_BB_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_bb_alt_up_SR.LL->SetDirectory(0);
        p_result.p_bb_alt_up_SR.ML     = new TH1F(Form("SRML_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_BB_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_bb_alt_up_SR.ML->SetDirectory(0);
        p_result.p_bb_alt_up_SR.LM     = new TH1F(Form("SRLM_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_BB_UP_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_bb_alt_up_SR.LM->SetDirectory(0);
        p_result.p_bb_alt_up_SR.INCL     = new TH1F(Form("SRINCL_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_BB_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_bb_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_bb_alt_up_SR.MI1     = new TH1F(Form("SRMI1_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_BB_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_bb_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_bb_alt_up_SR.MI2     = new TH1F(Form("SRMI2_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_BB_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_bb_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.HH     = new TH1F(Form("SRHH_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_BB_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_bb_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.HL     = new TH1F(Form("SRHL_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_BB_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_bb_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.LL     = new TH1F(Form("SRLL_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_BB_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_bb_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.ML     = new TH1F(Form("SRML_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_BB_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_bb_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.LM     = new TH1F(Form("SRLM_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_BB_DN_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_bb_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_BB_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_bb_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_BB_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_bb_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_BB_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_bb_alt_dn_SR.MI2->SetDirectory(0);
    }
    //For trigger SFs
    if (isData==0) {
        p_result.p_trigger_alt_up_SR.SRCR     = new TH1F(Form("SRCR_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_trigger_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_trigger_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_trigger_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_trigger_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_trigger_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_trigger_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_trigger_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_trigger_alt_up_SR.HH     = new TH1F(Form("SRHH_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_trigger_alt_up_SR.HH->SetDirectory(0);
        p_result.p_trigger_alt_up_SR.HL     = new TH1F(Form("SRHL_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_trigger_alt_up_SR.HL->SetDirectory(0);
        p_result.p_trigger_alt_up_SR.LL     = new TH1F(Form("SRLL_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_trigger_alt_up_SR.LL->SetDirectory(0);
        p_result.p_trigger_alt_up_SR.ML     = new TH1F(Form("SRML_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_trigger_alt_up_SR.ML->SetDirectory(0);
        p_result.p_trigger_alt_up_SR.LM     = new TH1F(Form("SRLM_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_trigger_alt_up_SR.LM->SetDirectory(0);
        p_result.p_trigger_alt_up_SR.INCL     = new TH1F(Form("SRINCL_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_trigger_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_trigger_alt_up_SR.MI1     = new TH1F(Form("SRMI1_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_trigger_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_trigger_alt_up_SR.MI2     = new TH1F(Form("SRMI2_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_TRIGGER_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_trigger_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_trigger_alt_dn_SR.HH     = new TH1F(Form("SRHH_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_trigger_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_trigger_alt_dn_SR.HL     = new TH1F(Form("SRHL_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_trigger_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_trigger_alt_dn_SR.LL     = new TH1F(Form("SRLL_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_trigger_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_trigger_alt_dn_SR.ML     = new TH1F(Form("SRML_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_trigger_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_trigger_alt_dn_SR.LM     = new TH1F(Form("SRLM_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_trigger_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_trigger_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_trigger_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_trigger_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_trigger_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_trigger_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_TRIGGER_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_trigger_alt_dn_SR.MI2->SetDirectory(0);
    }
    //For prefiring SF variations for 2016, 2017
    if (isData==0) {
        p_result.p_prefire_alt_up_SR.SRCR     = new TH1F(Form("SRCR_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_prefire_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_prefire_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_prefire_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_prefire_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_prefire_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_prefire_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_prefire_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_prefire_alt_up_SR.HH     = new TH1F(Form("SRHH_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_prefire_alt_up_SR.HH->SetDirectory(0);
        p_result.p_prefire_alt_up_SR.HL     = new TH1F(Form("SRHL_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_prefire_alt_up_SR.HL->SetDirectory(0);
        p_result.p_prefire_alt_up_SR.LL     = new TH1F(Form("SRLL_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_prefire_alt_up_SR.LL->SetDirectory(0);
        p_result.p_prefire_alt_up_SR.ML     = new TH1F(Form("SRML_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_prefire_alt_up_SR.ML->SetDirectory(0);
        p_result.p_prefire_alt_up_SR.LM     = new TH1F(Form("SRLM_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_prefire_alt_up_SR.LM->SetDirectory(0);
        p_result.p_prefire_alt_up_SR.INCL     = new TH1F(Form("SRINCL_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_prefire_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_prefire_alt_up_SR.MI1     = new TH1F(Form("SRMI1_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_prefire_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_prefire_alt_up_SR.MI2     = new TH1F(Form("SRMI2_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_prefire_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_prefire_alt_dn_SR.HH     = new TH1F(Form("SRHH_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_prefire_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_prefire_alt_dn_SR.HL     = new TH1F(Form("SRHL_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_prefire_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_prefire_alt_dn_SR.LL     = new TH1F(Form("SRLL_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_prefire_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_prefire_alt_dn_SR.ML     = new TH1F(Form("SRML_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_prefire_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_prefire_alt_dn_SR.LM     = new TH1F(Form("SRLM_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_prefire_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_prefire_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_prefire_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_prefire_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_prefire_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_prefire_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_prefire_alt_dn_SR.MI2->SetDirectory(0);
    }
    //For PU variations
    if (doFakes == 1 || isData==0) {
        p_result.p_isr_alt_up_SR.SRCR     = new TH1F(Form("SRCR_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISR_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_isr_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISR_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_isr_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_isr_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISR_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_isr_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISR_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_isr_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_isr_alt_up_SR.HH     = new TH1F(Form("SRHH_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ISR_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_isr_alt_up_SR.HH->SetDirectory(0);
        p_result.p_isr_alt_up_SR.HL     = new TH1F(Form("SRHL_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ISR_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_isr_alt_up_SR.HL->SetDirectory(0);
        p_result.p_isr_alt_up_SR.LL     = new TH1F(Form("SRLL_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ISR_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_isr_alt_up_SR.LL->SetDirectory(0);
        p_result.p_isr_alt_up_SR.ML     = new TH1F(Form("SRML_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_ISR_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_isr_alt_up_SR.ML->SetDirectory(0);
        p_result.p_isr_alt_up_SR.LM     = new TH1F(Form("SRLM_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_ISR_UP_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_isr_alt_up_SR.LM->SetDirectory(0);
        p_result.p_isr_alt_up_SR.INCL     = new TH1F(Form("SRINCL_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_ISR_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_isr_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_isr_alt_up_SR.MI1     = new TH1F(Form("SRMI1_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_ISR_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_isr_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_isr_alt_up_SR.MI2     = new TH1F(Form("SRMI2_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_ISR_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_isr_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.HH     = new TH1F(Form("SRHH_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ISR_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_isr_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.HL     = new TH1F(Form("SRHL_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ISR_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_isr_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.LL     = new TH1F(Form("SRLL_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ISR_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_isr_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.ML     = new TH1F(Form("SRML_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_ISR_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_isr_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.LM     = new TH1F(Form("SRLM_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_ISR_DN_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_isr_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_ISR_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_isr_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_ISR_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_isr_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_ISR_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_isr_alt_dn_SR.MI2->SetDirectory(0);
    }
    p_result.p_unw_raw_SR.SRCR     = new TH1F(Form("SRCR_UNW_RAW_TOTAL_%s"   , chainTitleCh) , Form("SRCR_UNW_RAW_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_unw_raw_SR.SRCR->SetDirectory(0);
    p_result.p_unw_sgn_SR.SRCR     = new TH1F(Form("SRCR_UNW_SGN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_UNW_SGN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_unw_sgn_SR.SRCR->SetDirectory(0);
    //For theory variations
    if (isData==0) {
        p_result.p_pdf_alt_up_SR.SRCR     = new TH1F(Form("SRCR_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PDF_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_pdf_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PDF_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_pdf_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_pdf_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PDF_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_pdf_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PDF_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_pdf_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_scale_alt_up_SR.SRCR     = new TH1F(Form("SRCR_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_scale_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_scale_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_scale_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_scale_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_scale_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.SRCR     = new TH1F(Form("SRCR_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_alphas_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_alphas_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_alphas_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_alphas_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.SRCR     = new TH1F(Form("SRCR_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_isrvar_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_isrvar_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_isrvar_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_isrvar_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.SRCR     = new TH1F(Form("SRCR_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_fsrvar_alt_up_SR.SRCR->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 ); p_result.p_fsrvar_alt_dn_SR.SRCR->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_fsrvar_alt_up_SR.SRDISC->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 ); p_result.p_fsrvar_alt_dn_SR.SRDISC->SetDirectory(0);
        p_result.p_pdf_alt_up_SR.HH     = new TH1F(Form("SRHH_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_PDF_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_pdf_alt_up_SR.HH->SetDirectory(0);
        p_result.p_pdf_alt_up_SR.HL     = new TH1F(Form("SRHL_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_PDF_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_pdf_alt_up_SR.HL->SetDirectory(0);
        p_result.p_pdf_alt_up_SR.LL     = new TH1F(Form("SRLL_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_PDF_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_pdf_alt_up_SR.LL->SetDirectory(0);
        p_result.p_pdf_alt_up_SR.ML     = new TH1F(Form("SRML_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_PDF_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_pdf_alt_up_SR.ML->SetDirectory(0);
        p_result.p_pdf_alt_up_SR.LM     = new TH1F(Form("SRLM_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_PDF_UP_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_pdf_alt_up_SR.LM->SetDirectory(0);
        p_result.p_pdf_alt_up_SR.INCL     = new TH1F(Form("SRINCL_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_PDF_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_pdf_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_pdf_alt_up_SR.MI1     = new TH1F(Form("SRMI1_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_PDF_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_pdf_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_pdf_alt_up_SR.MI2     = new TH1F(Form("SRMI2_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_PDF_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_pdf_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.HH     = new TH1F(Form("SRHH_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_PDF_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_pdf_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.HL     = new TH1F(Form("SRHL_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_PDF_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_pdf_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.LL     = new TH1F(Form("SRLL_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_PDF_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_pdf_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.ML     = new TH1F(Form("SRML_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_PDF_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_pdf_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.LM     = new TH1F(Form("SRLM_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_PDF_DN_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_pdf_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_PDF_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_pdf_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_PDF_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_pdf_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_PDF_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_pdf_alt_dn_SR.MI2->SetDirectory(0);
        p_result.p_scale_alt_up_SR.HH     = new TH1F(Form("SRHH_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_scale_alt_up_SR.HH->SetDirectory(0);
        p_result.p_scale_alt_up_SR.HL     = new TH1F(Form("SRHL_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_scale_alt_up_SR.HL->SetDirectory(0);
        p_result.p_scale_alt_up_SR.LL     = new TH1F(Form("SRLL_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_scale_alt_up_SR.LL->SetDirectory(0);
        p_result.p_scale_alt_up_SR.ML     = new TH1F(Form("SRML_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_scale_alt_up_SR.ML->SetDirectory(0);
        p_result.p_scale_alt_up_SR.LM     = new TH1F(Form("SRLM_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_scale_alt_up_SR.LM->SetDirectory(0);
        p_result.p_scale_alt_up_SR.INCL     = new TH1F(Form("SRINCL_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_scale_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_scale_alt_up_SR.MI1     = new TH1F(Form("SRMI1_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_scale_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_scale_alt_up_SR.MI2     = new TH1F(Form("SRMI2_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_scale_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.HH     = new TH1F(Form("SRHH_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_scale_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.HL     = new TH1F(Form("SRHL_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_scale_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.LL     = new TH1F(Form("SRLL_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_scale_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.ML     = new TH1F(Form("SRML_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_scale_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.LM     = new TH1F(Form("SRLM_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_scale_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_scale_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_scale_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_scale_alt_dn_SR.MI2->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.HH     = new TH1F(Form("SRHH_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_alphas_alt_up_SR.HH->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.HL     = new TH1F(Form("SRHL_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_alphas_alt_up_SR.HL->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.LL     = new TH1F(Form("SRLL_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_alphas_alt_up_SR.LL->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.ML     = new TH1F(Form("SRML_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_alphas_alt_up_SR.ML->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.LM     = new TH1F(Form("SRLM_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_alphas_alt_up_SR.LM->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.INCL     = new TH1F(Form("SRINCL_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_alphas_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.MI1     = new TH1F(Form("SRMI1_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_alphas_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.MI2     = new TH1F(Form("SRMI2_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_alphas_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.HH     = new TH1F(Form("SRHH_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_alphas_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.HL     = new TH1F(Form("SRHL_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_alphas_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.LL     = new TH1F(Form("SRLL_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_alphas_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.ML     = new TH1F(Form("SRML_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_alphas_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.LM     = new TH1F(Form("SRLM_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_alphas_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_alphas_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_alphas_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_alphas_alt_dn_SR.MI2->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.HH     = new TH1F(Form("SRHH_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_isrvar_alt_up_SR.HH->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.HL     = new TH1F(Form("SRHL_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_isrvar_alt_up_SR.HL->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.LL     = new TH1F(Form("SRLL_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_isrvar_alt_up_SR.LL->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.ML     = new TH1F(Form("SRML_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_isrvar_alt_up_SR.ML->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.LM     = new TH1F(Form("SRLM_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_isrvar_alt_up_SR.LM->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.INCL     = new TH1F(Form("SRINCL_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_isrvar_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.MI1     = new TH1F(Form("SRMI1_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_isrvar_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.MI2     = new TH1F(Form("SRMI2_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_isrvar_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.HH     = new TH1F(Form("SRHH_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_isrvar_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.HL     = new TH1F(Form("SRHL_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_isrvar_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.LL     = new TH1F(Form("SRLL_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_isrvar_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.ML     = new TH1F(Form("SRML_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_isrvar_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.LM     = new TH1F(Form("SRLM_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_isrvar_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_isrvar_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_isrvar_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_isrvar_alt_dn_SR.MI2->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.HH     = new TH1F(Form("SRHH_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_fsrvar_alt_up_SR.HH->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.HL     = new TH1F(Form("SRHL_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_fsrvar_alt_up_SR.HL->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.LL     = new TH1F(Form("SRLL_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_fsrvar_alt_up_SR.LL->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.ML     = new TH1F(Form("SRML_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_fsrvar_alt_up_SR.ML->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.LM     = new TH1F(Form("SRLM_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLM_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_fsrvar_alt_up_SR.LM->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.INCL     = new TH1F(Form("SRINCL_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_fsrvar_alt_up_SR.INCL->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.MI1     = new TH1F(Form("SRMI1_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_fsrvar_alt_up_SR.MI1->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.MI2     = new TH1F(Form("SRMI2_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_fsrvar_alt_up_SR.MI2->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.HH     = new TH1F(Form("SRHH_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_fsrvar_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.HL     = new TH1F(Form("SRHL_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_fsrvar_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.LL     = new TH1F(Form("SRLL_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_fsrvar_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.ML     = new TH1F(Form("SRML_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_fsrvar_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.LM     = new TH1F(Form("SRLM_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLM_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nLMsr,  0.5, nLMsr+0.5 ); p_result.p_fsrvar_alt_dn_SR.LM->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.INCL     = new TH1F(Form("SRINCL_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRINCL_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nINCLsr,  0.5, nINCLsr+0.5 ); p_result.p_fsrvar_alt_dn_SR.INCL->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.MI1     = new TH1F(Form("SRMI1_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI1_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nMI1sr,  0.5, nMI1sr+0.5 ); p_result.p_fsrvar_alt_dn_SR.MI1->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.MI2     = new TH1F(Form("SRMI2_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRMI2_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nMI2sr,  0.5, nMI2sr+0.5 ); p_result.p_fsrvar_alt_dn_SR.MI2->SetDirectory(0);

    }


    bool doPoisson = isData && !doFlips && !doFakes;

    p_result.SR.InitHistError(doPoisson);
    p_result.p_jes_alt_up_SR.InitHistError(doPoisson);
    p_result.p_jes_alt_dn_SR.InitHistError(doPoisson);
    p_result.p_jer_alt_up_SR.InitHistError(doPoisson);
    p_result.p_jer_alt_dn_SR.InitHistError(doPoisson);
    p_result.p_btagSF_alt_up_SR.InitHistError(doPoisson);
    p_result.p_btagSF_alt_dn_SR.InitHistError(doPoisson);
    p_result.p_pu_alt_up_SR.InitHistError(doPoisson);
    p_result.p_lep_alt_up_SR.InitHistError(doPoisson);
    p_result.p_met_alt_up_SR.InitHistError(doPoisson);
    p_result.p_isr_alt_up_SR.InitHistError(doPoisson);
    p_result.p_isr_alt_dn_SR.InitHistError(doPoisson);
    p_result.p_bb_alt_up_SR.InitHistError(doPoisson);
    p_result.p_pu_alt_dn_SR.InitHistError(doPoisson);
    p_result.p_fake_unw_up_SR.InitHistError(doPoisson);
    p_result.p_fake_alt_up_SR.InitHistError(doPoisson);
    p_result.p_fake_nb0_up_SR.InitHistError(doPoisson);
    p_result.p_fake_nb1_up_SR.InitHistError(doPoisson);
    p_result.p_fake_nb2_up_SR.InitHistError(doPoisson);
    p_result.p_fake_nb3_up_SR.InitHistError(doPoisson);
    p_result.p_pdf_alt_up_SR.InitHistError(doPoisson);
    p_result.p_pdf_alt_dn_SR.InitHistError(doPoisson);
    p_result.p_scale_alt_up_SR.InitHistError(doPoisson);
    p_result.p_scale_alt_dn_SR.InitHistError(doPoisson);
    p_result.p_alphas_alt_up_SR.InitHistError(doPoisson);
    p_result.p_alphas_alt_dn_SR.InitHistError(doPoisson);
    p_result.p_isrvar_alt_up_SR.InitHistError(doPoisson);
    p_result.p_isrvar_alt_dn_SR.InitHistError(doPoisson);
    p_result.p_fsrvar_alt_up_SR.InitHistError(doPoisson);
    p_result.p_fsrvar_alt_dn_SR.InitHistError(doPoisson);
    p_result.p_unw_raw_SR.InitHistError(doPoisson);
    p_result.p_unw_sgn_SR.InitHistError(doPoisson);


    //nEvents in chain
    unsigned int nEventsTotal = 0;
    unsigned int nEventsChain = chain->GetEntries();

    //Set up iterator
    TObjArray *listOfFiles = chain->GetListOfFiles();
    TIter fileIter(listOfFiles);
    TFile *currentFile = 0;

    //Number of selected events
    //int nSelected = 0;

    tqdm bar;
    // bar.set_theme_braille();

    //File Loop
    while ( (currentFile = (TFile*)fileIter.Next()) ){

        if (STOP_REQUESTED) break;

        // Get File Content
        TFile *file = new TFile(currentFile->GetTitle());

        float norm_scale_up = 1.;
        float norm_scale_dn = 1.;
        float norm_pdf_up = 1.;
        float norm_pdf_dn = 1.;
        if (file->Get("counts")) {
            h_counts = (TH2D*)((file->Get("counts"))->Clone("h_counts"));
        }
        if (file->Get("weight")) {
            h_weights = (TH1D*)((file->Get("weight"))->Clone("h_weights"));
            norm_scale_up = getWeightNormalization(1);
            norm_scale_dn = getWeightNormalization(2);
            norm_pdf_up = getWeightNormalization(3);
            norm_pdf_dn = getWeightNormalization(4);
            if (norm_scale_up <= 0.) norm_scale_up = 1.;
            if (norm_scale_dn <= 0.) norm_scale_dn = 1.;
            if (norm_pdf_up <= 0.) norm_pdf_up = 1.;
            if (norm_pdf_dn <= 0.) norm_pdf_dn = 1.;
        }
        TString filename = currentFile->GetTitle();
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        bool isVH = filename.Contains("VHtoNonBB");

        int higgs_type = -1;
        float higgs_weight = 0;
        if (isHiggsScan) {
            if (filename.Contains("ttH")) higgs_type = 1;
            else if (filename.Contains("tHW")) higgs_type = 2;
            else if (filename.Contains("tHq")) higgs_type = 3;
            if (isHiggsPseudoscalar) higgs_type += 3;
            float xsec = xsec_higgs(higgs_type, higgs_mass);
            if (isHiggsBoth) {
                if (filename.Contains("ttH")) xsec += xsec_higgs(higgs_type+3, higgs_mass);
                else if (filename.Contains("tHW")) xsec += xsec_higgs(higgs_type+3, higgs_mass);
                else if (filename.Contains("tHq")) xsec += xsec_higgs(higgs_type+3, higgs_mass);
            }
            if (h_counts) {
                higgs_weight = 1000. * xsec / h_counts->GetEntries();
            } else {
                std::cout << "No count histogram, so can't normalize this higgs sample!" << std::endl;
                continue;
            }
            if (!quiet) {
                std::cout << "This is a higgs scan of type,mass = " << higgs_type << "," << higgs_mass << " with xsec,scale1fb = " << xsec << "," << higgs_weight << std::endl;
            }
        }

        // Loop over Events in current file
        for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){

            if (STOP_REQUESTED) break;

            //Get Event Content
            samesign.GetEntry(event);
            nEventsTotal++;

            if (!quiet) bar.progress(nEventsTotal, nEventsChain);

            if (isSS) {
                if (ss::hyp_class() == 4 and !ss::is_real_data()) continue; // don't need OS MC for anything
                if (ss::njets() < 2 and ss::njets_unc_up() < 2 and ss::njets_unc_dn() < 2) continue;
            } else {
                if (!ss::skim()) continue;
            }

            if (isFastsim) {
                if (isRPV) {
                    if (fabs(mysparms[0]-ss::sparms()[0]) > 0.1) continue;
                } else {
                    if ((fabs(mysparms[0]-ss::sparms()[0]) > 0.1) || (fabs(mysparms[1]-ss::sparms()[1]) > 0.1)) continue;
                }
            }

            // Reject not triggered
            if (!isFastsim) {
                if (isSS) {
                    if (year == 2017) { // FIXME FIXME need to update tag
                        if (!ss::fired_trigger_ss()) continue;
                    } else {
                        if (!ss::fired_trigger_ss()) continue;
                    }
                } else {
                    if (!ss::fired_trigger()) continue;
                }
            }
            if (!ss::passes_met_filters()) continue;



            // if (isHiggsScan) {
            //     // make sure the higgs mass point we are considering is the same as chain title
            //     if (fabs(mysparms[0]-ss::higgs_mass()) > 0.1) continue;
            // }

            float weight =  ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG;

            if (partialUnblind and ss::is_real_data()) {
                if (!goodrun(ss::run(), ss::lumi())) continue;
            }

            if (isFastsim) {
                weight = getSMSscale1fb(isRPV)*lumiAG;
                weight *= invFilterEff;
                if (isT5qqqqWW) {
                    // filter for 2 Ws when running on VV to get WW (and multiply by filter eff above)
                    if (ss::ncharginos() < 2) continue;
                }
                if (isT6ttHZ) {
                    if (isT6ttHZ_both) {
                        if (ss::nhiggs() != 1) continue;
                    } else if (isT6ttHZ_h) {
                        if (ss::nhiggs() != 2) continue;
                    } else if (isT6ttHZ_z) {
                        if (ss::nhiggs() != 0) continue;
                    }
                }
            }

            if (isRPVfullsim) {
                weight = getRPVscale1fb()*lumiAG;
            }


            if (isHiggsScan) {
                weight = higgs_weight*lumiAG;
            }

            if (istttt) {
                // force xsec here
                weight *= XSEC_TTTT / fabs(1000.0*ss::xsec());
            }
            if (isVH) {
                // force xsec here
                weight *= 0.952 / fabs(ss::xsec());
            }
            weight*=scaleLumi;
            // weight *= weightScale;

            float lep1ccpt = ss::lep1_coneCorrPt();
            float lep2ccpt = ss::lep2_coneCorrPt();
            float lep3ccpt = ss::lep3_coneCorrPt();
            float lep1pt = ss::lep1_p4().pt();
            float lep2pt = ss::lep2_p4().pt();
            float lep3pt = ss::lep3_p4().pt();
            float lep1eta = ss::lep1_p4().eta();
            float lep2eta = ss::lep2_p4().eta();
            float lep3eta = ss::lep3_p4().eta();
            float lep1phi = ss::lep1_p4().phi();
            float lep2phi = ss::lep2_p4().phi();
            float lep3phi = ss::lep3_p4().phi();
            int lep1id = ss::lep1_id();
            int lep2id = ss::lep2_id();
            int lep3id = ss::lep3_id();
            int lep1q = (lep1id > 0) ? -1 : 1;
            int lep2q = (lep2id > 0) ? -1 : 1;
            int lep3q = (lep3id > 0) ? -1 : 1;
            int lep1good = ss::lep1_passes_id();
            int lep2good = ss::lep2_passes_id();
            int lep3good = ss::lep3_passes_id();
            float mtl1 = calcMT(lep1ccpt, ss::lep1_p4().phi(), ss::met(), ss::metPhi());
            float mtl2 = calcMT(lep2ccpt, ss::lep2_p4().phi(), ss::met(), ss::metPhi());
            float mtmin = mtl1 > mtl2 ? mtl2 : mtl1;

            float met = ss::met();
            float ht = ss::ht();

            float met_unc_up = ss::met_unc_up();
            float met_unc_dn = ss::met_unc_dn();
            float metPhi_unc_up = ss::metPhi_unc_up();
            float metPhi_unc_dn = ss::metPhi_unc_dn();
            float ht_unc_up = ss::ht_unc_up();
            float ht_unc_dn = ss::ht_unc_dn();
            float njets_unc_up = ss::njets_unc_up();
            float njets_unc_dn = ss::njets_unc_dn();
            float nbtags_unc_up = ss::nbtags_unc_up();
            float nbtags_unc_dn = ss::nbtags_unc_dn();

            float met_JER_up = ss::met_JER_up();
            float met_JER_dn = ss::met_JER_dn();
            float metPhi_JER_up = ss::metPhi_JER_up();
            float metPhi_JER_dn = ss::metPhi_JER_dn();
            float ht_JER_up = ss::ht_JER_up();
            float ht_JER_dn = ss::ht_JER_dn();
            float njets_JER_up = ss::njets_JER_up();
            float njets_JER_dn = ss::njets_JER_dn();
            float nbtags_JER_up = ss::nbtags_JER_up();
            float nbtags_JER_dn = ss::nbtags_JER_dn();

            // float met_unc_up = ss::met();
            // float met_unc_dn = ss::met();
            // float metPhi_unc_up = ss::metPhi();
            // float metPhi_unc_dn = ss::metPhi();
            // float ht_unc_up = ss::ht();
            // float ht_unc_dn = ss::ht();
            // float njets_unc_up = ss::njets();
            // float njets_unc_dn = ss::njets();
            // float nbtags_unc_up = ss::nbtags();
            // float nbtags_unc_dn = ss::nbtags();

            if ((met_unc_up > met*2) || (met_unc_dn > met*2) || (met_unc_up < met/2) || (met_unc_dn < met/2) || (ht_unc_up > ht*2) || (ht_unc_dn > ht*2) || (ht_unc_up < ht/2) || (ht_unc_dn < ht/2)) {
                met_unc_up = ss::met();
                met_unc_dn = ss::met();
                metPhi_unc_up = ss::metPhi();
                metPhi_unc_dn = ss::metPhi();
                ht_unc_up = ss::ht();
                ht_unc_dn = ss::ht();
                njets_unc_up = ss::njets();
                njets_unc_dn = ss::njets();
                nbtags_unc_up = ss::nbtags();
                nbtags_unc_dn = ss::nbtags();
            }


            float mtl1_unc_up = calcMT(lep1ccpt, ss::lep1_p4().phi(), met_unc_up, metPhi_unc_up);
            float mtl2_unc_up = calcMT(lep2ccpt, ss::lep2_p4().phi(), met_unc_up, metPhi_unc_up);
            float mtmin_unc_up = mtl1_unc_up > mtl2_unc_up ? mtl2_unc_up : mtl1_unc_up;
            float mtl1_unc_dn = calcMT(lep1ccpt, ss::lep1_p4().phi(), met_unc_dn, metPhi_unc_dn);
            float mtl2_unc_dn = calcMT(lep2ccpt, ss::lep2_p4().phi(), met_unc_dn, metPhi_unc_dn);
            float mtmin_unc_dn = mtl1_unc_dn > mtl2_unc_dn ? mtl2_unc_dn : mtl1_unc_dn;

            int nleps = 2;
            if (isSS) {
                nleps = (lep3good) ? ((ss::lep4_passes_id() and (ss::lep4_p4().pt() > (abs(ss::lep4_id())==11 ? 15 : 10))) ? 4 : 3) : 2;
            } else {
                nleps = (lep3good and lep3ccpt > 20) ? ((ss::lep4_passes_id() and ss::lep4_p4().pt() > 20) ? 4 : 3) : 2;
            }

            region_t categ = Undefined;
            region_t categ_genmet = Undefined;
            region_t categ_unc_up = categ;
            region_t categ_unc_dn = categ;
            bool passbr_nom = false;
            bool passbr_unc_up = false;
            bool passbr_unc_dn = false;
            bool passbr_JER_up = false;
            bool passbr_JER_dn = false;
            if (isSS) {
                //Require nominal baseline selections
                passbr_nom = passes_baseline_ss(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), lep1id, lep2id, lep1ccpt, lep2ccpt);
                passbr_unc_up = passes_baseline_ss(njets_unc_up, nbtags_unc_up, met_unc_up, ht_unc_up, lep1id, lep2id, lep1ccpt, lep2ccpt);
                passbr_unc_dn = passes_baseline_ss(njets_unc_dn, nbtags_unc_dn, met_unc_dn, ht_unc_dn, lep1id, lep2id, lep1ccpt, lep2ccpt);
                if (passbr_nom) { categ = analysis_category_ss(ss::lep1_id(), ss::lep2_id(), lep1ccpt, lep2ccpt, lep3ccpt, nleps, ss::ht(), ss::met()); }
                if (passbr_unc_up) { categ_unc_up = analysis_category_ss(lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt, nleps, ht_unc_up, met_unc_up); }
                if (passbr_unc_dn) { categ_unc_dn = analysis_category_ss(lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt, nleps, ht_unc_dn, met_unc_dn); }
                if (categ == Undefined and categ_unc_up == Undefined and categ_unc_dn == Undefined) continue; // HighHigh = 0, HighLow, LowLow, Multilepton
                if (categ == Multilepton or categ_unc_up == Multilepton or categ_unc_dn == Multilepton) {
                    float mtl3 = calcMT(lep3ccpt, ss::lep3_p4().phi(), ss::met(), ss::metPhi());
                    float mtl3_unc_up = calcMT(lep3ccpt, ss::lep3_p4().phi(), met_unc_up, metPhi_unc_up);
                    float mtl3_unc_dn = calcMT(lep3ccpt, ss::lep3_p4().phi(), met_unc_dn, metPhi_unc_dn);
                    mtmin = min(mtl3, mtmin);
                    mtmin_unc_up = min(mtl3_unc_up, mtmin_unc_up);
                    mtmin_unc_dn = min(mtl3_unc_dn, mtmin_unc_dn);
                }
            } else {
                // if (!passes_baseline_ft(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), lep1id, lep2id, lep1ccpt, lep2ccpt)) continue;
                passbr_nom = passes_baseline_ft(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), lep1id, lep2id, lep1ccpt, lep2ccpt);
                passbr_unc_up = passes_baseline_ft(njets_unc_up, nbtags_unc_up, met_unc_up, ht_unc_up, lep1id, lep2id, lep1ccpt, lep2ccpt);
                passbr_unc_dn = passes_baseline_ft(njets_unc_dn, nbtags_unc_dn, met_unc_dn, ht_unc_dn, lep1id, lep2id, lep1ccpt, lep2ccpt);
                passbr_JER_up = passes_baseline_ft(njets_JER_up, nbtags_JER_up, met_JER_up, ht_JER_up, lep1id, lep2id, lep1ccpt, lep2ccpt);
                passbr_JER_dn = passes_baseline_ft(njets_JER_dn, nbtags_JER_dn, met_JER_dn, ht_JER_dn, lep1id, lep2id, lep1ccpt, lep2ccpt);
                if (!passbr_nom and !passbr_unc_up and !passbr_unc_dn and !passbr_JER_up and !passbr_JER_dn) continue;
            }

            if (isSS and !ss::is_real_data()) {
                if (lep1ccpt < 25. and lep2ccpt < 25. and nleps==2 and year==2017) {
                    // Low low dilepton+HT triggers missing in 2017B (11.6% of 2017 lumi, so weight down MC by (1-0.116));
                    weight *= 1-0.116;
                }
            }
            region_t categ_JER_up = categ;
            region_t categ_JER_dn = categ;


            // // FIXME
            // if (isttW || isttZ) {
            //     weight *= 1.3;
            // }
            
            // if (isttH) {
            //     weight *= 1.26;
            // }


            // if (isHiggsPseudoscalar) weight *= ss::xsec_ps()/ss::xsec();
            float nomtrigsf = 1.;
            if (ss::is_real_data()==0) {
                if (lep1good) weight *= leptonScaleFactor(year, lep1id, lep1ccpt, lep1eta, ss::ht(), analysis);
                if (lep2good) weight *= leptonScaleFactor(year, lep2id, lep2ccpt, lep2eta, ss::ht(), analysis);
                if ((isSS and (categ==Multilepton)) || (!isSS and lep3good and lep3ccpt>20)) {
                    weight *= leptonScaleFactor(year, lep3id, lep3ccpt, lep3eta, ss::ht(), analysis);
                } else {
                    nomtrigsf = triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht(), analysis);
                }
                weight *= nomtrigsf;
                weight *= ss::weight_btagsf();
                weight *= getTruePUw(year, ss::trueNumInt()[0], 0);
                if (!isFastsim) {
                    if (not isSS) {
                        weight *= ss::decayWSF();
                    }
                }
            }
            if (isFastsim) {
                if (lep1good) weight *= fastsim_leptonScaleFactor(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                if (lep2good) weight *= fastsim_leptonScaleFactor(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                if (isSS) {
                    if (categ == Multilepton) weight *= fastsim_leptonScaleFactor(year, lep3id, lep3ccpt, lep3eta, ss::ht());
                    else weight *= fastsim_triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht());
                    // if (year == 2016) weight *= ss::weight_isr()*fastsim_isr_norm_central;
                    weight *= ss::weight_isr()*fastsim_isr_norm_central;
                } else {
                    if (lep3good && lep3ccpt>20) weight *= fastsim_leptonScaleFactor(year, lep3id, lep3ccpt, lep3eta, ss::ht());
                    else weight *= fastsim_triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht());
                }
            }

            if (ss::is_real_data()==0) {
                if (year == 2016) {
                    weight *= ss::prefire2016_sf();
                } else if (year == 2017) {
                    weight *= ss::prefire2017_sf();
                }
            }

#ifdef SSLOOP
            float weight_isr_up_alt = weight;
            float weight_isr_dn_alt = weight;
            // if (isFastsim and year == 2016)
            if (isFastsim) {
              weight_isr_up_alt *= ss::weight_isr_UP()*fastsim_isr_norm_up;
              weight_isr_dn_alt *= ss::weight_isr_DN()*fastsim_isr_norm_down;
            }
            // FIXME
            // if (isttW) {
            //     // later on in a script, we take 50% of the effect as unc, and since we scale
            //     // nominal weight, scale up twice
            //     float w = isrWeight(year, ss::nisrMatch(), 1);
            //     weight_isr_up_alt *= w*w;
            //     weight *= w;
            // }
            // if (isttZ) {
            //     float w = isrWeight(year, ss::nisrMatch(), 2);
            //     weight_isr_up_alt *= w*w;
            //     weight *= w;
            // }
            // if (istt) {
            //     float w = isrWeight(year, ss::nisrMatch(), 10);
            //     weight_isr_up_alt *= w*w;
            //     weight *= w;
            // }

            // ttbb bb scaling for HF
            // scale up weight by 70% and take 85% of that as bb_up
            float weight_bb_up_alt = weight;
            if ((isttZ || isttW || isttH)) {
                if (ss::extragenb() >= 2) {
                    // weight *= (0.7+1);
                    weight_bb_up_alt = (0.6+1)*weight;
                }
            }

#else

            float weight_isr_up_alt = weight;
            if (isttW) {
                // later on in a script, we take 50% of the effect as unc, and since we scale
                // nominal weight, scale up twice
                float w = isrWeight(year, ss::nisrMatch(), 1);
                weight_isr_up_alt *= w*w;
                weight *= w;
            }
            if (isttZ) {
                float w = isrWeight(year, ss::nisrMatch(), 2);
                weight_isr_up_alt *= w*w;
                weight *= w;
            }
            if (istt) {
                float w = isrWeight(year, ss::nisrMatch(), 10);
                weight_isr_up_alt *= w*w;
                weight *= w;
            }

            // ttbb bb scaling for HF
            // scale up weight by 70% and take 85% of that as bb_up
            float weight_bb_up_alt = weight;
            if ((isttZ || isttW || isttH)) {
                if (ss::extragenb() >= 2) {

                    weight *= (0.7+1);
                    weight_bb_up_alt = (0.6+1)*weight;

                    // weight *= (0.3+1);
                    // weight_bb_up_alt = (0.2+1)*weight;

                    // // increase uncertainty for ttW
                    // if (isttW) {
                    //     weight_bb_up_alt = (0.78+1)*weight;
                    // } else {
                    //     weight_bb_up_alt = (0.6+1)*weight;
                    // }

                }
            }

            // // No scaling just unc.
            // float weight_isr_up_alt = weight;
            // if (isttW) {
            //     float w = isrWeight(year, ss::nisrMatch(), 1);
            //     weight_isr_up_alt *= w;
            // }
            // if (isttZ) {
            //     float w = isrWeight(year, ss::nisrMatch(), 2);
            //     weight_isr_up_alt *= w;
            // }
            // if (istt) {
            //     float w = isrWeight(year, ss::nisrMatch(), 10);
            //     weight_isr_up_alt *= w;
            // }

            // // ttbb bb scaling for HF
            // // scale up weight by 70% and take 85% of that as bb_up
            // float weight_bb_up_alt = weight;
            // if ((isttZ || isttW || isttH)) {
            //     if (ss::extragenb() >= 2) {
            //         weight_bb_up_alt = (0.6+1)*weight;
            //     }
            // }

#endif

            // float weight_bb_up_alt = weight;
            // if ((isttZ || isttW)) {
            //     if (ss::extragenb() == 2) {
            //         weight_bb_up_alt = (0.35+1)*weight;
            //     }
            // }


            // Done modifying weights, so consider up and down variations

            // Trigger stuff
            float weight_trigger_up_alt = weight;
            float weight_trigger_dn_alt = weight;

            // Prefire stuff
            float weight_prefire_up_alt = weight;
            float weight_prefire_dn_alt = weight;

            if (ss::is_real_data()==0) {

                // Prefire
                weight_prefire_up_alt = weight*ss::prefire_sfup()/ss::prefire_sf();
                weight_prefire_dn_alt = weight*ss::prefire_sfdown()/ss::prefire_sf();

                // Trigger
                weight_trigger_up_alt = weight;
                weight_trigger_dn_alt = weight;
                if ((isSS and (categ==Multilepton)) || (!isSS and lep3good and lep3ccpt>20)) {
                    weight_trigger_up_alt = weight*1.02;
                    weight_trigger_dn_alt = weight/1.02;
                } else {
                    if (year == 2016 and analysis == FTANA) nomtrigsf = triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht(), analysis, 2);
                    if (nomtrigsf >= 1.e-4) {
                        weight_trigger_up_alt = weight/nomtrigsf*triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht(), analysis, 1);
                        weight_trigger_dn_alt = weight/nomtrigsf*triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht(), analysis, -1);
                    }
                }

            }

            // // FIXME
            // if (isFakes) weight *= 1.1887430404510724;
            // if (isFlips) weight *= 1.034058340048447;
            // if (isRares) weight *= 1.0520979051157986;
            // if (isttH) weight *= 1.067226056010255;
            // if (isttVV) weight *= 1.0331088148013614;
            // if (isttW) weight *= 1.3331521252844587;
            // if (isttZ) weight *= 1.3291216173351943;
            // if (isXgamma) weight *= 1.0394166879436677;



            float weight_alt_FR = weight;
            float weight_btag_up_alt = weight;
            float weight_btag_dn_alt = weight;
            float weight_btagheavy_up_alt = weight;
            float weight_btagheavy_dn_alt = weight;
            float weight_btaglight_up_alt = weight;
            float weight_btaglight_dn_alt = weight;
            float weight_pu_up_alt = weight;
            float weight_pu_dn_alt = weight;
            float weight_lep_up_alt = weight;
            if (ss::is_real_data()==0) {
                // weight_btag_up_alt = ss::weight_btagsf()>0 ? weight*ss::weight_btagsf_UP()/ss::weight_btagsf() : weight;
                // weight_btag_dn_alt = ss::weight_btagsf()>0 ? weight*ss::weight_btagsf_DN()/ss::weight_btagsf() : weight;
                weight_btagheavy_up_alt = ss::weight_btagsf()>0 ? weight*ss::weight_btagsf_heavy_UP()/ss::weight_btagsf() : weight;
                weight_btagheavy_dn_alt = ss::weight_btagsf()>0 ? weight*ss::weight_btagsf_heavy_DN()/ss::weight_btagsf() : weight;
                weight_btaglight_up_alt = ss::weight_btagsf()>0 ? weight*ss::weight_btagsf_light_UP()/ss::weight_btagsf() : weight;
                weight_btaglight_dn_alt = ss::weight_btagsf()>0 ? weight*ss::weight_btagsf_light_DN()/ss::weight_btagsf() : weight;
                weight_pu_up_alt = getTruePUw(year, ss::trueNumInt()[0], 0)>0 ? weight*getTruePUw(year, ss::trueNumInt()[0], 1)/getTruePUw(year, ss::trueNumInt()[0], 0) : weight;
                weight_pu_dn_alt = getTruePUw(year, ss::trueNumInt()[0], 0)>0 ? weight*getTruePUw(year, ss::trueNumInt()[0],-1)/getTruePUw(year, ss::trueNumInt()[0], 0) : weight;
                if (
#ifdef SSLOOP
                        categ != Multilepton
#else
                        nleps == 2
#endif
                        ) {
                    weight_lep_up_alt = weight*(1.0+leptonScaleFactorError(year,lep1id,  lep1pt,  lep1eta,  ss::ht(), analysis)+leptonScaleFactorError(year,lep2id,  lep2pt,  lep2eta,  ss::ht(), analysis));
                } else {
                    weight_lep_up_alt = weight*(1.0+leptonScaleFactorError(year,lep1id,  lep1pt,  lep1eta,  ss::ht(), analysis)+leptonScaleFactorError(year,lep2id,  lep2pt,  lep2eta,  ss::ht(), analysis)+leptonScaleFactorError(year,lep3id,  lep3pt,  lep3eta,  ss::ht(), analysis));
                }
            }

            // if (fabs(weight) < 1.0e-7) {
            //     std::cout << "weight: " << weight <<  " nleps: " << nleps <<  " lep1id: " << lep1id <<  " lep2id: " << lep2id <<  " lep1pt: " << lep1pt <<  " lep2pt: " << lep2pt <<  " lep1eta: " << lep1eta <<  " lep2eta: " << lep2eta <<  " ss::ht(): " << ss::ht() <<  " ss::trueNumInt()[0]: " << ss::trueNumInt()[0] <<  std::endl;
            //     std::cout << "   lumiweight: " << ss::scale1fb() << std::endl;
            //     std::cout << "   ccpts: " << lep1ccpt << ", " << lep2ccpt << ", " << lep3ccpt << std::endl;
            //     std::cout << "   lepsf1: " << leptonScaleFactor(year, lep1id, lep1ccpt, lep1eta, ss::ht()) << std::endl;
            //     std::cout << "   lepsf2: " << leptonScaleFactor(year, lep2id, lep2ccpt, lep2eta, ss::ht()) << std::endl;
            //     std::cout << "   lepsf3: " << leptonScaleFactor(year, lep3id, lep3ccpt, lep3eta, ss::ht()) << std::endl;
            //     std::cout << "   trigsf: " << triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht()) << std::endl;
            //     std::cout << "   btagsf: " << ss::weight_btagsf() << std::endl;
            //     std::cout << "   numint: " << ss::trueNumInt()[0] << std::endl;
            //     std::cout << "   puw: " << getTruePUw(year, ss::trueNumInt()[0], 0) << std::endl;
            //     std::cout << "   decayWSF: " << ss::decayWSF() << std::endl;
            // }

            //Progress
            // SSAG::progress(nEventsTotal, nEventsChain);

            // Only keep good events
            int ssclass = 3;
            bool isClass6 = ss::hyp_class() == 6;
#ifdef SSLOOP
            if (!doFlips && !doFakes) {
                if (ss::hyp_class() != ssclass && !isClass6) continue;
                if (!truthfake and !skipmatching) {
                    if (!isData && !isGamma && ss::lep1_motherID()==2) continue;
                    if (!isData && !isGamma && ss::lep2_motherID()==2) continue;
                    if (!isData && !( (ss::lep1_motherID()==1 && ss::lep2_motherID()==1) || (ss::lep1_motherID()==-3 || ss::lep2_motherID()==-3)) ) continue;
                    if (categ == Multilepton) {
                        if (!isData && !isGamma && !(ss::lep3_motherID()==1 || ss::lep3_motherID()==-3)) continue;
                    }
                }
            }
            if (doFlips and isSS and (categ == Multilepton)) continue; // no flips for multilepton regions
            if (isWW and isSS and (categ == Multilepton)) continue; // no ww for multilepton regions
#else
            if (!doFlips && !doFakes && exclude == 0 && !truthfake) {
                if (ss::hyp_class() != ssclass && !isClass6) continue;
                if (!isData && !isGamma && ss::lep1_motherID()==2) continue;
                if (!isData && !isGamma && ss::lep2_motherID()==2) continue;
                if (!isData && !( (ss::lep1_motherID()==1 && ss::lep2_motherID()==1) || (ss::lep1_motherID()==-3 || ss::lep2_motherID()==-3)) ) continue;
            }
#endif

            // Don't take leptons from photons if not x-gamma
            if (!skipmatching && (ss::lep1_motherID()==-3 || ss::lep2_motherID()==-3) && !isGamma) {
                continue;
            }

            if (doFlips) {
                if (ss::hyp_class() != 4 && !isClass6) continue;
                float flipFact = 0.;
                if (abs(lep1id)==11){
                    float flr = flipRate(year,lep1pt, lep1eta, analysis);
                    flipFact += (flr/(1-flr));
                }
                if (abs(lep2id)==11){
                    float flr = flipRate(year,lep2pt, lep2eta, analysis);
                    flipFact += (flr/(1-flr));
                }
                weight *= flipFact;
                if (weight == 0.0) continue; // just quit if there are no flips.
            }

            if (truthfake) {
                if (ss::hyp_class() == ssclass) {
                    int nbadlegs = (ss::lep1_motherID() <= 0) + (ss::lep2_motherID() <= 0);
#ifdef SSLOOP
                    if (categ == Multilepton) nbadlegs += (ss::lep3_motherID() <= 0);
#else
                    if (nleps > 2) nbadlegs += (ss::lep3_motherID() <= 0);
#endif
                    // if it's MC and we're truthfake, then skip the event if it's 
                    // truth matched to be prompt prompt. we only want reco tight-tight
                    // events that are prompt-nonprompt (or nonprompt nonprompt)
                    if (!isData && (nbadlegs == 0)) continue;
                } else if (isClass6) {
                    int nbadlegs = (ss::lep1_motherID() <= 0) + (ss::lep2_motherID() <= 0) + (ss::lep3_motherID() <= 0);
                    if (!isData && (nbadlegs == 0)) continue;
                } else {
                    continue;
                }
            }

            bool fake_is_mu = false;
            bool fake_is_highpt = false;

            //QCD Fakes
            bool isClass6Fake = false;
            if (doFakes and doStitch) {
                if (!ss::passfilter()) continue;
            }
            if (doFakes == 1) {
                if (!isClass6) {
                    if (ss::hyp_class() == 3
#ifdef SSLOOP
                            and (categ == Multilepton)
#else
#endif
                            ) {
                        if (lep1good && lep2good && !lep3good && lep3pt>min_pt_fake
#ifdef SSLOOP
#else
                                && (lep3ccpt>20) 
#endif
                                ) {  // lep3 fake
                            float fr = fakeRate(year, lep3id, lep3ccpt, lep3eta, ss::ht(), analysis, new2016FRBins, !minPtFake18);
                            float fra = alternativeFakeRate(year, lep3id, lep3ccpt, lep3eta, ss::ht(), analysis, new2016FRBins, !minPtFake18);
                            if (!ignoreFakeFactor) weight *= fr / (1-fr);
                            if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                            // XXX now we bump nleps to 3 since this is a 3 lepton fake event
                            nleps = 3;
                            fake_is_highpt = (lep3ccpt > 35);
                            fake_is_mu = (abs(lep3id) == 13);
                        } else {
                            continue;
                        }
                    } else {
                        if (ss::hyp_class() != 2 && ss::hyp_class() != 1) continue;
                        bool foundGoodLoose = false;
                        if (!lep1good && lep1pt>min_pt_fake) {
                            float fr = fakeRate(year, lep1id, lep1ccpt, lep1eta, ss::ht(), analysis, new2016FRBins, !minPtFake18, categ==LowLow);
                            float fra = alternativeFakeRate(year, lep1id, lep1ccpt, lep1eta, ss::ht(), analysis, new2016FRBins, !minPtFake18, categ==LowLow);
                            if (!ignoreFakeFactor) weight *= fr/(1.-fr);
                            if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                            foundGoodLoose = true;
                            fake_is_highpt = (lep1ccpt > 35);
                            fake_is_mu = (abs(lep1id) == 13);
                        }
                        if (!lep2good && lep2pt>min_pt_fake) {
                            float fr = fakeRate(year, lep2id, lep2ccpt, lep2eta, ss::ht(), analysis, new2016FRBins, !minPtFake18, categ==LowLow);
                            float fra = alternativeFakeRate(year, lep2id, lep2ccpt, lep2eta, ss::ht(), analysis, new2016FRBins, !minPtFake18, categ==LowLow);
                            if (!ignoreFakeFactor) weight *= fr/(1.-fr);
                            if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                            foundGoodLoose = true;
                            fake_is_highpt = (lep2ccpt > 35);
                            fake_is_mu = (abs(lep2id) == 13);
                        }
                        if (!foundGoodLoose) continue;
                        if (ss::hyp_class() == 1 && lep1pt>min_pt_fake && lep2pt>min_pt_fake) weight *= -1.;
#ifdef SSLOOP
#else
                        if (!ss::is_real_data() && isData) {
                            weight *= -1.;
                        }
#endif
                    }
                } 
#ifdef SSLOOP
                else if (categ == Multilepton) 
#else
                else
#endif
                {

                    bool lep1_lowpt_veto = lep1pt < (abs(lep1id) == 11 ? 15 : 10);
                    bool lep2_lowpt_veto = lep2pt < (abs(lep2id) == 11 ? 15 : 10);
                    bool lep3_lowpt_veto = lep3pt < (abs(lep3id) == 11 ? 15 : 10);

                    int nClass6Fakes = 0;
                    if (ss::lep3_fo() and !ss::lep3_tight() and !lep3_lowpt_veto and lep1good and lep2good && lep3pt>min_pt_fake) {  // lep3 fake
                        float fr = fakeRate(year, lep3id, lep3ccpt, lep3eta, ss::ht(), analysis, new2016FRBins, !minPtFake18);
                        float fra = alternativeFakeRate(year, lep3id, lep3ccpt, lep3eta, ss::ht(), analysis, new2016FRBins, !minPtFake18);
                        isClass6Fake = true;
                        nClass6Fakes++;
                        if (!ignoreFakeFactor) weight *= fr / (1-fr);
                        if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                        fake_is_highpt = (lep3ccpt > 35);
                        fake_is_mu = (abs(lep3id) == 13);
                    }
                    if (ss::lep2_fo() and !ss::lep2_tight() and !lep2_lowpt_veto and lep1good and lep3good && lep2pt>min_pt_fake) {  // lep2 fake
                        float fr = fakeRate(year, lep2id, lep2ccpt, lep2eta, ss::ht(), analysis, new2016FRBins, !minPtFake18);
                        float fra = alternativeFakeRate(year, lep2id, lep2ccpt, lep2eta, ss::ht(), analysis, new2016FRBins, !minPtFake18);
                        isClass6Fake = true;
                        nClass6Fakes++;
                        if (!ignoreFakeFactor) weight *= fr / (1-fr);
                        if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                        fake_is_highpt = (lep2ccpt > 35);
                        fake_is_mu = (abs(lep2id) == 13);
                    }
                    if (ss::lep1_fo() and !ss::lep1_tight() and !lep1_lowpt_veto and lep2good and lep3good && lep1pt>min_pt_fake) {  // lep1 fake
                        float fr = fakeRate(year, lep1id, lep1ccpt, lep1eta, ss::ht(), analysis, new2016FRBins, !minPtFake18);
                        float fra = alternativeFakeRate(year, lep1id, lep1ccpt, lep1eta, ss::ht(), analysis, new2016FRBins, !minPtFake18);
                        isClass6Fake = true;
                        nClass6Fakes++;
                        if (!ignoreFakeFactor) weight *= fr / (1-fr);
                        if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                        fake_is_highpt = (lep1ccpt > 35);
                        fake_is_mu = (abs(lep1id) == 13);
                    }
                    // subtract double fakes, add triple fakes: x = (f1*LTT + f2*TLT + f3*TTL - f1*f2*LLT - f2*f3*TLL - f1*f3*LTL + f1*f2*f3*LLL) /. {LTT -> LLT*f2, TTL -> TLL*f2, LTL -> LLL*f2}
                    if (nClass6Fakes == 2) weight *= -1;
                }
#ifdef SSLOOP
                // Real data fakes chain has prompt MC too (ttV,ttH)...we want to subtract out such prompt contributions
                if (!ss::is_real_data() && isData) {
                    weight *= -1.;
                    weight_alt_FR *= -1.;
                }
#else
#endif
            }


            float mtnonz = mtmin;
            float mtnonz_unc_up = mtmin_unc_up;
            float mtnonz_unc_dn = mtmin_unc_dn;
            float zmass = -1;
            if (isClass6) {
                float zmass23 = lep2id == -lep3id ? (ss::lep2_p4()+ss::lep3_p4()).mass() : -999.0;
                float zmass31 = lep3id == -lep1id ? (ss::lep3_p4()+ss::lep1_p4()).mass() : -999.0;
                if (fabs(zmass31 - 91.2) < fabs(zmass23 - 91.2)) {
                    zmass = zmass31;
                    mtnonz = calcMT(lep2ccpt, ss::lep2_p4().phi(), ss::met(), ss::metPhi());
                    mtnonz_unc_up = calcMT(lep2ccpt, ss::lep2_p4().phi(), met_unc_up, metPhi_unc_up);
                    mtnonz_unc_dn = calcMT(lep2ccpt, ss::lep2_p4().phi(), met_unc_dn, metPhi_unc_dn);
                } else {
                    zmass = zmass23;
                    mtnonz = calcMT(lep1ccpt, ss::lep1_p4().phi(), ss::met(), ss::metPhi());
                    mtnonz_unc_up = calcMT(lep1ccpt, ss::lep1_p4().phi(), met_unc_up, metPhi_unc_up);
                    mtnonz_unc_dn = calcMT(lep1ccpt, ss::lep1_p4().phi(), met_unc_dn, metPhi_unc_dn);
                }
                if (fabs(zmass-91.2) > 15) continue;
#ifdef SSLOOP
                if (doFakes) {
                    // if (isData) {
                    //     if (!ss::is_real_data()) continue;
                    // }
                    if (!isClass6Fake) continue;
                }
#else
                if (doFakes && isData) {
                    if (!ss::is_real_data()) continue;
                    if (!isClass6Fake) continue;
                }
#endif
                if( !( isClass6Fake || (lep1good && lep2good && lep3good) ) ) continue;
            }
#ifdef SSLOOP
            if ((categ != Multilepton) && isClass6) continue;
#else
            //
#endif

            if (!skipmatching && !doFlips && !doFakes && exclude == 0) {
#ifdef SSLOOP
                if (categ == Multilepton) {
                    if (!isData && !isGamma && ss::lep3_motherID()==2) continue;
                }
#else
                if (nleps > 2) {
                    if (!isData && !isGamma && ss::lep3_motherID()==2) continue;
                }
#endif
            }

            // if all 3 charges are the same, throw the event away
#ifdef SSLOOP
            if (categ == Multilepton and ((lep1id>0 and lep2id>0 and lep3id>0) or
                               (lep1id<0 and lep2id<0 and lep3id<0))) continue;
#else
            if (nleps > 2 and ((lep1id>0 and lep2id>0 and lep3id>0) or
                               (lep1id<0 and lep2id<0 and lep3id<0))) continue;
#endif

            //Reject duplicates (after selection otherwise flips are ignored...)
            if (isData && ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)){ continue; }
            }

            float mll = (ss::lep1_p4()*lep1ccpt/lep1pt+ss::lep2_p4()*lep2ccpt/lep2pt).M();

            if (isSS) {
                if (categ == LowMet) {
                    // veto ee Z peak for low met regions
                    if (mll > 76 and mll < 106 and abs(lep1id)==11 and abs(lep2id)==11) continue;
                }
            }

            float pt1 = lep1ccpt;
            float pt2 = lep2ccpt;
            float pt3 = lep3ccpt;
            float pto1 = max(max(pt1,pt2),pt3);
            float pto3 = min(min(pt1,pt2),pt3);
            float pto2 = pt1+pt2+pt3-pto1-pto3;
            int mytype = ss::hyp_type();
            if (mytype==2 && abs(lep1id)==13) mytype = 1;

#ifdef SSLOOP
            int SR = signal_region_ss(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt, nleps, isClass6, mtnonz);
            int SRgenmet = signal_region_ss(ss::njets(), ss::nbtags(), ss::gen_met(), ss::ht(), mtmin, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt, nleps, isClass6, mtnonz);
            categ_genmet = analysis_category_ss(ss::lep1_id(), ss::lep2_id(), lep1ccpt, lep2ccpt, lep3ccpt, nleps, ss::ht(), ss::gen_met());
#else
            int SR = (passbr_nom ? signal_region_ft(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::mtmin(), lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt, nleps, isClass6) : -1);
            int SRgenmet = signal_region_ft(ss::njets(), ss::nbtags(), ss::gen_met(), ss::ht(), ss::mtmin(), lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt, nleps, isClass6);
#endif

#ifdef SSLOOP
            // if (
            //         !std::isfinite(ss::met_unc_up()) or !std::isfinite(ss::met_unc_dn()) or
            //         !std::isfinite(ss::ht_unc_up()) or !std::isfinite(ss::ht_unc_dn()) or
            //         !std::isfinite(ss::met_JER_up()) or !std::isfinite(ss::met_JER_dn()) or
            //         !std::isfinite(ss::ht_JER_up()) or !std::isfinite(ss::ht_JER_dn())
            //         ) {
            //     std::cout << "Event, run, lumi, sample has nonfinite met/ht variations. Skipping. " << ss::event() << " " << ss::run() << " " << ss::lumi() << " " << chainTitle << " " 
            //         << ss::met_unc_up() << " " << ss::met_unc_dn() << " "
            //         << ss::ht_unc_up() << " " << ss::ht_unc_dn() << " "
            //         << ss::met_JER_up() << " " << ss::met_JER_dn() << " "
            //         << ss::ht_JER_up() << " " << ss::ht_JER_dn() 
            //         << std::endl;
            //     continue;
            // }
            // JEC can migrate between low and high MET categories (HH and LM)
            int SR_unc_up = signal_region_ss(njets_unc_up, nbtags_unc_up, met_unc_up, ht_unc_up, mtmin_unc_up, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6, mtnonz_unc_up);
            int SR_unc_dn = signal_region_ss(njets_unc_dn, nbtags_unc_dn, met_unc_dn, ht_unc_dn, mtmin_unc_dn, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6, mtnonz_unc_dn);
#else
            int SR_unc_up = (passbr_unc_up ? signal_region_ft(njets_unc_up, nbtags_unc_up, met_unc_up, ht_unc_up, mtmin_unc_up, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6) : -1);
            int SR_unc_dn = (passbr_unc_dn ? signal_region_ft(njets_unc_dn, nbtags_unc_dn, met_unc_dn, ht_unc_dn, mtmin_unc_dn, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6) : -1);
#endif

            // JER
            float mtl1_JER_up = 0.;
            float mtl2_JER_up = 0.;
            float mtmin_JER_up = 0.;
            float mtl1_JER_dn = 0.;
            float mtl2_JER_dn = 0.;
            float mtmin_JER_dn = 0.;
            met_JER_up = ((std::isfinite(met_JER_up) and (met_JER_up < 5e3)) ? met_JER_up : ss::met());
            ht_JER_up = ((std::isfinite(ht_JER_up) and (ht_JER_up < 5e3)) ? ht_JER_up : ss::ht());
            int SR_JER_up = SR;
            int SR_JER_dn = SR;
            if (!ss::is_real_data()) {
                mtl1_JER_up = calcMT(lep1ccpt, ss::lep1_p4().phi(), met_JER_up  , metPhi_JER_up);
                mtl2_JER_up = calcMT(lep2ccpt, ss::lep2_p4().phi(), met_JER_up  , metPhi_JER_up);
                mtmin_JER_up = mtl1_JER_up > mtl2_JER_up ? mtl2_JER_up : mtl1_JER_up;
                mtl1_JER_dn = calcMT(lep1ccpt, ss::lep1_p4().phi(), met_JER_dn, metPhi_JER_dn);
                mtl2_JER_dn = calcMT(lep2ccpt, ss::lep2_p4().phi(), met_JER_dn, metPhi_JER_dn);
                mtmin_JER_dn = mtl1_JER_dn > mtl2_JER_dn ? mtl2_JER_dn : mtl1_JER_dn;
#ifdef SSLOOP
                SR_JER_up = signal_region_ss(njets_JER_up, nbtags_JER_up,     met_JER_up, ht_JER_up, mtmin_JER_up, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6, mtnonz);
                SR_JER_dn = signal_region_ss(njets_JER_dn, nbtags_JER_dn,     met_JER_dn, ht_JER_dn, mtmin_JER_dn, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6, mtnonz);
                // JER can migrate between low and high MET categories (HH and LM)
                categ_JER_up = analysis_category_ss(lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt, nleps, ht_JER_up, met_JER_up);
                categ_JER_dn = analysis_category_ss(lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt, nleps, ht_JER_dn, met_JER_dn);
#else
                SR_JER_up = (passbr_JER_up ? signal_region_ft(njets_JER_up, nbtags_JER_up,     met_JER_up  , ht_JER_up, mtmin_JER_up, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6) : -1);
                SR_JER_dn = (passbr_JER_up ? signal_region_ft(njets_JER_dn, nbtags_JER_dn,     met_JER_dn  , ht_JER_dn, mtmin_JER_dn, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6) : -1);
#endif
            }

            float mvavalue = -10.;
            float mvavalueup = -10.;
            float mvavaluedn = -10.;
            float mvavalueJERup = -10.;
            float mvavalueJERdn = -10.;
            float pred = -1.;
            if (evaluateBDT and not isSS) {

                float f_nbtags = ss::bdt_nbtags();
                float f_njets = ss::bdt_njets();
                float f_met = ss::bdt_met();
                float f_ptl2 = ss::bdt_ptl2();
                float f_nlb40 = ss::bdt_nlb40();
                float f_ntb40 = ss::bdt_ntb40();
                float f_nleps = ss::bdt_nleps();
                float f_htb = ss::bdt_htb();
                float f_q1 = ss::bdt_q1();
                float f_ptj1 = ss::bdt_ptj1();
                float f_ptj6 = ss::bdt_ptj6();
                float f_ptj7 = ss::bdt_ptj7();
                float f_ml1j1 = ss::bdt_ml1j1();
                float f_dphil1l2 = ss::bdt_dphil1l2();
                float f_maxmjoverpt = ss::bdt_maxmjoverpt();
                float f_ptl1 = ss::bdt_ptl1();
                float f_detal1l2 = ss::bdt_detal1l2();
                float f_ptj8 = ss::bdt_ptj8();
                float f_ptl3 = ss::bdt_ptl3();

                bool dotestbdt = false;
                // if (dotestbdt) mvavalue = testbdt::get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                // else mvavalue = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                mvavalue = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);

                f_nbtags = ss::bdt_jec_up_nbtags();
                f_njets = ss::bdt_jec_up_njets();
                f_met = ss::bdt_jec_up_met();
                f_htb = ss::bdt_jec_up_htb();
                f_nlb40 = ss::bdt_jec_up_nlb40();
                f_ntb40 = ss::bdt_jec_up_ntb40();
                // if (dotestbdt) mvavalueup = testbdt::get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                // else mvavalueup = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                mvavalueup = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);

                f_nbtags = ss::bdt_jec_dn_nbtags();
                f_njets = ss::bdt_jec_dn_njets();
                f_met = ss::bdt_jec_dn_met();
                f_htb = ss::bdt_jec_dn_htb();
                f_nlb40 = ss::bdt_jec_dn_nlb40();
                f_ntb40 = ss::bdt_jec_dn_ntb40();
                // if (dotestbdt) mvavaluedn = testbdt::get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                // else mvavaluedn = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                mvavaluedn = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);

                f_nbtags = ss::bdt_jer_up_nbtags();
                f_njets = ss::bdt_jer_up_njets();
                f_met = ss::bdt_jer_up_met();
                f_htb = ss::bdt_jer_up_htb();
                f_nlb40 = ss::bdt_jer_up_nlb40();
                f_ntb40 = ss::bdt_jer_up_ntb40();
                // if (dotestbdt) mvavalueJERup = testbdt::get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                // else mvavalueJERup = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                mvavalueJERup = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);

                f_nbtags = ss::bdt_jer_dn_nbtags();
                f_njets = ss::bdt_jer_dn_njets();
                f_met = ss::bdt_jer_dn_met();
                f_htb = ss::bdt_jer_dn_htb();
                f_nlb40 = ss::bdt_jer_dn_nlb40();
                f_ntb40 = ss::bdt_jer_dn_ntb40();
                // if (dotestbdt) mvavalueJERdn = testbdt::get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                // else mvavalueJERdn = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                mvavalueJERdn = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);

                p_result.h_disc.br->Fill(mvavalue,weight);
            }

#ifdef SSLOOP
            // int SRdisc =        getBDTBin(nbdtbins, mvavalue, SR==1);
            // int SRdisc_unc_up = getBDTBin(nbdtbins, mvavalueup, SR_unc_up==1);
            // int SRdisc_unc_dn = getBDTBin(nbdtbins, mvavaluedn, SR_unc_dn==1);
            // int SRdisc_JER_up = getBDTBin(nbdtbins, mvavalueJERup, SR_JER_up==1);
            // int SRdisc_JER_dn = getBDTBin(nbdtbins, mvavalueJERdn, SR_JER_dn==1);
#else
            int SRdisc =        (passbr_nom    ? getBDTBin(nbdtbins, mvavalue, SR==1) : -1);
            int SRdisc_unc_up = (passbr_unc_up ? getBDTBin(nbdtbins, mvavalueup, SR_unc_up==1) : -1);
            int SRdisc_unc_dn = (passbr_unc_up ? getBDTBin(nbdtbins, mvavaluedn, SR_unc_dn==1) : -1);
            int SRdisc_JER_up = (passbr_JER_up ? getBDTBin(nbdtbins, mvavalueJERup, SR_JER_up==1) : -1);
            int SRdisc_JER_dn = (passbr_JER_up ? getBDTBin(nbdtbins, mvavalueJERdn, SR_JER_dn==1) : -1);

            // if (ss::is_real_data() and doFakes and SR==10) {
            //     float bpt1 = ((ss::btags().size() > 0) ? ss::btags()[0].pt() : -1);
            //     float bpt2 = ((ss::btags().size() > 1) ? ss::btags()[1].pt() : -1);
            //     float bpt3 = ((ss::btags().size() > 2) ? ss::btags()[2].pt() : -1);
            //     float bpt4 = ((ss::btags().size() > 3) ? ss::btags()[3].pt() : -1);
            //     float bpt5 = ((ss::btags().size() > 4) ? ss::btags()[4].pt() : -1);
            //     std::cout <<  " ss::run(): " << ss::run() <<  " ss::lumi(): " << ss::lumi() <<  " ss::event(): " << ss::event() <<  " nleps: " << nleps <<  " SRdisc: " << SRdisc <<  " lep1ccpt: " << lep1ccpt <<  " lep2ccpt: " << lep2ccpt <<  " lep1eta: " << lep1eta <<  " lep2eta: " << lep2eta <<  " lep1id: " << lep1id <<  " lep2id: " << lep2id <<  " lep1phi: " << lep1phi <<  " lep2phi: " << lep2phi <<  " mtmin: " << mtmin <<  " ss::njets(): " << ss::njets() <<  " ss::nbtags(): " << ss::nbtags() <<  " ss::met(): " << ss::met() <<  " ss::ht(): " << ss::ht() <<  " year: " << year <<  " bpt1: " << bpt1 <<  " bpt2: " << bpt2 <<  " bpt3: " << bpt3 <<  " bpt4: " << bpt4 <<  " bpt5: " << bpt5 <<  " ss::hyp_class(): " << ss::hyp_class() <<  " weight: " << weight <<  std::endl;
            // }
            // if (SRdisc > 15) {
            //     if (ss::is_real_data() and !doFakes and !doFlips) {
            //         float bpt1 = ((ss::btags().size() > 0) ? ss::btags()[0].pt() : -1);
            //         float bpt2 = ((ss::btags().size() > 1) ? ss::btags()[1].pt() : -1);
            //         float bpt3 = ((ss::btags().size() > 2) ? ss::btags()[2].pt() : -1);
            //         float bpt4 = ((ss::btags().size() > 3) ? ss::btags()[3].pt() : -1);
            //         float bpt5 = ((ss::btags().size() > 4) ? ss::btags()[4].pt() : -1);
            //         // ss::run() ss::lumi() ss::event() nleps SRdisc lep1ccpt lep2ccpt lep1eta lep2eta lep1id lep2id lep1phi lep2phi mtmin ss::njets() ss::nbtags() ss::met() ss::ht() year bpt1 bpt2 bpt3 bpt4 bpt5 
            //         float miniiso1 = ss::lep1_miniIso();
            //         float ptrel1 = ss::lep1_ptrel_v1();
            //         float ptratio1 = ss::lep1_ptratio();
            //         float sip1 = ss::lep1_sip();
            //         float miniiso2 = ss::lep2_miniIso();
            //         float ptrel2 = ss::lep2_ptrel_v1();
            //         float ptratio2 = ss::lep2_ptratio();
            //         float sip2 = ss::lep2_sip();
            //         std::cout <<  " ss::run(): " << ss::run() <<  " ss::lumi(): " << ss::lumi() <<  " ss::event(): " << ss::event() <<  " nleps: " << nleps <<  " SRdisc: " << SRdisc <<  " lep1ccpt: " << lep1ccpt <<  " lep2ccpt: " << lep2ccpt <<  " lep1eta: " << lep1eta <<  " lep2eta: " << lep2eta <<  " lep1id: " << lep1id <<  " lep2id: " << lep2id <<  " lep1phi: " << lep1phi <<  " lep2phi: " << lep2phi <<  " mtmin: " << mtmin <<  " ss::njets(): " << ss::njets() <<  " ss::nbtags(): " << ss::nbtags() <<  " ss::met(): " << ss::met() <<  " ss::ht(): " << ss::ht() <<  " year: " << year <<  " bpt1: " << bpt1 <<  " bpt2: " << bpt2 <<  " bpt3: " << bpt3 <<  " bpt4: " << bpt4 <<  " bpt5: " << bpt5 <<  " miniiso1: " << miniiso1 <<  " ptrel1: " << ptrel1 <<  " ptratio1: " << ptratio1 <<  " sip1: " << sip1 <<  " miniiso2: " << miniiso2 <<  " ptrel2: " << ptrel2 <<  " ptratio2: " << ptratio2 <<  " sip2: " << sip2 <<  std::endl;
            //     }
            // }
            // if (SRdisc > 15) {
            //     if (ss::is_real_data() and !doFakes and !doFlips) {
            //         float miniiso1 = ss::lep1_miniIso();
            //         float ptrel1 = ss::lep1_ptrel_v1();
            //         float ptratio1 = ss::lep1_ptratio();
            //         float sip1 = ss::lep1_sip();
            //         float miniiso2 = ss::lep2_miniIso();
            //         float ptrel2 = ss::lep2_ptrel_v1();
            //         float ptratio2 = ss::lep2_ptratio();
            //         float sip2 = ss::lep2_sip();
            //         std::cout <<  " year: " << year <<  " ss::run(): " << ss::run() <<  " ss::lumi(): " << ss::lumi() <<  " ss::event(): " << ss::event() <<  " nleps: " << nleps <<  " SRdisc: " << SRdisc <<  " lep1ccpt: " << lep1ccpt <<  " lep2ccpt: " << lep2ccpt <<  " lep1eta: " << lep1eta <<  " lep2eta: " << lep2eta <<  " lep1id: " << lep1id <<  " lep2id: " << lep2id <<  " lep1phi: " << lep1phi <<  " lep2phi: " << lep2phi <<  " mtmin: " << mtmin <<  " ss::njets(): " << ss::njets() <<  " ss::nbtags(): " << ss::nbtags() <<  " ss::met(): " << ss::met() <<  " ss::ht(): " << ss::ht() <<  " miniiso1: " << miniiso1 <<  " ptrel1: " << ptrel1 <<  " ptratio1: " << ptratio1 <<  " sip1: " << sip1 <<  " miniiso2: " << miniiso2 <<  " ptrel2: " << ptrel2 <<  " ptratio2: " << ptratio2 <<  " sip2: " << sip2 <<  std::endl;
            //     }
            // }

#endif

#ifdef SSLOOP
            int SRmi1 = signal_region_mi1(met,ht);
            int SRmi2 = signal_region_mi2(met,ht);
            // std::vector<int> SRincl = { 2, 3, 5 };
            std::vector<int> SRincl = signal_region_incl(categ, ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin);
            if (isData == 0 && SR_unc_up > 0) p_result.p_jes_alt_up_SR.CatFill(categ_unc_up, SR_unc_up, weight, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR_unc_dn > 0) p_result.p_jes_alt_dn_SR.CatFill(categ_unc_dn, SR_unc_dn, weight, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR_JER_up > 0) p_result.p_jer_alt_up_SR.CatFill(categ_JER_up, SR_JER_up, weight, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR_JER_dn > 0) p_result.p_jer_alt_dn_SR.CatFill(categ_JER_dn, SR_JER_dn, weight, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SRgenmet > 0) p_result.p_met_alt_up_SR.CatFill(categ_genmet, SRgenmet, weight, SRmi1, SRmi2, SRincl);
            if (doFakes == 1 && SR >= 0) p_result.p_fake_alt_up_SR.CatFill(categ, SR, weight_alt_FR, SRmi1, SRmi2, SRincl);
            if (doFakes == 1 && SR >= 0) p_result.p_fake_nb0_up_SR.CatFill(categ, SR, weight*(ss::nbtags() == 0 ? 1.3 : 1), SRmi1, SRmi2, SRincl);
            if (doFakes == 1 && SR >= 0) p_result.p_fake_nb1_up_SR.CatFill(categ, SR, weight*(ss::nbtags() == 1 ? 1.3 : 1), SRmi1, SRmi2, SRincl);
            if (doFakes == 1 && SR >= 0) p_result.p_fake_nb2_up_SR.CatFill(categ, SR, weight*(ss::nbtags() == 2 ? 1.3 : 1), SRmi1, SRmi2, SRincl);
            if (doFakes == 1 && SR >= 0) p_result.p_fake_nb3_up_SR.CatFill(categ, SR, weight*(ss::nbtags() >= 3 ? 1.3 : 1), SRmi1, SRmi2, SRincl);


            if (doFakes == 1 && SR >= 0 && ss::is_real_data()) p_result.p_fake_unw_up_SR.CatFill(categ, SR, weight > 0 ? 1 : 0, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR >= 0) p_result.p_btagSF_alt_up_SR.CatFill(categ, SR, weight_btag_up_alt, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR >= 0) p_result.p_btagSF_alt_dn_SR.CatFill(categ, SR, weight_btag_dn_alt, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR >= 0) p_result.p_btagSFheavy_alt_up_SR.CatFill(categ, SR, weight_btagheavy_up_alt, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR >= 0) p_result.p_btagSFheavy_alt_dn_SR.CatFill(categ, SR, weight_btagheavy_dn_alt, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR >= 0) p_result.p_btagSFlight_alt_up_SR.CatFill(categ, SR, weight_btaglight_up_alt, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR >= 0) p_result.p_btagSFlight_alt_dn_SR.CatFill(categ, SR, weight_btaglight_dn_alt, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR >= 0) p_result.p_pu_alt_up_SR.CatFill(categ, SR, weight_pu_up_alt, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR >= 0) p_result.p_pu_alt_dn_SR.CatFill(categ, SR, weight_pu_dn_alt, SRmi1, SRmi2, SRincl);
            if (isFastsim && SR >= 0) p_result.p_isr_alt_up_SR.CatFill(categ, SR, weight_isr_up_alt, SRmi1, SRmi2, SRincl);
            if (isFastsim && SR >= 0) p_result.p_isr_alt_dn_SR.CatFill(categ, SR, weight_isr_dn_alt, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR >= 0) p_result.p_bb_alt_up_SR.CatFill(categ, SR, weight_bb_up_alt, SRmi1, SRmi2, SRincl);
            if (isData == 0 && SR >= 0) p_result.p_lep_alt_up_SR.CatFill(categ, SR, weight_lep_up_alt, SRmi1, SRmi2, SRincl);
            if (isData  == 0 && SR >= 0) p_result.p_prefire_alt_up_SR.CatFill(categ, SR, weight_prefire_up_alt, SRmi1, SRmi2, SRincl);
            if (isData  == 0 && SR >= 0) p_result.p_prefire_alt_dn_SR.CatFill(categ, SR, weight_prefire_dn_alt, SRmi1, SRmi2, SRincl);
            if (isData  == 0 && SR >= 0) p_result.p_trigger_alt_up_SR.CatFill(categ, SR, weight_trigger_up_alt, SRmi1, SRmi2, SRincl);
            if (isData  == 0 && SR >= 0) p_result.p_trigger_alt_dn_SR.CatFill(categ, SR, weight_trigger_dn_alt, SRmi1, SRmi2, SRincl);
            if (SR > 0) {
                p_result.p_scale_alt_up_SR.CatFill(categ, SR, (ss::weight_scale_UP() > -9000 ? ss::weight_scale_UP()/norm_scale_up : 1.0)*weight, SRmi1, SRmi2, SRincl);
                p_result.p_scale_alt_dn_SR.CatFill(categ, SR, (ss::weight_scale_DN() > -9000 ? ss::weight_scale_DN()/norm_scale_dn : 1.0)*weight, SRmi1, SRmi2, SRincl);
                p_result.p_alphas_alt_up_SR.CatFill(categ, SR, (ss::weight_alphas_UP() > -9000 ? ss::weight_alphas_UP() : 1.0)*weight, SRmi1, SRmi2, SRincl);
                p_result.p_alphas_alt_dn_SR.CatFill(categ, SR, (ss::weight_alphas_DN() > -9000 ? ss::weight_alphas_DN() : 1.0)*weight, SRmi1, SRmi2, SRincl);
                p_result.p_pdf_alt_up_SR.CatFill(categ, SR, (ss::weight_pdf_UP() > -9000 ? ss::weight_pdf_UP()/norm_pdf_up : 1.0)*weight, SRmi1, SRmi2, SRincl);
                p_result.p_pdf_alt_dn_SR.CatFill(categ, SR, (ss::weight_pdf_DN() > -9000 ? ss::weight_pdf_DN()/norm_pdf_dn : 1.0)*weight, SRmi1, SRmi2, SRincl);
                p_result.SR.CatFill(categ, SR, weight, SRmi1, SRmi2, SRincl);
            }

            // OR together all possible categories for this event
            int kcategs = kBaseline;
            if (categ == Multilepton && SR > 0) {
                kcategs |= kMultilepton;
                if (SR > 21) kcategs |= kMultileptonOnZ;
                else kcategs |= kMultileptonOffZ;
            }
            if (SR > 0) {
                kcategs |= kSignalRegion;
                if (categ == HighHigh) kcategs |= kHighHigh;
                else if (categ == HighLow) kcategs |= kHighLow;
                else if (categ == LowLow) kcategs |= kLowLow;
                else if (categ == LowMet) kcategs |= kLowMet;
            }

            // Only plot 3rd lepton if event is in ML region
            bool plotlep3 = categ == Multilepton;

            // if (ss::is_real_data() and !doFakes and !doFlips and categ == HighHigh and (SR==52 or SR==53)) {
            //     std::cout <<  " ss::run(): " << ss::run() <<  " ss::lumi(): " << ss::lumi() <<  " ss::event(): " << ss::event() <<  " nleps: " << nleps <<  " SR: " << SR <<  " categ: " << categ <<  " lep1ccpt: " << lep1ccpt <<  " lep2ccpt: " << lep2ccpt <<  " lep1eta: " << lep1eta <<  " lep2eta: " << lep2eta <<  " lep1id: " << lep1id <<  " lep2id: " << lep2id <<  " lep1phi: " << lep1phi <<  " lep2phi: " << lep2phi <<  " mtmin: " << mtmin <<  " ss::njets(): " << ss::njets() <<  " ss::nbtags(): " << ss::nbtags() <<  " ss::met(): " << ss::met() <<  " ss::ht(): " << ss::ht() <<  " year: " << year <<  " ss::hyp_class(): " << ss::hyp_class() <<  std::endl;
            // }
            
            // if (ss::is_real_data() and 
            //         // !doFakes and 
            //         !doFlips and (
            //             (categ == HighHigh and (SR==52 or SR==53 or SR==54)) or
            //             (categ == LowMet and (SR==7))
            //             )
            //    ) {
            //     // ss::run() ss::lumi() ss::event() nleps SR categ lep1ccpt lep2ccpt lep1eta lep2eta lep1id lep2id lep1phi lep2phi mtmin ss::njets() ss::nbtags() ss::met() ss::ht() year ss::hyp_class() 
            //     int iloose = 1*(!ss::lep1_passes_id()) + 2*(!ss::lep2_passes_id());
            //     std::string catstr = "HH";
            //     if (categ == LowMet) catstr = "LM";

            //     // std::cout << catstr << SR << " " << ss::run() <<  ":" << ss::lumi() <<  ":" << ss::event()
            //     //     << " year: " << year << " class " << ss::hyp_class() << " iloose " << iloose
            //     //     << " l1id: " << lep1id <<  " l2id: " << lep2id 
            //     //     << " l1pt: " << lep1ccpt <<  " l2pt: " << lep2ccpt 
            //     //     << " l1eta: " << lep1eta <<  " l2eta: " << lep2eta 
            //     //     <<  " l1phi: " << lep1phi <<  " l2phi: " << lep2phi 
            //     //     <<  " mtmin: " << mtmin <<  " njets: " << ss::njets() <<  " nbtags: " << ss::nbtags() <<  " met: " << ss::met() <<  " ht: " << ss::ht()
            //     //     <<  std::endl;

            //     std::cout << catstr << SR << " " << ss::run() <<  ":" << ss::lumi() <<  ":" << ss::event()
            //         << Form(" year: %d class: %d iloose: %d", year, ss::hyp_class(), iloose)
            //         << Form(" l1id: %+2d l2id: %+2d", lep1id, lep2id)
            //         << Form(" l1pt: %4.2f l2pt: %4.2f", lep1ccpt, lep2ccpt)
            //         << Form(" l1eta: %2.3f l2eta: %2.3f", lep1eta, lep2eta)
            //         << Form(" l1phi: %2.3f l2phi: %2.3f", lep1phi, lep2phi)
            //         << Form(" njets: %2d nbtags: %2d met: %5.1f ht: %5.1f mtmin: %5.1f", ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin)
            //         << std::endl;

            // }
            
            // // FIXME FIXME
            // if (categ == Multilepton and SR!=20) continue;
            // if (ss::is_real_data() and !doFakes and !doFlips and categ == Multilepton and (SR==20)) {
            //     std::cout <<  " ss::run(): " << ss::run() <<  " ss::lumi(): " << ss::lumi() <<  " ss::event(): " << ss::event() <<  " nleps: " << nleps <<  " SR: " << SR <<  " categ: " << categ <<  " lep1ccpt: " << lep1ccpt <<  " lep2ccpt: " << lep2ccpt <<  " lep1eta: " << lep1eta <<  " lep2eta: " << lep2eta <<  " lep1id: " << lep1id <<  " lep2id: " << lep2id <<  " lep1phi: " << lep1phi <<  " lep2phi: " << lep2phi <<  " mtmin: " << mtmin <<  " ss::njets(): " << ss::njets() <<  " ss::nbtags(): " << ss::nbtags() <<  " ss::met(): " << ss::met() <<  " ss::ht(): " << ss::ht() <<  " year: " << year <<  " ss::hyp_class(): " << ss::hyp_class() <<  std::endl;
            // }

            // if (categ == LowMet) {
            //     if (ss::is_real_data() and !doFakes and !doFlips and year==2016) {
            //         // std::cout <<  " run: " << ss::run() <<  " lumi: " << ss::lumi() <<  " event: " << ss::event() <<  " hyp_class: " << ss::hyp_class() <<  " SR: " << SR <<  " lep1ccpt: " << lep1ccpt <<  " lep2ccpt: " << lep2ccpt <<  " lep1pt: " << lep1pt <<  " lep2pt: " << lep2pt <<  " lep1eta: " << lep1eta <<  " lep2eta: " << lep2eta <<  " lep1id: " << lep1id <<  " lep2id: " << lep2id <<  " lep1good: " << lep1good <<  " lep2good: " << lep2good <<  " njets: " << ss::njets() <<  " nbtags: " << ss::nbtags() <<  " met: " << ss::met() <<  " ht: " << ss::ht() <<  std::endl;
            //     }
            // }


            // Fill based on categories
            p_result.h_l1pt.CatFill(kcategs, pto1, weight);
            p_result.h_l2pt.CatFill(kcategs, pto2, weight);
            p_result.h_l3pt.CatFill(kcategs, pto3, weight);
            if (abs(ss::lep1_id()) == 11) {
                p_result.h_el_l1eta.CatFill(kcategs, lep1eta, weight);
                p_result.h_el_l1pt.CatFill(kcategs, lep1ccpt, weight);
                p_result.h_el_l1phi.CatFill(kcategs, ss::lep1_p4().phi(), weight);
            } else {
                p_result.h_mu_l1eta.CatFill(kcategs, lep1eta, weight);
                p_result.h_mu_l1pt.CatFill(kcategs, lep1ccpt, weight);
                p_result.h_mu_l1phi.CatFill(kcategs, ss::lep1_p4().phi(), weight);
            }
            if (abs(ss::lep2_id()) == 11) {
                p_result.h_el_l2eta.CatFill(kcategs, lep2eta, weight);
                p_result.h_el_l2pt.CatFill(kcategs, lep2ccpt, weight);
                p_result.h_el_l2phi.CatFill(kcategs, ss::lep2_p4().phi(), weight);
            } else {
                p_result.h_mu_l2eta.CatFill(kcategs, lep2eta, weight);
                p_result.h_mu_l2pt.CatFill(kcategs, lep2ccpt, weight);
                p_result.h_mu_l2phi.CatFill(kcategs, ss::lep2_p4().phi(), weight);
            }
            if (plotlep3) {
                p_result.h_q3.CatFill(kcategs, lep3id > 0 ? -0.5 : 0.5, weight);
                if (abs(ss::lep3_id()) == 11) {
                    p_result.h_el_l3eta.CatFill(kcategs, lep3eta, weight);
                    p_result.h_el_l3pt.CatFill(kcategs, lep3ccpt, weight);
                } else {
                    p_result.h_mu_l3eta.CatFill(kcategs, lep3eta, weight);
                    p_result.h_mu_l3pt.CatFill(kcategs, lep3ccpt, weight);
                }
            }
            p_result.h_met.CatFill(kcategs, ss::met(), weight);
            p_result.h_ht.CatFill(kcategs, ss::ht(), weight);
            p_result.h_mll.CatFill(kcategs, mll, weight);
            p_result.h_mllbig.CatFill(kcategs, mll, weight);
            p_result.h_mllos.CatFill(kcategs, zmass, weight);

            p_result.h_lumiblock.CatFill(kcategs, (ss::is_real_data() ? ss::lumi() : 9999), weight);
            p_result.h_run.CatFill(kcategs, ss::run(), weight);

            float mt2 = -1;
            float mt2min = -1;
            if (categ == Multilepton) {
                if ((lep1id>0) == (lep3id>0)) {
                    // all the same sign? just put it in highest MT2 bin
                    mt2 =  300.;
                } else {
                    float mt2_13 = MT2(ss::met(), ss::metPhi(), ss::lep1_p4()*lep1ccpt/lep1pt, ss::lep3_p4()*lep3ccpt/lep3pt);
                    float mt2_23 = MT2(ss::met(), ss::metPhi(), ss::lep2_p4()*lep2ccpt/lep2pt, ss::lep3_p4()*lep3ccpt/lep3pt);
                    if (lep1ccpt > lep2ccpt) {
                        mt2 = mt2_13;
                    } else {
                        mt2 = mt2_23;
                    }
                    mt2min = min(mt2_13, mt2_23);
                }
            }
            p_result.h_mt2.CatFill(kcategs, mt2, weight);
            p_result.h_mt2min.CatFill(kcategs, mt2min, weight);
            p_result.h_mtmin.CatFill(kcategs, kcategs & kMultileptonOnZ ? mtnonz : mtmin, weight);
            p_result.h_nbtags.CatFill(kcategs, ss::nbtags(), weight);
            p_result.h_njets.CatFill(kcategs, ss::njets(), weight);
            p_result.h_nleps.CatFill(kcategs, nleps, weight);
            // p_result.h_nleptonicW.CatFill(kcategs, ss::nleptonicW(), weight);
            p_result.h_type.CatFill(kcategs, mytype, weight);
            p_result.h_class.CatFill(kcategs, ss::hyp_class(), weight);
            p_result.h_type3.CatFill(kcategs, (abs(lep1id)==11)+(abs(lep2id)==11)+(abs(lep3id)==11), weight);
            p_result.h_category.CatFill(kcategs,
                    1.0*(categ == HighHigh) +
                    2.0*(categ == HighLow) +
                    3.0*(categ == LowLow) +
                    4.0*((kcategs & kMultileptonOffZ)>0) +
                    5.0*((kcategs & kMultileptonOnZ)>0) +
                    6.0*(categ == LowMet),
                    weight);
            p_result.h_charge.CatFill(kcategs, lep1id > 0 ? -0.5 : 0.5, weight);
            p_result.h_dphi.CatFill(kcategs, calcDeltaPhi(lep1phi,lep2phi), weight);
            if (abs(lep1id) == 11) p_result.h_el_charge.CatFill(kcategs, lep1id > 0 ? -0.5 : 0.5, weight);
            if (abs(lep1id) == 13) p_result.h_mu_charge.CatFill(kcategs, lep1id > 0 ? -0.5 : 0.5, weight);
            if (abs(lep2id) == 11) p_result.h_el_charge.CatFill(kcategs, lep2id > 0 ? -0.5 : 0.5, weight);
            if (abs(lep2id) == 13) p_result.h_mu_charge.CatFill(kcategs, lep2id > 0 ? -0.5 : 0.5, weight);
            p_result.h_charge3.CatFill(kcategs, ((lep1id>0)^(lep3id>0))-0.5, weight); // 0.5 if 3 charges are SSO, -0.5 if SSS
            p_result.h_nvtx.CatFill(kcategs, ss::nGoodVertices(), weight);

            if (write_tree and SR > 0 and categ != Undefined) {
                output_tree.FillVars(
                        weight, SR, categ, ss::hyp_class(),
                        ss::njets(), ss::nbtags(), mtmin, ss::ht(), ss::met(),
                        ss::is_real_data(), lep1id, lep2id, lep3id,
                        lep1ccpt, lep2ccpt, lep3ccpt
                        );
            }

#else
            if (isData  == 0 && SR_unc_up > 0)            p_result.p_jes_alt_up_SR.CatFill(1, SR_unc_up, weight);
            if (isData  == 0 && SR_unc_dn > 0)            p_result.p_jes_alt_dn_SR.CatFill(1, SR_unc_dn, weight);

            if (isData  == 0 && SR_JER_up > 0)            p_result.p_jer_alt_up_SR.CatFill(1, SR_JER_up, weight);
            if (isData  == 0 && SR_JER_dn > 0)            p_result.p_jer_alt_dn_SR.CatFill(1, SR_JER_dn, weight);

            if (isData  == 0 && SRdisc_unc_up > 0)            p_result.p_jes_alt_up_SR.CatFill(2, SRdisc_unc_up, weight);
            if (isData  == 0 && SRdisc_unc_dn > 0)            p_result.p_jes_alt_dn_SR.CatFill(2, SRdisc_unc_dn, weight);

            if (isData  == 0 && SRdisc_JER_up > 0)            p_result.p_jer_alt_up_SR.CatFill(2, SRdisc_JER_up, weight);
            if (isData  == 0 && SRdisc_JER_dn > 0)            p_result.p_jer_alt_dn_SR.CatFill(2, SRdisc_JER_dn, weight);

            if (isData  == 0 && SRgenmet > 0)            p_result.p_met_alt_up_SR.CatFill(1, SRgenmet, weight);
            if (SR >= 0) {
                p_result.SR.CatFill(1, SR, weight);
                if ( !doFakes || ss::is_real_data() ) { // when filling for fakes, only consider real data
                    p_result.p_unw_raw_SR.CatFill(1, SR, 1);
                    p_result.p_unw_sgn_SR.CatFill(1, SR, weight > 0 ? 1 : -1);
                }
                if (doFakes == 1 )            p_result.p_fake_alt_up_SR.CatFill(1, SR, weight_alt_FR);
                if (doFakes == 1  && ss::is_real_data())            p_result.p_fake_unw_up_SR.CatFill(1, SR, weight > 0 ? 1 : 0);
                if (isData  == 0 )            p_result.p_btagSF_alt_up_SR.CatFill(1, SR, weight_btag_up_alt);
                if (isData  == 0 )            p_result.p_btagSF_alt_dn_SR.CatFill(1, SR, weight_btag_dn_alt);
                if (isData  == 0 )            p_result.p_btagSFheavy_alt_up_SR.CatFill(1, SR, weight_btagheavy_up_alt);
                if (isData  == 0 )            p_result.p_btagSFheavy_alt_dn_SR.CatFill(1, SR, weight_btagheavy_dn_alt);
                if (isData  == 0 )            p_result.p_btagSFlight_alt_up_SR.CatFill(1, SR, weight_btaglight_up_alt);
                if (isData  == 0 )            p_result.p_btagSFlight_alt_dn_SR.CatFill(1, SR, weight_btaglight_dn_alt);
                if (isData  == 0 )            p_result.p_pu_alt_up_SR.CatFill(1, SR, weight_pu_up_alt);
                if (isData  == 0 )            p_result.p_pu_alt_dn_SR.CatFill(1, SR, weight_pu_dn_alt);
                if (isData  == 0 )            p_result.p_isr_alt_up_SR.CatFill(1, SR, weight_isr_up_alt);
                if (isData  == 0 )            p_result.p_bb_alt_up_SR.CatFill(1, SR, weight_bb_up_alt);
                if (isData  == 0 )            p_result.p_prefire_alt_up_SR.CatFill(1, SR, weight_prefire_up_alt);
                if (isData  == 0 )            p_result.p_prefire_alt_dn_SR.CatFill(1, SR, weight_prefire_dn_alt);
                if (isData  == 0 )            p_result.p_trigger_alt_up_SR.CatFill(1, SR, weight_trigger_up_alt);
                if (isData  == 0 )            p_result.p_trigger_alt_dn_SR.CatFill(1, SR, weight_trigger_dn_alt);
                if (isData  == 0 )            p_result.p_lep_alt_up_SR.CatFill(1, SR, weight_lep_up_alt);
                // if (istttt || isttW || isttZ || isttH) {
                    p_result.p_scale_alt_up_SR.CatFill(1, SR,    (ss::weight_scale_UP() > -9000 ? ss::weight_scale_UP()/norm_scale_up : 1.0)*weight);
                    p_result.p_scale_alt_dn_SR.CatFill(1, SR,    (ss::weight_scale_DN() > -9000 ? ss::weight_scale_DN()/norm_scale_dn : 1.0)*weight);
                    p_result.p_alphas_alt_up_SR.CatFill(1, SR,    (ss::weight_alphas_UP() > -9000 ? ss::weight_alphas_UP() : 1.0)*weight);
                    p_result.p_alphas_alt_dn_SR.CatFill(1, SR,    (ss::weight_alphas_DN() > -9000 ? ss::weight_alphas_DN() : 1.0)*weight);
                    p_result.p_pdf_alt_up_SR.CatFill(1, SR,      (ss::weight_pdf_UP() > -9000 ? ss::weight_pdf_UP()/norm_pdf_up : 1.0)*weight);
                    p_result.p_pdf_alt_dn_SR.CatFill(1, SR,      (ss::weight_pdf_DN() > -9000 ? ss::weight_pdf_DN()/norm_pdf_dn : 1.0)*weight);
                    p_result.p_isrvar_alt_up_SR.CatFill(1, SR      , (year != 2018 ? ss::weight_isrvar_UP()*weight : weight));
                    p_result.p_isrvar_alt_dn_SR.CatFill(1, SR      , (year != 2018 ? ss::weight_isrvar_DN()*weight : weight));
                    p_result.p_fsrvar_alt_up_SR.CatFill(1, SR      , (year != 2018 ? ss::weight_fsrvar_UP()*weight : weight));
                    p_result.p_fsrvar_alt_dn_SR.CatFill(1, SR      , (year != 2018 ? ss::weight_fsrvar_DN()*weight : weight));
                // }
            }
            if (SRdisc >= 0) {
                p_result.SR.CatFill(2, SRdisc, weight);
                if (doFakes == 1 )            p_result.p_fake_alt_up_SR.CatFill(2, SRdisc, weight_alt_FR);
                if (doFakes == 1  && ss::is_real_data())            p_result.p_fake_unw_up_SR.CatFill(2, SRdisc, weight > 0 ? 1 : 0);
                if (isData  == 0 )            p_result.p_btagSF_alt_up_SR.CatFill(2, SRdisc, weight_btag_up_alt);
                if (isData  == 0 )            p_result.p_btagSF_alt_dn_SR.CatFill(2, SRdisc, weight_btag_dn_alt);
                if (isData  == 0 )            p_result.p_btagSFheavy_alt_up_SR.CatFill(2, SRdisc, weight_btagheavy_up_alt);
                if (isData  == 0 )            p_result.p_btagSFheavy_alt_dn_SR.CatFill(2, SRdisc, weight_btagheavy_dn_alt);
                if (isData  == 0 )            p_result.p_btagSFlight_alt_up_SR.CatFill(2, SRdisc, weight_btaglight_up_alt);
                if (isData  == 0 )            p_result.p_btagSFlight_alt_dn_SR.CatFill(2, SRdisc, weight_btaglight_dn_alt);
                if (isData  == 0 )            p_result.p_pu_alt_up_SR.CatFill(2, SRdisc, weight_pu_up_alt);
                if (isData  == 0 )            p_result.p_pu_alt_dn_SR.CatFill(2, SRdisc, weight_pu_dn_alt);
                if (isData  == 0 )            p_result.p_isr_alt_up_SR.CatFill(2, SRdisc, weight_isr_up_alt);
                if (isData  == 0 )            p_result.p_bb_alt_up_SR.CatFill(2, SRdisc, weight_bb_up_alt);
                if (isData  == 0 )            p_result.p_prefire_alt_up_SR.CatFill(2, SRdisc, weight_prefire_up_alt);
                if (isData  == 0 )            p_result.p_prefire_alt_dn_SR.CatFill(2, SRdisc, weight_prefire_dn_alt);
                if (isData  == 0 )            p_result.p_trigger_alt_up_SR.CatFill(2, SRdisc, weight_trigger_up_alt);
                if (isData  == 0 )            p_result.p_trigger_alt_dn_SR.CatFill(2, SRdisc, weight_trigger_dn_alt);
                if (isData  == 0 )            p_result.p_lep_alt_up_SR.CatFill(2, SRdisc, weight_lep_up_alt);
                // if (istttt || isttW || isttZ || isttH) {
                    p_result.p_scale_alt_up_SR.CatFill(2, SRdisc,    (ss::weight_scale_UP() > -9000 ? ss::weight_scale_UP()/norm_scale_up : 1.0)*weight);
                    p_result.p_scale_alt_dn_SR.CatFill(2, SRdisc,    (ss::weight_scale_DN() > -9000 ? ss::weight_scale_DN()/norm_scale_dn : 1.0)*weight);
                    p_result.p_alphas_alt_up_SR.CatFill(2, SRdisc,    (ss::weight_alphas_UP() > -9000 ? ss::weight_alphas_UP() : 1.0)*weight);
                    p_result.p_alphas_alt_dn_SR.CatFill(2, SRdisc,    (ss::weight_alphas_DN() > -9000 ? ss::weight_alphas_DN() : 1.0)*weight);
                    p_result.p_pdf_alt_up_SR.CatFill(2, SRdisc,      (ss::weight_pdf_UP() > -9000 ? ss::weight_pdf_UP()/norm_pdf_up : 1.0)*weight);
                    p_result.p_pdf_alt_dn_SR.CatFill(2, SRdisc,      (ss::weight_pdf_DN() > -9000 ? ss::weight_pdf_DN()/norm_pdf_dn : 1.0)*weight);
                    p_result.p_isrvar_alt_up_SR.CatFill(2, SRdisc, (year != 2018 ? ss::weight_isrvar_UP()*weight : weight));
                    p_result.p_isrvar_alt_dn_SR.CatFill(2, SRdisc, (year != 2018 ? ss::weight_isrvar_DN()*weight : weight));
                    p_result.p_fsrvar_alt_up_SR.CatFill(2, SRdisc, (year != 2018 ? ss::weight_fsrvar_UP()*weight : weight));
                    p_result.p_fsrvar_alt_dn_SR.CatFill(2, SRdisc, (year != 2018 ? ss::weight_fsrvar_DN()*weight : weight));
                // }
            }

            bool plotlep3 = nleps > 2;


            // {
            //     float lepsf = 1.;
            //     float btagsf = 1.;
            //     float wsf = 1.;
            //     float puw = 1.;
            //     float trigsf = 1.;
            //     float isrw = 1.;
            //     float w = ss::is_real_data() ? 0.0 : ss::scale1fb()*lumiAG; // ignore data
            //     if (ss::is_real_data()==0) {
            //         if (lep1good) lepsf *= leptonScaleFactor(year, lep1id, lep1ccpt, lep1eta, ss::ht());
            //         if (lep2good) lepsf *= leptonScaleFactor(year, lep2id, lep2ccpt, lep2eta, ss::ht());
            //         if (lep3good && lep3ccpt>20) lepsf *= leptonScaleFactor(year, lep3id, lep3ccpt, lep3eta, ss::ht());
            //         trigsf *= triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht());
            //         btagsf *= ss::weight_btagsf();
            //         puw *= getTruePUw(year, ss::trueNumInt()[0], 0);
            //         wsf *= ss::decayWSF();
            //         if (isttW) isrw *= isrWeight(year, ss::nisrMatch(), 1);
            //         if (isttZ) isrw *= isrWeight(year, ss::nisrMatch(), 2);
            //     }
            //     p_result.h_lepsf.br->Fill(lepsf, w);
            //     p_result.h_btagsf.br->Fill(btagsf, w);
            //     p_result.h_wsf.br->Fill(wsf, w);
            //     p_result.h_puw.br->Fill(puw, w);
            //     p_result.h_trigsf.br->Fill(trigsf, w);
            //     p_result.h_isrw.br->Fill(isrw, w);
            // }

            if (write_tree) {
                output_tree.FillVars(
                        weight, SR, SRdisc, ss::hyp_class()
                        );
            }


            if ((SR > 1) or (SRdisc > 1)) { // baseline, but not ttZ
                float f_nbtags = ss::bdt_nbtags();
                float f_njets = ss::bdt_njets();
                float f_met = ss::bdt_met();
                float f_ptl2 = ss::bdt_ptl2();
                float f_nlb40 = ss::bdt_nlb40();
                float f_ntb40 = ss::bdt_ntb40();
                float f_nleps = ss::bdt_nleps();
                float f_htb = ss::bdt_htb();
                float f_q1 = ss::bdt_q1();
                float f_ptj1 = ss::bdt_ptj1();
                float f_ptj6 = ss::bdt_ptj6();
                float f_ptj7 = ss::bdt_ptj7();
                float f_ml1j1 = ss::bdt_ml1j1();
                float f_dphil1l2 = ss::bdt_dphil1l2();
                float f_maxmjoverpt = ss::bdt_maxmjoverpt();
                float f_ptl1 = ss::bdt_ptl1();
                float f_detal1l2 = ss::bdt_detal1l2();
                float f_ptj8 = ss::bdt_ptj8();
                float f_ptl3 = ss::bdt_ptl3();

                p_result.h_detal1l2.br->Fill(ss::bdt_detal1l2(),weight);
                p_result.h_dphil1l2.br->Fill(ss::bdt_dphil1l2(),weight);
                p_result.h_htb.br->Fill(ss::bdt_htb(),weight);
                p_result.h_maxmjoverpt.br->Fill(ss::bdt_maxmjoverpt(),weight);
                p_result.h_ml1j1.br->Fill(ss::bdt_ml1j1(),weight);
                p_result.h_nlb40.br->Fill(ss::bdt_nlb40(),weight);
                p_result.h_ntb40.br->Fill(ss::bdt_ntb40(),weight);
                p_result.h_ptj1.br->Fill(ss::bdt_ptj1(),weight);
                p_result.h_ptj6.br->Fill(ss::bdt_ptj6(),weight);
                p_result.h_ptj7.br->Fill(ss::bdt_ptj7(),weight);
                p_result.h_ptj8.br->Fill(ss::bdt_ptj8(),weight);

                p_result.h_disc.sr->Fill(mvavalue,weight);
                p_result.h_njets.br->Fill(ss::njets() , weight);
                p_result.h_nbtags.br->Fill(ss::nbtags() , weight);
                p_result.h_type.br->Fill(mytype , weight);
                p_result.h_type3.br->Fill((abs(lep1id)==11)+(abs(lep2id)==11)+(abs(lep3id)==11), weight);
                p_result.h_nvtx.br->Fill(ss::nGoodVertices() , weight);
                p_result.h_charge.br->Fill(lep1id > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.br->Fill(nleps , weight);
                p_result.h_ht.br->Fill(ss::ht() , weight);
                p_result.h_met.br->Fill(ss::met() , weight);
                // p_result.h_mvis.br->Fill(mvis , weight);
                // p_result.h_mtvis.br->Fill(mtvis , weight);
                p_result.h_mll.br->Fill(mll , weight);
                p_result.h_mtmin.br->Fill(mtmin , weight);
                // p_result.h_l1pt.br->Fill(pto1, weight);
                // p_result.h_l2pt.br->Fill(pto2, weight);
                p_result.h_l1pt.br->Fill(lep1ccpt, weight);
                p_result.h_l2pt.br->Fill(lep2ccpt, weight);
                (abs(lep1id) == 11) ? p_result.h_el_l1pt.br->Fill(pto1, weight) : p_result.h_mu_l1pt.br->Fill(pto1, weight);
                (abs(lep2id) == 11) ? p_result.h_el_l2pt.br->Fill(pto2, weight) : p_result.h_mu_l2pt.br->Fill(pto2, weight);
                if (abs(lep1id) == 11) p_result.h_el_nmiss.br->Fill(ss::lep1_el_exp_innerlayers(), weight);
                if (abs(lep2id) == 11) p_result.h_el_nmiss.br->Fill(ss::lep2_el_exp_innerlayers(), weight);
                if (plotlep3) {
                    // p_result.h_l3pt.br->Fill(pto3, weight);
                    p_result.h_l3pt.br->Fill(lep3ccpt, weight);
                    (abs(lep3id) == 11) ? p_result.h_el_l3pt.br->Fill(pto3, weight) : p_result.h_mu_l3pt.br->Fill(pto3, weight);
                }
                // for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
                //     p_result.h_bjetpt.br->Fill(ss::btags()[ijet].pt(),weight);
                // }
                // for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
                //     p_result.h_jetpt.br->Fill(ss::jets()[ijet].pt(),weight);
                // }
                if (abs(lep1id) == 11) p_result.h_mva.br->Fill(ss::lep1_MVA()                                                     , weight);
                if(abs(lep1id) == 13) {
                    p_result.h_mu_sip3d_lep1.br  ->Fill(ss::lep1_sip()                                                                          , weight);
                    p_result.h_mu_l1pt.br       ->Fill(lep1pt                                                                     , weight);
                    p_result.h_mu_l1eta.br       ->Fill(lep1eta                                                                     , weight);
                    p_result.h_mu_l1phi.br       ->Fill(lep1phi                                                                     , weight);
                    p_result.h_mu_lep1_miniIso.br->Fill(ss::lep1_miniIso()                                                                      , weight);
                    p_result.h_mu_lep1_ptRel.br  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    p_result.h_mu_lep1_ptRatio.br->Fill(ss::lep1_ptratio()                                                                               , weight);
                } else {
                    p_result.h_el_sip3d_lep1.br  ->Fill(ss::lep1_sip()                                                                          , weight);
                    p_result.h_el_l1pt.br        ->Fill(lep1ccpt                                                                                 , weight);
                    p_result.h_el_l1eta.br       ->Fill(lep1eta                                                                     , weight);
                    p_result.h_el_l1phi.br       ->Fill(lep1phi                                                                     , weight);
                    p_result.h_el_lep1_miniIso.br->Fill(ss::lep1_miniIso()                                                                      , weight);
                    p_result.h_el_lep1_ptRel.br  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    p_result.h_el_lep1_ptRatio.br->Fill(ss::lep1_ptratio()                                                                               , weight);
                }
                if (abs(lep2id) == 11) p_result.h_mva.br->Fill(ss::lep2_MVA()                                                     , weight);
                if(abs(lep2id) == 13) {
                    p_result.h_mu_sip3d_lep2.br  ->Fill(ss::lep2_sip()                                                                          , weight);
                    p_result.h_mu_l2eta.br       ->Fill(lep2eta                                                                     , weight);
                    p_result.h_mu_l2phi.br       ->Fill(lep2phi                                                                     , weight);
                    p_result.h_mu_lep2_miniIso.br->Fill(ss::lep2_miniIso()                                                                      , weight);
                    p_result.h_mu_lep2_ptRel.br  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    p_result.h_mu_lep2_ptRatio.br->Fill(ss::lep2_ptratio()                                                                               , weight);
                } else {
                    p_result.h_el_sip3d_lep2.br  ->Fill(ss::lep2_sip()                                                                          , weight);
                    p_result.h_el_l2eta.br       ->Fill(lep2eta                                                                     , weight);
                    p_result.h_el_l2phi.br       ->Fill(lep2phi                                                                     , weight);
                    p_result.h_el_lep2_miniIso.br->Fill(ss::lep2_miniIso()                                                                      , weight);
                    p_result.h_el_lep2_ptRel.br  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    p_result.h_el_lep2_ptRatio.br->Fill(ss::lep2_ptratio()                                                                               , weight);
                }
                if (plotlep3) {
                    if(abs(lep3id) == 13) {
                        p_result.h_mu_l3eta.br       ->Fill(lep3eta                                                                     , weight);
                    } else {
                        p_result.h_el_l3eta.br       ->Fill(lep3eta                                                                     , weight);
                    }
                }
            }

            if (SR == 1) { // ttZ CR
                p_result.h_disc.ttzcr->Fill(mvavalue,weight);
                p_result.h_njets.ttzcr->Fill(ss::njets() , weight);
                p_result.h_nbtags.ttzcr->Fill(ss::nbtags() , weight);
                p_result.h_type.ttzcr->Fill(mytype , weight);
                p_result.h_type3.ttzcr->Fill((abs(lep1id)==11)+(abs(lep2id)==11)+(abs(lep3id)==11), weight);
                p_result.h_charge.ttzcr->Fill(lep1id > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.ttzcr->Fill(nleps , weight);
                p_result.h_ht.ttzcr->Fill(ss::ht() , weight);
                p_result.h_met.ttzcr->Fill(ss::met() , weight);
                // p_result.h_mvis.ttzcr->Fill(mvis , weight);
                // p_result.h_mtvis.ttzcr->Fill(mtvis , weight);
                p_result.h_mll.ttzcr->Fill(mll , weight);
                p_result.h_mtmin.ttzcr->Fill(mtmin , weight);
                p_result.h_l1pt.ttzcr->Fill(pto1, weight);
                p_result.h_l2pt.ttzcr->Fill(pto2, weight);
                (abs(lep1id) == 11) ? p_result.h_el_l1pt.ttzcr->Fill(pto1, weight) : p_result.h_mu_l1pt.ttzcr->Fill(pto1, weight);
                (abs(lep2id) == 11) ? p_result.h_el_l2pt.ttzcr->Fill(pto2, weight) : p_result.h_mu_l2pt.ttzcr->Fill(pto2, weight);
                if (plotlep3) {
                    float mllos1 = (ss::lep1_p4()*lep1ccpt/lep1pt+ss::lep3_p4()*lep3ccpt/lep3pt).M();
                    float mllos2 = (ss::lep2_p4()*lep2ccpt/lep2pt+ss::lep3_p4()*lep3ccpt/lep3pt).M();
                    float mllos = fabs(mllos1 - 91.2) < fabs(mllos2 - 91.2) ? mllos1 : mllos2;
                    p_result.h_mllos.ttzcr->Fill(mllos, weight);
                    p_result.h_l3pt.ttzcr->Fill(pto3, weight);
                    (abs(lep3id) == 11) ? p_result.h_el_l3pt.ttzcr->Fill(pto3, weight) : p_result.h_mu_l3pt.ttzcr->Fill(pto3, weight);
                }
            }

            if (SR == 2) { // ttW CR
                p_result.h_disc.ttwcr->Fill(mvavalue,weight);
                p_result.h_njets.ttwcr->Fill(ss::njets() , weight);
                p_result.h_nbtags.ttwcr->Fill(ss::nbtags() , weight);
                p_result.h_type.ttwcr->Fill(mytype , weight);
                p_result.h_charge.ttwcr->Fill(lep1id > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.ttwcr->Fill(nleps , weight);
                p_result.h_ht.ttwcr->Fill(ss::ht() , weight);
                p_result.h_met.ttwcr->Fill(ss::met() , weight);
                // p_result.h_mvis.ttwcr->Fill(mvis , weight);
                // p_result.h_mtvis.ttwcr->Fill(mtvis , weight);
                p_result.h_mll.ttwcr->Fill(mll , weight);
                p_result.h_mtmin.ttwcr->Fill(mtmin , weight);
                p_result.h_l1pt.ttwcr->Fill(pto1, weight);
                p_result.h_l2pt.ttwcr->Fill(pto2, weight);
                (abs(lep1id) == 11) ? p_result.h_el_l1pt.ttwcr->Fill(pto1, weight) : p_result.h_mu_l1pt.ttwcr->Fill(pto1, weight);
                (abs(lep2id) == 11) ? p_result.h_el_l2pt.ttwcr->Fill(pto2, weight) : p_result.h_mu_l2pt.ttwcr->Fill(pto2, weight);
                if (plotlep3) {
                    p_result.h_l3pt.ttwcr->Fill(pto3, weight);
                    (abs(lep3id) == 11) ? p_result.h_el_l3pt.ttwcr->Fill(pto3, weight) : p_result.h_mu_l3pt.ttwcr->Fill(pto3, weight);
                }
            }

            if (SR > nCR) {
                // std::cout <<  " ss::ht(): " << ss::ht() <<  " weight: " << weight <<  " SR: " << SR <<  std::endl;
                p_result.SR.CatFill(0, SR-nCR, weight);
                p_result.h_njets.sr->Fill(ss::njets() , weight);
                p_result.h_nbtags.sr->Fill(ss::nbtags() , weight);
                p_result.h_type.sr->Fill(mytype , weight);
                p_result.h_type3.sr->Fill((abs(lep1id)==11)+(abs(lep2id)==11)+(abs(lep3id)==11), weight);
                p_result.h_charge.sr->Fill(lep1id > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.sr->Fill(nleps , weight);
                p_result.h_ht.sr->Fill(ss::ht() , weight);
                // if (SR == 1+2)  p_result.h_ht_sr1.sr->Fill(ss::ht() , weight);
                // if (SR == 2+2)  p_result.h_ht_sr2.sr->Fill(ss::ht() , weight);
                // if (SR == 4+2)  p_result.h_ht_sr4.sr->Fill(ss::ht() , weight);
                // if (SR == 5+2)  p_result.h_ht_sr5.sr->Fill(ss::ht() , weight);
                // if (SR == 11+2) p_result.h_ht_sr11.sr->Fill(ss::ht() , weight);
                p_result.h_met.sr->Fill(ss::met() , weight);
                // p_result.h_mvis.sr->Fill(mvis , weight);
                // p_result.h_mtvis.sr->Fill(mtvis , weight);
                p_result.h_mll.sr->Fill(mll , weight);
                p_result.h_mtmin.sr->Fill(mtmin , weight);
                p_result.h_l1pt.sr->Fill(pto1, weight);
                p_result.h_l2pt.sr->Fill(pto2, weight);
                (abs(lep1id) == 11) ? p_result.h_el_l1pt.sr->Fill(pto1, weight) : p_result.h_mu_l1pt.sr->Fill(pto1, weight);
                (abs(lep2id) == 11) ? p_result.h_el_l2pt.sr->Fill(pto2, weight) : p_result.h_mu_l2pt.sr->Fill(pto2, weight);
                if (plotlep3) {
                    p_result.h_l3pt.sr->Fill(pto3, weight);
                    (abs(lep3id) == 11) ? p_result.h_el_l3pt.sr->Fill(pto3, weight) : p_result.h_mu_l3pt.sr->Fill(pto3, weight);
                }
                // for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
                //     p_result.h_bjetpt.sr->Fill(ss::btags()[ijet].pt(),weight);
                // }
                // for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
                //     p_result.h_jetpt.sr->Fill(ss::jets()[ijet].pt(),weight);
                // }
                if (abs(lep1id) == 11) p_result.h_mva.sr->Fill(ss::lep1_MVA()                                                     , weight);
                if(abs(lep1id) == 13) {
                    p_result.h_mu_sip3d_lep1.sr  ->Fill(ss::lep1_sip()                                                                          , weight);
                    p_result.h_mu_l1eta.sr       ->Fill(lep1eta                                                                     , weight);
                    p_result.h_mu_l1phi.sr       ->Fill(lep1phi                                                                     , weight);
                    p_result.h_mu_lep1_miniIso.sr->Fill(ss::lep1_miniIso()                                                                      , weight);
                    p_result.h_mu_lep1_ptRel.sr  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    p_result.h_mu_lep1_ptRatio.sr->Fill(ss::lep1_ptratio()                                                                               , weight);
                } else {
                    p_result.h_el_sip3d_lep1.sr  ->Fill(ss::lep1_sip()                                                                          , weight);
                    p_result.h_el_l1eta.sr       ->Fill(lep1eta                                                                     , weight);
                    p_result.h_el_l1phi.sr       ->Fill(lep1phi                                                                     , weight);
                    p_result.h_el_lep1_miniIso.sr->Fill(ss::lep1_miniIso()                                                                      , weight);
                    p_result.h_el_lep1_ptRel.sr  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    p_result.h_el_lep1_ptRatio.sr->Fill(ss::lep1_ptratio()                                                                               , weight);
                }
                if (abs(lep2id) == 11) p_result.h_mva.sr->Fill(ss::lep2_MVA()                                                     , weight);
                if(abs(lep2id) == 13) {
                    p_result.h_mu_sip3d_lep2.sr  ->Fill(ss::lep2_sip()                                                                          , weight);
                    p_result.h_mu_l2eta.sr       ->Fill(lep2eta                                                                     , weight);
                    p_result.h_mu_l2phi.sr       ->Fill(lep2phi                                                                     , weight);
                    p_result.h_mu_lep2_miniIso.sr->Fill(ss::lep2_miniIso()                                                                      , weight);
                    p_result.h_mu_lep2_ptRel.sr  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    p_result.h_mu_lep2_ptRatio.sr->Fill(ss::lep2_ptratio()                                                                               , weight);
                } else {
                    p_result.h_el_sip3d_lep2.sr  ->Fill(ss::lep2_sip()                                                                          , weight);
                    p_result.h_el_l2eta.sr       ->Fill(lep2eta                                                                     , weight);
                    p_result.h_el_l2phi.sr       ->Fill(lep2phi                                                                     , weight);
                    p_result.h_el_lep2_miniIso.sr->Fill(ss::lep2_miniIso()                                                                      , weight);
                    p_result.h_el_lep2_ptRel.sr  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    p_result.h_el_lep2_ptRatio.sr->Fill(ss::lep2_ptratio()                                                                               , weight);
                }
                if (plotlep3) {
                    if(abs(lep3id) == 13) {
                        p_result.h_mu_l3eta.sr       ->Fill(lep3eta                                                                     , weight);
                    } else {
                        p_result.h_el_l3eta.sr       ->Fill(lep3eta                                                                     , weight);
                    }
                }
            }
#endif


        }//event loop
        file->Close();
    }//file loop
    if (!quiet) bar.finish();

    if (write_tree) {
        if (!quiet) std::cout << "Writing output tree" << std::endl;
        output_tree.Write();
    }

    //Return yield
    return p_result;

}

void avoidNegativeYields(TH1F* plot) {
    for (int bin=1;bin<=plot->GetNbinsX();++bin) {
        if (plot->GetBinContent(bin)<0) {
            // if (!0) cout << "warning: plot " << plot->GetName() << " has negative yield in bin " << bin << " value=" << plot->GetBinContent(bin) << " error=" << plot->GetBinError(bin) << "; setting to 1E-6."<< endl;
            plot->SetBinContent(bin,1E-6);
        }
    }
}


float getSMSscale1fb(bool is1d=false) {
    if (!h_counts) {
        std::cerr << "Count histogram (h_counts) isn't loaded!" << std::endl;
        exit(1);
    }
    float count = h_counts->GetBinContent(h_counts->FindBin(ss::sparms()[0],(is1d ? 1. : ss::sparms()[1])));
    return 1000.0*ss::xsec()/count;
}

float getRPVscale1fb() {
    if (!h_counts) {
        std::cerr << "Count histogram (h_counts) isn't loaded!" << std::endl;
        exit(1);
    }
    float count = h_counts->GetBinContent(h_counts->FindBin(1,1));
    return 1000.0*ss::xsec()/count;
}

float getWeightNormalization(int idx) {
    if (!h_weights) {
        std::cerr << "Weight histogram (h_weights) isn't loaded!" << std::endl;
        exit(1);
    }
                        // weight_hist->Fill(0.5, nom);
                    // weight_hist->Fill(1.5, scale_up_raw);
                    // weight_hist->Fill(2.5, scale_down_raw);
                    // weight_hist->Fill(3.5, pdf_up_raw);
                    // weight_hist->Fill(4.5, pdf_down_raw);
    float sf = (h_weights->GetBinContent(idx+1))/(h_weights->GetBinContent(1));
    return sf;
}

float calcDeltaPhi(float phi1, float phi2){
  float dPhi = phi1 - phi2;
  while (dPhi  >  TMath::Pi()) dPhi -= 2*TMath::Pi();
  while (dPhi <= -TMath::Pi()) dPhi += 2*TMath::Pi();
  return fabs(dPhi);
}

float calcMT(float pt1, float phi1, float pt2, float phi2){
  return sqrt( 2 * pt1 * pt2 * ( 1 - cos( phi1 - phi2 ) ) );
}

