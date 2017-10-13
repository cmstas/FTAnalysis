#pragma GCC diagnostic ignored "-Wsign-compare"
#include "../../common/Software/dataMCplotMaker/dataMCplotMaker.h"

#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TH1.h"
#include "TChain.h"
#include "Math/VectorUtil.h"

#include "CMS3.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace tas;

ofstream logger("tau.log");

enum PDG{
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


struct GenTau;


struct PFTau{

    unsigned int idx;
    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> p4;
    int byTightIsolationMVArun2v1DBoldDMwLT;
    GenTau* matched_gentau = nullptr;

    PFTau(){};
    virtual ~PFTau(){};

    bool passes_id() const {
        return byTightIsolationMVArun2v1DBoldDMwLT == 1;
    }

    void construct_id(){
        const vector<vector<float>> &pf_IDs = tas::taus_pf_IDs();
        const vector<TString> &pf_IDnames = tas::taus_pf_IDnames();
        for(int id_idx=0; id_idx<pf_IDnames.size(); id_idx++){
            if(pf_IDnames[id_idx] == "byTightIsolationMVArun2v1DBoldDMwLT"){
                this->byTightIsolationMVArun2v1DBoldDMwLT = pf_IDs[this->idx][id_idx];
                break;
            }
        }
    }

    static vector<PFTau> build(bool only_pass_id=true){
        vector<PFTau> pftaus;
        unsigned int Npftau = tas::taus_pf_p4().size();
        for(unsigned int pfidx=0; pfidx<Npftau; pfidx++){
            PFTau pftau;
            pftau.idx = pfidx;
            pftau.p4 = tas::taus_pf_p4()[pfidx];
            pftau.construct_id();
            if(only_pass_id){
                if(pftau.passes_id())
                    pftaus.push_back(pftau);
            } else{
                pftaus.push_back(pftau);
            }
        }
        return pftaus;
    };

};


struct GenTau{

    unsigned int idx;
    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> p4;
    PDG pid;
    PDG mother_pid;
    std::vector<unsigned int> children_idxs;
    std::vector<PDG> children_pids;

    std::vector<PFTau*> matched_pftau;

    GenTau(){};
    virtual ~GenTau(){};

    static vector<GenTau> build(bool hadronic_only=true){

        std::vector<unsigned int> gentauidxs;
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
                gentauidxs.push_back(daughter_indices.first);
            } else if (dau_id1 == PDG::NU_TAU && dau_id2 == PDG::TAU){
                gentauidxs.push_back(daughter_indices.second);
            }
        }

        vector<int> hadronicgentauidxs;
        vector<GenTau> gentaus;
        for (auto gentauidx : gentauidxs) {
            GenTau gentau;
            gentau.idx = gentauidx;
            gentau.pid = (PDG)tas::genps_id()[gentauidx];
            gentau.mother_pid = (PDG)tas::genps_id_mother()[gentauidx];
            gentau.p4 = tas::genps_p4()[gentauidx];
            for (unsigned int igen = 0; igen < Ngen; igen++){
                if (tas::genps_idx_mother()[igen] == gentau.idx) {
                    gentau.children_idxs.push_back(igen);
                    gentau.children_pids.push_back((PDG)tas::genps_id()[igen]);
                }
            }
            if (hadronic_only){
                bool foundemu = false;
                for (auto id : gentau.children_pids) {
                    if (id == PDG::E || id == PDG::MU){
                        foundemu = true;
                        break;
                    }
                }
                if (!foundemu)
                    gentaus.push_back(gentau);
            } else{
                gentaus.push_back(gentau);
            }
        }
        return gentaus;
    };

};


void match(vector<GenTau>& gentaus, vector<PFTau>& pftaus, float dR){
    for(GenTau& gentau : gentaus){
        gentau.matched_pftau.clear();
    }
    for(PFTau& pftau : pftaus){
        float min_dR = 100;
        pftau.matched_gentau = nullptr;
        for(GenTau& gentau : gentaus){
            float dR = ROOT::Math::VectorUtil::DeltaR(pftau.p4, gentau.p4);
            if(dR < 0.5 and dR < min_dR){
                min_dR = dR;
                pftau.matched_gentau = &gentau;
            }
        }
        if (pftau.matched_gentau != nullptr){
            pftau.matched_gentau->matched_pftau.push_back(&pftau);
        }
    }
}


void log_tau_matches(const vector<PFTau>& pftaus){
    for(const PFTau& pftau : pftaus){
        logger << "PFTau:" << endl
             << "  idx: " << pftau.idx << endl
             << "  pt: " << pftau.p4.pt() << endl
             << "  eta: " << pftau.p4.eta() << endl
             << "  phi: " << pftau.p4.phi() << endl;
        if(pftau.matched_gentau == nullptr){
            logger << "  matched_gentau: N/A"  << endl;
        } else{
            const GenTau& gentau = *pftau.matched_gentau;
            logger << "  matched_gentau:"  << endl
                 << "    dR: " << ROOT::Math::VectorUtil::DeltaR(pftau.p4, gentau.p4) << endl
                 << "    pt: " << gentau.p4.pt() << endl
                 << "    eta: " << gentau.p4.eta() << endl
                 << "    phi: " << gentau.p4.phi() << endl
                 << "    idx: " << gentau.idx << endl
                 << "    pid: " << gentau.pid << endl
                 << "    mother_pid: " << gentau.mother_pid << endl
                 << "    matched_pftaus: (";
             for (const PFTau* pftau : gentau.matched_pftau){
                 logger << pftau->idx << ", ";
             }
             logger << ")" << endl;
        }
    }
}

int ScanChain(TChain *ch){

    // TH1F * h_met = new TH1F("met", "met", 50, 0, 300);
    TH1F h_pf_eta("pf_eta", "pf_eta", 50, -4, 4);
    TH1F h_pf_pt("pf_pt", "pf_pt", 50, 0, 400);
    TH1F h_gen_eta("gen_eta", "gen_eta", 50, -4, 4);
    TH1F h_gen_pt("gen_pt", "gen_pt", 50, 0, 400);

    TH1F h_gen_match_eff_v_pt("gen_match_eff_v_pt", "gen_match_eff_v_pt",             50, 0, 600);
    TH1F h_gen_match_eff_v_pt_num("gen_match_eff_v_pt_num", "gen_match_eff_v_pt_num", 50, 0, 600);
    TH1F h_gen_match_eff_v_pt_den("gen_match_eff_v_pt_den", "gen_match_eff_v_pt_den", 50, 0, 600);

    TH1F h_gen_match_eff_v_eta("gen_match_eff_v_eta", "gen_match_eff_v_eta",             50, -4, 4);
    TH1F h_gen_match_eff_v_eta_num("gen_match_eff_v_eta_num", "gen_match_eff_v_eta_num", 50, -4, 4);
    TH1F h_gen_match_eff_v_eta_den("gen_match_eff_v_eta_den", "gen_match_eff_v_eta_den", 50, -4, 4);

    TH1F h_pf_select_eff_v_pt("pf_select_eff_v_pt", "pf_select_eff_v_pt",             50, 0, 600);
    TH1F h_pf_select_eff_v_pt_num("pf_select_eff_v_pt_num", "pf_select_eff_v_pt_num", 50, 0, 600);
    TH1F h_pf_select_eff_v_pt_den("pf_select_eff_v_pt_den", "pf_select_eff_v_pt_den", 50, 0, 600);

    TH1F h_pf_select_eff_v_eta("pf_select_eff_v_eta", "pf_select_eff_v_eta",             50, -4, 4);
    TH1F h_pf_select_eff_v_eta_num("pf_select_eff_v_eta_num", "pf_select_eff_v_eta_num", 50, -4, 4);
    TH1F h_pf_select_eff_v_eta_den("pf_select_eff_v_eta_den", "pf_select_eff_v_eta_den", 50, -4, 4);

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    while ( (currentFile = (TFile*)fileIter.Next()) ) { 

        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        cms3.Init(tree);

        TString filename(currentFile->GetTitle());

        for( unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            cms3.GetEntry(event);
            nEventsTotal++;

            /* if (event > 10000) break; */

            CMS3::progress(nEventsTotal, nEventsChain);

            // Get all hadronically decaying Generator-Level Taus in the event
            std::vector<GenTau> gentaus = GenTau::build();
            // Get all Particle-Flow Taus in event passing the
            // "byTightIsolationMVArun2v1DBoldDMwLT" Identification Criteria
            std::vector<PFTau> pftaus = PFTau::build();

            for(const PFTau& pftau : pftaus){
                h_pf_eta.Fill(pftau.p4.eta());
                h_pf_pt.Fill(pftau.p4.pt());
            }

            for(const GenTau& gentau : gentaus){
                h_gen_eta.Fill(gentau.p4.eta());
                h_gen_pt.Fill(gentau.p4.pt());
            }

            // Match Genrator Taus w/ Particle-Flow Taus based on a dR window of 0.5
            match(gentaus, pftaus, 0.5);
            /* log_tau_matches(pftaus); */


            // Gen Matching Efficiency (#Gen w/ match / #Gen) vs. genpt
            for(const GenTau& gentau : gentaus){
                h_gen_match_eff_v_pt_den.Fill(gentau.p4.pt());
                if(gentau.matched_pftau.size() > 0){
                    h_gen_match_eff_v_pt_num.Fill(gentau.p4.pt());
                }
            }

            // Gen Matching Efficiency v eta for genpt > 20GeV
            for(const GenTau& gentau : gentaus){
                if(gentau.p4.pt() < 20) continue;
                h_gen_match_eff_v_eta_den.Fill(gentau.p4.eta());
                if(gentau.matched_pftau.size() > 0){
                    h_gen_match_eff_v_eta_num.Fill(gentau.p4.eta());
                }
            }

            // PF Selection Efficiency (#PF w/ match / #PF) vs. pfpt
            for(const PFTau& pftau : pftaus){
                h_pf_select_eff_v_pt_den.Fill(pftau.p4.pt());
                if(pftau.matched_gentau != nullptr){
                    h_pf_select_eff_v_pt_num.Fill(pftau.p4.pt());
                }
            }

            // PF Selection Efficiency v eta for pfpt > 20GeV
            for(const PFTau& pftau : pftaus){
                if(pftau.p4.pt() < 20) continue;
                h_pf_select_eff_v_eta_den.Fill(pftau.p4.eta());
                if(pftau.matched_gentau != nullptr){
                    h_pf_select_eff_v_eta_num.Fill(pftau.p4.eta());
                }
            }

        }  // event loop
        delete file;
    }  // file loop

    TFile out("tau.root", "RECREATE");

    h_pf_eta.Write();
    h_pf_pt.Write();
    h_gen_eta.Write();
    h_gen_pt.Write();

    h_gen_match_eff_v_pt.Sumw2();
    h_gen_match_eff_v_pt.Divide(&h_gen_match_eff_v_pt_num, &h_gen_match_eff_v_pt_den, 1, 1, "B");
    h_gen_match_eff_v_pt.Write();
    h_gen_match_eff_v_pt_num.Write();
    h_gen_match_eff_v_pt_den.Write();

    h_gen_match_eff_v_eta.Sumw2();
    h_gen_match_eff_v_eta.Divide(&h_gen_match_eff_v_eta_num, &h_gen_match_eff_v_eta_den, 1, 1, "B");
    h_gen_match_eff_v_eta.Write();
    h_gen_match_eff_v_eta_num.Write();
    h_gen_match_eff_v_eta_den.Write();

    h_pf_select_eff_v_pt.Sumw2();
    h_pf_select_eff_v_pt.Divide(&h_pf_select_eff_v_pt_num, &h_pf_select_eff_v_pt_den, 1, 1, "B");
    h_pf_select_eff_v_pt.Write();
    h_pf_select_eff_v_pt_num.Write();
    h_pf_select_eff_v_pt_den.Write();

    h_pf_select_eff_v_eta.Sumw2();
    h_pf_select_eff_v_eta.Divide(&h_pf_select_eff_v_eta_num, &h_pf_select_eff_v_eta_den, 1, 1, "B");
    h_pf_select_eff_v_eta.Write();
    h_pf_select_eff_v_eta_num.Write();
    h_pf_select_eff_v_eta_den.Write();

    return 0;
}

