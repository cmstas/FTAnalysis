#ifndef SIGNALREGIONS_H
#define SIGNALREGIONS_H
int getNsrsTTTTBDT() { return 7; }
int signalRegionBDT(float disc){
    return 1+(int)(disc*getNsrsTTTTBDT());
}

/**
 * Cut-Based SR definitions
 */
struct SRDefinition {
    static const int nSRs = 19;

    static int SR3(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
        if (lep1pt < 25.) return -1;
        if (lep2pt < 20.) return -1;
        if (njets < 2) return -1;
        if (nbtags < 2) return -1;
        if (ht < 300) return -1;
        if (isClass6) return 1;
        if (nleps == 2) {
            if (nbtags == 2) {
                if (njets <= 5) return 2;
            }
        }
        return 3;
    }

    static int SR4(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
        if (lep1pt < 25.) return -1;
        if (lep2pt < 20.) return -1;
        if (njets < 2) return -1;
        if (nbtags < 2) return -1;
        if (ht < 300) return -1;
        if (isClass6) return 1;
        if (nleps == 2) {
            if (nbtags == 2) {
                if (njets <= 5) return 2;
            }
        } else {
            return 3;
        }
        return 4;
    }

    static int SR7(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
        if (lep1pt < 25.) return -1;
        if (lep2pt < 20.) return -1;
        if (njets < 2) return -1;
        if (nbtags < 2) return -1;
        if (ht < 300) return -1;
        if (isClass6) return 1;
        if (nleps == 2) {
            if (nbtags == 2) {
                if (njets <= 5) return 2;
                return 3;
            }
            if (nbtags == 3) return 4;
            if (nbtags >= 4) return 5;
        } else {
            if (nbtags == 2) return 6;
            if (nbtags >= 3) return 7;
        }
        return -1;
    }

    static int SR8(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
        if (lep1pt < 25.) return -1;
        if (lep2pt < 20.) return -1;
        if (njets < 2) return -1;
        if (nbtags < 2) return -1;
        if (ht < 300) return -1;
        if (isClass6) return -1;
        if (nleps == 2) {
            if (nbtags == 2) {
                if (njets <= 5) return -1;
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

    static int SR10(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, int nleps, int isClass6){
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

    static int SR19(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, float lep3pt, int nleps, int isClass6) {
        if (lep1pt < 25.) return -1;
        if (lep2pt < 20.) return -1;
        if (njets < 2) return -1;
        if (nbtags < 2) return -1;
        if (ht < 300) return -1;
        if (nleps>=3 && isClass6) return 1;  // CRZ
        /* if (isClass6) return 1;  // CRZ */
        if (nleps == 2) {
            if (!isClass6 and 10 < lep3pt  and lep3pt < 20) return 19;  // 3rd lepton veto
            if (nbtags == 2) {
                if (njets <= 5) return 2;  // CRW
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

    static int SR(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, float lep3pt, int nleps, int isClass6) {
        int theSR = -1;
        switch (nSRs) {
            case 3:
                theSR = SR3(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, nleps, isClass6);
                break;
            case 4:
                theSR = SR4(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, nleps, isClass6);
                break;
            case 7:
                theSR = SR7(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, nleps, isClass6);
                break;
            case 8:
                theSR = SR8(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, nleps, isClass6);
                break;
            case 10:
                theSR = SR10(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, nleps, isClass6);
                break;
            case 19:
                theSR = SR19(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, lep3pt, nleps, isClass6);
                break;
            default:
                std::cout << "Error: SR" << nSRs << " not implemented!" << std::endl;
        }
        return theSR;
    }
};


#endif
