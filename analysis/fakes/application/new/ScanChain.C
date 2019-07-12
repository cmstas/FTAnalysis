#pragma GCC diagnostic ignored "-Wsign-compare"

#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TRandom.h"
#include "TH2F.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include "Math/VectorUtil.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../../misc/class_files/v8.02/SS.h"
#include "../../../../common/CORE/Tools/utils.h"
#include "../../../misc/common_utils.h"
#include "../../../misc/tqdm.h"

namespace frs2016 {
// #include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/derivation/frs_2016_ss.h"
#include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/derivation/frs_2016_temp.h"
}
namespace frs2017 {
// #include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/derivation/frs_2017_ss.h"
#include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/derivation/frs_2017_temp.h"
}
namespace frs2018 {
// #include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/derivation/frs_2018_ss.h"
#include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/derivation/frs_2018_temp.h"
}

namespace frs2017ft {
// #include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/derivation/frs_2017_ss.h"
#include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/derivation/frs_ft_2017_temp.h"
}
namespace frs2018ft {
// #include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/derivation/frs_2018_ss.h"
#include "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/derivation/frs_ft_2018_temp.h"
}

using namespace std;
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> Vec4;
// using namespace tas;

bool STOP_REQUESTED = false;
struct HistCol {
    map<string, TH1D> e;
    map<string, TH1D> m;
    map<string, TH1D> in;


    HistCol(vector<string> regions, const string& name, int nbins, const float* bins, vector<HistCol*>* registry=nullptr) {
        for (string region : regions) {
            string base_name = region + "_" + name;
            string base_title = region + " " + name;
            e.emplace(region, TH1D((base_name + "_e").c_str(), (base_title + " e").c_str(), nbins, bins));
            m.emplace(region, TH1D((base_name + "_m").c_str(), (base_title + " m").c_str(), nbins, bins));
            in.emplace(region, TH1D((base_name + "_in").c_str(), (base_title + " in").c_str(), nbins, bins));
        }
        if (registry != nullptr)
            registry->push_back(this);
    }

    HistCol(vector<string> regions, const string& name, int nbins, float low, float high, vector<HistCol*>* registry=nullptr) {
        for (string region : regions) {
            string base_name = region + "_" + name;
            string base_title = region + " " + name;
            e.emplace(region, TH1D((base_name + "_e").c_str(), (base_title + " e").c_str(), nbins, low, high));
            m.emplace(region, TH1D((base_name + "_m").c_str(), (base_title + " m").c_str(), nbins, low, high));
            in.emplace(region, TH1D((base_name + "_in").c_str(), (base_title + " in").c_str(), nbins, low, high));
        }
        if (registry != nullptr)
            registry->push_back(this);
    }

    void Fill(const string& region, int id, float val, float weight) {
        in[region].Fill(val, weight);
        if (abs(id) == 11) {
            e[region].Fill(val, weight);
        } else {
            m[region].Fill(val, weight);
        }
    }
    void Write() {
        for (auto p : e) p.second.Write();
        for (auto p : m) p.second.Write();
        for (auto p : in) p.second.Write();
    }
};

float getFakeRate(int year, int id, float pt, float eta, bool doSS){
    float fact = 0.;
    if (year == 2016) fact = (abs(id)==13) ? frs2016::muonQCDMCFakeRate_IsoTrigs(pt,eta) : frs2016::electronQCDMCFakeRate_IsoTrigs(pt,eta);
    if (doSS) {
        if (year == 2017) fact = (abs(id)==13) ? frs2017::muonQCDMCFakeRate_IsoTrigs(pt,eta) : frs2017::electronQCDMCFakeRate_IsoTrigs(pt,eta);
        if (year == 2018) fact = (abs(id)==13) ? frs2018::muonQCDMCFakeRate_IsoTrigs(pt,eta) : frs2018::electronQCDMCFakeRate_IsoTrigs(pt,eta);
    } else {
        if (year == 2017) fact = (abs(id)==13) ? frs2017ft::muonQCDMCFakeRate_IsoTrigs(pt,eta) : frs2017ft::electronQCDMCFakeRate_IsoTrigs(pt,eta);
        if (year == 2018) fact = (abs(id)==13) ? frs2018ft::muonQCDMCFakeRate_IsoTrigs(pt,eta) : frs2018ft::electronQCDMCFakeRate_IsoTrigs(pt,eta);
    }
    if (fact > 0.8) fact = 0.8;
    if (fact < 0.) fact = 0.;
    return fact;
}

float getFakeRateError(int year, int id, float pt, float eta, bool doSS) {
    float fact = 0.;
    if (year == 2016) fact = (abs(id)==13) ? frs2016::muonQCDMCFakeRateError_IsoTrigs(pt,eta) : frs2016::electronQCDMCFakeRateError_IsoTrigs(pt,eta);
    if (doSS) {
        if (year == 2017) fact = (abs(id)==13) ? frs2017::muonQCDMCFakeRateError_IsoTrigs(pt,eta) : frs2017::electronQCDMCFakeRateError_IsoTrigs(pt,eta);
        if (year == 2018) fact = (abs(id)==13) ? frs2018::muonQCDMCFakeRateError_IsoTrigs(pt,eta) : frs2018::electronQCDMCFakeRateError_IsoTrigs(pt,eta);
    } else {
        if (year == 2017) fact = (abs(id)==13) ? frs2017ft::muonQCDMCFakeRateError_IsoTrigs(pt,eta) : frs2017ft::electronQCDMCFakeRateError_IsoTrigs(pt,eta);
        if (year == 2018) fact = (abs(id)==13) ? frs2018ft::muonQCDMCFakeRateError_IsoTrigs(pt,eta) : frs2018ft::electronQCDMCFakeRateError_IsoTrigs(pt,eta);
    }
    return fact;
}

float getFakeWeight(int year, int id, float pt, float eta, bool doSS, int which=0) {
    float fr = getFakeRate(year,id,pt,eta,doSS);
    if (which == 0) {
    } else if (which == -1) {
        fr -= getFakeRateError(year,id,pt,eta,doSS);
        if (fr < 0.) fr = 0.;
    } else if (which == 1) {
        fr += getFakeRateError(year,id,pt,eta,doSS);
        if (fr > 0.8) fr = 0.8;
    }
    return fr/(1.-fr);
}



int ScanChain(TChain *ch, TString options="", TString outputdir="outputs"){

    signal(SIGINT, [](int){
            cout << "SIGINT Caught, stopping after current event" << endl;
            STOP_REQUESTED=true;
            });
    STOP_REQUESTED=false;

    bool doSS = options.Contains("doSS");
    bool quiet = options.Contains("quiet");

    ana_t analysis = FTANA;
    if (doSS) {
        analysis = SSANA;
    }

    TString proc(ch->GetTitle());

    int year;
    float lumiAG = 0.;
    if (options.Contains("Data2016")) {
        lumiAG = getLumi(2016);
        year = 2016;
    } else if (options.Contains("Data2017")) {
        lumiAG = getLumi(2017);
        year = 2017;
    } else if (options.Contains("Data2018")) {
        lumiAG = getLumi(2018);
        year = 2018;
    } else {
        cout << "Which Need to specify year!\n";
        return -1;
    }

    if (!quiet) cout << "Working on " << proc << endl;

    vector<string> regions = {
        "obs",
        "pred",
        "pred_up",
        "pred_down",
    };

    vector<HistCol*> registry;
    HistCol h_met (regions   , "met"    , 20, 0 , 200 , &registry);
    HistCol h_ht (regions    , "ht"     , 15, 0 , 600, &registry);
    HistCol h_mtmin (regions , "mtmin"  , 20, 0 , 200 , &registry);
    HistCol h_njets (regions , "njets"  , 8 , 0 , 8   , &registry);
    HistCol h_nbtags (regions, "nbtags" , 5 , 0 , 5   , &registry);
    HistCol h_match (regions, "match" , 5 , 0 , 5   , &registry);
    HistCol h_pt1 (regions   , "pt1"    , 30, 0 , 150 , &registry);
    HistCol h_pt2 (regions   , "pt2"    , 30, 0 , 150 , &registry);
    HistCol h_eta1 (regions   , "eta1"    , 16, -2.5, 2.5 , &registry);
    HistCol h_eta2 (regions   , "eta2"    , 16, -2.5, 2.5 , &registry);
    HistCol h_pttrue (regions, "pttrue" , 30, 0 , 150 , &registry);
    HistCol h_ptfake (regions, "ptfake" , 30, 0 , 150 , &registry);

    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    tqdm bar;
    bar.set_theme_braille();

    while ( (currentFile = (TFile*)fileIter.Next()) ) {
        if (STOP_REQUESTED) break;

        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        TString filename(currentFile->GetTitle());

        auto tokens = filename.Tokenize("/");
        auto basename = ((TObjString*)(tokens->At(tokens->GetEntries()-1)))->String().Data();
        bar.set_label(basename);

        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            if (STOP_REQUESTED) break;

            samesign.GetEntry(event);
            nEventsTotal++;
            
            if (!quiet) bar.progress(nEventsTotal, nEventsChain);


            // Main skimming cuts in baseline
            if (ss::njets() < 2) continue;
            if (ss::met() < 50.) continue;

            bool pass_trig = (doSS) ? ss::fired_trigger_ss() : ss::fired_trigger();
            // bool pass_trig = ss::fired_trigger();
            if (!pass_trig) continue;
            if (!ss::passes_met_filters()) continue;

            int hyp_class = ss::hyp_class();
            bool class3 = hyp_class == 3;
            bool class2 = hyp_class == 2;

            if (not class3 and not class2) continue;

            if (fabs(ss::lep1_ip3d()/ss::lep1_ip3d_err())>4.) continue;
            if (fabs(ss::lep2_ip3d()/ss::lep2_ip3d_err())>4.) continue;

            // Save a bunch of event info for quick reference later
            int njets = ss::njets();
            int nbtags = ss::nbtags();
            float met = ss::met();
            int lep1id = ss::lep1_id();
            int lep2id = ss::lep2_id();
            float lep1ccpt = ss::lep1_coneCorrPt();
            float lep2ccpt = ss::lep2_coneCorrPt();
            float lep1pt = ss::lep1_p4().pt();
            float lep2pt = ss::lep2_p4().pt();
            float lep3pt = ss::lep3_p4().pt();
            float lep1eta = ss::lep1_p4().eta();
            float lep2eta = ss::lep2_p4().eta();
            float ht = ss::ht();
            float mtmin = ss::mtmin();
            int mid1 = ss::lep1_motherID();
            int mid2 = ss::lep2_motherID();

            if (abs(lep1id)==11 && lep1pt<15.) continue;
            if (abs(lep2id)==11 && lep2pt<15.) continue;
            if (abs(lep1id)==13 && lep1pt<10.) continue;
            if (abs(lep2id)==13 && lep2pt<10.) continue;

            if (!doSS) {
                if (lep1ccpt < 25.) continue;
                if (lep2ccpt < 20.) continue;
                if (lep1pt < 18.) continue;
                if (lep2pt < 18.) continue;
            }

            bool lep1nonprompt = (mid1 <= 0 and mid1 != -3);
            bool lep2nonprompt = (mid2 <= 0 and mid2 != -3);
            bool lep1prompt = (mid1 == 1);
            bool lep2prompt = (mid2 == 1);
            bool lep1good = ss::lep1_passes_id();
            bool lep2good = ss::lep2_passes_id();

            float weight = ss::is_real_data() ? 1 : ss::scale1fb()*lumiAG;
            if (!ss::is_real_data()) {
                weight *= getTruePUw(year, ss::trueNumInt()[0]);
                weight *= leptonScaleFactor(year, lep1id, lep1pt, lep1eta, ht, analysis);
                weight *= leptonScaleFactor(year, lep2id, lep2pt, lep2eta, ht, analysis);
                weight *= ss::weight_btagsf();
            }
            
            int match_type = 0;
            if ((mid2==1 || mid2==0) && (mid1==1 || mid1==0)) match_type = 0; // prompt/light
            if ((mid2==1 || mid2==-1) && (mid1==1 || mid1==-1)) match_type = 1; // prompt or b
            if ((mid2==1 || mid2==-2) && (mid1==1 || mid1==-2)) match_type = 2; // prompt or c
            if ((mid2==1 || mid2==-3) && (mid1==1 || mid1==-3)) match_type = 3; // prompt or photon
            if ((mid2==1 || mid2==-4) && (mid1==1 || mid1==-4)) match_type = 4; // prompt or light

            auto fill_region = [&](const string& region, float weight, int whichfake) {
                if (std::find(regions.begin(), regions.end(), region) == regions.end()) return;

                // Fill all observables for a region
                auto do_fill = [region, weight](HistCol& h, int id, float val) {
                    h.Fill(region, id, val, weight);
                };
                do_fill(h_met   , (whichfake == 1 ? lep1id : lep2id) , met);
                do_fill(h_ht    , (whichfake == 1 ? lep1id : lep2id) , ht);
                do_fill(h_mtmin , (whichfake == 1 ? lep1id : lep2id) , mtmin);
                do_fill(h_njets , (whichfake == 1 ? lep1id : lep2id) , njets);
                do_fill(h_nbtags, (whichfake == 1 ? lep1id : lep2id) , nbtags);
                do_fill(h_match , (whichfake == 1 ? lep1id : lep2id) , match_type);
                do_fill(h_pt1   , lep1id , lep1ccpt);
                do_fill(h_pt2   , lep2id , lep2ccpt);
                do_fill(h_eta1  , (whichfake == 1 ? lep1id : lep2id) , lep1eta);
                do_fill(h_eta2  , (whichfake == 1 ? lep1id : lep2id) , lep2eta);
                do_fill(h_pttrue, (whichfake == 1 ? lep1id : lep2id) , (whichfake == 1 ? lep2ccpt : lep1ccpt));
                do_fill(h_ptfake, (whichfake == 1 ? lep1id : lep2id) , (whichfake == 1 ? lep1ccpt : lep2ccpt));
            };

            // Observed fakes
            if (class3) {
                if (lep1prompt and lep2nonprompt) {
                    fill_region("obs",weight,2);
                }
                if (lep2prompt and lep1nonprompt) {
                    fill_region("obs",weight,1);
                }
            }
            // Predicted fakes
            if (class2) {
                if (lep1good and not lep2good) {
                    float w = getFakeWeight(year, lep2id, lep2ccpt, lep2eta, doSS, 0);
                    float w_up = getFakeWeight(year, lep2id, lep2ccpt, lep2eta, doSS, 1);
                    float w_down = getFakeWeight(year, lep2id, lep2ccpt, lep2eta, doSS, -1);
                    fill_region("pred",weight*w,2);
                    fill_region("pred_up",weight*w_up,2);
                    fill_region("pred_down",weight*w_down,2);
                }
                if (lep2good and not lep1good) {
                    float w = getFakeWeight(year, lep1id, lep1ccpt, lep1eta, doSS, 0);
                    float w_up = getFakeWeight(year, lep1id, lep1ccpt, lep1eta, doSS, 1);
                    float w_down = getFakeWeight(year, lep1id, lep1ccpt, lep1eta, doSS, -1);
                    fill_region("pred",weight*w,1);
                    fill_region("pred_up",weight*w_up,1);
                    fill_region("pred_down",weight*w_down,1);
                }
            }

        }//event loop
        delete file;
    }//file loop

    TFile f1(Form("%s/histos_%s.root", outputdir.Data(), ch->GetTitle()), "RECREATE");
    for (HistCol* coll : registry) coll->Write();
    f1.Close();
    if (!quiet) cout << "\n Done!" << endl;
    return 0;
}

