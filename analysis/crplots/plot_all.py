from tqdm import tqdm
import json
import os
import numpy as np
import uproot

from analysis.limits.errors import E
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
    "matchtype": "unmatched/light, b, c",
    "mtmin": "$m_{T}^\\mathrm{min}$",
    "nleps": "nleps",
    "pt1": "$p_T$(lep1)",
    "pt2": "$p_T$(lep2)",
    "pt3": "$p_T$(lep3)",
    "pte": "$p_T$(e)",
    "ptm": "$p_T$(m)",
    "eta1": r"$\eta$(lep1)",
    "eta2": r"$\eta$(lep2)",
    "etae": r"$\eta$(e)",
    "etam": r"$\eta(\mu)$",
    "q1": "charge - lep 1",
    "nvtx": "# good vertices",
    "nisrjets": "Njets (ISR)",
    "nisrmatch": "Njets (ISR - Truth Matched)",
    "rawmet": "Raw "+MET_LATEX,
    "zmll": "$m_{ll} (Z-cand)$",
    "ptrele": "$p_T$rel - e",
    "ptrelm": r"$p_T$rel - $\mu$",
    "ptratioe": "$p_T$ratio - e",
    "ptratiom": r"$p_T$ratio - $\mu$",
    "ptrelfaile": "$p_T$rel - e, fail $p_T$ratio",
    "ptrelfailm": r"$p_T$rel - $\mu$, fail $p_T$ratio",
    "miniisoe": "mini-iso - e",
    "miniisom": r"mini-iso - $\mu$",

    # "ptrel1": "$p_T$rel - lep 1",
    # "ptrel2": "$p_T$rel - lep 2",
    # "ptratio1": "$p_T$ratio - lep 1",
    # "ptratio2": "$p_T$ratio - lep 2",
    # "ptrelfail1": "$p_T$rel - lep 1, fail $p_T$ratio",
    # "ptrelfail2": "$p_T$rel - lep 2, fail $p_T$ratio",
    # "miniiso1": "mini-iso - lep 1",
    # "miniiso2": "mini-iso - lep 2",
    # "nmiss1": "exp miss inner hits - lep 1",
    # "nmiss2": "exp miss inner hits - lep 2",
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
        "osloose": { k:d_label_colors[k] for k in [ "flips", "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        "htnb1": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "htnb1mc": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "ttfake", "rares", "singletop", ] },
        "crz": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "crw": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "br": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "tt_isr": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        "tt_isr_reweight_check": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        }

def make_plots(outputdir="plots", inputdir="outputs", year=2017, lumi="41.5", other_years=[]):
    bin_json = {}

    os.system("mkdir -p {}/".format(outputdir))

    files = { proc:uproot.open("{}/histos_{}_{}.root".format(inputdir,proc,year)) for proc in (list(set(sum(map(lambda x:x.keys(),bginfo.values()),[])))+["data"]) }
    other_files = {}
    for y in other_years:
        other_files[y] = { proc:uproot.open("{}/histos_{}_{}.root".format(inputdir,proc,y)) for proc in (list(set(sum(map(lambda x:x.keys(),bginfo.values()),[])))+["data"]) }
    # files = { proc:uproot.open("{}/histos_{}.root".format(inputdir,proc)) for proc in (list(set(sum(map(lambda x:x.keys(),bginfo.values()),[])))+["data"]) }

    # for which in ["htnb1mc","htnb1","os","osloose","br","crw","crz","tt_isr_reweight_check"]:
    for which in ["htnb1mc","htnb1","os","osloose","br","crw","crz"]:
    # for which in ["os","osloose"]:
    # for which in ["btagcr","btagcr_unw", "btagcr_njrw"]:
    # for which in [
    #         "os", # have _btagup/dn
    #         "os_njetreweight", # have _btagup/dn
    #         "os_noisr",
    #         "os_btagno",
    #         # "os_stop",
    #         # "os_njetreweight_btagup2",
    #         # "os_njetreweight_btagdn2",
    #         ]:

    # for which in ["btagcr_unw"]:
        # title = "CRW ({})".format(year) if which == "crw" else "NB==1, all HT ({})".format(year)
        title = which.upper()
        for var, xlabel in labels.items():
            for flav in ["ee","em","mm","in"]:
            # for flav in ["ee","mm","in"]:
            # for flav in ["em","in"]:

                hname = "{}_{}_{}".format(which,var,flav)

                # htnb1mc is htnb1 but with ttbar MC fakes instead of data-driven
                # we're using the original htnb1 histograms though, so fix hname
                if which == "htnb1mc":
                    hname = hname.replace("htnb1mc","htnb1")

                if other_years:
                    bgs = [
                            sum([Hist1D(files[proc][hname],label=label,color=color)] + [Hist1D(other_files[y][proc][hname],label=label,color=color) for y in other_years])
                            for proc,(label,color) in bginfo[which].items()
                            ]
                    data = sum([Hist1D(files["data"][hname])] + [Hist1D(other_files[y]["data"][hname]) for y in other_years])
                else:
                    bgs = [Hist1D(files[proc][hname], label=label,color=color) for proc,(label,color) in bginfo[which].items()]
                    data = Hist1D(files["data"][hname])
                bgs = sorted(bgs, key=lambda bg: bg.get_integral())
                data.set_attr("label", "Data [{}]".format(int(data.get_integral())))
                if data.get_integral() < 1e-6: continue
                if abs(sum(bgs).get_integral()) < 1e-6: continue

                do_bkg_syst = False

                if other_years:
                    fname = "{}/run2_{}_{}_{}.png".format(outputdir,which,var,flav)
                else:
                    fname = "{}/y{}_{}_{}_{}.png".format(outputdir,year,which,var,flav)
                # _, _, bin_coords = 
                plot_stack(bgs=bgs, data=data, title=title, xlabel=xlabel, filename=fname,
                           cms_type = "Preliminary",
                           lumi = lumi,
                           # return_bin_coordinates=True,
                           ratio_range=[0.0,2.0],
                           # ratio_range=[0.5,1.5],
                           do_bkg_syst = do_bkg_syst,
                           # ratio = ratio,
                           # ratio_range=[0.0,1.0],
                           # ratio = hpurity,
                           # mpl_ratio_params = {
                           #     "label": "prompt purity: {:.2f}".format(fpurity),
                           #     "color": "C3",
                           #     },
                           )
                # os.system("ic {}".format(fname))
                table_info = write_table(data,bgs,outname=fname.replace(".png",".txt"))
                # bin_json[fname.split("/")[-1].split(".")[0]] = get_info(bin_coords,table_info)
                print "Wrote {}".format(fname)

    return bin_json

if __name__ == "__main__":

    # inputdir = "outputs2017"
    # outputdir = "plots"

    # for val in [10,12,14,16,20]:
    #     inputdir = "outputs_2017_ptfake{}".format(val)
    #     outputdir = "plots_ptfake{}".format(val)
    #     total_bin_json = {}
    #     total_bin_json.update(make_plots(
    #             outputdir=outputdir,
    #             inputdir=inputdir,
    #             year=2017,
    #             lumi="41.5",
    #             ))

    # # inputdir = "outputs_2017_all"
    # inputdir = "outputs_2017_njetreweight"
    # outputdir = "plots_njetreweight"
    # total_bin_json = {}
    # total_bin_json.update(make_plots(
    #         outputdir=outputdir,
    #         inputdir=inputdir,
    #         year=2017,
    #         lumi="41.5",
    #         ))

    # 2018 alone
    inputdir = "outputs_2018"
    outputdir = "plots_2018"
    make_plots(
            outputdir=outputdir,
            inputdir=inputdir,
            year=2018,
            lumi="35.5",
            )
    outputdir = "plots_20172018"
    # 2018 + 2017
    make_plots(
            outputdir=outputdir,
            inputdir=inputdir,
            year=2018,
            lumi="77.1", # 2017+2017 --> 41.53+35.53 = 77.06
            other_years = [2017],
            )

    # total_bin_json.update(make_plots(
    #         outputdir=outputdir,
    #         inputdir=inputdir,
    #         year=2016,
    #         lumi="35.9",
    #         ))
    # json.dump(total_bin_json, open("{}/binInfo.json".format(outputdir),"w"))
