from tqdm import tqdm
import json
import os
import numpy as np
import itertools
import uproot
import re

from analysis.utils.plotting_utils import write_table

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery.plotter import plot_stack
from matplottery.utils import Hist1D, MET_LATEX, binomial_obs_z
import matplotlib


labels = {}

labels["ft"] = {


        "TOTAL"        : [("SRCR","SR","SRDISC"), "Region"],
        "ht"              : [("ttzcr","ttwcr","sr","br","brpostfit"),r"$H_\mathrm{T}$ (GeV)"],
        "met"             : [("ttzcr","ttwcr","sr","br","brpostfit"), r"$p_\mathrm{T}^{\mathrm{miss}}$ (GeV)"],
        "mtmin"           : [("ttzcr","ttwcr","sr","br","brpostfit"), r"$m_\mathrm{T}^{\mathrm{min}}$"],
        "mll"             : [("ttzcr","ttwcr","sr","br","brpostfit"), r"$m_{ll}$"],
        "njets"           : [("ttzcr","ttwcr","sr","br","brpostfit"), r"$N_\mathrm{jets}$"],
        "nbtags"          : [("ttzcr","ttwcr","sr","br","brpostfit"),r"$N_\mathrm{b}$"],
        "type"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "type (mm, em, ee)"],
        "type3"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "type3 (mmm, mme, mee, eee)"],
        "charge"          : [("ttzcr","ttwcr","sr","br","brpostfit"), "charge"],
        "nleps"           : [("ttzcr","ttwcr","sr","br","brpostfit"), "Nlep"],
        "l1pt"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "l1pt"],
        "l2pt"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "l2pt"],
        "l3pt"            : [("ttzcr","ttwcr","sr","br","brpostfit"), "l3pt"],
        "disc"            : [("br","brpostfit","ttwcr","ttzcr"), "disc"],
        "l3pt"            : [("ttzcr","sr","br","brpostfit"), "l3pt"],
        "mllos"           : [("ttzcr",), "mllos"],
        "dphil1l2": [("br","brpostfit"), r"$\Delta\phi(l_1,l_2)$"],
        "htb": [("br","brpostfit"), r"$H_{T}$(b-jets)"],
        "nlb40": [("br","brpostfit"), r"N-loose b-tags, $p_{T}>40$"],
        "ntb40": [("br","brpostfit"), r"N-tight b-tags, $p_{T}>40$"],
        "detal1l2": [("br","brpostfit"), r"$\Delta\eta(l_1,l_2)$"],
        "maxmjoverpt": [("br","brpostfit"), r"max($m_j/p_T$)"],
        "ml1j1": [("br","brpostfit"), r"m$(l_1,j_2)$"],
        "ptj1": [("br","brpostfit"), "$p_T$ - jet 1"],
        "ptj6": [("br","brpostfit"), "$p_T$ - jet 6"],
        "ptj7": [("br","brpostfit"), "$p_T$ - jet 7"],
        "ptj8": [("br","brpostfit"), "$p_T$ - jet 8"],


        # "el_l1pt"         : [("sr","br"), "el_l1pt"],
        # "el_l2pt"         : [("sr","br"), "el_l2pt"],
        # "el_l1eta"        : [("sr","br"), "el_l1eta"],
        # "el_l2eta"        : [("sr","br"), "el_l2eta"],
        # "el_l1phi"        : [("sr","br"), "el_l1phi"],
        # "el_l2phi"        : [("sr","br"), "el_l2phi"],

        # "mu_l1pt"         : [("sr","br"), "mu_l1pt"],
        # "mu_l2pt"         : [("sr","br"), "mu_l2pt"],
        # "mu_l1eta"        : [("sr","br"), "mu_l1eta"],
        # "mu_l2eta"        : [("sr","br"), "mu_l2eta"],
        # "mu_l1phi"        : [("sr","br"), "mu_l1phi"],
        # "mu_l2phi"        : [("sr","br"), "mu_l2phi"],


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


def remove(rs1,rs2):
    return list(set(rs1)-set(rs2))
ssregions = ("ssbr","br","ml","mlonz","mloffz","hh","lm")
ssregions_ll = ("ssbr","br","ml","mlonz","mloffz","hh","lm","ll")
# ssregions = ("lm",)
labels["ss"] = {

        # "TOTAL"      : [("SRHH","SRHL","SRLL","SRML","SRLM"), "Region"],
        # "ht"         : [("br",), r"$H_\mathrm{T}$ (GeV)"],
        # "met"        : [("br",), r"$p_\mathrm{T}^{\mathrm{miss}}$ (GeV)"],
        # "mtmin"      : [("br",), r"$m_\mathrm{T}^\mathrm{min}$"],
        # "njets"      : [("br",), r"$N_\mathrm{jets}$"],
        # "nbtags"     : [("br",), r"$N_\mathrm{b}$"],
        # "charge"     : [("br",), r"SS charge"],

        "TOTAL"      : [("SRHH","SRHL","SRLL","SRML","SRLM"), "Region"],
        "category"   : [("sr",), r"HH,HL,LL,MLoffZ,MLonZ,LM"],
        "mtmin"      : [ssregions_ll, r"$m_\mathrm{T}^\mathrm{min}$ (GeV)"],
        "ht"         : [ssregions_ll, r"$H_\mathrm{T}$ (GeV)"],
        "njets"      : [ssregions, r"$N_\mathrm{jets}$"],
        "met"        : [ssregions_ll, r"$p_\mathrm{T}^{\mathrm{miss}}$ (GeV)"],
        "mll"        : [ssregions, r"$m_{ll}$"],
        "mllbig"     : [ssregions, r"$m_{ll}$"],
        "mllos"      : [ssregions, r"$m_{ll}$(OS)"],
        "nbtags"     : [ssregions, r"$N_\mathrm{b}$"],
        "type"       : [ssregions, r"$\mu\mu,\mu e,e\mu,ee$"],
        "charge"     : [ssregions, r"SS charge"],
        "el_charge"  : [ssregions, r"SS el charge"],
        "mu_charge"  : [ssregions, r"SS mu charge"],
        "dphi"       : [ssregions, r"dphi(l1,l2)"],
        "nleps"      : [ssregions, r"$N_\mathrm{leps}$"],
        "l1pt"       : [ssregions, r"ordered $p_{T}$(lep1)"],
        "l2pt"       : [ssregions, r"ordered $p_{T}$(lep2)"],
        "el_l1pt"    : [ssregions, r"unsorted $p_{T}$(lep1, e)"],
        "el_l2pt"    : [ssregions, r"unsorted $p_{T}$(lep2, e)"],
        "el_l1eta"   : [ssregions, r"unsorted $\eta $(lep1, e)"],
        "el_l2eta"   : [ssregions, r"unsorted $\eta $(lep2, e)"],
        "mu_l1pt"    : [ssregions, r"unsorted $p_{T}$(lep1, $\mu$)"],
        "mu_l2pt"    : [ssregions, r"unsorted $p_{T}$(lep2, $\mu$)"],
        "mu_l1eta"   : [ssregions, r"unsorted $\eta $(lep1, $\mu$)"],
        "mu_l2eta"   : [ssregions, r"unsorted $\eta $(lep2, $\mu$)"],
        "el_l1phi"   : [ssregions, "el_l1phi"],
        "el_l2phi"   : [ssregions, "el_l2phi"],
        "mu_l1phi"   : [ssregions, "mu_l1phi"],
        "mu_l2phi"   : [ssregions, "mu_l2phi"],
        "nvtx"       : [ssregions, r"nvtx"],
        "lumiblock"  : [ssregions, "lumiblock"],
        "run"        : [ssregions, "run"],
        "class"      : [ssregions, r"hypclass"],
        "l3pt"       : [ssregions, r"ordered $p_{T}$(lep3)"],
        "type3"      : [remove(ssregions,("lm",)), r"$\mu\mu\mu,\mu\mu e,\mu ee,eee$"],
        "q3"         : [remove(ssregions,("lm",)), r"lep 3 charge"],
        "charge3"    : [remove(ssregions,("lm",)), r"$\pm\pm\pm$, $\pm\pm\mp$"],
        "el_l3pt"    : [remove(ssregions,("lm",)), r"unsorted $p_{T}$(lep3, e)"],
        "el_l3eta"   : [remove(ssregions,("lm",)), r"unsorted $\eta $(lep3, e)"],
        "mu_l3pt"    : [remove(ssregions,("lm",)), r"unsorted $p_{T}$(lep3, $\mu$)"],
        "mu_l3eta"   : [remove(ssregions,("lm",)), r"unsorted $\eta $(lep3, $\mu$)"],

}

do_paper_plots_only = True
if do_paper_plots_only:
    labels["ft"] = {

            # "ht"              : [("sr",),r"$H_\mathrm{T}$ (GeV)"],
            # "met"             : [("sr",), r"$p_\mathrm{T}^{\mathrm{miss}}$ (GeV)"],
            "njets"           : [("ttwcrpostfit",), r"$N_\mathrm{jets}$"],
            # "nbtags"          : [("ttzcr","ttwcr","sr"),r"$N_\mathrm{b}$"],

            # "nbtags"          : [("ttzcr",),r"$N_\mathrm{b}$"],
            # "met"             : [("sr",), r"$p_\mathrm{T}^{\mathrm{miss}}$ (GeV)"],

            }
    labels["ss"] = {
            # "TOTAL"      : [("SRHH","SRHL","SRLL","SRML","SRLM"), "Region"],
            "ht"         : [("br",), r"$H_\mathrm{T}$ (GeV)"],
            "met"        : [("br",), r"$p_\mathrm{T}^{\mathrm{miss}}$ (GeV)"],
            "mtmin"      : [("br",), r"$m_\mathrm{T}^\mathrm{min}$ (GeV)"],
            "njets"      : [("br",), r"$N_\mathrm{jets}$"],
            "nbtags"     : [("br",), r"$N_\mathrm{b}$"],
            "charge"     : [("br",), r"SS charge"],
        }

d_label_colors = {}
d_label_colors["ft"] = {
        "fakes" : (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
        # "fakes_mc" : (r"MC fakes", [0.85, 0.85, 0.85]),
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
        "fakes": 0.40,
        "flips": 0.2,
        "rares": 0.5,
        "ww": 0.3,
        "ttw": 0.3,
        "ttz": 0.3,
        "wz": 0.3,
        "tth": 0.3,
        "xg": 0.5,
        }
# "fakes/ttZ/ttH (30%); flips/WW (20%); WZ/ttW (13%); Xg/Rares (50%)"

# from ../limits/test/crfit/
# after doing testcrbins.py to reduce bins (3 bins from crwsplit)
# and run_all_limits.sh with extra containing --unblinded

# d_crpostfitsf = {'fakes': 1.1948199139074693,
#         'flips': 1.020599049200507,
#         'rares': 1.1224350665249947,
#         'total': 1.2794573639702325,
#         'total_background': 1.3056080457107933,
#         'total_signal': 0.0,
#         'tth': 1.155083830402133,
#         'tttt': 0.0,
#         'ttvv': 1.0550142330872638,
#         'ttw': 1.4807971683302559,
#         'ttz': 1.3958306982737914,
#         'xg': 1.0842022472153388}
# d_crpostfitsf_errs = {'fakes': 0.5330058353969307,
#         'flips': 0.14395684087751284,
#         'rares': 0.3343497393873837,
#         'total': 0.14901268940160062,
#         'total_background': 0.1538191984828496,
#         'total_signal': 0.0,
#         'tth': 0.30185266795309806,
#         'tttt': 0.0,
#         'ttvv': 0.28627852193747866,
#         'ttw': 0.28537781734630363,
#         'ttz': 0.2800949349793666,
#         'xg': 0.3090238224979612}

# d_crpostfitsf = {'fakes': 1.133,
#  'flips': 1.001,
#  'rares': 1.02,
#  'total': 1.2039836025074864,
#  'total_background': 1.2254378772947825,
#  'total_signal': 1.076,
#  'tth': 1.094,
#  'tttt': 1.076,
#  'ttvv': 1.014,
#  'ttw': 1.31,
#  'ttz': 1.266,
#  'xg': 1.017}
# d_crpostfitsf_errs = {'fakes': 0.6836929317614069,
#  'flips': 0.5375067661493199,
#  'rares': 0.2513117714751323,
#  'total': 0.18140301518082388,
#  'total_background': 0.18528878200425344,
#  'total_signal': 0.0,
#  'tth': 0.31260232283875494,
#  'tttt': 0.0,
#  'ttvv': 0.1789620767510818,
#  'ttw': 0.5355134476152815,
#  'ttz': 0.348624399392768,
#  'xg': 0.2526828586109242}

# d_crwpostfitsf = {'ttz': 1.250664386115976, 'total_background': 1.213869308634892, 'ttw': 1.4194405049410324, 'xg': 1.0172555920921562, 'fakes': 1.1242900136810103, 'total_signal': 0.7905064076746741, 'tth': 1.07430012587059, 'tttt': 0.7905064076746741, 'rares': 1.0341635356730219, 'ttvv': 1.0162117960119978, 'total': 1.1902705241949165, 'flips': 1.0031427420184953}
d_crwpostfitsf = {'ttz': 1.258240079004025, 'total_background': 1.1806959307212466, 'ttw': 1.2976632996307766, 'xg': 1.014169618208126, 'fakes': 1.1252420864862231, 'total_signal': 1.0571571859443718, 'tth': 1.0876841166064246, 'tttt': 1.0571571859443718, 'rares': 1.0171380863680408, 'ttvv': 1.0113054013836507, 'total': 1.1739011489125408, 'flips': 1.0008309273211684}
d_crwpostfitsf_errs = {'ttz': 0.0952150490606959, 'total_background': 0.042418686289518566, 'ttw': 0.1641408704231698, 'xg': 0.10860077249935805, 'fakes': 0.23947684018971313, 'total_signal': 0.17910949873457052, 'tth': 0.14929586947931764, 'tttt': 0.17910949873457052, 'rares': 0.11287539300468974, 'ttvv': 0.0680479562909091, 'total': 0.04035615876779859, 'flips': 0.12669058272368544}
# d_crwpostfitsf_errs = {'ttz': 0.0895831306866982, 'total_background': 0.030912910675930577, 'ttw': 0.06802089200542652, 'xg': 0.07710601377027705, 'fakes': 0.10385255816014034, 'total_signal': 0.12610672330255662, 'tth': 0.06579515014611349, 'tttt': 0.12610672330255662, 'rares': 0.10295330117848163, 'ttvv': 0.034640313444739806, 'total': 0.029609819286351844, 'flips': 0.03444138757248993}

d_flat_systematics["ft"] = d_crwpostfitsf_errs

# yuck, gotta make these global for multiprocessing since uproot file objects can't be pickled
files = {}
signames_ = []

def worker(info):
    global files, signames_

    analysis, outputdir, year, lumi, (var, (regions, xlabel)) = info

    if signames_ == ["tttt"]:
        sigstrs = [r"$t\bar{t}t\bar{t}$"]
    else:
        sigstrs = []
        for signame in signames_:
            # print signame
            sigtag = signame.split("_m")[0].replace("fs_","")
            # print sigtag
            massstr =  ",".join(re.findall("_m([0-9]*)",signame))
            modelname = {
                    "t1tttt": "T1tttt",
                    "t6ttww": "T6ttWW",
                    "t5qqqqvv": "T5qqqqVV",
                    "t5qqqqvv_dm20": "T5qqqqVV, $\Delta$m=20",
                    "rpv_t1qqqql": "T1qqqqL",
                    }.get(sigtag,sigtag)
            sigstr = r"{} ({}) $\times 10$".format(modelname,massstr)
            sigstrs.append(sigstr)

    fnames = []
    for region in regions:

        title = region.upper()
        lumi_ = str(lumi)

        if analysis == "ft":
            if title in ["SRCR","SR"]:
                if var == "TOTAL":
                    title = "Cut-based"
                else:
                    title = "SR (pre-fit)"
            elif title == "SRDISC":
                title = "BDT"
            elif title == "TTZCR":
                title = "CRZ (pre-fit)"
            elif title == "TTWCR":
                title = "CRW (pre-fit)"

        if analysis == "ss":
            if title in ["BR"]:
                title = "Baseline"
            if title in ["SRHH","SRHL","SRLL","SRML","SRLM"] and (var == "TOTAL"):
                title = title.replace("SR","")

        region_for_hist = region[:]
        if region == "ttwcrpostfit":
            region_for_hist = "ttwcr"
            def get_sf(proc):
                return d_crwpostfitsf.get(proc,1.0)
            bgs = [
                    sum(
                        get_sf(proc)*Hist1D(files[y][proc]["{}_{}_{}".format(region_for_hist,var,proc)],
                            # label="{} ($\\times${:.2f})".format(label,get_sf(proc)),
                            label=label,
                            color=color,
                            dummy=proc,
                            ) for y in files.keys())
                        for proc,(label,color) in d_label_colors[analysis].items()
                        ]
            print(bgs)
        else:
            bgs = [
                    sum(Hist1D(files[y][proc]["{}_{}_{}".format(region_for_hist,var,proc)],label=label,color=color,dummy=proc) for y in files.keys())
                    for proc,(label,color) in d_label_colors[analysis].items()
                    ]

        data = sum([Hist1D(files[y]["data"]["{}_{}_data".format(region_for_hist,var)]) for y in files.keys()])
        # sigcolors = ["red","#5863F8","#FCCA46","#04A777","#944BBB","#233D4D"]
        sigcolors = [[0.75,0.15,0.22],"#5863F8","#FCCA46","#04A777","#944BBB","#233D4D"]
        # print sigcolors
        if region == "ttwcrpostfit":
            region_for_hist = "ttwcr"
            def get_sf(proc):
                return d_crwpostfitsf.get(proc,1.0)
            sigs = [
                    sum(
                        get_sf(signame)*Hist1D(files[y][signame]["{}_{}_{}".format(region_for_hist,var,signame)],
                            # label="{} ($\\times${:.2f})".format(sigstr,get_sf(signame)),
                            label=label,
                            color=sigcolors[isig],
                            dummy=signame,
                            ) for y in files.keys())
                        for isig,(signame,sigstr) in enumerate(zip(signames_,sigstrs))
                        ]
        else:
            sigs = [
                    sum([Hist1D(files[y][signame]["{}_{}_{}".format(region_for_hist,var,signame)],color=sigcolors[isig]) for y in files.keys()])
                    for isig,signame in enumerate(signames_)
                    ]
            for isig,sigstr in enumerate(sigstrs):
                sigs[isig].set_attr("label", sigstr)
                if signames_[isig] != "tttt":
                    sigs[isig] *= 10.

        bgs = sorted(bgs, key=lambda bg: bg.get_integral())
        for bg in bgs:
            # add flat systematic to stat unc in quadrature
            bg._errors = np.hypot(bg._counts*d_flat_systematics[analysis].get(bg.get_attr("dummy"),0.),bg._errors)

        if analysis == "ss" and region in ["SRML","ml","mlonz","mloffz"]:
            # remove flips and ww since they are 0 for multilepton regions
            new_bgs = [bg for bg in bgs if (bg.get_attr("dummy") not in ["flips","ww"])]
            bgs = new_bgs

        data.set_attr("label", "Data")

        # if data.get_integral() < 1e-6: return
        if abs(sum(bgs).get_integral()) < 1e-6: continue

        ax_main_callback = None
        ax_ratio_callback = None
        mpl_legend_params = {}
        ratio_range = [0.,3.]
        xticks = []
        mpl_xtick_params = {}


        if region.lower() in ["sr","srdisc"]:
            data.convert_to_poisson_errors()
            if var.lower() in ["total"]:
                xticks = range(1,20)
            if region.lower() in ["srdisc"]:
                xticks = ["CRZ"]+range(1,25)

        if (region.lower() in ["srcr"]) and (var.lower() in ["total"]):
            data.set_attr("label", "Data")
            def ax_main_callback(ax):
                ax.set_ylim([0.1,ax.get_ylim()[1]*1.5])
                ax.set_yscale("log", nonposy='clip'),
            xticks = ["CRZ","CRW"]+range(1,20)
        if (var.lower() in ["disc"]) and (region.lower() not in ["ttwcr","ttzcr"]):
            data.set_attr("label", "Data")

        if len(files.keys()) > 1:
            fname = "{}/run2_{}_{}.pdf".format(outputdir,region,var)
        else:
            fname = "{}/y{}_{}_{}.pdf".format(outputdir,files.keys()[0],region,var)
        fnames.append(fname)

        ylabel="Events"
        binwidth = data.get_bin_widths()[0]
        if var in ["ht","met","njets","nbtags"]:
            if var in ["ht","met"]:
                ylabel = "Events / {} GeV".format(int(binwidth))
            mpl_legend_params["fontsize"] = 12
            mpl_legend_params["framealpha"] = 0.0

            if var in ["njets","nbtags"] and region in ["ttwcr","ttzcr"]:
                data.poissonify()
                ratio_range = [0.,2.]

            if var in ["njets","nbtags"] and region in ["sr"]:
                ratio_range = [0.,2.]


            def ax_main_callback(ax,title=title):
                # ax.text(0.5,0.97,title,fontsize=13,horizontalalignment="center",verticalalignment="top", transform=ax.transAxes)
                if var in ["ht"] and region in ["sr"]:
                    ax.yaxis.set_major_locator(matplotlib.ticker.MaxNLocator(nbins=5,integer=True))
                if var in ["njets","ht"] and region in ["sr"]:
                    ax.set_ylim([0.,ax.get_ylim()[1]*1.2])

            title = ""


            # if var in ["ht"] and region in ["sr"]:
            #     def ax_main_callback(ax,title=title):
            #         ax.yaxis.set_major_locator(matplotlib.ticker.MaxNLocator(nbins=5,integer=True))
            #         ax.text(0.5,0.97,title,fontsize=13,horizontalalignment="center",verticalalignment="top", transform=ax.transAxes)

            if var in ["met","njets"]:
                def ax_ratio_callback(ax):
                    ax.xaxis.labelpad = -1


            if var in ["nbtags"] and region in ["ttwcr","ttzcr","sr"]:
                def drop2(x):
                    x._counts = x._counts[2:]
                    x._edges = x._edges[2:]
                    x._errors = x._errors[2:]
                    if x._errors_up is not None:
                        x._errors_up = x._errors_up[2:]
                        x._errors_down = x._errors_down[2:]
                map(drop2,bgs+[data]+sigs)

            # mpl_legend_params["framealpha"] = 0.4
            # mpl_legend_params["ncol"] = 1
            # mpl_legend_params["labelspacing"] = 0.10

        # for do_log in [False,True]:
        for do_log in [False]:
            fname_tmp = str(fname)
            if do_log:
                fname_tmp = fname.replace(".pdf","_log.pdf").replace(".png","_log.png")
            plot_stack(bgs=bgs, data=data, title=title, xlabel=xlabel, ylabel=ylabel, filename=fname_tmp,
                       # cms_type = "Preliminary",
                       cms_type = "",
                       lumi = lumi_,
                       ratio_range=ratio_range,
                       sigs=sigs,
                       do_log=do_log,
                       mpl_xtick_params=mpl_xtick_params,
                       mpl_ratio_params={
                           "label":"Data / Pred.",
                           },
                       xticks=xticks,
                       yticks_ratio=range(3),
                       mpl_sig_params={
                           # "hist":False,
                           },
                       ax_main_callback=ax_main_callback,
                       mpl_legend_params=mpl_legend_params,
                       ax_ratio_callback=ax_ratio_callback,
                       do_bkg_syst=True,
                       percentbox=False,
                       data_top_of_legend=True,
                       )

        if (region in ["sr","ttwcrpostfit","ttwcr","ttzcr"]):
            if region == "ttwcrpostfit": title = "CRWpostfit"
            fname_tmp = fname.replace(".pdf","_stacked.pdf").replace(".png","_stacked.png")
            plot_stack(bgs=bgs+sigs, data=data, title=title, xlabel=xlabel, ylabel=ylabel, filename=fname_tmp,
                       # cms_type = "Preliminary",
                       cms_type = "",
                       lumi = lumi_,
                       ratio_range=ratio_range,
                       # sigs=sigs,
                       do_log=False,
                       mpl_xtick_params=mpl_xtick_params,
                       mpl_ratio_params={
                           "label":"Data / Pred.",
                           },
                       xticks=xticks,
                       yticks_ratio=range(3),
                       mpl_sig_params={
                           # "hist":False,
                           },
                       ax_main_callback=ax_main_callback,
                       mpl_legend_params=mpl_legend_params,
                       ax_ratio_callback=ax_ratio_callback,
                       do_bkg_syst=True,
                       percentbox=False,
                       data_top_of_legend=True,
                       )

            os.system("ic {}".format(fname_tmp))

        # print bgs
        # print data
        # return

        table_info = write_table(data,bgs,signal=(None if not sigs else sigs[0]),outname=fname.replace(".pdf",".txt"))
        # table_info = write_table(data,bgs,signal=sig,outname=fname.replace(".pdf",".txt"),
        #         binlabels=xticks,signame=sigstrs[0].replace(r"$\times 10$","x10").replace(","," "),csv=True)
    return ", ".join(fnames)

def make_plots(outputdir="plots", inputdir="outputs", year=2017, lumi="41.5", other_years=[], regions=[], signames="tttt", doss=False, show_mcfakes=False):
    global files, signames_, other_files
    signames_ = signames

    os.system("mkdir -p {}/".format(outputdir))

    analysis = "ft"
    if doss: analysis = "ss"

    if show_mcfakes and ("fakes" in d_label_colors[analysis]):
        del d_label_colors[analysis]["fakes"]
        d_label_colors[analysis]["fakes_mc"] = (r"MC fakes", [0.85, 0.85, 0.85])

    files = {}
    for y in [year]+other_years:
        files[y] = { }
        for proc in d_label_colors[analysis].keys()+["data"]+signames_:
            # try:
            ystr = str(y)
            # if any(x in proc for x in ["fs_","rpv_"]): ystr = "2016"
            files[y][proc] = uproot.open("{}/output_{}_{}.root".format(inputdir,ystr,proc))
            # except IOError:
            #     print("{}/output_{}_{}.root doesn't exist, but ignoring because it's probably signal".format(inputdir,y,proc))

    infos = [[analysis,outputdir,year,lumi,x] for x in labels[analysis].items()]

    # print infos

    # # Smarter. Run the first couple without thread pool so that it doesn't obscure errors when crashing
    # # Then run rest in parallel
    # # wtf. Can't do this because of https://github.com/uqfoundation/pathos/issues/153
    # map(worker,infos[:2])
    # os.nice(10)
    # from multiprocessing import Pool as ThreadPool
    # pool = ThreadPool(15)
    # for res in pool.imap_unordered(worker,infos[1:]):
    #     if res:
    #         print "Wrote {}".format(res)

    map(worker,infos)

    # # Don't be nice for plots since I need them *now*
    # # os.nice(4)
    # from multiprocessing import Pool as ThreadPool
    # pool = ThreadPool(15)
    # # print infos
    # for res in pool.imap_unordered(worker,infos):
    #     if res:
    #         print "Wrote {}".format(res)



if __name__ == "__main__":

    make_plots(
            outputdir="plots/",
            inputdir="outputs",
            year=2018,
            lumi=("137"),
            other_years = [2016,2017],
            doss=False,
            signames=["tttt"],
            )


    # /home/users/namin/2018/fourtop/all/FTAnalysis/common/matplottery/matplottery/plotter.py
