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


labels = {}

labels["ft"] = {


        "TOTAL"        : [("SRCR","SR","SRDISC"), "TOTAL"],
        "ht"              : [("ttzcr","ttwcr","sr","br","brpostfit"), "$H_{T}$"],
        "met"             : [("ttzcr","ttwcr","sr","br","brpostfit"), MET_LATEX],
        "mtmin"           : [("ttzcr","ttwcr","sr","br","brpostfit"), "mtmin"],
        "mll"             : [("ttzcr","ttwcr","sr","br","brpostfit"), "mll"],
        "njets"           : [("ttzcr","ttwcr","sr","br","brpostfit"), "Njets"],
        "nbtags"          : [("ttzcr","ttwcr","sr","br","brpostfit"), "Nbtags"],
        "type"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "type (mm, em, ee)"],
        "type3"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "type3 (mmm, mme, mee, eee)"],
        "charge"          : [("ttzcr","ttwcr","sr","br","brpostfit"), "charge"],
        "nleps"           : [("ttzcr","ttwcr","sr","br","brpostfit"), "nleps"],
        "l1pt"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "l1pt"],
        "l2pt"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "l2pt"],
        "l3pt"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "l3pt"],
        "disc"            : [("br","brpostfit","ttwcr","ttzcr"), "disc"],
        "l3pt"            : [("ttzcr","sr","br","brpostfit"), "l3pt"],
        "mllos"           : [("ttzcr",), "mllos"],

        "el_l1pt"         : [("sr","br"), "el_l1pt"],
        "el_l2pt"         : [("sr","br"), "el_l2pt"],
        "el_l1eta"        : [("sr","br"), "el_l1eta"],
        "el_l2eta"        : [("sr","br"), "el_l2eta"],
        "el_l1phi"        : [("sr","br"), "el_l1phi"],
        "el_l2phi"        : [("sr","br"), "el_l2phi"],

        "mu_l1pt"         : [("sr","br"), "mu_l1pt"],
        "mu_l2pt"         : [("sr","br"), "mu_l2pt"],
        "mu_l1eta"        : [("sr","br"), "mu_l1eta"],
        "mu_l2eta"        : [("sr","br"), "mu_l2eta"],
        "mu_l1phi"        : [("sr","br"), "mu_l1phi"],
        "mu_l2phi"        : [("sr","br"), "mu_l2phi"],


        # "lepsf"            : [("br",), "lepsf"],
        # "btagsf"            : [("br",), "btagsf"],
        # "trigsf"            : [("br",), "trigsf"],
        # "isrw"            : [("br",), "isrw"],
        # "puw"            : [("br",), "puw"],
        # "wsf"            : [("br",), "wsf"],

        # "mvis"            : [("ttzcr","ttwcr","sr","br"), "mvis"],
        # "mtvis"           : [("ttzcr","ttwcr","sr","br"), "mtvis"],
        # "mtop1"           : [("sr",), "mtop1"],
        # "mtop2"           : [("sr",), "mtop2"],
        # "sip3d_mu_lep1"   : [("sr","br"), "sip3d_mu_lep1"],
        # "sip3d_mu_lep2"   : [("sr","br"), "sip3d_mu_lep2"],
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

labels["ss"] = {

        "TOTAL"      : [("SRHH","SRHL","SRLL","SRML","SRLM"), "TOTAL"],
        "category"   : [("sr",                     ), r"HH,HL,LL,MLoffZ,MLonZ"],
        "ht"         : [("br","ml","mlonz","mloffz","hh","lm"), r"$H_T$"],
        "met"        : [("br","ml","mlonz","mloffz","hh","lm"), MET_LATEX],
        "mll"        : [("br","ml","mlonz","mloffz","hh","lm"), r"$m_{ll}$"],
        "mllos"      : [("br","ml","mlonz","mloffz","hh","lm"), r"$m_{ll}$(OS)"],
        "njets"      : [("br","ml","mlonz","mloffz","hh","lm"), r"$N_\mathrm{jets}$"],
        "nbtags"     : [("br","ml","mlonz","mloffz","hh","lm"), r"$N_\mathrm{btags}$"],
        "type"       : [("br","ml","mlonz","mloffz","hh","lm"), r"$\mu\mu,\mu e,e\mu,ee$"],
        "type3"      : [("br","ml","mlonz","mloffz","hh","lm"), r"$\mu\mu\mu,\mu\mu e,\mu ee,eee$"],
        "charge"     : [("br","ml","mlonz","mloffz","hh","lm"), r"SS charge"],
        "q3"         : [("br","ml","mlonz","mloffz","hh","lm"), r"lep 3 charge"],
        "charge3"    : [("br","ml","mlonz","mloffz","hh","lm"), r"$\pm\pm\pm$, $\pm\pm\mp$"],
        "nleps"      : [("br","ml","mlonz","mloffz","hh","lm"), r"$N_\mathrm{leps}$"],
        "l1pt"       : [("br","ml","mlonz","mloffz","hh","lm"), r"ordered $p_{T}$(lep1)"],
        "l2pt"       : [("br","ml","mlonz","mloffz","hh","lm"), r"ordered $p_{T}$(lep2)"],
        "l3pt"       : [("br","ml","mlonz","mloffz","hh","lm"), r"ordered $p_{T}$(lep3)"],
        "mt2"        : [("br","ml","mlonz","mloffz","hh","lm"), r"$m_{T}^{2}$"],
        "mt2min"     : [("br","ml","mlonz","mloffz","hh","lm"), r"$m_{T}^{2,\mathrm{min}}$"],
        "mtmin"      : [("br","ml","mlonz","mloffz","hh","lm"), r"$m_{T}^\mathrm{min}$"],
        "el_l1pt"    : [("br","ml","mlonz","mloffz","hh","lm"), r"unsorted $p_{T}$(lep1, e)"],
        "el_l2pt"    : [("br","ml","mlonz","mloffz","hh","lm"), r"unsorted $p_{T}$(lep2, e)"],
        "el_l3pt"    : [("br","ml","mlonz","mloffz","hh","lm"), r"unsorted $p_{T}$(lep3, e)"],
        "el_l1eta"   : [("br","ml","mlonz","mloffz","hh","lm"), r"unsorted $\eta $(lep1, e)"],
        "el_l2eta"   : [("br","ml","mlonz","mloffz","hh","lm"), r"unsorted $\eta $(lep2, e)"],
        "el_l3eta"   : [("br","ml","mlonz","mloffz","hh","lm"), r"unsorted $\eta $(lep3, e)"],
        "mu_l1pt"    : [("br","ml","mlonz","mloffz","hh","lm"), r"unsorted $p_{T}$(lep1, \mu)"],
        "mu_l2pt"    : [("br","ml","mlonz","mloffz","hh","lm"), r"unsorted $p_{T}$(lep2, \mu)"],
        "mu_l3pt"    : [("br","ml","mlonz","mloffz","hh","lm"), r"unsorted $p_{T}$(lep3, \mu)"],
        "mu_l1eta"   : [("br","ml","mlonz","mloffz","hh","lm"), r"unsorted $\eta $(lep1, \mu)"],
        "mu_l2eta"   : [("br","ml","mlonz","mloffz","hh","lm"), r"unsorted $\eta $(lep2, \mu)"],
        "mu_l3eta"   : [("br","ml","mlonz","mloffz","hh","lm"), r"unsorted $\eta $(lep3, \mu)"],

}

d_label_colors = {}
d_label_colors["ft"] = {
        "fakes" : (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
        # "fakes_mc" : (r"$t\bar{t}$", [0.85, 0.85, 0.85]),
        "flips" : (r"Charge misid.", [0.4, 0.4, 0.4]),
        "rares" : (r"Rare", [1.0, 0.4, 1.0]),
        "tth"   : (r"$t\bar{t}H$", [0.4, 0.4, 0.6]),
        "ttvv"  : (r"$t\bar{t}VV$" , [0.4, 0.6, 1.0]),
        "ttw"   : (r"$t\bar{t}W$", [0.0, 0.4, 0.0]),
        "ttz"   : (r"$t\bar{t}Z$", [0.4, 0.8, 0.4]),
        "xg"    : (r"$X\gamma$" , [0.4, 0.0, 0.8]),
        }
d_label_colors["ss"] = {
        "fakes" : (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
        # "fakes_mc" : (r"$t\bar{t}$", [0.85, 0.85, 0.85]),
        "flips" : (r"Charge misid.", [0.4, 0.4, 0.4]),
        "rares" : (r"Rare", [1.0, 0.4, 1.0]),
        "tth"   : (r"$t\bar{t}H$", [0.4, 0.4, 0.6]),
        "wz"  : (r"WZ" , [1.0,0.8,0.0]),
        "ww"  : (r"WW" , [1.0,0.6,0.0]),
        "ttw"   : (r"$t\bar{t}W$", [0.0, 0.4, 0.0]),
        "ttz"   : (r"$t\bar{t}Z$", [0.4, 0.8, 0.4]),
        "xg"    : (r"$X\gamma$" , [0.4, 0.0, 0.8]),
        }

d_flat_systematics = { }
d_flat_systematics["ft"] = {
        "fakes": 0.3,
        "flips": 0.2,
        "rares": 0.2,
        "ttw": 0.4,
        "ttz": 0.4,
        "tth": 0.25,
        "ttvv": 0.11,
        "xg": 0.11,
        }
d_flat_systematics["ss"] = {
        "fakes": 0.3,
        "flips": 0.2,
        "rares": 0.5,
        "ww": 0.2,
        "ttw": 0.13,
        "ttz": 0.3,
        "wz": 0.13,
        "tth": 0.3,
        "xg": 0.5,
        }
# "fakes/ttZ/ttH (30%); flips/WW (20%); WZ/ttW (13%); Xg/Rares (50%)"

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
signame_ = None

def worker(info):
    global files, signame_

    analysis, outputdir, year, lumi, (var, (regions, xlabel)) = info

    if signame_ == "tttt":
        sigstr = r"$t\bar{t}t\bar{t}$"
    else:
        m1,m2 = map(int,signame_.split("_m")[1:])
        modelname = signame_.split("_m")[0].replace("fs_","").capitalize().replace("ww","WW")
        sigstr = r"{} ({},{}) $\times 10$".format(modelname,m1,m2)

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
                            color=color,
                            dummy=proc,
                            ) for y in files.keys())
                        for proc,(label,color) in d_label_colors[analysis].items()
                        ]
        else:
            bgs = [
                    sum(Hist1D(files[y][proc]["{}_{}_{}".format(region_for_hist,var,proc)],label=label,color=color,dummy=proc) for y in files.keys())
                    for proc,(label,color) in d_label_colors[analysis].items()
                    ]

        data = sum([Hist1D(files[y]["data"]["{}_{}_data".format(region_for_hist,var)]) for y in files.keys()])
        sig = sum([Hist1D(files[y][signame_]["{}_{}_{}".format(region_for_hist,var,signame_)],color="r") for y in files.keys()])
        sig.set_attr("label", sigstr)
        if signame_ != "tttt":
            sig *= 10.
        bgs = sorted(bgs, key=lambda bg: bg.get_integral())
        for bg in bgs:
            # add flat systematic to stat unc in quadrature
            bg._errors = np.hypot(d_flat_systematics[analysis].get(bg.get_attr("dummy"),0.),bg._errors)

        data.set_attr("label", "Data [{}]".format(int(data.get_integral())))

        # if data.get_integral() < 1e-6: return
        if abs(sum(bgs).get_integral()) < 1e-6: continue

        ax_main_callback = None
        ax_ratio_callback = None
        mpl_legend_params = {}
        ratio_range = [0.,2.]
        xticks = []

        if region in ["SRHH","SRHL","SRLL","SRML","SRLM"] and (var == "TOTAL"):
            ratio_range = [0.,3.]
            mpl_legend_params["fontsize"] = 9
            mpl_legend_params["framealpha"] = 0.5
            mpl_legend_params["ncol"] = 2
            mpl_legend_params["labelspacing"] = 0.15

            data.convert_to_poisson_errors()

            if year != 2016:
                data._counts *= 0.
                data._errors *= 0.
                data.set_attr("label", "Data (blind)")

            def ax_main_callback(ax):
                ax.set_ylim([0.05,ax.get_ylim()[1]*2.0])
                ax.set_yscale("log", nonposy='clip'),

            if region in ["SRML","SRLM","SRLL","SRHL","SRHH"]:
                xticks = range(1,70)
            # if region in ["SRML"]:
                # xticks = []
                # xticks += ["{}  ".format(x) if x not in [1,2,3,4,14,15,16] else ("{}A".format(x)) for x in range(1,16+1)]+["1B","2B","3B","4B","14B","15B","16B"]
                # xticks += ["{}  ".format(x) if x not in [1,2,3,4,14,15,16] else ("{}A".format(x)) for x in range(1,16+1)]+["1B","2B","3B","4B","14B","15B","16B"]
                # def ax_ratio_callback(ax):
                #     ax.text(11,2.35,"off-Z", color="blue", ha="center", va="center", fontsize=10.0, wrap=True)
                #     ax.text(10+23,2.35,"on-Z", color="blue", ha="center", va="center", fontsize=10.0, wrap=True)
                #     ax.axvline(x=23.5, color="blue", lw=1.5)

        elif region.lower() in ["sr","srdisc"]:
            # data._counts *= 0.
            # data._errors *= 0.
            data.set_attr("label", "Data (blind)")
            if var.lower() in ["total"]:
                xticks = range(1,20)
            if region.lower() in ["srdisc"]:
                # def ax_main_callback(ax):
                #     ax.set_ylim([0.1,ax.get_ylim()[1]*1.5])
                #     ax.set_yscale("log", nonposy='clip'),
                xticks = ["CRZ"]+range(1,25)

        if (region.lower() in ["srcr"]) and (var.lower() in ["total"]):
            # data._counts[2:] *= 0.
            # data._errors[2:] *= 0.
            data.set_attr("label", "Data [{}]".format(int(data.get_integral())))
            def ax_main_callback(ax):
                ax.set_ylim([0.1,ax.get_ylim()[1]*1.5])
                ax.set_yscale("log", nonposy='clip'),
            xticks = ["CRZ","CRW"]+range(1,20)
        if (var.lower() in ["disc"]) and (region.lower() not in ["ttwcr","ttzcr"]):
            data._counts[-10:] *= 0.
            data._errors[-10:] *= 0.
            data.set_attr("label", "Data [{}]".format(int(data.get_integral())))

        if len(files.keys()) > 1:
            fname = "{}/run2_{}_{}.pdf".format(outputdir,region,var)
        else:
            fname = "{}/y{}_{}_{}.pdf".format(outputdir,files.keys()[0],region,var)
        fnames.append(fname)
        plot_stack(bgs=bgs, data=data, title=title, xlabel=xlabel, filename=fname,
                   cms_type = "Preliminary",
                   lumi = lumi,
                   ratio_range=ratio_range,
                   sigs=[sig],
                   xticks=xticks,
                   mpl_sig_params={
                       # "hist":False,
                       },
                   ax_main_callback=ax_main_callback,
                   mpl_legend_params=mpl_legend_params,
                   ax_ratio_callback=ax_ratio_callback,
                   do_bkg_syst=True,
                   )
        # os.system("ic {}".format(fname))

        # print bgs
        # print data
        # return

        table_info = write_table(data,bgs,signal=sig,outname=fname.replace(".pdf",".txt"))
        # table_info = write_table(data,bgs,signal=sig,outname=fname.replace(".pdf",".txt"),
        #         binlabels=xticks,signame=sigstr.replace(r"$\times 10$","x10").replace(","," "),csv=True)
    return ", ".join(fnames)

def make_plots(outputdir="plots", inputdir="outputs", year=2017, lumi="41.5", other_years=[], regions=[], signame="tttt", doss=False, show_mcfakes=False):
    global files, signame_, other_files
    signame_ = signame

    os.system("mkdir -p {}/".format(outputdir))

    analysis = "ft"
    if doss: analysis = "ss"

    if show_mcfakes and ("fakes" in d_label_colors[analysis]):
        del d_label_colors[analysis]["fakes"]
        d_label_colors[analysis]["fakes_mc"] = (r"$t\bar{t}$", [0.85, 0.85, 0.85])

    files = {}
    for y in [year]+other_years:
        files[y] = { }
        for proc in d_label_colors[analysis].keys()+["data",signame_]:
            # try:
            ystr = str(y)
            if "fs_" in proc: ystr = "2016"
            files[y][proc] = uproot.open("{}/output_{}_{}.root".format(inputdir,ystr,proc))
            # except IOError:
            #     print("{}/output_{}_{}.root doesn't exist, but ignoring because it's probably signal".format(inputdir,y,proc))


    # print files

    # # for region in ["htnb1mc","htnb1","os","osloose","br","crw","crz","tt_isr_reweight_check"]:
    # # regions = ["htnb1mc","htnb1","htnb1mcmu","htnb1mu","os","os_noht","osloose","br","crw","crz"]
    # regions = regions or ["htnb1mc","htnb1","os","sshh","sshhmc","osloose","br","crw","crz"]
    # flavs = ["ee","em","mm","in"]
    # varss = labels_ft.keys()
    infos = [[analysis,outputdir,year,lumi,x] for x in labels[analysis].items()]
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

