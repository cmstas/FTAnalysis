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

        "TOTAL"      : [("SRHH","SRHL","SRLL","SRML"), "TOTAL"],
        "category"   : [("sr",                     ), r"HH,HL,LL,MLoffZ,MLonZ"],
        "ht"         : [("br","ml","mlonz","mloffz"), r"$H_T$"],
        "met"        : [("br","ml","mlonz","mloffz"), MET_LATEX],
        "mll"        : [("br","ml","mlonz","mloffz"), r"$m_{ll}$"],
        "mllos"      : [("br","ml","mlonz","mloffz"), r"$m_{ll}$(OS)"],
        "njets"      : [("br","ml","mlonz","mloffz"), r"$N_\mathrm{jets}$"],
        "nbtags"     : [("br","ml","mlonz","mloffz"), r"$N_\mathrm{btags}$"],
        "type"       : [("br","ml","mlonz","mloffz"), r"$\mu\mu,\mu e,e\mu,ee$"],
        "type3"      : [("br","ml","mlonz","mloffz"), r"$\mu\mu\mu,\mu\mu e,\mu ee,eee$"],
        "charge"     : [("br","ml","mlonz","mloffz"), r"SS charge"],
        "q3"         : [("br","ml","mlonz","mloffz"), r"lep 3 charge"],
        "charge3"    : [("br","ml","mlonz","mloffz"), r"$\pm\pm\pm$, $\pm\pm\mp$"],
        "nleps"      : [("br","ml","mlonz","mloffz"), r"$N_\mathrm{leps}$"],
        "l1pt"       : [("br","ml","mlonz","mloffz"), r"ordered $p_{T}$(lep1)"],
        "l2pt"       : [("br","ml","mlonz","mloffz"), r"ordered $p_{T}$(lep2)"],
        "l3pt"       : [("br","ml","mlonz","mloffz"), r"ordered $p_{T}$(lep3)"],
        "mt2"        : [("br","ml","mlonz","mloffz"), r"$m_{T}^{2}$"],
        "mt2min"     : [("br","ml","mlonz","mloffz"), r"$m_{T}^{2,\mathrm{min}}$"],
        "mtmin"      : [("br","ml","mlonz","mloffz"), r"$m_{T}^\mathrm{min}$"],
        # "mtmax"      : [("br","ml","mlonz","mloffz"), r"$m_{T}^\mathrm{max}$"],
        "twobinmt2"  : [("br","ml","mlonz","mloffz"), r"$m_{T}^{2}$"],
        "twobinmtmin": [("br","ml","mlonz","mloffz"), r"$m_{T}^\mathrm{min}$"],
        "nleptonicW" : [("br","ml","mlonz","mloffz"), r"$N_\mathrm{leptonicW}$"],
        "el_l1pt"    : [("br","ml","mlonz","mloffz"), r"unsorted $p_{T}$(lep1, e)"],
        "el_l2pt"    : [("br","ml","mlonz","mloffz"), r"unsorted $p_{T}$(lep2, e)"],
        "el_l3pt"    : [("br","ml","mlonz","mloffz"), r"unsorted $p_{T}$(lep3, e)"],
        "el_l1eta"   : [("br","ml","mlonz","mloffz"), r"unsorted $\eta $(lep1, e)"],
        "el_l2eta"   : [("br","ml","mlonz","mloffz"), r"unsorted $\eta $(lep2, e)"],
        "el_l3eta"   : [("br","ml","mlonz","mloffz"), r"unsorted $\eta $(lep3, e)"],
        "mu_l1pt"    : [("br","ml","mlonz","mloffz"), r"unsorted $p_{T}$(lep1, \mu)"],
        "mu_l2pt"    : [("br","ml","mlonz","mloffz"), r"unsorted $p_{T}$(lep2, \mu)"],
        "mu_l3pt"    : [("br","ml","mlonz","mloffz"), r"unsorted $p_{T}$(lep3, \mu)"],
        "mu_l1eta"   : [("br","ml","mlonz","mloffz"), r"unsorted $\eta $(lep1, \mu)"],
        "mu_l2eta"   : [("br","ml","mlonz","mloffz"), r"unsorted $\eta $(lep2, \mu)"],
        "mu_l3eta"   : [("br","ml","mlonz","mloffz"), r"unsorted $\eta $(lep3, \mu)"],

}

d_label_colors = {

        # "fakes" : (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
        # "fakes_mc" : (r"MC fakes", [0.75, 0.75, 0.75]),
        # "fakes_mchybrid" : (r"MC fakes", [0.75, 0.75, 0.75]),
        "ttph" : (r"$t\bar{t}$ (powheg)", [0.75, 0.75, 0.75]),
        # "ttmg" : (r"$t\bar{t}$ (MG)", [0.75, 0.75, 0.75]),

        "flips" : (r"Charge misid.", [0.4, 0.4, 0.4]),
        "rares" : (r"Rare", [1.0, 0.4, 1.0]),
        "tth"   : (r"$t\bar{t}H$", [0.4, 0.4, 0.6]),
        # "ttvv"  : (r"$t\bar{t}VV$" , [0.4, 0.6, 1.0]),
        "wz"  : (r"WZ" , [1.0,0.8,0.0]),
        "ww"  : (r"WW" , [1.0,0.6,0.0]),
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
signame_ = None

def worker(info):
    global files, signame_

    outputdir, year, lumi, (var, (regions, xlabel)) = info

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
                            color=color
                            ) for y in files.keys())
                        for proc,(label,color) in d_label_colors.items()
                        ]
        else:
            try:
                bgs = [
                        sum(Hist1D(files[y][proc]["{}_{}_{}".format(region_for_hist,var,proc)],label=label,color=color) for y in files.keys())
                        for proc,(label,color) in d_label_colors.items()
                        ]
            except:
                continue

        data = sum([Hist1D(files[y]["data"]["{}_{}_data".format(region_for_hist,var)]) for y in files.keys()])
        sig = sum([Hist1D(files[y][signame_]["{}_{}_{}".format(region_for_hist,var,signame_)],color="r") for y in files.keys()])
        sig.set_attr("label", sigstr)
        sig *= 10.
        bgs = sorted(bgs, key=lambda bg: bg.get_integral())
        data.set_attr("label", "Data [{}]".format(int(data.get_integral())))

        # if data.get_integral() < 1e-6: return
        if abs(sum(bgs).get_integral()) < 1e-6: continue

        ax_main_callback = None
        ax_ratio_callback = None
        xticks = []

        if region in ["SRHH","SRHL","SRLL","SRML"] and (var == "TOTAL"):
            # data._counts *= 0.
            # data._errors *= 0.
            # data.set_attr("label", "Data (blind)")
            # if var.lower() in ["total"]:
            #     xticks = range(1,20)
            # if region in ["SRHH","SRHL"]:

            def ax_main_callback(ax):
                ax.set_ylim([0.1,ax.get_ylim()[1]*2.0])
                ax.set_yscale("log", nonposy='clip'),

            if region in ["SRML"]:
                # xticks = range(1,50)
                xticks = []
                xticks += ["{}  ".format(x) if x not in [1,2,3,4,14,15,16] else ("{}A".format(x)) for x in range(1,16+1)]+["1B","2B","3B","4B","14B","15B","16B"]
                xticks += ["{}  ".format(x) if x not in [1,2,3,4,14,15,16] else ("{}A".format(x)) for x in range(1,16+1)]+["1B","2B","3B","4B","14B","15B","16B"]
                def ax_ratio_callback(ax):
                    ax.text(11,2.35,"off-Z", color="blue", ha="center", va="center", fontsize=10.0, wrap=True)
                    ax.text(10+23,2.35,"on-Z", color="blue", ha="center", va="center", fontsize=10.0, wrap=True)
                    ax.axvline(x=23.5, color="blue", lw=1.5)
        # if (region.lower() in ["srcr"]) and (var.lower() in ["total"]):
        #     data._counts[2:] *= 0.
        #     data._errors[2:] *= 0.
        #     data.set_attr("label", "Data [{}]".format(int(data.get_integral())))
        #     def ax_main_callback(ax):
        #         ax.set_ylim([0.1,ax.get_ylim()[1]*1.5])
        #         ax.set_yscale("log", nonposy='clip'),
        #     xticks = ["CRZ","CRW"]+range(1,20)
        # if (var.lower() in ["disc"]):
        #     data._counts[-10:] *= 0.
        #     data._errors[-10:] *= 0.
        #     data.set_attr("label", "Data [{}]".format(int(data.get_integral())))

        if len(files.keys()) > 1:
            fname = "{}/run2_{}_{}.pdf".format(outputdir,region,var)
        else:
            fname = "{}/y{}_{}_{}.pdf".format(outputdir,files.keys()[0],region,var)
        # print fname
        fnames.append(fname)

        if not ax_ratio_callback and len(bgs[0].edges) < 15:
            def wrapper(ratio, sosb, hidesosb=False):
                binwidth = float(ratio.edges[1]-ratio.edges[0])
                def f(ax):
                    for x,y,sb in zip(ratio.get_bin_centers(),ratio.get_counts(),sosb):
                        if 0. < y < 2.:
                            ax.text(x-0.05*binwidth,y-0.1,"{:.2f}".format(y),
                                    color="black", ha="right", va="center", fontsize=6.0,
                                    wrap=True)
                        if 0. < sb < 2. and not hidesosb:
                            ax.text(x-0.05*binwidth,max(min(sb+0.2,1.9),0.2),"{:.2f}".format(sb),
                                    color="red", ha="right", va="center", fontsize=6.0,
                                    wrap=True)
                            pass
                    if not hidesosb:
                        ax.plot(ratio.get_bin_centers(), sosb, color="r", linestyle="", marker="o", markersize=3, linewidth=1.5, label=r"$s/\sqrt{s+b}$")
                        ax.legend(ncol=2)
                return f
            ax_ratio_callback = wrapper(data/sum(bgs), (sig.counts/10.)/((sig.counts/10.)+sum(bgs).counts)**0.5)
        
        plot_stack(bgs=bgs, data=data, title=title, xlabel=xlabel, filename=fname,
                   cms_type = "Preliminary",
                   lumi = lumi,
                   ratio_range=[0.0,3.0],
                   sigs=[sig],
                   xticks=xticks,
                   mpl_sig_params={
                       # "hist":False,
                       },
                   ax_main_callback=ax_main_callback,
                   ax_ratio_callback=ax_ratio_callback,
                    mpl_legend_params={
                        "ncol": 2,
                        # "framealpha": 0.4,
                        # "labelspacing": 0.5,
                        # "columnspacing": 0.5,
                        },
                    # do_bkg_syst=True,
                   )
        # os.system("ic {}".format(fname))
        # print bgs
        # print data
        # return
        table_info = write_table(data,bgs,signal=sig,outname=fname.replace(".pdf",".txt"),
                binlabels=xticks,signame=sigstr.replace(r"$\times 10$","x10").replace(","," "),csv=True)

    return ", ".join(fnames)

def make_plots(outputdir="plots", inputdir="outputs", year=2017, lumi="41.5", other_years=[], regions=[], signame="tttt"):
    global files, signame_
    signame_ = signame

    os.system("mkdir -p {}/".format(outputdir))

    files = {}
    for y in [year]+other_years:
        files[y] = { proc:uproot.open("{}/output_{}_{}.root".format(inputdir,y,proc)) for proc in d_label_colors.keys()+["data",signame_] }

    # print files

    # # for region in ["htnb1mc","htnb1","os","osloose","br","crw","crz","tt_isr_reweight_check"]:
    # # regions = ["htnb1mc","htnb1","htnb1mcmu","htnb1mu","os","os_noht","osloose","br","crw","crz"]
    # regions = regions or ["htnb1mc","htnb1","os","sshh","sshhmc","osloose","br","crw","crz"]
    # flavs = ["ee","em","mm","in"]
    # varss = labels.keys()
    infos = [[outputdir,year,lumi,x] for x in labels.items()]

    # print infos
    # for res in map(worker,infos):
    #     if res: print "Wrote {}".format(res)

    os.nice(10)
    from multiprocessing import Pool as ThreadPool
    pool = ThreadPool(25)
    for res in pool.imap_unordered(worker,infos):
        if res: print "Wrote {}".format(res)


if __name__ == "__main__":


    make_plots(
            outputdir="plots_temp",
            inputdir="outputs",
            year=2018,
            lumi="124.0", # 2016+2017+2018 --> 35.87+41.53+46.57 = 124.0
            other_years = [2016,2017],
            )

