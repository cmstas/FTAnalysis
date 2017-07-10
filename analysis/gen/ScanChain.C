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
#include "../../common/CORE/CMS3.h"

using namespace std;
using namespace tas;

int ScanChain(TChain *ch){

    TH1F * h_met = new TH1F("met", "met", 50, 0, 300);

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    float weight_tot = 0.;
    float weight_ss = 0.;
    float weight_pt = 0.;
    float weight_nj = 0.;
    float weight_nb = 0.;
    float weight_ht = 0.;
    float weight_met = 0.;
    while ( (currentFile = (TFile*)fileIter.Next()) ) { 

        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        cms3.Init(tree);

        TString filename(currentFile->GetTitle());

        for( unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            cms3.GetEntry(event);
            nEventsTotal++;

            CMS3::progress(nEventsTotal, nEventsChain);

            h_met->Fill(evt_pfmet());

            // if (event > 20) break;

            float sgnMCweight = ((tas::genps_weight() > 0) - (tas::genps_weight() < 0));
            float scale1fb = sgnMCweight*1.0152e-05;
            float weight = 35.9*scale1fb;

            // Get Nleps and see if we have SS
            int nleps = 0;
            int chargeproduct = 1; // multiply lepton ids into this. if 2 leps and positive, then SS
            float lepptsum = 0.;
            bool hasSS = false;
            for (unsigned int igen = 0; igen < genps_p4().size(); igen++){
                int id = genps_id()[igen];
                int mid = genps_id_mother()[igen];
                if (abs(id) != 11 && abs(id) != 13) continue;
                if (abs(mid) != 24) continue;
                if (!(genps_isPromptFinalState()[igen])) continue;
                auto lep = genps_p4()[igen];
                if (lep.pt() < 20) continue;
                if (abs(lep.eta()) > 2.4) continue;
                nleps++;
                chargeproduct *= id;
                lepptsum += lep.pt();
            }
            if (nleps == 2 && (chargeproduct > 0)) hasSS = true;
            if (nleps >= 3) hasSS = true;
            if (!hasSS) continue; // skip event
            weight_ss += weight;

            // We counted leptons with pt>20. To ensure that the highest pt
            // lepton has pt>25, make sure sum(pt)>{45,65,85} if {2,3,4} lep event
            bool leptonptgood = lepptsum > (45 + 20*(nleps-2));
            if (!leptonptgood) continue; // skip event
            weight_pt += weight;


            // Get HT and njets
            float ht = 0.;
            int njets = 0;
            for (unsigned int ijet = 0; ijet < genjets_p4NoMuNoNu().size(); ijet++){
                auto jet = genjets_p4NoMuNoNu()[ijet];
                if (jet.pt() < 40) continue;
                if (abs(jet.eta()) > 2.4) continue;
                ht += jet.pt();
                njets++;
            }
            if (njets < 4) continue; // skip event
            weight_nj += weight;

            // Find bquarks
            int nbtags = 0;
            std::vector<int> genbidxs;
            for (unsigned int igen = 0; igen < genps_p4().size(); igen++){
                int stat = genps_status()[igen];
                int id = genps_id()[igen];
                int mid = genps_id_mother()[igen];
                if (abs(id) != 5) continue;
                if (abs(mid) != 6) continue;
                if (stat != 23) continue;
                auto bquark = genps_p4()[igen];
                bool foundone = false;
                for (unsigned int ijet = 0; ijet < genjets_p4NoMuNoNu().size(); ijet++){
                    auto jet = genjets_p4NoMuNoNu()[ijet];
                    if (jet.pt() < 25) continue;
                    if (abs(jet.eta()) > 2.4) continue;
                    float dR = ROOT::Math::VectorUtil::DeltaR(jet,bquark);
                    if (dR > 0.4) continue;
                    foundone = true;
                    break;
                }
                if (!foundone) continue; // FIXME
                genbidxs.push_back(igen);
                nbtags++;
            }
            if (nbtags < 2) continue; // skip event
            weight_nb += weight;

            if (ht < 300) continue; // skip event
            weight_ht += weight;

            // Get MET
            LorentzVector neutrinop4(0,0,0,0);
            int nneutrinos = 0;
            float met = 0;
            for (unsigned int igen = 0; igen < genps_p4().size(); igen++){
                int id = genps_id()[igen];
                int mid = genps_id_mother()[igen];
                if (abs(id) != 12 && abs(id) != 14 && abs(id) != 16) continue;
                if (abs(mid) != 24) continue;
                if (!(genps_isPromptFinalState()[igen])) continue;
                neutrinop4 += genps_p4()[igen];
                nneutrinos++;
            }
            met = neutrinop4.pt();
            if (met < 50) continue; // skip event
            weight_met += weight;

            


            int SR = -1;
            if (nleps == 2) {
                if (nbtags == 2) {
                    if (njets <= 5) SR = 2;
                    if (njets == 6) SR = 3;
                    if (njets == 7) SR = 4;
                    if (njets >= 8) SR = 5;
                } else if (nbtags == 3) {
                    if (njets == 5) SR = 6;
                    if (njets == 6) SR = 6;
                    if (njets >= 7) SR = 7;
                } else if (nbtags >= 4) {
                    if (njets >= 5) SR = 8;
                }
            } else {
                if (nbtags == 2) {
                    if (njets >= 5) SR = 9;
                } else if (nbtags >= 3) {
                    if (njets >= 4) SR = 10;
                }
            }
            // SR -= 2; // SRCR to SR number // NOTE, now we want to include the CRs
            if (SR < 1) continue; // skip event


            // std::cout <<  " hasSS: " << hasSS <<  " nleps: " << nleps <<  " nbtags: " << nbtags <<  " met: " << met <<  " njets: " << njets <<  " ht: " << ht <<  " leptonptgood: " << leptonptgood <<  std::endl;

            weight_tot += weight;


        }//event loop

        delete file;
    }//file loop

    std::cout <<  " weight_ss:  " << weight_ss <<  std::endl;
    std::cout <<  " weight_pt:  " << weight_pt <<  std::endl;
    std::cout <<  " weight_nj:  " << weight_nj <<  std::endl;
    std::cout <<  " weight_nb:  " << weight_nb <<  std::endl;
    std::cout <<  " weight_ht:  " << weight_ht <<  std::endl;
    std::cout <<  " weight_met: " << weight_met <<  std::endl;

    std::cout <<  " nEventsSR: " << weight_tot <<  std::endl;
    std::cout <<  " nEventsTotal4top: " << 35.9*9.2 <<  std::endl;

    TFile *fout = new TFile("histos.root", "RECREATE");
    h_met->Write();
    fout->Close();

    return 0;

}

