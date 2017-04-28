#include "ElectronSelections.h"
#include "Math/LorentzVector.h"
#include "Math/VectorUtil.h"
#include "IsolationTools.h"

//Development Notes
  //Original Author: Alex (UCSB), who stole functions from Indara, Jason, Giuseppe
  //POG Electron IDs are defined in: 
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification#Electron_ID_Working_Points

readMVA* globalEleMVAreader = 0;

bool elIDCacheSet = false;
elIDcache elID_cache;
void elID::setCache(int idx, float mva, float miniiso, float ptratio, float ptrel) {
  assert(elIDCacheSet==false);//you must unset it before setting it again
  elID_cache.setCacheValues(idx,mva,miniiso,ptratio,ptrel);
  elIDCacheSet=true;
}
void elID::unsetCache() {
  elIDCacheSet=false;
}

using namespace tas;

bool isVetoElectronPOG(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){
    if (fabs(els_dEtaIn().at(elIdx)) >= 0.007) return false; 
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.8) return false; 
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.01) return false; 
    if (els_hOverE().at(elIdx) >= 0.15) return false; 
    if (fabs(els_dxyPV().at(elIdx)) >= 0.04) return false;
    if (fabs(els_dzPV().at(elIdx)) >= 0.2) return false; 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
    if (fabs(els_dEtaIn().at(elIdx)) >= 0.01) return false; 
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.7) return false; 
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.03) return false; 
    if (fabs(els_dxyPV().at(elIdx)) >= 0.04) return false;
    if (fabs(els_dzPV().at(elIdx)) >= 0.2) return false; 
  }
  else return false;

  return true;
}

bool isLooseElectronPOG(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.01) return false;
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
    if (els_hOverE().at(elIdx) >= 0.12) return false;
    if (els_conv_vtx_flag().at(elIdx)) return false;
    if (fabs(els_dEtaIn().at(elIdx)) >= 0.007) return false;
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.15) return false;
    if (fabs(els_dxyPV().at(elIdx)) >= 0.02) return false;
    if (fabs(els_dzPV().at(elIdx)) >= 0.2) return false; 
    if (els_exp_innerlayers().at(elIdx) > 1) return false;
  }
  else if((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.03) return false;
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
    if (els_hOverE().at(elIdx) >= 0.1) return false;
    if (els_conv_vtx_flag().at(elIdx) ) return false;
    if (fabs(els_dEtaIn().at(elIdx)) >= 0.009) return false;
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.10) return false;
    if (fabs(els_dxyPV().at(elIdx)) >= 0.02) return false; 
    if (fabs(els_dzPV().at(elIdx)) >= 0.2) return false; 
    if (els_exp_innerlayers().at(elIdx) > 1) return false;
  }
  else return false;
  return true;
}

bool isMediumElectronPOG(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){
    if (fabs(els_dEtaIn().at(elIdx)) >= 0.004) return false;
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.06) return false; 
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.01) return false; 
    if (els_hOverE().at(elIdx) >= 0.12) return false; 
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
    if (els_conv_vtx_flag().at(elIdx)) return false;
    if (els_exp_innerlayers().at(elIdx) > 1) return false;
    if (fabs(els_dxyPV().at(elIdx)) >= 0.02) return false; 
    if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
  }
  else if((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
    if (els_conv_vtx_flag().at(elIdx)) return false;
    if (els_exp_innerlayers().at(elIdx) > 0) return false;
    if (fabs(els_dxyPV().at(elIdx)) >= 0.02) return false; 
    if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
    if (fabs(els_dEtaIn().at(elIdx)) >= 0.007) return false;
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.03) return false; 
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.03) return false; 
    if (els_hOverE().at(elIdx) >= 0.1) return false; 
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
  }
  else return false;
  return true;
}

bool isTightElectronPOG(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){
    if (fabs(els_dEtaIn().at(elIdx)) >= 0.004) return false;
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.03) return false; 
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.01) return false; 
    if (els_hOverE().at(elIdx) >= 0.12) return false; 
    if (fabs((1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
    if (fabs(els_dxyPV().at(elIdx)) >= 0.02) return false; 
    if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
    if (els_conv_vtx_flag().at(elIdx)) return false;
    if (els_exp_innerlayers().at(elIdx) > 0) return false;
  }
  else if((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
    if (fabs(els_dEtaIn().at(elIdx)) >= 0.005) return false;
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.02) return false; 
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.03) return false; 
    if (els_hOverE().at(elIdx) >= 0.1) return false; 
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
    if (fabs(els_dxyPV().at(elIdx)) >= 0.02) return false;
    if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
    if (els_conv_vtx_flag().at(elIdx)) return false;
    if (els_exp_innerlayers().at(elIdx) > 0) return false;
  }
  else return false;
  return true;
}

bool electronID(unsigned int elIdx, id_level_t id_level){

  analysis_t analysis = whichAnalysis(id_level);

  switch (id_level){

   //////////////////
   // SS veto v1 ////
   //////////////////

    case(SS_veto_noiso_v1):
      if (fabs(els_etaSC().at(elIdx)) <= 1.479){
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.007) return false; 
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.8) return false; 
        if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.01) return false; 
        if (els_hOverE().at(elIdx) >= 0.15) return false; 
      }
      else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.01) return false; 
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.7) return false; 
        if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.03) return false; 
      }
      else return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 1) return false;
      if (fabs(els_dxyPV().at(elIdx)) >= 0.05) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
      break;

    case(SS_veto_v1):
      if (electronID(elIdx, SS_veto_noiso_v1)==0) return false; 
      if (eleRelIso03(elIdx, analysis) >= 0.50) return false; 
      return true;
      break;

   ////////////////////
   /// SS veto v2 ///
   ////////////////////

    case(SS_veto_noiso_v2):
      //trigger match cuts
      if (fabs(els_etaSC().at(elIdx)) <= 1.479){
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.01) return false; 
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.15) return false; 
        if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
        if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.011) return false; 
        if (els_hOverE().at(elIdx) >= 0.12) return false; 
      }
      else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.01) return false; 
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.1) return false; 
        if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
        if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.031) return false; 
        if (els_hOverE().at(elIdx) >= 0.1) return false; 
      }
      else return false;
      if (fabs(els_etaSC().at(elIdx)) > 2.4) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 1) return false;
      if (fabs(els_dxyPV().at(elIdx)) >= 0.05) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
      if (globalEleMVAreader==0) {
	cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	return false;
      }
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

    case(SS_veto_v2):
      if (electronID(elIdx, SS_veto_noiso_v2)==0) return false; 
      if (eleRelIso03(elIdx, analysis) >= 0.50) return false; 
      return true;
      break;

   ////////////////////
   /// SS veto v3 ///
   ////////////////////

    case(SS_veto_noiso_v3):
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 1) return false;
      if (fabs(els_dxyPV().at(elIdx)) >= 0.05) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

    case(SS_veto_v3):
      if (electronID(elIdx, SS_veto_noiso_v3)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx) >= 0.40) return false;
      return true;
      break;

    case(SS_veto_noiso_noip_v3):
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 1) return false;
      //if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

   //////////////////
   /// SS veto v4 ///
   //////////////////

    case(SS_veto_noiso_v4):
      //trigger match cuts
      if (!isTriggerSafenoIso_v1(elIdx)) return false;
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 1) return false;
      if (fabs(els_dxyPV().at(elIdx)) >= 0.05) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

    case(SS_veto_v4):
      if (electronID(elIdx, SS_veto_noiso_v4)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx,1) >= 0.40) return false;
      return true;
      break;

    case(SS_veto_noiso_noip_v4):
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 1) return false;
      //if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

   //////////////////
   /// SS veto v5 ///
   //////////////////

    case(SS_veto_noiso_v5):
      //trigger match cuts
      if (!isTriggerSafenoIso_v1(elIdx)) return false;
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 1) return false;
      if (fabs(els_dxyPV().at(elIdx)) >= 0.05) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

    case(SS_veto_v5):
      if (electronID(elIdx, SS_veto_noiso_v5)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx,1) >= 0.40) return false;
      return true;
      break;

    case(SS_veto_noiso_noip_v5):
      //trigger match cuts
      if (!isTriggerSafenoIso_v1(elIdx)) return false;
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 1) return false;
      //if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

   ////////////////////
   /// WW veto v1   ///
   ////////////////////

    case(WW_veto_noiso_v1):

      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 1) return false;
      if (fabs(els_dxyPV().at(elIdx)) >= 0.05) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

    case(WW_veto_v1):
      if (electronID(elIdx, WW_veto_noiso_v1)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx) >= 0.40) return false;
      return true;
      break;

    case(WW_veto_noiso_noip_v1):
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 1) return false;
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

   ////////////////////
   /// WW veto v2   ///
   ////////////////////

    case(WW_veto_noiso_v2):

      if (!isVetoElectronPOGspring15noIso_v1(elIdx)) return false;
      return true;
      break;

    case(WW_veto_v2):

      if (!isVetoElectronPOGspring15_v1(elIdx)) return false;
      return true;
      break;
      
   ////////////////////
   /// HAD veto v1 ////
   ////////////////////

      // same as POG veto, except using sigmaIEtaIEta (not _full5x5)
    case(HAD_veto_noiso_v1):
      if (fabs(els_etaSC().at(elIdx)) <= 1.479){
	if (fabs(els_dEtaIn().at(elIdx)) >= 0.007) return false; 
	if (fabs(els_dPhiIn().at(elIdx)) >= 0.8) return false; 
	if (els_sigmaIEtaIEta().at(elIdx) >= 0.01) return false; 
	if (els_hOverE().at(elIdx) >= 0.15) return false; 
	if (fabs(els_dxyPV().at(elIdx)) >= 0.04) return false;
	if (fabs(els_dzPV().at(elIdx)) >= 0.2) return false; 
      }
      else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
	if (fabs(els_dEtaIn().at(elIdx)) >= 0.01) return false; 
	if (fabs(els_dPhiIn().at(elIdx)) >= 0.7) return false; 
	if (els_sigmaIEtaIEta().at(elIdx) >= 0.03) return false; 
	if (fabs(els_dxyPV().at(elIdx)) >= 0.04) return false;
	if (fabs(els_dzPV().at(elIdx)) >= 0.2) return false; 
      }
      return true;
      break;

    case(HAD_veto_v1):
      if (electronID(elIdx, HAD_veto_noiso_v1)==0) return false;
      if (eleRelIso03(elIdx, analysis) >= 0.15) return false; 
      return true;
      break;

   ////////////////////
   /// HAD veto v2 ////
   ////////////////////

    // same as POG phys14 veto
    case(HAD_veto_noiso_v2):
    case(HAD_veto_noiso_v3):
      if (!isVetoElectronPOGphys14noIso(elIdx)) return false;
      return true;
      break;

    // now using mini iso
    case(HAD_veto_v2):
      if (electronID(elIdx, HAD_veto_noiso_v2)==0) return false;
      if (elMiniRelIso(elIdx) > 0.1) return false; 
      return true;
      break;

   ////////////////////
   /// HAD veto v3 ////
   ////////////////////

    // same ID as v2, but use CMS3 miniIso with EA corrections
    case(HAD_veto_v3):
      if (electronID(elIdx, HAD_veto_noiso_v3)==0) return false;
      if (elMiniRelIsoCMS3_EA(elIdx) > 0.1) return false; 
      return true;
      break;


   ////////////////////
   /// HAD veto v4 ////
   ////////////////////

    // use Spring15 POG veto ID
    case(HAD_veto_noiso_v4):
      if (!isVetoElectronPOGspring15noIso_v1(elIdx)) return false;
      return true;
      break;

    // updated EA values
    case(HAD_veto_v4):
      if (electronID(elIdx, HAD_veto_noiso_v4)==0) return false;
      if (elMiniRelIsoCMS3_EA(elIdx,1) > 0.1) return false; 
      return true;
      break;

//   ////////////////////
//   /// HAD veto v5 (80X cut-based ID: never actually adopted)////
//   ////////////////////
//      
//      // use Spring15 POG veto ID
//    case(HAD_veto_noiso_v5):
//      if (!isVetoElectronPOGspring16noIso_v1(elIdx)) return false;
//      if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.2) return false;// abs(d0)       //LooseIP2D                          
//      if (fabs(els_dzPV()             .at(elIdx))                       >= 0.5) return false;// abs(dz)  
//      return true;
//      break;
//      
//      // updated EA values
//    case(HAD_veto_v5):
//      if (electronID(elIdx, HAD_veto_noiso_v5)==0) return false;
//      if (elMiniRelIsoCMS3_EA(elIdx,2) > 0.1) return false; 
//      return true;
//      break;




   /////////////////////
   /// STOP veto v1 ////
   /////////////////////

    case(STOP_veto_v1):
      if (!isVetoElectronPOG(elIdx)) return false;
      return true;
      break;

    case(STOP_veto_v2):
      if (!isVetoElectronPOGphys14noIso_v2(elIdx)) return false;
      //if ( elMiniRelIso(elIdx, true, 0.0, true, false) > 0.2) return false;
      if (  elMiniRelIsoCMS3_DB(elIdx)      > 0.2) return false;
      return true;
      break;

    case(STOP_veto_v3):
      if (!isVetoElectronPOGspring15noIso_v1(elIdx)) return false;
      //if ( elMiniRelIso(elIdx, true, 0.0, true, false) > 0.2) return false;
      if (  elMiniRelIsoCMS3_EA(elIdx,1)      > 0.2) return false;
      return true;
      break;

   /////////////////////
   /// STOP loose    ///
   /////////////////////

    case(STOP_loose_v1):
      if (!isLooseElectronPOG(elIdx)) return false;
      if (eleRelIso03(elIdx, STOP) >= 0.15) return false; 
      return true;
      break;

    case(STOP_loose_v2):
      if (!isLooseElectronPOGphys14noIso_v2(elIdx)) return false;
      //if ( elMiniRelIso(elIdx, true, 0.0, true, false) >= 0.1) return false;
      if (  elMiniRelIsoCMS3_DB(elIdx)       > 0.2) return false;
      return true;
      break;

    case(STOP_loose_v3):
      if (!isLooseElectronPOGspring15noIso_v1(elIdx)) return false;
      if (  elMiniRelIsoCMS3_EA(elIdx,1)      > 0.2) return false;
      return true;
      break;
   ///////////////////
   /// SS FO v1 /// same as medium, but no SIP3D cut and looser iso
   ///////////////////

    case(SS_fo_noiso_v1):
      if (fabs(els_etaSC().at(elIdx)) <= 1.479){
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.004) return false;
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.06) return false; 
        if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.01) return false; 
        if (els_hOverE().at(elIdx) >= 0.12) return false; 
        if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
      }
      else if((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.007) return false;
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.03) return false; 
        if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.03) return false; 
        if (els_hOverE().at(elIdx) >= 0.1) return false; 
        if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
      }
      else return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      break;

    case(SS_fo_v1):
      if (electronID(elIdx, SS_fo_noiso_v1)==0) return false; 
      if (eleRelIso03(elIdx, analysis) >= 0.50) return false; 
      return true;
      break;

   ///////////////////
   /// SS FO v2 /// same as medium, but no SIP3D cut and looser iso
   ///////////////////

    case(SS_fo_noiso_v2):
      if (electronID(elIdx, SS_veto_noiso_v2)==0) return false;//make sure it's tighter than veto
      if (globalEleMVAreader==0) {
	cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.4) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      //if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);

    case(SS_fo_v2):
      if (electronID(elIdx, SS_fo_noiso_v2)==0) return false; 
      if (eleRelIso03(elIdx, analysis) >= 0.50) return false; 
      return true;
      break;

   ///////////////////
   /// SS FO v3 /// same as medium, but looser iso and option for looser MVA cut
   ///////////////////

    case(SS_fo_noiso_v3):
    case(SS_fo_looseMVA_noiso_v3):
      if (electronID(elIdx, SS_veto_noiso_v3)==0) return false;//make sure it's tighter than veto
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dxyPV().at(elIdx)) > 0.05) return false;
      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);

    case(SS_fo_looseMVA_noiso_noip_v3):
      if (electronID(elIdx, SS_veto_noiso_noip_v3)==0) return false;//make sure it's tighter than veto
      if (globalEleMVAreader==0){
        cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
        return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      //if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

    case(SS_fo_v3):
      if (electronID(elIdx, SS_fo_noiso_v3)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx) >= 0.40) return false;
      return true;
      break;

    case(SS_fo_looseMVA_v3):
      if (electronID(elIdx, SS_fo_looseMVA_noiso_v3)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx) >= 0.40) return false;
      return true;
      break;

   ///////////////////
   /// SS FO v4 /// same as medium, but looser iso and option for looser MVA cut
   ///////////////////

    case(SS_fo_noiso_v4):
    case(SS_fo_looseMVA_noiso_v4):
      if (electronID(elIdx, SS_veto_noiso_v4)==0) return false;//make sure it's tighter than veto
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dxyPV().at(elIdx)) > 0.05) return false;
      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);

    case(SS_fo_looseMVA_noiso_noip_v4):
      if (electronID(elIdx, SS_veto_noiso_noip_v4)==0) return false;//make sure it's tighter than veto
      if (globalEleMVAreader==0){
        cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
        return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      //if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

    case(SS_fo_v4):
      if (electronID(elIdx, SS_fo_noiso_v4)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx,1) >= 0.40) return false;
      return true;
      break;

    case(SS_fo_looseMVA_v4):
      if (electronID(elIdx, SS_fo_looseMVA_noiso_v4)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx,1) >= 0.40) return false;
      return true;
      break;

   ///////////////////
   /// SS FO v5 /// same as medium, but looser iso and option for looser MVA cut
   ///////////////////

    case(SS_fo_noiso_v5):
    case(SS_fo_looseMVA_noiso_v5):
      if (electronID(elIdx, SS_veto_noiso_v5)==0) return false;//make sure it's tighter than veto
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dxyPV().at(elIdx)) > 0.05) return false;
      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

    case(SS_fo_noiso_no3chg_v5):
    case(SS_fo_looseMVA_noiso_noip_v5):
      if (electronID(elIdx, SS_veto_noiso_noip_v5)==0) return false;//make sure it's tighter than veto
      if (globalEleMVAreader==0){
        cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
        return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

    case(SS_fo_looseMVA_noiso_noip_no3chg_v5):
      if (electronID(elIdx, SS_veto_noiso_noip_v5)==0) return false;//make sure it's tighter than veto
      if (globalEleMVAreader==0){
        cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
        return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

    case(SS_fo_v5):
      if (electronID(elIdx, SS_fo_noiso_v5)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx,1) >= 0.40) return false;
      return true;
      break;

    case(SS_fo_looseMVA_v5):
      if (electronID(elIdx, SS_fo_looseMVA_noiso_v5)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx,1) >= 0.40) return false;
      return true;
      break;

    case(SS_fo_looseMVA_no3chg_v5):
      if (electronID(elIdx, SS_fo_looseMVA_noiso_no3chg_v5)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx,1) >= 0.40) return false;
      return true;
      break;

   ///////////////////
   /// WW FO v1    /// same as medium, but looser iso and option for looser MVA cut
   ///////////////////

    case(WW_fo_noiso_v1):
    case(WW_fo_looseMVA_noiso_v1):
      if (electronID(elIdx, WW_veto_noiso_v1)==0) return false;//make sure it's tighter than veto
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);

    case(WW_fo_looseMVA_noiso_noip_v1):
      if (electronID(elIdx, WW_veto_noiso_noip_v1)==0) return false;//make sure it's tighter than veto
      if (globalEleMVAreader==0){
        cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
        return false;
      }
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

    case(WW_fo_v1):
      if (electronID(elIdx, WW_fo_noiso_v1)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx) >= 0.40) return false;
      return true;
      break;

    case(WW_fo_looseMVA_v1):
      if (electronID(elIdx, WW_fo_looseMVA_noiso_v1)==0) return false; 
      if (elMiniRelIsoCMS3_EA(elIdx) >= 0.40) return false;
      return true;
      break;

   ////////////////////
   /// WW FO v2     ///
   ////////////////////

    case(WW_fo_noiso_v2):

      //      if (electronID(elIdx, WW_veto_noiso_v2)==0) return false; 
      if (!isTriggerSafenoIso_v1(elIdx)) return false;
      if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
	if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.1) return false;// abs(d0)                                 
	if (fabs(els_dzPV()             .at(elIdx))                       >= 0.373) return false;// abs(dz)                                 
	if (els_exp_innerlayers()       .at(elIdx)                        > 2        ) return false;// expectedMissingInnerHits 
	if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
      }
      else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
	if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.2) return false;// abs(d0)                                 
	if (fabs(els_dzPV()             .at(elIdx))                       >= 0.602) return false;// abs(dz)                                  
	if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
	if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
      }
      else return false;
      return true;

      break;

    case(WW_fo_v2):

      if (electronID(elIdx, WW_fo_noiso_v2)==0) return false; 
      if (!isTriggerSafe_v1(elIdx)) return false;
      return true;
      break;

   ////////////////////
   /// WW FO v3     ///
   ////////////////////

    case(WW_fo_noiso_v3):

      if (!isTriggerSafenoIso_v2(elIdx)) return false;
      if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
	if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.1) return false;// abs(d0)                                 
	if (fabs(els_dzPV()             .at(elIdx))                       >= 0.373) return false;// abs(dz)                                 
	if (els_exp_innerlayers()       .at(elIdx)                        > 0        ) return false;// expectedMissingInnerHits 
	if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
      }
      else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
	if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.2) return false;// abs(d0)                                 
	if (fabs(els_dzPV()             .at(elIdx))                       >= 0.602) return false;// abs(dz)                                  
	if (els_exp_innerlayers()       .at(elIdx)                        > 0        ) return false;// expectedMissingInnerHits
	if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
      }
      else return false;
      return true;

      break;

    case(WW_fo_v3):

      if (electronID(elIdx, WW_fo_noiso_v2)==0) return false; 
      if (!isTriggerSafe_v2(elIdx)) return false;
      return true;
      break;

   ////////////////////
   /// HAD loose v1 ///
   ////////////////////

    case(HAD_loose_noiso_v1):
    case(HAD_loose_v1):
      if (fabs(els_etaSC().at(elIdx)) <= 1.479){
        if (els_sigmaIEtaIEta().at(elIdx) >= 0.01) return false;
        if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
        if (els_hOverE().at(elIdx) >= 0.12) return false;
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.007) return false;
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.15) return false;
      }
      else if((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
        if (els_sigmaIEtaIEta().at(elIdx) >= 0.03) return false;
        if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
        if (els_hOverE().at(elIdx) >= 0.1) return false;
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.009) return false;
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.10) return false;
      }
      else return false;
      break;

   /////////////////////
   /// HAD loose v2 ////
   /////////////////////

    // same as POG phys14 loose
    case(HAD_loose_noiso_v2):
    case(HAD_loose_noiso_v3):
      if (!isLooseElectronPOGphys14noIso(elIdx)) return false;
      return true;
      break;

    case(HAD_loose_v2):
      if (electronID(elIdx, HAD_loose_noiso_v2)==0) return false;
      if (elMiniRelIso(elIdx) > 0.1) return false; 
      return true;
      break;

   /////////////////////
   /// HAD loose v3 ////
   /////////////////////

    // same ID as v2, but use CMS3 miniIso with EA corrections
    case(HAD_loose_v3):
      if (electronID(elIdx, HAD_loose_noiso_v3)==0) return false;
      if (elMiniRelIsoCMS3_EA(elIdx) > 0.1) return false; 
      return true;
      break;

   ////////////////////
   /// HAD loose v4 ////
   ////////////////////

    // use Spring15 POG loose ID
    case(HAD_loose_noiso_v4):
      if (!isLooseElectronPOGspring15noIso_v1(elIdx)) return false;
      return true;
      break;

    // updated EA values
    case(HAD_loose_v4):
      if (electronID(elIdx, HAD_loose_noiso_v4)==0) return false;
      if (elMiniRelIsoCMS3_EA(elIdx,1) > 0.1) return false; 
      return true;
      break;

//   ////////////////////
//   /// HAD loose v5 (80X cut-based ID: never actually adopted)////
//   ////////////////////
//      
//      // use Spring15 POG loose ID
//    case(HAD_loose_noiso_v5):
//      if (!isLooseElectronPOGspring16noIso_v1(elIdx)) return false;
//      if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.05) return false;// abs(d0)       //TightIP2D                          
//      if (fabs(els_dzPV()             .at(elIdx))                       >= 0.10) return false;// abs(dz)  
//      return true;
//      break;
//      
//      // updated EA values
//    case(HAD_loose_v5):
//      if (electronID(elIdx, HAD_loose_noiso_v5)==0) return false;
//      if (elMiniRelIsoCMS3_EA(elIdx,2) > 0.1) return false; 
//      return true;
//      break;


   //////////////////////
   /// STOP sync      ///
   //////////////////////
    case(STOP_sync_v1):
      if (!isMediumElectronPOGphys14(elIdx)) return false;
      return true;
      break;

   //////////////////////
   /// STOP medium  ///
   //////////////////////

    case(STOP_medium_v1):
      if (!isMediumElectronPOG(elIdx)) return false;
      if (eleRelIso03(elIdx, STOP) >= 0.15) return false; 
      return true;
      break;

    case(STOP_medium_v2):
      if (!isMediumElectronPOGphys14noIso_v2(elIdx)) return false;
      //if ( elMiniRelIso(elIdx, true, 0.0, true, false) > 0.1) return false;
      if (  elMiniRelIsoCMS3_DB(elIdx)        > 0.1) return false;
      return true;
      break;

    case(STOP_medium_v3):
      if (!isMediumElectronPOGspring15noIso_v1(elIdx)) return false;
      //if ( elMiniRelIso(elIdx, true, 0.0, true, false) > 0.1) return false;
      if (  elMiniRelIsoCMS3_EA(elIdx,1)        > 0.1) return false;
      return true;
      break;

   /////////////////////
   /// HAD medium v1 ///
   /////////////////////

    case(HAD_medium_noiso_v1):
    case(HAD_medium_v1):
      if (fabs(els_etaSC().at(elIdx)) <= 1.479){
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.004) return false;
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.06) return false; 
        if (els_sigmaIEtaIEta().at(elIdx) >= 0.01) return false; 
        if (els_hOverE().at(elIdx) >= 0.12) return false; 
        if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
      }
      else if((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.007) return false;
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.03) return false; 
        if (els_sigmaIEtaIEta().at(elIdx) >= 0.03) return false; 
        if (els_hOverE().at(elIdx) >= 0.1) return false; 
        if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
      }
      else return false;
      break;

   //////////////////////
   /// HAD medium v2 ////
   //////////////////////

    // same as POG phys14 medium
    case(HAD_medium_noiso_v2):
    case(HAD_medium_noiso_v3):
      if (!isMediumElectronPOGphys14noIso(elIdx)) return false;
      return true;
      break;

    case(HAD_medium_v2):
      if (electronID(elIdx, HAD_medium_noiso_v2)==0) return false;
      if (elMiniRelIso(elIdx) > 0.1) return false; 
      return true;
      break;

   //////////////////////
   /// HAD medium v3 ////
   //////////////////////

    // same ID as v2, but use CMS3 miniIso with EA corrections
    case(HAD_medium_v3):
      if (electronID(elIdx, HAD_medium_noiso_v3)==0) return false;
      if (elMiniRelIsoCMS3_EA(elIdx) > 0.1) return false; 
      return true;
      break;

   ////////////////////
   /// HAD medium v4 ////
   ////////////////////

    // use Spring15 POG medium ID
    case(HAD_medium_noiso_v4):
      if (!isMediumElectronPOGspring15noIso_v1(elIdx)) return false;
      return true;
      break;

    // updated EA values
    case(HAD_medium_v4):
      if (electronID(elIdx, HAD_medium_noiso_v4)==0) return false;
      if (elMiniRelIsoCMS3_EA(elIdx,1) > 0.1) return false; 
      return true;
      break;

//   ////////////////////
//   /// HAD medium v5 (80X cut-based ID: never actually adopted)////
//   ////////////////////
//      
//      // use Spring15 POG medium ID
//    case(HAD_medium_noiso_v5):
//      if (!isMediumElectronPOGspring16noIso_v1(elIdx)) return false;
//      if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.05) return false;// abs(d0)       //TightIP2D                          
//      if (fabs(els_dzPV()             .at(elIdx))                       >= 0.10) return false;// abs(dz)  
//      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false; // Tight IP3D
//      return true;
//      break;
//      
//      // updated EA values
//    case(HAD_medium_v5):
//      if (electronID(elIdx, HAD_medium_noiso_v5)==0) return false;
//      if (elMiniRelIsoCMS3_EA(elIdx,2) > 0.1) return false; 
//      return true;
//      break;

   ////////////////////
   /// SS medium v1 ///
   ////////////////////

    case(SS_medium_noiso_v1):
      if (fabs(els_etaSC().at(elIdx)) <= 1.479){
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.004) return false;
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.06) return false; 
        if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.01) return false; 
        if (els_hOverE().at(elIdx) >= 0.12) return false; 
        if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
      }
      else if((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.007) return false;
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.03) return false; 
        if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.03) return false; 
        if (els_hOverE().at(elIdx) >= 0.1) return false; 
        if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
      }
      else return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
      break;

    case(SS_medium_v1):
      if (electronID(elIdx, SS_medium_noiso_v1)==0) return false; 
      if (eleRelIso03(elIdx, analysis) >= 0.10) return false; 
      return true;
      break;

   ////////////////////
   /// SS medium v2 ///
   ////////////////////

    case(SS_medium_noiso_v2):
      if (electronID(elIdx, SS_fo_noiso_v2)==0) return false;//make sure it's tighter than FO
      if (globalEleMVAreader==0) {
	cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.4) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);

    case(SS_medium_v2):
      if (electronID(elIdx, SS_medium_noiso_v2)==0) return false; 
      if (eleRelIso03(elIdx, analysis) >= 0.10) return false; 
      return true;
      break;

   ////////////////////
   /// SS medium v3 ///
   ////////////////////

    case(SS_medium_noiso_v3):
      if (electronID(elIdx, SS_fo_noiso_v3)==0) return false;//make sure it's tighter than FO
      if (globalEleMVAreader==0) {
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);

    case(SS_medium_v3):
      if (electronID(elIdx, SS_medium_noiso_v3)==0) return false; 
      if (elIDCacheSet) return passMultiIsoCuts(0.10, 0.70, 7.0, elID_cache.getMiniiso(elIdx), elID_cache.getPtratio(elIdx), elID_cache.getPtrel(elIdx) );
      else return passMultiIso(11, elIdx, 0.10, 0.70, 7.0, 0, 0);
      break;

    case(SS_medium_looseMVA_noip_v3): 
    case(SS_medium_noip_v3):
      if (electronID(elIdx, SS_fo_looseMVA_noiso_noip_v3)==0) return false;//make sure it's tighter than FO
      if (globalEleMVAreader==0) {
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      if (!globalEleMVAreader->passesElectronMVAid(elIdx, id_level)) return false;
      //return passMultiIso(11, elIdx, 0.40, 0.7, 7.0);
      return true;
      break;

   ////////////////////
   /// SS medium v4 ///
   ////////////////////

    case(SS_medium_noiso_v4):
      if (electronID(elIdx, SS_fo_noiso_v4)==0) return false;//make sure it's tighter than FO
      if (globalEleMVAreader==0) {
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);

    case(SS_medium_v4):
      if (electronID(elIdx, SS_medium_noiso_v4)==0) return false; 
      if (elIDCacheSet) return passMultiIsoCuts(0.10, 0.76, 7.6, elID_cache.getMiniiso(elIdx), elID_cache.getPtratio(elIdx), elID_cache.getPtrel(elIdx) );
      else return passMultiIso(11, elIdx, 0.10, 0.76, 7.6, 1, 1);
      break;

    case(SS_medium_looseMVA_noip_v4): 
    case(SS_medium_noip_v4):
      if (electronID(elIdx, SS_fo_looseMVA_noiso_noip_v4)==0) return false;//make sure it's tighter than FO
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      if (!globalEleMVAreader->passesElectronMVAid(elIdx, id_level)) return false;
      //return passMultiIso(11, elIdx, 0.40, 0.7, 7.0);
      return true;
      break;

   ////////////////////
   /// SS medium v5 ///
   ////////////////////

    case(SS_medium_noiso_v5):
      if (electronID(elIdx, SS_fo_noiso_v5)==0) return false;//make sure it's tighter than FO
      if (globalEleMVAreader==0) {
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);

    case(SS_medium_noiso_no3chg_v5):
      if (electronID(elIdx, SS_fo_noiso_no3chg_v5)==0) return false;//make sure it's tighter than FO
      if (globalEleMVAreader==0) {
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);

    case(SS_medium_v5):
      if (electronID(elIdx, SS_medium_noiso_v5)==0) return false; 
      if (elIDCacheSet) return passMultiIsoCuts(0.12, 0.80, 7.2, elID_cache.getMiniiso(elIdx), elID_cache.getPtratio(elIdx), elID_cache.getPtrel(elIdx) );
      else return passMultiIso(11, elIdx, 0.12, 0.80, 7.2, 1, 2);
      break;

    case(SS_medium_no3chg_v5):
      if (electronID(elIdx, SS_medium_noiso_no3chg_v5)==0) return false; 
      if (elIDCacheSet) return passMultiIsoCuts(0.12, 0.80, 7.2, elID_cache.getMiniiso(elIdx), elID_cache.getPtratio(elIdx), elID_cache.getPtrel(elIdx) );
      else return passMultiIso(11, elIdx, 0.12, 0.80, 7.2, 1, 2);
      break;

    /////////
    // RA7 //
    /////////

    case(RA7_el_loose):
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_p4().at(elIdx).pt() < 10.) return false;
      if (fabs(els_dxyPV().at(elIdx)) >= 0.05) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false; 
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 1) return false;
      if (!isTriggerSafeRA7(elIdx)) return false;
	  if( elMiniRelIsoCMS3_EA( elIdx, 1 ) > 0.4        ) return false; // minireliso < 0.4
      if (globalEleMVAreader==0){
	    return false;
      }
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);
      break;

    case(RA7_el_fo):
      if (electronID(elIdx, RA7_el_loose)==0) return false;//make sure it's tighter than FO
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false;
      break;

    case(RA7_el_tight):
      if (electronID(elIdx, RA7_el_fo)==0) return false;//make sure it's tighter than FO
      if (elIDCacheSet) return passMultiIsoCuts(0.12, 0.76, 7.2, elID_cache.getMiniiso(elIdx), elID_cache.getPtratio(elIdx), elID_cache.getPtrel(elIdx) );
      else return passMultiIso(11, elIdx, 0.12, 0.76, 7.2, 1, 2);
      break;

    case(SS_medium_looseMVA_noip_v5): 
    case(SS_medium_noip_v5):
      if (electronID(elIdx, SS_fo_looseMVA_noiso_noip_v5)==0) return false;//make sure it's tighter than FO
      if (globalEleMVAreader==0){
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      if (fabs(els_etaSC().at(elIdx)) > 2.5) return false;
      if (els_conv_vtx_flag().at(elIdx)) return false;
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (threeChargeAgree(elIdx)==0) return false;
      if (fabs(els_dzPV().at(elIdx)) >= 0.1) return false;
      if( id_level != SS_medium_looseMVA_noip_v5) {
          if (!globalEleMVAreader->passesElectronMVAid(elIdx, id_level)) return false;
      }
      //return passMultiIso(11, elIdx, 0.40, 0.7, 7.0);
      return true;
      break;


   ////////////////////
   /// WW medium v1 ///
   ////////////////////

    case(WW_medium_noiso_v1):
      if (electronID(elIdx, WW_fo_noiso_v1)==0) return false;//make sure it's tighter than FO
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (globalEleMVAreader==0) {
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      return globalEleMVAreader->passesElectronMVAid(elIdx, id_level);

    case(WW_medium_v1):
      if (electronID(elIdx, WW_medium_noiso_v1)==0) return false; 
      if (elIDCacheSet) return passMultiIsoCuts(0.10, 0.70, 7.0, elID_cache.getMiniiso(elIdx), elID_cache.getPtratio(elIdx), elID_cache.getPtrel(elIdx) );
      else return passMultiIso(11, elIdx, 0.10, 0.70, 7.0, 0, 0);
      break;

    case(WW_medium_looseMVA_noip_v1): 
    case(WW_medium_noip_v1):
      if (electronID(elIdx, WW_fo_looseMVA_noiso_noip_v1)==0) return false;//make sure it's tighter than FO
      if (els_exp_innerlayers().at(elIdx) > 0) return false;
      if (globalEleMVAreader==0) {
	    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
	    return false;
      }
      if (!globalEleMVAreader->passesElectronMVAid(elIdx, id_level)) return false;
      if (elIDCacheSet) return passMultiIsoCuts(0.10, 0.70, 7.0, elID_cache.getMiniiso(elIdx), elID_cache.getPtratio(elIdx), elID_cache.getPtrel(elIdx) );
      else return passMultiIso(11, elIdx, 0.10, 0.70, 7.0, 0, 0);
      break;
      
   ////////////////////
   /// WW medium v2 ///
   ////////////////////

    case(WW_medium_noiso_v2):

      if (!isTriggerSafenoIso_v1(elIdx)) return false;
      if (!isTightElectronPOGspring15noIso_v1(elIdx)) return false;
      return true;
      break;

    case(WW_medium_v2):

      if (!isTriggerSafe_v1(elIdx)) return false;
      if (!isTightElectronPOGspring15_v1(elIdx)) return false;
      return true;
      break;

   ////////////////////
   /// WW medium v3 ///
   ////////////////////

    case(WW_medium_noiso_v3):

      if (!isTriggerSafenoIso_v2(elIdx)) return false;
      if (!isTightElectronPOGspring15noIso_v1(elIdx)) return false;
      return true;
      break;

    case(WW_medium_v3):

      if (!isTriggerSafe_v2(elIdx)) return false;
      if (!isTightElectronPOGspring15_v1(elIdx)) return false;
      return true;
      break;

   /////////////////////
   /// STOP tight    ///
   /////////////////////

    case(STOP_tight_v1):
      if (!isTightElectronPOG(elIdx)) return false;
      if (eleRelIso03(elIdx, STOP) >= 0.10) return false; 
      return true;
      break;
  
    case(STOP_tight_v2):
      if (!isTightElectronPOGphys14noIso_v2(elIdx)) return false;
      //if ( elMiniRelIso(elIdx, true, 0.0, true, false) > 0.1) return false;
      if (  elMiniRelIsoCMS3_DB(elIdx)       > 0.1) return false;
      return true;
      break;

    case(STOP_tight_v3):
      if (!isTightElectronPOGspring15noIso_v1(elIdx)) return false;
      //if ( elMiniRelIso(elIdx, true, 0.0, true, false) > 0.1) return false;
      if (  elMiniRelIsoCMS3_EA(elIdx,1)       > 0.1) return false;
      return true;
      break;

   ////////////////////
   /// HAD tight v1 ///
   ////////////////////

    case(HAD_tight_noiso_v1):
    case(HAD_tight_v1):
      if (fabs(els_etaSC().at(elIdx)) <= 1.479){
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.004) return false;
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.03) return false; 
        if (els_sigmaIEtaIEta().at(elIdx) >= 0.01) return false; 
        if (els_hOverE().at(elIdx) >= 0.12) return false; 
        if (fabs((1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
      }
      else if((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){
        if (fabs(els_dEtaIn().at(elIdx)) >= 0.005) return false;
        if (fabs(els_dPhiIn().at(elIdx)) >= 0.02) return false; 
        if (els_sigmaIEtaIEta().at(elIdx) >= 0.03) return false; 
        if (els_hOverE().at(elIdx) >= 0.1) return false; 
        if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.05) return false;
      }
      else return false;
      break;

   /////////////////////
   /// HAD tight v2 ////
   /////////////////////

    // same as POG phys14 tight
    case(HAD_tight_noiso_v2):
    case(HAD_tight_noiso_v3):
      if (!isTightElectronPOGphys14noIso(elIdx)) return false;
      return true;
      break;

    case(HAD_tight_v2):
      if (electronID(elIdx, HAD_tight_noiso_v2)==0) return false;
      if (elMiniRelIso(elIdx) > 0.1) return false; 
      return true;
      break;

   /////////////////////
   /// HAD tight v3 ////
   /////////////////////

    // same ID as v2, but use CMS3 miniIso with EA corrections
    case(HAD_tight_v3):
      if (electronID(elIdx, HAD_tight_noiso_v3)==0) return false;
      if (elMiniRelIsoCMS3_EA(elIdx) > 0.1) return false; 
      return true;
      break;

   ////////////////////
   /// HAD tight v4 ////
   ////////////////////

    // use Spring15 POG tight ID
    case(HAD_tight_noiso_v4):
      if (!isTightElectronPOGspring15noIso_v1(elIdx)) return false;
      return true;
      break;

    // updated EA values
    case(HAD_tight_v4):
      if (electronID(elIdx, HAD_tight_noiso_v4)==0) return false;
      if (elMiniRelIsoCMS3_EA(elIdx,1) > 0.1) return false; 
      return true;
      break;

//   ////////////////////
//   /// HAD tight v5 (80X cut-based ID: never actually adopted)////
//   ////////////////////
//      
//      // use Spring15 POG tight ID
//    case(HAD_tight_noiso_v5):
//      if (!isTightElectronPOGspring16noIso_v1(elIdx)) return false;
//      if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.05) return false;// abs(d0)       //TightIP2D                          
//      if (fabs(els_dzPV()             .at(elIdx))                       >= 0.10) return false;// abs(dz)  
//      if (fabs(els_ip3d().at(elIdx))/els_ip3derr().at(elIdx) >= 4) return false; // Tight IP3D
//      return true;
//      break;
//      
//      // updated EA values
//    case(HAD_tight_v5):
//      if (electronID(elIdx, HAD_tight_noiso_v5)==0) return false;
//      if (elMiniRelIsoCMS3_EA(elIdx,2) > 0.1) return false; 
//      return true;
//      break;

	//////////////////////
	/// ZMET MVA id v1 ///
	//////////////////////

    case(ZMET_tightMVA_v2): // updated EA values
	  if( electronID(elIdx, ZMET_tightMVA_noiso_v2)==0 ) return false; // tight MVA id no iso
	  if( elMiniRelIsoCMS3_EA( elIdx, 1 ) > 0.1        ) return false; // minireliso < 0.1
	  else return true;
	  break;

    case(ZMET_tightMVA_noiso_v2):
	  if (globalEleMVAreader==0) {
		cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
		return false;
	  }
	  if (fabs(els_etaSC()     .at(elIdx)) >  2.5 ) return false;
	  if (els_conv_vtx_flag()  .at(elIdx)         ) return false;
	  if (els_exp_innerlayers().at(elIdx)  >  0   ) return false;
	  if (fabs(els_dzPV()      .at(elIdx)) >= 0.1 ) return false;
	  if (fabs(els_dxyPV()     .at(elIdx)) >= 0.05) return false;
	  if (!globalEleMVAreader->passesElectronMVAid(elIdx, SS_medium_noip_v5)) return false; //tight MVA working point 
	  else return true;
	  break;

	  // loose MVA used to veto on electrons to stay synced with edge/multilepton

    case(ZMET_looseMVA_v1): // updated EA values
	  if( electronID(elIdx, ZMET_looseMVA_noiso_v1)==0 ) return false; // loose MVA id no iso
	  if( elMiniRelIsoCMS3_EA( elIdx, 1 ) > 0.4        ) return false; // minireliso < 0.4
	  else return true;
	  break;

    case(ZMET_looseMVA_noiso_v1):
	  if (globalEleMVAreader==0) {
		cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
		return false;
	  }
	  if (fabs(els_etaSC()     .at(elIdx)) >  2.5 ) return false;
	  if (els_conv_vtx_flag()  .at(elIdx)         ) return false;
	  if (els_exp_innerlayers().at(elIdx)  >  0   ) return false;
	  if (fabs(els_dzPV()      .at(elIdx)) >= 0.1 ) return false;
	  if (fabs(els_dxyPV()     .at(elIdx)) >= 0.05) return false;
	  if (!globalEleMVAreader->passesElectronMVAid(elIdx, SS_fo_looseMVA_noiso_noip_v5)) return false; //loose MVA working point 
	  else return true;
	  break;
	  
    case(ZMET_tightMVA_v1):
	  if( electronID(elIdx, ZMET_tightMVA_noiso_v1)==0 ) return false; // tight MVA id no iso
	  // if( elMiniRelIso( elIdx, true, 0.0, false, true ) != elMiniRelIsoCMS3_EA( elIdx ) ){ // this is a check. delete me later
	  // 	std::cout<<"pfcandiso: "<< elMiniRelIso( elIdx, true, 0.0, false, true ) << endl;
	  // 	std::cout<<"cms3iso:   "<< elMiniRelIsoCMS3_EA( elIdx ) << endl;
	  // }	  
	  // // if( elMiniRelIso( elIdx, true, 0.0, false, true ) > 0.1 ) return false;
	  if( elMiniRelIsoCMS3_EA( elIdx ) > 0.1           ) return false; // minireliso < 0.1
	  else return true;
	  break;

    case(ZMET_tightMVA_noiso_v1):
	  if (globalEleMVAreader==0) {
		cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
		return false;
	  }
	  if (fabs(els_etaSC()     .at(elIdx)) >  2.5 ) return false;
	  if (els_conv_vtx_flag()  .at(elIdx)         ) return false;
	  if (els_exp_innerlayers().at(elIdx)  >  0   ) return false;
	  if (fabs(els_dzPV()      .at(elIdx)) >= 0.1 ) return false;
	  if (fabs(els_dxyPV()     .at(elIdx)) >= 0.05) return false;
	  if (!globalEleMVAreader->passesElectronMVAid(elIdx, SS_medium_noip_v3)) return false; //tight MVA working point 
	  else return true;
	  break;
	  
	/////////////////////
	/// ZMET loose v1 ///
	/////////////////////

  case(ZMET_loose_v2):
	if( !isLooseElectronPOGphys14noIso(elIdx)               ) return false;
	if( elMiniRelIso( elIdx, true, 0.0, false, true ) > 0.1 ) return false;
	else return true;
	break;

  case(ZMET_loose_noiso_v2):
	if( !isLooseElectronPOGphys14noIso(elIdx)               ) return false;
	else return true;
	break;

  case(ZMET_loose_v1):
	if (!isLooseElectronPOGphys14noIso(elIdx)) return false;
	if (eleRelIso03(elIdx, analysis) >= 0.15) return false; 
	else return true;
	break;

  case(ZMET_loose_noiso_v1):
	if (!isLooseElectronPOGphys14noIso(elIdx)) return false;
	else return true;
	break;

   ///////////////
   /// Default ///
   ///////////////
  default:
    {
      cout << "Warning! Electron ID not defined for this id_level!" << endl;
      return false;
    }

  }//switch
  return true;
}

bool isVetoElectronPOGphys14(unsigned int elIdx){
  if (!isVetoElectronPOGphys14noIso(elIdx)) return false;
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (eleRelIso03DB(elIdx)                                          >= 0.164369) return false;// PF isolation w/dBeta PU correction / pT (cone dR=0.3) 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (eleRelIso03DB(elIdx)                                          >= 0.212604) return false;// PF isolation w/dBeta PU correction / pT (cone dR=0.3) 
  }
  else return false;
  return true;
}

bool isVetoElectronPOGphys14_v2(unsigned int elIdx){
  if (!isVetoElectronPOGphys14noIso_v2(elIdx)) return false;
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (eleRelIso03EA(elIdx)                                          >= 0.158721) return false;// PF isolation w/dBeta PU correction / pT (cone dR=0.3) 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (eleRelIso03EA(elIdx)                                          >= 0.177032) return false;// PF isolation w/dBeta PU correction / pT (cone dR=0.3) 
  }
  else return false;
  return true;
}

bool isVetoElectronPOGspring15_v1(unsigned int elIdx){
  if (!isVetoElectronPOGspring15noIso_v1(elIdx)) return false;
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (eleRelIso03_90ContEA(elIdx)                                          >= 0.126) return false;// PF isolation w/dBeta PU correction / pT (cone dR=0.3) 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (eleRelIso03_90ContEA(elIdx)                                          >= 0.144) return false;// PF isolation w/dBeta PU correction / pT (cone dR=0.3) 
  }
  else return false;
  return true;
}

bool isLooseElectronPOGphys14(unsigned int elIdx){
  if (!isLooseElectronPOGphys14noIso(elIdx)) return false;
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (eleRelIso03DB(elIdx)                                          >= 0.120026) return false;// PF isolation w/dBeta PU correction / pT (cone dR=0.3) 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (eleRelIso03DB(elIdx)                                          >= 0.162914) return false;// PF isolation w/dBeta PU correction / pT (cone dR=0.3) 
  }
  else return false;
  return true;
}

bool isLooseElectronPOGphys14_v2(unsigned int elIdx){
  if (!isLooseElectronPOGphys14noIso_v2(elIdx)) return false;
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (eleRelIso03EA(elIdx)                                          >= 0.130136) return false;// PF isolation w/EA PU correction / pT (cone dR=0.3) 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (eleRelIso03EA(elIdx)                                          >= 0.163368) return false;// PF isolation w/EA PU correction / pT (cone dR=0.3) 
  }
  else return false;
  return true;
}

bool isLooseElectronPOGspring15_v1(unsigned int elIdx){
  if (!isLooseElectronPOGspring15noIso_v1(elIdx)) return false;
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (eleRelIso03_90ContEA(elIdx)                                          >= 0.0893) return false;// PF isolation w/EA PU correction / pT (cone dR=0.3) 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (eleRelIso03_90ContEA(elIdx)                                          >= 0.121) return false;// PF isolation w/EA PU correction / pT (cone dR=0.3) 
  }
  else return false;
  return true;
}

bool isMediumElectronPOGphys14(unsigned int elIdx){
  if (!isMediumElectronPOGphys14noIso(elIdx)) return false;
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (eleRelIso03DB(elIdx)                                          >= 0.097213) return false;// PF isolation w/dBeta PU correction / pT (cone dR=0.3) 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (eleRelIso03DB(elIdx)                                          >= 0.116708) return false;// PF isolation w/dBeta PU correction / pT (cone dR=0.3) 
  }
  else return false;
  return true;
}

bool isMediumElectronPOGphys14_v2(unsigned int elIdx){
  if (!isMediumElectronPOGphys14noIso_v2(elIdx)) return false;
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (eleRelIso03EA(elIdx)                                          >= 0.107587) return false;// PF isolation w/EA PU correction / pT (cone dR=0.3) 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (eleRelIso03EA(elIdx)                                          >= 0.113254) return false;// PF isolation w/EA PU correction / pT (cone dR=0.3) 
  }
  else return false;
  return true;
}

bool isMediumElectronPOGspring15_v1(unsigned int elIdx){
  if (!isMediumElectronPOGspring15noIso_v1(elIdx)) return false;
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (eleRelIso03_90ContEA(elIdx)                                          >= 0.0766) return false;// PF isolation w/EA PU correction / pT (cone dR=0.3) 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (eleRelIso03_90ContEA(elIdx)                                          >= 0.0678) return false;// PF isolation w/EA PU correction / pT (cone dR=0.3) 
  }
  else return false;
  return true;
}

bool isTightElectronPOGphys14(unsigned int elIdx){
  if (!isTightElectronPOGphys14noIso(elIdx)) return false;
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (eleRelIso03DB(elIdx)                                          >= 0.074355) return false;// PF isolation w/dBeta PU correction / pT (cone dR=0.3) 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (eleRelIso03DB(elIdx)                                          >= 0.090185) return false;// PF isolation w/dBeta PU correction / pT (cone dR=0.3) 
  }
  else return false;
  return true;
}

bool isTightElectronPOGphys14_v2(unsigned int elIdx){
  if (!isTightElectronPOGphys14noIso_v2(elIdx)) return false;
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (eleRelIso03EA(elIdx)                                          >= 0.069537) return false;// PF isolation w/EA PU correction / pT (cone dR=0.3) 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (eleRelIso03EA(elIdx)                                          >= 0.078265) return false;// PF isolation w/EA PU correction / pT (cone dR=0.3) 
  }
  else return false;
  return true;
}

bool isTightElectronPOGspring15_v1(unsigned int elIdx){
  if (!isTightElectronPOGspring15noIso_v1(elIdx)) return false;
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (eleRelIso03_90ContEA(elIdx)                                          >= 0.0354) return false;// PF isolation w/EA PU correction / pT (cone dR=0.3) 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (eleRelIso03_90ContEA(elIdx)                                          >= 0.0646) return false;// PF isolation w/EA PU correction / pT (cone dR=0.3) 
  }
  else return false;
  return true;
}

bool isVetoElectronPOGphys14noIso(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.011100) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.016315) return false;// abs(dEtaIn) 			   
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.252044) return false;// abs(dPhiIn) 			   
    if (els_hOverE()                                     .at(elIdx)   >= 0.345843) return false;// hOverE 				   	
	if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) - 							                           
			  (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.248070) return false;// ooEmooP				   
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.060279) return false;// abs(d0)				   
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.800538) return false;// abs(dz) 				   
	if (els_exp_innerlayers()       .at(elIdx)                        > 2        ) return false;// expectedMissingInnerHits 
	if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.033987) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.010671) return false;// abs(dEtaIn) 			    
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.245263) return false;// abs(dPhiIn) 			    
    if (els_hOverE()                                     .at(elIdx)   >= 0.134691) return false;// hOverE 				    
	if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) - 							                           
			  (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.157160) return false;// ooEmooP				   
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.273097) return false;// abs(d0)				   
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.885860) return false;// abs(dz) 				    
	if (els_exp_innerlayers()       .at(elIdx)                        > 3        ) return false;// expectedMissingInnerHits
	if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

bool isVetoElectronPOGphys14noIso_v2(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.011586) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.013625) return false;// abs(dEtaIn)                     
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.230374) return false;// abs(dPhiIn)                     
    if (els_hOverE()                                     .at(elIdx)   >= 0.181130) return false;// hOverE                                       
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.295751) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.094095) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.713070) return false;// abs(dz)                                 
        if (els_exp_innerlayers()       .at(elIdx)                        > 2        ) return false;// expectedMissingInnerHits 
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.031849) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.011932) return false;// abs(dEtaIn)                      
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.255450) return false;// abs(dPhiIn)                      
    if (els_hOverE()                                     .at(elIdx)   >= 0.223870) return false;// hOverE                                   
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.155501) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.342293) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.953461) return false;// abs(dz)                                  
        if (els_exp_innerlayers()       .at(elIdx)                        > 3        ) return false;// expectedMissingInnerHits
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

bool isVetoElectronPOGspring15noIso_v1(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.0114) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.0152) return false;// abs(dEtaIn)                     
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.216) return false;// abs(dPhiIn)                     
    if (els_hOverE()                                     .at(elIdx)   >= 0.181) return false;// hOverE                                       
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.207) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.0564) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.472) return false;// abs(dz)                                 
        if (els_exp_innerlayers()       .at(elIdx)                        > 2        ) return false;// expectedMissingInnerHits 
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.0352) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.0113) return false;// abs(dEtaIn)                      
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.237) return false;// abs(dPhiIn)                      
    if (els_hOverE()                                     .at(elIdx)   >= 0.116) return false;// hOverE                                   
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.174) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.222) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.921) return false;// abs(dz)                                  
        if (els_exp_innerlayers()       .at(elIdx)                        > 3        ) return false;// expectedMissingInnerHits
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

// This is the 80X tuning. It comes with no IP cuts, which have to be included later. It also uses a different DEta
bool isVetoElectronPOGspring16noIso_v1(unsigned int elIdx){ 
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.0115) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn().at(elIdx) - els_etaSC().at(elIdx) + els_scSeedEta().at(elIdx) )  >= 0.00749) return false;// abs(dEtaIn)                     
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.228) return false;// abs(dPhiIn)                     
    if (els_hOverE()                                     .at(elIdx)   >= 0.356) return false;// hOverE                                       
    if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.299) return false;// ooEmooP                             
    if (els_exp_innerlayers()       .at(elIdx)                        > 2        ) return false;// expectedMissingInnerHits 
    if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto   
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.037) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn().at(elIdx) - els_etaSC().at(elIdx) + els_scSeedEta().at(elIdx) )  >= 0.00895) return false;// abs(dEtaIn)                      
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.213) return false;// abs(dPhiIn)                      
    if (els_hOverE()                                     .at(elIdx)   >= 0.211) return false;// hOverE                                   
    if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
	      (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.15) return false;// ooEmooP                             
    if (els_exp_innerlayers()       .at(elIdx)                        > 3        ) return false;// expectedMissingInnerHits
    if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

bool isLooseElectronPOGphys14noIso(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.010557) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.012442) return false;// abs(dEtaIn) 			   
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.072624) return false;// abs(dPhiIn) 			   
    if (els_hOverE()                                     .at(elIdx)   >= 0.121476) return false;// hOverE 				   
	if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) - 							                           
			  (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.221803) return false;// ooEmooP				   
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.022664) return false;// abs(d0)				   
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.173670) return false;// abs(dz) 				   
	if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits 
	if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.032602) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.010654) return false;// abs(dEtaIn) 			    
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.145129) return false;// abs(dPhiIn) 			    
    if (els_hOverE()                                     .at(elIdx)   >= 0.131862) return false;// hOverE 				    
	if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) - 							                           
			  (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.142283) return false;// ooEmooP				   
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.097358) return false;// abs(d0)				   
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.198444) return false;// abs(dz) 				    
	if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
	if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

bool isLooseElectronPOGphys14noIso_v2(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.010331) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.009277) return false;// abs(dEtaIn)                     
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.094739) return false;// abs(dPhiIn)                     
    if (els_hOverE()                                     .at(elIdx)   >= 0.093068) return false;// hOverE                                  
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.189968) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.035904) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.075496) return false;// abs(dz)                                 
        if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits 
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.031838) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.009833) return false;// abs(dEtaIn)                      
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.149934) return false;// abs(dPhiIn)                      
    if (els_hOverE()                                     .at(elIdx)   >= 0.115754) return false;// hOverE                                   
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.140662) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.099266) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.197897) return false;// abs(dz)                                  
        if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

bool isLooseElectronPOGspring15noIso_v1(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.0103) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.0105) return false;// abs(dEtaIn)                     
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.115) return false;// abs(dPhiIn)                     
    if (els_hOverE()                                     .at(elIdx)   >= 0.104) return false;// hOverE                                  
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.102) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.0261) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.41) return false;// abs(dz)                                 
        if (els_exp_innerlayers()       .at(elIdx)                        > 2        ) return false;// expectedMissingInnerHits 
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.0301) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.00814) return false;// abs(dEtaIn)                      
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.182) return false;// abs(dPhiIn)                      
    if (els_hOverE()                                     .at(elIdx)   >= 0.0897) return false;// hOverE                                   
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.126) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.118) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.822) return false;// abs(dz)                                  
        if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

// This is the 80X tuning. It comes with no IP cuts, which have to be included later. It also uses a different DEta
bool isLooseElectronPOGspring16noIso_v1(unsigned int elIdx){ 
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.011) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn().at(elIdx) - els_etaSC().at(elIdx) + els_scSeedEta().at(elIdx) )  >= 0.00477) return false;// abs(dEtaIn)                     
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.222) return false;// abs(dPhiIn)                     
    if (els_hOverE()                                     .at(elIdx)   >= 0.298) return false;// hOverE                                       
    if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
	      (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.241) return false;// ooEmooP                             
    if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits 
    if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto  
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.0314) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn().at(elIdx) - els_etaSC().at(elIdx) + els_scSeedEta().at(elIdx) )  >= 0.00868) return false;// abs(dEtaIn)                      
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.213) return false;// abs(dPhiIn)                      
    if (els_hOverE()                                     .at(elIdx)   >= 0.101) return false;// hOverE                                   
    if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
	      (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.14) return false;// ooEmooP                             
    if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
    if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

bool isMediumElectronPOGphys14noIso(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.010399) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.007641) return false;// abs(dEtaIn) 			   
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.032643) return false;// abs(dPhiIn) 			   
    if (els_hOverE()                                     .at(elIdx)   >= 0.060662) return false;// hOverE 				   
	if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) - 							                           
			  (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.153897) return false;// ooEmooP				   
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.011811) return false;// abs(d0)				   
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.070775) return false;// abs(dz) 				   
	if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits 
	if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.029524) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.009285) return false;// abs(dEtaIn) 			    
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.042447) return false;// abs(dPhiIn) 			    
    if (els_hOverE()                                     .at(elIdx)   >= 0.104263) return false;// hOverE 				    
	if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) - 							                           
			  (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.137468) return false;// ooEmooP				   
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.051682) return false;// abs(d0)				   
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.180720) return false;// abs(dz) 				    
	if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
	if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

bool isMediumElectronPOGphys14noIso_v2(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.009996) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.008925) return false;// abs(dEtaIn)                     
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.035973) return false;// abs(dPhiIn)                     
    if (els_hOverE()                                     .at(elIdx)   >= 0.050537) return false;// hOverE                                  
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.091942) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.012235) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.042020) return false;// abs(dz)                                 
        if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits 
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.030135) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.007429) return false;// abs(dEtaIn)                      
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.067879) return false;// abs(dPhiIn)                      
    if (els_hOverE()                                     .at(elIdx)   >= 0.086782) return false;// hOverE                                   
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.100683) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.036719) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.138142) return false;// abs(dz)                                  
        if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

bool isMediumElectronPOGspring15noIso_v1(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.0101) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.0103) return false;// abs(dEtaIn)                     
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.0336) return false;// abs(dPhiIn)                     
    if (els_hOverE()                                     .at(elIdx)   >= 0.0876) return false;// hOverE                                  
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.0174) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.0118) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.373) return false;// abs(dz)                                 
        if (els_exp_innerlayers()       .at(elIdx)                        > 2        ) return false;// expectedMissingInnerHits 
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.0283) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.00733) return false;// abs(dEtaIn)                      
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.114) return false;// abs(dPhiIn)                      
    if (els_hOverE()                                     .at(elIdx)   >= 0.0678) return false;// hOverE                                   
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.0898) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.0739) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.602) return false;// abs(dz)                                  
        if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

// This is the 80X tuning. It comes with no IP cuts, which have to be included later. It also uses a different DEta
bool isMediumElectronPOGspring16noIso_v1(unsigned int elIdx){ 
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.00998) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn().at(elIdx) - els_etaSC().at(elIdx) + els_scSeedEta().at(elIdx) )  >= 0.00311) return false;// abs(dEtaIn)                     
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.103) return false;// abs(dPhiIn)                     
    if (els_hOverE()                                     .at(elIdx)   >= 0.253) return false;// hOverE                                       
    if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
	      (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.134) return false;// ooEmooP                             
    if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits 
    if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto   
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.0298) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn().at(elIdx) - els_etaSC().at(elIdx) + els_scSeedEta().at(elIdx) )  >= 0.00609) return false;// abs(dEtaIn)                      
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.045) return false;// abs(dPhiIn)                      
    if (els_hOverE()                                     .at(elIdx)   >= 0.0878) return false;// hOverE                                   
    if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
	      (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.13) return false;// ooEmooP                             
    if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
    if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto 
  }
  else return false;
  return true;
}

bool isTightElectronPOGphys14noIso(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.010181) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.006574) return false;// abs(dEtaIn) 			   
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.022868) return false;// abs(dPhiIn) 			   
    if (els_hOverE()                                     .at(elIdx)   >= 0.037553) return false;// hOverE 				   
	if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) - 							                           
			  (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.131191) return false;// ooEmooP				   
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.009924) return false;// abs(d0)				   
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.015310) return false;// abs(dz) 				   
	if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits 
	if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.028766) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.005681) return false;// abs(dEtaIn) 			    
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.032046) return false;// abs(dPhiIn) 			    
    if (els_hOverE()                                     .at(elIdx)   >= 0.081902) return false;// hOverE 				    
	if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) - 							                           
			  (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.106055) return false;// ooEmooP				   
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.027261) return false;// abs(d0)				   
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.147154) return false;// abs(dz) 				    
	if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
	if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

bool isTightElectronPOGphys14noIso_v2(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.009947) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.006046) return false;// abs(dEtaIn)                     
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.028092) return false;// abs(dPhiIn)                     
    if (els_hOverE()                                     .at(elIdx)   >= 0.045772) return false;// hOverE                                  
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.020118) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.008790) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.021226) return false;// abs(dz)                                 
        if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits 
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.028237) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.007057) return false;// abs(dEtaIn)                      
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.030159) return false;// abs(dPhiIn)                      
    if (els_hOverE()                                     .at(elIdx)   >= 0.067778) return false;// hOverE                                   
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.098919) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.027984) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.133431) return false;// abs(dz)                                  
        if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

bool isTightElectronPOGspring15noIso_v1(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.0101) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.00926) return false;// abs(dEtaIn)                     
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.0336) return false;// abs(dPhiIn)                     
    if (els_hOverE()                                     .at(elIdx)   >= 0.0597) return false;// hOverE                                  
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.012) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.0111) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.0466) return false;// abs(dz)                                 
        if (els_exp_innerlayers()       .at(elIdx)                        > 2        ) return false;// expectedMissingInnerHits 
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.0279) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn()                                .at(elIdx))  >= 0.00724) return false;// abs(dEtaIn)                      
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.0918) return false;// abs(dPhiIn)                      
    if (els_hOverE()                                     .at(elIdx)   >= 0.0615) return false;// hOverE                                   
        if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
                          (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.00999) return false;// ooEmooP                             
    if (fabs(els_dxyPV()            .at(elIdx))                       >= 0.0351) return false;// abs(d0)                                 
    if (fabs(els_dzPV()             .at(elIdx))                       >= 0.417) return false;// abs(dz)                                  
        if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
        if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

// This is the 80X tuning. It comes with no IP cuts, which have to be included later. It also uses a different DEta
bool isTightElectronPOGspring16noIso_v1(unsigned int elIdx){ 
  if (fabs(els_etaSC().at(elIdx)) <= 1.479){                                                    // Barrel 
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.00998) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn().at(elIdx) - els_etaSC().at(elIdx) + els_scSeedEta().at(elIdx) )  >= 0.00318) return false;// abs(dEtaIn)                     
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.0816) return false;// abs(dPhiIn)                     
    if (els_hOverE()                                     .at(elIdx)   >= 0.0414) return false;// hOverE                                       
    if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
	      (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.0129) return false;// ooEmooP                             
    if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits 
    if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto   
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_sigmaIEtaIEta_full5x5()                      .at(elIdx)   >= 0.0292) return false;// full5x5_sigmaIetaIeta   
    if (fabs(els_dEtaIn().at(elIdx) - els_etaSC().at(elIdx) + els_scSeedEta().at(elIdx) )  >= 0.00605) return false;// abs(dEtaIn)                      
    if (fabs(els_dPhiIn()                                .at(elIdx))  >= 0.0394) return false;// abs(dPhiIn)                      
    if (els_hOverE()                                     .at(elIdx)   >= 0.0641) return false;// hOverE                                   
    if (fabs( (1.0/els_ecalEnergy()                      .at(elIdx)) -
	      (els_eOverPIn().at(elIdx)/els_ecalEnergy() .at(elIdx))) >= 0.0129) return false;// ooEmooP                             
    if (els_exp_innerlayers()       .at(elIdx)                        > 1        ) return false;// expectedMissingInnerHits
    if (els_conv_vtx_flag()         .at(elIdx)                                   ) return false;// pass conversion veto    
  }
  else return false;
  return true;
}

// Implementation taken from:
// cut values: https://github.com/ikrav/cmssw/blob/egm_id_7.4.12_v1/RecoEgamma/ElectronIdentification/python/Identification/heepElectronID_HEEPV60_cff.py
// list of cuts modules: https://github.com/ikrav/cmssw/blob/egm_id_7.4.12_v1/RecoEgamma/ElectronIdentification/python/Identification/heepElectronID_tools.py#L258
// cut modules: https://github.com/ikrav/cmssw/tree/egm_id_7.4.12_v1/RecoEgamma/ElectronIdentification/plugins/cuts
bool isHEEPV60(unsigned int elIdx){
  if (fabs(els_etaSC().at(elIdx)) <= 1.4442){ 
    if (els_isEcalDriven()                                                   .at(elIdx)   == false  ) return false;
    if (fabs(els_dEtaIn().at(elIdx) - els_etaSC().at(elIdx) + els_scSeedEta().at(elIdx))  >=  0.004 ) return false;
    if (fabs(els_dPhiIn()                                                    .at(elIdx))  >=  0.06  ) return false;
    if (els_hOverE().at(elIdx) >= 1/els_eSC().at(elIdx) + 0.05                                      ) return false;
    if ( els_e2x5Max_full5x5().at(elIdx) / els_e5x5_full5x5().at(elIdx) <= 0.94 &&
	 els_e1x5_full5x5().at(elIdx)    / els_e5x5_full5x5().at(elIdx) <= 0.83                     ) return false;
    if ( (els_ecalIso().at(elIdx) + els_hcalDepth1TowerSumEt().at(elIdx) ) 
	 >= 2 + 0.03*els_p4().at(elIdx).pt() + 0.28 * evt_fixgridfastjet_all_rho()                  ) return false;
    if (els_tkIso()                                                          .at(elIdx)   >=  5     ) return false;
    if (els_exp_innerlayers()                                                .at(elIdx)   > 1       ) return false;
    if (fabs(els_dxyPV()                                                     .at(elIdx))  >= 0.02   ) return false;                                 
  }
  else if ((fabs(els_etaSC().at(elIdx)) > 1.566) && (fabs(els_etaSC().at(elIdx)) < 2.5)){       // Endcap
    if (els_isEcalDriven()                                                   .at(elIdx)   == false  ) return false;
    if (fabs(els_dEtaIn().at(elIdx) - els_etaSC().at(elIdx) + els_scSeedEta().at(elIdx))  >=  0.006 ) return false;
    if (fabs(els_dPhiIn()                                                    .at(elIdx))  >=  0.06  ) return false;
    if (els_hOverE().at(elIdx) >= 5/els_eSC().at(elIdx) + 0.05                                      ) return false;
    if (els_sigmaIEtaIEta_full5x5()                                          .at(elIdx)   >=  0.03  ) return false;
    if ( els_p4().at(elIdx).pt() < 50 && 
	 ( (els_ecalIso().at(elIdx) + els_hcalDepth1TowerSumEt().at(elIdx))  
	   >= 2.5 + 0.28 * evt_fixgridfastjet_all_rho() )                                           ) return false;
    if ( els_p4().at(elIdx).pt() > 50 &&
	 ( (els_ecalIso().at(elIdx) + els_hcalDepth1TowerSumEt().at(elIdx)) 
	   >= 2   + 0.03*(els_p4().at(elIdx).pt()-50) + 0.28 * evt_fixgridfastjet_all_rho() )       ) return false;
    if (els_tkIso()                                                          .at(elIdx)   >=  5     ) return false;
    if (els_exp_innerlayers()                                                .at(elIdx)   > 1       ) return false;
    if (fabs(els_dxyPV()                                                     .at(elIdx))  >= 0.05   ) return false;     
  }
  else return false; // HEEP does not use the crack region
  return true;
  
}

int eleTightID(unsigned int elIdx, analysis_t analysis, int version){
  switch (analysis){
    case (POG):
      if (!isTightElectronPOG(elIdx)) return 3;
      if (!isMediumElectronPOG(elIdx)) return 2;
      if (!isLooseElectronPOG(elIdx)) return 1;
      if (!isVetoElectronPOG(elIdx)) return 0;
      break;
    case (SS):
      if (electronID(elIdx, SS_medium_v4)) return 2;
      if (electronID(elIdx, SS_fo_looseMVA_v4)) return 1;
      if (electronID(elIdx, SS_veto_v4)) return 0;
      break;
    case (WW):
      if (electronID(elIdx, WW_medium_v2)) return 2;
      if (electronID(elIdx, WW_fo_v2)) return 1;
      if (electronID(elIdx, WW_veto_v2)) return 0;
      break;
    case (HAD):
      if (version == 1){
        if (electronID(elIdx, HAD_tight_v1)) return 3;
        if (electronID(elIdx, HAD_medium_v1)) return 2;
        if (electronID(elIdx, HAD_loose_v1)) return 1;
        if (electronID(elIdx, HAD_veto_v1)) return 0;
      }
      if (version == 2){
        if (electronID(elIdx, HAD_tight_v2)) return 3;
        if (electronID(elIdx, HAD_medium_v2)) return 2;
        if (electronID(elIdx, HAD_loose_v2)) return 1;
        if (electronID(elIdx, HAD_veto_v2)) return 0;
      }
      if (version == 3){
        if (electronID(elIdx, HAD_tight_v3)) return 3;
        if (electronID(elIdx, HAD_medium_v3)) return 2;
        if (electronID(elIdx, HAD_loose_v3)) return 1;
        if (electronID(elIdx, HAD_veto_v3)) return 0;
      }
      if (version == 4){
        if (electronID(elIdx, HAD_tight_v4)) return 3;
        if (electronID(elIdx, HAD_medium_v4)) return 2;
        if (electronID(elIdx, HAD_loose_v4)) return 1;
        if (electronID(elIdx, HAD_veto_v4)) return 0;
      }
      break;
    case (STOP):
      if (electronID(elIdx, STOP_tight_v2)) return 3;
      if (electronID(elIdx, STOP_medium_v2)) return 2;
      if (electronID(elIdx, STOP_loose_v2)) return 1;
      if (electronID(elIdx, STOP_veto_v2)) return 0;
      break;
    case (ZMET):
      if (electronID(elIdx, ZMET_loose_v1)) return 0;
  }
  return -1;
}

bool threeChargeAgree(unsigned int elIdx){
  return els_isGsfCtfScPixChargeConsistent().at(elIdx);
}

int tightChargeEle(unsigned int elIdx){
  if (threeChargeAgree(elIdx))                                          return 2;
  else if (els_trk_charge().at(elIdx) == els_sccharge().at(elIdx))      return 1;
  else                                                                  return 0;
}

void readMVA::DumpValues(){
  cout << "ele_kfhits:                   " << ele_kfhits_                   << endl;
  cout << "ele_oldsigmaietaieta:         " << ele_oldsigmaietaieta_         << endl;
  cout << "ele_oldsigmaiphiiphi:         " << ele_oldsigmaiphiiphi_         << endl;
  cout << "ele_oldcircularity:           " << ele_oldcircularity_           << endl;
  cout << "ele_oldr9:                    " << ele_oldr9_                    << endl;
  cout << "ele_scletawidth:              " << ele_scletawidth_              << endl;
  cout << "ele_sclphiwidth:              " << ele_sclphiwidth_              << endl;
  cout << "ele_he:                       " << ele_he_                       << endl;
  cout << "ele_kfchi2:                   " << ele_kfchi2_                   << endl;
  cout << "ele_chi2_hits:                " << ele_chi2_hits_                << endl;
  cout << "ele_fbrem:                    " << ele_fbrem_                    << endl;
  cout << "ele_ep:                       " << ele_ep_                       << endl;
  cout << "ele_eelepout:                 " << ele_eelepout_                 << endl;
  cout << "ele_IoEmIop:                  " << ele_IoEmIop_                  << endl;
  cout << "ele_deltaetain:               " << ele_deltaetain_               << endl;
  cout << "ele_deltaphiin:               " << ele_deltaphiin_               << endl;
  cout << "ele_deltaetaseed:             " << ele_deltaetaseed_             << endl;
  cout << "ele_psEoverEraw:              " << ele_psEoverEraw_              << endl;
  cout << "ele_pT:                       " << ele_pT_                       << endl;
  cout << "ele_isbarrel:                 " << ele_isbarrel_                 << endl;
  cout << "ele_isendcap:                 " << ele_isendcap_                 << endl;
  cout << "scl_eta:                      " << scl_eta_                      << endl;
  //additional variables for 25ns version    
  cout << "ele_gsfhits:                  " << ele_gsfhits_                  << endl;
  cout << "ele_expectedMissingInnerHits: " << ele_expectedMissingInnerHits_ << endl;
  cout << "ele_convVtxFitProbability:    " << ele_convVtxFitProbability_    << endl;

}

void readMVA::InitMVA(string path, bool v25ns, bool use_miniaod, int MVAversion){

  v25ns_ = v25ns;
  use_miniaod_ = use_miniaod;
  MVAversion_ = MVAversion;

  //Declare all variables
  ele_kfhits_           = 0;  
  ele_oldsigmaietaieta_ = 0;  
  ele_oldsigmaiphiiphi_ = 0;  
  ele_oldcircularity_   = 0;  
  ele_oldr9_            = 0;  
  ele_scletawidth_      = 0;  
  ele_sclphiwidth_      = 0;  
  ele_he_               = 0;  
  ele_kfchi2_           = 0;  
  ele_chi2_hits_        = 0;  
  ele_fbrem_            = 0;  
  ele_ep_               = 0;  
  ele_eelepout_         = 0;  
  ele_IoEmIop_          = 0;  
  ele_deltaetain_       = 0;  
  ele_deltaphiin_       = 0;  
  ele_deltaetaseed_     = 0;  
  ele_psEoverEraw_      = 0;
  ele_pT_               = 0; 
  ele_isbarrel_         = 0; 
  ele_isendcap_         = 0; 
  scl_eta_              = 0; 
  //additional variables for 25ns version    
  ele_gsfhits_                  = 0;
  ele_expectedMissingInnerHits_ = 0;
  ele_convVtxFitProbability_    = 0;

  //Declare Reader
  TMVA::Reader *reader1 = new TMVA::Reader();
  TMVA::Reader *reader2 = new TMVA::Reader();
  TMVA::Reader *reader3 = new TMVA::Reader();
  TMVA::Reader *reader4 = new TMVA::Reader();
  TMVA::Reader *reader5 = new TMVA::Reader();
  TMVA::Reader *reader6 = new TMVA::Reader();
  readers.push_back(reader1);
  readers.push_back(reader2);
  readers.push_back(reader3);
  readers.push_back(reader4);
  readers.push_back(reader5);
  readers.push_back(reader6);

  //Shut the hell up
  TMVA::gConfig().SetSilent( kTRUE );

  //Find files
  TString version = "oldscenario2phys14";
  if (v25ns) version = "oldNonTrigSpring15_ConvVarCwoBoolean_TMVA412_FullStatLowPt_PairNegWeightsGlobal";
  if (MVAversion == 74) {
    files.push_back(Form("%s/data/EIDmva_EE_10_%s_BDT.weights.xml" , path.c_str(), version.Data()));
    files.push_back(Form("%s/data/EIDmva_EB1_10_%s_BDT.weights.xml", path.c_str(), version.Data()));
    files.push_back(Form("%s/data/EIDmva_EB2_10_%s_BDT.weights.xml", path.c_str(), version.Data()));
    files.push_back(Form("%s/data/EIDmva_EE_5_%s_BDT.weights.xml"  , path.c_str(), version.Data()));
    files.push_back(Form("%s/data/EIDmva_EB1_5_%s_BDT.weights.xml" , path.c_str(), version.Data()));
    files.push_back(Form("%s/data/EIDmva_EB2_5_%s_BDT.weights.xml" , path.c_str(), version.Data()));
  }
  else if (MVAversion == 80) {
    files.push_back(Form("%s/data/electronID_mva_Spring16_GeneralPurpose_V1_EE_10.weights.xml" , path.c_str()));
    files.push_back(Form("%s/data/electronID_mva_Spring16_GeneralPurpose_V1_EB1_10.weights.xml", path.c_str()));
    files.push_back(Form("%s/data/electronID_mva_Spring16_GeneralPurpose_V1_EB2_10.weights.xml", path.c_str()));
    files.push_back(Form("%s/data/electronID_mva_Spring16_HZZ_V1_EE_5.weights.xml"  , path.c_str())); // GP does not go below 10 GeV, so use HZZ
    files.push_back(Form("%s/data/electronID_mva_Spring16_HZZ_V1_EB1_5.weights.xml" , path.c_str()));
    files.push_back(Form("%s/data/electronID_mva_Spring16_HZZ_V1_EB2_5.weights.xml" , path.c_str()));
  } 

  //Loop over Files
  for (unsigned int i = 0; i < 6; i++){
    //order and spelling of variables matter
    //Add all variables to reader
    if (v25ns_) {
      if (MVAversion_ == 74) {
	readers[i]->AddVariable( "ele_oldsigmaietaieta" , &ele_oldsigmaietaieta_ );
	readers[i]->AddVariable( "ele_oldsigmaiphiiphi" , &ele_oldsigmaiphiiphi_ );
	readers[i]->AddVariable( "ele_oldcircularity"   , &ele_oldcircularity_   );
	readers[i]->AddVariable( "ele_oldr9"            , &ele_oldr9_            );
	readers[i]->AddVariable( "ele_scletawidth"      , &ele_scletawidth_      );
	readers[i]->AddVariable( "ele_sclphiwidth"      , &ele_sclphiwidth_      );
	readers[i]->AddVariable( "ele_he"               , &ele_he_               );
	if (i == 0 || i == 3){
	  readers[i]->AddVariable( "ele_psEoverEraw"      , &ele_psEoverEraw_      );
	}
	readers[i]->AddVariable( "ele_kfhits"           , &ele_kfhits_           );
	readers[i]->AddVariable( "ele_kfchi2"           , &ele_kfchi2_           );
	readers[i]->AddVariable( "ele_gsfchi2"          , &ele_chi2_hits_        );
	readers[i]->AddVariable( "ele_fbrem"            , &ele_fbrem_            );
	readers[i]->AddVariable( "ele_gsfhits"                       ,  &ele_gsfhits_                  );
	readers[i]->AddVariable( "ele_expected_inner_hits"           ,  &ele_expectedMissingInnerHits_ );
	readers[i]->AddVariable( "ele_conversionVertexFitProbability",  &ele_convVtxFitProbability_    );
	readers[i]->AddVariable( "ele_ep"               , &ele_ep_               );
	readers[i]->AddVariable( "ele_eelepout"         , &ele_eelepout_         );
	readers[i]->AddVariable( "ele_IoEmIop"          , &ele_IoEmIop_          );
	readers[i]->AddVariable( "ele_deltaetain"       , &ele_deltaetain_       );
	readers[i]->AddVariable( "ele_deltaphiin"       , &ele_deltaphiin_       );
	readers[i]->AddVariable( "ele_deltaetaseed"     , &ele_deltaetaseed_     );
	
	readers[i]->AddSpectator("ele_pT"               , &ele_pT_               );
	readers[i]->AddSpectator("ele_isbarrel"         , &ele_isbarrel_         );
	readers[i]->AddSpectator("ele_isendcap"         , &ele_isendcap_         );
	readers[i]->AddSpectator("scl_eta"              , &scl_eta_              );
	readers[i]->AddSpectator("ele_eClass",                 &dummySpectator_);
	readers[i]->AddSpectator("ele_pfRelIso",               &dummySpectator_);
	readers[i]->AddSpectator("ele_expected_inner_hits",    &dummySpectator_);
	readers[i]->AddSpectator("ele_vtxconv",                &dummySpectator_);
	readers[i]->AddSpectator("mc_event_weight",            &dummySpectator_);
	readers[i]->AddSpectator("mc_ele_CBmatching_category", &dummySpectator_);
      }
      else if (MVAversion_ == 80) {
	readers[i]->AddVariable( "ele_oldsigmaietaieta" , &ele_oldsigmaietaieta_ );
	readers[i]->AddVariable( "ele_oldsigmaiphiiphi" , &ele_oldsigmaiphiiphi_ );
	readers[i]->AddVariable( "ele_oldcircularity"   , &ele_oldcircularity_   );
	readers[i]->AddVariable( "ele_oldr9"            , &ele_oldr9_            );
	readers[i]->AddVariable( "ele_scletawidth"      , &ele_scletawidth_      );
	readers[i]->AddVariable( "ele_sclphiwidth"      , &ele_sclphiwidth_      );
	readers[i]->AddVariable( "ele_oldhe"            , &ele_oldhe_            ); 

	readers[i]->AddVariable( "ele_kfhits"           , &ele_kfhits_           );
	readers[i]->AddVariable( "ele_kfchi2"           , &ele_kfchi2_           );
	readers[i]->AddVariable( "ele_gsfchi2"          , &ele_chi2_hits_        );
	readers[i]->AddVariable( "ele_fbrem"            , &ele_fbrem_            );
	readers[i]->AddVariable( "ele_gsfhits"                       ,  &ele_gsfhits_                  );
	readers[i]->AddVariable( "ele_expected_inner_hits"           ,  &ele_expectedMissingInnerHits_ );
	readers[i]->AddVariable( "ele_conversionVertexFitProbability",  &ele_convVtxFitProbability_    );
	readers[i]->AddVariable( "ele_ep"               , &ele_ep_               );
	readers[i]->AddVariable( "ele_eelepout"         , &ele_eelepout_         );
	readers[i]->AddVariable( "ele_IoEmIop"          , &ele_IoEmIop_          );
	readers[i]->AddVariable( "ele_deltaetain"       , &ele_deltaetain_       );
	readers[i]->AddVariable( "ele_deltaphiin"       , &ele_deltaphiin_       );
	readers[i]->AddVariable( "ele_deltaetaseed"     , &ele_deltaetaseed_     );
	if (i < 3) { // high pT GP MVA requires pt/eta
	  readers[i]->AddVariable( "ele_pt"               , &ele_pT_               );
	  readers[i]->AddVariable( "scl_eta"              , &scl_eta_              );
	}

	if (i == 0 || i == 3){
	  readers[i]->AddVariable( "ele_psEoverEraw"      , &ele_psEoverEraw_      );
	}
	
	if (i >= 3) { // low pT HZZ MVA requires spectator variables 
	  readers[i]->AddSpectator("ele_pt"               , &ele_pT_               );
	  readers[i]->AddSpectator("ele_isEE"             , &ele_isendcap_         );
	  readers[i]->AddSpectator("ele_isEB"             , &ele_isbarrel_         );
	  readers[i]->AddSpectator("ele_isEBEtaGap"       , &ele_isendcap_        );
	  readers[i]->AddSpectator("ele_isEBPhiGap"       , &ele_isendcap_        );
	  readers[i]->AddSpectator("ele_isEBEEGap"        , &ele_isendcap_        );
	  readers[i]->AddSpectator("ele_isEERingGap"      , &ele_isendcap_        );
	  readers[i]->AddSpectator("ele_isEEDeeGap"       , &ele_isendcap_        );
	  readers[i]->AddSpectator("ele_isEE"             , &ele_isendcap_         );
	  readers[i]->AddSpectator("scl_eta"              , &scl_eta_              );
	  readers[i]->AddSpectator("ele_eClass"           , &dummySpectator_);
	  readers[i]->AddSpectator("mc_ele_matchedFromCB" , &dummySpectator_);
	}
	
      }
    } else {
      //Add all variables to reader
      readers[i]->AddVariable( "ele_kfhits"           , &ele_kfhits_           );
      readers[i]->AddVariable( "ele_oldsigmaietaieta" , &ele_oldsigmaietaieta_ );
      readers[i]->AddVariable( "ele_oldsigmaiphiiphi" , &ele_oldsigmaiphiiphi_ );
      readers[i]->AddVariable( "ele_oldcircularity"   , &ele_oldcircularity_   );
      readers[i]->AddVariable( "ele_oldr9"            , &ele_oldr9_            );
      readers[i]->AddVariable( "ele_scletawidth"      , &ele_scletawidth_      );
      readers[i]->AddVariable( "ele_sclphiwidth"      , &ele_sclphiwidth_      );
      readers[i]->AddVariable( "ele_he"               , &ele_he_               );
      if (i == 0 || i == 3){
	readers[i]->AddVariable( "ele_psEoverEraw"      , &ele_psEoverEraw_      );
      }
      readers[i]->AddVariable( "ele_kfchi2"           , &ele_kfchi2_           );
      readers[i]->AddVariable( "ele_chi2_hits"        , &ele_chi2_hits_        );
      readers[i]->AddVariable( "ele_fbrem"            , &ele_fbrem_            );
      readers[i]->AddVariable( "ele_ep"               , &ele_ep_               );
      readers[i]->AddVariable( "ele_eelepout"         , &ele_eelepout_         );
      readers[i]->AddVariable( "ele_IoEmIop"          , &ele_IoEmIop_          );
      readers[i]->AddVariable( "ele_deltaetain"       , &ele_deltaetain_       );
      readers[i]->AddVariable( "ele_deltaphiin"       , &ele_deltaphiin_       );
      readers[i]->AddVariable( "ele_deltaetaseed"     , &ele_deltaetaseed_     );
      readers[i]->AddSpectator("ele_pT"               , &ele_pT_               );
      readers[i]->AddSpectator("ele_isbarrel"         , &ele_isbarrel_         );
      readers[i]->AddSpectator("ele_isendcap"         , &ele_isendcap_         );
      readers[i]->AddSpectator("scl_eta"              , &scl_eta_              );
    }
    

    // Debugging
    // std::cout<<"About to call it on file "<<files[i]<<endl;
    // readers[i]->SetVerbose(true);
    // vector<TString> varList = readers[i]->DataInfo().GetListOfVariables();
    // for (int j=0;j<varList.size();j++)cout<<varList[j]<<endl;
    // std::cout<<"N Spectators "<<readers[i]->DataInfo().GetNSpectators()<<std::endl;
    // std::vector<TMVA::VariableInfo> specInfo = readers[i]->DataInfo().GetSpectatorInfos();
    // for(int j=0;j<specInfo.size();j++) std::cout<< specInfo[j].GetExpression() <<std::endl;

    //Book MVA
    readers[i]->BookMVA("BDT", files[i]);
  }

}

float readMVA::MVA(unsigned int index){

  if (elIDCacheSet) return elID_cache.getMVA(index);

  //Load all variables from tree
  ele_kfhits_           = tas::els_ckf_laywithmeas().at(index);
  ele_oldsigmaietaieta_ = tas::els_sigmaIEtaIEta_full5x5().at(index); 
  ele_oldsigmaiphiiphi_ = tas::els_sigmaIPhiIPhi_full5x5().at(index);
  ele_oldcircularity_   = 1.0 - tas::els_e1x5_full5x5().at(index)/tas::els_e5x5_full5x5().at(index); 
  ele_oldr9_            = tas::els_r9_full5x5().at(index); 
  ele_scletawidth_      = tas::els_etaSCwidth().at(index);
  ele_sclphiwidth_      = tas::els_phiSCwidth().at(index);
  ele_he_               = tas::els_hOverE().at(index);
  // this is hack to calculate full5x5_hcalOverEcal(), which we are missing in CMS3
  ele_oldhe_            = tas::els_hOverE().at(index) * tas::els_ecalEnergy().at(index) / tas::els_eSC().at(index); 
  ele_psEoverEraw_      = tas::els_eSCPresh().at(index)/tas::els_eSCRaw().at(index);
  ele_kfchi2_           = tas::els_ckf_chi2().at(index)/tas::els_ckf_ndof().at(index);
  ele_chi2_hits_        = tas::els_chi2().at(index)/tas::els_ndof().at(index);
  ele_fbrem_            = tas::els_fbrem().at(index);
  ele_ep_               = tas::els_eOverPIn().at(index);
  ele_eelepout_         = tas::els_eOverPOut().at(index);
  ele_IoEmIop_          = (tas::els_ecalEnergy().at(index) != 0 && tas::els_p4In().at(index).P() != 0) ? 1.0/tas::els_ecalEnergy().at(index) - 1.0/tas::els_p4In().at(index).P() : 999999;
  ele_deltaetain_       = fabs(tas::els_dEtaIn().at(index));
  ele_deltaphiin_       = fabs(tas::els_dPhiIn().at(index));
  ele_deltaetaseed_     = fabs(tas::els_dEtaOut().at(index));
  ele_pT_               = tas::els_p4().at(index).pt(); 
  ele_isbarrel_         = fabs(tas::els_etaSC().at(index)) < 1.479 ? 1 : 0; 
  ele_isendcap_         = fabs(tas::els_etaSC().at(index)) > 1.479 ? 1 : 0; 
  scl_eta_              = tas::els_etaSC().at(index); 
  //additional variables for 25ns version    
  if (v25ns_) {
    ele_gsfhits_                  = tas::els_nlayers().at(index);
    ele_expectedMissingInnerHits_ = tas::els_exp_innerlayers().at(index);
    ele_convVtxFitProbability_    = tas::els_conv_vtx_prob().at(index);
  }
  dummySpectator_ = 999;

  //bindVariables  
  if(ele_fbrem_ < -1.) ele_fbrem_ = -1.;
  ele_deltaetain_ = fabs(ele_deltaetain_);
  if(ele_deltaetain_ > 0.06) ele_deltaetain_ = 0.06;
  ele_deltaphiin_ = fabs(ele_deltaphiin_);
  if(ele_deltaphiin_ > 0.6) ele_deltaphiin_ = 0.6;
  if(ele_ep_ > 20.) ele_ep_ = 20.;
  if(ele_eelepout_ > 20.) ele_eelepout_ = 20.;
  ele_deltaetaseed_ = fabs(ele_deltaetaseed_);
  if(ele_deltaetaseed_ > 0.2) ele_deltaetaseed_ = 0.2;
  if(ele_oldcircularity_ < -1.) ele_oldcircularity_ = -1;
  if(ele_oldcircularity_ > 2.) ele_oldcircularity_ = 2.; 
  if(ele_oldr9_ > 5) ele_oldr9_ = 5;
  if(ele_chi2_hits_ > 200.) ele_chi2_hits_ = 200;
  if(ele_kfchi2_ > 10.) ele_kfchi2_ = 10.;
  if(std::isnan(ele_oldsigmaiphiiphi_)) ele_oldsigmaiphiiphi_ = 0.;

  float disc = -1.0;
  if (ele_pT_ >= 10.0){
    if      (fabs(scl_eta_) < 0.8) disc = readers[1]->EvaluateMVA( "BDT" );
    else if (fabs(scl_eta_) <= 1.479 && fabs(scl_eta_) >= 0.8) disc = readers[2]->EvaluateMVA( "BDT" );
    else  disc = readers[0]->EvaluateMVA( "BDT" );
  }
  else if (ele_pT_ >= 5.0){
    if      (fabs(scl_eta_) < 0.8) disc = readers[4]->EvaluateMVA( "BDT" );
    else if (fabs(scl_eta_) <= 1.479 && fabs(scl_eta_) >= 0.8) disc = readers[5]->EvaluateMVA( "BDT" );
    else  disc = readers[3]->EvaluateMVA( "BDT" );
  }

  return disc;

}

bool readMVA::passesElectronMVAid(unsigned int index, id_level_t id_level){
  float disc = use_miniaod_ ?  els_VIDNonTrigMvaValue().at(index) : MVA(index);
  // careful, the MVA(index) call sets value of scl_eta_, so we must directly use the branch here in case use_miniaod_ is true
  float aeta = fabs(tas::els_etaSC().at(index));
  float pt = tas::els_p4().at(index).pt();

  // for morioned MVA WPs: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SUSLeptonSF#ID_IP_ISO_AN1
  // returns A if pt<ptmin, B if pt>ptmax, and linear interpolation between. if pt<10, use C
  float ptmin = 15;
  float ptmax = 25;
  auto mvacut = [ptmin,ptmax](float A, float B, float C, float pt_) {
      return pt_>10 ? std::min(A, std::max(B,A + (B-A)/(ptmax-ptmin)*(pt_-ptmin))) : C; 
  };

  switch (id_level){
  case(SS_veto_noiso_v2):
    if (aeta < 0.8) return disc > 0.35;
    if ((aeta >= 0.8 && aeta <= 1.479)) return disc > 0.20;
    if (aeta > 1.479) return disc > -0.52;
    break;

  case(SS_veto_noiso_v3):
  case (SS_veto_noiso_noip_v3):
  case(SS_fo_looseMVA_noiso_v3):
  case (SS_fo_looseMVA_noiso_noip_v3):
  case (SS_medium_looseMVA_noip_v3):
    if (aeta < 0.8) return disc > -0.11;
    if ((aeta >= 0.8 && aeta <= 1.479)) return disc > -0.35;
    if (aeta > 1.479) return disc > -0.55;
    break;

  case(SS_fo_noiso_v2):
  case(SS_medium_noiso_v2):
  case(SS_fo_noiso_v3):
  case(SS_medium_noiso_v3):
  case(SS_medium_noip_v3):
    if (aeta < 0.8) return disc > 0.73;
    if ((aeta >= 0.8 && aeta <= 1.479)) return disc > 0.57;
    if (aeta > 1.479) return disc > 0.05;
    break;

  case(SS_veto_noiso_v4):
  case (SS_veto_noiso_noip_v4):
  case(SS_veto_noiso_v5):
  case (SS_veto_noiso_noip_v5):
    return true;
    break;

  case (RA7_el_loose):
  case (SS_medium_noiso_no3chg_v5):
  case (SS_medium_no3chg_v5):
  case (SS_fo_looseMVA_noiso_noip_no3chg_v5):
  case (SS_fo_looseMVA_noiso_no3chg_v5):
  case (SS_fo_looseMVA_no3chg_v5):
  case(SS_fo_looseMVA_noiso_v4):
  case (SS_fo_looseMVA_noiso_noip_v4):
  case(SS_fo_looseMVA_noiso_v5):
  case (SS_fo_looseMVA_noiso_noip_v5): // VLooseFOIDEmu
    // if (aeta < 0.8) return disc > -0.70;
    // if ((aeta >= 0.8 && aeta <= 1.479)) return disc > -0.83;
    // if (aeta > 1.479) return disc > -0.92;
    if (aeta < 0.8) return disc > mvacut(-0.86,-0.96,-0.3,pt);
    if ((aeta >= 0.8 && aeta <= 1.479)) return disc > mvacut(-0.85,-0.96,-0.36,pt);
    if (aeta > 1.479) return disc > mvacut(-0.81,-0.95,-0.63,pt);
    break;


  case (RA7_el_tight):
  case (SS_fo_noiso_v4):
  case (SS_fo_v4):
  case(SS_medium_noiso_v4):
  case(SS_medium_noip_v4):
  case (SS_fo_noiso_v5):
  case (SS_fo_noiso_no3chg_v5):
  case (SS_fo_v5):
  case(SS_medium_noiso_v5):
  case(SS_medium_noip_v5): // Tight
    // if (aeta < 0.8) return disc > 0.87;
    // if ((aeta >= 0.8 && aeta <= 1.479)) return disc > 0.60;
    // if (aeta > 1.479) return disc > 0.17;
    if (aeta < 0.8) return disc > mvacut(0.77,0.52,0.77,pt);
    if ((aeta >= 0.8 && aeta <= 1.479)) return disc > mvacut(0.56,0.11,0.56,pt);
    if (aeta > 1.479) return disc > mvacut(0.48,-0.01,0.48,pt);
    break;

  case (WW_veto_noiso_v1):
  case (WW_veto_noiso_noip_v1):
  case (WW_fo_looseMVA_noiso_v1):
  case (WW_fo_looseMVA_noiso_noip_v1):
  case (WW_medium_looseMVA_noip_v1):
    if (aeta < 0.8) return disc > -0.11;
    if ((aeta >= 0.8 && aeta <= 1.479)) return disc > -0.35;
    if (aeta > 1.479) return disc > -0.55;
    break;

  case(WW_fo_noiso_v1):
  case(WW_medium_noiso_v1):
  case(WW_medium_noip_v1):
    if (aeta < 0.8) return disc > 0.73;
    if ((aeta >= 0.8 && aeta <= 1.479)) return disc > 0.57;
    if (aeta > 1.479) return disc > 0.05;
    break;

  default:
    cout << "WARNING: bad id level " << id_level << endl;
    break;
  }
  return -99999;

}

void createAndInitMVA(std::string pathToCORE, bool v25ns, bool use_miniaod, int MVAversion){
  if (MVAversion!=74 && MVAversion !=80) {
    cout<< "Illegal version number for MVA in ElectronSelection "<< MVAversion << endl;
    return;
  }
  globalEleMVAreader = new readMVA();
  globalEleMVAreader->InitMVA(pathToCORE, v25ns, use_miniaod, MVAversion); 
}

float getMVAoutput(unsigned int index, bool use_miniaod){
  if (globalEleMVAreader==0) {
    cout << "readMVA=0, please create and init it (e.g with createAndInitMVA function)" << endl;
    return -999.;
  }
  return use_miniaod ?  els_VIDNonTrigMvaValue().at(index) : globalEleMVAreader->MVA(index);
}

bool isTriggerSafenoIso_v1(unsigned int elIdx) {

  if (fabs(els_etaSC().at(elIdx)) <= 1.479) {
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.011) return false;
    if (els_hOverE().at(elIdx) >= 0.08) return false;
    if (fabs(els_dEtaIn().at(elIdx)) >= 0.01) return false;
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.04) return false;
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.01) return false;
  } else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)) {
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.031) return false;
    if (els_hOverE().at(elIdx) >= 0.08) return false;
    if (fabs(els_dEtaIn().at(elIdx)) >= 0.01) return false;
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.08) return false;
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.01) return false;
  }
  return true;
}

bool isTriggerSafeRA7(unsigned int elIdx) {

  if (fabs(els_etaSC().at(elIdx)) <= 1.479) {
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.011) return false;
    if (els_hOverE().at(elIdx) >= 0.1) return false;
    if (fabs(els_dEtaIn().at(elIdx)) >= 0.01) return false;
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.04) return false;
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.01) return false;
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) < -0.05) return false;
  } else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)) {
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.03) return false;
    if (els_hOverE().at(elIdx) >= 0.07) return false;
    if (fabs(els_dEtaIn().at(elIdx)) >= 0.008) return false;
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.07) return false;
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.005) return false;
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) < -0.05) return false;
  }
  return true;

}

bool isTriggerSafenoIso_v2(unsigned int elIdx) {

  if (fabs(els_etaSC().at(elIdx)) <= 1.479) {
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.011) return false;
    if (els_hOverE().at(elIdx) >= 0.060) return false;
    if (fabs(els_dEtaOut().at(elIdx)) >= 0.004) return false;
    if (fabs(els_dPhiIn().at(elIdx)) >= 0.020) return false;
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.013) return false;
  } else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)) {
    if (els_sigmaIEtaIEta_full5x5().at(elIdx) >= 0.031) return false;
    if (els_hOverE().at(elIdx) >= 0.060) return false;
    if (fabs( (1.0/els_ecalEnergy().at(elIdx)) - (els_eOverPIn().at(elIdx)/els_ecalEnergy().at(elIdx)) ) >= 0.013) return false;
    if (fabs(els_chi2().at(elIdx)) >= 3) return false;
  }
  return true;
}

bool isTriggerSafe_v1(unsigned int elIdx) {

  if (!isTriggerSafenoIso_v1(elIdx)) return false;

  if (els_ecalPFClusterIso().at(elIdx)/els_p4().at(elIdx).pt() >= 0.45) return false;
  if (els_hcalPFClusterIso().at(elIdx)/els_p4().at(elIdx).pt() >= 0.25) return false;
  if (els_tkIso().at(elIdx)/els_p4().at(elIdx).pt() >= 0.2) return false;

  return true;

}

bool isTriggerSafe_v2(unsigned int elIdx) {

  if (!isTriggerSafenoIso_v2(elIdx)) return false;

  if (fabs(els_etaSC().at(elIdx)) <= 1.479) {
      
    if ((els_ecalPFClusterIso().at(elIdx)-evt_fixgridfastjet_allcalo_rho()*0.165)/els_p4().at(elIdx).pt() >= 0.160) return false;
    if ((els_hcalPFClusterIso().at(elIdx)-evt_fixgridfastjet_allcalo_rho()*0.060)/els_p4().at(elIdx).pt() >= 0.120) return false;
    if (els_tkIso().at(elIdx)/els_p4().at(elIdx).pt() >= 0.080) return false;

  } else if ((fabs(els_etaSC().at(elIdx)) > 1.479) && (fabs(els_etaSC().at(elIdx)) < 2.5)) {

    if ((els_ecalPFClusterIso().at(elIdx)-evt_fixgridfastjet_allcalo_rho()*0.132)/els_p4().at(elIdx).pt() >= 0.120) return false;
    if ((els_hcalPFClusterIso().at(elIdx)-evt_fixgridfastjet_allcalo_rho()*0.131)/els_p4().at(elIdx).pt() >= 0.120) return false;
    if (els_tkIso().at(elIdx)/els_p4().at(elIdx).pt() >= 0.080) return false;

  }
  
  return true;

}
