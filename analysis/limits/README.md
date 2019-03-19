### Same sign (SUS)

#### Run upper limits

* Assuming you've run `py_doAll.py` and ended up with a folder here,
edit `make_commands.sh` to point to the relevant directory and make sure
the glob string for `procs` includes what you want. 

* Then run `./make_commands.sh > commands.txt`
to populate a text file with some commands which will make ROOT workspaces from the cards.
You can run a few of them interactively, or use GNU parallel (`./parallel --jobs 30 --bar < commands.txt`)
to speed up the process. Then you will end up with `${basedir}/card_*_run2.root` files.
  * NOTE: If running parallel, you need to run one of the commands serially first (there is a race condition otherwise :( ). I need to fix it eventually.

* Now you can run limits with HiggsCombine via `combine -M AsymptoticLimits basedir/card_blah.root`.  For batch submission, see `batch/`.

### Four top (TOP)

The combine documentation lives in [here](https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHiggsAnalysisCombinedLimit#How_to_run_the_tool)

#### Make some limits, but remember that *America has no limits*.
```bash
# run limits (automatically makes cards if not there)
./runLimits.py -h

# print prefit/postfit yield tables after fitting
./printTables.py -h

# plot NLL scan using output of runLimits.py
./plot1DScan.py -h

# clean up stupid root files and kill runaway combine processes
./drainTheSwamp.py
```

#### Handy commands for 2016+2017 combination

##### Combined significance

Make sure `makeCombinedCard.py` has the correct correlation model. Then,
`./run_all_limits <dir>`.



##### Impact plots
Instructions are at https://twiki.cern.ch/twiki/bin/view/CMS/HiggsWG/SWGuideNonStandardCombineUses#Nuisance_parameter_impacts

To install,
```bash
cd $CMSSW_BASE/src
git clone https://github.com/cms-analysis/CombineHarvester.git CombineHarvester
cd CombineHarvester
scram b -j 15
echo "This may not compile fully but it's fine. Just ignore it and proceed."
```

Then take a card (`combined_uncorrelated.txt` for example) and do
```bash
# make combined_uncorrelated.root
text2workspace.py combined_uncorrelated.txt
cardname=combined_uncorrelated.root
# do initial fit (~20 secs)
combineTool.py -d ${cardname} -M Impacts --robustFit 1 -t -1 --expectSignal=1 -m 125 --doInitialFit
# do 15 parallel fits for all ~200 nuisances (~2 min)
combineTool.py -d ${cardname} -M Impacts --robustFit 1 -t -1 --expectSignal=1 -m 125 --doFits --parallel 15 
# there will be ~200 root files here, but we'll clean them up later
combineTool.py -M Impacts -d ${cardname} -m 125 -o impacts.json
# make plots in `impacts.pdf`
plotImpacts.py -i impacts.json -o impacts
# clean up
rm *_paramFit_*
```

