#include "lepton_sfs_el.h"
#include "lepton_sfs_mu.h"

float leptonScaleFactor(int pdgId, float pt, float eta, float ht, float rand=-1.) {
    int iera = 0;
    if (rand < 0.) iera = 0; // BCDEF
    else if (rand < 0.115429) iera = 1; // B
    else if (rand < 0.347474) iera = 2; // C
    else if (rand < 0.449755) iera = 3; // D
    else if (rand < 0.674003) iera = 4; // E
    else iera = 5; // F

    if (abs(pdgId)==13) {
        // FIXME -- SFs don't go below 20
        if (pt < 20.) return 1.;
        if (iera == 0) return muonScaleFactor_RunBCDEF(pt,eta);
        else if (iera == 1 || iera == 2) return muonScaleFactor_RunBC(pt,eta);
        else if (iera == 3 || iera == 4) return muonScaleFactor_RunDE(pt,eta);
        else if (iera == 5) return muonScaleFactor_RunF(pt,eta);
    } else if (abs(pdgId)==11){
        if (iera == 0) return electronScaleFactor_RunBCDEF(pt,eta)*electronScaleFactorReco_RunBCDEF(pt,eta);
        else if (iera == 1) return electronScaleFactor_RunBCDEF(pt,eta)*electronScaleFactorReco_RunB(pt,eta);
        else if (iera == 2) return electronScaleFactor_RunBCDEF(pt,eta)*electronScaleFactorReco_RunC(pt,eta);
        else if (iera == 3) return electronScaleFactor_RunBCDEF(pt,eta)*electronScaleFactorReco_RunD(pt,eta);
        else if (iera == 4) return electronScaleFactor_RunBCDEF(pt,eta)*electronScaleFactorReco_RunE(pt,eta);
        else if (iera == 5) return electronScaleFactor_RunBCDEF(pt,eta)*electronScaleFactorReco_RunF(pt,eta);
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

    if (abs(pdgId)==13) {
        // FIXME -- SFs don't go below 20 -- take 2% per lepton
        if (pt < 20.) return 0.02;
        float e1 = 0.;
        float e2 = 0.;
        if (iera == 0) { e1=muonScaleFactorError_RunBCDEF(pt,eta); }
        else if (iera == 1 || iera == 2) { e1=muonScaleFactorError_RunBC(pt,eta); }
        else if (iera == 3 || iera == 4) { e1=muonScaleFactorError_RunDE(pt,eta); }
        else if (iera == 5) { e1=muonScaleFactorError_RunF(pt,eta); }
        return pow(e1*e1+e2*e2,0.5);
    } else if (abs(pdgId)==11){
        float e1 = 0.;
        float e2 = 0.;
        if (iera == 0) { e1=electronScaleFactorError_RunBCDEF(pt,eta); e2=electronScaleFactorRecoError_RunBCDEF(pt,eta); }
        else if (iera == 1) { e1=electronScaleFactorError_RunBCDEF(pt,eta); e2=electronScaleFactorRecoError_RunB(pt,eta); }
        else if (iera == 2) { e1=electronScaleFactorError_RunBCDEF(pt,eta); e2=electronScaleFactorRecoError_RunC(pt,eta); }
        else if (iera == 3) { e1=electronScaleFactorError_RunBCDEF(pt,eta); e2=electronScaleFactorRecoError_RunD(pt,eta); }
        else if (iera == 4) { e1=electronScaleFactorError_RunBCDEF(pt,eta); e2=electronScaleFactorRecoError_RunE(pt,eta); }
        else if (iera == 5) { e1=electronScaleFactorError_RunBCDEF(pt,eta); e2=electronScaleFactorRecoError_RunF(pt,eta); }
        return pow(e1*e1+e2*e2,0.5);
    }
    return 0.;
}
