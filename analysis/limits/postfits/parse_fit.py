import os
import numpy as np
import uproot

from analysis.utils.plotting_utils import write_table

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery import Hist1D, plot_stack
from matplottery.utils import binomial_obs_z
# ../../../common/matplottery/matplottery/utils.py
import pandas as pd

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
f = uproot.open("fitDiagnosticsssforpostfitnew.root") # all 5 regions
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
        # "fakes_mc" : (r"MC fakes", [0.85, 0.85, 0.85]),
        "flips" : (r"Charge misid.", [0.4, 0.4, 0.4]),
        "rares" : (r"Rare", [1.0, 0.4, 1.0]),
        "tth"   : (r"$t\bar{t}H$", [0.4, 0.4, 0.6]),
        "wz"  : (r"WZ" , [1.0,0.8,0.0]),
        "ww"  : (r"WW" , [1.0,0.6,0.0]),
        "ttw"   : (r"$t\bar{t}W$", [0.0, 0.4, 0.0]),
        "ttz"   : (r"$t\bar{t}Z$", [0.4, 0.8, 0.4]),
        "xg"    : (r"$X\gamma$" , [0.4, 0.0, 0.8]),
        }

def make_plots(
        regions=["srhh","srhl","srll","srml","srlm"],
        outputdir="plots/",
        lumi="136.3",
        typ="shapes_fit_b",
        procs=['fakes', 'flips', 'wz', 'ttz', 'ttw', 'tth', 'rares', 'ww', 'xg'],
        year="run2",
        ):

    # region = "srhl"
    # outputdir = "plots/"
    # lumi = "61.3"

    for region in regions:
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
        bgs = sorted(bgs, key=lambda bg: bg.get_integral())

        which = "prefit" if "prefit" in typ else "postfit"
        title = "{} ({})".format(region.upper(),which)


        if region == "srll" and lumi == "136.3": lumi_ = "131.5"

        data.set_attr("label", "Data [{}]".format(int(data.get_integral())))

        ax_main_callback = None
        ax_ratio_callback = None
        mpl_legend_params = {}

        ratio_range = [0.,3.]
        mpl_legend_params["fontsize"] = 8
        mpl_legend_params["framealpha"] = 0.4
        mpl_legend_params["ncol"] = 2
        mpl_legend_params["labelspacing"] = 0.12
        xticks = range(1,70)

        def ax_main_callback(ax):
            ax.set_ylim([0.05,ax.get_ylim()[1]*2.0])
            ax.set_yscale("log", nonposy='clip'),

        if year in [2016,2017,2018]:
            ystr = "y{}".format(year)
        else:
            ystr = "run2"
        fname = "{}/{}_{}_TOTAL_{}.pdf".format(outputdir,ystr,region.upper(),which)
        plot_stack(bgs=bgs, data=data, title=title, xlabel="bin", filename=fname,
                   cms_type = "Preliminary",
                   lumi = lumi,
                   ratio_range=ratio_range,
                   xticks=xticks,
                   ax_main_callback=ax_main_callback,
                   mpl_legend_params=mpl_legend_params,
                   ax_ratio_callback=ax_ratio_callback,
                   do_bkg_syst=True,
                   override_bkg_syst=totbg.errors,
                   )
        print "Made {}".format(fname)
        os.system("ic {}".format(fname))
        table_info = write_table(data,bgs,outname=fname.replace(".pdf",".txt"))

    print d_sfs
    print pd.DataFrame(d_sfs.items()).to_csv()
    # print bgs
    # print data
    # return
    # print "prefit bg,data:",totbg_prefit, totdata_prefit
    # print "postfit bg,data:",totbg_postfit, totdata_postfit
    # pulls = np.concatenate([d_hists["shapes_prefit"][x]["pulls"] for x in sorted(d_hists["shapes_prefit"].keys())])
    # print "prefit",pulls.mean(), pulls.std()
    # h_pulls_prefit = Hist1D(pulls,bins=np.linspace(-3,3,20),label=r"prefit [${:.2f}\pm{:.2f}$]".format(pulls.mean(),pulls.std()),
    #         weights=np.concatenate([d_hists["shapes_prefit"][x]["total_background"].counts for x in sorted(d_hists["shapes_prefit"].keys())]), # weight the pull by the yield in that bin
    #         )
    # pulls = np.concatenate([d_hists["shapes_fit_b"][x]["pulls"] for x in sorted(d_hists["shapes_fit_b"].keys())])
    # print "postfit",pulls.mean(), pulls.std()
    # h_pulls_postfit = Hist1D(pulls,bins=np.linspace(-3,3,20),label=r"postfit [${:.2f}\pm{:.2f}$]".format(pulls.mean(),pulls.std()),
    #         # weights=np.concatenate([d_hists["shapes_fit_b"][x]["total_background"].counts for x in sorted(d_hists["shapes_fit_b"].keys())]), # weight the pull by the yield in that bin
    #         )
    # print h_pulls_postfit.vis()
    # # fakes_after = d_hists["shapes_fit_b"]["srhh"]["total_background"].get_integral()
    # # print fakes_after/fakes_before
    # plot_stack(
    #         # bgs=[h_pulls_prefit,h_pulls_postfit],
    #         bgs=[h_pulls_postfit],
    #         do_stack=False,
    #         filename="plots/pulls.png",
    #         mpl_hist_params=dict(histtype="step",linewidth=2),
    #         do_bkg_errors=True,
    #         )
    # os.system("ic plots/pulls.png")

def parse_yields(d_in):
    d_yields = {}
    for key in d_in:
        if key in ["pulls"]: continue
        h1 = d_in[key]
        vals,errs = h1.counts, h1.errors
        d_yields[key] = {}
        d_yields[key]["central"] = vals
        d_yields[key]["error"] = errs
    return d_yields

def print_table(d_yields, regions="srcr",precision=2,blinded=False):
    nbins = len(d_yields["ttz"]["central"])

    # colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","$\\ttVV$","X+$\\gamma$","Rares","Charge misid.","Nonprompt lep.","SM expected","Data","$\\tttt$"]
    # procs = ["ttw","ttz","tth","ttvv","xg","rares","flips","fakes","total_background","data","tttt"]
    # if regions == "srcr":
    #     srnames = ["CRZ","CRW"]+["SR{}".format(i) for i in range(1,20)]
    # elif regions == "srdisc":
    #     # srnames = ["SR{}".format(i) for i in range(1,15)]
    #     srnames = ["CRZ"]+["SR{}".format(i) for i in range(1,20)]

    if regions in ["srml"]:
        colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","$\\WZ$","X+$\\gamma$","Rares","Nonprompt lep.","SM expected","Data"]
        procs = ["ttw","ttz","tth","wz","xg","rares","fakes","total_background","data"]
    else:
        colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","$\\WZ$","WW","X+$\\gamma$","Rares","Charge misid.","Nonprompt lep.","SM expected","Data"]
        procs = ["ttw","ttz","tth","wz","ww","xg","rares","flips","fakes","total_background","data"]
    srnames = ["SR{}".format(i) for i in range(1,100)]

    # print
    buff = ""
    for ibin in range(nbins):
        if ibin == 0:
            # print "&".join(map(lambda x: "{0:12s}".format(x),colnames)),
            buff += "&".join(map(lambda x: "{0:12s}".format(x),colnames))
            buff += " "
            # print r"\\"
            buff += "\\\\ \n"
            # print r"\hline\hline"
            buff += "\\hline \\hline \n"
            # print
            buff += "\n"
        tojoin = [srnames[ibin]]
        for proc in procs:
            # cent = d_yields[proc]["central"][ibin]
            cent = max(d_yields[proc]["central"][ibin],0.)
            err = d_yields[proc]["error"][ibin]
            if "data" in proc:
                if ibin in [0,1] or not blinded:
                    tojoin.append("{0:.0f}".format(cent))
                else:
                    tojoin.append("-".format(cent))
            else:
                tojoin.append("{0:5.2f}$\\pm${1:5.2f}".format(cent,err))
        # print " & ".join(tojoin),
        buff += " & ".join(tojoin) + " "
        # print r"\\"
        buff += "\\\\ \n"
    # print
    buff += "\n"
    return buff
    # print buff

    # from /home/users/namin/2018/fourtop/all/FTAnalysis/common/CMSSW_9_4_9/src/CombineHarvester/CombineTools/python/pdgRounding.py

def pdgRound(value, error) :
    "Given a value and an error, round and format them according to the PDG rules for significant digits"
    def threeDigits(value) :
        "extract the three most significant digits and return them as an int"
        return int(("%.2e"%float(error)).split('e')[0].replace('.','').replace('+','').replace('-',''))
    def nSignificantDigits(threeDigits) :
        assert threeDigits<1000,"three digits (%d) cannot be larger than 10^3"%threeDigits
        if threeDigits<101 : return 2 # not sure
        elif threeDigits<356 : return 2
        elif threeDigits<950 : return 1
        else : return 2
    def frexp10(value) :
        "convert to mantissa+exp representation (same as frex, but in base 10)"
        valueStr = ("%e"%float(value)).split('e')
        return float(valueStr[0]), int(valueStr[1])
    def nDigitsValue(expVal, expErr, nDigitsErr) :
        "compute the number of digits we want for the value, assuming we keep nDigitsErr for the error"
        return expVal-expErr+nDigitsErr
    def formatValue(value, exponent, nDigits, extraRound=0) :
        "Format the value; extraRound is meant for the special case of threeDigits>950"
        roundAt = nDigits-1-exponent - extraRound
        nDec = roundAt if exponent<nDigits else 0
        nDec = max([nDec, 0])
        return ('%.'+str(nDec)+'f')%round(value,roundAt)
    tD = threeDigits(error)
    nD = nSignificantDigits(tD)
    expVal, expErr = frexp10(value)[1], frexp10(error)[1]
    extraRound = 1 if tD>=950 else 0
    return (formatValue(value, expVal, nDigitsValue(expVal, expErr, nD), extraRound),
            formatValue(error,expErr, nD, extraRound))

def print_slim_table(d,x="shapes_fit_b"):
    buff = ""
    v_hh_obs = d[x]["srhh"]["data"].counts
    v_hh_exp = d[x]["srhh"]["total_background"].counts
    e_hh_exp = d[x]["srhh"]["total_background"].errors
    v_hl_obs = d[x]["srhl"]["data"].counts
    v_hl_exp = d[x]["srhl"]["total_background"].counts
    e_hl_exp = d[x]["srhl"]["total_background"].errors
    v_ll_obs = d[x]["srll"]["data"].counts
    v_ll_exp = d[x]["srll"]["total_background"].counts
    e_ll_exp = d[x]["srll"]["total_background"].errors
    v_ml_obs = d[x]["srml"]["data"].counts
    v_ml_exp = d[x]["srml"]["total_background"].counts
    e_ml_exp = d[x]["srml"]["total_background"].errors
    v_lm_obs = d[x]["srlm"]["data"].counts
    v_lm_exp = d[x]["srlm"]["total_background"].counts
    e_lm_exp = d[x]["srlm"]["total_background"].errors
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
        if i < len(v_hh_obs): hh_obs, hh_exp = str(int(v_hh_obs[i])), r"{}$\pm${}".format(*pdgRound(v_hh_exp[i],e_hh_exp[i]))
        if i < len(v_hl_obs): hl_obs, hl_exp = str(int(v_hl_obs[i])), r"{}$\pm${}".format(*pdgRound(v_hl_exp[i],e_hl_exp[i]))
        if i < len(v_ll_obs): ll_obs, ll_exp = str(int(v_ll_obs[i])), r"{}$\pm${}".format(*pdgRound(v_ll_exp[i],e_ll_exp[i]))
        if i < len(v_ml_obs): ml_obs, ml_exp = str(int(v_ml_obs[i])), r"{}$\pm${}".format(*pdgRound(v_ml_exp[i],e_ml_exp[i]))
        if i < len(v_lm_obs): lm_obs, lm_exp = str(int(v_lm_obs[i])), r"{}$\pm${}".format(*pdgRound(v_lm_exp[i],e_lm_exp[i]))
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

    fh = open("table_dump.txt","w")
    for x in ["shapes_prefit","shapes_fit_b"]:
        make_plots(typ=x)
        for region in ch_name_map.values():
            d_yields = parse_yields(d_hists[x][region])
            buff = print_table(d_yields,regions=region)
            fh.write("\n\n----- {} {} -----\n\n".format(region.upper(),x))
            fh.write(buff)
        buff = print_slim_table(d_hists)
        fh.write("\n\n----- SUMMARY SLIM TABLE for {} {} -----\n\n".format(region.upper(),x))
        fh.write(buff)
    fh.close()
