import ROOT as r
import sys
import numpy as np
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')

import os
# import pandas as pd
# from mytqdm import tqdm

import matplotlib as mpl
mpl.use('Agg')
from matplotlib import rcParams
import matplotlib.pyplot as plt
from matplottery.utils import Hist1D
from matplottery.plotter import plot_stack

import uproot
import itertools
# import root_numpy as rn

np.set_printoptions(linewidth=150)

def hist(ax, h,**kwargs):
    counts = h.counts
    yerrs = h.errors
    xerrs = 0.5*h.get_bin_widths()
    centers = h.get_bin_centers()
    do_text = kwargs.pop("text",False)
    good = counts != 0.
    patches = ax.errorbar(centers[good],counts[good],xerr=xerrs[good],yerr=yerrs[good],**kwargs)
    if do_text:
        for x,y,yerr in zip(centers[good],counts[good],yerrs[good]):
            ax.text(x,y+yerr,"{:.2f}".format(y), horizontalalignment="center",verticalalignment="bottom", fontsize=12, color=patches[0].get_color())
    return patches

def filter_arrs(arrs,sel):
    for k in arrs.keys():
        arrs[k] = arrs[k][sel]


# f = uproot.open(fname_in)
# t = f["t"]
# arrs = t.arrays(["hyp_class","br","scale1fb","nbtags","btags_flavor","fired_trigger"])
# filter_arrs(
#         arrs, 
#         (arrs["hyp_class"]==3) & (arrs["br"]) & (arrs["fired_trigger"])
#         )
# weights = 35.9*arrs["scale1fb"]
# print weights
# print arrs

ch1 = r.TChain("t")
ch1.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2017/Higgs_ttH_525.root")
ch2 = r.TChain("t")
ch2.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2017/Higgs_tHq_525.root")
ch3 = r.TChain("t")
ch3.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2017/Higgs_tHW_525.root")
ch4 = r.TChain("t")
ch4.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2017/TTHtoNonBB.root")
ch5 = r.TChain("t")
ch5.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2017/TTTTnew.root")

h_higgs = r.TH1F("higgs","",50,0,1.)
ch1.Draw("bdt_disc>>+higgs","(br && hyp_class==3)*7.78")
ch2.Draw("bdt_disc>>+higgs","(br && hyp_class==3)*4.59")
ch3.Draw("bdt_disc>>+higgs","(br && hyp_class==3)*3.86")

h_ttw = r.TH1F("ttw","",50,0,1.)
ch4.Draw("bdt_disc>>+ttw","(br && hyp_class==3)")

h_tttt = r.TH1F("tttt","",50,0,1.)
ch5.Draw("bdt_disc>>+tttt","(br && hyp_class==3)")

h_higgs.Scale(1./h_higgs.Integral())
h_ttw.Scale(1./h_ttw.Integral())
h_tttt.Scale(1./h_tttt.Integral())

higgs = Hist1D(h_higgs,label=r"$t\bar{t}H+tHW+tHq$, $m_{H}=525$",color="#5863F8")
ttw = Hist1D(h_ttw,label="$ttW$",color=[0.0, 0.4, 0.0])
tttt = Hist1D(h_tttt,label=r"SM $t\bar{t}t\bar{t}$",color="red")

bgs = [higgs,ttw,tttt]

plot_stack(
        bgs=bgs,
        filename="plots/bdt_disc_2hdm.pdf",
        do_log=False,
        xlabel=r"Raw discriminator output",
        ylabel="norm.",
        cms_type = "Preliminary",
        lumi = -1.,
        mpl_hist_params = {
            "histtype": "step",
            "stacked": False,
            "linewidth": 2.0,
            },
        do_bkg_errors=True,
        # xticks = range(2,5),
        # mpl_xtick_params=dict(fontsize=14),
        )
os.system("ic plots/bdt_disc_2hdm.pdf")


