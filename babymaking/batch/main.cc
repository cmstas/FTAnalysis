#include "./CORE/CMS3.h"
#include "./CORE/SSSelections.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "helper_babymaker.h"
#include "CORE/Tools/goodrun.h"
#include "CORE/Tools/JetCorrector.h"
#include "CORE/Tools/datasetinfo/getDatasetInfo.h"
#include "CORE/Tools/jetcorr/JetCorrectionUncertainty.h"  
#include "CORE/Tools/jetcorr/SimpleJetCorrectionUncertainty.h"

int main(int argc, char *argv[]){

    TString filename;
    TString outname = "output.root";
    unsigned int nevents_max = 0;
    // bool useXrootd = false;
    char hostnamestupid[100];
    int result = gethostname(hostnamestupid, 100);
    TString hostname(hostnamestupid);
    std::cout << ">>> Hostname is " << hostname << std::endl;  
    bool useXrootd = !(hostname.Contains("t2.ucsd.edu"));
    // bool useXrootd = false;
    if (hostname.Contains("uafino")) {
        std::cout << ">>> We're on uafino, so using xrootd!" << std::endl;  
        useXrootd = true;
    }
    string good_run_file = "goodRunList/goldenJson_2016rereco_36p46ifb.txt";
    string jecEra = "Summer16_23Sep2016BCDV3";
    std::string jecEraMC = "Summer16_23Sep2016V3";


    if (argc > 1) filename = TString(argv[1]);
    if (argc > 2) outname     = TString(argv[2]);
    if (argc > 3) nevents_max = atoi(argv[3]);

    std::cout << ">>> Args: " << std::endl;
    std::cout << "     filename:    " << filename << std::endl;
    std::cout << "     outname:     " << outname << std::endl;
    std::cout << "     nevents_max: " << nevents_max << std::endl;

    if (argc <= 1) { 
        std::cout << ">>> [!] Not enough arguments!" << std::endl;  
        return 0;
    }


    babyMaker *mylooper = new babyMaker();

    bool isData = filename.Contains("run2_data") || filename.Contains("Run2016") || filename.Contains("Run2017");
    int isSignal = 0; 

    if (filename.Contains("/SMS")) {
        if (filename.Contains("SMS-T1tttt_Tune")) isSignal = 1;
        else if (filename.Contains("SMS-T5qqqqVV_Tune")) isSignal = 2;
        else if (filename.Contains("SMS-T5qqqqVV_dM20_Tune")) isSignal = 3;
        else if (filename.Contains("SMS-T6ttWW_Tune")) isSignal = 10;
        else if (filename.Contains("SMS-T1ttbb_Tune")) isSignal = 6;
        else if (filename.Contains("SMS-T5tttt_dM175_Tune")) isSignal = 5;
        else if (filename.Contains("SMS-T5ttcc_Tune")) isSignal = 4;
        else {
            std::cout << ">>> [!] Can't figure out which signal sample this is!" << std::endl;
            return 0;
        }
    }
    if (filename.Contains("_HToTT_")) isSignal = 101; // isSignal > 100 used only for non SMS stuff

    //Set up file and tree
    mylooper->MakeBabyNtuple(outname.Data(), isSignal);

    TFile *f;
    if (useXrootd) {
        std::cout << ">>> Using xrootd" << std::endl;  
        TString nohadoop = filename.Copy();
        nohadoop.ReplaceAll("/hadoop/cms/", "/");
        f = TFile::Open("root://cmsxrootd.fnal.gov/"+nohadoop); 
    } else {
        f = TFile::Open(filename); 
    }
    if (!f) {
        std::cout << ">>> [!] File does not exist or is bad!" << std::endl;  
        return 0;
    }
    std::cout << ">>> File opened" << std::endl;  
    TTree *tree = (TTree*)f->Get("Events");
    cms3.Init(tree);

    //Event Counting
    unsigned int nEvents = tree->GetEntries();
    unsigned int nEventsTotal = 0;
    std::cout << ">>> Nevents: " << nEvents << std::endl;  

    //Add good run list
    set_goodrun_file(good_run_file.c_str());


    //Make Jet Energy Uncertainties
    JetCorrectionUncertainty *jecUnc = 0;
    // if ( isData)      jecUnc = new JetCorrectionUncertainty("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA_Uncertainty_AK4PFchs.txt"); 
    if (!isData)      jecUnc = new JetCorrectionUncertainty("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_Uncertainty_AK4PFchs.txt"); 
    if (isSignal > 0) jecUnc = new JetCorrectionUncertainty("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_Uncertainty_AK4PFchs.txt"); 

    //Init MVA
    // createAndInitMVA("./CORE", true, true); // ICHEP
    // createAndInitMVA("./CORE", true, false, 80); // Moriond
    createAndInitMVA("./CORE", true, true, 80); // Moriond // FIXME


    //JEC files -- 25 ns MC
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL1L2L3;
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L3Absolute_AK4PFchs.txt");

    //JEC files -- 25 ns DATA
    std::vector<std::string> jetcorr_filenames_25ns_DATA_pfL1L2L3;
    // jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA_L1FastJet_AK4PFchs.txt");
    // jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA_L2Relative_AK4PFchs.txt");
    // jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA_L3Absolute_AK4PFchs.txt");
    // jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA_L2L3Residual_AK4PFchs.txt");

    //JEC files -- 25 ns FASTSIM
    std::vector<std::string> jetcorr_filenames_25ns_FASTSIM_pfL1L2L3;
    jetcorr_filenames_25ns_FASTSIM_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_FASTSIM_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_L2Relative_AK4PFchs.txt"); 
    jetcorr_filenames_25ns_FASTSIM_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_L3Absolute_AK4PFchs.txt"); 

    //JECs
    std::vector<std::string> filenames;
    // if ( isData     ) filenames = jetcorr_filenames_25ns_DATA_pfL1L2L3;
    if (!isData     ) filenames = jetcorr_filenames_25ns_MC_pfL1L2L3;
    if (isSignal > 0) filenames = jetcorr_filenames_25ns_FASTSIM_pfL1L2L3;

    FactorizedJetCorrector *jetCorrAG;
    if (!isData) {
        for(auto fname : filenames) {
        }
        jetCorrAG = makeJetCorrector(filenames);
    }

    // // Jet Resolution ... :sad:
  // JetResolution res;  
  // res.loadResolutionFile("CORE/Tools/JetResolution/data/Spring16_25nsV10_MC/Spring16_25nsV10_MC_PtResolution_AK4PFchs.txt");
  // res.loadScaleFactorFile("CORE/Tools/JetResolution/data/Spring16_25nsV10_MC/Spring16_25nsV10_MC_SF_AK4PFchs.txt");


    //Histograms for cross-section calculation
    struct csErr_info_t { TH1F* cs; float cs_scale_up; float cs_scale_dn; float cs_pdf[102]; TH1F* results; int nEntries; int mGluino; int mLSP;}; 
    vector <csErr_info_t> csErr_info_v; 

    bool haveMadeErrStruct = false;

    auto t0 = time(0);

    //Event Loop
    for(unsigned int eventAG=0; eventAG < nEvents; eventAG++){


        if ((nevents_max > 0) && (eventAG > nevents_max)) break;

        //Get Event Content
        cms3.GetEntry(eventAG);
        nEventsTotal++;

        // HAVE TO MAKE DATA JEC HERE SINCE WE NEED RUN NUMBER BECAUSE SO MANY JECS
        // Only need to check first event because a merged file can't span eras
        if (isData && eventAG == 0) {
            if (tas::evt_run() <= 276811) jecEra = "Summer16_23Sep2016BCDV3";
            else if (tas::evt_run() <= 278801 && tas::evt_run() >= 276831) jecEra = "Summer16_23Sep2016EFV3";
            else if (tas::evt_run() <= 280385 && tas::evt_run() >= 278802) jecEra = "Summer16_23Sep2016GV3";
            else if (tas::evt_run() >= 280919) jecEra = "Summer16_23Sep2016HV3";
            else std::cout << ">>> [!] Shouldn't get here! Can't figure out JEC. isData,run = " << isData << "," << tas::evt_run() << std::endl;
            jecUnc = new JetCorrectionUncertainty("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_Uncertainty_AK4PFchs.txt"); 
            jetcorr_filenames_25ns_DATA_pfL1L2L3.clear();
            jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_L1FastJet_AK4PFchs.txt");
            jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_L2Relative_AK4PFchs.txt");
            jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_L3Absolute_AK4PFchs.txt");
            jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_L2L3Residual_AK4PFchs.txt");
            jetCorrAG = makeJetCorrector(jetcorr_filenames_25ns_DATA_pfL1L2L3);
        }

        //See if mass point exists already
        int idx = -1;
        if (!(tas::evt_isRealData())) {
            if (!haveMadeErrStruct) {
                csErr_info_t csErr; 
                csErr.cs = new TH1F("cs","cs", 1, 0, 1);
                csErr.cs->Sumw2(); 
                csErr.cs_scale_up = 0;
                csErr.cs_scale_dn = 0;
                for (int i = 0; i < 102; i++) csErr.cs_pdf[i] = 0; 
                csErr.results = new TH1F("csErr","csErr", 16000, 0, 16000); 
                csErr.results->Sumw2(); 
                csErr.nEntries = 0; 
                idx = 0;
                csErr_info_v.push_back(csErr); 
                haveMadeErrStruct = true;
            }
        }

        //If data, check good run list
        if (tas::evt_isRealData() && !goodrun(tas::evt_run(), tas::evt_lumiBlock())) continue;

        // if (cms3.pfjets_p4().size() > 0) {
        //     res.resetSeed();
        //     int ijet = 0;
        //     vector<Double_t> GenJetPt;
        //     for (auto& genjet : cms3.genjets_p4NoMuNoNu()) {
        //         GenJetPt.push_back(genjet.pt());
        //     }
        //     res.loadVariable("JetEta", cms3.pfjets_p4().at(ijet).eta());
        //     res.loadVariable("Rho", cms3.evt_fixgridfastjet_all_rho());
        //     res.loadVariable("JetPt", cms3.pfjets_p4().at(ijet).pt()); // should be the corrected energy. Not taken into account here.
        //     auto smearing = res.smear(cms3.pfjets_p4().at(ijet), cms3.genjets_p4NoMuNoNu(), GenJetPt, 0);
        //     auto smearingup = res.smear(cms3.pfjets_p4().at(ijet), cms3.genjets_p4NoMuNoNu(), GenJetPt, 1);
        //     auto smearingdn = res.smear(cms3.pfjets_p4().at(ijet), cms3.genjets_p4NoMuNoNu(), GenJetPt, -1);
        //     auto matching = res.match();
        //     cout << "   MC resolution:        " << res.getResolution() << endl;
        //     cout << "   data/MC scale factor: " << res.getScaleFactor(0) << endl;
        //     cout << "   jet eta:              " << cms3.pfjets_p4().at(ijet).eta() << endl;
        //     cout << "   jet phi:              " << cms3.pfjets_p4().at(ijet).phi() << endl;
        //     cout << "   rho:                  " << cms3.evt_fixgridfastjet_all_rho() << endl;
        //     cout << "   is matched:           " << matching[0] << endl;
        //     cout << "   original jet pT:      " << cms3.pfjets_p4().at(ijet).pt() << endl;
        //     cout << "   smeared jet pT:       " << smearing[0] << endl;
        //     cout << "   smeared jet pT up:       " << smearingup[0] << endl;
        //     cout << "   smeared jet pT dn:       " << smearingdn[0] << endl;
        //     cout << endl;
        // }


        //Progress bar
        CMS3::progress(nEventsTotal, nEvents);

        if (nEventsTotal % 5000 == 0) std::cout << "Begin processing entry " << nEventsTotal << " at " << time(0) << "." << std::endl;

        csErr_t csErr = mylooper->ProcessBaby(filename.Data(), jetCorrAG, jecUnc, isSignal); 
        int SR = csErr.SR; 
        bool isGood = csErr.isGood; 
        
        // if (SR == 1 || SR == 10) {
        //     // SR isGood 
        //     std::cout << " SR: " << SR << " isGood: " << isGood  << std::endl;
        // }

        //c-s error variables
        if (isSignal > 0 || haveMadeErrStruct){
            if (haveMadeErrStruct) idx = 0;
            csErr_info_v[idx].results->Fill(0.5, 1);  
            csErr_info_v[idx].results->Fill(3.5, csErr.cs_scale_up); 
            csErr_info_v[idx].results->Fill(4.5, csErr.cs_scale_dn); 
            if (SR > 0 && isGood) csErr_info_v[idx].results->Fill(200+SR-0.5, csErr.cs_scale_no); 
            if (SR > 0 && isGood) csErr_info_v[idx].results->Fill(400+SR-0.5, csErr.cs_scale_up); 
            if (SR > 0 && isGood) csErr_info_v[idx].results->Fill(600+SR-0.5, csErr.cs_scale_dn); 
            for (unsigned int i = 0; i < 100; i++){
                if (SR > 0 && isGood) csErr_info_v[idx].results->Fill(1000+100*(SR-1)+i-0.5, csErr.cs_pdf[i]);
                csErr_info_v[idx].results->Fill(6+i-0.5, csErr.cs_pdf[i]); 
            }
            if (SR > 0 && isGood) csErr_info_v[idx].results->Fill(15600+SR-0.5, csErr.cs_pdf[100]); 
            if (SR > 0 && isGood) csErr_info_v[idx].results->Fill(15700+SR-0.5, csErr.cs_pdf[101]); 
        }

    }//event loop 

    if (isSignal > 0 || haveMadeErrStruct){
        for (unsigned int j = 0; j < csErr_info_v.size(); j++){
            //bin 1: nEntries
            //bin 2: cross-section
            csErr_info_v[j].results->SetBinContent(2, csErr_info_v[j].cs->Integral()); 
            //bin 3: cross-section stat err
            csErr_info_v[j].results->SetBinError(3, csErr_info_v[j].cs->GetBinError(1)); 
            //bin 4: cross-section scale up
            //bin 5: cross-section scale dn
            //bin 6-107: cross-section PDF error
            //bin 201-266: yield in each SR (for cross-check)
            //bin 401-466: scale up in each SR
            //bin 601-666: scale dn in each SR 
            //bin 1000-1099: SR1 PDF weights
            //bin 1100-1199: SR2 PDF weights
            //bin 7500-7599: SR66 PDF weights
            //bin 15600-15666: alpha_s up in each SR
            //bin 15700-15766: alpha_s dn in each SR

        }
    }

    auto t1 = time(0);
    std::cout << "Processed " << nEventsTotal << " events in " << t1-t0 << " seconds @ " << 0.001*nEventsTotal/(t1-t0) << "kHz" << std::endl;

    //Delete Chain
    mylooper->CloseBabyNtuple();

    //Open the baby file again
    TFile* BabyFile = new TFile(outname, "UPDATE");
    BabyFile->cd();
    for (unsigned int j = 0; j < csErr_info_v.size(); j++){
        csErr_info_v[j].results->Write(); 
    }

    return 0;

}
/* vim: set ft=cpp: */
