#pragma GCC diagnostic ignored "-Wsign-compare"

#include <iostream>
#include <fstream>

// Following 3 includes are for ctrl-c handling
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include "yaml-cpp/yaml.h"

#include "CORE/CMS3.h"

#include "CORE/SSSelections.h"
#include "CORE/ElectronSelections.h"
#include "CORE/MuonSelections.h"
#include "CORE/Tools/datasetinfo/getDatasetInfo.h"

#include "lep_utils.hpp"
#include "hist_utils.hpp"

int N_EVENTS_CURRENT = 0;
bool STOP_REQUESTED = false;

ofstream* logger;

void event_summary(vector<RecoLepton>& els, vector<RecoLepton>& mus, vector<RecoLepton>& taus, int nJet, int nBJet) {
    cout << "---Event " << N_EVENTS_CURRENT << endl;
    cout << "Els: \n  ";
    for(auto& lep : els) cout << lep << ", ";
    cout << "\nMus: \n  ";
    for(auto& lep : mus) cout << lep << ", ";
    cout << "\nTaus: \n  ";
    for(auto& lep : taus) cout << lep << ", ";
    cout << endl;
}

float mc_weight(DatasetInfoFromFile& df) {
    using namespace tas;
    float sgnMCweight = ((genps_weight() > 0) - (genps_weight() < 0));
    float scale1fb = sgnMCweight*df.getScale1fbFromFile(evt_dataset()[0].Data(),evt_CMS3tag()[0].Data());
    float int_lumi = 75;
    return int_lumi*scale1fb;
}


bool matched_to_set(const RecoLepton& lep, const vector<RecoLepton*>& leps, float min_pt_cut = 20, float dR_cut=0.4) {
    for (auto lep2 : leps) {
        if (abs(lep2->id) != 11 and abs(lep2->id) != 13) continue;
        float dR = ROOT::Math::VectorUtil::DeltaR(lep2->p4, lep.p4);
        if (dR < dR_cut) {
            *logger << "tau " << lep.idx << " matched w/ (" << lep2->id << ", " << lep2->idx << ") w/ dR=" << dR << endl;
            return true;
        }
    }
    *logger << "tau " << lep.idx << " passed cross-cleaning." << endl;
    return false;
}

struct Leptons {
    vector<RecoLepton> els;
    vector<RecoLepton> mus;
    vector<RecoLepton> taus;
    vector<GenLepton> gen_taus;

    vector<RecoLepton*> good_leps;
    vector<RecoLepton*> fakable_leps;
    vector<RecoLepton*> pos_leps;
    vector<RecoLepton*> neg_leps;

    vector<RecoLepton*> sel_leps;

    bool is_ss = false;
    bool is_multilep = false;

    int nGoodEls = 0;
    int nGoodMus = 0;
    int nGoodTaus = 0;

    int nSelEls = 0;
    int nSelMus = 0;
    int nSelTaus = 0;

    Leptons(bool ignore_taus, bool cross_clean_taus, bool truth_match_taus)
      :els(RecoLepton::els()), mus(RecoLepton::mus()),
       taus(RecoLepton::taus()), gen_taus(GenLepton::taus()) {
        using namespace std;

        for (auto& lep : els) {
            if (lep.isFakable) fakable_leps.push_back(&lep);
            if (lep.isGood) {
                good_leps.push_back(&lep);
                nGoodEls++;
            }
        }
        for (auto& lep : mus) {
            if (lep.isFakable) fakable_leps.push_back(&lep);
            if (lep.isGood) {
                good_leps.push_back(&lep);
                nGoodMus++;
            }
        }

        if (!ignore_taus) {
            match(taus, gen_taus, 0.3);

            for (auto& lep : taus) {
                if (lep.isFakable) fakable_leps.push_back(&lep);
                if (!lep.isGood) continue;
                if (cross_clean_taus and matched_to_set(lep, fakable_leps)) continue;
                if (truth_match_taus and lep.match == nullptr) continue;
                good_leps.push_back(&lep);
                nGoodTaus++;
            }
        }

        sort(good_leps.begin(), good_leps.end(),
             [](const RecoLepton* lep1, const RecoLepton* lep2){
                 return lep1->p4.pt() > lep2->p4.pt();
             });

        for (auto& lep : good_leps) {
            if(lep->charge > 0) pos_leps.push_back(lep);
            else                neg_leps.push_back(lep);
        }

        auto z_veto = [](const RecoLepton* l1, const RecoLepton* l2) {
            float invMass = (l1->p4 + l2->p4).M();
            return abs(l1->id) == abs(l2->id) and (abs(invMass - 91.18) < 15 or invMass < 12);
        };

        std::vector<RecoLepton*> pos_sel_leps;
        std::vector<RecoLepton*> neg_sel_leps;
        bool pos_z_veto = false;
        bool neg_z_veto = false;

        // Case positive SS pair passing pt reqs
        if (pos_leps.size() >= 2 and pos_leps[0]->p4.pt() > 25 and pos_leps[1]->p4.pt() > 20) {
            pos_sel_leps.push_back(pos_leps[0]);
            pos_sel_leps.push_back(pos_leps[1]);
            for (const RecoLepton* lep : neg_leps) {
                if ((abs(lep->id) == 11 and lep->p4.pt() < 7) or
                    (abs(lep->id) == 13 and lep->p4.pt() < 5)) continue;
                if (z_veto(lep, pos_leps[0]) or z_veto(lep, pos_leps[1])) {
                    pos_z_veto = true;
                    break;
                }
            }
        }

        // Case negative SS pair passing pt reqs
        if (neg_leps.size() >= 2 and neg_leps[0]->p4.pt() > 25 and neg_leps[1]->p4.pt() > 20) {
            neg_sel_leps.push_back(neg_leps[0]);
            neg_sel_leps.push_back(neg_leps[1]);
            for (const RecoLepton* lep : pos_leps) {
                if ((abs(lep->id) == 11 and lep->p4.pt() < 7) or
                    (abs(lep->id) == 13 and lep->p4.pt() < 5)) continue;
                if (z_veto(lep, neg_leps[0]) or z_veto(lep, neg_leps[1])) {
                    neg_z_veto = true;
                    break;
                }
            }
        }

        bool ss_pair = false;
        if (!pos_sel_leps.empty() and !pos_z_veto) {
            sel_leps = pos_sel_leps;
            ss_pair = true;
        } else if (!neg_sel_leps.empty() and !neg_z_veto) {
            sel_leps = neg_sel_leps;
            ss_pair = true;
        }

        if (ss_pair) {
            is_ss = true;
            is_multilep = false;

            auto is_same_lep = [](RecoLepton* lep1, RecoLepton* lep2) {
                return abs(lep1->id) == abs(lep2->id) and lep1->idx == lep2->idx;
            };

            // Search for a additional leptons not already in the SS pair w/ sufficient pt
            for (RecoLepton* lep : good_leps) {
                if (is_same_lep(lep, sel_leps[0]) or is_same_lep(lep, sel_leps[1])) continue;
                if (lep->p4.pt() < 20) continue;
                sel_leps.push_back(lep);
                is_ss = false;
                is_multilep = true;
            }
        }

        for (auto& sel_lep : sel_leps) {
            switch (abs(sel_lep->id)) {
                case 11: nSelEls++;
                         break;
                case 13: nSelMus++;
                         break;
                case 15: nSelTaus++;
            }
        }
    }
};

struct Jets {
    float ht = 0;
    int nJet = 0;
    int nBJet = 0;

    Jets(const Leptons& leps) {
        auto& jets_p4 = tas::pfjets_p4();
        int nJetsAll = jets_p4.size();
        for (size_t idx=0; idx<nJetsAll; idx++) {
            if (!isLoosePFJet_50nsV1(idx)) continue;
            bool matched_lepton = false;
            for (auto& lep : leps.fakable_leps) {
                if (lep->p4.pt() < 10) continue;
                float dR = ROOT::Math::VectorUtil::DeltaR(jets_p4[idx], lep->p4);
                if (dR < 0.4) {
                    matched_lepton = true;
                    break;
                }
            }
            if (matched_lepton) continue;
            float jetPt = jets_p4[idx].pt();
            float jetEta = jets_p4[idx].eta();
            float disc = tas::getbtagvalue("deepFlavourJetTags:probb",idx) +
                         tas::getbtagvalue("deepFlavourJetTags:probbb",idx);
            if (abs(jetEta) > 2.4) continue;
            if (jetPt > 40) {
                nJet++;
                ht += jetPt;
            }
            if (jetPt > 25 and disc>0.6324) nBJet++;
        }
    }

};

int to_SR(int n_sr, const Leptons& leps, const Jets& jets, float met) {
    if(jets.ht < 300 or met < 50) return -1;
    auto is_ss = leps.is_ss;
    auto is_multilep = leps.is_multilep;
    auto nBJet = jets.nBJet;
    auto nJet = jets.nJet;
    if (n_sr == 8) {
        if (is_ss       and nBJet == 2 and nJet == 6)                return 1;
        if (is_ss       and nBJet == 2 and nJet == 7)                return 2;
        if (is_ss       and nBJet == 2 and nJet >= 8)                return 3;
        if (is_ss       and nBJet == 3 and (nJet == 5 or nJet == 6)) return 4;
        if (is_ss       and nBJet == 3 and nJet >= 7)                return 5;
        if (is_ss       and nBJet >= 4 and nJet >= 5)                return 6;
        if (is_multilep and nBJet == 2 and nJet >= 5)                return 7;
        if (is_multilep and nBJet >= 3 and nJet >= 4)                return 8;
        return -1;
    } else if (n_sr == 16) {
//         if (is_ss       and nBJet == 2 and nJet <= 5) return 1;  // CRW
        if (is_ss       and nBJet == 2 and nJet == 6) return 1;
        if (is_ss       and nBJet == 2 and nJet == 7) return 2;
        if (is_ss       and nBJet == 2 and nJet >= 8) return 3;

        if (is_ss       and nBJet == 3 and nJet == 5) return 4;
        if (is_ss       and nBJet == 3 and nJet == 6) return 5;
        if (is_ss       and nBJet == 3 and nJet == 7) return 6;
        if (is_ss       and nBJet == 3 and nJet >= 8) return 7;

        if (is_ss       and nBJet >= 4 and nJet == 5) return 8;
        if (is_ss       and nBJet >= 4 and nJet == 6) return 9;
        if (is_ss       and nBJet >= 4 and nJet >= 7) return 10;

        if (is_multilep and nBJet == 2 and nJet == 5) return 11;
        if (is_multilep and nBJet == 2 and nJet == 6) return 12;
        if (is_multilep and nBJet == 2 and nJet >= 7) return 13;

        if (is_multilep and nBJet >= 3 and nJet == 4) return 14;
        if (is_multilep and nBJet >= 3 and nJet == 5) return 15;
        if (is_multilep and nBJet >= 3 and nJet >= 6) return 16;
        return -1;
    } else {
        return -1;
    }
}

int ScanChain(const std::string& dataset, const std::string& config_filename) {
    using namespace std;
    using namespace tas;

    cout << "Finding yields with dataset=" << dataset << endl;
    YAML::Node config = YAML::LoadFile(config_filename);

    auto join = [](const vector<string> v) {
        stringstream res;
        for (auto s : v) res << s;
        return TString(res.str());
    };

    string output_path = config["output_path"].as<string>("output/");
    system(("mkdir -p " + output_path).c_str());
    system(("cp " + config_filename + " " + output_path + "/config.yaml").c_str());
    TChain *chain = new TChain("Events");
    TFile dest_file(join({output_path, "/", "yield_", dataset, ".root"}), "RECREATE");

    string data_dir = config[dataset]["dir"].as<string>();
    for (auto fname : config[dataset]["files"].as<vector<string>>()) {
        chain->Add(join({data_dir, "/", fname}));
    }

    if (config["logging_enabled"].as<bool>(false))
        logger = new ofstream(output_path + "/log_" + dataset + ".txt");
    else
        logger = new ofstream("/dev/null");

    int n_sr = config["n_sr"].as<int>(8);

    float xsec_scale = 1.0;
    if (dataset == "tttt") {
        xsec_scale = 11.97 / 9.103;
    }

    signal(SIGINT, [](int){
        cout << "SIGINT Caught, stopping after current event" << endl;
        STOP_REQUESTED=true;
    });
    STOP_REQUESTED=false;

    // Initialize the MVA for b-tagging
    createAndInitMVA("CORE", true, false, 80);
    // instantiate and load dataset information (used for event-weighting)
    DatasetInfoFromFile df;
    df.loadFromFile("CORE/Tools/datasetinfo/scale1fbs.txt");

    N_EVENTS_CURRENT = 0;
    int nEventsChain = chain->GetEntries();
    int events_max = config["events_max"].as<int>(-1);
    if (events_max > nEventsChain or events_max <= 0) {
        events_max = nEventsChain;
    }

    // good_ratio is needed to compensate for some corrupted data files (this is just the ratio of events in the chain
    // to the number of events in the dataset from whence the weights have been calculated)
    float good_ratio = config[dataset]["good_ratio"].as<float>(1.0);
    float event_fraction = (float(events_max) / nEventsChain) * good_ratio;

    TFile *currentFile = 0;
    TObjArray *listOfFiles = chain->GetListOfFiles();
    TIter fileIter(listOfFiles);

    TH1F h_nEls("nEls",   "nEls",  10, -0.5, 9.5);
    TH1F h_nMus("nMus",   "nMus",  10, -0.5, 9.5);
    TH1F h_nTaus("nTaus", "nTaus", 10, -0.5, 9.5);
    TH1F h_nGenTaus("nGenTaus", "nGenTaus", 10, -0.5, 9.5);
    TH2F h_nGen_v_RecoTaus("nGen_v_RecoTaus", "nGen_v_RecoTaus", 10, -0.5, 9.5, 10, -0.5, 9.5);

    TH1F h_nEls_in_SR("nEls_in_SR",   "nEls_in_SR",  10, -0.5, 9.5);
    TH1F h_nMus_in_SR("nMus_in_SR",   "nMus_in_SR",  10, -0.5, 9.5);
    TH1F h_nTaus_in_SR("nTaus_in_SR", "nTaus_in_SR", 10, -0.5, 9.5);
    TH1F h_nGenTaus_in_SR("nGenTaus_in_SR", "nGenTaus_in_SR", 10, -0.5, 9.5);
    TH2F h_nGen_v_RecoTaus_in_SR("nGen_v_RecoTaus_in_SR", "nGen_v_RecoTaus_in_SR", 5, -0.5, 4.5, 5, -0.5, 4.5);

    TH1F h_nSelEls("nSelEls",   "nSelEls",  5, -0.5, 4.5);
    TH1F h_nSelMus("nSelMus",   "nSelMus",  5, -0.5, 4.5);
    TH1F h_nSelTaus("nSelTaus", "nSelTaus", 5, -0.5, 4.5);

    TH1F h_ht("ht", "Ht", 15, 100, 1600);
    TH1F h_met("met", "met", 15, 0, 600);
    TH1F h_njet("njet", "njet", 12, -0.5, 11.5);
    TH1F h_nbjet("nbjet", "nbjet", 7, -0.5, 6.5);

    TH1F h_ht_in_SR("ht_in_SR", "Ht in SR", 15, 100, 1600);
    TH1F h_met_in_SR("met_in_SR", "met in SR", 15, 0, 600);
    TH1F h_njet_in_SR("njet_in_SR", "njet in SR", 12, -0.5, 11.5);
    TH1F h_nbjet_in_SR("nbjet_in_SR", "nbjet in SR", 7, -0.5, 6.5);

    TH1F h_reco_tau_genmatch_id("h_reco_tau_genmatch_id", "h_reco_tau_genmatch_id", 5, -0.5, 4.5);

    EfficiencyMeasurement tau_purity_v_pt("tau_purity_v_pt", 20, 0, 300);
    EfficiencyMeasurement tau_purity_v_eta("tau_purity_v_eta", 20, -3, 3);
    EfficiencyMeasurement2D tau_purity_v_pt_nJet("tau_purity_v_pt_nJet", 20, 0, 300, 16, -0.5, 15.5);
    EfficiencyMeasurement2D tau_purity_v_eta_nJet("tau_purity_v_eta_nJet", 20, -3, 3, 16, -0.5, 15.5);

    EfficiencyMeasurement tau_efficiency_v_pt("tau_efficiency_v_pt", 20, 0, 300);
    EfficiencyMeasurement tau_efficiency_v_eta("tau_efficiency_v_eta", 20, -3, 3);
    EfficiencyMeasurement2D tau_efficiency_v_pt_nJet("tau_efficiency_v_pt_nJet", 20, 0, 300, 16, -0.5, 15.5);
    EfficiencyMeasurement2D tau_efficiency_v_eta_nJet("tau_efficiency_v_eta_nJet", 20, -3, 3, 16, -0.5, 15.5);

    // Signal region for ID'd Selected taus
    std::vector<TH1F> tau_SRs = {
        {"SRs_0tau", "SRs_0tau", n_sr, 0.5, n_sr+.5},
        {"SRs_1tau", "SRs_1tau", n_sr, 0.5, n_sr+.5},
        {"SRs_2tau", "SRs_2tau", n_sr, 0.5, n_sr+.5},
    };

    // Signal region for Truth Matched Selected taus
    std::vector<TH1F> tmtau_SRs = {
        {"SRs_0tmtau", "SRs_0tmtau", n_sr, 0.5, n_sr+.5},
        {"SRs_1tmtau", "SRs_1tmtau", n_sr, 0.5, n_sr+.5},
        {"SRs_2tmtau", "SRs_2tmtau", n_sr, 0.5, n_sr+.5},
    };

    std::vector<TH2F> tmtau_SRs_diff_nGenTau = {
        {"SRs_0tmtau_diff_nGenTau", "SRs_0tmtau_diff_nGenTau", n_sr, 0.5, n_sr+.5, 5, -0.5, 4.5},
        {"SRs_1tmtau_diff_nGenTau", "SRs_1tmtau_diff_nGenTau", n_sr, 0.5, n_sr+.5, 5, -0.5, 4.5},
        {"SRs_2tmtau_diff_nGenTau", "SRs_2tmtau_diff_nGenTau", n_sr, 0.5, n_sr+.5, 5, -0.5, 4.5},
    };

    TH1F ignore_tau_SRs("ignore_tau_SRs", "ignore_tau_SRs", n_sr, 0.5, n_sr+.5);

    while ((currentFile = (TFile*)fileIter.Next())) {
        if(STOP_REQUESTED) break;

        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        cms3.Init(tree);

        for (unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {
            if(STOP_REQUESTED) break;

            cms3.GetEntry(event);
            N_EVENTS_CURRENT++;
            if (N_EVENTS_CURRENT == events_max) STOP_REQUESTED = true;

            CMS3::progress(N_EVENTS_CURRENT, events_max);

            Leptons leps(false, true, false);
            Jets jets(leps);

            float met = tas::evt_pfmet();

            // Calculate the monte-carlo event weight
            float weight = mc_weight(df) / event_fraction;
            weight *= xsec_scale;


            h_nEls.Fill(leps.nGoodEls, weight);
            h_nMus.Fill(leps.nGoodMus, weight);
            h_nTaus.Fill(leps.nGoodTaus, weight);
            h_nGenTaus.Fill(leps.gen_taus.size(), weight);
            h_nGen_v_RecoTaus.Fill(leps.gen_taus.size(), leps.nGoodTaus, weight);

            h_met.Fill(met, weight);
            h_ht.Fill(jets.ht, weight);
            h_njet.Fill(jets.nJet, weight);
            h_nbjet.Fill(jets.nBJet, weight);

            // Fill SRs where taus are included and broken down into
            // categories based on # of taus
            int theSR = to_SR(n_sr, leps, jets, met);
            if (theSR != -1) {
                tau_SRs[min(leps.nSelTaus, 2)].Fill(theSR, weight);

                h_nEls_in_SR.Fill(leps.nGoodEls, weight);
                h_nMus_in_SR.Fill(leps.nGoodMus, weight);
                h_nTaus_in_SR.Fill(leps.nGoodTaus, weight);
                h_nGenTaus_in_SR.Fill(leps.gen_taus.size(), weight);
                h_nGen_v_RecoTaus_in_SR.Fill(leps.gen_taus.size(), leps.nSelTaus, weight);

                h_met_in_SR.Fill(met, weight);
                h_ht_in_SR.Fill(jets.ht, weight);
                h_njet_in_SR.Fill(jets.nJet, weight);
                h_nbjet_in_SR.Fill(jets.nBJet, weight);

                h_nSelEls.Fill(leps.nSelEls, weight);
                h_nSelMus.Fill(leps.nSelMus, weight);
                h_nSelTaus.Fill(leps.nSelTaus, weight);

                for (auto tau : leps.good_leps) {
                    if (abs(tau->id) != 15) continue;
                    if (tau->p4.pt() < 20 ) continue;
                    bool pass = tau->match != nullptr;
                    tau_purity_v_pt.fill(tau->p4.pt(), pass);
                    tau_purity_v_eta.fill(tau->p4.eta(), pass);
                    tau_purity_v_pt_nJet.fill(tau->p4.pt(), jets.nJet, pass);
                    tau_purity_v_eta_nJet.fill(tau->p4.eta(), jets.nJet, pass);
                }

                // Selection Efficiency
                std::vector<RecoLepton> sel_taus;
                for (auto sel_lep : leps.sel_leps) {
                    if (abs(sel_lep->id) == 15) sel_taus.push_back(*sel_lep);
                }
                match(sel_taus, leps.gen_taus, 0.3);
                for (auto& gentau : leps.gen_taus) {
                    bool pass =  gentau.match != nullptr;
                    tau_efficiency_v_pt.fill(gentau.p4.pt(), pass);
                    tau_efficiency_v_eta.fill(gentau.p4.eta(), pass);
                    tau_efficiency_v_pt_nJet.fill(gentau.p4.pt(), jets.nJet, pass);
                    tau_efficiency_v_eta_nJet.fill(gentau.p4.eta(), jets.nJet, pass);
                }
                for (const auto& lep : sel_taus) {
                    int match_id = closest_prompt_gen_match(lep);
                    h_reco_tau_genmatch_id.Fill(match_id);
                }
            }


            Leptons leps_ignore_tau(true, false, false);
            Jets jets_ignore_tau(leps_ignore_tau);

            // Fill SRs where taus are ignored
            theSR = to_SR(n_sr, leps_ignore_tau, jets_ignore_tau, met);
            if (theSR != -1) {
                ignore_tau_SRs.Fill(theSR, weight);
            }

            Leptons leps_tmtau(false, true, true);
            Jets jets_tmtau(leps_tmtau);

            // Fill SRs where taus are truth-matched
            theSR = to_SR(n_sr, leps_tmtau, jets_tmtau, met);
            if (theSR != -1) {
                tmtau_SRs[min(leps_tmtau.nSelTaus, 2)].Fill(theSR, weight);
                tmtau_SRs_diff_nGenTau[min(leps_tmtau.nSelTaus, 2)].Fill(theSR, leps_tmtau.gen_taus.size(), weight);
            }
        }  // event loop
        delete file;
    }  // file loop
    cout << "\nLoop Finished. Saving Results... " << flush;

    dest_file.cd();

    ignore_tau_SRs.Write();
    for (auto& sr : tau_SRs) sr.Write();
    for (auto& sr : tmtau_SRs) sr.Write();
    for (auto& sr : tmtau_SRs_diff_nGenTau) sr.Write();


    h_nEls.Write();
    h_nMus.Write();
    h_nTaus.Write();
    h_nGenTaus.Write();
    h_nGen_v_RecoTaus.Write();

    h_nEls_in_SR.Write();
    h_nMus_in_SR.Write();
    h_nTaus_in_SR.Write();
    h_nGenTaus_in_SR.Write();
    h_nGen_v_RecoTaus_in_SR.Write();

    h_nSelEls.Write();
    h_nSelMus.Write();
    h_nSelTaus.Write();

    h_met.Write();
    h_ht.Write();
    h_njet.Write();
    h_nbjet.Write();

    h_met_in_SR.Write();
    h_ht_in_SR.Write();
    h_njet_in_SR.Write();
    h_nbjet_in_SR.Write();

    h_reco_tau_genmatch_id.Write();

    tau_purity_v_pt.save();
    tau_purity_v_eta.save();
    tau_purity_v_pt_nJet.save();
    tau_purity_v_eta_nJet.save();

    tau_efficiency_v_pt.save();
    tau_efficiency_v_eta.save();
    tau_efficiency_v_pt_nJet.save();
    tau_efficiency_v_eta_nJet.save();

    cout << "Done!" << endl;

    cout << "Summary: " << endl
         << "Ran Over " << N_EVENTS_CURRENT << " total events" << endl;
    delete logger;
    return 0;
}
