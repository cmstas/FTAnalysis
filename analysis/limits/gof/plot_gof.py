#!/usr/bin/env python


import os
import ROOT as r
r.gROOT.ProcessLine(".x /home/users/namin/2016/ss/master/SSAnalysis/draw/dmcStyle.C");

def draw_text(text, x, y, color=r.kBlack):
    lat = r.TLatex()
    lat.SetNDC(1)
    lat.SetTextColor(color) 
    lat.SetTextAlign(13) 
    lat.SetTextFont(42)
    lat.SetTextSize(0.035)
    lat.DrawLatex(x,y,text)

def draw_line(x, ymax, width=2, style=7, color=r.kBlack):
    line = r.TLine()
    line.SetLineColor(color)
    line.SetLineWidth(width)
    line.SetLineStyle(style)
    line.DrawLine(x,0,x,ymax)

observed = -1
with open("log_gof_observed.txt", "r") as fhin:
    for line in fhin:
        if "Best fit test statistic:" in line:
            observed = float(line.split(":")[-1])

nbins = 15

ch = r.TChain("limit")
ch.Add("all_toys.root")
ch.Draw("limit")
mini = ch.GetMinimum("limit")
maxi = ch.GetMaximum("limit")
ch.Draw("limit>>h1({},{},{})".format(nbins,mini*0.9,maxi*1.1),"", "goff")
h1 = r.gDirectory.Get("h1")
c1 = r.TCanvas()
h1.SetLineColor(r.kBlue)
h1.GetXaxis().SetTitle("test statistic")
h1.GetYaxis().SetTitle("counts")
h1.SetTitle("Goodness of Fit")
h1.Draw()

draw_text("Expected: {:.1f} #pm{:.1f}".format(h1.GetMean(),h1.GetRMS()), 0.65, 0.85, color=r.kBlue)
draw_text("Observed: {:.1f}".format(observed), 0.65, 0.78, color=r.kBlack)

draw_line(h1.GetMean()-h1.GetRMS(), h1.GetMaximum(), color=r.kBlue) # -1sigma
draw_line(h1.GetMean()+h1.GetRMS(), h1.GetMaximum(), color=r.kBlue) # +1sigma
draw_line(observed, h1.GetMaximum(), width=4, style=1, color=r.kBlack) # observed


c1.SaveAs("plots/gof.pdf")
os.system("ic plots/gof.pdf")

