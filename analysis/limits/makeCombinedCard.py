#!/usr/bin/env python

import ROOT as r
import os
import sys

to_correlate = [
        "scale",
        "pdf",
        "alphas",
        "bb",
        "TTH",
        "TTWSF",
        "TTZSF",
        "rares",
        "XG",
        "TTVV",
        "fakes",
        ]
correlate_all = False

region = "srcr"

# basedirs = ["v1.04_baseline/"]
# years = [2017]
# procs = ["tttt"]

# basedirs = ["v1.00_2016_35p9_v1","v1.04_baseline"]
basedirs = ["v1.00_2016_35p9_18bins_v1","v1.04_baseline_v2_41p3_18bins"]
years = [2016,2017]
procs = ["data", "fakes", "fakes_mc", "flips", "rares", "tth", "tttt", "ttvv", "ttw", "ttz", "xg"]

output_cardname = "combined_uncorrelated.txt"
new_prefix = "comb_"

for basedir,year in zip(basedirs,years):
    new_basedir = "{}{}".format(new_prefix,basedir)
    os.system("mkdir -p {}".format(new_basedir))
    nuisance_map = {}
    for proc in procs:
        if year == 2017: lumi = 41.3
        else: lumi = 35.9
        basename = "{}_histos_{}_{}ifb.root".format(proc,region,lumi)
        fin = r.TFile("{}/{}".format(basedir,basename))
        fout = r.TFile("{}/{}".format(new_basedir,basename), "RECREATE")

        keys = [x.GetName() for x in fin.GetListOfKeys()]
        for key in keys:
            do_correlate = any(tc+"Up"==key or tc+"Down"==key for tc in to_correlate)
            if correlate_all:
                do_correlate = True
            new_name = key[:]

            # don't rename the main sr histograms to 2017_sr, 2016_sr, so pretend it's correlated
            if key == "sr":
                do_correlate = True

            if not do_correlate:
                new_name = "y{}_{}".format(year,key)

            # store mapping for later
            nuisance_map[key] = new_name

            # get the histogram, clone it with the new name and write to output file
            h_in = fin.Get(key)
            h_out = h_in.Clone(new_name)
            h_out.Write()

        fin.Close()
        fout.Close()

    card_basename = "card_tttt_{}.txt".format(region)
    with open("{}/{}".format(basedir,card_basename),"r") as fhin:
        with open("{}/{}".format(new_basedir,card_basename),"w") as fhout:
            for line in fhin:
                if not line.strip(): continue

                parts = line.split()
                if len(parts) > 1 and parts[1] == "lnN":
                    if parts[0] not in to_correlate and not correlate_all:
                        new_name = "y{}_{}".format(year,parts[0])
                        line = line.replace(parts[0],new_name,1)
                        line = line.replace(" "*(len(new_name)-len(parts[0])),"",1)

                for key,val in nuisance_map.items():
                    # all the nuisances in the cards do not have Up and Down, as the histograms do
                    key_noupdown = key.replace("Up","").replace("Down","")
                    val_noupdown = val.replace("Up","").replace("Down","")
                    # if we found a matching nuisance at the beginning, map it to the output nuisance
                    # which could just be the same as the initial nuisance, then break since only 1 match
                    if line.split()[0] == key_noupdown:
                        line = line.replace(key_noupdown,val_noupdown,1)
                        # if the output nuisance is longer, then remove some spaces to keep nice alignment
                        if len(val_noupdown) > len(key_noupdown):
                            line = line.replace(" "*(len(val_noupdown)-len(key_noupdown)),"",1)
                        break

                if line.split()[0] == "shapes":
                    # replace line like
                    #     shapes tttt * v1.00_2016_35p9_v1/tttt_histos_srcr_35.9ifb.root sr $SYSTEMATIC
                    # with
                    #     shapes tttt * tttt_histos_srcr_35.9ifb.root sr $SYSTEMATIC
                    # (strip out first directory, because when we use combineCards.py, combine adds
                    # in the directory automatically, so we don't want double)
                    parts = line.strip().split()
                    parts[3] = parts[3].split("/",1)[-1].lstrip("/")
                    line = " ".join(parts) + "\n"

                fhout.write(line)


os.system("combineCards.py {0}{1}/card_tttt_{3}.txt {0}{2}/card_tttt_{3}.txt > {4}".format(new_prefix,basedirs[0], basedirs[1], region, output_cardname))
