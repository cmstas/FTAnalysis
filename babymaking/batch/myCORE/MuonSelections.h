#ifndef MUONSELECTIONS_H
#define MUONSELECTIONS_H
#include "CMS3.h"
#include "TString.h"
#include "Base.h"

//POG IDs
bool isMediumMuonPOG_forICHEP( unsigned int muIdx );
bool isLooseMuonPOG(unsigned int muIdx);
bool isMediumMuonPOG(unsigned int muIdx);
bool isTightMuonPOG(unsigned int muIdx);
bool isHighPtMuonPOG(unsigned int muIdx);
bool isBadGlobalMuon(unsigned int muIdx, bool selectClones = false);

//Main Muon ID function
bool muonID(unsigned int muIdx, id_level_t id_level);

//Tightest ID passed by muon
int muTightID(unsigned int muIdx, analysis_t analysis, int version = 1);

// tight charge requirement
int tightChargeMuon(unsigned int muIdx);

bool PassSoftMuonCut(unsigned int muIdx);

struct muIDcache {
public:
  void  setCacheValues(int idx, float miniiso, float ptratio, float ptrel) {
    idx_ = idx;
    miniiso_ = miniiso;
    ptratio_ = ptratio;
    ptrel_ = ptrel;
  }
  //make sure it was set for this electron before returning
  float getMiniiso(int idx) {assert(idx==idx_); return miniiso_;}
  float getPtratio(int idx) {assert(idx==idx_); return ptratio_;}
  float getPtrel(int idx) {assert(idx==idx_); return ptrel_;}
private:
  int idx_;
  float miniiso_;
  float ptratio_;
  float ptrel_;
};
namespace muID {
  void setCache(int idx, float miniiso, float ptratio, float ptrel);
  void unsetCache();
}

// check CMS3 version to see which c++ type is stored in the ntuples for mus_gfit_ndof
int get_mus_gfit_ndof( unsigned int muIdx );

#endif
