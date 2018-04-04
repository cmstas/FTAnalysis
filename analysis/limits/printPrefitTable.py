#!/usr/bin/env python
# -*- coding: UTF-8 -*-
from __future__ import print_function

import sys
import os
import argparse
import createCard
import commands
import ROOT as r
import numpy as np

def get_yields(card, regions="srcr",stats_only=False):

    if ".txt" not in card:
        card += "/card_tttt_{0}.txt".format(regions)
        print(">>> [!] no card name specified, so using {0}".format(card))

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
        if stats_only and "_stat" not in nuisance["name"]: continue
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
                # up = val*np.array(list(f1.Get(ns["name"]+"Up"))[1:-1])-central
                up = 0

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

    return  d_yields


def print_table(d_yields, slim, pretty, regions="srcr",precision=2):
    nbins = len(d_yields["ttz"]["central"])
    colname_lookup = {
        "ttw":   "$\\ttW$",
        "ttz":   "$\\ttZ$",
        "tth":   "$\\ttH$",
        "ttvv":  "$\\ttVV$",
        "xg":    "X+$\\gamma$",
        "rares": "Rares",
        "flips": "Flips",
        "fakes": "Fakes",
        "bgtot": "Total",
        "data":  "Data",
        "tttt":  "$\\tttt$"
    }
    procs = ["ttw","ttz","tth","ttvv","xg","rares","fakes_mc","bgtot","data","tttt"]
    if slim:
        procs = ["ttw","ttz","tth","bgtot","tttt"]
    if regions == "srcr":
        srnames = ["CRZ","CRW"]+["SR{}".format(i+1) for i in range(16)]
    elif regions == "srdisc":
        srnames = ["SR{}".format(i+1) for i in range(13)]

    if not pretty:
        header = "&".join("{0:12s}".format(colname_lookup[proc]) for proc in procs)
        header += r"\\ \hline\hline"
        body = []
        for ibin, srname in enumerate(srnames):

            tojoin = [srname]
            for proc in procs:
                # cent = d_yields[proc]["central"][ibin]
                cent = max(d_yields[proc]["central"][ibin],0.)
                err = d_yields[proc]["error"][ibin]
                if "data" in proc:
                    if ibin in [0,1]:
                        tojoin.append("{0:.0f}".format(cent))
                    else:
                        tojoin.append("-".format(cent))
                else:
                    tojoin.append("{0:5.2f}$\\pm${1:5.2f}".format(cent,err))
            body.append(" & ".join(tojoin) + r"\\")
        print("\n".join([header]+body)
    else:
        from pytable import Table
        tab = Table()
        tab.set_column_names([cname.replace("$","").replace("\\","") for cname in colnames])
        sep = u"\u00B1".encode("utf-8")  # +-
        if len(srnames)+1 == len(d_yields[d_yields.keys()[0]]["central"]):
            # if we have one more bin than bins we expect,
            # the protocol is that the last one was made to be
            # the sum of the usual bins
            # srnames.append("tot")
            srnames.append(u"\u01A9".encode("utf-8"))
        for ibin,binrow in enumerate(srnames):
            row = [binrow]
            for proc in procs:
                cent = max(d_yields[proc]["central"][ibin],0.)
                err = d_yields[proc]["error"][ibin]
                row.append(("{0:5.%if} {1}{2:%i.%if}" % (precision,precision+3,precision)).format(cent,sep,err))
            tab.add_row(row)
        tab.print_table(show_row_separators=False,show_alternating=False)



if __name__ == "__main__":
    import json

    parser = argparse.ArgumentParser()
    parser.add_argument("card", help="card name in directory")
    parser.add_argument("-s", "--slim", help="slimmed table for simple studies", action="store_true")
    parser.add_argument("-d", "--denominator", help="another card to use as the denominator (table becomes a ratio)", default=None)
    parser.add_argument("-p", "--pretty", help="use pretty pytable", action="store_true")
    parser.add_argument("-r", "--regions", help="srcr or disc for SR+CR limits or BDT limits", default="srcr")
    parser.add_argument("-o", "--stats_only", help="only statistical errors", action="store_true")
    parser.add_argument("-n", "--precision", help="number of decimal places", default=2)
    args = parser.parse_args()

    d_yields = get_yields(
            card=args.card,
            regions=args.regions,
            stats_only=args.stats_only,
            )

    if args.denominator:
        from errors import E
        d_yields_den = get_yields(
                card=args.denominator,
                regions=args.regions,
                stats_only=args.stats_only,
                )

        for proc in d_yields.keys():
            numer = map(lambda x:E(*x), zip(d_yields[proc]["central"],d_yields[proc]["error"]))
            denom = map(lambda x:E(*x), zip(d_yields_den[proc]["central"],d_yields_den[proc]["error"]))
            # add last bin, which is sum of the SRs
            # numer.append(sum(numer[2:]))
            # denom.append(sum(denom[2:]))
            numer.append(sum(numer))
            denom.append(sum(denom))
            ratios = []
            for n,d in zip(numer,denom):
                r = n/d
                if r[0] < 100 and r[1] < 100:
                    ratios.append(r)
                else:
                    ratios.append(E(-1.,0))

            vals = [x[0] for x in ratios]
            errs = [x[1] for x in ratios]
            d_yields[proc]["central"] = vals
            d_yields[proc]["error"] = errs

    print_table(d_yields, slim=args.slim, pretty=args.pretty, regions=args.regions, precision=int(args.precision))


