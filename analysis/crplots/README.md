
## CR plots

Makes plots for the opposite-sign and tight-loose control regions (application regions).

```
# edit doAll.C to have correct bools in `options`
# and correct `tag`, `basedir`
root -b -q -l doAll.C
# output goes into `outputs/`
python plot.py
# output goes into `plots/`
```
