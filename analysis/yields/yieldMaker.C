#include <bitset>
#include <iostream>
#include <algorithm>
#include "TChain.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "Math/LorentzVector.h"
#include "TKey.h"
#include "TSystem.h"
#include "TMVA/Reader.h"
#include "../misc/class_files/v8.02/SS.h"
#include "../../common/CORE/SSSelections.h"
#include "../../common/CORE/Tools/dorky/dorky.h"
// #include "../../common/CORE/Tools/utils.h"
#include "../misc/common_utils.h"
#include "../misc/signal_regions.h"
#include "../misc/bdt.h"
#include "xgbheader.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "../misc/tqdm.h"

#define WRITE(var) { if (plots.var) plots.var->Write(); }

bool STOP_REQUESTED = false;

float XSEC_TTTT = 11.97;
float scaleLumi = 1; // 75/35.87

struct SR_t     { TH1F* EE; TH1F* EM; TH1F* MM; TH1F* TOTAL; };
struct triple_t { TH1F* ttzcr; TH1F* ttwcr; TH1F* sr; TH1F* br; };
struct plots_t  {
    SR_t SRCR;
    SR_t SRDISC;
    SR_t SR;

    SR_t p_alphas_alt_dn_SRCR;
    SR_t p_alphas_alt_dn_SRDISC;
    SR_t p_alphas_alt_up_SRCR;
    SR_t p_alphas_alt_up_SRDISC;
    SR_t p_bb_alt_up_SRCR;
    SR_t p_bb_alt_up_SRDISC;
    SR_t p_bb_alt_dn_SRCR;
    SR_t p_bb_alt_dn_SRDISC;
    SR_t p_btagSF_alt_dn_SRCR;
    SR_t p_btagSF_alt_dn_SRDISC;
    SR_t p_btagSF_alt_up_SRCR;
    SR_t p_btagSF_alt_up_SRDISC;
    SR_t p_fake_alt_up_SRCR;
    SR_t p_fake_alt_up_SRDISC;
    SR_t p_fake_unw_up_SRCR;
    SR_t p_fake_unw_up_SRDISC;
    SR_t p_fsrvar_alt_dn_SRCR;
    SR_t p_fsrvar_alt_dn_SRDISC;
    SR_t p_fsrvar_alt_up_SRCR;
    SR_t p_fsrvar_alt_up_SRDISC;
    SR_t p_isr_alt_up_SRCR;
    SR_t p_isr_alt_up_SRDISC;
    SR_t p_isr_alt_dn_SRCR;
    SR_t p_isr_alt_dn_SRDISC;
    SR_t p_isrvar_alt_dn_SRCR;
    SR_t p_isrvar_alt_dn_SRDISC;
    SR_t p_isrvar_alt_up_SRCR;
    SR_t p_isrvar_alt_up_SRDISC;
    SR_t p_jer_alt_dn_SRCR;
    SR_t p_jer_alt_dn_SRDISC;
    SR_t p_jer_alt_up_SRCR;
    SR_t p_jer_alt_up_SRDISC;
    SR_t p_jes_alt_dn_SRCR;
    SR_t p_jes_alt_dn_SRDISC;
    SR_t p_jes_alt_up_SRCR;
    SR_t p_jes_alt_up_SRDISC;
    SR_t p_met_alt_up_SRCR;
    SR_t p_met_alt_up_SRDISC;
    SR_t p_met_alt_dn_SRCR;
    SR_t p_met_alt_dn_SRDISC;
    SR_t p_lep_alt_up_SRCR;
    SR_t p_lep_alt_up_SRDISC;
    SR_t p_lep_alt_dn_SRCR;
    SR_t p_lep_alt_dn_SRDISC;
    SR_t p_pdf_alt_dn_SRCR;
    SR_t p_pdf_alt_dn_SRDISC;
    SR_t p_pdf_alt_up_SRCR;
    SR_t p_pdf_alt_up_SRDISC;
    SR_t p_pu_alt_dn_SRCR;
    SR_t p_pu_alt_dn_SRDISC;
    SR_t p_pu_alt_up_SRCR;
    SR_t p_pu_alt_up_SRDISC;
    SR_t p_scale_alt_dn_SRCR;
    SR_t p_scale_alt_dn_SRDISC;
    SR_t p_scale_alt_up_SRCR;
    SR_t p_scale_alt_up_SRDISC;

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

    triple_t h_ht_sr1;
    triple_t h_ht_sr2;
    triple_t h_ht_sr4;
    triple_t h_ht_sr5;
    triple_t h_ht_sr11;

};


//function declaration
plots_t run(TChain *chain, int year, TString options);
float calcDeltaPhi(float phi1, float phi2);
float calcMT(float pt1, float phi1, float pt2, float phi2);
void avoidNegativeYields(TH1F* plot);
void initHistError(bool usePoisson, TH1F* plot);
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
    if (options.Contains("Data2016")) {
        year = 2016;
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
    TString outname = Form("%s/output_%d_%s.root", outputdir.Data(), year, chainTitle.Data());

    if (!quiet) std::cout << "Writing out " << outname << std::endl;
    TFile *fout = new TFile(outname, "RECREATE");

    WRITE(h_ht_sr1.sr)      ;
    WRITE(h_ht_sr2.sr)      ;
    WRITE(h_ht_sr4.sr)      ;
    WRITE(h_ht_sr5.sr)      ;
    WRITE(h_ht_sr11.sr)      ;

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

    WRITE(h_mid1.br); WRITE(h_mid2.br); WRITE(h_mid3.br);

    WRITE(h_charge3.sr);

    WRITE(h_mllos.ttzcr)     ; 

    WRITE(h_disc.br);
    WRITE(SRCR.TOTAL); WRITE(SRDISC.TOTAL); WRITE(SR.TOTAL);

    WRITE(h_lepsf.br);
    WRITE(h_btagsf.br);
    WRITE(h_wsf.br);
    WRITE(h_puw.br);
    WRITE(h_trigsf.br);
    WRITE(h_isrw.br);

    WRITE(p_alphas_alt_dn_SRCR.TOTAL); WRITE(p_alphas_alt_dn_SRDISC.TOTAL);
    WRITE(p_fake_alt_up_SRCR.TOTAL); WRITE(p_fake_alt_up_SRDISC.TOTAL);
    WRITE(p_fake_unw_up_SRCR.TOTAL); WRITE(p_fake_unw_up_SRDISC.TOTAL);
    WRITE(p_alphas_alt_up_SRCR.TOTAL); WRITE(p_alphas_alt_up_SRDISC.TOTAL);
    WRITE(p_bb_alt_up_SRCR.TOTAL); WRITE(p_bb_alt_up_SRDISC.TOTAL);
    WRITE(p_bb_alt_dn_SRCR.TOTAL); WRITE(p_bb_alt_dn_SRDISC.TOTAL);
    WRITE(p_btagSF_alt_dn_SRCR.TOTAL); WRITE(p_btagSF_alt_dn_SRDISC.TOTAL);
    WRITE(p_btagSF_alt_up_SRCR.TOTAL); WRITE(p_btagSF_alt_up_SRDISC.TOTAL);
    WRITE(p_fsrvar_alt_dn_SRCR.TOTAL); WRITE(p_fsrvar_alt_dn_SRDISC.TOTAL);
    WRITE(p_fsrvar_alt_up_SRCR.TOTAL); WRITE(p_fsrvar_alt_up_SRDISC.TOTAL);
    WRITE(p_isr_alt_up_SRCR.TOTAL); WRITE(p_isr_alt_up_SRDISC.TOTAL);
    WRITE(p_isr_alt_dn_SRCR.TOTAL); WRITE(p_isr_alt_dn_SRDISC.TOTAL);
    WRITE(p_isrvar_alt_dn_SRCR.TOTAL); WRITE(p_isrvar_alt_dn_SRDISC.TOTAL);
    WRITE(p_isrvar_alt_up_SRCR.TOTAL); WRITE(p_isrvar_alt_up_SRDISC.TOTAL);
    WRITE(p_jer_alt_dn_SRCR.TOTAL); WRITE(p_jer_alt_dn_SRDISC.TOTAL);
    WRITE(p_jer_alt_up_SRCR.TOTAL); WRITE(p_jer_alt_up_SRDISC.TOTAL);
    WRITE(p_jes_alt_dn_SRCR.TOTAL); WRITE(p_jes_alt_dn_SRDISC.TOTAL);
    WRITE(p_jes_alt_up_SRCR.TOTAL); WRITE(p_jes_alt_up_SRDISC.TOTAL);
    WRITE(p_lep_alt_up_SRCR.TOTAL); WRITE(p_lep_alt_up_SRDISC.TOTAL);
    WRITE(p_lep_alt_dn_SRCR.TOTAL); WRITE(p_lep_alt_dn_SRDISC.TOTAL);
    WRITE(p_met_alt_up_SRCR.TOTAL); WRITE(p_met_alt_up_SRDISC.TOTAL);
    WRITE(p_met_alt_dn_SRCR.TOTAL); WRITE(p_met_alt_dn_SRDISC.TOTAL);
    WRITE(p_pdf_alt_dn_SRCR.TOTAL); WRITE(p_pdf_alt_dn_SRDISC.TOTAL);
    WRITE(p_pdf_alt_up_SRCR.TOTAL); WRITE(p_pdf_alt_up_SRDISC.TOTAL);
    WRITE(p_pu_alt_dn_SRCR.TOTAL); WRITE(p_pu_alt_dn_SRDISC.TOTAL);
    WRITE(p_pu_alt_up_SRCR.TOTAL); WRITE(p_pu_alt_up_SRDISC.TOTAL);
    WRITE(p_scale_alt_dn_SRCR.TOTAL); WRITE(p_scale_alt_dn_SRDISC.TOTAL);
    WRITE(p_scale_alt_up_SRCR.TOTAL); WRITE(p_scale_alt_up_SRDISC.TOTAL);

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


    bool isData = 0;
    bool doFlips = 0;
    int doFakes = 0;
    int exclude = 0;
    bool isSignal = 0;
    bool isGamma = 0;
    bool truthfake = 0;
    bool ignoreFakeFactor = 0;

    bool evaluateBDT = options.Contains("evaluateBDT");
    bool quiet = options.Contains("quiet");
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
    if (options.Contains("doXgamma")) {
        if (!quiet) std::cout << "Doing x+gamma" << std::endl;
        isGamma = 1;
    }

    if (!quiet) cout << "Running with lumi=" << lumiAG*scaleLumi << endl;

    // out_file->cd();

    plots_t plots;

    TString chainTitle = chain->GetTitle();
    const char* chainTitleCh = chainTitle.Data();
    if (!quiet) std::cout << "Working on " << chainTitle << std::endl;

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

    plots_t  p_result;

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
    p_result.h_disc.br     = new TH1F(Form("br_disc_%s"       , chainTitleCh) , Form("disc_%s"       , chainTitleCh) , 20       , 0.0    , 1.0);
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

    p_result.h_lepsf.br = new TH1F(Form("br_lepsf_%s"   , chainTitleCh) , Form("lepsf_%s"  , chainTitleCh) , 50 , 0.7 , 1.3);
    p_result.h_btagsf.br = new TH1F(Form("br_btagsf_%s" , chainTitleCh) , Form("btagsf_%s" , chainTitleCh) , 50 , 0.7 , 1.3);
    p_result.h_wsf.br = new TH1F(Form("br_wsf_%s"       , chainTitleCh) , Form("wsf_%s"    , chainTitleCh) , 50 , 0.7 , 1.3);
    p_result.h_puw.br = new TH1F(Form("br_puw_%s"       , chainTitleCh) , Form("puw_%s"    , chainTitleCh) , 50 , 0.7 , 1.3);
    p_result.h_trigsf.br = new TH1F(Form("br_trigsf_%s" , chainTitleCh) , Form("trigsf_%s" , chainTitleCh) , 50 , 0.7 , 1.3);
    p_result.h_isrw.br = new TH1F(Form("br_isrw_%s"     , chainTitleCh) , Form("isrw_%s"   , chainTitleCh) , 50 , 0.7 , 1.3);

    p_result.h_ht_sr1.sr       = new TH1F(Form("sr_ht_sr1_%s"         , chainTitleCh) , Form("ht_sr1_%s"         , chainTitleCh) , 10      , 100   , 1600);
    p_result.h_ht_sr2.sr       = new TH1F(Form("sr_ht_sr2_%s"         , chainTitleCh) , Form("ht_sr2_%s"         , chainTitleCh) , 10      , 100   , 1600);
    p_result.h_ht_sr4.sr       = new TH1F(Form("sr_ht_sr4_%s"         , chainTitleCh) , Form("ht_sr4_%s"         , chainTitleCh) , 10      , 100   , 1600);
    p_result.h_ht_sr5.sr       = new TH1F(Form("sr_ht_sr5_%s"         , chainTitleCh) , Form("ht_sr5_%s"         , chainTitleCh) , 10      , 100   , 1600);
    p_result.h_ht_sr11.sr       = new TH1F(Form("sr_ht_sr11_%s"         , chainTitleCh) , Form("ht_sr11_%s"         , chainTitleCh) , 10      , 100   , 1600);

    //For FR variations
    if (doFakes == 1) {
        p_result.p_fake_alt_up_SRCR.TOTAL        = new TH1F(Form("SRCR_FR_TOTAL_%s"      , chainTitleCh) , Form("SRCR_FR_TOTAL_%s"      , chainTitleCh) , nsr   , 0.5, nsr+0.5 );
        p_result.p_fake_alt_up_SRDISC.TOTAL        = new TH1F(Form("SRDISC_FR_TOTAL_%s"      , chainTitleCh) , Form("SRDISC_FR_TOTAL_%s"      , chainTitleCh) , nsrdisc   , 0.5, nsrdisc+0.5 );
    } else {
        p_result.p_fake_alt_up_SRCR.TOTAL        = 0;
        p_result.p_fake_alt_up_SRDISC.TOTAL        = 0;
    }
    //For unw FR application counts
    if (doFakes == 1) {
        p_result.p_fake_unw_up_SRCR.TOTAL        = new TH1F(Form("SRCR_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRCR_UNWFR_TOTAL_%s"      , chainTitleCh) , nsr   , 0.5, nsr+0.5 );
        p_result.p_fake_unw_up_SRDISC.TOTAL        = new TH1F(Form("SRDISC_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRDISC_UNWFR_TOTAL_%s"      , chainTitleCh) , nsrdisc   , 0.5, nsrdisc+0.5 );
    } else {
        p_result.p_fake_unw_up_SRCR.TOTAL        = 0;
        p_result.p_fake_unw_up_SRDISC.TOTAL        = 0;
    }
    //For JES variations
    if (doFakes == 1 || isData==0) {
        p_result.p_jes_alt_up_SRCR.TOTAL     = new TH1F(Form("SRCR_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JES_UP_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 );
        p_result.p_jes_alt_dn_SRCR.TOTAL     = new TH1F(Form("SRCR_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JES_DN_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 );
        p_result.p_jes_alt_up_SRDISC.TOTAL     = new TH1F(Form("SRDISC_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JES_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 );
        p_result.p_jes_alt_dn_SRDISC.TOTAL     = new TH1F(Form("SRDISC_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JES_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 );
    } else {
        p_result.p_jes_alt_up_SRCR.TOTAL     = 0;
        p_result.p_jes_alt_dn_SRCR.TOTAL     = 0;
        p_result.p_jes_alt_up_SRDISC.TOTAL     = 0;
        p_result.p_jes_alt_dn_SRDISC.TOTAL     = 0;
    }
    //For JER variations
    if (doFakes == 1 || isData==0) {
        p_result.p_jer_alt_up_SRCR.TOTAL     = new TH1F(Form("SRCR_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JER_UP_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 );
        p_result.p_jer_alt_dn_SRCR.TOTAL     = new TH1F(Form("SRCR_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JER_DN_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 );
        p_result.p_jer_alt_up_SRDISC.TOTAL     = new TH1F(Form("SRDISC_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JER_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 );
        p_result.p_jer_alt_dn_SRDISC.TOTAL     = new TH1F(Form("SRDISC_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JER_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 );
    } else {
        p_result.p_jer_alt_up_SRCR.TOTAL     = 0;
        p_result.p_jer_alt_dn_SRCR.TOTAL     = 0;
        p_result.p_jer_alt_up_SRDISC.TOTAL     = 0;
        p_result.p_jer_alt_dn_SRDISC.TOTAL     = 0;
    }
    //For btag SF variations
    if (isData==0){
        p_result.p_btagSF_alt_up_SRCR.TOTAL = new TH1F(Form("SRCR_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRCR_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nsr,  0.5, nsr+0.5 );
        p_result.p_btagSF_alt_dn_SRCR.TOTAL = new TH1F(Form("SRCR_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRCR_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nsr,  0.5, nsr+0.5 );
        p_result.p_btagSF_alt_up_SRDISC.TOTAL = new TH1F(Form("SRDISC_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRDISC_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_btagSF_alt_dn_SRDISC.TOTAL = new TH1F(Form("SRDISC_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRDISC_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nsrdisc,  0.5, nsrdisc+0.5 );
    }
    else {
        p_result.p_btagSF_alt_up_SRCR.TOTAL = 0;
        p_result.p_btagSF_alt_dn_SRCR.TOTAL = 0;
        p_result.p_btagSF_alt_up_SRDISC.TOTAL = 0;
        p_result.p_btagSF_alt_dn_SRDISC.TOTAL = 0;
    }
    //For PU variations
    if (doFakes == 1 || isData==0) {
        p_result.p_pu_alt_up_SRCR.TOTAL     = new TH1F(Form("SRCR_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PU_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_pu_alt_dn_SRCR.TOTAL     = new TH1F(Form("SRCR_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PU_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_pu_alt_up_SRDISC.TOTAL     = new TH1F(Form("SRDISC_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PU_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_pu_alt_dn_SRDISC.TOTAL     = new TH1F(Form("SRDISC_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PU_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    } else {
        p_result.p_pu_alt_up_SRCR.TOTAL     = 0;
        p_result.p_pu_alt_dn_SRCR.TOTAL     = 0;
        p_result.p_pu_alt_up_SRDISC.TOTAL     = 0;
        p_result.p_pu_alt_dn_SRDISC.TOTAL     = 0;
    }
    //For fastsim MET variations
    if (isData==0) {
        p_result.p_met_alt_up_SRCR.TOTAL     = new TH1F(Form("SRCR_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_MET_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_met_alt_dn_SRCR.TOTAL     = new TH1F(Form("SRCR_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_MET_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_met_alt_up_SRDISC.TOTAL     = new TH1F(Form("SRDISC_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_MET_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_met_alt_dn_SRDISC.TOTAL     = new TH1F(Form("SRDISC_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_MET_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    } else {
        p_result.p_met_alt_up_SRCR.TOTAL     = 0;
        p_result.p_met_alt_dn_SRCR.TOTAL     = 0;
        p_result.p_met_alt_up_SRDISC.TOTAL     = 0;
        p_result.p_met_alt_dn_SRDISC.TOTAL     = 0;
    }
    //For PU variations
    if (doFakes == 1 || isData==0) {
        p_result.p_lep_alt_up_SRCR.TOTAL     = new TH1F(Form("SRCR_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_LEP_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_lep_alt_dn_SRCR.TOTAL     = new TH1F(Form("SRCR_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_LEP_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_lep_alt_up_SRDISC.TOTAL     = new TH1F(Form("SRDISC_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_LEP_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_lep_alt_dn_SRDISC.TOTAL     = new TH1F(Form("SRDISC_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_LEP_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    } else {
        p_result.p_lep_alt_up_SRCR.TOTAL     = 0;
        p_result.p_lep_alt_dn_SRCR.TOTAL     = 0;
        p_result.p_lep_alt_up_SRDISC.TOTAL     = 0;
        p_result.p_lep_alt_dn_SRDISC.TOTAL     = 0;
    }
    //For bb variations
    if (doFakes == 1 || isData==0) {
        p_result.p_bb_alt_up_SRCR.TOTAL     = new TH1F(Form("SRCR_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_BB_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_bb_alt_dn_SRCR.TOTAL     = new TH1F(Form("SRCR_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_BB_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_bb_alt_up_SRDISC.TOTAL     = new TH1F(Form("SRDISC_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_BB_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_bb_alt_dn_SRDISC.TOTAL     = new TH1F(Form("SRDISC_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_BB_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    } else {
        p_result.p_bb_alt_up_SRCR.TOTAL     = 0;
        p_result.p_bb_alt_dn_SRCR.TOTAL     = 0;
        p_result.p_bb_alt_up_SRDISC.TOTAL     = 0;
        p_result.p_bb_alt_dn_SRDISC.TOTAL     = 0;
    }
    //For PU variations
    if (doFakes == 1 || isData==0) {
        p_result.p_isr_alt_up_SRCR.TOTAL     = new TH1F(Form("SRCR_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISR_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_isr_alt_dn_SRCR.TOTAL     = new TH1F(Form("SRCR_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISR_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_isr_alt_up_SRDISC.TOTAL     = new TH1F(Form("SRDISC_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISR_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_isr_alt_dn_SRDISC.TOTAL     = new TH1F(Form("SRDISC_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISR_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    } else {
        p_result.p_isr_alt_up_SRCR.TOTAL     = 0;
        p_result.p_isr_alt_dn_SRCR.TOTAL     = 0;
        p_result.p_isr_alt_up_SRDISC.TOTAL     = 0;
        p_result.p_isr_alt_dn_SRDISC.TOTAL     = 0;
    }
    //For theory variations
    if (isData==0) {
        p_result.p_pdf_alt_up_SRCR.TOTAL     = new TH1F(Form("SRCR_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PDF_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_pdf_alt_dn_SRCR.TOTAL     = new TH1F(Form("SRCR_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PDF_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_pdf_alt_up_SRDISC.TOTAL     = new TH1F(Form("SRDISC_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PDF_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_pdf_alt_dn_SRDISC.TOTAL     = new TH1F(Form("SRDISC_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PDF_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_scale_alt_up_SRCR.TOTAL     = new TH1F(Form("SRCR_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_scale_alt_dn_SRCR.TOTAL     = new TH1F(Form("SRCR_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_scale_alt_up_SRDISC.TOTAL     = new TH1F(Form("SRDISC_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_scale_alt_dn_SRDISC.TOTAL     = new TH1F(Form("SRDISC_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_alphas_alt_up_SRCR.TOTAL     = new TH1F(Form("SRCR_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_alphas_alt_dn_SRCR.TOTAL     = new TH1F(Form("SRCR_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_alphas_alt_up_SRDISC.TOTAL     = new TH1F(Form("SRDISC_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_alphas_alt_dn_SRDISC.TOTAL     = new TH1F(Form("SRDISC_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_isrvar_alt_up_SRCR.TOTAL     = new TH1F(Form("SRCR_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_isrvar_alt_dn_SRCR.TOTAL     = new TH1F(Form("SRCR_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_isrvar_alt_up_SRDISC.TOTAL     = new TH1F(Form("SRDISC_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_isrvar_alt_dn_SRDISC.TOTAL     = new TH1F(Form("SRDISC_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_fsrvar_alt_up_SRCR.TOTAL     = new TH1F(Form("SRCR_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_fsrvar_alt_dn_SRCR.TOTAL     = new TH1F(Form("SRCR_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_fsrvar_alt_up_SRDISC.TOTAL     = new TH1F(Form("SRDISC_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_fsrvar_alt_dn_SRDISC.TOTAL     = new TH1F(Form("SRDISC_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        
    } else {
        p_result.p_pdf_alt_up_SRCR.TOTAL     = 0;
        p_result.p_pdf_alt_dn_SRCR.TOTAL     = 0;
        p_result.p_pdf_alt_up_SRDISC.TOTAL     = 0;
        p_result.p_pdf_alt_dn_SRDISC.TOTAL     = 0;
        p_result.p_scale_alt_up_SRCR.TOTAL     = 0;
        p_result.p_scale_alt_dn_SRCR.TOTAL     = 0;
        p_result.p_scale_alt_up_SRDISC.TOTAL     = 0;
        p_result.p_scale_alt_dn_SRDISC.TOTAL     = 0;
        p_result.p_alphas_alt_up_SRCR.TOTAL     = 0;
        p_result.p_alphas_alt_dn_SRCR.TOTAL     = 0;
        p_result.p_alphas_alt_up_SRDISC.TOTAL     = 0;
        p_result.p_alphas_alt_dn_SRDISC.TOTAL     = 0;
        p_result.p_isrvar_alt_up_SRCR.TOTAL     = 0;
        p_result.p_isrvar_alt_dn_SRCR.TOTAL     = 0;
        p_result.p_isrvar_alt_up_SRDISC.TOTAL     = 0;
        p_result.p_isrvar_alt_dn_SRDISC.TOTAL     = 0;
        p_result.p_fsrvar_alt_up_SRCR.TOTAL     = 0;
        p_result.p_fsrvar_alt_dn_SRCR.TOTAL     = 0;
        p_result.p_fsrvar_alt_up_SRDISC.TOTAL     = 0;
        p_result.p_fsrvar_alt_dn_SRDISC.TOTAL     = 0;
    }


    bool doPoisson = isData && !doFlips && !doFakes;
    initHistError(doPoisson, p_result.h_disc.br);
    initHistError(doPoisson, p_result.SRCR.TOTAL   );
    initHistError(doPoisson, p_result.SRDISC.TOTAL   );
    initHistError(doPoisson, p_result.SR.TOTAL   );
    initHistError(doPoisson, p_result.p_jes_alt_up_SRCR.TOTAL   );
    initHistError(doPoisson, p_result.p_jes_alt_up_SRDISC.TOTAL   );
    initHistError(doPoisson, p_result.p_jes_alt_dn_SRCR.TOTAL   );
    initHistError(doPoisson, p_result.p_jes_alt_dn_SRDISC.TOTAL   );
    initHistError(doPoisson, p_result.p_jer_alt_up_SRCR.TOTAL   );
    initHistError(doPoisson, p_result.p_jer_alt_up_SRDISC.TOTAL   );
    initHistError(doPoisson, p_result.p_jer_alt_dn_SRCR.TOTAL   );
    initHistError(doPoisson, p_result.p_jer_alt_dn_SRDISC.TOTAL   );
    initHistError(doPoisson, p_result.p_btagSF_alt_up_SRCR.TOTAL);
    initHistError(doPoisson, p_result.p_btagSF_alt_up_SRDISC.TOTAL);
    initHistError(doPoisson, p_result.p_btagSF_alt_dn_SRCR.TOTAL);
    initHistError(doPoisson, p_result.p_btagSF_alt_dn_SRDISC.TOTAL);
    initHistError(doPoisson, p_result.p_pu_alt_up_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_pu_alt_up_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_lep_alt_up_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_lep_alt_up_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_met_alt_up_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_met_alt_up_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_isr_alt_up_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_isr_alt_up_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_bb_alt_up_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_bb_alt_up_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_pu_alt_dn_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_pu_alt_dn_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_fake_unw_up_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_fake_unw_up_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_fake_alt_up_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_fake_alt_up_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_pdf_alt_up_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_pdf_alt_up_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_pdf_alt_dn_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_pdf_alt_dn_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_scale_alt_up_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_scale_alt_up_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_scale_alt_dn_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_scale_alt_dn_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_alphas_alt_up_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_alphas_alt_up_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_alphas_alt_dn_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_alphas_alt_dn_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_isrvar_alt_up_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_isrvar_alt_up_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_isrvar_alt_dn_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_isrvar_alt_dn_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_fsrvar_alt_up_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_fsrvar_alt_up_SRDISC.TOTAL    );
    initHistError(doPoisson, p_result.p_fsrvar_alt_dn_SRCR.TOTAL    );
    initHistError(doPoisson, p_result.p_fsrvar_alt_dn_SRDISC.TOTAL    );


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
        TString filename = currentFile->GetTitle();
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        bool isFastSim = false;

        // Loop over Events in current file
        for(unsigned int event = 0; event < tree->GetEntriesFast(); event++){

            if (STOP_REQUESTED) break;

            //Get Event Content
            samesign.GetEntry(event);
            nEventsTotal++;

            if (!quiet) bar.progress(nEventsTotal, nEventsChain);

            //Reject not triggered
            if (!isFastSim) {
                if (!ss::fired_trigger()) continue;
            }
            if (!ss::passes_met_filters()) continue;

            // skim = (njets_unc_dn>=2 or njets_JER_dn>=2 or njets>=2 or njets_unc_up>=2 or njets_JER_up>=2) and \
            //        (met_unc_dn>=50 or met_JER_dn>=50 or met>=50 or met_unc_up>=50 or met_JER_up>=50) and \
            //        (hyp_class != 4 or is_real_data);
            if (!ss::skim()) continue;

            if (isHiggsScan) {
                // make sure the higgs mass point we are considering is the same as chain title
                if (fabs(mysparms[0]-ss::higgs_mass()) > 0.1) continue;
            }

            float weight =  ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG;

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

            int nleps = (lep3good and lep3ccpt > 20) ? ((ss::lep4_passes_id() and ss::lep4_p4().pt() > 20) ? 4 : 3) : 2;

            //Require nominal baseline selections
            if (!passes_baseline(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), lep1id, lep2id, lep1ccpt, lep2ccpt)) continue;

            // // FIXME
            // if (isttW || isttZ) {
            //     weight *= 1.3;
            // }

            if (isHiggsPseudoscalar) weight *= ss::xsec_ps()/ss::xsec();
            if (ss::is_real_data()==0) {
                // weight*=eventScaleFactor(lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht());
                // missing trigger SF
                if (lep1good) weight *= leptonScaleFactor(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                if (lep2good) weight *= leptonScaleFactor(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                if (lep3good && lep3ccpt>20) weight *= leptonScaleFactor(year, lep3id, lep3ccpt, lep3eta, ss::ht());
                weight *= triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht());
                weight *= ss::weight_btagsf();
                weight *= getTruePUw(year, ss::trueNumInt()[0], 0);
                if (!isFastSim) {
                    weight *= ss::decayWSF();
                }
            }
            if (isFastSim) {
                if (!quiet) std::cout << "FIXME: no fastsim SFs being applied right now" << std::endl;
                // weight*=eventScaleFactorFastSim(lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht(), ss::trueNumInt()[0]);
            }

            float weight_isr_up_alt = weight;
            if (isttW) {
                // want to take 50% of the effect as unc, and since we scale
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
                if (nleps == 2) {
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

            //Only keep good events
            int ssclass = 3;
            bool isClass6 = ss::hyp_class() == 6;
            if (!doFlips && !doFakes && exclude == 0 && !truthfake) {
                if (ss::hyp_class() != ssclass && !isClass6 && ss::hyp_class() != 8) continue;
                if (!isData && !isGamma && ss::lep1_motherID()==2) continue;
                if (!isData && !isGamma && ss::lep2_motherID()==2) continue;
                if (!isData && !( (ss::lep1_motherID()==1 && ss::lep2_motherID()==1) || (ss::lep1_motherID()==-3 || ss::lep2_motherID()==-3)) ) continue;
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
                    if (nleps > 2) nbadlegs += (ss::lep3_motherID() <= 0);
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
                    if (ss::hyp_class() == 3) {
                        if (lep1good && lep2good && !lep3good && (lep3ccpt>20) && lep3pt>min_pt_fake) {  // lep3 fake
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
                        if (!lep1good && lep1pt>min_pt_fake) {
                            float fr = fakeRate(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                            float fra = alternativeFakeRate(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                            if (!ignoreFakeFactor) weight *= fr/(1.-fr);
                            if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                            foundGoodLoose = true;
                        }
                        if (!lep2good && lep2pt>min_pt_fake) {
                            float fr = fakeRate(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                            float fra = alternativeFakeRate(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                            if (!ignoreFakeFactor) weight *= fr/(1.-fr);
                            if (!ignoreFakeFactor) weight_alt_FR *= fra/(1.-fra);
                            foundGoodLoose = true;
                        }
                        if (!foundGoodLoose) continue;
                        if (ss::hyp_class() == 1 && lep1pt>min_pt_fake && lep2pt>min_pt_fake) weight *= -1.;
                        if (!ss::is_real_data() && isData) {
                            weight *= -1.;
                        }
                    }
                } else {

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
            }

            if (isClass6) {
                float zmass23 = lep2id == -lep3id ? (ss::lep2_p4()+ss::lep3_p4()).mass() : -999.0;
                float zmass31 = lep3id == -lep1id ? (ss::lep3_p4()+ss::lep1_p4()).mass() : -999.0;
                float zmass = ( fabs(zmass31 - 91.2) < fabs(zmass23 - 91.2) ) ? zmass31 : zmass23;
                if (fabs(zmass-91.2) > 15) continue;
                if (doFakes && isData) {
                    if (!ss::is_real_data()) continue;
                    if (!isClass6Fake) continue;
                }
                if( !( isClass6Fake || (lep1good && lep2good && lep3good) ) ) continue;
            }

            if (!doFlips && !doFakes && exclude == 0) {
                if (nleps > 2) {
                    if (!isData && !isGamma && ss::lep3_motherID()==2) continue;
                }
            }

            // if all 3 charges are the same, throw the event away
            if (nleps > 2 and ((lep1id>0 and lep2id>0 and lep3id>0) or
                               (lep1id<0 and lep2id<0 and lep3id<0))) continue;

            //Reject duplicates (after selection otherwise flips are ignored...)
            if (isData && ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)){ continue; }
            }

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
            float mtvis = visible.Mt();

            float pt1 = lep1ccpt;
            float pt2 = lep2ccpt;
            float pt3 = lep3ccpt;
            float pto1 = max(max(pt1,pt2),pt3);
            float pto3 = min(min(pt1,pt2),pt3);
            float pto2 = pt1+pt2+pt3-pto1-pto3;
            int mytype = ss::hyp_type();
            if (mytype==2 && abs(lep1id)==13) mytype = 1;

            int SR = signalRegionTest(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::mtmin(), lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt, nleps, isClass6);
            int SRgenmet = signalRegionTest(ss::njets(), ss::nbtags(), ss::gen_met(), ss::ht(), ss::mtmin(), lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt, nleps, isClass6);

            // JEC
            float mll = (ss::lep1_p4()*lep1ccpt/lep1pt+ss::lep2_p4()*lep2ccpt/lep2pt).M();
            float mtl1_unc_up = calcMT(lep1ccpt, ss::lep1_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
            float mtl2_unc_up = calcMT(lep2ccpt, ss::lep2_p4().phi(), ss::met_unc_up(), ss::metPhi_unc_up());
            float mtmin_unc_up = mtl1_unc_up > mtl2_unc_up ? mtl2_unc_up : mtl1_unc_up;
            float mtl1_unc_dn = calcMT(lep1ccpt, ss::lep1_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
            float mtl2_unc_dn = calcMT(lep2ccpt, ss::lep2_p4().phi(), ss::met_unc_dn(), ss::metPhi_unc_dn());
            float mtmin_unc_dn = mtl1_unc_dn > mtl2_unc_dn ? mtl2_unc_dn : mtl1_unc_dn;
            int SR_unc_up = signalRegionTest(ss::njets_unc_up(), ss::nbtags_unc_up(), ss::met_unc_up(), ss::ht_unc_up(), mtmin_unc_up, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6);
            int SR_unc_dn = signalRegionTest(ss::njets_unc_dn(), ss::nbtags_unc_dn(), ss::met_unc_dn(), ss::ht_unc_dn(), mtmin_unc_dn, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6);

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
                SR_JER_up = signalRegionTest(ss::njets_JER_up(), ss::nbtags_JER_up(), ss::met_JER_up(), ss::ht_JER_up(), mtmin_JER_up, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6);
                SR_JER_dn = signalRegionTest(ss::njets_JER_dn(), ss::nbtags_JER_dn(), ss::met_JER_dn(), ss::ht_JER_dn(), mtmin_JER_dn, lep1id, lep2id, lep1ccpt, lep2ccpt, lep3ccpt,  nleps, isClass6);
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

            int SRdisc =        getBDTBin(nbdtbins, mvavalue, SR==1);
            int SRdisc_unc_up = getBDTBin(nbdtbins, mvavalueup, SR_unc_up==1);
            int SRdisc_unc_dn = getBDTBin(nbdtbins, mvavaluedn, SR_unc_dn==1);
            int SRdisc_JER_up = getBDTBin(nbdtbins, mvavalueJERup, SR_JER_up==1);
            int SRdisc_JER_dn = getBDTBin(nbdtbins, mvavalueJERdn, SR_JER_dn==1);

            // FIXME -- put CRW into 2nd bin
            if (SR == 2) {
                SRdisc = 2;
                SRdisc_unc_up = 2;
                SRdisc_unc_dn = 2;
                SRdisc_JER_up = 2;
                SRdisc_JER_dn = 2;
            }

            // if (SR >= 0) {
            //     std::cout <<  " mvavalueup: " << mvavalueup <<  " mvavaluedn: " << mvavaluedn <<  " mvavalueJERup: " << mvavalueJERup <<  " mvavalueJERdn: " << mvavalueJERdn <<  std::endl;
            // }

            if (isData  == 0 && SR >= 0 && SR_unc_up > 0)            p_result.p_jes_alt_up_SRCR.TOTAL->Fill(SR_unc_up, weight);
            if (isData  == 0 && SR >= 0 && SR_unc_dn > 0)            p_result.p_jes_alt_dn_SRCR.TOTAL->Fill(SR_unc_dn, weight);

            if (isData  == 0 && SR >= 0 && SR_JER_up > 0)            p_result.p_jer_alt_up_SRCR.TOTAL->Fill(SR_JER_up, weight);
            if (isData  == 0 && SR >= 0 && SR_JER_dn > 0)            p_result.p_jer_alt_dn_SRCR.TOTAL->Fill(SR_JER_dn, weight);

            if (isData  == 0 && SRdisc >= 0 && SRdisc_unc_up > 0)            p_result.p_jes_alt_up_SRDISC.TOTAL->Fill(SRdisc_unc_up, weight);
            if (isData  == 0 && SRdisc >= 0 && SRdisc_unc_dn > 0)            p_result.p_jes_alt_dn_SRDISC.TOTAL->Fill(SRdisc_unc_dn, weight);

            if (isData  == 0 && SRdisc >= 0 && SRdisc_JER_up > 0)            p_result.p_jer_alt_up_SRDISC.TOTAL->Fill(SRdisc_JER_up, weight);
            if (isData  == 0 && SRdisc >= 0 && SRdisc_JER_dn > 0)            p_result.p_jer_alt_dn_SRDISC.TOTAL->Fill(SRdisc_JER_dn, weight);


            // truth match the third lepton
            if (!isData) {
                p_result.h_mid1.br->Fill(ss::lep1_motherID(), weight);
                p_result.h_mid2.br->Fill(ss::lep2_motherID(), weight);
                if (ss::lep3_passes_id()) p_result.h_mid3.br->Fill(ss::lep3_motherID(), weight);
            }


            if (SR >= 0) {
                p_result.SRCR.TOTAL->Fill(SR, weight);
                if (doFakes == 1 )            p_result.p_fake_alt_up_SRCR.TOTAL->Fill(SR, weight_alt_FR);
                if (doFakes == 1 )            p_result.p_fake_unw_up_SRCR.TOTAL->Fill(SR, weight > 0 ? 1 : -1);
                if (isData  == 0 )            p_result.p_btagSF_alt_up_SRCR.TOTAL->Fill(SR, weight_btag_up_alt);
                if (isData  == 0 )            p_result.p_btagSF_alt_dn_SRCR.TOTAL->Fill(SR, weight_btag_dn_alt);
                if (isData  == 0 )            p_result.p_pu_alt_up_SRCR.TOTAL->Fill(SR, weight_pu_up_alt);
                if (isData  == 0 )            p_result.p_pu_alt_dn_SRCR.TOTAL->Fill(SR, weight_pu_dn_alt);
                if (isData  == 0 )            p_result.p_isr_alt_up_SRCR.TOTAL->Fill(SR, weight_isr_up_alt);
                if (isData  == 0 )            p_result.p_bb_alt_up_SRCR.TOTAL->Fill(SR, weight_bb_up_alt);
                if (isData  == 0 )            p_result.p_lep_alt_up_SRCR.TOTAL->Fill(SR, weight_lep_up_alt);
                if (isData  == 0 && SRgenmet > 0)            p_result.p_met_alt_up_SRCR.TOTAL->Fill(SRgenmet, weight);
                if (istttt || isttW || isttZ || isttH) {
                    p_result.p_scale_alt_up_SRCR.TOTAL->Fill(SR,    (ss::weight_scale_UP() > -9000 ? ss::weight_scale_UP() : 1.0)*weight);
                    p_result.p_scale_alt_dn_SRCR.TOTAL->Fill(SR,    (ss::weight_scale_DN() > -9000 ? ss::weight_scale_DN() : 1.0)*weight);
                    p_result.p_alphas_alt_up_SRCR.TOTAL->Fill(SR,    (ss::weight_alphas_UP() > -9000 ? ss::weight_alphas_UP() : 1.0)*weight);
                    p_result.p_alphas_alt_dn_SRCR.TOTAL->Fill(SR,    (ss::weight_alphas_DN() > -9000 ? ss::weight_alphas_DN() : 1.0)*weight);
                    p_result.p_pdf_alt_up_SRCR.TOTAL->Fill(SR,      (ss::weight_pdf_UP() > -9000 ? ss::weight_pdf_UP() : 1.0)*weight);
                    p_result.p_pdf_alt_dn_SRCR.TOTAL->Fill(SR,      (ss::weight_pdf_DN() > -9000 ? ss::weight_pdf_DN() : 1.0)*weight);
                    p_result.p_isrvar_alt_up_SRCR.TOTAL->Fill(SR      , ss::weight_isrvar_UP()*weight);
                    p_result.p_isrvar_alt_dn_SRCR.TOTAL->Fill(SR      , ss::weight_isrvar_DN()*weight);
                    p_result.p_fsrvar_alt_up_SRCR.TOTAL->Fill(SR      , ss::weight_fsrvar_UP()*weight);
                    p_result.p_fsrvar_alt_dn_SRCR.TOTAL->Fill(SR      , ss::weight_fsrvar_DN()*weight);
                }
            }
            if (SRdisc >= 0) {
                p_result.SRDISC.TOTAL->Fill(SRdisc, weight);
                if (doFakes == 1 )            p_result.p_fake_alt_up_SRDISC.TOTAL->Fill(SRdisc, weight_alt_FR);
                if (doFakes == 1 )            p_result.p_fake_unw_up_SRDISC.TOTAL->Fill(SRdisc, weight > 0 ? 1 : -1);
                if (isData  == 0 )            p_result.p_btagSF_alt_up_SRDISC.TOTAL->Fill(SRdisc, weight_btag_up_alt);
                if (isData  == 0 )            p_result.p_btagSF_alt_dn_SRDISC.TOTAL->Fill(SRdisc, weight_btag_dn_alt);
                if (isData  == 0 )            p_result.p_pu_alt_up_SRDISC.TOTAL->Fill(SRdisc, weight_pu_up_alt);
                if (isData  == 0 )            p_result.p_pu_alt_dn_SRDISC.TOTAL->Fill(SRdisc, weight_pu_dn_alt);
                if (isData  == 0 )            p_result.p_isr_alt_up_SRDISC.TOTAL->Fill(SRdisc, weight_isr_up_alt);
                if (isData  == 0 )            p_result.p_bb_alt_up_SRDISC.TOTAL->Fill(SRdisc, weight_bb_up_alt);
                if (isData  == 0 )            p_result.p_lep_alt_up_SRDISC.TOTAL->Fill(SRdisc, weight_lep_up_alt);
                if (istttt || isttW || isttZ || isttH) {
                    p_result.p_scale_alt_up_SRDISC.TOTAL->Fill(SRdisc,    (ss::weight_scale_UP() > -9000 ? ss::weight_scale_UP() : 1.0)*weight);
                    p_result.p_scale_alt_dn_SRDISC.TOTAL->Fill(SRdisc,    (ss::weight_scale_DN() > -9000 ? ss::weight_scale_DN() : 1.0)*weight);
                    p_result.p_alphas_alt_up_SRDISC.TOTAL->Fill(SRdisc,    (ss::weight_alphas_UP() > -9000 ? ss::weight_alphas_UP() : 1.0)*weight);
                    p_result.p_alphas_alt_dn_SRDISC.TOTAL->Fill(SRdisc,    (ss::weight_alphas_DN() > -9000 ? ss::weight_alphas_DN() : 1.0)*weight);
                    p_result.p_pdf_alt_up_SRDISC.TOTAL->Fill(SRdisc,      (ss::weight_pdf_UP() > -9000 ? ss::weight_pdf_UP() : 1.0)*weight);
                    p_result.p_pdf_alt_dn_SRDISC.TOTAL->Fill(SRdisc,      (ss::weight_pdf_DN() > -9000 ? ss::weight_pdf_DN() : 1.0)*weight);
                    p_result.p_isrvar_alt_up_SRDISC.TOTAL->Fill(SRdisc, ss::weight_isrvar_UP()*weight);
                    p_result.p_isrvar_alt_dn_SRDISC.TOTAL->Fill(SRdisc, ss::weight_isrvar_DN()*weight);
                    p_result.p_fsrvar_alt_up_SRDISC.TOTAL->Fill(SRdisc, ss::weight_fsrvar_UP()*weight);
                    p_result.p_fsrvar_alt_dn_SRDISC.TOTAL->Fill(SRdisc, ss::weight_fsrvar_DN()*weight);
                }
            }

            bool plotlep3 = ss::lep3_passes_id() && lep3ccpt > 20.;


            {
                float lepsf = 1.;
                float btagsf = 1.;
                float wsf = 1.;
                float puw = 1.;
                float trigsf = 1.;
                float isrw = 1.;
                float w = ss::is_real_data() ? 0.0 : ss::scale1fb()*lumiAG; // ignore data
                if (ss::is_real_data()==0) {
                    if (lep1good) lepsf *= leptonScaleFactor(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                    if (lep2good) lepsf *= leptonScaleFactor(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                    if (lep3good && lep3ccpt>20) lepsf *= leptonScaleFactor(year, lep3id, lep3ccpt, lep3eta, ss::ht());
                    trigsf *= triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht());
                    btagsf *= ss::weight_btagsf();
                    puw *= getTruePUw(year, ss::trueNumInt()[0], 0);
                    wsf *= ss::decayWSF();
                    if (isttW) isrw *= isrWeight(year, ss::nisrMatch(), 1);
                    if (isttZ) isrw *= isrWeight(year, ss::nisrMatch(), 2);
                }
                p_result.h_lepsf.br->Fill(lepsf, w);
                p_result.h_btagsf.br->Fill(btagsf, w);
                p_result.h_wsf.br->Fill(wsf, w);
                p_result.h_puw.br->Fill(puw, w);
                p_result.h_trigsf.br->Fill(trigsf, w);
                p_result.h_isrw.br->Fill(isrw, w);
            }



            if (SR != 1) { // non ttZ CR
                p_result.h_njets.br->Fill(ss::njets() , weight);
                p_result.h_nbtags.br->Fill(ss::nbtags() , weight);
                p_result.h_type.br->Fill(mytype , weight);
                p_result.h_nvtx.br->Fill(ss::nGoodVertices() , weight);
                p_result.h_charge.br->Fill(lep1id > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.br->Fill(nleps , weight);
                p_result.h_ht.br->Fill(ss::ht() , weight);
                p_result.h_met.br->Fill(ss::met() , weight);
                p_result.h_mvis.br->Fill(mvis , weight);
                p_result.h_mtvis.br->Fill(mtvis , weight);
                p_result.h_mll.br->Fill(mll , weight);
                p_result.h_mtmin.br->Fill(mtmin , weight);
                p_result.h_l1pt.br->Fill(pto1, weight);
                p_result.h_l2pt.br->Fill(pto2, weight);
                (abs(lep1id) == 11) ? p_result.h_el_l1pt.br->Fill(pto1, weight) : p_result.h_mu_l1pt.br->Fill(pto1, weight);
                (abs(lep2id) == 11) ? p_result.h_el_l2pt.br->Fill(pto2, weight) : p_result.h_mu_l2pt.br->Fill(pto2, weight);
                if (abs(lep1id) == 11) p_result.h_el_nmiss.br->Fill(ss::lep1_el_exp_innerlayers(), weight);
                if (abs(lep2id) == 11) p_result.h_el_nmiss.br->Fill(ss::lep2_el_exp_innerlayers(), weight);
                if (plotlep3) {
                    p_result.h_l3pt.br->Fill(pto3, weight);
                    (abs(lep3id) == 11) ? p_result.h_el_l3pt.br->Fill(pto3, weight) : p_result.h_mu_l3pt.br->Fill(pto3, weight);
                }
                for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
                    p_result.h_bjetpt.br->Fill(ss::btags()[ijet].pt(),weight);
                }
                for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
                    p_result.h_jetpt.br->Fill(ss::jets()[ijet].pt(),weight);
                }
                if (abs(lep1id) == 11) p_result.h_mva.br->Fill(ss::lep1_MVA()                                                     , weight);
                if(abs(lep1id) == 13) {
                    p_result.h_mu_sip3d_lep1.br  ->Fill(ss::lep1_sip()                                                                          , weight);
                    p_result.h_mu_l1eta.br       ->Fill(lep1eta                                                                     , weight);
                    p_result.h_mu_lep1_miniIso.br->Fill(ss::lep1_miniIso()                                                                      , weight);
                    p_result.h_mu_lep1_ptRel.br  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    p_result.h_mu_lep1_ptRatio.br->Fill(ss::lep1_ptratio()                                                                               , weight);
                } else {
                    p_result.h_el_sip3d_lep1.br  ->Fill(ss::lep1_sip()                                                                          , weight);
                    p_result.h_el_l1pt.br        ->Fill(lep1ccpt                                                                                 , weight);
                    p_result.h_el_l1eta.br       ->Fill(lep1eta                                                                     , weight);
                    p_result.h_el_lep1_miniIso.br->Fill(ss::lep1_miniIso()                                                                      , weight);
                    p_result.h_el_lep1_ptRel.br  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    p_result.h_el_lep1_ptRatio.br->Fill(ss::lep1_ptratio()                                                                               , weight);
                }
                if (abs(lep2id) == 11) p_result.h_mva.br->Fill(ss::lep2_MVA()                                                     , weight);
                if(abs(lep2id) == 13) {
                    p_result.h_mu_sip3d_lep2.br  ->Fill(ss::lep2_sip()                                                                          , weight);
                    p_result.h_mu_l2eta.br       ->Fill(lep2eta                                                                     , weight);
                    p_result.h_mu_lep2_miniIso.br->Fill(ss::lep2_miniIso()                                                                      , weight);
                    p_result.h_mu_lep2_ptRel.br  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    p_result.h_mu_lep2_ptRatio.br->Fill(ss::lep2_ptratio()                                                                               , weight);
                } else {
                    p_result.h_el_sip3d_lep2.br  ->Fill(ss::lep2_sip()                                                                          , weight);
                    p_result.h_el_l2eta.br       ->Fill(lep2eta                                                                     , weight);
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
                p_result.h_njets.ttzcr->Fill(ss::njets() , weight);
                p_result.h_nbtags.ttzcr->Fill(ss::nbtags() , weight);
                p_result.h_type.ttzcr->Fill(mytype , weight);
                p_result.h_charge.ttzcr->Fill(lep1id > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.ttzcr->Fill(nleps , weight);
                p_result.h_ht.ttzcr->Fill(ss::ht() , weight);
                p_result.h_met.ttzcr->Fill(ss::met() , weight);
                p_result.h_mvis.ttzcr->Fill(mvis , weight);
                p_result.h_mtvis.ttzcr->Fill(mtvis , weight);
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
                p_result.h_njets.ttwcr->Fill(ss::njets() , weight);
                p_result.h_nbtags.ttwcr->Fill(ss::nbtags() , weight);
                p_result.h_type.ttwcr->Fill(mytype , weight);
                p_result.h_charge.ttwcr->Fill(lep1id > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.ttwcr->Fill(nleps , weight);
                p_result.h_ht.ttwcr->Fill(ss::ht() , weight);
                p_result.h_met.ttwcr->Fill(ss::met() , weight);
                p_result.h_mvis.ttwcr->Fill(mvis , weight);
                p_result.h_mtvis.ttwcr->Fill(mtvis , weight);
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
                p_result.SR.TOTAL->Fill(SR-nCR, weight);
                p_result.h_njets.sr->Fill(ss::njets() , weight);
                p_result.h_nbtags.sr->Fill(ss::nbtags() , weight);
                p_result.h_type.sr->Fill(mytype , weight);
                p_result.h_charge.sr->Fill(lep1id > 0 ? -0.5: 0.5 , weight);
                p_result.h_nleps.sr->Fill(nleps , weight);
                p_result.h_ht.sr->Fill(ss::ht() , weight);
                // if (SR == 1+2)  p_result.h_ht_sr1.sr->Fill(ss::ht() , weight);
                // if (SR == 2+2)  p_result.h_ht_sr2.sr->Fill(ss::ht() , weight);
                // if (SR == 4+2)  p_result.h_ht_sr4.sr->Fill(ss::ht() , weight);
                // if (SR == 5+2)  p_result.h_ht_sr5.sr->Fill(ss::ht() , weight);
                // if (SR == 11+2) p_result.h_ht_sr11.sr->Fill(ss::ht() , weight);
                p_result.h_met.sr->Fill(ss::met() , weight);
                p_result.h_mvis.sr->Fill(mvis , weight);
                p_result.h_mtvis.sr->Fill(mtvis , weight);
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
                for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
                    p_result.h_bjetpt.sr->Fill(ss::btags()[ijet].pt(),weight);
                }
                for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
                    p_result.h_jetpt.sr->Fill(ss::jets()[ijet].pt(),weight);
                }
                if (abs(lep1id) == 11) p_result.h_mva.sr->Fill(ss::lep1_MVA()                                                     , weight);
                if(abs(lep1id) == 13) {
                    p_result.h_mu_sip3d_lep1.sr  ->Fill(ss::lep1_sip()                                                                          , weight);
                    p_result.h_mu_l1eta.sr       ->Fill(lep1eta                                                                     , weight);
                    p_result.h_mu_lep1_miniIso.sr->Fill(ss::lep1_miniIso()                                                                      , weight);
                    p_result.h_mu_lep1_ptRel.sr  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    p_result.h_mu_lep1_ptRatio.sr->Fill(ss::lep1_ptratio()                                                                               , weight);
                } else {
                    p_result.h_el_sip3d_lep1.sr  ->Fill(ss::lep1_sip()                                                                          , weight);
                    p_result.h_el_l1eta.sr       ->Fill(lep1eta                                                                     , weight);
                    p_result.h_el_lep1_miniIso.sr->Fill(ss::lep1_miniIso()                                                                      , weight);
                    p_result.h_el_lep1_ptRel.sr  ->Fill(ss::lep1_ptrel_v1()                                                                     , weight);
                    p_result.h_el_lep1_ptRatio.sr->Fill(ss::lep1_ptratio()                                                                               , weight);
                }
                if (abs(lep2id) == 11) p_result.h_mva.sr->Fill(ss::lep2_MVA()                                                     , weight);
                if(abs(lep2id) == 13) {
                    p_result.h_mu_sip3d_lep2.sr  ->Fill(ss::lep2_sip()                                                                          , weight);
                    p_result.h_mu_l2eta.sr       ->Fill(lep2eta                                                                     , weight);
                    p_result.h_mu_lep2_miniIso.sr->Fill(ss::lep2_miniIso()                                                                      , weight);
                    p_result.h_mu_lep2_ptRel.sr  ->Fill(ss::lep2_ptrel_v1()                                                                     , weight);
                    p_result.h_mu_lep2_ptRatio.sr->Fill(ss::lep2_ptratio()                                                                               , weight);
                } else {
                    p_result.h_el_sip3d_lep2.sr  ->Fill(ss::lep2_sip()                                                                          , weight);
                    p_result.h_el_l2eta.sr       ->Fill(lep2eta                                                                     , weight);
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


        }//event loop
        file->Close();
    }//file loop
    if (!quiet) bar.finish();

    if (!isData) {
        avoidNegativeYields(p_result.p_btagSF_alt_up_SRCR.TOTAL);
        avoidNegativeYields(p_result.p_btagSF_alt_dn_SRCR.TOTAL);
        avoidNegativeYields(p_result.p_btagSF_alt_up_SRDISC.TOTAL);
        avoidNegativeYields(p_result.p_btagSF_alt_dn_SRDISC.TOTAL);
        avoidNegativeYields(p_result.p_pu_alt_up_SRCR.TOTAL);
        avoidNegativeYields(p_result.p_pu_alt_dn_SRCR.TOTAL);
        avoidNegativeYields(p_result.p_pu_alt_up_SRDISC.TOTAL);
        avoidNegativeYields(p_result.p_pu_alt_dn_SRDISC.TOTAL);
        avoidNegativeYields(p_result.p_isr_alt_up_SRCR.TOTAL);
        avoidNegativeYields(p_result.p_isr_alt_dn_SRCR.TOTAL);
        avoidNegativeYields(p_result.p_isr_alt_up_SRDISC.TOTAL);
        avoidNegativeYields(p_result.p_isr_alt_dn_SRDISC.TOTAL);
        avoidNegativeYields(p_result.p_lep_alt_up_SRCR.TOTAL);
        avoidNegativeYields(p_result.p_lep_alt_dn_SRCR.TOTAL);
        avoidNegativeYields(p_result.p_lep_alt_up_SRDISC.TOTAL);
        avoidNegativeYields(p_result.p_lep_alt_dn_SRDISC.TOTAL);
        avoidNegativeYields(p_result.SRCR.TOTAL);
        avoidNegativeYields(p_result.SRDISC.TOTAL);
        avoidNegativeYields(p_result.SR.TOTAL);
        avoidNegativeYields(p_result.p_jes_alt_up_SRCR.TOTAL);
        avoidNegativeYields(p_result.p_jes_alt_dn_SRCR.TOTAL);
        avoidNegativeYields(p_result.p_jes_alt_up_SRDISC.TOTAL);
        avoidNegativeYields(p_result.p_jes_alt_dn_SRDISC.TOTAL);
        avoidNegativeYields(p_result.p_jer_alt_up_SRCR.TOTAL);
        avoidNegativeYields(p_result.p_jer_alt_dn_SRCR.TOTAL);
        avoidNegativeYields(p_result.p_jer_alt_up_SRDISC.TOTAL);
        avoidNegativeYields(p_result.p_jer_alt_dn_SRDISC.TOTAL);
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


void initHistError(bool usePoisson, TH1F* plot) {
    if (plot==0) return;
    if (usePoisson) plot->SetBinErrorOption(TH1::kPoisson);
    else  plot->Sumw2();
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

