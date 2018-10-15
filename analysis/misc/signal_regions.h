#ifndef SIGNALREGIONS_H
#define SIGNALREGIONS_H

// need to either inline everything because function bodies are in this header file
// or need to make sure it gets included only once in total
// header guard at the top only prevents against multiple inclusions in one file
// so otherwise we get the "multiple definitions of..." when compiling/linking :(
// https://stackoverflow.com/questions/8201944/multiple-definition-and-header-only-libraries

float ptcut(int id){
  return (abs(id) == 11) ? 15 : 10; 
}
int baseline_region(int njets, int nbtags, float met, float ht, int id1, int id2, float lep1_pt, float lep2_pt, float metcut) {
  //Kinematic cuts
  if (lep1_pt < ptcut(id1)) return -1;
  if (lep2_pt < ptcut(id2)) return -1;
  if (njets < 2) return -1;
  if (met < metcut) return -1;
 
  //Return baseline region
  if      (nbtags == 0) return 0;
  else if (nbtags == 1) return 1;
  else if (nbtags == 2) return 2;
  else                  return 3;
}

bool passes_baseline(int njets, int nbtags, float met, float ht, int id1, int id2, float lep1_pt, float lep2_pt) {
    if (lep1_pt < 25.) return 0;
    else if (lep2_pt < 20.) return 0;
    else if (njets < 2) return 0;
    else if (nbtags < 2) return 0;
    else if (ht < 300) return 0;
    else if (met < 50) return 0;
    else return 1;
}

/*
# paste the below into the python interpreter
import numpy as np
def get_sigmoid_binning(nbins):
    x = np.linspace(0.,1.,nbins+1)
    k = 11.20-0.22*nbins # XXX rederive every time we re-train
    k = k*np.ones(len(x))
    k[x < 0.5] *= 0.5 # for bkg enriched, don't split too finely, so soften the sigmoid by 1/2 -- threshold of 0.5 to keep continuity
    bins = 1.0/(1+np.exp(-k*(x-0.5)))+2.0*(x-0.5)/(1+np.exp(k/2))
    return bins

# note that the number of SRs is actually 1 higher than `nbins` because we add CRZ in front
nbins=13; print "int getBDTBin(float disc, bool crz) {\n    if (crz) return 1;\n"+"\n".join(["    if (disc > {:.3f}) return {};".format(x,ix+2) for ix,x in list(enumerate(get_sigmoid_binning(nbins)[:-1]))[::-1]])+"\n    return -1;\n}}\nint getNsrsDisc() {{ return {}; }}".format(nbins+1)
# or for many at once, labeled by the bin numbers
for nbins in range(10,21): print "int getBDTBin_{}(float disc, bool crz) {{\n    if (crz) return 1;\n".format(nbins)+"\n".join(["    if (disc > {:.3f}) return {};".format(x,ix+2) for ix,x in list(enumerate(get_sigmoid_binning(nbins)[:-1]))[::-1]])+"\n    return -1;\n}}\nint getNsrsDisc_{}() {{ return {}; }}".format(nbins,nbins+1)
*/

int getBDTBin_10(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.982) return 11;
    if (disc > 0.944) return 10;
    if (disc > 0.863) return 9;
    if (disc > 0.713) return 8;
    if (disc > 0.500) return 7;
    if (disc > 0.370) return 6;
    if (disc > 0.251) return 5;
    if (disc > 0.149) return 4;
    if (disc > 0.066) return 3;
    if (disc > 0.000) return 2;
    return -1;
}
int getNsrsDisc_10() { return 11; }
int getBDTBin_11(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.983) return 12;
    if (disc > 0.950) return 11;
    if (disc > 0.886) return 10;
    if (disc > 0.771) return 9;
    if (disc > 0.600) return 8;
    if (disc > 0.441) return 7;
    if (disc > 0.327) return 6;
    if (disc > 0.224) return 5;
    if (disc > 0.135) return 4;
    if (disc > 0.060) return 3;
    if (disc > 0.000) return 2;
    return -1;
}
int getNsrsDisc_11() { return 12; }
int getBDTBin_12(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.984) return 13;
    if (disc > 0.955) return 12;
    if (disc > 0.902) return 11;
    if (disc > 0.811) return 10;
    if (disc > 0.673) return 9;
    if (disc > 0.500) return 8;
    if (disc > 0.394) return 7;
    if (disc > 0.294) return 6;
    if (disc > 0.203) return 5;
    if (disc > 0.123) return 4;
    if (disc > 0.056) return 3;
    if (disc > 0.000) return 2;
    return -1;
}
int getNsrsDisc_12() { return 13; }
int getBDTBin_13(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.984) return 14;
    if (disc > 0.958) return 13;
    if (disc > 0.912) return 12;
    if (disc > 0.838) return 11;
    if (disc > 0.727) return 10;
    if (disc > 0.581) return 9;
    if (disc > 0.451) return 8;
    if (disc > 0.356) return 7;
    if (disc > 0.267) return 6;
    if (disc > 0.186) return 5;
    if (disc > 0.114) return 4;
    if (disc > 0.053) return 3;
    if (disc > 0.000) return 2;
    return -1;
}
int getNsrsDisc_13() { return 14; }
int getBDTBin_14(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.985) return 15;
    if (disc > 0.960) return 14;
    if (disc > 0.920) return 13;
    if (disc > 0.858) return 12;
    if (disc > 0.766) return 11;
    if (disc > 0.643) return 10;
    if (disc > 0.500) return 9;
    if (disc > 0.411) return 8;
    if (disc > 0.326) return 7;
    if (disc > 0.246) return 6;
    if (disc > 0.172) return 5;
    if (disc > 0.107) return 4;
    if (disc > 0.050) return 3;
    if (disc > 0.000) return 2;
    return -1;
}
int getNsrsDisc_14() { return 15; }
int getBDTBin_15(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.985) return 16;
    if (disc > 0.962) return 15;
    if (disc > 0.926) return 14;
    if (disc > 0.872) return 13;
    if (disc > 0.795) return 12;
    if (disc > 0.692) return 11;
    if (disc > 0.567) return 10;
    if (disc > 0.459) return 9;
    if (disc > 0.378) return 8;
    if (disc > 0.300) return 7;
    if (disc > 0.228) return 6;
    if (disc > 0.161) return 5;
    if (disc > 0.101) return 4;
    if (disc > 0.047) return 3;
    if (disc > 0.000) return 2;
    return -1;
}
int getNsrsDisc_15() { return 16; }
int getBDTBin_16(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.985) return 17;
    if (disc > 0.963) return 16;
    if (disc > 0.930) return 15;
    if (disc > 0.883) return 14;
    if (disc > 0.816) return 13;
    if (disc > 0.728) return 12;
    if (disc > 0.620) return 11;
    if (disc > 0.500) return 10;
    if (disc > 0.424) return 9;
    if (disc > 0.350) return 8;
    if (disc > 0.279) return 7;
    if (disc > 0.213) return 6;
    if (disc > 0.152) return 5;
    if (disc > 0.096) return 4;
    if (disc > 0.045) return 3;
    if (disc > 0.000) return 2;
    return -1;
}
int getNsrsDisc_16() { return 17; }
int getBDTBin_17(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.985) return 18;
    if (disc > 0.963) return 17;
    if (disc > 0.933) return 16;
    if (disc > 0.891) return 15;
    if (disc > 0.833) return 14;
    if (disc > 0.757) return 13;
    if (disc > 0.663) return 12;
    if (disc > 0.556) return 11;
    if (disc > 0.465) return 10;
    if (disc > 0.395) return 9;
    if (disc > 0.327) return 8;
    if (disc > 0.262) return 7;
    if (disc > 0.200) return 6;
    if (disc > 0.143) return 5;
    if (disc > 0.091) return 4;
    if (disc > 0.043) return 3;
    if (disc > 0.000) return 2;
    return -1;
}
int getNsrsDisc_17() { return 18; }
int getBDTBin_18(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.985) return 19;
    if (disc > 0.964) return 18;
    if (disc > 0.935) return 17;
    if (disc > 0.896) return 16;
    if (disc > 0.845) return 15;
    if (disc > 0.778) return 14;
    if (disc > 0.697) return 13;
    if (disc > 0.602) return 12;
    if (disc > 0.500) return 11;
    if (disc > 0.434) return 10;
    if (disc > 0.370) return 9;
    if (disc > 0.307) return 8;
    if (disc > 0.247) return 7;
    if (disc > 0.190) return 6;
    if (disc > 0.137) return 5;
    if (disc > 0.087) return 4;
    if (disc > 0.042) return 3;
    if (disc > 0.000) return 2;
    return -1;
}
int getNsrsDisc_18() { return 19; }
int getBDTBin_19(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.985) return 20;
    if (disc > 0.964) return 19;
    if (disc > 0.937) return 18;
    if (disc > 0.901) return 17;
    if (disc > 0.854) return 16;
    if (disc > 0.795) return 15;
    if (disc > 0.723) return 14;
    if (disc > 0.640) return 13;
    if (disc > 0.548) return 12;
    if (disc > 0.469) return 11;
    if (disc > 0.408) return 10;
    if (disc > 0.348) return 9;
    if (disc > 0.289) return 8;
    if (disc > 0.234) return 7;
    if (disc > 0.180) return 6;
    if (disc > 0.130) return 5;
    if (disc > 0.084) return 4;
    if (disc > 0.040) return 3;
    if (disc > 0.000) return 2;
    return -1;
}
int getNsrsDisc_19() { return 20; }
int getBDTBin_20(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.984) return 21;
    if (disc > 0.964) return 20;
    if (disc > 0.938) return 19;
    if (disc > 0.904) return 18;
    if (disc > 0.862) return 17;
    if (disc > 0.809) return 16;
    if (disc > 0.745) return 15;
    if (disc > 0.670) return 14;
    if (disc > 0.587) return 13;
    if (disc > 0.500) return 12;
    if (disc > 0.442) return 11;
    if (disc > 0.385) return 10;
    if (disc > 0.329) return 9;
    if (disc > 0.274) return 8;
    if (disc > 0.222) return 7;
    if (disc > 0.172) return 6;
    if (disc > 0.125) return 5;
    if (disc > 0.081) return 4;
    if (disc > 0.039) return 3;
    if (disc > 0.000) return 2;
    return -1;
}
int getNsrsDisc_20() { return 21; }

int getNsrsDisc(int nbins) {
    if (nbins == 10) return getNsrsDisc_10();
    if (nbins == 11) return getNsrsDisc_11();
    if (nbins == 12) return getNsrsDisc_12();
    if (nbins == 13) return getNsrsDisc_13();
    if (nbins == 14) return getNsrsDisc_14();
    if (nbins == 15) return getNsrsDisc_15();
    if (nbins == 16) return getNsrsDisc_16();
    if (nbins == 17) return getNsrsDisc_17();
    if (nbins == 18) return getNsrsDisc_18();
    if (nbins == 19) return getNsrsDisc_19();
    if (nbins == 20) return getNsrsDisc_20();
    return -1;
}
int getBDTBin(int nbins, float disc, bool crz) {
    if (nbins == 10) return getBDTBin_10(disc,crz);
    if (nbins == 11) return getBDTBin_11(disc,crz);
    if (nbins == 12) return getBDTBin_12(disc,crz);
    if (nbins == 13) return getBDTBin_13(disc,crz);
    if (nbins == 14) return getBDTBin_14(disc,crz);
    if (nbins == 15) return getBDTBin_15(disc,crz);
    if (nbins == 16) return getBDTBin_16(disc,crz);
    if (nbins == 17) return getBDTBin_17(disc,crz);
    if (nbins == 18) return getBDTBin_18(disc,crz);
    if (nbins == 19) return getBDTBin_19(disc,crz);
    if (nbins == 20) return getBDTBin_20(disc,crz);
    return -1;
}


// // nominal
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, float lep3pt, int nleps, int isClass6){
//     if (lep1pt < 25.) return -1;
//     if (lep2pt < 20.) return -1;
//     if (njets < 2) return -1;
//     if (nbtags < 2) return -1;
//     if (ht < 300) return -1;
//     if (isClass6) {
//         if (lep3pt >= 20.) return 1;
//         else return -1;
//     }
//     if (nleps == 2) {
//         if (nbtags == 2) {
//             if (njets <= 5) return 2;
//             if (njets == 6) return 3;
//             if (njets == 7) return 4;
//             if (njets >= 8) return 5;
//         } else if (nbtags == 3) {
//             if (njets == 5) return 6;
//             if (njets == 6) return 6;
//             if (njets >= 7) return 7;
//         } else if (nbtags >= 4) {
//             if (njets >= 5) return 8;
//         }
//     } else {
//         if (nbtags == 2) {
//             if (njets >= 5) return 9;
//         } else if (nbtags >= 3) {
//             if (njets >= 4) return 10;
//         }
//     }
//     return -1;
// }
// int getNsrsTTTT() { return 10; }

// 18 bins
int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, float lep3pt, int nleps, bool isClass6){
    if (lep1pt < 25.) return -1;
    if (lep2pt < 20.) return -1;
    if (njets < 2) return -1;
    if (nbtags < 2) return -1;
    if (ht < 300) return -1;
    if (met < 50) return -1;
    if (isClass6) {
        if (lep3pt >= 20.) return 1;
        else return -1;
    }
    if (nleps == 2) {
        if (nbtags == 2) {
            if (njets <= 5) return 2;
            if (njets == 6) return 3;
            if (njets == 7) return 4;
            if (njets >= 8) return 5;
        } else if (nbtags == 3) {
            if (njets == 5) return 6;
            if (njets == 6) return 7;
            if (njets == 7) return 8;
            if (njets >= 8) return 9;
        } else if (nbtags >= 4) {
            if (njets == 5) return 10;
            if (njets == 6) return 11;
            if (njets >= 7) return 12;
        }
    } else {
        if (nbtags == 2) {
            if (njets == 5) return 13;
            if (njets == 6) return 14;
            if (njets >= 7) return 15;
        } else if (nbtags >= 3) {
            if (njets == 4) return 16;
            if (njets == 5) return 17;
            if (njets >= 6) return 18;
        }
    }
    return -1;
}
int getNsrs() { return 18; } // note that this includes CRs
int getNCRs() { return 2; }
// int getNsrsDisc() { return 13; }

// // 19 bins -- 18 bins + srnb3nl2nj4
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, float lep3pt, int nleps, int isClass6){
//     if (lep1pt < 25.) return -1;
//     if (lep2pt < 20.) return -1;
//     if (njets < 2) return -1;
//     if (nbtags < 2) return -1;
//     if (ht < 300) return -1;
//     if (met < 50) return -1;
//     if (isClass6) {
//         if (lep3pt >= 20.) return 1;
//         else return -1;
//     }
//     if (nleps == 2) {
//         if (nbtags == 2) {
//             if (njets <= 5) return 2;
//             if (njets == 6) return 3;
//             if (njets == 7) return 4;
//             if (njets >= 8) return 5;
//         } else if (nbtags == 3) {
//             if (njets == 4) return 6;
//             if (njets == 5) return 7;
//             if (njets == 6) return 8;
//             if (njets == 7) return 9;
//             if (njets >= 8) return 10;
//         } else if (nbtags >= 4) {
//             if (njets == 5) return 11;
//             if (njets == 6) return 12;
//             if (njets >= 7) return 13;
//         }
//     } else {
//         if (nbtags == 2) {
//             if (njets == 5) return 14;
//             if (njets == 6) return 15;
//             if (njets >= 7) return 16;
//         } else if (nbtags >= 3) {
//             if (njets == 4) return 17;
//             if (njets == 5) return 18;
//             if (njets >= 6) return 19;
//         }
//     }
//     return -1;
// }
// int getNsrs() { return 19; } // note that this includes CRs
// int getNCRs() { return 2; }
// int getNsrsDisc() { return 13; }

// // 19 bins  with q split CRW
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, float lep3pt, int nleps, int isClass6){
//     if (lep1pt < 25.) return -1;
//     if (lep2pt < 20.) return -1;
//     if (njets < 2) return -1;
//     if (nbtags < 2) return -1;
//     if (ht < 300) return -1;
//     if (met < 50) return -1;
//     if (isClass6) {
//         if (lep3pt >= 20.) return 1;
//         else return -1;
//     }
//     if (nleps == 2) {
//         if (nbtags == 2) {
//             if (njets <= 5) return 2 + (id1>0);
//             if (njets == 6) return 4;
//             if (njets == 7) return 5;
//             if (njets >= 8) return 6;
//         } else if (nbtags == 3) {
//             if (njets == 5) return 7;
//             if (njets == 6) return 8;
//             if (njets == 7) return 9;
//             if (njets >= 8) return 10;
//         } else if (nbtags >= 4) {
//             if (njets == 5) return 11;
//             if (njets == 6) return 12;
//             if (njets >= 7) return 13;
//         }
//     } else {
//         if (nbtags == 2) {
//             if (njets == 5) return 14;
//             if (njets == 6) return 15;
//             if (njets >= 7) return 16;
//         } else if (nbtags >= 3) {
//             if (njets == 4) return 17;
//             if (njets == 5) return 18;
//             if (njets >= 6) return 19;
//         }
//     }
//     return -1;
// }
// int getNsrs() { return 19; } // note that this includes CRs
// int getNCRs() { return 3; }
// int getNsrsDisc() { return 13; }


#endif
