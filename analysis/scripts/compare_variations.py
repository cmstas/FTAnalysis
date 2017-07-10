import ROOT as r
import numpy as np


import os
from ROOT import gROOT, gSystem, gDirectory
gROOT.ProcessLine(".L /home/users/namin/2016/ss/master/SSAnalysis/batch_new/compare/../../commonUtils.h");
gSystem.Load("/home/users/namin/2016/ss/master/SSAnalysis/batch_new/compare/../../CORE/CMS3_CORE.so");
gROOT.ProcessLine(".L /home/users/namin/2016/ss/master/SSAnalysis/batch_new/compare/../../CORE/SSSelections.cc");
gROOT.ProcessLine(".x /home/users/namin/2016/ss/master/SSAnalysis/batch_new/compare/../../draw/dmcStyle.C");


from ROOT import *

def make_aliases(ch):
    ch.SetAlias("lep1_ptgood", "lep1_coneCorrPt > 25")
    ch.SetAlias("lep2_ptgood", "lep2_coneCorrPt > 20")

    ch.SetAlias("pt1", "lep1_coneCorrPt");
    ch.SetAlias("pt2", "lep2_coneCorrPt");

    ch.SetAlias("eta1", "lep1_p4.eta()");
    ch.SetAlias("eta2", "lep2_p4.eta()");

    ch.SetAlias("id1", "lep1_id");
    ch.SetAlias("id2", "lep2_id");

    # ch.SetAlias("class3", "hyp_class == 3");
    ch.SetAlias("class3", "hyp_class == 3 || hyp_class == 4");
    ch.SetAlias("filters", "(fired_trigger) && passes_met_filters");
    ch.SetAlias("baseline", "njets>=2 && (met>=50) && (nbtags>=2) && (ht>300) && lep1_ptgood && lep2_ptgood");
    ch.SetAlias("good", "baseline && filters && class3");

    ch.SetAlias("mt1", "sqrt(2.0*pt1*met*(1-cos(lep1_p4.phi()-metPhi)))");
    ch.SetAlias("mt2", "sqrt(2.0*pt2*met*(1-cos(lep2_p4.phi()-metPhi)))");
    ch.SetAlias("mtmin_new", "mt1*(mt2>mt1)+mt2*(mt2<=mt1)");


    # sr = "signalRegion2016(njets,nbtags,met,ht,mtmin_new,id1,id2,pt1,pt2)"

def make_plots():

    fnames_map = {
            "nominal": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_osft/output/TTTTnew.root",
            "isrup": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_osft/output/TTTTisrup.root",
            "isrdown": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_osft/output/TTTTisrdown.root",
            "fsrup": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_osft/output/TTTTfsrup.root",
            "fsrdown": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_osft/output/TTTTfsrdown.root",
            }

    chain_map = {}
    for key in fnames_map:
        chain_map[key] = r.TChain("t")
        make_aliases(chain_map[key])
        chain_map[key].Add(fnames_map[key])

    thedir = "plots_variations/"
    os.system("mkdir -p %s" % thedir)

    plots = [

            ("njets", "Njets", "njets", "(11,0,11)", "1"),
            ("nbtags", "Nbtags", "nbtags", "(11,0,11)", "1"),
            ("mtmin", "m_{T}^{min}", "mtmin", "(10,0,300)", "1"),
            ("lep1pt", "p_{T}(lep1)", "lep1_coneCorrPt", "(10,0,300)", "1"),
            ("lep2pt", "p_{T}(lep2)", "lep2_coneCorrPt", "(10,0,200)", "1"),
            ("mll", "m_{ll}", "dilep_p4.M()", "(10,0,300)", "1"),
            ("met", "MET", "met", "(15,0,800)", "1"),
            ("ht", "H_{T}", "ht", "(15,0,1500)", "1"),
            ]


    for shortname, title, variable, bins, selection in plots:

        for do_normalize in [True,False]:

            names = fnames_map.keys()
            print "started drawing %s" % title

            extra = "norm" if do_normalize else ""

            d_hists = {}
            for name in names:
                N = chain_map[name].Draw("%s>>h1%s%s%s" % (variable, name, extra, bins),"(35.9*scale1fb) * ((%s && good) )" % selection, "goff")
                d_hists[name] = gDirectory.Get("h1"+name+extra)

            c1 = TCanvas("c1")
            pads = []
            pads.append(TPad("1","1",0.0,0.16,1.0,1.0))
            pads.append(TPad("2","2",0.0,0.0,1.0,0.17))
            pads[0].SetTopMargin(0.08)
            pads[0].SetLeftMargin(0.12)
            pads[0].SetBottomMargin(0.12)
            pads[1].SetLeftMargin(0.12)
            pads[0].Draw()
            pads[1].Draw()
            pads[0].cd()

            map(lambda x: x.Sumw2(), d_hists.values())
            h1 = d_hists.get("nominal")
            h1.GetYaxis().SetTitle("Events")
            if do_normalize:
                h1.SetTitle("%s [norm. to nominal]" % (title))
            else:
                h1.SetTitle("%s" % (title))
            h1.GetXaxis().SetTitle(title)

            maxy = max(map(lambda x: x.GetMaximum(), d_hists.values()))

            h1.SetMaximum(1.2*maxy)
            h1.SetMinimum(0.0)

            h1.SetLineColor(kBlack)
            h1.SetMarkerColor(kBlack)

            rest = ["isrup","isrdown","fsrup","fsrdown"]
            colors = [r.kRed-2,r.kRed-4,r.kAzure-2,r.kAzure-4]

            # do_normalize = True
            h1.Draw("histe")
            for iname,name in enumerate(rest):
                hist = d_hists[name]
                if do_normalize:
                    hist.Scale(h1.Integral()/hist.Integral())
                hist.SetLineColor(colors[iname])
                hist.SetMarkerColor(colors[iname])
                hist.Draw("histesame")
            h1.Draw("histesame")

            leg = TLegend(0.75,0.75,0.95,0.90)
            def get_integral_string(h):
                return " [%.2f]" % h.Integral()
            leg.AddEntry(h1, "nominal"+get_integral_string(h1))
            for iname,name in enumerate(rest):
                hist = d_hists[name]
                leg.AddEntry(hist, name+get_integral_string(hist))
            leg.Draw("same")

            pads[1].cd()


            # for orig,ratio in zip([h2,h3],[r2,r3]):
            d_ratios = {}
            for iname,name in enumerate(rest):
                orig = d_hists[name]
                d_ratios[name] = orig.Clone("ratio"+name)
                ratio = d_ratios[name]
            # for orig,ratio in [[h2,r2]]:
                ratio.Sumw2()
                ratio.Divide(h1)
                ratio.Draw("samepe")
                ratio.SetMarkerColor(orig.GetMarkerColor())
                ratio.SetLineColor(orig.GetLineColor())
                ratio.GetYaxis().SetRangeUser(0.6,1.4)
                ratio.GetYaxis().SetNdivisions(204,False)
                ratio.SetTitle("")
                ratio.GetYaxis().SetTitle("*/nominal")        
                ratio.GetYaxis().SetTitleSize(0.18)
                ratio.GetYaxis().SetTitleOffset(0.30)
                ratio.GetYaxis().SetLabelSize(0.13)
                ratio.GetYaxis().CenterTitle()
                ratio.GetXaxis().SetLabelSize(0.0)
                ratio.GetXaxis().SetTitle("")
                ratio.GetXaxis().SetTickSize(0.06)
            line = TLine()
            line.SetLineColor(kGray+2)
            line.SetLineWidth(2)
            line.SetLineStyle(7)
            xmin = ratio.GetXaxis().GetBinLowEdge(1)
            xmax = ratio.GetXaxis().GetBinUpEdge(h1.GetNbinsX())
            line.DrawLine(xmin,1,xmax,1)

            print "done drawing"
            if do_normalize:
                c1.SaveAs("%s/%s_norm.pdf" % (thedir,shortname))
            else:
                c1.SaveAs("%s/%s.pdf" % (thedir,shortname))

if __name__ == "__main__":

    make_plots()
    os.system("niceplots plots_variations plots_ttttisrfsr_Jun5")

