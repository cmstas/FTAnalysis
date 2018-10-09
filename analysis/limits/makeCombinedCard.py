#!/usr/bin/env python

import ROOT as r
import os
import sys
import commands
import argparse

def do_combine(
        basedir,
        regions="srcr",
        years = [2016,2017,2018],
        procs = ["data", "fakes", "fakes_mc", "flips", "rares", "tth", "tttt", "ttvv", "ttw", "ttz", "xg"],
        ):

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

    basedirs = [basedir for _ in years]

    to_combine = []
    for basedir,year in zip(basedirs,years):
        nuisance_map = {}
        for proc in procs:
            basename = "{}_histos_{}_{}.root".format(proc,regions,year)
            fin = r.TFile("{}/{}".format(basedir,basename), "UPDATE")

            keys = [x.GetName() for x in fin.GetListOfKeys()]
            for key in keys:
                if key.startswith("y201"): continue

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
                if new_name not in keys:
                    h_out = h_in.Clone(new_name)
                    h_out.Write()

            fin.Close()

        card_basename = "card_tttt_{}_{}.txt".format(regions,year)
        with open("{}/{}".format(basedir,card_basename),"r") as fhin:
            with open("{}/{}_tmp".format(basedir,card_basename),"w") as fhout:
                for line in fhin:
                    if not line.strip(): continue

                    parts = line.split()
                    if len(parts) > 1 and parts[1] == "lnN":
                        if parts[0] not in to_correlate and not correlate_all:
                            new_name = "y{}_{}".format(year,parts[0])
                            if not line.startswith("y201"):
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

                    # if line.split()[0] == "shapes":
                    #     # replace line like
                    #     #     shapes tttt * v1.00_2016_35p9_v1/tttt_histos_srcr_35.9ifb.root sr $SYSTEMATIC
                    #     # with
                    #     #     shapes tttt * tttt_histos_srcr_35.9ifb.root sr $SYSTEMATIC
                    #     # (strip out first directory, because when we use combineCards.py, combine adds
                    #     # in the directory automatically, so we don't want double)
                    #     parts = line.strip().split()
                    #     parts[3] = parts[3].split("/",1)[-1].lstrip("/")
                    #     line = " ".join(parts) + "\n"

                    fhout.write(line)

            os.system("mv {0}/{1}_tmp {0}/{1}".format(basedir,card_basename))
            to_combine.append("{}={}".format("y{}".format(year),card_basename))

    cmd = "(cd {1} ; combineCards.py {0} > combined_card.txt) && text2workspace.py {1}/combined_card.txt".format(" ".join(to_combine), basedirs[0])
    print "[!] Running: {}".format(cmd)
    stat, out = commands.getstatusoutput(cmd)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("directory", help="card directory")
    parser.add_argument("-r", "--regions", help="srcr or disc for SR+CR limits or BDT limits", default="srcr")
    args, unknown = parser.parse_known_args()
    print "[?] Found some unknown args, but just ignoring them:", unknown

    do_combine(args.directory,regions=args.regions)
