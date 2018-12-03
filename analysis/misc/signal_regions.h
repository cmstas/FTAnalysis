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
    # k = 11.20-0.22*nbins # XXX rederive every time we re-train
    k = 11.5 # test effect of flat k independent of nbins
    k = k*np.ones(len(x))
    k[x < 0.5] *= 0.5 # for bkg enriched, don't split too finely, so soften the sigmoid by 1/2 -- threshold of 0.5 to keep continuity
    bins = 1.0/(1+np.exp(-k*(x-0.5)))+2.0*(x-0.5)/(1+np.exp(k/2))
    return bins

# note that the number of SRs is actually 1 higher than `nbins` because we add CRZ in front
nbins=13; print "int getBDTBin(float disc, bool crz) {\n    if (crz) return 1;\n"+"\n".join(["    if (disc > {:.4f}) return {};".format(x,ix+2) for ix,x in list(enumerate(get_sigmoid_binning(nbins)[:-1]))[::-1]])+"\n    return -1;\n}}\nint getNsrsDisc() {{ return {}; }}".format(nbins+1)
# or for many at once, labeled by the bin numbers
for nbins in range(10,21): print "int getBDTBin_{}(float disc, bool crz) {{\n    if (crz) return 1;\n".format(nbins)+"\n".join(["    if (disc > {:.4f}) return {};".format(x,ix+2) for ix,x in list(enumerate(get_sigmoid_binning(nbins)[:-1]))[::-1]])+"\n    return -1;\n}}\nint getNsrsDisc_{}() {{ return {}; }}".format(nbins,nbins+1)
*/

int getBDTBin_10(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.9926) return 11;
    if (disc > 0.9711) return 10;
    if (disc > 0.9101) return 9;
    if (disc > 0.7601) return 8;
    if (disc > 0.5000) return 7;
    if (disc > 0.3494) return 6;
    if (disc > 0.2191) return 5;
    if (disc > 0.1192) return 4;
    if (disc > 0.0484) return 3;
    if (disc > 0.0000) return 2;
    return -1;
}
int getNsrsDisc_10() { return 11; }
int getBDTBin_11(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.9936) return 12;
    if (disc > 0.9769) return 11;
    if (disc > 0.9332) return 10;
    if (disc > 0.8284) return 9;
    if (disc > 0.6281) return 8;
    if (disc > 0.4302) return 7;
    if (disc > 0.2989) return 6;
    if (disc > 0.1887) return 5;
    if (disc > 0.1043) return 4;
    if (disc > 0.0432) return 3;
    if (disc > 0.0000) return 2;
    return -1;
}
int getNsrsDisc_11() { return 12; }
int getBDTBin_12(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.9944) return 13;
    if (disc > 0.9809) return 12;
    if (disc > 0.9482) return 11;
    if (disc > 0.8728) return 10;
    if (disc > 0.7233) return 9;
    if (disc > 0.5000) return 8;
    if (disc > 0.3735) return 7;
    if (disc > 0.2594) return 6;
    if (disc > 0.1652) return 5;
    if (disc > 0.0926) return 4;
    if (disc > 0.0390) return 3;
    if (disc > 0.0000) return 2;
    return -1;
}
int getNsrsDisc_12() { return 13; }
int getBDTBin_13(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.9950) return 14;
    if (disc > 0.9839) return 13;
    if (disc > 0.9584) return 12;
    if (disc > 0.9025) return 11;
    if (disc > 0.7911) return 10;
    if (disc > 0.6091) return 9;
    if (disc > 0.4408) return 8;
    if (disc > 0.3276) return 7;
    if (disc > 0.2281) return 6;
    if (disc > 0.1466) return 5;
    if (disc > 0.0832) return 4;
    if (disc > 0.0355) return 3;
    if (disc > 0.0000) return 2;
    return -1;
}
int getNsrsDisc_13() { return 14; }
int getBDTBin_14(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.9955) return 15;
    if (disc > 0.9861) return 14;
    if (disc > 0.9657) return 13;
    if (disc > 0.9230) return 12;
    if (disc > 0.8388) return 11;
    if (disc > 0.6950) return 10;
    if (disc > 0.5000) return 9;
    if (disc > 0.3911) return 8;
    if (disc > 0.2902) return 7;
    if (disc > 0.2029) return 6;
    if (disc > 0.1316) return 5;
    if (disc > 0.0755) return 4;
    if (disc > 0.0326) return 3;
    if (disc > 0.0000) return 2;
    return -1;
}
int getNsrsDisc_14() { return 15; }
int getBDTBin_15(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.9959) return 16;
    if (disc > 0.9878) return 15;
    if (disc > 0.9711) return 14;
    if (disc > 0.9375) return 13;
    if (disc > 0.8728) return 12;
    if (disc > 0.7601) return 11;
    if (disc > 0.5949) return 10;
    if (disc > 0.4487) return 9;
    if (disc > 0.3494) return 8;
    if (disc > 0.2594) return 7;
    if (disc > 0.1823) return 6;
    if (disc > 0.1192) return 5;
    if (disc > 0.0691) return 4;
    if (disc > 0.0302) return 3;
    if (disc > 0.0000) return 2;
    return -1;
}
int getNsrsDisc_15() { return 16; }
int getBDTBin_16(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.9963) return 17;
    if (disc > 0.9892) return 16;
    if (disc > 0.9752) return 15;
    if (disc > 0.9482) return 14;
    if (disc > 0.8974) return 13;
    if (disc > 0.8089) return 12;
    if (disc > 0.6727) return 11;
    if (disc > 0.5000) return 10;
    if (disc > 0.4044) return 9;
    if (disc > 0.3143) return 8;
    if (disc > 0.2338) return 7;
    if (disc > 0.1652) return 6;
    if (disc > 0.1089) return 5;
    if (disc > 0.0637) return 4;
    if (disc > 0.0280) return 3;
    if (disc > 0.0000) return 2;
    return -1;
}
int getNsrsDisc_16() { return 17; }
int getBDTBin_17(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.9966) return 18;
    if (disc > 0.9903) return 17;
    if (disc > 0.9784) return 16;
    if (disc > 0.9562) return 15;
    if (disc > 0.9156) return 14;
    if (disc > 0.8453) return 13;
    if (disc > 0.7345) return 12;
    if (disc > 0.5839) return 11;
    if (disc > 0.4547) return 10;
    if (disc > 0.3664) return 9;
    if (disc > 0.2847) return 8;
    if (disc > 0.2124) return 7;
    if (disc > 0.1509) return 6;
    if (disc > 0.1001) return 5;
    if (disc > 0.0590) return 4;
    if (disc > 0.0262) return 3;
    if (disc > 0.0000) return 2;
    return -1;
}
int getNsrsDisc_17() { return 18; }
int getBDTBin_18(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.9968) return 19;
    if (disc > 0.9912) return 18;
    if (disc > 0.9809) return 17;
    if (disc > 0.9624) return 16;
    if (disc > 0.9294) return 15;
    if (disc > 0.8728) return 14;
    if (disc > 0.7828) return 13;
    if (disc > 0.6549) return 12;
    if (disc > 0.5000) return 11;
    if (disc > 0.4149) return 10;
    if (disc > 0.3336) return 9;
    if (disc > 0.2594) return 8;
    if (disc > 0.1942) return 7;
    if (disc > 0.1387) return 6;
    if (disc > 0.0926) return 5;
    if (disc > 0.0550) return 4;
    if (disc > 0.0246) return 3;
    if (disc > 0.0000) return 2;
    return -1;
}
int getNsrsDisc_18() { return 19; }
int getBDTBin_19(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.9970) return 20;
    if (disc > 0.9919) return 19;
    if (disc > 0.9830) return 18;
    if (disc > 0.9672) return 17;
    if (disc > 0.9399) return 16;
    if (disc > 0.8938) return 15;
    if (disc > 0.8204) return 14;
    if (disc > 0.7131) return 13;
    if (disc > 0.5753) return 12;
    if (disc > 0.4594) return 11;
    if (disc > 0.3800) return 10;
    if (disc > 0.3053) return 9;
    if (disc > 0.2378) return 8;
    if (disc > 0.1786) return 7;
    if (disc > 0.1282) return 6;
    if (disc > 0.0862) return 5;
    if (disc > 0.0515) return 4;
    if (disc > 0.0232) return 3;
    if (disc > 0.0000) return 2;
    return -1;
}
int getNsrsDisc_19() { return 20; }
int getBDTBin_20(float disc, bool crz) {
    if (crz) return 1;
    if (disc > 0.9972) return 21;
    if (disc > 0.9926) return 20;
    if (disc > 0.9847) return 19;
    if (disc > 0.9711) return 18;
    if (disc > 0.9482) return 17;
    if (disc > 0.9101) return 16;
    if (disc > 0.8497) return 15;
    if (disc > 0.7601) return 14;
    if (disc > 0.6402) return 13;
    if (disc > 0.5000) return 12;
    if (disc > 0.4233) return 11;
    if (disc > 0.3494) return 10;
    if (disc > 0.2808) return 9;
    if (disc > 0.2191) return 8;
    if (disc > 0.1652) return 7;
    if (disc > 0.1192) return 6;
    if (disc > 0.0805) return 5;
    if (disc > 0.0484) return 4;
    if (disc > 0.0219) return 3;
    if (disc > 0.0000) return 2;
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
// int getNsrs() { return 10; } // note that this includes CRs
// int getNCRs() { return 2; }

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
