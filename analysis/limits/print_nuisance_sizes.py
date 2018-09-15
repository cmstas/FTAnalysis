from optparse import OptionParser
from HiggsAnalysis.CombinedLimit.DatacardParser import parseCard
import ROOT as r
import numpy as np
from pprint import pprint

from analysis.utils.pytable import Table

# import uproot
# from matplottery.matplotter.utils import Hist1D


def print_table(fname):
    opts = type("opts", (object,), dict(bin=True,noJMax=False,stat=False,nuisancesToExclude=[]))
    with open(fname,"r") as fh:
        dc = parseCard(fh, opts)
        # print dc.print_structure()
        shapes = [s for s in dc.systs if s[2] == "shape"]
        shape_procs = {}
        for name,_,_,_,info in shapes:
            shape_procs[name] = []
            for proc,val in info["SS"].items():
                if val > 0.: shape_procs[name].append(proc)
        shape_procs["stat"] = dc.processes[:]
        proc_files = dict([(k,v[0]) for k,v in dc.shapeMap.values()[0].items()])

    # pprint(shape_procs)
    # pprint(proc_files)

    files = { proc:r.TFile(proc_files[proc]) for proc in proc_files }

    def hist_array(x): return np.array(list(x)[1:-1])

    def get_median_percent_diff(proc,nuisance):
        up = hist_array(files[proc].Get("{}Up".format(nuisance if "stat" not in nuisance else "{}_{}".format(proc,nuisance))))
        down = hist_array(files[proc].Get("{}Down".format(nuisance if "stat" not in nuisance else "{}_{}".format(proc,nuisance))))
        sr = hist_array(files[proc].Get("sr"))
        ratios = np.r_[up/sr,sr/down]
        ratios[ratios < 1] = 1./ratios[ratios<1]
        pct = (ratios - 1.)*100.
        pct[pct > 150.] = np.nan
        return np.nanmedian(pct)

    tab = Table()
    tab.set_theme_basic()
    tab.set_column_names(["nuisance"]+sorted(proc_files.keys()))
    for nuisance,procs in shape_procs.items():
        dline = { k:None for k in proc_files.keys() }
        for proc in procs:
            diff = get_median_percent_diff(proc,nuisance)
            dline[proc] = diff
        line = [nuisance] + [("{:.1f}%".format(v) if v is not None else "") for k,v in sorted(dline.items())]
        tab.add_row(line)
    tab.print_table(show_row_separators=False,show_alternating=False)

if __name__ == "__main__":

    fname = "original_cards_2017_94X_mcfakes_75ifb/card_tttt_srcr.txt"
    print_table(fname)
    fname = "v1.00_2016_75p0_v1_try2/card_tttt_srcr.txt"
    print_table(fname)

    # proc = "tttt"
    # nuisance = "pdf"
    # print proc, nuisance, get_median_percent_diff(proc,nuisance)

    # print np.array(list(files["tttt"].Get("sr"))[1:-1])
    # print files


