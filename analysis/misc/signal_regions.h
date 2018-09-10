#ifndef SIGNALREGIONS_H
#define SIGNALREGIONS_H

// need to either inline everything because function bodies are in this header file
// or need to make sure it gets included only once in total
// header guard at the top only prevents against multiple inclusions in one file
// so otherwise we get the "multiple definitions of..." when compiling/linking :(
// https://stackoverflow.com/questions/8201944/multiple-definition-and-header-only-libraries

int getNsrsTTTTBDT() { return 7; }
int signalRegionBDT(float disc){
    return 1+(int)(disc*getNsrsTTTTBDT());
}

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
int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, float lep3pt, int nleps, int isClass6){
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
int getNsrsTTTT() { return 18; }


#endif
