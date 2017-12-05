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

bool isFakebleTau(size_t idx){
    return false; //TODO: Do something more reasonable here.
                  // false just exludes taus from the jet cross-cleaning
}

struct RecoLepton;  // Forward Declaration

struct GenLepton {
    struct Child {
        unsigned int idx;
        int id;
        float charge;
        P4 p4;
        int status;
    };

    unsigned int idx;
    int status;
    int decay_mode;  // Really only valid on taus
    P4 p4;
    int id;
    int mother_id;
    int gmother_id;
    vector<Child> children;

    RecoLepton* match = nullptr;
    float matched_dR = 100;

    GenLepton(){};
    virtual ~GenLepton(){};

    P4 vis_p4(){
        P4 _p4;
        for(const Child& child : children){
            if(abs(child.id) != 12 &&
               abs(child.id) != 14 &&
               abs(child.id) != 16){
                _p4 += child.p4;
            }
        }
        return _p4;
    }

    bool passes_acceptance(float max_eta=2.4) const{
        return abs(p4.eta()) < max_eta;
    }

    bool is_hadronic(){
        return decay_mode == 1 or
               decay_mode == 3;
    }

    void set_decay_mode(){
        unsigned int prong_count = 0;
        for (auto child : children) {
            if (abs(child.id) == 11 ){
                decay_mode = 11;
                return;
            }
            else if(abs(child.id) == 13){
                decay_mode = 13;
                return;
            }
            else if (abs(child.charge) == 1) {
              prong_count++;
            }
        }
        if(prong_count == 1)
            decay_mode = 1;
        else if(prong_count == 3)
            decay_mode = 3;
        else
            decay_mode = -1;
    }

    static vector<GenLepton> taus(){
        unsigned int Ngen = tas::genps_p4().size();

        std::vector<unsigned int> gentauidxs;
        // Just find all non-status 23 taus.
        auto& id = tas::genps_id();
        auto& status = tas::genps_status();
        for (unsigned int igen = 0; igen < Ngen; igen++){
            if (abs(id[igen]) == 15 and status[igen] != 23) {
                gentauidxs.push_back(igen);
            }
        }

        vector<GenLepton> gentaus;
        for (auto gentauidx : gentauidxs) {
            GenLepton genlep;
            genlep.idx = gentauidx;
            genlep.status = status[gentauidx];
            genlep.id = id[gentauidx];
            genlep.mother_id = tas::genps_id_mother()[gentauidx];
            genlep.gmother_id = tas::genps_id_simplegrandma()[gentauidx];
            genlep.p4 = tas::genps_p4()[gentauidx];
            for (unsigned int igen = 0; igen < Ngen; igen++){
                if (tas::genps_idx_mother()[igen] == genlep.idx) {
                    genlep.children.push_back({igen,
                                               id[igen],
                                               tas::genps_charge()[igen],
                                               tas::genps_p4()[igen],
                                               status[igen]});
                }
            }
            genlep.set_decay_mode();
            gentaus.push_back(genlep);
        }
        return gentaus;
    };
};

struct RecoLepton {
    unsigned int idx;
    int charge;
    int id;
    P4 p4;
    bool isGood;
    bool isFakable;

    GenLepton* match = nullptr;
    float matched_dR = 100;

    RecoLepton() {};
    virtual ~RecoLepton() {};

    static vector<RecoLepton> els(){
        vector<RecoLepton> recos;
        auto& p4s = tas::els_p4();
        auto& charges = tas::els_charge();
        size_t nReco = p4s.size();
        for (size_t recoIdx=0; recoIdx<nReco; recoIdx++) {
            RecoLepton reco;
            reco.idx = recoIdx;
            reco.charge = charges[recoIdx];
            reco.id = (reco.charge<0) ? 11 : -11;
            reco.p4 = p4s[recoIdx];
            reco.isGood = isGoodLepton(reco.id, reco.idx);
            reco.isFakable = isFakableElectron(reco.idx);
            recos.push_back(reco);
        }
        return recos;
    };

    static vector<RecoLepton> mus(){
        vector<RecoLepton> recos;
        auto& p4s = tas::mus_p4();
        auto& charges = tas::mus_charge();
        size_t nReco = p4s.size();
        for (size_t recoIdx=0; recoIdx<nReco; recoIdx++) {
            RecoLepton reco;
            reco.idx = recoIdx;
            reco.charge = charges[recoIdx];
            reco.id = (reco.charge<0) ? 13 : -13;
            reco.p4 = p4s[recoIdx];
            reco.isGood = isGoodLepton(reco.id, reco.idx);
            reco.isFakable = isFakableMuon(reco.idx);
            recos.push_back(reco);
        }
        return recos;
    };

    static vector<RecoLepton> taus() {
        vector<RecoLepton> recos;
        auto& p4s = tas::taus_pf_p4();
        auto& charges = tas::taus_pf_charge();
        size_t nReco = p4s.size();
        for (unsigned int recoIdx=0; recoIdx<nReco; recoIdx++) {
            RecoLepton reco;
            reco.idx = recoIdx;
            reco.charge = charges[recoIdx];
            reco.id = (reco.charge<0) ? 15 : -15;
            reco.p4 = p4s[recoIdx];
            reco.isGood = isGoodTau(reco.idx);
            reco.isFakable = isFakebleTau(reco.idx);
            recos.push_back(reco);
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

template <typename T1, typename T2>
void match(vector<T1>& setA, vector<T2>& setB, float dR_window){
    for(T1& lepA : setA){
        lepA.match = nullptr;
        lepA.matched_dR = dR_window;
    }
    for(T2& lepB : setB){
        float min_dR = dR_window;
        lepB.match = nullptr;
        for(T1& lepA : setA){
            float dR = ROOT::Math::VectorUtil::DeltaR(lepB.p4, lepA.p4);
            if(dR < min_dR){
                min_dR = dR;
                lepB.match = &lepA;
                lepB.matched_dR = dR;
            }
        }
        if(lepB.match != nullptr and lepB.matched_dR < lepB.match->matched_dR){
            lepB.match->match = &lepB;
            lepB.match->matched_dR = lepB.matched_dR;
        }
    }
}

#endif  // LEP_UTILS_CPP
