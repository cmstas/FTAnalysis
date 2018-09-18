#ifndef _commonUtils_h_
#define _commonUtils_h_

#include "year_2016/all.h"
#include "year_2017/all.h"
#include "year_2018/all.h"

float getTruePUw(int year, int nvtx, int which=0) { 
    // which = 0, 1, -1 for nominal, up, down
    if (which == 0) {
        if (year == 2016) return y2016::getTruePUw(nvtx);
        else if (year == 2017) return y2017::getTruePUw(nvtx);
        else if (year == 2018) return y2018::getTruePUw(nvtx);
    } else if (which == 1) {
        if (year == 2016) return y2016::getTruePUwUp(nvtx);
        else if (year == 2017) return y2017::getTruePUwUp(nvtx);
        else if (year == 2018) return y2018::getTruePUwUp(nvtx);
    } else if (which == -1) {
        if (year == 2016) return y2016::getTruePUwDn(nvtx);
        else if (year == 2017) return y2017::getTruePUwDn(nvtx);
        else if (year == 2018) return y2018::getTruePUwDn(nvtx);
    }
    return 0.;
}

float isrWeight(int year, int nisrmatch, int sample) { 
    if (year == 2016) return y2016::isrWeight(nisrmatch, sample);
    else if (year == 2017) return y2017::isrWeight(nisrmatch, sample);
    else if (year == 2018) return y2018::isrWeight(nisrmatch, sample);
    else return 0.;
}


float leptonScaleFactor(int year, int id, float pt, float eta, float ht) { 
    // FIXME Note: muon SFs do not go below pT of 20 - this is fine for four top, but
    // modify for SS!
    if (year == 2016) return y2016::leptonScaleFactor(id, pt, eta, ht);
    else if (year == 2017) return y2017::leptonScaleFactor(id, pt, eta, ht);
    else if (year == 2018) return y2018::leptonScaleFactor(id, pt, eta, ht);
    else return 0.;
}

float leptonScaleFactorError(int year, int id, float pt, float eta, float ht) { 
    if (year == 2016) return y2016::leptonScaleFactor_err(id, pt, eta, ht);
    else if (year == 2017) return y2017::leptonScaleFactor_err(id, pt, eta, ht);
    else if (year == 2018) return y2018::leptonScaleFactor_err(id, pt, eta, ht);
    else return 0.;
}

float triggerScaleFactor(int year, int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht) { 
    if (year == 2016) return y2016::triggerScaleFactor(pdgId1, pdgId2, pt1, pt2, eta1, eta2, ht);
    else if (year == 2017) return y2017::triggerScaleFactor(pdgId1, pdgId2, pt1, pt2, eta1, eta2, ht);
    else if (year == 2018) return y2018::triggerScaleFactor(pdgId1, pdgId2, pt1, pt2, eta1, eta2, ht);
    else return 0.;
}

float flipRate(int year, float pt, float eta) { 
    if (year == 2016) return y2016::flipRate(pt, eta);
    else if (year == 2017) return y2017::flipRate(pt, eta);
    else if (year == 2018) return y2018::flipRate(pt, eta);
    else return 0.;
}

float flipRateError(int year, float pt, float eta) { 
    if (year == 2016) return y2016::flipRateError(pt, eta);
    else if (year == 2017) return y2017::flipRateError(pt, eta);
    else if (year == 2018) return y2018::flipRateError(pt, eta);
    else return 0.;
}

float fakeRate(int year, int id, float pt, float eta, float ht) { 
    if (year == 2016) return y2016::fakeRate(id, pt, eta, ht);
    else if (year == 2017) return y2017::fakeRate(id, pt, eta, ht);
    else if (year == 2018) return y2018::fakeRate(id, pt, eta, ht);
    else return 0.;
}
float fakeRateError(int year, int id, float pt, float eta, float ht) { 
    if (year == 2016) return y2016::fakeRateError(id, pt, eta, ht);
    else if (year == 2017) return y2017::fakeRateError(id, pt, eta, ht);
    else if (year == 2018) return y2018::fakeRateError(id, pt, eta, ht);
    else return 0.;
}

float alternativeFakeRate(int year, int id, float pt, float eta, float ht) { 
    if (year == 2016) return y2016::alternativeFakeRate(id, pt, eta, ht);
    else if (year == 2017) return y2017::alternativeFakeRate(id, pt, eta, ht);
    else if (year == 2018) return y2018::alternativeFakeRate(id, pt, eta, ht);
    else return 0.;
}

float qcdMCFakeRate(int year, int id, float pt, float eta, float ht) { 
    if (year == 2016) return y2016::qcdMCFakeRate(id, pt, eta, ht);
    else if (year == 2017) return y2017::qcdMCFakeRate(id, pt, eta, ht);
    else if (year == 2018) return y2018::qcdMCFakeRate(id, pt, eta, ht);
    else return 0.;
}

float qcdMCFakeRateError(int year, int id, float pt, float eta, float ht) { 
    if (year == 2016) return y2016::qcdMCFakeRateError(id, pt, eta, ht);
    else if (year == 2017) return y2017::qcdMCFakeRateError(id, pt, eta, ht);
    else if (year == 2018) return y2018::qcdMCFakeRateError(id, pt, eta, ht);
    else return 0.;
}

float getLumi(int year) {
    if (year == 2016) return y2016::getLumi();
    else if (year == 2017) return y2017::getLumi();
    else if (year == 2018) return y2018::getLumi();
    else return 0.;
}

#endif
