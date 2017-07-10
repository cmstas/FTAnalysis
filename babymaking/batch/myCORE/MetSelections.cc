#include <iostream>

#include "MetSelections.h"

#include "Math/VectorUtil.h"
#include "MetSelections.h"

#include "Tools/JetCorrector.h"
#include "Tools/jetcorr/FactorizedJetCorrector.h"
#include "Tools/jetcorr/JetCorrectionUncertainty.h"

#include "Tools/jetcorr/Utilities.icc"
#include "Tools/jetcorr/JetCorrectionUncertainty.icc"
#include "Tools/jetcorr/SimpleJetCorrectionUncertainty.icc"


using namespace tas;

metStruct trackerMET(float deltaZCut, const std::vector<LorentzVector>* jets) {

  if ( vtxs_isFake().empty() ) return metStruct();
  double pX(0), pY(0);
  
  for (unsigned int i=0; i<pfcands_particleId().size(); ++i){
    if ( pfcands_charge().at(i)==0 ) continue;
    if ( jets ){
      bool matched = false;
      for ( std::vector<LorentzVector>::const_iterator jet = jets->begin(); jet != jets->end(); ++jet )
    	if ( fabs(ROOT::Math::VectorUtil::DeltaR(pfcands_p4().at(i),*jet))<0.5 ) matched=true;
      if (matched) continue;
    }
    
    if ( fabs(pfcands_dz().at(i)) > deltaZCut) continue;
    
    pX -= pfcands_p4().at(i).px();
    pY -= pfcands_p4().at(i).py();
  }
  
  if (jets){
    for ( std::vector<LorentzVector>::const_iterator jet = jets->begin(); jet != jets->end(); ++jet ){
      pX -= jet->px();
      pY -= jet->py();
    }
  }
  metStruct met;
  met.met     = sqrt(pX * pX + pY * pY);
  met.metphi  = atan2(pY, pX);
  met.metx = pX;
  met.mety = pY;
  return met;
}

bool hbheNoiseFilter(int minZeros) {
  // http://cmslxr.fnal.gov/lxr/source/CommonTools/RecoAlgos/plugins/HBHENoiseFilterResultProducer.cc?v=CMSSW_7_4_1
  // by default (false --> reject), maxZeros() cut will never cause a lost event
  if(hcalnoise_maxHPDHits()>=17) return false;
  if(hcalnoise_maxHPDNoOtherHits()>=10) return false;
  if(hcalnoise_maxZeros()>=minZeros) return false;
  if(hcalnoise_HasBadRBXTS4TS5()) return false;
  return true;
}

bool hbheNoiseFilter_25ns(int minZeros) {
  // http://cmslxr.fnal.gov/lxr/source/CommonTools/RecoAlgos/plugins/HBHENoiseFilterResultProducer.cc?v=CMSSW_7_4_1
  // by default (false --> reject), maxZeros() cut will never cause a lost event
  if(hcalnoise_maxHPDHits()>=17) return false;
  if(hcalnoise_maxHPDNoOtherHits()>=10) return false;
  if(hcalnoise_maxZeros()>=minZeros) return false;
  if(hcalnoise_HasBadRBXRechitR45Loose()) return false;
  return true;
}

bool hbheIsoNoiseFilter() {
  // false = reject event
  if(hcalnoise_numIsolatedNoiseChannels()>=10) return false;
  if(hcalnoise_isolatedNoiseSumE()>=50.0) return false;
  if(hcalnoise_isolatedNoiseSumEt()>=25.0) return false;
  return true;
}

bool badMuonFilter() {
  // false = reject event
  // based on https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#snippet_on_how_to_flag_the_charg

  float minMuonTrackRelErr = 0.5;
  float minMuonPt = 100.0;
  float maxDR = 0.001;
  int suspiciousAlgo = 14;
  bool flagged = false;

  for (unsigned int imu = 0; imu < cms3.mus_p4().size(); imu++) {
    bool foundBadTrack = false;

    if(cms3.mus_trk_p4().at(imu).pt() < minMuonPt) continue;

    LorentzVector trk_p4 = cms3.mus_trk_p4().at(imu);
    float trk_pterr = cms3.mus_ptErr().at(imu);

    // reco::TrackBase::highPurity is bit 2...skip muon if inner track is high purity
    if( (cms3.mus_qualityMask().at(imu) & (1 << 2))>>2 ) continue;
        
    // Consider only muons with large relative pt error
    if(!(trk_pterr/trk_p4.pt() > minMuonTrackRelErr) ) continue;

    if(cms3.mus_algo().at(imu) == suspiciousAlgo && cms3.mus_algoOrig().at(imu) == suspiciousAlgo) foundBadTrack = true;

    if(foundBadTrack) {
      for (unsigned int icand = 0; icand < pfcands_p4().size(); icand++){

        int pdgId = pfcands_particleId().at(icand);
        LorentzVector cand_p4 = pfcands_p4().at(icand);

        if(cand_p4.pt() < minMuonPt) continue;
        if(abs(pdgId) != 13) continue;

        if (ROOT::Math::VectorUtil::DeltaR(cms3.mus_p4().at(imu), cand_p4) < maxDR) {
          flagged = true;
          break;
        }
      } // loop over pfcands

    }

    if(flagged) break;

  } // loop over muons

  return !flagged;

}

bool badMuonFilterV2() {
  // false = reject event
  // based on https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#snippet_on_how_to_flag_the_charg
  const float minMuonSegCompatibility = 0.3;
  const float maxMuonBestTrackRelErr = 2.;
  const float maxMuonInnerTrackRelErr = 1.;
  const float minMuonPt = 100.0;
  const float maxDR = 0.001;
  const int suspiciousAlgo = 14; // muonSeededStepOutIn

  for (unsigned int imu = 0; imu < cms3.mus_p4().size(); imu++) {

    if ((cms3.mus_p4().at(imu).pt() <= minMuonPt) && (cms3.mus_trk_p4().at(imu).pt() <= minMuonPt)) continue;
    if (cms3.mus_algo().at(imu) != suspiciousAlgo) continue;
    if (cms3.mus_algoOrig().at(imu) != suspiciousAlgo) continue;    
    if (((cms3.mus_type().at(imu)) & (1<<1)) == 0) continue; // require muon is global
    
    bool fails_seg_compatibility = (cms3.mus_segmCompatibility().at(imu) < minMuonSegCompatibility);
    bool fails_best_track_ptrelerr =  (cms3.mus_bfit_ptErr().at(imu)/cms3.mus_bfit_p4().at(imu).pt() > maxMuonBestTrackRelErr);
    bool fails_inner_track_ptrelerr = (cms3.mus_ptErr().at(imu)/cms3.mus_trk_p4().at(imu).pt() > maxMuonInnerTrackRelErr);

    if (fails_seg_compatibility || fails_best_track_ptrelerr || fails_inner_track_ptrelerr) {
      for (unsigned int icand = 0; icand < pfcands_p4().size(); icand++) {
        
        int pdgId = pfcands_particleId().at(icand);
        LorentzVector cand_p4 = pfcands_p4().at(icand);

        if (abs(pdgId) != 13) continue;
        if (cand_p4.pt() <= minMuonPt) continue;

        if ((ROOT::Math::VectorUtil::DeltaR(cms3.mus_p4().at(imu), cand_p4) >= maxDR)) continue;

        return false;
      } // loop over pfcands    
    }
  } // loop over reco muons

  return true;
}


bool badChargedCandidateFilter() {
  // false = reject event
  // based on https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#snippet_on_how_to_flag_the_charg
  float minMuonTrackRelErr = 0.5;
  float minMuonPt = 100.0;
  float maxDR = 0.001;
  float minPtDiffRel = -0.5;
  for (unsigned int imu = 0; imu < cms3.mus_p4().size(); imu++)
  {
    if(cms3.mus_p4().at(imu).pt() < minMuonPt) continue;

    LorentzVector trk_p4 = cms3.mus_trk_p4().at(imu);
    float trk_pterr = cms3.mus_ptErr().at(imu);

    // reco::TrackBase::highPurity is bit 2...skip muon if inner track is high purity
    if( (cms3.mus_qualityMask().at(imu) & (1 << 2))>>2 ) continue;

    // Consider only muons with large relative pt error
    if(!(trk_pterr/trk_p4.pt() > minMuonTrackRelErr) ) continue;

    for (unsigned int icand = 0; icand < pfcands_p4().size(); icand++){

      int pdgId = pfcands_particleId().at(icand);
      LorentzVector cand_p4 = pfcands_p4().at(icand);

      if(abs(pdgId) != 211) continue;

      if ((ROOT::Math::VectorUtil::DeltaR(trk_p4, cand_p4) < maxDR) &&
          (cand_p4.pt()-trk_p4.pt())/(0.5*(cand_p4.pt()+trk_p4.pt())) > minPtDiffRel) {
        return false;
      }

    }
  }

  return true;
}

bool badChargedCandidateFilterV2() {
  // false = reject event
  // based on https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#snippet_on_how_to_flag_the_charg
  // based on https://github.com/cms-sw/cmssw/blob/bacc91ec51a32014b3b6bff66dbe92a51c7c266e/RecoMET/METFilters/plugins/BadChargedCandidateFilter.cc
  const float minMuonSegCompatibility = 0.3;
  const float maxMuonBestTrackRelErr = 2.;
  const float maxMuonInnerTrackRelErr = 1.;
  const float minMuonPt = 100.0;
  const float maxDR = 0.00001;
  const float maxPtDiffRel = 0.00001;
    
  for (unsigned int imu = 0; imu < cms3.mus_p4().size(); imu++) {
    if ((cms3.mus_p4().at(imu).pt() <= minMuonPt) && (cms3.mus_trk_p4().at(imu).pt() <= minMuonPt)) continue;
    if (((cms3.mus_type().at(imu)) & (1<<1)) == 0) continue; // require muon is global
    if (cms3.mus_pid_PFMuon().at(imu)) continue; // muon must fail PF
    
    bool fails_seg_compatibility = (cms3.mus_segmCompatibility().at(imu) < minMuonSegCompatibility);
    bool fails_best_track_ptrelerr =  (cms3.mus_bfit_ptErr().at(imu)/cms3.mus_bfit_p4().at(imu).pt() > maxMuonBestTrackRelErr);
    bool fails_inner_track_ptrelerr = (cms3.mus_ptErr().at(imu)/cms3.mus_trk_p4().at(imu).pt() > maxMuonInnerTrackRelErr);

    if (fails_seg_compatibility || fails_best_track_ptrelerr || fails_inner_track_ptrelerr) {
      for (unsigned int icand = 0; icand < cms3.pfcands_p4().size(); icand++) {
        
        int pdgId = cms3.pfcands_particleId().at(icand);
        LorentzVector cand_p4 = cms3.pfcands_p4().at(icand);
        LorentzVector trk_p4 = cms3.mus_trk_p4().at(imu);
        
        if (abs(pdgId) != 211) continue;
        
        if ((ROOT::Math::VectorUtil::DeltaR(trk_p4, cand_p4) >= maxDR)) continue;
        
        float diffPt = cand_p4.pt() - trk_p4.pt();
        float avgPt = 0.5 * (cand_p4.pt() + trk_p4.pt());

        if (fabs(diffPt)/avgPt >= maxPtDiffRel) continue;

        return false;
      } // loop over pfcands    
    } 
  } // loop over reco muons

  return true;
}

// takes in an already initialized FactorizedJetCorrector object
// and returns T1 Corrected MET using the CHS jet collection
// NOTE: option for unclustered uncertainty is NOT official, just a guess.  Use 1 for UP, -1 for DOWN
pair <float, float> getT1CHSMET( FactorizedJetCorrector * jet_corrector, JetCorrectionUncertainty* jecUnc, bool uncUp, int doUnclusteredUnc ){
  float T1_met    = cms3.evt_METToolbox_pfmet_raw();
  float T1_metPhi = cms3.evt_METToolbox_pfmetPhi_raw();
  float T1_metx   = T1_met * cos(T1_metPhi);
  float T1_mety   = T1_met * sin(T1_metPhi);
  float unclustered_metx   = T1_met * cos(T1_metPhi);
  float unclustered_mety   = T1_met * sin(T1_metPhi);

  //Run over same jets that were produced with MET tools
  for(unsigned int iJet = 0; iJet < cms3.pfjets_METToolbox_p4().size(); iJet++){

    // // get uncorrected jet p4 to use as input for corrections
    LorentzVector jetp4_uncorr = cms3.pfjets_METToolbox_p4().at(iJet);		  
    float emfrac = (cms3.pfjets_METToolbox_chargedEmE().at(iJet) + cms3.pfjets_METToolbox_neutralEmE().at(iJet)) / jetp4_uncorr.E();

    if (emfrac > 0.9                  ) continue; // veto events with EM fraction > 0.9
    if( abs(jetp4_uncorr.eta()) > 9.9 ) continue; // veto jets with eta > 9.9

    // get L1FastL2L3 total correction
    jet_corrector->setRho   ( cms3.evt_fixgridfastjetMETTools_all_rho()      );
    jet_corrector->setJetA  ( cms3.pfjets_METToolbox_area().at(iJet) );
    jet_corrector->setJetPt ( jetp4_uncorr.pt()                      );
    jet_corrector->setJetEta( jetp4_uncorr.eta()                     );

    //Note the subcorrections are stored with corr_vals(N) = corr(N)*corr(N-1)*...*corr(1)
    vector<float> corr_vals = jet_corrector->getSubCorrections();

    double corr             = corr_vals.at(corr_vals.size()-1); // All corrections
    double corr_l1          = corr_vals.at(0);                  // offset correction
		  
    if (jecUnc != 0 && fabs(jetp4_uncorr.eta()) < 5.4) {
      jecUnc->setJetEta(jetp4_uncorr.eta()); 
      jecUnc->setJetPt(jetp4_uncorr.pt()*corr); 
      double unc = jecUnc->getUncertainty(true);
      if (uncUp) corr=corr*(1+unc);
      else  corr=corr*(1-unc);
    }
	  
    //	
    // remove SA or global muons from jets before correcting
    //
    // for (unsigned int imu = 0; imu < cms3.mus_p4().size(); imu++)
    //   {
    // 	int index = cms3.mus_pfidx().at(imu);
    // 	if (index < 0) continue;
    // 	bool is_global     = !(((cms3.mus_type().at(imu)) & (1<<1)) == 0);
    // 	bool is_standalone = !(((cms3.mus_type().at(imu)) & (1<<3)) == 0);
    // 	if (!(is_global || is_standalone)) continue;            
    // 	if (std::find(cms3.pfjets_METToolbox_pfcandIndicies().at(iJet).begin(),
    // 				  cms3.pfjets_METToolbox_pfcandIndicies().at(iJet).end(), index) == cms3.pfjets_METToolbox_pfcandIndicies().at(iJet).end()) continue;
    // 	jetp4_uncorr -= cms3.pfcands_p4()   .at(index);
    //   }

    // Alternative way to do muon corrections, done by MET group
    for (unsigned int pfcind = 0; pfcind < cms3.pfjets_METToolbox_pfcandIndicies().at(iJet).size(); pfcind++){
      int index = cms3.pfjets_METToolbox_pfcandIndicies().at(iJet).at(pfcind);
      if( cms3.pfcands_isGlobalMuon()    .at(index) ||
          cms3.pfcands_isStandAloneMuon().at(index)){
        jetp4_uncorr -= cms3.pfcands_p4()   .at(index);
      }
    }
			  
    if (corr * jetp4_uncorr.pt() > 10.){		  
      T1_metx += jetp4_uncorr.px() * ( corr_l1 - corr );
      T1_mety += jetp4_uncorr.py() * ( corr_l1 - corr );
      unclustered_metx += jetp4_uncorr.px();
      unclustered_mety += jetp4_uncorr.py();
    }

  }
	  
  if (doUnclusteredUnc != 0) {
    T1_metx -= unclustered_metx * 0.10 * (float)doUnclusteredUnc; // using 10% from 8 TeV
    T1_mety -= unclustered_mety * 0.10 * (float)doUnclusteredUnc; // using 10% from 8 TeV
  }
	  
  T1_met    = std::sqrt(pow(T1_metx, 2) + pow(T1_mety, 2));
  T1_metPhi = std::atan2(T1_mety, T1_metx);

  return make_pair(T1_met, T1_metPhi);
}
// takes in an already initialized FactorizedJetCorrector object
// and returns T1 Corrected MET using the CHS jet collection from miniAOD
pair <float, float> getT1CHSMET_fromMINIAOD( FactorizedJetCorrector * jet_corrector, JetCorrectionUncertainty* jecUnc, bool uncUp, bool recompute_raw_met , int use_cleaned_met ){

  float T1_met    = cms3.evt_pfmet_raw();
  float T1_metPhi = cms3.evt_pfmetPhi_raw();
  //use option use_cleaned_met to select alternate met collections
  if (use_cleaned_met == 1) {
    //MuonEG cleaned MET
    T1_met    = cms3.evt_muegclean_pfmet_raw();
    T1_metPhi = cms3.evt_muegclean_pfmetPhi_raw();
  }
  float T1_metx   = T1_met * cos(T1_metPhi);
  float T1_mety   = T1_met * sin(T1_metPhi);
  
  if( recompute_raw_met ){
    LorentzVector met_raw_OTF(0,0,0,0);
    for( size_t pfind = 0; pfind < cms3.pfcands_p4().size(); pfind++ ){
      met_raw_OTF -= cms3.pfcands_p4().at(pfind);
    }
    T1_met    = met_raw_OTF.pt();
    T1_metPhi = met_raw_OTF.phi();
    T1_metx   = T1_met * cos(T1_metPhi);
    T1_mety   = T1_met * sin(T1_metPhi);
  }

  LorentzVector jetp4_unshift_vsum(0,0,0,0);
  LorentzVector jetp4_shifted_vsum(0,0,0,0);
  
  for(unsigned int iJet = 0; iJet < cms3.pfjets_p4().size(); iJet++){

    LorentzVector jetp4_uncorr = cms3.pfjets_p4().at(iJet)*cms3.pfjets_undoJEC().at(iJet);

    // get L1FastL2L3 total correction
    jet_corrector->setRho   ( cms3.evt_fixgridfastjet_all_rho()      );
    jet_corrector->setJetA  ( cms3.pfjets_area().at(iJet) );
    jet_corrector->setJetPt ( jetp4_uncorr.pt()                      );
    jet_corrector->setJetEta( jetp4_uncorr.eta()                     );

    //Note the subcorrections are stored with corr_vals(N) = corr(N)*corr(N-1)*...*corr(1)
    vector<float> corr_vals = jet_corrector->getSubCorrections();

    double corr             = corr_vals.at(corr_vals.size()-1); // All corrections

    double shift = 0.0;
    if (jecUnc != 0) {
      jecUnc->setJetEta(jetp4_uncorr.eta()); 
      jecUnc->setJetPt(jetp4_uncorr.pt()*corr); 
      double unc = jecUnc->getUncertainty(true);
      if( cms3.evt_isRealData() && corr_vals.size() == 4 ) shift = sqrt(unc*unc + pow((corr_vals.at(corr_vals.size()-1)/corr_vals.at(corr_vals.size()-2)-1.),2));	  
      else                                                 shift = unc;
    }

    double totalshift = 1.0;
    if (jecUnc != 0) {
      if (uncUp) totalshift += shift;
      else  totalshift      -= shift;
    }

    if ( corr * jetp4_uncorr.pt() > 15. ){		  
      jetp4_unshift_vsum += jetp4_uncorr*corr;
      jetp4_shifted_vsum += jetp4_uncorr*corr*totalshift;
    }				  

  }

  for(unsigned int iJet = 0; iJet < cms3.pfjets_p4().size(); iJet++){

    // // get uncorrected jet p4 to use as input for corrections
    LorentzVector jetp4_uncorr = cms3.pfjets_p4().at(iJet)*cms3.pfjets_undoJEC().at(iJet);
    float emfrac = (cms3.pfjets_chargedEmE().at(iJet) + cms3.pfjets_neutralEmE().at(iJet)) / jetp4_uncorr.E();

    if (emfrac > 0.9                  ) continue; // veto events with EM fraction > 0.9
    if( abs(jetp4_uncorr.eta()) > 9.9 ) continue; // veto jets with eta > 9.9

    // get L1FastL2L3 total correction
    jet_corrector->setRho   ( cms3.evt_fixgridfastjet_all_rho()      );
    jet_corrector->setJetA  ( cms3.pfjets_area().at(iJet) );
    jet_corrector->setJetPt ( jetp4_uncorr.pt()                      );
    jet_corrector->setJetEta( jetp4_uncorr.eta()                     );

    //Note the subcorrections are stored with corr_vals(N) = corr(N)*corr(N-1)*...*corr(1)
    vector<float> corr_vals = jet_corrector->getSubCorrections();

    double corr             = corr_vals.at(corr_vals.size()-1); // All corrections
    double corr_l1          = corr_vals.at(0);                  // offset correction

    //	
    // remove SA or global muons from jets before correcting
    //


    if ((cms3.evt_isRealData() || cms3.evt_CMS3tag().at(0).Contains("08-00-16")) && !(cms3.evt_CMS3tag().at(0).Contains("CMS4"))) {
        for (unsigned int pfcind = 0; pfcind < cms3.pfjets_pfcandIndicies().at(iJet).size(); pfcind++){
            int index = cms3.pfjets_pfcandIndicies().at(iJet).at(pfcind);
            if( cms3.pfcands_isGlobalMuon()    .at(index) ||
                    cms3.pfcands_isStandAloneMuon().at(index)){
                jetp4_uncorr -= cms3.pfcands_p4()   .at(index);
            }
        }
    } else {
        for (unsigned int pfcind = 0; pfcind < cms3.pfjets_pfcandmup4().at(iJet).size(); pfcind++){
            jetp4_uncorr -= cms3.pfjets_pfcandmup4().at(iJet).at(pfcind);
        }
    }

    if ( corr * jetp4_uncorr.pt() > 15. ){		  
      T1_metx += jetp4_uncorr.px() * ( corr_l1 - corr );
      T1_mety += jetp4_uncorr.py() * ( corr_l1 - corr );
    }

  }

  T1_metx += jetp4_unshift_vsum.px();
  T1_mety += jetp4_unshift_vsum.py();
  T1_metx -= jetp4_shifted_vsum.px();
  T1_mety -= jetp4_shifted_vsum.py();

  T1_met    = std::sqrt(pow(T1_metx, 2) + pow(T1_mety, 2));
  T1_metPhi = std::atan2(T1_mety, T1_metx);

  return make_pair(T1_met, T1_metPhi);
}

pair <float, float> getT1PUPPIMET_fromMINIAOD( FactorizedJetCorrector * jet_corrector ){
  float T1_met    = cms3.evt_puppi_pfmet_raw();
  float T1_metPhi = cms3.evt_puppi_pfmetPhi_raw();
  float T1_metx   = T1_met * cos(T1_metPhi);
  float T1_mety   = T1_met * sin(T1_metPhi);

  //Run over same jets that were produced with MET tools
  for(unsigned int iJet = 0; iJet < cms3.pfjets_puppi_p4().size(); iJet++){

    // // get uncorrected jet p4 to use as input for corrections
    LorentzVector jetp4_uncorr = cms3.pfjets_puppi_p4().at(iJet)*cms3.pfjets_puppi_undoJEC().at(iJet);
    float emfrac = (cms3.pfjets_puppi_chargedEmE().at(iJet) + cms3.pfjets_puppi_neutralEmE().at(iJet)) / jetp4_uncorr.E();

    if (emfrac > 0.9                  ) continue; // veto events with EM fraction > 0.9
    if( abs(jetp4_uncorr.eta()) > 9.9 ) continue; // veto jets with eta > 9.9

    // get L1FastL2L3 total correction
    jet_corrector->setRho   ( cms3.evt_fixgridfastjet_all_rho()      );
    jet_corrector->setJetA  ( cms3.pfjets_area().at(iJet) );
    jet_corrector->setJetPt ( jetp4_uncorr.pt()                      );
    jet_corrector->setJetEta( jetp4_uncorr.eta()                     );

    //Note the subcorrections are stored with corr_vals(N) = corr(N)*corr(N-1)*...*corr(1)
    vector<float> corr_vals = jet_corrector->getSubCorrections();

    double corr             = corr_vals.at(corr_vals.size()-1); // All corrections
    double corr_l1          = corr_vals.at(0);                  // offset correction
		  
    //	
    // remove SA or global muons from jets before correcting
    //
    for (unsigned int pfcind = 0; pfcind < cms3.pfjets_puppi_pfcandIndicies().at(iJet).size(); pfcind++){
      int index = cms3.pfjets_puppi_pfcandIndicies().at(iJet).at(pfcind);
      if( cms3.pfcands_isGlobalMuon()    .at(index) ||
          cms3.pfcands_isStandAloneMuon().at(index)){
        jetp4_uncorr -= cms3.pfcands_p4()   .at(index);
      }
    }
			  
    if (corr * jetp4_uncorr.pt() > 15.){		  
      T1_metx += jetp4_uncorr.px() * ( corr_l1 - corr );
      T1_mety += jetp4_uncorr.py() * ( corr_l1 - corr );
    }

  }
	  
  T1_met    = std::sqrt(pow(T1_metx, 2) + pow(T1_mety, 2));
  T1_metPhi = std::atan2(T1_mety, T1_metx);

  return make_pair(T1_met, T1_metPhi);
}

pair<float,float> MET3p0() { 

  float met_pt  = 0.0;
  float met_phi = 0.0;
  
  LorentzVector pfcands3p0_p4(0,0,0,0);
  
  for( size_t pfind = 0; pfind < cms3.pfcands_p4().size(); pfind++ ){
	
    if( abs( cms3.pfcands_p4().at(pfind).eta() ) < 3.0 ){ 
      pfcands3p0_p4 -= cms3.pfcands_p4().at(pfind);
    }
  }
  met_pt  = pfcands3p0_p4.pt();
  met_phi = pfcands3p0_p4.phi();

  return make_pair( met_pt, met_phi ); 
}

bool passesMETfilter(bool is25){

  //primary vertex filter (re-run by user)
  if (firstGoodVertex() == -1) return false;

  //CSC beam halo filter 
  if (!filt_cscBeamHalo()) return false;
  
  //HBHE noise filter (re-run by user)
  if (!is25 && !hbheNoiseFilter()) return false;
  if ( is25 && !hbheNoiseFilter_25ns()) return false;

  //ee badSC noise filter NEW
  if (!filt_eeBadSc()) return false;

  //Otherwise good
  return true;

}

bool passesMETfilterv2(){

  //primary vertex filter (re-run by user)
  if (firstGoodVertex() == -1) return false;

  //CSC beam halo filter -- this should be replaced with txt list of events, will be updated when available
  //if (!filt_cscBeamHalo()) return false;
  
  //HBHE noise filter (re-run by user)
  if (!hbheNoiseFilter_25ns()) return false;
  if (!hbheIsoNoiseFilter())   return false;

  //ee badSC noise filter NEW
  if (!filt_eeBadSc()) return false;

  //Otherwise good
  return true;

}

bool passesMETfilters2016(bool isData){

  //primary vertex filter (re-run by user)
  if (firstGoodVertex() == -1) return false;

  if (!filt_globalTightHalo2016()) return false;
  
  if (!filt_hbheNoise()) return false;
  if (!filt_hbheNoiseIso())   return false;

  if (!filt_ecalTP()) return false;
  if (!filt_eeBadSc()) return false;

  if (!filt_goodVertices()) return false;


  // MC samples don't have algoOrig branch
  if(isData) {
    if (!badChargedCandidateFilter()) return false;
    if (!badMuonFilter()) return false;
  }

  //Otherwise good
  return true;

}

bool passesMETfiltersMoriond17(bool isData){

  //primary vertex filter (re-run by user)
  if (firstGoodVertex() == -1) return false;

  if (!filt_globalTightHalo2016()) return false;
  
  if (!filt_hbheNoise()) return false;
  if (!filt_hbheNoiseIso())   return false;

  if (!filt_ecalTP()) return false;
  if (!filt_eeBadSc()) return false;

  if (!filt_goodVertices()) return false;

  if (!badChargedCandidateFilterV2()) return false;
  // MC samples don't have algoOrig branch
  if(isData) {
    if (!badMuonFilterV2()) return false;
  }

  //Otherwise good
  return true;

}

// takes in an already initialized FactorizedJetCorrector object
// and returns T1 Corrected MET using the CHS jet collection
// THIS FUNCTION IS NOT VALIDATED CURRENTLY!
pair <float, float> getT1CHSMET3p0(   FactorizedJetCorrector * jet_corrector ){
  float T1_met    = cms3.evt_METToolboxNoHF_pfmet_raw();
  float T1_metPhi = cms3.evt_METToolboxNoHF_pfmetPhi_raw();
  float T1_metx   = T1_met * cos(T1_metPhi);
  float T1_mety   = T1_met * sin(T1_metPhi);

  //Run over same jets that were produced with MET tools
  for(unsigned int iJet = 0; iJet < cms3.pfjets_METToolbox_p4().size(); iJet++){

    if( cms3.pfjets_METToolbox_p4().at(iJet).eta() > 3.0 ) continue;// only use jets with eta<3.0 to correct MET 3.0
	
    // // get uncorrected jet p4 to use as input for corrections
    LorentzVector jetp4_uncorr = cms3.pfjets_METToolbox_p4().at(iJet);		  
    float emfrac = (cms3.pfjets_METToolbox_chargedEmE().at(iJet) + cms3.pfjets_METToolbox_neutralEmE().at(iJet)) / jetp4_uncorr.E();

    if (emfrac > 0.9                  ) continue; // veto events with EM fraction > 0.9
    if( abs(jetp4_uncorr.eta()) > 9.9 ) continue; // veto jets with eta > 9.9

    // get L1FastL2L3 total correction
    jet_corrector->setRho   ( cms3.evt_fixgridfastjet_all_rho()      );
    jet_corrector->setJetA  ( cms3.pfjets_METToolbox_area().at(iJet) );
    jet_corrector->setJetPt ( jetp4_uncorr.pt()                      );
    jet_corrector->setJetEta( jetp4_uncorr.eta()                     );

    //Note the subcorrections are stored with corr_vals(N) = corr(N)*corr(N-1)*...*corr(1)
    vector<float> corr_vals = jet_corrector->getSubCorrections();

    double corr             = corr_vals.at(corr_vals.size()-1); // All corrections
    double corr_l1          = corr_vals.at(0);                  // offset correction

    // Alternative way to do muon corrections, done by MET group
    for (unsigned int pfcind = 0; pfcind < cms3.pfjets_METToolbox_pfcandIndicies().at(iJet).size(); pfcind++){
      int index = cms3.pfjets_METToolbox_pfcandIndicies().at(iJet).at(pfcind);
      if( cms3.pfcands_isGlobalMuon()    .at(index) ||
          cms3.pfcands_isStandAloneMuon().at(index)){
        jetp4_uncorr -= cms3.pfcands_p4()   .at(index);
      }
    }
			  
    if (corr * jetp4_uncorr.pt() > 10.){		  
      T1_metx += jetp4_uncorr.px() * ( corr_l1 - corr );
      T1_mety += jetp4_uncorr.py() * ( corr_l1 - corr );
    }

  }
	  
  T1_met    = std::sqrt(pow(T1_metx, 2) + pow(T1_mety, 2));
  T1_metPhi = std::atan2(T1_mety, T1_metx);

  return make_pair(T1_met, T1_metPhi);
}

metStruct METpuppi() {

  double pX(0), pY(0), set(0);
  for (unsigned int i=0; i<pfcands_p4().size(); ++i){
    
    pX -= pfcands_p4().at(i).px()*pfcands_puppiWeight().at(i);
    pY -= pfcands_p4().at(i).py()*pfcands_puppiWeight().at(i);
    set += pfcands_p4().at(i).pt()*pfcands_puppiWeight().at(i);

  }
  
  metStruct met;
  met.met     = sqrt(pX * pX + pY * pY);
  met.metphi  = atan2(pY, pX);
  met.metx = pX;
  met.mety = pY;
  met.sumet = set;
  return met;
}

metStruct trackerMETpuppi(float deltaZCut, const std::vector<LorentzVector>* jets) {

  if ( vtxs_isFake().empty() ) return metStruct();
  double pX(0), pY(0);
  
  for (unsigned int i=0; i<pfcands_particleId().size(); ++i){
    if ( pfcands_charge().at(i)==0 ) continue;
    if ( jets ){
      bool matched = false;
      for ( std::vector<LorentzVector>::const_iterator jet = jets->begin(); jet != jets->end(); ++jet )
    	if ( fabs(ROOT::Math::VectorUtil::DeltaR(pfcands_p4().at(i),*jet))<0.5 ) matched=true;
      if (matched) continue;
    }
    
    if ( fabs(pfcands_dz().at(i)) > deltaZCut) continue;
    
    pX -= pfcands_p4().at(i).px()*pfcands_puppiWeight().at(i);
    pY -= pfcands_p4().at(i).py()*pfcands_puppiWeight().at(i);
  }
  
  if (jets){
    for ( std::vector<LorentzVector>::const_iterator jet = jets->begin(); jet != jets->end(); ++jet ){
      pX -= jet->px();
      pY -= jet->py();
    }
  }
  metStruct met;
  met.met     = sqrt(pX * pX + pY * pY);
  met.metphi  = atan2(pY, pX);
  met.metx = pX;
  met.mety = pY;
  return met;
}
