## Pileup weight derivation

1. For 2017 MC/data, log into lxplus, get a 92X CMSSW environment and execute
```bash
mainjson=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt
pujson=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PileUp/pileup_latest.txt
pileupCalc.py -i $mainjson --inputLumiJSON $pujson --calcMode true --minBiasXsec 69200 --maxPileupBin 90 --numPileupBins 90 MyDataPileupHistogram.root
pileupCalc.py -i $mainjson --inputLumiJSON $pujson --calcMode true --minBiasXsec 72660 --maxPileupBin 90 --numPileupBins 90 MyDataPileupHistogramUp.root
pileupCalc.py -i $mainjson --inputLumiJSON $pujson --calcMode true --minBiasXsec 65740 --maxPileupBin 90 --numPileupBins 90 MyDataPileupHistogramDown.root
```
and copy the output histograms here

2. Edit `trueNumVtx.C` to have a baby with high statistics (e.g., DY with OS included), then run with `root -b -q -l trueNumVtx.C`.
3. `root -b -q -l makePUw.C > out.h` will write out the C++ functions for the weights and write out a ROOT file called `puw_2017.root`.
4. Make sure bins are not crazy.


### 2018
```bash
# with recorded luminosity : ~60 /fb
mainjson=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/PromptReco/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt
pujson=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/PileUp/pileup_latest.txt
pileupCalc.py -i $mainjson --inputLumiJSON $pujson --calcMode true --minBiasXsec 69200 --maxPileupBin 100 --numPileupBins 100 MyDataPileupHistogram.root
pileupCalc.py -i $mainjson --inputLumiJSON $pujson --calcMode true --minBiasXsec 72660 --maxPileupBin 100 --numPileupBins 100 MyDataPileupHistogramUp.root
pileupCalc.py -i $mainjson --inputLumiJSON $pujson --calcMode true --minBiasXsec 65740 --maxPileupBin 100 --numPileupBins 100 MyDataPileupHistogramDown.root
```
* Then run `python trueNumVtx.py` after appropriate edits. I checked that the CMS4 MC distribution matches the "true" one from the CMSSW config file, so I used
the latter for stats.
