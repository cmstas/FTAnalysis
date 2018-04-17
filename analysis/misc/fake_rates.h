float electronFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.244082;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.222025;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.188659;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0421375;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0783781;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0673139;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0632121;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0589394;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0601202;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0892793;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0941343;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0865112;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.159015;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.193473;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.133453;
   return 0.;
}
float electronFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0222764;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0251537;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0281177;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.00612273;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0100858;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0100988;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.00518558;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.00487818;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00506222;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.00993541;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0102271;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.00905256;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0262085;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0248379;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0181063;
   return 0.;
}
float electronAlternativeFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.244155;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.222085;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.188742;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0423019;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0784983;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0674737;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0638544;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0593757;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0605636;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0916962;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0964063;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0882057;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.164631;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.198142;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.137209;
   return 0.;
}
float muonFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.309832;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.408972;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.485772;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.072003;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.112084;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0855626;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0439507;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0531381;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.068734;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0417987;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0508508;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0942771;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0723428;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0560981;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0464978;
   return 0.;
}
float muonFakeRateError(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0129435;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0160188;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0329446;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00780544;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00750898;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0135309;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00218218;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00303943;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00676066;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00364013;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00474149;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0149072;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0121591;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0126004;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0267537;
   return 0.;
}
float muonAlternativeFakeRate(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.309829;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.408969;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.48577;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0719943;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.112076;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0855532;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0439242;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0531092;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0687045;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0416918;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0507542;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0941462;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0718578;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0556732;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0460408;
   return 0.;
}
float electronFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.294658;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.283482;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.222592;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0970226;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.16366;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.123141;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.191757;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.141326;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.142856;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.273293;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.241021;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.247253;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.46997;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.388314;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.264423;
   return 0.;
}
float electronFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0271085;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0324307;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0359084;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0135391;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0212557;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0205563;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.016295;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0131165;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0137263;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0262548;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0246127;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0222313;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0569915;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.0435232;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.0313979;
   return 0.;
}
float electronAlternativeFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.294735;
   if (pt>=10 && pt<15 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.283543;
   if (pt>=10 && pt<15 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.222699;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.0972948;
   if (pt>=15 && pt<25 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.163846;
   if (pt>=15 && pt<25 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.123417;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.193176;
   if (pt>=25 && pt<35 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.142219;
   if (pt>=25 && pt<35 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.14379;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.278127;
   if (pt>=35 && pt<50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.245143;
   if (pt>=35 && pt<50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.250671;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<0.8 ) return 0.478027;
   if (pt>=50 && fabs(eta)>=0.8 && fabs(eta)<1.479 ) return 0.394939;
   if (pt>=50 && fabs(eta)>=1.479 && fabs(eta)<2.5 ) return 0.269571;
   return 0.;
}
float muonFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.318972;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.408425;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.458146;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0780752;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.108356;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.126249;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0455878;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0545962;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0725825;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0414607;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0590616;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0918162;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0774624;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0590693;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0604607;
   return 0.;
}
float muonFakeRateError_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0120941;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0141439;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0299662;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0055892;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00725279;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0154824;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00215439;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00295834;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.00707409;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.00390978;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.00574184;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.015023;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0142545;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0158598;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0401517;
   return 0.;
}
float muonAlternativeFakeRate_IsoTrigs(float pt, float eta) {
   if (pt>=10 && pt<15 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.318966;
   if (pt>=10 && pt<15 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.408419;
   if (pt>=10 && pt<15 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.458142;
   if (pt>=15 && pt<25 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.078059;
   if (pt>=15 && pt<25 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.108339;
   if (pt>=15 && pt<25 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.126231;
   if (pt>=25 && pt<35 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0455374;
   if (pt>=25 && pt<35 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0545405;
   if (pt>=25 && pt<35 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0725247;
   if (pt>=35 && pt<50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0412577;
   if (pt>=35 && pt<50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0588573;
   if (pt>=35 && pt<50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.091565;
   if (pt>=50 && fabs(eta)>=0 && fabs(eta)<1.2 ) return 0.0764859;
   if (pt>=50 && fabs(eta)>=1.2 && fabs(eta)<2.1 ) return 0.0581053;
   if (pt>=50 && fabs(eta)>=2.1 && fabs(eta)<2.4 ) return 0.0592665;
   return 0.;
}
