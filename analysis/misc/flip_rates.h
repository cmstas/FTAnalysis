float flipRate(float pt, float eta) {
    float scale = 1.51;
   if (pt>=15.0 && pt<40.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 2.18435660709e-05*scale;
   if (pt>=15.0 && pt<40.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 7.78790800364e-05*scale;
   if (pt>=15.0 && pt<40.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.000722802402378*scale;
   if (pt>=40.0 && pt<60.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 1.69790243449e-05*scale;
   if (pt>=40.0 && pt<60.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.000102342243818*scale;
   if (pt>=40.0 && pt<60.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.000771112102137*scale;
   if (pt>=60.0 && pt<80.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 1.29440566301e-05*scale;
   if (pt>=60.0 && pt<80.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.000151952982096*scale;
   if (pt>=60.0 && pt<80.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.00119904308503*scale;
   if (pt>=80.0 && pt<100.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 2.7866227964e-05*scale;
   if (pt>=80.0 && pt<100.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.000178155724029*scale;
   if (pt>=80.0 && pt<100.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.00143116618752*scale;
   if (pt>=100.0 && pt<200.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 4.86767869324e-05*scale;
   if (pt>=100.0 && pt<200.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.000382820064789*scale;
   if (pt>=100.0 && pt<200.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.00321468757702*scale;
   if (pt>=200.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 9.52243219009e-05*scale;
   if (pt>=200.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.00125209223419*scale;
   if (pt>=200.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.00571999621753*scale;
   return 0.;
}
float flipRateError(float pt, float eta) {
    float scale = 1.51;
   if (pt>=15.0 && pt<40.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 3.28926363695e-06*scale; // 15.1%
   if (pt>=15.0 && pt<40.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 7.68142970222e-06*scale; // 9.9%
   if (pt>=15.0 && pt<40.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 2.56822628026e-05*scale; // 3.6%
   if (pt>=40.0 && pt<60.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 2.79169117232e-06*scale; // 16.4%
   if (pt>=40.0 && pt<60.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 8.35541442991e-06*scale; // 8.2%
   if (pt>=40.0 && pt<60.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 2.65879971377e-05*scale; // 3.4%
   if (pt>=60.0 && pt<80.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 6.67764957013e-06*scale; // 51.6%
   if (pt>=60.0 && pt<80.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 3.24312080455e-05*scale; // 21.3%
   if (pt>=60.0 && pt<80.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.000105445190651*scale; // 8.8%
   if (pt>=80.0 && pt<100.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 1.51495623058e-05*scale; // 54.4%
   if (pt>=80.0 && pt<100.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 6.14897079202e-05*scale; // 34.5%
   if (pt>=80.0 && pt<100.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.000208413302702*scale; // 14.6%
   if (pt>=100.0 && pt<200.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 2.21658577568e-05*scale; // 45.5%
   if (pt>=100.0 && pt<200.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 8.52497127691e-05*scale; // 22.3%
   if (pt>=100.0 && pt<200.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.000358501949695*scale; // 11.2%
   if (pt>=200.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 4.30182803232e-05*scale; // 45.2%
   if (pt>=200.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.000527108960785*scale; // 42.1%
   if (pt>=200.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.00180973443199*scale; // 31.6%
   return 0.;
}
