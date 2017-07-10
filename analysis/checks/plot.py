import ROOT as r
import sys
import array

r.gROOT.SetBatch(1)

import os
from ROOT import gROOT, gSystem, gDirectory

from ROOT import *

def set_palette():
    #"Official" SUSY palette
    mypalette = array.array('i')

    NRGBs = 5
    NCont = 255
    stops = array.array('d', [0.00, 0.34, 0.61, 0.84, 1.00])
    red   = array.array('d', [0.50, 0.50, 1.00, 1.00, 1.00])
    green = array.array('d', [0.50, 1.00, 1.00, 0.60, 0.50])
    blue  = array.array('d', [1.00, 1.00, 0.50, 0.40, 0.50])
    FI = r.TColor.CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont)
    for i in range (0,NCont): mypalette.append(FI+i)
    r.gStyle.SetPalette(NCont,mypalette)
    r.gStyle.SetNumberContours(NCont)

def make_eff_plot(hnum,hden,title="Title",xtitle="x",fname="plots/plot.pdf"):
    c1 = r.TCanvas()
    hnum.Sumw2()
    hden.Sumw2()
    hratio = hnum.Clone(hnum.GetName()+"new")
    hratio.Divide(hratio,hden,1,1,"B")
    hratio.SetTitle(title)
    hratio.GetXaxis().SetTitle(xtitle)
    hratio.SetMinimum(0)
    hratio.SetMaximum(1)
    if "Mistag" in title:
        hratio.SetMaximum(0.15)
    hratio.Draw("pe")
    c1.SaveAs(fname)

def make_2d_plot(hist,title="Title",xtitle="x",ytitle="y",fname="plots/plot.pdf"):
    # r.gStyle.SetPalette(r.kRainBow)
    c1 = r.TCanvas()
    hist.SetTitle(title)
    hist.GetXaxis().SetTitle(xtitle)
    hist.GetYaxis().SetTitle(ytitle)
    hist.Draw("colz")
    c1.SaveAs(fname)

def main():
    r.gStyle.SetOptStat(0)

    fbtag = r.TFile("btageff/histos.root")
    fiso = r.TFile("isoeff/histos.root")

    os.system("mkdir -p plots")

    make_eff_plot(hnum=fbtag.Get("h_num_vs_pt"),hden=fbtag.Get("h_den_vs_pt"), title="B-tag eff. vs p_{T}", xtitle="p_{T}", fname="plots/beff_pt.pdf")
    make_eff_plot(hnum=fbtag.Get("h_num_vs_nj"),hden=fbtag.Get("h_den_vs_nj"), title="B-tag eff. vs Njets", xtitle="Njets", fname="plots/beff_nj.pdf")
    make_eff_plot(hnum=fbtag.Get("h_num_vs_dr"),hden=fbtag.Get("h_den_vs_dr"), title="B-tag eff. vs min #Delta R(b-tag, jet)", xtitle="min #Delta R(b-tag, jet)", fname="plots/beff_dr.pdf")
    make_eff_plot(hnum=fbtag.Get("h_mis_vs_pt"),hden=fbtag.Get("h_dms_vs_pt"), title="Mistag eff. vs p_{T}", xtitle="p_{T}", fname="plots/meff_pt.pdf")
    make_eff_plot(hnum=fbtag.Get("h_mis_vs_nj"),hden=fbtag.Get("h_dms_vs_nj"), title="Mistag eff. vs Njets", xtitle="Njets", fname="plots/meff_nj.pdf")
    make_eff_plot(hnum=fbtag.Get("h_mis_vs_dr"),hden=fbtag.Get("h_dms_vs_dr"), title="Mistag eff. vs min #Delta R(b-tag, jet)", xtitle="min #Delta R(b-tag, jet)", fname="plots/meff_dr.pdf")

    make_eff_plot(hnum=fiso.Get("h_num_vs_pt"),hden=fiso.Get("h_den_vs_pt"), title="Lepton iso. eff. for #geq 2L tttt events", xtitle="Lepton p_{T}", fname="plots/isoeff_pt.pdf")
    make_eff_plot(hnum=fiso.Get("h_num_vs_nj"),hden=fiso.Get("h_den_vs_nj"), title="Lepton iso. eff. for #geq 2L tttt events", xtitle="Njets", fname="plots/isoeff_nj.pdf")
    make_eff_plot(hnum=fiso.Get("h_num_vs_pt3"),hden=fiso.Get("h_den_vs_pt3"), title="Lepton iso. eff. for #geq 3L tttt events", xtitle="Lepton p_{T}", fname="plots/isoeff_pt3.pdf")
    make_eff_plot(hnum=fiso.Get("h_num_vs_pt2"),hden=fiso.Get("h_den_vs_pt2"), title="Lepton iso. eff. for 2L tttt events", xtitle="Lepton p_{T}", fname="plots/isoeff_pt2.pdf")
    make_eff_plot(hnum=fiso.Get("h_num_vs_pt3"),hden=fiso.Get("h_den_vs_pt3"), title="Lepton iso. eff. for #geq 3L tttt events", xtitle="Lepton p_{T}", fname="plots/isoeff_pt3.pdf")
    make_eff_plot(hnum=fiso.Get("h_num_vs_nj2"),hden=fiso.Get("h_den_vs_nj2"), title="Lepton iso. eff. for 2L tttt events", xtitle="Njets", fname="plots/isoeff_nj2.pdf")
    make_eff_plot(hnum=fiso.Get("h_num_vs_nj3"),hden=fiso.Get("h_den_vs_nj3"), title="Lepton iso. eff. for #geq 3L tttt events", xtitle="Njets", fname="plots/isoeff_nj3.pdf")

    set_palette()
    make_2d_plot(hist=fbtag.Get("h_dr_vs_pt"), title="Max #Delta R(b-tag, lepton) vs lepton p_{T}", xtitle="Lepton p_{T}", ytitle="Max #Delta R", fname="plots/dr_vs_pt.pdf")

    os.system("niceplots plots plots_checks_Jul8")

if __name__ == "__main__":


    main()
