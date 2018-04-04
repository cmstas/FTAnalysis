#ifndef SIGNALREGIONS_H
#define SIGNALREGIONS_H

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

    static int SR18(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, float lep3pt, int nleps, int isClass6) {
        if (lep1pt < 25.) return -1;
        if (lep2pt < 20.) return -1;
        if (njets < 2) return -1;
        if (nbtags < 2) return -1;
        if (ht < 300) return -1;
        if (isClass6 && nleps < 3) return -1;  // Z-Veto w/ low pt 3rd lepton
        if (isClass6 && nleps >= 3) return 1;  // CRZ
        if (nleps == 2) {
            /* if (10 < lep3pt  and lep3pt < 20) return -1;  // 3rd lepton veto */
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

    static int SR19(int njets, int nbtags, float met, float ht, float mt_min, int id1, int id2, float lep1pt, float lep2pt, float lep3pt, int nleps, int isClass6) {
        if (lep1pt < 25.) return -1;
        if (lep2pt < 20.) return -1;
        if (njets < 2) return -1;
        if (nbtags < 2) return -1;
        if (ht < 300) return -1;
        if (isClass6 && nleps < 3) return -1;  // Z-Veto w/ low pt 3rd lepton
        if (isClass6 && nleps >= 3) return 1;  // CRZ
        if (nleps == 2) {
            if (10 < lep3pt  and lep3pt < 20) return 19;  // 3rd lepton veto
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
        switch (nSRs) {
            case 3:
                return SR3(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, nleps, isClass6);
            case 4:
                return SR4(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, nleps, isClass6);
            case 7:
                return SR7(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, nleps, isClass6);
            case 8:
                return SR8(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, nleps, isClass6);
            case 10:
                return SR10(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, nleps, isClass6);
            case 18:
                return SR18(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, lep3pt, nleps, isClass6);
            case 19:
                return SR19(njets, nbtags, met, ht, mt_min, id1, id2, lep1pt, lep2pt, lep3pt, nleps, isClass6);
            default:
                std::cout << "Error: SR" << nSRs << " not implemented!" << std::endl;
                return -1;
        }
    }
};

#endif
