float muonScaleFactor_RunBC(float pt, float eta) {
  if (pt >= 20 && pt < 25 && eta >= 0.000 && eta < 0.900) return 0.9911;
  if (pt >= 20 && pt < 25 && eta >= 0.900 && eta < 1.200) return 1.0034;
  if (pt >= 20 && pt < 25 && eta >= 1.200 && eta < 2.100) return 0.9967;
  if (pt >= 20 && pt < 25 && eta >= 2.100 && eta < 2.400) return 0.9765;
  if (pt >= 25 && pt < 30 && eta >= 0.000 && eta < 0.900) return 0.9950;
  if (pt >= 25 && pt < 30 && eta >= 0.900 && eta < 1.200) return 0.9943;
  if (pt >= 25 && pt < 30 && eta >= 1.200 && eta < 2.100) return 0.9914;
  if (pt >= 25 && pt < 30 && eta >= 2.100 && eta < 2.400) return 0.9733;
  if (pt >= 30 && pt < 40 && eta >= 0.000 && eta < 0.900) return 0.9976;
  if (pt >= 30 && pt < 40 && eta >= 0.900 && eta < 1.200) return 0.9993;
  if (pt >= 30 && pt < 40 && eta >= 1.200 && eta < 2.100) return 0.9981;
  if (pt >= 30 && pt < 40 && eta >= 2.100 && eta < 2.400) return 0.9794;
  if (pt >= 40 && pt < 50 && eta >= 0.000 && eta < 0.900) return 0.9966;
  if (pt >= 40 && pt < 50 && eta >= 0.900 && eta < 1.200) return 0.9968;
  if (pt >= 40 && pt < 50 && eta >= 1.200 && eta < 2.100) return 0.9960;
  if (pt >= 40 && pt < 50 && eta >= 2.100 && eta < 2.400) return 0.9760;
  if (pt >= 50 && pt < 60 && eta >= 0.000 && eta < 0.900) return 0.9934;
  if (pt >= 50 && pt < 60 && eta >= 0.900 && eta < 1.200) return 0.9963;
  if (pt >= 50 && pt < 60 && eta >= 1.200 && eta < 2.100) return 0.9940;
  if (pt >= 50 && pt < 60 && eta >= 2.100 && eta < 2.400) return 0.9748;
  if (pt >= 60  && eta >= 0.000 && eta < 0.900) return 0.9971;
  if (pt >= 60  && eta >= 0.900 && eta < 1.200) return 0.9953;
  if (pt >= 60  && eta >= 1.200 && eta < 2.100) return 0.9980;
  if (pt >= 60  && eta >= 2.100 && eta < 2.400) return 0.9886;
  return 1.;
}

float muonScaleFactorError_RunBC(float pt, float eta) {
  if (pt >= 20 && pt < 25 && eta >= 0.000 && eta < 0.900) return 0.0047;
  if (pt >= 20 && pt < 25 && eta >= 0.900 && eta < 1.200) return 0.0065;
  if (pt >= 20 && pt < 25 && eta >= 1.200 && eta < 2.100) return 0.0031;
  if (pt >= 20 && pt < 25 && eta >= 2.100 && eta < 2.400) return 0.0065;
  if (pt >= 25 && pt < 30 && eta >= 0.000 && eta < 0.900) return 0.0018;
  if (pt >= 25 && pt < 30 && eta >= 0.900 && eta < 1.200) return 0.0024;
  if (pt >= 25 && pt < 30 && eta >= 1.200 && eta < 2.100) return 0.0013;
  if (pt >= 25 && pt < 30 && eta >= 2.100 && eta < 2.400) return 0.0107;
  if (pt >= 30 && pt < 40 && eta >= 0.000 && eta < 0.900) return 0.0004;
  if (pt >= 30 && pt < 40 && eta >= 0.900 && eta < 1.200) return 0.0008;
  if (pt >= 30 && pt < 40 && eta >= 1.200 && eta < 2.100) return 0.0005;
  if (pt >= 30 && pt < 40 && eta >= 2.100 && eta < 2.400) return 0.0013;
  if (pt >= 40 && pt < 50 && eta >= 0.000 && eta < 0.900) return 0.0003;
  if (pt >= 40 && pt < 50 && eta >= 0.900 && eta < 1.200) return 0.0008;
  if (pt >= 40 && pt < 50 && eta >= 1.200 && eta < 2.100) return 0.0003;
  if (pt >= 40 && pt < 50 && eta >= 2.100 && eta < 2.400) return 0.0014;
  if (pt >= 50 && pt < 60 && eta >= 0.000 && eta < 0.900) return 0.0010;
  if (pt >= 50 && pt < 60 && eta >= 0.900 && eta < 1.200) return 0.0018;
  if (pt >= 50 && pt < 60 && eta >= 1.200 && eta < 2.100) return 0.0012;
  if (pt >= 50 && pt < 60 && eta >= 2.100 && eta < 2.400) return 0.0043;
  if (pt >= 60  && eta >= 0.000 && eta < 0.900) return 0.0021;
  if (pt >= 60  && eta >= 0.900 && eta < 1.200) return 0.0037;
  if (pt >= 60  && eta >= 1.200 && eta < 2.100) return 0.0026;
  if (pt >= 60  && eta >= 2.100 && eta < 2.400) return 0.0107;
  return 1.;
}

float muonScaleFactor_RunDE(float pt, float eta) {
  if (pt >= 20 && pt < 25 && eta >= 0.000 && eta < 0.900) return 0.9966;
  if (pt >= 20 && pt < 25 && eta >= 0.900 && eta < 1.200) return 1.0051;
  if (pt >= 20 && pt < 25 && eta >= 1.200 && eta < 2.100) return 0.9970;
  if (pt >= 20 && pt < 25 && eta >= 2.100 && eta < 2.400) return 0.9760;
  if (pt >= 25 && pt < 30 && eta >= 0.000 && eta < 0.900) return 0.9925;
  if (pt >= 25 && pt < 30 && eta >= 0.900 && eta < 1.200) return 0.9956;
  if (pt >= 25 && pt < 30 && eta >= 1.200 && eta < 2.100) return 0.9942;
  if (pt >= 25 && pt < 30 && eta >= 2.100 && eta < 2.400) return 0.9729;
  if (pt >= 30 && pt < 40 && eta >= 0.000 && eta < 0.900) return 0.9990;
  if (pt >= 30 && pt < 40 && eta >= 0.900 && eta < 1.200) return 0.9999;
  if (pt >= 30 && pt < 40 && eta >= 1.200 && eta < 2.100) return 0.9983;
  if (pt >= 30 && pt < 40 && eta >= 2.100 && eta < 2.400) return 0.9799;
  if (pt >= 40 && pt < 50 && eta >= 0.000 && eta < 0.900) return 0.9965;
  if (pt >= 40 && pt < 50 && eta >= 0.900 && eta < 1.200) return 0.9980;
  if (pt >= 40 && pt < 50 && eta >= 1.200 && eta < 2.100) return 0.9963;
  if (pt >= 40 && pt < 50 && eta >= 2.100 && eta < 2.400) return 0.9766;
  if (pt >= 50 && pt < 60 && eta >= 0.000 && eta < 0.900) return 0.9940;
  if (pt >= 50 && pt < 60 && eta >= 0.900 && eta < 1.200) return 0.9979;
  if (pt >= 50 && pt < 60 && eta >= 1.200 && eta < 2.100) return 0.9955;
  if (pt >= 50 && pt < 60 && eta >= 2.100 && eta < 2.400) return 0.9702;
  if (pt >= 60  && eta >= 0.000 && eta < 0.900) return 0.9964;
  if (pt >= 60  && eta >= 0.900 && eta < 1.200) return 0.9969;
  if (pt >= 60  && eta >= 1.200 && eta < 2.100) return 0.9943;
  if (pt >= 60  && eta >= 2.100 && eta < 2.400) return 0.9777;
  return 1.;
}

float muonScaleFactorError_RunDE(float pt, float eta) {
  if (pt >= 20 && pt < 25 && eta >= 0.000 && eta < 0.900) return 0.0056;
  if (pt >= 20 && pt < 25 && eta >= 0.900 && eta < 1.200) return 0.0074;
  if (pt >= 20 && pt < 25 && eta >= 1.200 && eta < 2.100) return 0.0036;
  if (pt >= 20 && pt < 25 && eta >= 2.100 && eta < 2.400) return 0.0050;
  if (pt >= 25 && pt < 30 && eta >= 0.000 && eta < 0.900) return 0.0021;
  if (pt >= 25 && pt < 30 && eta >= 0.900 && eta < 1.200) return 0.0036;
  if (pt >= 25 && pt < 30 && eta >= 1.200 && eta < 2.100) return 0.0043;
  if (pt >= 25 && pt < 30 && eta >= 2.100 && eta < 2.400) return 0.0038;
  if (pt >= 30 && pt < 40 && eta >= 0.000 && eta < 0.900) return 0.0005;
  if (pt >= 30 && pt < 40 && eta >= 0.900 && eta < 1.200) return 0.0009;
  if (pt >= 30 && pt < 40 && eta >= 1.200 && eta < 2.100) return 0.0005;
  if (pt >= 30 && pt < 40 && eta >= 2.100 && eta < 2.400) return 0.0021;
  if (pt >= 40 && pt < 50 && eta >= 0.000 && eta < 0.900) return 0.0003;
  if (pt >= 40 && pt < 50 && eta >= 0.900 && eta < 1.200) return 0.0005;
  if (pt >= 40 && pt < 50 && eta >= 1.200 && eta < 2.100) return 0.0005;
  if (pt >= 40 && pt < 50 && eta >= 2.100 && eta < 2.400) return 0.0016;
  if (pt >= 50 && pt < 60 && eta >= 0.000 && eta < 0.900) return 0.0010;
  if (pt >= 50 && pt < 60 && eta >= 0.900 && eta < 1.200) return 0.0029;
  if (pt >= 50 && pt < 60 && eta >= 1.200 && eta < 2.100) return 0.0013;
  if (pt >= 50 && pt < 60 && eta >= 2.100 && eta < 2.400) return 0.0052;
  if (pt >= 60  && eta >= 0.000 && eta < 0.900) return 0.0023;
  if (pt >= 60  && eta >= 0.900 && eta < 1.200) return 0.0113;
  if (pt >= 60  && eta >= 1.200 && eta < 2.100) return 0.0033;
  if (pt >= 60  && eta >= 2.100 && eta < 2.400) return 0.0131;
  return 1.;
}

float muonScaleFactor_RunF(float pt, float eta) {
  if (pt >= 20 && pt < 25 && eta >= 0.000 && eta < 0.900) return 0.9964;
  if (pt >= 20 && pt < 25 && eta >= 0.900 && eta < 1.200) return 1.0054;
  if (pt >= 20 && pt < 25 && eta >= 1.200 && eta < 2.100) return 0.9929;
  if (pt >= 20 && pt < 25 && eta >= 2.100 && eta < 2.400) return 0.9755;
  if (pt >= 25 && pt < 30 && eta >= 0.000 && eta < 0.900) return 0.9952;
  if (pt >= 25 && pt < 30 && eta >= 0.900 && eta < 1.200) return 0.9992;
  if (pt >= 25 && pt < 30 && eta >= 1.200 && eta < 2.100) return 0.9951;
  if (pt >= 25 && pt < 30 && eta >= 2.100 && eta < 2.400) return 0.9775;
  if (pt >= 30 && pt < 40 && eta >= 0.000 && eta < 0.900) return 0.9990;
  if (pt >= 30 && pt < 40 && eta >= 0.900 && eta < 1.200) return 0.9993;
  if (pt >= 30 && pt < 40 && eta >= 1.200 && eta < 2.100) return 0.9984;
  if (pt >= 30 && pt < 40 && eta >= 2.100 && eta < 2.400) return 0.9780;
  if (pt >= 40 && pt < 50 && eta >= 0.000 && eta < 0.900) return 0.9970;
  if (pt >= 40 && pt < 50 && eta >= 0.900 && eta < 1.200) return 0.9978;
  if (pt >= 40 && pt < 50 && eta >= 1.200 && eta < 2.100) return 0.9955;
  if (pt >= 40 && pt < 50 && eta >= 2.100 && eta < 2.400) return 0.9765;
  if (pt >= 50 && pt < 60 && eta >= 0.000 && eta < 0.900) return 0.9939;
  if (pt >= 50 && pt < 60 && eta >= 0.900 && eta < 1.200) return 0.9960;
  if (pt >= 50 && pt < 60 && eta >= 1.200 && eta < 2.100) return 0.9945;
  if (pt >= 50 && pt < 60 && eta >= 2.100 && eta < 2.400) return 0.9681;
  if (pt >= 60  && eta >= 0.000 && eta < 0.900) return 0.9985;
  if (pt >= 60  && eta >= 0.900 && eta < 1.200) return 0.9948;
  if (pt >= 60  && eta >= 1.200 && eta < 2.100) return 0.9969;
  if (pt >= 60  && eta >= 2.100 && eta < 2.400) return 0.9762;
  return 1.;
}

float muonScaleFactorError_RunF(float pt, float eta) {
  if (pt >= 20 && pt < 25 && eta >= 0.000 && eta < 0.900) return 0.0063;
  if (pt >= 20 && pt < 25 && eta >= 0.900 && eta < 1.200) return 0.0083;
  if (pt >= 20 && pt < 25 && eta >= 1.200 && eta < 2.100) return 0.0017;
  if (pt >= 20 && pt < 25 && eta >= 2.100 && eta < 2.400) return 0.0069;
  if (pt >= 25 && pt < 30 && eta >= 0.000 && eta < 0.900) return 0.0016;
  if (pt >= 25 && pt < 30 && eta >= 0.900 && eta < 1.200) return 0.0035;
  if (pt >= 25 && pt < 30 && eta >= 1.200 && eta < 2.100) return 0.0019;
  if (pt >= 25 && pt < 30 && eta >= 2.100 && eta < 2.400) return 0.0042;
  if (pt >= 30 && pt < 40 && eta >= 0.000 && eta < 0.900) return 0.0006;
  if (pt >= 30 && pt < 40 && eta >= 0.900 && eta < 1.200) return 0.0010;
  if (pt >= 30 && pt < 40 && eta >= 1.200 && eta < 2.100) return 0.0009;
  if (pt >= 30 && pt < 40 && eta >= 2.100 && eta < 2.400) return 0.0024;
  if (pt >= 40 && pt < 50 && eta >= 0.000 && eta < 0.900) return 0.0004;
  if (pt >= 40 && pt < 50 && eta >= 0.900 && eta < 1.200) return 0.0006;
  if (pt >= 40 && pt < 50 && eta >= 1.200 && eta < 2.100) return 0.0004;
  if (pt >= 40 && pt < 50 && eta >= 2.100 && eta < 2.400) return 0.0019;
  if (pt >= 50 && pt < 60 && eta >= 0.000 && eta < 0.900) return 0.0012;
  if (pt >= 50 && pt < 60 && eta >= 0.900 && eta < 1.200) return 0.0022;
  if (pt >= 50 && pt < 60 && eta >= 1.200 && eta < 2.100) return 0.0016;
  if (pt >= 50 && pt < 60 && eta >= 2.100 && eta < 2.400) return 0.0061;
  if (pt >= 60  && eta >= 0.000 && eta < 0.900) return 0.0026;
  if (pt >= 60  && eta >= 0.900 && eta < 1.200) return 0.0056;
  if (pt >= 60  && eta >= 1.200 && eta < 2.100) return 0.0042;
  if (pt >= 60  && eta >= 2.100 && eta < 2.400) return 0.0156;
  return 1.;
}

float muonScaleFactor_RunBCDEF(float pt, float eta) {
  if (pt >= 20 && pt < 25 && eta >= 0.000 && eta < 0.900) return 0.9946;
  if (pt >= 20 && pt < 25 && eta >= 0.900 && eta < 1.200) return 1.0046;
  if (pt >= 20 && pt < 25 && eta >= 1.200 && eta < 2.100) return 0.9955;
  if (pt >= 20 && pt < 25 && eta >= 2.100 && eta < 2.400) return 0.9760;
  if (pt >= 25 && pt < 30 && eta >= 0.000 && eta < 0.900) return 0.9943;
  if (pt >= 25 && pt < 30 && eta >= 0.900 && eta < 1.200) return 0.9963;
  if (pt >= 25 && pt < 30 && eta >= 1.200 && eta < 2.100) return 0.9935;
  if (pt >= 25 && pt < 30 && eta >= 2.100 && eta < 2.400) return 0.9746;
  if (pt >= 30 && pt < 40 && eta >= 0.000 && eta < 0.900) return 0.9985;
  if (pt >= 30 && pt < 40 && eta >= 0.900 && eta < 1.200) return 0.9995;
  if (pt >= 30 && pt < 40 && eta >= 1.200 && eta < 2.100) return 0.9982;
  if (pt >= 30 && pt < 40 && eta >= 2.100 && eta < 2.400) return 0.9791;
  if (pt >= 40 && pt < 50 && eta >= 0.000 && eta < 0.900) return 0.9967;
  if (pt >= 40 && pt < 50 && eta >= 0.900 && eta < 1.200) return 0.9975;
  if (pt >= 40 && pt < 50 && eta >= 1.200 && eta < 2.100) return 0.9959;
  if (pt >= 40 && pt < 50 && eta >= 2.100 && eta < 2.400) return 0.9764;
  if (pt >= 50 && pt < 60 && eta >= 0.000 && eta < 0.900) return 0.9938;
  if (pt >= 50 && pt < 60 && eta >= 0.900 && eta < 1.200) return 0.9967;
  if (pt >= 50 && pt < 60 && eta >= 1.200 && eta < 2.100) return 0.9946;
  if (pt >= 50 && pt < 60 && eta >= 2.100 && eta < 2.400) return 0.9711;
  if (pt >= 60  && eta >= 0.000 && eta < 0.900) return 0.9973;
  if (pt >= 60  && eta >= 0.900 && eta < 1.200) return 0.9957;
  if (pt >= 60  && eta >= 1.200 && eta < 2.100) return 0.9964;
  if (pt >= 60  && eta >= 2.100 && eta < 2.400) return 0.9810;
  return 1.;
}

float muonScaleFactorError_RunBCDEF(float pt, float eta) {
  if (pt >= 20 && pt < 25 && eta >= 0.000 && eta < 0.900) return 0.0032;
  if (pt >= 20 && pt < 25 && eta >= 0.900 && eta < 1.200) return 0.0043;
  if (pt >= 20 && pt < 25 && eta >= 1.200 && eta < 2.100) return 0.0072;
  if (pt >= 20 && pt < 25 && eta >= 2.100 && eta < 2.400) return 0.0198;
  if (pt >= 25 && pt < 30 && eta >= 0.000 && eta < 0.900) return 0.0011;
  if (pt >= 25 && pt < 30 && eta >= 0.900 && eta < 1.200) return 0.0076;
  if (pt >= 25 && pt < 30 && eta >= 1.200 && eta < 2.100) return 0.0016;
  if (pt >= 25 && pt < 30 && eta >= 2.100 && eta < 2.400) return 0.0209;
  if (pt >= 30 && pt < 40 && eta >= 0.000 && eta < 0.900) return 0.0003;
  if (pt >= 30 && pt < 40 && eta >= 0.900 && eta < 1.200) return 0.0005;
  if (pt >= 30 && pt < 40 && eta >= 1.200 && eta < 2.100) return 0.0057;
  if (pt >= 30 && pt < 40 && eta >= 2.100 && eta < 2.400) return 0.0264;
  if (pt >= 40 && pt < 50 && eta >= 0.000 && eta < 0.900) return 0.0002;
  if (pt >= 40 && pt < 50 && eta >= 0.900 && eta < 1.200) return 0.0004;
  if (pt >= 40 && pt < 50 && eta >= 1.200 && eta < 2.100) return 0.0002;
  if (pt >= 40 && pt < 50 && eta >= 2.100 && eta < 2.400) return 0.0010;
  if (pt >= 50 && pt < 60 && eta >= 0.000 && eta < 0.900) return 0.0006;
  if (pt >= 50 && pt < 60 && eta >= 0.900 && eta < 1.200) return 0.0013;
  if (pt >= 50 && pt < 60 && eta >= 1.200 && eta < 2.100) return 0.0008;
  if (pt >= 50 && pt < 60 && eta >= 2.100 && eta < 2.400) return 0.0030;
  if (pt >= 60  && eta >= 0.000 && eta < 0.900) return 0.0013;
  if (pt >= 60  && eta >= 0.900 && eta < 1.200) return 0.0043;
  if (pt >= 60  && eta >= 1.200 && eta < 2.100) return 0.0020;
  if (pt >= 60  && eta >= 2.100 && eta < 2.400) return 0.0076;
  return 1.;
}

