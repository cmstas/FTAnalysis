float flipRate(float pt, float eta) {
    float scale = 1.44;
   if (pt>=15.0 && pt<40.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 2.57848449393e-05*scale;
   if (pt>=15.0 && pt<40.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.000100001476552*scale;
   if (pt>=15.0 && pt<40.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.000759164737562*scale;
   if (pt>=40.0 && pt<60.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 2.29310358346e-05*scale;
   if (pt>=40.0 && pt<60.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.000108643363828*scale;
   if (pt>=40.0 && pt<60.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.000822999050219*scale;
   if (pt>=60.0 && pt<80.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 2.09211466173e-05*scale;
   if (pt>=60.0 && pt<80.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.000174044127827*scale;
   if (pt>=60.0 && pt<80.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.00116438034111*scale;
   if (pt>=80.0 && pt<100.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 2.81946848624e-05*scale;
   if (pt>=80.0 && pt<100.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.000207875385845*scale;
   if (pt>=80.0 && pt<100.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.00148457386015*scale;
   if (pt>=100.0 && pt<200.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 7.51495783061e-05*scale;
   if (pt>=100.0 && pt<200.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.000387472612312*scale;
   if (pt>=100.0 && pt<200.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.00317953266448*scale;
   if (pt>=200.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 0.000125935710133*scale;
   if (pt>=200.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.00166070229342*scale;
   if (pt>=200.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.00555874673691*scale;
   return 0.;
}
float flipRateError(float pt, float eta) {
    float scale = 1.44;
   if (pt>=15.0 && pt<40.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 3.50182362222e-06*scale;
   if (pt>=15.0 && pt<40.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 9.22848493895e-06*scale;
   if (pt>=15.0 && pt<40.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 2.55655082457e-05*scale;
   if (pt>=40.0 && pt<60.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 3.33933193974e-06*scale;
   if (pt>=40.0 && pt<60.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 8.89422836051e-06*scale;
   if (pt>=40.0 && pt<60.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 2.75357100093e-05*scale;
   if (pt>=60.0 && pt<80.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 8.64436721854e-06*scale;
   if (pt>=60.0 && pt<80.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 3.52012673786e-05*scale;
   if (pt>=60.0 && pt<80.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.000105944329844*scale;
   if (pt>=80.0 && pt<100.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 1.80452164275e-05*scale;
   if (pt>=80.0 && pt<100.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 6.40994538988e-05*scale;
   if (pt>=80.0 && pt<100.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.000218211652992*scale;
   if (pt>=100.0 && pt<200.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 3.00084307415e-05*scale;
   if (pt>=100.0 && pt<200.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 9.01104098992e-05*scale;
   if (pt>=100.0 && pt<200.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.000371251701405*scale;
   if (pt>=200.0 && fabs(eta)>=0.000 && fabs(eta)<0.800 ) return 5.23142245565e-05*scale;
   if (pt>=200.0 && fabs(eta)>=0.800 && fabs(eta)<1.479 ) return 0.00067650484727*scale;
   if (pt>=200.0 && fabs(eta)>=1.479 && fabs(eta)<2.500 ) return 0.00194617279471*scale;
   return 0.;
}
