import ROOT as r
import numpy as np


import os
from ROOT import gROOT, gSystem, gDirectory
gSystem.Load("/home/users/namin/2016/ss/master/SSAnalysis/batch_new/compare/../../CORE/CMS3_CORE.so");
gROOT.ProcessLine(".x /home/users/namin/2016/ss/master/SSAnalysis/batch_new/compare/../../draw/dmcStyle.C");


from ROOT import *

def make_plots():

    fnames_map = {
            "fullsim": "/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttH_HToTT_1J_mH-370_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/*.root",
            # "fastsim": "/hadoop/cms/store/group/snt/run2_moriond17_cms4/ttH_HToTT_1J_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUSummer16Fast_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V00-00-02/*.root",
            "fastsim": "/home/users/namin/2017/fourtop/analysis/scripts/skim/higgs/skim_higgs_m370_fastsim.root",
            }

    chain_map = {}
    for key in fnames_map:
        chain_map[key] = r.TChain("Events")
        chain_map[key].Add(fnames_map[key])

    thedir = "plots_higgs/"
    os.system("mkdir -p %s" % thedir)

    plots = [

            ("met", "", "evt_pfmet", "(50,0,500)", "1"),
            ("njets", "", "Sum$(pfjets_p4.pt()>40)", "(15,0,15)", "1"),
            ("ht", "", "Sum$(pfjets_p4.pt()*(pfjets_p4.pt()>40))", "(50,0,1500)", "1"),
            ("leppt", "", "hyp_lt_p4[0].pt()", "(50,0,200)","1"),
            ("nss", "", "Sum$((hyp_lt_id*hyp_ll_id)>0)", "(6,0,6)","1"),
            ("nhyps", "", "Length$(hyp_ll_p4)", "(15,0,15)","1"),
            ("nels", "", "Length$(els_p4)", "(15,0,15)","1"),
            ("nmus", "", "Length$(mus_p4)", "(15,0,15)","1"),
            ("nvtxs", "", "evt_nvtxs", "(50,0,50)","1"),
            # ("nbtags", "Nbtags", "nbtags", "(11,0,11)", "1"),
            # ("mtmin", "m_{T}^{min}", "mtmin", "(10,0,300)", "1"),
            # ("lep1pt", "p_{T}(lep1)", "lep1_coneCorrPt", "(10,0,300)", "1"),
            # ("lep2pt", "p_{T}(lep2)", "lep2_coneCorrPt", "(10,0,200)", "1"),
            # ("mll", "m_{ll}", "dilep_p4.M()", "(10,0,300)", "1"),
            # ("met", "MET", "met", "(15,0,800)", "1"),
            # ("ht", "H_{T}", "ht", "(15,0,1500)", "1"),
            ]
    
    d_weights = {
            "fastsim": 0.0002204*91175/95639,
            "fullsim": 0.00023054,
            }


    for shortname, title, variable, bins, selection in plots:

        # for do_normalize in [True,False]:
        for do_normalize in [False]:

            names = fnames_map.keys()
            print "started drawing %s" % shortname

            extra = "norm" if do_normalize else ""

            d_hists = {}
            for name in names:
                # extrasel = "1"
                # if "fast" in name: extrasel = "sparm_values==370"
                weight = d_weights[name]
                # N = chain_map[name].Draw("{}>>h1{}{}{}".format(variable, name, extra, bins),"{} * ({} && {})".format(weight,selection,extrasel), "goff")
                N = chain_map[name].Draw("{}>>h1{}{}{}".format(variable, name, extra, bins),"{:.6f} * ({})".format(weight,selection), "goff")
                d_hists[name] = gDirectory.Get("h1"+name+extra)
                if not title: title = d_hists[name].GetTitle()

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
            h1 = d_hists.get("fullsim")
            h1.GetYaxis().SetTitle("Events")
            if do_normalize:
                h1.SetTitle("%s [norm. to fullsim]" % (title))
            else:
                h1.SetTitle("%s" % (title))
            h1.GetXaxis().SetTitle("")

            maxy = max(map(lambda x: x.GetMaximum(), d_hists.values()))

            h1.SetMaximum(1.2*maxy)
            h1.SetMinimum(0.0)

            h1.SetLineColor(r.kRed-4)
            h1.SetMarkerColor(r.kRed-4)

            rest = ["fastsim"]
            colors = [r.kAzure-4]

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
            leg.AddEntry(h1, "fullsim"+get_integral_string(h1))
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
                ratio.GetYaxis().SetTitle("*/fullsim")        
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
    os.system("niceplots plots_higgs plots_higgs_Jul8")

