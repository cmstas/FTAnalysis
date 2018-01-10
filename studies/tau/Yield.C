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

#include "CORE/CMS3.h"

#include "CORE/SSSelections.h"
#include "CORE/ElectronSelections.h"
#include "CORE/MuonSelections.h"
#include "CORE/Tools/datasetinfo/getDatasetInfo.h"

#include "lep_utils.hpp"
#include "hist_utils.hpp"

int N_EVENTS_CURRENT = 0;
bool STOP_REQUESTED = false;

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

float mcWeight(DatasetInfoFromFile& df) {
    using namespace tas;
    float sgnMCweight = ((genps_weight() > 0) - (genps_weight() < 0));
    float scale1fb = sgnMCweight*df.getScale1fbFromFile(evt_dataset()[0].Data(),evt_CMS3tag()[0].Data());
    float moriond_int_lumi = 35.9;
    return moriond_int_lumi*scale1fb;
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

    bool is_ss = false;
    bool is_multilep = false;

    int nGoodEls = 0;
    int nGoodMus = 0;
    int nGoodTaus = 0;

    Leptons(int tau_selection)
      :els(RecoLepton::els()), mus(RecoLepton::mus()),
       taus(RecoLepton::taus()), gen_taus(GenLepton::taus()) {
        using namespace std;

        for (auto& lep : els) {
            if (lep.isGood) {
                good_leps.push_back(&lep);
                nGoodEls++;
            }
            if (lep.isFakable) fakable_leps.push_back(&lep);
        }
        for (auto& lep : mus) {
            if (lep.isGood) {
                good_leps.push_back(&lep);
                nGoodMus++;
            }
            if (lep.isFakable) fakable_leps.push_back(&lep);
        }

        match(taus, gen_taus, 0.3);

        for (auto& lep : taus) {
            switch (tau_selection) {
                case 0:
                   break;
                case 1:
                   if (lep.isGood) good_leps.push_back(&lep);
                   if (lep.isFakable) fakable_leps.push_back(&lep);
                   break;
                case 2:
                   if (lep.isGood and lep.match != nullptr) good_leps.push_back(&lep);
                   if (lep.isFakable) fakable_leps.push_back(&lep);
                   break;
            }
            if (lep.isGood) nGoodTaus++;
        }

        sort(good_leps.begin(), good_leps.end(),
             [](const RecoLepton* lep1, const RecoLepton* lep2){
                 return lep1->p4.pt() > lep2->p4.pt();
             });

        for (auto& lep : good_leps) {
            if(lep->charge > 0) pos_leps.push_back(lep);
            else                neg_leps.push_back(lep);
        }

        auto zVeto = [](const RecoLepton* l1, const RecoLepton* l2) {
            float invMass = (l1->p4 + l2->p4).M();
            return abs(l1->id) == abs(l2->id) and (abs(invMass - 91.18) < 15 or invMass < 12);
        };

        bool pos_ss_pair = false;
        bool neg_ss_pair = false;
        bool zVeto_pos = false;
        bool zVeto_neg = false;
        // Case positive SS pair passing pt reqs
        if (pos_leps.size() >= 2 and pos_leps[0]->p4.pt() > 25 and pos_leps[1]->p4.pt() > 20) {
            pos_ss_pair = true;
            for (const RecoLepton* lep : neg_leps) {
                if(zVeto(lep, pos_leps[0]) or zVeto(lep, pos_leps[1])) {
                    zVeto_pos = true;
                    break;
                }
            }
        }

        // Case negative SS pair passing pt reqs
        if (neg_leps.size() >= 2 and neg_leps[0]->p4.pt() > 25 and neg_leps[1]->p4.pt() > 20) {
            neg_ss_pair = true;
            for (const RecoLepton* lep : pos_leps) {
                if(zVeto(lep, neg_leps[0]) or zVeto(lep, neg_leps[1])) {
                    zVeto_neg = true;
                    break;
                }
            }
        }

        bool ss_pair = pos_ss_pair or neg_ss_pair;
        bool passes_zVeto = !(zVeto_pos or zVeto_neg);

        if (ss_pair and passes_zVeto) {
            if (good_leps.size() > 2 and good_leps[2]->p4.pt() > 20) {
                is_multilep = true;
            } else {
                is_ss = true;
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

int to_SR(const Leptons& leps, const Jets& jets) {
    auto is_ss = leps.is_ss;
    auto is_multilep = leps.is_multilep;
    auto nBJet = jets.nBJet;
    auto nJet = jets.nJet;
    if (is_ss       and nBJet == 2 and nJet == 6) return 1;
    if (is_ss       and nBJet == 2 and nJet == 7) return 2;
    if (is_ss       and nBJet == 2 and nJet >= 8) return 3;
    if (is_ss       and nBJet == 3 and (nJet == 5 or nJet == 6)) return 4;
    if (is_ss       and nBJet == 3 and nJet >= 7) return 5;
    if (is_ss       and nBJet >= 4 and nJet >= 5) return 6;
    if (is_multilep and nBJet == 2 and nJet >= 5) return 7;
    if (is_multilep and nBJet >= 3 and nJet >= 4) return 8;
    return -1;
}

int ScanChain(TChain *ch, TFile* dest_file) {
    using namespace std;
    using namespace tas;

    // tau_selection:
    //   0: no taus
    //   1: all id'd taus
    //   2: only id'd taus w/ gen match

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
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    TH1F nEls("nEls",   "nEls",  10, -0.5, 9.5);
    TH1F nMus("nMus",   "nMus",  10, -0.5, 9.5);
    TH1F nTaus("nTaus", "nTaus", 10, -0.5, 9.5);
    TH1F nGenTaus("nGenTaus", "nGenTaus", 10, -0.5, 9.5);
    TH2F nGen_v_RecoTaus("nGen_v_RecoTaus", "nGen_v_RecoTaus", 10, -0.5, 9.5, 10, -0.5, 9.5);

    TH1F h_ht("ht", "Ht", 15, 100, 1600);
    TH1F h_met("met", "met", 15, 0, 600);
    TH1F h_njet("njet", "njet", 12, -0.5, 11.5);
    TH1F h_nbjet("nbjet", "nbjet", 7, -0.5, 6.5);

    TH1F h_ht_in_SR("ht_in_SR", "Ht in SR", 15, 100, 1600);
    TH1F h_met_in_SR("met_in_SR", "met in SR", 15, 0, 600);
    TH1F h_njet_in_SR("njet_in_SR", "njet in SR", 12, -0.5, 11.5);
    TH1F h_nbjet_in_SR("nbjet_in_SR", "nbjet in SR", 7, -0.5, 6.5);

    EfficiencyMeasurement tau_purity_v_pt("tau_purity_v_pt", 20, 0, 300);
    EfficiencyMeasurement tau_purity_v_eta("tau_purity_v_eta", 20, -3, 3);
    EfficiencyMeasurement2D tau_purity_v_pt_nJet("tau_purity_v_pt_nJet", 20, 0, 300, 16, -0.5, 15.5);
    EfficiencyMeasurement2D tau_purity_v_eta_nJet("tau_purity_v_eta_nJet", 20, -3, 3, 16, -0.5, 15.5);

    EfficiencyMeasurement tau_efficiency_v_pt("tau_efficiency_v_pt", 20, 0, 300);
    EfficiencyMeasurement tau_efficiency_v_eta("tau_efficiency_v_eta", 20, -3, 3);
    EfficiencyMeasurement2D tau_efficiency_v_pt_nJet("tau_efficiency_v_pt_nJet", 20, 0, 300, 16, -0.5, 15.5);
    EfficiencyMeasurement2D tau_efficiency_v_eta_nJet("tau_efficiency_v_eta_nJet", 20, -3, 3, 16, -0.5, 15.5);

    TH1F SRs_0tau("SRs_0tau", "SRs_0tau", 8, 0.5, 8.5);
    TH1F SRs_1tau("SRs_1tau", "SRs_1tau", 8, 0.5, 8.5);
    TH1F SRs_2tau("SRs_2tau", "SRs_2tau", 8, 0.5, 8.5);
    TH1F SRs_3tau("SRs_3tau", "SRs_3tau", 8, 0.5, 8.5);
    TH1F SRs_4tau("SRs_4tau", "SRs_4tau", 8, 0.5, 8.5);

    TH1F* SRs[] = {&SRs_0tau, &SRs_1tau,
                   &SRs_2tau, &SRs_3tau,
                   &SRs_4tau};


    while ((currentFile = (TFile*)fileIter.Next())) {
        if(STOP_REQUESTED) break;

        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        cms3.Init(tree);

        for (unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {
            if(STOP_REQUESTED) break;

            cms3.GetEntry(event);
            N_EVENTS_CURRENT++;

            /* if (N_EVENTS_CURRENT > 5000) break; */
            CMS3::progress(N_EVENTS_CURRENT, nEventsChain);

            Leptons leps(0);
            Jets jets(leps);

            float met = tas::evt_pfmet();

            // Calculate the monte-carlo event weight
            float weight = mcWeight(df);

            nEls.Fill(leps.nGoodEls, weight);
            nMus.Fill(leps.nGoodMus, weight);
            nTaus.Fill(leps.nGoodTaus, weight);
            nGenTaus.Fill(leps.gen_taus.size(), weight);
            nGen_v_RecoTaus.Fill(leps.gen_taus.size(), leps.nGoodTaus, weight);


            bool evnt_reqs = jets.ht > 300 and met > 50;
            int theSR = to_SR(leps, jets);
            if (evnt_reqs and theSR != -1) {
                SRs[min(leps.nGoodTaus, 4)]->Fill(theSR, weight);
            }

            h_met.Fill(met, weight);
            h_ht.Fill(jets.ht, weight);
            h_njet.Fill(jets.nJet, weight);
            h_nbjet.Fill(jets.nBJet, weight);
            if (theSR != -1) {
                h_met_in_SR.Fill(met, weight);
                h_ht_in_SR.Fill(jets.ht, weight);
                h_njet_in_SR.Fill(jets.nJet, weight);
                h_nbjet_in_SR.Fill(jets.nBJet, weight);
            }

            for (auto& tau : leps.taus) {
                if (tau.isGood) {
                    bool pass = tau.match != nullptr;
                    tau_purity_v_pt.fill(tau.p4.pt(), pass);
                    tau_purity_v_eta.fill(tau.p4.eta(), pass);
                    tau_purity_v_pt_nJet.fill(tau.p4.pt(), jets.nJet, pass);
                    tau_purity_v_eta_nJet.fill(tau.p4.eta(), jets.nJet, pass);
                }
            }
            for (auto& gentau : leps.gen_taus) {
                bool pass =  gentau.match != nullptr and gentau.match->isGood;
                tau_efficiency_v_pt.fill(gentau.p4.pt(), pass);
                tau_efficiency_v_eta.fill(gentau.p4.eta(), pass);
                tau_efficiency_v_pt_nJet.fill(gentau.p4.pt(), jets.nJet, pass);
                tau_efficiency_v_eta_nJet.fill(gentau.p4.eta(), jets.nJet, pass);
            }

        }  // event loop
        delete file;
    }  // file loop
    cout << "\nLoop Finished. Saving Results... " << flush;

    dest_file->cd();
    SRs_0tau.Write();
    SRs_1tau.Write();
    SRs_2tau.Write();
    SRs_3tau.Write();
    SRs_4tau.Write();

    nEls.Write();
    nMus.Write();
    nTaus.Write();
    nGenTaus.Write();
    nGen_v_RecoTaus.Write();

    h_ht.Write();
    h_met.Write();
    h_njet.Write();
    h_nbjet.Write();

    h_ht_in_SR.Write();
    h_met_in_SR.Write();
    h_njet_in_SR.Write();
    h_nbjet_in_SR.Write();

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
    return 0;
}
