### So you want to show you're the best?

#### First time? Set up combine.
The combine documentation lives in [here](https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHiggsAnalysisCombinedLimit#How_to_run_the_tool)
```bash
# Assuming you've already run the setup.sh script in the main directory
pushd
cd $CMSSW_BASE/src
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git checkout 74x-root6
scramv1 b vclean
scramv1 b -j 15
popd
```

#### I want to run some limits.
```bash
# make cards
./createCard.py -h

# run limits (automatically makes cards if not there)
./runLimits.py -h

# print prefit yield table from cards
./printPrefitTable.py -h

# run single bin limits on a directory (makes cards if not there)
./singleBinLimits.py -h

# makes NLL scan with asimov dataset
./makeScan.py -h

# clean up stupid root files and kill runaway combine processes
./drainTheSwamp.py
```


