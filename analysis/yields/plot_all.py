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
        "ht"              : [("ttzcr","ttwcr","sr","br","brpostfit"), "ht"],
        "met"             : [("ttzcr","ttwcr","sr","br","brpostfit"), "met"],
        "mtmin"           : [("ttzcr","ttwcr","sr","br","brpostfit"), "mtmin"],
        "mll"             : [("ttzcr","ttwcr","sr","br","brpostfit"), "mll"],
        "njets"           : [("ttzcr","ttwcr","sr","br","brpostfit"), "njets"],
        "nbtags"          : [("ttzcr","ttwcr","sr","br","brpostfit"), "nbtags"],
        "type"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "type"],
        "charge"          : [("ttzcr","ttwcr","sr","br","brpostfit"), "charge"],
        "nleps"           : [("ttzcr","ttwcr","sr","br","brpostfit"), "nleps"],
        "l1pt"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "l1pt"],
        "l2pt"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "l2pt"],
        "disc"            : [("br","brpostfit"), "disc"],
        "l3pt"            : [("ttzcr","sr","br","brpostfit"), "l3pt"],
        "mllos"           : [("ttzcr",), "mllos"],
        "lepsf"            : [("br",), "lepsf"],
        "btagsf"            : [("br",), "btagsf"],
        "trigsf"            : [("br",), "trigsf"],
        "isrw"            : [("br",), "isrw"],
        "puw"            : [("br",), "puw"],
        "wsf"            : [("br",), "wsf"],
        # "ht_sr1"            : [("sr",), "ht_sr1"],
        # "ht_sr2"            : [("sr",), "ht_sr2"],
        # "ht_sr4"            : [("sr",), "ht_sr4"],
        # "ht_sr5"            : [("sr",), "ht_sr5"],
        # "ht_sr11"            : [("sr",), "ht_sr11"],

        # "mvis"            : [("ttzcr","ttwcr","sr","br"), "mvis"],
        # "mtvis"           : [("ttzcr","ttwcr","sr","br"), "mtvis"],
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
        # "bjetpt"          : [("sr","br"), "bjetpt"],
        # "jetpt"           : [("sr","br"), "jetpt"],
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

# from python getPostFit.py 
# after doing testcrbins.py to reduce bins (3 bins from crwsplit)
# and run_all_limits.sh with extra containing --unblinded
d_crpostfitsf = {'fakes': 1.1948199139074693,
        'flips': 1.020599049200507,
        'rares': 1.1224350665249947,
        'total': 1.2794573639702325,
        'total_background': 1.3056080457107933,
        'total_signal': 0.0,
        'tth': 1.155083830402133,
        'tttt': 0.0,
        'ttvv': 1.0550142330872638,
        'ttw': 1.4807971683302559,
        'ttz': 1.3958306982737914,
        'xg': 1.0842022472153388}
d_crpostfitsf_errs = {'fakes': 0.5330058353969307,
        'flips': 0.14395684087751284,
        'rares': 0.3343497393873837,
        'total': 0.14901268940160062,
        'total_background': 0.1538191984828496,
        'total_signal': 0.0,
        'tth': 0.30185266795309806,
        'tttt': 0.0,
        'ttvv': 0.28627852193747866,
        'ttw': 0.28537781734630363,
        'ttz': 0.2800949349793666,
        'xg': 0.3090238224979612}

# yuck, gotta make these global for multiprocessing since uproot file objects can't be pickled
files = {}

def worker(info):
    global files

    outputdir, year, lumi, (var, (regions, xlabel)) = info

    fnames = []
    for region in regions:

        title = region.upper()

        region_for_hist = region[:]
        if region == "brpostfit":
            region_for_hist = "br"
            def get_sf(proc):
                return d_crpostfitsf.get(proc,1.0)
            bgs = [
                    sum(
                        get_sf(proc)*Hist1D(files[y][proc]["{}_{}_{}".format(region_for_hist,var,proc)],
                            label="{} ($\\times${:.2f})".format(label,get_sf(proc)),
                            color=color
                            ) for y in files.keys())
                        for proc,(label,color) in d_label_colors.items()
                        ]
        else:
            bgs = [
                    sum(Hist1D(files[y][proc]["{}_{}_{}".format(region_for_hist,var,proc)],label=label,color=color) for y in files.keys())
                    for proc,(label,color) in d_label_colors.items()
                    ]

        data = sum([Hist1D(files[y]["data"]["{}_{}_data".format(region_for_hist,var)]) for y in files.keys()])
        sig = sum([Hist1D(files[y]["tttt"]["{}_{}_tttt".format(region_for_hist,var)],color="r") for y in files.keys()])
        sig.set_attr("label", r"$t\bar{t}t\bar{t}$")
        bgs = sorted(bgs, key=lambda bg: bg.get_integral())
        data.set_attr("label", "Data [{}]".format(int(data.get_integral())))

        # if data.get_integral() < 1e-6: return
        if abs(sum(bgs).get_integral()) < 1e-6: return

        ax_main_callback = None
        xticks = []

        if region.lower() in ["sr","srdisc"]:
            data._counts *= 0.
            data._errors *= 0.
            data.set_attr("label", "Data (blind)")
            if var.lower() in ["total"]:
                xticks = range(1,20)
            if region.lower() in ["srdisc"]:
                # def ax_main_callback(ax):
                #     ax.set_ylim([0.1,ax.get_ylim()[1]*1.5])
                #     ax.set_yscale("log", nonposy='clip'),
                xticks = ["CRZ"]+range(1,25)
        if (region.lower() in ["srcr"]) and (var.lower() in ["total"]):
            data._counts[2:] *= 0.
            data._errors[2:] *= 0.
            data.set_attr("label", "Data [{}]".format(int(data.get_integral())))
            def ax_main_callback(ax):
                ax.set_ylim([0.1,ax.get_ylim()[1]*1.5])
                ax.set_yscale("log", nonposy='clip'),
            xticks = ["CRZ","CRW"]+range(1,20)
        if (var.lower() in ["disc"]):
            data._counts[-10:] *= 0.
            data._errors[-10:] *= 0.
            data.set_attr("label", "Data [{}]".format(int(data.get_integral())))

        if len(files.keys()) > 1:
            fname = "{}/run2_{}_{}.png".format(outputdir,region,var)
        else:
            fname = "{}/{}_{}_{}.png".format(files.keys()[0],outputdir,region,var)
        fnames.append(fname)
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
    return ", ".join(fnames)

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
    # print infos
    for res in pool.imap_unordered(worker,infos):
        if res:
            print "Wrote {}".format(res)


if __name__ == "__main__":


    make_plots(
            outputdir="plots_temp",
            inputdir="outputs",
            year=2018,
            lumi="124.0", # 2016+2017+2018 --> 35.87+41.53+46.57 = 124.0
            other_years = [2016,2017],
            )

