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
#include "../../common/CORE/Tools/utils.h"
#include "../misc/common_utils.h"
#include "../misc/signal_regions.h"

using namespace std;
// using namespace tas;

float lumiAG = getLumi();

void flip(TChain *ch) {

  int nBinsX = 6; 
  int nBinsY = 3; 
  float xbins[] = { 15, 40, 60, 80, 100, 200, 300 }; 
  float ybins[] = { 0, 0.8, 1.479, 2.5 }; 
  TH2D *numer = new TH2D("numer" , "numer" , nBinsX, xbins, nBinsY, ybins);
  TH2D *denom = new TH2D("denom" , "denom" , nBinsX, xbins, nBinsY, ybins);
  numer->Sumw2();
  denom->Sumw2();

  //Set up chains
  TChain *chain = new TChain("t");

    std::cout << "Working on " << ch->GetTitle() << std::endl;

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

            if (!ss::fired_trigger()) continue;
            if (ss::hyp_class() != 3 && ss::hyp_class() != 4) continue;

            SSAG::progress(nEventsTotal, nEventsChain);

            //Calculate weight
            float weight = ss::is_real_data() ? 1 : ss::scale1fb()*lumiAG;

            if (!ss::is_real_data()) {
                weight *= getTruePUw(ss::trueNumInt()[0]);
                // weight *= eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
                // weight *= ss::weight_btagsf();
            }

            if (!ss::lep1_passes_id() || !ss::lep2_passes_id()) continue;

            //If they make it this far, they are denominator events
            if (abs(ss::lep1_id()) == 11) denom->Fill(min(ss::lep1_p4().pt(), float(299.)), fabs(ss::lep1_p4().eta()), weight); 
            if (abs(ss::lep2_id()) == 11) denom->Fill(min(ss::lep2_p4().pt(), float(299.)), fabs(ss::lep2_p4().eta()), weight); 

            //Now require exactly one charge flip
            bool isCF1 = 0;
            bool isCF2 = 0;
            if (abs(ss::lep1_id()) == 11 && abs(ss::lep1_mc_id()) == 11 && ss::lep1_id() != ss::lep1_mc_id()) isCF1 = 1;
            if (abs(ss::lep2_id()) == 11 && abs(ss::lep2_mc_id()) == 11 && ss::lep2_id() != ss::lep2_mc_id()) isCF2 = 1;
            if (!isCF1 && !isCF2) continue;
            if ( isCF1 &&  isCF2) continue;

            //The one that flipped is in the numerator
            if (isCF1) numer->Fill(ss::lep1_p4().pt(), fabs(ss::lep1_p4().eta()), weight); 
            if (isCF2) numer->Fill(ss::lep2_p4().pt(), fabs(ss::lep2_p4().eta()), weight); 

        }//event loop

        delete file;
    }//file loop


    TFile* f1 = new TFile(Form("outputs/histos_%s.root",ch->GetTitle()),"RECREATE");

    TH2D* ratio = (TH2D*)(numer->Clone("ratio"));
    ratio->Divide(numer, denom, 1, 1, "b"); 

    numer->Write();
    denom->Write();
    ratio->Write();

    f1->Close();


}

