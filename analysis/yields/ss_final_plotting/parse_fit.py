import os
import numpy as np
import uproot
import pickle
import math
import re

from analysis.utils.plotting_utils import write_table

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery import Hist1D, plot_stack
from matplottery.utils import binomial_obs_z
import matplotlib
# ../../../common/matplottery/matplottery/utils.py
import pandas as pd

# SIGH
matplotlib.rcParams['xtick.direction'] = 'in'
matplotlib.rcParams['ytick.direction'] = 'in'

np.set_printoptions(linewidth=180)

ch_name_map = {
        "ch1": "srhh",
        "ch2": "srhl",
        "ch3": "srll",
        "ch4": "srml",
        "ch5": "srlm",
        }

name_nbin_map = {
        "srhh": 62,
        "srhl": 43,
        "srll": 8,
        "srml": 44,
        "srlm": 11,
        }

# f = uproot.open("../fitDiagnosticsss.root") # all 5 regions
f = uproot.open("fitDiagnosticsv2.root") # all 5 regions
# f = uproot.open("ssforpostfitUncorrTTVWZFakes.root") # all 5 regions
d_hists = {}
d_sfs = {} # key is process and value is postfit/prefit SF # NOTE, b-only fit
# for typ in ["shapes_fit_b","shapes_prefit","shapes_fit_s"]:
for typ in ["shapes_fit_b","shapes_prefit"]:
# for typ in ["shapes_fit_b"]:
    shapes = f[typ]
    d_hists[typ] = {}
    for ch in shapes:
        ch = ch.rsplit(";")[0]
        # if not ch.startswith("ch"): continue
        hists = shapes[ch]
        # if type(hists) is not uproot.rootio.ROOTDirectory: continue
        if "ROOTDirectory" not in str(type(hists)): continue
        name = ch_name_map.get(ch,ch)
        d_hists[typ][name] = {}
        print typ,ch,hists,type(hists), hists.keys()
        for hname in hists:
            hname = hname.rsplit(";")[0]
            if "covar" in hname: continue
            # print name, hname, type(hname)
            h = Hist1D(hists[hname],label=hname)
            if name in name_nbin_map: h.restrict_bins(range(name_nbin_map[name]))
            d_hists[typ][name][hname] = h
            # d_hists[typ][name][hname] = 1
        data = d_hists[typ][name]["data"]
        bg = d_hists[typ][name]["total_background"]
        d_hists[typ][name]["pulls"] = binomial_obs_z(data.counts,bg.counts,bg.errors)
for proc in d_hists["shapes_prefit"].values()[0].keys():
    if proc in ["pulls","data"]: continue
    ipostfit = 0.
    iprefit = 0.
    for region in d_hists["shapes_fit_b"].keys():
        postfit = d_hists["shapes_fit_b"][region].get(proc,None)
        prefit = d_hists["shapes_prefit"][region].get(proc,None)
        if postfit is not None: ipostfit += postfit.get_integral()
        if prefit is not None: iprefit += prefit.get_integral()
    d_sfs[proc] = ipostfit/iprefit

d_label_colors = {}
d_label_colors["ss"] = {
        "fakes" : (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
        "ttw"   : (r"$\mathregular{t\bar{t}W}$", [0.0, 0.4, 0.0]),
        "wz"  : (r"WZ" , [1.0,0.8,0.0]),
        "xg"    : (r"$\mathregular{X\gamma}$" , [0.4, 0.0, 0.8]),
        "ww"  : (r"WW" , [1.0,0.6,0.0]),
        "flips" : (r"Charge misid.", [0.4, 0.4, 0.4]),
        "rares" : (r"Rare", [1.0, 0.4, 1.0]),
        "tth"   : (r"$\mathregular{t\bar{t}H}}$", [0.4, 0.4, 0.6]),
        "ttz"   : (r"$\mathregular{t\bar{t}Z}$", [0.4, 0.8, 0.4]),
        }

def cull_bins(h, first=None,last=None):
    h = Hist1D(h)
    h._counts = h._counts[first:last]
    h._errors = h._errors[first:last]
    if h._errors_up is not None:
        h._errors_up = h._errors_up[first:last]
        h._errors_down = h._errors_down[first:last]
    if last:
        last += 1
    if first:
        first -= 1
    h._edges = h._edges[first:last]
    return h

def make_plots(
        regions=["srhh","srhl","srll","srmlonz","srmloffz","srlm"],
        outputdir="plots/",
        lumi="137",
        typ="shapes_fit_b",
        # procs=['fakes', 'flips', 'wz', 'ttz', 'ttw', 'tth', 'rares', 'ww', 'xg'],
        procs=['fakes', 'ttw', 'wz', 'xg', 'ww', 'flips', 'rares', 'tth', 'ttz'][::-1],
        year="run2",
        ):


    if year == 2016:
        lumi = "35.9"
    if year == 2017:
        lumi = "41.5"
    if year == 2018:
        lumi = "59.7"
    # region = "srhl"
    # outputdir = "plots/"
    # lumi = "61.3"

    for region in regions:
        rawregion = region[:]

        onz = False
        if region.startswith("srml"):
            if region.endswith("onz"): onz = True
            region = "srml"

        hists = d_hists[typ][region]
        data = hists["data"]
        totbg = hists["total_background"]
        procs_ = procs[:]
        if region == "srml": procs_ = list(set(procs_)-set(["ww","flips"]))
        bgs = [
                Hist1D(
                    hists[k],
                    label=d_label_colors["ss"][k][0],
                    color=d_label_colors["ss"][k][1],
                    ) for k in procs_
                ]
        # use the specified order unless it's ML
        if region == "srml":
            bgs = sorted(bgs, key=lambda bg: bg.get_integral())

        # FIXME
        sig = None
        signame = "fs_t1tttt_m1600_m600"
        basedir = "v3.31_ss_Jun26_v1/"
        sigstr = r"$t\bar{t}t\bar{t}$"
        sigtag = signame.split("_m")[0].replace("fs_","")
        sigcolor = [0.75,0.15,0.22]
        modelname = {
                "t1tttt": "T1tttt",
                "t6ttww": "T6ttWW",
                "t5qqqqvv": "T5qqqqVV",
                "t5qqqqvv_dm20": "T5qqqqVV, $\Delta$m=20",
                "rpv_t1qqqql": "T1qqqqL",
                }.get(sigtag,sigtag)
        # fs_t1tttt_m1600_m600_histos_srhh_2016.root
        massstr =  ",".join(re.findall("_m([0-9]*)",signame))
        sigstr = r"{} ({}) $\times 10$".format(modelname,massstr)
        sig = sum([Hist1D(uproot.open("{}/{}_histos_{}_{}.root".format(basedir,signame,region.lower(),y))["sr"]) for y in [2016,2017,2018]])
        sig.set_attr("label", sigstr)
        sig.set_attr("color", sigcolor)
        sig *= 10.
        print sig
        print sigcolor
        print sig._counts
        sig._edges -= 0.5
        print sig._edges
        print bgs[0]._edges

        # which = "prefit" if "prefit" in typ else "postfit"
        which = "prefit" if "prefit" in typ else "postfit"
        title = "{} ({})".format(region.upper().replace("SR",""),which)
        title = title.replace(" (prefit)","")


        if region == "srll" and lumi == "137": lumi_ = "132"

        data.set_attr("label", "Data")

        ax_main_callback = None
        ax_ratio_callback = None
        mpl_legend_params = {}

        ratio_range = [0.,3.]

        mpl_legend_params["fontsize"] = 10
        mpl_legend_params["framealpha"] = 0.0
        mpl_legend_params["ncol"] = 3
        mpl_legend_params["labelspacing"] = 0.10
        mpl_legend_params["columnspacing"] = 1.05
        mpl_legend_params["bbox_to_anchor"] = (1.0,0.97)
        mpl_legend_params["loc"] = "upper right"

        if region == "srml":
            mpl_legend_params["columnspacing"] = 1.15
            mpl_legend_params["bbox_to_anchor"] = (0.97,0.95)

        xticks = range(1,70)

        mpl_xtick_params = dict()
        if region in ["srhl","srml"]:
            mpl_xtick_params = dict(fontsize=8)
        if region in ["srll","srlm"]:
            mpl_xtick_params = dict(rotation=0, fontsize=12)

        yticks_ratio = [0,1,2]

        def ax_main_callback(ax,title=title):
            ax.set_ylim([0.05,ax.get_ylim()[1]*50.0])
            ax.set_yscale("log", nonposy='clip')
            # ax.text(0.5,0.97,title,fontsize=13,horizontalalignment="center",verticalalignment="top", transform=ax.transAxes)
            xshift = 0.
            if region == "srml":
                xshift = 0.035
                if onz:
                    title = "on-Z ML"
                else:
                    title = "off-Z ML"
            ax.text(0.05+xshift,0.85,title,fontsize=14,horizontalalignment="center",verticalalignment="top", transform=ax.transAxes)
        title = ""

        # # specific plots tuned by hand
        # if True:
        #     if region == "srll":
        #         def ax_main_callback(ax):
        #             ax.set_ylim([0.05,ax.get_ylim()[1]*5.5])
        #             ax.set_yscale("log", nonposy='clip'),
        #     if region == "srml":
        #         # mpl_legend_params["fontsize"] = 8
        #         # mpl_legend_params["framealpha"] = 0.4
        #         mpl_legend_params["ncol"] = 3
        #         # mpl_legend_params["labelspacing"] = 0.12
        #         def ax_main_callback(ax):
        #             ax.set_ylim([0.05,ax.get_ylim()[1]*7.5])
        #             ax.set_yscale("log", nonposy='clip'),
        #     if region == "srlm":
        #         def ax_main_callback(ax):
        #             ax.set_ylim([0.05,ax.get_ylim()[1]*4.0])
        #             ax.set_yscale("log", nonposy='clip'),

        if region == "srml":

            if onz:
                bgs = [cull_bins(bg, first=-23) for bg in bgs]
                data = cull_bins(data, first=-23)
                sig = cull_bins(sig, first=-23)
                totbg = cull_bins(totbg, first=-23)
            else:
                for bg in bgs: cull_bins(bg, last=21)
                bgs = [cull_bins(bg, last=21) for bg in bgs]
                data = cull_bins(data, last=21)
                sig = cull_bins(sig, last=21)
                totbg = cull_bins(totbg, last=21)

            print len(sig._counts)
            print len(sig._errors)
            print len(sig._edges)


        if year in [2016,2017,2018]:
            ystr = "y{}".format(year)
        else:
            ystr = "run2"
        fname = "{}/{}_{}_TOTAL_{}.pdf".format(outputdir,ystr,rawregion.upper(),which)
        plot_stack(bgs=bgs, data=data, title=title.replace("fit","-fit"), xlabel="SR", ylabel="Events", filename=fname,
                   # cms_type = "Preliminary",
                   cms_type = "",
                   lumi = lumi,
                   ratio_range=ratio_range,
                   xticks=xticks,
                   yticks_ratio=yticks_ratio,
                   mpl_xtick_params=mpl_xtick_params,
                   mpl_ratio_params={
                       "label":"Data / Pred.",
                       },
                   ax_main_callback=ax_main_callback,
                   mpl_legend_params=mpl_legend_params,
                   ax_ratio_callback=ax_ratio_callback,
                   do_bkg_syst=True,
                   override_bkg_syst=totbg.errors,
                   sigs=[sig],
                   mpl_sig_params={
                       # "hist":False,
                       },
                   percentbox=False,
                   data_top_of_legend=True,
                   )
        print "Made {}".format(fname)
        os.system("ic {}".format(fname))
        table_info = write_table(data,bgs,outname=fname.replace(".pdf",".txt"))

def parse_yields(d_in, extra=""):
    d_yields = {}
    for key in d_in:
        if key in ["pulls"]: continue
        h1 = d_in[key]
        if extra == "onz":
            h1 = cull_bins(h1, first=-23)
        if extra == "offz":
            h1 = cull_bins(h1, last=21)
        vals,errs = h1.counts, h1.errors
        d_yields[key] = {}
        d_yields[key]["central"] = vals
        d_yields[key]["error"] = errs
    return d_yields

def print_table(d_yields, regions="srcr",precision=2,blinded=False):
    nbins = len(d_yields["ttz"]["central"])

    if regions in ["srml"]:
        colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","$\\WZ$","X+$\\gamma$","Rares","Nonprompt lep.","SM expected","Data"]
        procs = ["ttw","ttz","tth","wz","xg","rares","fakes","total_background","data"]
    else:
        colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","$\\WZ$","WW","X+$\\gamma$","Rares","Charge misid.","Nonprompt lep.","SM expected","Data"]
        procs = ["ttw","ttz","tth","wz","ww","xg","rares","flips","fakes","total_background","data"]
    srnames = ["SR{}".format(i) for i in range(1,100)]

    buff = ""
    for ibin in range(nbins+1):
        if ibin == 0:
            buff += "&".join(map(lambda x: "{0:12s}".format(x),colnames))
            buff += " "
            buff += "\\\\ \n"
            buff += "\\hline \\hline \n"
            buff += "\n"
        # if ibin == nbins:
        #     continue
        if ibin == nbins:
            tojoin = ["Total"]
        else:
            tojoin = [srnames[ibin]]
        for proc in procs:

            if ibin == nbins:
                cent = max(sum(d_yields[proc]["central"]), 0.)
                err = sum([x**2. for x in d_yields[proc]["error"]])**0.5
            else:
                cent = max(d_yields[proc]["central"][ibin],0.)
                err = d_yields[proc]["error"][ibin]


            if "data" in proc:
                if not blinded:
                    tojoin.append("{0:.0f}".format(cent))
                else:
                    tojoin.append("-".format(cent))
            else:
                if cent < 0.01:
                    tojoin.append("-")
                else:
                    tojoin.append("{}$\\pm${}".format(*GetRoundedValues(cent,min(err,cent))))
        buff += " & ".join(tojoin) + " "
        if ibin == nbins-1:
            buff += "\\hline "
        buff += "\\\\ \n"
    buff += "\n"
    return buff

# from https://github.com/cmstas/MT2Analysis/blob/master/scripts/makeSRYieldsTable.py#L119-L131
def GetRoundedValues(rate, unc, unc_sig_fig=2, prec_cap=-2):
    if unc == 0:
        prec = 0
    else:
        prec = int(math.floor(math.log10(unc))) - (unc_sig_fig - 1)
    prec = max(prec, prec_cap)
    ndec = max(0,-prec)
    rnd = lambda x: "{{0:.{0}f}}".format(ndec).format(int(round(float(x)/10**prec)) * 10**prec)

    return rnd(rate), rnd(unc)

def print_slim_table(d):
    buff = ""
    v_hh_obs = d["srhh"]["data"].counts
    v_hh_exp = d["srhh"]["total_background"].counts
    e_hh_exp = d["srhh"]["total_background"].errors
    v_hl_obs = d["srhl"]["data"].counts
    v_hl_exp = d["srhl"]["total_background"].counts
    e_hl_exp = d["srhl"]["total_background"].errors
    v_ll_obs = d["srll"]["data"].counts
    v_ll_exp = d["srll"]["total_background"].counts
    e_ll_exp = d["srll"]["total_background"].errors
    v_ml_obs = d["srml"]["data"].counts
    v_ml_exp = d["srml"]["total_background"].counts
    e_ml_exp = d["srml"]["total_background"].errors
    v_lm_obs = d["srlm"]["data"].counts
    v_lm_exp = d["srlm"]["total_background"].counts
    e_lm_exp = d["srlm"]["total_background"].errors
    for i in range(len(v_hh_obs)):
        srs = "SR{}".format(i+1)
        hh_exp = ""
        hh_obs = ""
        hl_exp = ""
        hl_obs = ""
        ll_exp = ""
        ll_obs = ""
        ml_exp = ""
        ml_obs = ""
        lm_exp = ""
        lm_obs = ""
        if i < len(v_hh_obs): hh_obs, hh_exp = str(int(v_hh_obs[i])), r"{}$\pm${}".format(*GetRoundedValues(v_hh_exp[i],min(e_hh_exp[i],v_hh_exp[i])))
        if i < len(v_hl_obs): hl_obs, hl_exp = str(int(v_hl_obs[i])), r"{}$\pm${}".format(*GetRoundedValues(v_hl_exp[i],min(e_hl_exp[i],v_hl_exp[i])))
        if i < len(v_ll_obs): ll_obs, ll_exp = str(int(v_ll_obs[i])), r"{}$\pm${}".format(*GetRoundedValues(v_ll_exp[i],min(e_ll_exp[i],v_ll_exp[i])))
        if i < len(v_ml_obs): ml_obs, ml_exp = str(int(v_ml_obs[i])), r"{}$\pm${}".format(*GetRoundedValues(v_ml_exp[i],min(e_ml_exp[i],v_ml_exp[i])))
        if i < len(v_lm_obs): lm_obs, lm_exp = str(int(v_lm_obs[i])), r"{}$\pm${}".format(*GetRoundedValues(v_lm_exp[i],min(e_lm_exp[i],v_lm_exp[i])))
        buff += "{:<5s} & {:<15s} & {:<5s} & {:<15s} & {:<5s} & {:<15s} & {:<5s} & {:<15s} & {:<5s} & {:<15s} & {:<5s} \\\\ \n".format(
            srs,
            hh_exp,hh_obs,
            hl_exp,hl_obs,
            ll_exp,ll_obs,
            ml_exp,ml_obs,
            lm_exp,lm_obs,
            )
    return buff


if __name__ == "__main__":
    # make_plots()

    # fh = open("table_dump.txt","w")
    # for x in ["shapes_prefit","shapes_fit_b"]:
    #     make_plots(typ=x)
    #     for region in ch_name_map.values():
    #         d_yields = parse_yields(d_hists[x][region])
    #         buff = print_table(d_yields,regions=region)
    #         fh.write("\n\n----- {} {} -----\n\n".format(region.upper(),x))
    #         fh.write(buff)
    #     if x == "shapes_fit_b":
    #     # if x == "shapes_prefit":
    #         with open("dump.pkl","w") as fhdump:
    #             pickle.dump(d_hists[x],fhdump)
    #     buff = print_slim_table(d_hists[x])
    #     fh.write("\n\n----- SUMMARY SLIM TABLE for {} {} -----\n\n".format(region.upper(),x))
    #     fh.write(buff)
    # fh.close()

    # make_plots(typ="shapes_prefit", regions=["srmloffz","srmlonz"])
    # sys.exit()

    # d_yields = parse_yields(d_hists["shapes_prefit"]["srml"], extra="onz")
    # buff = print_table(d_yields,regions="srml")
    # print(
    #         buff
    # )
    # # print_table(d_yields,regions="srmloffz")

    """
    fh = open("table_dump.txt","w")
    for x in ["shapes_prefit"]:
        make_plots(typ=x)
        for region in ch_name_map.values():
            d_yields = parse_yields(d_hists[x][region])
            buff = print_table(d_yields,regions=region)
            fh.write("\n\n----- {} {} -----\n\n".format(region.upper(),x))
            fh.write(buff)

    fh.write("\n\n----- {} {} -----\n\n".format("SRMLONZ","shapes_prefit"))
    d_yields = parse_yields(d_hists["shapes_prefit"]["srml"], extra="onz")
    fh.write(print_table(d_yields,regions="srml"))

    fh.write("\n\n----- {} {} -----\n\n".format("SRMLOFFZ","shapes_prefit"))
    d_yields = parse_yields(d_hists["shapes_prefit"]["srml"], extra="offz")
    fh.write(print_table(d_yields,regions="srml"))

    fh.close()
    """

    from totalregions import get_total_yields
    fh = open("table_dump_total.txt","w")
    for x in ["shapes_prefit"]:
        for region in ch_name_map.values():
            d_yields = get_total_yields(region)
            buff = print_table(d_yields,regions=region)
            fh.write("\n\n----- {} {} -----\n\n".format(region.upper(),x))
            fh.write(buff)

    fh.write("\n\n----- {} {} -----\n\n".format("SRMLONZ","shapes_prefit"))
    d_yields = get_total_yields("srml",extra="onz")
    fh.write(print_table(d_yields,regions="srml"))

    fh.write("\n\n----- {} {} -----\n\n".format("SRMLOFFZ","shapes_prefit"))
    d_yields = get_total_yields("srml",extra="offz")
    fh.write(print_table(d_yields,regions="srml"))

    fh.close()
