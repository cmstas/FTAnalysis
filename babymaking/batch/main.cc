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

#include <chrono>

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
    bool goodRunsOnly = true;
    // bool useXrootd = false;
    if (hostname.Contains("uafino")) {
        std::cout << ">>> We're on uafino, so using xrootd!" << std::endl;  
        useXrootd = true;
    }



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


    bool isData = filename.Contains("run2_data") || filename.Contains("Run201");
    int isSignal = 0; 
    int year = -1;
    if (
            filename.Contains("Run2016") || filename.Contains("Moriond17")
            || filename.Contains("RunIISummer16")
       ) year = 2016;
    if (
            filename.Contains("Run2017") || filename.Contains("RunIIFall17")
            || filename.Contains("_mc2017_")
       ) year = 2017;
    if (
            filename.Contains("Run2018") || filename.Contains("RunIISpring18")
            || filename.Contains("RunIISummer18")
       ) year = 2018;

    if (year < 0) {
        std::cout << ">>> [!] Couldn't figure out year, so setting it to 2017. Make sure this is what you want!" << std::endl;
        year = 2017;
    } else {
        std::cout << ">>> Figured out that the year is " << year << "." << std::endl;
    }

    if (year == 2016) {
        gconf.year = year;
        gconf.ea_version = 1;
        gconf.btag_disc_wp = 0.6324;
        gconf.multiiso_el_minireliso = 0.12;
        gconf.multiiso_el_ptratio = 0.80;
        gconf.multiiso_el_ptrel = 7.2;
        gconf.multiiso_mu_minireliso = 0.16;
        gconf.multiiso_mu_ptratio = 0.76;
        gconf.multiiso_mu_ptrel = 7.2;
    }
    if (year == 2017) {
        gconf.year = year;
        gconf.ea_version = 3;
        gconf.btag_disc_wp = 0.4941;
        gconf.multiiso_el_minireliso = 0.09;
        gconf.multiiso_el_ptratio = 0.85;
        gconf.multiiso_el_ptrel = 9.2;
        gconf.multiiso_mu_minireliso = 0.12;
        gconf.multiiso_mu_ptratio = 0.80;
        gconf.multiiso_mu_ptrel = 7.5;
    }
    if (year == 2018) {
        gconf.year = year;
        gconf.ea_version = 3;
        gconf.btag_disc_wp = 0.4941;
        gconf.multiiso_el_minireliso = 0.09;
        gconf.multiiso_el_ptratio = 0.85;
        gconf.multiiso_el_ptrel = 9.2;
        gconf.multiiso_mu_minireliso = 0.12;
        gconf.multiiso_mu_ptratio = 0.80;
        gconf.multiiso_mu_ptrel = 7.5;
    }

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
    // if (filename.Contains("_HToTT_")) isSignal = 101; // isSignal > 100 used only for non SMS stuff
    if (filename.Contains("_HToTT_") && filename.Contains("Summer16Fast")) isSignal = 101; // fastsim higgs vs fullsim higgs


    //Set up file and tree
    babyMaker *mylooper = new babyMaker();
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


    // 2016
    string good_run_file = "goodRunList/goldenJson_2016rereco_36p46ifb.txt";
    string jecEra = "Summer16_23Sep2016BCDV3";
    std::string jecEraMC = "Summer16_23Sep2016V3";

    if (gconf.year == 2017) {
        good_run_file = "goodRunList/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1_snt.txt";
        jecEra = "Fall17_17Nov2017B_V6";
        jecEraMC = "Fall17_17Nov2017_V6";
    }

    if (gconf.year == 2018) {
        good_run_file = "goodRunList/Cert_314472-317591_13TeV_PromptReco_Collisions18_JSON_snt.txt";
        jecEra = "Fall17_17Nov2017C_V6";
        jecEraMC = "Fall17_17Nov2017_V6";
    }

    //Add good run list
    set_goodrun_file(good_run_file.c_str());


    //Make Jet Energy Uncertainties
    JetCorrectionUncertainty *jecUnc = 0;
    if (!isData)      jecUnc = new JetCorrectionUncertainty("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_Uncertainty_AK4PFchs.txt"); 
    if (isSignal > 0) jecUnc = new JetCorrectionUncertainty("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_Uncertainty_AK4PFchs.txt"); 

    //Init MVA
    createAndInitMVA("./CORE", true, true, 80);


    //JEC files -- 25 ns MC
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL1;
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL2L3;
    std::vector<std::string> jetcorr_filenames_25ns_MC_pfL1L2L3;
    jetcorr_filenames_25ns_MC_pfL1.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L3Absolute_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L2Relative_AK4PFchs.txt");
    jetcorr_filenames_25ns_MC_pfL1L2L3.push_back  ("CORE/Tools/jetcorr/data/run2_25ns/"+jecEraMC+"_MC/"+jecEraMC+"_MC_L3Absolute_AK4PFchs.txt");

    //JEC files -- 25 ns DATA
    std::vector<std::string> jetcorr_filenames_25ns_DATA_pfL1;
    std::vector<std::string> jetcorr_filenames_25ns_DATA_pfL2L3;
    std::vector<std::string> jetcorr_filenames_25ns_DATA_pfL1L2L3;

    //JEC files -- 25 ns FASTSIM
    std::vector<std::string> jetcorr_filenames_25ns_FASTSIM_pfL1;
    std::vector<std::string> jetcorr_filenames_25ns_FASTSIM_pfL2L3;
    std::vector<std::string> jetcorr_filenames_25ns_FASTSIM_pfL1L2L3;
    jetcorr_filenames_25ns_FASTSIM_pfL1.push_back("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_L1FastJet_AK4PFchs.txt");
    jetcorr_filenames_25ns_FASTSIM_pfL2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_L2Relative_AK4PFchs.txt"); 
    jetcorr_filenames_25ns_FASTSIM_pfL2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_L3Absolute_AK4PFchs.txt"); 
    jetcorr_filenames_25ns_FASTSIM_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_L2Relative_AK4PFchs.txt"); 
    jetcorr_filenames_25ns_FASTSIM_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_L3Absolute_AK4PFchs.txt"); 
    jetcorr_filenames_25ns_FASTSIM_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/Spring16_FastSimV1_L1FastJet_AK4PFchs.txt");

    //JECs

    FactorizedJetCorrector *jetCorrAG;
    FactorizedJetCorrector *jetCorrAG_L1;
    FactorizedJetCorrector *jetCorrAG_L2L3;
    if (!isData) {
        if (!isSignal) {
            jetCorrAG_L1 = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL1);
            jetCorrAG_L2L3 = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL2L3);
            jetCorrAG = makeJetCorrector(jetcorr_filenames_25ns_MC_pfL1L2L3);
        } else {
            jetCorrAG_L1 = makeJetCorrector(jetcorr_filenames_25ns_FASTSIM_pfL1);
            jetCorrAG_L2L3 = makeJetCorrector(jetcorr_filenames_25ns_FASTSIM_pfL2L3);
            jetCorrAG = makeJetCorrector(jetcorr_filenames_25ns_FASTSIM_pfL1L2L3);
        }
        // for closestJet() in CORE/IsolationTools.cc
        gconf.jet_corrector_L1 = jetCorrAG_L1;
        gconf.jet_corrector_L2L3 = jetCorrAG_L2L3;
    }

    //Histograms for cross-section calculation
    struct csErr_info_t { TH1F* cs; float cs_scale_up; float cs_scale_dn; float cs_pdf[102]; TH1F* results; int nEntries; int mGluino; int mLSP;}; 
    vector <csErr_info_t> csErr_info_v; 

    bool haveMadeErrStruct = false;

    auto t0 = std::chrono::steady_clock::now();
    auto tlast = std::chrono::steady_clock::now();

    //Event Loop
    for(unsigned int eventAG=0; eventAG < nEvents; eventAG++){

        if ((nevents_max > 0) && (eventAG > nevents_max)) break;

        //Get Event Content
        cms3.GetEntry(eventAG);
        nEventsTotal++;

        // HAVE TO MAKE DATA JEC HERE SINCE WE NEED RUN NUMBER BECAUSE SO MANY JECS
        // Only need to check first event because a merged file can't span eras
        if (isData && eventAG == 0) {
            if (     tas::evt_run() <= 276811) jecEra = "Summer16_23Sep2016BCDV3";
            else if (tas::evt_run() <= 278801 && tas::evt_run() >= 276831) jecEra = "Summer16_23Sep2016EFV3";
            else if (tas::evt_run() <= 280385 && tas::evt_run() >= 278802) jecEra = "Summer16_23Sep2016GV3";
            else if (tas::evt_run() <  294645 && tas::evt_run() >= 280919) jecEra = "Summer16_23Sep2016HV3";
            else if (tas::evt_run() <= 299329 && tas::evt_run() >= 297046) jecEra = "Fall17_17Nov2017B_V6";
            else if (tas::evt_run() <= 302029 && tas::evt_run() >= 299368) jecEra = "Fall17_17Nov2017C_V6";
            else if (tas::evt_run() <= 303434 && tas::evt_run() >= 302030) jecEra = "Fall17_17Nov2017D_V6";
            else if (tas::evt_run() <= 304797 && tas::evt_run() >= 303824) jecEra = "Fall17_17Nov2017E_V6";
            else if (tas::evt_run() <= 306462 && tas::evt_run() >= 305040) jecEra = "Fall17_17Nov2017F_V6";
            else if (tas::evt_run() > 306462) jecEra = "Fall17_17Nov2017C_V6"; // FIXME 2018?
            else std::cout << ">>> [!] Shouldn't get here! Can't figure out JEC. isData,run = " << isData << "," << tas::evt_run() << std::endl;
            jecUnc = new JetCorrectionUncertainty("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_Uncertainty_AK4PFchs.txt"); 
            jetcorr_filenames_25ns_DATA_pfL1.clear();
            jetcorr_filenames_25ns_DATA_pfL1.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_L1FastJet_AK4PFchs.txt");
            jetcorr_filenames_25ns_DATA_pfL2L3.clear();
            jetcorr_filenames_25ns_DATA_pfL2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_L2Relative_AK4PFchs.txt");
            jetcorr_filenames_25ns_DATA_pfL2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_L3Absolute_AK4PFchs.txt");
            jetcorr_filenames_25ns_DATA_pfL2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_L2L3Residual_AK4PFchs.txt");
            jetcorr_filenames_25ns_DATA_pfL1L2L3.clear();
            jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_L1FastJet_AK4PFchs.txt");
            jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_L2Relative_AK4PFchs.txt");
            jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_L3Absolute_AK4PFchs.txt");
            jetcorr_filenames_25ns_DATA_pfL1L2L3.push_back("CORE/Tools/jetcorr/data/run2_25ns/"+jecEra+"_DATA/"+jecEra+"_DATA_L2L3Residual_AK4PFchs.txt");
            jetCorrAG_L1 = makeJetCorrector(jetcorr_filenames_25ns_DATA_pfL1);
            jetCorrAG_L2L3 = makeJetCorrector(jetcorr_filenames_25ns_DATA_pfL2L3);
            jetCorrAG = makeJetCorrector(jetcorr_filenames_25ns_DATA_pfL1L2L3);
            // for closestJet() in CORE/IsolationTools.cc
            gconf.jet_corrector_L1 = jetCorrAG_L1;
            gconf.jet_corrector_L2L3 = jetCorrAG_L2L3;

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
        if (goodRunsOnly && tas::evt_isRealData() && !goodrun(tas::evt_run(), tas::evt_lumiBlock())) continue;

        //Progress bar
        CMS3::progress(nEventsTotal, nEvents);

        if (nEventsTotal % 5000 == 0) {
            auto t1 = std::chrono::steady_clock::now();
            float duration = 0.001*(std::chrono::duration_cast<std::chrono::milliseconds>(t1 - tlast)).count();
            std::cout << "Begin processing entry " << nEventsTotal << " at " << time(0) << " (" << 0.001*round(5000/duration) << " kHz)." << std::endl;
            tlast = t1;
        }

        csErr_t csErr = mylooper->ProcessBaby(filename.Data(), jetCorrAG, jecUnc, isSignal); 
        int SR = csErr.SR; 
        bool isGood = csErr.isGood; 
        
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

    auto t1 = std::chrono::steady_clock::now();
    float duration = 0.001*(std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0)).count();
    std::cout << "Processed " << nEventsTotal << " events in " << duration << " seconds @ " << 0.001*round(nEventsTotal/duration) << " kHz" << std::endl;

    //Delete Chain
    mylooper->CloseBabyNtuple();

    //Open the baby file again
    TFile* BabyFile = new TFile(outname, "UPDATE");
    BabyFile->cd();
    for (unsigned int j = 0; j < csErr_info_v.size(); j++){
        csErr_info_v[j].results->Write(); 
    }

    if (nEvents != nEventsTotal) {
        std::cout << "Number of input events in tree is " << nEvents << " but we processed " << nEventsTotal << ", ";
        std::cout << "so something went wrong and the return code will not be 0." << std::endl;
        return 1;
    }

    return 0;

}
/* vim: set ft=cpp: */
