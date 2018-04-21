#pragma GCC diagnostic ignored "-Wsign-compare"

#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TRandom.h"
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
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> Vec4;
// using namespace tas;

float lumiAG = getLumi();
// float lumiAG = 36.3;
struct HistCol {
    map<string, TH1D> ee;
    map<string, TH1D> em;
    map<string, TH1D> mm;
    map<string, TH1D> in;

    HistCol(vector<string> regions, const string& name, int nbins, float low, float high, vector<HistCol*>* registry=nullptr) {
        for (string region : regions) {
            string base_name = region + "_" + name;
            string base_title = region + " " + name;
            ee.emplace(region, TH1D((base_name + "_ee").c_str(), (base_title + " ee").c_str(), nbins, low, high));
            em.emplace(region, TH1D((base_name + "_em").c_str(), (base_title + " em").c_str(), nbins, low, high));
            mm.emplace(region, TH1D((base_name + "_mm").c_str(), (base_title + " mm").c_str(), nbins, low, high));
            in.emplace(region, TH1D((base_name + "_in").c_str(), (base_title + " in").c_str(), nbins, low, high));
        }
        if (registry != nullptr)
            registry->push_back(this);
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
    bool doFlips = options.Contains("doFlips");
    bool useInclusiveSFs = options.Contains("useInclusiveSFs");

    // Clear already-seen list
    duplicate_removal::clear_list();

    // Used to determine which "era" a MC even is in
    TRandom *tr1 = new TRandom();

    cout << "Working on " << ch->GetTitle() << endl;

    vector<string> regions = {"os", "tl",                     // OS tight-tight and SS tight-loose
                              "crz", "crw",                   // CRZ, CRW
                              "bdt_nb", "bdt_ht", "bdt_met",  // Baseline w/ inverted nbtags/Ht/MET selection
                              "isr"};

    vector<HistCol*> registry;
    HistCol h_met    (regions, "met"   , 15, 0    , 600 , &registry);
    HistCol h_ht     (regions, "ht"    , 16, 0    , 1600, &registry);
    HistCol h_mll    (regions, "mll"   , 25, 0    , 300 , &registry);
    HistCol h_zmll   (regions, "zmll"  , 25, 0    , 300 , &registry);
    HistCol h_mtmin  (regions, "mtmin" , 25, 0    , 300 , &registry);
    HistCol h_njets  (regions, "njets" , 8 , 0    , 8   , &registry);
    HistCol h_nbtags (regions, "nbtags", 5 , 0    , 5   , &registry);
    HistCol h_pt1    (regions, "pt1"   , 25, 0    , 300 , &registry);
    HistCol h_pt2    (regions, "pt2"   , 25, 0    , 300 , &registry);
    HistCol h_pt3    (regions, "pt3"   , 25, 0    , 300 , &registry);
    HistCol h_eta1   (regions, "eta1"  , 25, -3.2 , 3.2 , &registry);
    HistCol h_eta2   (regions, "eta2"  , 25, -3.2 , 3.2 , &registry);
    HistCol h_etae   (regions, "etae"  , 25, -3.2 , 3.2 , &registry);
    HistCol h_etam   (regions, "etam"  , 25, -3.2 , 3.2 , &registry);
    HistCol h_pte    (regions, "pte"   , 25, 0    , 300 , &registry);
    HistCol h_ptm    (regions, "ptm"   , 25, 0    , 300 , &registry);
    HistCol h_type   (regions, "type"  , 3 , 0    , 3   , &registry);
    HistCol h_nvtx   (regions, "nvtx"  , 70, 0    , 70  , &registry);

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

            /* if (event > 10000) break; */

            if (!ss::fired_trigger()) continue;
            if (!ss::passes_met_filters()) continue;

            SSAG::progress(nEventsTotal, nEventsChain);

            // Reject duplicates
            if (ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)) continue;
            }

            // Save a bunch of event info for quick reference later
            float lep1ccpt = ss::lep1_coneCorrPt();
            float lep2ccpt = ss::lep2_coneCorrPt();
            float lep3ccpt = ss::lep3_coneCorrPt();
            float lep1pt = ss::lep1_p4().pt();
            float lep2pt = ss::lep2_p4().pt();
            float lep3pt = ss::lep3_p4().pt();
            float lep1eta = ss::lep1_p4().eta();
            float lep2eta = ss::lep2_p4().eta();
            float lep3eta = ss::lep3_p4().eta();
            int lep1id = ss::lep1_id();
            int lep2id = ss::lep2_id();
            int lep3id = ss::lep3_id();
            int lep1good = ss::lep1_passes_id();
            int lep2good = ss::lep2_passes_id();
            int lep3good = ss::lep3_passes_id();
            int nleps = (ss::lep3_passes_id() and lep3ccpt > 20.) ? 3 : 2;
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
                float rand = -1;
                if (!useInclusiveSFs) {
                    tr1->SetSeed(ss::event());
                    rand = tr1->Rndm();
                }
                weight *= getTruePUw(ss::trueNumInt()[0]);
                weight *= leptonScaleFactor(lep1id, lep1pt, lep1eta, ht, rand);
                weight *= leptonScaleFactor(lep2id, lep2pt, lep2eta, ht, rand);
                if (nleps > 2) {
                    weight *= leptonScaleFactor(lep3id, lep3pt, lep3eta, ht, rand);
                }
                weight *= ss::weight_btagsf();
                // weight *= eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
            }

            bool class6Fake = false;
            if (doFakes) {
                if (hyp_class == 6) {
                    if ((ss::lep3_fo() && !ss::lep3_tight()) && lep1good && lep2good) {  // lep3 fake
                        float fr = fakeRate(lep3id, lep3pt, lep3eta, ht);
                        class6Fake = true;
                        weight *= fr / (1-fr);
                    }
                    if ((ss::lep2_fo() && !ss::lep2_tight()) && lep1good && lep3good) {  // lep2 fake
                        float fr = fakeRate(lep2id, lep2pt, lep2eta, ht);
                        class6Fake = true;
                        weight *= fr / (1-fr);
                    }
                    if ((ss::lep1_fo() && !ss::lep1_tight()) && lep2good && lep3good) {  // lep1 fake
                        float fr = fakeRate(lep1id, lep1pt, lep1eta, ht);
                        class6Fake = true;
                        weight *= fr / (1-fr);
                    }
                    if (!class6Fake) {
                        continue; // No fakes!
                    }
                } else if (hyp_class == 1 or hyp_class == 2) {
                    if (ss::lep1_passes_id()==0) {
                        float fr = fakeRate(lep1id, lep1pt, lep1eta, ht);
                        weight *= fr/(1.-fr);
                    }
                    if (ss::lep2_passes_id()==0) {
                        float fr = fakeRate(lep2id, lep2pt, lep2eta, ht);
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


            if (doFlips) {
                /* if (hyp_class != 4 && hyp_class != 6) continue; */
                if (hyp_class == 4) hyp_class = 3; // we've flipped an OS to a SS
                else if (hyp_class == 6) class6Fake = true;
                else continue;
                float flipFact = 0.;
                if (abs(lep1id) == 11) {
                    float flr = flipRate(lep1pt, lep1eta);
                    flipFact += (flr/(1-flr));
                }
                if (abs(lep2id) == 11) {
                    float flr = flipRate(lep2pt, lep2eta);
                    flipFact += (flr/(1-flr));
                }
                weight *= flipFact;
                if (weight == 0.0) continue; // just quit if there are no flips.
            }

            // if all 3 charges are the same, throw the event away
            if ((lep1id>0) == (lep2id>0) and nleps > 2) {
              int q1 = 2*(lep1id > 0) - 1;
              int q3 = 2*(lep3id > 0) - 1;
              if (q3==q1) {
                  // cout << "Skipping +++/---" << endl;
                  continue;
              }
            }

            auto mll = [](const Vec4& p1, const Vec4& p2) {return (p1+p2).M();};
            auto sfos = [](int id1, int id2) {return abs(id1) == abs(id2) and id1*id2<0;};
            auto z_veto = [](bool sfos, float mll) { return sfos and (mll < 12 or abs(mll - 91.1) < 15); };
            float m12 = mll(ss::lep1_p4(), ss::lep2_p4());
            float m13 = mll(ss::lep1_p4(), ss::lep3_p4());
            float m23 = mll(ss::lep2_p4(), ss::lep3_p4());
            auto zveto12 = z_veto(sfos(lep1id, lep2id), m12);

            auto fill_region = [&](const string& region) {
                // Fill all observables for a region
                auto do_fill = [region, lep1id, lep2id, weight](HistCol& h, float val) {
                    h.Fill(region, lep1id, lep2id, val, weight);
                };
                do_fill(h_met,    met);
                do_fill(h_njets,  njets);
                do_fill(h_ht,     ht);
                do_fill(h_nbtags, nbtags);
                do_fill(h_mll,    ss::dilep_p4().M());
                do_fill(h_mtmin,  ss::mtmin());
                do_fill(h_pt1,    lep1ccpt);
                do_fill(h_pt2,    lep2ccpt);
                if (nleps > 2) do_fill(h_pt3, lep3pt);
                if (nleps > 2 and sfos(lep1id, lep3id)) do_fill(h_zmll, m13);
                if (nleps > 2 and sfos(lep2id, lep3id)) do_fill(h_zmll, m23);

                if (abs(lep1id) == 11) {
                   do_fill(h_pte, lep1ccpt);
                   do_fill(h_etae, lep1eta);
                } else {
                   do_fill(h_ptm, lep1ccpt);
                   do_fill(h_etam, lep1eta);
                }
                if (abs(lep2id) == 11) {
                   do_fill(h_pte, lep2ccpt);
                   do_fill(h_etae, lep2eta);
                } else {
                   do_fill(h_ptm, lep2ccpt);
                   do_fill(h_etam, lep2eta);
                }
                do_fill(h_eta1,   ss::lep1_p4().eta());
                do_fill(h_eta2,   ss::lep2_p4().eta());
                int type = ss::hyp_type();
                do_fill(h_type,   type>1 ? type-1 : type);
                do_fill(h_nvtx,   ss::nGoodVertices());
            };

            if (hyp_class == 6 and
                    lep1ccpt > 25. and
                    lep2ccpt > 20. and
                    lep3ccpt > 20. and
                    (class6Fake or (lep1good and lep2good and lep3good)) and
                    ht > 300 and
                    met > 50 and
                    nleps > 2 and
                    nbtags >= 2 and
                    njets >= 2) fill_region("crz");

            if (hyp_class == 3 and
                    lep1ccpt > 25. and
                    lep2ccpt > 20. and
                    ht > 300 and
                    met > 50 and
                    nleps == 2 and
                    nbtags == 2 and
                    njets <= 5) fill_region("crw");

            if (lep1ccpt > 25. and
                    lep2ccpt > 20. and
                    njets >= 2 and
                    met > 50. and
                    ht > 300) {
                if (hyp_class == 4 and !zveto12) fill_region("os");
                if (hyp_class == 2)            fill_region("tl");
            }

            if (hyp_class == 3 and
                    lep1ccpt >= 25. and
                    lep2ccpt >= 20. and
                    njets >= 2) {
                if (nbtags < 2  and ht > 300 and met > 50) fill_region("bdt_nb");   // invert Nb
                if (nbtags >= 2 and ht < 300 and met > 50) fill_region("bdt_ht");   // invert Ht
                if (nbtags >= 2 and ht > 300 and met < 50) fill_region("bdt_met");  // invert MET
            }

            if (hyp_class == 4 and !zveto12 and
                    lep1ccpt >= 25. and
                    lep2ccpt >= 20. and
                    nbtags == 2 and
                    njets >= 2 and
                    ht > 300 and
                    met > 50) fill_region("isr");
        }//event loop
        delete file;
    }//file loop

    TFile f1(Form("outputs/histos_%s.root", ch->GetTitle()), "RECREATE");
    for (HistCol* coll : registry) coll->Write();
    f1.Close();
    return 0;
}

