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
#include "../../misc/class_files/v8.02/SS.h"

using namespace std;

int ScanChain(TChain *ch){

    TH1F * h_num_vs_pt = new TH1F("h_num_vs_pt", "h_num_vs_pt", 60, 0, 300); // real b && btagged
    TH1F * h_mis_vs_pt = new TH1F("h_mis_vs_pt", "h_mis_vs_pt", 60, 0, 300); // light && btagged
    TH1F * h_den_vs_pt = new TH1F("h_den_vs_pt", "h_den_vs_pt", 60, 0, 300); // denominator (real b)
    TH1F * h_dms_vs_pt = new TH1F("h_dms_vs_pt", "h_dms_vs_pt", 60, 0, 300); // denominator (mistag, i.e., light)

    TH1F * h_num_vs_dr = new TH1F("h_num_vs_dr", "h_num_vs_dr", 40, 0, 4);
    TH1F * h_mis_vs_dr = new TH1F("h_mis_vs_dr", "h_mis_vs_dr", 40, 0, 4);
    TH1F * h_den_vs_dr = new TH1F("h_den_vs_dr", "h_den_vs_dr", 40, 0, 4);
    TH1F * h_dms_vs_dr = new TH1F("h_dms_vs_dr", "h_dms_vs_dr", 40, 0, 4);

    TH1F * h_num_vs_nj = new TH1F("h_num_vs_nj", "h_num_vs_nj", 15, 0, 15);
    TH1F * h_mis_vs_nj = new TH1F("h_mis_vs_nj", "h_mis_vs_nj", 15, 0, 15);
    TH1F * h_den_vs_nj = new TH1F("h_den_vs_nj", "h_den_vs_nj", 15, 0, 15);
    TH1F * h_dms_vs_nj = new TH1F("h_dms_vs_nj", "h_dms_vs_nj", 15, 0, 15);

    TH2F * h_dr_vs_pt = new TH2F("h_dr_vs_pt", "h_dr_vs_pt", 60, 0, 300, 12, 0, 6);

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    while ( (currentFile = (TFile*)fileIter.Next()) ) { 

        // Get File Content
        TFile *file = new TFile(currentFile->GetTitle());
        TString filename = currentFile->GetTitle();
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        for( unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            samesign.GetEntry(event);
            nEventsTotal++;

            SSAG::progress(nEventsTotal, nEventsChain);


            for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
                float pt = ss::jets()[ijet].pt()*ss::jets_undoJEC()[ijet]*ss::jets_JEC()[ijet];
                float eta = ss::jets()[ijet].eta();
                int flavor = abs(ss::jets_flavor()[ijet]);
                bool is_tagged = ss::jets_disc()[ijet] > 0.6324;
                bool is_b = flavor == 5;
                bool is_light = flavor != 5; // && flavor != 4;
                float mindR = 999.;
                for (unsigned int ibtag = 0; ibtag < ss::btags().size(); ibtag++) {
                    float dR = ROOT::Math::VectorUtil::DeltaR(ss::jets()[ijet],ss::btags()[ibtag]);
                    if (dR < 0.01) continue;
                    if (dR < mindR) mindR = dR;
                }
                if (is_b) {
                    if (is_tagged) {
                        h_num_vs_nj->Fill(ss::jets().size());
                        h_num_vs_dr->Fill(mindR);
                        h_num_vs_pt->Fill(pt);
                    }
                    h_den_vs_nj->Fill(ss::jets().size());
                    h_den_vs_dr->Fill(mindR);
                    h_den_vs_pt->Fill(pt);
                }
                if (is_light) {
                    if (is_tagged) {
                        h_mis_vs_nj->Fill(ss::jets().size());
                        h_mis_vs_dr->Fill(mindR);
                        h_mis_vs_pt->Fill(pt);
                    }
                    h_dms_vs_nj->Fill(ss::jets().size());
                    h_dms_vs_dr->Fill(mindR);
                    h_dms_vs_pt->Fill(pt);
                }
            }

            if (ss::hyp_class()==3 && ss::nbtags()>=2 && ss::njets()>=2 && ss::met()>50 && ss::ht()>300) {
                float maxdr1 = 0.;
                float maxdr2 = 0.;
                for (unsigned int ibtag = 0; ibtag < ss::btags().size(); ibtag++) {
                    float dr1 = ROOT::Math::VectorUtil::DeltaR(ss::lep1_p4(),ss::btags()[ibtag]);
                    float dr2 = ROOT::Math::VectorUtil::DeltaR(ss::lep2_p4(),ss::btags()[ibtag]);
                    if (dr1 > maxdr1) maxdr1 = dr1;
                    if (dr2 > maxdr2) maxdr2 = dr2;
                }
                h_dr_vs_pt->Fill(ss::lep1_p4().pt(), maxdr1);
                h_dr_vs_pt->Fill(ss::lep2_p4().pt(), maxdr2);
            }

        }//event loop

        delete file;
    }//file loop


    TFile *fout = new TFile("histos.root", "RECREATE");
    h_num_vs_pt->Write();
    h_mis_vs_pt->Write();
    h_den_vs_pt->Write();
    h_dms_vs_pt->Write();
    h_num_vs_dr->Write();
    h_mis_vs_dr->Write();
    h_den_vs_dr->Write();
    h_dms_vs_dr->Write();
    h_num_vs_nj->Write();
    h_mis_vs_nj->Write();
    h_den_vs_nj->Write();
    h_dms_vs_nj->Write();
    h_dr_vs_pt->Write();
    fout->Close();

    return 0;

}

