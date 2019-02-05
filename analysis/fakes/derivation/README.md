## Measurement region for fakes

### Make lepton tree babies and get trigger normalizations

1. Make lepton tree babies from `../babymaker`

2. We need Z normalization factors for the various triggers. Compute these in `normalize_zpeak/` and then paste into the top part of `ScanChain_fast.C`.

### Run looper to compute fake rate

3. Edit `py_doAll.py` and `ScanChain_fast.C` with appropriate logic/information.

4. Run with `python py_doAll.py`

5. Edit and run `all.py` to compute the EWK normalization factors (via fit), and then compute all the fake rates and plot them. The output maps will go into, e.g., `frs_2017_temp.h`.

6. Next, on to the application region after updating the fake rate maps: `cd ../application`
