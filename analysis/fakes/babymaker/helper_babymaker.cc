#include "helper_babymaker.h"
#include "CORE/Tools/btagsf/BTagCalibrationStandalone.h"
#include "CORE/Tools/jetcorr/Utilities.icc"
#include "CORE/Tools/jetcorr/SimpleJetCorrectionUncertainty.h"
#include "CORE/Tools/jetcorr/JetCorrectionUncertainty.h"

#include "misc/common_utils.h"

using namespace tas;

//Main functions
void babyMaker::MakeBabyNtuple(const char* output_name, int isFastsim){

  //Create Baby
  BabyFile = new TFile(Form("%s/%s", path.Data(), output_name), "RECREATE");
  BabyFile->cd();
  BabyTree = new TTree("t", "SS2015 Baby Ntuple");

  // Event level
  BabyTree->Branch("ntight_el" , &ntight_el );
  BabyTree->Branch("nloose_el" , &nloose_el );
  BabyTree->Branch("ntight_mu" , &ntight_mu );
  BabyTree->Branch("nloose_mu" , &nloose_mu );

  BabyTree->Branch("sample"            , &sample); // XXX
  BabyTree->Branch("evt_corrMET"       , &evt_corrMET); // XXX
  BabyTree->Branch("evt_corrMETPhi"    , &evt_corrMETPhi); // XXX
  BabyTree->Branch("evt_event"         , &evt_event); // XXX
  BabyTree->Branch("evt_lumiBlock"     , &evt_lumiBlock); // XXX
  BabyTree->Branch("evt_run"           , &evt_run); // XXX
  BabyTree->Branch("passes_met_filters", &passes_met_filters);  // XXX
  BabyTree->Branch("passes_any_trigger", &passes_any_trigger);  // XXX
  BabyTree->Branch("evt_isRealData"    , &evt_isRealData); // XXX
  BabyTree->Branch("scale1fb"          , &scale1fb); // XXX
  BabyTree->Branch("qscale"          , &qscale); // XXX
  BabyTree->Branch("njets"             , &njets); // XXX
  BabyTree->Branch("njets_recoil"             , &njets_recoil); // XXX
  BabyTree->Branch("ht_SS"             , &ht_SS); // XXX
  BabyTree->Branch("nFOs_SS"           , &nFOs_SS); // XXX
  BabyTree->Branch("nvtx"              , &nvtx); // XXX
  BabyTree->Branch("trueNumInt"              , &trueNumInt); // XXX
  BabyTree->Branch("puweight"              , &puweight); // XXX


  // Lepton level

  BabyTree->Branch("p4"                            , &p4); // XXX
  BabyTree->Branch("tag_p4"                        , &tag_p4); // XXX
  BabyTree->Branch("dilep_p4"                      , &dilep_p4); // XXX
  BabyTree->Branch("id"                            , &id); // XXX
  BabyTree->Branch("idx"                           , &idx); // XXX
  BabyTree->Branch("motherID"                      , &motherID); // XXX
  BabyTree->Branch("have_tag"                      , &have_tag); // XXX
  BabyTree->Branch("tag_pt"                      , &tag_pt); // XXX
  BabyTree->Branch("p4_pt"                      , &p4_pt); // XXX
  BabyTree->Branch("p4_eta"                      , &p4_eta); // XXX

  BabyTree->Branch("mva"                      , &mva); // XXX
  BabyTree->Branch("etaSC"                      , &etaSC); // XXX

  BabyTree->Branch("passes_SS_tight_v6"            , &passes_SS_tight_v6); // XXX
  BabyTree->Branch("passes_SS_tight_noiso_v6"      , &passes_SS_tight_noiso_v6); // XXX
  BabyTree->Branch("passes_SS_fo_v6"               , &passes_SS_fo_v6); // XXX
  BabyTree->Branch("passes_SS_fo_noiso_v6"         , &passes_SS_fo_noiso_v6); // XXX
  BabyTree->Branch("passes_SS_fo_looseMVA_v6"      , &passes_SS_fo_looseMVA_v6); // XXX
  BabyTree->Branch("passes_SS_fo_looseMVA_noiso_v6", &passes_SS_fo_looseMVA_noiso_v6); // XXX

  BabyTree->Branch("ip3d"                          , &ip3d); // XXX
  BabyTree->Branch("ip3derr"                       , &ip3derr); // XXX
  BabyTree->Branch("mt"                            , &mt); // XXX
  BabyTree->Branch("ptrelv1"                       , &ptrelv1); // XXX
  BabyTree->Branch("coneCorrPt"                       , &coneCorrPt); // XXX
  BabyTree->Branch("miniiso"                       , &miniiso); // XXX
  BabyTree->Branch("close_jet_v5"                 , &close_jet_v5); // XXX
  BabyTree->Branch("ptratio_v5"                       , &ptratio_v5); // XXX
  BabyTree->Branch("isTriggerSafe"               , &isTriggerSafe); // XXX
  BabyTree->Branch("isTriggerSafenoIso"               , &isTriggerSafenoIso); // XXX
  BabyTree->Branch("dilep_mass"                                              , &dilep_mass); // XXX

  //Single Muon Triggers
  BabyTree->Branch("HLT_Mu8_TrkIsoVVL"                    , &HLT_Mu8_TrkIsoVVL); // XXX
  BabyTree->Branch("HLT_Mu17_TrkIsoVVL"                   , &HLT_Mu17_TrkIsoVVL); // XXX
  BabyTree->Branch("HLT_Mu8"                              , &HLT_Mu8); // XXX
  BabyTree->Branch("HLT_Mu17"                             , &HLT_Mu17); // XXX
  BabyTree->Branch("HLT_IsoMu27"                          , &HLT_IsoMu27); // XXX

  //Single Electron Triggers
  BabyTree->Branch("HLT_Ele8_CaloIdM_TrackIdM_PFJet30"                      , &HLT_Ele8_CaloIdM_TrackIdM_PFJet30); // XXX
  BabyTree->Branch("HLT_Ele12_CaloIdM_TrackIdM_PFJet30"                     , &HLT_Ele12_CaloIdM_TrackIdM_PFJet30); // XXX
  BabyTree->Branch("HLT_Ele17_CaloIdM_TrackIdM_PFJet30"                     , &HLT_Ele17_CaloIdM_TrackIdM_PFJet30); // XXX
  BabyTree->Branch("HLT_Ele18_CaloIdM_TrackIdM_PFJet30"                     , &HLT_Ele18_CaloIdM_TrackIdM_PFJet30); // XXX
  BabyTree->Branch("HLT_Ele23_CaloIdM_TrackIdM_PFJet30"                     , &HLT_Ele23_CaloIdM_TrackIdM_PFJet30); // XXX
  BabyTree->Branch("HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30"                , &HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30); // XXX
  BabyTree->Branch("HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30"               , &HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30); // XXX
  BabyTree->Branch("HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30"               , &HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30); // XXX
  BabyTree->Branch("HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30"               , &HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30); // XXX
  BabyTree->Branch("HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30"               , &HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30); // XXX
  BabyTree->Branch("HLT_Ele23_CaloIdL_TrackIdL_IsoVL"                       , &HLT_Ele23_CaloIdL_TrackIdL_IsoVL); // XXX
  BabyTree->Branch("HLT_Ele12_CaloIdL_TrackIdL_IsoVL"                       , &HLT_Ele12_CaloIdL_TrackIdL_IsoVL); // XXX

  //MuElectron Triggers
  BabyTree->Branch("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300"            , &HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300); // XXX
  BabyTree->Branch("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL"        , &HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL); // XXX
  BabyTree->Branch("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL"         , &HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL); // XXX

  //Double Muon Triggers
  BabyTree->Branch("HLT_DoubleMu8_Mass8_PFHT300"                            , &HLT_DoubleMu8_Mass8_PFHT300); // XXX
  BabyTree->Branch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL"                       , &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL); // XXX
  BabyTree->Branch("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL"                     , &HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL); // XXX
  BabyTree->Branch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ"                    , &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ); // XXX
  BabyTree->Branch("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ"                  , &HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ); // XXX

  //Double Electron Triggers
  BabyTree->Branch("HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300"          , &HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300); // XXX
  BabyTree->Branch("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ"              , &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ); // XXX
  BabyTree->Branch("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL"                 , &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL); // XXX
  BabyTree->Branch("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ"              , &HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ); // XXX
  BabyTree->Branch("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL"                 , &HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL); // XXX

  // Load scale1fbs/xsecs from file
  df.loadFromFile("CORE/Tools/datasetinfo/scale1fbs.txt");

}

void babyMaker::fillTriggerBranches(){

  //Single Electron Trigger
  setHLTBranch("HLT_Ele8_CaloIdM_TrackIdM_PFJet30_v" ,  true, HLT_Ele8_CaloIdM_TrackIdM_PFJet30 );
  setHLTBranch("HLT_Ele12_CaloIdM_TrackIdM_PFJet30_v",  true, HLT_Ele12_CaloIdM_TrackIdM_PFJet30);
  setHLTBranch("HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",  true, HLT_Ele17_CaloIdM_TrackIdM_PFJet30);
  setHLTBranch("HLT_Ele18_CaloIdM_TrackIdM_PFJet30_v",  true, HLT_Ele18_CaloIdM_TrackIdM_PFJet30);
  setHLTBranch("HLT_Ele23_CaloIdM_TrackIdM_PFJet30_v",  true, HLT_Ele23_CaloIdM_TrackIdM_PFJet30);
  setHLTBranch("HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v",  true, HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30);
  setHLTBranch("HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",  true, HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30);
  setHLTBranch("HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_v", true, HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30);
  setHLTBranch("HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_v",  true, HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30);
  setHLTBranch("HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v",  true, HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30);
  setHLTBranch("HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v",  true, HLT_Ele23_CaloIdL_TrackIdL_IsoVL);
  setHLTBranch("HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v",  true, HLT_Ele12_CaloIdL_TrackIdL_IsoVL);

  //MuElectron Triggers
  setHLTBranch("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v",     true    , HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300);
  setHLTBranch("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v", true, HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL);
  setHLTBranch("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v",  true , HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL);

  //Double Electron Trigger
  setHLTBranch("HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v",  true, HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300);
  setHLTBranch("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",  true, HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
  setHLTBranch("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v",  true, HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL);
  setHLTBranch("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",  true, HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
  setHLTBranch("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_v",     true, HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL);

  //Single Muon Triggers
  setHLTBranch("HLT_Mu8_v"             , true, HLT_Mu8             );
  setHLTBranch("HLT_Mu17_v"            , true, HLT_Mu17            );
  setHLTBranch("HLT_Mu8_TrkIsoVVL_v"   , true, HLT_Mu8_TrkIsoVVL   );
  setHLTBranch("HLT_Mu17_TrkIsoVVL_v"  , true, HLT_Mu17_TrkIsoVVL  );
  setHLTBranch("HLT_IsoMu27_v"         , true, HLT_IsoMu27         );

  //Double Muon Trigger
  setHLTBranch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v"     , true, HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL     );
  setHLTBranch("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v"   , true, HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL   );
  setHLTBranch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v"  , true, HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ  );
  setHLTBranch("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v", true, HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ);
  setHLTBranch("HLT_DoubleMu8_Mass8_PFHT300_v",  true, HLT_DoubleMu8_Mass8_PFHT300);

}

void babyMaker::InitBabyNtuple(){

    sample = "";
    evt_corrMET = -1.;
    evt_corrMETPhi = -1.;
    evt_event = -1;
    evt_lumiBlock = -1;
    evt_run = -1;
    passes_met_filters = 0;
    passes_any_trigger = 0;
    evt_isRealData = 0;
    scale1fb    = -1.;
    qscale    = -1.;
    njets = -1;
    njets_recoil = -1;
    ht_SS = -1.;
    nFOs_SS = -1;
    nvtx = -1;
    trueNumInt = -1.;
    puweight = 1.;

    nloose_el = 0;
    ntight_el = 0;
    nloose_mu = 0;
    ntight_mu = 0;


    //Single Muon Trigger
    HLT_Mu8_TrkIsoVVL = 0;
    HLT_Mu17_TrkIsoVVL = 0;
    HLT_Mu8 = 0;
    HLT_Mu17 = 0;
    HLT_IsoMu27 = 0;

    //Single Electron Trigger
    HLT_Ele8_CaloIdM_TrackIdM_PFJet30 = 0;
    HLT_Ele12_CaloIdM_TrackIdM_PFJet30 = 0;
    HLT_Ele17_CaloIdM_TrackIdM_PFJet30 = 0;
    HLT_Ele18_CaloIdM_TrackIdM_PFJet30 = 0;
    HLT_Ele23_CaloIdM_TrackIdM_PFJet30 = 0;
    HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0;
    HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0;
    HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0;
    HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0;
    HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 = 0;
    HLT_Ele23_CaloIdL_TrackIdL_IsoVL = 0;
    HLT_Ele12_CaloIdL_TrackIdL_IsoVL = 0;

    //MuEl Trigger
    HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300 = 0;
    HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL = 0;
    HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL = 0;

    //Double Muon Trigger
    HLT_DoubleMu8_Mass8_PFHT300 = 0;
    HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL = 0;
    HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL = 0;
    HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ = 0;
    HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ = 0;

    //Double Electron Trigger
    HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300 = 0;
    HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = 0;
    HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL = 0;
    HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = 0;
    HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL = 0;

}

void babyMaker::InitLeptonBranches(){

    //Leptons
    p4 = LorentzVector(0,0,0,0);
    tag_p4 = LorentzVector(0,0,0,0);
    dilep_p4 = LorentzVector(0,0,0,0);
    id = -1;
    idx = -1;
    motherID = -1;
    have_tag = 0;
    tag_pt = 0.;
    p4_pt = 0.;
    p4_eta = 0.;

    mva = -999.;
    etaSC = -999.;

    passes_SS_tight_v6 = 0;
    passes_SS_tight_noiso_v6 = 0;
    passes_SS_fo_v6 = 0;
    passes_SS_fo_noiso_v6 = 0;
    passes_SS_fo_looseMVA_v6 = 0;
    passes_SS_fo_looseMVA_noiso_v6 = 0;

    ip3d = -1;
    ip3derr = -1;
    mt = -1;
    ptrelv1 = -1;
    coneCorrPt = -1;
    miniiso = -1;
    close_jet_v5 = LorentzVector(0,0,0,0);
    ptratio_v5 = -1;
    isTriggerSafe = 0;
    isTriggerSafenoIso = 0;
    dilep_mass = -1.;

}

//Main function
csErr_t babyMaker::ProcessBaby(string filename_in, FactorizedJetCorrector* jetCorr, JetCorrectionUncertainty* jecUnc, int isFastsim){

  //Initialize variables
  InitBabyNtuple();

  csErr_t babyErrorStruct;

  //Preliminary stuff
  if (verbose) std::cout << "--------------\nEVENT: " << tas::evt_run() << ":" << tas::evt_lumiBlock() << ":" << tas::evt_event() << "\n--------------" << std::endl;


  //Fill Easy Variables
  evt_event = tas::evt_event();
  evt_lumiBlock = tas::evt_lumiBlock();
  evt_run = tas::evt_run();
  evt_isRealData = tas::evt_isRealData();

  nloose_el = 0; // num loose el with pt>10
  ntight_el = 0; // num tight el with pt>10
  nloose_mu = 0; // num loose mu with pt>10
  ntight_mu = 0; // num tight mu with pt>10
  // vector<int> idx_loose_el;
  // vector<int> idx_tight_el;
  // vector<int> idx_loose_mu;
  // vector<int> idx_tight_mu;
  vector<Lepton> leps;
  for (int ilep = 0; ilep < els_p4().size(); ilep++) {
      float pt = els_p4()[ilep].pt();
      bool is_loose = isDenominatorLepton(11,ilep) and (pt > 10.);
      if (not is_loose) continue;
      bool is_tight = isGoodLepton(11,ilep);
      nloose_el += is_loose;
      ntight_el += is_tight;
      // if (is_loose) idx_loose_el.push_back(ilep);
      // if (is_tight) idx_tight_el.push_back(ilep);
      Lepton lep = Lepton(-11*els_charge()[ilep],ilep);
      lep.set_idlevel(is_loose+is_tight); // 0 is default, 1 is loose, 2 is tight
      leps.push_back(lep);
  }
  for (int ilep = 0; ilep < mus_p4().size(); ilep++) {
      float pt = mus_p4()[ilep].pt();
      bool is_loose = isDenominatorLepton(13,ilep) and (pt > 10.);
      if (not is_loose) continue;
      bool is_tight = isGoodLepton(13,ilep);
      nloose_mu += is_loose;
      ntight_mu += is_tight;
      // if (is_loose) idx_loose_mu.push_back(ilep);
      // if (is_tight) idx_tight_mu.push_back(ilep);
      Lepton lep = Lepton(-13*mus_charge()[ilep],ilep);
      lep.set_idlevel(is_loose+is_tight);
      leps.push_back(lep);
  }


  // num FOs with pt>10
  nFOs_SS = nloose_mu + nloose_el;

  // If there's not even one FO, no point
  if (leps.size() < 1) return babyErrorStruct;

  // Fill trigger branches
  fillTriggerBranches();

  passes_any_trigger = (HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30 or
          HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30 or
          HLT_Ele17_CaloIdM_TrackIdM_PFJet30 or
          HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 or
          HLT_Ele23_CaloIdM_TrackIdM_PFJet30 or
          HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 or
          HLT_Ele8_CaloIdM_TrackIdM_PFJet30 or
          HLT_IsoMu27 or
          HLT_Mu17 or
          HLT_Mu17_TrkIsoVVL or
          HLT_Mu8 or
          HLT_Mu8_TrkIsoVVL or
          HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30);
  if (not passes_any_trigger) return babyErrorStruct;

  // MC stuff and filters
  scale1fb = 1;
  if (!evt_isRealData) {
      trueNumInt = tas::puInfo_trueNumInteractions()[0];
      nvtx = 0;
      for(unsigned int ivtx=0; ivtx < tas::evt_nvtxs(); ivtx++){
          nvtx += isGoodVertex(ivtx);
      }
      float sgnMCweight = ((tas::genps_weight() > 0) - (tas::genps_weight() < 0));
      scale1fb = sgnMCweight*df.getScale1fbFromFile(tas::evt_dataset()[0].Data(),tas::evt_CMS3tag()[0].Data());
      qscale = genps_qScale(); // for stitching the enriched QCD samples
  }
  passes_met_filters = evt_isRealData ? passesMETfiltersMoriond17(evt_isRealData) : 1;

  // Jets
  vector<Jet> jets = SSJetsCalculator(jetCorr, 1).first;
  njets = jets.size();
  ht_SS = 0;
  for (unsigned int ijet = 0; ijet < jets.size(); ijet++){
      auto jet = jets[ijet];
      ht_SS += jet.pt()*jet.undo_jec()*jet.jec();
  }

  //MET variables
  // for old 09-04-17 tag QCD, use 3 to bypass any alternative MET collection selection
  // and for newer tags (-19) use 2 to get EE noise fixed MET
  bool isOldTag = filename_in.find("09-04-17") != std::string::npos;
  int use_cleaned_met = (isOldTag ? 3 : 2);
  pair<float,float> corrMETPair = getT1CHSMET_fromMINIAOD(jetCorr, NULL, 0, false, use_cleaned_met);
  evt_corrMET    = corrMETPair.first;
  evt_corrMETPhi = corrMETPair.second;

  // Now for each loose loose lepton, fill branches
  // And if one of the loose leptons is also tight AND has another tight lepton forming a Z with it, then flag this as a Z and store Z stuff

  bool event_maybe_has_z = (ntight_el>=2) or (ntight_mu>=2);

  for (int ilep = 0; ilep < leps.size(); ilep++) {

      InitLeptonBranches();

      Lepton lep = leps[ilep];

      id = lep.id();
      idx = lep.idx();
      p4 = lep.p4();
      p4_pt = lep.pt();
      p4_eta = lep.eta();
      mt = MT(p4_pt, p4.phi(), evt_corrMET, evt_corrMETPhi);


      // Loop and fill branches if there actually is a tight-tight Z pair
      // Consider jlep>ilep to avoid counting 2 separate events in the tree as having a Z
      if (event_maybe_has_z and lep.idlevel()==2 and lep.pt()>30.) {
          bool found_z = false;
          Lepton zlep;
          for (int jlep = ilep+1; jlep < leps.size(); jlep++) {
              Lepton otherlep = leps[jlep];
              // Require OSSF tight-tight pT>30,30
              if (otherlep.idlevel() != 2) continue;
              if (otherlep.pt() < 30.) continue;
              if (lep.id()+otherlep.id() != 0) continue;
              float mll = (lep.p4()+otherlep.p4()).M();
              if (fabs(mll - 91.18) > 15.) continue;
              found_z = true;
              zlep = otherlep;
              break;
          }
          if (found_z) {
              dilep_p4 = (lep.p4()+zlep.p4());
              dilep_mass = dilep_p4.M();
              tag_p4 = zlep.p4();
              have_tag = true;
              tag_pt = zlep.pt();
          }
      }


      float A = lep.is_el() ? gconf.multiiso_el_minireliso : gconf.multiiso_mu_minireliso;
      float B = lep.is_el() ? gconf.multiiso_el_ptratio : gconf.multiiso_mu_ptratio;
      float C = lep.is_el() ? gconf.multiiso_el_ptrel : gconf.multiiso_mu_ptrel;
      mva = lep.is_el() ? getMVAoutput(idx, true) : -999.;
      etaSC = lep.is_el() ? els_etaSC().at(idx) : -999.;
      close_jet_v5 = closestJet(lep.p4(), 0.4, 3.0, 2);
      ptrelv1 = ptRel(lep.p4(), close_jet_v5, true);
      ptratio_v5 = close_jet_v5.pt() > 0 ? lep.pt()/close_jet_v5.pt() : 1;
      miniiso = lep.is_el() ?  elMiniRelIsoCMS3_EA(idx,gconf.ea_version) : muMiniRelIsoCMS3_EA(idx,gconf.ea_version);
      coneCorrPt = ((ptrelv1 > C) ? lep.pt()*(1 + std::max((float)0, miniiso - A)) : std::max(lep.pt(), close_jet_v5.pt() * B));
      isTriggerSafe = lep.is_el() ? isTriggerSafe_v1(idx) : true;
      isTriggerSafenoIso = lep.is_el() ? isTriggerSafenoIso_v1(idx) : true;

      passes_SS_tight_v6 = lep.idlevel() == 2;
      passes_SS_fo_v6 = lep.idlevel() >= 1;
      passes_SS_fo_looseMVA_v6 = lep.idlevel() >= 1;

      if (lep.is_el()) {
          elID::setCache(idx,mva,miniiso,ptratio_v5,ptrelv1);
          passes_SS_tight_noiso_v6 = isGoodLeptonNoIso(lep.id(),idx);
          passes_SS_fo_noiso_v6 = isDenominatorLeptonNoIso(lep.id(),idx);
          elID::unsetCache();

          ip3d = els_ip3d().at(idx);
          ip3derr = els_ip3derr().at(idx);

      } else {
          muID::setCache(idx,miniiso,ptratio_v5,ptrelv1);
          passes_SS_tight_noiso_v6 = isGoodLeptonNoIso(lep.id(),idx);
          passes_SS_fo_looseMVA_noiso_v6 = isDenominatorLeptonNoIso(lep.id(),idx);
          muID::unsetCache();

          ip3d = mus_ip3d().at(idx);
          ip3derr = mus_ip3derr().at(idx);

      }

      njets_recoil = 0;
      for(unsigned int ijet=0; ijet < jets.size(); ijet++)  {
          auto jet = jets[ijet];
          if(ROOT::Math::VectorUtil::DeltaR(jet.p4(), p4) < 1.) continue;
          float pt = jet.pt()*jet.undo_jec()*jet.jec();
          if(pt > 40. && fabs(jets[ijet].eta()) < 2.4) {
              njets_recoil++;
          }
      }

      if (!evt_isRealData){
          motherID = lepMotherID_v2(Lep(id,idx)).first;
      }

      //Fill Baby
      BabyTree->Fill();


  }

  return babyErrorStruct;

}

