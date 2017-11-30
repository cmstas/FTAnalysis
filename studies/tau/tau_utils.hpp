#ifndef TAU_UTILS_CPP
#define TAU_UTILS_CPP

#include <iostream>
#include <fstream>
#include <map>
#include <set>

#include "Math/VectorUtil.h"

#include "CORE/CMS3.h"

struct GenTau;
struct RecoTau : public RecoLepton {

    GenTau* matched_gentau = nullptr;
    float matched_dR = 100;

    RecoTau() {};
    virtual ~RecoTau() {};

    static vector<RecoTau> all(){
        vector<RecoTau> recoTaus;
        unsigned int NrecoTau = tas::taus_pf_p4().size();
        for(unsigned int recoIdx=0; recoIdx<NrecoTau; recoIdx++){
            RecoTau recotau;
            recotau.idx = recoIdx;
            recotau.p4 = tas::taus_pf_p4()[recoIdx];
            recotau.construct_id();
            recoTaus.push_back(recotau);
        }
        return recoTaus;
    };
};

struct GenTau {

    struct Child {
        unsigned int idx;
        PDG::PDG pid;
        float charge;
        P4 p4;
        int status;
    };

    unsigned int idx;
    int status;
    TAU_DECAY_MODE::TAU_DECAY_MODE decay_mode;
    P4 p4;
    PDG::PDG pid;
    PDG::PDG mother_pid;
    PDG::PDG gmother_pid;
    vector<Child> children;

    bool from_top_w;

    RecoTau* matched_recoTau = nullptr;
    float matched_dR = 100;

    GenTau(){};
    virtual ~GenTau(){};

    P4 vis_p4(){
        P4 _p4;
        for(const Child& child : children){
            if(abs(child.pid) != 12 &&
               abs(child.pid) != 14 &&
               abs(child.pid) != 16){
                _p4 += child.p4;
            }
        }
        return _p4;
    }

    bool passes_acceptance(float max_eta=2.4) const{
        return abs(p4.eta()) < max_eta;
    }

    bool is_hadronic(){
        return decay_mode == TAU_DECAY_MODE::ONE_PRONG or
               decay_mode == TAU_DECAY_MODE::THREE_PRONG;
    }

    void set_decay_mode(){
        unsigned int prong_count = 0;
        for (auto child : children) {
            if (abs(child.pid) == PDG::E ){
                decay_mode = TAU_DECAY_MODE::E;
                return;
            }
            else if(abs(child.pid) == PDG::MU){
                decay_mode = TAU_DECAY_MODE::MU;
                return;
            }
            else if (abs(child.charge) == 1) {
              prong_count++;
            }
        }
        if(prong_count == 1)
            decay_mode = TAU_DECAY_MODE::ONE_PRONG;
        else if(prong_count == 3)
            decay_mode = TAU_DECAY_MODE::THREE_PRONG;
        else
            decay_mode = TAU_DECAY_MODE::UNDEF;
    }

    /** Get idxs of all taus from top->W->tau decay chain.
     */
    static void from_w_top(set<unsigned int>& top_w_idxs){
        unsigned int Ngen = tas::genps_p4().size();

        // Loop over the vector of gen particles
        for (unsigned int igen = 0; igen < Ngen; igen++){
            // PDG id
            int id = tas::genps_id()[igen];
            // mother particle PDG ID
            int mid = tas::genps_id_mother()[igen];
            if (abs(id) != PDG::W) continue; // skip if not W
            if (abs(mid) != PDG::TOP) continue; // skip if not from top
            std::pair<int,int> daughter_indices = {-1,-1};
            // Loop through genparticles again to look for daughters (i.e., which particles have a mother which is igen)
            for (unsigned int idau = 0; idau < Ngen; idau++){
                int midx = tas::genps_idx_mother()[idau];
                if (abs(tas::genps_id()[idau]) == PDG::TAU && tas::genps_status()[idau] == 23) continue; // status 23 taus
                if (midx != igen) continue;
                // Fill first, then second, then break once both are in,
                // since we can have duplicate particles with different statuses
                if (daughter_indices.first == -1){
                    daughter_indices.first = idau;
                } else{
                    daughter_indices.second = idau;
                    break;
                }
            }
            // Stop if we didn't find both children (should never happen (i think))
            if (daughter_indices.first == -1 || daughter_indices.second == -1) continue;
            unsigned int dau_id1 = abs(tas::genps_id()[daughter_indices.first]);
            unsigned int dau_id2 = abs(tas::genps_id()[daughter_indices.second]);
            if (dau_id1 == PDG::TAU && dau_id2 == PDG::NU_TAU){
                top_w_idxs.insert(daughter_indices.first);
            } else if (dau_id1 == PDG::NU_TAU && dau_id2 == PDG::TAU){
                top_w_idxs.insert(daughter_indices.second);
            }
        }
    }

    static vector<GenTau> build(){
        std::set<unsigned int> top_w_idxs;
        from_w_top(top_w_idxs);

        unsigned int Ngen = tas::genps_p4().size();

        std::vector<unsigned int> gentauidxs;
        // Just find all non-status 23 taus.
        for (unsigned int igen = 0; igen < Ngen; igen++){
            if (abs(tas::genps_id()[igen]) != PDG::TAU) continue;
            if (tas::genps_status()[igen] == 23) continue; // status 23 taus
            gentauidxs.push_back(igen);
        }

        vector<GenTau> gentaus;
        for (auto gentauidx : gentauidxs) {
            GenTau gentau;
            gentau.idx = gentauidx;
            gentau.status = tas::genps_status()[gentauidx];
            gentau.pid = (PDG::PDG)tas::genps_id()[gentauidx];
            gentau.mother_pid = (PDG::PDG)tas::genps_id_mother()[gentauidx];
            gentau.gmother_pid = (PDG::PDG)tas::genps_id_simplegrandma()[gentauidx];
            gentau.p4 = tas::genps_p4()[gentauidx];
            gentau.from_top_w = top_w_idxs.find(gentauidx) != top_w_idxs.end();
            for (unsigned int igen = 0; igen < Ngen; igen++){
                if (tas::genps_idx_mother()[igen] == gentau.idx) {
                    gentau.children.push_back({igen,
                                               (PDG::PDG)tas::genps_id()[igen],
                                               tas::genps_charge()[igen],
                                               tas::genps_p4()[igen],
                                               tas::genps_status()[igen]});
                }
            }
            gentau.set_decay_mode();
            gentaus.push_back(gentau);
        }
        return gentaus;
    };

};

std::ostream& operator<<(std::ostream& os, const RecoTau& recoTau) {
    os << "RecoTau:" << endl
       << "  idx: " << recoTau.idx << endl
       << "  passes_id: " << recoTau.passes_id() << endl
       << "  passes_acceptance: " << recoTau.passes_acceptance() << endl
       << "  pt: " << recoTau.p4.pt() << endl
       << "  eta: " << recoTau.p4.eta() << endl
       << "  phi: " << recoTau.p4.phi() << endl;
    if(recoTau.matched_gentau == nullptr){
        os << "  matched_gentau: N/A"  << endl;
    } else{
        const GenTau& gentau = *recoTau.matched_gentau;
        os << "  matched_gentau:"  << endl
           << "    matched_recoTau: " << gentau.matched_recoTau->idx << endl
           << "    matched_dR: " << gentau.matched_dR << endl
           << "    status: " << gentau.status << endl
           << "    pt: " << gentau.p4.pt() << endl
           << "    eta: " << gentau.p4.eta() << endl
           << "    phi: " << gentau.p4.phi() << endl
           << "    idx: " << gentau.idx << endl
           << "    pid: " << gentau.pid << endl
           << "    mother_pid: " << gentau.mother_pid << endl;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const GenTau& gentau) {
    os << "GenTau:"  << endl
       << "  status: " << gentau.status << endl
       << "  decay_mode: " << gentau.decay_mode << endl
       << "  pt: " << gentau.p4.pt() << endl
       << "  eta: " << gentau.p4.eta() << endl
       << "  phi: " << gentau.p4.phi() << endl
       << "  idx: " << gentau.idx << endl
       << "  pid: " << gentau.pid << endl
       << "  mother_pid: " << gentau.mother_pid << endl
       << "  gmother_pid: " << gentau.gmother_pid << endl
       << "  children: " << endl;
    for (const GenTau::Child& child : gentau.children) {
        os << "    child: " << child.idx << endl
           << "      pid: " << child.pid << endl
           << "      charge: " << child.charge << endl
           << "      status: " << child.status << endl;
    }
    if (gentau.matched_recoTau == nullptr) {
        os << "  matched_recoTau: N/A"  << endl;
    } else {
        const RecoTau& recoTau = *gentau.matched_recoTau;
        os << "  matched_recoTau: "  << endl
           << "    matched_gentau: " << recoTau.matched_gentau->idx << endl
           << "    matched_dR: " << recoTau.matched_dR << endl
           << "    idx: " << recoTau.idx << endl
           << "    pt: " << recoTau.p4.pt() << endl
           << "    eta: " << recoTau.p4.eta() << endl
           << "    phi: " << recoTau.p4.phi() << endl;
    }
    return os;
}

void match(vector<GenTau>& gentaus, vector<RecoTau>& recoTaus, float dR_window){
    for(GenTau& gentau : gentaus){
        gentau.matched_recoTau = nullptr;
        gentau.matched_dR = dR_window;
    }
    for(RecoTau& recoTau : recoTaus){
        float min_dR = dR_window;
        recoTau.matched_gentau = nullptr;
        for(GenTau& gentau : gentaus){
            float dR = ROOT::Math::VectorUtil::DeltaR(recoTau.p4, gentau.p4);
            if(dR < min_dR){
                min_dR = dR;
                recoTau.matched_gentau = &gentau;
                recoTau.matched_dR = dR;
            }
        }
        if(recoTau.matched_gentau != nullptr and recoTau.matched_dR < recoTau.matched_gentau->matched_dR){
            recoTau.matched_gentau->matched_recoTau = &recoTau;
            recoTau.matched_gentau->matched_dR = recoTau.matched_dR;
        }
    }
}

void fill_tau_ids(std::map<string, TH1F> &hists){

    const vector<TString> &pf_IDnames = tas::taus_pf_IDnames();
    const vector<vector<float>> &pf_IDs = tas::taus_pf_IDs();

    unsigned int idx = 0;
    for(const TString& id_name_t : pf_IDnames){
        string id_name(id_name_t);
        auto s_result = hists.find(id_name);
        if(s_result == hists.end()){
            hists[id_name] = TH1F(id_name_t, id_name_t, 10000, -200, 200);
        }
        for(const vector<float>& id_vals: pf_IDs){
            hists[id_name].Fill(id_vals.at(idx));
        }
        idx++;
    }
}

#endif  // TAU_UTILS_CPP
