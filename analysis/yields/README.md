### Important files

#### `py_doAll.py`
* This contains locations of ntuples, option strings, TChain contents, other configuration, and automatically invokes the next 3 scripts
* Note the two sets of stuff to switch back and forth between TOP and SUS analyses :(

#### `yieldMaker.C`
* This is the actual looper

#### `plot_all.py`
* Convoluted plotter. Multi-threaded since there's lots of plots (x4 for 3 years and the sum)

#### `make_shape_hists.py`
* Script that massages output histograms from looper into a format usable with cards (infrastructure in the `../limits` directory)


### Instructions

#### Example running

`python py_doAll.py -h` gives more options, but to run the SUS same sign analysis, do
```bash
python py_doAll.py --tag v3.28_ss_mar13final_v1  --ss --fastsim --ncpu 20 --slim --plots --shapes
```

This runs the looper on ~200 TChains (roughly number of background/data/signal
processes times the number of years) with SUS settings/configuration (`--ss`).
Include fastsim signals (`--fastsim`), but only some mass points (`--slim`) so
that the looper runs in minutes instead of hours. Use 20 cores to loop (`--ncpu`).
Make plots (`--plots`) and make template histograms for cards/limits (`--shapes`).
The important `--tag` parameter is for bookkeeping. A folder with this name containing
histograms and plots will be copied to the `../limits/` directory after running.

As an example, the ttW TChain for 2018 will get a rootfile named
`output_2018_ttw.root` in `outputs/` after the looper is run. With the `--shapes` flag,
the `make_shape_hists.py` script is called after the looper is run. That file would be converted into
`ttw_histos_srhh_2018.root`, ..., `ttw_histos_srlm_2018.root` in `outputs/`, one per "kinematic region"
(5 in total: SRHH, SRHL, SRLL, SRML, SRLM).

After running this, look at the instructions in `../limits/`.

#### Running full signal scans
The script `./runsignals.sh` will run the looper on subsets sequentially
(`--proc` parameter restricts to certain TChains with globbers and `--year` to certain years).
Note that only at the end do we make the histograms for the cards for all points with `--shapes`,
and we don't need to loop on anything at that point, so `--noloop`. 
This takes typically 1 hour to loop over everything and make the
histograms for cards and copy to `../limits`.

