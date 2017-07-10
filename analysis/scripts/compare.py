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

    ch.SetAlias("class3", "hyp_class == 3");
    ch.SetAlias("filters", "(fired_trigger) && passes_met_filters");
    ch.SetAlias("baseline", "njets>=2 && (met>=50) && (nbtags>=2) && (ht>300) && lep1_ptgood && lep2_ptgood");
    ch.SetAlias("good", "baseline && filters && class3");

    ch.SetAlias("mt1", "sqrt(2.0*pt1*met*(1-cos(lep1_p4.phi()-metPhi)))");
    ch.SetAlias("mt2", "sqrt(2.0*pt2*met*(1-cos(lep2_p4.phi()-metPhi)))");
    ch.SetAlias("mtmin_new", "mt1*(mt2>mt1)+mt2*(mt2<=mt1)");


    # sr = "signalRegion2016(njets,nbtags,met,ht,mtmin_new,id1,id2,pt1,pt2)"

def make_plots():
    old = r.TChain("t")
    new = r.TChain("t")
    make_aliases(old)
    make_aliases(new)
    # oldname = "old"
    # newname = "new"
    # rtitle = "new/old"
    # old_fnames = "output_igprof.root"
    # new_fnames = "output_igprofmaodmva.root"
    # old_fnames = "/nfs-7/userdata/namin/tupler_babies/merged//FT/v0.04_scalepdf/output/TTTT.root"
    # new_fnames = "../TTTTnew.root"

    # old_fnames = "../TTTW_4f.root"
    # new_fnames = "../TTTW_5f.root"
    # oldname = "4f"
    # newname = "5f"
    # rtitle = "5f/4f"

    old_fnames = "/nfs-7/userdata/namin/tupler_babies/merged//FT/v0.01/output/TTWW.root"
    new_fnames = "/nfs-7/userdata/namin/tupler_babies/merged//FT/v0.04/output/TTWW.root"
    oldname = "15BW"
    newname = "100BWnoH"
    rtitle = "100BWnoH/15BW"

    old.Add(old_fnames)
    new.Add(new_fnames)
    # what = "met>>h1(50,0,300)"
    # h2 = gDirectory.Get("h2")
    # old.Draw("what","")

    thedir = "plots/"
    os.system("mkdir -p %s" % thedir)


    # shortname, title, variable, bins, selection
    plots = [

            ("njets", "Njets", "njets", "(11,0,11)", "1"),
            ("nbtags", "Nbtags", "nbtags", "(11,0,11)", "1"),
            ("mtmin", "m_{T}^{min}", "mtmin", "(15,0,300)", "1"),
            ("lep1pt", "p_{T}(lep1)", "lep1_coneCorrPt", "(10,0,300)", "1"),
            ("lep2pt", "p_{T}(lep2)", "lep2_coneCorrPt", "(10,0,200)", "1"),
            ("mll", "m_{ll}", "dilep_p4.M()", "(15,0,300)", "1"),
            ("type", "type (mm,me,em,ee)", "hyp_type-(hyp_type==2 && abs(lep1_id)==13)", "(4,0,4)", "1"),
            # ("lep3pt", "p_{T}(lep3)", "lep3_coneCorrPt", "(10,0,200)", "1"),
            ("met", "MET", "met", "(30,0,800)", "1"),
            ("ht", "H_{T}", "ht", "(30,0,1500)", "1"),
            # ("njets", "Njets", "njets", "(11,0,11)", "1"),
            # ("nbtags", "Nbtags", "nbtags", "(11,0,11)", "1"),
            # ("mtmin", "m_{T}^{min}", "mtmin", "(30,0,300)", "1"),
            # ("lep1pt", "p_{T}(lep1)", "lep1_coneCorrPt", "(20,0,300)", "1"),
            # ("lep2pt", "p_{T}(lep2)", "lep2_coneCorrPt", "(20,0,200)", "1"),
            # ("mll", "m_{ll}", "dilep_p4.M()", "(30,0,300)", "1"),
            # ("type", "type (mm,me,em,ee)", "hyp_type-(hyp_type==2 && abs(lep1_id)==13)", "(4,0,4)", "1"),
            # ("lep3pt", "p_{T}(lep3)", "lep3_coneCorrPt", "(20,0,200)", "1"),
            # ("met", "MET", "met", "(30,0,800)", "1"),
            # ("ht", "H_{T}", "ht", "(30,0,1500)", "1"),

            # ("lep1_MVA", "mva1", "lep1_MVA", "(30,-3.,1.)", "1"),
            # ("lep2_MVA", "mva2", "lep2_MVA", "(30,-3.,1.)", "1"),
            ]


    for shortname, title, variable, bins, selection in plots:

        print "started drawing %s" % title

        N = old.Draw("%s>>h1%s" % (variable, bins),"(35.9*scale1fb) * ((%s && good) )" % selection, "goff") #, 50000)
        print "%i events from old" % N
        h1 = gDirectory.Get("h1")
        N = new.Draw("%s>>h2%s" % (variable, bins),"(35.9*scale1fb) * ((%s && good) )" % selection, "goff") #, 50000)
        print "%i events from new" % N
        h2 = gDirectory.Get("h2")
        # N = old.Draw("%s>>h1%s" % (variable, bins),"(1) * ((%s) )" % selection, "goff") #, 50000)
        # h1 = gDirectory.Get("h1")
        # N = new.Draw("%s>>h2%s" % (variable, bins),"(1) * ((%s) )" % selection, "goff") #, 50000)
        # h2 = gDirectory.Get("h2")

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

        h1.Sumw2()
        h2.Sumw2()

        h1.GetYaxis().SetTitle("Events")
        h1.SetTitle("%s" % (title))

        h1.GetXaxis().SetTitle(title)

        h1.SetMaximum(1.2*max([h1.GetMaximum(),h2.GetMaximum()]))
        h2.SetMaximum(1.2*max([h1.GetMaximum(),h2.GetMaximum()]))
        h1.SetMinimum(0.0)
        h2.SetMinimum(0.0)

        h1.SetLineColor(kBlack)
        h1.SetMarkerColor(kBlack)
        h2.SetLineColor(kRed)
        h2.SetMarkerColor(kRed)

        h1.Draw("histe")
        h2.Draw("histesame")

        leg = TLegend(0.75,0.75,0.95,0.90)
        leg.AddEntry(h1, oldname)
        leg.AddEntry(h2, newname)
        leg.Draw("same")

        pads[1].cd()


        h1vals = np.array([h1.GetBinContent(ib) for ib in range(1,h1.GetNbinsX()+1)])
        h2vals = np.array([h2.GetBinContent(ib) for ib in range(1,h2.GetNbinsX()+1)])
        h1errs = np.array([h1.GetBinError(ib)   for ib in range(1,h1.GetNbinsX()+1)])
        h2errs = np.array([h2.GetBinError(ib)   for ib in range(1,h2.GetNbinsX()+1)])
        goodvals = h1vals != 0.
        h1vals = h1vals[goodvals]
        h2vals = h2vals[goodvals]
        h1errs = h1errs[goodvals]
        h2errs = h2errs[goodvals]
        chi2 = np.sum((h1vals-h2vals)**2/(h1errs**2.+h2errs**2.))
        ndof = len(h1vals)-1
        pchi = r.TMath.Prob(chi2,ndof)
        h1.SetTitle(h1.GetTitle() + (" #color[4]{{#chi^{2} prob} = %.2f}" % pchi))

        r2 = h2.Clone("ratio")
        # for orig,ratio in zip([h2,h3],[r2,r3]):
        for orig,ratio in [[h2,r2]]:
            ratio.Sumw2()
            ratio.Divide(h1)
            ratio.Draw("samepe")

            ratio.SetMarkerColor(orig.GetMarkerColor())
            ratio.SetLineColor(orig.GetLineColor())
            ratio.GetYaxis().SetRangeUser(0.4,1.6)
            ratio.GetYaxis().SetNdivisions(204,False)
            ratio.SetTitle("")
            ratio.GetYaxis().SetTitle(rtitle)        
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
        c1.SaveAs("%s/%s.pdf" % (thedir,shortname))






if __name__ == "__main__":

    make_plots()
    os.system("niceplots plots plots_ttww_May12")

