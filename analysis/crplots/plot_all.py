from tqdm import tqdm
import json
import os
import numpy as np
import itertools
import uproot

from analysis.utils.plotting_utils import write_table

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery.plotter import plot_stack
from matplottery.utils import Hist1D, MET_LATEX


labels = {

    "ht": "$H_{T}$",
    "met": MET_LATEX,
    "mll": "$m_{ll}$",
    "njets": "Njets",
    "nbtags": "Nbtags",
    "bdisc1": "Disc leading bjet",
    "type": "hyp type (mm, em, ee)",
    # "matchtype": "unmatched/light, b, c",
    # "mtmin": "$m_{T}^\\mathrm{min}$",
    "nleps": "nleps",
    "pt1": "$p_T$(lep1)",
    "pt2": "$p_T$(lep2)",
    "pt3": "$p_T$(lep3)",
    "pte": "$p_T$(e)",
    "ptm": "$p_T$(m)",
    # "eta1": r"$\eta$(lep1)",
    # "eta2": r"$\eta$(lep2)",
    "etae": r"$\eta$(e)",
    "etam": r"$\eta(\mu)$",
    "phie": r"$\phi$(e)",
    "phim": r"$\phi(\mu)$",
    "q1": "charge - lep 1",
    "nvtx": "# good vertices",
    # "nisrjets": "Njets (ISR)",
    # "nisrmatch": "Njets (ISR - Truth Matched)",
    "rawmet": "Raw "+MET_LATEX,
    # "zmll": "$m_{ll} (Z-cand)$",
    "ptrele": "$p_T$rel - e",
    "ptrelm": r"$p_T$rel - $\mu$",
    "ptratioe": "$p_T$ratio - e",
    "ptratiom": r"$p_T$ratio - $\mu$",
    "ptrelfaile": "$p_T$rel - e, fail $p_T$ratio",
    "ptrelfailm": r"$p_T$rel - $\mu$, fail $p_T$ratio",
    "miniisoe": "mini-iso - e",
    "miniisom": r"mini-iso - $\mu$",
    "nmiss1": "exp miss inner hits - lep 1",
    "nmiss2": "exp miss inner hits - lep 2",

    # "ptrel1": "$p_T$rel - lep 1",
    # "ptrel2": "$p_T$rel - lep 2",
    # "ptratio1": "$p_T$ratio - lep 1",
    # "ptratio2": "$p_T$ratio - lep 2",
    # "ptrelfail1": "$p_T$rel - lep 1, fail $p_T$ratio",
    # "ptrelfail2": "$p_T$rel - lep 2, fail $p_T$ratio",
    # "miniiso1": "mini-iso - lep 1",
    # "miniiso2": "mini-iso - lep 2",
    # "htb": "$H_{T} B-Jets$",
    # "nlb40": "N-loose Btags, $p_T$>40",
    # "ntb40": "N-tight Btags, $p_T$>40",
    # "type3l": "3l type (mmm, mme, mee, eee)",

}

d_label_colors = {
            "dy":                      (r"DY+jets",        [0.4, 0.6, 1.0]),
            "fakes":                   (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
            "flips":                   (r"Charge misid.",  [0.4, 0.4, 0.4]),
            "rares":                   ("Rare",            [1.0, 0.4, 1.0]),
            "singletop":               ("Single Top",      [1.0, 0.4, 0.0]),
            "tt":                      (r"$t\bar{t}$",     [0.8, 0.8, 0.8]),
            "ttfake":                   (r"$t\bar{t}$ Nonprompt", [0.85, 0.85, 0.85]),
            "tth":                     (r"$t\bar{t}H$",    [0.4, 0.4, 0.6]),
            "ttw":                     (r"$t\bar{t}W$",    [0.0, 0.4, 0.0]),
            "ttz":                     (r"$t\bar{t}Z$",    [0.4, 0.8, 0.4]),
            "vv":                      (r"VV",             [0.0, 0.4, 0.8]),
        }

bginfo = {
        "os": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        "os_noht": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        "osloose": { k:d_label_colors[k] for k in [ "flips", "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        "nj2": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "nj2mc": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "ttfake", "rares", ] },
        "htnb1": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "htnb1mu": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "htnb1mc": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "ttfake", "rares", "singletop", ] },
        "htnb1mcmu": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "ttfake", "rares", "singletop", ] },
        "crz": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "crw": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "br": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "sshh": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "sshhmc": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "ttfake", "rares", ] },
        "tt_isr": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        "tt_isr_reweight_check": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        }

# yuck, gotta make these global for multiprocessing since uproot file objects can't be pickled
files, other_files = {}, {}

def worker(info):
    global files, other_files

    outputdir, year, lumi, region, flav, var = info
    title = region.upper()
    xlabel = labels[var]
    hname = "{}_{}_{}".format(region,var,flav)

    # htnb1mc is htnb1 but with ttbar MC fakes instead of data-driven
    # we're using the original htnb1 histograms though, so fix hname
    if "htnb1mc" in region:
        hname = hname.replace("htnb1mc","htnb1")
    if "sshhmc" in region:
        hname = hname.replace("sshhmc","sshh")
    if "nj2mc" in region:
        hname = hname.replace("nj2mc","nj2")

    if other_files:
        bgs = [
                sum([Hist1D(files[proc][hname],label=label,color=color)] + [Hist1D(other_files[y][proc][hname],label=label,color=color) for y in other_files.keys()])
                for proc,(label,color) in bginfo[region].items()
                ]
        data = sum([Hist1D(files["data"][hname])] + [Hist1D(other_files[y]["data"][hname]) for y in other_files.keys()])
    else:
        bgs = [Hist1D(files[proc][hname], label=label,color=color) for proc,(label,color) in bginfo[region].items()]
        data = Hist1D(files["data"][hname])
    bgs = sorted(bgs, key=lambda bg: bg.get_integral())
    data.set_attr("label", "Data [{}]".format(int(data.get_integral())))
    if data.get_integral() < 1e-6: return
    if abs(sum(bgs).get_integral()) < 1e-6: return

    if other_files:
        fname = "{}/run2_{}_{}_{}.png".format(outputdir,region,var,flav)
    else:
        fname = "{}/y{}_{}_{}_{}.png".format(outputdir,year,region,var,flav)
    plot_stack(bgs=bgs, data=data, title=title, xlabel=xlabel, filename=fname,
               cms_type = "Preliminary",
               lumi = lumi,
               ratio_range=[0.0,2.0],
               )
    # os.system("ic {}".format(fname))
    table_info = write_table(data,bgs,outname=fname.replace(".png",".txt"))
    return fname

def make_plots(outputdir="plots", inputdir="outputs", year=2017, lumi="41.5", other_years=[], regions=[]):
    global files, other_files

    os.system("mkdir -p {}/".format(outputdir))

    files = { proc:uproot.open("{}/histos_{}_{}.root".format(inputdir,proc,year)) for proc in (list(set(sum(map(lambda x:x.keys(),bginfo.values()),[])))+["data"]) }
    other_files = {}
    for y in other_years:
        other_files[y] = { proc:uproot.open("{}/histos_{}_{}.root".format(inputdir,proc,y)) for proc in (list(set(sum(map(lambda x:x.keys(),bginfo.values()),[])))+["data"]) }


    # for region in ["htnb1mc","htnb1","os","osloose","br","crw","crz","tt_isr_reweight_check"]:
    # regions = ["htnb1mc","htnb1","htnb1mcmu","htnb1mu","os","os_noht","osloose","br","crw","crz"]
    regions = regions or ["htnb1mc","htnb1","os","sshh","sshhmc","osloose","br","crw","crz"]
    flavs = ["ee","em","mm","in"]
    varss = labels.keys()
    infos = [[outputdir,year,lumi]+list(x) for x in itertools.product(regions,flavs,varss)]

    os.nice(10)
    from multiprocessing import Pool as ThreadPool
    pool = ThreadPool(25)
    for res in pool.imap_unordered(worker,infos):
        if res:
            print "Wrote {}".format(res)


if __name__ == "__main__":

    # make_plots(
    #         outputdir="plots_20172018_nj2",
    #         inputdir="outputs_temp",
    #         year=2018,
    #         lumi="77.1", # 2017+2018 --> 41.53+35.53 = 77.06
    #         other_years = [2017],
    #         regions = ["nj2","nj2mc"],
    #         )

    make_plots(
            outputdir="plots_temp",
            inputdir="outputs_temp",
            regions = ["htnb1","htnb1mu"],
            year=2018,
            lumi="112.9", # 2016+2017+2018 --> 35.87+41.53+35.53 = 112.9
            other_years = [2016,2017],
            )

    # make_plots(
    #         outputdir="plots_2017_nmiss0",
    #         inputdir="outputs_2017_nmiss0",
    #         year=2017, lumi="41.5",
    #         )
    # make_plots(
    #         outputdir="plots_2017_nmiss1",
    #         inputdir="outputs_2017_nmiss1",
    #         year=2017, lumi="41.5",
    #         )

    # inputdir = "outputs_2018"
    # # 2017 alone
    # outputdir = "plots_2017"
    # make_plots(
    #         outputdir=outputdir,
    #         inputdir=inputdir,
    #         year=2017,
    #         lumi="41.5",
    #         )
    # # 2018 alone
    # outputdir = "plots_2018"
    # make_plots(
    #         outputdir=outputdir,
    #         inputdir=inputdir,
    #         year=2018,
    #         lumi="35.5",
    #         )
    # # 2018 + 2017
    # outputdir = "plots_20172018"
    # make_plots(
    #         outputdir=outputdir,
    #         inputdir=inputdir,
    #         year=2018,
    #         lumi="77.1", # 2017+2018 --> 41.53+35.53 = 77.06
    #         other_years = [2017],
    #         )
    # # 2016 + 2018 + 2017
    # outputdir = "plots_run2"
    # make_plots(
    #         outputdir=outputdir,
    #         inputdir=inputdir,
    #         year=2018,
    #         lumi="112.9", # 2016+2017+2018 --> 35.87+41.53+35.53 = 112.9
    #         other_years = [2016,2017],
    #         )
    # os.system("niceplots plots_2017 plots_crs2017_Sep3")
    # os.system("niceplots plots_2018 plots_crs2018_Sep3")
    # os.system("niceplots plots_20172018 plots_crs20172018_Sep3")
    # os.system("niceplots plots_run2 plots_crsrun2_Sep3")
