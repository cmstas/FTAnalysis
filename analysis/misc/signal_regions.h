#ifndef SIGNALREGIONS_H
#define SIGNALREGIONS_H
int getNsrsTTTTBDT() { return 7; }
int signalRegionBDT(float disc){
    return 1+(int)(disc*getNsrsTTTTBDT());
}

// nominal
int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
    if (lep1pt < 25.) return -1;
    if (lep2pt < 20.) return -1;
    if (njets < 2) return -1;
    if (nbtags < 2) return -1;
    if (ht < 300) return -1;
    if (isClass6) return 1;
    if (nleps == 2) {
        if (nbtags == 2) {
            if (njets <= 5) return 2;
            if (njets == 6) return 3;
            if (njets == 7) return 4;
            if (njets >= 8) return 5;
        } else if (nbtags == 3) {
            if (njets == 5) return 6;
            if (njets == 6) return 6;
            if (njets >= 7) return 7;
        } else if (nbtags >= 4) {
            if (njets >= 5) return 8;
        }
    } else {
        if (nbtags == 2) {
            if (njets >= 5) return 9;
        } else if (nbtags >= 3) {
            if (njets >= 4) return 10;
        }
    }
    return -1;
}
int getNsrsTTTT() { return 10; }


// // 3 bins
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
//     if (lep1pt < 25.) return -1;
//     if (lep2pt < 20.) return -1;
//     if (njets < 2) return -1;
//     if (nbtags < 2) return -1;
//     if (ht < 300) return -1;
//     if (isClass6) return 1;
//     if (nleps == 2) {
//         if (nbtags == 2) {
//             if (njets <= 5) return 2;
//         }
//     }
//     return 3;
// }
// int getNsrsTTTT() { return 3; }

// // 4 bins
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
//     if (lep1pt < 25.) return -1;
//     if (lep2pt < 20.) return -1;
//     if (njets < 2) return -1;
//     if (nbtags < 2) return -1;
//     if (ht < 300) return -1;
//     if (isClass6) return 1;
//     if (nleps == 2) {
//         if (nbtags == 2) {
//             if (njets <= 5) return 2;
//         }
//     } else {
//         return 3;
//     }
//     return 4;
// }
// int getNsrsTTTT() { return 4; }

// // 7 bins
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
//     if (lep1pt < 25.) return -1;
//     if (lep2pt < 20.) return -1;
//     if (njets < 2) return -1;
//     if (nbtags < 2) return -1;
//     if (ht < 300) return -1;
//     if (isClass6) return 1;
//     if (nleps == 2) {
//         if (nbtags == 2) {
//             if (njets <= 5) return 2;
//             return 3;
//         }
//         if (nbtags == 3) return 4;
//         if (nbtags >= 4) return 5;
//     } else {
//         if (nbtags == 2) return 6;
//         if (nbtags >= 3) return 7;
//     }
// }
// int getNsrsTTTT() { return 7; }

// // 8 bins (nominal - 2 CRs)
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
//     if (lep1pt < 25.) return -1;
//     if (lep2pt < 20.) return -1;
//     if (njets < 2) return -1;
//     if (nbtags < 2) return -1;
//     if (ht < 300) return -1;
//     if (isClass6) return -1;
//     if (nleps == 2) {
//         if (nbtags == 2) {
//             if (njets <= 5) return -1;
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
// int getNsrsTTTT() { return 8; }

// // 18 bins
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
//     if (lep1pt < 25.) return -1;
//     if (lep2pt < 20.) return -1;
//     if (njets < 2) return -1;
//     if (nbtags < 2) return -1;
//     if (ht < 300) return -1;
//     if (isClass6) return 1;
//     if (nleps == 2) {
//         if (nbtags == 2) {
//             if (njets <= 5) return 2;
//             if (njets == 6) return 3;
//             if (njets == 7) return 4;
//             if (njets >= 8) return 5;
//         } else if (nbtags == 3) {
//             if (njets <= 5) return 6;
//             if (njets == 6) return 7;
//             if (njets == 7) return 8;
//             if (njets >= 8) return 9;
//         } else if (nbtags >= 4) {
//             if (njets <= 5) return 10;
//             if (njets == 6) return 11;
//             if (njets >= 7) return 12;
//         }
//     } else {
//         if (nbtags == 2) {
//             if (njets <= 5) return 13;
//             if (njets == 6) return 14;
//             if (njets >= 7) return 15;
//         } else if (nbtags >= 3) {
//             if (njets <= 4) return 16;
//             if (njets == 5) return 17;
//             if (njets >= 6) return 18;
//         }
//     }
//     return -1;
// }
// int getNsrsTTTT() { return 18; }


#endif
