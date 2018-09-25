import os


import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')

import numpy as np
import uproot

from matplottery.utils import Hist1D
from matplottery.plotter import plot_stack
import pickle

import xgboost as xgb

f = uproot.open("make_inputs/output_total.root")
t = f["t"]
arrs = t.arrays([
        "stype",
        "disc",
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
pass_br = arrs["br"]
stype = arrs["stype"]
disc = arrs["disc"]
# classprobs = arrs["classprobs"]
weight = arrs["weight"]
sr = arrs["SR"]
# print classprobs
# classprobs = np.array(classprobs).reshape(len(classprobs), len(classprobs[0]))
# print classprobs
# print classprobs.shape
# print 

bst = pickle.load(open("bst.pkl","rb"))
y_pred = bst.predict(xgb.DMatrix(x_data))
# print y_pred - disc

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

# bins = range(1,18+1)
bins = np.linspace(0.,1.,15+1)
# bins = range(0,20)
# bins = np.linspace(0.,11,11+1)
# comsel = (pass_br)
comsel = (pass_br)
# comsel = (pass_br > -1)
# comsel = (pass_br) & (sr>0)
# var = sr
var = 1.0*y_pred
# var = 1.0*(classprobs[:,0] > 0.2) + \
#       2.0*(classprobs[:,1] > 0.2) + \
#       3.0*(classprobs[:,2] > 0.2) + \
#       4.0*(classprobs[:,3] > 0.2)
# print var
# var = classprobs[:,0] # sig
# var = classprobs[:,1] # ttw
# var = classprobs[:,2] # ttz
# var = classprobs[:,3] # tth
# var = classprobs[:,4] # fakes
# var = classprobs[:,5] # xg
# var = classprobs[:,6] # ttvv
# var = classprobs[:,7] # rares
# var = np.ones(classprobs.shape[0])
# var[classprobs[:,1] > 0.2] = 2
# var[classprobs[:,2] > 0.1] = 3
# var[classprobs[:,3] > 0.2] = 4
# var[classprobs[:,4] > 0.2] = 5
# var[classprobs[:,5] > 0.2] = 6
# var[classprobs[:,6] > 0.2] = 6
# var[classprobs[:,7] > 0.7] = 6
# # var[classprobs[:,0] > 0.0] = 6
# var[classprobs[:,0] > 0.2] = 7
# var[classprobs[:,0] > 0.4] = 8
# var[classprobs[:,0] > 0.6] = 9
# var[classprobs[:,0] > 0.8] = 10
# var[classprobs[:,0] > 0.9] = 11
# var = disc
# var = disc-classprobs[:,4]-classprobs[:,1]-classprobs[:,2]-classprobs[:,3]
# var = disc-classprobs[:,4]
# var = disc
# var = np.rint(disc*11)+1
# var = disc
sig = Hist1D(var[comsel & (stype==0)]  , bins=bins ,weights=weight[comsel & (stype==0)] , label=dlc["tttt"][0] , color=dlc["tttt"][1])
ttw = Hist1D(var[comsel & (stype==1)]  , bins=bins ,weights=weight[comsel & (stype==1)] , label=dlc["ttw"][0]  , color =dlc["ttw"][1])
ttz = Hist1D(var[comsel & (stype==2)]  , bins=bins ,weights=weight[comsel & (stype==2)] , label=dlc["ttz"][0]  , color =dlc["ttz"][1])
tth = Hist1D(var[comsel & (stype==3)]  , bins=bins ,weights=weight[comsel & (stype==3)] , label=dlc["tth"][0]  , color =dlc["tth"][1])
fakes = Hist1D(var[comsel & (stype==4)], bins=bins ,weights=weight[comsel & (stype==4)] , label=dlc["fakes"][0], color=dlc["fakes"][1])
# print fakes
# print weight[comsel & (stype==4)]
flips = Hist1D(var[comsel & (stype==5)], bins=bins ,weights=weight[comsel & (stype==5)] , label=dlc["flips"][0], color=dlc["flips"][1])
xg = Hist1D(var[comsel & (stype==6)]   , bins=bins ,weights=weight[comsel & (stype==6)] , label=dlc["xg"][0]   , color =dlc["xg"][1])
ttvv = Hist1D(var[comsel & (stype==7)] , bins=bins ,weights=weight[comsel & (stype==7)] , label=dlc["ttvv"][0] , color=dlc["ttvv"][1])
rares = Hist1D(var[comsel & (stype==8)], bins =bins,weights=weight[comsel & (stype==8)] , label=dlc["rares"][0], color=dlc["rares"][1])

# for st in range(0,9):
#     print st,var[comsel & (stype==st)].mean()

# print weight[stype==4]

bgs = [ttw,ttz,tth,fakes,flips,xg,ttvv,rares]
bgs = sorted(bgs, key=lambda bg: bg.get_integral())

def ax_main_callback(ax):
    pass
    # ax.set_ylim([0.1,ax.get_ylim()[1]*1.5])
    # ax.set_yscale("log", nonposy='clip'),

def ax_ratio_callback(ratio):
    def f(ax):
        # ax.set_ylim([0.1,ax.get_ylim()[1]*1.5])
        # ax.set_yscale("log", nonposy='clip'),

        # print ratio
        for x,y in zip(ratio.get_bin_centers(),ratio.get_counts()):
            ax.text(x,y+0.30,"{:.2f}".format(y),
                    color="red", ha="center", va="center", fontsize=8.0,
                    wrap=True)
    return f

# help(plot_stack)
ratio = sig*0.
ratio._counts = sig._counts/np.sqrt(sum(bgs)._counts + sig._counts)
ratio._errors *= 0.
plot_stack(bgs=bgs, sigs=[sig], filename="test.png",
        ratio = ratio,
        mpl_ratio_params = {
            "label": "$s/\\sqrt{s+b}$",
            "color": "C3",
            },
        ratio_range = [0.,2.5],
        do_bkg_syst = True,

        ax_ratio_callback=ax_ratio_callback(ratio),
        ax_main_callback=ax_main_callback,
        )
os.system("ic test.png")
