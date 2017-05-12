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


