### So you want to show you're the best?

The combine documentation lives in [here](https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHiggsAnalysisCombinedLimit#How_to_run_the_tool)

#### Make some limits, but remember that *America has no limits*.
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

#### Handy commands for 2016+2017 combination

##### Combined significance

Copy 2016 and 2017 directories here, then
`./runLimits.py <2016 dir> --redolimits --redocard --noscan`
and repeat for 2017.

Then edit `makeCombinedCard.py` to have the correct `basedirs`, correlation model, and `region`. Run with `python makeCombinedCard.py`. 
`combined_uncorrelated.txt` will be created. You can then run somehting like 
```
combine -M Significance combined_uncorrelated.txt -t -1 --expectSignal=1
```
to get the combined significance (add `--unblinded` to the `runLimits.py` invocation and remove `-t -1 --expectSignal=1` when unblinding).


##### Impact plots
Instructions are at https://twiki.cern.ch/twiki/bin/view/CMS/HiggsWG/SWGuideNonStandardCombineUses#Nuisance_parameter_impacts

To install,
```bash
cd $CMSSW_BASE/src
git clone https://github.com/cms-analysis/CombineHarvester.git CombineHarvester
cd CombineHarvester
scram b -j 15
```

Then take a card (`combined_uncorrelated.txt` for example) and do
```bash
# make combined_uncorrelated.root
text2workspace.py combined_uncorrelated.txt
# do initial fit (~20 secs)
combineTool.py -d combined_uncorrelated.root -M Impacts --doInitialFit --robustFit 1 -m 125  -t -1 --expectSignal=1
# do 15 parallel fits for all ~200 nuisances (~5 min)
combineTool.py -d combined_uncorrelated.root -M Impacts --robustFit 1 --doFits -m 125 --parallel 15  -t -1 --expectSignal=1
# there will be ~200 root files here, but we'll clean them up later
combineTool.py -M Impacts -d combined_uncorrelated.root -m 125 -o impacts.json
# make plots in `impacts.pdf`
plotImpacts.py -i impacts.json -o impacts
# clean up
rm *_paramFit_*
```

