import os
import numpy as np
import uproot
import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery.plotter import plot_2d
from matplottery.utils import Hist2D, register_root_palettes
import matplotlib

# f = uproot.open("outputs/histos_dy.root")
# f = uproot.open("outputs/histos_tt.root")
f = uproot.open("outputs/histos_both.root")

numer = Hist2D(f["numer"])
denom = Hist2D(f["denom"])
ratio = Hist2D(f["ratio"])
numer_raw = Hist2D(f["numer_raw"])

xticks,yticks = numer.get_edges()

os.system("mkdir -p plots/")

register_root_palettes()

opts = {
        "do_colz": True,
        "logx": True,
        "xticks": xticks,
        "yticks": yticks,
        "cms_type": "Simulation",
        "lumi": "41.3",
        "xlabel": "$p_{T}$ [GeV]",
        "ylabel": "$|\\eta|$",
        "cmap": "Blues_r",
        # "cmap": "PuBu",
        # "cmap": "kBird",
        # "cmap": "SUSY",
        }
opts["colz_fmt"] = ".1f"
plot_2d(numer, filename="plots/numer.pdf", title="Numerator", **opts)
plot_2d(denom, filename="plots/denom.pdf", title="Denominator", **opts)

opts["colz_fmt"] = ".0f"
plot_2d(numer_raw, filename="plots/numer_raw.pdf", title="Unw. numer.", **opts)

opts["colz_fmt"] = ".1e"
opts["mpl_2d_params"] = dict(
    norm=matplotlib.colors.LogNorm(vmin=1.e-6,vmax=1.e-2),
    )
plot_2d(ratio, filename="plots/ratio.pdf", title="Rate", **opts)

# os.system("ic plots/denom.pdf")
# os.system("ic plots/numer_raw.pdf")
os.system("ic plots/ratio.pdf")
print("Made plots")
