#ifndef _commonUtils_h_
#define _commonUtils_h_

float getLumi() { return 41.3; }
float getLumiUnblind() { return 41.3; }
bool isUnblindRun(int run) { return true; }
TString getTag() { return "v8.07"; }
TString getTagData() { return "v8.07"; }
bool applyThirdLeptonVeto() { return false; }
#include "flip_rates.h"
#include "fake_rates.h"
#include "trigger_sf.h"
#include "pu_weights.h"
#include "lepton_sf.h"

#include "LaurentSFs_fastsim.h"
#include "lepton_sf_fastsim.h"

float fakeRate(int id, float pt, float eta, float ht) { 
    if (abs(id)==11) return electronFakeRate_IsoTrigs(pt,eta);
    else if (abs(id)==13) return muonFakeRate_IsoTrigs(pt,eta);
    else return 0.;
}

float fakeRateError(int id, float pt, float eta, float ht) { 
    if (abs(id)==11) return electronFakeRateError_IsoTrigs(pt,eta);
    else if (abs(id)==13) return muonFakeRateError_IsoTrigs(pt,eta);
    else return 0.;
}

float alternativeFakeRate(int id, float pt, float eta, float ht) { 
    if (abs(id)==11) return electronAlternativeFakeRate_IsoTrigs(pt,eta);
    else if (abs(id)==13) return muonAlternativeFakeRate_IsoTrigs(pt,eta);
    else return 0.;
}

// returns A if pt<ptmin, B if pt>ptmax, and linear interpolation between. if pt<10, use C
float mvacut(float A, float B, float C, float pt_) {
    float ptmin = 15;
    float ptmax = 25;
    return pt_>10 ? std::min(A, std::max(B,A + (B-A)/(ptmax-ptmin)*(pt_-ptmin))) : C; 
}

// C if pT<10, A if pT=10, B if pt>=25, lerp between A,B for pT in [10,25]
float mvacut2017(float A, float B, float C, float pt_) {
    if (pt_ < 10) return C;
    else if (pt_ > 25) return B;
    else {
        return A + (B-A)/15.0f*(pt_-10.0f);
    }
}

bool passIsolatedFO(int id, float eta, float disc, float pt) {
  if (abs(id)==13) return true;
  float aeta = fabs(eta);
  // if (aeta < 0.8) return mva > -0.155;
  // if ((aeta >= 0.8 && aeta <= 1.479)) return mva > -0.56;
  // if (aeta > 1.479) return mva > -0.76;
    // if (aeta < 0.8) return disc > mvacut(-0.86,-0.96,-0.3,pt);
    // if ((aeta >= 0.8 && aeta <= 1.479)) return disc > mvacut(-0.85,-0.96,-0.36,pt);
    // if (aeta > 1.479) return disc > mvacut(-0.81,-0.95,-0.63,pt);
    if (aeta < 0.8) return disc > mvacut2017(-0.93,-0.887,-0.135,pt);
    if ((aeta >= 0.8 && aeta <= 1.479)) return disc > mvacut2017(-0.93,-0.89,-0.417,pt);
    if (aeta > 1.479) return disc > mvacut2017(-0.942,-0.91,-0.470,pt);
  return false;
}

#endif
