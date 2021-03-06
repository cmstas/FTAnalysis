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
            "TTWW": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_raresxg/output/TTWW.root",
            "TTTW": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_raresxg/output/TTTW.root",
            "TTWZ": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_raresxg/output/TTWZ.root",
            "TTTJ": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_raresxg/output/TTTJ.root",
            "TTWH": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_raresxg/output/TTWH.root",
            "TTHH": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_raresxg/output/TTHH.root",
            "TTZH": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_raresxg/output/TTZH.root",
            "TTZZ": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_raresxg/output/TTZZ.root",
            }

    chain_map = {}
    for key in fnames_map:
        chain_map[key] = r.TChain("t")
        make_aliases(chain_map[key])
        chain_map[key].Add(fnames_map[key])

    thedir = "plots_rares/"
    os.system("mkdir -p %s" % thedir)

    plotinfo = ("sr", "", "sr-2", "(8,0.5,8.5)", "(sr>2)")
    shortname, title, variable, bins, selection = plotinfo
    d_hists = {}
    for name in fnames_map.keys():
        chain_map[name].Draw("%s>>h1%s%s" % (variable, name, bins),"(35.9*scale1fb) * ((%s && good) )" % selection, "goff")
        d_hists[name] = gDirectory.Get("h1"+name)

    # from https://github.com/sgnoohc/Ditto/blob/master/python/makeplot.py#L193-L200
    mycolors = []
    mycolors.append(r.TColor(7000 ,   0/255. ,   0/255. ,   0/255.))
    mycolors.append(r.TColor(7001 , 213/255. ,  94/255. ,   0/255.)) #r
    mycolors.append(r.TColor(7002 , 230/255. , 159/255. ,   0/255.)) #o
    mycolors.append(r.TColor(7003 , 240/255. , 228/255. ,  66/255.)) #y
    mycolors.append(r.TColor(7004 ,   0/255. , 158/255. , 115/255.)) #g
    mycolors.append(r.TColor(7005 ,   0/255. , 114/255. , 178/255.)) #b
    mycolors.append(r.TColor(7006 ,  86/255. , 180/255. , 233/255.)) #k
    mycolors.append(r.TColor(7007 , 204/255. , 121/255. , 167/255.)) #p
    mycolors.append(r.TColor(7011 , 110/255. ,  54/255. ,   0/255.)) #alt r
    mycolors.append(r.TColor(7017 , 140/255. ,  93/255. , 119/255.)) #alt p

    bginfo =   [
            # ("TTWW" , "TTWW" , r.kMagenta-7 , 0.0),
            # ("TTTW" , "TTTW" , r.kGray+2    , 0.0),
            # ("TTWZ" , "TTWZ" , r.kViolet+2  , 0.0),
            # ("TTTJ" , "TTTJ" , r.kAzure-4   , 0.0),
            # ("TTWH" , "TTWH" , r.kGreen-6   , 0.0),
            # ("TTHH" , "TTHH" , 18           , 0.0),
            # ("TTZH" , "TTZH" , r.kBlue-5    , 0.0),
            # ("TTZZ" , "TTZZ" , r.kGreen+3   , 0.0),
            # ("TTTW" , "TTTW" , 7006 , 0.0),
            # ("TTWW" , "TTWW" , 7001 , 0.0),
            # ("TTWZ" , "TTWZ" , 7003 , 0.0),
            # ("TTWH" , "TTWH" , 7005 , 0.0),
            # ("TTHH" , "TTHH" , 7002 , 0.0),
            # ("TTZH" , "TTZH" , 7007 , 0.0),
            # ("TTTJ" , "TTTJ" , 7004 , 0.0),
            # ("TTZZ" , "TTZZ" , 7011 , 0.0),
            ("TTTW" , "TTTW" , 7006 , 0.0),
            ("TTWW" , "TTWW" , 7005 , 0.0),
            ("TTWZ" , "TTWZ" , 7004 , 0.0),
            ("TTWH" , "TTWH" , 7003 , 0.0),
            ("TTHH" , "TTHH" , 7002 , 0.0),
            ("TTZH" , "TTZH" , 7001 , 0.0),
            ("TTTJ" , "TTTJ" , 7007 , 0.0),
            ("TTZZ" , "TTZZ" , 7017 , 0.0),
               ]


    bgnames, titles, colors, systs = map(list,zip(*bginfo))
    subtitle = ""

    opts_str = "--darkColorLines --lumi 35.9 --topYaxisTitle Data/Pred. --type Preliminary --poissonErrorsNoZeros --dataName Data --outOfFrame --systInclStat --systFillStyle 3344 --xAxisLabel SR --noDivisionLabel --noXaxisUnit --isLinear --noOverflow --legendUp -.03 --legendRight -0.05    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable    --percentageInBox --xAxisBinLabels SR1,SR2,SR3,SR4,SR5,SR6,SR7,SR8 --legendCounts "

    bgs = map(lambda x: d_hists.get(x), bgnames)
    h_data_empty = bgs[0].Clone("empty")
    h_data_empty.Reset()

    d_newopts = {}
    d_newopts["outputName"] = thedir+"sr.pdf"
    d_newopts["poissonErrorsNoZeros"] = False
    d_newopts["noTextBetweenPads"] = False
    # d_newopts["flagLocation"] = "0.6,0.965,0.07"
    # d_newopts["flagLocation"] = "0.21,0.96,0.08"
    dataMCplot(h_data_empty, bgs=bgs, systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)

if __name__ == "__main__":

    make_plots()
    os.system("niceplots plots_rares plots_ttttrares_Jun16")

