#!/usr/bin/env bash

# Data
# ./main.exe /hadoop/cms/store/user/namin/ProjectMetis/DoubleEG_Run2017B-PromptReco-v2_MINIAOD_CMS4_V00-00-06/merged_ntuple_12.root test_output_Data_1.root 20000
# ./main.exe /hadoop/cms/store/user/namin/ProjectMetis/DoubleEG_Run2017D-PromptReco-v1_MINIAOD_CMS4_V00-00-06/merged_ntuple_12.root test_output_Data_1.root 20000
./main.exe /hadoop/cms/store/user/namin/ProjectMetis/MuonEG_Run2017D-PromptReco-v1_MINIAOD_CMS4_V00-00-06/merged_ntuple_6.root test_output_Data_1.root 100000

# # MC
# ./main.exe /hadoop/cms/store/user/namin/ProjectMetis/TTTT_TuneCP5_13TeV-amcatnlo-pythia8_RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1_MINIAODSIM_CMS4_V00-00-10_test/merged_ntuple_1.root test_output_MC_1.root 1000

# wait

echo $?
