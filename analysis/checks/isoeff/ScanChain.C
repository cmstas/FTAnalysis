#pragma GCC diagnostic ignored "-Wsign-compare"

#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TH1.h"
#include "TChain.h"
#include "Math/VectorUtil.h"

#include "CORE/CMS3.h"
#include "CORE/SSSelections.h"
#include "CORE/ElectronSelections.h"
#include "CORE/MuonSelections.h"
#include "CORE/Tools/JetCorrector.h"

using namespace std;
using namespace tas;

int ScanChain(TChain *ch){

    createAndInitMVA("./CORE", true, false, 80); // Moriond
    std::string jecEraMC = "Summer16_23Sep2016V3";
    std::vector<std::string> filenames;
    filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L1FastJet_AK4PFchs.txt");
    filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L2Relative_AK4PFchs.txt");
    filenames.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L3Absolute_AK4PFchs.txt");
    FactorizedJetCorrector *jetCorr = makeJetCorrector(filenames);


    TH1F * h_num_vs_pt = new TH1F("h_num_vs_pt", "h_num_vs_pt", 50, 0, 300);
    TH1F * h_den_vs_pt = new TH1F("h_den_vs_pt", "h_den_vs_pt", 50, 0, 300);
    TH1F * h_num_vs_nj = new TH1F("h_num_vs_nj", "h_num_vs_nj", 8, -0.5, 8.5);
    TH1F * h_den_vs_nj = new TH1F("h_den_vs_nj", "h_den_vs_nj", 8, -0.5, 8.5);

    TH1F * h_num_vs_pt2 = new TH1F("h_num_vs_pt2", "h_num_vs_pt2", 50, 0, 300);
    TH1F * h_den_vs_pt2 = new TH1F("h_den_vs_pt2", "h_den_vs_pt2", 50, 0, 300);
    TH1F * h_num_vs_nj2 = new TH1F("h_num_vs_nj2", "h_num_vs_nj2", 8, -0.5, 8.5);
    TH1F * h_den_vs_nj2 = new TH1F("h_den_vs_nj2", "h_den_vs_nj2", 8, -0.5, 8.5);
    
    TH1F * h_num_vs_pt3 = new TH1F("h_num_vs_pt3", "h_num_vs_pt3", 50, 0, 300);
    TH1F * h_den_vs_pt3 = new TH1F("h_den_vs_pt3", "h_den_vs_pt3", 50, 0, 300);
    TH1F * h_num_vs_nj3 = new TH1F("h_num_vs_nj3", "h_num_vs_nj3", 8, -0.5, 8.5);
    TH1F * h_den_vs_nj3 = new TH1F("h_den_vs_nj3", "h_den_vs_nj3", 8, -0.5, 8.5);

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    while ( (currentFile = (TFile*)fileIter.Next()) ) { 

        // Get File Content
        TFile *file = new TFile(currentFile->GetTitle());
        TString filename = currentFile->GetTitle();
        TTree *tree = (TTree*)file->Get("Events");
        cms3.Init(tree);

        for( unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            cms3.GetEntry(event);
            nEventsTotal++;

            CMS3::progress(nEventsTotal, nEventsChain);


            int njets = -1;

            // Find 4 pairs of daughters from W decays
            std::vector<std::pair<int,int> > genwdaughteridxs;
            std::vector<bool> genwleptonic;
            std::vector<bool> genwtau;
            for (unsigned int igen = 0; igen < tas::genps_p4().size(); igen++){
                int id = tas::genps_id()[igen];
                int mid = tas::genps_id_mother()[igen];
                if (abs(id) != 24) continue;
                if (abs(mid) != 6) continue;
                std::pair<int,int> daughter_indices = {-1,-1};
                for (unsigned int idau = 0; idau < tas::genps_p4().size(); idau++){
                    int midx = tas::genps_idx_mother()[idau];
                    if (midx != igen) continue;
                    // Fill first, then second, then break once both are in, since we can have duplicates :(
                    if (daughter_indices.first == -1) daughter_indices.first = idau;
                    else if (daughter_indices.second == -1) daughter_indices.second = idau;
                    else break;
                }
                if (daughter_indices.first == -1 && daughter_indices.second == -1) continue;
                genwdaughteridxs.push_back(daughter_indices);
                genwleptonic.push_back(abs(tas::genps_id()[daughter_indices.first]) <= 16 && abs(tas::genps_id()[daughter_indices.first]) >= 11);
            }
            int nlep = std::accumulate(genwleptonic.begin(),genwleptonic.end(),0);
            if (nlep < 2) continue;

            for (int ihyp = 0; ihyp < (int)(hyp_ll_id().size()); ihyp++) {
                int lep1_id = hyp_ll_id().at(ihyp);
                int lep2_id = hyp_lt_id().at(ihyp);
                int lep1_idx = hyp_ll_index().at(ihyp);
                int lep2_idx = hyp_lt_index().at(ihyp);
                float lep1_pt = hyp_ll_p4().at(ihyp).Pt();
                float lep2_pt = hyp_lt_p4().at(ihyp).Pt();
                float lep1_eta = hyp_ll_p4().at(ihyp).eta();
                float lep2_eta = hyp_lt_p4().at(ihyp).eta();
                if (lep1_pt <= 25) continue;
                if (lep2_pt <= 20) continue;
                if (abs(lep1_eta) >= (abs(lep1_id) == 11 ? 2.5 : 2.4)) continue;
                if (abs(lep2_eta) >= (abs(lep2_id) == 11 ? 2.5 : 2.4)) continue;
                if (lep1_id * lep2_id < 0) continue; // skip if not SS
                bool lep1_passes_idnoiso = isGoodLeptonNoIso(lep1_id, lep1_idx);
                bool lep2_passes_idnoiso = isGoodLeptonNoIso(lep2_id, lep2_idx);
                bool lep1_passes_id = isGoodLepton(lep1_id, lep1_idx);
                bool lep2_passes_id = isGoodLepton(lep2_id, lep2_idx);
                if (!(lep1_passes_id || lep1_passes_idnoiso || lep2_passes_idnoiso || lep2_passes_id)) continue;

                if (njets < 0) {
                    auto jet_results = SSJetsCalculator(jetCorr, 1);
                    njets = jet_results.first.size();
                }

                if (njets > 8) njets = 8;


                if (lep1_passes_idnoiso) {
                    if (lep1_passes_id) {
                        h_num_vs_pt->Fill(lep1_pt);
                        h_num_vs_nj->Fill(njets);
                        if (nlep == 2) {
                            h_num_vs_pt2->Fill(lep1_pt);
                            h_num_vs_nj2->Fill(njets);
                        } else {
                            h_num_vs_pt3->Fill(lep1_pt);
                            h_num_vs_nj3->Fill(njets);
                        }
                    }
                    h_den_vs_pt->Fill(lep1_pt);
                    h_den_vs_nj->Fill(njets);
                    if (nlep == 2) {
                        h_den_vs_pt2->Fill(lep1_pt);
                        h_den_vs_nj2->Fill(njets);
                    } else {
                        h_den_vs_pt3->Fill(lep1_pt);
                        h_den_vs_nj3->Fill(njets);
                    }
                }

                if (lep2_passes_idnoiso) {
                    if (lep2_passes_id) {
                        h_num_vs_pt->Fill(lep2_pt);
                        h_num_vs_nj->Fill(njets);
                        if (nlep == 2) {
                            h_num_vs_pt2->Fill(lep2_pt);
                            h_num_vs_nj2->Fill(njets);
                        } else {
                            h_num_vs_pt3->Fill(lep2_pt);
                            h_num_vs_nj3->Fill(njets);
                        }
                    }
                    h_den_vs_pt->Fill(lep2_pt);
                    h_den_vs_nj->Fill(njets);
                    if (nlep == 2) {
                        h_den_vs_pt2->Fill(lep2_pt);
                        h_den_vs_nj2->Fill(njets);
                    } else {
                        h_den_vs_pt3->Fill(lep2_pt);
                        h_den_vs_nj3->Fill(njets);
                    }
                }



            }

        }//event loop

        delete file;
    }//file loop


    TFile *fout = new TFile("histos.root", "RECREATE");
    h_num_vs_pt->Write();
    h_den_vs_pt->Write();
    h_num_vs_nj->Write();
    h_den_vs_nj->Write();
    h_num_vs_pt2->Write();
    h_den_vs_pt2->Write();
    h_num_vs_nj2->Write();
    h_den_vs_nj2->Write();
    h_num_vs_pt3->Write();
    h_den_vs_pt3->Write();
    h_num_vs_nj3->Write();
    h_den_vs_nj3->Write();
    fout->Close();

    return 0;

}

