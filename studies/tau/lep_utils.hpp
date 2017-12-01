#ifndef LEP_UTILS_CPP
#define LEP_UTILS_CPP

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <algorithm>

#include "Math/VectorUtil.h"

#include "CORE/CMS3.h"
#include "CORE/SSSelections.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> P4;

namespace PDG {
    enum PDG {
        DOWN    = 1,  DOWN_B    = -1,
        UP      = 2,  UP_B      = -2,
        STRANGE = 3,  STRANGE_B = -3,
        CHARM   = 4,  CHARM_B   = -4,
        BOTTOM  = 5,  BOTTOM_B  = -5,
        TOP     = 6,  TOP_B     = -6,
        E       = 11, E_B       = -11,
        NU_E    = 12, NU_E_B    = -12,
        MU      = 13, MU_B      = -13,
        NU_MU   = 14, NU_MU_B   = -14,
        TAU     = 15, TAU_B     = -15,
        NU_TAU  = 16, NU_TAU_B  = -16,
        W       = 24, W_B       = -24
    };
}

namespace TAU_DECAY_MODE {
    enum TAU_DECAY_MODE{
        UNDEF = 0,
        ONE_PRONG = 1,
        THREE_PRONG = 3,
        E = 11,
        MU = 13
    };
}

map<string, int> id_lookup = {
    {"byTightIsolationMVArun2v1DBoldDMwLT", -1},
    {"byTightIsolationMVArun2v1DBdR03oldDMwLT", -1}
};
bool id_lookup_populated = false;
bool isGoodTau(size_t idx, const string& id_name = "byTightIsolationMVArun2v1DBoldDMwLT") {
    //Find index of specific ids for first event only, cache result
    if (!id_lookup_populated){
        const vector<TString> &pf_IDnames = tas::taus_pf_IDnames();
        for(unsigned int idx=0; idx<pf_IDnames.size(); idx++){
            string s = string(pf_IDnames[idx]);
            if(id_lookup.find(s) == id_lookup.end()) continue;
            id_lookup[s] = idx;
        }
        id_lookup_populated = true;
    }
    const vector<vector<float>> &pf_IDs = tas::taus_pf_IDs();
    return pf_IDs[idx][id_lookup[id_name]];
}

struct RecoLepton {
    unsigned int idx;
    int charge;
    int id;
    P4 p4;
    bool isGood;

    RecoLepton() {};
    virtual ~RecoLepton() {};

    static vector<RecoLepton> els(){
        vector<RecoLepton> recos;
        size_t nReco = tas::els_p4().size();
        for (size_t recoIdx=0; recoIdx<nReco; recoIdx++) {
            RecoLepton reco;
            reco.idx = recoIdx;
            reco.charge = tas::els_charge()[recoIdx];
            reco.id = (reco.charge<0) ? 11 : -11;
            reco.p4 = tas::els_p4()[recoIdx];
            reco.isGood = isGoodLepton(reco.id, reco.idx);
            if(reco.isGood) recos.push_back(reco);
        }
        return recos;
    };

    static vector<RecoLepton> mus(){
        vector<RecoLepton> recos;
        size_t nReco = tas::mus_p4().size();
        for (size_t recoIdx=0; recoIdx<nReco; recoIdx++) {
            RecoLepton reco;
            reco.idx = recoIdx;
            reco.charge = tas::mus_charge()[recoIdx];
            reco.id = (reco.charge<0) ? 13 : -13;
            reco.p4 = tas::mus_p4()[recoIdx];
            reco.isGood = isGoodLepton(reco.id, reco.idx);
            if(reco.isGood) recos.push_back(reco);
        }
        return recos;
    };

    static vector<RecoLepton> taus() {
        vector<RecoLepton> recos;
        unsigned int nReco = tas::taus_pf_p4().size();
        for (unsigned int recoIdx=0; recoIdx<nReco; recoIdx++) {
            RecoLepton reco;
            reco.idx = recoIdx;
            reco.charge = tas::taus_pf_charge()[recoIdx];
            reco.id = (reco.charge<0) ? 15 : -15;
            reco.p4 = tas::taus_pf_p4()[recoIdx];
            reco.isGood = isGoodTau(reco.idx);
            if (reco.isGood) recos.push_back(reco);
        }
        return recos;
    };
};

std::ostream& operator<<(std::ostream& os, const RecoLepton& reco) {
    os << "< RecoLepton: idx="  << reco.idx
       << ", id=" << reco.id
       << ", pt=" << reco.p4.pt()
       << ", isGood=" << reco.isGood
       << " >";
    return os;
}

#endif  // LEP_UTILS_CPP
