import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')

import os
import numpy as np
import pandas as pd
from mytqdm import tqdm

import matplotlib as mpl
mpl.use('Agg')
from matplotlib import rcParams
import matplotlib.pyplot as plt
from matplottery.utils import Hist1D
from matplottery.plotter import plot_stack

import uproot
import itertools
import root_numpy as rn

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

fname_in = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.13_all/output/year_2016/TTWnlo.root"
arr2016 = rn.root2array(fname_in,"t", branches=[
    "nbtags",
    "Sum$(abs(btags_flavor)==5)",
    "Sum$(abs(btags_flavor)==4)",
    "nbtags-(Sum$(abs(btags_flavor)==5)+Sum$(abs(btags_flavor)==4))",
    "(35.9)*scale1fb",
    ],
    selection="hyp_class==3 && br && fired_trigger && passes_met_filters",
    )
arr2016.dtype.names = (
        "nbtags",
        "ntrueb",
        "ntruec",
        "ntruelight",
        "weight",
        )

fname_in = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.13_all/output/year_2017/TTWnlo.root"
arr2017 = rn.root2array(fname_in,"t", branches=[
    "nbtags",
    "Sum$(abs(btags_flavor)==5)",
    "Sum$(abs(btags_flavor)==4)",
    "nbtags-(Sum$(abs(btags_flavor)==5)+Sum$(abs(btags_flavor)==4))",
    "(41.5)*scale1fb",
    ],
    selection="hyp_class==3 && br && fired_trigger && passes_met_filters",
    )
arr2017.dtype.names = (
        "nbtags",
        "ntrueb",
        "ntruec",
        "ntruelight",
        "weight",
        )
arr = np.hstack([arr2016,arr2017])

# sel = (arr["ntrueb"]==2)
# sel = (arr["ntruelight"]==2)
bins = [1.5,2.5,3.5,4.5]
colors = [
        [0.0, 0.4, 0.0],
        [0.4, 0.8, 0.4],
        [1.0,0.8,0.0],
        [1.0,0.6,0.0],
        [0.4, 0.0, 0.8],
        [1.0, 0.4, 1.0],
        [0.4, 0.6, 1.0],
        [0.85, 0.85, 0.85],
        [0.4, 0.4, 0.4],
        [0.4, 0.4, 0.6],
        ]

hists = []
for color,(ntb,ntc,ntl) in zip(colors,[
        [2,0,0],
        [1,1,0],
        [1,0,1],
        [2,1,0],
        [2,0,1],
        [3,0,0],
        [4,0,0],
        [0,1,1],
        [1,1,1],
        [2,1,1],
        ]):
    sel = (arr["ntrueb"]==ntb) & (arr["ntruec"]==ntc) & (arr["ntruelight"]==ntl)
    # h = Hist1D(arr["nbtags"][sel], weights=arr["weight"][sel],bins=bins, label="{}b+{}c+{}l".format(ntb,ntc,ntl),color=color)
    h = Hist1D(arr["nbtags"][sel], weights=arr["weight"][sel], bins=bins, label="{}b+{}c+{}l={} reco-b".format(ntb,ntc,ntl,ntb+ntc+ntl),color=color)
    hists.append(h)
    print ntb,ntc,ntl,h


plot_stack(
        bgs=hists[::-1],
        filename="plots_btag/ttw_btag_composition.pdf",
        logy=True,
        xlabel=r"$\mathrm{N_b}$",
        ylabel="Events",
        cms_type = "Preliminary",
        lumi = 35.9+41.5,
        xticks = range(2,5),
        mpl_xtick_params=dict(fontsize=14),
        )
os.system("ic plots_btag/ttw_btag_composition.pdf")


# norms = Hist1D(f["weight"]).counts
# t = f["t"]
# varname = "sr"
# var = arrs[varname]
# lumi = 41.5
# weights_nom = lumi*arrs["scale1fb"]

# pdfweights = arrs["pdfweights"]
# nrows = len(pdfweights)
# ncols = len(pdfweights[0])
# print nrows, ncols
# print pdfweights
# print pdfweights.content
# regweights = np.reshape(pdfweights, (nrows,ncols))
# transpweights = regweights.T

# # print regweights[0]
# print (regweights).std(axis=0).std()
# print (regweights)[:,:100].std(axis=0).std()
# print (regweights)[:,:100]

# sys.exit()

# bins = np.arange(1.5,19.5,1.)
# hnom = Hist1D(var,bins=bins,weights=weights_nom)

# # print norms[9:]
# # print norms[0]

# fig, ax = plt.subplots()
# # for ipdf in range(0,len(transpweights)):
# data = []
# for ipdf in tqdm(range(0,len(transpweights))):
#     # if ipdf>100:
#     #     break
#     # if not(800<ipdf<900):
#     #     continue
#     # if ipdf%10 != 0:
#     #     continue
#     normfact = norms[0]/norms[9+ipdf]
#     h = Hist1D(var,bins=bins,weights=weights_nom*transpweights[ipdf])
#     h *= float(normfact)
#     # hist(ax,h,lw=0.1,fmt="o",markersize=.0,color="k")
#     data.append(h.counts)
# fig.tight_layout()
# fname = "test.png"
# fig.savefig(fname)
# os.system("ic "+fname)
# data = np.array(data)
# print data.std(axis=0)
# print data

# print data.sum(axis=1)
# # print data.sum(axis=1).std()
# print "rel std of acceptances (%): {:.2f}%".format(100.*data.sum(axis=1).std()/data[0].sum())

# srcr = hnom.get_bin_centers()
# crms = 100.*data.std(axis=0)/data[0]
# cmm = 100.*(data.max(axis=0)-data.min(axis=0))/data[0]
# cstat = 100.*hnom.errors/hnom.counts

# from analysis.utils.pytable import Table
# tab = Table()
# tab.set_theme_csv()
# tab.set_column_names(["bin","1sigma","max-min","stat."])
# for row in zip(srcr,crms,cmm,cstat):
#     tab.add_row(row)
# tab.print_table(show_row_separators=False,show_alternating=False)
# # elif which == "pdf":
# #     wup = arrs["weight_pdf_UP"]
# #     wdown = arrs["weight_pdf_DN"]
# #     wup[wup < -9000] = 1.
# #     wdown[wdown < -9000] = 1.
# #     weights_up = lumi*arrs["scale1fb"]* wup * norms.counts[0]/norms.counts[3]
# #     weights_down = lumi*arrs["scale1fb"]* wdown * norms.counts[0]/norms.counts[4]

# # if not len(var):
# #     print "no events for {}".format(sample)
# #     continue

# # bins = np.arange(0.5,max(var.max()+1.5,5.5),1.)
# # # bins = np.linspace(0.5,var.max()+1.5,50)
# # var_nom = Hist1D(var, weights=weights_nom, bins=bins)
# # var_up = Hist1D(var, weights=weights_up, bins=bins)
# # var_down = Hist1D(var, weights=weights_down, bins=bins)

# # if var_nom.get_integral() < 1.e-6:
# #     print "no events for {}".format(sample)
# #     continue

# # # var_up *= var_nom.get_integral()/var_up.get_integral()
# # # var_down *= var_nom.get_integral()/var_down.get_integral()

# # int_nom = var_nom.get_integral()
# # int_up = var_up.get_integral()
# # int_down = var_down.get_integral()
# # pct_up = 100.*(int_up/int_nom-1.)
# # pct_down = 100.*(int_down/int_nom-1.)
# # normfact_up = var_nom.counts.sum()/var_up.counts.sum()
# # normfact_down = var_nom.counts.sum()/var_down.counts.sum()
# # pctresids = 100.0*np.r_[np.abs(normfact_up*var_up.counts/var_nom.counts-1),np.abs(normfact_down*var_down.counts/var_nom.counts-1)]
# # medpctresids = np.median(pctresids[np.isfinite(pctresids)])


# # fig, ax = plt.subplots()

# # # ax.set_yscale("log",nonposy="clip")

# # hist(ax,var_nom,lw=1.5,fmt="o",markersize=5.0,label="nominal [N={:.2f}]".format(int_nom),text=False,color="k")
# # hist(ax,var_up,lw=1.5,fmt="o",markersize=5.0,label="{} up [{:+.1f}%]".format(which,pct_up),text=False,color="C0")
# # hist(ax,var_down,lw=1.5,fmt="o",markersize=5.0,label="{} down [{:+.1f}%]".format(which,pct_down),text=False,color="C3")
# # # print var_nom
# # ax.set_xlabel(varname)
# # ax.set_title("[{}] {} acceptance+shape, {}".format(year,which,nicesample))
# # ax.legend()
# # fig.tight_layout()
# # fname = "plots/y{}_{}_{}_{}.pdf".format(year,which,region,sample)
# # fig.savefig(fname)
# # fig.savefig(fname.replace(".pdf",".png"))
# # # os.system("ic "+fname)
