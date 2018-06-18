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
#include "TMVA/Reader.h"

#include "../misc/class_files/v8.02/SS.h"
#include "../../common/CORE/Tools/dorky/dorky.h"
#include "../../common/CORE/Tools/utils.h"
#include "../misc/common_utils.h"
#include "info_2016.h"
#include "info_2018.h"
#include "tqdm.h"

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


    HistCol(vector<string> regions, const string& name, int nbins, const float* bins, vector<HistCol*>* registry=nullptr) {
        for (string region : regions) {
            string base_name = region + "_" + name;
            string base_title = region + " " + name;
            ee.emplace(region, TH1D((base_name + "_ee").c_str(), (base_title + " ee").c_str(), nbins, bins));
            em.emplace(region, TH1D((base_name + "_em").c_str(), (base_title + " em").c_str(), nbins, bins));
            mm.emplace(region, TH1D((base_name + "_mm").c_str(), (base_title + " mm").c_str(), nbins, bins));
            in.emplace(region, TH1D((base_name + "_in").c_str(), (base_title + " in").c_str(), nbins, bins));
        }
        if (registry != nullptr)
            registry->push_back(this);
    }

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

tuple<int, int, int, float, float> calc_jet_quants() {
    float bloose =  0.1522;
    float bmed = 0.4941;
    float btight = 0.8001;

    int nlb40 = 0;
    int ntb40 = 0;
    int nisrjets = 0;
    float mjoverpt = 0.;
    float htb = 0;
    for (unsigned int ijet = 0; ijet < ss::jets().size(); ijet++) {
        const auto& jet = ss::jets()[ijet];
        mjoverpt = std::max(mjoverpt, jet.M()/jet.pt());
        float disc = ss::jets_disc().at(ijet);

        if (disc > bloose) nlb40++;
        if (disc > btight) ntb40++;
        if (disc < bmed) nisrjets++;
    }
    for (size_t ibtag=0; ibtag < ss::btags().size(); ibtag++) {
        htb += ss::btags().at(ibtag).pt();
    }
    return std::make_tuple(nlb40, ntb40, nisrjets, mjoverpt, htb);
}

float isr_reweight(const std::string& proc, int nisrmatch, bool normalized=true) {
    std::vector<float> weights = {0.95574998, 0.84294129, 0.8859281, 1.02554868,
                                  1.35228373, 1.20783354, 1.81862093, 1.49815168};
    std::vector<float> weights_norm = {1.04525021, 0.92187767, 0.96888994, 1.12158514,
                                       1.47891696, 1.32093988, 1.98892384, 1.63844457};
    if (proc != "tt") return 1;
    if (nisrmatch < 0 or nisrmatch >= weights.size()) return 1;
    if (normalized) return weights_norm[nisrmatch];
    else return weights[nisrmatch];
}

float btag_reweight(int nbtags) {
    if (ss::is_real_data()) return 1;
    if (nbtags == 0) return 1.14;
    if (nbtags == 1) return 0.979;
    if (nbtags == 2) return 0.905;
    if (nbtags == 3) return 1.26;
    if (nbtags >= 4) return 1.11;
    return 1;
}


int ScanChain(TChain *ch, int year, TString options="", TString outputdir="outputs"){

    bool doFakes = options.Contains("doFakes");
    bool doFlips = options.Contains("doFlips");
    bool useInclusiveSFs = options.Contains("useInclusiveSFs");
    bool zeroMissingInnerHits = options.Contains("zeroMissingInnerHits");
    bool evaluateBDT = options.Contains("evaluateBDT");
    bool useNonIsoTriggers2017 = options.Contains("useNonIsoTriggers2017");
    bool useNonIsoTriggers2016 = options.Contains("useNonIsoTriggers2016");
    bool useIsoTriggers2016 = options.Contains("useIsoTriggers2016");
    bool doHighHT = options.Contains("doHighHT");
    bool doTruthFake = options.Contains("doTruthFake");
    bool quiet = options.Contains("quiet");
    std::string proc = ch->GetTitle();
    int year = 2017;
    if (options.Contains("Data2016")) year = 2016;
    if (options.Contains("Data2017")) year = 2017;
    if (options.Contains("Data2018")) year = 2018;

    if (year == 2016) lumiAG = 35.87;
    if ((year == 2017) && useNonIsoTriggers2017) lumiAG = 36.529;
    if (year == 2018) lumiAG = 14.383;
    // if (year == 2018) lumiAG = 555.01 / 1000.0;

    bool is2016 = year == 2016;
    bool is2017 = year == 2017;
    bool is2018 = year == 2018;

    if (is2016) {
        lumiAG = 35.87;
    } else if (is2017) {
        if (useNonIsoTriggers) {
            lumiAG = 36.529;
        } else{
            lumiAG = 41.3;
        }
    } else {
        cout << "Year not Implemented: " << year << endl;
        return -1;
    }

    // Clear already-seen list
    duplicate_removal::clear_list();

    // Used to determine which "era" a MC event is in
    TRandom *tr1 = new TRandom();

    if (!quiet) cout << "Working on " << proc << endl;


    vector<string> regions = {
                                "osloose",
                                "os",
                              "os_noisr", "os_btagreweight", // OS tight-tight and variants
                              "os_highbdt", "os_lowbdt",
                              "tl",                                // SS tight-loose
                              "crz", 
                              "bdt_nb", "bdt_ht",                  // Baseline w/ inverted nbtags/Ht/MET selection
                              "bdt_met","bdt_met_ht",
                              "bdt_train",                         // BDT Training region - BR + CRW
                              "isr",                               // ISR Reweighting derivation region
                              "isr_reweight_check",
                              "crw",                        // CRZ, CRW
                              "htnb1"                                // Baseline without Ht, with Nb==1
                              };

    vector<HistCol*> registry;
    HistCol h_met         (regions, "met"        , 30, 0   , 600 , &registry);
    HistCol h_metphi         (regions, "metphi"        , 60, -3.2   , 3.2 , &registry);
    HistCol h_rawmet         (regions, "rawmet"        , 60, 0   , 600 , &registry);
    HistCol h_calomet         (regions, "calomet"        , 60, 0   , 600 , &registry);
    HistCol h_ht          (regions, "ht"         , 16, 0   , 1600, &registry);
    HistCol h_htb         (regions, "htb"        , 16, 0   , 1600, &registry);
    HistCol h_mll         (regions, "mll"        , 30, 0   , 300 , &registry);
    HistCol h_zmll        (regions, "zmll"       , 30, 0   , 300 , &registry);
    HistCol h_mtmin       (regions, "mtmin"      , 30, 0   , 300 , &registry);
    HistCol h_njets       (regions, "njets"      , 8 , 0   , 8   , &registry);
    HistCol h_nisrjets    (regions, "nisrjets"   , 8 , 0   , 8   , &registry);
    HistCol h_nisrmatch   (regions, "nisrmatch"  , 8 , 0   , 8   , &registry);
    HistCol h_nlb40       (regions, "nlb40"      , 8 , 0   , 8   , &registry);
    HistCol h_ntb40       (regions, "ntb40"      , 8 , 0   , 8   , &registry);
    HistCol h_nbtags      (regions, "nbtags"     , 5 , 0   , 5   , &registry);
    HistCol h_maxmjoverpt (regions, "maxmjoverpt", 50, 0   , 0.35, &registry);

    HistCol h_pt1         (regions, "pt1"        , 30, 0   , 300 , &registry);
    HistCol h_pt2         (regions, "pt2"        , 30, 0   , 300 , &registry);
    HistCol h_pt3         (regions, "pt3"        , 30, 0   , 300 , &registry);
    HistCol h_pte         (regions, "pte"        , 30, 0   , 300 , &registry);
    HistCol h_ptm         (regions, "ptm"        , 30, 0   , 300 , &registry);

    HistCol h_eta1        (regions, "eta1"       , 25, -3.2, 3.2 , &registry);
    HistCol h_eta2        (regions, "eta2"       , 25, -3.2, 3.2 , &registry);
    HistCol h_etae        (regions, "etae"       , 25, -3.2, 3.2 , &registry);
    HistCol h_etam        (regions, "etam"       , 25, -3.2, 3.2 , &registry);

    HistCol h_ptrel1      (regions, "ptrel1"     , 15, 0   , 50  , &registry);
    HistCol h_ptrel2      (regions, "ptrel2"     , 15, 0   , 50  , &registry);
    HistCol h_ptrele      (regions, "ptrele"     , 15, 0   , 50  , &registry);
    HistCol h_ptrelm      (regions, "ptrelm"     , 15, 0   , 50  , &registry);

    HistCol h_nmiss1      (regions, "nmiss1"     , 3, 0   , 3  , &registry);
    HistCol h_nmiss2      (regions, "nmiss2"     , 3, 0   , 3  , &registry);

    HistCol h_ptrelfail1  (regions, "ptrelfail1" , 15, 0   , 50  , &registry);
    HistCol h_ptrelfail2  (regions, "ptrelfail2" , 15, 0   , 50  , &registry);
    HistCol h_ptrelfaile  (regions, "ptrelfaile" , 15, 0   , 50  , &registry);
    HistCol h_ptrelfailm  (regions, "ptrelfailm" , 15, 0   , 50  , &registry);

    HistCol h_ptratio1    (regions, "ptratio1"   , 30, 0   , 1.5 , &registry);
    HistCol h_ptratio2    (regions, "ptratio2"   , 30, 0   , 1.5 , &registry);
    HistCol h_ptratioe    (regions, "ptratioe"   , 30, 0   , 1.5 , &registry);
    HistCol h_ptratiom    (regions, "ptratiom"   , 30, 0   , 1.5 , &registry);

    HistCol h_miniiso1    (regions, "miniiso1"   , 15,  0  , 0.2 , &registry);
    HistCol h_miniiso2    (regions, "miniiso2"   , 15,  0  , 0.2 , &registry);
    HistCol h_miniisoe    (regions, "miniisoe"   , 15,  0  , 0.2 , &registry);
    HistCol h_miniisom    (regions, "miniisom"   , 15,  0  , 0.2 , &registry);

    HistCol h_dphil1l2    (regions, "dphil1l2"   , 15,  0  , 4   , &registry);
    HistCol h_detal1l2    (regions, "detal1l2"   , 30,  -4 , 4   , &registry);
    HistCol h_absdetal1l2 (regions, "absdetal1l2", 15,  0  , 4   , &registry);

    HistCol h_type        (regions, "type"       , 3  , 0  , 3   , &registry);
    HistCol h_q1          (regions, "q1"         , 2  , -2 , 2   , &registry);
    HistCol h_type3l      (regions, "type3l"     , 4  , 0  , 4   , &registry);
    HistCol h_nvtx        (regions, "nvtx"       , 14 , 0  , 70  , &registry);

    HistCol h_ptj1        (regions, "ptj1"       , 50 , 0  , 500 , &registry);
    HistCol h_ptj2        (regions, "ptj2"       , 50 , 0  , 500 , &registry);
    HistCol h_ptj3        (regions, "ptj3"       , 50 , 0  , 500 , &registry);
    HistCol h_ptj4        (regions, "ptj4"       , 50 , 0  , 500 , &registry);
    HistCol h_ptj5        (regions, "ptj5"       , 50 , 0  , 500 , &registry);
    HistCol h_ptj6        (regions, "ptj6"       , 30 , 0  , 300 , &registry);
    HistCol h_ptj7        (regions, "ptj7"       , 30 , 0  , 300 , &registry);
    HistCol h_ptj8        (regions, "ptj8"       , 30 , 0  , 300 , &registry);

    HistCol h_ml1j1       (regions, "ml1j1"      , 50 , 0  , 500 , &registry);
    HistCol h_matchtype   (regions, "matchtype"  , 4  , 0  , 4   , &registry);

    HistCol h_matchtype       (regions, "matchtype"      , 4 , 0  , 4 , &registry);

    /* HistCol h_event_bdt   (regions, "event_bdt"  , 25 , -1 , 1   , &registry); */
    float bdt_bins[] = {-1.00, -0.48, -0.37, -0.25, -0.14, -0.02, 0.075, 0.21, 0.31, 0.465, 0.60, 0.715, 0.81, 1.00};
    HistCol h_event_bdt   (regions, "event_bdt"  , 13 , bdt_bins  , &registry);


    // Declare a bunch of event variables to be filled below in the loop
    float lep1ccpt, lep2ccpt, lep3ccpt;
    float lep1pt,   lep2pt,   lep3pt;
    float lep1eta,  lep2eta,  lep3eta;
    float lep1phi,  lep2phi,  lep3phi;
    int   lep1id,   lep2id,   lep3id;
    int   lep1q,    lep2q,    lep3q;
    int   lep1good, lep2good, lep3good;

    float lep1ptrel, lep2ptrel;
    float lep1miniiso, lep2miniiso;
    float lep1ptratio, lep2ptratio;
    float dphil1l2, detal1l2;
    float nmiss1, nmiss2;

    int nleps, njets, nbtags;
    float ht, htb, met, metphi, rawmet, calomet;
    float maxmjoverpt, ml1j1;
    int matchtype;
    int nlb40, ntb40, nisrjets, nisrmatch;
    float ptj1, ptj2, ptj3, ptj4, ptj5, ptj6, ptj7, ptj8;

    int SR;
    float weight;

    // Because TMVA is dumb, we need also float versions of these variables.
    float nbtags_f, njets_f, nlb40_f, ntb40_f, nleps_f, lep1q_f, SR_f;
    auto set_float_vals = [&]() {
        nbtags_f = nbtags;
        njets_f = njets;
        nlb40_f = nlb40;
        ntb40_f = ntb40;
        nleps_f = nleps;
        lep1q_f = lep1q;
        SR_f = SR;
    };
    TMVA::Reader reader("Silent");
    reader.AddVariable("nbtags",      &nbtags_f);
    reader.AddVariable("njets",       &njets_f);
    reader.AddVariable("met",         &met);
    reader.AddVariable("ptl2",        &lep2ccpt);
    reader.AddVariable("nlb40",       &nlb40_f);
    reader.AddVariable("ntb40",       &ntb40_f);
    reader.AddVariable("nleps",       &nleps_f);
    reader.AddVariable("htb",         &htb);
    reader.AddVariable("q1",          &lep1q_f);
    reader.AddVariable("ptj1",        &ptj1);
    reader.AddVariable("ptj6",        &ptj6);
    reader.AddVariable("ptj7",        &ptj7);
    reader.AddVariable("ml1j1",       &ml1j1);
    reader.AddVariable("dphil1l2",    &dphil1l2);
    reader.AddVariable("maxmjoverpt", &maxmjoverpt);
    reader.AddVariable("ptl1",        &lep1ccpt);
    reader.AddVariable("detal1l2",    &detal1l2);
    reader.AddVariable("ptj8",        &ptj8);
    reader.AddVariable("ptl3",        &lep3ccpt);

    reader.AddSpectator("weight",     &weight);
    reader.AddSpectator("ptl1",       &lep1ccpt);
    reader.AddSpectator("ptl2",       &lep2ccpt);
    reader.AddSpectator("SR",         &SR_f);

    reader.BookMVA("BDT","TMVAClassification_BDT_19vars.xml");


    int nEventsTotal = 0;
    int nEventsChain = ch->GetEntries();

    TFile *currentFile = 0;
    TObjArray *listOfFiles = ch->GetListOfFiles();
    TIter fileIter(listOfFiles);

    tqdm bar;
    bar.set_theme_braille();

    while ( (currentFile = (TFile*)fileIter.Next()) ) {

        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("t");
        samesign.Init(tree);

        TString filename(currentFile->GetTitle());

        auto tokens = filename.Tokenize("/");
        auto basename = ((TObjString*)(tokens->At(tokens->GetEntries()-1)))->String().Data();
        bar.set_label(basename);

        for(unsigned int event = 0; event < tree->GetEntriesFast(); ++event) {

            samesign.GetEntry(event);
            nEventsTotal++;

            /* if (event > 20) break; */

            // Simple cuts first to speed things up
            lep1ccpt = ss::lep1_coneCorrPt();
            lep2ccpt = ss::lep2_coneCorrPt();
            njets = ss::njets();
            nbtags = ss::nbtags();
            met = ss::met();
            metphi = ss::metPhi();
            rawmet = ss::rawmet();
            calomet = ss::calomet();
            lep1id = ss::lep1_id();
            lep2id = ss::lep2_id();
            if (lep1ccpt < 25) continue;
            if (lep2ccpt < 20) continue;

            if (doHighHT and ss::ht() < 400.) continue;

            bool pass_trig = ss::fired_trigger();
            if ((year == 2017) && !useNonIsoTriggers2017) {
                if (abs(lep1id) == 11 and !ss::lep1_isTrigSafev1()) continue;
                if (abs(lep2id) == 11 and !ss::lep2_isTrigSafev1()) continue;
            }
            // if (ss::is_real_data()) {
                if ((year == 2017) && useNonIsoTriggers2017) {
                    pass_trig = false;
                    if ( ss::hyp_type()==0 && ((ss::triggers() & 1<<7)==(1<<7)) ) pass_trig = true;
                    else if ( ss::hyp_type()==3 && (ss::triggers() & 1<<5)==(1<<5) ) pass_trig = true;
                    else if ( (ss::hyp_type()==1 || ss::hyp_type()==2) && ((ss::triggers() & 1<<0)==(1<<0)) ) pass_trig = true;
                    else {}
                }
                if ((year == 2016) && useIsoTriggers2016) {
                    pass_trig = false;
                    if ( ss::hyp_type()==0 && ((ss::triggers() & 1<<3)==(1<<3) || (ss::triggers() & 1<<4)==(1<<4)) ) pass_trig = true;
                    else if ( ss::hyp_type()==3 && (ss::triggers() & 1<<6)==(1<<6) ) pass_trig = true;
                    else if ( (ss::hyp_type()==1 || ss::hyp_type()==2) && ((ss::triggers() & 1<<1)==(1<<1) || (ss::triggers() & 1<<2)==(1<<2)) ) pass_trig = true;
                    else {}

                    if (abs(lep1id) == 11 and !ss::lep1_isTrigSafev1()) continue;
                    if (abs(lep2id) == 11 and !ss::lep2_isTrigSafev1()) continue;
                }
                if ((year == 2016) && useNonIsoTriggers2016) {
                    pass_trig = false;
                    if ( ss::hyp_type()==0 && ((ss::triggers() & 1<<7)==(1<<7)) ) pass_trig = true;
                    else if ( ss::hyp_type()==3 && (ss::triggers() & 1<<5)==(1<<5) ) pass_trig = true;
                    else if ( (ss::hyp_type()==1 || ss::hyp_type()==2) && ((ss::triggers() & 1<<0)==(1<<0)) ) pass_trig = true;
                    else {}
                }
            // }
            if (!pass_trig) continue;

            if (!ss::passes_met_filters()) continue;
            if (zeroMissingInnerHits and (ss::lep1_el_exp_innerlayers() > 0 or
                                          ss::lep2_el_exp_innerlayers() > 0)) continue;

            if (!quiet) {
                // SSAG::progress(nEventsTotal, nEventsChain);
                bar.progress(nEventsTotal, nEventsChain);
            }

            // Reject duplicates
            if (ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)) continue;
            }

            // Save a bunch of event info for quick reference later
            lep3ccpt = ss::lep3_coneCorrPt();
            lep1pt = ss::lep1_p4().pt();
            lep2pt = ss::lep2_p4().pt();
            lep3pt = ss::lep3_p4().pt();
            lep1eta = ss::lep1_p4().eta();
            lep2eta = ss::lep2_p4().eta();
            lep3eta = ss::lep3_p4().eta();
            lep1phi = ss::lep1_p4().phi();
            lep2phi = ss::lep2_p4().phi();
            lep3phi = ss::lep3_p4().phi();
            lep3id = ss::lep3_id();
            lep1q = (lep1id > 0) ? -1 : 1;
            lep2q = (lep2id > 0) ? -1 : 1;
            lep3q = (lep3id > 0) ? -1 : 1;
            lep1good = ss::lep1_passes_id();
            lep2good = ss::lep2_passes_id();
            lep3good = ss::lep3_passes_id();
            lep1ptrel = ss::lep1_ptrel_v1();
            lep2ptrel = ss::lep2_ptrel_v1();
            lep1miniiso = ss::lep1_miniIso();
            lep2miniiso = ss::lep2_miniIso();
            nmiss1 = ss::lep1_el_exp_innerlayers();
            nmiss2 = ss::lep2_el_exp_innerlayers();
            lep1ptratio = ss::lep1_closeJet().pt() > 0 ? lep1pt/ss::lep1_closeJet().pt() : 1.0;
            lep2ptratio = ss::lep2_closeJet().pt() > 0 ? lep2pt/ss::lep2_closeJet().pt() : 1.0;
            nleps = (lep3good and lep3ccpt > 20) ? 3 : 2;
            ht = ss::ht();
            // std::tie(nlb40, ntb40, nisrjets, maxmjoverpt, htb) = calc_jet_quants();
            nisrmatch = ss::nisrMatch();
            SR = 0; // Just a dummy for now

            matchtype = 0;
            if (doTruthFake) {
                int nbadlegs = (ss::lep1_motherID() <= 0) + (ss::lep2_motherID() <= 0);
                int ngoodlegs = (ss::lep1_motherID() == 1) + (ss::lep2_motherID() == 1);
                // skip the event if it's 
                // truth matched to be prompt prompt. we only want reco tight-tight
                // events that are prompt-nonprompt (or nonprompt nonprompt)
                if (ngoodlegs == 2) continue;

                //consider only prompt or lights
                if ((ss::lep2_motherID()==1 || ss::lep2_motherID()==0) && (ss::lep1_motherID()==1 || ss::lep1_motherID()==0)) matchtype = 0;
                //consider only prompt or bs
                if ((ss::lep2_motherID()==1 || ss::lep2_motherID()==-1) && (ss::lep1_motherID()==1 || ss::lep1_motherID()==-1)) matchtype = 1;
                //consider only prompt or cs
                if ((ss::lep2_motherID()==1 || ss::lep2_motherID()==-2) && (ss::lep1_motherID()==1 || ss::lep1_motherID()==-2)) matchtype = 2;

            }

            ptj1 = (njets >= 1) ? ss::jets()[0].pt() : 0;
            ptj2 = (njets >= 2) ? ss::jets()[1].pt() : 0;
            ptj3 = (njets >= 3) ? ss::jets()[2].pt() : 0;
            ptj4 = (njets >= 4) ? ss::jets()[3].pt() : 0;
            ptj5 = (njets >= 5) ? ss::jets()[4].pt() : 0;
            ptj6 = (njets >= 6) ? ss::jets()[5].pt() : 0;
            ptj7 = (njets >= 7) ? ss::jets()[6].pt() : 0;
            ptj8 = (njets >= 8) ? ss::jets()[7].pt() : 0;

            ml1j1 = (njets >= 1) ? (ss::jets()[0] + ss::lep1_p4()).M() : 0;
            set_float_vals();

            matchtype = 0;
            if (doTruthFake) {
                int nbadlegs = (ss::lep1_motherID() <= 0) + (ss::lep2_motherID() <= 0);
                int ngoodlegs = (ss::lep1_motherID() == 1) + (ss::lep2_motherID() == 1);
                // skip the event if it's truth matched to be prompt prompt.
                // We only want reco tight-tight events that are prompt-nonprompt (or nonprompt nonprompt)
                if (ngoodlegs == 2) continue;

                //consider only prompt or lights
                if ((ss::lep2_motherID()==1 || ss::lep2_motherID()==0) and
                    (ss::lep1_motherID()==1 || ss::lep1_motherID()==0)) matchtype = 0;
                //consider only prompt or bs
                if ((ss::lep2_motherID()==1 || ss::lep2_motherID()==-1) and
                    (ss::lep1_motherID()==1 || ss::lep1_motherID()==-1)) matchtype = 1;
                //consider only prompt or cs
                if ((ss::lep2_motherID()==1 || ss::lep2_motherID()==-2) and
                    (ss::lep1_motherID()==1 || ss::lep1_motherID()==-2)) matchtype = 2;
            }

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
            weight = ss::is_real_data() ? 1 : ss::scale1fb()*lumiAG;

            if (!ss::is_real_data()) {
                float rand = -1;
                if (!useInclusiveSFs) {
                    tr1->SetSeed(ss::event());
                    rand = tr1->Rndm();
                }
                if (year == 2017) {
                    weight *= getTruePUw(ss::trueNumInt()[0]);
                    weight *= leptonScaleFactor(lep1id, lep1ccpt, lep1eta, ht, rand);
                    weight *= leptonScaleFactor(lep2id, lep2ccpt, lep2eta, ht, rand);
                    if (nleps > 2) {
                        weight *= leptonScaleFactor(lep3id, lep3ccpt, lep3eta, ht, rand);
                    }
                    weight *= ss::weight_btagsf();
                } else if(year == 2016) {
                    weight *= getTruePUw_2016(ss::trueNumInt()[0]);
                    weight *= leptonScaleFactor_2016(lep1id, lep1ccpt, lep1eta, ht);
                    weight *= leptonScaleFactor_2016(lep2id, lep2ccpt, lep2eta, ht);
                    if (nleps > 2) {
                        weight *= leptonScaleFactor_2016(lep3id, lep3ccpt, lep3eta, ht);
                    }
                    weight *= ss::weight_btagsf();
                    weight *= triggerScaleFactor(lep1id, lep2id, lep1pt, lep2pt, lep1eta, lep2eta, ht);
                } else if(year == 2018) {
                    weight *= getTruePUw_2018(ss::trueNumInt()[0]);
                    weight *= leptonScaleFactor(lep1id, lep1ccpt, lep1eta, ht, rand);
                    weight *= leptonScaleFactor(lep2id, lep2ccpt, lep2eta, ht, rand);
                    weight *= ss::weight_btagsf();
                }
                // weight *= isr_reweight(proc, nisrmatch);
                // weight *= eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
            }

            bool class6Fake = false;
            if (doFakes) {
                auto yearly_fake_rate = [&](int id, float pt, float eta) {
                    if (is2016) return fakeRate_2016(id, pt, eta, ht);
                    if (is2017) return fakeRate(id, pt, eta, ht);
                    if (is2018) return fakeRate(id, pt, eta, ht);  // Just use 2017 for now
                    return 0.0f;
                };

                if (hyp_class == 6) {
                    bool lep1_lowpt_veto = lep1pt < (abs(lep1id) == 11 ? 15 : 10);
                    bool lep2_lowpt_veto = lep2pt < (abs(lep2id) == 11 ? 15 : 10);
                    bool lep3_lowpt_veto = lep3pt < (abs(lep3id) == 11 ? 15 : 10);
                    if (ss::lep3_fo() and !ss::lep3_tight() and !lep3_lowpt_veto and lep1good and lep2good) {  // lep3 fake
                        float fr = (year == 2017) ? fakeRate(lep3id, lep3ccpt, lep3eta, ht) : fakeRate_2016(lep3id, lep3ccpt, lep3eta, ht);
                        class6Fake = true;
                        weight *= fr / (1-fr);
                    }
                    if (ss::lep2_fo() and !ss::lep2_tight() and !lep2_lowpt_veto and lep1good and lep3good) {  // lep2 fake
                        float fr = (year == 2017) ? fakeRate(lep2id, lep2ccpt, lep2eta, ht) : fakeRate_2016(lep2id, lep2ccpt, lep2eta, ht);
                        class6Fake = true;
                        weight *= fr / (1-fr);
                    }
                    if (ss::lep1_fo() and !ss::lep1_tight() and !lep1_lowpt_veto and lep2good and lep3good) {  // lep1 fake
                        float fr = (year == 2017) ? fakeRate(lep1id, lep1ccpt, lep1eta, ht) : fakeRate_2016(lep1id, lep1ccpt, lep1eta, ht);
                        class6Fake = true;
                        weight *= fr / (1-fr);
                    }
                    if (!class6Fake) {
                        continue; // No fakes!
                    }
                } else if (hyp_class == 1 or hyp_class == 2) {
                    if (ss::lep1_passes_id()==0) {
                        float fr = (year == 2017) ? fakeRate(lep1id, lep1ccpt, lep1eta, ht) : fakeRate_2016(lep1id, lep1ccpt, lep1eta, ht);
                        weight *= fr/(1.-fr);
                    }
                    if (ss::lep2_passes_id()==0) {
                        float fr = (year == 2017) ? fakeRate(lep2id, lep2ccpt, lep2eta, ht) : fakeRate_2016(lep2id, lep2ccpt, lep2eta, ht);
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
                auto yearly_flip_rate = [&](float pt, float eta) {
                    if (is2016) return flipRate_2016(pt, eta);
                    if (is2017) return flipRate(pt, eta);
                    if (is2018) return flipRate(pt, eta);  // Just use 2017 for now
                    return 0.0f;
                };
                if (hyp_class == 4) hyp_class = 3; // we've flipped an OS to a SS
                else if (hyp_class == 6) class6Fake = true;
                else continue;
                float flipFact = 0.;
                if (abs(lep1id) == 11) {
                    float flr = (year == 2017) ? flipRate(lep1pt, lep1eta) : flipRate_2016(lep1pt, lep1eta);
                    flipFact += (flr/(1-flr));
                }
                if (abs(lep2id) == 11) {
                    float flr = (year == 2017) ? flipRate(lep2pt, lep2eta) : flipRate_2016(lep2pt, lep2eta);
                    flipFact += (flr/(1-flr));
                }
                weight *= flipFact;
                if (weight == 0.0) continue; // just quit if there are no flips.
            }

            // if all 3 charges are the same, throw the event away
            if (nleps > 2 and ((lep1id>0 and lep2id>0 and lep3id>0) or
                               (lep1id<0 and lep2id<0 and lep3id<0))) continue;

            auto mll = [](const Vec4& p1, const Vec4& p2, float ccpt1=-1, float ccpt2=-1) {
                /* Calculate dilepton mass with optional rescaling based on cone-corrected lepton pt */
                if (ccpt1 == -1) return (p1 + p2).M();
                else             return (p1*ccpt1/p1.pt() + p2*ccpt2/p2.pt()).M();
            };
            float m12 = mll(ss::lep1_p4(), ss::lep2_p4(), lep1ccpt, lep2ccpt);
            float m13 = mll(ss::lep1_p4(), ss::lep3_p4(), lep1ccpt, lep3ccpt);
            float m23 = mll(ss::lep2_p4(), ss::lep3_p4(), lep2ccpt, lep3ccpt);

            auto z_cand = [](int id1, int id2, float mll) {
                return abs(id1) == abs(id2) and  // Same flavor
                       id1*id2<0 and             // Opposite sign
                       abs(mll - 91.2) < 15;     // Z-mass window
            };
            bool zcand12 = z_cand(lep1id, lep2id, m12);
            bool zcand13 = z_cand(lep1id, lep3id, m13);
            bool zcand23 = z_cand(lep2id, lep3id, m23);
            float mllos = fabs(m13 - 91.2) < fabs(m23 - 91.2) ? m13 : m23;

            float event_bdt = (evaluateBDT) ? reader.EvaluateMVA("BDT") : 0;

            cout << "Made it to region definitions" << endl;
            auto fill_region = [&](const string& region, float weight) {
                cout << "Filling Region: " << region << endl;
                // Fill all observables for a region
                auto do_fill = [region, lep1id, lep2id, weight](HistCol& h, float val) {
                    h.Fill(region, lep1id, lep2id, val, weight);
                };
                do_fill(h_met, met);
                do_fill(h_metphi, metphi);
                do_fill(h_rawmet, rawmet);
                do_fill(h_calomet, calomet);
                do_fill(h_ht, ht);
                do_fill(h_htb, htb);
                do_fill(h_mll, ss::dilep_p4().M());
                if (nleps > 2) do_fill(h_zmll, mllos);
                do_fill(h_mtmin, ss::mtmin());
                do_fill(h_njets, njets);
                do_fill(h_nisrjets, nisrjets);
                do_fill(h_nisrmatch, nisrmatch);
                do_fill(h_nlb40, nlb40);
                do_fill(h_ntb40, ntb40);
                do_fill(h_nbtags, nbtags);
                do_fill(h_maxmjoverpt, maxmjoverpt);

                do_fill(h_pt1, lep1ccpt);
                do_fill(h_pt2, lep2ccpt);
                if (nleps > 2) do_fill(h_pt3, lep3pt);
                do_fill(abs(lep1id) == 11 ? h_pte      : h_ptm,      lep1ccpt);
                do_fill(abs(lep2id) == 11 ? h_pte      : h_ptm,      lep2ccpt);

                do_fill(h_eta1,   ss::lep1_p4().eta());
                do_fill(h_eta2,   ss::lep2_p4().eta());
                do_fill(abs(lep1id) == 11 ? h_etae     : h_etam,     lep1eta);
                do_fill(abs(lep2id) == 11 ? h_etae     : h_etam,     lep2eta);

                if (abs(lep1id) == 11) do_fill(h_nmiss1, nmiss1);
                if (abs(lep2id) == 11) do_fill(h_nmiss2, nmiss2);

                do_fill(h_ptrel1, lep1ptrel);
                do_fill(h_ptrel2, lep2ptrel);
                do_fill(abs(lep1id) == 11 ? h_ptrele   : h_ptrelm,   lep1ptrel);
                do_fill(abs(lep2id) == 11 ? h_ptrele   : h_ptrelm,   lep2ptrel);

                bool lep1_fail_ptratio = false;
                bool lep2_fail_ptratio = false;

                if (year == 2017) {
                    lep1_fail_ptratio = (lep1ptratio >= 0.) && (lep1ptratio < (abs(lep1id) == 11 ? 0.85 : 0.80));
                    lep2_fail_ptratio = (lep2ptratio >= 0.) && (lep2ptratio < (abs(lep2id) == 11 ? 0.85 : 0.80));
                } else {
                    lep1_fail_ptratio = (lep1ptratio >= 0.) && (lep1ptratio < (abs(lep1id) == 11 ? 0.80 : 0.76));
                    lep2_fail_ptratio = (lep2ptratio >= 0.) && (lep2ptratio < (abs(lep2id) == 11 ? 0.80 : 0.76));
                }

                if (lep1_fail_ptratio) do_fill(h_ptrelfail1, lep1ptrel);
                if (lep2_fail_ptratio) do_fill(h_ptrelfail2, lep2ptrel);
                if (lep1_fail_ptratio) do_fill(abs(lep1id) == 11 ? h_ptrelfaile   : h_ptrelfailm,   lep1ptrel);
                if (lep2_fail_ptratio) do_fill(abs(lep2id) == 11 ? h_ptrelfaile   : h_ptrelfailm,   lep2ptrel);

                do_fill(h_ptratio1, lep1ptratio);
                do_fill(h_ptratio2, lep2ptratio);
                do_fill(abs(lep1id) == 11 ? h_ptratioe : h_ptratiom, lep1ptratio);
                do_fill(abs(lep2id) == 11 ? h_ptratioe : h_ptratiom, lep2ptratio);

                do_fill(h_miniiso1, lep1miniiso);
                do_fill(h_miniiso2, lep2miniiso);
                do_fill(abs(lep1id) == 11 ? h_miniisoe : h_miniisom, lep1miniiso);
                do_fill(abs(lep2id) == 11 ? h_miniisoe : h_miniisom, lep2miniiso);

                do_fill(h_dphil1l2, lep1phi - lep2phi);
                do_fill(h_detal1l2, lep1eta - lep2eta);
                do_fill(h_absdetal1l2, abs(lep1eta - lep2eta));

                do_fill(h_matchtype,   matchtype);

                int type = ss::hyp_type();
                do_fill(h_type,   type>1 ? type-1 : type);
                do_fill(h_q1,   lep1id>0 ? -1 : 1);
                if (nleps > 2) {
                    if (abs(lep1id) + abs(lep2id) + abs(lep3id) == 39) do_fill(h_type3l, 0); // mu mu mu
                    if (abs(lep1id) + abs(lep2id) + abs(lep3id) == 37) do_fill(h_type3l, 1); // mu mu e
                    if (abs(lep1id) + abs(lep2id) + abs(lep3id) == 35) do_fill(h_type3l, 2); // mu e e
                    if (abs(lep1id) + abs(lep2id) + abs(lep3id) == 33) do_fill(h_type3l, 3); // e e e
                }
                do_fill(h_nvtx,   ss::nGoodVertices());

                if (njets >= 1) do_fill(h_ptj1, ptj1);
                if (njets >= 2) do_fill(h_ptj2, ptj2);
                if (njets >= 3) do_fill(h_ptj3, ptj3);
                if (njets >= 4) do_fill(h_ptj4, ptj4);
                if (njets >= 5) do_fill(h_ptj5, ptj5);
                if (njets >= 6) do_fill(h_ptj6, ptj6);
                if (njets >= 7) do_fill(h_ptj7, ptj7);
                if (njets >= 8) do_fill(h_ptj8, ptj8);

                if (njets >= 1) do_fill(h_ml1j1, ml1j1);

                do_fill(h_event_bdt, event_bdt);
            };

            bool BR_lite = ht > 300       and njets >= 2 and
                           nbtags >= 2    and met >= 50;
            bool BR = BR_lite and hyp_class == 3;
            bool CRW = BR and nleps == 2 and nbtags == 2 and njets <= 5;
            bool CRZ = BR_lite and hyp_class == 6 and (zcand13 or zcand23) and lep3ccpt > 20 and
                       (class6Fake or (lep1good and lep2good and lep3good));

            if (CRZ) fill_region("crz", weight);

            if (CRW) fill_region("crw", weight);

            if (lep1ccpt > 25. and
                    lep2ccpt > 20.) {
                if (hyp_class == 4) {
                    fill_region("osloose", weight);
                }
            }

            if (lep1ccpt > 25. and
                    lep2ccpt > 20. and
                    njets >= 2 and
                    met > 50. and
                    ht > 300) {
                if (hyp_class == 4 and !zcand12) {
                    fill_region("os_noisr", weight / isr_reweight(proc, nisrmatch));
                    fill_region("os", weight);
                    fill_region("os_btagreweight", weight * btag_reweight(nbtags));
                    if (evaluateBDT) {
                        if (event_bdt > 0.21) {
                            fill_region("os_highbdt", weight);
                        } else {
                            fill_region("os_lowbdt", weight);
                        }
                    }
                }
                if (hyp_class == 2) fill_region("tl", weight);
            }

            // BDT Validation Regions
            if (hyp_class == 3 and
                    njets >= 2) {
                if (nbtags == 1 and met > 50) fill_region("htnb1", weight);  // invert Nb
                if (nbtags < 2  and ht > 300 and met > 50) fill_region("bdt_nb", weight);  // invert Nb
                if (nbtags >= 2 and ht < 300 and met > 50) {                               // invert Ht
                    fill_region("bdt_ht", weight);
                    fill_region("bdt_met_ht", weight);
                }
                if (nbtags >= 2 and ht > 300 and met < 50) {                               // invert MET
                    fill_region("bdt_met", weight);
                    fill_region("bdt_met_ht", weight);
                }
            }

            bool BDT_train = hyp_class == 3 and
                             ht > 250       and njets >= 2    and
                             nbtags >= 1;
            if ((BDT_train and not BR) or CRW) fill_region("bdt_train", weight);

            if (hyp_class == 4 and !zcand12 and
                    nbtags == 2 and
                    njets >= 2) {
                fill_region("isr", weight / isr_reweight(proc, nisrmatch));
                fill_region("isr_reweight_check", weight);
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

