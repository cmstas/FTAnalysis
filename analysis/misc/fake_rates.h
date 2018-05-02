float electronFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.201199;
   if (pt>=15 && pt<20 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.27175;
   if (pt>=15 && pt<20 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.250587;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0963775;
   if (pt>=20 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.122828;
   if (pt>=20 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.102317;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0596173;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0770384;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0828341;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0978299;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0969798;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.100947;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.155702;
   if (pt>=50 && pt<70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.164853;
   if (pt>=50 && pt<70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.154025;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.283834;
   if (pt>=70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.312779;
   if (pt>=70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.270962;
   return 0.;
}
float electronFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0263322;
   if (pt>=15 && pt<20 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.034296;
   if (pt>=15 && pt<20 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0316408;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0190117;
   if (pt>=20 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0202145;
   if (pt>=20 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0212158;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.00461403;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.00550755;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00546555;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.010285;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.00965387;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00820706;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0254307;
   if (pt>=50 && pt<70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.02301;
   if (pt>=50 && pt<70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0161729;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0660854;
   if (pt>=70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0556699;
   if (pt>=70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0333519;
   return 0.;
}
float electronAlternativeFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.201199;
   if (pt>=15 && pt<20 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.27175;
   if (pt>=15 && pt<20 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.250587;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0963775;
   if (pt>=20 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.122828;
   if (pt>=20 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.102317;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0596173;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0770384;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0828341;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0978299;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0969798;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.100947;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.155702;
   if (pt>=50 && pt<70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.164853;
   if (pt>=50 && pt<70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.154025;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.283834;
   if (pt>=70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.312779;
   if (pt>=70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.270962;
   return 0.;
}
float electronQCDMCFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.286641;
   if (pt>=15 && pt<20 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.289538;
   if (pt>=15 && pt<20 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.320237;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.122423;
   if (pt>=20 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.140543;
   if (pt>=20 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.1662;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0730217;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.112091;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.14065;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.108176;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.115992;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.139436;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.15206;
   if (pt>=50 && pt<70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.226249;
   if (pt>=50 && pt<70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.190889;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.278822;
   if (pt>=70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.30058;
   if (pt>=70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.299518;
   return 0.;
}
float electronQCDMCFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0179672;
   if (pt>=15 && pt<20 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0249113;
   if (pt>=15 && pt<20 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0281407;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0148393;
   if (pt>=20 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0190877;
   if (pt>=20 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0212239;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.00883853;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0160654;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0160101;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0169397;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0177498;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0191835;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0273717;
   if (pt>=50 && pt<70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0392315;
   if (pt>=50 && pt<70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.034857;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0371682;
   if (pt>=70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0410107;
   if (pt>=70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0311908;
   return 0.;
}
float muonFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.319009;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.361743;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.411638;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0898978;
   if (pt>=15 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.174819;
   if (pt>=15 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.193705;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0394799;
   if (pt>=20 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0656867;
   if (pt>=20 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.075447;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0456297;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0498856;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0676442;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0408914;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0542565;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0690335;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0617261;
   if (pt>=50 && pt<70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.072451;
   if (pt>=50 && pt<70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.149303;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.144608;
   if (pt>=70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0666672;
   if (pt>=70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   return 0.;
}
float muonFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0132389;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0160981;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0326047;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00792901;
   if (pt>=15 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0207252;
   if (pt>=15 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0303755;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00631081;
   if (pt>=20 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00982633;
   if (pt>=20 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0200564;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00215025;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00329285;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0067392;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0044093;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00571458;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0132998;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0134675;
   if (pt>=50 && pt<70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0149665;
   if (pt>=50 && pt<70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0456383;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0443325;
   if (pt>=70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0379481;
   if (pt>=70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0789102;
   return 0.;
}
float muonAlternativeFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.319009;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.361743;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.411638;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0898978;
   if (pt>=15 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.174819;
   if (pt>=15 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.193705;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0394799;
   if (pt>=20 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0656867;
   if (pt>=20 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.075447;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0456297;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0498856;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0676442;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0408914;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0542565;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0690335;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0617261;
   if (pt>=50 && pt<70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.072451;
   if (pt>=50 && pt<70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.149303;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.144608;
   if (pt>=70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0666672;
   if (pt>=70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   return 0.;
}
float muonQCDMCFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.341741;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.416695;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.427799;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.105342;
   if (pt>=15 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.143614;
   if (pt>=15 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.144438;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0762528;
   if (pt>=20 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0905147;
   if (pt>=20 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0906184;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0557719;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0651419;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0754305;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0582006;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0625352;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0703365;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0592371;
   if (pt>=50 && pt<70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0684556;
   if (pt>=50 && pt<70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0684345;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0594973;
   if (pt>=70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0491275;
   if (pt>=70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0770299;
   return 0.;
}
float muonQCDMCFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00623083;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00829146;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0142216;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00382055;
   if (pt>=15 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00569991;
   if (pt>=15 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00956422;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00370558;
   if (pt>=20 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00465703;
   if (pt>=20 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00879755;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00265863;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00384452;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00726145;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00370219;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00445069;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0094944;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00552773;
   if (pt>=50 && pt<70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00884159;
   if (pt>=50 && pt<70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0121543;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00610976;
   if (pt>=70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00750984;
   if (pt>=70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0221746;
   return 0.;
}
float electronFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.251015;
   if (pt>=15 && pt<20 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.327886;
   if (pt>=15 && pt<20 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.290013;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.152354;
   if (pt>=20 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.183855;
   if (pt>=20 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.1624;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.143982;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.154433;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.181773;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.255648;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.241511;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.226873;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.429321;
   if (pt>=50 && pt<70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.399234;
   if (pt>=50 && pt<70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.28502;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.599063;
   if (pt>=70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.499801;
   if (pt>=70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.388264;
   return 0.;
}
float electronFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0314925;
   if (pt>=15 && pt<20 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0399951;
   if (pt>=15 && pt<20 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.037598;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0285951;
   if (pt>=20 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.031849;
   if (pt>=20 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0354462;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0124793;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0123167;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0120675;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0258581;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0211023;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0170732;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0475718;
   if (pt>=50 && pt<70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0408263;
   if (pt>=50 && pt<70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0276114;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0675133;
   if (pt>=70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0726012;
   if (pt>=70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0422192;
   return 0.;
}
float electronAlternativeFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.251015;
   if (pt>=15 && pt<20 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.327886;
   if (pt>=15 && pt<20 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.290013;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.152354;
   if (pt>=20 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.183855;
   if (pt>=20 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.1624;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.143982;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.154433;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.181773;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.255648;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.241511;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.226873;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.429321;
   if (pt>=50 && pt<70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.399234;
   if (pt>=50 && pt<70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.28502;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.599063;
   if (pt>=70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.499801;
   if (pt>=70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.388264;
   return 0.;
}
float electronQCDMCFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.368489;
   if (pt>=15 && pt<20 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.353314;
   if (pt>=15 && pt<20 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.342813;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.198769;
   if (pt>=20 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.23454;
   if (pt>=20 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.253641;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.155658;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.207813;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.249177;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.233512;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.212333;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.2534;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.271108;
   if (pt>=50 && pt<70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.363175;
   if (pt>=50 && pt<70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.329684;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.506294;
   if (pt>=70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.423658;
   if (pt>=70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.365134;
   return 0.;
}
float electronQCDMCFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0239698;
   if (pt>=15 && pt<20 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0304988;
   if (pt>=15 && pt<20 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0346636;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0248803;
   if (pt>=20 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0319;
   if (pt>=20 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0328811;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.020221;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0296743;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0283797;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0350354;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0338762;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0349765;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0505829;
   if (pt>=50 && pt<70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0632304;
   if (pt>=50 && pt<70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.055762;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0574432;
   if (pt>=70 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0622989;
   if (pt>=70 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0391698;
   return 0.;
}
float muonFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.327241;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.398117;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.51883;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0896082;
   if (pt>=15 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.145858;
   if (pt>=15 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.178783;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.047268;
   if (pt>=20 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.068064;
   if (pt>=20 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0754655;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0477584;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0574743;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0781997;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0453657;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0546735;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0975147;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0559244;
   if (pt>=50 && pt<70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0850034;
   if (pt>=50 && pt<70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.162929;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.108952;
   if (pt>=70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.101923;
   if (pt>=70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   return 0.;
}
float muonFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0119611;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.014903;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0330991;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00689645;
   if (pt>=15 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0146401;
   if (pt>=15 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0243575;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00632666;
   if (pt>=20 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00930615;
   if (pt>=20 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0188966;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00237653;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00335257;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00776849;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00472764;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00606128;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0181067;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0144602;
   if (pt>=50 && pt<70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.018044;
   if (pt>=50 && pt<70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0499182;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0436465;
   if (pt>=70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0517722;
   if (pt>=70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.171083;
   return 0.;
}
float muonAlternativeFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.327241;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.398117;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.51883;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0896082;
   if (pt>=15 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.145858;
   if (pt>=15 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.178783;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.047268;
   if (pt>=20 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.068064;
   if (pt>=20 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0754655;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0477584;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0574743;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0781997;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0453657;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0546735;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0975147;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0559244;
   if (pt>=50 && pt<70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0850034;
   if (pt>=50 && pt<70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.162929;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.108952;
   if (pt>=70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.101923;
   if (pt>=70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0;
   return 0.;
}
float muonQCDMCFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.341928;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.417727;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.429419;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.105653;
   if (pt>=15 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.145056;
   if (pt>=15 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.146002;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0768457;
   if (pt>=20 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0921339;
   if (pt>=20 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0926839;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0564312;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0670055;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0771854;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0590234;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0652197;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0743404;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0611789;
   if (pt>=50 && pt<70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0724815;
   if (pt>=50 && pt<70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0708633;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0615759;
   if (pt>=70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0547282;
   if (pt>=70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0817403;
   return 0.;
}
float muonQCDMCFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00623736;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0083217;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0142751;
   if (pt>=15 && pt<20 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00383292;
   if (pt>=15 && pt<20 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00575624;
   if (pt>=15 && pt<20 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00966308;
   if (pt>=20 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00373324;
   if (pt>=20 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00474823;
   if (pt>=20 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00901155;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00269129;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0039575;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00744669;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00375652;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00465315;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0100123;
   if (pt>=50 && pt<70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00571372;
   if (pt>=50 && pt<70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0093701;
   if (pt>=50 && pt<70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0128454;
   if (pt>=70 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00635514;
   if (pt>=70 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0082114;
   if (pt>=70 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.023859;
   return 0.;
}
