#include "./CORE/CMS3.h"
#include "./CORE/SSSelections.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "helper_babymaker.h"
#include "CORE/Tools/goodrun.h"
#include "CORE/Tools/JetCorrector.h"
#include "CORE/Tools/datasetinfo/getDatasetInfo.h"
#include "CORE/Tools/jetcorr/JetCorrectionUncertainty.h"  
#include "CORE/Tools/jetcorr/SimpleJetCorrectionUncertainty.h"

bool STOP_REQUESTED;

int main(int argc, char *argv[]){

    signal(SIGINT, [](int){
            cout << "SIGINT Caught, stopping after current event" << endl;
            STOP_REQUESTED=true;
            });
    STOP_REQUESTED = false;

    // FactorizedJetCorrector * jc = makeJetCorrector({
    //         "CORE/Tools/jetcorr/data/run2_25ns/Fall17_17Nov2017_V6_MC/Fall17_17Nov2017_V6_MC_L1FastJet_AK4PFchs.txt",
    //         "CORE/Tools/jetcorr/data/run2_25ns/Fall17_17Nov2017_V6_MC/Fall17_17Nov2017_V6_MC_L2Relative_AK4PFchs.txt",
    //         "CORE/Tools/jetcorr/data/run2_25ns/Fall17_17Nov2017_V6_MC/Fall17_17Nov2017_V6_MC_L3Absolute_AK4PFchs.txt",
    //         });
    // jc->setRho(23.2);
    // jc->setJetA(0.5);
    // jc->setJetPt(157.2);
    // jc->setJetEta(-1.4);
    // float corr = jc->getCorrection();
    // std::cout <<  " corr: " << corr <<  std::endl;
    // // for (auto c : corrs) {
    // //     std::cout <<  " c: " << c <<  std::endl;
    // // }
    // return 0;

    TString filenames;
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

    if (argc > 1) filenames   = TString(argv[1]);
    if (argc > 2) outname     = TString(argv[2]);
    if (argc > 3) nevents_max = atoi(argv[3]);

    std::cout << ">>> Args: " << std::endl;
    std::cout << "     filenames:    " << filenames << std::endl;
    std::cout << "     outname:     " << outname << std::endl;
    std::cout << "     nevents_max: " << nevents_max << std::endl;

    if (argc <= 1) { 
        std::cout << ">>> [!] Not enough arguments!" << std::endl;  
        return 1;
    }


    bool isData = filenames.Contains("run2_data") || filenames.Contains("Run201");
    int isSignal = 0; 
    int year = -1;
    if (
            filenames.Contains("Run2016")
            || filenames.Contains("Moriond17")
            || filenames.Contains("RunIISummer16")
            || filenames.Contains("run2_data2016")
            || filenames.Contains("run2_moriond17")
       ) year = 2016;
    if (
            filenames.Contains("Run2017")
            || filenames.Contains("RunIIFall17")
            || filenames.Contains("_mc2017_")
            || filenames.Contains("run2_mc2017")
       ) year = 2017;
    if (
            filenames.Contains("Run2018")
            || filenames.Contains("RunIISpring18")
            || filenames.Contains("RunIISummer18")
            || filenames.Contains("run2_mc2018")
       ) year = 2018;

    if (year < 0) {
        std::cout << ">>> [!] Couldn't figure out year, so setting it to 2017. Make sure this is what you want!" << std::endl;
        year = 2017;
    } else {
        std::cout << ">>> Figured out that the year is " << year << "." << std::endl;
    }

    // 2016

    string good_run_file;
    string jecEra; // XXX NOTE that you also have to change the run-dependent data JECs below too
    std::string jecEraMC;


    gconf.SS_innerlayers = 0;

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
        if (filenames.Contains("-17Jul2018-")) {
            gconf.cmssw_ver = 94;
        }
        good_run_file = "goodRunList/goldenJson_2016rereco_36p46ifb.txt";
        jecEra = "Summer16_23Sep2016BCDV4";
        jecEraMC = "Summer16_23Sep2016V4";
    }
    if (year == 2017) {
        gconf.year = year;
        gconf.ea_version = 4;
        gconf.btag_disc_wp = 0.4941;
        gconf.multiiso_el_minireliso = 0.09;
        gconf.multiiso_el_ptratio = 0.85;
        gconf.multiiso_el_ptrel = 9.2;
        gconf.multiiso_mu_minireliso = 0.12;
        gconf.multiiso_mu_ptratio = 0.80;
        gconf.multiiso_mu_ptrel = 7.5;
        good_run_file = "goodRunList/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1_snt.txt";
        jecEra = "Fall17_17Nov2017B_V6";
        jecEraMC = "Fall17_17Nov2017_V6";
    }
    if (year == 2018) {
        gconf.year = year;
        gconf.ea_version = 4;
        gconf.btag_disc_wp = 0.4941;
        gconf.multiiso_el_minireliso = 0.09;
        gconf.multiiso_el_ptratio = 0.85;
        gconf.multiiso_el_ptrel = 9.2;
        gconf.multiiso_mu_minireliso = 0.12;
        gconf.multiiso_mu_ptratio = 0.80;
        gconf.multiiso_mu_ptrel = 7.5;
        good_run_file = "goodRunList/Cert_314472-321461_13TeV_PromptReco_Collisions18_JSON_snt.txt";
        jecEra = "Fall17_17Nov2017C_V6";
        jecEraMC = "Fall17_17Nov2017_V6";
    }

    if (filenames.Contains("/SMS")) {
        if (filenames.Contains("SMS-T1tttt_Tune")) isSignal = 1;
        else if (filenames.Contains("SMS-T5qqqqVV_Tune")) isSignal = 2;
        else if (filenames.Contains("SMS-T5qqqqVV_dM20_Tune")) isSignal = 3;
        else if (filenames.Contains("SMS-T6ttWW_Tune")) isSignal = 10;
        else if (filenames.Contains("SMS-T1ttbb_Tune")) isSignal = 6;
        else if (filenames.Contains("SMS-T5tttt_dM175_Tune")) isSignal = 5;
        else if (filenames.Contains("SMS-T5ttcc_Tune")) isSignal = 4;
        else {
            std::cout << ">>> [!] Can't figure out which signal sample this is!" << std::endl;
            return 1;
        }
    }
    // if (filenames.Contains("_HToTT_")) isSignal = 101; // isSignal > 100 used only for non SMS stuff
    if (filenames.Contains("_HToTT_") && filenames.Contains("Summer16Fast")) isSignal = 101; // fastsim higgs vs fullsim higgs


    //Set up file and tree
    babyMaker *mylooper = new babyMaker();
    mylooper->MakeBabyNtuple(outname.Data(), isSignal);

    if (std::getenv("noscale1fb")) {
        mylooper->ignore_scale1fb = true;
        std::cout << ">>> [!] Ignoring scale1fb text file!" << std::endl;
    }
    if (std::getenv("noos")) {
        mylooper->ignore_os = true;
        std::cout << ">>> [!] Skipping OS events for MC!" << std::endl;
    }

    TChain *chain = new TChain("Events");

    if (useXrootd) {
        std::cout << ">>> Using xrootd" << std::endl;  
        filenames.ReplaceAll("/hadoop/cms", "root://cmsxrootd.fnal.gov/");
    }

    TObjArray *files = filenames.Tokenize(",");
    for (Int_t i = 0; i < files->GetEntries(); i++)  {
        TString fname(((TObjString *)(files->At(i)))->String());
        std::cout << ">>> Adding file: " << fname << std::endl;  
        chain->Add(fname);
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

    TH2F* count_hist = new TH2F("counts","",500,0,2500,500,0,2500);

    auto t0 = std::chrono::steady_clock::now();
    auto tlast = std::chrono::steady_clock::now();

    // Set up loop over chain
    unsigned int nEvents = chain->GetEntries();
    unsigned int nEventsTotal = 0;
    unsigned int nEventsToDo = chain->GetEntries();
    if(nEvents > 0) nEventsToDo = nEvents;
    TObjArray *listOfFiles = chain->GetListOfFiles();
    TIter fileIter(listOfFiles);
    TFile *currentFile = 0;

    // Event Counting
    std::cout << ">>> Events in TChain: " << nEvents << std::endl;  

    // File Loop
    while ( (currentFile = (TFile*)fileIter.Next()) ) { 
        if (STOP_REQUESTED) break;

        // Get File Content
        if(nEventsTotal >= nEventsToDo) continue;
        TFile *file = TFile::Open( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("Events");
        TString filename(currentFile->GetTitle());
        cms3.Init(tree);

        // Loop over Events in current file
        unsigned int nEventsTree = tree->GetEntriesFast();
        for (unsigned int evt = 0; evt < nEventsTree; evt++){

            if (STOP_REQUESTED) break;

            if ((nevents_max > 0) && (evt > nevents_max)) break;

            //Get Event Content
            cms3.GetEntry(evt);
            chain->LoadTree(evt);

            nEventsTotal++;

            // HAVE TO MAKE DATA JEC HERE SINCE WE NEED RUN NUMBER BECAUSE SO MANY JECS
            // Only need to check first event because a merged file can't span eras
            if (isData && evt == 0) {
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

            ////See if mass point exists already
            //int idx = -1;
            //if (!(tas::evt_isRealData())) {
            //    if (!haveMadeErrStruct) {
            //        csErr_info_t csErr; 
            //        csErr.cs = new TH1F("cs","cs", 1, 0, 1);
            //        csErr.cs->Sumw2(); 
            //        csErr.cs_scale_up = 0;
            //        csErr.cs_scale_dn = 0;
            //        for (int i = 0; i < 102; i++) csErr.cs_pdf[i] = 0; 
            //        csErr.results = new TH1F("csErr","csErr", 16000, 0, 16000); 
            //        csErr.results->Sumw2(); 
            //        csErr.nEntries = 0; 
            //        idx = 0;
            //        csErr_info_v.push_back(csErr); 
            //        haveMadeErrStruct = true;
            //    }
            //}

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
            // int SR = csErr.SR; 
            // bool isGood = csErr.isGood; 

            //c-s error variables
            if (isSignal > 0) {
                vector<float> sparms = tas::sparm_values();
                float s1 = 1.;
                float s2 = 1.;
                if (sparms.size() > 0) s1 = sparms[0];
                if (sparms.size() > 1) s2 = sparms[1];
                count_hist->Fill(s1,s2);
            } else {
                count_hist->Fill(1,1);
            }
            
        } // event loop 

        // // XXX hmm, these 2 lines cause segfaults. why? maybe ROOT garbage collection already
        // // takes care of this when file goes out of scope...
        // file->Close();
        // delete file;

        cout << ">>> File done" <<endl;

    } // file loop

    auto t1 = std::chrono::steady_clock::now();
    float duration = 0.001*(std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0)).count();
    std::cout << "Processed " << nEventsTotal << " events in " << duration << " seconds @ " << 0.001*round(nEventsTotal/duration) << " kHz" << std::endl;

    // Cleanup
    mylooper->BabyFile->cd();
    mylooper->BabyTree->Write();
    count_hist->Write();
    mylooper->BabyFile->Close();

    ////Open the baby file again
    //TFile* BabyFile = new TFile(outname, "UPDATE");
    //BabyFile->cd();
    //for (unsigned int j = 0; j < csErr_info_v.size(); j++){
    //    csErr_info_v[j].results->Write(); 
    //}
    // //count_hist->Write();

    if (nEvents != nEventsTotal) {
        std::cout << "Number of input events in tree is " << nEvents << " but we processed " << nEventsTotal << ", ";
        std::cout << "so something went wrong and the return code will not be 0." << std::endl;
        return 1;
    }

    return 0;

}
/* vim: set ft=cpp: */
