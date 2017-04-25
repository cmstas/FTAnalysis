int getNsrsTTTTBDT() { return 7; }
int signalRegionBDT(float disc){
    return 1+(int)(disc*getNsrsTTTTBDT());
}

int getNsrsTTTT() { return 9; }
int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
    if (lep1pt < 25.) return -1;
    if (lep2pt < 20.) return -1;
    if (njets < 2) return -1;
    if (nbtags < 2) return -1;
    if (ht < 300) return -1;
    if (isClass6) return 1;
    if (nleps == 2) {
        if (nbtags == 2) {
            if (njets == 2) return 2;
            if (njets == 3) return 2;
            if (njets == 4) return 2;
            if (njets == 5) return 2;
            if (njets == 6) return 2;
            if (njets == 7) return 3;
            if (njets >= 8) return 4;
        } else if (nbtags == 3) {
            if (njets == 3) return -1;
            if (njets == 4) return -1;
            if (njets == 5) return 5;
            if (njets == 6) return 5;
            if (njets >= 7) return 6;
        } else if (nbtags >= 4) {
            if (njets == 4) return -1;
            if (njets == 5) return 7;
            if (njets == 6) return 7;
            if (njets >= 7) return 7;
        }
    } else {
        if (nbtags == 2) {
            if (njets == 3) return -1;
            if (njets == 4) return -1;
            if (njets == 5) return 8;
            if (njets == 6) return 8;
            if (njets >= 7) return 8;
        } else if (nbtags == 3) {
            if (njets == 4) return 9;
            if (njets >= 5) return 9;
        } else if (nbtags >= 4) {
            if (njets > 3) return 9;
        }
    }
    return -1;
}

// int getNsrsTTTT() { return 10; }
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
//     if (lep1pt < 25.) return -1;
//     if (lep2pt < 20.) return -1;
//     if (njets < 2) return -1;
//     if (nbtags < 2) return -1;
//     if (ht < 300) return -1;
//     if (isClass6) return 1;
//     if (nleps == 2) {
//         if (nbtags == 2) {
//             if (njets == 2) return 2;
//             if (njets == 3) return 2;
//             if (njets == 4) return 2;
//             if (njets == 5) return 2;
//             if (njets == 6) return 2;
//             if (njets == 7) return 3;
//             if (njets >= 8) return 4;
//         } else if (nbtags == 3) {
//             if (njets == 3) return -1;
//             if (njets == 4) return -1;
//             if (njets == 5) return 5;
//             if (njets == 6) return 5;
//             if (njets >= 7) return 6;
//         } else if (nbtags >= 4) {
//             if (njets == 4) return -1;
//             if (njets == 5) return 7;
//             if (njets == 6) return 7;
//             if (njets >= 7) return 7;
//         }
//     } else {
//         if (nbtags == 2) {
//             if (njets == 3) return -1;
//             if (njets == 4) return -1;
//             if (njets == 5) return 8;
//             if (njets == 6) return 8;
//             if (njets >= 7) return 9;
//         } else if (nbtags == 3) {
//             if (njets == 4) return 10;
//             if (njets >= 5) return 10;
//         } else if (nbtags >= 4) {
//             if (njets > 3) return 10;
//         }
//     }
//     return -1;
// }

// int getNsrsTTTT() { return 9; }
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps){
//     if (lep1pt < 25.) return -1;
//     if (lep2pt < 20.) return -1;
//     if (ht >= 300) {
//         if (nleps == 2) {
//             if (nbtags == 2) {
//                 if (njets == 2) return 1;
//                 if (njets == 3) return 1;
//                 if (njets == 4) return 1;
//                 if (njets == 5) return 1;
//                 if (njets == 6) return 1;
//                 if (njets == 7) return 2;
//                 if (njets >= 8) return 3;
//             } else if (nbtags == 3) {
//                 if (njets == 3) return -1;
//                 if (njets == 4) return -1;
//                 if (njets == 5) return 4;
//                 if (njets == 6) return 4;
//                 if (njets >= 7) return 5;
//             } else if (nbtags >= 4) {
//                 if (njets == 4) return -1;
//                 if (njets == 5) return 6;
//                 if (njets == 6) return 6;
//                 if (njets >= 7) return 6;
//             }
//         } else {
//             if (nbtags == 2) {
//                 if (njets == 3) return -1;
//                 if (njets == 4) return -1;
//                 if (njets == 5) return 7;
//                 if (njets == 6) return 7;
//                 if (njets >= 7) return 8;
//             } else if (nbtags == 3) {
//                 if (njets == 4) return 9;
//                 if (njets >= 5) return 9;
//             } else if (nbtags >= 4) {
//                 if (njets > 3) return 9;
//             }
//         }
//     }
//     // 24-4-3+1-3+1-4+1
//     return -1;
// }

// int getNsrsTTTT() { return 13; }
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps){
//     if (ht >= 300) {
//         if (nleps == 2) {
//             if (nbtags == 2) {
//                 if (njets == 2) return 1;
//                 if (njets == 3) return 1;
//                 if (njets == 4) return 1;
//                 if (njets == 5) return 1;
//                 if (njets == 6) return 2;
//                 if (njets == 7) return 3;
//                 if (njets >= 8) return 4;
//             } else if (nbtags == 3) {
//                 if (njets == 3) return -1;
//                 if (njets == 4) return 5;
//                 if (njets == 5) return 6;
//                 if (njets == 6) return 7;
//                 if (njets >= 7) return 8;
//             } else if (nbtags >= 4) {
//                 if (njets == 4) return -1;
//                 if (njets == 5) return 9;
//                 if (njets == 6) return 9;
//                 if (njets >= 7) return 9;
//             }
//         } else {
//             if (nbtags == 2) {
//                 if (njets == 3) return -1;
//                 if (njets == 4) return -1;
//                 if (njets == 5) return 10;
//                 if (njets == 6) return 11;
//                 if (njets >= 7) return 12;
//             } else if (nbtags == 3) {
//                 if (njets == 4) return 13;
//                 if (njets >= 5) return 13;
//             } else if (nbtags >= 4) {
//                 if (njets > 3) return 13;
//             }
//         }
//     }
//     // 24-4-3+1-3+1-4+1
//     return -1;
// }

// int getNsrsTTTT() { return 24; }
// int signalRegionTest(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps){
//     if (ht >= 300) {
//         if (nleps == 2) {
//             if (nbtags == 2) {
//                 if (njets == 2) return 1;
//                 if (njets == 3) return 2;
//                 if (njets == 4) return 3;
//                 if (njets == 5) return 4;
//                 if (njets == 6) return 5;
//                 if (njets == 7) return 6;
//                 if (njets >= 8) return 7;
//             } else if (nbtags == 3) {
//                 if (njets == 3) return 8;
//                 if (njets == 4) return 9;
//                 if (njets == 5) return 10;
//                 if (njets == 6) return 11;
//                 if (njets >= 7) return 12;
//             } else if (nbtags >= 4) {
//                 if (njets == 4) return 13;
//                 if (njets == 5) return 14;
//                 if (njets == 6) return 15;
//                 if (njets >= 7) return 16;
//             }
//         } else {
//             if (nbtags == 2) {
//                 if (njets == 3) return 17;
//                 if (njets == 4) return 18;
//                 if (njets == 5) return 19;
//                 if (njets == 6) return 20;
//                 if (njets >= 7) return 21;
//             } else if (nbtags == 3) {
//                 if (njets == 4) return 22;
//                 if (njets >= 5) return 23;
//             } else if (nbtags >= 4) {
//                 if (njets > 3) return 24;
//             }
//         }
//     }
//     return -1;
// }
