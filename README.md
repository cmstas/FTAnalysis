# fourtop
Looking for four top in all the right places.

<img src="http://i.imgur.com/k2FvE22.png" width="400">

Standard disclaimer
> No identification with actual persons (living or deceased),
> places, buildings, and products is intended or should be inferred.

## How to take over the country
### Prepare the campaign
```bash
# winners use git
git clone https://github.com/cmstas/FTAnalysis
# winners use bash (but mainly python)
# execute from this directory and only this directory,
# every time you want to work in this repository
source setup.sh
```

### [Make babies](babymaking/batch/) (more babies = more votes)
```bash
cd babymaking/batch/
```

### [Count the votes](analysis/yields/)
```bash
cd analysis/yields/
```

### [Show we are the best](analysis/limits/)
```bash
cd analysis/limits/
```

## Hitting the ground running (quick start)
```
# clone and set up
git clone https://github.com/cmstas/FTAnalysis
cd FTAnalysis
source setup.sh

# get the yields
cd analysis/yields/
root -b -q -l do.C

# run the limits (making postfit histograms)
cd ../limits/
./runLimits.py v0.10_Oct3_test --redocard --redolimits --unblinded
# note: paper result has sigma=1.556 to make sure quick start is "calibrated"

# now go back and make the plots (which uses postfit histograms as input)
cd ../yields/
python plot.py # puts plots in `plots/`
```
