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


    ch.SetAlias("promptMC", "lep1_motherID==1 && lep2_motherID==1 && (nleps < 3 || lep3_motherID==1)")

    # sr = "signalRegion2016(njets,nbtags,met,ht,mtmin_new,id1,id2,pt1,pt2)"

def do_cutflow():

    fnames_map = {
            "tttt"        : [
                            "/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSFv2/output/TTTTnew.root",
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

        selections = [
        ("SS","hyp_class==3 && promptMC"),
        ("Trigger","fired_trigger && passes_met_filters"),
        ("pT 25,20","lep1_coneCorrPt>25 && lep2_coneCorrPt>20"),
        ("Njets>=2","njets>=2"),
        ("Nb>=2","nbtags>=2"),
        ("HT>300","ht>300"),
        ("MET>50","met>50"),
        ]

    # weight = "(35.9*scale1fb*decayWSF*eventScaleFactor(lep1_id,lep2_id,lep1_p4.pt(),lep2_p4.pt(),lep1_p4.eta(),lep2_p4.eta(),ht)*getTruePUw_Moriond(trueNumInt[0])*weight_btagsf)"
    weight = "(35.9*scale1fb*decayWSF*eventScaleFactor(lep1_id,lep2_id,lep1_p4.pt(),lep2_p4.pt(),lep1_p4.eta(),lep2_p4.eta(),ht)*getTruePUw_Moriond(trueNumInt[0])*weight_btagsf)"

    norm = 35.9*9.2/(1.5/1.6)
    totsel = "1 "
    for isel,(seltitle,sel) in enumerate(selections):
        totsel += "&& ({})".format(sel)
        # print sel, totsel
    # title, variable, bins, selection = plotinfo
        name = "tttt"
        chain_map[name].Draw("1>>h1{}".format(name)," {} * ({})".format(weight,totsel), "goff")
        h1 = gDirectory.Get("h1"+name)
        print "+{:10s} : {:.1f}%".format(seltitle,100.0*h1.Integral()/norm)
    # d_hists = {}
    # print d_hists


if __name__ == "__main__":

    do_cutflow()

