{

  gROOT->ProcessLine(".L ../../misc/common_utils.h");
  gSystem->Load("../../../common/CORE/CMS3_CORE.so");

  gROOT->ProcessLine(".L ScanChain_fast.C+");

  TChain *data = new TChain("t"); 

  // note this file only works with ScanChain.C which is out of date. will need to remake with newer LeptonBabyMaker code
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017C-31Mar2018-v1_LEPTON_v8/output_2*.root");

  std::cout <<  " data->GetEntries(): " << data->GetEntries() <<  std::endl;
  ScanChain(data,"./rate_histos_data_LooseEMVA.root","useLooseEMVA"); 
  ScanChain(data,"./rate_histos_data_LooseEMVA_IsoTrigs.root","useLooseEMVA,IsoTrigs"); 

}
