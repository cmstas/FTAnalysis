float electronFakeRate_2016(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.212947;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.191067;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.114535;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0598557;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0583876;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0485127;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0502397;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.055552;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0547698;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0612282;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0709114;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0685393;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.126793;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.134804;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.138614;
   return 0.;
}


float muonFakeRate_2016(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.303083;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.358831;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.383154;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0711473;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0771226;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0880371;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0423236;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0447963;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0513743;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0452653;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0421763;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0437186;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0587507;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0498613;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   return 0.;
}

float fakeRate_2016(int id, float pt, float eta, float ht) { 
  if (ht>300.) {
    if (abs(id)==11) return electronFakeRate(pt,eta);
    else if (abs(id)==13) return muonFakeRate(pt,eta);
    else return 0.;
  } else {
    if (abs(id)==11) return electronFakeRate_IsoTrigs(pt,eta);
    else if (abs(id)==13) return muonFakeRate_IsoTrigs(pt,eta);
    else return 0.;
  }
}

float flipRate_2016(float pt, float eta) {
    // float scale = 1.20;
    // float scale = 0.8;
    // float scale = 1.0;
    float scale = 0.97;
   if (pt>=15 && pt<40 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 3.27436e-05*scale;
   if (pt>=15 && pt<40 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000181807*scale;
   if (pt>=15 && pt<40 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0013915*scale;
   if (pt>=40 && pt<60 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 1.84387e-05*scale;
   if (pt>=40 && pt<60 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000253821*scale;
   if (pt>=40 && pt<60 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00191133*scale;
   if (pt>=60 && pt<80 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 3.41058e-05*scale;
   if (pt>=60 && pt<80 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000469437*scale;
   if (pt>=60 && pt<80 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00283116*scale;
   if (pt>=80 && pt<100 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 2.92427e-05*scale;
   if (pt>=80 && pt<100 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000649611*scale;
   if (pt>=80 && pt<100 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00411099*scale;
   if (pt>=100 && pt<200 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 7.72003e-05*scale;
   if (pt>=100 && pt<200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000851706*scale;
   if (pt>=100 && pt<200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00448173*scale;
   if (pt>=200 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 8.51165e-06*scale;
   if (pt>=200 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.000804451*scale;
   if (pt>=200 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00451892*scale;
   return 0.*scale;
}

float getTruePUw_2016(int nvtx) {
    // below derived using CMS3
   if (nvtx>=0.000000 && nvtx<1.000000) return 0.456317;
   if (nvtx>=1.000000 && nvtx<2.000000) return 0.868822;
   if (nvtx>=2.000000 && nvtx<3.000000) return 1.060915;
   if (nvtx>=3.000000 && nvtx<4.000000) return 0.804616;
   if (nvtx>=4.000000 && nvtx<5.000000) return 1.279666;
   if (nvtx>=5.000000 && nvtx<6.000000) return 1.377812;
   if (nvtx>=6.000000 && nvtx<7.000000) return 0.704106;
   if (nvtx>=7.000000 && nvtx<8.000000) return 0.494254;
   if (nvtx>=8.000000 && nvtx<9.000000) return 0.718032;
   if (nvtx>=9.000000 && nvtx<10.000000) return 0.853469;
   if (nvtx>=10.000000 && nvtx<11.000000) return 0.938278;
   if (nvtx>=11.000000 && nvtx<12.000000) return 1.057068;
   if (nvtx>=12.000000 && nvtx<13.000000) return 1.122293;
   if (nvtx>=13.000000 && nvtx<14.000000) return 1.153207;
   if (nvtx>=14.000000 && nvtx<15.000000) return 1.205728;
   if (nvtx>=15.000000 && nvtx<16.000000) return 1.198719;
   if (nvtx>=16.000000 && nvtx<17.000000) return 1.184970;
   if (nvtx>=17.000000 && nvtx<18.000000) return 1.176247;
   if (nvtx>=18.000000 && nvtx<19.000000) return 1.127847;
   if (nvtx>=19.000000 && nvtx<20.000000) return 1.083409;
   if (nvtx>=20.000000 && nvtx<21.000000) return 1.063132;
   if (nvtx>=21.000000 && nvtx<22.000000) return 1.042620;
   if (nvtx>=22.000000 && nvtx<23.000000) return 1.045930;
   if (nvtx>=23.000000 && nvtx<24.000000) return 1.047821;
   if (nvtx>=24.000000 && nvtx<25.000000) return 1.042626;
   if (nvtx>=25.000000 && nvtx<26.000000) return 1.056079;
   if (nvtx>=26.000000 && nvtx<27.000000) return 1.068642;
   if (nvtx>=27.000000 && nvtx<28.000000) return 1.077145;
   if (nvtx>=28.000000 && nvtx<29.000000) return 1.090889;
   if (nvtx>=29.000000 && nvtx<30.000000) return 1.102057;
   if (nvtx>=30.000000 && nvtx<31.000000) return 1.089665;
   if (nvtx>=31.000000 && nvtx<32.000000) return 1.066530;
   if (nvtx>=32.000000 && nvtx<33.000000) return 1.037292;
   if (nvtx>=33.000000 && nvtx<34.000000) return 0.969761;
   if (nvtx>=34.000000 && nvtx<35.000000) return 0.900420;
   if (nvtx>=35.000000 && nvtx<36.000000) return 0.814996;
   if (nvtx>=36.000000 && nvtx<37.000000) return 0.701494;
   if (nvtx>=37.000000 && nvtx<38.000000) return 0.601403;
   if (nvtx>=38.000000 && nvtx<39.000000) return 0.492305;
   if (nvtx>=39.000000 && nvtx<40.000000) return 0.400363;
   if (nvtx>=40.000000 && nvtx<41.000000) return 0.308498;
   if (nvtx>=41.000000 && nvtx<42.000000) return 0.225170;
   if (nvtx>=42.000000 && nvtx<43.000000) return 0.162180;
   if (nvtx>=43.000000 && nvtx<44.000000) return 0.111863;
   if (nvtx>=44.000000 && nvtx<45.000000) return 0.077390;
   if (nvtx>=45.000000 && nvtx<46.000000) return 0.050697;
   if (nvtx>=46.000000 && nvtx<47.000000) return 0.031394;
   if (nvtx>=47.000000 && nvtx<48.000000) return 0.020052;
   if (nvtx>=48.000000 && nvtx<49.000000) return 0.012629;
   if (nvtx>=49.000000 && nvtx<50.000000) return 0.007430;
   return 0.;
}

float muonScaleFactor_2016(float pt, float eta) {
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

float trackingSF_2016(float eta) {
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

float electronScaleFactorHighHT_2016(float pt, float eta) {
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

float electronGSF_2016(float pt, float eta) {
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

float electronScaleFactorLowHT_2016(float pt, float eta) {
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

float leptonScaleFactor_2016(int pdgId, float pt, float eta, float ht) {
  if (abs(pdgId)==13) return muonScaleFactor_2016(pt, eta)*trackingSF_2016(eta);
  else if (abs(pdgId)==11){
    if (ht>300) return electronScaleFactorHighHT_2016(pt, eta)*electronGSF_2016(pt, eta);
      else return electronScaleFactorLowHT_2016(pt, eta)*electronGSF_2016(pt, eta);
  }
  return 0.;
}

float triggerScaleFactor_2016(int pdgId1, int pdgId2, float pt1, float pt2, float eta1, float eta2, float ht) {
    return TotalTriggerSF(pdgId1, pt1, eta1, pdgId2, pt2, eta2, ht);
}
