#!/bin/bash

hostname
uname -a

# Patch1 not visible on condor nodes, so use nominal CMSSW release
if [ "$CMSSW_VER" == "CMSSW_8_0_5_patch1" ]; then CMSSW_VER=CMSSW_8_0_5; fi
export SCRAM_ARCH=$SCRAM_VER
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /cvmfs/cms.cern.ch/$SCRAM_VER/cms/cmssw/$CMSSW_VER/src/
eval `scramv1 runtime -sh`
cd -

# Split Output files line into an array
OUTPUT_NAMES=(`echo $OUTPUT_NAMES | sed s/,/" "/g`)
echo "OUTPUT_NAMES=${OUTPUT_NAMES[*]}"

# Split Extra Args into an array 
EXE_ARGS=(`echo $EXE_ARGS | sed s/,/" "/g`)
echo "EXE_ARGS=${EXE_ARGS[*]}"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

ls -l

echo "running: ./main.exe $FILENAME"
./main.exe $FILENAME

ls -l
