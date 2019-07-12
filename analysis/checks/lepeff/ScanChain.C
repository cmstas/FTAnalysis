#pragma GCC diagnostic ignored "-Wsign-compare"

#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TH1.h"
#include "TChain.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "CORE/CMS3.h"
#include "CORE/ElectronSelections.h"
#include "CORE/SSSelections.h"
#include "CORE/Tools/JetCorrector.h"
#include "CORE/MuonSelections.h"
#include "CORE/TriggerSelections.h"
#include "CORE/Tools/datasetinfo/getDatasetInfo.h"

#include "../../misc/common_utils.h"

using namespace std;
using namespace tas;

bool STOP_REQUESTED = false;

int ScanChain(TChain *ch, TString outname="outputs/out.root", TString options=""){

    bool doElectrons = options.Contains("electrons");
    if (options.Contains("muons")) doElectrons = false;
    int year = 2016;
    if (options.Contains("2017")) year = 2017;
    if (options.Contains("2018")) year = 2018;

    if (year < 0) {
        std::cout << "Don't know what year this is, idiot." << std::endl;
        return 1;
    }

    std::string good_run_file;
    std::string jecEra; // XXX NOTE that you also have to change the run-dependent data JECs below too
    std::string jecEraMC;
    float lumi = -1.;
    if (year == 2016) {
        lumi = 35.9;
        gconf.year = year;
        gconf.cmssw_ver = 94;
        gconf.ea_version = 1;
        gconf.btag_disc_wp = 0.6324;
        gconf.WP_DEEPCSV_TIGHT  = 0.8958;
        gconf.WP_DEEPCSV_MEDIUM = 0.6324;
        gconf.WP_DEEPCSV_LOOSE  = 0.2219;
        gconf.multiiso_el_minireliso = 0.12;
        gconf.multiiso_el_ptratio = 0.80;
        gconf.multiiso_el_ptrel = 7.2;
        gconf.multiiso_mu_minireliso = 0.16;
        gconf.multiiso_mu_ptratio = 0.76;
        gconf.multiiso_mu_ptrel = 7.2;
        jecEra = "Summer16_23Sep2016BCDV4";
        jecEraMC = "Summer16_23Sep2016V4";
        gconf.SS_innerlayers = 0;
    }
    if (year == 2017) {
        lumi = 41.5;
        gconf.year = year;
        gconf.ea_version = 4;
        gconf.cmssw_ver = 94;
        gconf.btag_disc_wp = 0.4941;
        gconf.WP_DEEPCSV_TIGHT  = 0.8001;
        gconf.WP_DEEPCSV_MEDIUM = 0.4941;
        gconf.WP_DEEPCSV_LOOSE  = 0.1522;
        gconf.multiiso_el_minireliso = 0.07;
        gconf.multiiso_el_ptratio = 0.78;
        gconf.multiiso_el_ptrel = 8.0;
        gconf.multiiso_mu_minireliso = 0.11;
        gconf.multiiso_mu_ptratio = 0.74;
        gconf.multiiso_mu_ptrel = 6.8;
        jecEra = "Fall17_17Nov2017B_V32";
        jecEraMC = "Fall17_17Nov2017_V32";
        gconf.SS_innerlayers = 0;
    }
    if (year == 2018) {
        lumi = 59.7;
        gconf.year = year;
        gconf.ea_version = 4;
        gconf.cmssw_ver = 102;
        gconf.btag_disc_wp = 0.4184;
        gconf.WP_DEEPCSV_TIGHT  = 0.7527;
        gconf.WP_DEEPCSV_MEDIUM = 0.4184;
        gconf.WP_DEEPCSV_LOOSE  = 0.1241;
        gconf.multiiso_el_minireliso = 0.07;
        gconf.multiiso_el_ptratio = 0.78;
        gconf.multiiso_el_ptrel = 8.0;
        gconf.multiiso_mu_minireliso = 0.11;
        gconf.multiiso_mu_ptratio = 0.74;
        gconf.multiiso_mu_ptrel = 6.8;
        jecEra = "Autumn18_RunA_V8";
        jecEraMC = "Autumn18_V8";
        gconf.SS_innerlayers = 0;
    }
    //JEC files -- 25 ns MC
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL1;
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL2L3;
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL1L2L3;
    jetcorr_filenames_25ns_MC_pfL1.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L3Absolute_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L3Absolute_AK4PFchs.txt");
    FactorizedJetCorrector *jetCorrAG = nullptr;
    FactorizedJetCorrector *jetCorrAG_L1 = nullptr;
    FactorizedJetCorrector *jetCorrAG_L2L3 = nullptr;
    jetCorrAG_L1 = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL1);
    jetCorrAG_L2L3 = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL2L3);
    jetCorrAG = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL1L2L3);
    // for closestJet() in CORE/IsolationTools.cc
    gconf.jet_corrector_L1 = jetCorrAG_L1;
    gconf.jet_corrector_L2L3 = jetCorrAG_L2L3;
    createAndInitMVA("CORE", true, true, 80);

    DatasetInfoFromFile df;
    df.loadFromFile("CORE/Tools/datasetinfo/scale1fbs.txt");
    float magnitude_scale1fb = -1.;

    signal(SIGINT, [](int){
            cout << "SIGINT Caught, stopping after current event" << endl;
            STOP_REQUESTED=true;
            });
    STOP_REQUESTED=false;


    bool require_z = true;
    // bool require_z = false;

    TH1D * h_cnt = new TH1D("h_cnt", "h_cnt", 1, 0, 1); h_cnt->Sumw2();
    TH1D * h_cnt_eff = new TH1D("h_cnt_eff", "h_cnt_eff", 1, 0, 1); h_cnt_eff->Sumw2();

    int el_nptbins = 6;
    int el_netabins = 5;
    float el_ptbins[7] = {15., 20., 25., 30., 40., 50., 200.};
    float el_etabins[6] = {0.0, 0.8, 1.442, 1.556, 2.0, 2.5};
    TH2D * h2d_el_pt_eta_all = new TH2D("h2d_el_pt_eta_all", "all electrons", el_nptbins,el_ptbins,el_netabins,el_etabins); h2d_el_pt_eta_all->Sumw2();
    TH2D * h2d_el_pt_eta_pass = new TH2D("h2d_el_pt_eta_pass", "RA5 tight electrons", el_nptbins,el_ptbins,el_netabins,el_etabins); h2d_el_pt_eta_pass->Sumw2();
    TH2D * h2d_el_pt_eta_pass_nosf = new TH2D("h2d_el_pt_eta_pass_nosf", "RA5 tight electrons", el_nptbins,el_ptbins,el_netabins,el_etabins); h2d_el_pt_eta_pass_nosf->Sumw2();

    int mu_nptbins = 7;
    int mu_netabins = 4;
    float mu_ptbins[8] = {10., 15, 20, 25, 30, 40, 50, 200};
    float mu_etabins[5] = {0.0, 0.9, 1.2, 2.1, 2.4};
    TH2D * h2d_mu_pt_eta_all = new TH2D("h2d_mu_pt_eta_all", "all muons", mu_nptbins,mu_ptbins,mu_netabins,mu_etabins); h2d_mu_pt_eta_all->Sumw2();
    TH2D * h2d_mu_pt_eta_pass = new TH2D("h2d_mu_pt_eta_pass", "RA5 tight muons", mu_nptbins,mu_ptbins,mu_netabins,mu_etabins); h2d_mu_pt_eta_pass->Sumw2();
    TH2D * h2d_mu_pt_eta_pass_nosf = new TH2D("h2d_mu_pt_eta_pass_nosf", "RA5 tight muons", mu_nptbins,mu_ptbins,mu_netabins,mu_etabins); h2d_mu_pt_eta_pass_nosf->Sumw2();


    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    while ( (currentFile = (TFile*)fileIter.Next()) ) { 

        if(STOP_REQUESTED) break;


        TString fname =  currentFile->GetTitle();

        TFile *file = new TFile(fname);
        TTree *tree = (TTree*)file->Get("Events");


        cms3.Init(tree);

        TString filename(currentFile->GetTitle());

        for( unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            if(STOP_REQUESTED) break;


            cms3.GetEntry(event);
            nEventsTotal++;

            CMS3::progress(nEventsTotal, nEventsChain);

            if (event == 0) {
                magnitude_scale1fb = df.getScale1fbFromFile(tas::evt_dataset()[0].Data(),tas::evt_CMS3tag()[0].Data());
            }

            // Counters, total and effective entries
            h_cnt->Fill(0.5);

            std::vector<std::pair<int,int> > genwdaughteridxs;
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
                if (!(abs(tas::genps_id()[daughter_indices.first]) <= 14 && abs(tas::genps_id()[daughter_indices.first]) >= 11)) continue;
                genwdaughteridxs.push_back(daughter_indices);
            }
            int nleptonicW = genwdaughteridxs.size();

            // if (event > 10000) break;

            float sgnMCweight = ((tas::genps_weight() > 0) - (tas::genps_weight() < 0));
            float scale1fb = sgnMCweight*magnitude_scale1fb;
            // float weight = lumi*scale1fb;
            float weight = sgnMCweight;
            h_cnt_eff->Fill(0.5,weight);

            for (int ipair = 0; ipair < genwdaughteridxs.size(); ipair++) {
                int idx1 = genwdaughteridxs[ipair].first;
                int idx2 = genwdaughteridxs[ipair].second;

                int idx = (((abs(idx1) == 12) || (abs(idx1) == 14)) ? idx2 : idx1);
                int id = tas::genps_id().at(idx);
                LorentzVector gen_p4  = tas::genps_p4().at(idx);

                bool isele = abs(id)==11;
                if (isele) {
                    if (gen_p4.pt() < 15) continue;
                    if (fabs(gen_p4.eta()) > 2.5) continue;
                    h2d_el_pt_eta_all->Fill(gen_p4.pt(), fabs(gen_p4.eta()), weight);
                    int best_ilep = -1;
                    float best_dR = 0.2;
                    for (int ilep = 0; ilep < els_p4().size(); ilep++) {
                        LorentzVector reco_p4 = els_p4().at(ilep);
                        if (reco_p4.pt() < 15) continue;
                        if (fabs(reco_p4.eta()) > 2.5) continue;
                        float dR = ROOT::Math::VectorUtil::DeltaR(reco_p4, gen_p4);
                        if (dR < best_dR) {
                            best_dR = dR;
                            best_ilep = ilep;
                        }
                    }
                    if (best_ilep >= 0) {
                        bool istight = isGoodLepton(11,best_ilep);
                        if (istight) {
                            LorentzVector reco_p4 = els_p4().at(best_ilep);
                            float lepsf = leptonScaleFactor(year, id, reco_p4.pt(), reco_p4.eta(), -1, SSANA);
                            h2d_el_pt_eta_pass->Fill(reco_p4.pt(), fabs(reco_p4.eta()), weight*lepsf);
                            h2d_el_pt_eta_pass_nosf->Fill(reco_p4.pt(), fabs(reco_p4.eta()), weight);
                        }
                    }
                } else {
                    if (gen_p4.pt() < 10) continue;
                    if (fabs(gen_p4.eta()) > 2.4) continue;
                    h2d_mu_pt_eta_all->Fill(gen_p4.pt(), fabs(gen_p4.eta()), weight);
                    int best_ilep = -1;
                    float best_dR = 0.2;
                    for (int ilep = 0; ilep < mus_p4().size(); ilep++) {
                        LorentzVector reco_p4 = mus_p4().at(ilep);
                        if (reco_p4.pt() < 10) continue;
                        if (fabs(reco_p4.eta()) > 2.4) continue;
                        float dR = ROOT::Math::VectorUtil::DeltaR(reco_p4, gen_p4);
                        if (dR < best_dR) {
                            best_dR = dR;
                            best_ilep = ilep;
                        }
                    }
                    if (best_ilep >= 0) {
                        bool istight = isGoodLepton(13,best_ilep);
                        if (istight) {
                            LorentzVector reco_p4 = mus_p4().at(best_ilep);
                            float lepsf = leptonScaleFactor(year, id, reco_p4.pt(), reco_p4.eta(), -1, SSANA);
                            h2d_mu_pt_eta_pass->Fill(reco_p4.pt(), fabs(reco_p4.eta()), weight*lepsf);
                            h2d_mu_pt_eta_pass_nosf->Fill(reco_p4.pt(), fabs(reco_p4.eta()), weight);
                        }
                    }
                }

            }



        }//event loop

        delete file;

    }//file loop

    TFile* fout = new TFile(outname,"RECREATE");
    h_cnt->Write();
    h_cnt_eff->Write();

    h2d_el_pt_eta_all->Write();
    h2d_el_pt_eta_pass->Write();
    h2d_el_pt_eta_pass_nosf->Write();
    h2d_mu_pt_eta_all->Write();
    h2d_mu_pt_eta_pass->Write();
    h2d_mu_pt_eta_pass_nosf->Write();

    TH2D *eff_el = (TH2D*) h2d_el_pt_eta_pass->Clone("eff_el");
    eff_el->Divide(h2d_el_pt_eta_pass,h2d_el_pt_eta_all,1,1,"B");
    eff_el->Write();

    TH2D *eff_mu = (TH2D*) h2d_mu_pt_eta_pass->Clone("eff_mu");
    eff_mu->Divide(h2d_mu_pt_eta_pass,h2d_mu_pt_eta_all,1,1,"B");
    eff_mu->Write();

    fout->Write();
    fout->Close();

    return 0;

}

