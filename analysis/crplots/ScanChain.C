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

#include "../misc/class_files/v8.02/SS.h"
#include "../../common/CORE/Tools/dorky/dorky.h"
#include "../../common/CORE/Tools/utils.h"
#include "../misc/common_utils.h"
#include "../misc/signal_regions.h"

using namespace std;
// using namespace tas;

float lumiAG = getLumi();

int ScanChain(TChain *ch){

    std::cout << "Working on " << ch->GetTitle() << std::endl;

    TH1F *h_os_met = new TH1F("os_met"      , "met"    , 50 , 0    , 300);
    TH1D *h_os_ht     = new TH1D("os_ht"     , "ht"     , 50 , 0    , 1000);
    TH1D *h_os_mll    = new TH1D("os_mll"    , "mll"    , 25 , 0    , 300);
    TH1D *h_os_mtmin  = new TH1D("os_mtmin"  , "mtmin"  , 25 , 0    , 300);
    TH1D *h_os_njets  = new TH1D("os_njets"  , "njets"  , 8  , 0    , 8  );
    TH1D *h_os_nbtags = new TH1D("os_nbtags" , "nbtags" , 5  , 0    , 5  );
    TH1D *h_os_pt1     = new TH1D("os_pt1"   , "pt1"    , 25 , 0    , 300);
    TH1D *h_os_pt2     = new TH1D("os_pt2"   , "pt2"    , 25 , 0    , 300);
    TH1D *h_os_eta1    = new TH1D("os_eta1"  , "eta1"   , 25 , -3.2 , 3.2);
    TH1D *h_os_eta2    = new TH1D("os_eta2"  , "eta2"   , 25 , -3.2 , 3.2);
    TH1D *h_os_pte     = new TH1D("os_pte"   , "pte"    , 25 , 0    , 300);
    TH1D *h_os_ptm     = new TH1D("os_ptm"   , "ptm"    , 25 , 0    , 300);
    TH1D *h_os_type     = new TH1D("os_type" , "type"   , 4  , 0    , 4);

    TH1F *h_tl_met = new TH1F("tl_met"      , "met"    , 50 , 0    , 300);
    TH1D *h_tl_ht     = new TH1D("tl_ht"     , "ht"     , 50 , 0    , 1000);
    TH1D *h_tl_mll    = new TH1D("tl_mll"    , "mll"    , 25 , 0    , 300);
    TH1D *h_tl_mtmin  = new TH1D("tl_mtmin"  , "mtmin"  , 25 , 0    , 300);
    TH1D *h_tl_njets  = new TH1D("tl_njets"  , "njets"  , 8  , 0    , 8  );
    TH1D *h_tl_nbtags = new TH1D("tl_nbtags" , "nbtags" , 5  , 0    , 5  );
    TH1D *h_tl_pt1     = new TH1D("tl_pt1"   , "pt1"    , 25 , 0    , 300);
    TH1D *h_tl_pt2     = new TH1D("tl_pt2"   , "pt2"    , 25 , 0    , 300);
    TH1D *h_tl_eta1    = new TH1D("tl_eta1"  , "eta1"   , 25 , -3.2 , 3.2);
    TH1D *h_tl_eta2    = new TH1D("tl_eta2"  , "eta2"   , 25 , -3.2 , 3.2);
    TH1D *h_tl_pte     = new TH1D("tl_pte"   , "pte"    , 25 , 0    , 300);
    TH1D *h_tl_ptm     = new TH1D("tl_ptm"   , "ptm"    , 25 , 0    , 300);
    TH1D *h_tl_type     = new TH1D("tl_type" , "type"   , 4  , 0    , 4);

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    while ( (currentFile = (TFile*)fileIter.Next()) ) { 

        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        TString filename(currentFile->GetTitle());

        for( unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            samesign.GetEntry(event);
            nEventsTotal++;

            // if (event > 10000) break;

            // Baseline selections
            if (ss::njets() < 2) continue;
            if (ss::lep1_coneCorrPt() < 25) continue;
            if (ss::lep2_coneCorrPt() < 20) continue;
            if (ss::met() < 50) continue;
            if (ss::ht() < 400) continue;
            if (!ss::fired_trigger()) continue;

            SSAG::progress(nEventsTotal, nEventsChain);

            // Reject duplicates 
            if (ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)) continue; 
            }

            //Calculate weight
            float weight = ss::is_real_data() ? 1 : ss::scale1fb()*lumiAG;

            if (ss::hyp_class() != 4 && ss::hyp_class() != 6 && ss::hyp_class() != 2) continue;

            if (!ss::is_real_data()) {
                weight *= getTruePUw(ss::trueNumInt()[0]);
                // weight *= eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
                // weight *= ss::weight_btagsf();
            }

            // Fill histograms

            if (ss::hyp_class() == 4 || ss::hyp_class() == 6) {
                h_os_met->Fill(ss::met(), weight);
                h_os_njets->Fill(ss::njets(), weight);
                h_os_ht->Fill(ss::ht(), weight);  
                h_os_nbtags->Fill(ss::nbtags(), weight);  
                h_os_mll->Fill(ss::dilep_p4().M() , weight);
                h_os_mtmin->Fill(ss::mtmin()      , weight);
                h_os_pt1->Fill(ss::lep1_coneCorrPt()  , weight);
                h_os_pt2->Fill(ss::lep2_coneCorrPt()  , weight);
                abs(ss::lep1_id()) == 11 ? h_os_pte->Fill(ss::lep1_coneCorrPt()  , weight) : h_os_ptm->Fill(ss::lep1_coneCorrPt()  , weight);
                abs(ss::lep2_id()) == 11 ? h_os_pte->Fill(ss::lep2_coneCorrPt()  , weight) : h_os_ptm->Fill(ss::lep2_coneCorrPt()  , weight);
                h_os_eta1->Fill(ss::lep1_p4().eta(), weight);
                h_os_eta2->Fill(ss::lep2_p4().eta(), weight);
                h_os_type->Fill(ss::hyp_type(), weight);
            }

            if (ss::hyp_class() == 2) {
                h_tl_met->Fill(ss::met(), weight);
                h_tl_njets->Fill(ss::njets(), weight);
                h_tl_ht->Fill(ss::ht(), weight);  
                h_tl_nbtags->Fill(ss::nbtags(), weight);  
                h_tl_mll->Fill(ss::dilep_p4().M() , weight);
                h_tl_mtmin->Fill(ss::mtmin()      , weight);
                h_tl_pt1->Fill(ss::lep1_coneCorrPt()  , weight);
                h_tl_pt2->Fill(ss::lep2_coneCorrPt()  , weight);
                abs(ss::lep1_id()) == 11 ? h_tl_pte->Fill(ss::lep1_coneCorrPt()  , weight) : h_tl_ptm->Fill(ss::lep1_coneCorrPt()  , weight);
                abs(ss::lep2_id()) == 11 ? h_tl_pte->Fill(ss::lep2_coneCorrPt()  , weight) : h_tl_ptm->Fill(ss::lep2_coneCorrPt()  , weight);
                h_tl_eta1->Fill(ss::lep1_p4().eta(), weight);
                h_tl_eta2->Fill(ss::lep2_p4().eta(), weight);
                h_tl_type->Fill(ss::hyp_type(), weight);
            }

        }//event loop

        delete file;
    }//file loop


    TFile* f1 = new TFile(Form("outputs/histos_%s.root",ch->GetTitle()),"RECREATE");

    h_os_met->Write();
    h_os_ht->Write();
    h_os_mll->Write();
    h_os_mtmin->Write();
    h_os_njets->Write();
    h_os_nbtags->Write();
    h_os_pt1->Write();
    h_os_pt2->Write();
    h_os_eta1->Write();
    h_os_eta2->Write();
    h_os_pte->Write();
    h_os_ptm->Write();
    h_os_type->Write();

    h_tl_met->Write();
    h_tl_ht->Write();
    h_tl_mll->Write();
    h_tl_mtmin->Write();
    h_tl_njets->Write();
    h_tl_nbtags->Write();
    h_tl_pt1->Write();
    h_tl_pt2->Write();
    h_tl_eta1->Write();
    h_tl_eta2->Write();
    h_tl_pte->Write();
    h_tl_ptm->Write();
    h_tl_type->Write();

    f1->Close();

    return 0;

}

