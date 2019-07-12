import math
import os
import numpy as np
import uproot

from analysis.utils.plotting_utils import write_table

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery import Hist1D, plot_stack
from matplottery.utils import binomial_obs_z
import matplotlib
# ../../../common/matplottery/matplottery/utils.py
import pandas as pd

np.set_printoptions(linewidth=180)

ch_name_map = {
        # "ch1": "srcr",
        }

# name_nbin_map = {
#         "srcr": 16,
#         "srdisc": 18,
#         }

d_label_colors = {}
d_label_colors["ss"] = {
        "fakes" : (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
        # "fakes_mc" : (r"MC fakes", [0.85, 0.85, 0.85]),
        "flips" : (r"Charge misid.", [0.4, 0.4, 0.4]),
        "rares" : (r"Rare", [1.0, 0.4, 1.0]),
        "tth"   : (r"$t\bar{t}H$", [0.4, 0.4, 0.6]),
        "wz"  : (r"WZ" , [1.0,0.8,0.0]),
        "ww"  : (r"WW" , [1.0,0.6,0.0]),
        "ttvv"  : (r"$t\bar{t}VV$" , [0.4, 0.6, 1.0]),
        "tttt"  : (r"$t\bar{t}t\bar{t}$" , [0.75,0.15,0.22]),
        "ttw"   : (r"$t\bar{t}W$", [0.0, 0.4, 0.0]),
        "ttz"   : (r"$t\bar{t}Z$", [0.4, 0.8, 0.4]),
        "xg"    : (r"$X\gamma$" , [0.4, 0.0, 0.8]),
        }

# f = uproot.open("../fitDiagnosticsss.root") # all 5 regions
# f = uproot.open("ssforpostfitUncorrTTVWZFakes.root") # all 5 regions
d_hists = {}
d_sfs = {} # key is process and value is postfit/prefit SF # NOTE, sb fit
def load_info(region="srcr",tag="run2"):
    global d_hists
    global d_sfs

    # f = uproot.open("fitDiagnosticsrun2_{}_obs.root".format(region))
    f = uproot.open("fitDiagnostics{}_{}_obs.root".format(tag,region))

    for typ in ["shapes_prefit","shapes_fit_s"]:
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
            # print typ,ch,hists,type(hists), hists.keys()
            for hname in hists:
                hname = hname.rsplit(";")[0]
                if "covar" in hname: continue
                # print name, hname, type(hname)
                h = Hist1D(hists[hname],label=hname)
                # if name in name_nbin_map: h.restrict_bins(range(name_nbin_map[name]))
                d_hists[typ][name][hname] = h
                # d_hists[typ][name][hname] = 1
            data = d_hists[typ][name]["data"]
            bg = d_hists[typ][name]["total_background"]
            d_hists[typ][name]["pulls"] = binomial_obs_z(data.counts,bg.counts,bg.errors)
    for proc in d_hists["shapes_prefit"].values()[0].keys():
        if proc in ["pulls","data"]: continue
        ipostfit = 0.
        iprefit = 0.
        for region in d_hists["shapes_fit_s"].keys():
            postfit = d_hists["shapes_fit_s"][region].get(proc,None)
            prefit = d_hists["shapes_prefit"][region].get(proc,None)
            if postfit is not None: ipostfit += postfit.get_integral()
            if prefit is not None: iprefit += prefit.get_integral()
        d_sfs[proc] = ipostfit/iprefit


def make_plots(
        regions=["srdisc"],
        outputdir="plots/",
        # outputdir="plots_unblind/",
        typ="shapes_prefit",
        # typ="shapes_fit_s",
        # lumi="41.5",
        lumi="137",
        procs=['fakes', 'flips', 'ttvv', 'ttz', 'ttw', 'tth', 'rares', 'xg','tttt'],
        # year=2017,
        year="run2",
        ):

    # print d_hists[typ]["SS"]["pulls"]

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
        # print d_hists
        # hists = d_hists["shapes_fit_s"][region]
        hists = d_hists[typ]["SS"] # note, single cards always have SS as the region, so hardcode! NOTE NOTE
        data = hists["data"]
        totbg = hists["total"]
        procs_ = procs[:]
        if region == "srml": procs_ = list(set(procs_)-set(["ww","flips"]))
        bgs = [
                Hist1D(
                    hists[k],
                    label=d_label_colors["ss"][k][0],
                    color=d_label_colors["ss"][k][1],
                    ) for k in procs_
                ]
        bgs[:-1] = sorted(bgs[:-1], key=lambda bg: bg.get_integral())

        which = "pre-fit" if "pre-fit" in typ else "post-fit"
        title = region.upper()
        if region.upper() == "SRCR":
            title = "Cut-based ({})".format(which)
        elif region.upper() == "SRDISC":
            title = "BDT ({})".format(which)



        # data.set_attr("label", "Data [{}]".format(int(data.get_integral())))
        data.set_attr("label", "Data")

        ax_main_callback = None
        ax_ratio_callback = None
        mpl_legend_params = {}

        ratio_range = [0.,3.]
        mpl_legend_params["fontsize"] = 10
        mpl_legend_params["framealpha"] = 0.4
        mpl_legend_params["ncol"] = 3
        mpl_legend_params["labelspacing"] = 0.12
        xticks = range(1,70)

        if region.lower() in ["srcr"]:
            xticks = ["CRZ","CRW"]+["SR{}".format(i) for i in range(1,20)]
        if region.lower() in ["srdisc"]:
            # mpl_legend_parms
            xticks = ["CRZ"]+["SR{}".format(i) for i in range(1,20)]

        def ax_main_callback(ax):
            # ax.set_ylim([0.05,ax.get_ylim()[1]*1.2])
            # ax.set_ylim([0.05,ax.get_ylim()[1]*2.0])
            if region.lower() == "srdisc":
                ax.set_ylim([0.08,ax.get_ylim()[1]*5.2])
            else:
                ax.set_ylim([0.08,ax.get_ylim()[1]*2.0])
            ax.set_yscale("log", nonposy='clip'),

        if year in [2016,2017,2018]:
            ystr = "y{}".format(year)
        else:
            ystr = "run2"
        fname = "{}/{}_{}_TOTAL_{}.pdf".format(outputdir,ystr,region.upper(),which.replace("-",""))
        plot_stack(bgs=bgs, data=data, title=title, xlabel="", ylabel="Events", filename=fname,
                   # cms_type = "Preliminary",
                   cms_type = "",
                   lumi = lumi,
                   ratio_range=ratio_range,
                   xticks=xticks,
                   mpl_xtick_params = {
                       "fontsize": 9,
                       },
                   mpl_ratio_params={
                       "label":"Data/Pred.",
                       },
                   ax_main_callback=ax_main_callback,
                   mpl_legend_params=mpl_legend_params,
                   ax_ratio_callback=ax_ratio_callback,
                   do_bkg_syst=True,
                   override_bkg_syst=totbg.errors,
                   )
        print "Made {}".format(fname)
        # os.system("ic {}".format(fname))
        table_info = write_table(data,bgs,outname=fname.replace(".pdf",".txt"))

        fname = "{}/{}_{}_TOTAL_{}_pulls.pdf".format(outputdir,ystr,region.upper(),which.replace("-",""))

        # hpulls = Hist1D(data)
        hpulls = Hist1D()
        # print hpulls
        hpulls._counts = 1.0*d_hists[typ]["SS"]["pulls"]
        hpulls._errors = 0.0*hpulls._counts
        hpulls._edges = 1.*data.edges
        # print hpulls
        # hpulls._errors *= 0.
        # print hpulls._errors_up
        # print hpulls._errors_down
        # hpulls._errors_down *= 0.
        print hpulls.counts
        def pull_ratio_callback(ax_ratio):
            for y in [-2,-1,0,1,2]:
                ax_ratio.plot([ax_ratio.get_xlim()[0],ax_ratio.get_xlim()[1]],[y,y],color="gray",linewidth=1.,alpha=0.5)
            ax_ratio.yaxis.set_ticks([-3,-2,-1,0,1,2,3])
            ax_ratio.yaxis.set_major_formatter(matplotlib.ticker.ScalarFormatter())

        plot_stack(bgs=bgs, data=data, title=title, xlabel="", filename=fname,
                   cms_type = "Preliminary",
                   lumi = lumi,
                   ratio = hpulls,
                   ratio_range=[-3.,3.],
                   xticks=xticks,
                   mpl_ratio_params={
                       "label":"Data/Pred.",
                       },
                   ax_main_callback=ax_main_callback,
                   mpl_legend_params=mpl_legend_params,
                   ax_ratio_callback=pull_ratio_callback,
                   # do_bkg_syst=True,
                   # override_bkg_syst=totbg.errors,
                   )
        print "Made {}".format(fname)
        # os.system("ic {}".format(fname))

    # print d_sfs
    # print pd.DataFrame(d_sfs.items()).to_csv()
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
    # pulls = np.concatenate([d_hists["shapes_fit_s"][x]["pulls"] for x in sorted(d_hists["shapes_fit_s"].keys())])
    # print "postfit",pulls.mean(), pulls.std()
    # h_pulls_postfit = Hist1D(pulls,bins=np.linspace(-3,3,20),label=r"postfit [${:.2f}\pm{:.2f}$]".format(pulls.mean(),pulls.std()),
    #         # weights=np.concatenate([d_hists["shapes_fit_s"][x]["total_background"].counts for x in sorted(d_hists["shapes_fit_s"].keys())]), # weight the pull by the yield in that bin
    #         )
    # print h_pulls_postfit.vis()
    # # fakes_after = d_hists["shapes_fit_s"]["srhh"]["total_background"].get_integral()
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
    # print value, error, tD, nD, expVal, expErr, extraRound
    return (formatValue(value, expVal, nDigitsValue(expVal, expErr, nD), extraRound),
            formatValue(error,expErr, nD, extraRound))

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

def print_table(d_yields, regions="srcr",precision=2,blinded=False,paper=False):
    nbins = len(d_yields["ttz"]["central"])
    colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","$\\ttVV$","X+$\\gamma$","Rares","Charge misid.","Nonprompt lep.","SM expected","Data","$\\tttt$"]
    procs = ["ttw","ttz","tth","ttvv","xg","rares","flips","fakes","total_background","data","tttt"]
    if paper:
        # colnames = ["","SM expected","Data","$\\tttt$"]
        # procs = ["total_background","data","tttt"]
        colnames = ["","SM expected","$\\tttt$","Data"]
        procs = ["total_background","tttt","data"]
    if regions == "srcr":
        srnames = ["CRZ","CRW"]+["SR{}".format(i) for i in range(1,20)]
    elif regions == "srdisc":
        # srnames = ["SR{}".format(i) for i in range(1,15)]
        srnames = ["CRZ"]+["SR{}".format(i) for i in range(1,20)]

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
                tojoin.append("{0:.0f}".format(cent))
                # if ibin in ([0,1] if regions == "srcr" else [0]) or not blinded:
                #     tojoin.append("{0:.0f}".format(cent))
                # else:
                #     tojoin.append("-".format(cent))
            else:
                if paper:
                    # tojoin.append("{}$\\pm${}".format(*pdgRound(cent,err)))
                    tojoin.append("{}$\\pm${}".format(*GetRoundedValues(cent,err)))
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

if __name__ == "__main__":

    fh = open("table_dump.txt","w")

    # fh = open("table_dump_unblind.txt","w")
    for year in [2016,2017,2018]:
        fh.write("\n\n----- {} only -----\n\n".format(year))
        for region in ["srcr","srdisc"]:
        # for region in ["srdisc"]:
            load_info(region,tag="y{}".format(year))
            for x in ["shapes_prefit","shapes_fit_s"]:
                make_plots(regions=[region],typ=x, year=year)
                d_yields = parse_yields(d_hists[x]["SS"])
                buff = print_table(d_yields,regions=region)
                fh.write("\n\n----- {} {} {} -----\n\n".format(year,region,x))
                fh.write(buff)

    fh.write("\n\n----- run2 -----\n\n")
    for region in ["srcr","srdisc"]:
    # for region in ["srdisc"]:
        load_info(region,tag="run2")
        for x in ["shapes_prefit","shapes_fit_s"]:
            make_plots(regions=[region],typ=x, year="run2")
            d_yields = parse_yields(d_hists[x]["SS"])
            buff = print_table(d_yields,regions=region)
            fh.write("\n\n----- run2 {} {} -----\n\n".format(region,x))
            fh.write(buff)
            if x == "shapes_fit_s":
                buff = print_table(d_yields,regions=region,paper=True)
                fh.write("\n\n----- FOR PAPER run2 {} {} -----\n\n".format(region,x))
                fh.write(buff)

    fh.close()

#     fh = open("slim_table_dump.txt","w")
#     for region in ["srcr","srdisc"]:
#         load_info(region,tag="run2")
#         for x in ["shapes_fit_s"]:
#             d_yields = parse_yields(d_hists[x]["SS"])
#             buff = print_table(d_yields,regions=region,paper=True)
#             fh.write("\n\n----- run2 {} {} -----\n\n".format(region,x))
#             print buff
#             fh.write(buff)
#     fh.close()
