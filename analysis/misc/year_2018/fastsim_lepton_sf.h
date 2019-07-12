float electronScaleFactorFastSim(float pt, float eta) {
  if (pt >= 10 && pt < 20 && eta >= -2.500 && eta < -1.444) return 0.6741;
  if (pt >= 10 && pt < 20 && eta >= -1.444 && eta < -0.800) return 0.8828;
  if (pt >= 10 && pt < 20 && eta >= -0.800 && eta < 0.000) return 0.8762;
  if (pt >= 10 && pt < 20 && eta >= 0.000 && eta < 0.800) return 0.9193;
  if (pt >= 10 && pt < 20 && eta >= 0.800 && eta < 1.444) return 0.8889;
  if (pt >= 10 && pt < 20 && eta >= 1.444 && eta < 2.500) return 0.6617;
  if (pt >= 20 && pt < 35 && eta >= -2.500 && eta < -1.444) return 0.7448;
  if (pt >= 20 && pt < 35 && eta >= -1.444 && eta < -0.800) return 0.8945;
  if (pt >= 20 && pt < 35 && eta >= -0.800 && eta < 0.000) return 0.9181;
  if (pt >= 20 && pt < 35 && eta >= 0.000 && eta < 0.800) return 0.9161;
  if (pt >= 20 && pt < 35 && eta >= 0.800 && eta < 1.444) return 0.8847;
  if (pt >= 20 && pt < 35 && eta >= 1.444 && eta < 2.500) return 0.7463;
  if (pt >= 35 && pt < 50 && eta >= -2.500 && eta < -1.444) return 0.8084;
  if (pt >= 35 && pt < 50 && eta >= -1.444 && eta < -0.800) return 0.9190;
  if (pt >= 35 && pt < 50 && eta >= -0.800 && eta < 0.000) return 0.9291;
  if (pt >= 35 && pt < 50 && eta >= 0.000 && eta < 0.800) return 0.9242;
  if (pt >= 35 && pt < 50 && eta >= 0.800 && eta < 1.444) return 0.9049;
  if (pt >= 35 && pt < 50 && eta >= 1.444 && eta < 2.500) return 0.8127;
  if (pt >= 50 && pt < 100 && eta >= -2.500 && eta < -1.444) return 0.8265;
  if (pt >= 50 && pt < 100 && eta >= -1.444 && eta < -0.800) return 0.9269;
  if (pt >= 50 && pt < 100 && eta >= -0.800 && eta < 0.000) return 0.9370;
  if (pt >= 50 && pt < 100 && eta >= 0.000 && eta < 0.800) return 0.9302;
  if (pt >= 50 && pt < 100 && eta >= 0.800 && eta < 1.444) return 0.9172;
  if (pt >= 50 && pt < 100 && eta >= 1.444 && eta < 2.500) return 0.8422;
  if (pt >= 100  && eta >= -2.500 && eta < -1.444) return 0.8931;
  if (pt >= 100  && eta >= -1.444 && eta < -0.800) return 0.9138;
  if (pt >= 100  && eta >= -0.800 && eta < 0.000) return 0.9549;
  if (pt >= 100  && eta >= 0.000 && eta < 0.800) return 0.9525;
  if (pt >= 100  && eta >= 0.800 && eta < 1.444) return 0.9390;
  if (pt >= 100  && eta >= 1.444 && eta < 2.500) return 0.8558;
  return 1.;
}

float muonScaleFactorFastSim(float pt, float eta) {
  if (pt >= 10 && pt < 20 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 0.9844;
  if (pt >= 10 && pt < 20 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9742;
  if (pt >= 10 && pt < 20 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9765;
  if (pt >= 10 && pt < 20 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9725;
  if (pt >= 20 && pt < 35 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 0.9990;
  if (pt >= 20 && pt < 35 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9965;
  if (pt >= 20 && pt < 35 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9934;
  if (pt >= 20 && pt < 35 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9955;
  if (pt >= 35 && pt < 50 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 1.0003;
  if (pt >= 35 && pt < 50 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9995;
  if (pt >= 35 && pt < 50 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9987;
  if (pt >= 35 && pt < 50 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9988;
  if (pt >= 50 && pt < 100 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 1.0003;
  if (pt >= 50 && pt < 100 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9995;
  if (pt >= 50 && pt < 100 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9995;
  if (pt >= 50 && pt < 100 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9993;
  if (pt >= 100 && pt < 200 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 1.0017;
  if (pt >= 100 && pt < 200 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9962;
  if (pt >= 100 && pt < 200 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 1.0018;
  if (pt >= 100 && pt < 200 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9914;
  if (pt >= 200  && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 0.9928;
  if (pt >= 200  && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 1.0000;
  if (pt >= 200  && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9914;
  if (pt >= 200  && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 1.0000;
  return 1.;
}

float leptonScaleFactorFastSim(int pdgId, float pt, float eta, int ht) {
  if (abs(pdgId)==13) return muonScaleFactorFastSim(pt, eta);
  return electronScaleFactorFastSim(pt, eta);
}
