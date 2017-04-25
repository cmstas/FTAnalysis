float triggerScaleFactor(int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht) {
    return TotalTriggerSF(pdgId1, pt1, eta1, pdgId2, pt2, eta2, ht);
}

float electronScaleFactorHighHT(float pt, float eta) {
  if (pt >= 10 && pt < 20 && eta >= -2.500 && eta < -2.000) return 0.8169;
  if (pt >= 10 && pt < 20 && eta >= -2.000 && eta < -1.566) return 0.8139;
  if (pt >= 10 && pt < 20 && eta >= -1.566 && eta < -1.442) return 0.9570;
  if (pt >= 10 && pt < 20 && eta >= -1.442 && eta < -0.800) return 0.9437;
  if (pt >= 10 && pt < 20 && eta >= -0.800 && eta < -0.000) return 0.9245;
  if (pt >= 10 && pt < 20 && eta >= -0.000 && eta < 0.800) return 0.9245;
  if (pt >= 10 && pt < 20 && eta >= 0.800 && eta < 1.442) return 0.9437;
  if (pt >= 10 && pt < 20 && eta >= 1.442 && eta < 1.566) return 0.9570;
  if (pt >= 10 && pt < 20 && eta >= 1.566 && eta < 2.000) return 0.8139;
  if (pt >= 10 && pt < 20 && eta >= 2.000 && eta < 2.500) return 0.8169;
  if (pt >= 20 && pt < 30 && eta >= -2.500 && eta < -2.000) return 0.8477;
  if (pt >= 20 && pt < 30 && eta >= -2.000 && eta < -1.566) return 0.8890;
  if (pt >= 20 && pt < 30 && eta >= -1.566 && eta < -1.442) return 0.9346;
  if (pt >= 20 && pt < 30 && eta >= -1.442 && eta < -0.800) return 0.9389;
  if (pt >= 20 && pt < 30 && eta >= -0.800 && eta < -0.000) return 0.9419;
  if (pt >= 20 && pt < 30 && eta >= -0.000 && eta < 0.800) return 0.9419;
  if (pt >= 20 && pt < 30 && eta >= 0.800 && eta < 1.442) return 0.9389;
  if (pt >= 20 && pt < 30 && eta >= 1.442 && eta < 1.566) return 0.9346;
  if (pt >= 20 && pt < 30 && eta >= 1.566 && eta < 2.000) return 0.8890;
  if (pt >= 20 && pt < 30 && eta >= 2.000 && eta < 2.500) return 0.8477;
  if (pt >= 30 && pt < 40 && eta >= -2.500 && eta < -2.000) return 0.9042;
  if (pt >= 30 && pt < 40 && eta >= -2.000 && eta < -1.566) return 0.9222;
  if (pt >= 30 && pt < 40 && eta >= -1.566 && eta < -1.442) return 0.9696;
  if (pt >= 30 && pt < 40 && eta >= -1.442 && eta < -0.800) return 0.9547;
  if (pt >= 30 && pt < 40 && eta >= -0.800 && eta < -0.000) return 0.9533;
  if (pt >= 30 && pt < 40 && eta >= -0.000 && eta < 0.800) return 0.9533;
  if (pt >= 30 && pt < 40 && eta >= 0.800 && eta < 1.442) return 0.9547;
  if (pt >= 30 && pt < 40 && eta >= 1.442 && eta < 1.566) return 0.9696;
  if (pt >= 30 && pt < 40 && eta >= 1.566 && eta < 2.000) return 0.9222;
  if (pt >= 30 && pt < 40 && eta >= 2.000 && eta < 2.500) return 0.9042;
  if (pt >= 40 && pt < 50 && eta >= -2.500 && eta < -2.000) return 0.9388;
  if (pt >= 40 && pt < 50 && eta >= -2.000 && eta < -1.566) return 0.9430;
  if (pt >= 40 && pt < 50 && eta >= -1.566 && eta < -1.442) return 0.9620;
  if (pt >= 40 && pt < 50 && eta >= -1.442 && eta < -0.800) return 0.9565;
  if (pt >= 40 && pt < 50 && eta >= -0.800 && eta < -0.000) return 0.9516;
  if (pt >= 40 && pt < 50 && eta >= -0.000 && eta < 0.800) return 0.9516;
  if (pt >= 40 && pt < 50 && eta >= 0.800 && eta < 1.442) return 0.9565;
  if (pt >= 40 && pt < 50 && eta >= 1.442 && eta < 1.566) return 0.9620;
  if (pt >= 40 && pt < 50 && eta >= 1.566 && eta < 2.000) return 0.9430;
  if (pt >= 40 && pt < 50 && eta >= 2.000 && eta < 2.500) return 0.9388;
  if (pt >= 50 && pt < 100 && eta >= -2.500 && eta < -2.000) return 0.9486;
  if (pt >= 50 && pt < 100 && eta >= -2.000 && eta < -1.566) return 0.9511;
  if (pt >= 50 && pt < 100 && eta >= -1.566 && eta < -1.442) return 0.9320;
  if (pt >= 50 && pt < 100 && eta >= -1.442 && eta < -0.800) return 0.9535;
  if (pt >= 50 && pt < 100 && eta >= -0.800 && eta < -0.000) return 0.9490;
  if (pt >= 50 && pt < 100 && eta >= -0.000 && eta < 0.800) return 0.9490;
  if (pt >= 50 && pt < 100 && eta >= 0.800 && eta < 1.442) return 0.9535;
  if (pt >= 50 && pt < 100 && eta >= 1.442 && eta < 1.566) return 0.9320;
  if (pt >= 50 && pt < 100 && eta >= 1.566 && eta < 2.000) return 0.9511;
  if (pt >= 50 && pt < 100 && eta >= 2.000 && eta < 2.500) return 0.9486;
  if (pt >= 100  && eta >= -2.500 && eta < -2.000) return 1.0145;
  if (pt >= 100  && eta >= -2.000 && eta < -1.566) return 0.9500;
  if (pt >= 100  && eta >= -1.566 && eta < -1.442) return 0.8957;
  if (pt >= 100  && eta >= -1.442 && eta < -0.800) return 0.9616;
  if (pt >= 100  && eta >= -0.800 && eta < -0.000) return 0.9573;
  if (pt >= 100  && eta >= -0.000 && eta < 0.800) return 0.9573;
  if (pt >= 100  && eta >= 0.800 && eta < 1.442) return 0.9616;
  if (pt >= 100  && eta >= 1.442 && eta < 1.566) return 0.8957;
  if (pt >= 100  && eta >= 1.566 && eta < 2.000) return 0.9500;
  if (pt >= 100  && eta >= 2.000 && eta < 2.500) return 1.0145;
  return 0.;
}

float electronScaleFactorLowHT(float pt, float eta) {
  if (pt >= 10 && pt < 20 && eta >= -2.500 && eta < -2.000) return 0.8165;
  if (pt >= 10 && pt < 20 && eta >= -2.000 && eta < -1.566) return 0.8107;
  if (pt >= 10 && pt < 20 && eta >= -1.566 && eta < -1.442) return 0.9568;
  if (pt >= 10 && pt < 20 && eta >= -1.442 && eta < -0.800) return 0.9437;
  if (pt >= 10 && pt < 20 && eta >= -0.800 && eta < -0.000) return 0.9212;
  if (pt >= 10 && pt < 20 && eta >= -0.000 && eta < 0.800) return 0.9212;
  if (pt >= 10 && pt < 20 && eta >= 0.800 && eta < 1.442) return 0.9437;
  if (pt >= 10 && pt < 20 && eta >= 1.442 && eta < 1.566) return 0.9568;
  if (pt >= 10 && pt < 20 && eta >= 1.566 && eta < 2.000) return 0.8107;
  if (pt >= 10 && pt < 20 && eta >= 2.000 && eta < 2.500) return 0.8165;
  if (pt >= 20 && pt < 30 && eta >= -2.500 && eta < -2.000) return 0.8455;
  if (pt >= 20 && pt < 30 && eta >= -2.000 && eta < -1.566) return 0.8897;
  if (pt >= 20 && pt < 30 && eta >= -1.566 && eta < -1.442) return 0.9357;
  if (pt >= 20 && pt < 30 && eta >= -1.442 && eta < -0.800) return 0.9385;
  if (pt >= 20 && pt < 30 && eta >= -0.800 && eta < -0.000) return 0.9401;
  if (pt >= 20 && pt < 30 && eta >= -0.000 && eta < 0.800) return 0.9401;
  if (pt >= 20 && pt < 30 && eta >= 0.800 && eta < 1.442) return 0.9385;
  if (pt >= 20 && pt < 30 && eta >= 1.442 && eta < 1.566) return 0.9357;
  if (pt >= 20 && pt < 30 && eta >= 1.566 && eta < 2.000) return 0.8897;
  if (pt >= 20 && pt < 30 && eta >= 2.000 && eta < 2.500) return 0.8455;
  if (pt >= 30 && pt < 40 && eta >= -2.500 && eta < -2.000) return 0.9044;
  if (pt >= 30 && pt < 40 && eta >= -2.000 && eta < -1.566) return 0.9224;
  if (pt >= 30 && pt < 40 && eta >= -1.566 && eta < -1.442) return 0.9701;
  if (pt >= 30 && pt < 40 && eta >= -1.442 && eta < -0.800) return 0.9543;
  if (pt >= 30 && pt < 40 && eta >= -0.800 && eta < -0.000) return 0.9522;
  if (pt >= 30 && pt < 40 && eta >= -0.000 && eta < 0.800) return 0.9522;
  if (pt >= 30 && pt < 40 && eta >= 0.800 && eta < 1.442) return 0.9543;
  if (pt >= 30 && pt < 40 && eta >= 1.442 && eta < 1.566) return 0.9701;
  if (pt >= 30 && pt < 40 && eta >= 1.566 && eta < 2.000) return 0.9224;
  if (pt >= 30 && pt < 40 && eta >= 2.000 && eta < 2.500) return 0.9044;
  if (pt >= 40 && pt < 50 && eta >= -2.500 && eta < -2.000) return 0.9389;
  if (pt >= 40 && pt < 50 && eta >= -2.000 && eta < -1.566) return 0.9431;
  if (pt >= 40 && pt < 50 && eta >= -1.566 && eta < -1.442) return 0.9616;
  if (pt >= 40 && pt < 50 && eta >= -1.442 && eta < -0.800) return 0.9562;
  if (pt >= 40 && pt < 50 && eta >= -0.800 && eta < -0.000) return 0.9506;
  if (pt >= 40 && pt < 50 && eta >= -0.000 && eta < 0.800) return 0.9506;
  if (pt >= 40 && pt < 50 && eta >= 0.800 && eta < 1.442) return 0.9562;
  if (pt >= 40 && pt < 50 && eta >= 1.442 && eta < 1.566) return 0.9616;
  if (pt >= 40 && pt < 50 && eta >= 1.566 && eta < 2.000) return 0.9431;
  if (pt >= 40 && pt < 50 && eta >= 2.000 && eta < 2.500) return 0.9389;
  if (pt >= 50 && pt < 100 && eta >= -2.500 && eta < -2.000) return 0.9488;
  if (pt >= 50 && pt < 100 && eta >= -2.000 && eta < -1.566) return 0.9516;
  if (pt >= 50 && pt < 100 && eta >= -1.566 && eta < -1.442) return 0.9324;
  if (pt >= 50 && pt < 100 && eta >= -1.442 && eta < -0.800) return 0.9532;
  if (pt >= 50 && pt < 100 && eta >= -0.800 && eta < -0.000) return 0.9484;
  if (pt >= 50 && pt < 100 && eta >= -0.000 && eta < 0.800) return 0.9484;
  if (pt >= 50 && pt < 100 && eta >= 0.800 && eta < 1.442) return 0.9532;
  if (pt >= 50 && pt < 100 && eta >= 1.442 && eta < 1.566) return 0.9324;
  if (pt >= 50 && pt < 100 && eta >= 1.566 && eta < 2.000) return 0.9516;
  if (pt >= 50 && pt < 100 && eta >= 2.000 && eta < 2.500) return 0.9488;
  if (pt >= 100  && eta >= -2.500 && eta < -2.000) return 1.0091;
  if (pt >= 100  && eta >= -2.000 && eta < -1.566) return 0.9358;
  if (pt >= 100  && eta >= -1.566 && eta < -1.442) return 0.8804;
  if (pt >= 100  && eta >= -1.442 && eta < -0.800) return 0.9390;
  if (pt >= 100  && eta >= -0.800 && eta < -0.000) return 0.9458;
  if (pt >= 100  && eta >= -0.000 && eta < 0.800) return 0.9458;
  if (pt >= 100  && eta >= 0.800 && eta < 1.442) return 0.9390;
  if (pt >= 100  && eta >= 1.442 && eta < 1.566) return 0.8804;
  if (pt >= 100  && eta >= 1.566 && eta < 2.000) return 0.9358;
  if (pt >= 100  && eta >= 2.000 && eta < 2.500) return 1.0091;
  return 0.;
}

float muonScaleFactor(float pt, float eta) {
  if (pt >= 10 && pt < 20 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 0.8964;
  if (pt >= 10 && pt < 20 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.8871;
  if (pt >= 10 && pt < 20 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9727;
  if (pt >= 10 && pt < 20 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9059;
  if (pt >= 20 && pt < 25 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 0.9560;
  if (pt >= 20 && pt < 25 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9518;
  if (pt >= 20 && pt < 25 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9736;
  if (pt >= 20 && pt < 25 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9197;
  if (pt >= 25 && pt < 30 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 0.9732;
  if (pt >= 25 && pt < 30 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9622;
  if (pt >= 25 && pt < 30 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9794;
  if (pt >= 25 && pt < 30 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9318;
  if (pt >= 30 && pt < 40 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 0.9833;
  if (pt >= 30 && pt < 40 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9720;
  if (pt >= 30 && pt < 40 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9839;
  if (pt >= 30 && pt < 40 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9388;
  if (pt >= 40 && pt < 50 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 0.9837;
  if (pt >= 40 && pt < 50 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9769;
  if (pt >= 40 && pt < 50 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9855;
  if (pt >= 40 && pt < 50 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9448;
  if (pt >= 50 && pt < 60 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 0.9672;
  if (pt >= 50 && pt < 60 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9727;
  if (pt >= 50 && pt < 60 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9801;
  if (pt >= 50 && pt < 60 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9355;
  if (pt >= 60  && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 0.9812;
  if (pt >= 60  && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9797;
  if (pt >= 60  && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9920;
  if (pt >= 60  && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9481;
  return 0.;
}

float electronGSF(float pt, float eta) {
  if (pt >= 25  && eta >= -2.500 && eta < -2.450) return 1.3176;
  if (pt >= 25  && eta >= -2.450 && eta < -2.400) return 1.1138;
  if (pt >= 25  && eta >= -2.400 && eta < -2.300) return 1.0246;
  if (pt >= 25  && eta >= -2.300 && eta < -2.200) return 1.0136;
  if (pt >= 25  && eta >= -2.200 && eta < -2.000) return 1.0073;
  if (pt >= 25  && eta >= -2.000 && eta < -1.800) return 0.9948;
  if (pt >= 25  && eta >= -1.800 && eta < -1.630) return 0.9948;
  if (pt >= 25  && eta >= -1.630 && eta < -1.566) return 0.9916;
  if (pt >= 25  && eta >= -1.566 && eta < -1.444) return 0.9631;
  if (pt >= 25  && eta >= -1.444 && eta < -1.200) return 0.9897;
  if (pt >= 25  && eta >= -1.200 && eta < -1.000) return 0.9857;
  if (pt >= 25  && eta >= -1.000 && eta < -0.600) return 0.9816;
  if (pt >= 25  && eta >= -0.600 && eta < -0.400) return 0.9847;
  if (pt >= 25  && eta >= -0.400 && eta < -0.200) return 0.9816;
  if (pt >= 25  && eta >= -0.200 && eta < 0.000) return 0.9804;
  if (pt >= 25  && eta >= 0.000 && eta < 0.200) return 0.9846;
  if (pt >= 25  && eta >= 0.200 && eta < 0.400) return 0.9888;
  if (pt >= 25  && eta >= 0.400 && eta < 0.600) return 0.9877;
  if (pt >= 25  && eta >= 0.600 && eta < 1.000) return 0.9877;
  if (pt >= 25  && eta >= 1.000 && eta < 1.200) return 0.9877;
  if (pt >= 25  && eta >= 1.200 && eta < 1.444) return 0.9877;
  if (pt >= 25  && eta >= 1.444 && eta < 1.566) return 0.9676;
  if (pt >= 25  && eta >= 1.566 && eta < 1.630) return 0.9896;
  if (pt >= 25  && eta >= 1.630 && eta < 1.800) return 0.9928;
  if (pt >= 25  && eta >= 1.800 && eta < 2.000) return 0.9918;
  if (pt >= 25  && eta >= 2.000 && eta < 2.200) return 0.9979;
  if (pt >= 25  && eta >= 2.200 && eta < 2.300) return 1.0010;
  if (pt >= 25  && eta >= 2.300 && eta < 2.400) return 0.9895;
  if (pt >= 25  && eta >= 2.400 && eta < 2.450) return 0.9705;
  if (pt >= 25  && eta >= 2.450 && eta < 2.500) return 0.9067;
  return 1.0;
}

float trackingSF(float eta) {
if (eta >= -2.400 && eta < -2.100) return 0.991237;
if (eta >= -2.100 && eta < -1.600) return 0.994853;
if (eta >= -1.600 && eta < -1.200) return 0.996413;
if (eta >= -1.200 && eta < -0.900) return 0.997157;
if (eta >= -0.900 && eta < -0.600) return 0.997512;
if (eta >= -0.600 && eta < -0.300) return 0.99756;
if (eta >= -0.300 && eta < -0.200) return 0.996745;
if (eta >= -0.200 && eta < 0.200) return 0.996996;
if (eta >= 0.200 && eta < 0.300) return 0.99772;
if (eta >= 0.300 && eta < 0.600) return 0.998604;
if (eta >= 0.600 && eta < 0.900) return 0.998321;
if (eta >= 0.900 && eta < 1.200) return 0.997682;
if (eta >= 1.200 && eta < 1.600) return 0.995252;
if (eta >= 1.600 && eta < 2.100) return 0.994919;
if (eta >= 2.100 && eta < 2.400) return 0.987334;
return 1.0;
}

float leptonScaleFactor(int pdgId, float pt, float eta, float ht) {
  if (abs(pdgId)==13) return muonScaleFactor(pt, eta)*trackingSF(eta);
  else if (abs(pdgId)==11){
    if (ht>300) return electronScaleFactorHighHT(pt, eta)*electronGSF(pt, eta);
      else return electronScaleFactorLowHT(pt, eta)*electronGSF(pt, eta);
  }
  return 0.;
}

float eventScaleFactor(int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht) {
  return triggerScaleFactor(pdgId1, pdgId2, pt1, pt2, eta1, eta2, ht) * leptonScaleFactor(pdgId1, pt1, eta1, ht) * leptonScaleFactor(pdgId2, pt2, eta2, ht);
}

float eventScaleFactor_trigonly(int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht) {
  return triggerScaleFactor(pdgId1, pdgId2, pt1, pt2, eta1, eta2, ht);
}
float eventScaleFactor_leponly(int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht) {
  return leptonScaleFactor(pdgId1, pt1, eta1, ht) * leptonScaleFactor(pdgId2, pt2, eta2, ht);
}
