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

map<string, int> tau_id_lookup = {
    {"decayModeFinding", -1},
    {"againstMuonTight3", -1},
    {"againstElectronTightMVA6", -1},
    {"byTightIsolationMVArun2v1DBdR03oldDMwLT", -1}
};

// void print_ids(size_t tau_idx) {
//     const vector<TString> &pf_IDnames = tas::taus_pf_IDnames();
//     const vector<vector<float>> &pf_IDs = tas::taus_pf_IDs();
//     cout << "NEW TAU: " << tau_idx << endl;
//     for(unsigned int idx=0; idx<pf_IDnames.size(); idx++) {
//         std::cout << "  " << pf_IDnames[idx] << ": " << pf_IDs[tau_idx][idx] << std::endl;
//     }
// }

// bool isTauIsoFromBJet(size_t tau_idx, float dR_min = 0.1) {
//     /** Remove taus that are not isolated from B-Jets. The idea being that B->tau
//      *  decays should be removed, only the W->tau should remain.
//      */
//     auto& tau_p4 = tas::taus_pf_p4()[tau_idx];
//     auto& jet_p4s  = tas::pfjets_p4();
//     int nJets = jet_p4s.size();
//     for(size_t jet_idx = 0; jet_idx<nJets; jet_idx++) {
//         float dR = ROOT::Math::VectorUtil::DeltaR(tau_p4, jet_p4s[jet_idx]);
//         float disc = tas::getbtagvalue("deepFlavourJetTags:probb", jet_idx) +
//                      tas::getbtagvalue("deepFlavourJetTags:probbb", jet_idx);
//         if (disc > 0.6324 and dR < dR_min) return false;
//     }
//     return true;
// }

bool ID_LOOKUP_POPULATED = false;
bool isGoodTau (size_t tau_idx) {
    /* print_ids(tau_idx); */
    //Find index of specific ids for first event only, cache result
    if (!ID_LOOKUP_POPULATED){
        const vector<TString> &pf_IDnames = tas::taus_pf_IDnames();
        for(unsigned int idx=0; idx<pf_IDnames.size(); idx++){
            string s = string(pf_IDnames[idx]);
            if(tau_id_lookup.find(s) == tau_id_lookup.end()) continue;
            tau_id_lookup[s] = idx;
        }
        ID_LOOKUP_POPULATED = true;
    }
    const vector<vector<float>> &pf_IDs = tas::taus_pf_IDs();
    for (const auto& p : tau_id_lookup) {
        if (pf_IDs[tau_idx][p.second]) continue;
        return false;
    }
    return true;
}




bool isFakebleTau(size_t idx){
    return isGoodTau(idx);
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
    P4 p4_full; // p4 of particle
    P4 p4;  // Just p4 of visible decay products
    int id;
    int mother_id;
    int gmother_id;
    vector<Child> children;

    RecoLepton* match = nullptr;
    float matched_dR = 100;

    GenLepton(){};
    virtual ~GenLepton(){};


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

    static vector<GenLepton> taus(bool hadronic_only=true) {
        unsigned int Ngen = tas::genps_p4().size();

        std::vector<unsigned int> gentauidxs;

        auto& id = tas::genps_id();
        auto& status = tas::genps_status();
        auto& mother = tas::genps_id_simplemother();
        auto& is_prompt = tas::genps_fromHardProcessDecayed();

        for (unsigned int igen = 0; igen < Ngen; igen++) {
            if ((abs(id[igen]) == 15) and is_prompt[igen]) {
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
            genlep.p4_full = tas::genps_p4()[gentauidx];
            for (unsigned int igen = 0; igen < Ngen; igen++) {
                if (tas::genps_idx_mother()[igen] == genlep.idx) {
                    genlep.children.push_back({igen,
                                               id[igen],
                                               tas::genps_charge()[igen],
                                               tas::genps_p4()[igen],
                                               status[igen]});
                }
            }
            // Calculate visible momentum
            for (const Child& child : genlep.children) {
                if(abs(child.id) != 12 &&
                   abs(child.id) != 14 &&
                   abs(child.id) != 16) {
                    genlep.p4 += child.p4;
                }
            }
            genlep.set_decay_mode();
            if (hadronic_only and !genlep.is_hadronic()) continue;
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

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    if (vec.size() == 0) os << "[]";
    else {
        os << "[" << vec[0];
        for(size_t i=1; i<vec.size(); i++) os << ", " << vec[i];
        os << "]";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const RecoLepton& reco) {
    os << "< RecoLepton: idx="  << reco.idx
       << ", id=" << reco.id
       << ", pt=" << reco.p4.pt()
       << ", isGood=" << reco.isGood
       << ((reco.match==nullptr) ? ", truthFailed" : ", truthMatched")
       << " >";
    return os;
}

std::ostream& operator<<(std::ostream& os, const GenLepton& gen) {
    os << "< GenLepton: idx="  << gen.idx
       << ", id=" << gen.id
       << ", mid=" << gen.mother_id
       << ", gid=" << gen.gmother_id
       << ", pt=" << gen.p4.pt()
       << ((gen.match==nullptr) ? ", truthFailed" : ", truthMatched")
       << " >";
    return os;
}

int closest_prompt_gen_match(const RecoLepton& lep, float dR_window=0.3) {
    auto& lep_p4 = lep.p4;
    float dR_min = dR_window;
    int closest_id = 0;

    auto& ids = tas::genps_id();
    auto& is_prompt = tas::genps_fromHardProcessDecayed();
    auto& p4s = tas::genps_p4();

    for (size_t igen=0; igen<ids.size(); igen++) {
        if (!is_prompt[igen]) continue;
        int id = abs(ids[igen]);
        if (id == 12 or id == 14 or id == 16) continue;
        float dR = ROOT::Math::VectorUtil::DeltaR(p4s[igen], lep_p4);
        if (dR < dR_min) {
            dR_min = dR;
            closest_id = id;
        }
    }
    switch (closest_id) {
        case 0:
            return 0;  // Matched nothing
        case 11:
            return 1;  // Matched electron
        case 13:
            return 2;  // Matched Muon
        case 15:
            return 3;  // Matched Tau
        default:
            return 4;  // Some hadron
    }
}

template <typename T1, typename T2>
void match(vector<T1>& setA, vector<T2>& setB, float dR_window) {
    for (T1& lepA : setA) {
        lepA.match = nullptr;
        lepA.matched_dR = dR_window;
    }
    for (T2& lepB : setB){
        float min_dR = dR_window;
        lepB.match = nullptr;
        for (T1& lepA : setA) {
            float dR = ROOT::Math::VectorUtil::DeltaR(lepB.p4, lepA.p4);
            if (dR < min_dR) {
                min_dR = dR;
                lepB.match = &lepA;
                lepB.matched_dR = dR;
            }
        }
        if (lepB.match != nullptr and lepB.matched_dR < lepB.match->matched_dR) {
            lepB.match->match = &lepB;
            lepB.match->matched_dR = lepB.matched_dR;
        }
    }
}

#endif  // LEP_UTILS_CPP
