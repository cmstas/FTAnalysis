#ifndef LTBABYMAKER_H
#define LTBABYMAKER_H

#include <vector>
#include <functional>
#include <algorithm>
#include <chrono>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TF1.h"
#include "TH2.h"
#include "Math/VectorUtil.h"
#include "CORE/CMS3.h"
#include "CORE/Config.h"
#include "CORE/Tools/utils.h"
#include "CORE/Tools/JetCorrector.h"
#include "CORE/SSSelections.h"
#include "CORE/ElectronSelections.h"
#include "CORE/IsolationTools.h"
#include "CORE/JetSelections.h"
#include "CORE/MetSelections.h"
#include "CORE/MCSelections.h"
#include "CORE/Tools/goodrun.h"
#include "CORE/Tools/jetcorr/JetCorrectionUncertainty.h"
#include "CORE/Tools/jetcorr/SimpleJetCorrectionUncertainty.h"
#include "CORE/Tools/datasetinfo/getDatasetInfo.h"
#include "TROOT.h"
#include "Math/Vector4D.h"
#include "Math/LorentzVector.h"
#include "TMath.h"


#ifdef __MAKECINT__
#pragma link C++ class ROOT::Math::PxPyPzE4D<float>+;
#pragma link C++ class ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >+;
#pragma link C++ typedef ROOT::Math::XYZTVectorF;
#endif
#include "Math/Vector4D.h"
#include "Math/LorentzVector.h"

#ifdef __MAKECINT__
#pragma link C++ class ROOT::Math::PxPyPzE4D<float>+;
#pragma link C++ class ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >+;
#pragma link C++ typedef ROOT::Math::XYZTVectorF;
#endif
#include "Math/Vector4D.h"
#include "Math/LorentzVector.h"

#ifdef __MAKECINT__
#pragma link C++ class ROOT::Math::PxPyPzE4D<float>+;
#pragma link C++ class ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >+;
#pragma link C++ typedef ROOT::Math::XYZTVectorF;
#endif

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

using namespace std;
using namespace tas;

struct csErr_t { float cs_scale_no = 0; float cs_scale_up = 0; float cs_scale_dn = 0; float cs_pdf[102] = {0}; int SR = -1; bool isGood = 0; };

struct Lepton {
    Lepton(int id=0, unsigned int idx=0):id_(id),idx_(idx) {
        if (id != 0) {
            pt_ = (abs(id_)==11 ? cms3.els_p4()[idx_].pt() : cms3.mus_p4()[idx_].pt());
            eta_ = (abs(id_)==11 ? cms3.els_p4()[idx_].eta() : cms3.mus_p4()[idx_].eta());
            p4_ = (abs(id_)==11 ? cms3.els_p4()[idx_] : cms3.mus_p4()[idx_]);
            idlevel_ = 0;
        }
    }
    void set_idlevel(int idlevel) {idlevel_ = idlevel;}
    int id() {return id_;}
    int absid() {return abs(id_);}
    int is_el() {return abs(id_) == 11;}
    int is_mu() {return abs(id_) == 13;}
    int charge() {return -1*id_/abs(id_);}
    unsigned int idx() {return idx_;}
    int idlevel() {return idlevel_;}
    LorentzVector p4() {return p4_;}
    float pt() {return pt_;}
    float eta() {return eta_;}
    private:
    int id_;
    float pt_ = 0.;
    float eta_ = 0.;
    LorentzVector p4_;
    unsigned int idx_;
    int idlevel_ = 0;
};
typedef std::vector<Lepton> Leptons;

// std::ostream& operator << (std::ostream& os, Lepton& lep)
// {
//     std::string lepstr = (abs(lep.id())==11) ? "Electron" : "Muon";
//     return os << "<" << lepstr << " id=" << std::showpos << setw(3) << lep.id() << std::noshowpos
//               << ", idx=" << setw(2) << lep.idx() << ", level=" << lep.idlevel()
//               << ", (pT,eta)=" << "(" << lep.pt() << ","<< lep.eta() << ")>";
// }

//Classes
class babyMaker {

  public:
    babyMaker(bool debug = 0) {
      path = ".";
      verbose = debug;
      evt_cut = 0;
    }
    void fillTriggerBranches();
    void MakeBabyNtuple(const char* output_name, int isFastsim);
    void InitBabyNtuple();
    void InitLeptonBranches();
    void CloseBabyNtuple () {
        BabyFile->cd();
        BabyTree->Write();
        BabyFile->Close();
    }
    csErr_t ProcessBaby(string filename_in, FactorizedJetCorrector* jetCorr, JetCorrectionUncertainty *jetUnc, int isFastsim = 0);

    bool ignore_scale1fb = false;
    bool ignore_os = false;
    bool read_psweights = false;
    bool has_lhe_branches = false;
    bool verbose = false;
    unsigned int evt_cut = 0;

    TFile* BabyFile;
    TTree* BabyTree;

  protected:
    //

  private:

    //Switches
    TString path;

    // DatasetInfo
  DatasetInfoFromFile df;

    int nloose_el;
    int ntight_el;
    int nloose_mu;
    int ntight_mu;

    TString sample;
    float evt_corrMET;
    float evt_corrMETPhi;
    float evt_rawMET;
    float evt_rawMETPhi;
    int evt_event;
    int evt_lumiBlock;
    int evt_run;
    bool passes_met_filters;
    bool passes_any_trigger;
    bool evt_isRealData;
    float scale1fb;     
    float qscale;     
    int njets;  
    int njets_recoil;  
    float ht_SS;  
    int nFOs_SS;
    int nvtx;
    float trueNumInt;
    float puweight;

    LorentzVector p4;
    LorentzVector tag_p4;
    LorentzVector dilep_p4;
    LorentzVector mc_p4;
    LorentzVector mc_motherp4;
    int mc_motherid;
    int id;
    int idx;
    int motherID;
    bool have_tag;
    float tag_pt;
    float p4_pt;
    float p4_eta;

    float mva;
    float etaSC;

    bool passes_SS_tight_v6;
    bool passes_SS_tight_noiso_v6;
    bool passes_SS_fo_v6;
    bool passes_SS_fo_noiso_v6;
    bool passes_SS_fo_looseMVA_v6;
    bool passes_SS_fo_looseMVA_noiso_v6;

    float ip3d;
    float ip3derr;
    float mt;
    float mt_raw;
    float ptrelv1;
    float coneCorrPt;
    float miniiso;
    LorentzVector close_jet_v5;
    float ptratio_v5;
    bool isTriggerSafe;
    bool isTriggerSafenoIso;
    float dilep_mass;

    //Single Muon Trigger
    int HLT_Mu8_TrkIsoVVL;
    int HLT_Mu17_TrkIsoVVL;
    int HLT_Mu8;
    int HLT_Mu17;
    int HLT_IsoMu27;

    //Single Electron Trigger
    int HLT_Ele8_CaloIdM_TrackIdM_PFJet30;
    int HLT_Ele12_CaloIdM_TrackIdM_PFJet30;
    int HLT_Ele17_CaloIdM_TrackIdM_PFJet30;
    int HLT_Ele18_CaloIdM_TrackIdM_PFJet30;
    int HLT_Ele23_CaloIdM_TrackIdM_PFJet30;
    int HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30;
    int HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30;
    int HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30;
    int HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30;
    int HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30;
    int HLT_Ele23_CaloIdL_TrackIdL_IsoVL;
    int HLT_Ele12_CaloIdL_TrackIdL_IsoVL;

    //MuEl Trigger
    int HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300;
    int HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
    int HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL;

    //Double Muon Trigger
    int HLT_DoubleMu8_Mass8_PFHT300;
    int HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL;
    int HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL;
    int HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ;
    int HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ;

    //Double Electron Trigger
    int HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300;
    int HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
    int HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
    int HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
    int HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL;

    bool el_pass_trigsafenoiso;
    bool el_pass_trigsafeiso;
    bool el_pass_v2trigsafenoiso;
    bool el_pass_v2trigsafeiso;
    bool el_pass_miniiso;
    bool el_pass_convvtx;
    bool el_pass_expinner;
    bool el_pass_dxy;
    bool el_pass_dz;
    bool el_pass_eta;
    bool el_pass_sip;
    bool el_pass_threecharge;
    bool el_pass_mva;
    bool el_loose;
    bool el_tight;
    bool el_loosewo_trigsafenoiso;
    bool el_loosewo_miniiso;
    bool el_loosewo_convvtx;
    bool el_loosewo_expinner;
    bool el_loosewo_dxy;
    bool el_loosewo_dz;
    bool el_loosewo_eta;
    bool el_loosewo_sip;
    bool el_loosewo_threecharge;
    bool el_loosewo_mva;

};

#endif
