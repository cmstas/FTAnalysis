#include "helper_babymaker.h"
#include "CORE/Tools/btagsf/BTagCalibrationStandalone.h"
#include "CORE/Tools/jetcorr/Utilities.icc"
#include "CORE/Tools/jetcorr/SimpleJetCorrectionUncertainty.h"
#include "CORE/Tools/jetcorr/JetCorrectionUncertainty.h"

using namespace tas;

const bool applyBtagSFs = true;

//Main functions
void babyMaker::MakeBabyNtuple(const char* output_name, int isFastsim){

  //Create Baby
  BabyFile = new TFile(Form("%s/%s", path.Data(), output_name), "RECREATE");
  BabyFile->cd();
  BabyTree = new TTree("t", "SS2015 Baby Ntuple");

  //Define Branches
  BabyTree->Branch("lep1_isPrompt"                                           , &lep1_isPrompt                                           );
  BabyTree->Branch("lep1_isDirectPrompt"                                     , &lep1_isDirectPrompt                                     );
  BabyTree->Branch("lep1_isStat3"                                            , &lep1_isStat3                                            );
  BabyTree->Branch("lep2_isPrompt"                                           , &lep2_isPrompt                                           );
  BabyTree->Branch("lep2_isDirectPrompt"                                     , &lep2_isDirectPrompt                                     );
  BabyTree->Branch("lep2_isStat3"                                            , &lep2_isStat3                                            );
  BabyTree->Branch("met"                                                     , &met                                                     );
  BabyTree->Branch("metPhi"                                                  , &metPhi                                                  );
  BabyTree->Branch("rawmet"                                                  , &rawmet                                                  );
  BabyTree->Branch("rawmetPhi"                                               , &rawmetPhi                                               );
  BabyTree->Branch("calomet"                                                  , &calomet                                                  );
  BabyTree->Branch("calometPhi"                                               , &calometPhi                                               );
  BabyTree->Branch("event"                                                   , &event                                                   );
  BabyTree->Branch("lumi"                                                    , &lumi                                                    );
  BabyTree->Branch("run"                                                     , &run                                                     );
  BabyTree->Branch("filt_csc"                                                , &filt_csc                                                );
  BabyTree->Branch("filt_hbhe"                                               , &filt_hbhe                                               );
  BabyTree->Branch("filt_hcallaser"                                          , &filt_hcallaser                                          );
  BabyTree->Branch("filt_ecaltp"                                             , &filt_ecaltp                                             );
  BabyTree->Branch("filt_trkfail"                                            , &filt_trkfail                                            );
  BabyTree->Branch("is_real_data"                                            , &is_real_data                                            );
  BabyTree->Branch("scale1fb"                                                , &scale1fb                                                );
  BabyTree->Branch("xsec"                                                    , &xsec                                                    );
  BabyTree->Branch("xsec_ps"                                                    , &xsec_ps                                                    );
  BabyTree->Branch("sparmNames"                                              , &sparmNames                                              );
  BabyTree->Branch("sparms"                                                  , &sparms                                                  );
  BabyTree->Branch("xsec_error"                                              , &xsec_error                                              );
  BabyTree->Branch("kfactor"                                                 , &kfactor                                                 );
  BabyTree->Branch("gen_met"                                                 , &gen_met                                                 );
  BabyTree->Branch("genweights"                                              , &genweights                                              );
  BabyTree->Branch("genweightsID"                                            , &genweightsID                                            );
  BabyTree->Branch("gen_met_phi"                                             , &gen_met_phi                                             );
  BabyTree->Branch("njets"                                                   , &njets                                                   );
  BabyTree->Branch("njets_raw"                                               , &njets_raw                                               );
  BabyTree->Branch("hyp_class"                                               , &hyp_class                                               );
  BabyTree->Branch("lep1_p4"                                                 , &lep1_p4                                                 );
  BabyTree->Branch("lep2_p4"                                                 , &lep2_p4                                                 );
  BabyTree->Branch("ht_raw"                                                  , &ht_raw                                                  );
  BabyTree->Branch("ht"                                                      , &ht                                                      );
  BabyTree->Branch("lep1_motherID"                                           , &lep1_motherID                                           );
  BabyTree->Branch("lep2_motherID"                                           , &lep2_motherID                                           );
  BabyTree->Branch("lep1_mc_id"                                              , &lep1_mc_id                                              );
  BabyTree->Branch("lep2_mc_id"                                              , &lep2_mc_id                                              );
  BabyTree->Branch("lep1_mc_motherid"                                              , &lep1_mc_motherid                                              );
  BabyTree->Branch("lep2_mc_motherid"                                              , &lep2_mc_motherid                                              );
  BabyTree->Branch("lep1_id"                                                 , &lep1_id                                                 );
  BabyTree->Branch("lep2_id"                                                 , &lep2_id                                                 );
  BabyTree->Branch("lep1_coneCorrPt"                                         , &lep1_coneCorrPt                                         );
  BabyTree->Branch("lep2_coneCorrPt"                                         , &lep2_coneCorrPt                                         );
  BabyTree->Branch("lep3_coneCorrPt"                                         , &lep3_coneCorrPt                                         );
  BabyTree->Branch("lep1_idx"                                                , &lep1_idx                                                );
  BabyTree->Branch("lep2_idx"                                                , &lep2_idx                                                );
  BabyTree->Branch("jets"                                                    , &jets                                                    );
  BabyTree->Branch("btags_flavor"                                              , &btags_flavor                                              );
  BabyTree->Branch("btags_disc"                                              , &btags_disc                                              );
  // BabyTree->Branch("btags_disc_mva"                                          , &btags_disc_mva                                          );
  // BabyTree->Branch("btags_disc_ivf"                                          , &btags_disc_ivf                                          );
  BabyTree->Branch("jets_flavor"                                               , &jets_flavor                                               );
  BabyTree->Branch("jets_disc"                                               , &jets_disc                                               );
  // BabyTree->Branch("jets_disc_mva"                                           , &jets_disc_mva                                           );
  // BabyTree->Branch("jets_disc_ivf"                                           , &jets_disc_ivf                                           );
  // BabyTree->Branch("jets_JEC"                                                , &jets_JEC                                                );
  // BabyTree->Branch("btags_JEC"                                               , &btags_JEC                                               );
  // BabyTree->Branch("jets_undoJEC"                                            , &jets_undoJEC                                            );
  // BabyTree->Branch("btags_undoJEC"                                           , &btags_undoJEC                                           );
  BabyTree->Branch("btags_unc"                                               , &btags_unc                                               );
  BabyTree->Branch("btags_eff"                                               , &btags_eff                                               );
  BabyTree->Branch("btags_effpt"                                               , &btags_effpt                                               );
  BabyTree->Branch("btags_sf"                                               , &btags_sf                                               );
  BabyTree->Branch("jets_unc"                                                , &jets_unc                                                );
  BabyTree->Branch("btags"                                                   , &btags                                                   );
  BabyTree->Branch("nbtags"                                                  , &nbtags                                                  );
  BabyTree->Branch("nbtags_raw"                                              , &nbtags_raw                                              );
  BabyTree->Branch("sf_dilepTrig_hpt"                                        , &sf_dilepTrig_hpt                                        );
  BabyTree->Branch("sf_dilepTrig_lpt"                                        , &sf_dilepTrig_lpt                                        );
  BabyTree->Branch("sf_dilepTrig_vlpt"                                       , &sf_dilepTrig_vlpt                                       );
  BabyTree->Branch("hyp_type"                                                , &hyp_type                                                );
  BabyTree->Branch("sf_dilep_eff"                                            , &sf_dilep_eff                                            );
  BabyTree->Branch("mt"                                                      , &mt                                                      );
  BabyTree->Branch("mt_l2"                                                   , &mt_l2                                                   );
  BabyTree->Branch("mt2"                                                     , &mt2                                                     );
  BabyTree->Branch("mtmin"                                                   , &mtmin                                                   );
  BabyTree->Branch("lep1_id_gen"                                             , &lep1_id_gen                                             );
  BabyTree->Branch("lep2_id_gen"                                             , &lep2_id_gen                                             );
  BabyTree->Branch("lep1_p4_gen"                                             , &lep1_p4_gen                                             );
  BabyTree->Branch("lep2_p4_gen"                                             , &lep2_p4_gen                                             );
  BabyTree->Branch("lep3_id"                                                 , &lep3_id                                                 );
  BabyTree->Branch("lep3_idx"                                                , &lep3_idx                                                );
  BabyTree->Branch("lep3_p4"                                                 , &lep3_p4                                                 );
  BabyTree->Branch("lep3_quality"                                            , &lep3_quality                                            );
  BabyTree->Branch("lep4_id"                                                 , &lep4_id                                                 );
  BabyTree->Branch("lep4_idx"                                                , &lep4_idx                                                );
  BabyTree->Branch("lep4_p4"                                                 , &lep4_p4                                                 );
  BabyTree->Branch("lep1_iso"                                                , &lep1_iso                                                );
  BabyTree->Branch("lep2_iso"                                                , &lep2_iso                                                );
  BabyTree->Branch("lep1_tkIso"                                              , &lep1_tkIso                                              );
  BabyTree->Branch("lep2_tkIso"                                              , &lep2_tkIso                                              );
  BabyTree->Branch("dilep_p4"                                                , &dilep_p4                                                );
  BabyTree->Branch("ncharginos"                                                , &ncharginos                                                );
  BabyTree->Branch("higgs_mass"                                                , &higgs_mass                                                );
  BabyTree->Branch("genps_p4"                                                , &genps_p4                                                );
  BabyTree->Branch("genps_id"                                                , &genps_id                                                );
  BabyTree->Branch("genps_id_mother"                                         , &genps_id_mother                                         );
  BabyTree->Branch("genps_idx_mother"                                        , &genps_idx_mother                                        );
  BabyTree->Branch("genps_status"                                            , &genps_status                                            );
  BabyTree->Branch("genps_id_grandma"                                        , &genps_id_grandma                                        );

  BabyTree->Branch("all_leps_p4"                                        , &all_leps_p4                                        );
  BabyTree->Branch("all_leps_id"                                        , &all_leps_id                                        );
  BabyTree->Branch("all_leps_veto"                                        , &all_leps_veto                                        );
  BabyTree->Branch("all_leps_mc3idx"                                        , &all_leps_mc3idx                                        );

  BabyTree->Branch("lep1_passes_id"                                          , &lep1_passes_id                                          );
  BabyTree->Branch("lep2_passes_id"                                          , &lep2_passes_id                                          );
  BabyTree->Branch("lep3_passes_id"                                          , &lep3_passes_id                                          );
  BabyTree->Branch("lep4_passes_id"                                          , &lep4_passes_id                                          );
  BabyTree->Branch("lep1_tight"                                              , &lep1_tight                                              );
  BabyTree->Branch("lep1_veto"                                               , &lep1_veto                                               );
  BabyTree->Branch("lep1_fo"                                                 , &lep1_fo                                                 );
  BabyTree->Branch("lep2_tight"                                              , &lep2_tight                                              );
  BabyTree->Branch("lep2_veto"                                               , &lep2_veto                                               );
  BabyTree->Branch("lep2_fo"                                                 , &lep2_fo                                                 );
  BabyTree->Branch("lep3_tight"                                              , &lep3_tight                                              );
  BabyTree->Branch("lep3_veto"                                               , &lep3_veto                                               );
  BabyTree->Branch("lep3_fo"                                                 , &lep3_fo                                                 );
  BabyTree->Branch("lep4_tight"                                              , &lep4_tight                                              );
  BabyTree->Branch("lep4_veto"                                               , &lep4_veto                                               );
  BabyTree->Branch("lep4_fo"                                                 , &lep4_fo                                                 );
  BabyTree->Branch("lep1_dxyPV"                                              , &lep1_dxyPV                                              );
  BabyTree->Branch("lep2_dxyPV"                                              , &lep2_dxyPV                                              );
  BabyTree->Branch("lep1_dZ"                                                 , &lep1_dZ                                                 );
  BabyTree->Branch("lep2_dZ"                                                 , &lep2_dZ                                                 );
  BabyTree->Branch("lep1_d0_err"                                             , &lep1_d0_err                                             );
  BabyTree->Branch("lep2_d0_err"                                             , &lep2_d0_err                                             );
  BabyTree->Branch("lep1_ip3d"                                               , &lep1_ip3d                                               );
  BabyTree->Branch("lep2_ip3d"                                               , &lep2_ip3d                                               );
  BabyTree->Branch("lep1_MVA"                                                , &lep1_MVA                                                );
  BabyTree->Branch("lep2_MVA"                                                , &lep2_MVA                                                );
  BabyTree->Branch("lep1_MVA_miniaod"                                        , &lep1_MVA_miniaod                                        );
  BabyTree->Branch("lep2_MVA_miniaod"                                        , &lep2_MVA_miniaod                                        );
  BabyTree->Branch("lep1_ip3d_err"                                           , &lep1_ip3d_err                                           );
  BabyTree->Branch("lep2_ip3d_err"                                           , &lep2_ip3d_err                                           );
  BabyTree->Branch("nVetoElectrons7"                                         , &nVetoElectrons7                                         );
  BabyTree->Branch("nVetoElectrons10"                                        , &nVetoElectrons10                                        );
  BabyTree->Branch("nVetoElectrons25"                                        , &nVetoElectrons25                                        );
  BabyTree->Branch("nVetoMuons5"                                             , &nVetoMuons5                                             );
  BabyTree->Branch("nVetoMuons10"                                            , &nVetoMuons10                                            );
  BabyTree->Branch("nVetoMuons25"                                            , &nVetoMuons25                                            );
  BabyTree->Branch("filename"                                                , &filename                                                );
  BabyTree->Branch("lep1_ptrel_v0"                                           , &lep1_ptrel_v0                                           );
  BabyTree->Branch("lep1_ptrel_v1"                                           , &lep1_ptrel_v1                                           );
  BabyTree->Branch("lep2_ptrel_v0"                                           , &lep2_ptrel_v0                                           );
  BabyTree->Branch("lep2_ptrel_v1"                                           , &lep2_ptrel_v1                                           );
  BabyTree->Branch("lep1_miniIso"                                            , &lep1_miniIso                                            );
  BabyTree->Branch("lep2_miniIso"                                            , &lep2_miniIso                                            );
  // BabyTree->Branch("jet_close_lep1"                                          , &jet_close_lep1                                          );
  // BabyTree->Branch("jet_close_lep2"                                          , &jet_close_lep2                                          );
  // BabyTree->Branch("eleID_kfhits"                                            , &eleID_kfhits                                            );
  // BabyTree->Branch("eleID_oldsigmaietaieta"                                  , &eleID_oldsigmaietaieta                                  );
  // BabyTree->Branch("eleID_oldsigmaiphiiphi"                                  , &eleID_oldsigmaiphiiphi                                  );
  // BabyTree->Branch("eleID_oldcircularity"                                    , &eleID_oldcircularity                                    );
  // BabyTree->Branch("eleID_oldr9"                                             , &eleID_oldr9                                             );
  // BabyTree->Branch("eleID_scletawidth"                                       , &eleID_scletawidth                                       );
  // BabyTree->Branch("eleID_sclphiwidth"                                       , &eleID_sclphiwidth                                       );
  // BabyTree->Branch("eleID_he"                                                , &eleID_he                                                );
  // BabyTree->Branch("eleID_psEoverEraw"                                       , &eleID_psEoverEraw                                       );
  // BabyTree->Branch("eleID_kfchi2"                                            , &eleID_kfchi2                                            );
  // BabyTree->Branch("eleID_chi2_hits"                                         , &eleID_chi2_hits                                         );
  // BabyTree->Branch("eleID_fbrem"                                             , &eleID_fbrem                                             );
  // BabyTree->Branch("eleID_ep"                                                , &eleID_ep                                                );
  // BabyTree->Branch("eleID_eelepout"                                          , &eleID_eelepout                                          );
  // BabyTree->Branch("eleID_IoEmIop"                                           , &eleID_IoEmIop                                           );
  // BabyTree->Branch("eleID_deltaetain"                                        , &eleID_deltaetain                                        );
  // BabyTree->Branch("eleID_deltaphiin"                                        , &eleID_deltaphiin                                        );
  // BabyTree->Branch("eleID_deltaetaseed"                                      , &eleID_deltaetaseed                                      );
  // BabyTree->Branch("eleID_pT"                                                , &eleID_pT                                                );
  // BabyTree->Branch("eleID_isbarrel"                                          , &eleID_isbarrel                                          );
  // BabyTree->Branch("eleID_isendcap"                                          , &eleID_isendcap                                          );
  // BabyTree->Branch("eleID_scl_eta"                                           , &eleID_scl_eta                                           );
  // BabyTree->Branch("muID_dzPV"                                               , &muID_dzPV                                               );
  // BabyTree->Branch("muID_ptSig"                                              , &muID_ptSig                                              );
  // BabyTree->Branch("muID_ip3dSig"                                            , &muID_ip3dSig                                            );
  // BabyTree->Branch("muID_medMuonPOG"                                         , &muID_medMuonPOG                                         );
  // BabyTree->Branch("muID_pt"                                                 , &muID_pt                                                 );
  // BabyTree->Branch("muID_eta"                                                , &muID_eta                                                );
  BabyTree->Branch("trueNumInt"                                              , &trueNumInt                                              );
  // BabyTree->Branch("nPUvertices"                                             , &nPUvertices                                             );
  BabyTree->Branch("nGoodVertices"                                           , &nGoodVertices                                           );
  BabyTree->Branch("lep1_trigMatch_noIsoReq"                                 , &lep1_trigMatch_noIsoReq                                 );
  BabyTree->Branch("lep1_trigMatch_isoReq"                                   , &lep1_trigMatch_isoReq                                   );
  BabyTree->Branch("lep2_trigMatch_noIsoReq"                                 , &lep2_trigMatch_noIsoReq                                 );
  BabyTree->Branch("lep2_trigMatch_isoReq"                                   , &lep2_trigMatch_isoReq                                   );
  // BabyTree->Branch("met3p0"                                                  , &met3p0                                                  );
  // BabyTree->Branch("jet_pt"                                                  , &jet_pt                                                  );
  // BabyTree->Branch("metphi3p0"                                               , &metphi3p0                                               );
  BabyTree->Branch("passes_met_filters"                                      , &passes_met_filters                                      );
  BabyTree->Branch("evt_egclean_pfmet"                                      , &evt_egclean_pfmet                                      );
  BabyTree->Branch("evt_muegclean_pfmet"                                      , &evt_muegclean_pfmet                                      );
  BabyTree->Branch("evt_muegcleanfix_pfmet"                                      , &evt_muegcleanfix_pfmet                                      );
  BabyTree->Branch("evt_uncorr_pfmet"                                      , &evt_uncorr_pfmet                                      );
  BabyTree->Branch("filt_noBadMuons"                                      , &filt_noBadMuons                                      );
  BabyTree->Branch("filt_duplicateMuons"                                      , &filt_duplicateMuons                                      );
  BabyTree->Branch("filt_badMuons"                                      , &filt_badMuons                                      );
  BabyTree->Branch("failsRA2Filter"                                      , &failsRA2Filter                                      );
  BabyTree->Branch("madeExtraZ"                                              , &madeExtraZ                                              );
  BabyTree->Branch("madeExtraG"                                              , &madeExtraG                                              );
  BabyTree->Branch("lep3_mcid"                                               , &lep3_mcid                                               );
  BabyTree->Branch("lep3_mc_motherid"                                               , &lep3_mc_motherid                                               );
  BabyTree->Branch("lep3_mc3idx"                                              , &lep3_mc3idx                                              );
  BabyTree->Branch("lep3_motherID"                                              , &lep3_motherID                                              );
  BabyTree->Branch("lep4_mcid"                                               , &lep4_mcid                                               );
  BabyTree->Branch("lep4_mcidx"                                              , &lep4_mcidx                                              );
  // BabyTree->Branch("mostJets"                                                , &mostJets                                                );
  // BabyTree->Branch("mostJets_rawp4"                                          , &mostJets_rawp4                                          );
  // BabyTree->Branch("mostJets_disc"                                           , &mostJets_disc                                           );
  // BabyTree->Branch("mostJets_unc"                                            , &mostJets_unc                                            );
  // BabyTree->Branch("mostJets_JEC"                                            , &mostJets_JEC                                            );
  // BabyTree->Branch("mostJets_undoJEC"                                        , &mostJets_undoJEC                                        );
  // BabyTree->Branch("mostJets_passID"                                         , &mostJets_passID                                         );
  // BabyTree->Branch("mostJets_passCleaning"                                   , &mostJets_passCleaning                                   );
  BabyTree->Branch("njets_unc_up"                                            , &njets_unc_up                                            );
  BabyTree->Branch("njets_unc_dn"                                            , &njets_unc_dn                                            );
  BabyTree->Branch("ht_unc_up"                                               , &ht_unc_up                                               );
  BabyTree->Branch("ht_unc_dn"                                               , &ht_unc_dn                                               );
  BabyTree->Branch("nbtags_unc_up"                                           , &nbtags_unc_up                                           );
  BabyTree->Branch("nbtags_unc_dn"                                           , &nbtags_unc_dn                                           );
  BabyTree->Branch("met_unc_up"                                              , &met_unc_up                                              );
  BabyTree->Branch("met_unc_dn"                                              , &met_unc_dn                                              );
  BabyTree->Branch("metPhi_unc_up"                                           , &metPhi_unc_up                                           );
  BabyTree->Branch("metPhi_unc_dn"                                           , &metPhi_unc_dn                                           );
  BabyTree->Branch("passedFilterList"                                        , &passedFilterList                                        );
  BabyTree->Branch("lep2_genps_isHardProcess"                                , &lep2_genps_isHardProcess                                );
  BabyTree->Branch("lep2_genps_fromHardProcessFinalState"                    , &lep2_genps_fromHardProcessFinalState                    );
  BabyTree->Branch("lep2_genps_fromHardProcessDecayed"                       , &lep2_genps_fromHardProcessDecayed                       );
  BabyTree->Branch("lep2_genps_isDirectHardProcessTauDecayProductFinalState" , &lep2_genps_isDirectHardProcessTauDecayProductFinalState );
  BabyTree->Branch("lep2_genps_fromHardProcessBeforeFSR"                     , &lep2_genps_fromHardProcessBeforeFSR                     );
  BabyTree->Branch("lep2_genps_isLastCopy"                                   , &lep2_genps_isLastCopy                                   );
  BabyTree->Branch("lep2_genps_isLastCopyBeforeFSR"                          , &lep2_genps_isLastCopyBeforeFSR                          );
  BabyTree->Branch("lep1_genps_isHardProcess"                                , &lep1_genps_isHardProcess                                );
  BabyTree->Branch("lep1_genps_fromHardProcessFinalState"                    , &lep1_genps_fromHardProcessFinalState                    );
  BabyTree->Branch("lep1_genps_fromHardProcessDecayed"                       , &lep1_genps_fromHardProcessDecayed                       );
  BabyTree->Branch("lep1_genps_isDirectHardProcessTauDecayProductFinalState" , &lep1_genps_isDirectHardProcessTauDecayProductFinalState );
  BabyTree->Branch("lep1_genps_fromHardProcessBeforeFSR"                     , &lep1_genps_fromHardProcessBeforeFSR                     );
  BabyTree->Branch("lep1_genps_isLastCopy"                                   , &lep1_genps_isLastCopy                                   );
  BabyTree->Branch("lep1_genps_isLastCopyBeforeFSR"                          , &lep1_genps_isLastCopyBeforeFSR                          );
  BabyTree->Branch("lep1_mc3idx"                                             , &lep1_mc3idx                                             );
  BabyTree->Branch("lep2_mc3idx"                                             , &lep2_mc3idx                                             );
  BabyTree->Branch("lep1_el_conv_vtx_flag"   , &lep1_el_conv_vtx_flag   );
  BabyTree->Branch("lep2_el_conv_vtx_flag"   , &lep2_el_conv_vtx_flag   );
  BabyTree->Branch("lep1_el_exp_innerlayers" , &lep1_el_exp_innerlayers );
  BabyTree->Branch("lep1_mu_ptErr" , &lep1_mu_ptErr );
  BabyTree->Branch("lep2_mu_ptErr" , &lep2_mu_ptErr );
  BabyTree->Branch("lep1_el_threeChargeAgree" , &lep1_el_threeChargeAgree );
  BabyTree->Branch("lep2_el_threeChargeAgree" , &lep2_el_threeChargeAgree );
  BabyTree->Branch("lep2_el_exp_innerlayers" , &lep2_el_exp_innerlayers );
  BabyTree->Branch("lep1_el_etaSC" , &lep1_el_etaSC );
  BabyTree->Branch("lep2_el_etaSC" , &lep2_el_etaSC );
  BabyTree->Branch("lep1_nPixelMiss"   , &lep1_nPixelMiss   );
  BabyTree->Branch("lep2_nPixelMiss"   , &lep2_nPixelMiss   );
  BabyTree->Branch("lep1_tightCharge"   , &lep1_tightCharge   );
  BabyTree->Branch("lep2_tightCharge"   , &lep2_tightCharge   );
  BabyTree->Branch("lep1_isTrigSafeNoIsov1"  , &lep1_isTrigSafeNoIsov1  );
  BabyTree->Branch("lep1_isTrigSafev1"       , &lep1_isTrigSafev1       );
  BabyTree->Branch("lep2_isTrigSafeNoIsov1"  , &lep2_isTrigSafeNoIsov1  );
  BabyTree->Branch("lep2_isTrigSafev1"       , &lep2_isTrigSafev1       );
  BabyTree->Branch("lep1_tightMuonPOG"  , &lep1_tightMuonPOG  );
  BabyTree->Branch("lep1_mediumMuonPOG"  , &lep1_mediumMuonPOG  );
  BabyTree->Branch("lep1_looseMuonPOG"  , &lep1_looseMuonPOG  );
  BabyTree->Branch("lep2_tightMuonPOG"  , &lep2_tightMuonPOG  );
  BabyTree->Branch("lep2_mediumMuonPOG"  , &lep2_mediumMuonPOG  );
  BabyTree->Branch("lep2_looseMuonPOG"  , &lep2_looseMuonPOG  );
  BabyTree->Branch("lep3_mediumMuonPOG"  , &lep3_mediumMuonPOG  );

  BabyTree->Branch("is_fastsim", &is_fastsim);

  //InSituFR
  BabyTree->Branch("lep1_isGoodLeg"                                          , &lep1_isGoodLeg                                                                          );
  BabyTree->Branch("lep2_isGoodLeg"                                          , &lep2_isGoodLeg                                                                          );
  BabyTree->Branch("lep1_isFakeLeg"                                          , &lep1_isFakeLeg                                                                          );
  BabyTree->Branch("lep2_isFakeLeg"                                          , &lep2_isFakeLeg                                                                          );
  BabyTree->Branch("lep1_multiIso"                                           , &lep1_multiIso                                                                           );
  BabyTree->Branch("lep2_multiIso"                                           , &lep2_multiIso                                                                           );
  BabyTree->Branch("lep1_sip"                                                , &lep1_sip                                                                                );
  BabyTree->Branch("lep2_sip"                                                , &lep2_sip                                                                                );
  // BabyTree->Branch("lep1_closeJet"                                           , &lep1_closeJet                                                                           );
  // BabyTree->Branch("lep2_closeJet"                                           , &lep2_closeJet                                                                           );
  BabyTree->Branch("passed_id_inSituFR_lep1"                                 , &passed_id_inSituFR_lep1                                                                 );
  BabyTree->Branch("passed_id_inSituFR_lep2"                                 , &passed_id_inSituFR_lep2                                                                 );

  //Triggers
  BabyTree->Branch("fired_trigger"                                           , &fired_trigger                                                                           );
  BabyTree->Branch("triggers"                                                , &triggers                                                                                );
  BabyTree->Branch("weight_btagsf"                                           , &weight_btagsf                                                                           );
  BabyTree->Branch("weight_btagsf_UP"                                        , &weight_btagsf_UP                                                                        );
  BabyTree->Branch("weight_btagsf_DN"                                        , &weight_btagsf_DN                                                                        );
  BabyTree->Branch("weight_btagsf1"                                           , &weight_btagsf1                                                                           );
  BabyTree->Branch("weight_btagsf1_UP"                                        , &weight_btagsf1_UP                                                                        );
  BabyTree->Branch("weight_btagsf1_DN"                                        , &weight_btagsf1_DN                                                                        );
  BabyTree->Branch("weight_btagsf2"                                           , &weight_btagsf2                                                                           );
  BabyTree->Branch("weight_btagsf2_UP"                                        , &weight_btagsf2_UP                                                                        );
  BabyTree->Branch("weight_btagsf2_DN"                                        , &weight_btagsf2_DN                                                                        );

  //SUSY stuff
  BabyTree->Branch("gl1_p4" , &gl1_p4 );
  BabyTree->Branch("gl2_p4" , &gl2_p4 );
  BabyTree->Branch("glglpt" , &glglpt );
  BabyTree->Branch("isr_unc", &isr_unc);
  BabyTree->Branch("nisrMatch", &nisrMatch);
  BabyTree->Branch("weight_isr"                                           , &weight_isr                                                                           );
  BabyTree->Branch("weight_isr_dy"                                           , &weight_isr_dy                                                                           );
  BabyTree->Branch("weight_isr_tt"                                           , &weight_isr_tt                                                                           );
  BabyTree->Branch("weight_isr_UP"                                        , &weight_isr_UP                                                                        );
  BabyTree->Branch("weight_isr_DN"                                        , &weight_isr_DN                                                                        );

  //Lep3 stuff
  BabyTree->Branch("lep3_el_etaSC"           , &lep3_el_etaSC           );
  BabyTree->Branch("lep3_el_conv_vtx_flag"   , &lep3_el_conv_vtx_flag   );
  BabyTree->Branch("lep3_el_exp_innerlayers" , &lep3_el_exp_innerlayers );
  BabyTree->Branch("lep3_el_threeChargeAgree", &lep3_el_threeChargeAgree);
  BabyTree->Branch("lep3_el_dxyPV"           , &lep3_el_dxyPV           );
  BabyTree->Branch("lep3_el_ip3d"            , &lep3_el_ip3d            );
  BabyTree->Branch("lep3_el_dzPV"            , &lep3_el_dzPV            );
  BabyTree->Branch("lep3_el_MVA_value"       , &lep3_el_MVA_value       );
  BabyTree->Branch("lep3_el_MVA"             , &lep3_el_MVA             );
  BabyTree->Branch("lep3_iso_RA5"            , &lep3_iso_RA5            );
  BabyTree->Branch("lep3_iso_RA7"            , &lep3_iso_RA7            );
  BabyTree->Branch("lep3_passes_RA5"         , &lep3_passes_RA5         );
  BabyTree->Branch("lep3_passes_RA7"         , &lep3_passes_RA7         );
  BabyTree->Branch("lep3_passes_RA7v2"         , &lep3_passes_RA7v2         );
  BabyTree->Branch("lep3_mu_dxyPV"           , &lep3_mu_dxyPV           );
  BabyTree->Branch("lep3_mu_ip3d"            , &lep3_mu_ip3d            );
  BabyTree->Branch("lep3_mu_dzPV"            , &lep3_mu_dzPV            );
  BabyTree->Branch("lep3_mu_ptErr"           , &lep3_mu_ptErr           );

  //Lep 4 stuff
  BabyTree->Branch("lep4_el_etaSC"           , &lep4_el_etaSC           );
  BabyTree->Branch("lep4_el_conv_vtx_flag"   , &lep4_el_conv_vtx_flag   );
  BabyTree->Branch("lep4_el_exp_innerlayers" , &lep4_el_exp_innerlayers );
  BabyTree->Branch("lep4_el_threeChargeAgree", &lep4_el_threeChargeAgree);
  BabyTree->Branch("lep4_el_dxyPV"           , &lep4_el_dxyPV           );
  BabyTree->Branch("lep4_el_ip3d"            , &lep4_el_ip3d            );
  BabyTree->Branch("lep4_el_dzPV"            , &lep4_el_dzPV            );
  BabyTree->Branch("lep4_el_MVA_value"       , &lep4_el_MVA_value       );
  BabyTree->Branch("lep4_el_MVA"             , &lep4_el_MVA             );
  BabyTree->Branch("lep4_iso_RA5"            , &lep4_iso_RA5            );
  BabyTree->Branch("lep4_iso_RA7"            , &lep4_iso_RA7            );
  BabyTree->Branch("lep4_passes_RA5"         , &lep4_passes_RA5         );
  BabyTree->Branch("lep4_passes_RA7"         , &lep4_passes_RA7         );
  BabyTree->Branch("lep4_mu_dxyPV"           , &lep4_mu_dxyPV           );
  BabyTree->Branch("lep4_mu_ip3d"            , &lep4_mu_ip3d            );
  BabyTree->Branch("lep4_mu_dzPV"            , &lep4_mu_dzPV            );
  BabyTree->Branch("lep4_mu_ptErr"           , &lep4_mu_ptErr           );
  BabyTree->Branch("lep3_isTrigSafeNoIsov1"  , &lep3_isTrigSafeNoIsov1  );
  BabyTree->Branch("lep3_isTrigSafev1"       , &lep3_isTrigSafev1       );
  BabyTree->Branch("lep4_isTrigSafeNoIsov1"  , &lep4_isTrigSafeNoIsov1  );
  BabyTree->Branch("lep4_isTrigSafev1"       , &lep4_isTrigSafev1       );

  if (applyBtagSFs) {
    // setup btag calibration readers
    // calib = new BTagCalibration("csvv2", "CORE/Tools/btagsf/data/run2_25ns/CSVv2_Moriond17_G_H.csv"); // https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco 
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME also below
    // calib = new BTagCalibration("deepcsv", "CORE/Tools/btagsf/data/run2_25ns/DeepCSV_Moriond17_B_H.csv"); // https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco
    calib = new BTagCalibration("deepcsv", "CORE/Tools/btagsf/data/run2_25ns/DeepCSV_Moriond17_B_F.csv"); // https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
    calib2 = new BTagCalibration("deepcsv", "CORE/Tools/btagsf/data/run2_25ns/DeepCSV_Moriond17_G_H.csv"); // https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco
    reader_heavy    = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "comb", "central"); // central
    reader_heavy_UP = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "comb", "up");      // sys up
    reader_heavy_DN = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "comb", "down");    // sys down
    reader_light    = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "incl", "central");   // central
    reader_light_UP = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "incl", "up");        // sys up
    reader_light_DN = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "incl", "down");      // sys down
    reader_light2    = new BTagCalibrationReader(calib2, BTagEntry::OP_MEDIUM, "incl", "central");   // central
    reader_light2_UP = new BTagCalibrationReader(calib2, BTagEntry::OP_MEDIUM, "incl", "up");        // sys up
    reader_light2_DN = new BTagCalibrationReader(calib2, BTagEntry::OP_MEDIUM, "incl", "down");      // sys down

    // And another one for fastsim
    // calib_fs = new BTagCalibration("csvv2_fs", "btagsf/CSV_13TEV_Combined_20_11_2015.csv");
    // calib_fs = new BTagCalibration("csvv2_fs", "btagsf/CSV_13TEV_T1tttt_1200_800_11_7_2016.csv");  // see email from Dominick titled "Fwd: btag eff and SFs for fastsim"
    calib_fs = new BTagCalibration("csvv2_fs", "CORE/Tools/btagsf/data/run2_fastsim/fastsim_csvv2_ttbar_26_1_2017.csv");  // see email from Dominick titled "Fwd: btag eff and SFs for fastsim"
    reader_fastsim    = new BTagCalibrationReader(calib_fs, BTagEntry::OP_MEDIUM, "fastsim", "central"); // central
    reader_fastsim_UP = new BTagCalibrationReader(calib_fs, BTagEntry::OP_MEDIUM, "fastsim", "up");      // sys up
    reader_fastsim_DN = new BTagCalibrationReader(calib_fs, BTagEntry::OP_MEDIUM, "fastsim", "down");    // sys down

    // get btag efficiencies
    TFile* f_btag_eff = 0;
    if (isFastsim == 0) {

        // f_btag_eff = new TFile("btagsf/bTagEffs_80X.root");
        // TH2D* h_btag_eff_b_temp    = (TH2D*) f_btag_eff->Get("eff_total_M_b");
        // TH2D* h_btag_eff_c_temp    = (TH2D*) f_btag_eff->Get("eff_total_M_c");
        // TH2D* h_btag_eff_udsg_temp = (TH2D*) f_btag_eff->Get("eff_total_M_udsg");

        // f_btag_eff = new TFile("btagsf/btageff__ttbar_powheg_pythia8_25ns.root");
        // f_btag_eff = new TFile("CORE/Tools/btagsf/data/run2_25ns/btageff__ttbar_powheg_pythia8_25ns_Moriond17.root");
        f_btag_eff = new TFile("CORE/Tools/btagsf/data/run2_25ns/btageff__ttbar_powheg_pythia8_25ns_Moriond17_deepCSV.root");
        TH2D* h_btag_eff_b_temp    = (TH2D*) f_btag_eff->Get("h2_BTaggingEff_csv_med_Eff_b");
        TH2D* h_btag_eff_c_temp    = (TH2D*) f_btag_eff->Get("h2_BTaggingEff_csv_med_Eff_c");
        TH2D* h_btag_eff_udsg_temp = (TH2D*) f_btag_eff->Get("h2_BTaggingEff_csv_med_Eff_udsg");

        BabyFile->cd();
        h_btag_eff_b               = (TH2D*) h_btag_eff_b_temp->Clone("h_btag_eff_b");
        h_btag_eff_c               = (TH2D*) h_btag_eff_c_temp->Clone("h_btag_eff_c");
        h_btag_eff_udsg            = (TH2D*) h_btag_eff_udsg_temp->Clone("h_btag_eff_udsg");
        f_btag_eff->Close();
    }
    if (isFastsim >  0 ) {
        f_btag_eff = new TFile("CORE/Tools/btagsf/data/run2_fastsim/btageff__SMS-T1bbbb-T1qqqq_25ns_Moriond17.root");
        TH2D* h_btag_eff_b_temp    = (TH2D*) f_btag_eff->Get("h2_BTaggingEff_csv_med_Eff_b");
        TH2D* h_btag_eff_c_temp    = (TH2D*) f_btag_eff->Get("h2_BTaggingEff_csv_med_Eff_c");
        TH2D* h_btag_eff_udsg_temp = (TH2D*) f_btag_eff->Get("h2_BTaggingEff_csv_med_Eff_udsg");
        BabyFile->cd();
        h_btag_eff_b               = (TH2D*) h_btag_eff_b_temp->Clone("h_btag_eff_b");
        h_btag_eff_c               = (TH2D*) h_btag_eff_c_temp->Clone("h_btag_eff_c");
        h_btag_eff_udsg            = (TH2D*) h_btag_eff_udsg_temp->Clone("h_btag_eff_udsg");
        f_btag_eff->Close();
    }
  }

  // Load scale1fbs/xsecs from file
  df.loadFromFile("CORE/Tools/datasetinfo/scale1fbs.txt");

}

void babyMaker::InitBabyNtuple(){

    is_fastsim = 0;
    rawmet = -1;
    rawmetPhi = -1;
    calomet = -1;
    calometPhi = -1;
    met = -1;
    metPhi = -1;
    event = -1;
    lumi = -1;
    run = -1;
    filt_csc = 0;
    filt_hbhe = 0;
    filt_hcallaser = 0;
    filt_ecaltp = 0;
    filt_trkfail = 0;
    lep1_isPrompt       = 0;
    lep1_isDirectPrompt = 0;
    lep1_isStat3        = 0;
    lep2_isPrompt       = 0;
    lep2_isDirectPrompt = 0;
    lep2_isStat3        = 0;
    is_real_data = 0;
    scale1fb = 1;
    xsec = -1;
    xsec_ps = -1;
    kfactor = -1;
    gen_met = -1;
    genweights.clear();
    genweightsID.clear();
    gen_met_phi = -1;
    njets = -1;
    njets_raw = -1;
    hyp_class = -1;
    ht = -1;
    ht_raw = -1;
    lep1_motherID = 0;
    lep2_motherID = 0;
    lep1_mc_id = -1;
    lep2_mc_id = -1;
    lep1_mc_motherid = -1;
    lep2_mc_motherid = -1;
    lep1_id = -1;
    lep2_id = -1;
    lep1_coneCorrPt = -1;
    lep2_coneCorrPt = -1;
    lep3_coneCorrPt = -1;
    lep1_idx = -1;
    lep2_idx = -1;
    jets.clear();
    btags_flavor.clear();
    btags_disc.clear();
    btags_disc_mva.clear();
    btags_disc_ivf.clear();
    // jet_pt.clear();
    ht_raw = -1;
    jets_flavor.clear();
    jets_disc.clear();
    jets_disc_mva.clear();
    jets_disc_ivf.clear();
    jets_JEC.clear();
    btags_JEC.clear();
    jets_undoJEC.clear();
    btags_undoJEC.clear();
    btags.clear();
    jets_unc.clear();
    btags_unc.clear();
    btags_eff.clear();
    btags_effpt.clear();
    btags_sf.clear();
    btags.clear();
    nbtags = -1;
    nbtags_raw = -1;
    sf_dilepTrig_hpt = -1;
    sf_dilepTrig_lpt = -1;
    sf_dilepTrig_vlpt = -1;
    hyp_type = -1;
    sf_dilep_eff = -1;
    mt = -1;
    mt_l2 = -1;
    mt2 = -1;
    xsec = -1;
    xsec_ps = -1;
    xsec_error = -1;
    mtmin = -1;
    lep3_id = -1;
    lep3_idx = -1;
    lep4_id = -1;
    lep4_idx = -1;
    lep3_quality = -1;
    lep3_mcid = -1;
    lep3_mc_motherid = -1;
    lep3_mc3idx = -1;
    lep3_motherID = -1;
    lep4_mcid = -1;
    lep4_mcidx = -1;
    lep1_iso = -1;
    lep2_iso = -1;
    lep1_tkIso = -1;
    lep2_tkIso = -1;
    ncharginos = -1;
    higgs_mass = -1;
    genps_p4.clear();
    genps_id.clear();
    genps_id_mother.clear();
    genps_idx_mother.clear();
    genps_status.clear();
    genps_id_grandma.clear();
    all_leps_p4.clear();
    all_leps_id.clear();
    all_leps_veto.clear();
    all_leps_mc3idx.clear();
    lep1_passes_id = false;
    lep2_passes_id = false;
    lep3_passes_id = false;
    lep4_passes_id = false;
    lep1_tight = false;
    lep1_veto = false;
    lep1_fo = false;
    lep2_tight = false;
    lep2_veto = false;
    lep2_fo = false;
    lep3_tight = false;
    lep3_veto = false;
    lep3_fo = false;
    lep4_tight = false;
    lep4_veto = false;
    lep4_fo = false;
    lep1_dxyPV = -999998;
    lep2_dxyPV = -999998;
    lep1_dZ = -999998;
    lep2_dZ = -999998;
    lep1_d0_err = -999998;
    lep2_d0_err = -999998;
    lep1_ip3d = -999998;
    lep2_ip3d = -999998;
    lep1_MVA = -999998;
    lep2_MVA = -999998;
    lep1_MVA_miniaod = -999998;
    lep2_MVA_miniaod = -999998;
    lep1_ip3d_err = -999998;
    lep2_ip3d_err = -999998;
    lep1_el_conv_vtx_flag = 0;
    lep1_nPixelMiss = 0;
    lep1_el_exp_innerlayers = 0;
    lep1_tightCharge = 0;
    lep1_mu_ptErr = 0;
    lep1_el_threeChargeAgree = 0;
    lep2_el_conv_vtx_flag = 0;
    lep2_nPixelMiss = 0;
    lep2_el_exp_innerlayers = 0;
    lep2_tightCharge = 0;
    lep2_mu_ptErr = 0;
    lep2_el_threeChargeAgree = 0;
    lep1_el_etaSC = -999;
    lep2_el_etaSC = -999;
    lep1_isTrigSafeNoIsov1 = 0;
    lep1_isTrigSafev1 = 0;
    lep2_isTrigSafeNoIsov1 = 0;
    lep2_isTrigSafev1 = 0;
    lep1_tightMuonPOG = 0;
    lep1_mediumMuonPOG = 0;
    lep1_looseMuonPOG = 0;
    lep2_tightMuonPOG = 0;
    lep2_mediumMuonPOG = 0;
    lep2_looseMuonPOG = 0;
    lep3_mediumMuonPOG = 0;
    nVetoElectrons7 = 0;
    nVetoElectrons10 = 0;
    nVetoElectrons25 = 0;
    nVetoMuons5 = 0;
    nVetoMuons10 = 0;
    nVetoMuons25 = 0;
    filename = "";
    lep1_ptrel_v0 = -1;
    lep1_ptrel_v1 = -1;
    lep2_ptrel_v0 = -1;
    lep2_ptrel_v1 = -1;
    lep1_miniIso = -1;
    lep2_miniIso = -1;
    jet_close_lep1 = LorentzVector(0,0,0,0);
    jet_close_lep2 = LorentzVector(0,0,0,0);
    lep1_p4 = LorentzVector(0,0,0,0);
    lep2_p4 = LorentzVector(0,0,0,0);
    lep3_p4 = LorentzVector(0,0,0,0);
    lep4_p4 = LorentzVector(0,0,0,0);
    lep1_p4_gen = LorentzVector(0,0,0,0);
    lep2_p4_gen = LorentzVector(0,0,0,0);
    lep1_closeJet = LorentzVector(0,0,0,0);
    lep2_closeJet = LorentzVector(0,0,0,0);
    dilep_p4 = LorentzVector(0,0,0,0);
    eleID_kfhits.clear();
    eleID_oldsigmaietaieta.clear();
    eleID_oldsigmaiphiiphi.clear();
    eleID_oldcircularity.clear();
    eleID_oldr9.clear();
    eleID_scletawidth.clear();
    eleID_sclphiwidth.clear();
    eleID_he.clear();
    eleID_psEoverEraw.clear();
    eleID_kfchi2.clear();
    eleID_chi2_hits.clear();
    eleID_fbrem.clear();
    eleID_ep.clear();
    eleID_eelepout.clear();
    eleID_IoEmIop.clear();
    eleID_deltaetain.clear();
    eleID_deltaphiin.clear();
    eleID_deltaetaseed.clear();
    eleID_pT.clear();
    eleID_isbarrel.clear();
    eleID_isendcap.clear();
    eleID_scl_eta.clear();
    muID_dzPV.clear();
    muID_ptSig.clear();
    muID_ip3dSig.clear();
    muID_medMuonPOG.clear();
    mostJets.clear();
    muID_pt.clear();
    muID_eta.clear();
    lep1_isGoodLeg = 0;
    lep2_isGoodLeg = 0;
    lep1_isFakeLeg = 0;
    lep2_isFakeLeg = 0;
    lep1_multiIso          = 0;
    lep2_multiIso          = 0;
    lep1_sip = -1;
    lep2_sip = -1;
    passed_id_inSituFR_lep1 = 0;
    passed_id_inSituFR_lep2 = 0;
    trueNumInt.clear();
    nPUvertices.clear();
    nGoodVertices = 0;
    lep1_trigMatch_noIsoReq = 0;
    lep1_trigMatch_isoReq = 0;
    lep2_trigMatch_noIsoReq = 0;
    lep2_trigMatch_isoReq = 0;
    fired_trigger = 0;
    triggers = 0;
    // met3p0 = 0;
    // metphi3p0 = 0;
    passes_met_filters = 0;
    evt_egclean_pfmet = -1.0;
    evt_muegclean_pfmet = -1.0;
    evt_muegcleanfix_pfmet = -1.0;
    evt_uncorr_pfmet = -1.0;
    filt_noBadMuons = 0;
    filt_duplicateMuons = 0;
    filt_badMuons = 0;
    failsRA2Filter = 0;
    madeExtraZ = 0;
    madeExtraG = 0;
    nisrMatch = -1;
    weight_btagsf  = -9999.;
    weight_btagsf_UP = -9999.;
    weight_btagsf_DN = -9999.;
    weight_btagsf1  = -9999.;
    weight_btagsf1_UP = -9999.;
    weight_btagsf1_DN = -9999.;
    weight_btagsf2  = -9999.;
    weight_btagsf2_UP = -9999.;
    weight_btagsf2_DN = -9999.;
    weight_isr  = -9999.;
    weight_isr_dy  = -9999.;
    weight_isr_tt  = -9999.;
    weight_isr_UP = -9999.;
    weight_isr_DN = -9999.;
    mostJets_rawp4.clear();
    mostJets_disc.clear();
    mostJets_unc.clear();
    mostJets_JEC.clear();
    mostJets_undoJEC.clear();
    mostJets_passID.clear();
    mostJets_passCleaning.clear();
    njets_unc_up = 0;
    njets_unc_dn = 0;
    ht_unc_up = 0;
    ht_unc_dn = 0;
    nbtags_unc_up = 0;
    nbtags_unc_dn = 0;
    met_unc_up = 0;
    met_unc_dn = 0;
    metPhi_unc_up = 0;
    metPhi_unc_dn = 0;
    passedFilterList = 1;
    lep1_genps_isHardProcess = 0;
    lep1_genps_fromHardProcessFinalState = 0;
    lep1_genps_fromHardProcessDecayed = 0;
    lep1_genps_isDirectHardProcessTauDecayProductFinalState = 0;
    lep1_genps_fromHardProcessBeforeFSR = 0;
    lep1_genps_isLastCopy = 0;
    lep1_genps_isLastCopyBeforeFSR = 0;
    lep2_genps_isHardProcess = 0;
    lep2_genps_fromHardProcessFinalState = 0;
    lep2_genps_fromHardProcessDecayed = 0;
    lep2_genps_isDirectHardProcessTauDecayProductFinalState = 0;
    lep2_genps_fromHardProcessBeforeFSR = 0;
    lep2_genps_isLastCopy = 0;
    lep2_genps_isLastCopyBeforeFSR = 0;
    glglpt = -1;
    isr_unc = -1;
    gl1_p4 = { 0, 0, 0, 0 };
    gl2_p4 = { 0, 0, 0, 0 };
    lep1_mc3idx = -1;
    lep2_mc3idx = -1;
    lep3_el_etaSC = 0;
    lep3_el_conv_vtx_flag = 0;
    lep3_el_exp_innerlayers = 0;
    lep3_el_threeChargeAgree = 0;
    lep3_el_dxyPV = 0;
    lep3_el_ip3d = 0;
    lep3_el_dzPV = 0;
    lep3_el_MVA_value = 0;
    lep3_el_MVA = 0;
    lep3_iso_RA5 = 0;
    lep3_iso_RA7 = 0;
    lep3_passes_RA5 = 0;
    lep3_passes_RA7 = 0;
    lep3_passes_RA7v2 = 0;
    lep3_mu_dxyPV = 0;
    lep3_mu_ip3d = 0;
    lep3_mu_dzPV = 0;
    lep3_mu_ptErr = 0;
    lep4_el_etaSC = 0;
    lep4_el_conv_vtx_flag = 0;
    lep4_el_exp_innerlayers = 0;
    lep4_el_threeChargeAgree = 0;
    lep4_el_dxyPV = 0;
    lep4_el_ip3d = 0;
    lep4_el_dzPV = 0;
    lep4_el_MVA_value = 0;
    lep4_el_MVA = 0;
    lep4_iso_RA5 = 0;
    lep4_iso_RA7 = 0;
    lep4_passes_RA5 = 0;
    lep4_passes_RA7 = 0;
    lep4_mu_dxyPV = 0;
    lep4_mu_ip3d = 0;
    lep4_mu_dzPV = 0;
    lep4_mu_ptErr = 0;
    lep3_isTrigSafeNoIsov1 = 0;
    lep3_isTrigSafev1 = 0;
    lep4_isTrigSafeNoIsov1 = 0;
    lep4_isTrigSafev1 = 0;
}

//Main function
csErr_t babyMaker::ProcessBaby(string filename_in, FactorizedJetCorrector* jetCorr, JetCorrectionUncertainty* jecUnc, int isFastsim){


  //Initialize variables
  InitBabyNtuple();

  csErr_t babyErrorStruct;

  //Debug mode
  if (verbose && evt_cut>0 && tas::evt_event() != evt_cut) return babyErrorStruct;

  verbose = false;
  // if (tas::evt_event() != 1995735) return babyErrorStruct;

  //Preliminary stuff
  if (tas::hyp_type().size() < 1) return babyErrorStruct;
  if (tas::mus_dxyPV().size() != tas::mus_dzPV().size()) return babyErrorStruct;

  if (verbose) std::cout << "--------------\nEVENT: " << tas::evt_run() << ":" << tas::evt_lumiBlock() << ":" << tas::evt_event() << "\n--------------" << std::endl;

  //Fill Easy Variables
  filename = filename_in;
  rawmet = evt_pfmet();
  rawmetPhi = evt_pfmetPhi();
  calomet = evt_calomet();
  calometPhi = evt_calometPhi();
  event = tas::evt_event();
  lumi = tas::evt_lumiBlock();
  run = tas::evt_run();
  is_real_data = tas::evt_isRealData();

  is_miniaodv1 = filename.find("MCRUN2_74_V9") != std::string::npos;
  is_miniaodv1_80X = filename.find("RunIISpring16MiniAODv1") != std::string::npos;
  bool isJetHT = filename.find("JetHT") != std::string::npos;
  bool isRunH = filename.find("Run2016H") != std::string::npos;
  bool isReMiniAOD = filename.find("03Feb2017") != std::string::npos;

  int nHHsr = 51;
  int nHLsr = 41;


  //Fill lepton variables
  std::pair<hyp_result_t,Lep> best_hyp_info_withlep3 = chooseBestHyp(verbose, /*doclass8=*/ false);
  hyp_result_t best_hyp_info = best_hyp_info_withlep3.first;
  Lep best_lep3 = best_hyp_info_withlep3.second;
  hyp_class = best_hyp_info.hyp_class;
  int best_hyp = best_hyp_info.best_hyp;

  // FIXME, don't care about in situ for now, or even nondata OS
  if (                 hyp_class == 5) return babyErrorStruct;
  if (!is_real_data && hyp_class == 4) return babyErrorStruct;

  if (hyp_class == 6){
    madeExtraZ = makesExtraZ(best_hyp).result;
    madeExtraG = makesExtraGammaStar(best_hyp);
  }
  if (verbose) cout << "chose hyp: " << best_hyp << " of class" << hyp_class << endl;
  if (hyp_class == 0 || hyp_class == -1) return babyErrorStruct;

  //These c-s errors
  if (!is_real_data && tas::genweights().size()>110) {
    babyErrorStruct.cs_scale_no += tas::genweights().at(0);
    babyErrorStruct.cs_scale_up += tas::genweights().at(4);
    babyErrorStruct.cs_scale_dn += tas::genweights().at(8);
    for (int i = 0; i < 100; i++) babyErrorStruct.cs_pdf[i] = tas::genweights().at(i+10);
  }

  //Corrected MET
  int use_cleaned_met = tas::evt_isRealData();
  pair <float, float> T1CHSMET;
  if(is_miniaodv1) {
    T1CHSMET = getT1CHSMET_fromMINIAOD( jetCorr, NULL, 0, true );
  } else {
    T1CHSMET = getT1CHSMET_fromMINIAOD(jetCorr, NULL, 0, false, use_cleaned_met);
  }
  met = T1CHSMET.first;
  metPhi = T1CHSMET.second;

  if (!is_real_data){
    float sgnMCweight = ((tas::genps_weight() > 0) - (tas::genps_weight() < 0));
    trueNumInt = tas::puInfo_trueNumInteractions();
    nPUvertices = puInfo_nPUvertices();
    gen_met = tas::gen_met();
    gen_met_phi = tas::gen_metPhi();
    genweights = tas::genweights();  // These two are 20% of the ntuple size
    genweightsID = tas::genweightsID();
    // scale1fb = tas::evt_scale1fb();
    scale1fb = sgnMCweight*df.getScale1fbFromFile(tas::evt_dataset()[0].Data(),tas::evt_CMS3tag()[0].Data());
    xsec = sgnMCweight*df.getXsecFromFile(tas::evt_dataset()[0].Data(),tas::evt_CMS3tag()[0].Data());
    kfactor = tas::evt_kfactor();
  }

  // if (hyp_class == 7 && !is_wjets) return babyErrorStruct; // only care about OS ISFR events since we want more stats for WJets (this doubles ntuple size!!)
  if (hyp_class == 7) return babyErrorStruct;
  lep1_p4 = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_p4().at(best_hyp) : tas::hyp_lt_p4().at(best_hyp);
  lep2_p4 = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_p4().at(best_hyp) : tas::hyp_lt_p4().at(best_hyp);
  lep1_id = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_id().at(best_hyp) : tas::hyp_lt_id().at(best_hyp);
  lep2_id = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_id().at(best_hyp) : tas::hyp_lt_id().at(best_hyp);
  lep1_idx = (tas::hyp_ll_p4().at(best_hyp).pt() > tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_index().at(best_hyp) : tas::hyp_lt_index().at(best_hyp);
  lep2_idx = (tas::hyp_ll_p4().at(best_hyp).pt() <= tas::hyp_lt_p4().at(best_hyp).pt()) ? tas::hyp_ll_index().at(best_hyp) : tas::hyp_lt_index().at(best_hyp);
  lep1_coneCorrPt = coneCorrPt(lep1_id, lep1_idx);
  lep2_coneCorrPt = coneCorrPt(lep2_id, lep2_idx);
  Lep lep1 = Lep(lep1_id, lep1_idx);
  Lep lep2 = Lep(lep2_id, lep2_idx);
  lep1_dxyPV = lep1.dxyPV();
  lep2_dxyPV = lep2.dxyPV();
  lep1_dZ = lep1.dzPV();
  lep2_dZ = lep2.dzPV();
  lep1_d0_err = lep1.d0Err();
  lep2_d0_err = lep2.d0Err();
  lep1_ip3d = lep1.ip3d();
  lep2_ip3d = lep2.ip3d();
  lep1_ip3d_err = lep1.ip3dErr();
  lep2_ip3d_err = lep2.ip3dErr();
  hyp_type = tas::hyp_type().at(best_hyp);
  pair <Lep, int> thirdLepton = make_pair(Lep(0,-1), -1);
  if (hyp_class == 8) {
      thirdLepton =  make_pair(best_lep3,2);
  } else {
      thirdLepton = getThirdLepton(best_hyp);
  }
  lep3_id = thirdLepton.first.pdgId();
  lep3_idx = thirdLepton.first.idx();
  if (lep3_idx >= 0 && (abs(lep3_id) == 11 || abs(lep3_id) == 13)){
    lep3_p4 = thirdLepton.first.p4();
    lep3_coneCorrPt = coneCorrPt(lep3_id, lep3_idx);
    if (!is_real_data){
      pair <int, int> lep3_parentage = lepMotherID_v2(thirdLepton.first);
      lep3_motherID = lep3_parentage.first;
      lep3_mc3idx = lep3_parentage.second;
      lep3_mcid = lep3_mc3idx >= 0 ? tas::genps_id().at(lep3_mc3idx) : -9999;
      lep3_mc_motherid                                              = (lep3_mc3idx < 0) ? 0 : tas::genps_id_mother().at(lep3_mc3idx);
    }
  }
  lep3_quality = thirdLepton.second;
  Lep fourthLepton = getFourthLepton(best_hyp, lep3_id, lep3_idx);
  lep4_id = fourthLepton.pdgId();
  lep4_idx = fourthLepton.idx();
  if (lep4_idx >= 0 && (abs(lep4_id) == 11 || abs(lep4_id) == 13)){
    lep4_p4 = fourthLepton.p4();
    if (!is_real_data){
      pair <int, int> lep4_parentage = lepMotherID_v2(fourthLepton);
      lep4_mcidx = lep4_parentage.second;
      lep4_mcid = lep4_mcidx >= 0 ? tas::genps_id().at(lep4_mcidx) : -9999;
    }
  }
  lep1_iso = abs(lep1_id) == 11 ? eleRelIso03(lep1_idx, SS) :  muRelIso03(lep1_idx, SS);
  lep2_iso = abs(lep2_id) == 11 ? eleRelIso03(lep2_idx, SS) :  muRelIso03(lep2_idx, SS);
  lep1_tkIso = abs(lep1_id) == 11 ? els_tkIso().at(lep1_idx)/lep1_p4.pt() : mus_iso03_sumPt().at(lep1_idx)/lep1_p4.pt();
  lep2_tkIso = abs(lep2_id) == 11 ? els_tkIso().at(lep2_idx)/lep2_p4.pt() : mus_iso03_sumPt().at(lep2_idx)/lep2_p4.pt();
  lep1_multiIso = abs(lep1_id) == 11 ? passMultiIso(11, lep1_idx, 0.12, 0.80, 7.2, 1, 2) : passMultiIso(13, lep1_idx, 0.16, 0.76, 7.2, 1, 2);
  lep2_multiIso = abs(lep2_id) == 11 ? passMultiIso(11, lep2_idx, 0.12, 0.80, 7.2, 1, 2) : passMultiIso(13, lep2_idx, 0.16, 0.76, 7.2, 1, 2);
  lep1_sip = abs(lep1_id) == 11 ? fabs(els_ip3d().at(lep1_idx))/els_ip3derr().at(lep1_idx) : fabs(mus_ip3d().at(lep1_idx))/mus_ip3derr().at(lep1_idx);
  lep2_sip = abs(lep2_id) == 11 ? fabs(els_ip3d().at(lep2_idx))/els_ip3derr().at(lep2_idx) : fabs(mus_ip3d().at(lep2_idx))/mus_ip3derr().at(lep2_idx);
  dilep_p4 = lep1_p4 + lep2_p4;
  lep1_passes_id = isGoodLepton(lep1_id, lep1_idx);
  lep2_passes_id = isGoodLepton(lep2_id, lep2_idx);
  lep1_MVA = abs(lep1_id) == 11 ? getMVAoutput(lep1_idx) : -9999;
  lep2_MVA = abs(lep2_id) == 11 ? getMVAoutput(lep2_idx) : -9999;
  lep1_MVA_miniaod = abs(lep1_id) == 11 ? els_VIDNonTrigMvaValue().at(lep1_idx) : -9999;
  lep2_MVA_miniaod = abs(lep2_id) == 11 ? els_VIDNonTrigMvaValue().at(lep2_idx) : -9999;

  //More First lepton stuff
  if (abs(lep1_id) == 11 || abs(lep1_id) == 13){
    lep1_passes_id = isGoodLepton(lep1_id, lep1_idx);
    lep1_tight = abs(lep1_id) == 11 ? isGoodElectron(lep1_idx) : isGoodMuon(lep1_idx);
    lep1_veto = abs(lep1_id) == 11 ? isGoodVetoElectron(lep1_idx) : isGoodVetoMuon(lep1_idx);
    lep1_fo = abs(lep1_id) == 11 ? isFakableElectron(lep1_idx) : isFakableMuon(lep1_idx);
    lep1_el_conv_vtx_flag = abs(lep1_id) == 11 ? (!(els_conv_vtx_flag().at(lep1_idx))) : 0;
    lep1_nPixelMiss = abs(lep1_id) == 11 ? (!(els_lost_pixelhits().at(lep1_idx))) : -1;
    lep1_tightCharge = abs(lep1_id) == 11 ? tightChargeEle(lep1_idx) : -1;
    lep1_el_exp_innerlayers  = abs(lep1_id) == 11 ? (els_exp_innerlayers().at(lep1_idx) <= 0) : -1;
    lep1_el_threeChargeAgree = abs(lep1_id) == 11 ? threeChargeAgree(lep1_idx) : -1;
    lep1_el_etaSC = abs(lep1_id) == 11 ? els_etaSC().at(lep1_idx) : -999;

    lep1_mu_ptErr = abs(lep1_id) == 13 ? (mus_ptErr().at(lep1_idx)/mus_trk_p4().at(lep1_idx).pt() < 0.2) : -1;

    lep1_isTrigSafeNoIsov1   = abs(lep1_id) == 11 ? isTriggerSafenoIso_v1(lep1_idx) : 1;
    lep1_isTrigSafev1        = abs(lep1_id) == 11 ? isTriggerSafe_v1(lep1_idx) : 1;

    lep1_tightMuonPOG = abs(lep1_id) == 13 ? isTightMuonPOG(lep1_idx) : 0;
    lep1_mediumMuonPOG = abs(lep1_id) == 13 ? isMediumMuonPOG(lep1_idx) : 0;
    lep1_looseMuonPOG = abs(lep1_id) == 13 ? isLooseMuonPOG(lep1_idx) : 0;
  }


  //More Second lepton stuff
  if (abs(lep2_id) == 11 || abs(lep2_id) == 13){
    lep2_passes_id = isGoodLepton(lep2_id, lep2_idx);
    lep2_tight = abs(lep2_id) == 11 ? isGoodElectron(lep2_idx) : isGoodMuon(lep2_idx);
    lep2_veto = abs(lep2_id) == 11 ? isGoodVetoElectron(lep2_idx) : isGoodVetoMuon(lep2_idx);
    lep2_fo = abs(lep2_id) == 11 ? isFakableElectron(lep2_idx) : isFakableMuon(lep2_idx);
    lep2_el_conv_vtx_flag = abs(lep2_id) == 11 ? (!(els_conv_vtx_flag().at(lep2_idx))) : 0;
    lep2_nPixelMiss = abs(lep2_id) == 11 ? (!(els_lost_pixelhits().at(lep2_idx))) : -1;
    lep2_tightCharge = abs(lep2_id) == 11 ? tightChargeEle(lep2_idx) : -1;
    lep2_el_exp_innerlayers  = abs(lep2_id) == 11 ? (els_exp_innerlayers().at(lep2_idx) <= 0) : -1;
    lep2_el_threeChargeAgree = abs(lep2_id) == 11 ? threeChargeAgree(lep2_idx) : -1;
    lep2_el_etaSC = abs(lep2_id) == 11 ? els_etaSC().at(lep2_idx) : -999;

    lep2_mu_ptErr = abs(lep2_id) == 13 ? (mus_ptErr().at(lep2_idx)/mus_trk_p4().at(lep2_idx).pt() < 0.2) : -1;

    lep2_isTrigSafeNoIsov1   = abs(lep2_id) == 11 ? isTriggerSafenoIso_v1(lep2_idx) : 1;
    lep2_isTrigSafev1        = abs(lep2_id) == 11 ? isTriggerSafe_v1(lep2_idx) : 1;

    lep2_tightMuonPOG = abs(lep2_id) == 13 ? isTightMuonPOG(lep2_idx) : 0;
    lep2_mediumMuonPOG = abs(lep2_id) == 13 ? isMediumMuonPOG(lep2_idx) : 0;
    lep2_looseMuonPOG = abs(lep2_id) == 13 ? isLooseMuonPOG(lep2_idx) : 0;
  }

  //More Third lepton stuff
  if (abs(lep3_id) == 11 || abs(lep3_id) == 13){
    lep3_passes_id = isGoodLepton(lep3_id, lep3_idx);
    lep3_tight = abs(lep3_id) == 11 ? isGoodElectron(lep3_idx) : isGoodMuon(lep3_idx);
    lep3_veto = abs(lep3_id) == 11 ? isGoodVetoElectron(lep3_idx) : isGoodVetoMuon(lep3_idx);
    lep3_fo = abs(lep3_id) == 11 ? isFakableElectron(lep3_idx) : isFakableMuon(lep3_idx);
    if (lep3_veto || hyp_class==8){
      if (abs(lep3_id) == 11){

          float ptmin = 15;
          float ptmax = 25;
          float elpt = els_p4().at(lep3_idx).pt();
          auto mvacut = [ptmin,ptmax](float A, float B, float C, float pt_) {
              return pt_>10 ? std::min(A, std::max(B,A + (B-A)/(ptmax-ptmin)*(pt_-ptmin))) : C;
          };

        float etaSC              = fabs(els_etaSC().at(lep3_idx));
        lep3_el_etaSC            = (etaSC <= 2.5);
        lep3_el_conv_vtx_flag    = (!(els_conv_vtx_flag().at(lep3_idx)));
        lep3_el_exp_innerlayers  = (els_exp_innerlayers().at(lep3_idx) <= 0);
        lep3_el_threeChargeAgree = (threeChargeAgree(lep3_idx));
        lep3_el_dxyPV            = (fabs(els_dxyPV().at(lep3_idx)) <= 0.05);
        lep3_el_ip3d             = (fabs(els_ip3d().at(lep3_idx))/els_ip3derr().at(lep3_idx) < 4);
        lep3_el_dzPV             = (fabs(els_dzPV().at(lep3_idx)) < 0.1);
        lep3_el_MVA_value        = getMVAoutput(lep3_idx);
        // lep3_el_MVA              = ((etaSC < 0.8) ? (lep3_el_MVA_value > 0.87) : ((etaSC <= 1.479) ? (lep3_el_MVA_value > 0.60) : (lep3_el_MVA_value > 0.17)));
        lep3_el_MVA = (etaSC < 0.8) ? (lep3_el_MVA_value > mvacut(0.77,0.52,0.77,elpt)) : ((etaSC >= 0.8 && etaSC <= 1.479) ? lep3_el_MVA_value > mvacut(0.56,0.11,0.56,elpt) : lep3_el_MVA_value > mvacut(0.48,-0.01,0.48,elpt));
        lep3_iso_RA5             = passMultiIso(11, lep3_idx, 0.12, 0.80, 7.2, 1, 2);
        lep3_iso_RA7             = passMultiIso(11, lep3_idx, 0.12, 0.76, 7.2, 1, 2);
        lep3_isTrigSafeNoIsov1   = isTriggerSafenoIso_v1(lep3_idx);
        lep3_isTrigSafev1        = isTriggerSafe_v1(lep3_idx);
        // lep3_passes_RA5 = (lep3_el_etaSC && lep3_el_conv_vtx_flag && lep3_el_exp_innerlayers && lep3_el_threeChargeAgree && lep3_el_dxyPV && lep3_el_ip3d && lep3_el_dzPV && lep3_el_MVA && lep3_iso_RA5 && lep3_isTrigSafeNoIsov1 && (ht > 300 || lep3_isTrigSafev1));
        lep3_passes_RA5 = (lep3_el_etaSC && lep3_el_conv_vtx_flag && lep3_el_exp_innerlayers && lep3_el_threeChargeAgree && lep3_el_dxyPV && lep3_el_ip3d && lep3_el_dzPV && lep3_el_MVA && lep3_iso_RA5 && lep3_isTrigSafeNoIsov1);
        lep3_passes_RA7 = (lep3_el_etaSC && lep3_el_conv_vtx_flag && lep3_el_exp_innerlayers && lep3_el_dxyPV && lep3_el_ip3d && lep3_el_dzPV && lep3_el_MVA && lep3_iso_RA7 && lep3_isTrigSafeNoIsov1);
        lep3_passes_RA7v2 = electronID(lep3_idx, RA7_el_tight);
      }
      if (abs(lep3_id) == 13){
        lep3_mu_dxyPV = (fabs(mus_dxyPV().at(lep3_idx)) <= 0.05);
        lep3_mu_ip3d  = (fabs(mus_ip3d().at(lep3_idx))/mus_ip3derr().at(lep3_idx) < 4);
        lep3_mu_dzPV  = (fabs(mus_dzPV().at(lep3_idx)) <= 0.1);
        lep3_mu_ptErr = (mus_ptErr().at(lep3_idx)/mus_trk_p4().at(lep3_idx).pt() < 0.2);
        lep3_iso_RA5  = passMultiIso(13, lep3_idx, 0.16, 0.76, 7.2, 1, 2);
        lep3_iso_RA7  = passMultiIso(13, lep3_idx, 0.16, 0.69, 6.0, 1, 2);
        lep3_mediumMuonPOG = isMediumMuonPOG(lep3_idx);
        lep3_passes_RA5 = (lep3_mu_dxyPV && lep3_mu_ip3d && lep3_mu_dzPV && lep3_mu_ptErr && lep3_iso_RA5 && lep3_mediumMuonPOG);
        lep3_passes_RA7 = (lep3_mu_dxyPV && lep3_mu_ip3d && lep3_mu_dzPV && lep3_iso_RA7 && lep3_mediumMuonPOG);
        lep3_passes_RA7v2 = muonID(lep3_idx, RA7_tight);
      }
    }
  }

  //More fourth lepton stuff
  if (abs(lep4_id) == 11 || abs(lep4_id) == 13){
    lep4_veto = abs(lep4_id) == 11 ? isGoodVetoElectron(lep4_idx) : isGoodVetoMuon(lep4_idx);
    if (lep4_veto){
      lep4_passes_id = isGoodLepton(lep4_id, lep4_idx);
      lep4_tight = abs(lep4_id) == 11 ? isGoodElectron(lep4_idx) : isGoodMuon(lep4_idx);
      lep4_fo = abs(lep4_id) == 11 ? isFakableElectron(lep4_idx) : isFakableMuon(lep4_idx);
      if (abs(lep4_id) == 11){
        float etaSC              = fabs(els_etaSC().at(lep4_idx));
        lep4_el_etaSC            = (etaSC <= 2.5);
        lep4_el_conv_vtx_flag    = (!(els_conv_vtx_flag().at(lep4_idx)));
        lep4_el_exp_innerlayers  = (els_exp_innerlayers().at(lep4_idx) <= 0);
        lep4_el_threeChargeAgree = (threeChargeAgree(lep4_idx));
        lep4_el_dxyPV            = (fabs(els_dxyPV().at(lep4_idx)) <= 0.05);
        lep4_el_ip3d             = (fabs(els_ip3d().at(lep4_idx))/els_ip3derr().at(lep4_idx) < 4);
        lep4_el_dzPV             = (fabs(els_dzPV().at(lep4_idx)) < 0.1);
        lep4_el_MVA_value        = getMVAoutput(lep4_idx);
        lep4_el_MVA              = ((etaSC < 0.8) ? (lep4_el_MVA_value > 0.87) : ((etaSC <= 1.479) ? (lep4_el_MVA_value > 0.60) : (lep4_el_MVA_value > 0.17)));
        lep4_iso_RA5             = passMultiIso(11, lep4_idx, 0.16, 0.76, 7.2, 1, 2);
        lep4_iso_RA7             = passMultiIso(11, lep4_idx, 0.12, 0.80, 7.2, 1, 2);
        lep4_isTrigSafeNoIsov1   = isTriggerSafenoIso_v1(lep4_idx);
        lep4_isTrigSafev1        = isTriggerSafe_v1(lep4_idx);
        lep4_passes_RA5 = (lep4_el_etaSC && lep4_el_conv_vtx_flag && lep4_el_exp_innerlayers && lep4_el_threeChargeAgree && lep4_el_dxyPV && lep4_el_ip3d && lep4_el_dzPV && lep4_el_MVA && lep4_iso_RA5 && lep4_isTrigSafeNoIsov1 && (ht > 300 || lep4_isTrigSafev1));
        lep4_passes_RA7 = (lep4_el_etaSC && lep4_el_conv_vtx_flag && lep4_el_exp_innerlayers && lep4_el_dxyPV && lep4_el_ip3d && lep4_el_dzPV && lep4_el_MVA && lep4_iso_RA7 & lep4_isTrigSafeNoIsov1);
      }
      if (abs(lep4_id) == 13){
        lep4_mu_dxyPV = (fabs(mus_dxyPV().at(lep4_idx)) <= 0.05);
        lep4_mu_ip3d  = (fabs(mus_ip3d().at(lep4_idx))/mus_ip3derr().at(lep4_idx) < 4);
        lep4_mu_dzPV  = (fabs(mus_dzPV().at(lep4_idx)) <= 0.1);
        lep4_mu_ptErr = (mus_ptErr().at(lep4_idx)/mus_trk_p4().at(lep4_idx).pt() < 0.2);
        lep4_iso_RA5  = passMultiIso(13, lep4_idx, 0.12, 0.80, 7.2, 1, 2);
        lep4_iso_RA7  = passMultiIso(13, lep4_idx, 0.16, 0.76, 7.2, 1, 2);
        lep4_passes_RA5 = (lep4_mu_dxyPV && lep4_mu_ip3d && lep4_mu_dzPV && lep4_mu_ptErr && lep4_iso_RA5);
        lep4_passes_RA7 = (lep4_mu_dxyPV && lep4_mu_ip3d && lep4_mu_dzPV && lep4_iso_RA7);
      }
    }
  }

  //Lepton MC variables
  if (!is_real_data){
    pair <int, int> lep1_parentage                          = lepMotherID_v2(lep1);
    pair <int, int> lep2_parentage                          = lepMotherID_v2(lep2);
    lep1_mc3idx                                             = lep1_parentage.second;
    lep2_mc3idx                                             = lep2_parentage.second;
    lep1_mc_id                                              = (lep1_mc3idx < 0) ? 0 : tas::genps_id().at(lep1_mc3idx);
    lep2_mc_id                                              = (lep2_mc3idx < 0) ? 0 : tas::genps_id().at(lep2_mc3idx);
    lep1_mc_motherid                                              = (lep1_mc3idx < 0) ? 0 : tas::genps_id_mother().at(lep1_mc3idx);
    lep2_mc_motherid                                              = (lep2_mc3idx < 0) ? 0 : tas::genps_id_mother().at(lep2_mc3idx);
    lep1_isPrompt                                           = (lep1_mc3idx < 0) ? 0 : tas::genps_isPromptFinalState().at(lep1_mc3idx);
    lep1_isStat3                                            = (lep1_mc3idx < 0) ? 0 : tas::genps_isMostlyLikePythia6Status3().at(lep1_mc3idx);
    lep1_isDirectPrompt                                     = (lep1_mc3idx < 0) ? 0 : tas::genps_isDirectPromptTauDecayProductFinalState().at(lep1_mc3idx);
    lep1_genps_isHardProcess                                = (lep1_mc3idx < 0) ? 0 : tas::genps_isHardProcess().at(lep1_mc3idx);
    lep1_genps_fromHardProcessFinalState                    = (lep1_mc3idx < 0) ? 0 : tas::genps_fromHardProcessFinalState().at(lep1_mc3idx);
    lep1_genps_fromHardProcessDecayed                       = (lep1_mc3idx < 0) ? 0 : tas::genps_fromHardProcessDecayed().at(lep1_mc3idx);
    lep1_genps_isDirectHardProcessTauDecayProductFinalState = (lep1_mc3idx < 0) ? 0 : tas::genps_isDirectHardProcessTauDecayProductFinalState().at(lep1_mc3idx);
    lep1_genps_fromHardProcessBeforeFSR                     = (lep1_mc3idx < 0) ? 0 : tas::genps_fromHardProcessBeforeFSR().at(lep1_mc3idx);
    lep1_genps_isLastCopy                                   = (lep1_mc3idx < 0) ? 0 : tas::genps_isLastCopy().at(lep1_mc3idx);
    lep1_genps_isLastCopyBeforeFSR                          = (lep1_mc3idx < 0) ? 0 : tas::genps_isLastCopyBeforeFSR().at(lep1_mc3idx);
    lep1_motherID                                           = lep1_parentage.first;
    lep2_motherID                                           = lep2_parentage.first;
    lep2_isPrompt                                           = (lep2_mc3idx < 0) ? 0 : tas::genps_isPromptFinalState().at(lep2_mc3idx);
    lep2_isDirectPrompt                                     = (lep2_mc3idx < 0) ? 0 : tas::genps_isDirectPromptTauDecayProductFinalState().at(lep2_mc3idx);
    lep2_isStat3                                            = (lep2_mc3idx < 0) ? 0 : tas::genps_isMostlyLikePythia6Status3().at(lep2_mc3idx);
    lep2_genps_isHardProcess                                = (lep2_mc3idx < 0) ? 0 : tas::genps_isHardProcess().at(lep2_mc3idx);
    lep2_genps_fromHardProcessFinalState                    = (lep2_mc3idx < 0) ? 0 : tas::genps_fromHardProcessFinalState().at(lep2_mc3idx);
    lep2_genps_fromHardProcessDecayed                       = (lep2_mc3idx < 0) ? 0 : tas::genps_fromHardProcessDecayed().at(lep2_mc3idx);
    lep2_genps_isDirectHardProcessTauDecayProductFinalState = (lep2_mc3idx < 0) ? 0 : tas::genps_isDirectHardProcessTauDecayProductFinalState().at(lep2_mc3idx);
    lep2_genps_fromHardProcessBeforeFSR                     = (lep2_mc3idx < 0) ? 0 : tas::genps_fromHardProcessBeforeFSR().at(lep2_mc3idx);
    lep2_genps_isLastCopy                                   = (lep2_mc3idx < 0) ? 0 : tas::genps_isLastCopy().at(lep2_mc3idx);
    lep2_genps_isLastCopyBeforeFSR                          = (lep2_mc3idx < 0) ? 0 : tas::genps_isLastCopyBeforeFSR().at(lep2_mc3idx);
  }


  //PtRel for both leptons
  lep1_ptrel_v0 = getPtRel(lep1_id, lep1_idx, false, ssWhichCorr);
  lep2_ptrel_v0 = getPtRel(lep2_id, lep2_idx, false, ssWhichCorr);
  lep1_ptrel_v1 = getPtRel(lep1_id, lep1_idx, true, ssWhichCorr);
  lep2_ptrel_v1 = getPtRel(lep2_id, lep2_idx, true, ssWhichCorr);

  //MiniIso
  lep1_miniIso = abs(lep1_id)==11 ? elMiniRelIsoCMS3_EA(lep1_idx, ssEAversion) : muMiniRelIsoCMS3_EA(lep1_idx, ssEAversion);
  lep2_miniIso = abs(lep2_id)==11 ? elMiniRelIsoCMS3_EA(lep2_idx, ssEAversion) : muMiniRelIsoCMS3_EA(lep2_idx, ssEAversion);

  //For inSituFR, both must pass looser ID (easier than selection ID)
  passed_id_inSituFR_lep1 = isInSituFRLepton(lep1_id, lep1_idx);
  passed_id_inSituFR_lep2 = isInSituFRLepton(lep2_id, lep2_idx);

  //Closest jet for both leptons
  lep1_closeJet = closestJet(lep1_p4, 0.4, 3.0, ssWhichCorr);
  lep2_closeJet = closestJet(lep2_p4, 0.4, 3.0, ssWhichCorr);

  if (verbose){
    cout << "lep1 ratio: " << lep1_p4.pt()/lep1_closeJet.pt() << endl;
    cout << "lep2 ratio: " << lep2_p4.pt()/lep2_closeJet.pt() << endl;
    std::cout << " lep1_passes_id: " << lep1_passes_id << " lep2_passes_id: " << lep2_passes_id << " lep3_passes_id: " << lep3_passes_id << std::endl;
    std::cout << " isGoodLepton(lep3_id,lep3_idx): " << isGoodLepton(lep3_id,lep3_idx) << " lep3_tight: " << lep3_tight << " lep3_veto: " << lep3_veto << " lep3_fo: " << lep3_fo << std::endl;

    // muonID(lep3_idx,SS_tight_noiso_v5) muonID(lep3_idx,SS_tight_v5) muonID(lep3_idx,SS_fo_noiso_v5) muonID(lep3_idx,SS_veto_noiso_v5) isMediumMuonPOG(lep3_idx) isLooseMuonPOG(lep3_idx)
    std::cout << " muonID(lep3_idx,SS_tight_noiso_v5): " << muonID(lep3_idx,SS_tight_noiso_v5) << " muonID(lep3_idx,SS_tight_v5): " << muonID(lep3_idx,SS_tight_v5) << " muonID(lep3_idx,SS_fo_noiso_v5): " << muonID(lep3_idx,SS_fo_noiso_v5) << " muonID(lep3_idx,SS_veto_noiso_v5): " << muonID(lep3_idx,SS_veto_noiso_v5) << " isMediumMuonPOG(lep3_idx): " << isMediumMuonPOG(lep3_idx) << " isLooseMuonPOG(lep3_idx): " << isLooseMuonPOG(lep3_idx) << std::endl;

  }


  // This is 50% of the ntuple size
  // Fill all generated particles
  bool do_mc_fill = false;
  if (!is_real_data && do_mc_fill){
    genps_p4 = tas::genps_p4();
    genps_id = tas::genps_id();
    genps_id_mother = tas::genps_id_mother();
    genps_idx_mother = tas::genps_idx_mother();
    genps_status = tas::genps_status();
    genps_id_grandma = tas::genps_id_simplegrandma();
    for (unsigned int i = 0; i < tas::els_p4().size(); i++){
        int mcidx = tas::els_mc3idx().at(i);
        if (mcidx < 0) continue;
        all_leps_p4.push_back(tas::els_p4()[i]);
        all_leps_id.push_back(-11*tas::els_charge().at(i));
        all_leps_mc3idx.push_back(mcidx);
        all_leps_veto.push_back(isGoodVetoElectron(i));
    }
    for (unsigned int i = 0; i < tas::mus_p4().size(); i++){
        int mcidx = tas::mus_mc3idx().at(i);
        if (mcidx < 0) continue;
        all_leps_p4.push_back(tas::mus_p4()[i]);
        all_leps_id.push_back(-13*tas::mus_charge().at(i));
        all_leps_mc3idx.push_back(mcidx);
        all_leps_veto.push_back(isGoodVetoMuon(i));
    }
  }



  if (!is_real_data && isFastsim > 0) {
      ncharginos = 0;
      for (unsigned int gp=0;gp<tas::genps_id().size();gp++) {
          if (abs(tas::genps_id()[gp])==1000024 && tas::genps_status()[gp]==22) ncharginos++;
      }
  }

  //Determine and save jet and b-tag variables, raw
  std::pair <vector <Jet>, vector <Jet> > jet_results = SSJetsCalculator(jetCorr, 2, (isFastsim > 0));
  vector <LorentzVector> jets_raw;
  vector <LorentzVector> btags_raw;
  vector <float> jets_undoJEC_raw;
  for (unsigned int i = 0; i < jet_results.first.size(); i++)  jets_raw.push_back(jet_results.first.at(i).p4());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_raw.push_back(jet_results.second.at(i).p4());
  for (unsigned int i = 0; i < jet_results.first.size(); i++)  jets_undoJEC_raw.push_back(jet_results.first.at(i).undo_jec());
  njets_raw = jets_raw.size();
  nbtags_raw = btags_raw.size();
  ht_raw = 0;
  for (unsigned int i = 0; i < jets_raw.size(); i++) ht_raw += jets_raw.at(i).pt()*jets_undoJEC_raw.at(i);


  //Determine and save jet and b-tag variables, corrected
  jet_results = SSJetsCalculator(jetCorr, 1);
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets.push_back(jet_results.first.at(i).p4());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_flavor.push_back(jet_results.first.at(i).mcFlavor()); // NJA
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_disc.push_back(jet_results.first.at(i).csv());
  // for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_disc_mva.push_back(jet_results.first.at(i).csvmva());
  // for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_disc_ivf.push_back(jet_results.first.at(i).csvivf());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_JEC.push_back(jet_results.first.at(i).jec());
  for (unsigned int i = 0; i < jet_results.first.size(); i++) jets_undoJEC.push_back(jet_results.first.at(i).undo_jec());

  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags.push_back(jet_results.second.at(i).p4());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_flavor.push_back(jet_results.second.at(i).mcFlavor()); // NJA
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_disc.push_back(jet_results.second.at(i).csv());
  // for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_disc_mva.push_back(jet_results.second.at(i).csvmva());
  // for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_disc_ivf.push_back(jet_results.second.at(i).csvivf());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_JEC.push_back(jet_results.second.at(i).jec());
  for (unsigned int i = 0; i < jet_results.second.size(); i++) btags_undoJEC.push_back(jet_results.second.at(i).undo_jec());

  for (unsigned int i = 0; i < jet_results.first.size(); i++){
    jecUnc->setJetEta(jets[i].eta());
    jecUnc->setJetPt(jets[i].pt()*jets_undoJEC[i]*jets_JEC[i]);
    jets_unc.push_back(jecUnc->getUncertainty(true));
  }
  for (unsigned int i = 0; i < jet_results.second.size(); i++){
    jecUnc->setJetEta(btags[i].eta());
    jecUnc->setJetPt(btags[i].pt()*btags_undoJEC[i]*btags_JEC[i]);
    btags_unc.push_back(jecUnc->getUncertainty(true));
  }
  njets = jets.size();
  nbtags = btags.size();
  ht = 0;
  for (unsigned int i = 0; i < jets.size(); i++){
     // jet_pt.push_back(jets.at(i).pt()*jets_undoJEC.at(i)*jets_JEC.at(i));
     ht += jets.at(i).pt()*jets_undoJEC.at(i)*jets_JEC.at(i);
  }

  //Reject events that fail trigger matching
  if (verbose) cout << "ht: " << ht << endl;
  if (ht < 300 && hyp_type != 0){
    if (abs(lep1_id) == 11 && !isTriggerSafe_v1(lep1_idx)) return babyErrorStruct;
    if (abs(lep2_id) == 11 && !isTriggerSafe_v1(lep2_idx)) return babyErrorStruct;
  }
  if (verbose) cout << "passed trigger safety" << endl;

  //now look at jets for get the btag scale factor (need to save down to 25 GeV corrected)
  // for applying btagging SFs, using Method 1a from the twiki below:
  //   https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods#1a_Event_reweighting_using_scale
  //   https://twiki.cern.ch/twiki/pub/CMS/BTagSFMethods/Method1aExampleCode_CSVM.cc.txt
  float btagprob_data = 1.;
  float btagprob_err_heavy_UP = 0.;
  float btagprob_err_heavy_DN = 0.;
  float btagprob_err_light_UP = 0.;
  float btagprob_err_light_DN = 0.;
  float btagprob_mc = 1.;
  jet_results = SSJetsCalculator(jetCorr, 1, 0, 1);
  for (unsigned int i = 0; i < jet_results.first.size(); i++) {
     if (is_real_data) continue;
     //get btag eff weights
     float jet_pt = jet_results.first.at(i).p4().pt()*jet_results.first.at(i).undo_jec()*jet_results.first.at(i).jec();
     // Don't consider any jets below 25
     if (jet_pt<25.) continue;
     // Don't consider nonbjets with 25<pT<40
     // if ((!jet_results.first.at(i).isBtag()) && (jet_pt<40.)) continue;
     float jet_eta = jet_results.first.at(i).p4().eta();
     int jet_mcFlavour = jet_results.first.at(i).mcFlavor();
     float eff = getBtagEffFromFile(jet_pt, jet_eta, jet_mcFlavour);
     BTagEntry::JetFlavor flavor = BTagEntry::FLAV_UDSG;
     if (abs(jet_mcFlavour) == 5) flavor = BTagEntry::FLAV_B;
     else if (abs(jet_mcFlavour) == 4) flavor = BTagEntry::FLAV_C;
     float pt_cutoff = std::max(30.,std::min(669.,double(jet_pt)));
     float weight_cent(1.), weight_UP(1.), weight_DN(1.);
     if (flavor == BTagEntry::FLAV_UDSG) {
       weight_cent    = reader_light->eval(flavor, jet_eta, pt_cutoff);
       weight_UP      = reader_light_UP->eval(flavor, jet_eta, pt_cutoff);
       weight_DN      = reader_light_DN->eval(flavor, jet_eta, pt_cutoff);
     }
     else {
       weight_cent    = reader_heavy->eval(flavor, jet_eta, pt_cutoff);
       weight_UP      = reader_heavy_UP->eval(flavor, jet_eta, pt_cutoff);
       weight_DN      = reader_heavy_DN->eval(flavor, jet_eta, pt_cutoff);
     }
     if (isFastsim > 0){
       weight_cent *= reader_fastsim->eval(flavor, jet_eta, pt_cutoff);
       weight_UP   *= reader_fastsim_UP->eval(flavor, jet_eta, pt_cutoff);
       weight_DN   *= reader_fastsim_DN->eval(flavor, jet_eta, pt_cutoff);
     }
     if (jet_results.first.at(i).isBtag()) {
       btagprob_data *= weight_cent * eff;
       btagprob_mc *= eff;
       float abserr_UP = weight_UP - weight_cent;
       float abserr_DN = weight_cent - weight_DN;
       if (flavor == BTagEntry::FLAV_UDSG) {
         btagprob_err_light_UP += abserr_UP/weight_cent;
         btagprob_err_light_DN += abserr_DN/weight_cent;
       }
       else {
         btagprob_err_heavy_UP += abserr_UP/weight_cent;
         btagprob_err_heavy_DN += abserr_DN/weight_cent;
       }
     }
     else {
       btagprob_data *= (1. - weight_cent * eff);
       btagprob_mc *= (1. - eff);
       float abserr_UP = weight_UP - weight_cent;
       float abserr_DN = weight_cent - weight_DN;
       if (flavor == BTagEntry::FLAV_UDSG) {
         btagprob_err_light_UP += (-eff * abserr_UP)/(1 - eff * weight_cent);
         btagprob_err_light_DN += (-eff * abserr_DN)/(1 - eff * weight_cent);
       }
       else {
         btagprob_err_heavy_UP += (-eff * abserr_UP)/(1 - eff * weight_cent);
         btagprob_err_heavy_DN += (-eff * abserr_DN)/(1 - eff * weight_cent);
       }
     }
     if (verbose) {
       cout << Form("proc jet pt, eta, isBtagged, mcFlav = %f, %f, %i, %i",jet_pt,jet_eta,jet_results.first.at(i).isBtag(),flavor) << endl;
       cout << Form("eff, SF = %f %f",eff,weight_cent) << endl;
       cout << Form("partial SF is %f",btagprob_data / btagprob_mc) << endl;
     }
     // btags_effpt.push_back(jet_pt);
     // btags_eff.push_back(eff);
     // btags_sf.push_back(weight_cent);
  }
  weight_btagsf1 = btagprob_data / btagprob_mc;
  weight_btagsf1_UP = weight_btagsf1 + (sqrt(pow(btagprob_err_heavy_UP,2) + pow(btagprob_err_light_UP,2)) * weight_btagsf1);
  weight_btagsf1_DN = weight_btagsf1 - (sqrt(pow(btagprob_err_heavy_DN,2) + pow(btagprob_err_light_DN,2)) * weight_btagsf1);

  // --------------------------------------- //
  // AGAIN BECAUSE THERE'S TWO GODDAMN FILES //
  // --------------------------------------- //
  btagprob_data = 1.;
  btagprob_err_heavy_UP = 0.;
  btagprob_err_heavy_DN = 0.;
  btagprob_err_light_UP = 0.;
  btagprob_err_light_DN = 0.;
  btagprob_mc = 1.;
  for (unsigned int i = 0; i < jet_results.first.size(); i++) {
     if (is_real_data) continue;
     //get btag eff weights
     float jet_pt = jet_results.first.at(i).p4().pt()*jet_results.first.at(i).undo_jec()*jet_results.first.at(i).jec();
     // Don't consider any jets below 25
     if (jet_pt<25.) continue;
     // Don't consider nonbjets with 25<pT<40
     // if ((!jet_results.first.at(i).isBtag()) && (jet_pt<40.)) continue;
     float jet_eta = jet_results.first.at(i).p4().eta();
     int jet_mcFlavour = jet_results.first.at(i).mcFlavor();
     float eff = getBtagEffFromFile(jet_pt, jet_eta, jet_mcFlavour);
     BTagEntry::JetFlavor flavor = BTagEntry::FLAV_UDSG;
     if (abs(jet_mcFlavour) == 5) flavor = BTagEntry::FLAV_B;
     else if (abs(jet_mcFlavour) == 4) flavor = BTagEntry::FLAV_C;
     float pt_cutoff = std::max(30.,std::min(669.,double(jet_pt)));
     float weight_cent(1.), weight_UP(1.), weight_DN(1.);
     if (flavor == BTagEntry::FLAV_UDSG) {
       weight_cent    = reader_light2->eval(flavor, jet_eta, pt_cutoff);
       weight_UP      = reader_light2_UP->eval(flavor, jet_eta, pt_cutoff);
       weight_DN      = reader_light2_DN->eval(flavor, jet_eta, pt_cutoff);
     }
     else {
       weight_cent    = reader_heavy->eval(flavor, jet_eta, pt_cutoff);
       weight_UP      = reader_heavy_UP->eval(flavor, jet_eta, pt_cutoff);
       weight_DN      = reader_heavy_DN->eval(flavor, jet_eta, pt_cutoff);
     }
     if (isFastsim > 0){
       weight_cent *= reader_fastsim->eval(flavor, jet_eta, pt_cutoff);
       weight_UP   *= reader_fastsim_UP->eval(flavor, jet_eta, pt_cutoff);
       weight_DN   *= reader_fastsim_DN->eval(flavor, jet_eta, pt_cutoff);
     }
     if (jet_results.first.at(i).isBtag()) {
       btagprob_data *= weight_cent * eff;
       btagprob_mc *= eff;
       float abserr_UP = weight_UP - weight_cent;
       float abserr_DN = weight_cent - weight_DN;
       if (flavor == BTagEntry::FLAV_UDSG) {
         btagprob_err_light_UP += abserr_UP/weight_cent;
         btagprob_err_light_DN += abserr_DN/weight_cent;
       }
       else {
         btagprob_err_heavy_UP += abserr_UP/weight_cent;
         btagprob_err_heavy_DN += abserr_DN/weight_cent;
       }
     }
     else {
       btagprob_data *= (1. - weight_cent * eff);
       btagprob_mc *= (1. - eff);
       float abserr_UP = weight_UP - weight_cent;
       float abserr_DN = weight_cent - weight_DN;
       if (flavor == BTagEntry::FLAV_UDSG) {
         btagprob_err_light_UP += (-eff * abserr_UP)/(1 - eff * weight_cent);
         btagprob_err_light_DN += (-eff * abserr_DN)/(1 - eff * weight_cent);
       }
       else {
         btagprob_err_heavy_UP += (-eff * abserr_UP)/(1 - eff * weight_cent);
         btagprob_err_heavy_DN += (-eff * abserr_DN)/(1 - eff * weight_cent);
       }
     }
     if (verbose) {
       cout << Form("proc jet pt, eta, isBtagged, mcFlav = %f, %f, %i, %i",jet_pt,jet_eta,jet_results.first.at(i).isBtag(),flavor) << endl;
       cout << Form("eff, SF = %f %f",eff,weight_cent) << endl;
       cout << Form("partial SF is %f",btagprob_data / btagprob_mc) << endl;
     }
     // btags_effpt.push_back(jet_pt);
     // btags_eff.push_back(eff);
     // btags_sf.push_back(weight_cent);
  }
  weight_btagsf2 = btagprob_data / btagprob_mc;
  weight_btagsf2_UP = weight_btagsf2 + (sqrt(pow(btagprob_err_heavy_UP,2) + pow(btagprob_err_light_UP,2)) * weight_btagsf2);
  weight_btagsf2_DN = weight_btagsf2 - (sqrt(pow(btagprob_err_heavy_DN,2) + pow(btagprob_err_light_DN,2)) * weight_btagsf2);

  // Now we do this stupid thing of rolling a random number and
  // deciding which era the MC "belongs to", to assign the proper btag weight
  // Seed by event number to keep deterministic
  TRandom *tr1 = new TRandom(tas::evt_event());
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
    // weight_btagsf =    weight_btagsf1;
    // weight_btagsf_UP = weight_btagsf1_UP;
    // weight_btagsf_DN = weight_btagsf1_DN;
  if (tr1->Rndm() < 0.55) {
    // B-F is 55% of the lumi
    weight_btagsf =    weight_btagsf1;
    weight_btagsf_UP = weight_btagsf1_UP;
    weight_btagsf_DN = weight_btagsf1_DN;
  } else {
    weight_btagsf =    weight_btagsf2;
    weight_btagsf_UP = weight_btagsf2_UP;
    weight_btagsf_DN = weight_btagsf2_DN;
  }
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME
      // FIXME FIXME FIXME

  // // ISR stuff for 2015
  // if (isFastsim > 0){
  //   for (unsigned int i = 0; i < tas::genps_status().size(); i++){
  //     if (isFastsim  < 3 && abs(tas::genps_id().at(i)) != 1000021) continue;
  //     if (isFastsim == 3 && abs(tas::genps_id().at(i)) != 1000005) continue;
  //     if (tas::genps_status().at(i) != 22) continue;
  //     if (gl1_p4.pt() <= 0){ gl1_p4 = tas::genps_p4().at(i); continue; }
  //     if (gl2_p4.pt() <= 0){ gl2_p4 = tas::genps_p4().at(i); break; }
  //   }
  //   glglpt = (gl1_p4 + gl2_p4).pt();
  //   isr_unc = (glglpt < 400) ? 0 : ((glglpt < 600) ? 0.15 : 0.30);
  // }

  // ISR stuff for 2016
  // https://github.com/cmstas/MT2Analysis/blob/cmssw80x/babymaker/ScanChain.cc#L1870
  // https://github.com/cmstas/CORE/blob/master/MCSelections.cc#L438
  if (!is_real_data) {
      nisrMatch = get_nisrMatch(jets);
      weight_isr = get_isrWeight(nisrMatch);
      float unc_isr = get_isrUnc(nisrMatch);
      weight_isr_UP = weight_isr + unc_isr;
      weight_isr_DN = weight_isr - unc_isr;
      weight_isr_dy = 1.00; weight_isr_tt = 1.00;
      if (nisrMatch == 0) { weight_isr_dy = 1.00; weight_isr_tt = 1.00; }
      if (nisrMatch == 1) { weight_isr_dy = 0.96; weight_isr_tt = 0.86; }
      if (nisrMatch == 2) { weight_isr_dy = 0.96; weight_isr_tt = 0.79; }
      if (nisrMatch == 3) { weight_isr_dy = 0.95; weight_isr_tt = 0.75; }
      if (nisrMatch == 4) { weight_isr_dy = 1.05; weight_isr_tt = 0.77; }
      if (nisrMatch == 5) { weight_isr_dy = 1.11; weight_isr_tt = 0.74; }
      if (nisrMatch == 6) { weight_isr_dy = 1.22; weight_isr_tt = 0.73; }
      if (nisrMatch == 7) { weight_isr_dy = 1.34; weight_isr_tt = 0.56; }
  }



  //Save Most jets
  vector <Jet> mostJets_jet;
  vector <int> mostJets_idx;
  vector <Jet> mostJets_jet_up;
  vector <Jet> mostJets_jet_dn;
  for (unsigned int i = 0; i < tas::pfjets_p4().size(); i++){
    //Alias
    LorentzVector jet = tas::pfjets_p4().at(i);

    //Cuts
    if (jet.pt() < 5.) continue;
    if (fabs(jet.eta()) > 2.4) continue;

    //Calculate raw pt
    float rawpt = jet.pt()*tas::pfjets_undoJEC().at(i);

    //Calculate jet corr
    jetCorr->setJetEta(jet.eta());
    jetCorr->setJetPt(rawpt);
    jetCorr->setJetA(tas::pfjets_area().at(i));
    jetCorr->setRho(tas::evt_fixgridfastjet_all_rho());
    float JEC = jetCorr->getCorrection();

    //Calculate jet unc
    jecUnc->setJetEta(pfjets_p4().at(i).eta());
    jecUnc->setJetPt(rawpt*JEC);
    float jetUnc = jecUnc->getUncertainty(true);

    //Save results
    mostJets.push_back(jet);
    if (rawpt*JEC > 40 && isLoosePFJet_50nsV1(i))            mostJets_jet.push_back(Jet(i, JEC));
    else                                                     mostJets_jet.push_back(Jet(-1, -9999));
    if (rawpt*JEC*(1+jetUnc) > 40 && isLoosePFJet_50nsV1(i)) mostJets_jet_up.push_back(Jet(i, JEC));
    else                                                     mostJets_jet_up.push_back(Jet(-1, -9999));
    if (rawpt*JEC*(1-jetUnc) > 40 && isLoosePFJet_50nsV1(i)) mostJets_jet_dn.push_back(Jet(i, JEC));
    else                                                     mostJets_jet_dn.push_back(Jet(-1, -9999));
    mostJets_rawp4.push_back(jet*tas::pfjets_undoJEC().at(i));
    mostJets_idx.push_back(i);
    // mostJets_disc.push_back(tas::getbtagvalue("pfCombinedInclusiveSecondaryVertexV2BJetTags", i));
    mostJets_disc.push_back(tas::getbtagvalue("deepFlavourJetTags:probb",i)+tas::getbtagvalue("deepFlavourJetTags:probbb",i));
    mostJets_unc.push_back(jetUnc);
    mostJets_JEC.push_back(JEC);
    mostJets_undoJEC.push_back(tas::pfjets_undoJEC().at(i));
    mostJets_passID.push_back(isLoosePFJet_50nsV1(i));
  }

  //Separate cleaning for all the permutations
  mostJets_passCleaning = cleanJets(mostJets_jet);
  vector <bool> mostJets_up_passCleaning = cleanJets(mostJets_jet_up);
  vector <bool> mostJets_dn_passCleaning = cleanJets(mostJets_jet_dn);

  //Unc up/down results
  int njetsAG = 0;
  for (unsigned int i = 0; i < mostJets.size(); i++){
    if (mostJets_passCleaning.at(i) == 0) continue;
    if (mostJets_passID.at(i) == 0) continue;
    float jet_pt = mostJets.at(i).pt()*mostJets_undoJEC.at(i)*mostJets_JEC.at(i);
    if (jet_pt > 40) njetsAG++;
  }
  for (unsigned int i = 0; i < mostJets.size(); i++){
    if (mostJets_up_passCleaning.at(i) == 0) continue;
    if (mostJets_passID.at(i) == 0) continue;
    float jet_pt_up = mostJets.at(i).pt()*mostJets_undoJEC.at(i)*mostJets_JEC.at(i)*(1.0+mostJets_unc.at(i));
    if (jet_pt_up > 40) njets_unc_up++;
    if (jet_pt_up > 40) ht_unc_up += jet_pt_up;
    if (mostJets_disc.at(i) < btagCut) continue;
    if (jet_pt_up > 25) nbtags_unc_up++;
  }
  for (unsigned int i = 0; i < mostJets.size(); i++){
    if (mostJets_dn_passCleaning.at(i) == 0) continue;
    if (mostJets_passID.at(i) == 0) continue;
    float jet_pt_dn = mostJets.at(i).pt()*mostJets_undoJEC.at(i)*mostJets_JEC.at(i)*(1.0-mostJets_unc.at(i));
    if (jet_pt_dn > 40) njets_unc_dn++;
    if (jet_pt_dn > 40) ht_unc_dn += jet_pt_dn;
    if (mostJets_disc.at(i) < btagCut) continue;
    if (jet_pt_dn > 25) nbtags_unc_dn++;
  }
  if(is_miniaodv1) {
    met_unc_up = getT1CHSMET_fromMINIAOD(jetCorr, jecUnc, 1, true).first;
    met_unc_dn = getT1CHSMET_fromMINIAOD(jetCorr, jecUnc, 0, true).first;
    metPhi_unc_up = getT1CHSMET_fromMINIAOD(jetCorr, jecUnc, 1, true).second;
    metPhi_unc_dn = getT1CHSMET_fromMINIAOD(jetCorr, jecUnc, 0, true).second;
  } else {
    met_unc_up = getT1CHSMET_fromMINIAOD(jetCorr, jecUnc, 1, false, use_cleaned_met).first;
    met_unc_dn = getT1CHSMET_fromMINIAOD(jetCorr, jecUnc, 0, false, use_cleaned_met).first;
    metPhi_unc_up = getT1CHSMET_fromMINIAOD(jetCorr, jecUnc, 1, false, use_cleaned_met).second;
    metPhi_unc_dn = getT1CHSMET_fromMINIAOD(jetCorr, jecUnc, 0, false, use_cleaned_met).second;
  }

  //Verbose for jets
  if (verbose){
    cout << "njets: " << njets << endl;
    cout << "nbtags: " <<  nbtags << endl;
    cout << "Printing Jets: " << endl;
    for (unsigned int i = 0; i < jets.size(); i++) cout << i << " " << jets[i].pt() << " " << jets[i].eta() << endl;
  }

  // //Closest Jet
  jet_close_lep1 = closestJet(lep1_p4, 0.4, 3.0, ssWhichCorr);
  jet_close_lep2 = closestJet(lep2_p4, 0.4, 3.0, ssWhichCorr);

  // //nVeto Leptons
  // if (verbose) cout << " PRINTING RECO ELECTRONS" << endl;
  // for (unsigned int eidx = 0; eidx < tas::els_p4().size(); eidx++){
  //   if (!isGoodVetoElectron(eidx)) continue;
  //   if (tas::els_p4().at(eidx).pt() < 7) continue;
  //   nVetoElectrons7++;
  //   if (verbose) cout << "good elec: " << tas::els_p4().at(eidx).pt() << endl;
  //   if (tas::els_p4().at(eidx).pt() < 10) continue;
  //   nVetoElectrons10++;
  //   if (tas::els_p4().at(eidx).pt() < 25) continue;
  //   nVetoElectrons25++;
  // }
  // if (verbose) cout << " PRINTING RECO MUONS" << endl;
  // for (unsigned int muidx = 0; muidx < tas::mus_p4().size(); muidx++){
  //   if (!isGoodVetoMuon(muidx)) continue;
  //   if (tas::mus_p4().at(muidx).pt() < 5) continue;
  //   nVetoMuons5++;
  //   if (verbose) cout << "good muon: " << tas::mus_p4().at(muidx).pt() << endl;
  //   if (tas::mus_p4().at(muidx).pt() < 10) continue;
  //   nVetoMuons10++;
  //   if (tas::mus_p4().at(muidx).pt() < 25) continue;
  //   nVetoMuons25++;
  // }

  //MT variables
  mt    = MT(lep1_p4.pt(), lep1_p4.phi(), met, metPhi);
  mt_l2 = MT(lep2_p4.pt(), lep2_p4.phi(), met, metPhi);
  mt2   = MT2(met, metPhi, lep1_p4, lep2_p4);
  mtmin = mt > mt_l2 ? mt_l2 : mt;

  //Ht and MET
  if (verbose) cout << "ht: " << ht << endl;
  if (verbose) cout << "met: " << met << endl;

  //Make sure one of our triggers fired
  // if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v")))  (triggers |= 1<<1);


  if (isRunH) {
      if (passHLTTrigger(triggerName("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v"))  ||
          passHLTTrigger(triggerName("HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v")))   (triggers |= 1<<2);

      if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v")))              (triggers |= 1<<3);
      if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v")))            (triggers |= 1<<4);
  } else {
      if (passHLTTrigger(triggerName("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v"))  ||
          passHLTTrigger(triggerName("HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v")))   (triggers |= 1<<2);

      if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v")))              (triggers |= 1<<3);
      if (passHLTTrigger(triggerName("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v")))            (triggers |= 1<<4);
  }

  if (passHLTTrigger(triggerName("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v")))        (triggers |= 1<<6);

  if (passHLTTrigger(triggerName("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v")))      (triggers |= 1<<0);
  if (passHLTTrigger(triggerName("HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v")))    (triggers |= 1<<5);
  if (passHLTTrigger(triggerName("HLT_DoubleMu8_Mass8_PFHT300_v")))                      (triggers |= 1<<7);

  // JetHT triggers
  if (passHLTTrigger(triggerName("HLT_AK8PFJet450")))                      (triggers |= 1<<8);
  if (passHLTTrigger(triggerName("HLT_PFJet450")))                         (triggers |= 1<<9);



  fired_trigger = false;
  if (triggers != 0) {
    if (ht<300) {
        // ee 3
        // mumu 0
        // emu  1
      if ( hyp_type==0 && ((triggers & 1<<3)==(1<<3) || (triggers & 1<<4)==(1<<4)) ) fired_trigger = true;
      if ( hyp_type==3 && (triggers & 1<<6)==(1<<6) ) fired_trigger = true;
      if ( (hyp_type==1 || hyp_type==2) && ((triggers & 1<<1)==(1<<1) || (triggers & 1<<2)==(1<<2)) ) fired_trigger = true;
    }
    else {
      if ( hyp_type==0 && (triggers & 1<<7)==(1<<7) ) fired_trigger = true;
      if ( hyp_type==3 && (triggers & 1<<5)==(1<<5) ) fired_trigger = true;
      if ( (hyp_type==1 || hyp_type==2) && (triggers & 1<<0)==(1<<0) ) fired_trigger = true;
    }

    if (isJetHT) {
      if ( (triggers & 1<<8)==(1<<8) ) fired_trigger = true;
      if ( (triggers & 1<<9)==(1<<9) ) fired_trigger = true;

      // for JetHT, just throw away the event if we didn't trigger...
      if (!fired_trigger) return babyErrorStruct;
    }
  }

  // //Electron ID variables
  // for (unsigned int index = 0; index < els_fbrem().size(); index++){
  //   if (!isGoodVetoElectron(index)) continue;
  //   if (tas::els_p4().at(index).pt() < 7) continue;
  //   if ((tas::els_p4().at(index).eta()) > 2.5) continue;
  //   eleID_kfhits          .push_back(tas::els_ckf_laywithmeas().at(index));
  //   eleID_oldsigmaietaieta.push_back(tas::els_sigmaIEtaIEta_full5x5().at(index));
  //   eleID_oldsigmaiphiiphi.push_back(tas::els_sigmaIPhiIPhi_full5x5().at(index));
  //   eleID_oldcircularity  .push_back(1.0 - tas::els_e1x5_full5x5().at(index)/tas::els_e5x5_full5x5().at(index));
  //   eleID_oldr9           .push_back(tas::els_r9_full5x5().at(index));
  //   eleID_scletawidth     .push_back(tas::els_etaSCwidth().at(index));
  //   eleID_sclphiwidth     .push_back(tas::els_phiSCwidth().at(index));
  //   eleID_he              .push_back(tas::els_hOverE().at(index));
  //   eleID_psEoverEraw     .push_back(tas::els_eSCPresh().at(index)/tas::els_eSCRaw().at(index));
  //   eleID_kfchi2          .push_back(tas::els_ckf_chi2().at(index)/tas::els_ckf_ndof().at(index));
  //   eleID_chi2_hits       .push_back(tas::els_chi2().at(index)/tas::els_ndof().at(index));
  //   eleID_fbrem           .push_back(tas::els_fbrem().at(index));
  //   eleID_ep              .push_back(tas::els_eOverPIn().at(index));
  //   eleID_eelepout        .push_back(tas::els_eOverPOut().at(index));
  //   eleID_IoEmIop         .push_back(tas::els_ecalEnergy().at(index) != 0 ? 1.0/tas::els_ecalEnergy().at(index) - tas::els_eOverPIn().at(index)/tas::els_ecalEnergy().at(index) : 999999);
  //   eleID_deltaetain      .push_back(tas::els_dEtaIn().at(index));
  //   eleID_deltaphiin      .push_back(tas::els_dPhiIn().at(index));
  //   eleID_deltaetaseed    .push_back(tas::els_dEtaOut().at(index));
  //   eleID_pT              .push_back(tas::els_p4().at(index).pt());
  //   eleID_isbarrel        .push_back(fabs(tas::els_etaSC().at(index)) < 1.479 ? 1 : 0);
  //   eleID_isendcap        .push_back(fabs(tas::els_etaSC().at(index)) > 1.479 ? 1 : 0);
  //   eleID_scl_eta         .push_back(tas::els_etaSC().at(index));
  // }

  // //Muon ID variables
  // for (unsigned int index = 0; index < mus_ip3d().size(); index++){
  //   if (!isGoodVetoMuon(index)) continue;
  //   if (tas::mus_p4().at(index).pt() < 5) continue;
  //   muID_dzPV      .push_back(fabs(mus_dzPV().at(index)));
  //   muID_ptSig     .push_back(mus_ptErr().at(index)/mus_trk_p4().at(index).pt());
  //   muID_ip3dSig   .push_back(fabs(mus_ip3d().at(index))/mus_ip3derr().at(index));
  //   muID_medMuonPOG.push_back(isMediumMuonPOG(index));
  //   muID_pt        .push_back(mus_p4().at(index).pt());
  //   muID_eta       .push_back(fabs(mus_p4().at(index).eta()));
  // }

  //Number of good vertices
  for (unsigned int i = 0; i < tas::vtxs_ndof().size(); i++){
    if (!isGoodVertex(i)) continue;
    nGoodVertices++;
  }

  // bool failsFastSimJetFilter = 0;
  // if(isFastsim) {
  //     for (unsigned int i = 0; i < mostJets.size(); i++){
  //         if (mostJets_passCleaning.at(i) == 0) continue;
  //         if (mostJets_passID.at(i) == 0) continue;
  //         float jet_pt = mostJets.at(i).pt()*mostJets_undoJEC.at(i)*mostJets_JEC.at(i);
  //         int jetIdx = mostJets_idx.at(i);
  //         bool isBtag = mostJets_disc.at(i) > btagCut;
  //         if( isBtag && jet_pt < 25.0) continue;
  //         if(!isBtag && jet_pt < 40.0) continue;
  //         if(isBadFastsimJet(jetIdx)) {
  //             failsFastSimJetFilter = 1;
  //             break;
  //         }
  //     }
  // }

  // failsRA2Filter = 0;
  // for (unsigned int ijet = 0; ijet < tas::pfjets_p4().size(); ijet++) {
  //     float dphi = fabs(tas::pfjets_p4()[ijet].phi()-metPhi);
  //     if( dphi > TMath::Pi() ) dphi = TMath::TwoPi() - dphi;
  //     float muf  = tas::pfjets_muonE()[ijet] / (tas::pfjets_undoJEC().at(ijet)*tas::pfjets_p4()[ijet].energy());
  //     if(tas::pfjets_p4()[ijet].pt()>200 && dphi>(TMath::Pi()-0.4) && muf>0.5) failsRA2Filter = true;
  // }

  // //MET3p0 (aka FKW MET)
  // pair<float,float> MET3p0_ = MET3p0();
  // met3p0 = MET3p0_.first;
  // metphi3p0 = MET3p0_.second;

  // Met filters not filled properly in 80X miniAODv1
  // if(is_miniaodv1_80X) passes_met_filters = true;
  // else passes_met_filters = (isFastsim > 0) ? !failsFastSimJetFilter : passesMETfilters2016(is_real_data);
  // passes_met_filters = (isFastsim > 0) ? !failsFastSimJetFilter : passesMETfiltersMoriond17(is_real_data);
  passes_met_filters = passesMETfiltersMoriond17(is_real_data);

  if (is_real_data && isReMiniAOD) {
      evt_egclean_pfmet = tas::evt_egclean_pfmet();
      evt_muegclean_pfmet = tas::evt_muegclean_pfmet();
      evt_muegcleanfix_pfmet = tas::evt_muegcleanfix_pfmet();
      evt_uncorr_pfmet = tas::evt_uncorr_pfmet();
      filt_noBadMuons = tas::filt_noBadMuons();
      filt_duplicateMuons = tas::filt_duplicateMuons();
      filt_badMuons = tas::filt_badMuons();
  }


  //Fill Baby
  BabyTree->Fill();

  // int SR = signalRegion2016(njets, nbtags, met, ht, mtmin, lep1_id, lep2_id, lep1_coneCorrPt, lep2_coneCorrPt);
  // int SR = signalRegionChargeSplit(njets, nbtags, met, ht, mtmin, lep1_id, lep2_id, lep1_coneCorrPt, lep2_coneCorrPt);
  int nleps = 2;
  int isClass6 = hyp_class == 6;
  if (lep3_passes_id) nleps++;
  int SR = signalRegionTest(njets, nbtags, met, ht, mtmin, lep1_id, lep2_id, lep1_coneCorrPt, lep2_coneCorrPt, nleps, isClass6);
  babyErrorStruct.SR = SR;
  if (hyp_class == 3 || (hyp_class == 6 && lep3_passes_id)) babyErrorStruct.isGood = true;

  return babyErrorStruct;

}

float babyMaker::getBtagEffFromFile(float pt, float eta, int mcFlavour){
  if(!h_btag_eff_b || !h_btag_eff_c || !h_btag_eff_udsg) {
    std::cout << "babyMaker::getBtagEffFromFile: ERROR: missing input hists" << std::endl;
    return 1.;
  }

  // only use pt bins up to 400 GeV for charm and udsg
  float pt_cutoff = std::max(20.,std::min(399.,double(pt)));
  TH2D* h(0);
  if (abs(mcFlavour) == 5) {
    h = h_btag_eff_b;
    // use pt bins up to 600 GeV for b
    pt_cutoff = std::max(20.,std::min(599.,double(pt)));
  }
  else if (abs(mcFlavour) == 4) {
    h = h_btag_eff_c;
  }
  else {
    h = h_btag_eff_udsg;
  }

  int binx = h->GetXaxis()->FindBin(pt_cutoff);
  int biny = h->GetYaxis()->FindBin(fabs(eta));
  return h->GetBinContent(binx,biny);
}
