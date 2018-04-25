1. Less steps here. Edit `doAll.C` to have the location of the babies.

2. `root -b -q -l -n doAll.C` (takes a few minutes). If you don't change certain settings in the doAll script,
your output will be called `histos__LooseEMVA_coneCorr_HH.root`. Otherwise, you'll have to make sure it matches in
the plotting script. Some simple closure numbers are spit out as output before the harmless segfault.

3. `python plot.py` will put plots into `plots/`

4. There will be something inevitably wrong, so `cd ../derivation/`.
