import os
import numpy as np
import uproot

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery.plotter import plot_stack, plot_2d
from matplottery.utils import Hist1D, Hist2D

from multiprocessing import Pool as ThreadPool

varmap = {

        "ht": "HT",
        "eta1": "L1ETA",
        "pt1": "L1PT",
        "eta2": "L2ETA",
        "pt2": "L2PT",
        "ptfake": "LFake",
        "pttrue": "LTrue",
        "match": "MATCH",
        "met": "MET",
        "mtmin": "MTMIN",
        "nbtags": "NB",
        "njets": "NJET",

        }


f_tt = {}
f_wj = {}


def plot_var(info):
    global f_tt, f_wj
    d_info = {}
    year, var, outputdir = info
    flavs = ["e","m","in"]
    # varnames = ["nbtags"]
    # flavs = ["in"]
    # for var in varnames:
    for flav in flavs:
        obs_ttbar = Hist1D(f_tt[year]["obs_{}_{}".format(var,flav)], color=(1.0,1.0,0.04), label="$t\\bar{t}$")
        obs_wjets = Hist1D(f_wj[year]["obs_{}_{}".format(var,flav)], color=(1.0,0.76,0.04), label="W+jets")
        pred = Hist1D(f_tt[year]["pred_{}_{}".format(var,flav)],label="Prediction")+Hist1D(f_wj[year]["pred_{}_{}".format(var,flav)],label="Prediction")
        pred_up = Hist1D(f_tt[year]["pred_up_{}_{}".format(var,flav)],label="Prediction")+Hist1D(f_wj[year]["pred_up_{}_{}".format(var,flav)],label="Prediction")
        pred_down = Hist1D(f_tt[year]["pred_down_{}_{}".format(var,flav)],label="Prediction")+Hist1D(f_wj[year]["pred_down_{}_{}".format(var,flav)],label="Prediction")

        # This isn't actually correct. if we did that, then we're assuming the stat errors
        # in different bins of the fake rate are correlated (i.e., they all move up when we make the pred_up hist)
        # # add 0.5(up-down) in quadrature with stat errors
        # pred._errors = np.hypot(0.5*(pred_up-pred_down).counts, pred.errors)

        for h in [obs_ttbar,obs_wjets,pred]:
            h.set_attr("label", "{} [{:.1f}]".format(h.get_attr("label"),h.get_integral()))

        xticks = []
        if "match" in var:
            xticks = ["nomatch","b","c","photon","light"]
            if year not in d_info: d_info[year] = {}
            d_info[year][flav] = {
                        "obs_ttbar": obs_ttbar.get_integral(),
                        "obs_wjets": obs_wjets.get_integral(),
                        "pred": pred.get_integral(),
                        }



        fname = "{}/y{}_{}{}.pdf".format(
                outputdir,
                year,
                varmap[var],
                {"e":"_el","m":"_mu","in":""}[flav],
                )
        plot_stack(bgs=[obs_ttbar,obs_wjets], data=pred, title="",
                xlabel="{}".format(varmap[var]),
                ylabel="Entries",
                filename=fname,
                cms_type = "Preliminary",
                lumi = {"2016":"35.9","2017":"41.5","2018":"58.8"}[str(year)],
                # ratio_range=[0.,2.],
                ratio_range=[0.5,1.5],
                mpl_hist_params = {
                    "edgecolor": "k",
                    "linewidth": 0.5,
                    },
                do_bkg_syst=True,
                xticks=xticks,
                )
        # os.system("ic {}".format(fname))
        # print "Wrote {}".format(fname)
    return fname, d_info

def make_plots(inputdir="outputs/",outputdir="plots_SS_19Jan25/"):
    global f_tt, f_wj
    varnames = [
            "match",
            "njets",
            "nbtags",
            "mtmin",
            "ht",
            "met",
            "pt1",
            "pt2",
            "eta1",
            "eta2",
            "pttrue",
            "ptfake",
            ]
    # outputdir = "plots_SS_19Jan25"

    f_tt = {
            2016: uproot.open("{}/histos_tt_{}.root".format(inputdir,2016)),
            2017: uproot.open("{}/histos_tt_{}.root".format(inputdir,2017)),
            2018: uproot.open("{}/histos_tt_{}.root".format(inputdir,2018)),
            }
    f_wj = {
            2016: uproot.open("{}/histos_wjets_{}.root".format(inputdir,2016)),
            2017: uproot.open("{}/histos_wjets_{}.root".format(inputdir,2017)),
            2018: uproot.open("{}/histos_wjets_{}.root".format(inputdir,2018)),
            }
    to_run = []
    for year in [2016, 2017, 2018]:
        for var in varnames:
            to_run.append([year, var, outputdir])

    os.system("mkdir -p {}".format(outputdir))
    os.nice(10)
    pool = ThreadPool(12)
    # map(plot_var,to_run)
    d_infos = {}
    for res in pool.imap_unordered(plot_var,to_run):
        fname,d_info = res
        if fname:
            print "Wrote {}".format(fname)
        if d_info:
            d_infos.update(d_info)
                        
    print
    for year,d_info in d_infos.items():
        for iflav,(flav,flavstr) in enumerate([("e","el"),("m","mu"),("in","in")]):
            d = d_info[flav]
            print "{}  {}  pred: {:.2f}".format(year if iflav == 0 else "    ",flavstr,d["pred"])
            print "          obs: {:.2f} (ttbar: {:.2f}, wjets: {:.2f})".format((d["obs_ttbar"]+d["obs_wjets"]),d["obs_ttbar"],d["obs_wjets"])
            print "          pred/obs: {:.2f}".format(d["pred"]/(d["obs_ttbar"]+d["obs_wjets"]))
        print


if __name__ == "__main__":

    # make_plots(outputdir="plots_SS_19Jan25")
    make_plots(inputdir="outputs_v2",outputdir="plots_SS_19Jan25_v2")


