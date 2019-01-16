#include "CMS3.h"
CMS3 cms3;

void CMS3::Init(TTree *tree) {
  tag_p4_branch = 0;
  if (tree->GetBranch("tag_p4") != 0) {
    tag_p4_branch = tree->GetBranch("tag_p4");
    if (tag_p4_branch) { tag_p4_branch->SetAddress(&tag_p4_); }
  }
  close_jet_v5_branch = 0;
  if (tree->GetBranch("close_jet_v5") != 0) {
    close_jet_v5_branch = tree->GetBranch("close_jet_v5");
    if (close_jet_v5_branch) { close_jet_v5_branch->SetAddress(&close_jet_v5_); }
  }
  p4_branch = 0;
  if (tree->GetBranch("p4") != 0) {
    p4_branch = tree->GetBranch("p4");
    if (p4_branch) { p4_branch->SetAddress(&p4_); }
  }
  dilep_p4_branch = 0;
  if (tree->GetBranch("dilep_p4") != 0) {
    dilep_p4_branch = tree->GetBranch("dilep_p4");
    if (dilep_p4_branch) { dilep_p4_branch->SetAddress(&dilep_p4_); }
  }
  tree->SetMakeClass(1);
  passes_SS_fo_looseMVA_noiso_v6_branch = 0;
  if (tree->GetBranch("passes_SS_fo_looseMVA_noiso_v6") != 0) {
    passes_SS_fo_looseMVA_noiso_v6_branch = tree->GetBranch("passes_SS_fo_looseMVA_noiso_v6");
    if (passes_SS_fo_looseMVA_noiso_v6_branch) { passes_SS_fo_looseMVA_noiso_v6_branch->SetAddress(&passes_SS_fo_looseMVA_noiso_v6_); }
  }
  passes_SS_fo_noiso_v6_branch = 0;
  if (tree->GetBranch("passes_SS_fo_noiso_v6") != 0) {
    passes_SS_fo_noiso_v6_branch = tree->GetBranch("passes_SS_fo_noiso_v6");
    if (passes_SS_fo_noiso_v6_branch) { passes_SS_fo_noiso_v6_branch->SetAddress(&passes_SS_fo_noiso_v6_); }
  }
  njets_branch = 0;
  if (tree->GetBranch("njets") != 0) {
    njets_branch = tree->GetBranch("njets");
    if (njets_branch) { njets_branch->SetAddress(&njets_); }
  }
  p4_pt_branch = 0;
  if (tree->GetBranch("p4_pt") != 0) {
    p4_pt_branch = tree->GetBranch("p4_pt");
    if (p4_pt_branch) { p4_pt_branch->SetAddress(&p4_pt_); }
  }
  HLT_Mu8_TrkIsoVVL_branch = 0;
  if (tree->GetBranch("HLT_Mu8_TrkIsoVVL") != 0) {
    HLT_Mu8_TrkIsoVVL_branch = tree->GetBranch("HLT_Mu8_TrkIsoVVL");
    if (HLT_Mu8_TrkIsoVVL_branch) { HLT_Mu8_TrkIsoVVL_branch->SetAddress(&HLT_Mu8_TrkIsoVVL_); }
  }
  evt_corrMET_branch = 0;
  if (tree->GetBranch("evt_corrMET") != 0) {
    evt_corrMET_branch = tree->GetBranch("evt_corrMET");
    if (evt_corrMET_branch) { evt_corrMET_branch->SetAddress(&evt_corrMET_); }
  }
  passes_SS_fo_v6_branch = 0;
  if (tree->GetBranch("passes_SS_fo_v6") != 0) {
    passes_SS_fo_v6_branch = tree->GetBranch("passes_SS_fo_v6");
    if (passes_SS_fo_v6_branch) { passes_SS_fo_v6_branch->SetAddress(&passes_SS_fo_v6_); }
  }
  etaSC_branch = 0;
  if (tree->GetBranch("etaSC") != 0) {
    etaSC_branch = tree->GetBranch("etaSC");
    if (etaSC_branch) { etaSC_branch->SetAddress(&etaSC_); }
  }
  evt_isRealData_branch = 0;
  if (tree->GetBranch("evt_isRealData") != 0) {
    evt_isRealData_branch = tree->GetBranch("evt_isRealData");
    if (evt_isRealData_branch) { evt_isRealData_branch->SetAddress(&evt_isRealData_); }
  }
  HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_branch = 0;
  if (tree->GetBranch("HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30") != 0) {
    HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_branch = tree->GetBranch("HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30");
    if (HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_branch) { HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->SetAddress(&HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_); }
  }
  ip3d_branch = 0;
  if (tree->GetBranch("ip3d") != 0) {
    ip3d_branch = tree->GetBranch("ip3d");
    if (ip3d_branch) { ip3d_branch->SetAddress(&ip3d_); }
  }
  HLT_Ele12_CaloIdM_TrackIdM_PFJet30_branch = 0;
  if (tree->GetBranch("HLT_Ele12_CaloIdM_TrackIdM_PFJet30") != 0) {
    HLT_Ele12_CaloIdM_TrackIdM_PFJet30_branch = tree->GetBranch("HLT_Ele12_CaloIdM_TrackIdM_PFJet30");
    if (HLT_Ele12_CaloIdM_TrackIdM_PFJet30_branch) { HLT_Ele12_CaloIdM_TrackIdM_PFJet30_branch->SetAddress(&HLT_Ele12_CaloIdM_TrackIdM_PFJet30_); }
  }
  coneCorrPt_branch = 0;
  if (tree->GetBranch("coneCorrPt") != 0) {
    coneCorrPt_branch = tree->GetBranch("coneCorrPt");
    if (coneCorrPt_branch) { coneCorrPt_branch->SetAddress(&coneCorrPt_); }
  }
  HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch = 0;
  if (tree->GetBranch("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ") != 0) {
    HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch = tree->GetBranch("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ");
    if (HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch) { HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch->SetAddress(&HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_); }
  }
  id_branch = 0;
  if (tree->GetBranch("id") != 0) {
    id_branch = tree->GetBranch("id");
    if (id_branch) { id_branch->SetAddress(&id_); }
  }
  qscale_branch = 0;
  if (tree->GetBranch("qscale") != 0) {
    qscale_branch = tree->GetBranch("qscale");
    if (qscale_branch) { qscale_branch->SetAddress(&qscale_); }
  }
  HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_branch = 0;
  if (tree->GetBranch("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL") != 0) {
    HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_branch = tree->GetBranch("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL");
    if (HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_branch) { HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_branch->SetAddress(&HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_); }
  }
  HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch = 0;
  if (tree->GetBranch("HLT_Ele8_CaloIdM_TrackIdM_PFJet30") != 0) {
    HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch = tree->GetBranch("HLT_Ele8_CaloIdM_TrackIdM_PFJet30");
    if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch) { HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch->SetAddress(&HLT_Ele8_CaloIdM_TrackIdM_PFJet30_); }
  }
  nloose_mu_branch = 0;
  if (tree->GetBranch("nloose_mu") != 0) {
    nloose_mu_branch = tree->GetBranch("nloose_mu");
    if (nloose_mu_branch) { nloose_mu_branch->SetAddress(&nloose_mu_); }
  }
  mt_branch = 0;
  if (tree->GetBranch("mt") != 0) {
    mt_branch = tree->GetBranch("mt");
    if (mt_branch) { mt_branch->SetAddress(&mt_); }
  }
  isTriggerSafenoIso_branch = 0;
  if (tree->GetBranch("isTriggerSafenoIso") != 0) {
    isTriggerSafenoIso_branch = tree->GetBranch("isTriggerSafenoIso");
    if (isTriggerSafenoIso_branch) { isTriggerSafenoIso_branch->SetAddress(&isTriggerSafenoIso_); }
  }
  HLT_Ele23_CaloIdM_TrackIdM_PFJet30_branch = 0;
  if (tree->GetBranch("HLT_Ele23_CaloIdM_TrackIdM_PFJet30") != 0) {
    HLT_Ele23_CaloIdM_TrackIdM_PFJet30_branch = tree->GetBranch("HLT_Ele23_CaloIdM_TrackIdM_PFJet30");
    if (HLT_Ele23_CaloIdM_TrackIdM_PFJet30_branch) { HLT_Ele23_CaloIdM_TrackIdM_PFJet30_branch->SetAddress(&HLT_Ele23_CaloIdM_TrackIdM_PFJet30_); }
  }
  passes_any_trigger_branch = 0;
  if (tree->GetBranch("passes_any_trigger") != 0) {
    passes_any_trigger_branch = tree->GetBranch("passes_any_trigger");
    if (passes_any_trigger_branch) { passes_any_trigger_branch->SetAddress(&passes_any_trigger_); }
  }
  HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_branch = 0;
  if (tree->GetBranch("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL") != 0) {
    HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_branch = tree->GetBranch("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL");
    if (HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_branch) { HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_branch->SetAddress(&HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_); }
  }
  evt_run_branch = 0;
  if (tree->GetBranch("evt_run") != 0) {
    evt_run_branch = tree->GetBranch("evt_run");
    if (evt_run_branch) { evt_run_branch->SetAddress(&evt_run_); }
  }
  passes_SS_tight_noiso_v6_branch = 0;
  if (tree->GetBranch("passes_SS_tight_noiso_v6") != 0) {
    passes_SS_tight_noiso_v6_branch = tree->GetBranch("passes_SS_tight_noiso_v6");
    if (passes_SS_tight_noiso_v6_branch) { passes_SS_tight_noiso_v6_branch->SetAddress(&passes_SS_tight_noiso_v6_); }
  }
  ht_SS_branch = 0;
  if (tree->GetBranch("ht_SS") != 0) {
    ht_SS_branch = tree->GetBranch("ht_SS");
    if (ht_SS_branch) { ht_SS_branch->SetAddress(&ht_SS_); }
  }
  puweight_branch = 0;
  if (tree->GetBranch("puweight") != 0) {
    puweight_branch = tree->GetBranch("puweight");
    if (puweight_branch) { puweight_branch->SetAddress(&puweight_); }
  }
  sample_branch = 0;
  if (tree->GetBranch("sample") != 0) {
    sample_branch = tree->GetBranch("sample");
    if (sample_branch) { sample_branch->SetAddress(&sample_); }
  }
  HLT_DoubleMu8_Mass8_PFHT300_branch = 0;
  if (tree->GetBranch("HLT_DoubleMu8_Mass8_PFHT300") != 0) {
    HLT_DoubleMu8_Mass8_PFHT300_branch = tree->GetBranch("HLT_DoubleMu8_Mass8_PFHT300");
    if (HLT_DoubleMu8_Mass8_PFHT300_branch) { HLT_DoubleMu8_Mass8_PFHT300_branch->SetAddress(&HLT_DoubleMu8_Mass8_PFHT300_); }
  }
  ptratio_v5_branch = 0;
  if (tree->GetBranch("ptratio_v5") != 0) {
    ptratio_v5_branch = tree->GetBranch("ptratio_v5");
    if (ptratio_v5_branch) { ptratio_v5_branch->SetAddress(&ptratio_v5_); }
  }
  miniiso_branch = 0;
  if (tree->GetBranch("miniiso") != 0) {
    miniiso_branch = tree->GetBranch("miniiso");
    if (miniiso_branch) { miniiso_branch->SetAddress(&miniiso_); }
  }
  HLT_Mu17_TrkIsoVVL_branch = 0;
  if (tree->GetBranch("HLT_Mu17_TrkIsoVVL") != 0) {
    HLT_Mu17_TrkIsoVVL_branch = tree->GetBranch("HLT_Mu17_TrkIsoVVL");
    if (HLT_Mu17_TrkIsoVVL_branch) { HLT_Mu17_TrkIsoVVL_branch->SetAddress(&HLT_Mu17_TrkIsoVVL_); }
  }
  HLT_Ele18_CaloIdM_TrackIdM_PFJet30_branch = 0;
  if (tree->GetBranch("HLT_Ele18_CaloIdM_TrackIdM_PFJet30") != 0) {
    HLT_Ele18_CaloIdM_TrackIdM_PFJet30_branch = tree->GetBranch("HLT_Ele18_CaloIdM_TrackIdM_PFJet30");
    if (HLT_Ele18_CaloIdM_TrackIdM_PFJet30_branch) { HLT_Ele18_CaloIdM_TrackIdM_PFJet30_branch->SetAddress(&HLT_Ele18_CaloIdM_TrackIdM_PFJet30_); }
  }
  nvtx_branch = 0;
  if (tree->GetBranch("nvtx") != 0) {
    nvtx_branch = tree->GetBranch("nvtx");
    if (nvtx_branch) { nvtx_branch->SetAddress(&nvtx_); }
  }
  isTriggerSafe_branch = 0;
  if (tree->GetBranch("isTriggerSafe") != 0) {
    isTriggerSafe_branch = tree->GetBranch("isTriggerSafe");
    if (isTriggerSafe_branch) { isTriggerSafe_branch->SetAddress(&isTriggerSafe_); }
  }
  HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_branch = 0;
  if (tree->GetBranch("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL") != 0) {
    HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_branch = tree->GetBranch("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL");
    if (HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_branch) { HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_branch->SetAddress(&HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_); }
  }
  njets_recoil_branch = 0;
  if (tree->GetBranch("njets_recoil") != 0) {
    njets_recoil_branch = tree->GetBranch("njets_recoil");
    if (njets_recoil_branch) { njets_recoil_branch->SetAddress(&njets_recoil_); }
  }
  motherID_branch = 0;
  if (tree->GetBranch("motherID") != 0) {
    motherID_branch = tree->GetBranch("motherID");
    if (motherID_branch) { motherID_branch->SetAddress(&motherID_); }
  }
  HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_branch = 0;
  if (tree->GetBranch("HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30") != 0) {
    HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_branch = tree->GetBranch("HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30");
    if (HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_branch) { HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->SetAddress(&HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_); }
  }
  tag_pt_branch = 0;
  if (tree->GetBranch("tag_pt") != 0) {
    tag_pt_branch = tree->GetBranch("tag_pt");
    if (tag_pt_branch) { tag_pt_branch->SetAddress(&tag_pt_); }
  }
  passes_SS_fo_looseMVA_v6_branch = 0;
  if (tree->GetBranch("passes_SS_fo_looseMVA_v6") != 0) {
    passes_SS_fo_looseMVA_v6_branch = tree->GetBranch("passes_SS_fo_looseMVA_v6");
    if (passes_SS_fo_looseMVA_v6_branch) { passes_SS_fo_looseMVA_v6_branch->SetAddress(&passes_SS_fo_looseMVA_v6_); }
  }
  passes_SS_tight_v6_branch = 0;
  if (tree->GetBranch("passes_SS_tight_v6") != 0) {
    passes_SS_tight_v6_branch = tree->GetBranch("passes_SS_tight_v6");
    if (passes_SS_tight_v6_branch) { passes_SS_tight_v6_branch->SetAddress(&passes_SS_tight_v6_); }
  }
  have_tag_branch = 0;
  if (tree->GetBranch("have_tag") != 0) {
    have_tag_branch = tree->GetBranch("have_tag");
    if (have_tag_branch) { have_tag_branch->SetAddress(&have_tag_); }
  }
  nFOs_SS_branch = 0;
  if (tree->GetBranch("nFOs_SS") != 0) {
    nFOs_SS_branch = tree->GetBranch("nFOs_SS");
    if (nFOs_SS_branch) { nFOs_SS_branch->SetAddress(&nFOs_SS_); }
  }
  HLT_IsoMu27_branch = 0;
  if (tree->GetBranch("HLT_IsoMu27") != 0) {
    HLT_IsoMu27_branch = tree->GetBranch("HLT_IsoMu27");
    if (HLT_IsoMu27_branch) { HLT_IsoMu27_branch->SetAddress(&HLT_IsoMu27_); }
  }
  ntight_el_branch = 0;
  if (tree->GetBranch("ntight_el") != 0) {
    ntight_el_branch = tree->GetBranch("ntight_el");
    if (ntight_el_branch) { ntight_el_branch->SetAddress(&ntight_el_); }
  }
  HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch = 0;
  if (tree->GetBranch("HLT_Ele17_CaloIdM_TrackIdM_PFJet30") != 0) {
    HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch = tree->GetBranch("HLT_Ele17_CaloIdM_TrackIdM_PFJet30");
    if (HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch) { HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch->SetAddress(&HLT_Ele17_CaloIdM_TrackIdM_PFJet30_); }
  }
  ip3derr_branch = 0;
  if (tree->GetBranch("ip3derr") != 0) {
    ip3derr_branch = tree->GetBranch("ip3derr");
    if (ip3derr_branch) { ip3derr_branch->SetAddress(&ip3derr_); }
  }
  ptrelv1_branch = 0;
  if (tree->GetBranch("ptrelv1") != 0) {
    ptrelv1_branch = tree->GetBranch("ptrelv1");
    if (ptrelv1_branch) { ptrelv1_branch->SetAddress(&ptrelv1_); }
  }
  HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_branch = 0;
  if (tree->GetBranch("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300") != 0) {
    HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_branch = tree->GetBranch("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300");
    if (HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_branch) { HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_branch->SetAddress(&HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_); }
  }
  HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_branch = 0;
  if (tree->GetBranch("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL") != 0) {
    HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_branch = tree->GetBranch("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL");
    if (HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_branch) { HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_branch->SetAddress(&HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_); }
  }
  evt_event_branch = 0;
  if (tree->GetBranch("evt_event") != 0) {
    evt_event_branch = tree->GetBranch("evt_event");
    if (evt_event_branch) { evt_event_branch->SetAddress(&evt_event_); }
  }
  HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch = 0;
  if (tree->GetBranch("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ") != 0) {
    HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch = tree->GetBranch("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ");
    if (HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch) { HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch->SetAddress(&HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_); }
  }
  passes_met_filters_branch = 0;
  if (tree->GetBranch("passes_met_filters") != 0) {
    passes_met_filters_branch = tree->GetBranch("passes_met_filters");
    if (passes_met_filters_branch) { passes_met_filters_branch->SetAddress(&passes_met_filters_); }
  }
  dilep_mass_branch = 0;
  if (tree->GetBranch("dilep_mass") != 0) {
    dilep_mass_branch = tree->GetBranch("dilep_mass");
    if (dilep_mass_branch) { dilep_mass_branch->SetAddress(&dilep_mass_); }
  }
  nloose_el_branch = 0;
  if (tree->GetBranch("nloose_el") != 0) {
    nloose_el_branch = tree->GetBranch("nloose_el");
    if (nloose_el_branch) { nloose_el_branch->SetAddress(&nloose_el_); }
  }
  HLT_Ele12_CaloIdL_TrackIdL_IsoVL_branch = 0;
  if (tree->GetBranch("HLT_Ele12_CaloIdL_TrackIdL_IsoVL") != 0) {
    HLT_Ele12_CaloIdL_TrackIdL_IsoVL_branch = tree->GetBranch("HLT_Ele12_CaloIdL_TrackIdL_IsoVL");
    if (HLT_Ele12_CaloIdL_TrackIdL_IsoVL_branch) { HLT_Ele12_CaloIdL_TrackIdL_IsoVL_branch->SetAddress(&HLT_Ele12_CaloIdL_TrackIdL_IsoVL_); }
  }
  HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch = 0;
  if (tree->GetBranch("HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30") != 0) {
    HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch = tree->GetBranch("HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30");
    if (HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch) { HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->SetAddress(&HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_); }
  }
  HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_branch = 0;
  if (tree->GetBranch("HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300") != 0) {
    HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_branch = tree->GetBranch("HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300");
    if (HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_branch) { HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_branch->SetAddress(&HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_); }
  }
  idx_branch = 0;
  if (tree->GetBranch("idx") != 0) {
    idx_branch = tree->GetBranch("idx");
    if (idx_branch) { idx_branch->SetAddress(&idx_); }
  }
  scale1fb_branch = 0;
  if (tree->GetBranch("scale1fb") != 0) {
    scale1fb_branch = tree->GetBranch("scale1fb");
    if (scale1fb_branch) { scale1fb_branch->SetAddress(&scale1fb_); }
  }
  HLT_Mu8_branch = 0;
  if (tree->GetBranch("HLT_Mu8") != 0) {
    HLT_Mu8_branch = tree->GetBranch("HLT_Mu8");
    if (HLT_Mu8_branch) { HLT_Mu8_branch->SetAddress(&HLT_Mu8_); }
  }
  p4_eta_branch = 0;
  if (tree->GetBranch("p4_eta") != 0) {
    p4_eta_branch = tree->GetBranch("p4_eta");
    if (p4_eta_branch) { p4_eta_branch->SetAddress(&p4_eta_); }
  }
  evt_corrMETPhi_branch = 0;
  if (tree->GetBranch("evt_corrMETPhi") != 0) {
    evt_corrMETPhi_branch = tree->GetBranch("evt_corrMETPhi");
    if (evt_corrMETPhi_branch) { evt_corrMETPhi_branch->SetAddress(&evt_corrMETPhi_); }
  }
  ntight_mu_branch = 0;
  if (tree->GetBranch("ntight_mu") != 0) {
    ntight_mu_branch = tree->GetBranch("ntight_mu");
    if (ntight_mu_branch) { ntight_mu_branch->SetAddress(&ntight_mu_); }
  }
  HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_branch = 0;
  if (tree->GetBranch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL") != 0) {
    HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_branch = tree->GetBranch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL");
    if (HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_branch) { HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_branch->SetAddress(&HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_); }
  }
  mva_branch = 0;
  if (tree->GetBranch("mva") != 0) {
    mva_branch = tree->GetBranch("mva");
    if (mva_branch) { mva_branch->SetAddress(&mva_); }
  }
  HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch = 0;
  if (tree->GetBranch("HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30") != 0) {
    HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch = tree->GetBranch("HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30");
    if (HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch) { HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->SetAddress(&HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_); }
  }
  HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_branch = 0;
  if (tree->GetBranch("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ") != 0) {
    HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_branch = tree->GetBranch("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ");
    if (HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_branch) { HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_branch->SetAddress(&HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_); }
  }
  evt_lumiBlock_branch = 0;
  if (tree->GetBranch("evt_lumiBlock") != 0) {
    evt_lumiBlock_branch = tree->GetBranch("evt_lumiBlock");
    if (evt_lumiBlock_branch) { evt_lumiBlock_branch->SetAddress(&evt_lumiBlock_); }
  }
  trueNumInt_branch = 0;
  if (tree->GetBranch("trueNumInt") != 0) {
    trueNumInt_branch = tree->GetBranch("trueNumInt");
    if (trueNumInt_branch) { trueNumInt_branch->SetAddress(&trueNumInt_); }
  }
  HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_branch = 0;
  if (tree->GetBranch("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL") != 0) {
    HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_branch = tree->GetBranch("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL");
    if (HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_branch) { HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_branch->SetAddress(&HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_); }
  }
  HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_branch = 0;
  if (tree->GetBranch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ") != 0) {
    HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_branch = tree->GetBranch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ");
    if (HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_branch) { HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_branch->SetAddress(&HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_); }
  }
  HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_branch = 0;
  if (tree->GetBranch("HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30") != 0) {
    HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_branch = tree->GetBranch("HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30");
    if (HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_branch) { HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->SetAddress(&HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_); }
  }
  HLT_Ele23_CaloIdL_TrackIdL_IsoVL_branch = 0;
  if (tree->GetBranch("HLT_Ele23_CaloIdL_TrackIdL_IsoVL") != 0) {
    HLT_Ele23_CaloIdL_TrackIdL_IsoVL_branch = tree->GetBranch("HLT_Ele23_CaloIdL_TrackIdL_IsoVL");
    if (HLT_Ele23_CaloIdL_TrackIdL_IsoVL_branch) { HLT_Ele23_CaloIdL_TrackIdL_IsoVL_branch->SetAddress(&HLT_Ele23_CaloIdL_TrackIdL_IsoVL_); }
  }
  HLT_Mu17_branch = 0;
  if (tree->GetBranch("HLT_Mu17") != 0) {
    HLT_Mu17_branch = tree->GetBranch("HLT_Mu17");
    if (HLT_Mu17_branch) { HLT_Mu17_branch->SetAddress(&HLT_Mu17_); }
  }
  tree->SetMakeClass(0);
}
void CMS3::GetEntry(unsigned int idx) {
  index = idx;
  passes_SS_fo_looseMVA_noiso_v6_isLoaded = false;
  passes_SS_fo_noiso_v6_isLoaded = false;
  tag_p4_isLoaded = false;
  njets_isLoaded = false;
  p4_pt_isLoaded = false;
  HLT_Mu8_TrkIsoVVL_isLoaded = false;
  evt_corrMET_isLoaded = false;
  passes_SS_fo_v6_isLoaded = false;
  etaSC_isLoaded = false;
  evt_isRealData_isLoaded = false;
  HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = false;
  ip3d_isLoaded = false;
  HLT_Ele12_CaloIdM_TrackIdM_PFJet30_isLoaded = false;
  coneCorrPt_isLoaded = false;
  HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_isLoaded = false;
  id_isLoaded = false;
  qscale_isLoaded = false;
  HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_isLoaded = false;
  HLT_Ele8_CaloIdM_TrackIdM_PFJet30_isLoaded = false;
  nloose_mu_isLoaded = false;
  mt_isLoaded = false;
  isTriggerSafenoIso_isLoaded = false;
  HLT_Ele23_CaloIdM_TrackIdM_PFJet30_isLoaded = false;
  passes_any_trigger_isLoaded = false;
  HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_isLoaded = false;
  evt_run_isLoaded = false;
  passes_SS_tight_noiso_v6_isLoaded = false;
  ht_SS_isLoaded = false;
  puweight_isLoaded = false;
  sample_isLoaded = false;
  HLT_DoubleMu8_Mass8_PFHT300_isLoaded = false;
  ptratio_v5_isLoaded = false;
  miniiso_isLoaded = false;
  HLT_Mu17_TrkIsoVVL_isLoaded = false;
  HLT_Ele18_CaloIdM_TrackIdM_PFJet30_isLoaded = false;
  nvtx_isLoaded = false;
  isTriggerSafe_isLoaded = false;
  HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_isLoaded = false;
  njets_recoil_isLoaded = false;
  close_jet_v5_isLoaded = false;
  motherID_isLoaded = false;
  HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = false;
  tag_pt_isLoaded = false;
  passes_SS_fo_looseMVA_v6_isLoaded = false;
  passes_SS_tight_v6_isLoaded = false;
  have_tag_isLoaded = false;
  nFOs_SS_isLoaded = false;
  HLT_IsoMu27_isLoaded = false;
  ntight_el_isLoaded = false;
  HLT_Ele17_CaloIdM_TrackIdM_PFJet30_isLoaded = false;
  ip3derr_isLoaded = false;
  ptrelv1_isLoaded = false;
  HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_isLoaded = false;
  HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_isLoaded = false;
  evt_event_isLoaded = false;
  HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_isLoaded = false;
  passes_met_filters_isLoaded = false;
  dilep_mass_isLoaded = false;
  nloose_el_isLoaded = false;
  HLT_Ele12_CaloIdL_TrackIdL_IsoVL_isLoaded = false;
  HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = false;
  HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_isLoaded = false;
  idx_isLoaded = false;
  p4_isLoaded = false;
  scale1fb_isLoaded = false;
  HLT_Mu8_isLoaded = false;
  p4_eta_isLoaded = false;
  evt_corrMETPhi_isLoaded = false;
  ntight_mu_isLoaded = false;
  HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_isLoaded = false;
  mva_isLoaded = false;
  HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = false;
  HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_isLoaded = false;
  evt_lumiBlock_isLoaded = false;
  dilep_p4_isLoaded = false;
  trueNumInt_isLoaded = false;
  HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_isLoaded = false;
  HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_isLoaded = false;
  HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = false;
  HLT_Ele23_CaloIdL_TrackIdL_IsoVL_isLoaded = false;
  HLT_Mu17_isLoaded = false;
}
void CMS3::LoadAllBranches() {
  if (passes_SS_fo_looseMVA_noiso_v6_branch != 0) passes_SS_fo_looseMVA_noiso_v6();
  if (passes_SS_fo_noiso_v6_branch != 0) passes_SS_fo_noiso_v6();
  if (tag_p4_branch != 0) tag_p4();
  if (njets_branch != 0) njets();
  if (p4_pt_branch != 0) p4_pt();
  if (HLT_Mu8_TrkIsoVVL_branch != 0) HLT_Mu8_TrkIsoVVL();
  if (evt_corrMET_branch != 0) evt_corrMET();
  if (passes_SS_fo_v6_branch != 0) passes_SS_fo_v6();
  if (etaSC_branch != 0) etaSC();
  if (evt_isRealData_branch != 0) evt_isRealData();
  if (HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30();
  if (ip3d_branch != 0) ip3d();
  if (HLT_Ele12_CaloIdM_TrackIdM_PFJet30_branch != 0) HLT_Ele12_CaloIdM_TrackIdM_PFJet30();
  if (coneCorrPt_branch != 0) coneCorrPt();
  if (HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch != 0) HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ();
  if (id_branch != 0) id();
  if (qscale_branch != 0) qscale();
  if (HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_branch != 0) HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL();
  if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch != 0) HLT_Ele8_CaloIdM_TrackIdM_PFJet30();
  if (nloose_mu_branch != 0) nloose_mu();
  if (mt_branch != 0) mt();
  if (isTriggerSafenoIso_branch != 0) isTriggerSafenoIso();
  if (HLT_Ele23_CaloIdM_TrackIdM_PFJet30_branch != 0) HLT_Ele23_CaloIdM_TrackIdM_PFJet30();
  if (passes_any_trigger_branch != 0) passes_any_trigger();
  if (HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_branch != 0) HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL();
  if (evt_run_branch != 0) evt_run();
  if (passes_SS_tight_noiso_v6_branch != 0) passes_SS_tight_noiso_v6();
  if (ht_SS_branch != 0) ht_SS();
  if (puweight_branch != 0) puweight();
  if (sample_branch != 0) sample();
  if (HLT_DoubleMu8_Mass8_PFHT300_branch != 0) HLT_DoubleMu8_Mass8_PFHT300();
  if (ptratio_v5_branch != 0) ptratio_v5();
  if (miniiso_branch != 0) miniiso();
  if (HLT_Mu17_TrkIsoVVL_branch != 0) HLT_Mu17_TrkIsoVVL();
  if (HLT_Ele18_CaloIdM_TrackIdM_PFJet30_branch != 0) HLT_Ele18_CaloIdM_TrackIdM_PFJet30();
  if (nvtx_branch != 0) nvtx();
  if (isTriggerSafe_branch != 0) isTriggerSafe();
  if (HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_branch != 0) HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL();
  if (njets_recoil_branch != 0) njets_recoil();
  if (close_jet_v5_branch != 0) close_jet_v5();
  if (motherID_branch != 0) motherID();
  if (HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30();
  if (tag_pt_branch != 0) tag_pt();
  if (passes_SS_fo_looseMVA_v6_branch != 0) passes_SS_fo_looseMVA_v6();
  if (passes_SS_tight_v6_branch != 0) passes_SS_tight_v6();
  if (have_tag_branch != 0) have_tag();
  if (nFOs_SS_branch != 0) nFOs_SS();
  if (HLT_IsoMu27_branch != 0) HLT_IsoMu27();
  if (ntight_el_branch != 0) ntight_el();
  if (HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch != 0) HLT_Ele17_CaloIdM_TrackIdM_PFJet30();
  if (ip3derr_branch != 0) ip3derr();
  if (ptrelv1_branch != 0) ptrelv1();
  if (HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_branch != 0) HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300();
  if (HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_branch != 0) HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL();
  if (evt_event_branch != 0) evt_event();
  if (HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch != 0) HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ();
  if (passes_met_filters_branch != 0) passes_met_filters();
  if (dilep_mass_branch != 0) dilep_mass();
  if (nloose_el_branch != 0) nloose_el();
  if (HLT_Ele12_CaloIdL_TrackIdL_IsoVL_branch != 0) HLT_Ele12_CaloIdL_TrackIdL_IsoVL();
  if (HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30();
  if (HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_branch != 0) HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300();
  if (idx_branch != 0) idx();
  if (p4_branch != 0) p4();
  if (scale1fb_branch != 0) scale1fb();
  if (HLT_Mu8_branch != 0) HLT_Mu8();
  if (p4_eta_branch != 0) p4_eta();
  if (evt_corrMETPhi_branch != 0) evt_corrMETPhi();
  if (ntight_mu_branch != 0) ntight_mu();
  if (HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_branch != 0) HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL();
  if (mva_branch != 0) mva();
  if (HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30();
  if (HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_branch != 0) HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ();
  if (evt_lumiBlock_branch != 0) evt_lumiBlock();
  if (dilep_p4_branch != 0) dilep_p4();
  if (trueNumInt_branch != 0) trueNumInt();
  if (HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_branch != 0) HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL();
  if (HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_branch != 0) HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ();
  if (HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30();
  if (HLT_Ele23_CaloIdL_TrackIdL_IsoVL_branch != 0) HLT_Ele23_CaloIdL_TrackIdL_IsoVL();
  if (HLT_Mu17_branch != 0) HLT_Mu17();
}
const bool &CMS3::passes_SS_fo_looseMVA_noiso_v6() {
  if (not passes_SS_fo_looseMVA_noiso_v6_isLoaded) {
    if (passes_SS_fo_looseMVA_noiso_v6_branch != 0) {
      passes_SS_fo_looseMVA_noiso_v6_branch->GetEntry(index);
    } else {
      printf("branch passes_SS_fo_looseMVA_noiso_v6_branch does not exist!\n");
      exit(1);
    }
    passes_SS_fo_looseMVA_noiso_v6_isLoaded = true;
  }
  return passes_SS_fo_looseMVA_noiso_v6_;
}
const bool &CMS3::passes_SS_fo_noiso_v6() {
  if (not passes_SS_fo_noiso_v6_isLoaded) {
    if (passes_SS_fo_noiso_v6_branch != 0) {
      passes_SS_fo_noiso_v6_branch->GetEntry(index);
    } else {
      printf("branch passes_SS_fo_noiso_v6_branch does not exist!\n");
      exit(1);
    }
    passes_SS_fo_noiso_v6_isLoaded = true;
  }
  return passes_SS_fo_noiso_v6_;
}
const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &CMS3::tag_p4() {
  if (not tag_p4_isLoaded) {
    if (tag_p4_branch != 0) {
      tag_p4_branch->GetEntry(index);
    } else {
      printf("branch tag_p4_branch does not exist!\n");
      exit(1);
    }
    tag_p4_isLoaded = true;
  }
  return *tag_p4_;
}
const int &CMS3::njets() {
  if (not njets_isLoaded) {
    if (njets_branch != 0) {
      njets_branch->GetEntry(index);
    } else {
      printf("branch njets_branch does not exist!\n");
      exit(1);
    }
    njets_isLoaded = true;
  }
  return njets_;
}
const float &CMS3::p4_pt() {
  if (not p4_pt_isLoaded) {
    if (p4_pt_branch != 0) {
      p4_pt_branch->GetEntry(index);
    } else {
      printf("branch p4_pt_branch does not exist!\n");
      exit(1);
    }
    p4_pt_isLoaded = true;
  }
  return p4_pt_;
}
const int &CMS3::HLT_Mu8_TrkIsoVVL() {
  if (not HLT_Mu8_TrkIsoVVL_isLoaded) {
    if (HLT_Mu8_TrkIsoVVL_branch != 0) {
      HLT_Mu8_TrkIsoVVL_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu8_TrkIsoVVL_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu8_TrkIsoVVL_isLoaded = true;
  }
  return HLT_Mu8_TrkIsoVVL_;
}
const float &CMS3::evt_corrMET() {
  if (not evt_corrMET_isLoaded) {
    if (evt_corrMET_branch != 0) {
      evt_corrMET_branch->GetEntry(index);
    } else {
      printf("branch evt_corrMET_branch does not exist!\n");
      exit(1);
    }
    evt_corrMET_isLoaded = true;
  }
  return evt_corrMET_;
}
const bool &CMS3::passes_SS_fo_v6() {
  if (not passes_SS_fo_v6_isLoaded) {
    if (passes_SS_fo_v6_branch != 0) {
      passes_SS_fo_v6_branch->GetEntry(index);
    } else {
      printf("branch passes_SS_fo_v6_branch does not exist!\n");
      exit(1);
    }
    passes_SS_fo_v6_isLoaded = true;
  }
  return passes_SS_fo_v6_;
}
const float &CMS3::etaSC() {
  if (not etaSC_isLoaded) {
    if (etaSC_branch != 0) {
      etaSC_branch->GetEntry(index);
    } else {
      printf("branch etaSC_branch does not exist!\n");
      exit(1);
    }
    etaSC_isLoaded = true;
  }
  return etaSC_;
}
const bool &CMS3::evt_isRealData() {
  if (not evt_isRealData_isLoaded) {
    if (evt_isRealData_branch != 0) {
      evt_isRealData_branch->GetEntry(index);
    } else {
      printf("branch evt_isRealData_branch does not exist!\n");
      exit(1);
    }
    evt_isRealData_isLoaded = true;
  }
  return evt_isRealData_;
}
const int &CMS3::HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30() {
  if (not HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded) {
    if (HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) {
      HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = true;
  }
  return HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_;
}
const float &CMS3::ip3d() {
  if (not ip3d_isLoaded) {
    if (ip3d_branch != 0) {
      ip3d_branch->GetEntry(index);
    } else {
      printf("branch ip3d_branch does not exist!\n");
      exit(1);
    }
    ip3d_isLoaded = true;
  }
  return ip3d_;
}
const int &CMS3::HLT_Ele12_CaloIdM_TrackIdM_PFJet30() {
  if (not HLT_Ele12_CaloIdM_TrackIdM_PFJet30_isLoaded) {
    if (HLT_Ele12_CaloIdM_TrackIdM_PFJet30_branch != 0) {
      HLT_Ele12_CaloIdM_TrackIdM_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele12_CaloIdM_TrackIdM_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele12_CaloIdM_TrackIdM_PFJet30_isLoaded = true;
  }
  return HLT_Ele12_CaloIdM_TrackIdM_PFJet30_;
}
const float &CMS3::coneCorrPt() {
  if (not coneCorrPt_isLoaded) {
    if (coneCorrPt_branch != 0) {
      coneCorrPt_branch->GetEntry(index);
    } else {
      printf("branch coneCorrPt_branch does not exist!\n");
      exit(1);
    }
    coneCorrPt_isLoaded = true;
  }
  return coneCorrPt_;
}
const int &CMS3::HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ() {
  if (not HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_isLoaded) {
    if (HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch != 0) {
      HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_isLoaded = true;
  }
  return HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_;
}
const int &CMS3::id() {
  if (not id_isLoaded) {
    if (id_branch != 0) {
      id_branch->GetEntry(index);
    } else {
      printf("branch id_branch does not exist!\n");
      exit(1);
    }
    id_isLoaded = true;
  }
  return id_;
}
const float &CMS3::qscale() {
  if (not qscale_isLoaded) {
    if (qscale_branch != 0) {
      qscale_branch->GetEntry(index);
    } else {
      printf("branch qscale_branch does not exist!\n");
      exit(1);
    }
    qscale_isLoaded = true;
  }
  return qscale_;
}
const int &CMS3::HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL() {
  if (not HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_isLoaded) {
    if (HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_branch != 0) {
      HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_isLoaded = true;
  }
  return HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_;
}
const int &CMS3::HLT_Ele8_CaloIdM_TrackIdM_PFJet30() {
  if (not HLT_Ele8_CaloIdM_TrackIdM_PFJet30_isLoaded) {
    if (HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch != 0) {
      HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele8_CaloIdM_TrackIdM_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele8_CaloIdM_TrackIdM_PFJet30_isLoaded = true;
  }
  return HLT_Ele8_CaloIdM_TrackIdM_PFJet30_;
}
const int &CMS3::nloose_mu() {
  if (not nloose_mu_isLoaded) {
    if (nloose_mu_branch != 0) {
      nloose_mu_branch->GetEntry(index);
    } else {
      printf("branch nloose_mu_branch does not exist!\n");
      exit(1);
    }
    nloose_mu_isLoaded = true;
  }
  return nloose_mu_;
}
const float &CMS3::mt() {
  if (not mt_isLoaded) {
    if (mt_branch != 0) {
      mt_branch->GetEntry(index);
    } else {
      printf("branch mt_branch does not exist!\n");
      exit(1);
    }
    mt_isLoaded = true;
  }
  return mt_;
}
const bool &CMS3::isTriggerSafenoIso() {
  if (not isTriggerSafenoIso_isLoaded) {
    if (isTriggerSafenoIso_branch != 0) {
      isTriggerSafenoIso_branch->GetEntry(index);
    } else {
      printf("branch isTriggerSafenoIso_branch does not exist!\n");
      exit(1);
    }
    isTriggerSafenoIso_isLoaded = true;
  }
  return isTriggerSafenoIso_;
}
const int &CMS3::HLT_Ele23_CaloIdM_TrackIdM_PFJet30() {
  if (not HLT_Ele23_CaloIdM_TrackIdM_PFJet30_isLoaded) {
    if (HLT_Ele23_CaloIdM_TrackIdM_PFJet30_branch != 0) {
      HLT_Ele23_CaloIdM_TrackIdM_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele23_CaloIdM_TrackIdM_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele23_CaloIdM_TrackIdM_PFJet30_isLoaded = true;
  }
  return HLT_Ele23_CaloIdM_TrackIdM_PFJet30_;
}
const bool &CMS3::passes_any_trigger() {
  if (not passes_any_trigger_isLoaded) {
    if (passes_any_trigger_branch != 0) {
      passes_any_trigger_branch->GetEntry(index);
    } else {
      printf("branch passes_any_trigger_branch does not exist!\n");
      exit(1);
    }
    passes_any_trigger_isLoaded = true;
  }
  return passes_any_trigger_;
}
const int &CMS3::HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL() {
  if (not HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_isLoaded) {
    if (HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_branch != 0) {
      HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_isLoaded = true;
  }
  return HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_;
}
const int &CMS3::evt_run() {
  if (not evt_run_isLoaded) {
    if (evt_run_branch != 0) {
      evt_run_branch->GetEntry(index);
    } else {
      printf("branch evt_run_branch does not exist!\n");
      exit(1);
    }
    evt_run_isLoaded = true;
  }
  return evt_run_;
}
const bool &CMS3::passes_SS_tight_noiso_v6() {
  if (not passes_SS_tight_noiso_v6_isLoaded) {
    if (passes_SS_tight_noiso_v6_branch != 0) {
      passes_SS_tight_noiso_v6_branch->GetEntry(index);
    } else {
      printf("branch passes_SS_tight_noiso_v6_branch does not exist!\n");
      exit(1);
    }
    passes_SS_tight_noiso_v6_isLoaded = true;
  }
  return passes_SS_tight_noiso_v6_;
}
const float &CMS3::ht_SS() {
  if (not ht_SS_isLoaded) {
    if (ht_SS_branch != 0) {
      ht_SS_branch->GetEntry(index);
    } else {
      printf("branch ht_SS_branch does not exist!\n");
      exit(1);
    }
    ht_SS_isLoaded = true;
  }
  return ht_SS_;
}
const float &CMS3::puweight() {
  if (not puweight_isLoaded) {
    if (puweight_branch != 0) {
      puweight_branch->GetEntry(index);
    } else {
      printf("branch puweight_branch does not exist!\n");
      exit(1);
    }
    puweight_isLoaded = true;
  }
  return puweight_;
}
const TString &CMS3::sample() {
  if (not sample_isLoaded) {
    if (sample_branch != 0) {
      sample_branch->GetEntry(index);
    } else {
      printf("branch sample_branch does not exist!\n");
      exit(1);
    }
    sample_isLoaded = true;
  }
  return sample_;
}
const int &CMS3::HLT_DoubleMu8_Mass8_PFHT300() {
  if (not HLT_DoubleMu8_Mass8_PFHT300_isLoaded) {
    if (HLT_DoubleMu8_Mass8_PFHT300_branch != 0) {
      HLT_DoubleMu8_Mass8_PFHT300_branch->GetEntry(index);
    } else {
      printf("branch HLT_DoubleMu8_Mass8_PFHT300_branch does not exist!\n");
      exit(1);
    }
    HLT_DoubleMu8_Mass8_PFHT300_isLoaded = true;
  }
  return HLT_DoubleMu8_Mass8_PFHT300_;
}
const float &CMS3::ptratio_v5() {
  if (not ptratio_v5_isLoaded) {
    if (ptratio_v5_branch != 0) {
      ptratio_v5_branch->GetEntry(index);
    } else {
      printf("branch ptratio_v5_branch does not exist!\n");
      exit(1);
    }
    ptratio_v5_isLoaded = true;
  }
  return ptratio_v5_;
}
const float &CMS3::miniiso() {
  if (not miniiso_isLoaded) {
    if (miniiso_branch != 0) {
      miniiso_branch->GetEntry(index);
    } else {
      printf("branch miniiso_branch does not exist!\n");
      exit(1);
    }
    miniiso_isLoaded = true;
  }
  return miniiso_;
}
const int &CMS3::HLT_Mu17_TrkIsoVVL() {
  if (not HLT_Mu17_TrkIsoVVL_isLoaded) {
    if (HLT_Mu17_TrkIsoVVL_branch != 0) {
      HLT_Mu17_TrkIsoVVL_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu17_TrkIsoVVL_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu17_TrkIsoVVL_isLoaded = true;
  }
  return HLT_Mu17_TrkIsoVVL_;
}
const int &CMS3::HLT_Ele18_CaloIdM_TrackIdM_PFJet30() {
  if (not HLT_Ele18_CaloIdM_TrackIdM_PFJet30_isLoaded) {
    if (HLT_Ele18_CaloIdM_TrackIdM_PFJet30_branch != 0) {
      HLT_Ele18_CaloIdM_TrackIdM_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele18_CaloIdM_TrackIdM_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele18_CaloIdM_TrackIdM_PFJet30_isLoaded = true;
  }
  return HLT_Ele18_CaloIdM_TrackIdM_PFJet30_;
}
const int &CMS3::nvtx() {
  if (not nvtx_isLoaded) {
    if (nvtx_branch != 0) {
      nvtx_branch->GetEntry(index);
    } else {
      printf("branch nvtx_branch does not exist!\n");
      exit(1);
    }
    nvtx_isLoaded = true;
  }
  return nvtx_;
}
const bool &CMS3::isTriggerSafe() {
  if (not isTriggerSafe_isLoaded) {
    if (isTriggerSafe_branch != 0) {
      isTriggerSafe_branch->GetEntry(index);
    } else {
      printf("branch isTriggerSafe_branch does not exist!\n");
      exit(1);
    }
    isTriggerSafe_isLoaded = true;
  }
  return isTriggerSafe_;
}
const int &CMS3::HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL() {
  if (not HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_isLoaded) {
    if (HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_branch != 0) {
      HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_isLoaded = true;
  }
  return HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_;
}
const int &CMS3::njets_recoil() {
  if (not njets_recoil_isLoaded) {
    if (njets_recoil_branch != 0) {
      njets_recoil_branch->GetEntry(index);
    } else {
      printf("branch njets_recoil_branch does not exist!\n");
      exit(1);
    }
    njets_recoil_isLoaded = true;
  }
  return njets_recoil_;
}
const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &CMS3::close_jet_v5() {
  if (not close_jet_v5_isLoaded) {
    if (close_jet_v5_branch != 0) {
      close_jet_v5_branch->GetEntry(index);
    } else {
      printf("branch close_jet_v5_branch does not exist!\n");
      exit(1);
    }
    close_jet_v5_isLoaded = true;
  }
  return *close_jet_v5_;
}
const int &CMS3::motherID() {
  if (not motherID_isLoaded) {
    if (motherID_branch != 0) {
      motherID_branch->GetEntry(index);
    } else {
      printf("branch motherID_branch does not exist!\n");
      exit(1);
    }
    motherID_isLoaded = true;
  }
  return motherID_;
}
const int &CMS3::HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30() {
  if (not HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded) {
    if (HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) {
      HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = true;
  }
  return HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_;
}
const float &CMS3::tag_pt() {
  if (not tag_pt_isLoaded) {
    if (tag_pt_branch != 0) {
      tag_pt_branch->GetEntry(index);
    } else {
      printf("branch tag_pt_branch does not exist!\n");
      exit(1);
    }
    tag_pt_isLoaded = true;
  }
  return tag_pt_;
}
const bool &CMS3::passes_SS_fo_looseMVA_v6() {
  if (not passes_SS_fo_looseMVA_v6_isLoaded) {
    if (passes_SS_fo_looseMVA_v6_branch != 0) {
      passes_SS_fo_looseMVA_v6_branch->GetEntry(index);
    } else {
      printf("branch passes_SS_fo_looseMVA_v6_branch does not exist!\n");
      exit(1);
    }
    passes_SS_fo_looseMVA_v6_isLoaded = true;
  }
  return passes_SS_fo_looseMVA_v6_;
}
const bool &CMS3::passes_SS_tight_v6() {
  if (not passes_SS_tight_v6_isLoaded) {
    if (passes_SS_tight_v6_branch != 0) {
      passes_SS_tight_v6_branch->GetEntry(index);
    } else {
      printf("branch passes_SS_tight_v6_branch does not exist!\n");
      exit(1);
    }
    passes_SS_tight_v6_isLoaded = true;
  }
  return passes_SS_tight_v6_;
}
const bool &CMS3::have_tag() {
  if (not have_tag_isLoaded) {
    if (have_tag_branch != 0) {
      have_tag_branch->GetEntry(index);
    } else {
      printf("branch have_tag_branch does not exist!\n");
      exit(1);
    }
    have_tag_isLoaded = true;
  }
  return have_tag_;
}
const int &CMS3::nFOs_SS() {
  if (not nFOs_SS_isLoaded) {
    if (nFOs_SS_branch != 0) {
      nFOs_SS_branch->GetEntry(index);
    } else {
      printf("branch nFOs_SS_branch does not exist!\n");
      exit(1);
    }
    nFOs_SS_isLoaded = true;
  }
  return nFOs_SS_;
}
const int &CMS3::HLT_IsoMu27() {
  if (not HLT_IsoMu27_isLoaded) {
    if (HLT_IsoMu27_branch != 0) {
      HLT_IsoMu27_branch->GetEntry(index);
    } else {
      printf("branch HLT_IsoMu27_branch does not exist!\n");
      exit(1);
    }
    HLT_IsoMu27_isLoaded = true;
  }
  return HLT_IsoMu27_;
}
const int &CMS3::ntight_el() {
  if (not ntight_el_isLoaded) {
    if (ntight_el_branch != 0) {
      ntight_el_branch->GetEntry(index);
    } else {
      printf("branch ntight_el_branch does not exist!\n");
      exit(1);
    }
    ntight_el_isLoaded = true;
  }
  return ntight_el_;
}
const int &CMS3::HLT_Ele17_CaloIdM_TrackIdM_PFJet30() {
  if (not HLT_Ele17_CaloIdM_TrackIdM_PFJet30_isLoaded) {
    if (HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch != 0) {
      HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele17_CaloIdM_TrackIdM_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele17_CaloIdM_TrackIdM_PFJet30_isLoaded = true;
  }
  return HLT_Ele17_CaloIdM_TrackIdM_PFJet30_;
}
const float &CMS3::ip3derr() {
  if (not ip3derr_isLoaded) {
    if (ip3derr_branch != 0) {
      ip3derr_branch->GetEntry(index);
    } else {
      printf("branch ip3derr_branch does not exist!\n");
      exit(1);
    }
    ip3derr_isLoaded = true;
  }
  return ip3derr_;
}
const float &CMS3::ptrelv1() {
  if (not ptrelv1_isLoaded) {
    if (ptrelv1_branch != 0) {
      ptrelv1_branch->GetEntry(index);
    } else {
      printf("branch ptrelv1_branch does not exist!\n");
      exit(1);
    }
    ptrelv1_isLoaded = true;
  }
  return ptrelv1_;
}
const int &CMS3::HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300() {
  if (not HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_isLoaded) {
    if (HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_branch != 0) {
      HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_isLoaded = true;
  }
  return HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_;
}
const int &CMS3::HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL() {
  if (not HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_isLoaded) {
    if (HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_branch != 0) {
      HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_isLoaded = true;
  }
  return HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_;
}
const int &CMS3::evt_event() {
  if (not evt_event_isLoaded) {
    if (evt_event_branch != 0) {
      evt_event_branch->GetEntry(index);
    } else {
      printf("branch evt_event_branch does not exist!\n");
      exit(1);
    }
    evt_event_isLoaded = true;
  }
  return evt_event_;
}
const int &CMS3::HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ() {
  if (not HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_isLoaded) {
    if (HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch != 0) {
      HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_isLoaded = true;
  }
  return HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_;
}
const bool &CMS3::passes_met_filters() {
  if (not passes_met_filters_isLoaded) {
    if (passes_met_filters_branch != 0) {
      passes_met_filters_branch->GetEntry(index);
    } else {
      printf("branch passes_met_filters_branch does not exist!\n");
      exit(1);
    }
    passes_met_filters_isLoaded = true;
  }
  return passes_met_filters_;
}
const float &CMS3::dilep_mass() {
  if (not dilep_mass_isLoaded) {
    if (dilep_mass_branch != 0) {
      dilep_mass_branch->GetEntry(index);
    } else {
      printf("branch dilep_mass_branch does not exist!\n");
      exit(1);
    }
    dilep_mass_isLoaded = true;
  }
  return dilep_mass_;
}
const int &CMS3::nloose_el() {
  if (not nloose_el_isLoaded) {
    if (nloose_el_branch != 0) {
      nloose_el_branch->GetEntry(index);
    } else {
      printf("branch nloose_el_branch does not exist!\n");
      exit(1);
    }
    nloose_el_isLoaded = true;
  }
  return nloose_el_;
}
const int &CMS3::HLT_Ele12_CaloIdL_TrackIdL_IsoVL() {
  if (not HLT_Ele12_CaloIdL_TrackIdL_IsoVL_isLoaded) {
    if (HLT_Ele12_CaloIdL_TrackIdL_IsoVL_branch != 0) {
      HLT_Ele12_CaloIdL_TrackIdL_IsoVL_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele12_CaloIdL_TrackIdL_IsoVL_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele12_CaloIdL_TrackIdL_IsoVL_isLoaded = true;
  }
  return HLT_Ele12_CaloIdL_TrackIdL_IsoVL_;
}
const int &CMS3::HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30() {
  if (not HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded) {
    if (HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) {
      HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = true;
  }
  return HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_;
}
const int &CMS3::HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300() {
  if (not HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_isLoaded) {
    if (HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_branch != 0) {
      HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_branch->GetEntry(index);
    } else {
      printf("branch HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_branch does not exist!\n");
      exit(1);
    }
    HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_isLoaded = true;
  }
  return HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_;
}
const int &CMS3::idx() {
  if (not idx_isLoaded) {
    if (idx_branch != 0) {
      idx_branch->GetEntry(index);
    } else {
      printf("branch idx_branch does not exist!\n");
      exit(1);
    }
    idx_isLoaded = true;
  }
  return idx_;
}
const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &CMS3::p4() {
  if (not p4_isLoaded) {
    if (p4_branch != 0) {
      p4_branch->GetEntry(index);
    } else {
      printf("branch p4_branch does not exist!\n");
      exit(1);
    }
    p4_isLoaded = true;
  }
  return *p4_;
}
const float &CMS3::scale1fb() {
  if (not scale1fb_isLoaded) {
    if (scale1fb_branch != 0) {
      scale1fb_branch->GetEntry(index);
    } else {
      printf("branch scale1fb_branch does not exist!\n");
      exit(1);
    }
    scale1fb_isLoaded = true;
  }
  return scale1fb_;
}
const int &CMS3::HLT_Mu8() {
  if (not HLT_Mu8_isLoaded) {
    if (HLT_Mu8_branch != 0) {
      HLT_Mu8_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu8_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu8_isLoaded = true;
  }
  return HLT_Mu8_;
}
const float &CMS3::p4_eta() {
  if (not p4_eta_isLoaded) {
    if (p4_eta_branch != 0) {
      p4_eta_branch->GetEntry(index);
    } else {
      printf("branch p4_eta_branch does not exist!\n");
      exit(1);
    }
    p4_eta_isLoaded = true;
  }
  return p4_eta_;
}
const float &CMS3::evt_corrMETPhi() {
  if (not evt_corrMETPhi_isLoaded) {
    if (evt_corrMETPhi_branch != 0) {
      evt_corrMETPhi_branch->GetEntry(index);
    } else {
      printf("branch evt_corrMETPhi_branch does not exist!\n");
      exit(1);
    }
    evt_corrMETPhi_isLoaded = true;
  }
  return evt_corrMETPhi_;
}
const int &CMS3::ntight_mu() {
  if (not ntight_mu_isLoaded) {
    if (ntight_mu_branch != 0) {
      ntight_mu_branch->GetEntry(index);
    } else {
      printf("branch ntight_mu_branch does not exist!\n");
      exit(1);
    }
    ntight_mu_isLoaded = true;
  }
  return ntight_mu_;
}
const int &CMS3::HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL() {
  if (not HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_isLoaded) {
    if (HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_branch != 0) {
      HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_isLoaded = true;
  }
  return HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_;
}
const float &CMS3::mva() {
  if (not mva_isLoaded) {
    if (mva_branch != 0) {
      mva_branch->GetEntry(index);
    } else {
      printf("branch mva_branch does not exist!\n");
      exit(1);
    }
    mva_isLoaded = true;
  }
  return mva_;
}
const int &CMS3::HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30() {
  if (not HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded) {
    if (HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) {
      HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = true;
  }
  return HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_;
}
const int &CMS3::HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ() {
  if (not HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_isLoaded) {
    if (HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_branch != 0) {
      HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_isLoaded = true;
  }
  return HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_;
}
const int &CMS3::evt_lumiBlock() {
  if (not evt_lumiBlock_isLoaded) {
    if (evt_lumiBlock_branch != 0) {
      evt_lumiBlock_branch->GetEntry(index);
    } else {
      printf("branch evt_lumiBlock_branch does not exist!\n");
      exit(1);
    }
    evt_lumiBlock_isLoaded = true;
  }
  return evt_lumiBlock_;
}
const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &CMS3::dilep_p4() {
  if (not dilep_p4_isLoaded) {
    if (dilep_p4_branch != 0) {
      dilep_p4_branch->GetEntry(index);
    } else {
      printf("branch dilep_p4_branch does not exist!\n");
      exit(1);
    }
    dilep_p4_isLoaded = true;
  }
  return *dilep_p4_;
}
const float &CMS3::trueNumInt() {
  if (not trueNumInt_isLoaded) {
    if (trueNumInt_branch != 0) {
      trueNumInt_branch->GetEntry(index);
    } else {
      printf("branch trueNumInt_branch does not exist!\n");
      exit(1);
    }
    trueNumInt_isLoaded = true;
  }
  return trueNumInt_;
}
const int &CMS3::HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL() {
  if (not HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_isLoaded) {
    if (HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_branch != 0) {
      HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_isLoaded = true;
  }
  return HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_;
}
const int &CMS3::HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ() {
  if (not HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_isLoaded) {
    if (HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_branch != 0) {
      HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_isLoaded = true;
  }
  return HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_;
}
const int &CMS3::HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30() {
  if (not HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded) {
    if (HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_branch != 0) {
      HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_isLoaded = true;
  }
  return HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_;
}
const int &CMS3::HLT_Ele23_CaloIdL_TrackIdL_IsoVL() {
  if (not HLT_Ele23_CaloIdL_TrackIdL_IsoVL_isLoaded) {
    if (HLT_Ele23_CaloIdL_TrackIdL_IsoVL_branch != 0) {
      HLT_Ele23_CaloIdL_TrackIdL_IsoVL_branch->GetEntry(index);
    } else {
      printf("branch HLT_Ele23_CaloIdL_TrackIdL_IsoVL_branch does not exist!\n");
      exit(1);
    }
    HLT_Ele23_CaloIdL_TrackIdL_IsoVL_isLoaded = true;
  }
  return HLT_Ele23_CaloIdL_TrackIdL_IsoVL_;
}
const int &CMS3::HLT_Mu17() {
  if (not HLT_Mu17_isLoaded) {
    if (HLT_Mu17_branch != 0) {
      HLT_Mu17_branch->GetEntry(index);
    } else {
      printf("branch HLT_Mu17_branch does not exist!\n");
      exit(1);
    }
    HLT_Mu17_isLoaded = true;
  }
  return HLT_Mu17_;
}
void CMS3::progress( int nEventsTotal, int nEventsChain ){
  int period = 1000;
  if(nEventsTotal%1000 == 0) {
    if (isatty(1)) {
      if( ( nEventsChain - nEventsTotal ) > period ){
        float frac = (float)nEventsTotal/(nEventsChain*0.01);
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
               "\033[0m\033[32m <---\033[0m\015", frac);
        fflush(stdout);
      }
      else {
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
               "\033[0m\033[32m <---\033[0m\015", 100.);
        cout << endl;
      }
    }
  }
}
namespace tas {
  const bool &passes_SS_fo_looseMVA_noiso_v6() { return cms3.passes_SS_fo_looseMVA_noiso_v6(); }
  const bool &passes_SS_fo_noiso_v6() { return cms3.passes_SS_fo_noiso_v6(); }
  const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &tag_p4() { return cms3.tag_p4(); }
  const int &njets() { return cms3.njets(); }
  const float &p4_pt() { return cms3.p4_pt(); }
  const int &HLT_Mu8_TrkIsoVVL() { return cms3.HLT_Mu8_TrkIsoVVL(); }
  const float &evt_corrMET() { return cms3.evt_corrMET(); }
  const bool &passes_SS_fo_v6() { return cms3.passes_SS_fo_v6(); }
  const float &etaSC() { return cms3.etaSC(); }
  const bool &evt_isRealData() { return cms3.evt_isRealData(); }
  const int &HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30() { return cms3.HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30(); }
  const float &ip3d() { return cms3.ip3d(); }
  const int &HLT_Ele12_CaloIdM_TrackIdM_PFJet30() { return cms3.HLT_Ele12_CaloIdM_TrackIdM_PFJet30(); }
  const float &coneCorrPt() { return cms3.coneCorrPt(); }
  const int &HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ() { return cms3.HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ(); }
  const int &id() { return cms3.id(); }
  const float &qscale() { return cms3.qscale(); }
  const int &HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL() { return cms3.HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL(); }
  const int &HLT_Ele8_CaloIdM_TrackIdM_PFJet30() { return cms3.HLT_Ele8_CaloIdM_TrackIdM_PFJet30(); }
  const int &nloose_mu() { return cms3.nloose_mu(); }
  const float &mt() { return cms3.mt(); }
  const bool &isTriggerSafenoIso() { return cms3.isTriggerSafenoIso(); }
  const int &HLT_Ele23_CaloIdM_TrackIdM_PFJet30() { return cms3.HLT_Ele23_CaloIdM_TrackIdM_PFJet30(); }
  const bool &passes_any_trigger() { return cms3.passes_any_trigger(); }
  const int &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL() { return cms3.HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL(); }
  const int &evt_run() { return cms3.evt_run(); }
  const bool &passes_SS_tight_noiso_v6() { return cms3.passes_SS_tight_noiso_v6(); }
  const float &ht_SS() { return cms3.ht_SS(); }
  const float &puweight() { return cms3.puweight(); }
  const TString &sample() { return cms3.sample(); }
  const int &HLT_DoubleMu8_Mass8_PFHT300() { return cms3.HLT_DoubleMu8_Mass8_PFHT300(); }
  const float &ptratio_v5() { return cms3.ptratio_v5(); }
  const float &miniiso() { return cms3.miniiso(); }
  const int &HLT_Mu17_TrkIsoVVL() { return cms3.HLT_Mu17_TrkIsoVVL(); }
  const int &HLT_Ele18_CaloIdM_TrackIdM_PFJet30() { return cms3.HLT_Ele18_CaloIdM_TrackIdM_PFJet30(); }
  const int &nvtx() { return cms3.nvtx(); }
  const bool &isTriggerSafe() { return cms3.isTriggerSafe(); }
  const int &HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL() { return cms3.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL(); }
  const int &njets_recoil() { return cms3.njets_recoil(); }
  const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &close_jet_v5() { return cms3.close_jet_v5(); }
  const int &motherID() { return cms3.motherID(); }
  const int &HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30() { return cms3.HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30(); }
  const float &tag_pt() { return cms3.tag_pt(); }
  const bool &passes_SS_fo_looseMVA_v6() { return cms3.passes_SS_fo_looseMVA_v6(); }
  const bool &passes_SS_tight_v6() { return cms3.passes_SS_tight_v6(); }
  const bool &have_tag() { return cms3.have_tag(); }
  const int &nFOs_SS() { return cms3.nFOs_SS(); }
  const int &HLT_IsoMu27() { return cms3.HLT_IsoMu27(); }
  const int &ntight_el() { return cms3.ntight_el(); }
  const int &HLT_Ele17_CaloIdM_TrackIdM_PFJet30() { return cms3.HLT_Ele17_CaloIdM_TrackIdM_PFJet30(); }
  const float &ip3derr() { return cms3.ip3derr(); }
  const float &ptrelv1() { return cms3.ptrelv1(); }
  const int &HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300() { return cms3.HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300(); }
  const int &HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL() { return cms3.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL(); }
  const int &evt_event() { return cms3.evt_event(); }
  const int &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ() { return cms3.HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ(); }
  const bool &passes_met_filters() { return cms3.passes_met_filters(); }
  const float &dilep_mass() { return cms3.dilep_mass(); }
  const int &nloose_el() { return cms3.nloose_el(); }
  const int &HLT_Ele12_CaloIdL_TrackIdL_IsoVL() { return cms3.HLT_Ele12_CaloIdL_TrackIdL_IsoVL(); }
  const int &HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30() { return cms3.HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30(); }
  const int &HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300() { return cms3.HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300(); }
  const int &idx() { return cms3.idx(); }
  const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &p4() { return cms3.p4(); }
  const float &scale1fb() { return cms3.scale1fb(); }
  const int &HLT_Mu8() { return cms3.HLT_Mu8(); }
  const float &p4_eta() { return cms3.p4_eta(); }
  const float &evt_corrMETPhi() { return cms3.evt_corrMETPhi(); }
  const int &ntight_mu() { return cms3.ntight_mu(); }
  const int &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL() { return cms3.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL(); }
  const float &mva() { return cms3.mva(); }
  const int &HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30() { return cms3.HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30(); }
  const int &HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ() { return cms3.HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ(); }
  const int &evt_lumiBlock() { return cms3.evt_lumiBlock(); }
  const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > &dilep_p4() { return cms3.dilep_p4(); }
  const float &trueNumInt() { return cms3.trueNumInt(); }
  const int &HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL() { return cms3.HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL(); }
  const int &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ() { return cms3.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ(); }
  const int &HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30() { return cms3.HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30(); }
  const int &HLT_Ele23_CaloIdL_TrackIdL_IsoVL() { return cms3.HLT_Ele23_CaloIdL_TrackIdL_IsoVL(); }
  const int &HLT_Mu17() { return cms3.HLT_Mu17(); }
}
