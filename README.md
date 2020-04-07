# fourtop
Looking for four top in all the right places.

<img src="http://i.imgur.com/k2FvE22.png" width="400">

Standard disclaimer
> No identification with actual persons (living or deceased),
> places, buildings, and products is intended or should be inferred.

## How to take over the country
### Prepare the campaign ("installation")
```bash
# winners use git
git clone https://github.com/cmstas/FTAnalysis
cd FTAnalysis
# winners use bash (but mainly python)
# execute from this directory and only this directory,
# every time you want to work in this repository
source setup.sh
```

### [Make babies](babymaking/batch/) (more babies = more votes)
```bash
cd babymaking/batch/
```

### [Count the votes](analysis/yields/) ("yields")
```bash
cd analysis/yields/
```

### [Show we are the best](analysis/limits/) ("limits")
```bash
cd analysis/limits/
```

## Hitting the ground running

### Quick start to get Run2 yields and combined significances
```
git clone https://github.com/cmstas/FTAnalysis
cd FTAnalysis
source /code/osgcode/cmssoft/cms/cmsset_default.sh
source setup.sh
cd $FTBASE/analysis/yields/
./py_doAll.py --shapes --plots --tag v3.09_quickstart_v0
cd $FTBASE/analysis/limits/
./run_all_limits.sh v3.09_quickstart_v0 --regions srcr
```

### Quick start to reproduce 2016 paper yields
```
# clone and set up
git clone https://github.com/cmstas/FTAnalysis
cd FTAnalysis
git checkout paper2017
source /code/osgcode/cmssoft/cms/cmsset_default.sh # might as well put in .bashrc
# patch setup to make sure we get the exact HiggsCombine hash
sed '/git checkout 74x-root6/agit checkout eea907cf928bf235579f015e9e6e6d1cd0a067b4' setup.sh
source setup.sh # configure ssh token with github since `git clone git@...` syntax used here

# get the yields
cd analysis/yields/
root -b -q -l do.C # there might be a harmless segfault at the end

# run the limits (making postfit histograms)
cd ../limits/
./runLimits.py v0.10_Oct3_test --redocard --redolimits --unblinded
# note: paper result has sigma=1.556 to make sure quick start is "calibrated"

# now go back and make the plots (which uses postfit histograms as input)
cd ../yields/
python plot.py # puts plots in `plots/`
```
