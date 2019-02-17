import os
import numpy as np
import uproot

from analysis.utils.plotting_utils import write_table

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery import Hist1D, plot_stack
from matplottery.utils import binomial_obs_z
# ../../../common/matplottery/matplottery/utils.py
import pandas as pd

np.set_printoptions(linewidth=180)

ch_name_map = {
        "ch1": "srhh",
        "ch2": "srhl",
        "ch3": "srll",
        "ch4": "srml",
        "ch5": "srlm",
        }

name_nbin_map = {
        "srhh": 62,
        "srhl": 43,
        "srll": 8,
        "srml": 44,
        "srlm": 11,
        }

# f = uproot.open("../fitDiagnosticsss.root") # all 5 regions
f = uproot.open("fitDiagnosticsssforpostfit.root") # all 5 regions
# f = uproot.open("ssforpostfitUncorrTTVWZFakes.root") # all 5 regions
d_hists = {}
d_sfs = {} # key is process and value is postfit/prefit SF # NOTE, b-only fit
for typ in ["shapes_fit_b","shapes_prefit","shapes_fit_s"]:
# for typ in ["shapes_fit_b"]:
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
            if name in name_nbin_map: h.restrict_bins(range(name_nbin_map[name]))
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

d_label_colors = {}
d_label_colors["ss"] = {
        "fakes" : (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
        # "fakes_mc" : (r"MC fakes", [0.85, 0.85, 0.85]),
        "flips" : (r"Charge misid.", [0.4, 0.4, 0.4]),
        "rares" : (r"Rare", [1.0, 0.4, 1.0]),
        "tth"   : (r"$t\bar{t}H$", [0.4, 0.4, 0.6]),
        "wz"  : (r"WZ" , [1.0,0.8,0.0]),
        "ww"  : (r"WW" , [1.0,0.6,0.0]),
        "ttw"   : (r"$t\bar{t}W$", [0.0, 0.4, 0.0]),
        "ttz"   : (r"$t\bar{t}Z$", [0.4, 0.8, 0.4]),
        "xg"    : (r"$X\gamma$" , [0.4, 0.0, 0.8]),
        }

def make_plots(
        regions=["srhh","srhl","srll","srml","srlm"],
        outputdir="plots/",
        lumi="136.3",
        procs=['fakes', 'flips', 'wz', 'ttz', 'ttw', 'tth', 'rares', 'ww', 'xg'],
        ):

    # region = "srhl"
    # outputdir = "plots/"
    # lumi = "61.3"

    for region in regions:
        hists = d_hists["shapes_fit_b"][region]
        data = hists["data"]
        totbg = hists["total_background"]
        procs_ = procs[:]
        if region == "srml": procs_ = list(set(procs_)-set(["ww","flips"]))
        bgs = [
                Hist1D(
                    hists[k],
                    label=d_label_colors["ss"][k][0],
                    color=d_label_colors["ss"][k][1],
                    ) for k in procs_
                ]
        bgs = sorted(bgs, key=lambda bg: bg.get_integral())

        title = "{} (postfit)".format(region.upper())


        if region == "srll" and lumi == "136.3": lumi_ = "131.5"

        data.set_attr("label", "Data [{}]".format(int(data.get_integral())))

        ax_main_callback = None
        ax_ratio_callback = None
        mpl_legend_params = {}

        ratio_range = [0.,3.]
        mpl_legend_params["fontsize"] = 8
        mpl_legend_params["framealpha"] = 0.4
        mpl_legend_params["ncol"] = 2
        mpl_legend_params["labelspacing"] = 0.12
        xticks = range(1,70)

        def ax_main_callback(ax):
            ax.set_ylim([0.05,ax.get_ylim()[1]*2.0])
            ax.set_yscale("log", nonposy='clip'),

        fname = "{}/run2_{}_TOTAL_postfit.pdf".format(outputdir,region.upper())
        plot_stack(bgs=bgs, data=data, title=title, xlabel="bin", filename=fname,
                   cms_type = "Preliminary",
                   lumi = lumi,
                   ratio_range=ratio_range,
                   xticks=xticks,
                   ax_main_callback=ax_main_callback,
                   mpl_legend_params=mpl_legend_params,
                   ax_ratio_callback=ax_ratio_callback,
                   do_bkg_syst=True,
                   override_bkg_syst=totbg.errors,
                   )
        print "Made {}".format(fname)
        os.system("ic {}".format(fname))
        table_info = write_table(data,bgs,outname=fname.replace(".pdf",".txt"))

    print d_sfs
    print pd.DataFrame(d_sfs.items()).to_csv()

    # print bgs
    # print data
    # return

    # print "prefit bg,data:",totbg_prefit, totdata_prefit
    # print "postfit bg,data:",totbg_postfit, totdata_postfit

    # pulls = np.concatenate([d_hists["shapes_prefit"][x]["pulls"] for x in sorted(d_hists["shapes_prefit"].keys())])
    # print "prefit",pulls.mean(), pulls.std()
    # h_pulls_prefit = Hist1D(pulls,bins=np.linspace(-3,3,20),label=r"prefit [${:.2f}\pm{:.2f}$]".format(pulls.mean(),pulls.std()),
    #         weights=np.concatenate([d_hists["shapes_prefit"][x]["total_background"].counts for x in sorted(d_hists["shapes_prefit"].keys())]), # weight the pull by the yield in that bin
    #         )

    pulls = np.concatenate([d_hists["shapes_fit_b"][x]["pulls"] for x in sorted(d_hists["shapes_fit_b"].keys())])
    print "postfit",pulls.mean(), pulls.std()
    h_pulls_postfit = Hist1D(pulls,bins=np.linspace(-3,3,20),label=r"postfit [${:.2f}\pm{:.2f}$]".format(pulls.mean(),pulls.std()),
            # weights=np.concatenate([d_hists["shapes_fit_b"][x]["total_background"].counts for x in sorted(d_hists["shapes_fit_b"].keys())]), # weight the pull by the yield in that bin
            )

    print h_pulls_postfit.vis()

    # fakes_after = d_hists["shapes_fit_b"]["srhh"]["total_background"].get_integral()
    # print fakes_after/fakes_before
    plot_stack(
            # bgs=[h_pulls_prefit,h_pulls_postfit],
            bgs=[h_pulls_postfit],
            do_stack=False,
            filename="plots/pulls.png",
            mpl_hist_params=dict(histtype="step",linewidth=2),
            do_bkg_errors=True,
            )
    os.system("ic plots/pulls.png")

if __name__ == "__main__":
    make_plots()
