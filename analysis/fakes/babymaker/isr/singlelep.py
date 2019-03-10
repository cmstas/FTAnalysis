import pickle
import os
import ROOT as r
import numpy as np

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery.plotter import plot_2d, plot_stack
from matplottery.utils import Hist2D, Hist1D
# sys.path.insert(0,'../derivation/')
# from plottery.utils import move_in_overflows

counter = 0
def get_hists(varexp, bins="", sel="1",extra_weight="1",_tmp=[]):
    global counter
    counter += 1

    bins = str(bins)
    if "(" not in bins: bins = "({})".format(bins)

    basedir = "/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v3.0_isrbabies/output/year_2018/"

    # weight_mc = "58.8*scale1fb*weight_pu"
    weight_mc = "58.8*scale1fb*weight_pu*{}".format(extra_weight)
    weight_data = "1"

    # ch_wjets_incl = r.TChain("t")
    # ch_wjets_incl.Add("{}/WJets_incl.root".format(basedir))

    ch_wjets_ht = r.TChain("t")
    ch_wjets_ht.Add("{}/WJets_HT*.root".format(basedir))

    ch_tt = r.TChain("t")
    ch_tt.Add("{}/TT*.root".format(basedir))
    # ch_tt.Add("/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/fakes/babymaker/TTBAR_PH.root")

    ch_dy = r.TChain("t")
    ch_dy.Add("{}/DY*.root".format(basedir))

    ch_other = r.TChain("t")
    ch_other.Add("{}/WW.root".format(basedir))
    ch_other.Add("{}/WZ.root".format(basedir))
    ch_other.Add("{}/TTW*.root".format(basedir))
    ch_other.Add("{}/TTZ*.root".format(basedir))

    ch_st = r.TChain("t")
    ch_st.Add("{}/ST*.root".format(basedir))

    ch_data = r.TChain("t")
    ch_data.Add("{}/Data*.root".format(basedir))
    ch_data.Add("{}/ReRecoData*.root".format(basedir))

    # ch_wjets_incl.Draw("{}>>h_wjets_incl_{}{}".format(varexp,counter,bins),"{} * ({})".format(weight_mc,sel))
    # h_wjets_incl = r.gDirectory.Get("h_wjets_incl_{}".format(counter))

    print "Drawing wjets"
    ch_wjets_ht.Draw("{}>>h_wjets_ht_{}{}".format(varexp,counter,bins),"{} * ({})".format(weight_mc,sel))
    h_wjets_ht = r.gDirectory.Get("h_wjets_ht_{}".format(counter))

    print "Drawing tt1"
    ch_tt.Draw("{}>>h_tt1lep_{}{}".format(varexp,counter,bins),"{} * ({} && nleptonicW!=2)".format(weight_mc,sel))
    print "Drawing tt2"
    ch_tt.Draw("{}>>h_tt2lep_{}{}".format(varexp,counter,bins),"{} * ({} && nleptonicW==2)".format(weight_mc,sel))
    h_tt1lep = r.gDirectory.Get("h_tt1lep_{}".format(counter))
    h_tt2lep = r.gDirectory.Get("h_tt2lep_{}".format(counter))

    print "Drawing dy"
    ch_dy.Draw("{}>>h_dy_{}{}".format(varexp,counter,bins),"{} * ({})".format(weight_mc,sel))
    h_dy = r.gDirectory.Get("h_dy_{}".format(counter))

    print "Drawing other"
    ch_other.Draw("{}>>h_other_{}{}".format(varexp,counter,bins),"{} * ({})".format(weight_mc,sel))
    h_other = r.gDirectory.Get("h_other_{}".format(counter))

    ch_st.Draw("{}>>h_st_{}{}".format(varexp,counter,bins),"{} * ({})".format(weight_mc,sel))
    h_st = r.gDirectory.Get("h_st_{}".format(counter))

    print "Drawing data"
    ch_data.Draw("{}>>h_data_{}{}".format(varexp,counter,bins),"{} * ({})".format(weight_data,sel))
    h_data = r.gDirectory.Get("h_data_{}".format(counter))

    print "Done drawing"

    bgs = [
            Hist1D(h_other,label="other",color=(0.97,0.80,0.28)),
            # Hist1D(h_wjets_incl,label="wjets_incl"),
            Hist1D(h_dy,label="DY",color=(0.54,0.10,0.13)),
            Hist1D(h_st,label="Single top", color=(0.85,0.73,0.56)),
            Hist1D(h_wjets_ht,label="W+jets", color=(0.37,0.64,0.27)),
            Hist1D(h_tt2lep,label="ttbar 2lep", color=(0.66,0.85,0.98)),
            Hist1D(h_tt1lep,label="ttbar 1lep", color=(0.23,0.57,0.83)),
            ]
    # for bg in bgs: bg.set_attr("label","{} [N={}]".format(bg.get_attr("label"),int(bg.get_integral())))
    # data = Hist1D(h_data,label="Data [N={}]".format(int(h_data.Integral())))
    data = Hist1D(h_data,label="Data ({})".format(int(h_data.Integral())))
    return data,bgs

    # data = Hist1D(h_wjets_incl,label="wjets_incl")
    # bgs = [
    #     Hist1D(h_wjets_ht,label="wjets_ht"),
    #     ]
    # return data,bgs


# # data,bgs = get_hists("evt_corrMET", (50,50,600), "p4_pt>25 && nbtags==2 && evt_corrMET>50 && ht_SS>300")
# # data,bgs = get_hists("njets-nbtags", (10,-2,8), "p4_pt>25 && nbtags==2 && evt_corrMET>50 && ht_SS>300")
# # data,bgs = get_hists("nisrMatch", (7,-1,6), "p4_pt>25 && nbtags==2 && evt_corrMET>50 && ht_SS>300")
# data,bgs = get_hists("ht_SS", (50,300,800), "p4_pt>25 && nbtags==2 && evt_corrMET>50 && ht_SS>300")
# fname = "test.png"
# xlabel = "ht"
# title = "test"
# plot_stack(
#         bgs=bgs,
#         data=data,
#         filename=fname,
#         xlabel=xlabel,
#         ylabel="Events",
#         title=title,
#         lumi = 58.8,
#         ratio_range=[0.,2.],
#         do_bkg_syst = True,
#         mpl_legend_params = dict(
#             ncol=1,
#             )
#         )
# os.system("ic "+fname)

# d = {}
# d["met_raw"] = get_hists("evt_corrMET", (50,50,600), "p4_pt>25 && nbtags==2 && evt_corrMET>50 && ht_SS>300")
# d["njmnb_raw"] = get_hists("njets-nbtags", (10,-2,8), "p4_pt>25 && nbtags==2 && evt_corrMET>50 && ht_SS>300")
# d["nisrmatch_raw"] = get_hists("nisrMatch", (7,-1,6), "p4_pt>25 && nbtags==2 && evt_corrMET>50 && ht_SS>300")
# d["njets_raw"] = get_hists("njets", (10,0,10), "p4_pt>25 && nbtags==2 && evt_corrMET>50 && ht_SS>300")
# d["ht_raw"] = get_hists("ht_SS", (50,300,800), "p4_pt>25 && nbtags==2 && evt_corrMET>50 && ht_SS>300")
# d["njets_corr"] = get_hists("njets", (10,0,10), "p4_pt>25 && nbtags==2 && evt_corrMET>50 && ht_SS>300",extra_weight="weight_isr")
# d["ht_corr"] = get_hists("ht_SS", (50,300,800), "p4_pt>25 && nbtags==2 && evt_corrMET>50 && ht_SS>300",extra_weight="weight_isr")
# with open("dump.pkl","w") as fh:
#     pickle.dump(d,fh)

for which,title,xlabel in [
        ["met_raw","MET (raw)","MET"],
        ["njmnb_raw","Njets-Nb (raw)","Njets-Nb"],
        ["nisrmatch_raw","NISRgen (raw)","NISRgen"],
        ["njets_raw","Njets (raw)","Njets"],
        ["njets_corr","Njets (reweighted)","Njets"],
        ["ht_raw","HT (raw)","HT"],
        ["ht_corr","HT (reweighted)","HT"],
        ]:
    d = pickle.load(open("dump.pkl"))
    data,bgs = d[which]

    iother = next(i for i,bg in enumerate(bgs) if ("other" in bg.get_attr("label")))
    # scale up since missing half ttz and all of tth
    bgs[iother] *= 2.

    bgs = sorted(bgs, key=lambda bg: bg.get_integral())
    sf = data.get_integral()/sum(bgs).get_integral()
    bgs = [bg*sf for bg in bgs]
    # title = ""
    title += " data/MC={:.2f} (scaled)".format(sf)

    for do_log in [False,True]:
        fname = "plots/{}{}.pdf".format(which,"_log" if do_log else "")
        plot_stack(
                bgs=bgs,
                data=data,
                filename=fname,
                xlabel=xlabel,
                ylabel="Events",
                do_log=do_log,
                title=title,
                lumi = 58.8,
                ratio_range=[0.,2.],
                do_bkg_syst = True,
                mpl_legend_params = dict(
                    ncol=2,
                    )
                )
        os.system("ic "+fname)

