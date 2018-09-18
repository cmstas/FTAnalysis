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

        "TOTAL"        : [("SRCR","SR","SRDISC"), "TOTAL"],

        "ht"              : [("ttzcr","ttwcr","sr","br"), "ht"],
        "met"             : [("ttzcr","ttwcr","sr","br"), "met"],
        # "mvis"            : [("ttzcr","ttwcr","sr","br"), "mvis"],
        # "mtvis"           : [("ttzcr","ttwcr","sr","br"), "mtvis"],
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
        # "mtop1"           : [("sr",), "mtop1"],
        # "mtop2"           : [("sr",), "mtop2"],
        # "sip3d_mu_lep1"   : [("sr","br"), "sip3d_mu_lep1"],
        # "sip3d_mu_lep2"   : [("sr","br"), "sip3d_mu_lep2"],
        # "mu_l1pt"         : [("sr","br"), "mu_l1pt"],
        # "mu_l2pt"         : [("sr","br"), "mu_l2pt"],
        # "mu_l3pt"         : [("sr","br"), "mu_l3pt"],
        # "l1eta_mu"        : [("sr","br"), "l1eta_mu"],
        # "l2eta_mu"        : [("sr","br"), "l2eta_mu"],
        # "lep1_mu_miniIso" : [("sr","br"), "lep1_mu_miniIso"],
        # "lep2_mu_miniIso" : [("sr","br"), "lep2_mu_miniIso"],
        # "lep1_mu_ptRel"   : [("sr","br"), "lep1_mu_ptRel"],
        # "lep2_mu_ptRel"   : [("sr","br"), "lep2_mu_ptRel"],
        # "lep1_mu_ptRelfail"   : [("sr","br"), "lep1_mu_ptRelfail"],
        # "lep2_mu_ptRelfail"   : [("sr","br"), "lep2_mu_ptRelfail"],
        # "lep1_mu_ptRatio"   : [("sr","br"), "lep1_mu_ptRatio"],
        # "lep2_mu_ptRatio"   : [("sr","br"), "lep2_mu_ptRatio"],
        # "sip3d_el_lep1"   : [("sr","br"), "sip3d_el_lep1"],
        # "sip3d_el_lep2"   : [("sr","br"), "sip3d_el_lep2"],
        # "el_l1pt"         : [("sr","br"), "el_l1pt"],
        # "el_l2pt"         : [("sr","br"), "el_l2pt"],
        # "el_l3pt"         : [("sr","br"), "el_l3pt"],
        # "l1eta_el"        : [("sr","br"), "l1eta_el"],
        # "l2eta_el"        : [("sr","br"), "l2eta_el"],
        # "lep1_el_miniIso" : [("sr","br"), "lep1_el_miniIso"],
        # "lep2_el_miniIso" : [("sr","br"), "lep2_el_miniIso"],
        # "lep1_el_ptRel"   : [("sr","br"), "lep1_el_ptRel"],
        # "lep2_el_ptRel"   : [("sr","br"), "lep2_el_ptRel"],
        # "lep1_el_ptRelfail"   : [("sr","br"), "lep1_el_ptRelfail"],
        # "lep2_el_ptRelfail"   : [("sr","br"), "lep2_el_ptRelfail"],
        # "lep1_el_ptRatio"   : [("sr","br"), "lep1_el_ptRatio"],
        # "lep2_el_ptRatio"   : [("sr","br"), "lep2_el_ptRatio"],
        "bjetpt"          : [("sr","br"), "bjetpt"],
        "jetpt"           : [("sr","br"), "jetpt"],
}

d_label_colors = {
        "fakes" : (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
        "flips" : (r"Charge misid.", [0.4, 0.4, 0.4]),
        "rares" : (r"Rare", [1.0, 0.4, 1.0]),
        "tth"   : (r"$t\bar{t}H$", [0.4, 0.4, 0.6]),
        "ttvv"  : (r"$t\bar{t}VV$" , [0.4, 0.6, 1.0]),
        "ttw"   : (r"$t\bar{t}W$", [0.0, 0.4, 0.0]),
        "ttz"   : (r"$t\bar{t}Z$", [0.4, 0.8, 0.4]),
        "xg"    : (r"$X\gamma$" , [0.4, 0.0, 0.8]),
        }

# yuck, gotta make these global for multiprocessing since uproot file objects can't be pickled
files = {}

def worker(info):
    global files

    outputdir, year, lumi, (var, (regions, xlabel)) = info

    for region in regions:
        title = region.upper()

        bgs = [
                sum(Hist1D(files[y][proc]["{}_{}_{}".format(region,var,proc)],label=label,color=color) for y in files.keys())
                for proc,(label,color) in d_label_colors.items()
                ]
        data = sum([Hist1D(files[y]["data"]["{}_{}_data".format(region,var)]) for y in files.keys()])
        sig = sum([Hist1D(files[y]["tttt"]["{}_{}_tttt".format(region,var)],color="r") for y in files.keys()])
        sig.set_attr("label", r"$t\bar{t}t\bar{t}$")
        # for y in files.keys():
            # print y,Hist1D(files[y]["data"]["{}_{}_data".format(region,var)])
        bgs = sorted(bgs, key=lambda bg: bg.get_integral())
        data.set_attr("label", "Data [{}]".format(int(data.get_integral())))

        if data.get_integral() < 1e-6: return
        if abs(sum(bgs).get_integral()) < 1e-6: return

        ax_main_callback = None
        xticks = []

        if region.lower() in ["sr","srdisc"]:
            data._counts *= 0.
            data._errors *= 0.
            data.set_attr("label", "Data (blind)")
            xticks = range(1,17)
        if region.lower() in ["srcr"]:
            data._counts[2:] *= 0.
            data._errors[2:] *= 0.
            data.set_attr("label", "Data [{}]".format(int(data.get_integral())))
            def ax_main_callback(ax):
                ax.set_ylim([0.1,ax.get_ylim()[1]*1.5])
                ax.set_yscale("log", nonposy='clip'),
            xticks = ["CRW","CRZ"]+range(1,17)

        if len(files.keys()) > 1:
            fname = "{}/run2_{}_{}.png".format(outputdir,region,var)
        else:
            fname = "{}/{}_{}_{}.png".format(files.keys()[0],outputdir,region,var)
        plot_stack(bgs=bgs, data=data, title=title, xlabel=xlabel, filename=fname,
                   cms_type = "Preliminary",
                   lumi = lumi,
                   ratio_range=[0.0,2.0],
                   sigs=[sig],
                   xticks=xticks,
                   mpl_sig_params={
                       # "hist":False,
                       },
                   ax_main_callback=ax_main_callback,
                    # mpl_legend_params={
                    #     "framealpha": 0.4,
                    #     "ncol": 2,
                    #     "labelspacing": 0.5,
                    #     "columnspacing": 0.5,
                    #     },
                    # do_bkg_syst=True,
                   )
        # os.system("ic {}".format(fname))

        # print bgs
        # print data
        # return

        table_info = write_table(data,bgs,signal=sig,outname=fname.replace(".png",".txt"))
    return fname

def make_plots(outputdir="plots", inputdir="outputs", year=2017, lumi="41.5", other_years=[], regions=[]):
    global files, other_files

    os.system("mkdir -p {}/".format(outputdir))

    files = {}
    for y in [year]+other_years:
        files[y] = { proc:uproot.open("{}/output_{}_{}.root".format(inputdir,y,proc)) for proc in d_label_colors.keys()+["data","tttt"] }

    # print files

    # # for region in ["htnb1mc","htnb1","os","osloose","br","crw","crz","tt_isr_reweight_check"]:
    # # regions = ["htnb1mc","htnb1","htnb1mcmu","htnb1mu","os","os_noht","osloose","br","crw","crz"]
    # regions = regions or ["htnb1mc","htnb1","os","sshh","sshhmc","osloose","br","crw","crz"]
    # flavs = ["ee","em","mm","in"]
    # varss = labels.keys()
    infos = [[outputdir,year,lumi,x] for x in labels.items()]
    # print infos

    # map(worker,infos)

    os.nice(10)
    from multiprocessing import Pool as ThreadPool
    pool = ThreadPool(25)
    for res in pool.imap_unordered(worker,infos):
        if res:
            print "Wrote {}".format(res)


if __name__ == "__main__":

    make_plots(
            outputdir="plots_temp",
            inputdir="outputs",
            year=2018,
            lumi="112.9", # 2016+2017+2018 --> 35.87+41.53+35.53 = 112.9
            other_years = [2016,2017],
            )

