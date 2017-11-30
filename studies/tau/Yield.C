#pragma GCC diagnostic ignored "-Wsign-compare"

#include <iostream>
#include <fstream>

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


int ScanChain(TChain *ch, TFile* dest_file, bool include_tau){
    using namespace std;
    using namespace tas;

    createAndInitMVA("CORE", true, false, 80);
    // instantiate and load dataset information
    DatasetInfoFromFile df;
    df.loadFromFile("CORE/Tools/datasetinfo/scale1fbs.txt");

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    float sr1 = 0;
    float sr2 = 0;
    float sr3 = 0;
    float sr4 = 0;
    float sr5 = 0;
    float sr6 = 0;
    float sr7 = 0;
    float sr8 = 0;


    while ( (currentFile = (TFile*)fileIter.Next()) ) { 

        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        cms3.Init(tree);

        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            cms3.GetEntry(event);
            nEventsTotal++;

            /* if (event > 100) break; */
            CMS3::progress(nEventsTotal, nEventsChain);


            if(tas::evt_pfmet_raw() < 50) continue;  // TODO: Verify this is correct branch

            // Calculate the monte-carlo event weight
            float sgnMCweight = ((genps_weight() > 0) - (genps_weight() < 0));
            float scale1fb = sgnMCweight*df.getScale1fbFromFile(evt_dataset()[0].Data(),evt_CMS3tag()[0].Data());
            float moriond_int_lumi = 35.9;
            float weight = moriond_int_lumi*scale1fb;  // 

            typedef vector<RecoLepton> RecoLeptons;
            // Get all leptons in event that pass appropriate selection.
            RecoLeptons all_leps;
            for(auto& lep : RecoLepton::els()) all_leps.push_back(lep);
            for(auto& lep : RecoLepton::mus()) all_leps.push_back(lep);
            if (include_tau) {
                for(auto& lep : RecoLepton::taus()) all_leps.push_back(lep);
            }

            sort(all_leps.begin(), all_leps.end(),
                 [](const RecoLepton& lep1, const RecoLepton& lep2){
                     return lep1.p4.pt() > lep2.p4.pt();
                 });

            RecoLeptons pos_leps;
            RecoLeptons neg_leps;
            for(auto& lep : all_leps){
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
            if (pos_leps.size() >= 2 &&
                pos_leps[0].p4.pt() > 25 && pos_leps[1].p4.pt() > 20) {
                pos_ss_pair = true;
                for (const RecoLepton& lep : neg_leps) {
                    if(zVeto(lep, pos_leps[0]) or zVeto(lep, pos_leps[1])) {
                        zVeto_pos = true;
                        break;
                    }
                }
            }

            // Case negative SS pair passing pt reqs
            if (neg_leps.size() >= 2 &&
                neg_leps[0].p4.pt() > 25 && neg_leps[1].p4.pt() > 20) {
                neg_ss_pair = true;
                for (const RecoLepton& lep : pos_leps) {
                    if(zVeto(lep, neg_leps[0]) or zVeto(lep, neg_leps[1])) {
                        zVeto_neg = true;
                        break;
                    }
                }
            }

            if (zVeto_pos or zVeto_neg) continue;


            bool is_ss = all_leps.size() == 2 and (pos_ss_pair or neg_ss_pair);
            bool is_multilep = all_leps.size() > 2 and (pos_ss_pair or neg_ss_pair);
            int nJet = 0;
            int nBJet = 0;

            int neg_charge_count = 0;
            int pos_charge_count = 0;
            for(auto& lep : all_leps) {
                if (lep.charge > 0) pos_charge_count++;
                else neg_charge_count--;
            }

            if ((neg_charge_count == 2 && pos_charge_count == 0) ||
                (pos_charge_count == 2 && neg_charge_count == 0)) {
                is_ss = true;
            }

            float Ht = 0;
            int nJetsAll = tas::pfjets_p4().size();
            for(size_t idx=0; idx<nJetsAll; idx++) {
                float jetPt = tas::pfjets_p4()[idx].pt();
                float jetEta = tas::pfjets_p4()[idx].eta();
                float disc = tas::getbtagvalue("deepFlavourJetTags:probb",idx) +
                             tas::getbtagvalue("deepFlavourJetTags:probbb",idx);
                if (jetPt > 40 && abs(jetEta) < 2.4) {
                    nJet++;
                    Ht += jetPt;
                }
                if (jetPt > 25 && disc>0.6324) nBJet++;
            }

            if (Ht <= 300) continue;

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

    cout << "Done!" << endl;
    return 0;
}
