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
  BabyTree->Branch("year"            , &year); // XXX
  BabyTree->Branch("evt_corrMET"       , &evt_corrMET); // XXX
  BabyTree->Branch("evt_rawMET"       , &evt_rawMET); // XXX
  BabyTree->Branch("evt_corrMETPhi"    , &evt_corrMETPhi); // XXX
  BabyTree->Branch("evt_rawMETPhi"    , &evt_rawMETPhi); // XXX
  BabyTree->Branch("evt_event"         , &evt_event); // XXX
  BabyTree->Branch("evt_lumiBlock"     , &evt_lumiBlock); // XXX
  BabyTree->Branch("evt_run"           , &evt_run); // XXX
  BabyTree->Branch("passes_met_filters", &passes_met_filters);  // XXX
  BabyTree->Branch("passes_any_trigger", &passes_any_trigger);  // XXX
  BabyTree->Branch("evt_isRealData"    , &evt_isRealData); // XXX
  BabyTree->Branch("scale1fb"          , &scale1fb); // XXX
  BabyTree->Branch("qscale"          , &qscale); // XXX
  BabyTree->Branch("njets"             , &njets); // XXX
  BabyTree->Branch("nisrMatch"             , &nisrMatch); // XXX
  BabyTree->Branch("weight_isr"             , &weight_isr); // XXX
  BabyTree->Branch("weight_pu"             , &weight_pu); // XXX
  BabyTree->Branch("nbtags"             , &nbtags); // XXX
  BabyTree->Branch("njets_recoil"             , &njets_recoil); // XXX
  BabyTree->Branch("recoil_jet_pt"             , &recoil_jet_pt); // XXX
  BabyTree->Branch("ht_SS"             , &ht_SS); // XXX
  BabyTree->Branch("nFOs_SS"           , &nFOs_SS); // XXX
  BabyTree->Branch("nvtx"              , &nvtx); // XXX
  BabyTree->Branch("trueNumInt"              , &trueNumInt); // XXX
  BabyTree->Branch("puweight"              , &puweight); // XXX

  BabyTree->Branch("nleptonicW"       , &nleptonicW       );
  BabyTree->Branch("nhadronicW"       , &nhadronicW       );
  BabyTree->Branch("nW"       , &nW       );

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
  BabyTree->Branch("lepnum"                      , &lepnum); // XXX

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
  BabyTree->Branch("mt_raw"                            , &mt_raw); // XXX
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
  BabyTree->Branch("HLT_Ele32_WPTight_Gsf"                       , &HLT_Ele32_WPTight_Gsf); // XXX

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

  BabyTree->Branch("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL"                 , &HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL); // XXX

  BabyTree->Branch("el_pass_trigsafenoiso", &el_pass_trigsafenoiso);
  BabyTree->Branch("el_pass_trigsafeiso", &el_pass_trigsafeiso);
  BabyTree->Branch("el_pass_v2trigsafenoiso", &el_pass_v2trigsafenoiso);
  BabyTree->Branch("el_pass_v2trigsafeiso", &el_pass_v2trigsafeiso);
  BabyTree->Branch("el_pass_miniiso", &el_pass_miniiso);
  BabyTree->Branch("el_pass_convvtx", &el_pass_convvtx);
  BabyTree->Branch("el_pass_expinner", &el_pass_expinner);
  BabyTree->Branch("el_pass_dxy", &el_pass_dxy);
  BabyTree->Branch("el_pass_dz", &el_pass_dz);
  BabyTree->Branch("el_pass_eta", &el_pass_eta);
  BabyTree->Branch("el_pass_sip", &el_pass_sip);
  BabyTree->Branch("el_pass_threecharge", &el_pass_threecharge);
  BabyTree->Branch("el_pass_mva", &el_pass_mva);
  BabyTree->Branch("el_loose", &el_loose);
  BabyTree->Branch("el_tight", &el_tight);
  BabyTree->Branch("el_loosewo_trigsafenoiso", &el_loosewo_trigsafenoiso);
  BabyTree->Branch("el_loosewo_miniiso", &el_loosewo_miniiso);
  BabyTree->Branch("el_loosewo_convvtx", &el_loosewo_convvtx);
  BabyTree->Branch("el_loosewo_expinner", &el_loosewo_expinner);
  BabyTree->Branch("el_loosewo_dxy", &el_loosewo_dxy);
  BabyTree->Branch("el_loosewo_dz", &el_loosewo_dz);
  BabyTree->Branch("el_loosewo_eta", &el_loosewo_eta);
  BabyTree->Branch("el_loosewo_sip", &el_loosewo_sip);
  BabyTree->Branch("el_loosewo_threecharge", &el_loosewo_threecharge);
  BabyTree->Branch("el_loosewo_mva", &el_loosewo_mva);

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
  setHLTBranch("HLT_Ele32_WPTight_Gsf_v",  true, HLT_Ele32_WPTight_Gsf);

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
    year = -1;
    evt_corrMET = -1.;
    evt_corrMETPhi = -1.;
    evt_rawMET = -1.;
    evt_rawMETPhi = -1.;
    evt_event = -1;
    evt_lumiBlock = -1;
    evt_run = -1;
    passes_met_filters = 0;
    passes_any_trigger = 0;
    evt_isRealData = 0;
    scale1fb    = -1.;
    qscale    = -1.;
    njets = -1;
    nisrMatch = -1;
    weight_isr = 1.;
    weight_pu = 1.;
    nbtags = -1;
    recoil_jet_pt = 0.;
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

    nleptonicW = 0;
    nhadronicW = 0;
    nW = 0;

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
    HLT_Ele32_WPTight_Gsf = 0;

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
    lepnum = -1;

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
    mt_raw = -1;
    ptrelv1 = -1;
    coneCorrPt = -1;
    miniiso = -1;
    close_jet_v5 = LorentzVector(0,0,0,0);
    ptratio_v5 = -1;
    isTriggerSafe = 0;
    isTriggerSafenoIso = 0;
    dilep_mass = -1.;

    el_pass_trigsafenoiso = 0;
    el_pass_trigsafeiso = 0;
    el_pass_v2trigsafenoiso = 0;
    el_pass_v2trigsafeiso = 0;
    el_pass_miniiso = 0;
    el_pass_convvtx = 0;
    el_pass_expinner = 0;
    el_pass_dxy = 0;
    el_pass_dz = 0;
    el_pass_eta = 0;
    el_pass_sip = 0;
    el_pass_threecharge = 0;
    el_pass_mva = 0;
    el_loose = 0;
    el_tight = 0;
    el_loosewo_trigsafenoiso = 0;
    el_loosewo_miniiso = 0;
    el_loosewo_convvtx = 0;
    el_loosewo_expinner = 0;
    el_loosewo_dxy = 0;
    el_loosewo_dz = 0;
    el_loosewo_eta = 0;
    el_loosewo_sip = 0;
    el_loosewo_threecharge = 0;
    el_loosewo_mva = 0;

}

////Main function
//csErr_t babyMaker::ProcessBaby(string filename_in, FactorizedJetCorrector* jetCorr, JetCorrectionUncertainty* jecUnc, int isFastsim){

//  //Initialize variables
//  InitBabyNtuple();

//  csErr_t babyErrorStruct;

//  //Preliminary stuff
//  if (verbose) std::cout << "--------------\nEVENT: " << tas::evt_run() << ":" << tas::evt_lumiBlock() << ":" << tas::evt_event() << "\n--------------" << std::endl;

//  // Fill trigger branches
//  fillTriggerBranches();
//  passes_any_trigger = (HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30 or
//          HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30 or
//          HLT_Ele17_CaloIdM_TrackIdM_PFJet30 or
//          HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30 or
//          HLT_Ele23_CaloIdM_TrackIdM_PFJet30 or
//          HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30 or
//          HLT_Ele8_CaloIdM_TrackIdM_PFJet30 or
//          HLT_IsoMu27 or
//          HLT_Mu17 or
//          HLT_Mu17_TrkIsoVVL or
//          HLT_Mu8 or
//          HLT_Mu8_TrkIsoVVL or
//          HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30);
//  if (not passes_any_trigger) return babyErrorStruct;

//  // nloose_el = 0; // num loose el with pt>10
//  // ntight_el = 0; // num tight el with pt>10
//  // vector<Lepton> leps;

//  //MET variables
//  // for old 09-04-17 tag QCD, use 3 to bypass any alternative MET collection selection
//  // and for newer tags (-19) use 2 to get EE noise fixed MET
//  bool isOldTag = filename_in.find("09-04-17") != std::string::npos;
//  int use_cleaned_met = (isOldTag ? 3 : 2);
//  pair<float,float> corrMETPair = getT1CHSMET_fromMINIAOD(jetCorr, NULL, 0, false, use_cleaned_met);
//  evt_corrMET    = corrMETPair.first;
//  evt_corrMETPhi = corrMETPair.second;

//  for (int ilep = 0; ilep < els_p4().size(); ilep++) {

//      float pt = els_p4()[ilep].pt();
//      if (pt < 25.) continue;
//      Lepton lep = Lepton(-11*els_charge()[ilep],ilep);

//      InitLeptonBranches();

//      id = lep.id();
//      idx = lep.idx();
//      p4 = lep.p4();
//      p4_pt = lep.pt();
//      p4_eta = lep.eta();
//      mt = MT(p4_pt, p4.phi(), evt_corrMET, evt_corrMETPhi);
//      coneCorrPt = getConeCorrPt(id,idx);

//        // SS_fo_looseMVA_v5
//      el_pass_trigsafenoiso = isTriggerSafenoIso_v1(ilep);
//      el_pass_trigsafeiso = isTriggerSafe_v1(ilep);
//      el_pass_v2trigsafenoiso = isTriggerSafenoIso_v2(ilep);
//      el_pass_v2trigsafeiso = isTriggerSafe_v2(ilep);
//      el_pass_miniiso = !(elMiniRelIsoCMS3_EA(ilep,gconf.ea_version) >= 0.40);
//      el_pass_convvtx = !els_conv_vtx_flag().at(ilep);
//      el_pass_expinner = !(els_exp_innerlayers().at(ilep) > 0);
//      el_pass_dxy = !(fabs(els_dxyPV().at(ilep)) >= 0.05);
//      el_pass_dz = !(fabs(els_dzPV().at(ilep)) >= 0.1);
//      el_pass_eta = !(fabs(els_etaSC().at(ilep)) > 2.5);
//      el_pass_sip = !(fabs(els_ip3d().at(ilep))/els_ip3derr().at(ilep) >= 4);
//      el_pass_threecharge = !(threeChargeAgree(ilep)==0);
//      el_pass_mva = false;
//      float aeta = fabs(els_etaSC().at(ilep));
//      float disc = getMVAoutput(ilep, true);

//      if (gconf.year == 2016) {
//          if ((aeta >= 0.8 && aeta <= 1.479)) el_pass_mva = (disc > -0.91);
//          if (aeta < 0.8) el_pass_mva = (disc > -0.85);
//          if (aeta > 1.479) el_pass_mva = (disc > -0.83);
//      } else if (gconf.year == 2017) {
//          if (aeta < 0.8) el_pass_mva = (disc > -0.64);
//          if ((aeta >= 0.8 && aeta <= 1.479)) el_pass_mva = (disc > -0.775);
//          if (aeta > 1.479) el_pass_mva = (disc > -0.733);
//      }

//      el_loose = el_pass_trigsafenoiso && el_pass_miniiso && el_pass_convvtx && el_pass_expinner && el_pass_dxy && el_pass_dz && el_pass_eta && el_pass_sip && el_pass_threecharge && el_pass_mva;
//      el_tight = isGoodLepton(11,ilep);

//      el_loosewo_trigsafenoiso =                          el_pass_miniiso && el_pass_convvtx && el_pass_expinner && el_pass_dxy && el_pass_dz && el_pass_eta && el_pass_sip && el_pass_threecharge && el_pass_mva;
//      el_loosewo_miniiso       = el_pass_trigsafenoiso &&                    el_pass_convvtx && el_pass_expinner && el_pass_dxy && el_pass_dz && el_pass_eta && el_pass_sip && el_pass_threecharge && el_pass_mva;
//      el_loosewo_convvtx       = el_pass_trigsafenoiso && el_pass_miniiso &&                    el_pass_expinner && el_pass_dxy && el_pass_dz && el_pass_eta && el_pass_sip && el_pass_threecharge && el_pass_mva;
//      el_loosewo_expinner      = el_pass_trigsafenoiso && el_pass_miniiso && el_pass_convvtx &&                     el_pass_dxy && el_pass_dz && el_pass_eta && el_pass_sip && el_pass_threecharge && el_pass_mva;
//      el_loosewo_dxy           = el_pass_trigsafenoiso && el_pass_miniiso && el_pass_convvtx && el_pass_expinner &&                el_pass_dz && el_pass_eta && el_pass_sip && el_pass_threecharge && el_pass_mva;
//      el_loosewo_dz            = el_pass_trigsafenoiso && el_pass_miniiso && el_pass_convvtx && el_pass_expinner && el_pass_dxy &&               el_pass_eta && el_pass_sip && el_pass_threecharge && el_pass_mva;
//      el_loosewo_eta           = el_pass_trigsafenoiso && el_pass_miniiso && el_pass_convvtx && el_pass_expinner && el_pass_dxy && el_pass_dz &&                el_pass_sip && el_pass_threecharge && el_pass_mva;
//      el_loosewo_sip           = el_pass_trigsafenoiso && el_pass_miniiso && el_pass_convvtx && el_pass_expinner && el_pass_dxy && el_pass_dz && el_pass_eta &&                el_pass_threecharge && el_pass_mva;
//      el_loosewo_threecharge   = el_pass_trigsafenoiso && el_pass_miniiso && el_pass_convvtx && el_pass_expinner && el_pass_dxy && el_pass_dz && el_pass_eta && el_pass_sip &&                        el_pass_mva;
//      el_loosewo_mva           = el_pass_trigsafenoiso && el_pass_miniiso && el_pass_convvtx && el_pass_expinner && el_pass_dxy && el_pass_dz && el_pass_eta && el_pass_sip && el_pass_threecharge               ;

//      //Fill Baby
//      BabyTree->Fill();

//  }

//  return babyErrorStruct;
//}

//Main function
csErr_t babyMaker::ProcessBaby(string filename_in, FactorizedJetCorrector* jetCorr, JetCorrectionUncertainty* jecUnc, int isFastsim){

  //Initialize variables
  InitBabyNtuple();

  csErr_t babyErrorStruct;

  //Preliminary stuff
  if (verbose) std::cout << "--------------\nEVENT: " << tas::evt_run() << ":" << tas::evt_lumiBlock() << ":" << tas::evt_event() << "\n--------------" << std::endl;

  // FIXME FIXME
  bool make_isr_baby = false;

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

  if (make_isr_baby) {
      // require exactly one tight lepton
      // but could still have another loose, or not
      if (ntight_mu + ntight_el != 1) return babyErrorStruct;
  }

  if (!make_isr_baby) {
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
  } else {
      // FIXME need to require single lepton trigger
      // and also switch doublemu -> single mu!
      setHLTBranch("HLT_Ele32_WPTight_Gsf_v",  true, HLT_Ele32_WPTight_Gsf);
      setHLTBranch("HLT_IsoMu27_v"         , true, HLT_IsoMu27         );
      if (not HLT_Ele32_WPTight_Gsf and not HLT_IsoMu27) return babyErrorStruct;
  }

  // MC stuff and filters
  scale1fb = 1;
  year = gconf.year;
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
  passes_met_filters = passesMETfiltersRun2(evt_isRealData);

  // Jets
  vector<Jet> jets = SSJetsCalculator(jetCorr, 1).first;
  njets = jets.size();
  nbtags = 0;
  ht_SS = 0;
  vector <LorentzVector> jets_p4;
  for (unsigned int ijet = 0; ijet < jets.size(); ijet++){
      auto jet = jets[ijet];
      float corr = jet.undo_jec()*jet.jec();
      jets_p4.push_back(jet.p4()*corr);
      float pt = jet.pt()*corr;
      ht_SS += pt;
      float disc = jet.disc();
      if (disc > gconf.btag_disc_wp) nbtags += 1;
  }
  if (make_isr_baby) {
      // require at least one b to speed things up
      if (nbtags == 0) return babyErrorStruct;
  }
  if (make_isr_baby and !evt_isRealData) {
      nisrMatch = get_nisrMatch(jets_p4);
      weight_isr = isrWeight(year,nisrMatch,10);
      weight_pu = getTruePUw(year,trueNumInt);
      nleptonicW = 0;
      nW = 0;
      nhadronicW = 0;
      for (unsigned int igen = 0; igen < tas::genps_p4().size(); igen++){
          int id = tas::genps_id()[igen];
          if (abs(id) != 24) continue;
          int smid = tas::genps_id_simplemother()[igen];
          if (abs(smid) == 24) continue;
          int stat = tas::genps_status()[igen];
          if (stat != 22 && stat != 52) continue;
          nW++;
      }
      for (unsigned int igen = 0; igen < tas::genps_p4().size(); igen++){
          int id = tas::genps_id()[igen];
          int mid = tas::genps_id_mother()[igen];
          if (abs(id) != 12 && abs(id) != 14 && abs(id) != 16) continue;
          if (abs(mid) != 24) continue;
          if (!(tas::genps_isPromptFinalState()[igen])) continue;
          nleptonicW++;
      }
      nhadronicW = nW - nleptonicW;
  }

  //MET variables
  // for old 09-04-17 tag QCD, use 3 to bypass any alternative MET collection selection
  // and for newer tags (-19) use 2 to get EE noise fixed MET
  bool isOldTag = filename_in.find("09-04-17") != std::string::npos;
  int use_cleaned_met = (isOldTag ? 3 : 2);
  pair<float,float> corrMETPair = getT1CHSMET_fromMINIAOD(jetCorr, NULL, 0, false, use_cleaned_met);
  evt_corrMET    = corrMETPair.first;
  evt_corrMETPhi = corrMETPair.second;
  evt_rawMET    = tas::evt_pfmet_raw();
  evt_rawMETPhi = tas::evt_pfmetPhi_raw();

  // Now for each loose loose lepton, fill branches
  // And if one of the loose leptons is also tight AND has another tight lepton forming a Z with it, then flag this as a Z and store Z stuff

  bool event_maybe_has_z = (ntight_el>=2) or (ntight_mu>=2);

  for (int ilep = 0; ilep < leps.size(); ilep++) {

      InitLeptonBranches();

      Lepton lep = leps[ilep];

      lepnum = ilep;
      id = lep.id();
      idx = lep.idx();
      p4 = lep.p4();
      p4_pt = lep.pt();
      p4_eta = lep.eta();
      mt = MT(p4_pt, p4.phi(), evt_corrMET, evt_corrMETPhi);
      mt_raw = MT(p4_pt, p4.phi(), evt_rawMET, evt_rawMETPhi);

      if (make_isr_baby) {

          passes_SS_tight_v6 = lep.idlevel() == 2;
          passes_SS_fo_v6 = lep.idlevel() >= 1;
          passes_SS_fo_looseMVA_v6 = lep.idlevel() >= 1;
          if (not passes_SS_tight_v6) continue;

      } else {


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
          recoil_jet_pt = 0.;
          for(unsigned int ijet=0; ijet < jets.size(); ijet++)  {
              auto jet = jets[ijet];
              if(ROOT::Math::VectorUtil::DeltaR(jet.p4(), p4) < 1.) continue;
              float pt = jet.pt()*jet.undo_jec()*jet.jec();
              if (fabs(jets[ijet].eta()) > 2.4) continue;
              if (pt > 40.) {
                  njets_recoil++;
                  recoil_jet_pt = pt;
              }
          }

          if (!evt_isRealData){
              motherID = lepMotherID_v2(Lep(id,idx)).first;
          }

      }


      // // debug
      // if (!passes_SS_tight_v6 and coneCorrPt<25 and abs(id)==11) {
      //     float mvant = els_VIDNonTrigMvaValue().at(idx);
      //     float mvagp = els_VIDSpring16GPMvaValue().at(idx);
      //     std::cout <<  " mvant: " << mvant <<  " mvagp: " << mvagp <<  " p4_pt: " << p4_pt <<  " p4_eta: " << p4_eta <<  std::endl;
      // }

      //Fill Baby
      BabyTree->Fill();


  }

  return babyErrorStruct;

}

