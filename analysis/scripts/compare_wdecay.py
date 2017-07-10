import ROOT as r
import numpy as np
import sys
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

    ch.SetAlias("promptMC", "lep1_motherID==1 && lep2_motherID==1 && (nleps < 3 || lep3_motherID==1)")


def make_plots():

    fnames_map = {
            "rares"        : [
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/ZZ.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/GGHtoZZto4L.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/WZZ.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/WZG.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/VHtoNonBB.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TZQ.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TWZ.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/WWDPS.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/QQWW.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/WWZ.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/WWW.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/WWG.root",
                ],
            "xg"        : [
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/WGToLNuGext.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTG.root",
                # "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTGdilep.root",
                # "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTGsinglelepbar.root",
                # "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTGsinglelep.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/ZG.root",
                ],
            "ttw"        : [
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTWnlo.root",
                ],
            "ttz"        : [
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTZnlo.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTZLOW.root",
                ],
            "tth"        : [
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTHtoNonBB.root",
                ],
            "tttt"        : [
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTTTnew.root",
                ],
            "mtop"        : [
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTHH.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTTJ.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTTW.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTWH.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTWW.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTWZ.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTZH.root",
                "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTZZ.root",
                # "tt"         : [
                #                 "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSF/output/TTBAR_PH.root",
                #                 ]
                ],


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

    thedir = "plots_wdecay/"
    os.system("mkdir -p %s" % thedir)

    bginfo =   [
            ("rares"          , "Rares"          , r.kMagenta-7  , 0.0) ,
            ("xg" , "X+#gamma" , r.kViolet+2  , 0.0) ,
            ("ttw" , "ttW" , r.kGreen+3  , 0.0) ,
            ("ttz" , "ttZ" , r.kGreen-6  , 0.0) ,
            ("tth" , "ttH" , r.kBlue-5  , 0.0) ,
            ("mtop" , "mtop" , r.kAzure-2  , 0.0) ,
            ("tttt" , "tttt" , r.kRed+1  , 0.0) ,
            ]

    plotinfos = [
            ("nw", "nW", "(6,0.5,6.5)", "good"),
            ("sf", "decayWSF", "(20,0.75,1.25)", "good && promptMC"),
            # ("nleptonicW", "nleptonicW", "(6,0.5,6.5)", "good"),
            ]
    for plotinfo in plotinfos:
        title, variable, bins, selection = plotinfo
        d_hists = {}
        for name in fnames_map.keys():
            chain_map[name].Draw("%s>>h1%s%s" % (variable, name, bins),"(35.9*scale1fb) * ((%s) )" % selection, "goff")
            d_hists[name] = gDirectory.Get("h1"+name)
        print d_hists


        bgnames, titles, colors, systs = map(list,zip(*bginfo))
        subtitle = ""

        opts_str = "--darkColorLines --lumi 35.9 --topYaxisTitle Data/Pred. --type Preliminary --poissonErrorsNoZeros --dataName Data --outOfFrame --systInclStat --systFillStyle 3344 --xAxisLabel {0} --noDivisionLabel --noXaxisUnit  --noOverflow --legendUp -.03 --legendRight -0.05    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable    --percentageInBox    ".format(title)

        bgs = map(lambda x: d_hists.get(x), bgnames)
        print bgs
        print map(lambda x: x.GetEntries(), bgs)
        h_data_empty = bgs[0].Clone("empty")
        h_data_empty.Reset()

        d_newopts = {}
        d_newopts["outputName"] = thedir+title+".pdf"
        d_newopts["poissonErrorsNoZeros"] = False
        d_newopts["noTextBetweenPads"] = False
        # d_newopts["flagLocation"] = "0.6,0.965,0.07"
        dataMCplot(h_data_empty, bgs=bgs, systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)

if __name__ == "__main__":

    make_plots()
    os.system("niceplots plots_wdecay plots_wdecay_Jul1")

