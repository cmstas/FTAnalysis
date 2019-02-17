#!/usr/bin/env python

import ROOT as r
import os
import sys
import commands
import argparse

from optparse import OptionParser
from HiggsAnalysis.CombinedLimit.DatacardParser import parseCard
import numpy as np
from pprint import pprint

from analysis.utils.pytable import Table

def fix_spacing(ss):
    if type(ss) in [str]:
        ss = ss.split()
    return "".join([
        ("{:<9s}".format(x) if len(x) < 9 else " {:<9s} ".format(x))
        for x in ss
        ])

def get_info(fname):
    if "rpv_" in fname:
        opts = type("opts", (object,), dict(bin=True,noJMax=False,stat=False,nuisancesToExclude=[],allowNoSignal=True))
    else:
        opts = type("opts", (object,), dict(bin=True,noJMax=False,stat=False,nuisancesToExclude=[]))
    with open(fname,"r") as fh:
        dc = parseCard(fh, opts)
        # print dc.print_structure()
        systs = {
                s[0]:dict(kind=s[2],procs=s[4].values()[0])
                for s in dc.systs if s[2] in ["shape","lnN"]}
        proc_files = dict([(k,v[0]) for k,v in dc.shapeMap.values()[0].items()])
    files = { proc:r.TFile(proc_files[proc]) for proc in proc_files }
    systs["sr"] = dict(kind="shape", procs=systs.values()[0]["procs"])
    return systs, files

def do_combine(
        basedir,
        regions="srcr",
        years = [2016,2017,2018],
        procs = ["data", "fakes", "fakes_mc", "flips", "rares", "tth", "tttt", "ttvv", "ttw", "ttz", "xg"],
        extra_procs = [],
        cardname = "combined_card.txt",
        correlate_all = False,
        correlate_none = False,
        autothresh = 0,
        signal = "tttt",
        inputcard_pattern = "",
        skiptext2workspace = False,
        verbose = True,
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
                ],
        ):
    if len(extra_procs) > 0: signal = extra_procs[0]

    if correlate_none:
        to_correlate = []

    procs += extra_procs

    yearly_files = {}
    yearly_systs = {} # actually, the set of systs is the same per year, so we only need one element
    for year in years:
        if inputcard_pattern:
            card_basename = "{}/{}".format(basedir,inputcard_pattern.format(year=year))
        else:
            card_basename = "{}/card_{}_{}_{}.txt".format(basedir,signal,regions,year)
        systs, files = get_info(card_basename)
        yearly_systs[year] = systs
        yearly_files[year] = files

    summed_files = {}
    for proc,tfile in yearly_files[2016].items():
        rname = tfile.GetName().replace("_2016","_run2")
        method = "recreate"
        if os.path.exists(rname):
            if verbose: print "Warning, not replacing {}, delete run2 stuff manually".format(rname)
            method = "read"
            # NOTE, only Write if writable below, then
        summed_files[proc] = r.TFile(rname,method)

    def make_hist(name,which="Up",spec_year=-1):
        hnames_to_sum = []
        outhname = "{}{}".format(name,which)
        if spec_year > 0:
            outhname = "y{}_{}{}".format(spec_year,name,which)
        for year in years:
            if spec_year > 0 and year == spec_year:
                hname = "{}{}".format(name,which)
            else:
                hname = "sr"
            hnames_to_sum.append(hname)
        isnorm = systs[name]["kind"] == "lnN"
        scales = [1.0 for y in years]
        procs = [p for p,v in systs[name]["procs"].items() if ((v != 0.) or (name in ["sr"]))]
        if isnorm:
            scales = [next(v for v in yearly_systs[y][name]["procs"].values() if v != 0.)**(-1 if which == "Down" else 1.)
                    for y in years]
            for i,y in enumerate(years):
                if spec_year > 0 and y != spec_year: scales[i] = 1.
        if isnorm or (name in ["sr"]):
            hnames_to_sum = ["sr" for _ in years]
            procs += ["data_obs"]
        for proc in procs:
            summed_files[proc].cd()

            hists = [yearly_files[year][proc].Get(hname) for year,hname in zip(years,hnames_to_sum)]
            copy_hists = [h.Clone("{}{}".format(outhname,"" if i == 0 else i)) for i,h in enumerate(hists)]

            if which in ["Up","Down"]:
                for ih,s in enumerate(scales):
                    copy_hists[ih].Scale(s) # inverse of s is already calculated above
            hout = copy_hists[0]
            for h in copy_hists[1:]:
                hout.Add(h)
            # if verbose:
            #     print "Year={}, nuisance={}, proc={}, summing {} scaled by {} into {} (integrals {}={:.2f})".format(
            #             spec_year, name, proc, hnames_to_sum, scales, outhname,
            #             "+".join(map(lambda x: "{:.2f}".format(x.Integral()), hists)),
            #             hout.Integral()
            #             )
            if summed_files[proc].IsWritable(): # NOTE
                hout.Write()
        return outhname.replace(which,""), procs


    systs = yearly_systs.values()[0]
    # For each syst
    nuisance_info = {}
    for name in systs.keys():
        if name in ["sr"]: continue
        # Do we correlate it across the years?
        correlate = name in to_correlate
        if correlate:
            # If so, we add up the 3 yearly histograms and have one single nuisance with 2 histograms (up, down)
            output_nuisance_name, procs = make_hist(name, which="Up")
            output_nuisance_name, procs = make_hist(name, which="Down")
            nuisance_info[output_nuisance_name] = procs

        else:
            # Otherwise, we get three nuisances with 6 histograms (up,down)
            # Sum the years except for one of them, use either the up or down variation
            # E.g., syst2016up = 2016up + 2017nominal + 2018nominal
            # print "uncorrelated"
            for y in years:
                for which in ["Up","Down"]:
                    output_nuisance_name, procs = make_hist(name, which=which, spec_year=y)
                nuisance_info[output_nuisance_name] = procs
    make_hist("sr", which="")
    del nuisance_info["y2018_prefire"]

    # map from proc to nominal yield
    yields = {proc:tfile.Get("sr").Integral() for proc,tfile in summed_files.items()}

    # Close all the files
    map(lambda x: x.Close(), summed_files.values())

    proc_columns = [signal]+sorted(y for y in yields.keys() if y not in [signal,"data_obs"])
    bin_str = " ".join(["SS" for _ in proc_columns])
    rate_str = " ".join(["{:.3f}".format(yields[proc]) for proc in proc_columns])
    procid_str = " ".join(map(str,range(len(proc_columns))))

    buff = ""
    buff += "imax 1\n"
    buff += "jmax *\n"
    buff += "kmax *\n"
    buff += "------------\n"
    for proc, tfile in summed_files.items():
        buff += "shapes {:<10s} * {} sr {}\n".format(proc,tfile.GetName(),"sr" if "data" in proc else "$SYSTEMATIC")
    buff += "------------\n"
    buff += "observation {}\n".format(yields["data_obs"])
    buff += "------------\n"
    buff += "bin                         {}\n".format(fix_spacing(bin_str))
    buff += "process                     {}\n".format(fix_spacing(procid_str))
    buff += "process                     {}\n".format(fix_spacing(proc_columns))
    buff += "rate                        {}\n".format(fix_spacing(rate_str))
    buff += "------------\n"
    for name,procs in sorted(nuisance_info.items()):
        row = ["-" for _ in proc_columns]
        for proc in procs: 
            if proc in proc_columns:
                row[proc_columns.index(proc)] = "1"
        buff += "{:<18s} shape         {}\n".format(name,fix_spacing(" ".join(row)))
    # buff += "SS autoMCStats 50\n"
    buff += "SS autoMCStats {}\n".format(autothresh)
    with open("{}/{}".format(basedir,cardname),"w") as fhout:
        fhout.write(buff)
    if not skiptext2workspace:
        cmd = "text2workspace.py {basedir}/{cardname}".format(basedir=basedir, cardname=cardname)
        if verbose: print "[!] Running: {}".format(cmd)
        stat, out = commands.getstatusoutput(cmd)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("directory", help="card directory")
    # parser.add_argument("--directory", help="card directory",default="v3.13_Nov30_v1/") # FIXME
    parser.add_argument("-r", "--regions", help="srcr or disc for SR+CR limits or BDT limits", default="srcr")
    parser.add_argument("-c", "--cardname", help="name of card", default="single_card.txt")
    parser.add_argument("-s", "--sig", help="signal name (default: %(default)s)", default="tttt")
    parser.add_argument(      "--correlate_all", help="correlate everything", action="store_true")
    parser.add_argument(      "--correlate_none", help="correlate nothing", action="store_true")
    parser.add_argument(      "--autothresh", help="autoMCstats threshold", default="0")
    args, unknown = parser.parse_known_args()
    print "[?] Found some unknown args, but just ignoring them:", unknown

    do_combine(
            args.directory,
            regions=args.regions,
            cardname=args.cardname,
            signal=args.sig,
            correlate_all=args.correlate_all,
            correlate_none=args.correlate_none,
            autothresh=args.autothresh,
            )
