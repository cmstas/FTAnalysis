#pragma GCC diagnostic ignored "-Wsign-compare"

#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TH1.h"
#include "TChain.h"
#include "Math/VectorUtil.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "CMS3.h"
#include "../../../misc/common_utils.h"
#include "../../../misc/tqdm.h"

using namespace std;
using namespace tas;

bool STOP_REQUESTED = false;

int ScanChain(TChain *ch, TString options="", TString outputdir="outputs"){

    signal(SIGINT, [](int){
            cout << "SIGINT Caught, stopping after current event" << endl;
            STOP_REQUESTED=true;
            });
    STOP_REQUESTED=false;

    bool quiet = options.Contains("quiet");
    bool onlyEl = options.Contains("onlyEl");
    bool onlyMu = options.Contains("onlyMu");
    bool doBoth = not(onlyEl ^ onlyMu);
    bool doPUweight = options.Contains("doPUweight");
    bool doLeptonSF = options.Contains("doLeptonSF");

    TString proc(ch->GetTitle());

    int year;
    float lumiAG = 0.;
    if (options.Contains("Data2016")) {
        lumiAG = getLumi(2016);
        // lumiAG = 35.9;
        year = 2016;
    } else if (options.Contains("Data2017")) {
        lumiAG = getLumi(2017);
        // lumiAG = 41.5;
        year = 2017;
    } else if (options.Contains("Data2018")) {
        lumiAG = getLumi(2018);
        // lumiAG = 58.8;
        year = 2018;
    } else {
        cout << "Need to specify year!\n";
        return -1;
    }

    if (!quiet) cout << "Working on " << proc << endl;


    TFile f1(Form("%s/histos_%s.root", outputdir.Data(), ch->GetTitle()), "RECREATE");
    TH1D* mll_HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30 = new TH1D("mll_HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30","mll_HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30", 60,60,120);
    TH1D* mll_HLT_Ele17_CaloIdM_TrackIdM_PFJet30 = new TH1D("mll_HLT_Ele17_CaloIdM_TrackIdM_PFJet30","mll_HLT_Ele17_CaloIdM_TrackIdM_PFJet30", 60,60,120);
    TH1D* mll_HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30 = new TH1D("mll_HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30","mll_HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30", 60,60,120);
    TH1D* mll_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 = new TH1D("mll_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30","mll_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30", 60,60,120);
    TH1D* mll_HLT_Ele23_CaloIdM_TrackIdM_PFJet30 = new TH1D("mll_HLT_Ele23_CaloIdM_TrackIdM_PFJet30","mll_HLT_Ele23_CaloIdM_TrackIdM_PFJet30", 60,60,120);
    TH1D* mll_HLT_Ele8_CaloIdM_TrackIdM_PFJet30 = new TH1D("mll_HLT_Ele8_CaloIdM_TrackIdM_PFJet30","mll_HLT_Ele8_CaloIdM_TrackIdM_PFJet30", 60,60,120);
    TH1D* mll_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 = new TH1D("mll_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30","mll_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30", 60,60,120);
    TH1D* mll_HLT_Mu17_TrkIsoVVL = new TH1D("mll_HLT_Mu17_TrkIsoVVL","mll_HLT_Mu17_TrkIsoVVL", 60,60,120);
    TH1D* mll_HLT_Mu17 = new TH1D("mll_HLT_Mu17","mll_HLT_Mu17", 60,60,120);
    TH1D* mll_HLT_Mu8_TrkIsoVVL = new TH1D("mll_HLT_Mu8_TrkIsoVVL","mll_HLT_Mu8_TrkIsoVVL", 60,60,120);
    TH1D* mll_HLT_Mu8 = new TH1D("mll_HLT_Mu8","mll_HLT_Mu8", 60,60,120);
    TH1D* mll_HLT_IsoMu27 = new TH1D("mll_HLT_IsoMu27","mll_HLT_IsoMu27", 60,60,120);

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    tqdm bar;
    // bar.set_theme_braille();
    // bar.set_theme_vertical();

    while ( (currentFile = (TFile*)fileIter.Next()) ) {
        if (STOP_REQUESTED) break;

        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("t");
        cms3.Init(tree);

        TString filename(currentFile->GetTitle());

        auto tokens = filename.Tokenize("/");
        auto basename = ((TObjString*)(tokens->At(tokens->GetEntries()-1)))->String().Data();
        bar.set_label(basename);

        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            if (STOP_REQUESTED) break;

            cms3.GetEntry(event);
            nEventsTotal++;
            
            if (!quiet) bar.progress(nEventsTotal, nEventsChain);

            // float pt = p4().pt();
            if (!passes_SS_tight_v6()) continue;
            float pt = coneCorrPt(); // for tight leptons, this is equivalent to p4().pt() but faster
            if (pt < 30.) continue;
            if (tag_pt() < 30.) continue;
            if (!passes_met_filters()) continue;

            float weight = 1;
            if (!evt_isRealData()) {
                weight *= scale1fb()*lumiAG;
                if (doPUweight) weight *= getTruePUw(year, trueNumInt());
                if (doLeptonSF) weight *= leptonScaleFactor(year, id(), pt, p4_eta(), 50.);
                if (doLeptonSF) weight *= leptonScaleFactor(year, id(), tag_pt(), tag_p4().eta(), 50.);
            }

            if ((doBoth or onlyEl) and (abs(id()) == 11)) {
                bool trigsafe_withoutiso = isTriggerSafenoIso();
                bool trigsafe_withiso = trigsafe_withoutiso && isTriggerSafe();
                float mll = dilep_mass();
                if (trigsafe_withiso) {
                    if (HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30() > 0) mll_HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30->Fill(mll, HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30()*weight);
                    if (HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30() > 0) mll_HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30->Fill(mll, HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30()*weight);
                    if (HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30() > 0) mll_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30->Fill(mll, HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30()*weight);
                    if (HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30() > 0) mll_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30->Fill(mll, HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30()*weight);
                }
                if (trigsafe_withoutiso) {
                    if (HLT_Ele17_CaloIdM_TrackIdM_PFJet30() > 0) mll_HLT_Ele17_CaloIdM_TrackIdM_PFJet30->Fill(mll, HLT_Ele17_CaloIdM_TrackIdM_PFJet30()*weight);
                    if (HLT_Ele23_CaloIdM_TrackIdM_PFJet30() > 0) mll_HLT_Ele23_CaloIdM_TrackIdM_PFJet30->Fill(mll, HLT_Ele23_CaloIdM_TrackIdM_PFJet30()*weight);
                    if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30() > 0) mll_HLT_Ele8_CaloIdM_TrackIdM_PFJet30->Fill(mll, HLT_Ele8_CaloIdM_TrackIdM_PFJet30()*weight);
                }
            }
            if ((doBoth or onlyMu) and (abs(id()) == 13)) {
                float mll = dilep_mass();
                if (HLT_Mu17_TrkIsoVVL() > 0) mll_HLT_Mu17_TrkIsoVVL->Fill(mll, HLT_Mu17_TrkIsoVVL()*weight);
                if (HLT_Mu17() > 0) mll_HLT_Mu17->Fill(mll, HLT_Mu17()*weight);
                if (HLT_Mu8_TrkIsoVVL() > 0) mll_HLT_Mu8_TrkIsoVVL->Fill(mll, HLT_Mu8_TrkIsoVVL()*weight);
                if (HLT_Mu8() > 0) mll_HLT_Mu8->Fill(mll, HLT_Mu8()*weight);
                if (HLT_IsoMu27() > 0) mll_HLT_IsoMu27->Fill(mll, HLT_IsoMu27()*weight);
            }

        }//event loop
        delete file;
    }//file loop

    f1.Write();
    f1.Close();
    if (!quiet) cout << "\n Done!" << endl;
    return 0;
}

