{

  gROOT->ProcessLine(".L ../../misc/common_utils.h");
  gSystem->Load("../../../common/CORE/CMS3_CORE.so");

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *data = new TChain("t"); 

  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017B-PromptReco-v1_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017B-PromptReco-v2_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017C-PromptReco-v1_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017C-PromptReco-v2_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017C-PromptReco-v3_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017D-PromptReco-v3_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017E-PromptReco-v1_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_SingleElectron_Run2017F-PromptReco-v1_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017B-PromptReco-v1_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017B-PromptReco-v2_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017C-PromptReco-v1_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017C-PromptReco-v2_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017C-PromptReco-v3_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017D-PromptReco-v3_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017E-PromptReco-v1_LEPTON_v1/*.root");
  data->Add("/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/FakeRate_DoubleMuon_Run2017F-PromptReco-v1_LEPTON_v1/*.root");

  std::cout <<  " data->GetEntries(): " << data->GetEntries() <<  std::endl;
  ScanChain(data,"./rate_histos_data_LooseEMVA.root","useLooseEMVA"); 
  ScanChain(data,"./rate_histos_data_LooseEMVA_IsoTrigs.root","useLooseEMVA,IsoTrigs"); 

}
