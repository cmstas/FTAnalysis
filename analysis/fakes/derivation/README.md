1. Make lepton tree babies from LeptonBabyMaker repository. For 2017 data, the appropriate branch is `2017`.

2. We need Z normalization factors for the various triggers. Normally this is done with a scan statement over all the data
and DY MC, but this takes many hours. Luckily when we made the lepton tree babies in step 1, we also made histograms from them using
Metis. These histograms have the mll distributions we need to normalize, so hadd them with `. do_hadd.sh` after editing in the correct path
and tag. Then run `python get_zsfs.py` to make plots of the Z peaks and spit out C++ code for normalization in `ScanChain.C`. Copy in these lines
over the previous lines in `ScanChain.C`.

3. Edit `ScanChain.C` with appropriate logic/information.

4. Some setup:
```
cp ../../limits/errors.py .
git clone https://github.com/aminnj/plottery
mkdir -p outputs/
mkdir -p pdfs/
mkdir -p plots/
```

5. A sample `doAll` script is available, but it's really slow. So we use `parallelize.py` to write one per sample and run them in parallel. 
Edit `parallelize.py` with the appropriate sample locations and tag. Note that outputs go into `outputs/`.

6. `python parallelize.py` will make multiple files with pattern `.parallel*`. You can run them individually or just use `. run_parallel.sh`.
Do this in a screen since the processes don't get `nohup`'d. This can take up to 3 or so hours depending on the largest sample.

7. Once they are done, we can extract the mT SFs and plot corrected fake rates. Edit `do_fits.py` to have the correct output directory from the 
parallel running on line 8 in the `get_fname` function. The output directory for plots is on line 12, `thedir`. Run the script with `python do_fits.py` and note the
four lines at the bottom. These are the mT SFs that you need to copy and paste into `all.C`.

8. Edit `all.C` to have the correct output directory from the ScanChain. Also paste in the correct mT SFs from the previous step. Run with
`root -b -q -l -n all.C` to make the corrected FRs and other plots too, in `pdfs/`. 

9. Now, edit `dumpFunctions.C` to have the correct ScanChain output near the top, and run `root -b -q -l -n dumpFunctions.C > fake_rates.h`. Clean up 
the top and the bottom of this header file, and you now have maps that can be placed into (or override) `../../misc/fake_rates.h`.

10. Next, on to the application region after updating the fake rate maps.
