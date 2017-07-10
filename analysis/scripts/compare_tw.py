import ROOT as r
import numpy as np
import sys
import numpy as np
# sys.path.insert(0,'../../../')
from common.Software.dataMCplotMaker.dataMCplotMaker import dataMCplot

r.gROOT.SetBatch(1)

r.gROOT.ProcessLine(".L ../misc/common_utils.h");

import os
from ROOT import gROOT, gSystem, gDirectory

from ROOT import *

def get_isrweight(nj):
    if (nj == 0): return 1.00;
    if (nj == 1): return 0.86;
    if (nj == 2): return 0.79;
    if (nj == 3): return 0.75;
    if (nj == 4): return 0.77;
    if (nj == 5): return 0.74;
    if (nj == 6): return 0.73;
    if (nj == 7): return 0.56;
    return 1.

def rw_hist(h1, sf=1.):

    # h1.Scale(sf)

    for ibin in range(1,h1.GetNbinsX()+1):
        val = h1.GetBinContent(ibin)
        err = h1.GetBinError(ibin)
        isrweight = get_isrweight(ibin-1)
        newval = val*isrweight
        newerr = (err*err + ((0.5)*(newval-val))**2)**0.5
        h1.SetBinContent( ibin,newval )
        h1.SetBinError( ibin,newerr)

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

    ch.SetAlias("lepsf", "eventScaleFactor(lep1_id,lep2_id,lep1_p4.pt(),lep2_p4.pt(),lep1_p4.eta(),lep2_p4.eta(),ht)")
    ch.SetAlias("pusf", "getTruePUw_Moriond(trueNumInt[0])")


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
                            ],
            "st"         : [
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/ST1.root",
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/ST2.root",
                            ],
            "data"         : [
                            # "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_data/output/DataMuonEG*root",
                            "/home/users/namin/2017/fourtop/analysis/scripts/skim/unique.root",
                            ],
            "dy"         : [
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_mcosv2/output/DY_*.root",
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

    thedir = "plots_tw/"
    os.system("mkdir -p %s" % thedir)

    # plotinfo = ("", "njets-1", "(10,0.5,10.5)", "((hyp_class==4) && Sum$(btags.pt()>40)==1 && abs(lep1_id)!=abs(lep2_id))")
    plotinfo = ("", "njets-1", "(10,-0.5,9.5)", "((hyp_class==4) && nbtags==1 && abs(lep1_id)!=abs(lep2_id))")
    title, variable, bins, selection = plotinfo
    d_hists = {}
    for name in fnames_map.keys():
        if name == "data":
            # chain_map[name].BuildIndex("run","event")
            blah = chain_map[name].Draw("%s>>h1%s%s" % (variable, name, bins),"(filters && (%s) )" % selection, "goff")
            # print "blah:",blah,"(filters && (%s) )" % selection
        else:
            # chain_map[name].Draw("%s>>h1%s%s" % (variable, name, bins),"(35.9*scale1fb) * ((%s) )" % selection, "goff")
            chain_map[name].Draw("%s>>h1%s%s" % (variable, name, bins),"(35.9*scale1fb*eventScaleFactor(lep1_id,lep2_id,lep1_p4.pt(),lep2_p4.pt(),lep1_p4.eta(),lep2_p4.eta(),ht)*getTruePUw_Moriond(trueNumInt[0])*weight_btagsf) * ((%s) )" % selection, "goff")
        d_hists[name] = gDirectory.Get("h1"+name)
        print name,d_hists[name].GetEntries(),d_hists[name].Integral()
    print d_hists

    bginfo =   [
            ("rares"          , "Rares"          , r.kMagenta-7  , 0.0) ,
            ("xg" , "X+#gamma" , r.kViolet+2  , 0.0) ,
            ("ttw" , "ttW" , r.kGreen+3  , 0.0) ,
            ("ttz" , "ttZ" , r.kGreen-6  , 0.0) ,
            ("tth" , "ttH" , r.kBlue-5  , 0.0) ,
            ("tt" , "ttbar" , r.kOrange+1  , 0.0) ,
            ("st" , "singletop" , r.kAzure+2  , 0.0) ,
            ("dy" , "DY" , r.kGray+2  , 0.0) ,
            ]

    bgnames, titles, colors, systs = map(list,zip(*bginfo))
    subtitle = ""

    # opts_str = "--darkColorLines --lumi 35.9 --topYaxisTitle Data/Pred. --type Preliminary --poissonErrorsNoZeros --dataName Data --outOfFrame --systInclStat --systFillStyle 3344 --xAxisLabel njets-1 --noDivisionLabel --noXaxisUnit --isLinear --noOverflow --legendUp -.03 --legendRight -0.05    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable    --percentageInBox   --legendCounts "
    opts_str = "--darkColorLines --lumi 35.9 --topYaxisTitle Data/Pred. --type Preliminary --poissonErrorsNoZeros --dataName Data --outOfFrame --systInclStat --systFillStyle 3344 --xAxisLabel njets-1 --noDivisionLabel --noXaxisUnit  --noOverflow --legendUp -.03 --legendRight -0.05    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable    --percentageInBox   --legendCounts "

    bgs = map(lambda x: d_hists.get(x), bgnames)
    # print bgs
    print map(lambda x: x.GetEntries(), bgs)
    h_data_empty = bgs[0].Clone("empty")
    h_data_empty.Reset()
    h_data = d_hists["data"]


    for bg in bgs:
        rw_hist(bg)
    # print h_data.GetEntries()
    # print h_data.Integral()

    tot_sum_bgs = sum([bg.Integral() for bg in bgs])
    sf = h_data.Integral() / tot_sum_bgs
    tot_data = np.array(list(h_data)[1:-1])
    for bg in bgs:
        bg.Scale(sf)

    d_newopts = {}
    d_newopts["outputName"] = thedir+"tw.pdf"
    # d_newopts["poissonErrorsNoZeros"] = False
    # d_newopts["noTextBetweenPads"] = False
    # d_newopts["flagLocation"] = "0.6,0.965,0.07"
    # dataMCplot(h_data_empty, bgs=bgs, systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)
    dataMCplot(h_data, bgs=bgs, systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)

if __name__ == "__main__":

    make_plots()
    os.system("niceplots plots_tw plots_tw_Jul6")

