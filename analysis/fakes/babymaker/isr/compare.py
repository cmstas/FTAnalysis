import pickle
import os
import ROOT as r
import numpy as np

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery.plotter import plot_2d, plot_stack
from matplottery.utils import Hist2D, Hist1D
# sys.path.insert(0,'../derivation/')
# from plottery.utils import move_in_overflows



ch_tt0 = r.TChain("t")
ch_tt0.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2018/TTdilep0jet.root")
ch_tt0.Draw("nisrMatch>>h_tt0(4,0,4)","(hyp_class==4) * scale1fb","norm")

ch_tt1 = r.TChain("t")
ch_tt1.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2018/TTdilep1jet.root")
ch_tt1.Draw("nisrMatch>>h_tt1(4,0,4)","(hyp_class==4) * scale1fb","norm")

ch_ttw = r.TChain("t")
ch_ttw.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2018/TTWnlo.root")
ch_ttw.Draw("nisrMatch>>h_ttw(4,0,4)","(hyp_class==4) * scale1fb","norm")

ch_ttz = r.TChain("t")
ch_ttz.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2018/TTZnlo.root")
ch_ttz.Draw("nisrMatch>>h_ttz(4,0,4)","(hyp_class==4) * scale1fb","norm")

# 1 parton -> ttz
# 0 parton -> ttw


h_tt0 = Hist1D(r.gDirectory.Get("h_tt0"),label="tt+0parton")
h_tt1 = Hist1D(r.gDirectory.Get("h_tt1"),label="tt+1partons")
h_ttw = Hist1D(r.gDirectory.Get("h_ttw"),label="ttW")
h_ttz = Hist1D(r.gDirectory.Get("h_ttz"),label="ttZ")

which = "ttw"
for which in ["ttw","ttz"]:
    fname = "plots_dilep/{}.pdf".format(which)
    if which == "ttw":
        bgs = [h_tt0,h_ttw]
    else:
        bgs = [h_tt1,h_ttz]
    plot_stack(
            bgs=bgs,
            filename=fname,
            mpl_hist_params = {
                "histtype": "step",
                "stacked": False,
                "linewidth": 2.0,
                },
            do_bkg_errors = True,
            xlabel="nISR",
            ylabel="Events",
            title="nISR jets (genmatched)",
            lumi = 58.8,
            ratio_range=[0.,2.],
            # do_bkg_syst = True,
            mpl_legend_params = dict(
                ncol=2,
                )
            )
    os.system("ic "+fname)

