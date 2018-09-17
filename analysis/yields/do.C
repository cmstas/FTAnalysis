{
  gROOT->ProcessLine(".L ../misc/class_files/v8.02/SS.cc+"); 
  // gSystem->Load("../../common/CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc+");
  gROOT->ProcessLine(".L yieldMaker.C+"); 

  TChain tttt_chain("t","tttt");
  tttt_chain.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.05_all/output/year_2017/TTTTnew.root");
  TString options = "";

  getyields(&tttt_chain, options);
}
