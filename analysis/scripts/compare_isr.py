import ROOT as r
import numpy as np
import sys
sys.path.insert(0,'../../../')
from common.Software.dataMCplotMaker.dataMCplotMaker import dataMCplot

r.gROOT.SetBatch(1)

import os
from ROOT import gROOT, gSystem, gDirectory

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
    ch.SetAlias("class3", "hyp_class == 3");
    ch.SetAlias("filters", "(fired_trigger) && passes_met_filters");
    ch.SetAlias("baseline", "njets>=2 && (met>=50) && (nbtags>=2) && (ht>300) && lep1_ptgood && lep2_ptgood");
    ch.SetAlias("good", "baseline && filters && class3");

    ch.SetAlias("nleps", "2+lep3_passes_id+(lep4_passes_id && lep4_p4.pt()>20)")
    ch.SetAlias("sr", "(nleps==2)*((nbtags == 2)*(2*(njets<=5)+3*(njets==6)+4*(njets==7)+5*(njets>=8)) + (nbtags == 3)*(6*(njets==5 || njets==6)+7*(njets>=7)) + (nbtags >= 4)*(8*(njets>=5)))+(nleps>2)*((nbtags == 2)*(9*(njets>=5)) + (nbtags >= 3)*(10*(njets>=4)))")


    # sr = "signalRegion2016(njets,nbtags,met,ht,mtmin_new,id1,id2,pt1,pt2)"

def make_plots():

    fnames_map = {
            "rares"        : [
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/ZZ.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/GGHtoZZto4L.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/WZZ.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/WZG.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/VHtoNonBB.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/TZQ.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/TWZ.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/WWDPS.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/QQWW.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/WWZ.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/WWW.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/WWG.root",
                            ],
            "xg"        : [
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/WGToLNuGext.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/TTG.root",
                            # "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/TTGdilep.root",
                            # "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/TTGsinglelepbar.root",
                            # "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/TTGsinglelep.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/ZG.root",
                            ],
            "ttw"        : [
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/TTWnlo.root",
                            ],
            "ttz"        : [
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/TTZnlo.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/TTZLOW.root",
                            ],
            "tth"        : [
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/TTHtoNonBB.root",
                            ],
            "tt"         : [
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/TTBAR_PH.root",
                            ]

            }

    chain_map = {}
    for key in fnames_map:
        chain_map[key] = r.TChain("t")
        make_aliases(chain_map[key])
        if type(fnames_map[key]) == type([]):
            for fname in fnames_map[key]:
                chain_map[key].Add(fname)
                print fname
        else:
            chain_map[key].Add(fnames_map[key])

    thedir = "plots_isr/"
    os.system("mkdir -p %s" % thedir)

    plotinfo = ("", "njets-2", "(10,0.5,10.5)", "((hyp_class==4||hyp_class==6) && Sum$(btags.pt()>40)==2 && abs(lep1_id)!=abs(lep2_id))")
    title, variable, bins, selection = plotinfo
    d_hists = {}
    for name in fnames_map.keys():
        chain_map[name].Draw("%s>>h1%s%s" % (variable, name, bins),"(35.9*scale1fb) * ((%s) )" % selection, "goff")
        d_hists[name] = gDirectory.Get("h1"+name)
    print d_hists

    bginfo =   [
            ("rares"          , "Rares"          , r.kMagenta-7  , 0.0) ,
            ("xg" , "X+#gamma" , r.kViolet+2  , 0.0) ,
            ("ttw" , "ttW" , r.kGreen+3  , 0.0) ,
            ("ttz" , "ttZ" , r.kGreen-6  , 0.0) ,
            ("tth" , "ttH" , r.kBlue-5  , 0.0) ,
            ("tt" , "ttbar" , r.kMagenta  , 0.0) ,
            ]

    bgnames, titles, colors, systs = map(list,zip(*bginfo))
    subtitle = ""

    opts_str = "--darkColorLines --lumi 35.9 --topYaxisTitle Data/Pred. --type Preliminary --poissonErrorsNoZeros --dataName Data --outOfFrame --systInclStat --systFillStyle 3344 --xAxisLabel njets-2 --noDivisionLabel --noXaxisUnit --isLinear --noOverflow --legendUp -.03 --legendRight -0.05    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable    --percentageInBox   --legendCounts "

    bgs = map(lambda x: d_hists.get(x), bgnames)
    print bgs
    print map(lambda x: x.GetEntries(), bgs)
    h_data_empty = bgs[0].Clone("empty")
    h_data_empty.Reset()

    d_newopts = {}
    d_newopts["outputName"] = thedir+"isr.pdf"
    d_newopts["poissonErrorsNoZeros"] = False
    d_newopts["noTextBetweenPads"] = False
    # d_newopts["flagLocation"] = "0.6,0.965,0.07"
    dataMCplot(h_data_empty, bgs=bgs, systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)

if __name__ == "__main__":

    make_plots()
    os.system("niceplots plots_isr plots_isr_Jun26")

