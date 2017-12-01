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

int ScanChain(TChain *ch, TFile* dest_file, bool include_tau){
    using namespace std;
    using namespace tas;

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

    float sr1 = 0;
    float sr2 = 0;
    float sr3 = 0;
    float sr4 = 0;
    float sr5 = 0;
    float sr6 = 0;
    float sr7 = 0;
    float sr8 = 0;


    while ((currentFile = (TFile*)fileIter.Next())) {
        if(STOP_REQUESTED) break;

        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        cms3.Init(tree);

        for (unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {
            if(STOP_REQUESTED) break;

            cms3.GetEntry(event);
            N_EVENTS_TOTAL++;

            /* if (event > 100) break; */
            CMS3::progress(N_EVENTS_TOTAL, nEventsChain);

            // Get all leptons in event that pass appropriate selection.
            RecoLeptons all_leps;
            RecoLeptons els = RecoLepton::els();
            RecoLeptons mus = RecoLepton::mus();
            RecoLeptons taus = RecoLepton::taus();
            nEls.Fill(els.size());
            nMus.Fill(mus.size());
            nTaus.Fill(taus.size());
            for(auto& lep : els) all_leps.push_back(lep);
            for(auto& lep : mus) all_leps.push_back(lep);
            if (include_tau) {
                for(auto& lep : taus) all_leps.push_back(lep);
            }

            sort(all_leps.begin(), all_leps.end(),
                 [](const RecoLepton& lep1, const RecoLepton& lep2){
                     return lep1.p4.pt() > lep2.p4.pt();
                 });

            RecoLeptons pos_leps;
            RecoLeptons neg_leps;
            for (auto& lep : all_leps) {
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
                if (all_leps.size() > 2 and all_leps[2].p4.pt() > 20) {
                    is_multilep = true;
                } else {
                    is_ss = true;
                }
            }

            float Ht = 0;
            // TODO: Try to use some jet/lepton cross-cleaning a la : https://github.com/cmstas/CORE/blob/master/SSSelections.cc#L302
            int nJet = 0;
            int nBJet = 0;
            int nJetsAll = tas::pfjets_p4().size();
            for(size_t idx=0; idx<nJetsAll; idx++) {
                float jetPt = tas::pfjets_p4()[idx].pt();
                float jetEta = tas::pfjets_p4()[idx].eta();
                float disc = tas::getbtagvalue("deepFlavourJetTags:probb",idx) +
                             tas::getbtagvalue("deepFlavourJetTags:probbb",idx);
                if (jetPt > 40 and abs(jetEta) < 2.4) {
                    nJet++;
                    Ht += jetPt;
                }
                if (jetPt > 25 and disc>0.6324) nBJet++;
            }

            /* if (all_leps.size() > 0) event_summary(els, mus, taus, nJet, nBJet); */

            if (Ht <= 300) continue;
            if (tas::evt_pfmet() < 50) continue;

            // Calculate the monte-carlo event weight
            float sgnMCweight = ((genps_weight() > 0) - (genps_weight() < 0));
            float scale1fb = sgnMCweight*df.getScale1fbFromFile(evt_dataset()[0].Data(),evt_CMS3tag()[0].Data());
            float moriond_int_lumi = 35.9;
            float weight = moriond_int_lumi*scale1fb;  // 

            if (is_ss       and nBJet == 2 and nJet == 6)                sr1 += weight;
            if (is_ss       and nBJet == 2 and nJet == 7)                sr2 += weight;
            if (is_ss       and nBJet == 2 and nJet >= 8)                sr3 += weight;
            if (is_ss       and nBJet == 3 and (nJet == 5 or nJet == 6)) sr4 += weight;
            if (is_ss       and nBJet == 3 and nJet >= 7)                sr5 += weight;
            if (is_ss       and nBJet >= 4 and nJet >= 5)                sr6 += weight;
            if (is_multilep and nBJet == 2 and nJet >= 5)                sr7 += weight;
            if (is_multilep and nBJet >= 3 and nJet >= 4)                sr8 += weight;
        }  // event loop
        delete file;
    }  // file loop
    cout << "\nLoop Finished. Saving Results... " << flush;

    dest_file->cd();
    TH1F SRs("SRs", "SRs", 8, 0.5, 8.5);
    SRs.Fill(1, sr1);
    SRs.Fill(2, sr2);
    SRs.Fill(3, sr3);
    SRs.Fill(4, sr4);
    SRs.Fill(5, sr5);
    SRs.Fill(6, sr6);
    SRs.Fill(7, sr7);
    SRs.Fill(8, sr8);
    SRs.Write();

    nEls.Write();
    nMus.Write();
    nTaus.Write();
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
