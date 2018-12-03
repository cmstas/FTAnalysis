import os


import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')

import numpy as np
import uproot

from matplottery.utils import Hist1D
from matplottery.plotter import plot_stack
import pickle

from sklearn.metrics import roc_curve, roc_auc_score
from scipy import interpolate
from scipy.signal import medfilt, savgol_filter

import xgboost as xgb

# np.set_printoptions(precision=4,linewidth=100)
np.set_printoptions(linewidth=100)

f = uproot.open("make_inputs/output_total_withtmva.root")
t = f["t"]
# print t.keys()
arrs = t.arrays([
        "stype",
        "disc",
        "disc_tmva",
        # "classprobs",
        "weight",
        "br",
        "SR",
        ])
feature_names = [
        "nbtags",
        "njets",
        "met",
        "ptl2",
        "nlb40",
        "ntb40",
        "nleps",
        "htb",
        "q1",
        "ptj1",
        "ptj6",
        "ptj7",
        "ml1j1",
        "dphil1l2",
        "maxmjoverpt",
        "ptl1",
        "detal1l2",
        "ptj8",
        "ptl3",
        ]
featarrs = t.arrays(t.keys())
x_data = np.column_stack([featarrs[name] for name in feature_names])
pass_br = arrs["br"] == 1
stype = arrs["stype"]
disc = arrs["disc"]
disc_tmva = 0.5*arrs["disc_tmva"]+0.5
weight = arrs["weight"]
sr = arrs["SR"]

# bst = pickle.load(open("bst_ssos.pkl","rb"))
# y_pred = bst.predict(xgb.DMatrix(x_data))
# y_pred.dump("ypred.npy")
y_pred = np.load("ypred.npy")

dlc = {
        "fakes" : (r"MC fakes", [0.85, 0.85, 0.85]),
        "flips" : (r"Charge misid.", [0.4, 0.4, 0.4]),
        "rares" : (r"Rare", [1.0, 0.4, 1.0]),
        "tth"   : (r"$t\bar{t}H$", [0.4, 0.4, 0.6]),
        "ttvv"  : (r"$t\bar{t}VV$" , [0.4, 0.6, 1.0]),
        "ttw"   : (r"$t\bar{t}W$", [0.0, 0.4, 0.0]),
        "ttz"   : (r"$t\bar{t}Z$", [0.4, 0.8, 0.4]),
        "xg"    : (r"$X\gamma$" , [0.4, 0.0, 0.8]),
        "tttt" : ("tttt", "red"),
        }

def inv_sigmoid(y,k=10,x0=5):
    return (np.log(1/y-1)-x0)/(-k)
def sigmoid(x,k=10,x0=0.5):
    return 1/(1+np.exp(-k*(x-x0)))

comsel = (pass_br) # & (sr != 1))
bins = np.linspace(0.,1.,20+1)
var = 1.0*y_pred

# print bins
# # # print inv_sigmoid(bins)
# bins = sigmoid(bins)
# print bins
# # # print 1.0/(1.+np.exp(-10.*bins+5))

comsel = (pass_br) # & (sr != 1))
# bins = np.linspace(0.15,1.,15)
# bins = np.linspace(0.15,1.,20+1)
# bins = np.linspace(0.15,1.,20+1)
# bins = np.linspace(0.0,1.,20+1)
bins = np.linspace(0.0,1.,20+1)
# bins = np.array([0.0, 0.075, 0.12, 0.166, 0.21, 0.259, 0.31, 0.361, 0.413, 0.466, 0.519, 0.572, 0.625, 0.677, 0.731, 0.783, 0.833, 0.885, 0.933, 1.0])
# bins = np.array([0.15,0.715,1.0])
# bins = np.array([0.15,0.8,1.0])
# bins = np.array([0.0,0.902,1.0])
# var = 1.0*disc_tmva
var = 1.0*y_pred



fpr_tmva,tpr_tmva,thresholds_tmva = roc_curve(stype[comsel]==0, disc_tmva[comsel], sample_weight=weight[comsel])
auc_tmva = roc_auc_score(stype[comsel]==0, disc_tmva[comsel], sample_weight=weight[comsel])
fpr_xgb,tpr_xgb,thresholds_xgb = roc_curve(stype[comsel]==0, y_pred[comsel], sample_weight=weight[comsel])
auc_xgb = roc_auc_score(stype[comsel]==0, y_pred[comsel], sample_weight=weight[comsel])
tot_bg = np.sum(np.histogram(var[comsel & (stype>0)], weights=weight[comsel & (stype>0)])[0])
points = []
for k in np.linspace(0.01,0.99999*tot_bg,100):
    t_xgb = thresholds_xgb[np.where(tot_bg*fpr_xgb >= k)[0][0]]
    t_tmva = thresholds_tmva[np.where(tot_bg*fpr_tmva >= k)[0][0]]
    # scale from 0.15-1.0 to 0.0-1.0
    t_tmva = (t_tmva-thresholds_tmva.min())/(1.0-thresholds_tmva.min())
    points.append([t_xgb,t_tmva])
points.append([0.,0])
points.append([1.,1])
points = np.array(points)
points = points[points[:,0].argsort()]
finterp = interpolate.interp1d(points[:,0], points[:,1],fill_value=(0.,1.))
pickle.dump(points,open("for_finterp.pkl","wb"))


# print "finterp(bins)",finterp(np.linspace(0.,1.,20+1))
# # print points
# finterp_inv = interpolate.interp1d(points[:,1], points[:,0],fill_value=(0.,1.))
# print "finterp_inv(bins)",finterp_inv(np.linspace(0.,1.,20+1))


# nbins = 20
# cuts = np.linspace(0.,1,nbins+1)[::-1]
# cuts = np.concatenate([np.linspace(0.0,0.5,6)[:-1],np.linspace(0.5,1.0,13)])[::-1]
# bins = []
# for cut in cuts:
#     t_xgb = thresholds_xgb[np.where(tpr_xgb >= cut)[0][0]]
#     bins.append(t_xgb)
# bins = np.array(bins)
# print bins
# # print cuts
# # print cuts.shape
# print tpr_xgb

def sigmoid_corr(x,k=8.0):
    k = k*np.ones(len(x))
    k[x<0.5] *= 0.5
    return 1.0/(1+np.exp(-k*(x-0.5)))+2.0*(x-0.5)/(1+np.exp(k/2))

# nbins, k = 13, 8.3
nbins, k = 20, 8.3
bins = np.linspace(0.,1.,nbins+1)
# bins = sigmoid_corr(bins,k)
# bins = sigmoid_corr(bins,k)
print bins
bins[0] = 0.
bins[-1] = 1.
print bins
print bins.shape

# # FIXME
# bins = np.array([0.0, 0.7680363, 0.8841157, 0.92737496, 0.95956606, 0.9679797, 0.9755356])

# blah

# # bins = finterp_inv(bins)
# popt = [8.90529061122706, 1.0, 0.0225909019370698, -0.0076777665868534685]
# print "before:",bins
# bins = sigmoid(bins,*popt)
# print "after:",bins

# # FIXME
# # stretch to match tmva distributions
# new_ypred = finterp(y_pred)
# mini = new_ypred.min()
# print mini
# new_ypred = (new_ypred-mini)/(1.0-mini)
# print new_ypred.min()
# var = new_ypred

use_srs = False

if use_srs:
    comsel = (pass_br) & (sr > 0)
    bins = np.linspace(sr[comsel].min()-0.5,sr[comsel].max()+0.5,(sr[comsel].max()-sr[comsel].min())+2)
    var = 1.0*sr

def get_mean(sel):
    return " ($\\mu$={:.2f})".format(np.average(var[sel],weights=weight[sel]))
mlabel = get_mean(comsel & (stype==0))
sig = Hist1D(var[comsel & (stype==0)]  , bins=bins ,weights=weight[comsel & (stype==0)] , label=dlc["tttt"][0]+mlabel , color=dlc["tttt"][1])
mlabel = get_mean(comsel & (stype==1))
ttw = Hist1D(var[comsel & (stype==1)]  , bins=bins ,weights=weight[comsel & (stype==1)] , label=dlc["ttw"][0]+mlabel  , color =dlc["ttw"][1])
mlabel = get_mean(comsel & (stype==2))
ttz = Hist1D(var[comsel & (stype==2)]  , bins=bins ,weights=weight[comsel & (stype==2)] , label=dlc["ttz"][0]+mlabel  , color =dlc["ttz"][1])
mlabel = get_mean(comsel & (stype==3))
tth = Hist1D(var[comsel & (stype==3)]  , bins=bins ,weights=weight[comsel & (stype==3)] , label=dlc["tth"][0]+mlabel  , color =dlc["tth"][1])
# fakes = Hist1D(var[comsel & (stype==4)], bins=bins ,weights=weight[comsel & (stype==4)] , label=dlc["fakes"][0], color=dlc["fakes"][1])
# flips = Hist1D(var[comsel & (stype==5)], bins=bins ,weights=weight[comsel & (stype==5)] , label=dlc["flips"][0], color=dlc["flips"][1])
# xg = Hist1D(var[comsel & (stype==6)]   , bins=bins ,weights=weight[comsel & (stype==6)] , label=dlc["xg"][0]   , color =dlc["xg"][1])
# ttvv = Hist1D(var[comsel & (stype==7)] , bins=bins ,weights=weight[comsel & (stype==7)] , label=dlc["ttvv"][0] , color=dlc["ttvv"][1])
# rares = Hist1D(var[comsel & (stype==8)], bins =bins,weights=weight[comsel & (stype==8)] , label=dlc["rares"][0], color=dlc["rares"][1])

mlabel = get_mean(comsel & ((stype==4) | (stype==5)))
others = Hist1D(var[comsel & ((stype==4) | (stype==5))], bins=bins ,weights=weight[comsel & ((stype==4) | (stype==5))] , label="Others"+mlabel, color=dlc["fakes"][1])
mlabel = get_mean(comsel & (stype>=6))
rares = Hist1D(var[comsel & (stype>=6)], bins=bins ,weights=weight[comsel & (stype>=6)] , label="Rares"+mlabel, color=dlc["ttvv"][1])

bgs = sorted([ttw,ttz,tth,others,rares], key=lambda bg: bg.get_integral())
sbgs = sum(bgs)

print sbgs
print sig

tot_bg = sbgs.get_integral()
tot_sig = sig.get_integral()
print tot_bg, tot_sig

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

fname = "plot.pdf"


fig, ax = plt.subplots()
ax.plot(fpr_tmva,tpr_tmva, label="tmva [AUC = {:.3f}]".format(auc_tmva))
ax.plot(fpr_xgb,tpr_xgb, label="xgb [AUC = {:.3f}]".format(auc_xgb))
ax.set_xlabel("bkg eff")
ax.set_ylabel("sig eff")
ax.set_xlim([0.01,0.9])
ax.set_ylim([0.2,1.0])
ax.set_title("ROC curves for tmva, xgb")
ax.legend()
fig.set_tight_layout(True)
# fig.savefig(fname)
fig.savefig("roc_tmva_xgb.pdf")
# os.system("ic {}".format(fname))


xfit = np.linspace(0.0,1,100)
yfit = finterp(xfit)
# yfit_inv = finterp_inv(xfit)
fig, ax = plt.subplots()
ax.plot(xfit,yfit, linewidth=3.0, label="interp")
# ax.plot(xfit,yfit_inv, linewidth=3.0, label="interp inverse")
ax.plot(points[:,0],points[:,1], "o", label="calculated")
ax.set_xlabel("xgb threshold")
ax.set_ylabel("tmva threshold")
ax.set_xlim([0.,1.])
ax.set_ylim([0.,1.])
# ax.set_title("ROC curves for tmva, xgb")
ax.legend()
fig.set_tight_layout(True)
fig.savefig(fname)
# os.system("ic {}".format(fname))

fig, ax = plt.subplots()
s,b = tot_sig*tpr_tmva, tot_bg*fpr_tmva
sigs = s/(s+b)**0.5
ax.plot(thresholds_tmva,sigs, label="tmva $s/\\sqrt{{s+b}}$")
am = np.nanargmax(sigs)
ax.plot([0.,1.],[np.nanmax(sigs)]*2,color="C0",linestyle="dashed",lw=1, label="max = {:.3f}, cut = {:.3f}".format(np.nanmax(sigs), thresholds_tmva[am]))
# sigs = np.sqrt(2*(s+b)*np.log(1+s/b)-2*s)
# ax.plot(thresholds_tmva[np.isfinite(sigs)][::1000],sigs[np.isfinite(sigs)][::1000], label="tmva $s/\\sqrt{s+b}$ better")
# print sigs[am]
s,b = tot_sig*tpr_xgb, tot_bg*fpr_xgb
sigs = s/(s+b)**0.5
ax.plot(thresholds_xgb,sigs, label="xgb $s/\\sqrt{{s+b}}$")
am = np.nanargmax(sigs)
ax.plot([0.,1.],[np.nanmax(sigs)]*2,color="C1",linestyle="dashed",lw=1, label="max = {:.3f}, cut = {:.3f}".format(np.nanmax(sigs), thresholds_xgb[am]))
# ax.plot(finterp(thresholds_xgb),sigs, label="xgb stretched $s/\\sqrt{{s+b}}$")
# sigs = np.sqrt(2*(s+b)*np.log(1+s/b)-2*s)
# # a = np.convolve(0.0001*np.ones(10000,"d"),sigs[np.isfinite(sigs)],mode="same")
# ax.plot(thresholds_xgb[np.isfinite(sigs)][::1000],sigs[np.isfinite(sigs)][::1000], label="xgb $s/\\sqrt{s+b}$ better")
# print sigs[am]
ax.set_xlim([0.,1.])
ax.set_xlabel("threshold")
ax.set_ylabel("$s/\\sqrt{s+b}$")
ax.set_title("cumulative significance vs threshold")
ax.legend()
fig.set_tight_layout(True)
fig.savefig(fname)
fig.savefig("sosb_tmva_xgb.pdf")
# os.system("ic {}".format(fname))


# print sbgs.counts
# print sbgs.edges
# # fpr,tpr,thresholds = roc_curve(stype[comsel]==0, disc_tmva[comsel], sample_weight=weight[comsel])
# fpr,tpr,thresholds = roc_curve(stype[comsel]==0, y_pred[comsel], sample_weight=weight[comsel])
# fpr *= sbgs.get_integral()
# print "fpr",fpr
# print "tpr",tpr
# print "thr",thresholds
# binmap = []
# for icnt,cnt in enumerate(np.cumsum(sbgs.counts[::-1])[::-1]):
#     thresh = thresholds[np.where(fpr>=cnt)[0][0]]
#     print icnt,cnt,thresh
#     binmap.append(thresh)
# binmap = np.array([0.]+binmap+[1.])
# print binmap
# print binmap.tolist()

def edit_edges(h):
    h._edges = np.arange(len(h._counts)+1)
# map(edit_edges,[sig,ttw,ttz,tth,fakes,flips,xg,ttvv,rares])
map(edit_edges,[sig]+bgs)
ratio = sig*0.
ratio._counts = sig._counts/np.sqrt(sbgs._counts + sig._counts)
ratio._errors *= 0.

def ax_main_callback(ax):
    pass
    # ax.set_ylim([0.1,ax.get_ylim()[1]*1.5])
    # ax.set_yscale("log", nonposy='clip'),

# import matplotlib.font_manager as fm

def ax_ratio_callback(ratio):
    def f(ax):
        # ax.set_ylim([0.1,ax.get_ylim()[1]*1.5])
        # ax.set_yscale("log", nonposy='clip'),

        # print ratio
        # fsize = fm.FontProperties(size=6)
        # ax.yaxis.get_label().set_fontproperties(fsize)
        for x,y in zip(ratio.get_bin_centers(),ratio.get_counts()):
            ax.text(x,y+0.30,"{:.2f}".format(y),
                    color="red", ha="center", va="center", fontsize=7.0,
                    wrap=True)
    return f


# help(plot_stack)
# ratio = sig*0.
# ratio._counts = sig._counts/np.sqrt(sum(bgs)._counts + sig._counts)
# ratio._errors *= 0.
# # pg 7 of http://www.phys.ufl.edu/~korytov/phz6355/note_A13_statistics.pdf
# s,b,berr = sig._counts, sum(bgs).counts, sum(bgs).errors
# ratio._counts = np.sqrt(2*(s+b)*np.log(1+s/b)-2*s)
# import ROOT as r
# def get_z((s_,b_,berr_)):
#     return r.RooStats.NumberCountingUtils.BinomialExpZ(max(s_,0.), max(b_,0.), max(berr_,0.)/max(b_,0.0001))
# ratio._counts = np.array(map(get_z, zip(s,b,berr)))
# print ratio._counts

plot_stack(bgs=bgs, sigs=[sig], filename=fname,
        ratio = ratio,
        mpl_ratio_params = {
            "label": "$s/\\sqrt{s+b}$",
            # "label": "$\\sqrt{2(s+b)ln(1+s/b)-2s}$",
            "color": "C3",
            },
        ratio_range = [0.,2.8],
        # ratio_range = [-1.,2],
        do_bkg_syst = True,

        ax_ratio_callback=ax_ratio_callback(ratio),
        ax_main_callback=ax_main_callback,
        )
os.system("ic {}".format(fname))

from significances.helper import get_significance
# to be conservative, clip the signal if it's negative, but abs() for bkg
yields = dict(
    tttt=np.clip(sig.counts,0.,999.),
    ttw=np.abs(ttw.counts),
    tth=np.abs(tth.counts),
    ttz=np.abs(ttz.counts),
    # fakes=np.abs(fakes.counts),
    # flips=np.abs(flips.counts),
    # xg=np.abs(xg.counts),
    # ttvv=np.abs(ttvv.counts),
    others=np.abs(others.counts),
    rares=np.abs(rares.counts),
    )
errors = dict(
    tttt=sig.errors,
    ttw=ttw.errors,
    tth=tth.errors,
    ttz=ttz.errors,
    # fakes=fakes.errors,
    # flips=flips.errors,
    # xg=xg.errors,
    # ttvv=ttvv.errors,
    others=others.errors,
    rares=rares.errors,
    )
print get_significance(yields,errors,do_mcstats=False)
print get_significance(yields,errors,do_mcstats=True)
