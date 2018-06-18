import os
import numpy as np
import uproot
import json

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery.plotter import plot_stack
from matplottery.utils import Hist1D

from analysis.limits.errors import E

bginfo = [
        # ("tightfake" , "Nonprompt lep.", [0.85, 0.85, 0.85]),
        # ("fakes_mc" , "$t\\bar{t}$", [0.9, 0.9, 0.9]),
        ("rares"    , "Rare"          , [1.0, 0.4, 1.0]),
        ("xg"       , "$X\\gamma$"    , [0.4, 0.0, 0.8]),
        ("ttvv"     , "$t\\bar{t}VV$" , [0.4, 0.6, 1.0]),
        ("flips"    , "Charge misid." , [0.4, 0.4, 0.4]),
        ("tth"      , "$t\\bar{t}H$"  , [0.4, 0.4, 0.6]),
        ("fakes" , "Nonprompt lep.", [0.85, 0.85, 0.85]),
        ("ttz"      , "$t\\bar{t}Z$"  , [0.4, 0.8, 0.4]),
        ("ttw"      , "$t\\bar{t}W$"  , [0.0, 0.4, 0.0]),
        ]
bginfo_app = [
        ("fakes_unw" , "AR nonprompt lep.", [0.45, 0.45, 0.45]),
        # ("fakes_mc_unw" , "AR $t\\bar{t}$", [0.45,0.45,0.45]),
        ]
label_sig = "$t\\bar{t}t\\bar{t}$"

plot_info = {

        "SR_TOTAL"        : [("",), "SR_TOTAL"],
        "SRCR_TOTAL"      : [("",), "SRCR_TOTAL"],
        "ht"              : [("ttzcr","ttwcr","sr","br"), "ht"],
        "met"             : [("ttzcr","ttwcr","sr","br"), "met"],
        "mvis"            : [("ttzcr","ttwcr","sr","br"), "mvis"],
        "mtvis"           : [("ttzcr","ttwcr","sr","br"), "mtvis"],
        "njets"           : [("ttzcr","ttwcr","sr","br"), "njets"],
        "nbtags"          : [("ttzcr","ttwcr","sr","br"), "nbtags"],
        "mtmin"           : [("ttzcr","ttwcr","sr","br"), "mtmin"],
        "mll"             : [("ttzcr","ttwcr","sr","br"), "mll"],
        "mllos"           : [("ttzcr",), "mllos"],
        "type"            : [("ttzcr","ttwcr","sr","br"), "type"],
        "charge"          : [("ttzcr","ttwcr","sr","br"), "charge"],
        "nleps"           : [("ttzcr","ttwcr","sr","br"), "nleps"],
        "l1pt"            : [("ttzcr","ttwcr","sr","br"), "l1pt"],
        "l2pt"            : [("ttzcr","ttwcr","sr","br"), "l2pt"],
        "l3pt"            : [("ttzcr","sr","br"), "l3pt"],
        "mtop1"           : [("sr",), "mtop1"],
        "mtop2"           : [("sr",), "mtop2"],
        "sip3d_mu_lep1"   : [("sr","br"), "sip3d_mu_lep1"],
        "sip3d_mu_lep2"   : [("sr","br"), "sip3d_mu_lep2"],
        "mu_l1pt"         : [("sr","br"), "mu_l1pt"],
        "mu_l2pt"         : [("sr","br"), "mu_l2pt"],
        "mu_l3pt"         : [("sr","br"), "mu_l3pt"],
        "l1eta_mu"        : [("sr","br"), "l1eta_mu"],
        "l2eta_mu"        : [("sr","br"), "l2eta_mu"],
        "lep1_mu_miniIso" : [("sr","br"), "lep1_mu_miniIso"],
        "lep2_mu_miniIso" : [("sr","br"), "lep2_mu_miniIso"],
        "lep1_mu_ptRel"   : [("sr","br"), "lep1_mu_ptRel"],
        "lep2_mu_ptRel"   : [("sr","br"), "lep2_mu_ptRel"],
        "lep1_mu_ptRelfail"   : [("sr","br"), "lep1_mu_ptRelfail"],
        "lep2_mu_ptRelfail"   : [("sr","br"), "lep2_mu_ptRelfail"],
        "lep1_mu_ptRatio"   : [("sr","br"), "lep1_mu_ptRatio"],
        "lep2_mu_ptRatio"   : [("sr","br"), "lep2_mu_ptRatio"],
        "sip3d_el_lep1"   : [("sr","br"), "sip3d_el_lep1"],
        "sip3d_el_lep2"   : [("sr","br"), "sip3d_el_lep2"],
        "el_l1pt"         : [("sr","br"), "el_l1pt"],
        "el_l2pt"         : [("sr","br"), "el_l2pt"],
        "el_l3pt"         : [("sr","br"), "el_l3pt"],
        "l1eta_el"        : [("sr","br"), "l1eta_el"],
        "l2eta_el"        : [("sr","br"), "l2eta_el"],
        "lep1_el_miniIso" : [("sr","br"), "lep1_el_miniIso"],
        "lep2_el_miniIso" : [("sr","br"), "lep2_el_miniIso"],
        "lep1_el_ptRel"   : [("sr","br"), "lep1_el_ptRel"],
        "lep2_el_ptRel"   : [("sr","br"), "lep2_el_ptRel"],
        "lep1_el_ptRelfail"   : [("sr","br"), "lep1_el_ptRelfail"],
        "lep2_el_ptRelfail"   : [("sr","br"), "lep2_el_ptRelfail"],
        "lep1_el_ptRatio"   : [("sr","br"), "lep1_el_ptRatio"],
        "lep2_el_ptRatio"   : [("sr","br"), "lep2_el_ptRatio"],
        "bjetpt"          : [("sr","br"), "bjetpt"],
        "jetpt"           : [("sr","br"), "jetpt"],

        }

f_2017 = uproot.open("histos.root")
f_2016 = uproot.open("/home/users/namin/2018/fourtop/80x/FTAnalysis/analysis/yields/histos.root")

def write_table(data, bgs, sig, outname=None):
    from pytable import Table
    tab = Table()
    sumbgs = sum(bgs)
    databg = data/sumbgs
    procs = bgs+[sumbgs,data,databg,sig]
    cnames = [bg[1].replace("$","").replace("\\","") for bg in bginfo] + ["Total bkg","Data", "Data/bkg", "tttt"]
    tab.set_column_names(["bin"]+cnames)
    sep = "+-"
    precision = 2
    binpairs = zip(data.edges[:-1],data.edges[1:])
    tab.set_theme_basic()
    for ibin,binrow in enumerate(binpairs):
        row = ["[{},{}]".format(*binrow)]
        for iproc,proc in enumerate(procs):
            cent = max(proc.counts[ibin],0.)
            err = proc.errors[ibin]
            row.append(("{0:5.%if} {1}{2:%i.%if}" % (precision,precision+3,precision)).format(cent,sep,err))
        tab.add_row(row)
    tab.add_line()

    row = ["total"]
    for iproc,proc in enumerate(procs):
        if iproc == len(procs)-2:
            totbg = E(sum(sumbgs.counts), np.sum(sumbgs.errors**2.)**0.5)
            totdata = E(sum(data.counts))
            ratio = totdata/ totbg
            cent, err = ratio[0], ratio[1]
        else:
            cent = sum(proc.counts)
            err = np.sum(proc.errors**2.)**0.5
        row.append(("{0:5.%if} {1}{2:%i.%if}" % (precision,precision+3,precision)).format(cent,sep,err))
    tab.add_row(row)

    if outname:
        with open(outname,"w") as fhout:
            towrite = "".join(tab.get_table_string(show_row_separators=False,show_alternating=False))
            fhout.write(towrite)
            parts = towrite.split("\n")
            header = parts[:3]
            binparts = parts[3:-4]
            total = parts[-4:-1]
            table_info = { "header":"<br>".join(header), "bins":binparts, "total":"<br>".join(total) }
            return table_info

def get_info(bin_coords, table_info):
    info = { "xedges": [], "yedges": [], "table": table_info }
    for ipair,pair in enumerate(zip(bin_coords[::2],bin_coords[1::2])):
        xlow,xhigh = round(pair[0][0],5), round(pair[1][0],5)
        ylow,yhigh = round(pair[0][1],5), round(pair[1][1],5)
        info["xedges"].append([xlow,xhigh])
        info["yedges"].append([ylow,yhigh])
    return info

def make_plots(pinfo):
    bin_json = {}
    hname, (regions, xlabel) = pinfo
    for region in regions:
        title = region.upper()


        name = "{}_{}".format(region,hname)
        if region == "": name = hname[:]
        outputname = "plots/{}.pdf".format(name.replace("_TOTAL",""))

        print "Working on {}".format(name)

        bgs_2017 = [Hist1D(f_2017["{}_{}".format(name,proc)], label=label,color=color) for proc,label,color in bginfo]
        sig_2017 = Hist1D(f_2017["{}_tttt".format(name)], color="r", label=label_sig)
        data_2017 = Hist1D(f_2017["{}_data".format(name)], label="Data")

        if region == "sr":
            data_2017._counts *= 0.
            data_2017._errors *= 0.
        if hname.startswith("SR_"):
            data_2017._counts *= 0.
            data_2017._errors *= 0.
        if hname.startswith("SRCR_"):
            data_2017._counts[2:] *= 0.
            data_2017._errors[2:] *= 0.


        _, _, bin_coords = plot_stack(
                bgs=bgs_2017, data=data_2017, sigs=[sig_2017], title=title, xlabel=xlabel, filename=outputname,
                cms_type = "Preliminary",
                lumi = "41.3",
                ratio_range=[0.,4.],
                mpl_hist_params={
                    # "edgecolor": "k",
                    # "linewidth": 0.1,
                    },
                mpl_legend_params={
                    "framealpha": 0.4,
                    "ncol": 2,
                    "labelspacing": 0.5,
                    "columnspacing": 0.5,
                    },
                # do_bkg_syst=True,
                return_bin_coordinates=True,
                )
        table_info = write_table(data_2017,bgs_2017,sig_2017,outname=outputname.replace(".pdf",".txt"))
        bin_json[name.replace("_TOTAL","")] = get_info(bin_coords,table_info)

        # 2016 + 2017 plots if there's a 2016 histogram
        if "{}_tttt".format(name) in f_2016:
            bgs_2016 = [Hist1D(f_2016["{}_{}".format(name,proc)], label=label,color=color) for proc,label,color in bginfo]
            sig_2016 = Hist1D(f_2016["{}_tttt".format(name)], color="r", label=label_sig)
            data_2016 = Hist1D(f_2016["{}_data".format(name)], label="Data")
            data_sum = data_2016 + data_2017
            sig_sum = sig_2016 + sig_2017
            bgs_sum = [bg1+bg2 for bg1,bg2 in zip(bgs_2016,bgs_2017)]
            sumname = "plots/run2_{}.pdf".format(name.replace("_TOTAL",""))
            _, _, bin_coords = plot_stack(
                    bgs=bgs_sum, data=data_sum, sigs=[sig_sum], title=title, xlabel=xlabel, filename=sumname,
                    cms_type = "Preliminary",
                    lumi = "77.2",
                    ratio_range=[0.,4.],
                    mpl_hist_params={
                        # "edgecolor": "k",
                        # "linewidth": 0.1,
                        },
                    mpl_legend_params={
                        "framealpha": 0.4,
                        "ncol": 2,
                        "labelspacing": 0.5,
                        "columnspacing": 0.5,
                        },
                    # do_bkg_syst=True,
                    return_bin_coordinates=True,
                    )
            table_info = write_table(data_sum,bgs_sum,sig_sum,outname=sumname.replace(".pdf",".txt"))
            bin_json["run2_"+name.replace("_TOTAL","")] = get_info(bin_coords,table_info)

            # os.system("ic {}".format(sumname))

        # Application region plots for 2017
        bgs = [Hist1D(f_2017["{}_{}".format(name,proc)], label=label,color=color) for proc,label,color in bginfo_app]
        apponame = "plots/app_{}.pdf".format(name.replace("_TOTAL",""))
        plot_stack(
                bgs=bgs, title=title, xlabel=xlabel, filename=apponame,
                cms_type = "Preliminary",
                lumi = "41.3",
                mpl_hist_params={
                    },
                mpl_legend_params={
                    "framealpha": 0.4,
                    "ncol": 2,
                    "labelspacing": 0.5,
                    "columnspacing": 0.5,
                    },
                do_bkg_syst=True,
                )

        # os.system("ic {}".format(outputname))

    return bin_json


# # single thread
# for pinfo in plot_info.items():
#     make_plots(pinfo)

# multithread
os.nice(10)
from multiprocessing import Pool as ThreadPool
pool = ThreadPool(15)
total_bin_json = {}
jsons = []
for result in pool.imap_unordered(make_plots, plot_info.items()):
    total_bin_json.update(result)
pool.close()
pool.join()

json.dump(total_bin_json, open("plots/binInfo.json","w"))

# os.system("NOCONVERT=true niceplots plots plots_94x_May1")
# os.system("NOCONVERT=true niceplots plots plots_94xmcfakes_May1")
os.system("NOCONVERT=true niceplots plots plots_yields_May4")
