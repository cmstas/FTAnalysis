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
// float lumiAG = 36.3;

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

    bool doFakes = options.Contains("doFakes");

    // Clear already-seen list
    duplicate_removal::clear_list();

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

        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            samesign.GetEntry(event);
            nEventsTotal++;

            // if (event > 10000) break;

            if (ss::lep1_el_exp_innerlayers() > 0) continue;
            if (ss::lep2_el_exp_innerlayers() > 0) continue;

            auto pass_trig = []() {
                if (ss::is_real_data()) {
                    if (ss::run() <= 299329 && ss::run() >= 297046) {  // Era B
                        /* hyp_type
                         * 0: mu mu
                         * 1/2: mu e/e mu
                         * 3: e e
                         */
                        int hyp = ss::hyp_type();
                        unsigned int triggers = ss::triggers();
                        /* Normally, the below triggers are only used if ht<400, but for Era B, use them
                         * always. (see `fired_trigger` in helper_babymaker.cc)
                         */
                        if (hyp==0             && ((triggers & 1<<3) || (triggers & 1<<4))) return true;
                        if (hyp==3             &&  (triggers & 1<<6))                       return true;
                        if ((hyp==1 || hyp==2) && ((triggers & 1<<1) || (triggers & 1<<2))) return true;
                        return ss::fired_trigger();
                    } else {
                        return ss::fired_trigger();
                    }
                } else {  // MC
                    return true;
                }
            };

            if (!pass_trig()) continue;

            SSAG::progress(nEventsTotal, nEventsChain);

            // Reject duplicates
            if (ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)) continue;
            }

            // Save a bunch of event info for quick reference later
            float lep1pt = ss::lep1_coneCorrPt();
            float lep2pt = ss::lep2_coneCorrPt();
            float lep3pt = ss::lep3_coneCorrPt();
            int lep1id = ss::lep1_id();
            int lep2id = ss::lep2_id();
            int lep3id = ss::lep3_id();
            int lep1good = ss::lep1_passes_id();
            int lep2good = ss::lep2_passes_id();
            int lep3good = ss::lep3_passes_id();
            int nleps = (ss::lep3_passes_id() and lep3pt > 20.) ? 3 : 2;
            int njets = ss::njets();
            int nbtags = ss::nbtags();
            int ht = ss::ht();
            int met = ss::met();
            /* hyp_class
             * 1: SS, loose-loose
             * 2: SS, tight-loose (or loose-tight)
             * 3: SS, tight-tight
             * 4: OS, tight-tight
             * 5: SS, inSituFR
             * 6: SS, tight-tight and fails Z-veto (lies! hyp_class==6 != lep1good and lep2good)
             */
            int hyp_class = ss::hyp_class();

            //Calculate weight
            float weight = ss::is_real_data() ? 1 : ss::scale1fb()*lumiAG;

            if (!ss::is_real_data()) {
                // weight *= getTruePUw(ss::trueNumInt()[0]);
                // weight *= eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
                // weight *= ss::weight_btagsf();
            }

            bool class6Fake = false;
            if (doFakes) {
                if (hyp_class == 6) {
                    if ((ss::lep3_fo() && !ss::lep3_tight()) && lep1good && lep2good) {  // lep3 fake
                        float fr = fakeRate(lep3id, ss::lep3_p4().pt(), ss::lep3_p4().eta(), ht);
                        class6Fake = true;
                        weight *= fr / (1-fr);
                    }
                    if ((ss::lep2_fo() && !ss::lep2_tight()) && lep1good && lep3good) {  // lep2 fake
                        float fr = fakeRate(lep2id, lep2pt, ss::lep2_p4().eta(), ht);
                        class6Fake = true;
                        weight *= fr / (1-fr);
                    }
                    if ((ss::lep1_fo() && !ss::lep1_tight()) && lep2good && lep3good) {  // lep1 fake
                        float fr = fakeRate(lep1id, lep1pt, ss::lep1_p4().eta(), ht);
                        class6Fake = true;
                        weight *= fr / (1-fr);
                    }
                    if (!class6Fake) {
                        continue; // No fakes!
                    }
                } else if (hyp_class == 1 or hyp_class == 2) {
                    if (ss::lep1_passes_id()==0) {
                        float fr = fakeRate(lep1id, lep1pt, ss::lep1_p4().eta(), ht);
                        weight *= fr/(1.-fr);
                    }
                    if (ss::lep2_passes_id()==0) {
                        float fr = fakeRate(lep2id, lep2pt, ss::lep2_p4().eta(), ht);
                        weight *= fr/(1.-fr);
                    }
                    // subtract double FO (why is this?)
                    if (hyp_class == 1) weight *= -1.;
                    hyp_class = 3; // we've faked a SS Tight-Tight with a SS LL or SS TL
                                   // Basically just update this so it gets put in the SR
                } else {
                    continue; // Not a fakeing hyp_class
                }
            }

            auto fill_region = [&](const string& region) {
                // Fill all observables for a region
                /* cout << "region: " << region << endl; */

                auto do_fill = [region, lep1id, lep2id, weight](HistCol& h, float val) {
                    h.Fill(region, lep1id, lep2id, val, weight);
                };
                do_fill(h_met,    met);
                do_fill(h_njets,  njets);
                do_fill(h_ht,     ht);
                do_fill(h_nbtags, nbtags);
                do_fill(h_mll,    ss::dilep_p4().M());
                do_fill(h_mtmin,  ss::mtmin());
                do_fill(h_pt1,    lep1pt);
                do_fill(h_pt2,    lep2pt);
                abs(lep1id) == 11 ? do_fill(h_pte, lep1pt) : do_fill(h_ptm, lep1pt);
                abs(lep2id) == 11 ? do_fill(h_pte, lep2pt) : do_fill(h_ptm, lep2pt);
                do_fill(h_eta1,   ss::lep1_p4().eta());
                do_fill(h_eta2,   ss::lep2_p4().eta());
                int type = ss::hyp_type();
                do_fill(h_type,   type>1 ? type-1 : type);
                do_fill(h_nvtx,   ss::nGoodVertices());
            };

            // if all 3 charges are the same, throw the event away
            if ((lep1id>0) == (lep2id>0) and nleps > 2) {
              int q1 = 2*(lep1id > 0) - 1;
              int q3 = 2*(lep3id > 0) - 1;
              if (q3==q1) {
                  // cout << "Skipping +++/---" << endl;
                  continue;
              }
            }

            bool ZVeto = false;
            if (hyp_class == 4 and abs(lep1id) == abs(lep2id) and lep1id*lep2id<0) {
                float m = (ss::lep1_p4()+ss::lep2_p4()).M();
                if (m < 12 or abs(m - 91.1) < 15) ZVeto = true;
            }

            if (hyp_class == 6 and
                    lep1pt > 25. and
                    lep2pt > 20. and
                    lep3pt > 20. and
                    (class6Fake or (lep1good and lep2good and lep3good)) and
                    ht > 350 and
                    met > 50 and
                    nleps > 2 and
                    nbtags >= 2 and
                    njets >= 2) fill_region("crz");

            if (hyp_class == 3 and
                    lep1pt > 25. and
                    lep2pt > 20. and
                    ht > 350 and
                    met > 50 and
                    nleps == 2 and
                    nbtags == 2 and
                    njets <= 5) fill_region("crw");

            if (lep1pt > 25. and
                    lep2pt > 20. and
                    njets >= 2 and
                    met > 50. and
                    ht > 350) {
                if (hyp_class == 4 and !ZVeto) fill_region("os");
                if (hyp_class == 2)            fill_region("tl");
            }

            if (hyp_class == 3 and
                    lep1pt >= 25. and
                    lep2pt >= 20. and
                    njets >= 2) {
                if (nbtags < 2  and ht > 350 and met > 50) fill_region("bdt_nb");   // invert Nb
                if (nbtags >= 2 and ht < 350 and met > 50) fill_region("bdt_ht");   // invert Ht
                if (nbtags >= 2 and ht > 350 and met < 50) fill_region("bdt_met");  // invert MET
            }

            if (lep1pt >= 25. and lep2pt >= 20.  and hyp_class == 4 and !ZVeto and nleps == 2 and nbtags == 2 and njets >= 2) {
                if (nbtags < 2  and ht > 350 and met > 50) fill_region("isr");
            }

        }//event loop

        delete file;
    }//file loop


    TFile f1(Form("outputs/histos_%s.root", ch->GetTitle()), "RECREATE");

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

