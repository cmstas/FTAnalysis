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
#include "TMVA/Reader.h"
#include "../../misc/class_files/v8.02/SS.h"
#include "../../misc/common_utils.h"
#include "../../misc/signal_regions.h"
#include "../../misc/tqdm.h"

using namespace std;

int ScanChain(TChain *ch, TString options="", TString outputdir="outputs/"){

    int tree_year = -1;
    int tree_stype = -1;
    float tree_weight = -1;
    bool tree_br = 0;
    int tree_SR = 0;
    float tree_nbtags = -1;
    float tree_njets = -1;
    float tree_met = -1;
    float tree_ptl2 = -1;
    float tree_nlb40 = -1;
    float tree_ntb40 = -1;
    float tree_nleps = -1;
    float tree_htb = -1;
    float tree_ml1j1 = -1;
    float tree_dphil1l2 = -1;
    float tree_maxmjoverpt = -1;
    float tree_detal1l2 = -1;
    float tree_q1 = -1;
    float tree_ptj1 = -1;
    float tree_ptj6 = -1;
    float tree_ptj7 = -1;
    float tree_ptj8 = -1;
    float tree_ptl1 = -1;
    float tree_ptl3 = -1;
    float tree_disc = -1;

    TMVA::Reader reader("Silent");
    reader.AddVariable("nbtags",      &tree_nbtags);
    reader.AddVariable("njets",       &tree_njets);
    reader.AddVariable("met",         &tree_met);
    reader.AddVariable("ptl2",        &tree_ptl2);
    reader.AddVariable("nlb40",       &tree_nlb40);
    reader.AddVariable("ntb40",       &tree_ntb40);
    reader.AddVariable("nleps",       &tree_nleps);
    reader.AddVariable("htb",         &tree_htb);
    reader.AddVariable("q1",          &tree_q1);
    reader.AddVariable("ptj1",        &tree_ptj1);
    reader.AddVariable("ptj6",        &tree_ptj6);
    reader.AddVariable("ptj7",        &tree_ptj7);
    reader.AddVariable("ml1j1",       &tree_ml1j1);
    reader.AddVariable("dphil1l2",    &tree_dphil1l2);
    reader.AddVariable("maxmjoverpt", &tree_maxmjoverpt);
    reader.AddVariable("ptl1",        &tree_ptl1);
    reader.AddVariable("detal1l2",    &tree_detal1l2);
    reader.AddVariable("ptj8",        &tree_ptj8);
    reader.AddVariable("ptl3",        &tree_ptl3);
    reader.AddSpectator("weight",     &tree_weight);
    reader.AddSpectator("ptl1",       &tree_ptl1);
    reader.AddSpectator("ptl2",       &tree_ptl2);
    reader.AddSpectator("SR",         &tree_SR);
    reader.BookMVA("BDT","../../yields/TMVAClassification_BDT_19vars.xml");

    TString proc(ch->GetTitle());

    if (proc == "tttt")  tree_stype = 0;
    if (proc == "ttw")   tree_stype = 1;
    if (proc == "ttz")   tree_stype = 2;
    if (proc == "tth")   tree_stype = 3;
    if (proc == "fakes") tree_stype = 4;
    if (proc == "flips") tree_stype = 5;
    if (proc == "xg")    tree_stype = 6;
    if (proc == "ttvv")  tree_stype = 7;
    if (proc == "rares") tree_stype = 8;

    TFile* out_file = new TFile(Form("%s/output_%s.root",outputdir.Data(),proc.Data()), "RECREATE");
    out_file->cd();
    TTree* out_tree = new TTree("t","fortraining");
    out_tree->Branch("year", &tree_year);
    out_tree->Branch("stype", &tree_stype);
    out_tree->Branch("weight", &tree_weight);
    out_tree->Branch("br", &tree_br);
    out_tree->Branch("SR", &tree_SR);
    out_tree->Branch("nbtags", &tree_nbtags);
    out_tree->Branch("njets", &tree_njets);
    out_tree->Branch("met", &tree_met);
    out_tree->Branch("ptl2", &tree_ptl2);
    out_tree->Branch("nlb40", &tree_nlb40);
    out_tree->Branch("ntb40", &tree_ntb40);
    out_tree->Branch("nleps", &tree_nleps);
    out_tree->Branch("htb", &tree_htb);
    out_tree->Branch("ml1j1", &tree_ml1j1);
    out_tree->Branch("dphil1l2", &tree_dphil1l2);
    out_tree->Branch("maxmjoverpt", &tree_maxmjoverpt);
    out_tree->Branch("detal1l2", &tree_detal1l2);
    out_tree->Branch("q1", &tree_q1);
    out_tree->Branch("ptj1", &tree_ptj1);
    out_tree->Branch("ptj6", &tree_ptj6);
    out_tree->Branch("ptj7", &tree_ptj7);
    out_tree->Branch("ptj8", &tree_ptj8);
    out_tree->Branch("ptl1", &tree_ptl1);
    out_tree->Branch("ptl3", &tree_ptl3);
    out_tree->Branch("disc", &tree_disc);

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    tqdm bar;
    // bar.set_theme_braille();
    while ( (currentFile = (TFile*)fileIter.Next()) ) { 

        // Get File Content
        TFile *file = new TFile(currentFile->GetTitle());


        TString filename = currentFile->GetTitle();
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        if (filename.Contains("2016")) tree_year = 2016;
        else if (filename.Contains("2017")) tree_year = 2017;
        else if (filename.Contains("2018")) tree_year = 2018;

        for( unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            samesign.GetEntry(event);
            nEventsTotal++;

            bar.progress(nEventsTotal, nEventsChain);


            // looser baseline
            if (ss::njets() < 2) continue;
            if (ss::nbtags() < 1) continue;
            if (ss::ht() < 250) continue;
            if (ss::met() < 30) continue;
            if (ss::lep1_coneCorrPt() < 15) continue;
            if (ss::lep2_coneCorrPt() < 15) continue;

            float weight = ss::is_real_data() ? 1.0 : 50.0*(ss::scale1fb());

            // Flips
            if (tree_stype == 5) {
                if (ss::hyp_class() != 4) continue;
                float ff = 0.;
                if (abs(ss::lep1_id()) == 11) {
                    float flr = flipRate(tree_year, ss::lep1_p4().pt(), ss::lep1_p4().eta());
                    ff += (flr/(1-flr));
                }
                if (abs(ss::lep2_id()) == 11) {
                    float flr = flipRate(tree_year, ss::lep2_p4().pt(), ss::lep2_p4().eta());
                    ff += (flr/(1-flr));
                }
                weight *= ff;
                if (weight == 0.0) continue; // just quit if there are no flips.
            } else {
                if (ss::hyp_class() != 3) continue;
            }

            // Truth fakes
            if (tree_stype == 4) {
                int nbadlegs = (ss::lep1_motherID() <= 0) + (ss::lep2_motherID() <= 0);
                int ngoodlegs = (ss::lep1_motherID() == 1) + (ss::lep2_motherID() == 1);
                // skip the event if it's truth matched to be prompt prompt.
                // We only want reco tight-tight events that are prompt-nonprompt (or nonprompt nonprompt)
                if (ngoodlegs == 2) continue;
            }

            bool br = passes_baseline(
                    ss::njets(),
                    ss::nbtags(),
                    ss::met(),
                    ss::ht(),
                    ss::lep1_id(),
                    ss::lep2_id(),
                    ss::lep1_coneCorrPt(),
                    ss::lep2_coneCorrPt()
                    );

            int SR = signalRegionTest(ss::njets(), ss::nbtags(), ss::met(), ss::ht(), ss::mtmin(), ss::lep1_id(), ss::lep2_id(), ss::lep1_coneCorrPt(), ss::lep2_coneCorrPt(), ss::lep3_coneCorrPt(), ss::bdt_nleps(), ss::hyp_class() == 6);


            tree_nbtags = ss::bdt_nbtags();
            tree_njets = ss::bdt_njets();
            tree_met = ss::bdt_met();
            tree_ptl2 = ss::bdt_ptl2();
            tree_nlb40 = ss::bdt_nlb40();
            tree_ntb40 = ss::bdt_ntb40();
            tree_nleps = ss::bdt_nleps();
            tree_htb = ss::bdt_htb();
            tree_ml1j1 = ss::bdt_ml1j1();
            tree_dphil1l2 = ss::bdt_dphil1l2();
            tree_maxmjoverpt = ss::bdt_maxmjoverpt();
            tree_detal1l2 = ss::bdt_detal1l2();
            tree_q1 = ss::bdt_q1();
            tree_ptj1 = ss::bdt_ptj1();
            tree_ptj6 = ss::bdt_ptj6();
            tree_ptj7 = ss::bdt_ptj7();
            tree_ptj8 = ss::bdt_ptj8();
            tree_ptl1 = ss::bdt_ptl1();
            tree_ptl3 = ss::bdt_ptl3();

            tree_weight = weight;
            tree_SR = SR;
            tree_br = br;

            tree_disc = reader.EvaluateMVA("BDT");

            out_tree->Fill();

        }//event loop

        delete file;
    }//file loop
    bar.finish();



    // TFile *fout = new TFile("histos.root", "RECREATE");
    // h_dr_vs_pt->Write();
    // h_mindr_vs_pt->Write();
    // fout->Close();

    // Write output tree
    out_file->cd();
    out_tree->Write();
    out_file->Close();

    return 0;

}

