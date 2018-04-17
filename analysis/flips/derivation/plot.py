import os
import numpy as np
import uproot

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from common.matplottery.plotter import plot_2d
from common.matplottery.utils import Hist2D

# f = uproot.open("outputs/histos_dy.root")
# f = uproot.open("outputs/histos_tt.root")
f = uproot.open("outputs/histos_both.root")

numer = Hist2D(f["numer"])
denom = Hist2D(f["denom"])
ratio = Hist2D(f["ratio"])
numer_raw = Hist2D(f["numer_raw"])

xticks,yticks = numer.get_edges()

os.system("mkdir -p plots/")

opts = {
        "do_colz": True,
        "colz_fmt": ".1e",
        "logx": True,
        "xticks": xticks,
        "yticks": yticks,
        "cms_type": "Simulation",
        "lumi": "41.3",
        "xlabel": "$p_{T}$ [GeV]",
        "ylabel": "$|\\eta|$",
        "cmap": "Blues_r",
        }
plot_2d(numer, filename="plots/numer.pdf", title="Numerator", **opts)
plot_2d(denom, filename="plots/denom.pdf", title="Denominator", **opts)
plot_2d(ratio, filename="plots/ratio.pdf", title="Rate", **opts)
opts["colz_fmt"] = ".0f"
plot_2d(numer_raw, filename="plots/numer_raw.pdf", title="Unw. numerator", **opts)
# os.system("web plots/ratio.pdf")
# os.system("ic plots/numer_raw.pdf")
os.system("ic plots/ratio.pdf")
print("Made plots")
