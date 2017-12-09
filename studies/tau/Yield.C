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

typedef vector<RecoLepton> RecoLeptons;

int N_EVENTS_TOTAL = 0;
bool STOP_REQUESTED = false;

void event_summary(RecoLeptons els, RecoLeptons mus, RecoLeptons taus, int nJet, int nBJet) {
    cout << "---Event " << N_EVENTS_TOTAL << endl;
    cout << "Els: \n  ";
    for(auto& lep : els) cout << lep << ", ";
    cout << "\nMus: \n  ";
    for(auto& lep : mus) cout << lep << ", ";
    cout << "\nTaus: \n  ";
    for(auto& lep : taus) cout << lep << ", ";
    cout << endl;
}

int ScanChain(TChain *ch, TFile* dest_file, int tau_selection) {
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

    createAndInitMVA("CORE", true, false, 80);
    // instantiate and load dataset information
    DatasetInfoFromFile df;
    df.loadFromFile("CORE/Tools/datasetinfo/scale1fbs.txt");

    N_EVENTS_TOTAL = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    TH1F nEls("nEls",   "nEls",  10, -0.5, 9.5);
    TH1F nMus("nMus",   "nMus",  10, -0.5, 9.5);
    TH1F nTaus("nTaus", "nTaus", 10, -0.5, 9.5);

    TH1F h_ht("ht", "Ht", 15, 100, 1600);
    TH1F h_met("met", "met", 15, 0, 600);
    TH1F h_njet("njet", "njet", 12, -0.5, 11.5);
    TH1F h_nbjet("nbjet", "nbjet", 7, -0.5, 6.5);

    TH1F h_ht_in_SR("ht_in_SR", "Ht in SR", 15, 100, 1600);
    TH1F h_met_in_SR("met_in_SR", "met in SR", 15, 0, 600);
    TH1F h_njet_in_SR("njet_in_SR", "njet in SR", 12, -0.5, 11.5);
    TH1F h_nbjet_in_SR("nbjet_in_SR", "nbjet in SR", 7, -0.5, 6.5);

    EfficiencyMeasurement tau_purity("tau_purity_v_pt", 50, 0, 300);

    float sr[] = {0, 0, 0, 0, 0, 0, 0, 0};
    TH1F SRs("SRs", "SRs", 8, 0.5, 8.5);


    while ((currentFile = (TFile*)fileIter.Next())) {
        if(STOP_REQUESTED) break;

        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        cms3.Init(tree);

        for (unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {
            if(STOP_REQUESTED) break;

            cms3.GetEntry(event);
            N_EVENTS_TOTAL++;

            /* if (N_EVENTS_TOTAL > 5000) break; */
            CMS3::progress(N_EVENTS_TOTAL, nEventsChain);

            // Get all leptons in event that pass appropriate selection.
            RecoLeptons all_good_leps;
            RecoLeptons all_fakable_leps;
            int nGoodEls = 0;
            int nGoodMus = 0;
            int nGoodTaus = 0;
            for(auto& lep : RecoLepton::els()) {
                if (lep.isGood) {
                    all_good_leps.push_back(lep);
                    nGoodEls++;
                }
                if (lep.isFakable) all_fakable_leps.push_back(lep);
            }
            for(auto& lep : RecoLepton::mus()) {
                if (lep.isGood) {
                    all_good_leps.push_back(lep);
                    nGoodMus++;
                }
                if (lep.isFakable) all_fakable_leps.push_back(lep);
            }
            vector<RecoLepton> taus_good;
            for (auto& lep : RecoLepton::taus()) {
                if (lep.isGood) taus_good.push_back(lep);
                if (tau_selection > 0 and lep.isFakable) all_fakable_leps.push_back(lep);
            }
            vector<GenLepton> gentaus_hadronic;
            for (auto& gentau : GenLepton::taus()) {
                if (gentau.is_hadronic()) gentaus_hadronic.push_back(gentau);
            }

            match(taus_good, gentaus_hadronic, 0.3);

            for (auto& lep : taus_good) {
                switch (tau_selection){
                    case 0:
                       break;
                    case 1:
                       all_good_leps.push_back(lep);
                       break;
                    case 2:
                       if (lep.match != nullptr) all_good_leps.push_back(lep);
                       break;
                }
            }

            sort(all_good_leps.begin(), all_good_leps.end(),
                 [](const RecoLepton& lep1, const RecoLepton& lep2){
                     return lep1.p4.pt() > lep2.p4.pt();
                 });

            RecoLeptons pos_leps;
            RecoLeptons neg_leps;
            for (auto& lep : all_good_leps) {
                if(lep.charge > 0) pos_leps.push_back(lep);
                else neg_leps.push_back(lep);
            }

            auto zVeto = [](const RecoLepton& l1, const RecoLepton& l2){
                float invMass = (l1.p4 + l2.p4).M();
                return abs(l1.id) == abs(l2.id) and (abs(invMass - 91.18) < 15 or invMass < 12);
            };

            bool pos_ss_pair = false;
            bool neg_ss_pair = false;
            bool zVeto_pos = false;
            bool zVeto_neg = false;
            // Case positive SS pair passing pt reqs
            if (pos_leps.size() >= 2 and pos_leps[0].p4.pt() > 25 and pos_leps[1].p4.pt() > 20) {
                pos_ss_pair = true;
                for (const RecoLepton& lep : neg_leps) {
                    if(zVeto(lep, pos_leps[0]) or zVeto(lep, pos_leps[1])) {
                        zVeto_pos = true;
                        break;
                    }
                }
            }

            // Case negative SS pair passing pt reqs
            if (neg_leps.size() >= 2 and neg_leps[0].p4.pt() > 25 and neg_leps[1].p4.pt() > 20) {
                neg_ss_pair = true;
                for (const RecoLepton& lep : pos_leps) {
                    if(zVeto(lep, neg_leps[0]) or zVeto(lep, neg_leps[1])) {
                        zVeto_neg = true;
                        break;
                    }
                }
            }

            bool ss_pair = pos_ss_pair or neg_ss_pair;
            bool passes_zVeto = !(zVeto_pos or zVeto_neg);

            bool is_ss = false;
            bool is_multilep = false;
            if (ss_pair and passes_zVeto){
                if (all_good_leps.size() > 2 and all_good_leps[2].p4.pt() > 20) {
                    is_multilep = true;
                } else {
                    is_ss = true;
                }
            }

            float met = tas::evt_pfmet();
            float ht = 0;
            int nJet = 0;
            int nBJet = 0;
            auto& jets_p4 = tas::pfjets_p4();
            int nJetsAll = jets_p4.size();
            for (size_t idx=0; idx<nJetsAll; idx++) {
                if (!isLoosePFJet_50nsV1(idx)) continue;
                bool matched_lepton = false;
                for (auto& lep : all_fakable_leps) {
                    float dR = ROOT::Math::VectorUtil::DeltaR(jets_p4[idx], lep.p4);
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

            /* if (all_good_leps.size() > 0) event_summary(els, mus, taus, nJet, nBJet); */

            // Calculate the monte-carlo event weight
            float sgnMCweight = ((genps_weight() > 0) - (genps_weight() < 0));
            float scale1fb = sgnMCweight*df.getScale1fbFromFile(evt_dataset()[0].Data(),evt_CMS3tag()[0].Data());
            float moriond_int_lumi = 35.9;
            float weight = moriond_int_lumi*scale1fb;

            nEls.Fill(nGoodEls, weight);
            nMus.Fill(nGoodMus, weight);
            nTaus.Fill(nGoodTaus, weight);


            bool evnt_reqs = ht > 300 and met > 50;
            bool inSR = false;
            if (evnt_reqs) {
                if (is_ss       and nBJet == 2 and nJet == 6)                {SRs.Fill(1, weight); inSR = true;}
                if (is_ss       and nBJet == 2 and nJet == 7)                {SRs.Fill(2, weight); inSR = true;}
                if (is_ss       and nBJet == 2 and nJet >= 8)                {SRs.Fill(3, weight); inSR = true;}
                if (is_ss       and nBJet == 3 and (nJet == 5 or nJet == 6)) {SRs.Fill(4, weight); inSR = true;}
                if (is_ss       and nBJet == 3 and nJet >= 7)                {SRs.Fill(5, weight); inSR = true;}
                if (is_ss       and nBJet >= 4 and nJet >= 5)                {SRs.Fill(6, weight); inSR = true;}
                if (is_multilep and nBJet == 2 and nJet >= 5)                {SRs.Fill(7, weight); inSR = true;}
                if (is_multilep and nBJet >= 3 and nJet >= 4)                {SRs.Fill(8, weight); inSR = true;}
            }

            h_ht.Fill(ht, weight);
            h_met.Fill(tas::evt_pfmet(), weight);
            h_njet.Fill(nJet, weight);
            h_nbjet.Fill(nBJet, weight);
            if (inSR) {
                h_ht_in_SR.Fill(ht, weight);
                h_met_in_SR.Fill(met, weight);
                h_njet_in_SR.Fill(nJet, weight);
                h_nbjet_in_SR.Fill(nBJet, weight);
            }

            for (auto& tau : taus_good)
                tau_purity.fill(tau.p4.pt(), tau.match != nullptr);

        }  // event loop
        delete file;
    }  // file loop
    cout << "\nLoop Finished. Saving Results... " << flush;

    dest_file->cd();
    SRs.Write();

    nEls.Write();
    nMus.Write();
    nTaus.Write();

    h_ht.Write();
    h_met.Write();
    h_njet.Write();
    h_nbjet.Write();

    h_ht_in_SR.Write();
    h_met_in_SR.Write();
    h_njet_in_SR.Write();
    h_nbjet_in_SR.Write();

    tau_purity.save();

    cout << "Done!" << endl;

    cout << "Summary: " << endl
         << "Ran Over " << N_EVENTS_TOTAL << " total events" << endl;

    cout << "Els:" << endl;
    for (size_t binIdx=0; binIdx<nEls.GetNbinsX(); binIdx++) {
        int n = nEls.GetBinContent(binIdx+1);
        cout << "  " << binIdx << " " << n << " " << float(n)/N_EVENTS_TOTAL*100 << "%" << endl;
    }
    cout << "Mus:" << endl;
    for (size_t binIdx=0; binIdx<nMus.GetNbinsX(); binIdx++) {
        int n = nMus.GetBinContent(binIdx+1);
        cout << "  " << binIdx << " " << n << " " << float(n)/N_EVENTS_TOTAL*100 << "%" << endl;
    }
    cout << "Taus:" << endl;
    for (size_t binIdx=0; binIdx<nTaus.GetNbinsX(); binIdx++) {
        int n = nTaus.GetBinContent(binIdx+1);
        cout << "  " << binIdx << " " << n << " " << float(n)/N_EVENTS_TOTAL*100 << "%" << endl;
    }
    return 0;
}
