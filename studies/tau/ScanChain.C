#pragma GCC diagnostic ignored "-Wsign-compare"

#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include "../../common/CORE/CMS3.h"

#include "../../common/CORE/SSSelections.h"
#include "../../common/CORE/ElectronSelections.h"
#include "../../common/CORE/MuonSelections.h"
#include "../../common/CORE/Tools/datasetinfo/getDatasetInfo.h"

#include "tau_utils.hpp"
#include "hist_utils.hpp"


int ScanChain(TChain *ch, TFile* dest_file){
    using namespace std;
    using namespace tas;

    KinematicMeasurement pf_kin("pf",   50, 600, 50, 4, 50);
    KinematicMeasurement gen_kin("gen", 50, 600, 50, 4, 50);


    EfficiencyMeasurement accept_eff("accept_eff", 50, 0, 300);
    EfficiencyMeasurement reco_eff("reco_eff", 50, 0, 300);
    EfficiencyMeasurement id_eff("id_eff", 50, 0, 300);
    EfficiencyMeasurement ult_eff("ult_eff", 50, 0, 300);

    EfficiencyMeasurement id_eff_old("id_eff_old", 50, 0, 300);
    EfficiencyMeasurement ult_eff_old("ult_eff_old", 50, 0, 300);

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);


    while ( (currentFile = (TFile*)fileIter.Next()) ) { 

        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        cms3.Init(tree);

        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            cms3.GetEntry(event);
            nEventsTotal++;

            if (event > 1000) break;
            CMS3::progress(nEventsTotal, nEventsChain);

            // Get all hadronically decaying (1- and 3-prong) Generator-Level
            // Taus in the event
            std::vector<GenTau> gentaus = GenTau::build();
            for(auto& gt : gentaus) gen_kin.fill(gt);

            std::vector<GenTau> gentaus_pass_accept;
            for(auto& gt : gentaus){
                if(gt.passes_acceptance()) gentaus_pass_accept.push_back(gt);
            }

            // Get all Particle-Flow Taus in event
            std::vector<PFTau> pftaus = PFTau::build();
            for(auto& pft : pftaus) pf_kin.fill(pft);

            // Get all Particle-Flow Taus in event that pass:
            //   - acceptance
            std::vector<PFTau> pftaus_pass_accept;
            for(auto& pft : pftaus){
                if(pft.passes_acceptance())
                    pftaus_pass_accept.push_back(pft);
            }

            // Get all Particle-Flow Taus in event that pass:
            //   - acceptance
            //   - identification
            std::vector<PFTau> pftaus_pass_id;
            std::vector<PFTau> pftaus_pass_id_old;
            for(auto& pft : pftaus){
                if(!pft.passes_acceptance()) continue;
                if(pft.passes_id("byTightIsolationMVArun2v1DBdR03oldDMwLT"))
                    pftaus_pass_id.push_back(pft);
                if(pft.passes_id("byTightIsolationMVArun2v1DBoldDMwLT"))
                    pftaus_pass_id_old.push_back(pft);
            }

            // acceptance efficiency (accept)
            // Proportion of GenTaus that pass acceptance requirement
            for(auto& gentau : gentaus){
                accept_eff.fill(gentau.p4.pt(), gentau.passes_acceptance());
            }

            // reco efficiency (reco wrt accept)
            // Proportion of Gentaus that pass acceptance and match a PFTau over
            // the number that only pass acceptance
            match(gentaus_pass_accept, pftaus_pass_accept, 0.5);
            for(auto& gentau : gentaus_pass_accept){
                reco_eff.fill(gentau.p4.pt(), gentau.matched_pftau != nullptr);
            }

            // ID efficiency (id wrt reco)
            // Proportion of PFTaus with match that pass acceptance and ID over only acceptance
            for(auto& pftau : pftaus_pass_accept){
                if(pftau.matched_gentau == nullptr) continue;
                id_eff.fill(pftau.p4.pt(), pftau.passes_id("byTightIsolationMVArun2v1DBdR03oldDMwLT"));
            }

            // "ultimate" efficiency (reco + id wrt accept)
            match(gentaus_pass_accept, pftaus_pass_id, 0.5);
            for(const GenTau& gentau : gentaus_pass_accept){
                ult_eff.fill(gentau.p4.pt(), gentau.matched_pftau != nullptr);
            }

            // ID efficiency (id wrt reco)
            // Proportion of PFTaus with match that pass acceptance and ID over only acceptance
            for(auto& pftau : pftaus_pass_accept){
                if(pftau.matched_gentau == nullptr) continue;
                id_eff_old.fill(pftau.p4.pt(), pftau.passes_id("byTightIsolationMVArun2v1DBoldDMwLT"));
            }

            // "ultimate" efficiency (reco + id wrt accept)
            match(gentaus_pass_accept, pftaus_pass_id_old, 0.5);
            for(const GenTau& gentau : gentaus_pass_accept){
                ult_eff_old.fill(gentau.p4.pt(), gentau.matched_pftau != nullptr);
            }

        }  // event loop
        delete file;
    }  // file loop
    cout << "\nLoop Finished. Saving Results... " << flush;

    dest_file->cd();
    accept_eff.save();
    reco_eff.save();
    id_eff.save();
    ult_eff.save();
    id_eff_old.save();
    ult_eff_old.save();

    pf_kin.save();
    gen_kin.save();

    cout << "Done!" << endl;
    return 0;
}
