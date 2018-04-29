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
#include "../../../common/CORE/Tools/utils.h"
#include "../../../common/CORE/Tools/dorky/dorky.h"
#include "../../misc/common_utils.h"
#include "../../misc/signal_regions.h"

using namespace std;

bool overflow = true;

float getFlipFactor(TH2D* rate){

  float pt1  = ss::lep1_p4().pt();  
  float eta1 = fabs(ss::lep1_p4().eta()); 
  float pt2  = ss::lep2_p4().pt();  
  float eta2 = fabs(ss::lep2_p4().eta()); 

  if (pt1<rate->GetXaxis()->GetBinCenter(1)) pt1=rate->GetXaxis()->GetBinCenter(1);
  if (pt1>rate->GetXaxis()->GetBinCenter(rate->GetNbinsX())) pt1=rate->GetXaxis()->GetBinCenter(rate->GetNbinsX());
  if (eta1<rate->GetYaxis()->GetBinCenter(1)) eta1=rate->GetYaxis()->GetBinCenter(1);
  if (eta1>rate->GetYaxis()->GetBinCenter(rate->GetNbinsY())) eta1=rate->GetYaxis()->GetBinCenter(rate->GetNbinsY());

  if (pt2<rate->GetXaxis()->GetBinCenter(1)) pt2=rate->GetXaxis()->GetBinCenter(1);
  if (pt2>rate->GetXaxis()->GetBinCenter(rate->GetNbinsX())) pt2=rate->GetXaxis()->GetBinCenter(rate->GetNbinsX());
  if (eta2<rate->GetYaxis()->GetBinCenter(1)) eta2=rate->GetYaxis()->GetBinCenter(1);
  if (eta2>rate->GetYaxis()->GetBinCenter(rate->GetNbinsY())) eta2=rate->GetYaxis()->GetBinCenter(rate->GetNbinsY());

  int bin1 = rate->FindBin(pt1, eta1);
  int bin2 = rate->FindBin(pt2, eta2);
  float FR1 = rate->GetBinContent(bin1);
  float FR2 = rate->GetBinContent(bin2);

  return (FR1/(1.-FR1) + FR2/(1.-FR2)); 
}

void closure(TChain *ch, TString flipfname, TString outname="outputs/histos.root", float minrun=-1, float maxrun=-1, float lumi=-1) {

    float lumiAG = getLumi();
    if (lumi > 0) lumiAG = lumi;
    duplicate_removal::clear_list();

    TFile *flip_file = new TFile(flipfname); 
    TH2D  *rate = (TH2D*)flip_file->Get("ratio"); 

    // Closure vs. inv mass plot
    constexpr int clos_mll_nBinsX = 9;
    TH1F* clos_mll_MC   = new TH1F("clos_mll_plot_MC"  , "clos_mll_plot_MC"  , clos_mll_nBinsX, 70, 115); 
    TH1F* clos_mll_MCp  = new TH1F("clos_mll_plot_MCp" , "clos_mll_plot_MCp" , clos_mll_nBinsX, 70, 115); 
    TH1F* clos_mll_data = new TH1F("clos_mll_plot_data", "clos_mll_plot_data", clos_mll_nBinsX, 70, 115); 
    TH1F* osee_mll_data = new TH1F("osee_mll_plot_data", "osee_mll_plot_data", 45, 70, 115); 
    TH1F* osee_mll_MC   = new TH1F("osee_mll_plot_MC"  , "osee_mll_plot_MC"  , 45, 70, 115); 
    clos_mll_MC->Sumw2();
    clos_mll_MCp->Sumw2();

    TH1F* clos_leppt_MC   = new TH1F("clos_leppt_plot_MC"  , "clos_leppt_plot_MC"  , 20, 0, 100); 
    TH1F* clos_leppt_MCp  = new TH1F("clos_leppt_plot_MCp" , "clos_leppt_plot_MCp" , 20, 0, 100); 
    TH1F* clos_leppt_data = new TH1F("clos_leppt_plot_data", "clos_leppt_plot_data", 20, 0, 100); 
    TH1F* osee_leppt_MC   = new TH1F("osee_leppt_plot_MC"  , "osee_leppt_plot_MC"  , 20, 0, 100); 
    TH1F* osee_leppt_data = new TH1F("osee_leppt_plot_data", "osee_leppt_plot_data", 20, 0, 100); 
    clos_leppt_MC->Sumw2();
    clos_leppt_MCp->Sumw2();

    TH1F* clos_lepeta_MC   = new TH1F("clos_lepeta_plot_MC"  , "clos_lepeta_plot_MC"  , 25, -2.5, 2.5); 
    TH1F* clos_lepeta_MCp  = new TH1F("clos_lepeta_plot_MCp" , "clos_lepeta_plot_MCp" , 25, -2.5, 2.5); 
    TH1F* clos_lepeta_data = new TH1F("clos_lepeta_plot_data", "clos_lepeta_plot_data", 25, -2.5, 2.5); 
    TH1F* osee_lepeta_MC   = new TH1F("osee_lepeta_plot_MC"  , "osee_lepeta_plot_MC"  , 25, -2.5, 2.5); 
    TH1F* osee_lepeta_data = new TH1F("osee_lepeta_plot_data", "osee_lepeta_plot_data", 25, -2.5, 2.5); 
    clos_lepeta_MC->Sumw2();
    clos_lepeta_MCp->Sumw2();

    TH1F* clos_lepphi_MC   = new TH1F("clos_lepphi_plot_MC"  , "clos_lepphi_plot_MC"  , 30, -3.14, 3.14); 
    TH1F* clos_lepphi_MCp  = new TH1F("clos_lepphi_plot_MCp" , "clos_lepphi_plot_MCp" , 30, -3.14, 3.14); 
    TH1F* clos_lepphi_data = new TH1F("clos_lepphi_plot_data", "clos_lepphi_plot_data", 30, -3.14, 3.14); 
    TH1F* osee_lepphi_MC   = new TH1F("osee_lepphi_plot_MC"  , "osee_lepphi_plot_MC"  , 30, -3.14, 3.14); 
    TH1F* osee_lepphi_data = new TH1F("osee_lepphi_plot_data", "osee_lepphi_plot_data", 30, -3.14, 3.14); 
    clos_lepphi_MC->Sumw2();
    clos_lepphi_MCp->Sumw2();

    TH1F* clos_ht_MC   = new TH1F("clos_ht_plot_MC"  , "clos_ht_plot_MC"  , 15, 0, 300); 
    TH1F* clos_ht_MCp  = new TH1F("clos_ht_plot_MCp" , "clos_ht_plot_MCp" , 15, 0, 300); 
    TH1F* clos_ht_data = new TH1F("clos_ht_plot_data", "clos_ht_plot_data", 15, 0, 300); 
    TH1F* osee_ht_MC   = new TH1F("osee_ht_plot_MC"  , "osee_ht_plot_MC"  , 15, 0, 300); 
    TH1F* osee_ht_data = new TH1F("osee_ht_plot_data", "osee_ht_plot_data", 15, 0, 300); 
    clos_ht_MC->Sumw2();
    clos_ht_MCp->Sumw2();

    TH1F* clos_met_MC   = new TH1F("clos_met_plot_MC"  , "clos_met_plot_MC"  , 15, 0, 150); 
    TH1F* clos_met_MCp  = new TH1F("clos_met_plot_MCp" , "clos_met_plot_MCp" , 15, 0, 150); 
    TH1F* clos_met_data = new TH1F("clos_met_plot_data", "clos_met_plot_data", 15, 0, 150); 
    TH1F* osee_met_MC   = new TH1F("osee_met_plot_MC"  , "osee_met_plot_MC"  , 15, 0, 150); 
    TH1F* osee_met_data = new TH1F("osee_met_plot_data", "osee_met_plot_data", 15, 0, 150); 
    clos_met_MC->Sumw2();
    clos_met_MCp->Sumw2();

    TH1F* clos_njets_MC   = new TH1F("clos_njets_plot_MC"  , "clos_njets_plot_MC"  , 6, 0, 6); 
    TH1F* clos_njets_MCp  = new TH1F("clos_njets_plot_MCp" , "clos_njets_plot_MCp" , 6, 0, 6); 
    TH1F* clos_njets_data = new TH1F("clos_njets_plot_data", "clos_njets_plot_data", 6, 0, 6); 
    TH1F* osee_njets_MC   = new TH1F("osee_njets_plot_MC"  , "osee_njets_plot_MC"  , 6, 0, 6); 
    TH1F* osee_njets_data = new TH1F("osee_njets_plot_data", "osee_njets_plot_data", 6, 0, 6); 
    clos_njets_MC->Sumw2();
    clos_njets_MCp->Sumw2();

    TH1F* clos_nbtags_MC   = new TH1F("clos_nbtags_plot_MC"  , "clos_nbtags_plot_MC"  , 6, 0, 6); 
    TH1F* clos_nbtags_MCp  = new TH1F("clos_nbtags_plot_MCp" , "clos_nbtags_plot_MCp" , 6, 0, 6); 
    TH1F* clos_nbtags_data = new TH1F("clos_nbtags_plot_data", "clos_nbtags_plot_data", 6, 0, 6); 
    TH1F* osee_nbtags_MC   = new TH1F("osee_nbtags_plot_MC"  , "osee_nbtags_plot_MC"  , 6, 0, 6); 
    TH1F* osee_nbtags_data = new TH1F("osee_nbtags_plot_data", "osee_nbtags_plot_data", 6, 0, 6); 
    clos_nbtags_MC->Sumw2();
    clos_nbtags_MCp->Sumw2();

    TH1F* clos_nvtx_MC   = new TH1F("clos_nvtx_plot_MC"  , "clos_nvtx_plot_MC"  , 80, 0, 80); 
    TH1F* clos_nvtx_MCp  = new TH1F("clos_nvtx_plot_MCp" , "clos_nvtx_plot_MCp" , 80, 0, 80); 
    TH1F* clos_nvtx_data = new TH1F("clos_nvtx_plot_data", "clos_nvtx_plot_data", 80, 0, 80); 
    TH1F* osee_nvtx_MC   = new TH1F("osee_nvtx_plot_MC"  , "osee_nvtx_plot_MC"  , 80, 0, 80); 
    TH1F* osee_nvtx_data = new TH1F("osee_nvtx_plot_data", "osee_nvtx_plot_data", 80, 0, 80); 
    clos_nvtx_MC->Sumw2();
    clos_nvtx_MCp->Sumw2();

    // Errors -- keep track of number of events in each FR bin so we can get the error 
    int nBinsX = rate->GetNbinsX(); 
    int nBinsY = rate->GetNbinsY(); 
    const TArrayD* xbins = rate->GetXaxis()->GetXbins();
    const TArrayD* ybins = rate->GetYaxis()->GetXbins();
    TH2D* errors[clos_mll_nBinsX] = { 0 };
    for (unsigned int i = 0; i < clos_mll_nBinsX; i++){
        errors[i] = new TH2D(Form("errors_%i", i) , "errors" , nBinsX, xbins->GetArray(), nBinsY, ybins->GetArray());
    }

    // Number duplicates rejected
    int reject = 0;

    // Results
    float nObs   = 0;
    float nObsMC = 0;
    float nPred  = 0;

    TH1F* mcCounterSS = new TH1F("mccounterss","mccounterss", 1, 0, 1);
    TH1F* mcCounterOS = new TH1F("mccounteros","mccounteros", 1, 0, 1);
    TH1F* mcCounterOSRW = new TH1F("mccounterosrw","mccounterosrw", 1, 0, 1);

    TH1F* dataCounterSS = new TH1F("datacounterss","datacounterss", 1, 0, 1);
    TH1F* dataCounterOS = new TH1F("datacounteros","datacounteros", 1, 0, 1);
    TH1F* dataCounterOSRW = new TH1F("datacounterosrw","datacounterosrw", 1, 0, 1);

    TH1F* mcCounterSS_bin = new TH1F("mccounterss_bin","mccounterss_bin", 1, 0, 1);
    TH1F* mcCounterOS_bin = new TH1F("mccounteros_bin","mccounteros_bin", 1, 0, 1);
    TH1F* mcCounterOSRW_bin = new TH1F("mccounterosrw_bin","mccounterosrw_bin", 1, 0, 1);

    TH1F* dataCounterSS_bin = new TH1F("datacounterss_bin","datacounterss_bin", 1, 0, 1);
    TH1F* dataCounterOS_bin = new TH1F("datacounteros_bin","datacounteros_bin", 1, 0, 1);
    TH1F* dataCounterOSRW_bin = new TH1F("datacounterosrw_bin","datacounterosrw_bin", 1, 0, 1);

    // Pred err variables
    float stat2 = 0; 
    float fr_err2 = 0; 


    // Set up chains
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

            SSAG::progress(nEventsTotal, nEventsChain);

            if (!ss::fired_trigger()) continue;
            if (ss::hyp_class() != 3 && ss::hyp_class() != 4) continue;
            if (ss::met() > 50.) continue; 
            if (abs(ss::lep1_id()) != 11 || abs(ss::lep2_id()) != 11) continue;
            if(ss::nbtags() > 0) continue;
            
            // Figure out if SS
            bool isSS = false;
            if (ss::hyp_class() == 3) isSS = true;


            //Calculate weight
            float weight = ss::is_real_data() ? 1 : ss::scale1fb()*lumiAG;

            if (!ss::is_real_data()) {
                weight *= getTruePUw(ss::trueNumInt()[0]);
                weight *= leptonScaleFactor(ss::lep1_id(), ss::lep1_p4().pt(), ss::lep1_p4().eta(), ss::ht(), -1.);
                weight *= leptonScaleFactor(ss::lep2_id(), ss::lep2_p4().pt(), ss::lep2_p4().eta(), ss::ht(), -1.);
                weight *= ss::weight_btagsf();
                // weight *= eventScaleFactor(ss::lep1_id(), ss::lep2_id(), ss::lep1_p4().pt(), ss::lep2_p4().pt(), ss::lep1_p4().eta(), ss::lep2_p4().eta(), ss::ht());
            }

            // Reject duplicates
            if (ss::is_real_data()){
                duplicate_removal::DorkyEventIdentifier id(ss::run(), ss::event(), ss::lumi());
                if (duplicate_removal::is_duplicate(id)) { reject++; continue; }

                if (minrun > 0 && ss::run() < minrun) continue;
                if (maxrun > 0 && ss::run() > maxrun) continue;
            }

            if (!ss::lep1_passes_id() || !ss::lep2_passes_id()) continue;

            // Observation
            if (isSS) {
                float mll = (ss::lep1_p4() + ss::lep2_p4()).M();
                if (ss::is_real_data()) {
                    clos_mll_data->Fill(mll, weight); 
                    if (mll > 80 && mll < 100) {
                        nObs += weight; 
                        if (fabs(ss::lep1_p4().eta()) < 0.8 && fabs(ss::lep2_p4().eta()) < 0.8 && ss::lep1_p4().pt() < 60 && ss::lep2_p4().pt() < 60) {
                            dataCounterSS_bin->Fill(0.5, weight);
                        }
                        clos_leppt_data->Fill(ss::lep1_p4().pt(), weight); 
                        clos_leppt_data->Fill(ss::lep2_p4().pt(), weight); 
                        clos_lepeta_data->Fill(ss::lep1_p4().eta(), weight); 
                        clos_lepeta_data->Fill(ss::lep2_p4().eta(), weight);
                        clos_lepphi_data->Fill(ss::lep1_p4().phi(), weight); 
                        clos_lepphi_data->Fill(ss::lep2_p4().phi(), weight);  
                        clos_ht_data->Fill(ss::ht(), weight); 
                        clos_met_data->Fill(ss::met(), weight); 
                        clos_njets_data->Fill(ss::njets(), weight); 
                        clos_nbtags_data->Fill(ss::nbtags(), weight); 
                        clos_nvtx_data->Fill(ss::nGoodVertices(), weight); 
                    }
                } else {
                    clos_mll_MCp->Fill(mll, weight); 
                    if (mll > 80 && mll < 100) {
                        nObsMC += weight; 
                        mcCounterSS->Fill(0.5, weight);
                        if (fabs(ss::lep1_p4().eta()) < 0.8 && fabs(ss::lep2_p4().eta()) < 0.8 && ss::lep1_p4().pt() < 60 && ss::lep2_p4().pt() < 60) {
                            mcCounterSS_bin->Fill(0.5, weight);
                        }
                        clos_leppt_MCp->Fill(ss::lep1_p4().pt(), weight); 
                        clos_leppt_MCp->Fill(ss::lep2_p4().pt(), weight); 
                        clos_lepeta_MCp->Fill(ss::lep1_p4().eta(), weight); 
                        clos_lepeta_MCp->Fill(ss::lep2_p4().eta(), weight); 
                        clos_lepphi_MCp->Fill(ss::lep1_p4().phi(), weight); 
                        clos_lepphi_MCp->Fill(ss::lep2_p4().phi(), weight); 
                        clos_ht_MCp->Fill(ss::ht(), weight); 
                        clos_met_MCp->Fill(ss::met(), weight); 
                        clos_njets_MCp->Fill(ss::njets(), weight); 
                        clos_nbtags_MCp->Fill(ss::nbtags(), weight); 
                        clos_nvtx_MCp->Fill(ss::nGoodVertices(), weight); 
                    }
                }
            } 

            // Prediction
            if (!isSS && ss::is_real_data()) {
                float pt1  = ss::lep1_p4().pt();  
                float eta1 = fabs(ss::lep1_p4().eta()); 
                float pt2  = ss::lep2_p4().pt();  
                float eta2 = fabs(ss::lep2_p4().eta()); 
                if (pt1<rate->GetXaxis()->GetBinCenter(1)) pt1=rate->GetXaxis()->GetBinCenter(1);
                if (pt1>rate->GetXaxis()->GetBinCenter(rate->GetNbinsX())) pt1=rate->GetXaxis()->GetBinCenter(rate->GetNbinsX());
                if (eta1<rate->GetYaxis()->GetBinCenter(1)) eta1=rate->GetYaxis()->GetBinCenter(1);
                if (eta1>rate->GetYaxis()->GetBinCenter(rate->GetNbinsY())) eta1=rate->GetYaxis()->GetBinCenter(rate->GetNbinsY());	  
                if (pt2<rate->GetXaxis()->GetBinCenter(1)) pt2=rate->GetXaxis()->GetBinCenter(1);
                if (pt2>rate->GetXaxis()->GetBinCenter(rate->GetNbinsX())) pt2=rate->GetXaxis()->GetBinCenter(rate->GetNbinsX());
                if (eta2<rate->GetYaxis()->GetBinCenter(1)) eta2=rate->GetYaxis()->GetBinCenter(1);
                if (eta2>rate->GetYaxis()->GetBinCenter(rate->GetNbinsY())) eta2=rate->GetYaxis()->GetBinCenter(rate->GetNbinsY());

                float ff = getFlipFactor(rate);
                float mll = (ss::lep1_p4() + ss::lep2_p4()).M();
                clos_mll_MC->Fill(mll, ff*weight); 


                int bin_in_errors = clos_mll_MC->FindBin(mll);
                if (bin_in_errors >= clos_mll_nBinsX) overflow ? bin_in_errors = clos_mll_nBinsX-1 : bin_in_errors = 999; 
                if (bin_in_errors < 0) overflow ? bin_in_errors = 0 : bin_in_errors = 999;
                if (bin_in_errors != 999) errors[bin_in_errors]->Fill(pt1, eta1, weight);
                if (bin_in_errors != 999) errors[bin_in_errors]->Fill(pt2, eta2, weight);
                if (mll > 80 && mll < 100) {

                    osee_mll_data->Fill(mll, weight);
                    osee_leppt_data->Fill(ss::lep1_p4().pt(), weight); 
                    osee_leppt_data->Fill(ss::lep2_p4().pt(), weight); 
                    osee_lepeta_data->Fill(ss::lep1_p4().eta(), weight); 
                    osee_lepeta_data->Fill(ss::lep2_p4().eta(), weight); 
                    osee_lepphi_data->Fill(ss::lep1_p4().phi(), weight); 
                    osee_lepphi_data->Fill(ss::lep2_p4().phi(), weight); 
                    osee_ht_data->Fill(ss::ht(), weight); 
                    osee_met_data->Fill(ss::met(), weight); 
                    osee_njets_data->Fill(ss::njets(), weight); 
                    osee_nbtags_data->Fill(ss::nbtags(), weight); 
                    osee_nvtx_data->Fill(ss::nGoodVertices(), weight); 

                    nPred += ff*weight;
                    clos_leppt_MC->Fill(ss::lep1_p4().pt(), ff*weight); 
                    clos_leppt_MC->Fill(ss::lep2_p4().pt(), ff*weight); 
                    clos_lepeta_MC->Fill(ss::lep1_p4().eta(), ff*weight); 
                    clos_lepeta_MC->Fill(ss::lep2_p4().eta(), ff*weight); 
                    clos_lepphi_MC->Fill(ss::lep1_p4().phi(), ff*weight); 
                    clos_lepphi_MC->Fill(ss::lep2_p4().phi(), ff*weight); 
                    clos_ht_MC->Fill(ss::ht(), ff*weight); 
                    clos_met_MC->Fill(ss::met(), ff*weight); 
                    clos_njets_MC->Fill(ss::njets(), ff*weight); 
                    clos_nbtags_MC->Fill(ss::nbtags(), ff*weight); 
                    clos_nvtx_MC->Fill(ss::nGoodVertices(), ff*weight); 

                    dataCounterOS->Fill(0.5, weight);
                    if (eta1 < 0.8 && eta2 < 0.8 && pt1 < 60 && pt2 < 60) {
                        dataCounterOS_bin->Fill(0.5, weight);
                        dataCounterOSRW_bin->Fill(0.5, ff*weight);
                    }

                }
            }

            if (!isSS && !(ss::is_real_data())){
                float pt1  = ss::lep1_p4().pt();  
                float eta1 = fabs(ss::lep1_p4().eta()); 
                float pt2  = ss::lep2_p4().pt();  
                float eta2 = fabs(ss::lep2_p4().eta()); 
                if (pt1<rate->GetXaxis()->GetBinCenter(1)) pt1=rate->GetXaxis()->GetBinCenter(1);
                if (pt1>rate->GetXaxis()->GetBinCenter(rate->GetNbinsX())) pt1=rate->GetXaxis()->GetBinCenter(rate->GetNbinsX());
                if (eta1<rate->GetYaxis()->GetBinCenter(1)) eta1=rate->GetYaxis()->GetBinCenter(1);
                if (eta1>rate->GetYaxis()->GetBinCenter(rate->GetNbinsY())) eta1=rate->GetYaxis()->GetBinCenter(rate->GetNbinsY());	  
                if (pt2<rate->GetXaxis()->GetBinCenter(1)) pt2=rate->GetXaxis()->GetBinCenter(1);
                if (pt2>rate->GetXaxis()->GetBinCenter(rate->GetNbinsX())) pt2=rate->GetXaxis()->GetBinCenter(rate->GetNbinsX());
                if (eta2<rate->GetYaxis()->GetBinCenter(1)) eta2=rate->GetYaxis()->GetBinCenter(1);
                if (eta2>rate->GetYaxis()->GetBinCenter(rate->GetNbinsY())) eta2=rate->GetYaxis()->GetBinCenter(rate->GetNbinsY());
                float ff = getFlipFactor(rate);
                float mll = (ss::lep1_p4() + ss::lep2_p4()).M();


                if (mll > 80 && mll < 100) {

                    osee_mll_MC->Fill(mll, weight);
                    osee_leppt_MC->Fill(ss::lep1_p4().pt(), weight); 
                    osee_leppt_MC->Fill(ss::lep2_p4().pt(), weight); 
                    osee_lepeta_MC->Fill(ss::lep1_p4().eta(), weight); 
                    osee_lepeta_MC->Fill(ss::lep2_p4().eta(), weight); 
                    osee_lepphi_MC->Fill(ss::lep1_p4().phi(), weight); 
                    osee_lepphi_MC->Fill(ss::lep2_p4().phi(), weight); 
                    osee_ht_MC->Fill(ss::ht(), weight); 
                    osee_met_MC->Fill(ss::met(), weight); 
                    osee_njets_MC->Fill(ss::njets(), weight); 
                    osee_nbtags_MC->Fill(ss::nbtags(), weight); 
                    osee_nvtx_MC->Fill(ss::nGoodVertices(), weight); 

                    mcCounterOSRW->Fill(0.5, ff*weight);
                    mcCounterOS->Fill(0.5, weight);
                    if (eta1 < 0.8 && eta2 < 0.8 && pt1 < 60 && pt2 < 60) {
                        mcCounterOS_bin->Fill(0.5, weight);
                        mcCounterOSRW_bin->Fill(0.5, ff*weight);
                    }
                }
            }

        }//event loop

        delete file;
    }//file loop

    cout << "number of duplicates removed: " << reject << endl;

    //Figure out error from "errors" plot:
    for (int i = 1; i <= clos_mll_nBinsX; i++) stat2 += pow(clos_mll_MC->GetBinError(i), 2);  
    float theerror[clos_mll_nBinsX] = { 0 } ;
    for (int k = 0; k < clos_mll_nBinsX; k++){
        //cout << endl << clos_mll_MC->GetBinContent(k+1) << endl;
        float yield = 0.;
        float error = 0; 
        for (int i = 1; i <= errors[0]->GetNbinsX(); i++){
            for (int j = 1; j <= errors[0]->GetNbinsY(); j++){
                float FR_val = rate->GetBinContent(i, j);
                float FR_err = rate->GetBinError(i, j);
                float nEntries = errors[k]->GetBinContent(i, j); 
                error = FR_err*nEntries/pow(1-FR_val,2);
                //cout << "FR_err[%]=" << 100*FR_err/FR_val << " FR_val=" << FR_val << " nEntries=" << nEntries << endl;
                theerror[k] += pow(error, 2);
                //cout << nEntries*FR_val/(1.-FR_val) << " +/- " << error << endl;
                yield+=(nEntries*FR_val/(1.-FR_val));
                fr_err2 += pow(error, 2); 
            }
        }
        //clos_mll_MC->SetBinError(k+1, sqrt(theerror[k] + pow(clos_mll_MC->GetBinError(k+1),2)));
        //cout << "set error = " <<  sqrt(theerror[k] + pow(clos_mll_MC->GetBinError(k+1),2)) << " yield=" << yield << endl;
    }
    cout << "HEY DUMMY. MAKE SURE YOU'RE NOT SCALING BY obs/pred TWICE!" << endl;
    std::cout <<  " mcCounterSS->GetBinContent(1): " << mcCounterSS->GetBinContent(1) <<  " mcCounterSS->GetBinError(1): " << mcCounterSS->GetBinError(1) <<  std::endl;
    std::cout <<  " mcCounterOSRW->GetBinContent(1): " << mcCounterOSRW->GetBinContent(1) <<  " mcCounterOSRW->GetBinError(1): " << mcCounterOSRW->GetBinError(1) <<  std::endl;
    cout << "obsMC: " << nObsMC << endl;
    cout << " pred: " << nPred  << " pm " << sqrt(stat2 + fr_err2) << endl;
    cout << "  obs: " << nObs   << " pm " << sqrt(nObs) << endl;

    //Split it
    cout << "pred stat: " << sqrt(stat2) << endl;
    cout << "pred syst: " << sqrt(fr_err2) << endl;

    cout << "GREP " << nObs << " " << nPred << " " << sqrt(stat2 + fr_err2) << " "
        << nObsMC << " " << mcCounterSS->GetBinError(1) << " "
        << mcCounterOSRW->GetBinContent(1) << " " << mcCounterOSRW->GetBinError(1) << " "
        << mcCounterSS_bin->GetBinContent(1) << " " << mcCounterOS_bin->GetBinContent(1) << " " << mcCounterOSRW_bin->GetBinContent(1) << " "
        << dataCounterSS_bin->GetBinContent(1) << " " << dataCounterOS_bin->GetBinContent(1) << " " << dataCounterOSRW_bin->GetBinContent(1) << " "
        << mcCounterOS->GetBinContent(1) << " " << dataCounterOS->GetBinContent(1) << " " 
        << std::endl;

    // // scale pred to obs
    // clos_mll_MC->Scale(clos_mll_data->Integral()/clos_mll_MC->Integral());
    // clos_leppt_MC->Scale(clos_leppt_data->Integral()/clos_leppt_MC->Integral());
    // clos_lepeta_MC->Scale(clos_lepeta_data->Integral()/clos_lepeta_MC->Integral());
    // clos_lepphi_MC->Scale(clos_lepphi_data->Integral()/clos_lepphi_MC->Integral());
    // clos_ht_MC->Scale(clos_ht_data->Integral()/clos_ht_MC->Integral());
    // clos_met_MC->Scale(clos_met_data->Integral()/clos_met_MC->Integral());
    // clos_njets_MC->Scale(clos_njets_data->Integral()/clos_njets_MC->Integral());
    // clos_nbtags_MC->Scale(clos_nbtags_data->Integral()/clos_nbtags_MC->Integral());

    TFile* f1 = new TFile(outname,"RECREATE");

    clos_mll_data->Write();
    clos_mll_MC->Write();
    clos_mll_MCp->Write();
    clos_leppt_data->Write();
    clos_leppt_MC->Write();
    clos_leppt_MCp->Write();
    clos_lepeta_data->Write();
    clos_lepeta_MC->Write();
    clos_lepeta_MCp->Write();
    clos_lepphi_data->Write();
    clos_lepphi_MC->Write();
    clos_lepphi_MCp->Write();
    clos_ht_data->Write();
    clos_ht_MC->Write();
    clos_ht_MCp->Write();
    clos_met_data->Write();
    clos_met_MC->Write();
    clos_met_MCp->Write();
    clos_njets_data->Write();
    clos_njets_MC->Write();
    clos_njets_MCp->Write();
    clos_nbtags_data->Write();
    clos_nbtags_MC->Write();
    clos_nbtags_MCp->Write();
    clos_nvtx_data->Write();
    clos_nvtx_MC->Write();
    clos_nvtx_MCp->Write();

    osee_mll_data->Write();
    osee_mll_MC->Write();
    osee_leppt_MC->Write();
    osee_leppt_data->Write();
    osee_lepeta_MC->Write();
    osee_lepeta_data->Write();
    osee_lepphi_MC->Write();
    osee_lepphi_data->Write();
    osee_ht_MC->Write();
    osee_ht_data->Write();
    osee_met_MC->Write();
    osee_met_data->Write();
    osee_njets_MC->Write();
    osee_njets_data->Write();
    osee_nbtags_MC->Write();
    osee_nbtags_data->Write();
    osee_nvtx_MC->Write();
    osee_nvtx_data->Write();

    f1->Close();


}

