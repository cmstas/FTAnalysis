// Usage:
// > root -b doAll.C

// C++
#include <iostream>
#include <iomanip>
#include <vector>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "Math/VectorUtil.h"
#include <unistd.h> //isatty

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

// lepfilter
#include "LeptonTree.cc"

#include "../../misc/common_utils.h"
#include "../../misc/tqdm.h"

using namespace std;
using namespace lepton_tree;

#ifdef __MAKECINT__ 
#pragma link C++ class ROOT::Math::PxPyPzE4D<float>+; 
#pragma link C++ class ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >+;
#endif 

float getPt(float pt, bool extrPtRel = false) {
    if(!extrPtRel && pt >= 90.) return 89.;
    if(extrPtRel && pt >= 150.) return 149.;
    if(pt < 10.)  return 11.;   //use this if lower FR histo bound is 10.
    return pt;
}

float getEta(float eta, float ht, bool extrPtRel = false) {
    if (extrPtRel) {
        if(ht >= 800) return 799;
        return ht;
    }
    if(fabs(eta) >= 2.4) return 2.3;
    return fabs(eta);
}


float DeltaPhi(float phi1, float phi2){
    float dPhi = phi1 - phi2;
    while (dPhi  >  TMath::Pi()) dPhi -= 2*TMath::Pi();
    while (dPhi <= -TMath::Pi()) dPhi += 2*TMath::Pi();
    return fabs(dPhi);
}

bool isFake() {
    return (motherID() <= 0 && motherID() != -3);
}

bool STOP_REQUESTED = false;

// int ScanChain( TChain* chain, TString outfile, int year=-1, TString option="", bool fast = true, int nEvents = -1) {//, string skimFilePrefix = "test")
int ScanChain( TChain* chain, TString option="", TString outfile="test.root", int year=-1, bool fast = true, int nEvents = -1) {//, string skimFilePrefix = "test") {

    // Benchmark
    TBenchmark *bmark = new TBenchmark();
    bmark->Start("benchmark");

    bool useRelIso = false;
    if (option.Contains("useRelIso")) useRelIso = true;

    bool useLooseEMVA = true;
    // if (option.Contains("useLooseEMVA")) useLooseEMVA = true;

    bool requireMCMatch = false;
    if (option.Contains("requireMCMatch")) requireMCMatch = true;
    
    bool doAbove25 = false;
    if (option.Contains("doAbove25")) doAbove25 = true;

    bool doMu18Only = false;
    if (option.Contains("doMu18Only")) doMu18Only = true;
    
    bool doAbove18 = false;
    if (option.Contains("doAbove18")) doAbove18 = true;

    bool noPUweight = false;
    if (option.Contains("noPUweight")) noPUweight = true;

    bool absweight = false;
    if (option.Contains("absweight")) absweight = true;

    bool noConeCorr = false;
    if (option.Contains("noConeCorr")) noConeCorr = true;

    bool usePtRatioCor = false;
    if (option.Contains("usePtRatioCor")) usePtRatioCor = true;

    bool doBonly = false;
    if (option.Contains("doBonly")) doBonly = true;

    bool doConly = false;
    if (option.Contains("doConly")) doConly = true;

    bool doLightonly = false;
    if (option.Contains("doLightonly")) doLightonly = true;

    bool useIsoTrigs = false;
    if (option.Contains("IsoTrigs")) useIsoTrigs = true;

    bool noMissHits = false;
    if (option.Contains("noMissHits")) noMissHits = true;

    bool doHighMET = false;
    if (option.Contains("doHighMET")) doHighMET = true;

    bool bypassLowMET = false;
    if (option.Contains("bypassLowMET")) bypassLowMET = true;

    bool bypassTriggers = false;
    if (option.Contains("bypassTriggers")) bypassTriggers = true;

    bool allowMoreFO = false;
    if (option.Contains("allowMoreFO")) allowMoreFO = true;

    bool requireTwoJets = false;
    if (option.Contains("requireTwoJets")) requireTwoJets = true;

    bool requireTag = false;
    if (option.Contains("requireTag")) requireTag = true;

    bool twoFO = false;
    if (option.Contains("twoFO")) twoFO = true;

    bool doubleHadronic = false;
    if (option.Contains("doubleHadronic")) doubleHadronic = true;

    bool singleHadronic = false;
    if (option.Contains("singleHadronic")) singleHadronic = true;

    if (option.Contains("Data2016")) year = 2016;
    if (option.Contains("Data2017")) year = 2017;
    if (option.Contains("Data2018")) year = 2018;

    bool isReReco = (option.Contains("ReReco"));

    bool quiet = option.Contains("quiet");

    bool anyPt = false;

    bool doJEC = false;

    bool debug = false;

    int nptbins = 7;
    float ptbins[8] = {10., 15., 20., 25., 35., 50., 70., 90.};
    int netabins = 3;
    float etabins_mu[4] = {0., 1.2, 2.1, 2.4};
    float etabins_el[4] = {0., 0.8, 1.479, 2.5};

    int fine_nptbins = 11;
    float fine_ptbins[12] = {10., 15., 20., 25., 30., 35., 45., 55., 70., 90., 120., 150.};
    int fine_netabins_el = 5;
    float fine_etabins_el[6] = {0., 0.5, 1.0, 1.5, 2.0, 2.5};
    int fine_netabins_mu = 5;
    float fine_etabins_mu[6] = {0., 0.5, 1.0, 1.5, 2.0, 2.4};

    // nominal
    float MTCR_MET_CUT = 30.;
    float MTCR_PT_CUT = 20.;





    // handmade "prescales" from normalizeToZPeak.C

    // // v13 - correct rho, latest fake WPs for 2017
    // float sf_HLT_Ele17_CaloIdM_TrackIdM_PFJet30 = 1.3700;
    // float sf_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.3764;
    // float sf_HLT_Ele8_CaloIdM_TrackIdM_PFJet30 = 1.1107;
    // float sf_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.1203;
    // float sf_HLT_Mu17_TrkIsoVVL = 1.0232;
    // float sf_HLT_Mu17 = 1.0227;
    // float sf_HLT_Mu8_TrkIsoVVL = 0.6999;
    // float sf_HLT_Mu8 = 0.7015;


    // // v23 latest 2017, nmiss==0
    // float sf_HLT_Ele17_CaloIdM_TrackIdM_PFJet30 = 1.3777;
    // float sf_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.3841;
    // float sf_HLT_Ele8_CaloIdM_TrackIdM_PFJet30 = 1.1027;
    // float sf_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.1112;
    // float sf_HLT_Mu17_TrkIsoVVL = 1.0261;
    // float sf_HLT_Mu17 = 1.0256;
    // float sf_HLT_Mu8_TrkIsoVVL = 0.7031;
    // float sf_HLT_Mu8 = 0.7048;
    // float sf_HLT_IsoMu27 = 1.1146;

    // // v23 with latest V1 electron SFs, but only medium muon ID SFs
    // float sf_HLT_Ele17_CaloIdM_TrackIdM_PFJet30 = 1.0475;
    // float sf_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.0523;
    // float sf_HLT_Ele8_CaloIdM_TrackIdM_PFJet30 = 0.8384;
    // float sf_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0.8448;
    // float sf_HLT_Mu17_TrkIsoVVL = 1.0300;
    // float sf_HLT_Mu17 = 1.0296;
    // float sf_HLT_Mu8_TrkIsoVVL = 0.7059;
    // float sf_HLT_Mu8 = 0.7075;
    // float sf_HLT_IsoMu27 = 1.1188;

    // new looper with v32 JECs
    float sf_HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.0259;
    float sf_HLT_Ele17_CaloIdM_TrackIdM_PFJet30 = 1.1322;
    float sf_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.1406;
    float sf_HLT_Ele23_CaloIdM_TrackIdM_PFJet30 = 1.1338;
    float sf_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0.9540;
    float sf_HLT_Ele8_CaloIdM_TrackIdM_PFJet30 = 0.9492;
    float sf_HLT_IsoMu27 = 1.1300;
    float sf_HLT_Mu17 = 1.0362;
    float sf_HLT_Mu17_TrkIsoVVL = 1.0358;
    float sf_HLT_Mu8 = 0.7121;
    float sf_HLT_Mu8_TrkIsoVVL = 0.7123;
    float sf_HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.1406;

    if (year == 2018) {
        // v23 2018

        // sf_HLT_Ele17_CaloIdM_TrackIdM_PFJet30 = 2.5130;
        // sf_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 = 2.5203;
        // sf_HLT_Ele8_CaloIdM_TrackIdM_PFJet30 = 2.1316;
        // sf_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 = 2.1833;
        // sf_HLT_Mu17_TrkIsoVVL = 1.9637;
        // sf_HLT_Mu17 = 1.9632;
        // sf_HLT_Mu8_TrkIsoVVL = 1.5784;
        // sf_HLT_Mu8 = 1.5784;
        // sf_HLT_IsoMu27 = 2.0158;

        sf_HLT_Ele17_CaloIdM_TrackIdM_PFJet30 = 1.0;
        sf_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.0;
        sf_HLT_Ele8_CaloIdM_TrackIdM_PFJet30 = 1.0;
        sf_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.0;
        sf_HLT_Mu17_TrkIsoVVL = 1.0;
        sf_HLT_Mu17 = 1.0;
        sf_HLT_Mu8_TrkIsoVVL = 1.0;
        sf_HLT_Mu8 = 1.0;
        sf_HLT_IsoMu27 = 1.0;

    }


    // From normalize_zpeak/

    if (year == 2016) {
// year = 2016
    sf_HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30 = 170.1279;
    sf_HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30 = 172.6997;
    sf_HLT_Ele17_CaloIdM_TrackIdM_PFJet30 = 173.0246;
    sf_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 = 172.9588;
    sf_HLT_Ele23_CaloIdM_TrackIdM_PFJet30 = 173.0231;
    sf_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 = 4241.8406;
    sf_HLT_Ele8_CaloIdM_TrackIdM_PFJet30 = 4231.5585;
    sf_HLT_IsoMu27 = 1.0268;
    sf_HLT_Mu17 = 89.5843;
    sf_HLT_Mu17_TrkIsoVVL = 89.5883;
    sf_HLT_Mu8 = 56.0419;
    sf_HLT_Mu8_TrkIsoVVL = 57.4986;
}
if (year == 2017) {
// year = 2017
    sf_HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.0259;
    sf_HLT_Ele17_CaloIdM_TrackIdM_PFJet30 = 1.1322;
    sf_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.1406;
    sf_HLT_Ele23_CaloIdM_TrackIdM_PFJet30 = 1.1338;
    sf_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0.9540;
    sf_HLT_Ele8_CaloIdM_TrackIdM_PFJet30 = 0.9492;
    sf_HLT_IsoMu27 = 1.1300;
    sf_HLT_Mu17 = 1.0362;
    sf_HLT_Mu17_TrkIsoVVL = 1.0358;
    sf_HLT_Mu8 = 0.7121;
    sf_HLT_Mu8_TrkIsoVVL = 0.7123;
}
if (year == 2018 and !isReReco) {
// year = 2018
    sf_HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0.8710;
    sf_HLT_Ele17_CaloIdM_TrackIdM_PFJet30 = 1.0038;
    sf_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 = 1.0027;
    sf_HLT_Ele23_CaloIdM_TrackIdM_PFJet30 = 1.0037;
    sf_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0.8481;
    sf_HLT_Ele8_CaloIdM_TrackIdM_PFJet30 = 0.8472;
    sf_HLT_IsoMu27 = 1.1954;
    sf_HLT_Mu17 = 1.1697;
    sf_HLT_Mu17_TrkIsoVVL = 1.1699;
    sf_HLT_Mu8 = 0.9427;
    sf_HLT_Mu8_TrkIsoVVL = 0.9554;
}
if (year == 2018 and  isReReco) {
// year = rereco_2018
    sf_HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0.8650;
    sf_HLT_Ele17_CaloIdM_TrackIdM_PFJet30 = 0.9899;
    sf_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0.9904;
    sf_HLT_Ele23_CaloIdM_TrackIdM_PFJet30 = 0.9939;
    sf_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0.8565;
    sf_HLT_Ele8_CaloIdM_TrackIdM_PFJet30 = 0.8440;
    sf_HLT_IsoMu27 = 1.1460;
    sf_HLT_Mu17 = 1.1200;
    sf_HLT_Mu17_TrkIsoVVL = 1.1202;
    sf_HLT_Mu8 = 0.8925;
    sf_HLT_Mu8_TrkIsoVVL = 0.9037;
}

    if (false) {
        //this should be ok as long as there are less bins in the extrPtRel case
        ptbins[0] = 10.;
        ptbins[1] = 50.;
        ptbins[2] = 100.;
        ptbins[3] = 150.;
        nptbins = 3;
        etabins_mu[0] = 0.;
        etabins_mu[1] = 400.;
        etabins_mu[2] = 800.;
        etabins_el[0] = 0.;
        etabins_el[1] = 400.;
        etabins_el[2] = 800.;
        netabins = 2;
    } 


    // Example Histograms
    TDirectory *rootdir = gDirectory->GetDirectory("Rint:");

    TH2D *Nt_histo_e = new TH2D("Nt_histo_e", "Nt vs Pt, Eta (electrons)", nptbins,ptbins,netabins,etabins_el);
    Nt_histo_e->SetDirectory(rootdir);
    Nt_histo_e->Sumw2();

    TH2D *Nl_histo_e = new TH2D("Nl_histo_e", "Nl vs Pt, Eta (electrons)", nptbins,ptbins,netabins,etabins_el);
    Nl_histo_e->SetDirectory(rootdir);
    Nl_histo_e->Sumw2();

    TH2D *Nt_histo_mu = new TH2D("Nt_histo_mu", "Nt vs Pt, Eta (muons)", nptbins,ptbins,netabins,etabins_mu);
    Nt_histo_mu->SetDirectory(rootdir);
    Nt_histo_mu->Sumw2();

    TH2D *Nl_histo_mu = new TH2D("Nl_histo_mu", "Nl vs Pt, Eta (muons)", nptbins,ptbins,netabins,etabins_mu);
    Nl_histo_mu->SetDirectory(rootdir);
    Nl_histo_mu->Sumw2();

    TH2D *Nl_cone_histo_e = new TH2D("Nl_cone_histo_e", "Nl vs Cone Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins_el);
    Nl_cone_histo_e->SetDirectory(rootdir);
    Nl_cone_histo_e->Sumw2();

    TH2D *Nl_cone_histo_mu = new TH2D("Nl_cone_histo_mu", "Nl vs Cone Energy, Eta (muons)", nptbins,ptbins,netabins,etabins_mu);
    Nl_cone_histo_mu->SetDirectory(rootdir);
    Nl_cone_histo_mu->Sumw2();

    TH2D *Nl_conevsraw_histo_e = new TH2D("Nl_conevsraw_histo_e", "Nl Cone pT vs Raw pT, Eta (electrons)", nptbins,ptbins,nptbins,ptbins);
    Nl_conevsraw_histo_e->SetDirectory(rootdir);
    Nl_conevsraw_histo_e->Sumw2();
    TH2D *Nl_conevsraw_histo_mu = new TH2D("Nl_conevsraw_histo_mu", "Nl Cone pT vs Raw pT, Eta (muons)", nptbins,ptbins,nptbins,ptbins);
    Nl_conevsraw_histo_mu->SetDirectory(rootdir);
    Nl_conevsraw_histo_mu->Sumw2();
    TH2D *Nt_conevsraw_histo_e = new TH2D("Nt_conevsraw_histo_e", "Nt Cone pT vs Raw pT, Eta (electrons)", nptbins,ptbins,nptbins,ptbins);
    Nt_conevsraw_histo_e->SetDirectory(rootdir);
    Nt_conevsraw_histo_e->Sumw2();
    TH2D *Nt_conevsraw_histo_mu = new TH2D("Nt_conevsraw_histo_mu", "Nt Cone pT vs Raw pT, Eta (muons)", nptbins,ptbins,nptbins,ptbins);
    Nt_conevsraw_histo_mu->SetDirectory(rootdir);
    Nt_conevsraw_histo_mu->Sumw2();



    // TH2D *Nt_fine_histo_e = new TH2D("Nt_fine_histo_e", "Nt vs Pt, Eta (electrons)", 30,0,150,10,0,2.5);
    TH2D *Nt_fine_histo_e = new TH2D("Nt_fine_histo_e", "Nt vs Pt, Eta (electrons)", fine_nptbins,fine_ptbins,fine_netabins_el,fine_etabins_el);
    Nt_fine_histo_e->SetDirectory(rootdir);
    Nt_fine_histo_e->Sumw2();

    // TH2D *Nl_fine_cone_histo_e = new TH2D("Nl_fine_cone_histo_e", "Nl vs Cone Energy, Eta (electrons)", 30,0,150,10,0,2.5);
    TH2D *Nl_fine_cone_histo_e = new TH2D("Nl_fine_cone_histo_e", "Nl vs Cone Energy, Eta (electrons)", fine_nptbins,fine_ptbins,fine_netabins_el,fine_etabins_el);
    Nl_fine_cone_histo_e->SetDirectory(rootdir);
    Nl_fine_cone_histo_e->Sumw2();

    TH2D *Nt_fine_histo_mu = new TH2D("Nt_fine_histo_mu", "Nt vs Pt, Eta (muons)", fine_nptbins,fine_ptbins,fine_netabins_mu,fine_etabins_mu);
    Nt_fine_histo_mu->SetDirectory(rootdir);
    Nt_fine_histo_mu->Sumw2();

    TH2D *Nl_fine_cone_histo_mu = new TH2D("Nl_fine_cone_histo_mu", "Nl vs Cone Energy, Eta (muons)", fine_nptbins,fine_ptbins,fine_netabins_mu,fine_etabins_mu);
    Nl_fine_cone_histo_mu->SetDirectory(rootdir);
    Nl_fine_cone_histo_mu->Sumw2();


    TH2D *Nt_jet_histo_e = new TH2D("Nt_jet_histo_e", "Nt vs Jet Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins_el);
    Nt_jet_histo_e->SetDirectory(rootdir);
    Nt_jet_histo_e->Sumw2();

    TH2D *Nt_jet_histo_mu = new TH2D("Nt_jet_histo_mu", "Nt vs Jet Energy, Eta (muons)", nptbins,ptbins,netabins,etabins_mu);
    Nt_jet_histo_mu->SetDirectory(rootdir);
    Nt_jet_histo_mu->Sumw2();

    TH2D *Nl_jet_histo_e = new TH2D("Nl_jet_histo_e", "Nl vs Jet Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins_el);
    Nl_jet_histo_e->SetDirectory(rootdir);
    Nl_jet_histo_e->Sumw2();

    TH2D *Nl_jet_histo_mu = new TH2D("Nl_jet_histo_mu", "Nl vs Jet Energy, Eta (muons)", nptbins,ptbins,netabins,etabins_mu);
    Nl_jet_histo_mu->SetDirectory(rootdir);
    Nl_jet_histo_mu->Sumw2();

    TH2D *Nt_jet_highpt_histo_e = new TH2D("Nt_jet_highpt_histo_e", "Nt vs Jet Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins_el);
    Nt_jet_highpt_histo_e->SetDirectory(rootdir);
    Nt_jet_highpt_histo_e->Sumw2();

    TH2D *Nt_jet_highpt_histo_mu = new TH2D("Nt_jet_highpt_histo_mu", "Nt vs Jet Energy, Eta (muons)", nptbins,ptbins,netabins,etabins_mu);
    Nt_jet_highpt_histo_mu->SetDirectory(rootdir);
    Nt_jet_highpt_histo_mu->Sumw2();

    TH2D *Nl_jet_highpt_histo_e = new TH2D("Nl_jet_highpt_histo_e", "Nl vs Jet Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins_el);
    Nl_jet_highpt_histo_e->SetDirectory(rootdir);
    Nl_jet_highpt_histo_e->Sumw2();

    TH2D *Nl_jet_highpt_histo_mu = new TH2D("Nl_jet_highpt_histo_mu", "Nl vs Jet Energy, Eta (muons)", nptbins,ptbins,netabins,etabins_mu);
    Nl_jet_highpt_histo_mu->SetDirectory(rootdir);
    Nl_jet_highpt_histo_mu->Sumw2();

    TH2D *Nt_jet_lowpt_histo_e = new TH2D("Nt_jet_lowpt_histo_e", "Nt vs Jet Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins_el);
    Nt_jet_lowpt_histo_e->SetDirectory(rootdir);
    Nt_jet_lowpt_histo_e->Sumw2();

    TH2D *Nt_jet_lowpt_histo_mu = new TH2D("Nt_jet_lowpt_histo_mu", "Nt vs Jet Energy, Eta (muons)", nptbins,ptbins,netabins,etabins_mu);
    Nt_jet_lowpt_histo_mu->SetDirectory(rootdir);
    Nt_jet_lowpt_histo_mu->Sumw2();

    TH2D *Nl_jet_lowpt_histo_e = new TH2D("Nl_jet_lowpt_histo_e", "Nl vs Jet Energy, Eta (electrons)", nptbins,ptbins,netabins,etabins_el);
    Nl_jet_lowpt_histo_e->SetDirectory(rootdir);
    Nl_jet_lowpt_histo_e->Sumw2();

    TH2D *Nl_jet_lowpt_histo_mu = new TH2D("Nl_jet_lowpt_histo_mu", "Nl vs Jet Energy, Eta (muons)", nptbins,ptbins,netabins,etabins_mu);
    Nl_jet_lowpt_histo_mu->SetDirectory(rootdir);
    Nl_jet_lowpt_histo_mu->Sumw2();

    TH1F *NBs_BR_histo_e = new TH1F("NBs_BR_histo_e", "Number of FO's from B's vs Nbtags (els)", 5,0,5);
    NBs_BR_histo_e->SetDirectory(rootdir);
    NBs_BR_histo_e->Sumw2();

    TH1F *NBs_BR_histo_mu = new TH1F("NBs_BR_histo_mu", "Number of FO's from B's vs Nbtags (muons)", 5,0,5);
    NBs_BR_histo_mu->SetDirectory(rootdir);
    NBs_BR_histo_mu->Sumw2();

    TH1F *NnotBs_BR_histo_e = new TH1F("NnotBs_BR_histo_e", "Number of FO's NOT from B's vs Nbtags (els)", 5,0,5);
    NnotBs_BR_histo_e->SetDirectory(rootdir);
    NnotBs_BR_histo_e->Sumw2();

    TH1F *NnotBs_BR_histo_mu = new TH1F("NnotBs_BR_histo_mu", "Number of FO's NOT from B's vs Nbtags (muons)", 5,0,5);
    NnotBs_BR_histo_mu->SetDirectory(rootdir);
    NnotBs_BR_histo_mu->Sumw2();

    TH2D *pTrelvsIso_histo_el = new TH2D("pTrelvsIso_histo_el", "pTrel vs Iso (Electrons)", 10, 0., 1., 15, 0., 30.);
    pTrelvsIso_histo_el->SetDirectory(rootdir);
    pTrelvsIso_histo_el->Sumw2();

    TH2D *pTrelvsIso_histo_mu = new TH2D("pTrelvsIso_histo_mu", "pTrel vs Iso (Muons)", 10, 0., 1., 15, 0., 30.);
    pTrelvsIso_histo_mu->SetDirectory(rootdir);
    pTrelvsIso_histo_mu->Sumw2();

    TH1D *pTrel_histo_el = new TH1D("pTrel_histo_el", "pTrel (Electrons)", 15, 0., 30.);
    pTrel_histo_el->SetDirectory(rootdir);
    pTrel_histo_el->Sumw2();

    TH1D *pTrel_histo_mu = new TH1D("pTrel_histo_mu", "pTrel (Muons)", 15, 0., 30.);
    pTrel_histo_mu->SetDirectory(rootdir);
    pTrel_histo_mu->Sumw2();

    TH1D *pTratio_histo_el = new TH1D("pTratio_histo_el", "pTratio (Electrons)", 15, 0., 1.5);
    pTratio_histo_el->SetDirectory(rootdir);
    pTratio_histo_el->Sumw2();

    TH1D *pTratio_histo_mu = new TH1D("pTratio_histo_mu", "pTratio (Muons)", 15, 0., 1.5);
    pTratio_histo_mu->SetDirectory(rootdir);
    pTratio_histo_mu->Sumw2();

    TH1D *miniiso_histo_el = new TH1D("miniiso_histo_el", "miniiso (Electrons)", 15, 0., 0.3);
    miniiso_histo_el->SetDirectory(rootdir);
    miniiso_histo_el->Sumw2();

    TH1D *miniiso_histo_mu = new TH1D("miniiso_histo_mu", "miniiso (Muons)", 15, 0., 0.3);
    miniiso_histo_mu->SetDirectory(rootdir);
    miniiso_histo_mu->Sumw2();

    TH1D *miniiso_histo_el_loose70 = new TH1D("miniiso_histo_el_loose70", "miniiso (Electrons)", 15, 0., 0.3);
    miniiso_histo_el_loose70->SetDirectory(rootdir);
    miniiso_histo_el_loose70->Sumw2();

    TH1D *miniiso_histo_el_tight70 = new TH1D("miniiso_histo_el_tight70", "miniiso (Electrons)", 15, 0., 0.3);
    miniiso_histo_el_tight70->SetDirectory(rootdir);
    miniiso_histo_el_tight70->Sumw2();

    TH1F *histo_ht = new TH1F("histo_ht", "HT", 20,0,1000);
    histo_ht->SetDirectory(rootdir);
    histo_ht->Sumw2();

    TH1F *histo_met = new TH1F("histo_met", "MET", 20,0,1000);
    histo_met->SetDirectory(rootdir);
    histo_met->Sumw2();

    TH1F *histo_met_all = new TH1F("histo_met_all", "MET", 20,0,200);
    histo_met_all->SetDirectory(rootdir);
    histo_met_all->Sumw2();

    TH1F *histo_met_all_el = new TH1F("histo_met_all_el", "MET", 20,0,200);
    histo_met_all_el->SetDirectory(rootdir);
    histo_met_all_el->Sumw2();

    TH1F *histo_met_all_mu = new TH1F("histo_met_all_mu", "MET", 20,0,200);
    histo_met_all_mu->SetDirectory(rootdir);
    histo_met_all_mu->Sumw2();

    TH1F *histo_met_lm = new TH1F("histo_met_lm", "MET", 20,0,200);
    histo_met_lm->SetDirectory(rootdir);
    histo_met_lm->Sumw2();

    TH1F *histo_met_lm_el = new TH1F("histo_met_lm_el", "MET", 20,0,200);
    histo_met_lm_el->SetDirectory(rootdir);
    histo_met_lm_el->Sumw2();

    TH1F *histo_met_lm_mu = new TH1F("histo_met_lm_mu", "MET", 20,0,200);
    histo_met_lm_mu->SetDirectory(rootdir);
    histo_met_lm_mu->Sumw2();

    TH1F *histo_met_cr = new TH1F("histo_met_cr", "MET", 20,0,200);
    histo_met_cr->SetDirectory(rootdir);
    histo_met_cr->Sumw2();

    TH1F *histo_met_cr_el = new TH1F("histo_met_cr_el", "MET", 20,0,200);
    histo_met_cr_el->SetDirectory(rootdir);
    histo_met_cr_el->Sumw2();

    TH1F *histo_met_cr_mu = new TH1F("histo_met_cr_mu", "MET", 20,0,200);
    histo_met_cr_mu->SetDirectory(rootdir);
    histo_met_cr_mu->Sumw2();

    TH1F *histo_mt = new TH1F("histo_mt", "MT", 20,0,1000);
    histo_mt->SetDirectory(rootdir);
    histo_mt->Sumw2();

    TH1F *histo_mt_all = new TH1F("histo_mt_all", "MT", 20,0,200);
    histo_mt_all->SetDirectory(rootdir);
    histo_mt_all->Sumw2();

    TH1F *histo_mt_all_el = new TH1F("histo_mt_all_el", "MT", 20,0,200);
    histo_mt_all_el->SetDirectory(rootdir);
    histo_mt_all_el->Sumw2();

    TH1F *histo_mt_all_mu = new TH1F("histo_mt_all_mu", "MT", 20,0,200);
    histo_mt_all_mu->SetDirectory(rootdir);
    histo_mt_all_mu->Sumw2();

    TH1F *histo_mt_lm = new TH1F("histo_mt_lm", "MT", 20,0,200);
    histo_mt_lm->SetDirectory(rootdir);
    histo_mt_lm->Sumw2();

    TH1F *histo_mt_lm_el = new TH1F("histo_mt_lm_el", "MT", 20,0,200);
    histo_mt_lm_el->SetDirectory(rootdir);
    histo_mt_lm_el->Sumw2();

    TH1F *histo_mt_lm_mu = new TH1F("histo_mt_lm_mu", "MT", 20,0,200);
    histo_mt_lm_mu->SetDirectory(rootdir);
    histo_mt_lm_mu->Sumw2();

    TH1F *histo_mt_cr = new TH1F("histo_mt_cr", "MT", 20,0,200);
    histo_mt_cr->SetDirectory(rootdir);
    histo_mt_cr->Sumw2();

    TH1F *histo_mt_cr_el = new TH1F("histo_mt_cr_el", "MT", 20,0,200);
    histo_mt_cr_el->SetDirectory(rootdir);
    histo_mt_cr_el->Sumw2();

    TH1F *histo_mt_cr_mu = new TH1F("histo_mt_cr_mu", "MT", 20,0,200);
    histo_mt_cr_mu->SetDirectory(rootdir);
    histo_mt_cr_mu->Sumw2();

    Double_t dbins_el[] = {0., 0.8, 1.479, 2.5};
    TH2D *histo_mteta_cr_el = new TH2D("histo_mteta_cr_el", "Eta vs MT (electrons)", 20,0.,200.,netabins,dbins_el);
    histo_mteta_cr_el->SetDirectory(rootdir);
    histo_mteta_cr_el->Sumw2();

    Double_t dbins_mu[] = {0., 1.2, 2.1, 2.4};
    TH2D *histo_mteta_cr_mu = new TH2D("histo_mteta_cr_mu", "Eta vs MT (muons)", 20,0.,200.,netabins,dbins_mu);
    histo_mteta_cr_mu->SetDirectory(rootdir);
    histo_mteta_cr_mu->Sumw2();

    TH1F *histo_mt_cr2_el = new TH1F("histo_mt_cr2_el", "MT", 20,0,200);
    histo_mt_cr2_el->SetDirectory(rootdir);
    histo_mt_cr2_el->Sumw2();

    TH1F *histo_mt_cr2_mu = new TH1F("histo_mt_cr2_mu", "MT", 20,0,200);
    histo_mt_cr2_mu->SetDirectory(rootdir);
    histo_mt_cr2_mu->Sumw2();


    TH1F *histo_mt_cr_dphi = new TH1F("histo_mt_cr_dphi", "dphi", 40,0,3.2);
    histo_mt_cr_dphi->SetDirectory(rootdir);
    histo_mt_cr_dphi->Sumw2();
    TH1F *histo_mt_cr_dphi_el = new TH1F("histo_mt_cr_dphi_el", "dphi", 40,0,3.2);
    histo_mt_cr_dphi_el->SetDirectory(rootdir);
    histo_mt_cr_dphi_el->Sumw2();
    TH1F *histo_mt_cr_dphi_mu = new TH1F("histo_mt_cr_dphi_mu", "dphi", 40,0,3.2);
    histo_mt_cr_dphi_mu->SetDirectory(rootdir);
    histo_mt_cr_dphi_mu->Sumw2();

    TH1F *histo_mt_cr_met = new TH1F("histo_mt_cr_met", "met", 20,0,200);
    histo_mt_cr_met->SetDirectory(rootdir);
    histo_mt_cr_met->Sumw2();
    TH1F *histo_mt_cr_met_el = new TH1F("histo_mt_cr_met_el", "met", 20,0,200);
    histo_mt_cr_met_el->SetDirectory(rootdir);
    histo_mt_cr_met_el->Sumw2();
    TH1F *histo_mt_cr_met_mu = new TH1F("histo_mt_cr_met_mu", "met", 20,0,200);
    histo_mt_cr_met_mu->SetDirectory(rootdir);
    histo_mt_cr_met_mu->Sumw2();

    TH1F *histo_mt_cr_pt = new TH1F("histo_mt_cr_pt", "pt", 20,0,200);
    histo_mt_cr_pt->SetDirectory(rootdir);
    histo_mt_cr_pt->Sumw2();
    TH1F *histo_mt_cr_pt_el = new TH1F("histo_mt_cr_pt_el", "pt", 20,0,200);
    histo_mt_cr_pt_el->SetDirectory(rootdir);
    histo_mt_cr_pt_el->Sumw2();
    TH1F *histo_mt_cr_pt_mu = new TH1F("histo_mt_cr_pt_mu", "pt", 20,0,200);
    histo_mt_cr_pt_mu->SetDirectory(rootdir);
    histo_mt_cr_pt_mu->Sumw2();

    TH1F *histo_mt_cr_noiso_el = new TH1F("histo_mt_cr_noiso_el", "MT", 20,0,200);
    histo_mt_cr_noiso_el->SetDirectory(rootdir);
    histo_mt_cr_noiso_el->Sumw2();


    TH1F *histo_mt_cr_noiso_mu = new TH1F("histo_mt_cr_noiso_mu", "MT", 20,0,200);
    histo_mt_cr_noiso_mu->SetDirectory(rootdir);
    histo_mt_cr_noiso_mu->Sumw2();

    TH1F *histo_mt_cr2_noiso_el = new TH1F("histo_mt_cr2_noiso_el", "MT", 20,0,200);
    histo_mt_cr2_noiso_el->SetDirectory(rootdir);
    histo_mt_cr2_noiso_el->Sumw2();

    TH1F *histo_mt_cr2_noiso_mu = new TH1F("histo_mt_cr2_noiso_mu", "MT", 20,0,200);
    histo_mt_cr2_noiso_mu->SetDirectory(rootdir);
    histo_mt_cr2_noiso_mu->Sumw2();

    ///
    TH1F *histo_mt_cr_noiso_dphi = new TH1F("histo_mt_cr_noiso_dphi", "dphi", 40,0,3.2);
    histo_mt_cr_noiso_dphi->SetDirectory(rootdir);
    histo_mt_cr_noiso_dphi->Sumw2();
    TH1F *histo_mt_cr_noiso_dphi_el = new TH1F("histo_mt_cr_noiso_dphi_el", "dphi", 40,0,3.2);
    histo_mt_cr_noiso_dphi_el->SetDirectory(rootdir);
    histo_mt_cr_noiso_dphi_el->Sumw2();
    TH1F *histo_mt_cr_noiso_dphi_mu = new TH1F("histo_mt_cr_noiso_dphi_mu", "dphi", 40,0,3.2);
    histo_mt_cr_noiso_dphi_mu->SetDirectory(rootdir);
    histo_mt_cr_noiso_dphi_mu->Sumw2();

    TH1F *histo_mt_cr_noiso_met = new TH1F("histo_mt_cr_noiso_met", "met", 20,0,200);
    histo_mt_cr_noiso_met->SetDirectory(rootdir);
    histo_mt_cr_noiso_met->Sumw2();
    TH1F *histo_mt_cr_noiso_met_el = new TH1F("histo_mt_cr_noiso_met_el", "met", 20,0,200);
    histo_mt_cr_noiso_met_el->SetDirectory(rootdir);
    histo_mt_cr_noiso_met_el->Sumw2();
    TH1F *histo_mt_cr_noiso_met_mu = new TH1F("histo_mt_cr_noiso_met_mu", "met", 20,0,200);
    histo_mt_cr_noiso_met_mu->SetDirectory(rootdir);
    histo_mt_cr_noiso_met_mu->Sumw2();

    TH1F *histo_mt_cr_noiso_pt = new TH1F("histo_mt_cr_noiso_pt", "pt", 20,0,200);
    histo_mt_cr_noiso_pt->SetDirectory(rootdir);
    histo_mt_cr_noiso_pt->Sumw2();
    TH1F *histo_mt_cr_noiso_pt_el = new TH1F("histo_mt_cr_noiso_pt_el", "pt", 20,0,200);
    histo_mt_cr_noiso_pt_el->SetDirectory(rootdir);
    histo_mt_cr_noiso_pt_el->Sumw2();
    TH1F *histo_mt_cr_noiso_pt_mu = new TH1F("histo_mt_cr_noiso_pt_mu", "pt", 20,0,200);
    histo_mt_cr_noiso_pt_mu->SetDirectory(rootdir);
    histo_mt_cr_noiso_pt_mu->Sumw2();
    ///

    TH1F *histo_mt_cr_hi = new TH1F("histo_mt_cr_hi", "MT", 20,0,200);
    histo_mt_cr_hi->SetDirectory(rootdir);
    histo_mt_cr_hi->Sumw2();

    TH1F *histo_mt_cr_hi_el = new TH1F("histo_mt_cr_hi_el", "MT", 20,0,200);
    histo_mt_cr_hi_el->SetDirectory(rootdir);
    histo_mt_cr_hi_el->Sumw2();

    TH1F *histo_mt_cr_hi_mu = new TH1F("histo_mt_cr_hi_mu", "MT", 20,0,200);
    histo_mt_cr_hi_mu->SetDirectory(rootdir);
    histo_mt_cr_hi_mu->Sumw2();

    TH1F *histo_mt_cr_vh = new TH1F("histo_mt_cr_vh", "MT", 20,0,200);
    histo_mt_cr_vh->SetDirectory(rootdir);
    histo_mt_cr_vh->Sumw2();

    TH1F *histo_mt_cr_vh_el = new TH1F("histo_mt_cr_vh_el", "MT", 20,0,200);
    histo_mt_cr_vh_el->SetDirectory(rootdir);
    histo_mt_cr_vh_el->Sumw2();

    TH1F *histo_mt_cr_vh_mu = new TH1F("histo_mt_cr_vh_mu", "MT", 20,0,200);
    histo_mt_cr_vh_mu->SetDirectory(rootdir);
    histo_mt_cr_vh_mu->Sumw2();

    TH1F *histo_mt_cr_low_el = new TH1F("histo_mt_cr_low_el", "MT", 20,0,200);
    histo_mt_cr_low_el->SetDirectory(rootdir);
    histo_mt_cr_low_el->Sumw2();

    TH1F *histo_mt_cr_low_mu = new TH1F("histo_mt_cr_low_mu", "MT", 20,0,200);
    histo_mt_cr_low_mu->SetDirectory(rootdir);
    histo_mt_cr_low_mu->Sumw2();

    TH1F *histo_mt_cr_high_el = new TH1F("histo_mt_cr_high_el", "MT", 20,0,200);
    histo_mt_cr_high_el->SetDirectory(rootdir);
    histo_mt_cr_high_el->Sumw2();

    TH1F *histo_mt_cr_high_mu = new TH1F("histo_mt_cr_high_mu", "MT", 20,0,200);
    histo_mt_cr_high_mu->SetDirectory(rootdir);
    histo_mt_cr_high_mu->Sumw2();

    TH1F *histo_mt_cr_unw_low_el = new TH1F("histo_mt_cr_unw_low_el", "MT", 20,0,200);
    histo_mt_cr_unw_low_el->SetDirectory(rootdir);
    histo_mt_cr_unw_low_el->Sumw2();

    TH1F *histo_mt_cr_unw_low_mu = new TH1F("histo_mt_cr_unw_low_mu", "MT", 20,0,200);
    histo_mt_cr_unw_low_mu->SetDirectory(rootdir);
    histo_mt_cr_unw_low_mu->Sumw2();

    TH1F *histo_mt_cr_unw_high_el = new TH1F("histo_mt_cr_unw_high_el", "MT", 20,0,200);
    histo_mt_cr_unw_high_el->SetDirectory(rootdir);
    histo_mt_cr_unw_high_el->Sumw2();

    TH1F *histo_mt_cr_unw_high_mu = new TH1F("histo_mt_cr_unw_high_mu", "MT", 20,0,200);
    histo_mt_cr_unw_high_mu->SetDirectory(rootdir);
    histo_mt_cr_unw_high_mu->Sumw2();

    TH1F *histo_pt_mu = new TH1F("histo_pt_mu", "pt mu", 100,0,200);
    histo_pt_mu->SetDirectory(rootdir);
    histo_pt_mu->Sumw2();

    TH1F *histo_pt_mu8 = new TH1F("histo_pt_mu8", "pt mu8", 100,0,200);
    histo_pt_mu8->SetDirectory(rootdir);
    histo_pt_mu8->Sumw2();

    TH1F *histo_pt_mu17 = new TH1F("histo_pt_mu17", "pt mu17", 100,0,200);
    histo_pt_mu17->SetDirectory(rootdir);
    histo_pt_mu17->Sumw2();

    TH1F *histo_pt_mu24 = new TH1F("histo_pt_mu24", "pt mu24", 100,0,200);
    histo_pt_mu24->SetDirectory(rootdir);
    histo_pt_mu24->Sumw2();

    TH1F *histo_pt_mu34 = new TH1F("histo_pt_mu34", "pt mu34", 100,0,200);
    histo_pt_mu34->SetDirectory(rootdir);
    histo_pt_mu34->Sumw2();

    TH1F *histo_pt_el = new TH1F("histo_pt_el", "pt el", 100,0,200);
    histo_pt_el->SetDirectory(rootdir);
    histo_pt_el->Sumw2();

    TH1F *histo_pt_el8 = new TH1F("histo_pt_el8", "pt el8", 100,0,200);
    histo_pt_el8->SetDirectory(rootdir);
    histo_pt_el8->Sumw2();

    TH1F *histo_pt_el12 = new TH1F("histo_pt_el12", "pt el12", 100,0,200);
    histo_pt_el12->SetDirectory(rootdir);
    histo_pt_el12->Sumw2();

    TH1F *histo_pt_el17 = new TH1F("histo_pt_el17", "pt el17", 100,0,200);
    histo_pt_el17->SetDirectory(rootdir);
    histo_pt_el17->Sumw2();

    TH1F *histo_pt_el24 = new TH1F("histo_pt_el24", "pt el24", 100,0,200);
    histo_pt_el24->SetDirectory(rootdir);
    histo_pt_el24->Sumw2();

    TH1F *histo_pt_el34 = new TH1F("histo_pt_el34", "pt el34", 100,0,200);
    histo_pt_el34->SetDirectory(rootdir);
    histo_pt_el34->Sumw2();

    TH1F *njets40_histo = new TH1F("njets40_histo", "Njets with pT > 40 GeV", 5,0,5);
    njets40_histo->SetDirectory(rootdir);
    njets40_histo->Sumw2();

    TH1D *Nt_nvtx_histo_e = new TH1D("Nt_nvtx_histo_e", "", 20, 0, 60);
    Nt_nvtx_histo_e->SetDirectory(rootdir);
    Nt_nvtx_histo_e->Sumw2();

    TH1D *Nt_nvtx_histo_mu = new TH1D("Nt_nvtx_histo_mu", "", 20, 0, 60);
    Nt_nvtx_histo_mu->SetDirectory(rootdir);
    Nt_nvtx_histo_mu->Sumw2();

    TH1D *Nl_cone_nvtx_histo_e = new TH1D("Nl_cone_nvtx_histo_e", "", 20, 0, 60);
    Nl_cone_nvtx_histo_e->SetDirectory(rootdir);
    Nl_cone_nvtx_histo_e->Sumw2();

    TH1D *Nl_cone_nvtx_histo_mu = new TH1D("Nl_cone_nvtx_histo_mu", "", 20, 0, 60);
    Nl_cone_nvtx_histo_mu->SetDirectory(rootdir);
    Nl_cone_nvtx_histo_mu->Sumw2();

    TH2D *hdenom = new TH2D("hdenom","", 20,25,125,5,0,2.5);
    hdenom->SetDirectory(rootdir);
    hdenom->Sumw2();
  
    //----------------------

    //e determination
    float Nt = 0.;  //# of tight leptons
    float Nl = 0.;  //# of loose leptons
    float e = 0.;   //rate = Nt/(Nl)
    float Nt_e = 0.;
    float Nl_e = 0.; 
    float e_e = 0.;
    float Nt_mu = 0.;  
    float Nl_mu = 0.; 
    float e_mu = 0.;
    //----------------
    float Bs_e = 0.;
    float notBs_e = 0.;
    float Bs_mu = 0.;
    float notBs_mu = 0.;

    //-------- JEC application


    signal(SIGINT, [](int){
            cout << "SIGINT Caught, stopping after current event" << endl;
            STOP_REQUESTED=true;
            });
    STOP_REQUESTED=false;


    // Loop over events to Analyze
    unsigned int nEventsTotal = 0;
    unsigned int nEventsChain = chain->GetEntries();
    if (not quiet) std::cout <<  " nEventsChain: " << nEventsChain <<  std::endl;
    if( nEvents >= 0 ) nEventsChain = nEvents;
    TObjArray *listOfFiles = chain->GetListOfFiles();
    TIter fileIter(listOfFiles);
    TFile *currentFile = 0;

    tqdm bar;
    // bar.set_theme_braille();

    // File Loop
    while ( (currentFile = (TFile*)fileIter.Next()) ) {
        if (STOP_REQUESTED) break;


        // Get File Content
        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("t");
        if(fast) TTreeCache::SetLearnEntries(10);
        if(fast) tree->SetCacheSize(128*1024*1024);
        lepton_tree_obj.Init(tree);

        bool isSyncFile = TString(currentFile->GetTitle()).Contains("Sync");

        // Apply JEC
        bool isDataFromFileName = TString(currentFile->GetTitle()).Contains("Run201") || 
            TString(currentFile->GetTitle()).Contains("DoubleMu") || TString(currentFile->GetTitle()).Contains("DoubleEG") || TString(currentFile->GetTitle()).Contains("EGamma") ||
            TString(currentFile->GetTitle()).Contains("Data");
        bool isDoubleMuon = TString(currentFile->GetTitle()).Contains("DoubleMu");
        bool isQCDMu = TString(currentFile->GetTitle()).Contains("QCD_Mu");
        bool isQCDEl = TString(currentFile->GetTitle()).Contains("QCD_EM") || TString(currentFile->GetTitle()).Contains("QCD_bcToE");
        bool isTTbar = TString(currentFile->GetTitle()).Contains("TTJets") || TString(currentFile->GetTitle()).Contains("output_tt_") || TString(currentFile->GetTitle()).Contains("TTBAR");
        bool isDY = TString(currentFile->GetTitle()).Contains("DY");
        bool isWjets = TString(currentFile->GetTitle()).Contains("WJets");

        auto filename = TString(currentFile->GetTitle());
        auto tokens = filename.Tokenize("/");
        auto basename = ((TObjString*)(tokens->At(tokens->GetEntries()-1)))->String().Data();
        bar.set_label(basename);

        if (not quiet) cout << " => File: " << filename << endl;

        // Loop over Events in current file   //ACTUALLY A LEPTON "EVENT" LOOP
        if( nEventsTotal >= nEventsChain ) continue;
        unsigned int nEventsTree = tree->GetEntriesFast();
        for( unsigned int event = 0; event < nEventsTree; ++event) {

            if (STOP_REQUESTED) break;

            // Get Event Content
            if( nEventsTotal >= nEventsChain ) continue;
            tree->LoadTree(event);
            lepton_tree_obj.GetEntry(event);
            ++nEventsTotal;

            // Progress
            // LeptonTree::progress( nEventsTotal, nEventsChain );
            if (not quiet) bar.progress(nEventsTotal, nEventsChain);

            if (debug) cout << "event=" << evt_event() << " run=" << evt_run() << endl;

            //cout << "pt=" << p4().pt() << " iso=" << RelIso03EA() << endl;
            if (debug) cout << "lepp4=" << p4() << " pt=" << p4().pt() << " eta=" << p4().eta() << " phi=" << p4().phi() << " jetp4=" << jet_close_lep() << endl;


            bool isEWK = false;
            if (isWjets || isDY || isTTbar) isEWK = true;

            bool isData = evt_isRealData();
            bool noMCMatch = false;
            if (isData || isEWK) noMCMatch = true;

            if (!isData && requireMCMatch) noMCMatch = false;

            //reject electrons from DoubleMu and muons from DoubleEG
            if (debug) cout << "check dataset" << endl;
            if (isData) {
                if ( isDoubleMuon && abs(id())!=13) continue;
                if (!isDoubleMuon && abs(id())!=11) continue;
            }

            if (doAbove25) {
                if (p4().pt() < 25.) continue;
                if (coneCorrPt() < 25.) continue;
            }

            if (doMu18Only) {
                if (p4().pt() < 18.) continue;
                if (coneCorrPt() < 18.) continue;
                anyPt = true; // don't require raw>25 for trigger
            }

            if (doAbove18) {
                if (p4().pt() < 18.) continue;
                if (coneCorrPt() < 18.) continue;
            }


            if (isData==0 && isEWK==0) {
                if (isQCDMu && abs(id())!=13) continue;
                if (isQCDEl && abs(id())!=11) continue;
            }


            if (!bypassTriggers) {
                //trigger selection
                if (abs(id())==11) {
                    if (useIsoTrigs) {
                        // if (year == 2016) {
                        //     if (HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30()<=0 && HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30()<=0) continue;
                        // } else {
                        //     if (HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30()<=0 && HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30()<=0) continue;
                        // }
                        if (false) {
                            if (HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30()<=0 && HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30()<=0) continue;
                        } else {
                            if (HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30()<=0 && HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30()<=0) continue;
                        }
                    } else {
                        if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30()<=0 && HLT_Ele17_CaloIdM_TrackIdM_PFJet30()<=0) continue;
                    }
                }
                if (abs(id())==13) {
                    if (useIsoTrigs) {
                        if (HLT_Mu8_TrkIsoVVL()<=0 && HLT_Mu17_TrkIsoVVL()<=0) continue;
                    } else {
                        if (HLT_Mu8()<=0 && HLT_Mu17()<=0) continue;
                    }
                }	
            }


            float ht = ht_SS();
            int njets40 = njets_recoil();
            bool jetptcut = (njets40 > 0) && (ht > 40);
            int nbtags = 0;

            if (!requireTwoJets) {
                if (!jetptcut) continue;
            } else {
                if (njets() < 2) continue;
            }

            if (requireTag) {
                if (tag_p4().pt() < 25) continue;
            }

            if (twoFO) {
                if (nFOs_SS() < 2) continue;
            } else if (!allowMoreFO) {
                if (nFOs_SS() > 1) continue;
            }

            if (singleHadronic) {
                if (nhadronicW() != 1) continue;
            }

            if (doubleHadronic) {
                if (nhadronicW() != 2) continue;
            }

            if (abs(id()) == 11 && p4().pt() < 15.) continue;
            if (abs(id()) == 13 && p4().pt() < 10.) continue;
            if (abs(id()) == 11 && fabs(p4().eta()) > 2.5) continue;
            if (abs(id()) == 13 && fabs(p4().eta()) > 2.4) continue;
            if (fabs(ip3d()/ip3derr())>4. ) continue;

            float weight = 1;
            if (!isDataFromFileName) {
                weight *= getLumi(year)*scale1fb();
                if (not noPUweight) weight *= getTruePUw(year, trueNumInt());

                // Soften super high weight events
                if (weight > 10000) {
                    weight *= 0.3;
                }

                if (year == 2016) {
                    // to match https://github.com/cmstas/SSAnalysis/blob/master/FakeRate/measurement_region/ScanChain.C
                    // take MuPt5 (20toInf) below pT15 and other bins above
                    if (isQCDMu) {
                        if (p4().pt()<15. &&  scale1fb() > 13.6 && scale1fb() < 13.9 ) continue;  //take only Mu15 above pT=15
                        if (p4().pt()>15. && (scale1fb() < 13.6 || scale1fb() > 13.9)) continue;  //take only Mu5 below pT=15
                        if (scale1fb() < 1.03 || scale1fb() > 600.) continue; //avoid extreme ranges and weights
                    }
                }

            }

            LorentzVector closejet = close_jet_v5();
            float ptrel =  ptrelv1();
            float coneptcorr = coneCorrPt() / p4().pt() - 1;
            if (noConeCorr) {
                coneptcorr = 0.;
            }
            float closejetpt = closejet.pt(); // V5
            float ptratio = ptratio_v5();


            bool passes_low = false;
            bool passes_high = false;

            //check prescales, apply cuts on the pT range depending on the trigger
            int prescale = -1;
            if (!bypassTriggers) {
                if (abs(id())==11) {
                    if (useIsoTrigs) {

                        if (false) {

                            if (p4().pt() >= 10 && p4().pt() < 25 && HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30()>0) { 
                                passes_low = true;
                                prescale = HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30();
                                if (isData) prescale *= sf_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30;
                            }
                            if ((anyPt || p4().pt() >= 25) && HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30()>0) {
                                passes_high = true;
                                prescale = HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30();
                                if (isData) prescale *= sf_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30;
                            }

                        } else {

                            if (p4().pt() >= 10 && p4().pt() < 25 && HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30()>0) { 
                                passes_low = true;
                                prescale = HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30();
                                if (isData) prescale *= sf_HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30;
                            }
                            if ((anyPt || p4().pt() >= 25) && HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30()>0) {
                                passes_high = true;
                                prescale = HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30();
                                if (isData) prescale *= sf_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30;
                            }

                        }

                        if (prescale>0) weight *= prescale;
                        else continue;	  
                    } else {
                        if (p4().pt() >= 10 && p4().pt() < 25 && HLT_Ele8_CaloIdM_TrackIdM_PFJet30()>0) { 
                            passes_low = true;
                            prescale = HLT_Ele8_CaloIdM_TrackIdM_PFJet30();
                            if (isData) prescale *= sf_HLT_Ele8_CaloIdM_TrackIdM_PFJet30;
                        }
                        if ((anyPt || p4().pt() >= 25) && HLT_Ele17_CaloIdM_TrackIdM_PFJet30()>0) {
                            passes_high = true;
                            prescale = HLT_Ele17_CaloIdM_TrackIdM_PFJet30();
                            if (isData) prescale *= sf_HLT_Ele17_CaloIdM_TrackIdM_PFJet30;
                        }
                        if (prescale>0) weight *= prescale;
                        else continue;	  
                    }
                }
                if (abs(id())==13) {	
                    // use mu8+mu17
                    if (useIsoTrigs) {
                        if (p4().pt()>=10 && p4().pt()<25 && HLT_Mu8_TrkIsoVVL()>0) { 
                            passes_low = true;
                            prescale = HLT_Mu8_TrkIsoVVL();
                            if (isData) prescale *= sf_HLT_Mu8_TrkIsoVVL;
                        }

                        if ((anyPt || p4().pt()>=25) && HLT_Mu17_TrkIsoVVL()>0) {
                            passes_high = true;
                            prescale = HLT_Mu17_TrkIsoVVL();
                            if (isData) prescale *= sf_HLT_Mu17_TrkIsoVVL;
                        }

                        if (prescale>0) weight *= prescale;
                        else continue;
                    } else {
                        if (p4().pt()>=10 && p4().pt()<25 && HLT_Mu8()>0) {
                            passes_low = true;
                            prescale = HLT_Mu8();
                            if (isData) prescale *= sf_HLT_Mu8;
                        }

                        if ((anyPt || p4().pt() >= 25) && HLT_Mu17()>0) {
                            passes_high = true;
                            prescale = HLT_Mu17();
                            if (isData) prescale *= sf_HLT_Mu17;
                        }

                        if (prescale>0) weight *= prescale;
                        else continue;
                    }
                }
            }

            bool passId = passes_SS_tight_v6();
            bool passFO = passes_SS_fo_v6();
            bool passId_noiso = passes_SS_tight_noiso_v6();
            bool passFO_noiso = passes_SS_fo_noiso_v6();
            if (useLooseEMVA && abs(id())==11) {
                bool passHltCuts = isTriggerSafenoIso();
                if (useIsoTrigs) {
                    // if (!passIsolatedFO(id(),etaSC(),mva_25ns(),p4().pt())) continue;
                    passHltCuts = isTriggerSafe();

                }
                passFO = passHltCuts && passes_SS_fo_looseMVA_v6();
                passFO_noiso = passHltCuts && passes_SS_fo_looseMVA_noiso_v6();
            }

            if (!passFO) continue;

            float evt_met = evt_corrMET();
            float evt_metPhi = evt_corrMETPhi();
            float evt_mt = mt();

            if (passId_noiso && !passId) {
                if (evt_met > MTCR_MET_CUT && p4().pt()>MTCR_PT_CUT) {
                    if (abs(id())==11) {
                        histo_mt_cr_noiso_el->Fill( std::min(evt_mt,float(200.)), weight );
                        if (p4().pt()>MTCR_PT_CUT) histo_mt_cr2_noiso_el->Fill( std::min(evt_mt,float(200.)), weight );
                        histo_mt_cr_noiso_pt_el->Fill( p4().pt(), weight );
                        histo_mt_cr_noiso_met_el->Fill( evt_met, weight );
                        histo_mt_cr_noiso_dphi_el->Fill( DeltaPhi(p4().phi(),evt_metPhi), weight );
                    }
                    if (abs(id())==13) {
                        histo_mt_cr_noiso_mu->Fill( std::min(evt_mt,float(200.)), weight );
                        if (p4().pt()>MTCR_PT_CUT) histo_mt_cr2_noiso_mu->Fill( std::min(evt_mt,float(200.)), weight );
                        histo_mt_cr_noiso_pt_mu->Fill( p4().pt(), weight );
                        histo_mt_cr_noiso_met_mu->Fill( evt_met, weight );
                        histo_mt_cr_noiso_dphi_mu->Fill( DeltaPhi(p4().phi(),evt_metPhi), weight );
                    }
                }
            }

            if (passId) {
                //mt control region
                if (evt_met > 30. && p4().pt()>30) {
                    float myweight = (isDataFromFileName ? weight : weight*leptonScaleFactor(year, abs(id()), p4().pt(), p4().eta(), ht));
                    // float myweight = weight;
                    histo_mt_all->Fill( std::min(evt_mt,float(200.)), myweight );
                    if (abs(id())==11) histo_mt_all_el->Fill( std::min(evt_mt,float(200.)), myweight );
                    if (abs(id())==13) histo_mt_all_mu->Fill( std::min(evt_mt,float(200.)), myweight );
                }
                // if (evt_met < 20.) {
                //     histo_mt_lm->Fill( std::min(evt_mt,float(200.)), weight );
                //     if (abs(id())==11) histo_mt_lm_el->Fill( std::min(evt_mt,float(200.)), weight );
                //     if (abs(id())==13) histo_mt_lm_mu->Fill( std::min(evt_mt,float(200.)), weight );
                // }
                if (evt_met > MTCR_MET_CUT && p4().pt()>MTCR_PT_CUT) {
                    histo_mt_cr->Fill( std::min(evt_mt,float(200.)), weight );
                    histo_mt_cr_pt->Fill( p4().pt(), weight );
                    histo_mt_cr_met->Fill( evt_met, weight );
                    histo_mt_cr_dphi->Fill( DeltaPhi(p4().phi(),evt_metPhi), weight );
                    float myweight = (isDataFromFileName ? weight : weight*leptonScaleFactor(year, abs(id()), p4().pt(), p4().eta(), ht));
                    // float myweight = weight;
                    if (abs(id())==11) {
                        histo_mt_cr_el->Fill( std::min(evt_mt,float(200.)), myweight );
                        histo_mteta_cr_el->Fill(std::min(evt_mt,float(200.)), getEta(fabs(p4().eta()),ht,false), myweight);
                        if (p4().pt()>MTCR_PT_CUT) histo_mt_cr2_el->Fill( std::min(evt_mt,float(200.)), myweight );
                        histo_mt_cr_pt_el->Fill( p4().pt(), myweight );
                        histo_mt_cr_met_el->Fill( evt_met, myweight );
                        histo_mt_cr_dphi_el->Fill( DeltaPhi(p4().phi(),evt_metPhi), myweight );
                    }
                    if (abs(id())==13) {
                        histo_mt_cr_mu->Fill( std::min(evt_mt,float(200.)), myweight );
                        histo_mteta_cr_mu->Fill(std::min(evt_mt,float(200.)), getEta(fabs(p4().eta()),ht,false), myweight);
                        if (p4().pt()>MTCR_PT_CUT) histo_mt_cr2_mu->Fill( std::min(evt_mt,float(200.)), myweight );
                        histo_mt_cr_pt_mu->Fill( p4().pt(), myweight );
                        histo_mt_cr_met_mu->Fill( evt_met, myweight );
                        histo_mt_cr_dphi_mu->Fill( DeltaPhi(p4().phi(),evt_metPhi), myweight );
                    }

                    if (abs(id())==11 && passes_low) histo_mt_cr_low_el->Fill( std::min(evt_mt,float(200.)), weight );
                    if (abs(id())==13 && passes_low) histo_mt_cr_low_mu->Fill( std::min(evt_mt,float(200.)), weight );
                    if (abs(id())==11 && passes_high) histo_mt_cr_high_el->Fill( std::min(evt_mt,float(200.)), weight );
                    if (abs(id())==13 && passes_high) histo_mt_cr_high_mu->Fill( std::min(evt_mt,float(200.)), weight );

                    if (abs(id())==11 && passes_low) histo_mt_cr_unw_low_el->Fill( std::min(evt_mt,float(200.)), 1 );
                    if (abs(id())==13 && passes_low) histo_mt_cr_unw_low_mu->Fill( std::min(evt_mt,float(200.)), 1 );
                    if (abs(id())==11 && passes_high) histo_mt_cr_unw_high_el->Fill( std::min(evt_mt,float(200.)), 1 );
                    if (abs(id())==13 && passes_high) histo_mt_cr_unw_high_mu->Fill( std::min(evt_mt,float(200.)), 1 );
                }
                if (evt_met > 50.) {
                    histo_mt_cr_hi->Fill( std::min(evt_mt,float(200.)), weight );
                    if (abs(id())==11) histo_mt_cr_hi_el->Fill( std::min(evt_mt,float(200.)), weight );
                    if (abs(id())==13) histo_mt_cr_hi_mu->Fill( std::min(evt_mt,float(200.)), weight );
                }
                if (evt_met > 80.) {
                    histo_mt_cr_vh->Fill( std::min(evt_mt,float(200.)), weight );
                    if (abs(id())==11) histo_mt_cr_vh_el->Fill( std::min(evt_mt,float(200.)), weight );
                    if (abs(id())==13) histo_mt_cr_vh_mu->Fill( std::min(evt_mt,float(200.)), weight );
                }
            }

            if (doHighMET) {
                if (evt_met < 50) continue;
            } else {
                if (!bypassLowMET) {
                    if( !(evt_met < 20. && evt_mt < 20) ) {
                        continue;
                    }
                }
            }

            if (isData && passFO) {
                if (abs(id())==11) {
                    histo_pt_el->Fill(p4().pt(), weight );
                }
                if (abs(id())==13) {
                    histo_pt_mu->Fill(p4().pt(), weight );	  
                }
            } 


            //------------------------------------------------------------------------------------------
            //---------------------------------Find e = f(const)---------------------------------------
            //------------------------------------------------------------------------------------------

            //Find ratio of nonprompt leps passing tight to nonprompt leps passing at least loose.  This is the fake rate 
            // Use lep_passes_id to see if num.  Use FO to see if Fakable Object (denom)
            //Calculate e=Nt/(Nl) where l->loose  (as opposed to loose-not-tight).

            //Using gen level info to see if prompt -> no prompt contamination in measurement region
            //everything else is RECO (p4, id, passes_id, FO, etc.)

            if( noMCMatch || (isFake() && (doBonly==0 || motherID() == -1) && (doConly==0 || motherID() == -2) && (doLightonly==0 || motherID() == 0) ) )  //if lep is nonprompt
            {

                if( abs( id() ) == 11 ) //it's an el
                {
                    // std::cout <<  " noMCMatch: " << noMCMatch <<  " motherID(): " << motherID() <<  " passFO: " << passFO <<  " passId: " << passId <<  std::endl;
                    if( passId )  //if el is tight
                    { 
                        Nt = Nt + weight;
                        Nt_e = Nt_e + weight;
                    }
                    if( passFO )
                    {
                        Nl = Nl + weight;     //l now means loose, as opposed to loose-not-tight
                        Nl_e = Nl_e + weight;
                    }
                }

                if( abs( id() ) == 13 ) //it's a mu
                {
                    if( passId )  //if mu is tight
                    { 
                        Nt = Nt + weight;
                        Nt_mu = Nt_mu + weight;
                    }
                    if( passFO )
                    {
                        Nl = Nl + weight;     //l now means loose, as opposed to loose-not-tight
                        Nl_mu = Nl_mu + weight;
                    }
                }
            } 

            //------------------------------------------------------------------------------------------
            //---------------------------------Find e = f(Pt,eta)---------------------------------------
            //------------------------------------------------------------------------------------------

            //Find ratio of nonprompt leps passing tight to nonprompt leps passing at least loose.  This is the fake rate 
            // Use lep_passes_id to see if num.  Use FO to see if Fakable Object (denom)
            //Calculate e=Nt/(Nl) where l->loose  (as opposed to loose-not-tight).

            //Using gen level info to see if prompt -> no prompt contamination in measurement region
            //everything else is RECO (p4, id, passes_id, FO, etc.)


            if( noMCMatch || (isFake() && (doBonly==0 || motherID() == -1) && (doConly==0 || motherID() == -2) && (doLightonly==0 || motherID() == 0) ) )  //if el is nonprompt (GEN info)
            {

                if (passFO) {
                    histo_ht->Fill( std::min(ht,float(1000.)) );
                    histo_met->Fill( std::min(evt_met,float(1000.)) );
                    histo_mt->Fill( std::min(evt_mt,float(1000.)) );

                    // if( abs( id() ) == 11 ) pTrelvsIso_histo_el->Fill( std::min(RelIso03EA(),float(0.99)), std::min(ptrel,float(29.9)) );
                    // if( abs( id() ) == 13 ) pTrelvsIso_histo_mu->Fill( std::min(RelIso03EA(),float(0.99)), std::min(ptrel,float(29.9)) );
                    if( abs( id() ) == 11 ) pTrel_histo_el->Fill( std::min(ptrel,float(29.9)) );
                    if( abs( id() ) == 13 ) pTrel_histo_mu->Fill( std::min(ptrel,float(29.9)) );
                    if( abs( id() ) == 11 ) pTratio_histo_el->Fill( std::min(ptratio,float(29.9)) );
                    if( abs( id() ) == 13 ) pTratio_histo_mu->Fill( std::min(ptratio,float(29.9)) );
                    if( abs( id() ) == 11 ) {
                        miniiso_histo_el->Fill( std::min(miniiso(),float(0.29)) );
                        if (getPt(p4().pt()*(1+coneptcorr),false) > 70.) {
                            miniiso_histo_el_loose70->Fill( std::min(miniiso(),float(0.29)) );
                            if (passId) {
                                miniiso_histo_el_tight70->Fill( std::min(miniiso(),float(0.29)) );
                            }
                        }
                    }
                    if( abs( id() ) == 13 ) miniiso_histo_mu->Fill( std::min(miniiso(),float(0.29)) );
                }

                if( abs( id() ) == 11 ) // it's an el
                {
                    if( passId )  //if el is tight
                    { 
                        //uncorrected and cone corrected FR
                        Nt_histo_e->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),ht,false), weight);   //
                        Nt_fine_histo_e->Fill(p4().pt(), fabs(p4().eta()), weight);   //
                        Nt_conevsraw_histo_e->Fill(p4().pt(),getPt(p4().pt(),false), weight);   //  cone vs raw pT

                        Nt_nvtx_histo_e->Fill(nvtx(), weight);   //

                        //jet corrected FR
                        Nt_jet_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
                        if (p4().pt()>25.) Nt_jet_highpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
                        else Nt_jet_lowpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
                    }

                    if( passFO )  //if el is FO
                    {
                        //not corrected FR
                        Nl_histo_e->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),ht,false), weight);   //  <-- loose (as opposed to l!t)
                        //cone corrected FR
                        if( passId ) Nl_cone_histo_e->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),ht,false), weight);   //  <-- loose (as opposed to l!t)
                        else Nl_cone_histo_e->Fill(getPt(p4().pt()*(1+coneptcorr),false), getEta(fabs(p4().eta()),ht,false), weight);

                        Nl_conevsraw_histo_e->Fill(p4().pt(),getPt(p4().pt()*(1+coneptcorr),false), weight);   //  cone vs raw pT

                        if( passId ) Nl_fine_cone_histo_e->Fill(p4().pt(), fabs(p4().eta()), weight);   //  <-- loose (as opposed to l!t)
                        else Nl_fine_cone_histo_e->Fill(p4().pt()*(1+coneptcorr), fabs(p4().eta()), weight);

                        Nl_cone_nvtx_histo_e->Fill(nvtx(), weight);

                        //jet corrected FR
                        Nl_jet_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
                        if (p4().pt()>25.) Nl_jet_highpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
                        else Nl_jet_lowpt_histo_e->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);

                        // if (isSyncFile) {
                        //     cout << Form("Electron FO raw pt=%6.2f corr pt=%6.2f eta=%5.2f miniiso=%.2f ptratio=%.2f ptrel=%5.2f mva=%5.2f isNum=%1i met=%5.2f mt=%5.2f event %i",
                        //             p4().pt(),p4().pt()*(1+coneptcorr),p4().eta(),miniiso(),ptratio,ptrel,mva_25ns(),passId,evt_met,evt_mt,(int)evt_event()) << endl;
                        // }

                        njets40_histo->Fill(njets40, weight);

                        if (noMCMatch==0 && doBonly==0 && doConly==0 && doLightonly==0) //abundance doesn't make sense otherwise
                        {
                            if(motherID()==-1){
                                NBs_BR_histo_e ->Fill(nbtags, weight);
                                Bs_e = Bs_e + weight;
                            }
                            else if(motherID()==-2 || motherID()==0){
                                NnotBs_BR_histo_e ->Fill(nbtags, weight);
                                notBs_e = notBs_e + weight;
                            }
                        }
                    }
                }
                if( abs( id() ) == 13 ) // it's a mu
                {
                    if( passId )  //if mu is tight
                    { 
                        //uncorrected and cone corrected FR
                        Nt_histo_mu->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),ht,false), weight);   //
                        Nt_fine_histo_mu->Fill(p4().pt(), fabs(p4().eta()), weight);   //
                        Nt_conevsraw_histo_mu->Fill(p4().pt(),getPt(p4().pt(),false), weight);   //  cone vs raw pT

                        Nt_nvtx_histo_mu->Fill(nvtx(), weight);   //

                        //jet corrected FR
                        Nt_jet_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
                        if (p4().pt()>25.) Nt_jet_highpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
                        else Nt_jet_lowpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
                    }

                    if( passFO )  //if mu is FO
                    {
                        //not corrected FR
                        Nl_histo_mu->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),ht,false), weight);   //  <-- loose (as opposed to l!t)
                        //cone corrected FR
                        if( passId ) Nl_cone_histo_mu->Fill(getPt(p4().pt(),false), getEta(fabs(p4().eta()),ht,false), weight);   //  <-- loose (as opposed to l!t)
                        else Nl_cone_histo_mu->Fill(getPt(p4().pt()*(1+coneptcorr),false), getEta(fabs(p4().eta()),ht,false), weight);

                        Nl_conevsraw_histo_mu->Fill(p4().pt(),getPt(p4().pt()*(1+coneptcorr),false), weight);   //  cone vs raw pT

                        auto pt = getPt(p4().pt()*(1+coneptcorr),false);
                        auto eta = getEta(fabs(p4().eta()),ht,false);

                        // if (pt > 35 && pt < 50 && eta < 2.1 && eta > 1.2) {
                        //     cout << Form("%1llu %7.3f %7.3f %6.3f %6.3f %6.3f %6.3f %1i %2i %6.3f %6.3f %2.4f %7.3f %2i",
                        //             (unsigned long long)evt_event() , p4().pt(),pt,eta,miniiso(),p4().pt()/closejetpt,ptrel,passId,motherID(),evt_met,evt_mt,weight,
                        //             tag_p4().pt(),hyp_class()
                        //             ) << endl;
                        // }

                        if( passId ) Nl_fine_cone_histo_mu->Fill(p4().pt(), fabs(p4().eta()), weight);   //  <-- loose (as opposed to l!t)
                        else Nl_fine_cone_histo_mu->Fill(p4().pt()*(1+coneptcorr), fabs(p4().eta()), weight);

                        hdenom->Fill(p4().pt()*(1+coneptcorr), fabs(p4().eta()), weight);

                        Nl_cone_nvtx_histo_mu->Fill(nvtx(), weight);

                        //jet corrected FR
                        Nl_jet_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
                        if (p4().pt()>25.) Nl_jet_highpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);
                        else Nl_jet_lowpt_histo_mu->Fill(getPt(closejetpt,false), getEta(fabs(p4().eta()),ht,false), weight);

                        if (isSyncFile) {

                            cout << Form("%1llu %7.3f %7.3f %6.3f %6.3f %6.3f %6.3f %1i %6.3f %6.3f %6.0f",(unsigned long long)evt_event() , p4().pt(),p4().pt()*(1+coneptcorr),p4().eta(),miniiso(),ptratio,ptrel,passId,evt_met,evt_mt,weight) << endl;
                        }

                        njets40_histo->Fill(njets40, weight);

                        if (noMCMatch==0 && doBonly==0 && doConly==0 && doLightonly==0) //abundance doesn't make sense otherwise
                        {
                            if(motherID()==-1){
                                NBs_BR_histo_mu ->Fill(nbtags, weight);
                                Bs_mu = Bs_mu + weight;
                            }
                            else if(motherID()==-2 || motherID()==0){
                                NnotBs_BR_histo_mu ->Fill(nbtags, weight);
                                notBs_mu = notBs_mu + weight;
                            }
                        }
                    }
                }
            } 

            //---------------------------------------------------------------------------------------------------------------------------

        }//end event loop

        // Clean Up
        delete tree;
        file->Close();
        delete file;
    }
    if ( nEventsChain != nEventsTotal ) {
        cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
    }

    e = Nt/(Nl);
    e_e = Nt_e/(Nl_e);
    e_mu = Nt_mu/(Nl_mu);

    if (not quiet) {
        cout<<"\nReco: "<<"Nt = "<<Nt<<", Nl = "<<Nl<<", e ="<<e<<endl;
        cout<<"\nReco (el): "<<"Nt = "<<Nt_e<<", Nl = "<<Nl_e<<", e ="<<e_e<<endl;
        cout<<"\nReco (mu): "<<"Nt = "<<Nt_mu<<", Nl = "<<Nl_mu<<", e ="<<e_mu<<endl<<endl;
        cout<<"\nAve B abundance (els)= "<<Bs_e/(Bs_e + notBs_e)<<endl;
        cout<<"Ave B abundance (mus)= "<<Bs_mu/(Bs_mu + notBs_mu)<<endl;
    }

    //Histograms
    // TH2D *rate_histo = (TH2D*) Nt_histo->Clone("rate_histo");
    TH2D *rate_histo_e = (TH2D*) Nt_histo_e->Clone("rate_histo_e");
    TH2D *rate_histo_mu = (TH2D*) Nt_histo_mu->Clone("rate_histo_mu");
    TH2D *rate_cone_histo_e = (TH2D*) Nt_histo_e->Clone("rate_cone_histo_e");
    TH2D *rate_cone_histo_mu = (TH2D*) Nt_histo_mu->Clone("rate_cone_histo_mu");
    TH2D *rate_fine_cone_histo_e = (TH2D*) Nt_fine_histo_e->Clone("rate_fine_cone_histo_e");
    TH2D *rate_fine_cone_histo_mu = (TH2D*) Nt_fine_histo_mu->Clone("rate_fine_cone_histo_mu");
    TH2D *rate_jet_histo_e = (TH2D*) Nt_jet_histo_e->Clone("rate_jet_histo_e");
    TH2D *rate_jet_histo_mu = (TH2D*) Nt_jet_histo_mu->Clone("rate_jet_histo_mu");
    TH2D *rate_jet_highpt_histo_e = (TH2D*) Nt_jet_highpt_histo_e->Clone("rate_jet_highpt_histo_e");
    TH2D *rate_jet_highpt_histo_mu = (TH2D*) Nt_jet_highpt_histo_mu->Clone("rate_jet_highpt_histo_mu");
    TH2D *rate_jet_lowpt_histo_e = (TH2D*) Nt_jet_lowpt_histo_e->Clone("rate_jet_lowpt_histo_e");
    TH2D *rate_jet_lowpt_histo_mu = (TH2D*) Nt_jet_lowpt_histo_mu->Clone("rate_jet_lowpt_histo_mu");
    TH1F *total_BR_histo_e = (TH1F*) NBs_BR_histo_e->Clone("total_BR_histo_e");
    TH1F *total_BR_histo_mu = (TH1F*) NBs_BR_histo_mu->Clone("total_BR_histo_mu");

    TH1D *rate_cone_nvtx_histo_e = (TH1D*) Nt_nvtx_histo_e->Clone("rate_cone_nvtx_histo_e");
    TH1D *rate_cone_nvtx_histo_mu = (TH1D*) Nt_nvtx_histo_mu->Clone("rate_cone_nvtx_histo_mu");
    rate_cone_nvtx_histo_e->Divide(rate_cone_nvtx_histo_e,Nl_cone_nvtx_histo_e);
    rate_cone_nvtx_histo_mu->Divide(rate_cone_nvtx_histo_mu,Nl_cone_nvtx_histo_mu);

    rate_histo_e->Divide(rate_histo_e,Nl_histo_e,1,1,"B");
    rate_histo_mu->Divide(rate_histo_mu,Nl_histo_mu,1,1,"B");
    rate_cone_histo_e->Divide(rate_cone_histo_e,Nl_cone_histo_e,1,1,"B");
    rate_cone_histo_mu->Divide(rate_cone_histo_mu,Nl_cone_histo_mu,1,1,"B");
    rate_fine_cone_histo_e->Divide(rate_fine_cone_histo_e,Nl_fine_cone_histo_e,1,1,"B");
    rate_fine_cone_histo_mu->Divide(rate_fine_cone_histo_mu,Nl_fine_cone_histo_mu,1,1,"B");
    rate_jet_histo_e->Divide(rate_jet_histo_e,Nl_jet_histo_e,1,1,"B");
    rate_jet_histo_mu->Divide(rate_jet_histo_mu,Nl_jet_histo_mu,1,1,"B");
    rate_jet_highpt_histo_e->Divide(rate_jet_highpt_histo_e,Nl_jet_highpt_histo_e,1,1,"B");
    rate_jet_highpt_histo_mu->Divide(rate_jet_highpt_histo_mu,Nl_jet_highpt_histo_mu,1,1,"B");
    rate_jet_lowpt_histo_e->Divide(rate_jet_lowpt_histo_e,Nl_jet_lowpt_histo_e,1,1,"B");
    rate_jet_lowpt_histo_mu->Divide(rate_jet_lowpt_histo_mu,Nl_jet_lowpt_histo_mu,1,1,"B");
    total_BR_histo_e->Add(NnotBs_BR_histo_e);
    total_BR_histo_mu->Add(NnotBs_BR_histo_mu);
    NBs_BR_histo_e->Divide(NBs_BR_histo_e, total_BR_histo_e,1,1,"B");
    NBs_BR_histo_mu->Divide(NBs_BR_histo_mu, total_BR_histo_mu,1,1,"B");

    rate_histo_e->GetXaxis()->SetTitle("pT (GeV)"); 
    rate_histo_e->GetYaxis()->SetTitle("eta");
    rate_histo_e->GetZaxis()->SetRangeUser(0,.5);
    rate_histo_e->SetTitle("Fake Rate vs Pt, Eta (electrons)");
    rate_histo_mu->GetXaxis()->SetTitle("pT (GeV)"); 
    rate_histo_mu->GetYaxis()->SetTitle("eta");
    rate_histo_mu->GetZaxis()->SetRangeUser(0,.5);
    rate_histo_mu->SetTitle("Fake Rate vs Pt, Eta (muons)");
    rate_cone_histo_e->GetXaxis()->SetTitle("corrected pT (GeV)"); 
    rate_cone_histo_e->GetYaxis()->SetTitle("eta");
    rate_cone_histo_e->GetZaxis()->SetRangeUser(0,.5);
    rate_cone_histo_e->SetTitle("Fake Rate vs Pt + Cone Energy, Eta (electrons)");
    rate_cone_histo_mu->GetXaxis()->SetTitle("corrected pT (GeV)"); 
    rate_cone_histo_mu->GetYaxis()->SetTitle("eta");
    rate_cone_histo_mu->GetZaxis()->SetRangeUser(0,.5);
    rate_cone_histo_mu->SetTitle("Fake Rate vs Pt + Cone Energy, Eta (muons)");
    rate_jet_histo_e->GetXaxis()->SetTitle("Jet pT (GeV)"); 
    rate_jet_histo_e->GetYaxis()->SetTitle("eta");
    rate_jet_histo_e->GetZaxis()->SetRangeUser(0,.5);
    rate_jet_histo_e->SetTitle("Fake Rate vs Jet Pt, Eta (electrons)");
    rate_jet_histo_mu->GetXaxis()->SetTitle("Jet pT (GeV)"); 
    rate_jet_histo_mu->GetYaxis()->SetTitle("eta");
    rate_jet_histo_mu->GetZaxis()->SetRangeUser(0,.5);
    rate_jet_histo_mu->SetTitle("Fake Rate vs Jet Pt, Eta (muons)");
    rate_jet_highpt_histo_e->GetXaxis()->SetTitle("Jet pT (GeV)"); 
    rate_jet_highpt_histo_e->GetYaxis()->SetTitle("eta");
    rate_jet_highpt_histo_e->GetZaxis()->SetRangeUser(0,.5);
    rate_jet_highpt_histo_e->SetTitle("Fake Rate vs Jet Pt, Eta (electrons)");
    rate_jet_highpt_histo_mu->GetXaxis()->SetTitle("Jet pT (GeV)"); 
    rate_jet_highpt_histo_mu->GetYaxis()->SetTitle("eta");
    rate_jet_highpt_histo_mu->GetZaxis()->SetRangeUser(0,.5);
    rate_jet_highpt_histo_mu->SetTitle("Fake Rate vs Jet Pt, Eta (muons)");
    rate_jet_lowpt_histo_e->GetXaxis()->SetTitle("Jet pT (GeV)"); 
    rate_jet_lowpt_histo_e->GetYaxis()->SetTitle("eta");
    rate_jet_lowpt_histo_e->GetZaxis()->SetRangeUser(0,.5);
    rate_jet_lowpt_histo_e->SetTitle("Fake Rate vs Jet Pt, Eta (electrons)");
    rate_jet_lowpt_histo_mu->GetXaxis()->SetTitle("Jet pT (GeV)"); 
    rate_jet_lowpt_histo_mu->GetYaxis()->SetTitle("eta");
    rate_jet_lowpt_histo_mu->GetZaxis()->SetRangeUser(0,.5);
    rate_jet_lowpt_histo_mu->SetTitle("Fake Rate vs Jet Pt, Eta (muons)");
    NBs_BR_histo_e->GetXaxis()->SetTitle("Nbjets"); 
    NBs_BR_histo_e->GetYaxis()->SetTitle("Abundance");
    NBs_BR_histo_e->GetYaxis()->SetRangeUser(0., 1.);
    NBs_BR_histo_e->SetTitle("B Abundance vs Nbtags (electrons)");
    NBs_BR_histo_mu->GetXaxis()->SetTitle("Nbjets"); 
    NBs_BR_histo_mu->GetYaxis()->SetTitle("Abundance");
    NBs_BR_histo_mu->GetYaxis()->SetRangeUser(0., 1.);
    NBs_BR_histo_mu->SetTitle("B Abundance vs Nbtags (muons)");
    pTrelvsIso_histo_el->GetXaxis()->SetTitle("Iso");
    pTrelvsIso_histo_el->GetYaxis()->SetTitle("pTrel");
    pTrelvsIso_histo_mu->GetXaxis()->SetTitle("Iso");
    pTrelvsIso_histo_mu->GetYaxis()->SetTitle("pTrel");
    njets40_histo->GetXaxis()->SetTitle("Njets");
    njets40_histo->GetYaxis()->SetTitle("Events");
    njets40_histo->SetTitle("Njets with pT > 40 GeV");

    gStyle->SetOptStat(0);
    gStyle->SetPaintTextFormat("1.3f");

    TCanvas *c11=new TCanvas("c11","B Abundance vs Nbjets (electrons)",800,800);
    c11->cd();
    NBs_BR_histo_e->Draw("histE");
    TCanvas *c12=new TCanvas("c12","B Abundance vs Nbjets (muons)",800,800);
    c12->cd();
    NBs_BR_histo_mu->Draw("histE");
    TCanvas *c13=new TCanvas("c13","Njets with pT > 40 GeV",800,800);
    c13->cd();
    njets40_histo->Draw("histE");

    //---save histos-------//
    if (not outfile.Contains(".root")) {
        TString procstr(chain->GetTitle());
        TString isostr = (useIsoTrigs ? "_IsoTrigs" : "");
        procstr.ReplaceAll("_iso","");
        procstr.ReplaceAll("_2016","");
        procstr.ReplaceAll("_2017","");
        procstr.ReplaceAll("_2018","");
        // outfile += Form("/rate_histos_%s_LooseEMVA%s.root",procstr.Data(),isostr.Data());
        outfile += Form("/y%i_rate_histos_%s_LooseEMVA%s.root",year,procstr.Data(),isostr.Data());
    }
    TFile *OutputFile = new TFile(outfile,"recreate");
    OutputFile->cd();
    Nl_histo_e->Write();
    Nl_histo_mu->Write();
    Nt_histo_e->Write();
    Nt_histo_mu->Write();
    Nt_fine_histo_e->Write();
    Nt_fine_histo_mu->Write();
    rate_histo_e->Write();
    rate_histo_mu->Write();
    Nl_cone_histo_e->Write();
    Nl_cone_histo_mu->Write();
    rate_cone_histo_e->Write();
    rate_cone_histo_mu->Write();

    Nl_fine_cone_histo_e->Write();
    Nl_fine_cone_histo_mu->Write();
    rate_fine_cone_histo_e->Write();
    rate_fine_cone_histo_mu->Write();

    rate_cone_nvtx_histo_e->Write();
    rate_cone_nvtx_histo_mu->Write();
    Nl_cone_nvtx_histo_e->Write();
    Nl_cone_nvtx_histo_mu->Write();
    Nt_nvtx_histo_e->Write();
    Nt_nvtx_histo_mu->Write();

    Nl_jet_histo_e->Write();
    Nl_jet_histo_mu->Write();
    Nt_jet_histo_e->Write();
    Nt_jet_histo_mu->Write();
    rate_jet_histo_e->Write();
    rate_jet_histo_mu->Write();
    Nl_jet_highpt_histo_e->Write();
    Nl_jet_highpt_histo_mu->Write();
    Nt_jet_highpt_histo_e->Write();
    Nt_jet_highpt_histo_mu->Write();
    rate_jet_highpt_histo_e->Write();
    rate_jet_highpt_histo_mu->Write();
    Nl_jet_lowpt_histo_e->Write();
    Nl_jet_lowpt_histo_mu->Write();
    Nt_jet_lowpt_histo_e->Write();
    Nt_jet_lowpt_histo_mu->Write();
    rate_jet_lowpt_histo_e->Write();
    rate_jet_lowpt_histo_mu->Write();
    NBs_BR_histo_e->Write();
    NBs_BR_histo_mu->Write();  
    pTrelvsIso_histo_el->Write();
    pTrelvsIso_histo_mu->Write();
    pTrel_histo_el->Write();
    pTrel_histo_mu->Write();
    pTratio_histo_el->Write();
    pTratio_histo_mu->Write();
    miniiso_histo_el->Write();
    miniiso_histo_mu->Write();
    miniiso_histo_el_loose70->Write();
    miniiso_histo_el_tight70->Write();
    histo_ht->Write();
    histo_met->Write();
    histo_met_all->Write();
    histo_met_all_el->Write();
    histo_met_all_mu->Write();
    histo_met_lm->Write();
    histo_met_lm_el->Write();
    histo_met_lm_mu->Write();
    histo_met_cr->Write();
    histo_met_cr_el->Write();
    histo_met_cr_mu->Write();
    histo_mt->Write();
    histo_mt_all->Write();
    histo_mt_all_el->Write();
    histo_mt_all_mu->Write();
    histo_mt_lm->Write();
    histo_mt_lm_el->Write();
    histo_mt_lm_mu->Write();
    histo_mt_cr->Write();
    histo_mt_cr_el->Write();
    histo_mteta_cr_el->Write();
    histo_mt_cr_mu->Write();
    histo_mteta_cr_mu->Write();
    histo_mt_cr2_el->Write();
    histo_mt_cr2_mu->Write();
    histo_mt_cr_dphi->Write();
    histo_mt_cr_dphi_el->Write();
    histo_mt_cr_dphi_mu->Write();
    histo_mt_cr_met->Write();
    histo_mt_cr_met_el->Write();
    histo_mt_cr_met_mu->Write();
    histo_mt_cr_pt->Write();
    histo_mt_cr_pt_el->Write();
    histo_mt_cr_pt_mu->Write();
    histo_mt_cr_noiso_el->Write();
    histo_mt_cr_noiso_mu->Write();
    histo_mt_cr2_noiso_el->Write();
    histo_mt_cr2_noiso_mu->Write();
    histo_mt_cr_noiso_dphi->Write();
    histo_mt_cr_noiso_dphi_el->Write();
    histo_mt_cr_noiso_dphi_mu->Write();
    histo_mt_cr_noiso_met->Write();
    histo_mt_cr_noiso_met_el->Write();
    histo_mt_cr_noiso_met_mu->Write();
    histo_mt_cr_noiso_pt->Write();
    histo_mt_cr_noiso_pt_el->Write();
    histo_mt_cr_noiso_pt_mu->Write();
    histo_mt_cr_hi->Write();
    histo_mt_cr_hi_el->Write();
    histo_mt_cr_hi_mu->Write();
    histo_mt_cr_vh->Write();
    histo_mt_cr_vh_el->Write();
    histo_mt_cr_vh_mu->Write();
    histo_mt_cr_low_el->Write();
    histo_mt_cr_low_mu->Write();
    histo_mt_cr_high_el->Write();
    histo_mt_cr_high_mu->Write();
    histo_mt_cr_unw_low_el->Write();
    histo_mt_cr_unw_low_mu->Write();
    histo_mt_cr_unw_high_el->Write();
    histo_mt_cr_unw_high_mu->Write();
    histo_pt_mu34->Write();
    histo_pt_mu24->Write();
    histo_pt_mu17->Write();
    histo_pt_mu8->Write();
    histo_pt_mu->Write();
    histo_pt_el34->Write();
    histo_pt_el24->Write();
    histo_pt_el17->Write();
    histo_pt_el12->Write();
    histo_pt_el8->Write();
    histo_pt_el->Write();
    njets40_histo->Write();
    hdenom->Write();
    Nl_conevsraw_histo_e->Write();
    Nl_conevsraw_histo_mu->Write();
    Nt_conevsraw_histo_e->Write();
    Nt_conevsraw_histo_mu->Write();
    OutputFile->Close();

    // delete jet_corrector_25ns_MC_pfL1; 
    // delete jet_corrector_25ns_MC_pfL1L2L3; 
    // delete jet_corrector_25ns_DATA_pfL1; 
    // delete jet_corrector_25ns_DATA_pfL1L2L3; 

    // return
    bmark->Stop("benchmark");
    if (not quiet) {
        cout << endl;
        cout << nEventsTotal << " Events Processed" << endl;
        cout << "------------------------------" << endl;
        cout << "CPU  Time:	" << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
        cout << "Real Time:	" << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
        cout << endl;
    }
    delete bmark;
    return 0;
}
