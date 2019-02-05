#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <map>
#include <fstream>
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TH2D.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TString.h"
#include "TPaveText.h"

#include "../../../common/CORE/SSSelections.h"
// #include "../../../common/CORE/IsolationTools.h"
// #include "../../../common/CORE/MCSelections.h"


// #include "../../../common/CORE/Tools/utils.h"
// #include "../../../common/CORE/Tools/dorky/dorky.cc"
// #include "../../../common/Software/dataMCplotMaker/PlotMaker2D.h"
// #include "../../../common/Software/dataMCplotMaker/dataMCplotMaker.h"
#include "../../misc/common_utils.h"
#include "../../misc/class_files/v8.02/SS.h"

using namespace std;
// using namespace duplicate_removal;

// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/fr_ttbar.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/fr_ttbar_highmet.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/fr_ttbar_highmet_allnfo.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/fr_ttbar_nom_full.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/fr_ttbar_nom_full_fine.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/rw_ttbar.h"
//
// 

// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/newoldfrs_v2.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/newfrs_nometmt.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/newoldfrs_v2_fine.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/newoldfrs_v3_fine.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/newoldfrs_v2_above25.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/newoldfrs_v2_noconecorr.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/newoldfrs_v2_mu18.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/newoldfrs_v2_newp.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/newfrs_ttbartest25.h"
// #include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/newfrs_ttbartest25_mid3.h"
#include "/home/users/namin/2018/fourtop/94x/FTAnalysis/analysis/fakes/derivation/newfrs_qcd18.h"
#include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/insitu/frs.h"

// #include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/derivation/testfrs.h"
#include "../derivation/newfrs_2017.h"
// #include "../derivation/newfrs_2017_above18.h"

namespace frs2017 {
#include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/derivation/frs_2017_ss.h"
}

bool inclHT = false;

bool doNew = false;
bool doQCD = true; // XXX
bool doHadFR = false;
bool doHadApp = false;
bool onlyMuMu = false;
bool doAbove18 = false; // XXX
bool doNewP = false;
bool doAbove25 = false;
bool bypassTrigger = false;
bool absweight = false; // XXX
bool ignoreConeCorr = false;

TH1D * evtCounter = new TH1D("","",1000,0,1000); 
map<TString, int> evtBinMap;
int evtBin = 0;
void initCounter() {
    evtCounter = new TH1D("","",1000,0,1000); 
    evtCounter->Sumw2();
    evtBinMap.clear();
}
void addToCounter(TString name, double weight=1.0) {
    if(evtBinMap.find(name) == evtBinMap.end() ) {
        evtBinMap[name] = evtBin;
        evtBin++;
    }
    evtCounter->Fill(evtBinMap[name], weight);
}
void printCounter(bool file = false) {
    ofstream outfile;
    if(file) outfile.open("counter.txt");
    cout << string(30, '-') << endl << "Counter totals: " << endl;
    for(map<TString,int>::iterator it = evtBinMap.begin(); it != evtBinMap.end(); it++) {
        int iBin = (it->second)+1;
        printf("%-15s %6.2f %6.2f\n",
                (it->first).Data(),
                evtCounter->GetBinContent(iBin),
                evtCounter->GetBinError(iBin) );
        if(file) outfile << (it->first).Data() << "  " << evtCounter->GetBinContent(iBin) << "  " << evtCounter->GetBinError(iBin) << endl;
    }
    cout << string(30, '-') << endl;
    if(file) outfile.close();
    if(file) cout << "Wrote counter to counter.txt" << endl;
}

float MT(float pt1, float phi1, float pt2, float phi2){
  return sqrt( 2 * pt1 * pt2 * ( 1 - cos( phi1 - phi2 ) ) );
}

bool isFakeLeg(int lep, bool doData=false){
  if (doData) return true;
  if (lep == 1) return (ss::lep1_motherID() <= 0 && ss::lep1_motherID() != -3);
  if (lep == 2) return (ss::lep2_motherID() <= 0 && ss::lep2_motherID() != -3);
  return 0;
}

bool isGoodLeg(int lep, bool doData=false){
  if (doData) return true;
  if (lep == 1) return (ss::lep1_motherID() == 1);
  if (lep == 2) return (ss::lep2_motherID() == 1);
  return 0;
}

float computePtRel(LorentzVector lepp4, LorentzVector jetp4, bool subtractLep){
  if (jetp4.pt()==0) return 0.;
  if (subtractLep) jetp4-=lepp4;
  float dot = lepp4.Vect().Dot( jetp4.Vect() );
  float ptrel = lepp4.P2() - dot*dot/jetp4.P2();
  ptrel = ptrel>0 ? sqrt(ptrel) : 0.0;
  return ptrel;
}

int number = 0;

float getFakeRate(int id, float pt, float eta, float ht, bool extrPtRel = false, bool doData = false, bool doInSitu = false){
    if (inclHT) ht = -1; // negative ht means use inclusive ht in commonUtils // FIXME

    float fact = 0.;

    if (doInSitu) {
        // fact = (abs(id)==13) ? numer1_mu_siphigh_FakeRate(pt,eta) : numer1_el_siphigh_FakeRate(pt,eta);
        fact = (abs(id)==13) ? numer1_mu_siplow_FakeRate(pt,eta) : numer1_el_siplow_FakeRate(pt,eta);
    } else {
        // fact = (abs(id)==13) ? muonQCDMCFakeRate_18noanypt_new(pt,eta) : electronQCDMCFakeRate_18noanypt_new(pt,eta);
        // fact = (abs(id)==13) ? mytest::muonQCDMCFakeRate(pt,eta) : mytest::electronQCDMCFakeRate(pt,eta);
        fact = (abs(id)==13) ? frs2017::muonQCDMCFakeRate_IsoTrigs(pt,eta) : frs2017::electronQCDMCFakeRate_IsoTrigs(pt,eta);
    }

    if (fact > 0.8) fact = 0.8;
    if (fact < 0.) fact = 0.;

    return fact;


    // return qcdMCFakeRate(id, pt, eta, ht);

    // if (doInSitu) return fakeRateInSitu(id, pt, eta, ht);
    // else if (doData ) return fakeRate(id, pt, eta, ht);
    // else return qcdMCFakeRate(id, pt, eta, ht);
}

float getFakeRateError(int id, float pt, float eta, float ht, bool doInSitu = false) { 
    float fact = 0.;
    if (doInSitu) {
        // return fakeRateErrorInSitu(id, pt, eta, ht);
        // fact = (abs(id)==13) ? numer1_mu_siphigh_FakeRateError(pt,eta) : numer1_el_siphigh_FakeRateError(pt,eta);
        fact = (abs(id)==13) ? numer1_mu_siplow_FakeRateError(pt,eta) : numer1_el_siplow_FakeRateError(pt,eta);
    }
    // else return fakeRateError(id, pt, eta, ht);
    // fact = (abs(id)==13) ? muonQCDMCFakeRateError_18noanypt_new(pt,eta) : electronQCDMCFakeRateError_18noanypt_new(pt,eta);
    // fact = (abs(id)==13) ? mytest::muonQCDMCFakeRateError(pt,eta) : mytest::electronQCDMCFakeRateError(pt,eta);
    fact = (abs(id)==13) ? frs2017::muonQCDMCFakeRate_IsoTrigs(pt,eta) : frs2017::electronQCDMCFakeRate_IsoTrigs(pt,eta);
    return fact;
}

void GetErrorPlot(TH1F *pred, vector< vector<TH2D*> > pred_err2_mu, vector< vector<TH2D*> > pred_err2_el, bool inSitu){

  for (int bin=1;bin<=pred->GetNbinsX();++bin) {
    int sr = bin-1;
    float p = pred->GetBinContent(bin);
    //stat error on nFOs
    float pe = pred->GetBinError(bin);
    float pefr2 = 0;
    //stat error on FR 
    for (unsigned int i = 0; i < 2; i++){
      if (pred_err2_mu[i][sr]!=0) { 
        for (int frbinx=1;frbinx<=pred_err2_mu[i][sr]->GetNbinsX();++frbinx) {
          for (int frbiny=1;frbiny<=pred_err2_mu[i][sr]->GetNbinsY();++frbiny) {
            float fr = getFakeRate(13, pred_err2_mu[i][sr]->GetXaxis()->GetBinLowEdge(frbinx), pred_err2_mu[i][sr]->GetYaxis()->GetBinLowEdge(frbiny), i == 0 ? 500 : 150, false, inSitu); 
            float fre = getFakeRateError(13, pred_err2_mu[i][sr]->GetXaxis()->GetBinLowEdge(frbinx), pred_err2_mu[i][sr]->GetYaxis()->GetBinLowEdge(frbiny), i == 0 ? 500 : 150,  inSitu); 
            float tot = pred_err2_mu[i][sr]->GetBinContent(frbinx,frbiny);
            pefr2 += fre*fre*pow(1-fr,-4)*tot*tot;
          }
        }
      }
      if (pred_err2_el[i][sr]!=0) { 
        for (int frbinx=1;frbinx<=pred_err2_el[i][sr]->GetNbinsX();++frbinx) {
          for (int frbiny=1;frbiny<=pred_err2_el[i][sr]->GetNbinsY();++frbiny) {
            float fr = getFakeRate(11, pred_err2_el[i][sr]->GetXaxis()->GetBinLowEdge(frbinx), pred_err2_el[i][sr]->GetYaxis()->GetBinLowEdge(frbiny), i == 0 ? 500 : 150, false, inSitu);
            float fre = getFakeRateError(11, pred_err2_el[i][sr]->GetXaxis()->GetBinLowEdge(frbinx), pred_err2_el[i][sr]->GetYaxis()->GetBinLowEdge(frbiny), i == 0 ? 500 : 150,  inSitu);
            float tot = pred_err2_el[i][sr]->GetBinContent(frbinx,frbiny);
            pefr2 += fre*fre*pow(1-fr,-4)*tot*tot;
          }
        }
      }
    }
    //now combine the two
    pe = sqrt(pe*pe + pefr2);
    pred->SetBinError(bin, pe); 
  }
}


//getPt and getEta need to stay on sync with meas region
float getPt(float pt, bool extrPtRel = false){
  if(pt < 10.)  return 11.;  
  if(!extrPtRel && pt >= 90.) return 89.;
  if(extrPtRel && pt >= 150.) return 149.;
  return pt;
}

float getEta(float eta, float ht, bool extrPtRel = false){
  if (extrPtRel) {
    if(ht >= 800) return 799;
    return ht;
  }
  if(fabs(eta) >= 2.4) return 2.3;
  return fabs(eta);
}

TH1F* histCreator(string str1, string str2, int n1, float n2, float n3){
  TH1F *temp = new TH1F(str1.c_str(), str2.c_str(), n1, n2, n3);   
  return temp;
}
TH1F* histCreator(string str1, string str2, int nbins, float xbins[]){
  // int nbins = 5;
  // float xbins[6] = {10, 15, 25, 35, 50, 1000};
  TH1F *temp = new TH1F(str1.c_str(), str2.c_str(), nbins, xbins);
  return temp;
}
TH2D* histCreator2(string str1, string str2, int nbinsx, float xbins[], int nbinsy, float ybins[]){
  TH2D *temp = new TH2D(str1.c_str(), str2.c_str(), nbinsx, xbins, nbinsy, ybins);
  return temp;
}

vector <TH1F*> hists; 
vector <TH2D*> hists2; 

// int getHist(string name){
int getHist(TString name){
  for (unsigned int i = 0; i < hists.size(); i++){
    if (hists[i]->GetName() == name) return i; 
  }
  cout << "getHist failed on: " << name << ".  Be careful of spurious spaces!" << endl;
  return -1;
}
int getHist2(TString name){
  for (unsigned int i = 0; i < hists2.size(); i++){
    if (hists2[i]->GetName() == name) return i; 
  }
  cout << "getHist2 failed on: " << name << ".  Be careful of spurious spaces!" << endl;
  return -1;
}


std::vector<std::pair<TH1F*, TH1F*> > getBackgrounds(std::string type, int isMu, std::vector<TString> filenames) {
  std::vector<std::pair<TH1F*, TH1F*> > pairs;
  for(unsigned int i = 0; i < filenames.size(); i++) {
      std::string histname = type;
      if(isMu == 0) histname += "_el";
      else if(isMu == 1) histname += "_mu";
      histname += filenames[i].Data();
      pairs.push_back( make_pair(hists[getHist(histname)],hists[getHist(histname)]) );
  }
  return pairs;
}

void printClosureNumbers(std::vector<TString> filenames) {
    for(int imu = -1; imu < 2; imu++) {
        TString musuffix = "";
        if (imu == 1) musuffix = "_mu";
        else if (imu == 0) musuffix = "_el";

        std::cout << "-- Closure for " << (imu >= 0 ? (imu == 1 ? "MU" : "EL" ) : "TOTAL") << " -- " << std::endl;
        float val_pred = hists[getHist("Npn_histo_sr_pred"+musuffix)]->Integral();
        float val_pred_unw = hists[getHist("Npn_histo_sr_pred_unw"+musuffix)]->Integral();
        std::cout << "  pred: " << val_pred << " (unweighted/T!L = " << val_pred_unw << ")" << std::endl;
        std::vector<std::pair<TH1F*,TH1F*> > vobs = getBackgrounds("Npn_histo_sr_obs",imu,filenames);
        float tot_obs = 0.;
        for(unsigned int ifile = 0; ifile < filenames.size(); ifile++) {
            float val_obs = vobs.at(ifile).first->Integral();
            tot_obs += val_obs;
            std::cout << "   " << filenames[ifile] << " obs: " << val_obs << std::endl;
        }
        std::cout << "  --> pred/obs: " << val_pred/tot_obs << std::endl;

        // std::string tag = "muonQCDMCFakeRate_18noanypt_new";
        // std::string tag = "muonTTbarMCFakeRate_new";
        // std::string tag = "muonTTbarMCFakeRatehad1_fogeq2_new";
        // std::string tag = "muonTTbarMCFakeRatehad1_fogeq2_notrig_new";
        // std::string tag = "muonTTbarMCFakeRatehad1_new";
        // std::string tag = "muonTTbarMCFakeRatehad2_new";
        // std::string tag = "muonTTbarMCFakeRatehad1bonly_new";
        // std::string tag = "muonTTbarMCFakeRatehad1_fogeq2_bonly_new";
        // std::string tag = "muonTTbarMCFakeRate_bonly_new";
        // std::string tag = "muonTTbarMCFakeRatehad2_bonly_new";
        
        // std::string tag = "muonTTbarMCFakeRatehad1_abs_fogeq2_new";
        // std::string tag = "muonTTbarMCFakeRatehad2_abs_new";
        // std::string tag = "testfrs18";
        std::string tag = "testfrsall";
        // std::string tag = "muonTTbarMCFakeRatehad1_abs_fogeq2_bonly_new";
        // std::string tag = "muonTTbarMCFakeRatehad2_abs_bonly_new";

        // std::string tag = "muonTTbarMCFakeRatehad1_fogeq2_newFine";
        // std::string tag = "muonTTbarMCFakeRatehad1_fogeq2_bonly_newFine";
        // std::string tag = "muonTTbarMCFakeRatehad1_fogeq2_fix_new";
        // std::string tag = "muonTTbarMCFakeRatehad1_fix_new";
        if (imu >= 0) {
            std::cout << ((imu == 1) ? "MU" : "EL" )
                << "GREP (L!T, pred, obs, pred/obs) " << tag << " "
                << val_pred_unw << " " << val_pred 
                << " " << tot_obs << " " << val_pred/tot_obs << std::endl;
        }
        std::cout << std::endl;
    }
}

int ScanChain( TChain* chain, TString option = "", TString ptRegion = "HH", bool doData = false, int nEvents = -1){

  initCounter();


  //Parse options
  bool coneCorr = option.Contains("coneCorr") ? true : false;
  bool doBonly = option.Contains("doBonly") ? true : false;
  bool doConly = option.Contains("doConly") ? true : false;
  bool doLightonly = option.Contains("doLightonly") ? true : false;
  bool inSitu = option.Contains("inSitu") ? true : false;
  bool soup = option.Contains("soup") ? true : false;
  bool highhigh = ptRegion.Contains("HH") ? true : false;
  bool highlow = ptRegion.Contains("HL") ? true : false;
  bool lowlow = ptRegion.Contains("LL") ? true : false;

  bool extrPt = option.Contains("_ept") ? true : false;
  inclHT = option.Contains("_hth") ? true : false;

  bool doLowHT = option.Contains("IsoTrigs") ? true : false;
  bool doHighHT = option.Contains("HTTrigs") ? true : false;

  bool doSpecial = option.Contains("special") ? true : false;

  bool weightOne = false;
  bool bypass = false;
  bool doSubtractContamination = false;
  bool requireIsoTriggers = false;

  // float luminosity = doData ? getLumi() : 12.9;
  int year = 2017;
  float luminosity = doData ? getLumi(year) : getLumi(year);

  //Dir
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");

  int nsr = 33;
  if (highlow) nsr = 27;
  if (lowlow)  nsr = 8;

  //Make rate histo (for shape only)
  float xbins[8] = {10, 15, 20, 25, 35, 50, 70, 250};
  float xbins_extr[7] = {10, 15, 25, 35, 50, 70, 1000};
  float xbins_abbrev[5] = {15, 25, 35, 50, 150};
  float ybinse[4] = {0, 0.8, 1.479, 2.5}; 
  float ybinsm[4] = {0, 1.2, 2.1, 2.4}; 

  TH2D *rate_histo_e;
  TH2D *rate_histo_mu;
  if (extrPt) {
      rate_histo_e  = new TH2D("elec", "elec", 6, xbins_extr, 3, ybinse);
      rate_histo_mu = new TH2D("muon", "muon", 6, xbins_extr, 3, ybinsm);
  } else {
      rate_histo_e  = new TH2D("elec", "elec", 5, xbins     , 3, ybinse);
      rate_histo_mu = new TH2D("muon", "muon", 5, xbins     , 3, ybinsm);
  }

  //Histograms
  vector<TString> possibleFilenames = {"TTBAR", "WJets"};
  for (unsigned int i = 0; i < possibleFilenames.size(); i++) {
      std::string fname = possibleFilenames[i].Data();
      hists.push_back( histCreator("Npn_histo_NB_obs"        + fname, "Observed Prompt-NonPrompt Background"              ,  4, 0,    4) ); 
      hists.push_back( histCreator("Npn_histo_NB_pred"              , "Predicted Prompt-NonPrompt Background"             ,  4, 0,    4) );
      hists.push_back( histCreator("Npn_histo_NB_obs_mu"     + fname, "Observed Prompt-NonPrompt Background (Single mu)"  ,  4, 0,    4) );
      hists.push_back( histCreator("Npn_histo_NB_pred_mu"           , "Predicted Prompt-NonPrompt Background (Single mu)" ,  4, 0,    4) );
      hists.push_back( histCreator("Npn_histo_NB_obs_el"     + fname, "Observed Prompt-NonPrompt Background (Single el)"  ,  4, 0,    4) );
      hists.push_back( histCreator("Npn_histo_NB_pred_el"           , "Predicted Prompt-NonPrompt Background (Single el)" ,  4, 0,    4) );
      hists.push_back( histCreator("Npn_histo_sr_obs"        + fname, "Observed Prompt-NonPrompt Background"              , nsr, 0.5,   nsr+0.5) );
      hists.push_back( histCreator("Npn_histo_sr_pred"              , "Predicted Prompt-NonPrompt Background"             , nsr, 0.5,   nsr+0.5) );
      hists.push_back( histCreator("Npn_histo_sr_pred_unw"              , "Predicted Prompt-NonPrompt Background"             , nsr, 0.5,   nsr+0.5) );
      hists.push_back( histCreator("Npn_histo_sr_obs_mu"     + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , nsr, 0.5,   nsr+0.5) );
      hists.push_back( histCreator("Npn_histo_sr_pred_mu"           , "Predicted Prompt-NonPrompt Background (Single mu)" , nsr, 0.5,   nsr+0.5) );
      hists.push_back( histCreator("Npn_histo_sr_pred_unw_mu"           , "Predicted Prompt-NonPrompt Background (Single mu)" , nsr, 0.5,   nsr+0.5) );
      hists.push_back( histCreator("Npn_histo_sr_obs_el"     + fname, "Observed Prompt-NonPrompt Background (Single el)"  , nsr, 0.5,   nsr+0.5) );
      hists.push_back( histCreator("Npn_histo_sr_pred_el"           , "Predicted Prompt-NonPrompt Background (Single el)" , nsr, 0.5,   nsr+0.5) );
      hists.push_back( histCreator("Npn_histo_sr_pred_unw_el"           , "Predicted Prompt-NonPrompt Background (Single el)" , nsr, 0.5,   nsr+0.5) );
      hists.push_back( histCreator("Npn_histo_HT_obs"        + fname, "Observed Prompt-NonPrompt Background"              , 15, 0, 600) );
      hists.push_back( histCreator("Npn_histo_HT_pred"              , "Predicted Prompt-NonPrompt Background"             , 15, 0, 600) );
      hists.push_back( histCreator("Npn_histo_HT_obs_mu"     + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 15, 0, 600) );
      hists.push_back( histCreator("Npn_histo_HT_pred_mu"           , "Predicted Prompt-NonPrompt Background (Single mu)" , 15, 0, 600) );
      hists.push_back( histCreator("Npn_histo_HT_obs_el"     + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 15, 0, 600) );
      hists.push_back( histCreator("Npn_histo_HT_pred_el"           , "Predicted Prompt-NonPrompt Background (Single el)" , 15, 0, 600) );
      hists.push_back( histCreator("Npn_histo_MET_obs"       + fname, "Observed Prompt-NonPrompt Background"              , 20, 0,   200) );
      hists.push_back( histCreator("Npn_histo_MET_pred"             , "Predicted Prompt-NonPrompt Background"             , 20, 0,   200) );
      hists.push_back( histCreator("Npn_histo_MET_obs_mu"    + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 20, 0,   200) );
      hists.push_back( histCreator("Npn_histo_MET_pred_mu"          , "Predicted Prompt-NonPrompt Background (Single mu)" , 20, 0,   200) );
      hists.push_back( histCreator("Npn_histo_MET_obs_el"    + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 20, 0,   200) );
      hists.push_back( histCreator("Npn_histo_MET_pred_el"          , "Predicted Prompt-NonPrompt Background (Single el)" , 20, 0,   200) );
      hists.push_back( histCreator("Npn_histo_MTMIN_obs"     + fname, "Observed Prompt-NonPrompt Background"              , 20, 0,   200) );
      hists.push_back( histCreator("Npn_histo_MTMIN_pred"           , "Predicted Prompt-NonPrompt Background"             , 20, 0,   200) );
      hists.push_back( histCreator("Npn_histo_MTMIN_obs_mu"  + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 20, 0,   200) );
      hists.push_back( histCreator("Npn_histo_MTMIN_pred_mu"        , "Predicted Prompt-NonPrompt Background (Single mu)" , 20, 0,   200) );
      hists.push_back( histCreator("Npn_histo_MTMIN_obs_el"  + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 20, 0,   200) );
      hists.push_back( histCreator("Npn_histo_MTMIN_pred_el"        , "Predicted Prompt-NonPrompt Background (Single el)" , 20, 0,   200) );
      hists.push_back( histCreator("Npn_histo_NJET_obs"     + fname, "Observed Prompt-NonPrompt Background"              , 8, 0,   8) );
      hists.push_back( histCreator("Npn_histo_NJET_pred"           , "Predicted Prompt-NonPrompt Background"             , 8, 0,   8) );
      hists.push_back( histCreator("Npn_histo_NJET_obs_mu"  + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 8, 0,   8) );
      hists.push_back( histCreator("Npn_histo_NJET_pred_mu"        , "Predicted Prompt-NonPrompt Background (Single mu)" , 8, 0,   8) );
      hists.push_back( histCreator("Npn_histo_NJET_obs_el"  + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 8, 0,   8) );
      hists.push_back( histCreator("Npn_histo_NJET_pred_el"        , "Predicted Prompt-NonPrompt Background (Single el)" , 8, 0,   8) );
      hists.push_back( histCreator("Npn_histo_MATCH_obs"     + fname, "Observed Prompt-NonPrompt Background"              , 5, 0,   5) );
      hists.push_back( histCreator("Npn_histo_MATCH_pred"           , "Predicted Prompt-NonPrompt Background"             , 5, 0,   5) );
      hists.push_back( histCreator("Npn_histo_MATCH_obs_mu"  + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 5, 0,   5) );
      hists.push_back( histCreator("Npn_histo_MATCH_pred_mu"        , "Predicted Prompt-NonPrompt Background (Single mu)" , 5, 0,   5) );
      hists.push_back( histCreator("Npn_histo_MATCH_obs_el"  + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 5, 0,   5) );
      hists.push_back( histCreator("Npn_histo_MATCH_pred_el"        , "Predicted Prompt-NonPrompt Background (Single el)" , 5, 0,   5) );
      hists.push_back( histCreator("Npn_histo_L1PT_obs"      + fname, "Observed Prompt-NonPrompt Background"              , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_L1PT_pred"            , "Predicted Prompt-NonPrompt Background"             , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_L1PT_obs_mu"   + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_L1PT_pred_mu"         , "Predicted Prompt-NonPrompt Background (Single mu)" , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_L1PT_obs_el"   + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_L1PT_pred_el"         , "Predicted Prompt-NonPrompt Background (Single el)" , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_L2PT_obs"      + fname, "Observed Prompt-NonPrompt Background"              , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_L2PT_pred"            , "Predicted Prompt-NonPrompt Background"             , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_L2PT_obs_mu"   + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_L2PT_pred_mu"         , "Predicted Prompt-NonPrompt Background (Single mu)" , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_L2PT_obs_el"   + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_L2PT_pred_el"         , "Predicted Prompt-NonPrompt Background (Single el)" , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_LTrue_obs"     + fname, "Observed Prompt-NonPrompt Background"              , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_LTrue_pred"           , "Predicted Prompt-NonPrompt Background"             , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_LTrue_obs_mu"  + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_LTrue_pred_mu"        , "Predicted Prompt-NonPrompt Background (Single mu)" , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_LTrue_obs_el"  + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_LTrue_pred_el"        , "Predicted Prompt-NonPrompt Background (Single el)" , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_LFake_obs"     + fname, "Observed Prompt-NonPrompt Background"              , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_LFake_pred"           , "Predicted Prompt-NonPrompt Background"             , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_LFake_obs_mu"  + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_LFake_pred_mu"        , "Predicted Prompt-NonPrompt Background (Single mu)" , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_LFake_obs_el"  + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 30, 0,   150) );
      hists.push_back( histCreator("Npn_histo_LFake_pred_el"        , "Predicted Prompt-NonPrompt Background (Single el)" , 30, 0,   150) );
      hists.push_back( histCreator("NBs_NB_histo_e"                 , "Number of FOs from B's vs Nbtags (els)"            ,  4, 0,    4) );
      hists.push_back( histCreator("NBs_NB_histo_mu"                , "Number of FOs from B's vs Nbtags (muons)"          ,  4, 0,    4) );
      hists.push_back( histCreator("NnotBs_NB_histo_e"              , "Number of FOs NOT from B's vs Nbtags (els)"        ,  4, 0,    4) );
      hists.push_back( histCreator("NnotBs_NB_histo_mu"             , "Number of FOs NOT from B's vs Nbtags (muons)"      ,  4, 0,    4) );
      hists.push_back( histCreator("pTrel_histo_el"                 , "pTrel (Electrons)"                                 , 15, 0,   30) );
      hists.push_back( histCreator("pTrel_histo_mu"                 , "pTrel (Muons)"                                     , 15, 0,   30) );
      hists.push_back( histCreator("Npn_histo_LFakerebin_obs"  + fname, "Observed Prompt-NonPrompt Background"  , 4, xbins_abbrev) );
      hists.push_back( histCreator("Npn_histo_LFakerebin_pred" , "Predicted Prompt-NonPrompt Background" , 4, xbins_abbrev) );
      hists.push_back( histCreator("Npn_histo_LFakerebin_obs_mu"  + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 4, xbins_abbrev) );
      hists.push_back( histCreator("Npn_histo_LFakerebin_pred_mu" , "Predicted Prompt-NonPrompt Background (Single mu)" , 4, xbins_abbrev) );
      hists.push_back( histCreator("Npn_histo_LFakerebin_obs_el"  + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 4, xbins_abbrev) );
      hists.push_back( histCreator("Npn_histo_LFakerebin_pred_el" , "Predicted Prompt-NonPrompt Background (Single el)" , 4, xbins_abbrev) );
      hists.push_back( histCreator("Npn_histo_L1ETA_obs"     + fname, "Observed Prompt-NonPrompt Background"              , 5, 0,   2.5) );
      hists.push_back( histCreator("Npn_histo_L1ETA_pred"           , "Predicted Prompt-NonPrompt Background"             , 5, 0,   2.5) );
      hists.push_back( histCreator("Npn_histo_L1ETA_obs_mu"  + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 3,  ybinsm) );
      hists.push_back( histCreator("Npn_histo_L1ETA_pred_mu"        , "Predicted Prompt-NonPrompt Background (Single mu)" , 3,  ybinsm) );
      hists.push_back( histCreator("Npn_histo_L1ETA_obs_el"  + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 3,  ybinse) );
      hists.push_back( histCreator("Npn_histo_L1ETA_pred_el"        , "Predicted Prompt-NonPrompt Background (Single el)" , 3,  ybinse) );
      hists.push_back( histCreator("Npn_histo_L2ETA_obs"     + fname, "Observed Prompt-NonPrompt Background"              , 5, 0,   2.5) );
      hists.push_back( histCreator("Npn_histo_L2ETA_pred"           , "Predicted Prompt-NonPrompt Background"             , 5, 0,   2.5) );
      hists.push_back( histCreator("Npn_histo_L2ETA_obs_mu"  + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 3,  ybinsm) );
      hists.push_back( histCreator("Npn_histo_L2ETA_pred_mu"        , "Predicted Prompt-NonPrompt Background (Single mu)" , 3,  ybinsm) );
      hists.push_back( histCreator("Npn_histo_L2ETA_obs_el"  + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 3,  ybinse) );
      hists.push_back( histCreator("Npn_histo_L2ETA_pred_el"        , "Predicted Prompt-NonPrompt Background (Single el)" , 3,  ybinse) );

      hists2.push_back( histCreator2("Npn_histo_PTETA_obs_mu"  + fname, "Observed Prompt-NonPrompt Background (Single mu)"  , 7, xbins, 3,  ybinsm) );
      hists2.push_back( histCreator2("Npn_histo_PTETA_pred_mu"        , "Predicted Prompt-NonPrompt Background (Single mu)" , 7, xbins, 3,  ybinsm) );
      hists2.push_back( histCreator2("Npn_histo_PTETA_obs_el"  + fname, "Observed Prompt-NonPrompt Background (Single el)"  , 7, xbins, 3,  ybinse) );
      hists2.push_back( histCreator2("Npn_histo_PTETA_pred_el"        , "Predicted Prompt-NonPrompt Background (Single el)" , 7, xbins, 3,  ybinse) );

  }

  for (unsigned int i = 0; i < hists.size(); i++){
    hists[i]->SetDirectory(rootdir); 
    hists[i]->Sumw2(); 
  }

  //2D histos
  TH2D *pTrelvsIso_histo_mu = new TH2D("pTrelvsIso_histo_mu", "pTrel vs Iso (Muons)", 10, 0., 1., 15, 0., 30.);
  TH2D *pTrelvsIso_histo_el = new TH2D("pTrelvsIso_histo_el", "pTrel vs Iso (Electrons)", 10, 0., 1., 15, 0., 30.);
  TH2D *pTrelvsMiniIso_histo_mu = new TH2D("pTrelvsMiniIso_histo_mu", "pTrel vs MiniIso (Muons)", 10, 0., 1., 15, 0., 30.);
  TH2D *pTrelvsMiniIso_histo_el = new TH2D("pTrelvsMiniIso_histo_el", "pTrel vs MiniIso (Electrons)", 10, 0., 1., 15, 0., 30.);


  //Errors
  vector< vector<TH2D*> > Npn_histo_NB_err2_pred_mu(2, vector<TH2D*>(4,0));  
  vector< vector<TH2D*> > Npn_histo_NB_err2_pred_el(2, vector<TH2D*>(4,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<4;++h){
      Npn_histo_NB_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_NB_err2_pred_mu_NB%i",h));
      Npn_histo_NB_err2_pred_mu[i][h]->Reset();
      Npn_histo_NB_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_NB_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_NB_err2_pred_el_NB%i",h));
      Npn_histo_NB_err2_pred_el[i][h]->Reset();
      Npn_histo_NB_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_sr_err2_pred_mu(2, vector<TH2D*>(40,0));  
  vector< vector<TH2D*> > Npn_histo_sr_err2_pred_el(2, vector<TH2D*>(40,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<40;++h) {
      Npn_histo_sr_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_sr_err2_pred_mu_sr%i",h));
      Npn_histo_sr_err2_pred_mu[i][h]->Reset();
      Npn_histo_sr_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_sr_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_sr_err2_pred_el_sr%i",h));
      Npn_histo_sr_err2_pred_el[i][h]->Reset();
      Npn_histo_sr_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_HT_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_HT_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_HT_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_HT_err2_pred_mu_HT%i",h));
      Npn_histo_HT_err2_pred_mu[i][h]->Reset();
      Npn_histo_HT_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_HT_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_HT_err2_pred_el_HT%i",h));
      Npn_histo_HT_err2_pred_el[i][h]->Reset();
      Npn_histo_HT_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_MET_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_MET_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_MET_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_MET_err2_pred_mu_MET%i",h));
      Npn_histo_MET_err2_pred_mu[i][h]->Reset();
      Npn_histo_MET_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_MET_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_MET_err2_pred_el_MET%i",h));
      Npn_histo_MET_err2_pred_el[i][h]->Reset();
      Npn_histo_MET_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_MTMIN_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_MTMIN_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_MTMIN_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_MTMIN_err2_pred_mu_MTMIN%i",h));
      Npn_histo_MTMIN_err2_pred_mu[i][h]->Reset();
      Npn_histo_MTMIN_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_MTMIN_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_MTMIN_err2_pred_el_MTMIN%i",h));
      Npn_histo_MTMIN_err2_pred_el[i][h]->Reset();
      Npn_histo_MTMIN_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }
  
  vector< vector<TH2D*> > Npn_histo_NJET_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_NJET_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_NJET_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_NJET_err2_pred_mu_NJET%i",h));
      Npn_histo_NJET_err2_pred_mu[i][h]->Reset();
      Npn_histo_NJET_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_NJET_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_NJET_err2_pred_el_NJET%i",h));
      Npn_histo_NJET_err2_pred_el[i][h]->Reset();
      Npn_histo_NJET_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_L1ETA_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_L1ETA_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_L1ETA_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_L1ETA_err2_pred_mu_L1ETA%i",h));
      Npn_histo_L1ETA_err2_pred_mu[i][h]->Reset();
      Npn_histo_L1ETA_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_L1ETA_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_L1ETA_err2_pred_el_L1ETA%i",h));
      Npn_histo_L1ETA_err2_pred_el[i][h]->Reset();
      Npn_histo_L1ETA_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_L2ETA_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_L2ETA_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_L2ETA_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_L2ETA_err2_pred_mu_L2ETA%i",h));
      Npn_histo_L2ETA_err2_pred_mu[i][h]->Reset();
      Npn_histo_L2ETA_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_L2ETA_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_L2ETA_err2_pred_el_L2ETA%i",h));
      Npn_histo_L2ETA_err2_pred_el[i][h]->Reset();
      Npn_histo_L2ETA_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_MATCH_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_MATCH_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_MATCH_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_MATCH_err2_pred_mu_MATCH%i",h));
      Npn_histo_MATCH_err2_pred_mu[i][h]->Reset();
      Npn_histo_MATCH_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_MATCH_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_MATCH_err2_pred_el_MATCH%i",h));
      Npn_histo_MATCH_err2_pred_el[i][h]->Reset();
      Npn_histo_MATCH_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_L1PT_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_L1PT_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_L1PT_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_L1PT_err2_pred_mu_L1PT%i",h));
      Npn_histo_L1PT_err2_pred_mu[i][h]->Reset();
      Npn_histo_L1PT_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_L1PT_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_L1PT_err2_pred_el_L1PT%i",h));
      Npn_histo_L1PT_err2_pred_el[i][h]->Reset();
      Npn_histo_L1PT_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_L2PT_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_L2PT_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_L2PT_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_L2PT_err2_pred_mu_L2PT%i",h));
      Npn_histo_L2PT_err2_pred_mu[i][h]->Reset();
      Npn_histo_L2PT_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_L2PT_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_L2PT_err2_pred_el_L2PT%i",h));
      Npn_histo_L2PT_err2_pred_el[i][h]->Reset();
      Npn_histo_L2PT_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_LTrue_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_LTrue_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_LTrue_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_LTrue_err2_pred_mu_LTrue%i",h));
      Npn_histo_LTrue_err2_pred_mu[i][h]->Reset();
      Npn_histo_LTrue_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_LTrue_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_LTrue_err2_pred_el_LTrue%i",h));
      Npn_histo_LTrue_err2_pred_el[i][h]->Reset();
      Npn_histo_LTrue_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_LFake_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_LFake_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_LFake_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_LFake_err2_pred_mu_LFake%i",h));
      Npn_histo_LFake_err2_pred_mu[i][h]->Reset();
      Npn_histo_LFake_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_LFake_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_LFake_err2_pred_el_LFake%i",h));
      Npn_histo_LFake_err2_pred_el[i][h]->Reset();
      Npn_histo_LFake_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  vector< vector<TH2D*> > Npn_histo_LFakerebin_err2_pred_mu(2, vector<TH2D*>(50,0));  
  vector< vector<TH2D*> > Npn_histo_LFakerebin_err2_pred_el(2, vector<TH2D*>(50,0));  
  for (int i=0;i<2;++i) {
    for (int h=0;h<50;++h) {
      Npn_histo_LFakerebin_err2_pred_mu[i][h] = (TH2D*) rate_histo_mu->Clone(Form("Npn_histo_LFakerebin_err2_pred_mu_LFakerebin%i",h));
      Npn_histo_LFakerebin_err2_pred_mu[i][h]->Reset();
      Npn_histo_LFakerebin_err2_pred_mu[i][h]->SetDirectory(rootdir);
      Npn_histo_LFakerebin_err2_pred_el[i][h] = (TH2D*) rate_histo_e->Clone(Form("Npn_histo_LFakerebin_err2_pred_el_LFakerebin%i",h));
      Npn_histo_LFakerebin_err2_pred_el[i][h]->Reset();
      Npn_histo_LFakerebin_err2_pred_el[i][h]->SetDirectory(rootdir);
    }
  }

  //----------------------
  float prompt2_gen = 0.;     //2 prompt leptons in ss pairs
  float prompt2_reco = 0.;    //2 prompt leptons in ss pairs
  float prompt1_gen = 0.;     //1 prompt leptons in ss pairs
  float prompt1_reco = 0.;    //1 prompt leptons in ss pairs
  float prompt0_gen = 0.;     //0 prompt leptons in ss pairs
  float prompt0_reco = 0.;    //0 prompt leptons in ss pairs
  float sign_misid_gen = 0.;  //number of reco ss that are gen os
  float sign_misid_reco = 0.; // =0
  float Nss_gen=0.;           //number ss events
  float Nss_reco=0.;          //number ss events
  float NpromptL1_reco=0.;    //events where lep1 is prompt
  float NpromptL1_gen=0.;     //events where lep1 is prompt
  float NpromptL2_reco=0.;    //events where lep2 is prompt
  float NpromptL2_gen=0.;     //events where lep2 is prompt
  //----------------------

  //e determination
  float Npn = 0.; //# of prompt-nonprompt tight-tight pairs
  float Npn_s = 0.; //signal contamination for # of prompt-nonprompt tight-tight pairs
  float Nnn = 0.; //# of nonprompt-nonprompt tight-tight pairs
  //float e = 0.;  //rate = Nt/Nl
  float e1 = 0.;  //rate = Nt/Nl
  float e2 = 0.;  //rate = Nt/Nl
  float e2a = 0.;  //rate = Nt/Nl
  //----------------
  int counter = 0;  
  float Bs_e = 0;
  float notBs_e = 0;
  float Bs_mu = 0;
  float notBs_mu = 0;

  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if( nEvents >= 0 ) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // File Loop
  vector<TString> filenames;
  TString prevFilename = "";

  TH2F *hnumer = new TH2F("hnumer","", 20,25,125,5,0,2.5);
  TH2F *hdenom = new TH2F("hdenom","", 20,25,125,5,0,2.5);

  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    
    TString fname(currentFile->GetTitle());

    // Get File Content
    TFile *file = new TFile( fname );
    TTree *tree = (TTree*)file->Get("t");
    samesign.Init(tree);

    std::cout <<  " fname: " << fname <<  std::endl;
         if(fname.Contains("/TTBAR")) { fname = "TTBAR";  }
    else if(fname.Contains("/TTSL"))    { fname = "TTBAR";     }
    else if(fname.Contains("/DY"))    { fname = "DY";     }
    else if(fname.Contains("/TTW"))   { fname = "TTW";    }
    else if(fname.Contains("/TTZ"))   { fname = "TTZ";    }
    else if(fname.Contains("/WJets")) { fname = "WJets";  }
    else if(fname.Contains("/W3Jets")) { fname = "WJets";  }
    else if(fname.Contains("/W4Jets")) { fname = "WJets";  }
    else if(fname.Contains("/WZ3L"))  { fname = "WZ";     }
    else if(fname.Contains("/Data"))  { fname = "Data";   }
    else if(fname.Contains("/output.root"))  { fname = "TTBAR";   }
    else if(fname.Contains("/output_"))  { fname = "TTBAR";   }
    std::cout <<  " fname: " << fname <<  std::endl;

    if(fname != prevFilename) {
        filenames.push_back(fname);
        prevFilename = fname;
    }

    // Loop over Events in current file
    if (nEventsTotal >= nEventsChain) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for(unsigned int eventAG = 0; eventAG < nEventsTree; ++eventAG){

      // Get Event Content
      if(nEventsTotal >= nEventsChain) continue;
      samesign.GetEntry(eventAG);
      ++nEventsTotal;

      //debug
      //if (ss::event() != 48767071) continue;

      // Progress
      SSAG::progress(nEventsTotal, nEventsChain);

      // if (ss::hyp_class() == 2) {
      //     if (fabs(ss::lep1_id()) == 13 && isFakeLeg(1)) {
      //         hdenom->Fill(ss::lep1_coneCorrPt(),fabs(ss::lep1_p4().eta()), ss::scale1fb());
      //         if (ss::lep1_passes_id()) {
      //             hnumer->Fill(ss::lep1_coneCorrPt(),fabs(ss::lep1_p4().eta()), ss::scale1fb());
      //         }
      //     }
      //     if (fabs(ss::lep2_id()) == 13 && isFakeLeg(2)) {
      //         hdenom->Fill(ss::lep2_coneCorrPt(),fabs(ss::lep2_p4().eta()), ss::scale1fb());
      //         if (ss::lep2_passes_id()) {
      //             hnumer->Fill(ss::lep2_coneCorrPt(),fabs(ss::lep2_p4().eta()), ss::scale1fb());
      //         }
      //     }
      // }

      // if (ss::hyp_class() == 2 && ss::met() > 50) {
      //     if (ss::lep1_passes_id() && fabs(ss::lep2_id())==13) {
      //         if (ss::lep2_coneCorrPt() > 35 && ss::lep2_coneCorrPt() < 50 && fabs(ss::lep2_p4().eta()) < 2.1 && fabs(ss::lep2_p4().eta()) > 1.2) {
      //             cout << Form("%1llu %7.3f %7.3f %6.3f %6.3f %6.3f %6.3f %1i %2i %6.3f %6.3f %2.4f",
      //                     (unsigned long long)ss::event() , ss::lep2_p4().pt(),ss::lep2_coneCorrPt(),fabs(ss::lep2_p4().eta()),ss::lep2_miniIso(),
      //                     ss::lep2_p4().pt()/ss::jet_close_lep2().pt(),ss::lep2_ptrel_v1(),ss::lep2_passes_id(),ss::lep2_motherID(),
      //                     ss::met(),ss::mt(),41.3*ss::scale1fb()) << endl;
      //         }
      //     } 
      //     if (ss::lep2_passes_id() && fabs(ss::lep1_id())==13) {
      //         if (ss::lep1_coneCorrPt() > 35 && ss::lep1_coneCorrPt() < 50 && fabs(ss::lep1_p4().eta()) < 2.1 && fabs(ss::lep1_p4().eta()) > 1.2) {
      //             cout << Form("%1llu %7.3f %7.3f %6.3f %6.3f %6.3f %6.3f %1i %2i %6.3f %6.3f %2.4f",
      //                     (unsigned long long)ss::event() , ss::lep1_p4().pt(),ss::lep1_coneCorrPt(),fabs(ss::lep1_p4().eta()),ss::lep1_miniIso(),
      //                     ss::lep1_p4().pt()/ss::jet_close_lep1().pt(),ss::lep1_ptrel_v1(),ss::lep1_passes_id(),ss::lep1_motherID(),
      //                     ss::met(),ss::mt(),41.3*ss::scale1fb()) << endl;
      //         }
      //     }
      // }



      // if (ss::is_real_data() ) {
      //     DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
      //     if(is_duplicate(id)) continue;
      // }

      // Analysis Code
      float weight = 1.0; 
      if (absweight) {
          weight = ss::is_real_data() ? 1.0 : fabs(ss::scale1fb())*luminosity;
      } else {
          weight = ss::is_real_data() ? 1.0 : ss::scale1fb()*luminosity;
      }

      if (!ss::is_real_data()) {
          weight *= getTruePUw(year,ss::trueNumInt()[0]);
          // if (ss::hyp_class() == 3) {
              weight *= leptonScaleFactor(year,abs(ss::lep1_id()), ss::lep1_p4().pt(), ss::lep1_p4().eta(), ss::ht());
              weight *= leptonScaleFactor(year,abs(ss::lep2_id()), ss::lep2_p4().pt(), ss::lep2_p4().eta(), ss::ht());
          // }
          weight *= ss::weight_btagsf();
      }

      TString filename = fname;

      // // ignore MC part of chain when looking at data, except for contamination subtraction
      // if(doData && !ss::is_real_data()) weight = 0; 

      // if( !(ss::njets() >= 2 && (ss::ht() > 500 ? 1 : ss::met() > 30) ) ) continue;
      if( !(ss::njets() >= 2 && ss::met() > 50 ) ) continue;


      //require triggers
      if (!bypassTrigger) {
          if (!ss::fired_trigger()) continue;
      }
      if (doLowHT) {
          if (ss::ht()>300.) continue;
      }
      if (doHighHT) {
          if (ss::ht()<300.) continue;
      }


      int match_type = 0;
      //consider only prompt or lights
      if ((ss::lep2_motherID()==1 || ss::lep2_motherID()==0) && (ss::lep1_motherID()==1 || ss::lep1_motherID()==0)) match_type = 0;
      //consider only prompt or bs
      if ((ss::lep2_motherID()==1 || ss::lep2_motherID()==-1) && (ss::lep1_motherID()==1 || ss::lep1_motherID()==-1)) match_type = 1;
      //consider only prompt or cs
      if ((ss::lep2_motherID()==1 || ss::lep2_motherID()==-2) && (ss::lep1_motherID()==1 || ss::lep1_motherID()==-2)) match_type = 2;
      // https://github.com/cmstas/CORE/blob/master/SSSelections.cc#L1207
      if ((ss::lep2_motherID()==1 || ss::lep2_motherID()==-3) && (ss::lep1_motherID()==1 || ss::lep1_motherID()==-3)) match_type = 3;
      if ((ss::lep2_motherID()==1 || ss::lep2_motherID()==-4) && (ss::lep1_motherID()==1 || ss::lep1_motherID()==-4)) match_type = 4;

      if (doBonly) {
        //consider only prompt or bs
        if (ss::lep2_motherID()!=1 && ss::lep2_motherID()!=-1) continue;
        if (ss::lep1_motherID()!=1 && ss::lep1_motherID()!=-1) continue;
      }
      else if (doConly) {
        //consider only prompt or cs
        if (ss::lep2_motherID()!=1 && ss::lep2_motherID()!=-2) continue;
        if (ss::lep1_motherID()!=1 && ss::lep1_motherID()!=-2) continue;
      }
      else if (doLightonly) {
        //consider only prompt or lights
        if (ss::lep2_motherID()!=1 && ss::lep2_motherID()!=0) continue;
        if (ss::lep1_motherID()!=1 && ss::lep1_motherID()!=0) continue;
        //EMEnriched starts at 20 GeV

        // if (!inSitu) {
        //     if ( (abs(ss::lep1_id())==11 && ss::lep1_motherID()==0 && ss::lep1_p4().pt() < 20) || 
        //          (abs(ss::lep2_id())==11 && ss::lep2_motherID()==0 && ss::lep2_p4().pt() < 20) ) continue;
        // }

      }

      float lep1_ptrel_v1 = ss::lep1_ptrel_v1();
      float lep2_ptrel_v1 = ss::lep2_ptrel_v1();
      // assert(fabs(lep1_ptrel_v1 - computePtRel(ss::lep1_p4(),ss::jet_close_lep1(), true))<0.0001);
      // assert(fabs(lep2_ptrel_v1 - computePtRel(ss::lep2_p4(),ss::jet_close_lep2(), true))<0.0001);
      float lep1_closejetpt = ss::lep1_closeJet().pt();
      float lep2_closejetpt = ss::lep2_closeJet().pt();

      if (fabs(ss::lep1_ip3d()/ss::lep1_ip3d_err())>4.) continue;
      if (fabs(ss::lep2_ip3d()/ss::lep2_ip3d_err())>4.) continue;

      //Determine lep pT
      float lep1_pT = ss::lep1_p4().pt();
      float lep2_pT = ss::lep2_p4().pt();

      if (doHadApp) {
          if (ss::nhadronicW() != 1) continue;
      }

      if (coneCorr){
          if (!ignoreConeCorr) {
              lep1_pT = ss::lep1_coneCorrPt();
              lep2_pT = ss::lep2_coneCorrPt();
          }
      }

      // if (lep1_pT < 25 || lep2_pT < 25) continue;

      if (abs(ss::lep1_id())==11 && lep1_pT<15.) continue;
      if (abs(ss::lep2_id())==11 && lep2_pT<15.) continue;

      bool verbose = false;

      //Determine passes ID
      bool lep1_passes_id = ss::lep1_passes_id();
      bool lep2_passes_id = ss::lep2_passes_id();

      if (verbose) {
          bool lep1prompt = ss::lep1_motherID()==1;
          bool lep2prompt = ss::lep2_motherID()==1;
          bool lep1nonprompt = isFakeLeg(1);
          bool lep2nonprompt = isFakeLeg(2);
          std::cout << " ss::lep1_passes_id(): " << ss::lep1_passes_id() << " ss::lep2_passes_id(): " << ss::lep2_passes_id() << std::endl;
          std::cout << " lep1_pT: " << lep1_pT << " lep2_pT: " << lep2_pT << std::endl;
          std::cout << " lep1prompt: " << lep1prompt << " lep2prompt: " << lep2prompt << " lep1nonprompt: " << lep1nonprompt << " lep2nonprompt: " << lep2nonprompt << std::endl;
      }

      // if (!passIsolatedFO(ss::lep1_id(), ss::lep1_p4().eta(), ss::lep1_MVA(), ss::lep1_p4().pt())) continue;
      // if (!passIsolatedFO(ss::lep2_id(), ss::lep2_p4().eta(), ss::lep2_MVA(), ss::lep2_p4().pt())) continue;


      //Determine mtMin
      float mtmin = ss::mt() > ss::mt_l2() ? ss::mt_l2() : ss::mt();
      // if (coneCorr){
      //     if (!ignoreConeCorr) {
      //         float mtl1 = MT(lep1_pT, ss::lep1_p4().phi(), ss::met(), ss::metPhi());
      //         float mtl2 = MT(lep2_pT, ss::lep2_p4().phi(), ss::met(), ss::metPhi());
      //         mtmin = mtl1 > mtl2 ? mtl2 : mtl1;
      //     }
      // }

      //Determine SR and BR
      anal_type_t ac_base = analysisCategory(ss::lep1_id(), ss::lep2_id(), lep1_pT, lep2_pT);
      // int br = baselineRegion(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::lep1_id(), ss::lep2_id(), lep1_pT, lep2_pT, true);
      int br = min(ss::nbtags(),3);
      if (ss::njets() < 2) continue;
      if (ss::met() < 50) continue;
      if (br<0) continue;
      // int sr = signalRegion2016(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), mtmin, ss::lep1_id(), ss::lep2_id(), lep1_pT, lep2_pT);
      int sr = 1;

      if (verbose) std::cout << " inSitu: " << inSitu << " br: " << br << " ac_base: " << ac_base << " ss::hyp_class(): " << ss::hyp_class() << std::endl;

      // SS Z veto -- this is to match the inSitu FR derivation macro
      // if ((inSitu && !doData) && (fabs((ss::lep1_p4() + ss::lep2_p4()).M() - 91) < 15)) continue; // FIXME
      if (inSitu && (ss::hyp_class() == 4 || ss::hyp_class() == 6)) continue;
      if (ss::hyp_class() == 7) continue;

      if (abs(ss::lep1_id())==11 && ss::lep1_p4().pt()<15.) continue;
      if (abs(ss::lep2_id())==11 && ss::lep2_p4().pt()<15.) continue;
      if (abs(ss::lep1_id())==13 && ss::lep1_p4().pt()<10.) continue;
      if (abs(ss::lep2_id())==13 && ss::lep2_p4().pt()<10.) continue;

      if (onlyMuMu && (ss::hyp_type() != 0)) continue;
      if (doAbove25) {
          if (ss::lep1_p4().pt() < 25) continue;
          if (ss::lep2_p4().pt() < 25) continue;
          if (!ignoreConeCorr) {
              if (ss::lep1_coneCorrPt() < 25) continue;
              if (ss::lep2_coneCorrPt() < 25) continue;
          }
      }
      if (doAbove18) {
          if (ss::lep1_p4().pt() < 18) continue;
          if (ss::lep2_p4().pt() < 18) continue;
          if (ss::lep1_coneCorrPt() < 18) continue;
          if (ss::lep2_coneCorrPt() < 18) continue;
      }

      if (doNewP) {
          if (ss::lep1_coneCorrPt() < 25. && ss::lep1_coneCorrPt() >= 20.0) {
              if (ss::lep1_p4().pt() < 20) continue;
          } else if (ss::lep1_coneCorrPt() < 35. && ss::lep1_coneCorrPt() >= 25.0) {
              if (ss::lep1_p4().pt() < 25) continue;
          } else if (ss::lep1_coneCorrPt() >= 35.0) {
              if (ss::lep1_p4().pt() < 25) continue;
          }

          if (ss::lep2_coneCorrPt() < 25. && ss::lep2_coneCorrPt() >= 20.0) {
              if (ss::lep2_p4().pt() < 20) continue;
          } else if (ss::lep2_coneCorrPt() < 35. && ss::lep2_coneCorrPt() >= 25.0) {
              if (ss::lep2_p4().pt() < 25) continue;
          } else if (ss::lep2_coneCorrPt() >= 35.0) {
              if (ss::lep2_p4().pt() < 25) continue;
          }

      }

      //////////////////////////////////////////////////////////////////////////////////////////
      //                         RECO SAME SIGN PAIRS                                         // 
      //////////////////////////////////////////////////////////////////////////////////////////
      // if we're doing data, we want to only fill prompt-nonprompt prediction (class 2) with data
      // and leave all MC in class 3 observations (if they have truth fakes)


      if (!ss::is_real_data() && ss::hyp_class() == 3) {

          bool isLep1Prompt = ss::lep1_motherID()==1;
          bool isLep2Prompt = ss::lep2_motherID()==1;
          bool isLep1NonPrompt = isFakeLeg(1);
          bool isLep2NonPrompt = isFakeLeg(2);

        //Counters
        counter++;
        Nss_reco = Nss_reco + weight;
        if( isLep1Prompt && isLep2Prompt ){
          prompt2_reco = prompt2_reco + weight;
          NpromptL1_reco = NpromptL1_reco + weight;
          NpromptL2_reco = NpromptL2_reco + weight;
        }
        else if ( ss::lep1_motherID()==2 || ss::lep2_motherID()==2 ) sign_misid_reco += weight; 


        //1) Lep 2 is non-prompt
        else if( isLep1Prompt && isLep2NonPrompt ){ 


          prompt1_reco += weight;  
          NpromptL2_reco += weight;  

          if(sr > 0) hists[getHist("Npn_histo_sr_obs"+fname)   ]->Fill(sr, weight);
          hists[getHist("Npn_histo_NB_obs"+fname)   ]->Fill(br, weight);
          hists[getHist("Npn_histo_HT_obs"+fname)   ]->Fill(ss::ht(), weight);
          hists[getHist("Npn_histo_MET_obs"+fname)  ]->Fill(ss::met(), weight);
          hists[getHist("Npn_histo_MTMIN_obs"+fname)]->Fill(mtmin, weight);
          hists[getHist("Npn_histo_NJET_obs"+fname)]->Fill(ss::njets(), weight);
          hists[getHist("Npn_histo_L1ETA_obs"+fname)]->Fill(fabs(ss::lep1_p4().eta()), weight);
          hists[getHist("Npn_histo_L2ETA_obs"+fname)]->Fill(fabs(ss::lep2_p4().eta()), weight);
          hists[getHist("Npn_histo_MATCH_obs"+fname)]->Fill(match_type, weight);
          hists[getHist("Npn_histo_L1PT_obs"+fname) ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
          hists[getHist("Npn_histo_L2PT_obs"+fname) ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
          hists[getHist("Npn_histo_LTrue_obs"+fname) ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
          hists[getHist("Npn_histo_LFake_obs"+fname) ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
          hists[getHist("Npn_histo_LFakerebin_obs"+fname) ]->Fill(min(coneCorr ? lep2_pT : ss::lep2_p4().pt(),69.F), weight);

          if(abs(ss::lep2_id()) == 11){

            if(sr > 0) hists[getHist("Npn_histo_sr_obs_el"+fname)   ]->Fill(sr, weight);
            hists[getHist("Npn_histo_NB_obs_el"+fname)   ]->Fill(br, weight);
            hists[getHist("Npn_histo_HT_obs_el"+fname)   ]->Fill(ss::ht(), weight);
            hists[getHist("Npn_histo_MET_obs_el"+fname)  ]->Fill(ss::met(), weight);
            hists[getHist("Npn_histo_MTMIN_obs_el"+fname)]->Fill(mtmin, weight);
            hists[getHist("Npn_histo_NJET_obs_el"+fname)]->Fill(ss::njets(), weight);
            hists[getHist("Npn_histo_L1ETA_obs_el"+fname)]->Fill(fabs(ss::lep1_p4().eta()), weight);
            hists[getHist("Npn_histo_L2ETA_obs_el"+fname)]->Fill(fabs(ss::lep2_p4().eta()), weight);
            hists2[getHist2("Npn_histo_PTETA_obs_el"+fname)]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            hists[getHist("Npn_histo_MATCH_obs_el"+fname)]->Fill(match_type, weight);
            hists[getHist("Npn_histo_L1PT_obs_el"+fname) ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_L2PT_obs_el"+fname) ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
            hists[getHist("Npn_histo_LTrue_obs_el"+fname) ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_LFake_obs_el"+fname) ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
            hists[getHist("Npn_histo_LFakerebin_obs_el"+fname) ]->Fill(min(coneCorr ? lep2_pT : ss::lep2_p4().pt(),69.F), weight);

            addToCounter(filename+Form("_obs_el_NB%i", br), weight);
            if (fabs(ss::lep2_p4().eta()) < 0.8 && lep2_pT >= 70) addToCounter(filename+"_obs_el_pteta2", 1);
          } 
          else if(abs(ss::lep2_id()) == 13){
            if(sr > 0) hists[getHist("Npn_histo_sr_obs_mu"+fname)   ]->Fill(sr, weight);
            hists[getHist("Npn_histo_NB_obs_mu"+fname)   ]->Fill(br, weight);
            hists[getHist("Npn_histo_HT_obs_mu"+fname)   ]->Fill(ss::ht(), weight);
            hists[getHist("Npn_histo_MET_obs_mu"+fname)  ]->Fill(ss::met(), weight);
            hists[getHist("Npn_histo_MTMIN_obs_mu"+fname)]->Fill(mtmin, weight);
            hists[getHist("Npn_histo_NJET_obs_mu"+fname)]->Fill(ss::njets(), weight);
            hists[getHist("Npn_histo_L1ETA_obs_mu"+fname)]->Fill(fabs(ss::lep1_p4().eta()), weight);
            hists[getHist("Npn_histo_L2ETA_obs_mu"+fname)]->Fill(fabs(ss::lep2_p4().eta()), weight);
            hists2[getHist2("Npn_histo_PTETA_obs_mu"+fname)]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), weight);
            hists[getHist("Npn_histo_MATCH_obs_mu"+fname)]->Fill(match_type, weight);
            hists[getHist("Npn_histo_L1PT_obs_mu"+fname) ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_L2PT_obs_mu"+fname) ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
            hists[getHist("Npn_histo_LTrue_obs_mu"+fname) ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_LFake_obs_mu"+fname) ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
            hists[getHist("Npn_histo_LFakerebin_obs_mu"+fname) ]->Fill(min(coneCorr ? lep2_pT : ss::lep2_p4().pt(),69.F), weight);

            addToCounter(filename+Form("_obs_mu_NB%i", br), weight);
            if (fabs(ss::lep2_p4().eta()) < 1.2 && lep2_pT >= 50 && lep2_pT < 70) {
                addToCounter(filename+"_obs_mu_pteta2", weight);
                // addToCounter(Form("%i:%llu",ss::lumi(), ss::event()));
            }

          }
        }

        //2) Lep 1 is non-prompt
        else if( isLep1NonPrompt && isLep2Prompt ){ 


          prompt1_reco += weight;
          NpromptL2_reco += weight;

          if(sr > 0) hists[getHist("Npn_histo_sr_obs"+fname)]   ->Fill(sr, weight);
          hists[getHist("Npn_histo_NB_obs"+fname)]   ->Fill(br, weight);
          hists[getHist("Npn_histo_HT_obs"+fname)]   ->Fill(ss::ht(), weight);
          hists[getHist("Npn_histo_MET_obs"+fname)]  ->Fill(ss::met(), weight);
          hists[getHist("Npn_histo_MTMIN_obs"+fname)]->Fill(mtmin, weight);
          hists[getHist("Npn_histo_NJET_obs"+fname)]->Fill(ss::njets(), weight);
          hists[getHist("Npn_histo_L1ETA_obs"+fname)]->Fill(fabs(ss::lep1_p4().eta()), weight);
          hists[getHist("Npn_histo_L2ETA_obs"+fname)]->Fill(fabs(ss::lep2_p4().eta()), weight);
          hists[getHist("Npn_histo_MATCH_obs"+fname)]->Fill(match_type, weight);
          hists[getHist("Npn_histo_L1PT_obs"+fname) ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
          hists[getHist("Npn_histo_L2PT_obs"+fname) ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
          hists[getHist("Npn_histo_LFake_obs"+fname) ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
          hists[getHist("Npn_histo_LFakerebin_obs"+fname) ]->Fill(min(coneCorr ? lep1_pT : ss::lep1_p4().pt(),69.F), weight);
          hists[getHist("Npn_histo_LTrue_obs"+fname) ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
          
          if(abs(ss::lep1_id()) == 11){

            if(sr > 0) hists[getHist("Npn_histo_sr_obs_el"+fname)]   ->Fill(sr, weight);
            hists[getHist("Npn_histo_NB_obs_el"+fname)]   ->Fill(br, weight);
            hists[getHist("Npn_histo_HT_obs_el"+fname)]   ->Fill(ss::ht(), weight);
            hists[getHist("Npn_histo_MET_obs_el"+fname)]  ->Fill(ss::met(), weight);
            hists[getHist("Npn_histo_MTMIN_obs_el"+fname)]->Fill(mtmin, weight);
            hists[getHist("Npn_histo_NJET_obs_el"+fname)]->Fill(ss::njets(), weight);
            hists[getHist("Npn_histo_L1ETA_obs_el"+fname)]->Fill(fabs(ss::lep1_p4().eta()), weight);
            hists[getHist("Npn_histo_L2ETA_obs_el"+fname)]->Fill(fabs(ss::lep2_p4().eta()), weight);
            hists2[getHist2("Npn_histo_PTETA_obs_el"+fname)]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            hists[getHist("Npn_histo_MATCH_obs_el"+fname)]->Fill(match_type, weight);
            hists[getHist("Npn_histo_L1PT_obs_el"+fname)] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_L2PT_obs_el"+fname)] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
            hists[getHist("Npn_histo_LFake_obs_el"+fname) ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_LFakerebin_obs_el"+fname) ]->Fill(min(coneCorr ? lep1_pT : ss::lep1_p4().pt(),69.F), weight);
            hists[getHist("Npn_histo_LTrue_obs_el"+fname) ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
            
            addToCounter(filename+Form("_obs_el_NB%i", br), weight);
            if (fabs(ss::lep1_p4().eta()) < 0.8 && lep1_pT >= 70) {
                addToCounter(filename+"_obs_el_pteta1", 1);
                // addToCounter(Form("%i:%llu",ss::lumi(), ss::event()));
            }

          } 
          else if(abs(ss::lep1_id()) == 13){
            if(sr > 0) hists[getHist("Npn_histo_sr_obs_mu"+fname)]   ->Fill(sr, weight);
            hists[getHist("Npn_histo_NB_obs_mu"+fname)]   ->Fill(br, weight);
            hists[getHist("Npn_histo_HT_obs_mu"+fname)]   ->Fill(ss::ht(), weight);
            hists[getHist("Npn_histo_MET_obs_mu"+fname)]  ->Fill(ss::met(), weight);
            hists[getHist("Npn_histo_MTMIN_obs_mu"+fname)]->Fill(mtmin, weight);
            hists[getHist("Npn_histo_NJET_obs_mu"+fname)]->Fill(ss::njets(), weight);
            hists[getHist("Npn_histo_L1ETA_obs_mu"+fname)]->Fill(fabs(ss::lep1_p4().eta()), weight);
            hists[getHist("Npn_histo_L2ETA_obs_mu"+fname)]->Fill(fabs(ss::lep2_p4().eta()), weight);
            hists2[getHist2("Npn_histo_PTETA_obs_mu"+fname)]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), weight);
            hists[getHist("Npn_histo_MATCH_obs_mu"+fname)]->Fill(match_type, weight);
            hists[getHist("Npn_histo_L1PT_obs_mu"+fname)] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_L2PT_obs_mu"+fname)] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);
            hists[getHist("Npn_histo_LFake_obs_mu"+fname) ]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), weight);
            hists[getHist("Npn_histo_LFakerebin_obs_mu"+fname) ]->Fill(min(coneCorr ? lep1_pT : ss::lep1_p4().pt(),69.F), weight);
            hists[getHist("Npn_histo_LTrue_obs_mu"+fname) ]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), weight);

            addToCounter(filename+Form("_obs_mu_NB%i", br), weight);
            if (fabs(ss::lep1_p4().eta()) < 1.2 && lep1_pT >= 50 && lep1_pT < 70) addToCounter(filename+"_obs_mu_pteta1", weight);
          }
        }

        //Both are non-prompt
        else if( isLep1NonPrompt && isLep2NonPrompt ) prompt0_reco += weight;

        //check for charge misID on gen level.
        if (ss::lep1_motherID()==2 || ss::lep2_motherID()==2) sign_misid_gen += weight;
        else {
          Nss_gen += weight;
          if( ss::lep1_motherID()==1 && ss::lep2_motherID()==1 ){
            prompt2_gen += weight;
            NpromptL1_gen += weight;
            NpromptL2_gen += weight;
          }
          else if( ss::lep1_motherID()==1 && isFakeLeg(2) ){
            prompt1_gen += weight;
            NpromptL1_gen += weight;
          }
          else if( isFakeLeg(1) && ss::lep2_motherID()==1 ){
            prompt1_gen += weight;
            NpromptL2_gen += weight;
          }
          else if( (ss::lep1_motherID()!=1 && ss::lep2_motherID()!=1) ) prompt0_gen += weight;
        }

      }
      
      // Contamination subtraction: Subtract prompt-prompt MC 
      // (which ends up as class 2 instead of 3) from data prediction by assigning negative weight
      bool subtractContamination = false;
      float mult = 1.0;
      if(doData && doSubtractContamination && !ss::is_real_data() && ss::hyp_class() == 2 && ss::lep1_motherID()==1 && ss::lep2_motherID()==1) {
        mult = -1.0; // invert weight to subtract from the prediction
        subtractContamination = true;
        filename = "Data"; // when filling counter below, want it to think we are filling in negative data
      }

      //////////////////////////////////////////////////////////////////////////////////////////
      //                         SINGLE FAKES                                                 // 
      //////////////////////////////////////////////////////////////////////////////////////////

      e1 = 0.; //rate for lep1
      e2 = 0.; //rate for lep2
      float w = 0.;


      //prompt-nonprompt background
      // if we're doing data, we want to only fill prompt-nonprompt prediction (class 2) with data
      // and leave all MC in class 3 observations (if they have truth fakes)
      if( ( doData && (ss::is_real_data() && (ss::hyp_class() == 2 || inSitu)) ) || // if doing data, only fill pred with data
          (!doData && (ss::hyp_class() == 2 || inSitu) ) || // if not doing data, fill pred with MC (old behaviour)
          (doData && subtractContamination && !ss::is_real_data()) ) { // do contamination subtraction if doData and MC
          

        int nbjets = ss::nbtags();
        if (nbjets > 3) nbjets = 3; 

        //0) InSituFR variables
      float ptrel_cut_1    = (abs(ss::lep1_id()) == 11 ? 9.20 : 7.50);
      float ptrel_cut_2    = (abs(ss::lep2_id()) == 11 ? 9.20 : 7.50);
      float ptratio_cut_1  = (abs(ss::lep1_id()) == 11 ? 0.85 : 0.80);
      float ptratio_cut_2  = (abs(ss::lep2_id()) == 11 ? 0.85 : 0.80);
      float mini_cut_1     = (abs(ss::lep1_id()) == 11 ? 0.09 : 0.12);
      float mini_cut_2     = (abs(ss::lep2_id()) == 11 ? 0.09 : 0.12);
      bool lep1_denom_iso  = ((ss::lep1_miniIso() < 0.4) && ((ss::lep1_ptrel_v1() > ptrel_cut_1) || ((ss::lep1_closeJet().pt()/ss::lep1_p4().pt()) < (1.0/ptratio_cut_1 + ss::lep1_miniIso()))));
      bool lep2_denom_iso  = ((ss::lep2_miniIso() < 0.4) && ((ss::lep2_ptrel_v1() > ptrel_cut_2) || ((ss::lep2_closeJet().pt()/ss::lep2_p4().pt()) < (1.0/ptratio_cut_2 + ss::lep2_miniIso()))));


      // // for new ntuples, we must re-compute the IDs (and apply MVA manually) because I have taken out the tight MVA at the babymaking level
      bool inSituFR_id_lep1 = ss::passed_id_inSituFR_lep1();
      bool inSituFR_id_lep2 = ss::passed_id_inSituFR_lep2();

      if (inSitu && (!inSituFR_id_lep1 || !inSituFR_id_lep2)) continue; // recomputed versions of variables above

      if (inSitu) {
          // normally, !lep1_passes_id means it is loose (conventional definition),
          // but we want !lep1_passes_id to mean loose (insitu definition)
          if (!inSituFR_id_lep1 || !lep1_denom_iso) lep1_passes_id = false;
          if (!inSituFR_id_lep2 || !lep2_denom_iso) lep2_passes_id = false;
      }

        //1) Lep1 is tight, lep2 is loose!tight
        if (lep1_passes_id && !lep2_passes_id){

          //Baseline selections
          if (!inSitu && (ss::hyp_class() != 2)) continue;
          if (inSitu && (ss::lep2_multiIso() || !isFakeLeg(2, doData) || !isGoodLeg(1, doData) || !lep2_denom_iso)) continue;

          if (abs(ss::lep2_id()) == 11){  

            e2 = getFakeRate(11, lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false, doData, inSitu );
            w = (e2/(1-e2))*weight;
            if(weightOne) w = 1.0;
            if(subtractContamination) w = mult*weight;

            addToCounter(filename+Form("_pred_el_NB%i", br), w);
            if (fabs(ss::lep2_p4().eta()) < 0.8 && lep2_pT >= 70) addToCounter(filename+"_pred_el_pteta2", w);
            if (fabs(ss::lep2_p4().eta()) < 0.8 && lep2_pT >= 70) addToCounter(filename+"_prednotf_el_pteta2", weight);

            if(sr > 0) hists[getHist("Npn_histo_sr_pred_el")]   ->Fill(sr, w);
            if(sr > 0) hists[getHist("Npn_histo_sr_pred_unw_el")]   ->Fill(sr, weight);
            hists[getHist("Npn_histo_NB_pred_el")]   ->Fill(br, w);
            hists[getHist("Npn_histo_HT_pred_el")]   ->Fill(ss::ht(), w);
            hists[getHist("Npn_histo_MET_pred_el")]  ->Fill(ss::met(), w);
            hists[getHist("Npn_histo_MTMIN_pred_el")]->Fill(mtmin, w);
            hists[getHist("Npn_histo_NJET_pred_el")]->Fill(ss::njets(), w);
            hists[getHist("Npn_histo_L1ETA_pred_el")]->Fill(fabs(ss::lep1_p4().eta()), w);
            hists[getHist("Npn_histo_L2ETA_pred_el")]->Fill(fabs(ss::lep2_p4().eta()), w);
            hists2[getHist2("Npn_histo_PTETA_pred_el")]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            hists[getHist("Npn_histo_MATCH_pred_el")]->Fill(match_type, w);
            hists[getHist("Npn_histo_L1PT_pred_el")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            hists[getHist("Npn_histo_L2PT_pred_el")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
            hists[getHist("Npn_histo_LTrue_pred_el")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            if(isFakeLeg(2)) hists[getHist("Npn_histo_LFake_pred_el")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
            if(isFakeLeg(2)) hists[getHist("Npn_histo_LFakerebin_pred_el")] ->Fill(min(coneCorr ? lep2_pT : ss::lep2_p4().pt(),69.F), w);
            if (sr>=0) Npn_histo_sr_err2_pred_el[(ss::ht() > 300)][sr-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_NB_err2_pred_el[ss::ht() > 300][br]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_HT_err2_pred_el[(ss::ht() > 300)][hists[getHist("Npn_histo_HT_pred_el")]->FindBin(ss::ht())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_MET_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_MET_pred_el")]->FindBin(ss::met())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_MTMIN_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_MTMIN_pred_el")]->FindBin(mtmin)-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_NJET_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_NJET_pred_el")]->FindBin(ss::njets())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_L1ETA_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_L1ETA_pred_el")]->FindBin(fabs(ss::lep1_p4().eta()))-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_L2ETA_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_L2ETA_pred_el")]->FindBin(fabs(ss::lep2_p4().eta()))-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_MATCH_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_MATCH_pred_el")]->FindBin(match_type)-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_L1PT_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_L1PT_pred_el")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_L2PT_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_L2PT_pred_el")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_LTrue_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_LTrue_pred_el")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_LFake_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_LFake_pred_el")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            // fill el abundance histos here w/ nbtags
            if(ss::lep2_motherID() == -1) hists[getHist("NBs_NB_histo_e")]->Fill(nbjets, w);
            if(ss::lep2_motherID() == -2 || ss::lep2_motherID() == 0) hists[getHist("NnotBs_NB_histo_e")]->Fill(nbjets, w);
            if(ss::lep2_motherID() == -1) Bs_e = Bs_e + mult*weight;
            if(ss::lep2_motherID() == -2 || ss::lep2_motherID() == 0) notBs_e = notBs_e + mult*weight;
          }
          else if (abs(ss::lep2_id()) == 13){ 
            e2 = getFakeRate(13, lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false, doData, inSitu );
            w = (e2/(1-e2))*weight;
            if(weightOne) w = 1.0;
            if(subtractContamination) w = mult*weight;

            addToCounter(filename+Form("_pred_mu_NB%i", br), w);
            if (fabs(ss::lep2_p4().eta()) < 1.2 && lep2_pT >= 50 && lep2_pT < 70) addToCounter(filename+"_pred_mu_pteta2", w);
            if (fabs(ss::lep2_p4().eta()) < 1.2 && lep2_pT >= 50 && lep2_pT < 70) addToCounter(filename+"_prednotf_mu_pteta2", weight);

            if(sr > 0) hists[getHist("Npn_histo_sr_pred_mu")]->Fill(sr, w);
            if(sr > 0) hists[getHist("Npn_histo_sr_pred_unw_mu")]->Fill(sr, weight);
            hists[getHist("Npn_histo_NB_pred_mu")]->Fill(br, w);
            hists[getHist("Npn_histo_HT_pred_mu")]->Fill(ss::ht(), w);
            hists[getHist("Npn_histo_MET_pred_mu")]->Fill(ss::met(), w);
            hists[getHist("Npn_histo_MTMIN_pred_mu")]->Fill(mtmin, w);
            hists[getHist("Npn_histo_NJET_pred_mu")]->Fill(ss::njets(), w);
            hists[getHist("Npn_histo_L1ETA_pred_mu")]->Fill(fabs(ss::lep1_p4().eta()), w);
            hists[getHist("Npn_histo_L2ETA_pred_mu")]->Fill(fabs(ss::lep2_p4().eta()), w);
            hists2[getHist2("Npn_histo_PTETA_pred_mu")]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            hists[getHist("Npn_histo_MATCH_pred_mu")]->Fill(match_type, w);
            hists[getHist("Npn_histo_L1PT_pred_mu")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            hists[getHist("Npn_histo_L2PT_pred_mu")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
            hists[getHist("Npn_histo_LTrue_pred_mu")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            if(isFakeLeg(2)) hists[getHist("Npn_histo_LFake_pred_mu")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
            if(isFakeLeg(2)) hists[getHist("Npn_histo_LFakerebin_pred_mu")] ->Fill(min(coneCorr ? lep2_pT : ss::lep2_p4().pt(),69.F), w);
            if (sr>=0) Npn_histo_sr_err2_pred_mu[(ss::ht() > 300)][sr-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_NB_err2_pred_mu[ss::ht() > 300][br]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_HT_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_HT_pred_mu")]->FindBin(ss::ht())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_MET_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_MET_pred_mu")]->FindBin(ss::met())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_MTMIN_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_MTMIN_pred_mu")]->FindBin(mtmin)-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_NJET_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_NJET_pred_mu")]->FindBin(ss::njets())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_L1ETA_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_L1ETA_pred_mu")]->FindBin(fabs(ss::lep1_p4().eta()))-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_L2ETA_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_L2ETA_pred_mu")]->FindBin(fabs(ss::lep2_p4().eta()))-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_MATCH_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_MATCH_pred_mu")]->FindBin(match_type)-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_L1PT_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_L1PT_pred_mu")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_L2PT_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_L2PT_pred_mu")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_LTrue_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_LTrue_pred_mu")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            Npn_histo_LFake_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_LFake_pred_mu")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep2_pT, fabs(ss::lep2_p4().eta()), w);
            if(ss::lep2_motherID() == -1) hists[getHist("NBs_NB_histo_mu")]->Fill(nbjets, mult*weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
            if(ss::lep2_motherID() == -2 || ss::lep2_motherID() == 0) hists[getHist("NnotBs_NB_histo_mu")]->Fill(nbjets, w);
            if(ss::lep2_motherID() == -1) Bs_mu = Bs_mu + mult*weight;
            if(ss::lep2_motherID() == -2 || ss::lep2_motherID() == 0) notBs_mu = notBs_mu + mult*weight;
          }
          Npn = Npn + w;
          if (ss::lep2_motherID()==1) Npn_s = Npn_s + w;
          if(sr > 0) hists[getHist("Npn_histo_sr_pred")]->Fill(sr, w);
          if(sr > 0) hists[getHist("Npn_histo_sr_pred_unw")]->Fill(sr, weight);
          hists[getHist("Npn_histo_NB_pred")]->Fill(br, w);
          hists[getHist("Npn_histo_HT_pred")]->Fill(ss::ht(), w);
          hists[getHist("Npn_histo_MET_pred")]->Fill(ss::met(), w);
          hists[getHist("Npn_histo_MTMIN_pred")]->Fill(mtmin, w);
          hists[getHist("Npn_histo_NJET_pred")]->Fill(ss::njets(), w);
          hists[getHist("Npn_histo_L1ETA_pred")]->Fill(fabs(ss::lep1_p4().eta()), w);
          hists[getHist("Npn_histo_L2ETA_pred")]->Fill(fabs(ss::lep2_p4().eta()), w);
          hists[getHist("Npn_histo_MATCH_pred")]->Fill(match_type, w);
          hists[getHist("Npn_histo_L1PT_pred")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
          hists[getHist("Npn_histo_L2PT_pred")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
          hists[getHist("Npn_histo_LTrue_pred")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
          if(isFakeLeg(2)) hists[getHist("Npn_histo_LFake_pred")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
          if(isFakeLeg(2)) hists[getHist("Npn_histo_LFakerebin_pred_mu")] ->Fill(min(coneCorr ? lep2_pT : ss::lep2_p4().pt(),69.F), w);
        }

        //2) Lep1 is loose!tight, lep2 is tight
        if (!lep1_passes_id && lep2_passes_id){

          //Baseline selections
          if (!inSitu && ss::hyp_class() != 2) continue;
          if (inSitu && (ss::lep1_multiIso() || !isFakeLeg(1, doData) || !isGoodLeg(2, doData) || !lep1_denom_iso)) continue;

          // if(doData && !ss::is_real_data() && isGoodLeg(1)) weight = -ss::scale1fb(); 
          // else weight = 0;


          if( abs(ss::lep1_id()) == 11 ){	//if el, use el rate.  FILL WITH NONPROMPT			  

            e1 = getFakeRate(11, lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false, doData, inSitu );
            w = (e1/(1-e1))*weight;
            if(weightOne) w = 1.0;
            if(subtractContamination) w = mult*weight;

            addToCounter(filename+Form("_pred_el_NB%i", br), w);
            if (fabs(ss::lep1_p4().eta()) < 0.8 && lep1_pT >= 70) addToCounter(filename+"_pred_el_pteta1", w);
            if (fabs(ss::lep1_p4().eta()) < 0.8 && lep1_pT >= 70) addToCounter(filename+"_prednotf_el_pteta1", weight);

            if(sr > 0) hists[getHist("Npn_histo_sr_pred_el")]   ->Fill(sr, w);
            if(sr > 0) hists[getHist("Npn_histo_sr_pred_unw_el")]   ->Fill(sr, weight);
            hists[getHist("Npn_histo_NB_pred_el")]   ->Fill(br, w);
            hists[getHist("Npn_histo_HT_pred_el")]   ->Fill(ss::ht(), w);
            hists[getHist("Npn_histo_MET_pred_el")]  ->Fill(ss::met(), w);
            hists[getHist("Npn_histo_MTMIN_pred_el")]->Fill(mtmin, w);
            hists[getHist("Npn_histo_NJET_pred_el")]->Fill(ss::njets(), w);
            hists[getHist("Npn_histo_L1ETA_pred_el")]->Fill(fabs(ss::lep1_p4().eta()), w);
            hists[getHist("Npn_histo_L2ETA_pred_el")]->Fill(fabs(ss::lep2_p4().eta()), w);
            hists2[getHist2("Npn_histo_PTETA_pred_el")]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            hists[getHist("Npn_histo_MATCH_pred_el")]->Fill(match_type, w);
            hists[getHist("Npn_histo_L1PT_pred_el")] ->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            hists[getHist("Npn_histo_L2PT_pred_el")] ->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
            if(isFakeLeg(1)) hists[getHist("Npn_histo_LFake_pred_el")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            if(isFakeLeg(1)) hists[getHist("Npn_histo_LFakerebin_pred_el")]->Fill(min(coneCorr ? lep1_pT : ss::lep1_p4().pt(),69.F), w);
            hists[getHist("Npn_histo_LTrue_pred_el")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);

            if (sr>=0) Npn_histo_sr_err2_pred_el[(ss::ht() > 300)][sr-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_NB_err2_pred_el[ss::ht() > 300][br]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_HT_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_HT_pred_el")]->FindBin(ss::ht())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_MET_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_MET_pred_el")]->FindBin(ss::met())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_MTMIN_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_MTMIN_pred_el")]->FindBin(mtmin)-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_NJET_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_NJET_pred_el")]->FindBin(ss::njets())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_L1ETA_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_L1ETA_pred_el")]->FindBin(fabs(ss::lep1_p4().eta()))-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_L2ETA_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_L2ETA_pred_el")]->FindBin(fabs(ss::lep2_p4().eta()))-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_MATCH_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_MATCH_pred_el")]->FindBin(match_type)-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_L1PT_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_L1PT_pred_el")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_L2PT_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_L2PT_pred_el")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_LFake_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_LFake_pred_el")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_LTrue_err2_pred_el[ss::ht() > 300][hists[getHist("Npn_histo_LTrue_pred_el")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            // fill el abundance histos here w/ nbtags
            if(ss::lep1_motherID() == -1) hists[getHist("NBs_NB_histo_e")]->Fill(nbjets, mult*weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
            if(ss::lep1_motherID() == -2 || ss::lep1_motherID() == 0) hists[getHist("NnotBs_NB_histo_e")]->Fill(nbjets, w);
            if(ss::lep1_motherID() == -1) Bs_e = Bs_e + mult*weight;
            if(ss::lep1_motherID() == -2 || ss::lep1_motherID() == 0) notBs_e = notBs_e + mult*weight;
          }
          else if( abs(ss::lep1_id()) == 13 ){ //if mu, use mu rate.  FILL WITH NONPROMPT				  
            e1 = getFakeRate(13, lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false, doData, inSitu );
            w = (e1/(1-e1))*weight;
            if(weightOne) w = 1.0;
            if(subtractContamination) w = mult*weight;

            addToCounter(filename+Form("_pred_mu_NB%i", br), w);
            if (fabs(ss::lep1_p4().eta()) < 1.2 && lep1_pT >= 50 && lep1_pT < 70) addToCounter(filename+"_pred_mu_pteta1", w);
            if (fabs(ss::lep1_p4().eta()) < 1.2 && lep1_pT >= 50 && lep1_pT < 70) addToCounter(filename+"_prednotf_mu_pteta1", weight);

            if(sr > 0) hists[getHist("Npn_histo_sr_pred_mu")]->Fill(sr, w);
            if(sr > 0) hists[getHist("Npn_histo_sr_pred_unw_mu")]->Fill(sr, weight);
            hists[getHist("Npn_histo_NB_pred_mu")]->Fill(br, w);
            hists[getHist("Npn_histo_HT_pred_mu")]->Fill(ss::ht(), w);
            hists[getHist("Npn_histo_MET_pred_mu")]->Fill(ss::met(), w);
            hists[getHist("Npn_histo_MTMIN_pred_mu")]->Fill(mtmin, w);
            hists[getHist("Npn_histo_NJET_pred_mu")]->Fill(ss::njets(), w);
            hists[getHist("Npn_histo_L1ETA_pred_mu")]->Fill(fabs(ss::lep1_p4().eta()), w);
            hists[getHist("Npn_histo_L2ETA_pred_mu")]->Fill(fabs(ss::lep2_p4().eta()), w);
            hists2[getHist2("Npn_histo_PTETA_pred_mu")]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            hists[getHist("Npn_histo_MATCH_pred_mu")]->Fill(match_type, w);
            hists[getHist("Npn_histo_L1PT_pred_mu")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            hists[getHist("Npn_histo_L2PT_pred_mu")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
            if(isFakeLeg(1)) hists[getHist("Npn_histo_LFake_pred_mu")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
            if(isFakeLeg(1)) hists[getHist("Npn_histo_LFakerebin_pred_mu")]->Fill(min(coneCorr ? lep1_pT : ss::lep1_p4().pt(),69.F), w);
            hists[getHist("Npn_histo_LTrue_pred_mu")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);

            if (sr>=0) Npn_histo_sr_err2_pred_mu[(ss::ht() > 300)][sr-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_NB_err2_pred_mu[ss::ht() > 300][br]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_HT_err2_pred_mu[(ss::ht() > 300)][hists[getHist("Npn_histo_HT_pred_mu")]->FindBin(ss::ht())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_MET_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_MET_pred_mu")]->FindBin(ss::met())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_MTMIN_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_MTMIN_pred_mu")]->FindBin(mtmin)-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_NJET_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_NJET_pred_mu")]->FindBin(ss::njets())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_L1ETA_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_L1ETA_pred_mu")]->FindBin(fabs(ss::lep1_p4().eta()))-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_L2ETA_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_L2ETA_pred_mu")]->FindBin(fabs(ss::lep2_p4().eta()))-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_MATCH_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_MATCH_pred_mu")]->FindBin(match_type)-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_L1PT_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_L1PT_pred_mu")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_L2PT_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_L2PT_pred_mu")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_LFake_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_LFake_pred_mu")]->FindBin(coneCorr ? lep1_pT : ss::lep1_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            Npn_histo_LTrue_err2_pred_mu[ss::ht() > 300][hists[getHist("Npn_histo_LTrue_pred_mu")]->FindBin(coneCorr ? lep2_pT : ss::lep2_p4().pt())-1]->Fill(lep1_pT, fabs(ss::lep1_p4().eta()), w);
            // fill el abundance histos here w/ nbtags
            if(ss::lep1_motherID() == -1) hists[getHist("NBs_NB_histo_mu")]->Fill(nbjets, mult*weight); //LOOSE!TIGHT, not LOOSE LIKE IN MEAS REGION
            if(ss::lep1_motherID() == -2 || ss::lep1_motherID() == 0) hists[getHist("NnotBs_NB_histo_mu")]->Fill(nbjets, w);
            if(ss::lep1_motherID() == -1) Bs_mu = Bs_mu + mult*weight;
            if(ss::lep1_motherID() == -2 || ss::lep1_motherID() == 0) notBs_mu = notBs_mu + mult*weight;
          }
          Npn = Npn + w;
          if (ss::lep1_motherID()==1) Npn_s = Npn_s + w;
          if(sr > 0) hists[getHist("Npn_histo_sr_pred")]->Fill(sr, w);
          if(sr > 0) hists[getHist("Npn_histo_sr_pred_unw")]->Fill(sr, weight);
          hists[getHist("Npn_histo_NB_pred")]->Fill(br, w);
          hists[getHist("Npn_histo_HT_pred")]->Fill(ss::ht(), w);
          hists[getHist("Npn_histo_MET_pred")]->Fill(ss::met(), w);
          hists[getHist("Npn_histo_MTMIN_pred")]->Fill(mtmin, w);
          hists[getHist("Npn_histo_NJET_pred")]->Fill(ss::njets(), w);
          hists[getHist("Npn_histo_L1ETA_pred")]->Fill(fabs(ss::lep1_p4().eta()), w);
          hists[getHist("Npn_histo_L2ETA_pred")]->Fill(fabs(ss::lep2_p4().eta()), w);
          hists[getHist("Npn_histo_MATCH_pred")]->Fill(match_type, w);
          hists[getHist("Npn_histo_L1PT_pred")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
          hists[getHist("Npn_histo_L2PT_pred")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
          if(isFakeLeg(1)) hists[getHist("Npn_histo_LFake_pred")]->Fill(coneCorr ? lep1_pT : ss::lep1_p4().pt(), w);
          hists[getHist("Npn_histo_LTrue_pred")]->Fill(coneCorr ? lep2_pT : ss::lep2_p4().pt(), w);
        }
      } //end hyp = 2 if statement
      
      // // resume ignoring MC part of chain when looking at data, except for contamination subtraction
      // if(doData && !ss::is_real_data()) weight = 0; 

      //nonprompt-nonprompt background
      else if(ss::hyp_class() == 1){
        if( ss::lep1_id()*ss::lep2_id() > 0 ){
          if( abs(ss::lep2_id()) == 11 ) e2 = getFakeRate(11, lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false, doData, inSitu );
          else if( abs(ss::lep2_id()) == 13 ) e2 = getFakeRate(13, lep2_pT, fabs(ss::lep2_p4().eta()), ss::ht(), false, doData, inSitu );
          if( abs(ss::lep1_id()) == 11)      e1 = getFakeRate(11, lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false, doData, inSitu );
          else if( abs(ss::lep1_id()) == 13) e1 = getFakeRate(13, lep1_pT, fabs(ss::lep1_p4().eta()), ss::ht(), false, doData, inSitu );
          Nnn = Nnn + w*weight;
        }
      } //end hyp = 1 if statement

    }//end event loop

    // Clean Up
    delete tree;
    file->Close();
    delete file;

  }//file loop

  cout<<"\n"<<"Hyp_type = 3 (Both leptons pass tight selection)"<<endl;
  cout<<"---------------------------------------------"<<endl;
  cout<<setw(15)<<""<<setw(15)<<"Reco Level"<<setw(15)<<"Gen Level"<<endl;
  cout<<setw(15)<<"ss prompt2"<<setw(15)<<prompt2_reco<<setw(15)<<prompt2_gen<<endl;
  cout<<setw(15)<<"ss prompt1"<<setw(15)<<prompt1_reco<<setw(15)<<prompt1_gen<<endl;
  cout<<setw(15)<<"ss prompt0"<<setw(15)<<prompt0_reco<<setw(15)<<prompt0_gen<<endl;
  cout<<setw(15)<<"charge misID"<<setw(15)<<sign_misid_reco<<setw(15)<<sign_misid_gen<<endl;
  cout<<setw(15)<<"Nss"<<setw(15)<<Nss_reco<<setw(15)<<Nss_gen<<endl;
  cout<<"---------------------------------------------"<<endl;
  cout<<setw(15)<<"Nprompt L1"<<setw(15)<<NpromptL1_reco<<setw(15)<<NpromptL1_gen<<endl;
  cout<<setw(15)<<"Nprompt L2"<<setw(15)<<NpromptL2_reco<<setw(15)<<NpromptL2_gen<<endl;
  cout<<"---------------------------------------------"<<endl;

  cout<<setw(25)<<" "<<setw(10)<<"EST"<<setw(10)<<"OBS"<<setw(10)<<"EST/OBS"<<endl;
  cout<<setw(25)<<"Npn:"<<setw(10)<<Npn<<setw(10)<<prompt1_reco<<setw(10)<<Npn/prompt1_reco<<endl;
  cout<<setw(25)<<"Npn-Npn_s:"<<setw(10)<<Npn-Npn_s<<setw(10)<<prompt1_reco<<setw(10)<<(Npn-Npn_s)/prompt1_reco<<endl;
  cout<<setw(25)<<"Nnn:"<<setw(10)<<Nnn<<setw(10)<<prompt0_reco<<setw(10)<<Nnn/prompt0_reco<<endl;

  cout<<"\nCounter = "<<counter<<endl;
  cout<<"\nAve B abundance (els)= "<<Bs_e/(Bs_e + notBs_e)<<endl;
  cout<<"\nAve B abundance (mus)= "<<Bs_mu/(Bs_mu + notBs_mu)<<endl;

  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("1.3f");

  //redefine option to save also ptRegion in output files
  option=option+"_"+ptRegion;

  std::string plotdir="plots/";

  // TString commonOptions = Form(" --isLinear --outOfFrame --type Supplementary (Simulation) --dataName Data --noDivisionLabel --noRatioPlot --lumi %.2f --yTitleOffset -0.2", luminosity);// --systBlack --systFillStyle 3345
  TString commonOptions = Form(" --ratioUpperBound 4 --legendCounts --isLinear --outOfFrame --type Supplementary (Simulation) --dataName Prediction --noDivisionLabel --lumi %.2f --yTitleOffset -0.2 --legendTaller 0.07 --legendRight -0.06", luminosity);// --systBlack --systFillStyle 3345
  TString commonOptionsLog = Form(" --legendCounts  --outOfFrame --type Supplementary (Simulation) --dataName Prediction --noDivisionLabel --lumi %.2f --yTitleOffset -0.2 --legendTaller 0.07 --legendRight -0.06", luminosity);// --systBlack --systFillStyle 3345

  std::string mc_type = "MC";
  std::vector<Color_t> colors;

  std::vector<std::string> obs_types;
  for(unsigned int i = 0; i < filenames.size(); i++) {
      if (filenames[i] == "TTBAR") {
          obs_types.push_back("t#bar{t}");
          colors.push_back(kYellow);
      }
      else if (filenames[i] == "WJets") {
          obs_types.push_back("W+jets");
          colors.push_back(kOrange);
      }
      else {
          obs_types.push_back(filenames[i].Data());
          colors.push_back(kGreen+2);
      }
  }

  if (!doData) {


  //BR plots
  GetErrorPlot(hists[getHist("Npn_histo_NB_pred")], Npn_histo_NB_err2_pred_mu, Npn_histo_NB_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_NB_pred_mu")], Npn_histo_NB_err2_pred_mu, Npn_histo_NB_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_NB_pred_el")], Npn_histo_NB_err2_pred_mu, Npn_histo_NB_err2_pred_el, inSitu);
  // dataMCplotMaker(hists[getHist("Npn_histo_NB_pred")], getBackgrounds("Npn_histo_NB_obs", -1, filenames), obs_types, "BRs", mc_type+"", Form("--outputName %s --xAxisLabel Baseline Region --noXaxisUnit"+commonOptions, (plotdir+"br_all"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_NB_pred_mu")], getBackgrounds("Npn_histo_NB_obs", 1, filenames), obs_types, "BRs", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel Baseline Region --noXaxisUnit"+commonOptions, (plotdir+"br_mu"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_NB_pred_el")], getBackgrounds("Npn_histo_NB_obs", 0, filenames), obs_types, "BRs", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel Baseline Region --noXaxisUnit"+commonOptions, (plotdir+"br_el"+option).Data()), {}, {}, colors); 

  // //SR plots
  // string typeAG = "HH";
  // if (highlow) typeAG = "HL";
  // if (lowlow)  typeAG = "LL";
  // GetErrorPlot(hists[getHist("Npn_histo_sr_pred")], Npn_histo_sr_err2_pred_mu, Npn_histo_sr_err2_pred_el, inSitu);
  // GetErrorPlot(hists[getHist("Npn_histo_sr_pred_mu")], Npn_histo_sr_err2_pred_mu, Npn_histo_sr_err2_pred_el, inSitu);
  // GetErrorPlot(hists[getHist("Npn_histo_sr_pred_el")], Npn_histo_sr_err2_pred_mu, Npn_histo_sr_err2_pred_el, inSitu);
  // dataMCplotMaker(hists[getHist("Npn_histo_sr_pred")], getBackgrounds("Npn_histo_sr_obs",-1,filenames), obs_types, Form("%s SRs", typeAG.c_str()), mc_type+"", Form("--outputName %s --xAxisLabel SR --noXaxisUnit"+commonOptions, (plotdir+"sr_all"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_sr_pred_mu")], getBackgrounds("Npn_histo_sr_obs",1,filenames), obs_types, Form("%s SRs", typeAG.c_str()), mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel SR --noXaxisUnit"+commonOptions, (plotdir+"sr_mu"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_sr_pred_el")], getBackgrounds("Npn_histo_sr_obs",0,filenames), obs_types, Form("%s SRs", typeAG.c_str()), mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel SR --noXaxisUnit"+commonOptions, (plotdir+"sr_el"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_sr_pred")], getBackgrounds("Npn_histo_sr_obs",-1,filenames), obs_types, Form("%s SRs", typeAG.c_str()), mc_type+"", Form("--outputName %s --xAxisLabel SR --noXaxisUnit"+commonOptionsLog, (plotdir+"sr_log_all"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_sr_pred_mu")], getBackgrounds("Npn_histo_sr_obs",1,filenames), obs_types, Form("%s SRs", typeAG.c_str()), mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel SR --noXaxisUnit"+commonOptionsLog, (plotdir+"sr_log_mu"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_sr_pred_el")], getBackgrounds("Npn_histo_sr_obs",0,filenames), obs_types, Form("%s SRs", typeAG.c_str()), mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel SR --noXaxisUnit"+commonOptionsLog, (plotdir+"sr_log_el"+option).Data()), {}, {}, colors); 

  //HT plots
  GetErrorPlot(hists[getHist("Npn_histo_HT_pred")], Npn_histo_HT_err2_pred_mu, Npn_histo_HT_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_HT_pred_mu")], Npn_histo_HT_err2_pred_mu, Npn_histo_HT_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_HT_pred_el")], Npn_histo_HT_err2_pred_mu, Npn_histo_HT_err2_pred_el, inSitu);
  // dataMCplotMaker(hists[getHist("Npn_histo_HT_pred")], getBackgrounds("Npn_histo_HT_obs",-1,filenames), obs_types, "H_{T}", mc_type+"", Form("--outputName %s --xAxisLabel H_{T}"+commonOptions, (plotdir+"HT_all"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_HT_pred_mu")], getBackgrounds("Npn_histo_HT_obs",1,filenames), obs_types, "H_{T}", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel H_{T}"+commonOptions, (plotdir+"HT_mu"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_HT_pred_el")], getBackgrounds("Npn_histo_HT_obs",0,filenames), obs_types, "H_{T}", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel H_{T}"+commonOptions, (plotdir+"HT_el"+option).Data()), {}, {}, colors); 

  //MET plots
  GetErrorPlot(hists[getHist("Npn_histo_MET_pred")], Npn_histo_MET_err2_pred_mu, Npn_histo_MET_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_MET_pred_mu")], Npn_histo_MET_err2_pred_mu, Npn_histo_MET_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_MET_pred_el")], Npn_histo_MET_err2_pred_mu, Npn_histo_MET_err2_pred_el, inSitu);
  // dataMCplotMaker(hists[getHist("Npn_histo_MET_pred")], getBackgrounds("Npn_histo_MET_obs",-1,filenames), obs_types, "MET", mc_type+"", Form("--outputName %s --xAxisLabel MET"+commonOptions, (plotdir+"MET_all"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_MET_pred_mu")], getBackgrounds("Npn_histo_MET_obs",1,filenames), obs_types, "MET", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel MET"+commonOptions, (plotdir+"MET_mu"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_MET_pred_el")], getBackgrounds("Npn_histo_MET_obs",0,filenames), obs_types, "MET", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel MET"+commonOptions, (plotdir+"MET_el"+option).Data()), {}, {}, colors); 

  //MTMIN plots
  GetErrorPlot(hists[getHist("Npn_histo_MTMIN_pred")], Npn_histo_MTMIN_err2_pred_mu, Npn_histo_MTMIN_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_MTMIN_pred_mu")], Npn_histo_MTMIN_err2_pred_mu, Npn_histo_MTMIN_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_MTMIN_pred_el")], Npn_histo_MTMIN_err2_pred_mu, Npn_histo_MTMIN_err2_pred_el, inSitu);
  // dataMCplotMaker(hists[getHist("Npn_histo_MTMIN_pred")], getBackgrounds("Npn_histo_MTMIN_obs",-1,filenames), obs_types, "M_{T}^{min}", mc_type+"", Form("--outputName %s --xAxisLabel M_{T}^{min}"+commonOptions, (plotdir+"MTMIN_all"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_MTMIN_pred_mu")], getBackgrounds("Npn_histo_MTMIN_obs",1,filenames), obs_types, "M_{T}^{min}", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel M_{T}^{min}"+commonOptions, (plotdir+"MTMIN_mu"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_MTMIN_pred_el")], getBackgrounds("Npn_histo_MTMIN_obs",0,filenames), obs_types, "M_{T}^{min}", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel M_{T}^{min}"+commonOptions, (plotdir+"MTMIN_el"+option).Data()), {}, {}, colors); 
  
  //NJET plots
  GetErrorPlot(hists[getHist("Npn_histo_NJET_pred")], Npn_histo_NJET_err2_pred_mu, Npn_histo_NJET_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_NJET_pred_mu")], Npn_histo_NJET_err2_pred_mu, Npn_histo_NJET_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_NJET_pred_el")], Npn_histo_NJET_err2_pred_mu, Npn_histo_NJET_err2_pred_el, inSitu);
  // dataMCplotMaker(hists[getHist("Npn_histo_NJET_pred")], getBackgrounds("Npn_histo_NJET_obs",-1,filenames), obs_types, "N_{jet}", mc_type+"", Form("--outputName %s --xAxisLabel N_{jet}"+commonOptions, (plotdir+"NJET_all"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_NJET_pred_mu")], getBackgrounds("Npn_histo_NJET_obs",1,filenames), obs_types, "N_{jet}", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel N_{jet}"+commonOptions, (plotdir+"NJET_mu"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_NJET_pred_el")], getBackgrounds("Npn_histo_NJET_obs",0,filenames), obs_types, "N_{jet}", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel N_{jet}"+commonOptions, (plotdir+"NJET_el"+option).Data()), {}, {}, colors); 
  
  //MATCH plots
  GetErrorPlot(hists[getHist("Npn_histo_MATCH_pred")], Npn_histo_MATCH_err2_pred_mu, Npn_histo_MATCH_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_MATCH_pred_mu")], Npn_histo_MATCH_err2_pred_mu, Npn_histo_MATCH_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_MATCH_pred_el")], Npn_histo_MATCH_err2_pred_mu, Npn_histo_MATCH_err2_pred_el, inSitu);
  // dataMCplotMaker(hists[getHist("Npn_histo_MATCH_pred")], getBackgrounds("Npn_histo_MATCH_obs",-1,filenames), obs_types, "match", mc_type+"", Form("--outputName %s --xAxisLabel match"+commonOptions, (plotdir+"MATCH_all"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_MATCH_pred_mu")], getBackgrounds("Npn_histo_MATCH_obs",1,filenames), obs_types, "match", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel match"+commonOptions, (plotdir+"MATCH_mu"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_MATCH_pred_el")], getBackgrounds("Npn_histo_MATCH_obs",0,filenames), obs_types, "match", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel match"+commonOptions, (plotdir+"MATCH_el"+option).Data()), {}, {}, colors); 

  //L1PT plots
  GetErrorPlot(hists[getHist("Npn_histo_L1PT_pred")], Npn_histo_L1PT_err2_pred_mu, Npn_histo_L1PT_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_L1PT_pred_mu")], Npn_histo_L1PT_err2_pred_mu, Npn_histo_L1PT_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_L1PT_pred_el")], Npn_histo_L1PT_err2_pred_mu, Npn_histo_L1PT_err2_pred_el, inSitu);
  // dataMCplotMaker(hists[getHist("Npn_histo_L1PT_pred")], getBackgrounds("Npn_histo_L1PT_obs",-1,filenames), obs_types, "L1 p_{T}", mc_type+"", Form("--outputName %s --xAxisLabel L1 p_{T}"+commonOptions, (plotdir+"L1PT_all"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_L1PT_pred_mu")], getBackgrounds("Npn_histo_L1PT_obs",1,filenames), obs_types, "L1 p_{T}", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel L1 p_{T}"+commonOptions, (plotdir+"L1PT_mu"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_L1PT_pred_el")], getBackgrounds("Npn_histo_L1PT_obs",0,filenames), obs_types, "L1 p_{T}", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel L1 p_{T}"+commonOptions, (plotdir+"L1PT_el"+option).Data()), {}, {}, colors); 

  //L2PT plots
  GetErrorPlot(hists[getHist("Npn_histo_L2PT_pred")], Npn_histo_L2PT_err2_pred_mu, Npn_histo_L2PT_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_L2PT_pred_mu")], Npn_histo_L2PT_err2_pred_mu, Npn_histo_L2PT_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_L2PT_pred_el")], Npn_histo_L2PT_err2_pred_mu, Npn_histo_L2PT_err2_pred_el, inSitu);
  // dataMCplotMaker(hists[getHist("Npn_histo_L2PT_pred")], getBackgrounds("Npn_histo_L2PT_obs",-1,filenames), obs_types, "L2 p_{T}", mc_type+"", Form("--outputName %s --xAxisLabel L2 p_{T}"+commonOptions, (plotdir+"L2PT_all"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_L2PT_pred_mu")], getBackgrounds("Npn_histo_L2PT_obs",1,filenames), obs_types, "L2 p_{T}", mc_type+", Nonprompt muons", Form("--outputName %s --xAxisLabel L2 p_{T}"+commonOptions, (plotdir+"L2PT_mu"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_L2PT_pred_el")], getBackgrounds("Npn_histo_L2PT_obs",0,filenames), obs_types, "L2 p_{T}", mc_type+", Nonprompt electrons", Form("--outputName %s --xAxisLabel L2 p_{T}"+commonOptions, (plotdir+"L2PT_el"+option).Data()), {}, {}, colors); 
  
  //LTrue plots
  GetErrorPlot(hists[getHist("Npn_histo_LTrue_pred")], Npn_histo_LTrue_err2_pred_mu, Npn_histo_LTrue_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_LTrue_pred_mu")], Npn_histo_LTrue_err2_pred_mu, Npn_histo_LTrue_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_LTrue_pred_el")], Npn_histo_LTrue_err2_pred_mu, Npn_histo_LTrue_err2_pred_el, inSitu);
  // dataMCplotMaker(hists[getHist("Npn_histo_LTrue_pred")], getBackgrounds("Npn_histo_LTrue_obs",-1,filenames), obs_types, "LTight p_{T}", mc_type+", all", Form("--outputName %s --outOfFrame --dataName Data --xAxisLabel LTight p_{T} "+commonOptions, (plotdir+"LTrue_all"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_LTrue_pred_mu")], getBackgrounds("Npn_histo_LTrue_obs",1,filenames), obs_types, "LTight p_{T}", mc_type+", Nonprompt muons", Form("--outputName %s --outOfFrame --dataName Data --xAxisLabel LTight p_{T} "+commonOptions, (plotdir+"LTrue_mu"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_LTrue_pred_el")], getBackgrounds("Npn_histo_LTrue_obs",0,filenames), obs_types, "LTight p_{T}", mc_type+", Nonprompt electrons", Form("--outputName %s --outOfFrame --dataName Data --xAxisLabel LTight p_{T} "+commonOptions, (plotdir+"LTrue_el"+option).Data()), {}, {}, colors); 

  //LFake plots
  GetErrorPlot(hists[getHist("Npn_histo_LFake_pred")], Npn_histo_LFake_err2_pred_mu, Npn_histo_LFake_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_LFake_pred_mu")], Npn_histo_LFake_err2_pred_mu, Npn_histo_LFake_err2_pred_el, inSitu);
  GetErrorPlot(hists[getHist("Npn_histo_LFake_pred_el")], Npn_histo_LFake_err2_pred_mu, Npn_histo_LFake_err2_pred_el, inSitu);
  // dataMCplotMaker(hists[getHist("Npn_histo_LFake_pred")], getBackgrounds("Npn_histo_LFake_obs",-1,filenames), obs_types, "LLoose p_{T}", mc_type+", all", Form("--outputName %s --outOfFrame --dataName Data --xAxisLabel LLoose p_{T} "+commonOptions, (plotdir+"LFake_all"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_LFake_pred_mu")], getBackgrounds("Npn_histo_LFake_obs",1,filenames), obs_types, "LLoose p_{T}", mc_type+", Nonprompt muons", Form("--outputName %s --outOfFrame --dataName Data --xAxisLabel LLoose p_{T} "+commonOptions, (plotdir+"LFake_mu"+option).Data()), {}, {}, colors); 
  // dataMCplotMaker(hists[getHist("Npn_histo_LFake_pred_el")], getBackgrounds("Npn_histo_LFake_obs",0,filenames), obs_types, "LLoose p_{T}", mc_type+", Nonprompt electrons", Form("--outputName %s --outOfFrame --dataName Data --xAxisLabel LLoose p_{T} "+commonOptions, (plotdir+"LFake_el"+option).Data()), {}, {}, colors); 


  TH1F *total_NB_histo_e = (TH1F*)  hists[getHist("NBs_NB_histo_e")]->Clone("total_NB_histo_e");
  TH1F *total_NB_histo_mu = (TH1F*) hists[getHist("NBs_NB_histo_mu")]->Clone("total_NB_histo_mu");
  total_NB_histo_e->Add(hists[getHist("NnotBs_NB_histo_e")]);
  total_NB_histo_mu->Add(hists[getHist("NnotBs_NB_histo_mu")]);
  hists[getHist("NBs_NB_histo_e")]->Divide(hists[getHist("NBs_NB_histo_e")], total_NB_histo_e,1,1,"B");
  hists[getHist("NBs_NB_histo_mu")]->Divide(hists[getHist("NBs_NB_histo_mu")], total_NB_histo_mu,1,1,"B");
  hists[getHist("NBs_NB_histo_e")]->GetXaxis()->SetTitle("Nbtags"); 
  hists[getHist("NBs_NB_histo_e")]->GetYaxis()->SetTitle("Abundance");
  hists[getHist("NBs_NB_histo_e")]->GetYaxis()->SetRangeUser(0.,1.);
  hists[getHist("NBs_NB_histo_e")]->SetTitle("B Abundance vs Nbtags (Njets >= 2) (electrons)");
  hists[getHist("NBs_NB_histo_mu")]->GetXaxis()->SetTitle("Nbtags"); 
  hists[getHist("NBs_NB_histo_mu")]->GetYaxis()->SetTitle("Abundance");
  hists[getHist("NBs_NB_histo_mu")]->GetYaxis()->SetRangeUser(0.,1.);
  hists[getHist("NBs_NB_histo_mu")]->SetTitle("B Abundance vs Nbtags (Njets >= 2) (muons)");

  TCanvas *c6 = new TCanvas("c6","B Abundance vs Nbtags (Njets >= 2) (electrons)", 800,800);
  c6->cd();
  hists[getHist("NBs_NB_histo_e")]->Draw("histE");
  TCanvas *c7 = new TCanvas("c7","B Abundance vs Nbtags (Njets >= 2) (muons)", 800,800);
  c7->cd();
  hists[getHist("NBs_NB_histo_mu")]->Draw("histE");

  TCanvas c8;
  hists[getHist("pTrel_histo_mu")]->Draw();
  // c8.SaveAs(plotdir+"pTrel_mu"+option+".png");
  hists[getHist("pTrel_histo_el")]->Draw();
  // c8.SaveAs(plotdir+"pTrel_el"+option+".png");
  c8.SetLogz();
  pTrelvsIso_histo_mu->Draw("colz");
  // c8.SaveAs(plotdir+"pTrelvsIso_mu"+option+".png");
  pTrelvsIso_histo_el->Draw("colz");
  // c8.SaveAs(plotdir+"pTrelvsIso_el"+option+".png");
  pTrelvsMiniIso_histo_mu->Draw("colz");
  // c8.SaveAs(plotdir+"pTrelvsMiniIso_mu"+option+".png");
  pTrelvsMiniIso_histo_el->Draw("colz");
  // c8.SaveAs(plotdir+"pTrelvsMiniIso_el"+option+".png");
    }


  TFile *file = new TFile("histos_"+option+".root", "RECREATE");
  file->Write(); 
  for (unsigned int i = 0; i < hists.size(); i++){
      if(doData && TString(hists[i]->GetName()).Contains("_obs")) continue;
      hists[i]->Write();
  }
  for (unsigned int i = 0; i < hists2.size(); i++){
      hists2[i]->Write();
  }
  hnumer->Write();
  hdenom->Write();
  file->Close();

  printCounter(true);

  if (!doData) printClosureNumbers(filenames);

  return 0;
}
