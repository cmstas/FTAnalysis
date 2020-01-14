import math
import os
import numpy as np
import uproot

from analysis.utils.plotting_utils import write_table

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery import Hist1D, plot_stack
from matplottery.utils import binomial_obs_z
import matplotlib
# ../../../common/matplottery/matplottery/utils.py
import pandas as pd

np.set_printoptions(linewidth=180)

ch_name_map = {
        }

d_label_colors = {}
d_label_colors["ss"] = {
        "fakes" : (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
        "flips" : (r"Charge misid.", [0.4, 0.4, 0.4]),
        "rares" : (r"Rare", [1.0, 0.4, 1.0]),
        "tth"   : (r"$t\bar{t}H$", [0.4, 0.4, 0.6]),
        "wz"  : (r"WZ" , [1.0,0.8,0.0]),
        "ww"  : (r"WW" , [1.0,0.6,0.0]),
        "ttvv"  : (r"$t\bar{t}VV$" , [0.4, 0.6, 1.0]),
        "tttt"  : (r"$t\bar{t}t\bar{t}$" , [0.75,0.15,0.22]),
        "ttw"   : (r"$t\bar{t}W$", [0.0, 0.4, 0.0]),
        "ttz"   : (r"$t\bar{t}Z$", [0.4, 0.8, 0.4]),
        "xg"    : (r"$X\gamma$" , [0.4, 0.0, 0.8]),
        }

d_hists = {}
d_sfs = {} # key is process and value is postfit/prefit SF # NOTE, sb fit
def load_info(region="srcr",tag="run2"):
    global d_hists
    global d_sfs

    f = uproot.open("fitDiagnostics{}_{}_obs.root".format(tag,region))
    for typ in ["shapes_prefit","shapes_fit_s"]:
        shapes = f[typ]
        d_hists[typ] = {}
        for ch in shapes:
            ch = ch.rsplit(";")[0]
            hists = shapes[ch]
            if "ROOTDirectory" not in str(type(hists)): continue
            name = ch_name_map.get(ch,ch)
            d_hists[typ][name] = {}
            for hname in hists:
                hname = hname.rsplit(";")[0]
                if "covar" in hname: continue
                h = Hist1D(hists[hname],label=hname)
                d_hists[typ][name][hname] = h
            data = d_hists[typ][name]["data"]
            bg = d_hists[typ][name]["total_background"]
            d_hists[typ][name]["pulls"] = binomial_obs_z(data.counts,bg.counts,bg.errors)
    for proc in d_hists["shapes_prefit"].values()[0].keys():
        if proc in ["pulls","data"]: continue
        ipostfit = 0.
        iprefit = 0.
        for region in d_hists["shapes_fit_s"].keys():
            postfit = d_hists["shapes_fit_s"][region].get(proc,None)
            prefit = d_hists["shapes_prefit"][region].get(proc,None)
            if postfit is not None: ipostfit += postfit.get_integral()
            if prefit is not None: iprefit += prefit.get_integral()
        d_sfs[proc] = ipostfit/iprefit

def make_plots(
        regions=["srdisc"],
        outputdir="plots/",
        typ="shapes_prefit",
        lumi="137",
        procs=['fakes', 'flips', 'ttvv', 'ttz', 'ttw', 'tth', 'rares', 'xg','tttt'],
        year="run2",
        ):

    region = regions[0]

    hists = d_hists[typ]["SS"]
    data = hists["data"]
    totbg = hists["total"]
    procs_ = procs[:]
    bgs = [
            Hist1D(
                hists[k],
                label=d_label_colors["ss"][k][0],
                color=d_label_colors["ss"][k][1],
                ) for k in procs_
            ]
    bgs[:-1] = sorted(bgs[:-1], key=lambda bg: bg.get_integral())

    title = "Cut-based (post-fit)"

    data.set_attr("label", "Data")

    ax_main_callback = None
    ax_ratio_callback = None
    mpl_legend_params = {}

    ratio_range = [0.,3.]
    mpl_legend_params["fontsize"] = 12
    mpl_legend_params["framealpha"] = 0.0
    mpl_legend_params["ncol"] = 4
    mpl_legend_params["labelspacing"] = 0.11
    mpl_legend_params["columnspacing"] = 1.25
    mpl_legend_params["bbox_to_anchor"] = (1.0,0.93)
    mpl_legend_params["loc"] = "upper right"
    xticks = range(1,70)

    mpl_figure_params = dict(
            gridspec_kw=dict(
                height_ratios=[9,2],
                top=0.94,
                bottom=0.2,
                ),
            # figsize=(6.4,4.8),
            figsize=(8,5.),
            )

    yticks_ratio = [0,1,2]

    if region.lower() in ["srcr"]:
        xticks = ["CRZ","CRW"]+["SR{}".format(i) for i in range(1,20)]

    def ax_main_callback(ax,title=title):
        ax.set_ylim([0.08,ax.get_ylim()[1]*(2.0+3)])
        ax.set_yscale("log", nonposy='clip'),
        ax.text(0.5,0.97,title,fontsize=13,horizontalalignment="center",verticalalignment="top", transform=ax.transAxes)
        ax.text(0.3,0.6,r"2 leptons",fontsize=13,horizontalalignment="center",verticalalignment="center", transform=ax.transAxes)
        ax.text(0.8,0.6,r"$\geq$3 leptons",fontsize=13,horizontalalignment="center",verticalalignment="center", transform=ax.transAxes)
    title = ""

    def ax_ratio_callback(ax):
        ax.set_ylabel("Data / Pred.",horizontalalignment="center",y=0.4)
        for x,ymin,ymax in [
                [1.,-1.3,5.5],
                [10.,-1.3,4.5],
                [5.,-1.3,1.25],
                [9.,-1.3,1.25],
                [13.,-1.3,1.25],
                ]:
            if ymax > 2: lw = 1.5-0.3
            else: lw = 1.0-0.3
            ax.axvline(x,ymin=ymin,ymax=ymax,clip_on=False,color="k",lw=lw,ls="--")

        # for i in set(range(1,16))-set([1,10,5,9,13]):
        #     ax.axvline(i,ymin=-0.95,ymax=0,clip_on=False,color="k",lw=0.5,ls="--")

        y1 = -2.3
        for x,y,s in [
                [0.5 ,y1,r"$\geq$2"],
                [1.5 ,y1,r"$\leq$5"],
                [2.5 ,y1,r"6"]      ,
                [3.5 ,y1,r"7"]      ,
                [4.5 ,y1,r"$\geq$8"],
                [5.5 ,y1,r"5"]      ,
                [6.5 ,y1,r"6"]      ,
                [7.5 ,y1,r"7"]      ,
                [8.5 ,y1,r"$\geq$8"],
                [9.5 ,y1,r"$\geq$5"],
                [10.5,y1,r"5"]      ,
                [11.5,y1,r"6"]      ,
                [12.5,y1,r"$\geq$7"],
                [13.5,y1,r"4"]      ,
                [14.5,y1,r"5"]      ,
                [15.5,y1,r"$\geq$6"],
                ]:
            ax.text(x,y,s,fontsize=13,horizontalalignment="center",verticalalignment="center")

        y2 = -3.7
        for x,y,s in [
                [0.5 ,y2,r"$\geq$2"],
                [3 ,y2,r"2"],
                [7 ,y2,r"3"],
                [9.5 ,y2,r"$\geq$4"],
                [11.5 ,y2,r"2"],
                [14.5 ,y2,r"$\geq$3"],
                ]:
            ax.text(x,y,s,fontsize=13,horizontalalignment="center",verticalalignment="center")
        ax.text(-0.7,y1,r"$N_\mathrm{jets}$",fontsize=13,horizontalalignment="center",verticalalignment="center")
        ax.text(-0.7,y2,r"$N_\mathrm{b}$",fontsize=13,horizontalalignment="center",verticalalignment="center")

    fname = "{}/aux_SRCR.pdf".format(outputdir)
    plot_stack(bgs=bgs, data=data, title=title, xlabel="", ylabel="Events", filename=fname,
               cms_type = "",
               lumi = lumi,
               ratio_range=ratio_range,
               xticks=xticks,
               yticks_ratio=yticks_ratio,
               mpl_xtick_params = {
                   "fontsize": 10,
                   "rotation": 0,
                   },
               mpl_ratio_params={
                   "label":"Data / Pred.",
                   },
               ax_main_callback=ax_main_callback,
               mpl_legend_params=mpl_legend_params,
               mpl_figure_params=mpl_figure_params,
               ax_ratio_callback=ax_ratio_callback,
               do_bkg_syst=True,
               override_bkg_syst=totbg.errors,
               percentbox=False,
               data_top_of_legend=True,
               )
    print "Made {}".format(fname)
    os.system("ic {}".format(fname))

if __name__ == "__main__":

    load_info("srcr",tag="run2")
    make_plots(regions=["srcr"],typ="shapes_fit_s", year="run2")
