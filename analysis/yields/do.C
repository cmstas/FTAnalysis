{
  gROOT->ProcessLine(".L ../misc/class_files/v8.02/SS.cc++"); 
  // gSystem->Load("../../common/CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc++");
  gROOT->ProcessLine(".L yieldMaker.C++"); 

  TString options_2016 = "Data2016 evaluateBDT ";
  TString options_2017 = "Data2017 minPtFake18 evaluateBDT ";
  TString outputdir = "outputs";

  gSystem->Exec(Form("mkdir -p %s", outputdir.Data()));

  // Note chain titles are important since they get parsed in yieldMaker

  TChain tttt_chain("t","tttt");
  tttt_chain.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2017/TTTTnew.root");
  getyields(&tttt_chain, options_2017, outputdir);

  TChain ttw_chain("t","ttw");
  ttw_chain.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2017/TTWnlo.root");
  getyields(&ttw_chain, options_2017, outputdir);

  // TChain ttw_chain("t","fakes_mc");
  // ttw_chain.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2017/TTBAR_*.root");
  // getyields(&ttw_chain, options_2017 + " doFakesMC ", outputdir);

  // TChain ttw_chain("t","fakes");
  // ttw_chain.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2017/Data*.root");
  // getyields(&ttw_chain, options_2017 + " doFakes ", outputdir);

  // TChain ttw_chain("t","fakes");
  // ttw_chain.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2016/Data*.root");
  // getyields(&ttw_chain, options_2016 + " doFakes ", outputdir);

  // TChain tttt_chain("t","tttt");
  // tttt_chain.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2016/TTTTnew.root");
  // getyields(&tttt_chain, options_2016, outputdir);

  // TChain ttttisrup_chain("t","ttttisrup");
  // ttttisrup_chain.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2016/TTTTisrup.root");
  // getyields(&ttttisrup_chain, options_2016, outputdir);

  // TChain ttttisrdn_chain("t","ttttisrdn");
  // ttttisrdn_chain.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2016/TTTTisrdown.root");
  // getyields(&ttttisrdn_chain, options_2016, outputdir);

  // TChain ttttfsrup_chain("t","ttttfsrup");
  // ttttfsrup_chain.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2016/TTTTfsrup.root");
  // getyields(&ttttfsrup_chain, options_2016, outputdir);

  // TChain ttttfsrdn_chain("t","ttttfsrdn");
  // ttttfsrdn_chain.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2016/TTTTfsrdown.root");
  // getyields(&ttttfsrdn_chain, options_2016, outputdir);

}
