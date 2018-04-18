#include "lepton_sfs_el.h"

float triggerScaleFactor(int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht) {
    return TotalTriggerSF(pdgId1, pt1, eta1, pdgId2, pt2, eta2, ht);
}

float leptonScaleFactor(int pdgId, float pt, float eta, float ht, float rand=-1.) {
    int iera = 0;
    if (rand < 0.) iera = 0; // BCDEF
    else if (rand < 0.115429) iera = 1; // B
    else if (rand < 0.347474) iera = 2; // C
    else if (rand < 0.449755) iera = 3; // D
    else if (rand < 0.674003) iera = 4; // E
    else iera = 5; // F

    if (abs(pdgId)==13) return 1.;
    else if (abs(pdgId)==11){
        if (iera == 0) return electronScaleFactor_RunBCDEF(pt,eta)*electronScaleFactorReco_RunBCDEF(pt,eta);
        if (iera == 1) return electronScaleFactor_RunB(pt,eta)*electronScaleFactorReco_RunB(pt,eta);
        if (iera == 2) return electronScaleFactor_RunC(pt,eta)*electronScaleFactorReco_RunC(pt,eta);
        if (iera == 3) return electronScaleFactor_RunD(pt,eta)*electronScaleFactorReco_RunD(pt,eta);
        if (iera == 4) return electronScaleFactor_RunE(pt,eta)*electronScaleFactorReco_RunE(pt,eta);
        if (iera == 5) return electronScaleFactor_RunF(pt,eta)*electronScaleFactorReco_RunF(pt,eta);
    }
    return 0.;
}

float leptonScaleFactor_err(int pdgId, float pt, float eta, float ht, float rand=-1.) {
    int iera = 0;
    if (rand < 0.) iera = 0; // BCDEF
    else if (rand < 0.115429) iera = 1; // B
    else if (rand < 0.347474) iera = 2; // C
    else if (rand < 0.449755) iera = 3; // D
    else if (rand < 0.674003) iera = 4; // E
    else iera = 5; // F

    if (abs(pdgId)==13) return 1.;
    else if (abs(pdgId)==11){
        float e1 = 0.;
        float e2 = 0.;
        if (iera == 0) { e1=electronScaleFactor_RunBCDEF(pt,eta); e2=electronScaleFactorReco_RunBCDEF(pt,eta); }
        if (iera == 1) { e1=electronScaleFactor_RunB(pt,eta); e2=electronScaleFactorReco_RunB(pt,eta); }
        if (iera == 2) { e1=electronScaleFactor_RunC(pt,eta); e2=electronScaleFactorReco_RunC(pt,eta); }
        if (iera == 3) { e1=electronScaleFactor_RunD(pt,eta); e2=electronScaleFactorReco_RunD(pt,eta); }
        if (iera == 4) { e1=electronScaleFactor_RunE(pt,eta); e2=electronScaleFactorReco_RunE(pt,eta); }
        if (iera == 5) { e1=electronScaleFactor_RunF(pt,eta); e2=electronScaleFactorReco_RunF(pt,eta); }
        return pow(e1*e1+e2*e2,0.5);
    }
    return 0.;
}

float eventScaleFactor(int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht, float rand=-1.) {
  return triggerScaleFactor(pdgId1, pdgId2, pt1, pt2, eta1, eta2, ht) * leptonScaleFactor(pdgId1, pt1, eta1, ht, rand) * leptonScaleFactor(pdgId2, pt2, eta2, ht, rand);
}

float eventScaleFactor_trigonly(int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht) {
  return triggerScaleFactor(pdgId1, pdgId2, pt1, pt2, eta1, eta2, ht);
}
float eventScaleFactor_leponly(int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht) {
  return leptonScaleFactor(pdgId1, pt1, eta1, ht) * leptonScaleFactor(pdgId2, pt2, eta2, ht);
}
