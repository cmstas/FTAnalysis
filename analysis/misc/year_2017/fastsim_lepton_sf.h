float electronScaleFactorFastSim(float pt, float eta) {
  if (pt >= 10 && pt < 20 && eta >= -2.500 && eta < -1.444) return 0.7069;
  if (pt >= 10 && pt < 20 && eta >= -1.444 && eta < -0.800) return 0.9707;
  if (pt >= 10 && pt < 20 && eta >= -0.800 && eta < 0.000) return 0.9450;
  if (pt >= 10 && pt < 20 && eta >= 0.000 && eta < 0.800) return 0.9649;
  if (pt >= 10 && pt < 20 && eta >= 0.800 && eta < 1.444) return 0.9629;
  if (pt >= 10 && pt < 20 && eta >= 1.444 && eta < 2.500) return 0.7410;
  if (pt >= 20 && pt < 35 && eta >= -2.500 && eta < -1.444) return 0.7481;
  if (pt >= 20 && pt < 35 && eta >= -1.444 && eta < -0.800) return 0.9377;
  if (pt >= 20 && pt < 35 && eta >= -0.800 && eta < 0.000) return 0.9338;
  if (pt >= 20 && pt < 35 && eta >= 0.000 && eta < 0.800) return 0.9254;
  if (pt >= 20 && pt < 35 && eta >= 0.800 && eta < 1.444) return 0.9217;
  if (pt >= 20 && pt < 35 && eta >= 1.444 && eta < 2.500) return 0.7567;
  if (pt >= 35 && pt < 50 && eta >= -2.500 && eta < -1.444) return 0.8027;
  if (pt >= 35 && pt < 50 && eta >= -1.444 && eta < -0.800) return 0.9354;
  if (pt >= 35 && pt < 50 && eta >= -0.800 && eta < 0.000) return 0.9361;
  if (pt >= 35 && pt < 50 && eta >= 0.000 && eta < 0.800) return 0.9329;
  if (pt >= 35 && pt < 50 && eta >= 0.800 && eta < 1.444) return 0.9273;
  if (pt >= 35 && pt < 50 && eta >= 1.444 && eta < 2.500) return 0.8149;
  if (pt >= 50 && pt < 100 && eta >= -2.500 && eta < -1.444) return 0.8052;
  if (pt >= 50 && pt < 100 && eta >= -1.444 && eta < -0.800) return 0.9390;
  if (pt >= 50 && pt < 100 && eta >= -0.800 && eta < 0.000) return 0.9365;
  if (pt >= 50 && pt < 100 && eta >= 0.000 && eta < 0.800) return 0.9382;
  if (pt >= 50 && pt < 100 && eta >= 0.800 && eta < 1.444) return 0.9321;
  if (pt >= 50 && pt < 100 && eta >= 1.444 && eta < 2.500) return 0.8229;
  if (pt >= 100  && eta >= -2.500 && eta < -1.444) return 0.8175;
  if (pt >= 100  && eta >= -1.444 && eta < -0.800) return 0.9174;
  if (pt >= 100  && eta >= -0.800 && eta < 0.000) return 0.9419;
  if (pt >= 100  && eta >= 0.000 && eta < 0.800) return 0.9346;
  if (pt >= 100  && eta >= 0.800 && eta < 1.444) return 0.9122;
  if (pt >= 100  && eta >= 1.444 && eta < 2.500) return 0.8418;
  return 1.;
}

float muonScaleFactorFastSim(float pt, float eta) {
  if (pt >= 10 && pt < 20 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 1.0136;
  if (pt >= 10 && pt < 20 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9995;
  if (pt >= 10 && pt < 20 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9978;
  if (pt >= 10 && pt < 20 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9989;
  if (pt >= 20 && pt < 35 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 0.9923;
  if (pt >= 20 && pt < 35 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9934;
  if (pt >= 20 && pt < 35 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9944;
  if (pt >= 20 && pt < 35 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9924;
  if (pt >= 35 && pt < 50 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 1.0004;
  if (pt >= 35 && pt < 50 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9990;
  if (pt >= 35 && pt < 50 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 1.0006;
  if (pt >= 35 && pt < 50 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 1.0001;
  if (pt >= 50 && pt < 100 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 1.0005;
  if (pt >= 50 && pt < 100 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9999;
  if (pt >= 50 && pt < 100 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9997;
  if (pt >= 50 && pt < 100 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9985;
  if (pt >= 100 && pt < 200 && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 1.0022;
  if (pt >= 100 && pt < 200 && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9977;
  if (pt >= 100 && pt < 200 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 1.0005;
  if (pt >= 100 && pt < 200 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9974;
  if (pt >= 200  && fabs(eta) >= 0.000 && fabs(eta) < 0.900) return 1.0003;
  if (pt >= 200  && fabs(eta) >= 0.900 && fabs(eta) < 1.200) return 0.9970;
  if (pt >= 200  && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.9972;
  if (pt >= 200  && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.9960;
  return 1.;
}

float leptonScaleFactorFastSim(int pdgId, float pt, float eta, int ht) {
  if (abs(pdgId)==13) return muonScaleFactorFastSim(pt, eta);
  return electronScaleFactorFastSim(pt, eta);
}
