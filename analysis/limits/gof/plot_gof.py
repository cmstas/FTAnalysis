#!/usr/bin/env python


import os
import numpy as np
import uproot
import commands

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery import Hist1D, plot_stack
from matplottery.utils import binomial_obs_z
# ../../common/matplottery/matplottery/utils.py


obs = -1
with open("log_gof_observed.txt", "r") as fhin:
    for line in fhin:
        if "Best fit test statistic:" in line:
            obs = float(line.split(":")[-1])
print obs

# f = uproot.open("all_toys.root")
# t = f["limit"]
# expvals = t.array("limit")

expvals = np.array(map(float,commands.getoutput("""grep "Best fit" *.txt | awk '{print $5}'""").splitlines()))


print len(expvals)
hexp = Hist1D(expvals,bins=np.linspace(100,250,50),label="toys")
# print hexp
# print hexp.vis()
# print t.keys()
# print f.keys()

# fakes_after = d_hists["shapes_fit_b"]["srhh"]["total_background"].get_integral()
# print fakes_after/fakes_before
fig, axs = plot_stack(
        bgs=[hexp],
        do_stack=False,
        title="b-only GOF",
        xlabel="test statistic",
        cms_type = "Preliminary",
        ylabel="num. toys",
        lumi="136.3",
        filename="plots/gof.png",
        mpl_hist_params=dict(histtype="step",linewidth=1.5),
        do_bkg_errors=True,
        )
axs[0].annotate(
        "observed", 
        xy=(obs,0),
        xytext=(obs,hexp.counts.max()/1.25),
        arrowprops=dict(arrowstyle="->"),
        ha="center",
        fontsize=12
        )
axs[0].annotate(
        r"$p_\mathrm{{obs}}={:.1f}\%$".format(100.*(expvals>obs).mean()), 
        xy=(0.5*(obs+expvals.mean()+expvals.std()*3),hexp.counts.max()/13),
        xytext=(1.0*(expvals.mean()+expvals.std()*3),hexp.counts.max()/1.5),
        arrowprops=dict(arrowstyle="->"),
        ha="center",
        fontsize=12
        )

print (expvals > (expvals.mean()+expvals.std())).mean()

for x in [1,2]:
    for pm in [-1,1]:
        axs[0].annotate(
                r"${}{}\sigma$".format("+" if pm>0 else "-",x),
                xy=(expvals.mean()+pm*x*expvals.std(),0),
                xytext=(expvals.mean()+pm*x*expvals.std(),hexp.counts.max()/1.05),
                color="C0",
                arrowprops=dict(arrowstyle="-",color="C0"),
                ha="center",
                fontsize=12
                )

fig.set_tight_layout(True)
fig.savefig("plots/gof.pdf")
fig.savefig("plots/gof.png")
os.system("ic plots/gof.png")

# def draw_text(text, x, y, color=r.kBlack):
#     lat = r.TLatex()
#     lat.SetNDC(1)
#     lat.SetTextColor(color) 
#     lat.SetTextAlign(13) 
#     lat.SetTextFont(42)
#     lat.SetTextSize(0.035)
#     lat.DrawLatex(x,y,text)

# def draw_line(x, ymax, width=2, style=7, color=r.kBlack):
#     line = r.TLine()
#     line.SetLineColor(color)
#     line.SetLineWidth(width)
#     line.SetLineStyle(style)
#     line.DrawLine(x,0,x,ymax)

# observed = -1
# with open("log_gof_observed.txt", "r") as fhin:
#     for line in fhin:
#         if "Best fit test statistic:" in line:
#             observed = float(line.split(":")[-1])

# nbins = 15

# ch = r.TChain("limit")
# ch.Add("all_toys.root")
# ch.Draw("limit")
# mini = ch.GetMinimum("limit")
# maxi = ch.GetMaximum("limit")
# ch.Draw("limit>>h1({},{},{})".format(nbins,mini*0.9,maxi*1.1),"", "goff")
# h1 = r.gDirectory.Get("h1")
# c1 = r.TCanvas()
# h1.SetLineColor(r.kBlue)
# h1.GetXaxis().SetTitle("test statistic")
# h1.GetYaxis().SetTitle("counts")
# h1.SetTitle("Goodness of Fit")
# h1.Draw()

# draw_text("Expected: {:.1f} #pm{:.1f}".format(h1.GetMean(),h1.GetRMS()), 0.65, 0.85, color=r.kBlue)
# draw_text("Observed: {:.1f}".format(observed), 0.65, 0.78, color=r.kBlack)

# draw_line(h1.GetMean()-h1.GetRMS(), h1.GetMaximum(), color=r.kBlue) # -1sigma
# draw_line(h1.GetMean()+h1.GetRMS(), h1.GetMaximum(), color=r.kBlue) # +1sigma
# draw_line(observed, h1.GetMaximum(), width=4, style=1, color=r.kBlack) # observed


# c1.SaveAs("plots/gof.pdf")
# os.system("ic plots/gof.pdf")

# # c1.SaveAs("plots/gof_bgonly.pdf")
# # os.system("ic plots/gof_bgonly.pdf")

