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
./runLimitScriptThatDoesntExistYetButWillSoon.py
```


