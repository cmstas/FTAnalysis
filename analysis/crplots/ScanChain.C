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

struct HistCol {
    map<string, TH1D> ee;
    map<string, TH1D> em;
    map<string, TH1D> mm;
    map<string, TH1D> in;

    HistCol(vector<string> regions, const string& name, int nbins, float low, float high) {
        for (string region : regions) {
            string base_name = region + "_" + name;
            string base_title = region + " " + name;
            ee.emplace(region, TH1D((base_name + "_ee").c_str(), (base_title + " ee").c_str(), nbins, low, high));
            em.emplace(region, TH1D((base_name + "_em").c_str(), (base_title + " em").c_str(), nbins, low, high));
            mm.emplace(region, TH1D((base_name + "_mm").c_str(), (base_title + " mm").c_str(), nbins, low, high));
            in.emplace(region, TH1D((base_name + "_in").c_str(), (base_title + " in").c_str(), nbins, low, high));
        }
    }

    void Fill(const string& region, int id1, int id2, float val, float weight) {
        in[region].Fill(val, weight);
        if (abs(id1) == 11 and abs(id2) == 11) {
            ee[region].Fill(val, weight);
        } else if (abs(id1) == 13 and abs(id2) == 13) {
            mm[region].Fill(val, weight);
        } else if ((abs(id1) == 11 and abs(id2) == 13) or
                   (abs(id1) == 13 and abs(id2) == 11)) {
            em[region].Fill(val, weight);
        } else {
            cout << "These ids are garbage: (" << id1 << ", " << id2 << ")\n";
        }
    }

    void Write() {
        for (auto p : ee) p.second.Write();
        for (auto p : em) p.second.Write();
        for (auto p : mm) p.second.Write();
        for (auto p : in) p.second.Write();
    }
};

int ScanChain(TChain *ch, TString options=""){

    /* bool doAllHT = options.Contains("doAllHT"); */
    /* bool useEraBLowHTTriggers = options.Contains("useEraBLowHTTriggers"); */

    cout << "Working on " << ch->GetTitle() << endl;

    vector<string> regions = {"os", "tl",                     // OS tight-tight and SS tight-loose
                              "crz", "crw",                   // CRZ, CRW
                              "bdt_nb", "bdt_ht", "bdt_met",  // Baseline w/ inverted nbtags/Ht/MET selection
                              "isr"};
    HistCol h_met    (regions, "met"   , 15, 0   , 600 );
    HistCol h_ht     (regions, "ht"    , 16, 0   , 1600);
    HistCol h_mll    (regions, "mll"   , 25, 0   , 300 );
    HistCol h_mtmin  (regions, "mtmin" , 25, 0   , 300 );
    HistCol h_njets  (regions, "njets" , 8 , 0   , 8   );
    HistCol h_nbtags (regions, "nbtags", 5 , 0   , 5   );
    HistCol h_pt1    (regions, "pt1"   , 25, 0   , 300 );
    HistCol h_pt2    (regions, "pt2"   , 25, 0   , 300 );
    HistCol h_eta1   (regions, "eta1"  , 25, -3.2, 3.2 );
    HistCol h_eta2   (regions, "eta2"  , 25, -3.2, 3.2 );
    HistCol h_pte    (regions, "pte"   , 25, 0   , 300 );
    HistCol h_ptm    (regions, "ptm"   , 25, 0   , 300 );
    HistCol h_type   (regions, "type"  , 3 , 0   , 3   );
    HistCol h_nvtx   (regions, "nvtx"  , 70, 0   , 70  );

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

            if (abs(ss::lep1_id()) == 11 && !ss::lep1_el_exp_innerlayers()) continue;
            if (abs(ss::lep2_id()) == 11 && !ss::lep2_el_exp_innerlayers()) continue;

            bool pass_trig = ss::fired_trigger();
            /* if (useEraBLowHTTriggers && ss::is_real_data()) { */
            /*     if (ss::run() <= 299329 && ss::run() >= 297046) {  // Era B */
            /*         if ( ss::hyp_type()==0 && ((ss::triggers() & 1<<3)==(1<<3) || (ss::triggers() & 1<<4)==(1<<4)) ) pass_trig = true; */
            /*         else if ( ss::hyp_type()==3 && (ss::triggers() & 1<<6)==(1<<6) ) pass_trig = true; */
            /*         else if ( (ss::hyp_type()==1 || ss::hyp_type()==2) && ((ss::triggers() & 1<<1)==(1<<1) || (ss::triggers() & 1<<2)==(1<<2)) ) pass_trig = true; */
            /*         else {} */
            /*     } */
            /* } */
            if (!pass_trig) continue;

            SSAG::progress(nEventsTotal, nEventsChain);

            // Reject duplicates
            if (ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)) continue;
            }

            //Calculate weight
            float weight = ss::is_real_data() ? 1 : ss::scale1fb()*lumiAG;

            if (!ss::is_real_data()) {
                // weight *= getTruePUw(ss::trueNumInt()[0]);
                // weight *= eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
                // weight *= ss::weight_btagsf();
            }
            int nleps = (ss::lep3_passes_id() and ss::lep3_p4().Pt()>20) ? 3 : 2;
            int SR = signalRegionTest(ss::njets(),  ss::nbtags(), ss::met(), ss::ht(), 0,
                                      ss::lep1_id(),  ss::lep2_id(), ss::lep1_coneCorrPt(), ss::lep2_coneCorrPt(),
                                      nleps, ss::hyp_class() == 6);
            bool SS = (ss::lep1_id()>0) == (ss::lep2_id()>0);

            // Fill histograms
            //
            auto fill_region = [&](const string& region) {
                int lep1id = ss::lep1_id();
                int lep2id = ss::lep2_id();

                auto do_fill = [region, lep1id, lep2id, weight](HistCol& h, float val) {
                    h.Fill(region, lep1id, lep2id, val, weight);
                };
                do_fill(h_met,    ss::met());
                do_fill(h_njets,  ss::njets());
                do_fill(h_ht,     ss::ht());
                do_fill(h_nbtags, ss::nbtags());
                do_fill(h_mll,    ss::dilep_p4().M());
                do_fill(h_mtmin,  ss::mtmin());
                do_fill(h_pt1,    ss::lep1_coneCorrPt());
                do_fill(h_pt2,    ss::lep2_coneCorrPt());
                abs(lep1id) == 11 ? do_fill(h_pte, ss::lep1_coneCorrPt()) : do_fill(h_ptm, ss::lep1_coneCorrPt());
                abs(lep2id) == 11 ? do_fill(h_pte, ss::lep2_coneCorrPt()) : do_fill(h_ptm, ss::lep2_coneCorrPt());
                do_fill(h_eta1,   ss::lep1_p4().eta());
                do_fill(h_eta2,   ss::lep2_p4().eta());
                int type = ss::hyp_type();
                do_fill(h_type,   type>1 ? type-1 : type);
                do_fill(h_nvtx,   ss::nGoodVertices());
            };

            /* Hyp Class
             * 1: SS, loose-loose
             * 2: SS, tight-loose
             * 3: SS, tight-tight
             * 4: OS, tight-tight
             * 5: SS, inSituFR
             * 6: SS, tight-tight and fails Z-veto
             */

            // if all 3 charges are the same, throw the event away
            if (SS and nleps > 2) {
              int q1 = 2*(ss::lep1_id() > 0) - 1;
              int q3 = 2*(ss::lep3_id() > 0) - 1;
              if (q3==q1) {
                   cout << "Skipping +++/---" << endl;
                  continue;
              }
            }

            if (nleps >= 3 and SR == 1) fill_region("crz");
            if (ss::hyp_class() == 3 and SR == 2) fill_region("crw");

            if (ss::lep1_coneCorrPt() > 25. and ss::lep2_coneCorrPt() > 20. and ss::njets() >= 2 and
                    ss::met() > 50. and ss::ht() > 400) {
                if (ss::hyp_class() == 4 || ss::hyp_class() == 6) fill_region("os");
                if (ss::hyp_class() == 2)                         fill_region("tl");
            }

            if (ss::hyp_class() == 3 and ss::lep1_coneCorrPt() >= 25. and ss::lep2_coneCorrPt() >= 20. and ss::njets() >= 2) {
                if (ss::nbtags() < 2  and ss::ht() > 400 and ss::met() > 50) fill_region("bdt_nb");   // invert Nb
                if (ss::nbtags() >= 2 and ss::ht() < 400 and ss::met() > 50) fill_region("bdt_ht");   // invert Ht
                if (ss::nbtags() >= 2 and ss::ht() > 400 and ss::met() < 50) fill_region("bdt_met");  // invert MET
            }

            if (ss::lep1_coneCorrPt() >= 25. and ss::lep2_coneCorrPt() >= 20.  and ss::njets() > 2 and
                    ss::hyp_class() != 6 and nleps == 2 and ss::nbtags() == 2 and ss::njets() >= 2) {
                if (ss::nbtags() < 2  and ss::ht() > 400 and ss::met() > 50) fill_region("isr");
            }

        }//event loop

        delete file;
    }//file loop


    TFile f1(Form("outputs%s/histos_%s.root", options.Data(), ch->GetTitle()), "RECREATE");

    h_met.Write();
    h_ht.Write();
    h_mll.Write();
    h_mtmin.Write();
    h_njets.Write();
    h_nbtags.Write();
    h_pt1.Write();
    h_pt2.Write();
    h_eta1.Write();
    h_eta2.Write();
    h_pte.Write();
    h_ptm.Write();
    h_type.Write();
    h_nvtx.Write();

    f1.Close();

    return 0;

}

