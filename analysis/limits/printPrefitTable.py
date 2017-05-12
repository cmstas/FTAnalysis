#!/usr/bin/env python

import sys
import os
import argparse
import createCard
import commands
import ROOT as r
import numpy as np

def get_yields(card, regions="srcr"):

    if ".txt" not in card:
        card += "/card_tttt_{0}.txt".format(regions)
        print ">>> [!] no card name specified, so using {0}".format(card)

    full_card_name = "{0}".format(card)

    d_shapes = {}
    processes = []
    nuisances = []
    with open(full_card_name, "r") as fhin:
        for line in fhin:
            line=line.strip()

            if any(line.startswith(k) for k in ["bin", "observation", "imax", "jmax", "kmax", "rate", "-", "#"]):
                continue

            if line.startswith("shapes"):
                parts = line.split()
                proc = parts[1]
                rootfile = parts[3]
                hname = parts[4]
                d_shapes[proc] = {"hname":hname, "rootfile":rootfile}

            elif line.startswith("process"):
                processes = line.split()[1:]

            else:
                parts = line.split()
                name = parts[0]
                kind = parts[1]
                vals = parts[2:]
                nuisances.append({"name":name, "kind":kind, "vals":vals})

    d_proc_nuisances = {}
    for nuisance in nuisances:
        name = nuisance["name"]
        kind = nuisance["kind"]
        slimnuisances = filter(lambda x:x[0]!="-",zip(nuisance["vals"],processes))
        for val,proc in slimnuisances:
            if proc not in d_proc_nuisances: d_proc_nuisances[proc] = []
            d_proc_nuisances[proc].append({"name":name, "val":float(val), "kind":kind})

    d_yields = {}

    nbins = None
    for proc in d_shapes.keys():
        if proc == "data_obs": continue

        hname = d_shapes[proc]["hname"]
        rootfile = d_shapes[proc]["rootfile"]

        f1 = r.TFile(rootfile)
        central = np.array(list(f1.Get(hname))[1:-1])
        upper = np.zeros(len(central))
        nbins = len(central)
        for ns in d_proc_nuisances[proc]:
            nsname = ns["name"]
            kind = ns["kind"]
            val = ns["val"]

            if kind == "lnN":
                up = val*central-central

            if kind == "shape":
                up = val*np.array(list(f1.Get(ns["name"]+"Up"))[1:-1])-central

            upper += up**2.0

        upper = upper**0.5
        d_yields[proc] = {}
        d_yields[proc]["central"] = central
        d_yields[proc]["error"] = upper

    bg_sum_central = np.zeros(nbins)
    bg_sum_error = np.zeros(nbins)
    for proc in d_yields:
        central = d_yields[proc]["central"]
        error = d_yields[proc]["error"]
        if proc == "tttt": continue
        bg_sum_central += central
        bg_sum_error += error**2.0
        # print proc, 
        # print map(lambda x:"{0:.2f} +- {1:.2f}".format(x[0],x[1]),zip(central,error))

    d_yields["bgtot"] = {}
    d_yields["bgtot"]["central"] = bg_sum_central
    d_yields["bgtot"]["error"] = bg_sum_error**0.5

    # data
    data_hname = d_shapes["data_obs"]["hname"]
    data_rootfile = d_shapes["data_obs"]["rootfile"]
    fdata = r.TFile(data_rootfile)
    d_yields["data"] = {}
    d_yields["data"]["central"] = np.array(list(fdata.Get(data_hname))[1:-1])
    d_yields["data"]["error"] = d_yields["data"]["central"]**0.5
    # print d_yields["data"]

    return  d_yields


def print_table(d_yields):
    nbins = len(d_yields["ttz"]["central"])
    print nbins
    # colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","WZ","WW","X+$\\gamma$","Rares","Flips","Fakes","Total","Data","$\\tttt$"]
    # procs = ["ttw","ttz","tth","wz","ww","xg","rares","flips","fakes","bgtot","data","tttt"]
    # colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","X+$\\gamma$","Rares","Flips","Fakes","Total","Data","$\\tttt$"]
    # procs = ["ttw","ttz","tth","xg","rares","flips","fakes","bgtot","data","tttt"]
    colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","$\\ttWW$","X+$\\gamma$","Rares","Flips","Fakes","Total","Data","$\\tttt$"]
    procs = ["ttw","ttz","tth","ttww","xg","rares","flips","fakes","bgtot","data","tttt"]
    srnames = ["CRZ","CRW","SR1","SR2","SR3","SR4","SR5","SR6","SR7","SR8"]
    for ibin in range(nbins):
        # print ibin
        if ibin == 0:
            print "&".join(map(lambda x: "{0:12s}".format(x),colnames)),
            print r"\\"
            print r"\hline\hline"

        tojoin = [srnames[ibin]]
        for proc in procs:
            # print proc
            cent = d_yields[proc]["central"][ibin]
            err = d_yields[proc]["error"][ibin]
            if "data" in proc:
                tojoin.append("{0:.0f}".format(cent))
            else:
                tojoin.append("{0:5.2f}$\\pm${1:5.2f}".format(cent,err))
        print " & ".join(tojoin),
        print r"\\"



if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("card", help="card name in directory")
    args = parser.parse_args()

    d_yields = get_yields(
            card=args.card,
            # card="v0.03_Apr27_test"
            # card="v0.04_Apr28_test"
            )

    print_table(d_yields)


