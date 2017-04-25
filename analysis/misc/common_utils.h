#ifndef _commonUtils_h_
#define _commonUtils_h_

float getLumi() { return 35.87; }
float getLumiUnblind() { return 35.87; }
bool isUnblindRun(int run) { return true; }
TString getTag() { return "v8.07"; }
TString getTagData() { return "v8.07"; }
bool applyThirdLeptonVeto() { return false; }
#include "flip_rates.h"
#include "fake_rates.h"
#include "trigger_sf.h"
#include "pu_weights.h"
#include "lepton_sf.h"

float fakeRate(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
    if (abs(id)==11) return electronFakeRate(pt,eta);
    else if (abs(id)==13) return muonFakeRate(pt,eta);
    else return 0.;
  } else {
    if (abs(id)==11) return electronFakeRate_IsoTrigs(pt,eta);
    else if (abs(id)==13) return muonFakeRate_IsoTrigs(pt,eta);
    else return 0.;
  }
}

float fakeRateError(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
    if (abs(id)==11) return electronFakeRateError(pt,eta);
    else if (abs(id)==13) return muonFakeRateError(pt,eta);
    else return 0.;
  } else {
    if (abs(id)==11) return electronFakeRateError_IsoTrigs(pt,eta);
    else if (abs(id)==13) return muonFakeRateError_IsoTrigs(pt,eta);
    else return 0.;
  }
}

float alternativeFakeRate(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
    if (abs(id)==11) return electronAlternativeFakeRate(pt,eta);
    else if (abs(id)==13) return muonAlternativeFakeRate(pt,eta);
    else return 0.;
  } else {
    if (abs(id)==11) return electronAlternativeFakeRate_IsoTrigs(pt,eta);
    else if (abs(id)==13) return muonAlternativeFakeRate_IsoTrigs(pt,eta);
    else return 0.;
  }
}

// returns A if pt<ptmin, B if pt>ptmax, and linear interpolation between. if pt<10, use C
float mvacut(float A, float B, float C, float pt_) {
    float ptmin = 15;
    float ptmax = 25;
    return pt_>10 ? std::min(A, std::max(B,A + (B-A)/(ptmax-ptmin)*(pt_-ptmin))) : C; 
}

bool passIsolatedFO(int id, float eta, float disc, float pt) {
  if (abs(id)==13) return true;
  float aeta = fabs(eta);
  // if (aeta < 0.8) return mva > -0.155;
  // if ((aeta >= 0.8 && aeta <= 1.479)) return mva > -0.56;
  // if (aeta > 1.479) return mva > -0.76;
    if (aeta < 0.8) return disc > mvacut(-0.86,-0.96,-0.3,pt);
    if ((aeta >= 0.8 && aeta <= 1.479)) return disc > mvacut(-0.85,-0.96,-0.36,pt);
    if (aeta > 1.479) return disc > mvacut(-0.81,-0.95,-0.63,pt);
  return false;
}

#endif
