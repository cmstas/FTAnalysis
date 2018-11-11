#include <bitset>
#include <iostream>
#include <algorithm>
#include "TChain.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TH2F.h"
#include "Math/LorentzVector.h"
#include "TKey.h"
#include "TSystem.h"
#include "TMVA/Reader.h"
#include "../misc/class_files/v8.02/SS.h"
// #include "../../common/CORE/SSSelections.h"
#include "../../common/CORE/Tools/dorky/dorky.h"
#include "../../common/CORE/Tools/MT2/MT2Utility.h"
#include "../../common/CORE/Tools/MT2/MT2.h"
#include "../misc/common_utils.h"
#include "../misc/signal_regions.h"
#include "../misc/bdt.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "../misc/tqdm.h"

#define WRITE(var) { if (plots.var) plots.var->Write(); }

bool STOP_REQUESTED = false;

float XSEC_TTTT = 11.97;
float scaleLumi = 1; // 75/35.87
TH2F *h_counts = 0;
bool doThirdLepton = true; // FIXME

void avoidNegativeYields(TH1F* plot);

struct SR_t     {
    TH1F* HH = 0;
    TH1F* HL = 0;
    TH1F* LL = 0;
    TH1F* ML = 0;
    // SR_t() {
    // }
    void Write() {
        AvoidNegativeYields();
        if (HH) HH->Write();
        if (HL) HL->Write();
        if (LL) LL->Write();
        if (ML) ML->Write();
    }
    void CatFill(region_t category, float value, float weight) {
        if ((category == HighHigh   ) && HH) HH->Fill(value, weight);
        if ((category == HighLow    ) && HL) HL->Fill(value, weight);
        if ((category == LowLow     ) && LL) LL->Fill(value, weight);
        if ((category == Multilepton) && ML) ML->Fill(value, weight);
    }
    void AvoidNegativeYields() {
        if (HH) avoidNegativeYields(HH);
        if (HL) avoidNegativeYields(HL);
        if (LL) avoidNegativeYields(LL);
        if (ML) avoidNegativeYields(ML);
    }
    void InitHistError(bool poisson) {
        if (poisson) {
            if (HH) HH->SetBinErrorOption(TH1::kPoisson);
            if (HL) HL->SetBinErrorOption(TH1::kPoisson);
            if (LL) LL->SetBinErrorOption(TH1::kPoisson);
            if (ML) ML->SetBinErrorOption(TH1::kPoisson);
        } else {
            if (HH) HH->Sumw2();
            if (HL) HL->Sumw2();
            if (LL) LL->Sumw2();
            if (ML) ML->Sumw2();
        }
    }
};


enum categbit_t { 
    kBaseline        = 1<<0,
    kSignalRegion    = 1<<1,
    kHighHigh        = 1<<2,
    kHighLow         = 1<<3,
    kLowLow          = 1<<4,
    kMultilepton     = 1<<5,
    kMultileptonOnZ  = 1<<6,
    kMultileptonOffZ = 1<<7
};

struct triple_t {
    TH1F* br = 0; 
    TH1F* hh = 0;
    TH1F* hl = 0;
    TH1F* ll = 0;
    TH1F* ml = 0;
    TH1F* mloffz = 0;
    TH1F* mlonz = 0;
    TH1F* sr = 0;
    // triple_t() {
    // }
    void Init(TString label, TString chainTitle, int nbins, float xlow, float xhigh) {
        br = new TH1F(Form("br_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); br->SetDirectory(0);
        hh = new TH1F(Form("hh_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); hh->SetDirectory(0);
        hl = new TH1F(Form("hl_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); hl->SetDirectory(0);
        ll = new TH1F(Form("ll_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); ll->SetDirectory(0);
        ml = new TH1F(Form("ml_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); ml->SetDirectory(0);
        mloffz = new TH1F(Form("mloffz_%s_%s",label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); mloffz->SetDirectory(0);
        mlonz = new TH1F(Form("mlonz_%s_%s"  ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); mlonz->SetDirectory(0);
        sr = new TH1F(Form("sr_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); sr->SetDirectory(0);
    }


    void CatFill(int kcategs, float value, float weight) {
        if ((kcategs & kBaseline   ) && br) br->Fill(value, weight);
        if ((kcategs & kSignalRegion   ) && sr) sr->Fill(value, weight);
        if ((kcategs & kHighHigh   ) && hh) hh->Fill(value, weight);
        if ((kcategs & kHighLow    ) && hl) hl->Fill(value, weight);
        if ((kcategs & kLowLow     ) && ll) ll->Fill(value, weight);
        if ((kcategs & kMultilepton) && ml) ml->Fill(value, weight);
        if ((kcategs & kMultileptonOnZ) && mlonz) mlonz->Fill(value, weight);
        if ((kcategs & kMultileptonOffZ) && mloffz) mloffz->Fill(value, weight);
    }
    void Write() {
        if (br) { br->Write(); }
        if (hh) { hh->Write(); }
        if (hl) { hl->Write(); }
        if (ll) { ll->Write(); }
        if (ml) { ml->Write(); }
        if (mloffz) { mloffz->Write(); }
        if (mlonz) { mlonz->Write(); }
        if (sr) { sr->Write(); }
    }
};

struct tree_t {
    TFile* out_file;
    TTree* out_tree;
    int tree_event = -1;
    int tree_lumi = -1;
    int tree_run = -1;
    float tree_weight = -1.;
    TString tree_name = "";
    int tree_stype = -1;
    int tree_SR = -1;
    int tree_hyp_class = -1;
    int tree_region = -1;
    int tree_categbits = -1;
    int tree_l1id = 0;
    int tree_l2id = 0;
    int tree_l3id = 0;
    int tree_nbtags = -1;
    int tree_njets = -1;
    int tree_nleps = -1;
    float tree_met = -1.;
    float tree_ht = -1.;
    float tree_mtmin = -1.;
    float tree_mt2 = -1.;
    float tree_mt2min = -1.;
    void Init(int year, TString chainTitle) {
        out_file = new TFile(Form("trees/tree_%i_%s.root",year,chainTitle.Data()),"RECREATE");
        out_file->cd();
        out_tree = new TTree("t", "tree_yieldMaker");
        out_tree->Branch("event" , &tree_event );
        out_tree->Branch("lumi" , &tree_lumi );
        out_tree->Branch("run" , &tree_run );
        out_tree->Branch("weight" , &tree_weight );
        out_tree->Branch("name" , &tree_name );
        out_tree->Branch("stype" , &tree_stype );
        out_tree->Branch("SR" , &tree_SR );
        out_tree->Branch("hyp_class" , &tree_hyp_class );
        out_tree->Branch("region" , &tree_region );
        out_tree->Branch("categbits" , &tree_categbits );
        out_tree->Branch("l1id", &tree_l1id );
        out_tree->Branch("l2id", &tree_l2id );
        out_tree->Branch("l3id", &tree_l3id );
        out_tree->Branch("nbtags", &tree_nbtags );
        out_tree->Branch("njets", &tree_njets );
        out_tree->Branch("nleps", &tree_nleps );
        out_tree->Branch("met", &tree_met );
        out_tree->Branch("ht", &tree_ht );
        out_tree->Branch("mtmin", &tree_mtmin );
        out_tree->Branch("mt2", &tree_mt2 );
        out_tree->Branch("mt2min", &tree_mt2min );
        tree_name = chainTitle;
        if (chainTitle.BeginsWith("fs_")) tree_stype = 0;
        if (chainTitle == "ttph") tree_stype = 1;
        if (chainTitle == "flips") tree_stype = 2;
        if (chainTitle == "wz") tree_stype = 3;
        if (chainTitle == "ww") tree_stype = 3;
        if (chainTitle == "ttw") tree_stype = 3;
        if (chainTitle == "ttz") tree_stype = 3;
        if (chainTitle == "tth") tree_stype = 3;
        if (chainTitle == "rares") tree_stype = 3;
        if (chainTitle == "xg") tree_stype = 3;
    }
    void FillVars(
            int event, int lumi, int run,
            float weight, 
            int SR, int hyp_class, int region, int categbits,
            int l1id, int l2id, int l3id,
            int nbtags, int njets, int nleps,
            float met, float ht, float mtmin, float mt2, float mt2min
            ) {
        tree_event = event;
        tree_lumi = lumi;
        tree_run = run;
        tree_weight = weight;
        tree_SR = SR;
        tree_hyp_class = hyp_class;
        tree_region = region;
        tree_categbits = categbits;
        tree_l1id = l1id;
        tree_l2id = l2id;
        tree_l3id = l3id;
        tree_nbtags = nbtags;
        tree_njets = njets;
        tree_nleps = nleps;
        tree_met = met;
        tree_ht = ht;
        tree_mtmin = mtmin;
        tree_mt2 = mt2;
        tree_mt2min = mt2min;
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
    SR_t p_btagSF_alt_dn_SR;
    SR_t p_btagSF_alt_up_SR;
    SR_t p_fake_alt_up_SR  ;
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

    triple_t h_ht;
    triple_t h_met;
    triple_t h_metnm1;
    triple_t h_mll;
    triple_t h_mllos;
    triple_t h_mtmin;
    triple_t h_mtmax;
    triple_t h_mt2;
    triple_t h_mt2min;
    triple_t h_twobinmtmin;
    triple_t h_twobinmt2;
    triple_t h_nleptonicW;
    triple_t h_njets;
    triple_t h_nbtags;
    triple_t h_l1pt;
    triple_t h_l2pt;
    triple_t h_l3pt;
    triple_t h_type;
    triple_t h_type3;
    triple_t h_category;
    triple_t h_nvtx;
    triple_t h_charge;
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

    triple_t h_lepsf;
    triple_t h_btagsf;
    triple_t h_wsf;
    triple_t h_puw;
    triple_t h_trigsf;
    triple_t h_isrw;


};


//function declaration
plots_t run(TChain *chain, int year, TString options);
float calcDeltaPhi(float phi1, float phi2);
float calcMT(float pt1, float phi1, float pt2, float phi2);
float getSMSscale1fb();
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


    plots_t plots = run(ch, year, options);


    TString chainTitle = ch->GetTitle();
    TString outname = Form("%s/output_%d%s_%s.root", outputdir.Data(), year, extra.Data(), chainTitle.Data());

    if (!quiet) std::cout << "Writing out " << outname << std::endl;
    TFile *fout = new TFile(outname, "RECREATE");
    fout->cd();

    plots.SR.Write();
    plots.h_bjetpt.Write();
    plots.h_btagsf.Write();
    plots.h_charge.Write();
    plots.h_q3.Write();
    plots.h_charge3.Write();
    plots.h_disc.Write();
    plots.h_el_l1eta.Write();
    plots.h_el_l1pt.Write();
    plots.h_el_l2eta.Write();
    plots.h_el_l2pt.Write();
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
    plots.h_mllos.Write();
    plots.h_mtmin.Write();
    plots.h_mtmax.Write();
    plots.h_mt2.Write();
    plots.h_mt2min.Write();
    plots.h_twobinmtmin.Write();
    plots.h_twobinmt2.Write();
    plots.h_nleptonicW.Write();
    plots.h_mtop1.Write();
    plots.h_mtop2.Write();
    plots.h_mtvis.Write();
    plots.h_mu_l1eta.Write();
    plots.h_mu_l1pt.Write();
    plots.h_mu_l2eta.Write();
    plots.h_mu_l2pt.Write();
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
    plots.h_type3.Write();
    plots.h_category.Write();
    plots.h_wsf.Write();
    plots.p_alphas_alt_dn_SR.Write();
    plots.p_alphas_alt_up_SR.Write();
    plots.p_bb_alt_dn_SR.Write();
    plots.p_bb_alt_up_SR.Write();
    plots.p_btagSF_alt_dn_SR.Write();
    plots.p_btagSF_alt_up_SR.Write();
    plots.p_fake_alt_up_SR.Write();
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


    float lumiAG = getLumi(year);
    int nsr = getNsrs();
    int nCR = getNCRs();
    int nbdtbins = 13;

    int nHHsr = 51;
    int nHLsr = 41;
    int nLLsr = 8;
    int nMLsr = 46; // (16+7)*2

    bool isData = 0;
    bool doFlips = 0;
    int doFakes = 0;
    bool isFastsim = 0;
    bool isGamma = 0;
    bool truthfake = 0;
    bool skipmatching = 0;
    bool ignoreFakeFactor = 0;


    bool evaluateBDT = options.Contains("evaluateBDT");
    bool quiet = options.Contains("quiet");
    bool write_tree = options.Contains("writeTree");
    bool minPtFake18 = options.Contains("minPtFake18");
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

    int nsrdisc = getNsrsDisc(nbdtbins); // this is supposed to be 1 more than nbdtbins (we add in CRZ as a "bin")

    float min_pt_fake = minPtFake18 ? 18. : -1;

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



    if (chainTitle.Contains("fs_")) {
        if (!quiet) std::cout << "Fastsim sample detected" << std::endl;
        isFastsim = true;
        evaluateBDT = false;
    }

    bool isWZ = (chainTitle=="wz");
    bool istttt = (chainTitle=="tttt");
    bool isttZ = (chainTitle=="ttz");
    bool istt = (chainTitle=="ttbar") || (chainTitle=="fakes_mc") || (chainTitle=="fakes_mc_unw");
    bool isttW = (chainTitle=="ttw");
    bool isttH = (chainTitle=="tth");
    bool isXgamma = (chainTitle=="xg");

    bool isHiggsScan = false;
    bool isHiggsPseudoscalar = false;
    float fastsim_isr_norm_central = 1.;
    float fastsim_isr_norm_up = 1.;
    float fastsim_isr_norm_down = 1.;
    vector<float> mysparms;
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
        if (mysparms.size() == 2) {
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

    p_result.h_l1pt.Init("l1pt", chainTitle, 30 , 0 , 150);
    p_result.h_l2pt.Init("l2pt", chainTitle, 30 , 0 , 150);
    p_result.h_l3pt.Init("l3pt", chainTitle, 20 , 0 , 100);
    p_result.h_el_l1eta.Init("el_l1eta", chainTitle, 12, -2.5 , 2.5);
    p_result.h_el_l1pt.Init("el_l1pt", chainTitle, 15 , 0 , 150);
    p_result.h_el_l2eta.Init("el_l2eta", chainTitle, 12, -2.5 , 2.5);
    p_result.h_el_l2pt.Init("el_l2pt", chainTitle, 15 , 0 , 150);
    p_result.h_el_l3eta.Init("el_l3eta", chainTitle, 8, -2.5 , 2.5);
    p_result.h_el_l3pt.Init("el_l3pt", chainTitle, 7 , 0 , 140);
    p_result.h_mu_l1eta.Init("mu_l1eta", chainTitle, 12, -2.5 , 2.5);
    p_result.h_mu_l1pt.Init("mu_l1pt", chainTitle, 15 , 0 , 150);
    p_result.h_mu_l2eta.Init("mu_l2eta", chainTitle, 12, -2.5 , 2.5);
    p_result.h_mu_l2pt.Init("mu_l2pt", chainTitle, 15 , 0 , 150);
    p_result.h_mu_l3eta.Init("mu_l3eta", chainTitle, 8, -2.5 , 2.5);
    p_result.h_mu_l3pt.Init("mu_l3pt", chainTitle, 7 , 0 , 140);

    p_result.h_met.Init("met", chainTitle, 20 , 0 , 500);
    p_result.h_ht.Init("ht", chainTitle, 15, 100, 1600);
    p_result.h_mll.Init("mll", chainTitle, 20 , 0 , 200);
    p_result.h_mllos.Init("mllos", chainTitle, 20 , 0 , 200);
    p_result.h_mtmin.Init("mtmin", chainTitle, 10 , 0 , 200);
    p_result.h_mtmax.Init("mtmax", chainTitle, 75 , 0 , 200);
    p_result.h_mt2.Init("mt2", chainTitle, 10 , 0 , 200);
    p_result.h_mt2min.Init("mt2min", chainTitle, 10 , 0 , 200);
    p_result.h_twobinmtmin.Init("twobinmtmin", chainTitle, 2 , 0 , 200);
    p_result.h_twobinmt2.Init("twobinmt2", chainTitle, 2 , 0 , 200);
    p_result.h_nbtags.Init("nbtags", chainTitle, 7 , -0.5 , 6.5);
    p_result.h_njets.Init("njets", chainTitle, 10 , 1.5 , 11.5);
    p_result.h_nleps.Init("nleps", chainTitle, 6 , 0 , 6);
    p_result.h_nleptonicW.Init("nleptonicW", chainTitle, 5 , -0.5 , 4.5);
    p_result.h_type.Init("type", chainTitle, 4 , 0 , 4);
    p_result.h_type3.Init("type3", chainTitle, 4 , 0 , 4);
    p_result.h_category.Init("category", chainTitle, 7 , 0.5 , 7.5);
    p_result.h_charge.Init("charge", chainTitle, 3 , -1 , 2);
    p_result.h_q3.Init("q3", chainTitle, 3 , -1 , 2);
    p_result.h_charge3.Init("charge3", chainTitle, 3 , -1 , 2);

    p_result.h_nvtx.Init("nvtx", chainTitle, 20 , 0 , 80);

    p_result.SR.HH = new TH1F(Form("SRHH_TOTAL_%s" , chainTitleCh) , Form("SRHH_TOTAL_%s" , chainTitleCh) , nHHsr     , 0.5  , nHHsr+0.5); p_result.SR.HH->SetDirectory(0);
    p_result.SR.HL = new TH1F(Form("SRHL_TOTAL_%s" , chainTitleCh) , Form("SRHL_TOTAL_%s" , chainTitleCh) , nHLsr     , 0.5  , nHLsr+0.5); p_result.SR.HL->SetDirectory(0);
    p_result.SR.LL = new TH1F(Form("SRLL_TOTAL_%s" , chainTitleCh) , Form("SRLL_TOTAL_%s" , chainTitleCh) , nLLsr     , 0.5  , nLLsr+0.5); p_result.SR.LL->SetDirectory(0);
    p_result.SR.ML = new TH1F(Form("SRML_TOTAL_%s" , chainTitleCh) , Form("SRML_TOTAL_%s" , chainTitleCh) , nMLsr     , 0.5  , nMLsr+0.5); p_result.SR.ML->SetDirectory(0);

    //For FR variations
    if (doFakes == 1) {
        p_result.p_fake_alt_up_SR.HH        = new TH1F(Form("SRHH_FR_TOTAL_%s"      , chainTitleCh) , Form("SRHH_FR_TOTAL_%s"      , chainTitleCh) , nHHsr   , 0.5, nHHsr+0.5 ); p_result.p_fake_alt_up_SR.HH->SetDirectory(0);
        p_result.p_fake_alt_up_SR.HL        = new TH1F(Form("SRHL_FR_TOTAL_%s"      , chainTitleCh) , Form("SRHL_FR_TOTAL_%s"      , chainTitleCh) , nHLsr   , 0.5, nHLsr+0.5 ); p_result.p_fake_alt_up_SR.HL->SetDirectory(0);
        p_result.p_fake_alt_up_SR.LL        = new TH1F(Form("SRLL_FR_TOTAL_%s"      , chainTitleCh) , Form("SRLL_FR_TOTAL_%s"      , chainTitleCh) , nLLsr   , 0.5, nLLsr+0.5 ); p_result.p_fake_alt_up_SR.LL->SetDirectory(0);
        p_result.p_fake_alt_up_SR.ML        = new TH1F(Form("SRML_FR_TOTAL_%s"      , chainTitleCh) , Form("SRML_FR_TOTAL_%s"      , chainTitleCh) , nMLsr   , 0.5, nMLsr+0.5 ); p_result.p_fake_alt_up_SR.ML->SetDirectory(0);
    }
    //For unw FR application counts
    if (doFakes == 1) {
        p_result.p_fake_unw_up_SR.HH        = new TH1F(Form("SRHH_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRHH_UNWFR_TOTAL_%s"      , chainTitleCh) , nHHsr   , 0.5, nHHsr+0.5 ); p_result.p_fake_unw_up_SR.HH->SetDirectory(0);
        p_result.p_fake_unw_up_SR.HL        = new TH1F(Form("SRHL_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRHL_UNWFR_TOTAL_%s"      , chainTitleCh) , nHLsr   , 0.5, nHLsr+0.5 ); p_result.p_fake_unw_up_SR.HL->SetDirectory(0);
        p_result.p_fake_unw_up_SR.LL        = new TH1F(Form("SRLL_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRLL_UNWFR_TOTAL_%s"      , chainTitleCh) , nLLsr   , 0.5, nLLsr+0.5 ); p_result.p_fake_unw_up_SR.LL->SetDirectory(0);
        p_result.p_fake_unw_up_SR.ML        = new TH1F(Form("SRML_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRML_UNWFR_TOTAL_%s"      , chainTitleCh) , nMLsr   , 0.5, nMLsr+0.5 ); p_result.p_fake_unw_up_SR.ML->SetDirectory(0);
    }
    //For JES variations
    if (doFakes == 1 || isData==0) {
        p_result.p_jes_alt_up_SR.HH     = new TH1F(Form("SRHH_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_JES_UP_TOTAL_%s"   , chainTitleCh) , nHHsr , 0.5, nHHsr+0.5 ); p_result.p_jes_alt_up_SR.HH->SetDirectory(0);
        p_result.p_jes_alt_up_SR.HL     = new TH1F(Form("SRHL_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_JES_UP_TOTAL_%s"   , chainTitleCh) , nHLsr , 0.5, nHLsr+0.5 ); p_result.p_jes_alt_up_SR.HL->SetDirectory(0);
        p_result.p_jes_alt_up_SR.LL     = new TH1F(Form("SRLL_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_JES_UP_TOTAL_%s"   , chainTitleCh) , nLLsr , 0.5, nLLsr+0.5 ); p_result.p_jes_alt_up_SR.LL->SetDirectory(0);
        p_result.p_jes_alt_up_SR.ML     = new TH1F(Form("SRML_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_JES_UP_TOTAL_%s"   , chainTitleCh) , nMLsr , 0.5, nMLsr+0.5 ); p_result.p_jes_alt_up_SR.ML->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.HH     = new TH1F(Form("SRHH_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_JES_DN_TOTAL_%s"   , chainTitleCh) , nHHsr , 0.5, nHHsr+0.5 ); p_result.p_jes_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.HL     = new TH1F(Form("SRHL_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_JES_DN_TOTAL_%s"   , chainTitleCh) , nHLsr , 0.5, nHLsr+0.5 ); p_result.p_jes_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.LL     = new TH1F(Form("SRLL_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_JES_DN_TOTAL_%s"   , chainTitleCh) , nLLsr , 0.5, nLLsr+0.5 ); p_result.p_jes_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_jes_alt_dn_SR.ML     = new TH1F(Form("SRML_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_JES_DN_TOTAL_%s"   , chainTitleCh) , nMLsr , 0.5, nMLsr+0.5 ); p_result.p_jes_alt_dn_SR.ML->SetDirectory(0);
    }
    //For JER variations
    if (doFakes == 1 || isData==0) {
        p_result.p_jer_alt_up_SR.HH     = new TH1F(Form("SRHH_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_JER_UP_TOTAL_%s"   , chainTitleCh) , nHHsr , 0.5, nHHsr+0.5 ); p_result.p_jer_alt_up_SR.HH->SetDirectory(0);
        p_result.p_jer_alt_up_SR.HL     = new TH1F(Form("SRHL_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_JER_UP_TOTAL_%s"   , chainTitleCh) , nHLsr , 0.5, nHLsr+0.5 ); p_result.p_jer_alt_up_SR.HL->SetDirectory(0);
        p_result.p_jer_alt_up_SR.LL     = new TH1F(Form("SRLL_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_JER_UP_TOTAL_%s"   , chainTitleCh) , nLLsr , 0.5, nLLsr+0.5 ); p_result.p_jer_alt_up_SR.LL->SetDirectory(0);
        p_result.p_jer_alt_up_SR.ML     = new TH1F(Form("SRML_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_JER_UP_TOTAL_%s"   , chainTitleCh) , nMLsr , 0.5, nMLsr+0.5 ); p_result.p_jer_alt_up_SR.ML->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.HH     = new TH1F(Form("SRHH_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_JER_DN_TOTAL_%s"   , chainTitleCh) , nHHsr , 0.5, nHHsr+0.5 ); p_result.p_jer_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.HL     = new TH1F(Form("SRHL_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_JER_DN_TOTAL_%s"   , chainTitleCh) , nHLsr , 0.5, nHLsr+0.5 ); p_result.p_jer_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.LL     = new TH1F(Form("SRLL_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_JER_DN_TOTAL_%s"   , chainTitleCh) , nLLsr , 0.5, nLLsr+0.5 ); p_result.p_jer_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_jer_alt_dn_SR.ML     = new TH1F(Form("SRML_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_JER_DN_TOTAL_%s"   , chainTitleCh) , nMLsr , 0.5, nMLsr+0.5 ); p_result.p_jer_alt_dn_SR.ML->SetDirectory(0);
    }
    //For btag SF variations
    if (isData==0){
        p_result.p_btagSF_alt_up_SR.HH = new TH1F(Form("SRHH_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRHH_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nHHsr,  0.5, nHHsr+0.5 ); p_result.p_btagSF_alt_up_SR.HH->SetDirectory(0);
        p_result.p_btagSF_alt_up_SR.HL = new TH1F(Form("SRHL_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRHL_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nHLsr,  0.5, nHLsr+0.5 ); p_result.p_btagSF_alt_up_SR.HL->SetDirectory(0);
        p_result.p_btagSF_alt_up_SR.LL = new TH1F(Form("SRLL_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRLL_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nLLsr,  0.5, nLLsr+0.5 ); p_result.p_btagSF_alt_up_SR.LL->SetDirectory(0);
        p_result.p_btagSF_alt_up_SR.ML = new TH1F(Form("SRML_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRML_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nMLsr,  0.5, nMLsr+0.5 ); p_result.p_btagSF_alt_up_SR.ML->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.HH = new TH1F(Form("SRHH_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRHH_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nHHsr,  0.5, nHHsr+0.5 ); p_result.p_btagSF_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.HL = new TH1F(Form("SRHL_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRHL_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nHLsr,  0.5, nHLsr+0.5 ); p_result.p_btagSF_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.LL = new TH1F(Form("SRLL_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRLL_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nLLsr,  0.5, nLLsr+0.5 ); p_result.p_btagSF_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_btagSF_alt_dn_SR.ML = new TH1F(Form("SRML_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRML_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nMLsr,  0.5, nMLsr+0.5 ); p_result.p_btagSF_alt_dn_SR.ML->SetDirectory(0);
    }
    //For PU variations
    if (doFakes == 1 || isData==0) {
        p_result.p_pu_alt_up_SR.HH     = new TH1F(Form("SRHH_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_PU_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_pu_alt_up_SR.HH->SetDirectory(0);
        p_result.p_pu_alt_up_SR.HL     = new TH1F(Form("SRHL_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_PU_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_pu_alt_up_SR.HL->SetDirectory(0);
        p_result.p_pu_alt_up_SR.LL     = new TH1F(Form("SRLL_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_PU_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_pu_alt_up_SR.LL->SetDirectory(0);
        p_result.p_pu_alt_up_SR.ML     = new TH1F(Form("SRML_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_PU_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_pu_alt_up_SR.ML->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.HH     = new TH1F(Form("SRHH_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_PU_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_pu_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.HL     = new TH1F(Form("SRHL_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_PU_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_pu_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.LL     = new TH1F(Form("SRLL_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_PU_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_pu_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_pu_alt_dn_SR.ML     = new TH1F(Form("SRML_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_PU_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_pu_alt_dn_SR.ML->SetDirectory(0);
    }
    //For fastsim MET variations
    if (isData==0) {
        p_result.p_met_alt_up_SR.HH     = new TH1F(Form("SRHH_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_MET_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_met_alt_up_SR.HH->SetDirectory(0);
        p_result.p_met_alt_up_SR.HL     = new TH1F(Form("SRHL_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_MET_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_met_alt_up_SR.HL->SetDirectory(0);
        p_result.p_met_alt_up_SR.LL     = new TH1F(Form("SRLL_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_MET_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_met_alt_up_SR.LL->SetDirectory(0);
        p_result.p_met_alt_up_SR.ML     = new TH1F(Form("SRML_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_MET_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_met_alt_up_SR.ML->SetDirectory(0);
        p_result.p_met_alt_dn_SR.HH     = new TH1F(Form("SRHH_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_MET_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_met_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_met_alt_dn_SR.HL     = new TH1F(Form("SRHL_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_MET_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_met_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_met_alt_dn_SR.LL     = new TH1F(Form("SRLL_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_MET_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_met_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_met_alt_dn_SR.ML     = new TH1F(Form("SRML_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_MET_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_met_alt_dn_SR.ML->SetDirectory(0);
    }
    //For PU variations
    if (doFakes == 1 || isData==0) {
        p_result.p_lep_alt_up_SR.HH     = new TH1F(Form("SRHH_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_LEP_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_lep_alt_up_SR.HH->SetDirectory(0);
        p_result.p_lep_alt_up_SR.HL     = new TH1F(Form("SRHL_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_LEP_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_lep_alt_up_SR.HL->SetDirectory(0);
        p_result.p_lep_alt_up_SR.LL     = new TH1F(Form("SRLL_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_LEP_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_lep_alt_up_SR.LL->SetDirectory(0);
        p_result.p_lep_alt_up_SR.ML     = new TH1F(Form("SRML_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_LEP_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_lep_alt_up_SR.ML->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.HH     = new TH1F(Form("SRHH_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_LEP_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_lep_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.HL     = new TH1F(Form("SRHL_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_LEP_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_lep_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.LL     = new TH1F(Form("SRLL_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_LEP_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_lep_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_lep_alt_dn_SR.ML     = new TH1F(Form("SRML_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_LEP_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_lep_alt_dn_SR.ML->SetDirectory(0);
    }
    //For bb variations
    if (doFakes == 1 || isData==0) {
        p_result.p_bb_alt_up_SR.HH     = new TH1F(Form("SRHH_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_BB_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_bb_alt_up_SR.HH->SetDirectory(0);
        p_result.p_bb_alt_up_SR.HL     = new TH1F(Form("SRHL_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_BB_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_bb_alt_up_SR.HL->SetDirectory(0);
        p_result.p_bb_alt_up_SR.LL     = new TH1F(Form("SRLL_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_BB_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_bb_alt_up_SR.LL->SetDirectory(0);
        p_result.p_bb_alt_up_SR.ML     = new TH1F(Form("SRML_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_BB_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_bb_alt_up_SR.ML->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.HH     = new TH1F(Form("SRHH_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_BB_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_bb_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.HL     = new TH1F(Form("SRHL_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_BB_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_bb_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.LL     = new TH1F(Form("SRLL_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_BB_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_bb_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_bb_alt_dn_SR.ML     = new TH1F(Form("SRML_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_BB_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_bb_alt_dn_SR.ML->SetDirectory(0);
    }
    //For PU variations
    if (isData==0) {
        p_result.p_isr_alt_up_SR.HH     = new TH1F(Form("SRHH_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ISR_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_isr_alt_up_SR.HH->SetDirectory(0);
        p_result.p_isr_alt_up_SR.HL     = new TH1F(Form("SRHL_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ISR_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_isr_alt_up_SR.HL->SetDirectory(0);
        p_result.p_isr_alt_up_SR.LL     = new TH1F(Form("SRLL_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ISR_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_isr_alt_up_SR.LL->SetDirectory(0);
        p_result.p_isr_alt_up_SR.ML     = new TH1F(Form("SRML_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_ISR_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_isr_alt_up_SR.ML->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.HH     = new TH1F(Form("SRHH_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ISR_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_isr_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.HL     = new TH1F(Form("SRHL_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ISR_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_isr_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.LL     = new TH1F(Form("SRLL_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ISR_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_isr_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_isr_alt_dn_SR.ML     = new TH1F(Form("SRML_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_ISR_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_isr_alt_dn_SR.ML->SetDirectory(0);
    }
    //For theory variations
    if (isData==0) {
        p_result.p_pdf_alt_up_SR.HH     = new TH1F(Form("SRHH_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_PDF_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_pdf_alt_up_SR.HH->SetDirectory(0);
        p_result.p_pdf_alt_up_SR.HL     = new TH1F(Form("SRHL_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_PDF_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_pdf_alt_up_SR.HL->SetDirectory(0);
        p_result.p_pdf_alt_up_SR.LL     = new TH1F(Form("SRLL_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_PDF_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_pdf_alt_up_SR.LL->SetDirectory(0);
        p_result.p_pdf_alt_up_SR.ML     = new TH1F(Form("SRML_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_PDF_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_pdf_alt_up_SR.ML->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.HH     = new TH1F(Form("SRHH_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_PDF_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_pdf_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.HL     = new TH1F(Form("SRHL_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_PDF_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_pdf_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.LL     = new TH1F(Form("SRLL_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_PDF_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_pdf_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_pdf_alt_dn_SR.ML     = new TH1F(Form("SRML_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_PDF_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_pdf_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_scale_alt_up_SR.HH     = new TH1F(Form("SRHH_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_scale_alt_up_SR.HH->SetDirectory(0);
        p_result.p_scale_alt_up_SR.HL     = new TH1F(Form("SRHL_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_scale_alt_up_SR.HL->SetDirectory(0);
        p_result.p_scale_alt_up_SR.LL     = new TH1F(Form("SRLL_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_scale_alt_up_SR.LL->SetDirectory(0);
        p_result.p_scale_alt_up_SR.ML     = new TH1F(Form("SRML_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_scale_alt_up_SR.ML->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.HH     = new TH1F(Form("SRHH_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_scale_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.HL     = new TH1F(Form("SRHL_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_scale_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.LL     = new TH1F(Form("SRLL_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_scale_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_scale_alt_dn_SR.ML     = new TH1F(Form("SRML_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_scale_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.HH     = new TH1F(Form("SRHH_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_alphas_alt_up_SR.HH->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.HL     = new TH1F(Form("SRHL_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_alphas_alt_up_SR.HL->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.LL     = new TH1F(Form("SRLL_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_alphas_alt_up_SR.LL->SetDirectory(0);
        p_result.p_alphas_alt_up_SR.ML     = new TH1F(Form("SRML_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_alphas_alt_up_SR.ML->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.HH     = new TH1F(Form("SRHH_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_alphas_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.HL     = new TH1F(Form("SRHL_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_alphas_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.LL     = new TH1F(Form("SRLL_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_alphas_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_alphas_alt_dn_SR.ML     = new TH1F(Form("SRML_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_alphas_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.HH     = new TH1F(Form("SRHH_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_isrvar_alt_up_SR.HH->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.HL     = new TH1F(Form("SRHL_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_isrvar_alt_up_SR.HL->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.LL     = new TH1F(Form("SRLL_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_isrvar_alt_up_SR.LL->SetDirectory(0);
        p_result.p_isrvar_alt_up_SR.ML     = new TH1F(Form("SRML_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_isrvar_alt_up_SR.ML->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.HH     = new TH1F(Form("SRHH_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_isrvar_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.HL     = new TH1F(Form("SRHL_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_isrvar_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.LL     = new TH1F(Form("SRLL_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_isrvar_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_isrvar_alt_dn_SR.ML     = new TH1F(Form("SRML_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_isrvar_alt_dn_SR.ML->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.HH     = new TH1F(Form("SRHH_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHH_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_fsrvar_alt_up_SR.HH->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.HL     = new TH1F(Form("SRHL_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRHL_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_fsrvar_alt_up_SR.HL->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.LL     = new TH1F(Form("SRLL_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRLL_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_fsrvar_alt_up_SR.LL->SetDirectory(0);
        p_result.p_fsrvar_alt_up_SR.ML     = new TH1F(Form("SRML_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRML_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_fsrvar_alt_up_SR.ML->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.HH     = new TH1F(Form("SRHH_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHH_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nHHsr,  0.5, nHHsr+0.5 ); p_result.p_fsrvar_alt_dn_SR.HH->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.HL     = new TH1F(Form("SRHL_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRHL_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nHLsr,  0.5, nHLsr+0.5 ); p_result.p_fsrvar_alt_dn_SR.HL->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.LL     = new TH1F(Form("SRLL_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRLL_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nLLsr,  0.5, nLLsr+0.5 ); p_result.p_fsrvar_alt_dn_SR.LL->SetDirectory(0);
        p_result.p_fsrvar_alt_dn_SR.ML     = new TH1F(Form("SRML_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRML_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nMLsr,  0.5, nMLsr+0.5 ); p_result.p_fsrvar_alt_dn_SR.ML->SetDirectory(0);
        
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

        if (file->Get("counts")) {
            // h_counts = ((TH2F*)(file->Get("counts")))->Clone("h_counts");
            h_counts = (TH2F*)((file->Get("counts"))->Clone("h_counts"));
        }
        TString filename = currentFile->GetTitle();
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        // Loop over Events in current file
        for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){

            if (STOP_REQUESTED) break;

            //Get Event Content
            samesign.GetEntry(event);
            nEventsTotal++;

            if (!quiet) bar.progress(nEventsTotal, nEventsChain);

            // skim = (njets_unc_dn>=2 or njets_JER_dn>=2 or njets>=2 or njets_unc_up>=2 or njets_JER_up>=2) and \
            //        (met_unc_dn>=50 or met_JER_dn>=50 or met>=50 or met_unc_up>=50 or met_JER_up>=50) and \
            //        (hyp_class != 4 or is_real_data);
            if (!ss::skim()) continue; // FIXME

            if (isFastsim) {
                if ((fabs(mysparms[0]-ss::sparms()[0]) > 0.1) || (fabs(mysparms[1]-ss::sparms()[1]) > 0.1)) continue;
            }

            //Reject not triggered
            if (!isFastsim) {
                if (!ss::fired_trigger()) continue;
            }
            if (!ss::passes_met_filters()) continue;


            if (isHiggsScan) {
                // make sure the higgs mass point we are considering is the same as chain title
                if (fabs(mysparms[0]-ss::higgs_mass()) > 0.1) continue;
            }

            float weight =  ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG;

            // if (!ss::is_real_data()) {
            //     // FIXME
            //     weight *= ss::prefire2017_sf();
            // }

            if (isFastsim) {
                weight = getSMSscale1fb()*lumiAG;

                // if (year == 2016) {
                //     weight *= ss::prefire2016_sf();
                // } else if (year == 2017) {
                //     weight *= ss::prefire2017_sf();
                // }

            }

            if (istttt) {
                // force xsec here
                weight *= XSEC_TTTT / fabs(1000.0*ss::xsec());
            }
            weight*=scaleLumi;

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

            int nleps = (lep3good) ? ((ss::lep4_passes_id() and (ss::lep4_p4().pt() > (abs(ss::lep4_id())==11 ? 15 : 10))) ? 4 : 3) : 2;
            if (!doThirdLepton) nleps = 2;

            // if (analysis_category(ss::lep1_id(), ss::lep2_id(), ss::lep1_coneCorrPt(), ss::lep2_coneCorrPt(), ss::lep3_coneCorrPt(), nleps) == Multilepton) {
            //     float pto1 = max(max(lep1ccpt,lep2ccpt),lep3ccpt);
            //     float pto3 = min(min(lep1ccpt,lep2ccpt),lep3ccpt);
            //     float pto2 = lep1ccpt+lep2ccpt+lep3ccpt-pto1-pto3;
            //     if (ss::njets() <= 1 and ss::nbtags() == 0 
            //             and pto1 > 25 and pto2 > 20 and pto3 > 20
            //             and ((lep1id>0)^(lep3id>0))
            //             and ((abs(lep1id) == abs(lep3id))^(abs(lep2id) == abs(lep3id)))
            //             and not doFlips
            //             ) {
            //         float mtl3 = calcMT(lep3ccpt, ss::lep3_p4().phi(), ss::met(), ss::metPhi());
            //         float mtmax = max(max(mtl1, mtl2), mtl3);
            //         p_result.h_mtmax.CatFill(kMultilepton, mtmax, weight);
            //     }
            // }

            //Require nominal baseline selections
            if (!passes_baseline_ss(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), lep1id, lep2id, lep1pt, lep2pt, 50.)) continue;

            region_t categ = analysis_category(ss::lep1_id(), ss::lep2_id(), ss::lep1_coneCorrPt(), ss::lep2_coneCorrPt(), ss::lep3_coneCorrPt(), nleps);
            if (categ == Undefined) continue; // HighHigh = 0, HighLow, LowLow, Multilepton

            if (categ == Multilepton) {
                float mtl3 = calcMT(lep3ccpt, ss::lep3_p4().phi(), ss::met(), ss::metPhi());
                mtmin = min(mtl3, mtmin);
            }

            // FIXME
            // from /home/users/namin/2016/ss/master/SSAnalysis/commonUtils.h
            if (isWZ) {
                weight *= 1.23;
            }
            if (isttZ) {
            // if (isttZ || isttH) {
                weight *= 1.14;
            }


            if (isHiggsPseudoscalar) weight *= ss::xsec_ps()/ss::xsec();
            if (ss::is_real_data()==0) {
                // weight*=eventScaleFactor(lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht());
                // missing trigger SF
                // if ((!isWZ) && (!isttZ)) {
                // if ((!isWZ) && (!isttZ) && (!isttH)) {
                    if (lep1good) weight *= leptonScaleFactor(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                    if (lep2good) weight *= leptonScaleFactor(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                    if (categ == Multilepton) weight *= leptonScaleFactor(year, lep3id, lep3ccpt, lep3eta, ss::ht());
                    weight *= triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht());
                // }
                weight *= ss::weight_btagsf();
                weight *= getTruePUw(year, ss::trueNumInt()[0], 0);
                if (!isFastsim) {
                    // weight *= ss::decayWSF();
                }
            }
            if (isFastsim) {
                weight *= fastsim_triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht());
                if (lep1good) weight *= fastsim_leptonScaleFactor(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                if (lep2good) weight *= fastsim_leptonScaleFactor(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                if (categ == Multilepton) weight *= fastsim_leptonScaleFactor(year, lep3id, lep3ccpt, lep3eta, ss::ht());
                weight *= ss::weight_isr()*fastsim_isr_norm_central;

            }

            float weight_isr_up_alt = weight;
            float weight_isr_dn_alt = weight;
            if (isFastsim) {
              weight_isr_up_alt *= ss::weight_isr_UP()*fastsim_isr_norm_up;
              weight_isr_dn_alt *= ss::weight_isr_DN()*fastsim_isr_norm_down;
            }
            // if (isttW) {
            //     // want to take 50% of the effect as unc, and since we scale
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

            float weight_bb_up_alt = weight;
            if ((isttZ || isttW)) {
                // want to take full effect as unc, and since we scale
                // nominal weight, scale up twice
                if (ss::extragenb() == 2) {
                    float scaleamt = 0.7;
                    weight_bb_up_alt = (0.5*scaleamt+1)*weight;
                    weight *= 1.0;
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
                weight_pu_up_alt = getTruePUw(year, ss::trueNumInt()[0], 0)>0 ? weight*getTruePUw(year, ss::trueNumInt()[0], 1)/getTruePUw(year, ss::trueNumInt()[0], 0) : weight;
                weight_pu_dn_alt = getTruePUw(year, ss::trueNumInt()[0], 0)>0 ? weight*getTruePUw(year, ss::trueNumInt()[0],-1)/getTruePUw(year, ss::trueNumInt()[0], 0) : weight;
                if (categ != Multilepton) {
                    weight_lep_up_alt = weight*(1.0+leptonScaleFactorError(year,lep1id,  lep1pt,  lep1eta,  ss::ht())+leptonScaleFactorError(year,lep2id,  lep2pt,  lep2eta,  ss::ht()));
                } else {
                    weight_lep_up_alt = weight*(1.0+leptonScaleFactorError(year,lep1id,  lep1pt,  lep1eta,  ss::ht())+leptonScaleFactorError(year,lep2id,  lep2pt,  lep2eta,  ss::ht())+leptonScaleFactorError(year,lep3id,  lep3pt,  lep3eta,  ss::ht()));
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

            // Don't take leptons from photons if not x-gamma
            if ((ss::lep1_motherID()==-3 || ss::lep2_motherID()==-3) && !isGamma) {
                continue;
            }

            if (doFlips) {
                if (ss::hyp_class() != 4 && !isClass6) continue;
                float flipFact = 0.;
                if (abs(lep1id)==11){
                    float flr = flipRate(year,lep1pt, lep1eta);
                    flipFact += (flr/(1-flr));
                }
                if (abs(lep2id)==11){
                    float flr = flipRate(year,lep2pt, lep2eta);
                    flipFact += (flr/(1-flr));
                }
                weight *= flipFact;
                if (weight == 0.0) continue; // just quit if there are no flips.
            }

            if (truthfake) {
                if (ss::hyp_class() == ssclass) {
                    int nbadlegs = (ss::lep1_motherID() <= 0) + (ss::lep2_motherID() <= 0);
                    if (categ == Multilepton) nbadlegs += (ss::lep3_motherID() <= 0);
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

            //QCD Fakes
            bool isClass6Fake = false;
            if (doFakes == 1) {
                if (!isClass6) {
                    if (ss::hyp_class() == 3 and (categ == Multilepton)) {
                        if (lep1good && lep2good && !lep3good && lep3pt>min_pt_fake) {  // lep3 fake
                            float fr = fakeRate(year, lep3id, lep3ccpt, lep3eta, ss::ht());
                            float fra = alternativeFakeRate(year, lep3id, lep3ccpt, lep3eta, ss::ht());
                            if (!ignoreFakeFactor) weight *= fr / (1-fr);
                            if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                        } else {
                            continue;
                        }
                    } else {
                        if (ss::hyp_class() != 2 && ss::hyp_class() != 1) continue;
                        bool foundGoodLoose = false;
                        if (!lep1good && lep1pt>min_pt_fake) { // lep1 fake
                            float fr = fakeRate(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                            float fra = alternativeFakeRate(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                            if (!ignoreFakeFactor) weight *= fr/(1.-fr);
                            if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                            foundGoodLoose = true;
                        }
                        if (!lep2good && lep2pt>min_pt_fake) { // lep2 fake
                            float fr = fakeRate(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                            float fra = alternativeFakeRate(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                            if (!ignoreFakeFactor) weight *= fr/(1.-fr);
                            if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                            foundGoodLoose = true;
                        }
                        if (!foundGoodLoose) continue;
                        if (ss::hyp_class() == 1 && lep1pt>min_pt_fake && lep2pt>min_pt_fake) weight *= -1.;
                    }
                } else if (categ == Multilepton) {

                    bool lep1_lowpt_veto = lep1pt < (abs(lep1id) == 11 ? 15 : 10);
                    bool lep2_lowpt_veto = lep2pt < (abs(lep2id) == 11 ? 15 : 10);
                    bool lep3_lowpt_veto = lep3pt < (abs(lep3id) == 11 ? 15 : 10);

                    int nClass6Fakes = 0;
                    if (ss::lep3_fo() and !ss::lep3_tight() and !lep3_lowpt_veto and lep1good and lep2good && lep3pt>min_pt_fake) {  // lep3 fake
                        float fr = fakeRate(year, lep3id, lep3ccpt, lep3eta, ss::ht());
                        float fra = alternativeFakeRate(year, lep3id, lep3ccpt, lep3eta, ss::ht());
                        isClass6Fake = true;
                        nClass6Fakes++;
                        if (!ignoreFakeFactor) weight *= fr / (1-fr);
                        if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                    }
                    if (ss::lep2_fo() and !ss::lep2_tight() and !lep2_lowpt_veto and lep1good and lep3good && lep2pt>min_pt_fake) {  // lep2 fake
                        float fr = fakeRate(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                        float fra = alternativeFakeRate(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                        isClass6Fake = true;
                        nClass6Fakes++;
                        if (!ignoreFakeFactor) weight *= fr / (1-fr);
                        if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                    }
                    if (ss::lep1_fo() and !ss::lep1_tight() and !lep1_lowpt_veto and lep2good and lep3good && lep1pt>min_pt_fake) {  // lep1 fake
                        float fr = fakeRate(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                        float fra = alternativeFakeRate(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                        isClass6Fake = true;
                        nClass6Fakes++;
                        if (!ignoreFakeFactor) weight *= fr / (1-fr);
                        if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                    }
                    // subtract double fakes, add triple fakes: x = (f1*LTT + f2*TLT + f3*TTL - f1*f2*LLT - f2*f3*TLL - f1*f3*LTL + f1*f2*f3*LLL) /. {LTT -> LLT*f2, TTL -> TLL*f2, LTL -> LLL*f2}
                    if (nClass6Fakes == 2) weight *= -1;
                }
                // Real data fakes chain has prompt MC too (ttV,ttH)...we want to subtract out such prompt contributions
                if (!ss::is_real_data() && isData) {
                    weight *= -1.;
                    weight_alt_FR *= -1.;
                }
            }

            float mtnonz = mtmin;
            float zmass = -1;
            if (isClass6) {
                float zmass23 = lep2id == -lep3id ? (ss::lep2_p4()+ss::lep3_p4()).mass() : -999.0;
                float zmass31 = lep3id == -lep1id ? (ss::lep3_p4()+ss::lep1_p4()).mass() : -999.0;

                if (fabs(zmass31 - 91.2) < fabs(zmass23 - 91.2)) {
                    zmass = zmass31;
                    mtnonz = calcMT(lep2ccpt, ss::lep2_p4().phi(), ss::met(), ss::metPhi());
                } else {
                    zmass = zmass23;
                    mtnonz = calcMT(lep1ccpt, ss::lep1_p4().phi(), ss::met(), ss::metPhi());
                }
                if (fabs(zmass-91.2) > 15) continue;
                if (doFakes) {
                    // if (isData) {
                    //     if (!ss::is_real_data()) continue;
                    // }
                    if (!isClass6Fake) continue;
                }
                if( !( isClass6Fake || (lep1good && lep2good && lep3good) ) ) continue;
            }
            if ((categ != Multilepton) && isClass6) continue;


            // // if all 3 charges are the same, throw the event away
            // if (nleps > 2 and ((lep1id>0 and lep2id>0 and lep3id>0) or
            //                    (lep1id<0 and lep2id<0 and lep3id<0))) continue;

            //Reject duplicates (after selection otherwise flips are ignored...)
            if (isData && ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)){ continue; }
            }

            // LorentzVector visible;
            // for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
            //     if (ss::btags()[ijet].pt() > 40.) continue;
            //     visible += ss::btags()[ijet];
            // }
            // for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
            //     visible += ss::jets()[ijet];
            // }
            // visible += ss::lep1_p4();
            // visible += ss::lep2_p4();
            // if (nleps > 2) visible += ss::lep3_p4();
            // float mvis = visible.M();
            // float mtvis = visible.Mt();

            float pt1 = lep1ccpt;
            float pt2 = lep2ccpt;
            float pt3 = lep3ccpt;
            float pto1 = max(max(pt1,pt2),pt3);
            float pto3 = min(min(pt1,pt2),pt3);
            float pto2 = pt1+pt2+pt3-pto1-pto3;
            int mytype = ss::hyp_type();
            if (mytype==2 && abs(lep1id)==13) mytype = 1;



            int SR = signal_region_ss(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt, nleps, isClass6, mtnonz);
            int SRgenmet = signal_region_ss(ss::njets(), ss::nbtags(), ss::gen_met(), ss::ht(), mtmin, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt, nleps, isClass6, mtnonz);

            // JEC
            float mll = (ss::lep1_p4()*lep1ccpt/lep1pt+ss::lep2_p4()*lep2ccpt/lep2pt).M();
            float mtl1_unc_up = calcMT(lep1ccpt, ss::lep1_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
            float mtl2_unc_up = calcMT(lep2ccpt, ss::lep2_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
            float mtmin_unc_up = mtl1_unc_up > mtl2_unc_up ? mtl2_unc_up : mtl1_unc_up;
            float mtl1_unc_dn = calcMT(lep1ccpt, ss::lep1_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
            float mtl2_unc_dn = calcMT(lep2ccpt, ss::lep2_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
            float mtmin_unc_dn = mtl1_unc_dn > mtl2_unc_dn ? mtl2_unc_dn : mtl1_unc_dn;
            int SR_unc_up = signal_region_ss(ss::njets_unc_up(), ss::nbtags_unc_up(), ss::met_unc_up(), ss::ht_unc_up(), mtmin_unc_up, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6, mtnonz);
            int SR_unc_dn = signal_region_ss(ss::njets_unc_up(), ss::nbtags_unc_up(), ss::met_unc_up(), ss::ht_unc_up(), mtmin_unc_up, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6, mtnonz);

            // JER
            float mtl1_JER_up = 0.;
            float mtl2_JER_up = 0.;
            float mtmin_JER_up = 0.;
            float mtl1_JER_dn = 0.;
            float mtl2_JER_dn = 0.;
            float mtmin_JER_dn = 0.;
            int SR_JER_up = SR;
            int SR_JER_dn = SR;
            if (!ss::is_real_data()) {
                mtl1_JER_up = calcMT(lep1ccpt, ss::lep1_p4().phi(), ss::met_JER_up(), ss::metPhi_JER_up());
                mtl2_JER_up = calcMT(lep2ccpt, ss::lep2_p4().phi(), ss::met_JER_up(), ss::metPhi_JER_up());
                mtmin_JER_up = mtl1_JER_up > mtl2_JER_up ? mtl2_JER_up : mtl1_JER_up;
                mtl1_JER_dn = calcMT(lep1ccpt, ss::lep1_p4().phi(), ss::met_JER_dn(), ss::metPhi_JER_dn());
                mtl2_JER_dn = calcMT(lep2ccpt, ss::lep2_p4().phi(), ss::met_JER_dn(), ss::metPhi_JER_dn());
                mtmin_JER_dn = mtl1_JER_dn > mtl2_JER_dn ? mtl2_JER_dn : mtl1_JER_dn;
                SR_JER_up = signal_region_ss(ss::njets_JER_up(), ss::nbtags_JER_up(), ss::met_JER_up(), ss::ht_JER_up(), mtmin_JER_up, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6, mtnonz);
                SR_JER_dn = signal_region_ss(ss::njets_JER_dn(), ss::nbtags_JER_dn(), ss::met_JER_dn(), ss::ht_JER_dn(), mtmin_JER_dn, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6, mtnonz);
            }


            float mvavalue = -10.;
            float mvavalueup = -10.;
            float mvavaluedn = -10.;
            float mvavalueJERup = -10.;
            float mvavalueJERdn = -10.;
            float pred = -1.;
            if (evaluateBDT) {

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
                mvavalue = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);

                f_nbtags = ss::bdt_jec_up_nbtags();
                f_njets = ss::bdt_jec_up_njets();
                f_met = ss::bdt_jec_up_met();
                f_htb = ss::bdt_jec_up_htb();
                f_nlb40 = ss::bdt_jec_up_nlb40();
                f_ntb40 = ss::bdt_jec_up_ntb40();
                mvavalueup = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);

                f_nbtags = ss::bdt_jec_dn_nbtags();
                f_njets = ss::bdt_jec_dn_njets();
                f_met = ss::bdt_jec_dn_met();
                f_htb = ss::bdt_jec_dn_htb();
                f_nlb40 = ss::bdt_jec_dn_nlb40();
                f_ntb40 = ss::bdt_jec_dn_ntb40();
                mvavaluedn = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);

                f_nbtags = ss::bdt_jer_up_nbtags();
                f_njets = ss::bdt_jer_up_njets();
                f_met = ss::bdt_jer_up_met();
                f_htb = ss::bdt_jer_up_htb();
                f_nlb40 = ss::bdt_jer_up_nlb40();
                f_ntb40 = ss::bdt_jer_up_ntb40();
                mvavalueJERup = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);

                f_nbtags = ss::bdt_jer_dn_nbtags();
                f_njets = ss::bdt_jer_dn_njets();
                f_met = ss::bdt_jer_dn_met();
                f_htb = ss::bdt_jer_dn_htb();
                f_nlb40 = ss::bdt_jer_dn_nlb40();
                f_ntb40 = ss::bdt_jer_dn_ntb40();
                mvavalueJERdn = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);

                p_result.h_disc.br->Fill(mvavalue,weight);
            }

            // int SRdisc =        getBDTBin(nbdtbins, mvavalue, SR==1);
            // int SRdisc_unc_up = getBDTBin(nbdtbins, mvavalueup, SR_unc_up==1);
            // int SRdisc_unc_dn = getBDTBin(nbdtbins, mvavaluedn, SR_unc_dn==1);
            // int SRdisc_JER_up = getBDTBin(nbdtbins, mvavalueJERup, SR_JER_up==1);
            // int SRdisc_JER_dn = getBDTBin(nbdtbins, mvavalueJERdn, SR_JER_dn==1);

            // // FIXME -- put CRW into 2nd bin
            // if (SR == 2) {
            //     SRdisc = 2;
            //     SRdisc_unc_up = 2;
            //     SRdisc_unc_dn = 2;
            //     SRdisc_JER_up = 2;
            //     SRdisc_JER_dn = 2;
            // }

            // if (SR >= 0) {
            //     std::cout <<  " mvavalueup: " << mvavalueup <<  " mvavaluedn: " << mvavaluedn <<  " mvavalueJERup: " << mvavalueJERup <<  " mvavalueJERdn: " << mvavalueJERdn <<  std::endl;
            // }

            if (isData == 0 && SR_unc_up > 0) p_result.p_jes_alt_up_SR.CatFill(categ, SR_unc_up, weight);
            if (isData == 0 && SR_unc_dn > 0) p_result.p_jes_alt_dn_SR.CatFill(categ, SR_unc_dn, weight);
            if (isData == 0 && SR_JER_up > 0) p_result.p_jer_alt_up_SR.CatFill(categ, SR_JER_up, weight);
            if (isData == 0 && SR_JER_dn > 0) p_result.p_jer_alt_dn_SR.CatFill(categ, SR_JER_dn, weight);
            if (isData == 0 && SRgenmet > 0) p_result.p_met_alt_up_SR.CatFill(categ, SRgenmet, weight);
            if (doFakes == 1 && SR >= 0) p_result.p_fake_alt_up_SR.CatFill(categ, SR, weight_alt_FR);
            if (doFakes == 1 && SR >= 0) p_result.p_fake_unw_up_SR.CatFill(categ, SR, weight > 0 ? 1 : -1);
            if (isData == 0 && SR >= 0) p_result.p_btagSF_alt_up_SR.CatFill(categ, SR, weight_btag_up_alt);
            if (isData == 0 && SR >= 0) p_result.p_btagSF_alt_dn_SR.CatFill(categ, SR, weight_btag_dn_alt);
            if (isData == 0 && SR >= 0) p_result.p_pu_alt_up_SR.CatFill(categ, SR, weight_pu_up_alt);
            if (isData == 0 && SR >= 0) p_result.p_pu_alt_dn_SR.CatFill(categ, SR, weight_pu_dn_alt);
            if (isFastsim && SR >= 0) p_result.p_isr_alt_up_SR.CatFill(categ, SR, weight_isr_up_alt);
            if (isFastsim && SR >= 0) p_result.p_isr_alt_dn_SR.CatFill(categ, SR, weight_isr_dn_alt);
            if (isData == 0 && SR >= 0) p_result.p_bb_alt_up_SR.CatFill(categ, SR, weight_bb_up_alt);
            if (isData == 0 && SR >= 0) p_result.p_lep_alt_up_SR.CatFill(categ, SR, weight_lep_up_alt);
            if (SR > 0) {
                p_result.p_scale_alt_up_SR.CatFill(categ, SR, (ss::weight_scale_UP() > -9000 ? ss::weight_scale_UP() : 1.0)*weight);
                p_result.p_scale_alt_dn_SR.CatFill(categ, SR, (ss::weight_scale_DN() > -9000 ? ss::weight_scale_DN() : 1.0)*weight);
                p_result.p_alphas_alt_up_SR.CatFill(categ, SR, (ss::weight_alphas_UP() > -9000 ? ss::weight_alphas_UP() : 1.0)*weight);
                p_result.p_alphas_alt_dn_SR.CatFill(categ, SR, (ss::weight_alphas_DN() > -9000 ? ss::weight_alphas_DN() : 1.0)*weight);
                p_result.p_pdf_alt_up_SR.CatFill(categ, SR, (ss::weight_pdf_UP() > -9000 ? ss::weight_pdf_UP() : 1.0)*weight);
                p_result.p_pdf_alt_dn_SR.CatFill(categ, SR, (ss::weight_pdf_DN() > -9000 ? ss::weight_pdf_DN() : 1.0)*weight);
                p_result.SR.CatFill(categ, SR, weight);
            }


            // OR together all possible categories for this event
            int kcategs = kBaseline;
            if (categ == Multilepton && SR > 0) {
                kcategs |= kMultilepton;
                if (SR >= 24) kcategs |= kMultileptonOnZ;
                else kcategs |= kMultileptonOffZ;
            }
            if (SR > 0) kcategs |= kSignalRegion;
            if (categ == HighHigh) kcategs |= kHighHigh;
            if (categ == HighLow) kcategs |= kHighLow;
            if (categ == LowLow) kcategs |= kLowLow;

            // Only plot 3rd lepton if event is in ML region
            bool plotlep3 = categ == Multilepton;


            // Fill based on categories
            p_result.h_l1pt.CatFill(kcategs, pto1, weight);
            p_result.h_l2pt.CatFill(kcategs, pto2, weight);
            p_result.h_l3pt.CatFill(kcategs, pto3, weight);
            if (abs(ss::lep1_id()) == 11) {
                p_result.h_el_l1eta.CatFill(kcategs, lep1eta, weight);
                p_result.h_el_l1pt.CatFill(kcategs, lep1ccpt, weight);
            } else {
                p_result.h_mu_l1eta.CatFill(kcategs, lep1eta, weight);
                p_result.h_mu_l1pt.CatFill(kcategs, lep1ccpt, weight);
            }
            if (abs(ss::lep2_id()) == 11) {
                p_result.h_el_l2eta.CatFill(kcategs, lep2eta, weight);
                p_result.h_el_l2pt.CatFill(kcategs, lep2ccpt, weight);
            } else {
                p_result.h_mu_l2eta.CatFill(kcategs, lep2eta, weight);
                p_result.h_mu_l2pt.CatFill(kcategs, lep2ccpt, weight);
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
            p_result.h_mllos.CatFill(kcategs, zmass, weight);

            // float mt2 = ss::mt2();
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
            p_result.h_twobinmt2.CatFill(kcategs, mt2, weight);
            p_result.h_twobinmtmin.CatFill(kcategs, kcategs & kMultileptonOnZ ? mtnonz : mtmin, weight);
            p_result.h_nbtags.CatFill(kcategs, ss::nbtags(), weight);
            p_result.h_njets.CatFill(kcategs, ss::njets(), weight);
            p_result.h_nleps.CatFill(kcategs, nleps, weight);
            p_result.h_nleptonicW.CatFill(kcategs, ss::nleptonicW(), weight);
            p_result.h_type.CatFill(kcategs, mytype, weight);
            p_result.h_type3.CatFill(kcategs, (abs(lep1id)==11)+(abs(lep2id)==11)+(abs(lep3id)==11), weight);
            p_result.h_category.CatFill(kcategs,
                    1.0*(categ == HighHigh) +
                    2.0*(categ == HighLow) +
                    3.0*(categ == LowLow) +
                    4.0*((kcategs & kMultileptonOffZ)>0) +
                    5.0*((kcategs & kMultileptonOnZ)>0),
                    weight);
            p_result.h_charge.CatFill(kcategs, lep1id > 0 ? -0.5 : 0.5, weight);
            p_result.h_charge3.CatFill(kcategs, ((lep1id>0)^(lep3id>0))-0.5, weight); // 0.5 if 3 charges are SSO, -0.5 if SSS
            p_result.h_nvtx.CatFill(kcategs, ss::nGoodVertices(), weight);



            if (write_tree) {
                output_tree.FillVars(
                        ss::event(), ss::lumi(), ss::run(),
                        weight, SR, ss::hyp_class(), categ, kcategs,
                        lep1id, lep2id, lep3id, ss::nbtags(), ss::njets(), nleps,
                        ss::met(), ss::ht(), mtmin, mt2, mt2min
                        );
            }


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


float getSMSscale1fb() {
    if (!h_counts) {
        std::cerr << "Count histogram (h_counts) isn't loaded!" << std::endl;
        exit(1);
    }
    float count = h_counts->GetBinContent(h_counts->FindBin(ss::sparms()[0],ss::sparms()[1]));
    // return 1000.0*ss::xsec()/count;
    return 1000.0*ss::xsec()/count;
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

