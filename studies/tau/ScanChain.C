#pragma GCC diagnostic ignored "-Wsign-compare"
#include "Software/dataMCplotMaker/dataMCplotMaker.h"

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

using namespace std;
using namespace tas;

int ScanChain(TChain *ch){

    // TH1F * h_met = new TH1F("met", "met", 50, 0, 300);

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

            if (event > 1000) break;

            CMS3::progress(nEventsTotal, nEventsChain);

            // Find 4 pairs of daughters from W decays
            std::vector<std::pair<int,int> > genwdaughteridxs;
            std::vector<bool> genwleptonic;
            std::vector<bool> genwtau;
            std::vector<int> tauidxs;
            // Loop over the vector of gen particles
            for (unsigned int igen = 0; igen < tas::genps_p4().size(); igen++){
                // PDG id
                int id = tas::genps_id()[igen];
                // mother particle PDG ID
                int mid = tas::genps_id_mother()[igen];
                if (abs(id) != 24) continue; // skip if not W
                if (abs(mid) != 6) continue; // skip if not from top
                std::pair<int,int> daughter_indices = {-1,-1};
                // Loop through genparticles again to look for daughters (i.e., which particles have a mother which is igen)
                for (unsigned int idau = 0; idau < tas::genps_p4().size(); idau++){
                    int midx = tas::genps_idx_mother()[idau];
                    if (abs(tas::genps_id()[idau]) == 15 && tas::genps_status()[idau] == 23) continue; // status 23 taus
                    if (midx != igen) continue;
                    // Fill first, then second, then break once both are in, since we can have duplicate particles with different statuses
                    if (daughter_indices.first == -1) daughter_indices.first = idau;
                    else if (daughter_indices.second == -1) daughter_indices.second = idau;
                    else break;
                }
                if (daughter_indices.first == -1 && daughter_indices.second == -1) continue;
                genwdaughteridxs.push_back(daughter_indices);
                genwleptonic.push_back(abs(tas::genps_id()[daughter_indices.first]) <= 16 && abs(tas::genps_id()[daughter_indices.first]) >= 11);
                bool istau = abs(tas::genps_id()[daughter_indices.first]) <= 16 && abs(tas::genps_id()[daughter_indices.first]) >= 15;
                genwtau.push_back(istau);
                if (istau) {
                    if (abs(tas::genps_id()[daughter_indices.first]) == 15) tauidxs.push_back(daughter_indices.first);
                    else  tauidxs.push_back(daughter_indices.second);
                }
            }

            vector<int> hadronicgentauidxs;
            vector<int> hadronicpftauidxs;
            for (auto tauidx : tauidxs) {
                vector<int> taudaughteridxs;
                for (unsigned int igen = 0; igen < tas::genps_p4().size(); igen++){
                    int midx = tas::genps_idx_mother()[igen];
                    int id = tas::genps_id()[igen];
                    if (midx == tauidx) {
                        taudaughteridxs.push_back(id);
                    }
                }
                bool foundemu = false;
                for (auto id : taudaughteridxs) {
                    if (abs(id) == 13 || abs(id) == 11) foundemu = true;
                }
                if (!foundemu) hadronicgentauidxs.push_back(tauidx);
            }

            // At this point, size of hadronic gen tau idxs vector matches the 
            // distribution of hadronic tops from the python script
            if (hadronicgentauidxs.size()  < 1) continue;

            for (auto genidx : hadronicgentauidxs) {
                int matchidx = -1;
                int mindR = 999.;
                for (unsigned int ipf = 0; ipf < tas::taus_pf_p4().size(); ipf++){
                    auto pftau = tas::taus_pf_p4()[ipf];
                    auto gentau = tas::genps_p4()[genidx];
                    float dR = ROOT::Math::VectorUtil::DeltaR(pftau,gentau);
                    if (dR > 0.5) continue;
                    if (dR < mindR) {
                        matchidx = ipf;
                        mindR = dR;
                    }
                }
                hadronicpftauidxs.push_back(matchidx);
            }

            // std::cout <<  " hadronicgentauidxs.size(): " << hadronicgentauidxs.size() <<  std::endl;
            // for (int i = 0; i < hadronicgentauidxs.size(); i++) {
            //     int genidx = hadronicgentauidxs[i];
            //     int pfidx = hadronicpftauidxs[i];
            //     if (pfidx < 0) continue;
            //     float genpt = tas::genps_p4()[genidx].pt();
            //     float pfpt = tas::taus_pf_p4()[pfidx].pt();
            //     std::cout <<  " genpt: " << genpt <<  " pfpt: " << pfpt <<  std::endl;
            // }
            // // for (auto genidx : hadronicgentauidxs) {
            // // }
            // // std::cout << std::endl;
            // // for (auto pfidx : hadronicpftauidxs) {
            // //     if (pfidx < 0)  {
            // //         std::cout << "       ";
            // //         continue;
            // //     }
            // // }
            // // std::cout << std::endl;

            



        }//event loop

        delete file;
    }//file loop

    // TString comt = " --outOfFrame --lumi 1.0 --type Simulation --darkColorLines --legendCounts --legendRight -0.05  --outputName plots/";
    // std::string com = comt.Data();
    // TH1F * empty = new TH1F("","",1,0,1);

    // dataMCplotMaker(empty,{h_met} ,{"t#bar{t}"},"MET","",com+"h_met.pdf --isLinear");

    return 0;

}

