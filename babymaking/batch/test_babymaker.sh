#!/usr/bin/env bash

# Data
# ./main.exe /hadoop/cms/store/user/namin/ProjectMetis/DoubleEG_Run2017B-PromptReco-v2_MINIAOD_CMS4_V00-00-06/merged_ntuple_12.root test_output_Data_1.root 20000
# ./main.exe /hadoop/cms/store/user/namin/ProjectMetis/DoubleEG_Run2017D-PromptReco-v1_MINIAOD_CMS4_V00-00-06/merged_ntuple_12.root test_output_Data_1.root 20000
# ./main.exe /hadoop/cms/store/user/namin/ProjectMetis/MuonEG_Run2017D-PromptReco-v1_MINIAOD_CMS4_V00-00-06/merged_ntuple_6.root test_output_Data_1.root 100000

# MC
# ./main.exe /hadoop/cms/store/user/namin/ProjectMetis/TTTT_TuneCP5_13TeV-amcatnlo-pythia8_RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1_MINIAODSIM_CMS4_V00-00-10_test/merged_ntuple_1.root test_output_MC_1.root 10000
# ./main.exe /hadoop/cms/store/group/snt/run2_mc2017//TTTT_TuneCP5_13TeV-amcatnlo-pythia8_RunIIFall17MiniAODv2-PU2017_pilot_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-13/merged_ntuple_1.root test_output_MC_2.root 10000
# ./main.exe /hadoop/cms/store/group/snt/run2_mc2017//DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017RECOSIMstep_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-13/merged_ntuple_1.root test_output_DY_1.root 1000



# # synch with 2017 --
# # /home/users/namin/2018/fourtop/94x/FTAnalysis/babymaking/batch/test_babymaker.sh
# ./main.exe /hadoop/cms/store/group/snt/run2_mc2017_test//TTTT_TuneCP5_PSweights_13TeV-amcatnlo-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_test_CMS4_V09-04-13/merged_ntuple_1.root test_synch_2017_tttt_1.root 20000
# ./main.exe /hadoop/cms/store/group/snt/run2_mc2017//DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017RECOSIMstep_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-13/merged_ntuple_1.root test_synch_2017_DY_1.root 100000
# ./main.exe /hadoop/cms/store/group/snt/run2_data2017//DoubleEG_Run2017F-31Mar2018-v1_MINIAOD_CMS4_V09-04-12/merged_ntuple_100.root test_synch_2017_DataDoubleEG_1.root 100000

# # synch with 2016 --
# # /home/users/namin/2018/fourtop/80x/FTAnalysis/babymaking/batch/test_babymaker.sh
# ./main.exe /hadoop/cms/store/group/snt/run2_data2016//MuonEG_Run2016C-03Feb2017-v1_MINIAOD_CMS4_V00-00-02_data2016/merged_ntuple_42.root test_synch_2016_DataMuonEGC_1.root 100000
./main.exe /hadoop/cms/store/user/namin/run2_moriond17_cms4/ProjectMetis/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_CMS4_V00-00-02_2017Sep27/merged_ntuple_4.root test_synch_2016_WZ_1.root 100000
# ./main.exe /hadoop/cms/store/user/namin/run2_moriond17_cms4/ProjectMetis/TTTT_TuneCUETP8M2T4_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_CMS4_V00-00-02_2017Sep27/merged_ntuple_1.root test_synch_2016_tttt_1.root 20000

# wait

echo $?
