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
#include "../misc/common_utils.h"
#include "../misc/signal_regions.h"
#include "../misc/bdt.h"
#include "../../babymaking/batch/nPoints.cc"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "../misc/tqdm.h"

#define WRITE(var) { if (plots.var) plots.var->Write(); }

bool STOP_REQUESTED = false;

float XSEC_TTTT = 11.97;
float scaleLumi = 1; // 75/35.87
TH2D *h_counts = 0;
TH1D *h_weights = 0;

void avoidNegativeYields(TH1F* plot);

struct SR_t     {
    TH1F* SR = 0;
    TH1F* SRCR = 0;
    TH1F* SRDISC = 0;
    void Write() {
        if (SR) SR->Write();
        if (SRCR) SRCR->Write();
        if (SRDISC) SRDISC->Write();
    }
    void CatFill(int which, float value, float weight) {
        if ((which == 0) && SR    ) SR->Fill(value, weight    );
        if ((which == 1) && SRCR  ) SRCR->Fill(value, weight  );
        if ((which == 2) && SRDISC) SRDISC->Fill(value, weight);
    }
    void AvoidNegativeYields() {
        if (SR) avoidNegativeYields(SR);
        if (SRCR) avoidNegativeYields(SRCR);
        if (SRDISC) avoidNegativeYields(SRDISC);
    }
    void InitHistError(bool poisson) {
        if (poisson) {
            if (SR) SR->SetBinErrorOption(TH1::kPoisson);
            if (SRCR) SRCR->SetBinErrorOption(TH1::kPoisson);
            if (SRDISC) SRDISC->SetBinErrorOption(TH1::kPoisson);
        } else {
            if (SR) SR->Sumw2();
            if (SRCR) SRCR->Sumw2();
            if (SRDISC) SRDISC->Sumw2();
        }
    }
};
struct triple_t {
    TH1F* ttzcr = 0;
    TH1F* ttwcr = 0;
    TH1F* sr = 0;
    TH1F* br = 0;
    void Init(TString label, TString chainTitle, int nbins, float xlow, float xhigh) {
        ttwcr = new TH1F(Form("ttwcr_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); ttwcr->SetDirectory(0);
        ttzcr = new TH1F(Form("ttzcr_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); ttzcr->SetDirectory(0);
        sr = new TH1F(Form("sr_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); sr->SetDirectory(0);
        br = new TH1F(Form("br_%s_%s"        ,label.Data(),chainTitle.Data()), Form("%s_%s",label.Data(),chainTitle.Data()), nbins, xlow, xhigh); br->SetDirectory(0);
    }
    void Write() {
        if (ttzcr) ttzcr->Write();
        if (ttwcr) ttwcr->Write();
        if (sr) sr->Write();
        if (br) br->Write();
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
    SR_t p_unw_raw_SR   ;
    SR_t p_unw_sgn_SR   ;

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
    triple_t h_type3;
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


};


//function declaration
plots_t run(TChain *chain, int year, TString options);
float calcDeltaPhi(float phi1, float phi2);
float calcMT(float pt1, float phi1, float pt2, float phi2);
float getSMSscale1fb();
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

    plots_t plots = run(ch, year, options);

    TString chainTitle = ch->GetTitle();
    TString outname = Form("%s/output_%d%s_%s.root", outputdir.Data(), year, extra.Data(), chainTitle.Data());

    if (!quiet) std::cout << "Writing out " << outname << std::endl;
    TFile *fout = new TFile(outname, "RECREATE");

    plots.SR.Write();
    plots.h_bjetpt.Write();
    plots.h_btagsf.Write();
    plots.h_charge.Write();
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
    plots.h_wsf.Write();
    plots.p_alphas_alt_dn_SR.Write();
    plots.p_alphas_alt_up_SR.Write();
    plots.p_bb_alt_dn_SR.Write();
    plots.p_bb_alt_up_SR.Write();
    plots.p_prefire_alt_dn_SR.Write();
    plots.p_prefire_alt_up_SR.Write();
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

// #include "test.h"

//doFakes = 1 for QCD, 2 for inSitu
plots_t run(TChain *chain, int year, TString options){
    
    // float f_nbtags = -1;
    // float f_njets = -1;
    // float f_met = -1;
    // float f_ptl2 = -1;
    // float f_nlb40 = -1;
    // float f_ntb40 = -1;
    // float f_nleps = -1;
    // float f_htb = -1;
    // float f_q1 = -1;
    // float f_ptj1 = -1;
    // float f_ptj6 = -1;
    // float f_ptj7 = -1;
    // float f_ml1j1 = -1;
    // float f_dphil1l2 = -1;
    // float f_maxmjoverpt = -1;
    // float f_ptl1 = -1;
    // float f_detal1l2 = -1;
    // float f_ptj8 = -1;
    // float f_ptl3 = -1;
    // float f_weight = 1.;
    // int f_SR = 5;
    // int f_br = 5;
    // TMVA::Reader* reader = new TMVA::Reader("Silent");
    // reader->AddVariable("nbtags",&f_nbtags);
    // reader->AddVariable("njets",&f_njets);
    // reader->AddVariable("met",&f_met);
    // reader->AddVariable("ptl2",&f_ptl2);
    // reader->AddVariable("nlb40",&f_nlb40);
    // reader->AddVariable("ntb40",&f_ntb40);
    // reader->AddVariable("nleps",&f_nleps);
    // reader->AddVariable("htb",&f_htb);
    // reader->AddVariable("q1",&f_q1);
    // reader->AddVariable("ptj1",&f_ptj1);
    // reader->AddVariable("ptj6",&f_ptj6);
    // reader->AddVariable("ptj7",&f_ptj7);
    // reader->AddVariable("ml1j1",&f_ml1j1);
    // reader->AddVariable("dphil1l2",&f_dphil1l2);
    // reader->AddVariable("maxmjoverpt",&f_maxmjoverpt);
    // reader->AddVariable("ptl1",&f_ptl1);
    // reader->AddVariable("detal1l2",&f_detal1l2);
    // reader->AddVariable("ptj8",&f_ptj8);
    // reader->AddVariable("ptl3",&f_ptl3);
    // reader->AddSpectator("weight",&f_weight);
    // reader->AddSpectator("ptl1",&f_ptl1);
    // reader->AddSpectator("ptl2",&f_ptl2);
    // reader->AddSpectator("SR",&f_SR);
    // reader->AddSpectator("br",&f_br);
    // reader->BookMVA("BDT","tmva.xml");


    float lumiAG = getLumi(year);
    int nsr = getNsrs();
    int nCR = getNCRs();
    int nbdtbins = 17;


    bool isData = 0;
    bool doFlips = 0;
    int doFakes = 0;
    int exclude = 0;
    bool isFastsim = 0;
    bool isGamma = 0;
    bool truthfake = 0;
    bool ignoreFakeFactor = 0;

    bool evaluateBDT = options.Contains("evaluateBDT") && not options.Contains("noBDT");
    bool quiet = options.Contains("quiet");
    bool minPtFake18 = options.Contains("minPtFake18");
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
    int higgs_mass = -1;
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
    }

    int bootstrap_index = -1; // Note, user starts indexing from 1 (e.g., 1 of 10), but parsing will subtract 1
    int bootstrap_number = -1;
    // if bootstrap_number is 10, we allow 10 possible bootstrap indices where we
    // consider 90% of the data only
    // event() % bootstrap_number is 0 to 9
    // and we want bootstrap_index to NOT match, so we require
    // if ((bootstrap_number > 0) and ((ss::event() % bootstrap_number) == bootstrap_index)) continue;
    // then we need to do
    // scaleLumi *= 1.0 * bootstrap_number / (bootstrap_number - 1)
    auto tokens1 = options.Tokenize(" ");
    for (int i = 0; i < tokens1->GetEntries(); i++) {
        auto token1 = ((TObjString *)(tokens1->At(i)))->String();
        if (!token1.Contains("bootstrap")) continue;
        auto tokens2 = token1.Tokenize("_");
        for (int j = 0; j < tokens2->GetEntries(); j++) {
            auto token2 = ((TObjString *)(tokens2->At(j)))->String();
            if (j == 1) bootstrap_index = token2.Atoi()-1;
            if (j == 2) bootstrap_number = token2.Atoi();
        }
    }
    if (bootstrap_number > 0) {
        float bootstrap_scale = 1.0 * bootstrap_number / (bootstrap_number - 1);
        scaleLumi *= bootstrap_scale;
        if (!quiet) {
            std::cout << "Bootstrap " << bootstrap_index+1 << " of " << bootstrap_number << " (scaling scaleLumi up by "
                << bootstrap_number << "/(" << bootstrap_number << "-1) = " << bootstrap_scale << std::endl;
        }
    }
    TRandom *tr1 = new TRandom(bootstrap_index);

    plots_t  p_result;

    p_result.h_bjetpt.Init("bjetpt"     , chainTitle , 16 , 0.   , 200.);
    p_result.h_charge3.Init("charge3"   , chainTitle , 3  , 0    , 3);
    p_result.h_charge.Init("charge"     , chainTitle , 3  , -1   , 2);
    p_result.h_ht.Init("ht"             , chainTitle , 15 , 100  , 1600);
    p_result.h_isrw.Init("isrw"         , chainTitle , 50 , 0.7  , 1.3);
    p_result.h_jetpt.Init("jetpt"       , chainTitle , 20 , 0.   , 200.);
    p_result.h_l1pt.Init("l1pt"         , chainTitle , 15 , 0    , 150);
    p_result.h_l2pt.Init("l2pt"         , chainTitle , 15 , 0    , 150);
    p_result.h_l3pt.Init("l3pt"         , chainTitle , 7  , 0    , 140);
    p_result.h_met.Init("met"           , chainTitle , 15 , 0    , 600);
    p_result.h_metnm1.Init("metnm1"     , chainTitle , 40 , 0    , 400);
    p_result.h_mid1.Init("mid1"         , chainTitle , 5  , -2   , 3);
    p_result.h_mid2.Init("mid2"         , chainTitle , 5  , -2   , 3);
    p_result.h_mid3.Init("mid3"         , chainTitle , 5  , -2   , 3);
    p_result.h_mll.Init("mll"           , chainTitle , 20 , 0    , 200);
    p_result.h_mllos.Init("mllos"       , chainTitle , 20 , 0    , 200);
    p_result.h_mtmin.Init("mtmin"       , chainTitle , 10 , 0    , 200);
    p_result.h_mtop1.Init("mtop1"       , chainTitle , 15 , 100  , 250);
    p_result.h_mtop2.Init("mtop2"       , chainTitle , 15 , 100  , 250);
    p_result.h_mtvis.Init("mtvis"       , chainTitle , 20 , 300  , 2500);
    p_result.h_mva.Init("mva"           , chainTitle , 10 , 0    , 1.5);
    p_result.h_mvis.Init("mvis"         , chainTitle , 15 , 250  , 2750);
    p_result.h_mvis.Init("mvis"         , chainTitle , 25 , 0    , 2500);
    p_result.h_nbtags.Init("nbtags"     , chainTitle , 7  , -0.5 , 6.5);
    p_result.h_njets.Init("njets"       , chainTitle , 10 , 1.5  , 11.5);
    p_result.h_nleps.Init("nleps"       , chainTitle , 6  , 0    , 6);
    p_result.h_ntopness.Init("ntopness" , chainTitle , 15 , 0.   , 0.75);
    p_result.h_ntops.Init("ntops"       , chainTitle , 5  , -0.5 , 4.5);
    p_result.h_nvtx.Init("nvtx"         , chainTitle , 20 , 0    , 80);
    p_result.h_puw.Init("puw"           , chainTitle , 50 , 0.7  , 1.3);
    p_result.h_type.Init("type"         , chainTitle , 4  , 0    , 4);
    p_result.h_type3.Init("type3"       , chainTitle , 4  , 0    , 4);

    p_result.h_topdisc1.Init("topdisc1" , chainTitle , 30 , -0.6 , 0.4);
    p_result.h_topdisc2.Init("topdisc2" , chainTitle , 30 , -0.6 , 0.4);

    p_result.h_el_l1eta.Init("l1eta_el"                   , chainTitle , 12 , -2.5 , 2.5);
    p_result.h_el_l1pt.Init("el_l1pt"                     , chainTitle , 15 , 0    , 150);
    p_result.h_el_l2eta.Init("l2eta_el"                   , chainTitle , 12 , -2.5 , 2.5);
    p_result.h_el_l2pt.Init("el_l2pt"                     , chainTitle , 15 , 0    , 150);
    p_result.h_el_l3eta.Init("l3eta_el"                   , chainTitle , 8  , -2.5 , 2.5);
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

    p_result.h_mu_l1eta.Init("l1eta_mu"                   , chainTitle , 12 , -2.5 , 2.5);
    p_result.h_mu_l1pt.Init("mu_l1pt"                     , chainTitle , 15 , 0    , 150);
    p_result.h_mu_l2eta.Init("l2eta_mu"                   , chainTitle , 12 , -2.5 , 2.5);
    p_result.h_mu_l2pt.Init("mu_l2pt"                     , chainTitle , 15 , 0    , 150);
    p_result.h_mu_l3eta.Init("l3eta_mu"                   , chainTitle , 8  , -2.5 , 2.5);
    p_result.h_mu_l3pt.Init("mu_l3pt"                     , chainTitle , 7  , 0    , 140);
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

    p_result.h_disc.Init("disc"         , chainTitle , 20 , 0    , 1);

    p_result.h_wsf.Init("wsf"       , chainTitle , 50 , 0.7 , 1.3);
    p_result.h_btagsf.Init("btagsf" , chainTitle , 50 , 0.7 , 1.3);
    p_result.h_lepsf.Init("lepsf"   , chainTitle , 50 , 0.7 , 1.3);
    p_result.h_trigsf.Init("trigsf" , chainTitle , 50 , 0.7 , 1.3);

    // p_result.h_disc.br = new TH1F(Form("br_disc_%s"      , chainTitleCh) , Form("disc_%s"         , chainTitleCh) , 16      , -1.0 , 1.0);
    // p_result.h_disc.br    = new TH1F(Form("br_disc_%s"      , chainTitleCh) , Form("disc_%s"         , chainTitleCh) , 20      , 0.0  , 1.0);
    p_result.SR.SRCR      = new TH1F(Form("SRCR_TOTAL_%s"   , chainTitleCh) , Form("SRCR_TOTAL_%s"   , chainTitleCh) , nsr     , 0.5  , nsr+0.5);
    p_result.SR.SRDISC    = new TH1F(Form("SRDISC_TOTAL_%s" , chainTitleCh) , Form("SRDISC_TOTAL_%s" , chainTitleCh) , nsrdisc , 0.5  , nsrdisc+0.5);
    p_result.SR.SR        = new TH1F(Form("SR_TOTAL_%s"     , chainTitleCh) , Form("SR_TOTAL_%s"     , chainTitleCh) , nsr-nCR , 0.5  , nsr-nCR+0.5);

    //For FR variations
    if (doFakes == 1) {
        p_result.p_fake_alt_up_SR.SRCR        = new TH1F(Form("SRCR_FR_TOTAL_%s"      , chainTitleCh) , Form("SRCR_FR_TOTAL_%s"      , chainTitleCh) , nsr   , 0.5, nsr+0.5 );
        p_result.p_fake_alt_up_SR.SRDISC        = new TH1F(Form("SRDISC_FR_TOTAL_%s"      , chainTitleCh) , Form("SRDISC_FR_TOTAL_%s"      , chainTitleCh) , nsrdisc   , 0.5, nsrdisc+0.5 );
    }
    //For unw FR application counts
    if (doFakes == 1) {
        p_result.p_fake_unw_up_SR.SRCR        = new TH1F(Form("SRCR_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRCR_UNWFR_TOTAL_%s"      , chainTitleCh) , nsr   , 0.5, nsr+0.5 );
        p_result.p_fake_unw_up_SR.SRDISC        = new TH1F(Form("SRDISC_UNWFR_TOTAL_%s"      , chainTitleCh) , Form("SRDISC_UNWFR_TOTAL_%s"      , chainTitleCh) , nsrdisc   , 0.5, nsrdisc+0.5 );
    }
    //For JES variations
    if (doFakes == 1 || isData==0) {
        p_result.p_jes_alt_up_SR.SRCR     = new TH1F(Form("SRCR_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JES_UP_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 );
        p_result.p_jes_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JES_DN_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 );
        p_result.p_jes_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_JES_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JES_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 );
        p_result.p_jes_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_JES_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JES_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 );
    }
    //For JER variations
    if (doFakes == 1 || isData==0) {
        p_result.p_jer_alt_up_SR.SRCR     = new TH1F(Form("SRCR_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JER_UP_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 );
        p_result.p_jer_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_JER_DN_TOTAL_%s"   , chainTitleCh) , nsr , 0.5, nsr+0.5 );
        p_result.p_jer_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_JER_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JER_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 );
        p_result.p_jer_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_JER_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_JER_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc , 0.5, nsrdisc+0.5 );
    }
    //For btag SF variations
    if (isData==0){
        p_result.p_btagSF_alt_up_SR.SRCR = new TH1F(Form("SRCR_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRCR_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nsr,  0.5, nsr+0.5 );
        p_result.p_btagSF_alt_dn_SR.SRCR = new TH1F(Form("SRCR_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRCR_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nsr,  0.5, nsr+0.5 );
        p_result.p_btagSF_alt_up_SR.SRDISC = new TH1F(Form("SRDISC_BTAGSF_UP_TOTAL_%s", chainTitleCh), Form("SRDISC_BTAGSF_UP_TOTAL_%s", chainTitleCh),  nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_btagSF_alt_dn_SR.SRDISC = new TH1F(Form("SRDISC_BTAGSF_DN_TOTAL_%s", chainTitleCh), Form("SRDISC_BTAGSF_DN_TOTAL_%s", chainTitleCh),  nsrdisc,  0.5, nsrdisc+0.5 );
    }
    //For PU variations
    if (doFakes == 1 || isData==0) {
        p_result.p_pu_alt_up_SR.SRCR     = new TH1F(Form("SRCR_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PU_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_pu_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PU_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_pu_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_PU_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PU_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_pu_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_PU_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PU_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    }
    //For fastsim MET variations
    if (isData==0) {
        p_result.p_met_alt_up_SR.SRCR     = new TH1F(Form("SRCR_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_MET_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_met_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_MET_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_met_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_MET_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_MET_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_met_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_MET_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_MET_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    }
    //For PU variations
    if (doFakes == 1 || isData==0) {
        p_result.p_lep_alt_up_SR.SRCR     = new TH1F(Form("SRCR_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_LEP_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_lep_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_LEP_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_lep_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_LEP_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_LEP_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_lep_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_LEP_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_LEP_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    }
    //For bb variations
    if (doFakes == 1 || isData==0) {
        p_result.p_bb_alt_up_SR.SRCR     = new TH1F(Form("SRCR_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_BB_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_bb_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_BB_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_bb_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_BB_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_BB_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_bb_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_BB_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_BB_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    }
    //For prefiring SF variations for 2016, 2017
    if (isData==0) {
        p_result.p_prefire_alt_up_SR.SRCR     = new TH1F(Form("SRCR_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_prefire_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_prefire_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PREFIRE_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_prefire_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PREFIRE_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    }
    //For PU variations
    if (doFakes == 1 || isData==0) {
        p_result.p_isr_alt_up_SR.SRCR     = new TH1F(Form("SRCR_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISR_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_isr_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISR_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_isr_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_ISR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISR_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_isr_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_ISR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISR_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
    }
    p_result.p_unw_raw_SR.SRCR     = new TH1F(Form("SRCR_UNW_RAW_TOTAL_%s"   , chainTitleCh) , Form("SRCR_UNW_RAW_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
    p_result.p_unw_sgn_SR.SRCR     = new TH1F(Form("SRCR_UNW_SGN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_UNW_SGN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
    //For theory variations
    if (isData==0) {
        p_result.p_pdf_alt_up_SR.SRCR     = new TH1F(Form("SRCR_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PDF_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_pdf_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_PDF_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_pdf_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_PDF_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PDF_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_pdf_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_PDF_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_PDF_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_scale_alt_up_SR.SRCR     = new TH1F(Form("SRCR_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_scale_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_scale_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_SCALE_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_SCALE_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_scale_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_SCALE_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_SCALE_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_alphas_alt_up_SR.SRCR     = new TH1F(Form("SRCR_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_alphas_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_alphas_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ALPHAS_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_alphas_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ALPHAS_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_isrvar_alt_up_SR.SRCR     = new TH1F(Form("SRCR_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_isrvar_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_isrvar_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISRVAR_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_isrvar_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_ISRVAR_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_fsrvar_alt_up_SR.SRCR     = new TH1F(Form("SRCR_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRCR_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_fsrvar_alt_dn_SR.SRCR     = new TH1F(Form("SRCR_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRCR_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nsr,  0.5, nsr+0.5 );
        p_result.p_fsrvar_alt_up_SR.SRDISC     = new TH1F(Form("SRDISC_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_FSRVAR_UP_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
        p_result.p_fsrvar_alt_dn_SR.SRDISC     = new TH1F(Form("SRDISC_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , Form("SRDISC_FSRVAR_DN_TOTAL_%s"   , chainTitleCh) , nsrdisc,  0.5, nsrdisc+0.5 );
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
        }
        TString filename = currentFile->GetTitle();
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        int higgs_type = -1;
        float higgs_weight = 0;
        if (isHiggsScan) {
            if (filename.Contains("ttH")) higgs_type = 1;
            else if (filename.Contains("tHW")) higgs_type = 2;
            else if (filename.Contains("tHq")) higgs_type = 3;
            if (isHiggsPseudoscalar) higgs_type += 3;
            float xsec = xsec_higgs(higgs_type, higgs_mass);
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

            // skim = (njets_unc_dn>=2 or njets_JER_dn>=2 or njets>=2 or njets_unc_up>=2 or njets_JER_up>=2) and \
            //        (met_unc_dn>=50 or met_JER_dn>=50 or met>=50 or met_unc_up>=50 or met_JER_up>=50) and \
            //        (hyp_class != 4 or is_real_data);
            if (!ss::skim()) continue;

            if (isFastsim) {
                if ((fabs(mysparms[0]-ss::sparms()[0]) > 0.1) || (fabs(mysparms[1]-ss::sparms()[1]) > 0.1)) continue;
            }


            // if ((bootstrap_number > 0) and ((int)(ss::event() % bootstrap_number) == bootstrap_index)) continue;
            if ((bootstrap_number > 0) and (tr1->Rndm() < 1.0/bootstrap_number)) continue;

            //Reject not triggered
            if (!isFastsim) {
                if (!ss::fired_trigger()) continue;
            }
            if (!ss::passes_met_filters()) continue;


            // if (isHiggsScan) {
            //     // make sure the higgs mass point we are considering is the same as chain title
            //     if (fabs(mysparms[0]-ss::higgs_mass()) > 0.1) continue;
            // }

            float weight =  ss::is_real_data() ? 1.0 : ss::scale1fb()*lumiAG;


            if (isFastsim) {
                weight = getSMSscale1fb()*lumiAG;
            }

            if (isHiggsScan) {
                weight = higgs_weight*lumiAG;
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

            int nleps = (lep3good and lep3ccpt > 20) ? ((ss::lep4_passes_id() and ss::lep4_p4().pt() > 20) ? 4 : 3) : 2;

            //Require nominal baseline selections
            if (!passes_baseline(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), lep1id, lep2id, lep1ccpt, lep2ccpt)) continue;

            // // FIXME
            // if (isttW || isttZ) {
            //     weight *= 1.3;
            // }
            
            // if (isttH) {
            //     weight *= 1.26;
            // }

            // if (isHiggsPseudoscalar) weight *= ss::xsec_ps()/ss::xsec();
            if (ss::is_real_data()==0) {
                // weight*=eventScaleFactor(lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht());
                // missing trigger SF
                if (lep1good) weight *= leptonScaleFactor(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                if (lep2good) weight *= leptonScaleFactor(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                if (lep3good && lep3ccpt>20) weight *= leptonScaleFactor(year, lep3id, lep3ccpt, lep3eta, ss::ht());
                weight *= triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht());
                weight *= ss::weight_btagsf();
                weight *= getTruePUw(year, ss::trueNumInt()[0], 0);
                if (!isFastsim) {
                    weight *= ss::decayWSF();
                }
            }
            if (isFastsim) {
                weight *= fastsim_triggerScaleFactor(year, lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ss::ht());
                if (lep1good) weight *= fastsim_leptonScaleFactor(year, lep1id, lep1ccpt, lep1eta, ss::ht());
                if (lep2good) weight *= fastsim_leptonScaleFactor(year, lep2id, lep2ccpt, lep2eta, ss::ht());
                if (lep3good && lep3ccpt>20) weight *= fastsim_leptonScaleFactor(year, lep3id, lep3ccpt, lep3eta, ss::ht());
            }

            // // FIXME
            // if (doFakes) {
            //     weight *= 0.5;
            // }

            // Prefire stuff
            float weight_prefire_up_alt = 1.;
            float weight_prefire_dn_alt = 1.;

            if (ss::is_real_data()==0) {
                if (year == 2016) {
                    // weight_prefire_up_alt = weight*ss::prefire2016_sfup();
                    // weight_prefire_dn_alt = weight*ss::prefire2016_sfdown();
                    weight_prefire_up_alt = 1.; // FIXME not in 2016 ntuples, but in 2017
                    weight_prefire_dn_alt = 1.; // FIXME
                    weight *= ss::prefire2016_sf(); // modify weight after setting up/down
                } else if (year == 2017) {
                    // weight_prefire_up_alt = weight*ss::prefire2017_sfup();
                    // weight_prefire_dn_alt = weight*ss::prefire2017_sfdown();
                    weight_prefire_up_alt = 1.; // FIXME
                    weight_prefire_dn_alt = 1.; // FIXME
                    weight *= ss::prefire2017_sf(); // modify weight after setting up/down
                }
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
                // want to take full effect as unc
                if (ss::extragenb() == 2) {
                    float scaleamt = 0.7;
                    weight_bb_up_alt = (0.35+1)*weight;
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
            if (doFakes and doStitch) {
                if (!ss::passfilter()) continue;
            }
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

                // f_nbtags = ss::bdt_nbtags();
                // f_njets = ss::bdt_njets();
                // f_met = ss::bdt_met();
                // f_ptl2 = ss::bdt_ptl2();
                // f_nlb40 = ss::bdt_nlb40();
                // f_ntb40 = ss::bdt_ntb40();
                // f_nleps = ss::bdt_nleps();
                // f_htb = ss::bdt_htb();
                // f_q1 = ss::bdt_q1();
                // f_ptj1 = ss::bdt_ptj1();
                // f_ptj6 = ss::bdt_ptj6();
                // f_ptj7 = ss::bdt_ptj7();
                // f_ml1j1 = ss::bdt_ml1j1();
                // f_dphil1l2 = ss::bdt_dphil1l2();
                // f_maxmjoverpt = ss::bdt_maxmjoverpt();
                // f_ptl1 = ss::bdt_ptl1();
                // f_detal1l2 = ss::bdt_detal1l2();
                // f_ptj8 = ss::bdt_ptj8();
                // f_ptl3 = ss::bdt_ptl3();

                mvavalue = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                // mvavalue = reader->EvaluateMVA("BDT");

                // float mvavalue_ttw = get_prediction_ttw(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                // p_result.h_discttw.br->Fill(mvavalue_ttw,weight);

                f_nbtags = ss::bdt_jec_up_nbtags();
                f_njets = ss::bdt_jec_up_njets();
                f_met = ss::bdt_jec_up_met();
                f_htb = ss::bdt_jec_up_htb();
                f_nlb40 = ss::bdt_jec_up_nlb40();
                f_ntb40 = ss::bdt_jec_up_ntb40();
                mvavalueup = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                // mvavalueup = reader->EvaluateMVA("BDT");

                f_nbtags = ss::bdt_jec_dn_nbtags();
                f_njets = ss::bdt_jec_dn_njets();
                f_met = ss::bdt_jec_dn_met();
                f_htb = ss::bdt_jec_dn_htb();
                f_nlb40 = ss::bdt_jec_dn_nlb40();
                f_ntb40 = ss::bdt_jec_dn_ntb40();
                mvavaluedn = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                // mvavaluedn = reader->EvaluateMVA("BDT");

                f_nbtags = ss::bdt_jer_up_nbtags();
                f_njets = ss::bdt_jer_up_njets();
                f_met = ss::bdt_jer_up_met();
                f_htb = ss::bdt_jer_up_htb();
                f_nlb40 = ss::bdt_jer_up_nlb40();
                f_ntb40 = ss::bdt_jer_up_ntb40();
                mvavalueJERup = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                // mvavalueJERup = reader->EvaluateMVA("BDT");

                f_nbtags = ss::bdt_jer_dn_nbtags();
                f_njets = ss::bdt_jer_dn_njets();
                f_met = ss::bdt_jer_dn_met();
                f_htb = ss::bdt_jer_dn_htb();
                f_nlb40 = ss::bdt_jer_dn_nlb40();
                f_ntb40 = ss::bdt_jer_dn_ntb40();
                mvavalueJERdn = get_prediction(f_nbtags,f_njets,f_met,f_ptl2,f_nlb40,f_ntb40,f_nleps,f_htb,f_q1,f_ptj1,f_ptj6,f_ptj7,f_ml1j1,f_dphil1l2,f_maxmjoverpt,f_ptl1,f_detal1l2,f_ptj8,f_ptl3);
                // mvavalueJERdn = reader->EvaluateMVA("BDT");

                p_result.h_disc.br->Fill(mvavalue,weight);


            }

            int SRdisc =        getBDTBin(nbdtbins, mvavalue, SR==1);
            int SRdisc_unc_up = getBDTBin(nbdtbins, mvavalueup, SR_unc_up==1);
            int SRdisc_unc_dn = getBDTBin(nbdtbins, mvavaluedn, SR_unc_dn==1);
            int SRdisc_JER_up = getBDTBin(nbdtbins, mvavalueJERup, SR_JER_up==1);
            int SRdisc_JER_dn = getBDTBin(nbdtbins, mvavalueJERdn, SR_JER_dn==1);

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

            if (isData  == 0 && SR >= 0 && SR_unc_up > 0)            p_result.p_jes_alt_up_SR.CatFill(1, SR_unc_up, weight);
            if (isData  == 0 && SR >= 0 && SR_unc_dn > 0)            p_result.p_jes_alt_dn_SR.CatFill(1, SR_unc_dn, weight);

            if (isData  == 0 && SR >= 0 && SR_JER_up > 0)            p_result.p_jer_alt_up_SR.CatFill(1, SR_JER_up, weight);
            if (isData  == 0 && SR >= 0 && SR_JER_dn > 0)            p_result.p_jer_alt_dn_SR.CatFill(1, SR_JER_dn, weight);

            if (isData  == 0 && SRdisc >= 0 && SRdisc_unc_up > 0)            p_result.p_jes_alt_up_SR.CatFill(2, SRdisc_unc_up, weight);
            if (isData  == 0 && SRdisc >= 0 && SRdisc_unc_dn > 0)            p_result.p_jes_alt_dn_SR.CatFill(2, SRdisc_unc_dn, weight);

            if (isData  == 0 && SRdisc >= 0 && SRdisc_JER_up > 0)            p_result.p_jer_alt_up_SR.CatFill(2, SRdisc_JER_up, weight);
            if (isData  == 0 && SRdisc >= 0 && SRdisc_JER_dn > 0)            p_result.p_jer_alt_dn_SR.CatFill(2, SRdisc_JER_dn, weight);

            if (isData  == 0 && SRgenmet > 0)            p_result.p_met_alt_up_SR.CatFill(1, SRgenmet, weight);
            if (SR >= 0) {
                p_result.SR.CatFill(1, SR, weight);
                if ( !doFakes || ss::is_real_data() ) { // when filling for fakes, only consider real data
                    p_result.p_unw_raw_SR.CatFill(1, SR, 1);
                    p_result.p_unw_sgn_SR.CatFill(1, SR, weight > 0 ? 1 : -1);
                }
                if (doFakes == 1 )            p_result.p_fake_alt_up_SR.CatFill(1, SR, weight_alt_FR);
                if (doFakes == 1 )            p_result.p_fake_unw_up_SR.CatFill(1, SR, weight > 0 ? 1 : -1);
                if (isData  == 0 )            p_result.p_btagSF_alt_up_SR.CatFill(1, SR, weight_btag_up_alt);
                if (isData  == 0 )            p_result.p_btagSF_alt_dn_SR.CatFill(1, SR, weight_btag_dn_alt);
                if (isData  == 0 )            p_result.p_pu_alt_up_SR.CatFill(1, SR, weight_pu_up_alt);
                if (isData  == 0 )            p_result.p_pu_alt_dn_SR.CatFill(1, SR, weight_pu_dn_alt);
                if (isData  == 0 )            p_result.p_isr_alt_up_SR.CatFill(1, SR, weight_isr_up_alt);
                if (isData  == 0 )            p_result.p_bb_alt_up_SR.CatFill(1, SR, weight_bb_up_alt);
                if (isData  == 0 )            p_result.p_prefire_alt_up_SR.CatFill(1, SR, weight_prefire_up_alt);
                if (isData  == 0 )            p_result.p_prefire_alt_dn_SR.CatFill(1, SR, weight_prefire_dn_alt);
                if (isData  == 0 )            p_result.p_lep_alt_up_SR.CatFill(1, SR, weight_lep_up_alt);
                // if (istttt || isttW || isttZ || isttH) {
                    p_result.p_scale_alt_up_SR.CatFill(1, SR,    (ss::weight_scale_UP() > -9000 ? ss::weight_scale_UP()/norm_scale_up : 1.0)*weight);
                    p_result.p_scale_alt_dn_SR.CatFill(1, SR,    (ss::weight_scale_DN() > -9000 ? ss::weight_scale_DN()/norm_scale_dn : 1.0)*weight);
                    p_result.p_alphas_alt_up_SR.CatFill(1, SR,    (ss::weight_alphas_UP() > -9000 ? ss::weight_alphas_UP() : 1.0)*weight);
                    p_result.p_alphas_alt_dn_SR.CatFill(1, SR,    (ss::weight_alphas_DN() > -9000 ? ss::weight_alphas_DN() : 1.0)*weight);
                    p_result.p_pdf_alt_up_SR.CatFill(1, SR,      (ss::weight_pdf_UP() > -9000 ? ss::weight_pdf_UP()/norm_pdf_up : 1.0)*weight);
                    p_result.p_pdf_alt_dn_SR.CatFill(1, SR,      (ss::weight_pdf_DN() > -9000 ? ss::weight_pdf_DN()/norm_pdf_dn : 1.0)*weight);
                    p_result.p_isrvar_alt_up_SR.CatFill(1, SR      , ss::weight_isrvar_UP()*weight);
                    p_result.p_isrvar_alt_dn_SR.CatFill(1, SR      , ss::weight_isrvar_DN()*weight);
                    p_result.p_fsrvar_alt_up_SR.CatFill(1, SR      , ss::weight_fsrvar_UP()*weight);
                    p_result.p_fsrvar_alt_dn_SR.CatFill(1, SR      , ss::weight_fsrvar_DN()*weight);
                // }
            }
            if (SRdisc >= 0) {
                p_result.SR.CatFill(2, SRdisc, weight);
                if (doFakes == 1 )            p_result.p_fake_alt_up_SR.CatFill(2, SRdisc, weight_alt_FR);
                if (doFakes == 1 )            p_result.p_fake_unw_up_SR.CatFill(2, SRdisc, weight > 0 ? 1 : -1);
                if (isData  == 0 )            p_result.p_btagSF_alt_up_SR.CatFill(2, SRdisc, weight_btag_up_alt);
                if (isData  == 0 )            p_result.p_btagSF_alt_dn_SR.CatFill(2, SRdisc, weight_btag_dn_alt);
                if (isData  == 0 )            p_result.p_pu_alt_up_SR.CatFill(2, SRdisc, weight_pu_up_alt);
                if (isData  == 0 )            p_result.p_pu_alt_dn_SR.CatFill(2, SRdisc, weight_pu_dn_alt);
                if (isData  == 0 )            p_result.p_isr_alt_up_SR.CatFill(2, SRdisc, weight_isr_up_alt);
                if (isData  == 0 )            p_result.p_bb_alt_up_SR.CatFill(2, SRdisc, weight_bb_up_alt);
                if (isData  == 0 )            p_result.p_prefire_alt_up_SR.CatFill(2, SRdisc, weight_prefire_up_alt);
                if (isData  == 0 )            p_result.p_prefire_alt_dn_SR.CatFill(2, SRdisc, weight_prefire_dn_alt);
                if (isData  == 0 )            p_result.p_lep_alt_up_SR.CatFill(2, SRdisc, weight_lep_up_alt);
                // if (istttt || isttW || isttZ || isttH) {
                    p_result.p_scale_alt_up_SR.CatFill(2, SRdisc,    (ss::weight_scale_UP() > -9000 ? ss::weight_scale_UP()/norm_scale_up : 1.0)*weight);
                    p_result.p_scale_alt_dn_SR.CatFill(2, SRdisc,    (ss::weight_scale_DN() > -9000 ? ss::weight_scale_DN()/norm_scale_dn : 1.0)*weight);
                    p_result.p_alphas_alt_up_SR.CatFill(2, SRdisc,    (ss::weight_alphas_UP() > -9000 ? ss::weight_alphas_UP() : 1.0)*weight);
                    p_result.p_alphas_alt_dn_SR.CatFill(2, SRdisc,    (ss::weight_alphas_DN() > -9000 ? ss::weight_alphas_DN() : 1.0)*weight);
                    p_result.p_pdf_alt_up_SR.CatFill(2, SRdisc,      (ss::weight_pdf_UP() > -9000 ? ss::weight_pdf_UP()/norm_pdf_up : 1.0)*weight);
                    p_result.p_pdf_alt_dn_SR.CatFill(2, SRdisc,      (ss::weight_pdf_DN() > -9000 ? ss::weight_pdf_DN()/norm_pdf_dn : 1.0)*weight);
                    p_result.p_isrvar_alt_up_SR.CatFill(2, SRdisc, ss::weight_isrvar_UP()*weight);
                    p_result.p_isrvar_alt_dn_SR.CatFill(2, SRdisc, ss::weight_isrvar_DN()*weight);
                    p_result.p_fsrvar_alt_up_SR.CatFill(2, SRdisc, ss::weight_fsrvar_UP()*weight);
                    p_result.p_fsrvar_alt_dn_SR.CatFill(2, SRdisc, ss::weight_fsrvar_DN()*weight);
                // }
            }

            bool plotlep3 = ss::lep3_passes_id() && lep3ccpt > 20.;


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



            if (SR != 1) { // non ttZ CR
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
                // for (unsigned int ijet = 0; ijet < ss::btags().size(); ijet++) {
                //     p_result.h_bjetpt.br->Fill(ss::btags()[ijet].pt(),weight);
                // }
                // for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
                //     p_result.h_jetpt.br->Fill(ss::jets()[ijet].pt(),weight);
                // }
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
        p_result.SR.AvoidNegativeYields();
        p_result.p_btagSF_alt_dn_SR.AvoidNegativeYields();
        p_result.p_btagSF_alt_up_SR.AvoidNegativeYields();
        p_result.p_isr_alt_dn_SR.AvoidNegativeYields();
        p_result.p_isr_alt_up_SR.AvoidNegativeYields();
        p_result.p_jer_alt_dn_SR.AvoidNegativeYields();
        p_result.p_jer_alt_up_SR.AvoidNegativeYields();
        p_result.p_jes_alt_dn_SR.AvoidNegativeYields();
        p_result.p_jes_alt_up_SR.AvoidNegativeYields();
        p_result.p_lep_alt_dn_SR.AvoidNegativeYields();
        p_result.p_lep_alt_up_SR.AvoidNegativeYields();
        p_result.p_pu_alt_dn_SR.AvoidNegativeYields();
        p_result.p_pu_alt_up_SR.AvoidNegativeYields();
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

