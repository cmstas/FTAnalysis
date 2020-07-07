import os
import numpy as np
import uproot
import pickle
import math
import re

from analysis.utils.plotting_utils import write_table

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery import Hist1D, plot_stack
from matplottery.utils import binomial_obs_z
import matplotlib
# ../../../common/matplottery/matplottery/utils.py
import pandas as pd

def get_total_yields(theregion, extra=""):
    ch_name_map = {
            "ch1": "srhh",
            "ch2": "srhl",
            "ch3": "srll",
            "ch4": "srml",
            "ch5": "srlm",
            }


    f = uproot.open("totalregion_fitDiagnosticsv2.root") # all 5 regions single bin each, with ML split into onz and offz (2 bins)
    d_hists = {}
    d_sfs = {} # key is process and value is postfit/prefit SF # NOTE, b-only fit
    for typ in ["shapes_fit_b","shapes_prefit"]:
        shapes = f[typ]
        d_hists[typ] = {}
        for ch in shapes:
            ch = ch.rsplit(";")[0]
            # if not ch.startswith("ch"): continue
            hists = shapes[ch]
            # if type(hists) is not uproot.rootio.ROOTDirectory: continue
            if "ROOTDirectory" not in str(type(hists)): continue
            name = ch_name_map.get(ch,ch)
            d_hists[typ][name] = {}
            print typ,ch,hists,type(hists), hists.keys()
            for hname in hists:
                hname = hname.rsplit(";")[0]
                if "covar" in hname: continue
                # print name, hname, type(hname)
                h = Hist1D(hists[hname],label=hname)
                # if name in name_nbin_map: h.restrict_bins(range(name_nbin_map[name]))
                d_hists[typ][name][hname] = h
                # d_hists[typ][name][hname] = 1
            data = d_hists[typ][name]["data"]
            bg = d_hists[typ][name]["total_background"]
            d_hists[typ][name]["pulls"] = binomial_obs_z(data.counts,bg.counts,bg.errors)
    for proc in d_hists["shapes_prefit"].values()[0].keys():
        if proc in ["pulls","data"]: continue
        ipostfit = 0.
        iprefit = 0.
        for region in d_hists["shapes_fit_b"].keys():
            postfit = d_hists["shapes_fit_b"][region].get(proc,None)
            prefit = d_hists["shapes_prefit"][region].get(proc,None)
            if postfit is not None: ipostfit += postfit.get_integral()
            if prefit is not None: iprefit += prefit.get_integral()
        d_sfs[proc] = ipostfit/iprefit

    def cull_bins(h, first=None,last=None):
        h = Hist1D(h)
        h._counts = h._counts[first:last]
        h._errors = h._errors[first:last]
        if h._errors_up is not None:
            h._errors_up = h._errors_up[first:last]
            h._errors_down = h._errors_down[first:last]
        if last:
            last += 1
        if first:
            first -= 1
        h._edges = h._edges[first:last]
        return h

    def parse_yields(d_in, extra=""):
        d_yields = {}
        for key in d_in:
            if key in ["pulls"]: continue
            h1 = d_in[key]
            print(h1)
            if extra == "onz":
                h1 = cull_bins(h1, last=1)
            if extra == "offz":
                h1 = cull_bins(h1, first=1)
            vals,errs = h1.counts, h1.errors
            d_yields[key] = {}
            d_yields[key]["central"] = vals
            d_yields[key]["error"] = errs
        return d_yields

    x = "shapes_prefit"
    return parse_yields(d_hists[x][theregion], extra=extra)

if __name__ == "__main__":
    print(get_total_yields("srhh"))
    pass
