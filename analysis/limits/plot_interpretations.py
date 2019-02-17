import sys
import os
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as mpatches
import matplotlib.lines as mlines
import matplotlib.patheffects as meffects
import pandas as pd
import glob
import ast

GREEN = (0.,0.8,0.)
YELLOW = (1.,0.8,0.)

def set_defaults():
    from matplotlib import rcParams
    rcParams["font.family"] = "sans-serif"
    rcParams["font.sans-serif"] = ["Helvetica", "Arial", "Liberation Sans", "Bitstream Vera Sans", "DejaVu Sans"]
    rcParams['legend.fontsize'] = 13
    rcParams['legend.labelspacing'] = 0.2
    rcParams['axes.xmargin'] = 0.0 # rootlike, no extra padding within x axis
    rcParams['axes.labelsize'] = 'x-large'
    rcParams['axes.formatter.use_mathtext'] = True
    rcParams['legend.framealpha'] = 0.65
    rcParams['axes.labelsize'] = 'x-large'
    rcParams['axes.titlesize'] = 'x-large'
    rcParams['xtick.labelsize'] = 'x-large'
    rcParams['ytick.labelsize'] = 'x-large'
    rcParams['figure.subplot.hspace'] = 0.1
    rcParams['figure.subplot.wspace'] = 0.1
    rcParams['figure.subplot.right'] = 0.96
    rcParams['figure.max_open_warning'] = 0
    rcParams['figure.dpi'] = 125
    rcParams["axes.formatter.limits"] = [-5,4] # scientific notation if log(y) outside this

def add_cms_info(ax, typ="Preliminary", lumi="136.3", xtype=0.11):
    ax.text(0.0, 1.01,"CMS", horizontalalignment='left', verticalalignment='bottom', transform = ax.transAxes, weight="bold", size="x-large")
    ax.text(xtype, 1.01,typ, horizontalalignment='left', verticalalignment='bottom', transform = ax.transAxes, style="italic", size="x-large")
    ax.text(0.99, 1.01,"%s fb${}^{-1}$ (13 TeV)" % (lumi), horizontalalignment='right', verticalalignment='bottom', transform = ax.transAxes, size="x-large")


class DoubleBandObject(object): pass
class DoubleBandObjectHandler(object):
    def legend_artist(self, legend, orig_handle, fontsize, handlebox):
        x0, y0 = handlebox.xdescent, handlebox.ydescent
        width, height = handlebox.width, handlebox.height
        patch = mpatches.Rectangle([x0, y0-height*0.25], width, height*1.5, facecolor=YELLOW,
                                   edgecolor="none", lw=0.,
                                   transform=handlebox.get_transform())
        handlebox.add_artist(patch)
        patch = mpatches.Rectangle([x0, y0+0.25*height*1.5-height*0.25], width, height*1.5*0.5, facecolor=GREEN,
                                   edgecolor="none", lw=0.,
                                   transform=handlebox.get_transform())
        handlebox.add_artist(patch)
        patch = mlines.Line2D(
                [x0+width*0.03,x0+width-width*0.03],[y0-height*0.25+height*0.75],color=(0.,0.,0.),linewidth=1,linestyle="--",
                transform=handlebox.get_transform(),
                )
        handlebox.add_artist(patch)
        return patch

class OneSideHatchObject(object): pass
class OneSideHatchObjectHandler(object):
    def legend_artist(self, legend, orig_handle, fontsize, handlebox):
        x0, y0 = handlebox.xdescent, handlebox.ydescent
        width, height = handlebox.width, handlebox.height
        patch = mlines.Line2D(
                [x0+width*0.03,x0+width-width*0.03],[y0+height*0.2,y0+height*0.2],color=(0.4,0.4,0.4),linewidth=2,linestyle="-",
                transform=handlebox.get_transform(),
                )
        handlebox.add_artist(patch)
        patch = mpatches.Rectangle([x0, y0+height*0.2], width, height-height*0.2, facecolor='none',
                                   edgecolor=(0.4,0.4,0.4), hatch='///', lw=0.,
                                   transform=handlebox.get_transform())
        handlebox.add_artist(patch)
        return patch

def make_yukawa_plot(scaninfo):

    from parse import get_stuff
    kts, ul, central, ups, downs = get_stuff(scaninfo)
    kts = np.array(kts)
    uls = np.array(ul)

    obs_cent = np.array(central)
    obs_up = np.array(central)+np.array(ups)
    obs_down = np.array(central)-np.array(downs)

    # A: gza
    # B: int 
    # F: higgs
    def calc_sigma(kt,A,B,F):
        return A + B*kt**2 + F*kt**4

    # kfactor such that xsec=11.97 at kt=1
    kfactor = 1.2445

    gza_13tev = 9.997*kfactor
    int_13tev = -1.547*kfactor
    higgs_13tev = 1.168*kfactor
    # scale variations for 13 tev numbers
    gza_13tev_up = 14.104*kfactor
    int_13tev_up = -2.152*kfactor
    higgs_13tev_up = 1.625*kfactor
    gza_13tev_down = 6.378*kfactor
    int_13tev_down = -0.999*kfactor
    higgs_13tev_down = 0.7655*kfactor

    kts_fine = np.linspace(kts.min(),kts.max(),31.)
    theory_cent = calc_sigma(kts_fine,gza_13tev,int_13tev,higgs_13tev)
    theory_down = calc_sigma(kts_fine,gza_13tev_down,int_13tev_down,higgs_13tev_down)
    theory_up = calc_sigma(kts_fine,gza_13tev_up,int_13tev_up,higgs_13tev_up)

    fig, ax = plt.subplots(gridspec_kw={"top":0.92,"bottom":0.14,"left":0.15,},figsize=(5.5,5.5))
    add_cms_info(ax, lumi="35.9")
    p3 = ax.fill_between(kts, obs_down, obs_up, linewidth=0., facecolor="k", alpha=0.25)
    p4 = ax.plot(kts, obs_cent, linestyle="-", marker="",color="k",solid_capstyle="butt")
    pobsline = mlines.Line2D([0],[0],color="k",linewidth=2,linestyle="-",solid_capstyle="butt")

    p5 = ax.fill_between(kts, uls, uls+3.0, linewidth=0., edgecolor=(0.4,0.4,0.4), facecolor="none",hatch="///")
    p6 = ax.plot(kts, uls, linestyle="-", marker="",color=(0.4,0.4,0.4))

    p1 = ax.fill_between(kts_fine, theory_down, theory_up, linewidth=0., facecolor="C0", alpha=0.3)
    p2 = ax.plot(kts_fine, theory_cent, linestyle="--", marker="",color=(0.04,0.16,0.31))

    legend = ax.legend(
            [
                OneSideHatchObject(),
                (p3,pobsline),
                (p1,p2[0]),
                ],
            [
                "Obs. upper limit",
                "Obs. cross section",
                "Predicted cross section,\nPhys. Rev. D 95 (2017) 053004",
                ],
            handler_map={OneSideHatchObject: OneSideHatchObjectHandler()},
            labelspacing=0.6,
            )

    ax.set_ylim([0.,65.])
    ax.set_ylabel(r"$\sigma_{t\bar{t}t\bar{t}}$ [fb]")
    ax.set_title("")
    ax.set_xlabel(r"$|y_\mathrm{t}/y_\mathrm{t}^\mathrm{SM}|$")
    fig.set_tight_layout(True)
    fname = "plots/yukawa_run2.pdf"
    fname_png = fname.replace(".pdf",".png")
    fig.savefig(fname)
    fig.savefig(fname_png)
    os.system("ic {}".format(fname_png))
    print "Saved {}".format(fname)

def make_higgs_plot(basedir,globber,do_scalar=True):

    logs = glob.glob("{}/{}".format(basedir,globber))

    XSEC_TTTT = 11.97
    def parse_lims(lim_lines, fb=False):
        """
        return dictionary with obs, exp, sp1, sm1 cross section limits in pb
        input xsec must be in pb
        """
        d = {}
        for line in lim_lines:
            if "Observed" in line: d["obs"] = float(line.split("<")[-1])
            elif "Expected" in line: d["exp_"+line.split("%")[0].replace("Expected","").strip()] = float(line.split("<")[-1])
            elif "Significance:" in line: d["significance"] = float(line.split(":")[-1])
            elif "p-value" in line: d["pvalue"] = float(line.split("=")[-1].replace(")",""))
            elif "Best fit r:" in line:
                parts = line.split(":")[-1].split()
                rval = parts[0]
                down, up = parts[1].replace("+","").replace("-","").split("/")
                d["mu"] = float(rval)
                d["mu_up"] = float(up)
                d["mu_down"] = float(down)

        mult = 1 if not fb else 1000.0
        obs = d.get("obs",-1)*XSEC_TTTT*mult
        exp = d.get("exp_50.0",-1)*XSEC_TTTT*mult
        exp_sm1 = d.get("exp_16.0",-1)*XSEC_TTTT*mult
        exp_sp1 = d.get("exp_84.0",-1)*XSEC_TTTT*mult
        return {
                "exprm2": d.get("exp_2.5",-1),
                "exprm1": d.get("exp_16.0",-1),
                "expr": d.get("exp_50.0",-1),
                "exprp1": d.get("exp_84.0",-1),
                "exprp2": d.get("exp_97.5",-1),
                "exp84": d.get("exp_50.0",-1),
                "obs":obs, "exp":exp,
                "sp1":exp_sp1,"sm1":exp_sm1,
                "significance": d.get("significance",-1.),
                "pvalue": d.get("pvalue",-1.),
                "mu": d.get("mu",-999.),
                "mu_up": d.get("mu_up",-999.),
                "mu_down": d.get("mu_down",-999.),
                }

    d_xsec = {
            "tth": {
                350: 0.001*24.2,
                370: 0.001*20.1,
                390: 0.001*17.8,
                410: 0.001*15.8,
                430: 0.001*14.9,
                450: 0.001*11.9,
                470: 0.001*11.2,
                490: 0.001*9.87,
                510: 0.001*9.75,
                530: 0.001*7.78,
                550: 0.001*7.49,
                570: 0.007207,
                590: 0.006467,
                610: 0.005803,
                630: 0.005222,
                650: 0.004728,
                670: 0.004325,
                690: 0.004020,
                },
            "thw": {
                350: 0.001*8.37,
                370: 0.001*7.61,
                390: 0.001*7.07,
                410: 0.001*6.58,
                430: 0.001*5.72,
                450: 0.001*5.16,
                470: 0.001*4.81,
                490: 0.001*4.34,
                510: 0.001*4.19,
                530: 0.001*3.86,
                550: 0.001*3.41,
                570: 0.003378,
                590: 0.003095,
                610: 0.002830,
                630: 0.002585,
                650: 0.002361,
                670: 0.002157,
                690: 0.001976,
                },
            "thq": {
                350: 0.001*15.8,
                370: 0.001*13.5,
                390: 0.001*11.7,
                410: 0.001*10.2,
                430: 0.001*8.82,
                450: 0.001*7.73,
                470: 0.001*6.78,
                490: 0.001*5.93,
                510: 0.001*5.23,
                530: 0.001*4.59,
                550: 0.001*4.11,
                570: 0.003786,
                590: 0.003403,
                610: 0.003071,
                630: 0.002778,
                650: 0.002514,
                670: 0.002267,
                690: 0.002024,
                },
        "tta": {
                350: 0.001*33.7,
                370: 0.001*28.6,
                390: 0.001*25.3,
                410: 0.001*22.0,
                430: 0.001*19.9,
                450: 0.001*17.7,
                470: 0.001*15.1,
                490: 0.001*14.9,
                510: 0.001*12.6,
                530: 0.001*11.5,
                550: 0.001*10.0,
                570: 0.009233,
                590: 0.008270,
                610: 0.007430,
                630: 0.006705,
                650: 0.006086,
                670: 0.005564,
                690: 0.005131,
                },
        "taw": {
                350: 0.001*7.97,
                370: 0.001*7.09,
                390: 0.001*6.88,
                410: 0.001*5.92,
                430: 0.001*5.39,
                450: 0.001*4.97,
                470: 0.001*4.59,
                490: 0.001*4.27,
                510: 0.001*3.87,
                530: 0.001*3.82,
                550: 0.001*3.26,
                570: 0.003167,
                590: 0.002946,
                610: 0.002739,
                630: 0.002542,
                650: 0.002348,
                670: 0.002155,
                690: 0.001958,
                },
        "taq": {
                350: 0.001*9.28,
                370: 0.001*8.14,
                390: 0.001*7.19,
                410: 0.001*6.43,
                430: 0.001*5.72,
                450: 0.001*5.07,
                470: 0.001*4.55,
                490: 0.001*4.09,
                510: 0.001*3.67,
                530: 0.001*3.27,
                550: 0.001*2.94,
                570: 0.002753,
                590: 0.002508,
                610: 0.002289,
                630: 0.002090,
                650: 0.001903,
                670: 0.001723,
                690: 0.001543,
                }
        }
    d_xsec["xsech"] = { mass:sum([d_xsec[proc].get(mass,0) for proc in ["tth","thq","thw"]]) for mass in set(sum([d_xsec[proc].keys() for proc in ["tth","thq","thw"]],[])) }
    d_xsec["xseca"] = { mass:sum([d_xsec[proc].get(mass,0) for proc in ["tta","taq","taw"]]) for mass in set(sum([d_xsec[proc].keys() for proc in ["tta","taq","taw"]],[])) }

    infos = []
    for log in logs:
        tmp = log.rsplit("/",1)[-1].split("_")[1].replace("higgs","")
        d = parse_lims(open(log).readlines())
        d["which"] = tmp[0]
        d["mass"] = int(tmp[1:])
        infos.append(d)
    df = pd.DataFrame(infos)
    df["obs"] *= 0.
    df = df.sort_values("mass")

    if do_scalar:
        which = "h"
    else:
        which = "a"
    df = df[df["which"]==which]
    df["xsec"] = df["mass"].apply(lambda x: 1000.*d_xsec["xsec"+which][x])
    obs = (df["expr"]*df["xsec"]).values
    exp = (df["expr"]*df["xsec"]).values
    sm2 = (df["exprm2"]*df["xsec"]).values
    sm1 = (df["exprm1"]*df["xsec"]).values
    sp1 = (df["exprp1"]*df["xsec"]).values
    sp2 = (df["exprp2"]*df["xsec"]).values
    mhs = df["mass"].values
    theory = df["xsec"].values

    fig, ax = plt.subplots(gridspec_kw={"top":0.92,"bottom":0.14,"left":0.15,},figsize=(5.5,5.5))
    add_cms_info(ax, lumi="136.3")
    pe2 = ax.fill_between(mhs, sm2, sp2, linewidth=0., facecolor=YELLOW, alpha=1.0)
    pe1 = ax.fill_between(mhs, sm1, sp1, linewidth=0., facecolor=GREEN, alpha=1.0)
    pe0 = ax.plot(mhs, exp, linestyle="--", marker="",color="k",solid_capstyle="butt")
    pobs = ax.plot(mhs, exp*0., linestyle="-", markersize=5.,marker="o",color="k",solid_capstyle="butt")
    ptheory = ax.plot(mhs, theory, linestyle="-", marker="",color="r",solid_capstyle="butt")
    legend = ax.legend(
            [
                (pobs[0],),
                DoubleBandObject(),
                (ptheory[0],),
                ],
            [
                "95% CL Observed",
                r"95% CL Expected $\pm$ 1 and $\pm$ 2 $\sigma_\mathrm{experiment}$",
                r"$\sigma^\mathrm{%s}_\mathrm{theory}$" % ("pseudoscalar" if which == "a" else "scalar"),
                ],
            handler_map={DoubleBandObject: DoubleBandObjectHandler()},
            labelspacing=0.6,
            )
    ax.set_ylim([0.,120.])
    ax.set_title("")
    fname = "scalar.png"
    if which == "h":
        fname = "plots/ft_higgs_scan_limit_run2.pdf"
        ax.set_ylabel(r"$\sigma$(pp$\rightarrow$ (t$\bar{\mathrm{t}}$,tW,tq)+H) $\times$ BR(H$\rightarrow$ t$\bar{\mathrm{t}}$) (fb)")
        ax.set_xlabel(r"$m_\mathrm{H}$ (GeV)")
    else:
        fname = "plots/ft_higgs_ps_scan_limit_run2.pdf"
        ax.set_ylabel(r"$\sigma$(pp$\rightarrow$ (t$\bar{\mathrm{t}}$,tW,tq)+A) $\times$ BR(A$\rightarrow$ t$\bar{\mathrm{t}}$) (fb)")
        ax.set_xlabel(r"$m_\mathrm{A}$ (GeV)")
    fig.set_tight_layout(True)
    fname_png = fname.replace(".pdf",".png")
    fig.savefig(fname)
    fig.savefig(fname_png)
    os.system("ic {}".format(fname_png))
    print "Saved {}".format(fname)

def make_rpv_plot(basedir,globber,do_tbs=True):
    # xsecs in pb -- https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVgluglu
    gluino_xsecs = {
            1000 : 0.385 ,
            1100 : 0.191 ,
            1200 : 0.0985 ,
            1300 : 0.0522 ,
            1400 : 0.0284 ,
            1500 : 0.0157 ,
            1600 : 0.00887 ,
            1700 : 0.00507 ,
            1800 : 0.00293 ,
            1900 : 0.00171 ,
            2000 : 0.00101 ,
            2100 : 0.000598 ,
            2200 : 0.000356 ,
            2300 : 0.000213 ,
            2400 : 0.000128 ,
            2500 : 7.68e-05 ,
            }

    def parse_lims(lim_lines):
        d = {}
        params = {}
        for line in lim_lines:
            if "Observed" in line: d["obs"] = float(line.split("<")[-1])
            elif "Expected" in line: d["exp_"+line.split("%")[0].replace("Expected","").strip()] = float(line.split("<")[-1])
            elif "Significance:" in line: d["significance"] = float(line.split(":")[-1])
            elif "p-value" in line: d["pvalue"] = float(line.split("=")[-1].replace(")",""))
            elif "Best fit r:" in line:
                parts = line.split(":")[-1].split()
                rval = parts[0]
                down, up = parts[1].replace("+","").replace("-","").split("/")
                d["mu"] = float(rval)
                d["mu_up"] = float(up)
                d["mu_down"] = float(down)
            elif "PARAMS" in line:
                params = ast.literal_eval(line.strip().split(":",1)[1].strip())
        mass = int(params["sig"].split("_m")[1])
        mult = 1.e3
        xsec = gluino_xsecs[mass]*mult
        obs = d.get("obs",-1)*xsec
        exp = d.get("exp_50.0",-1)*xsec
        exp_sm1 = d.get("exp_16.0",-1)*xsec
        exp_sp1 = d.get("exp_84.0",-1)*xsec
        exp_sm2 = d.get("exp_2.5",-1)*xsec
        exp_sp2 = d.get("exp_97.5",-1)*xsec
        return {
                "mass": mass,
                "xsec": xsec,
                "obs":obs, "exp":exp,
                "sp1":exp_sp1,"sm1":exp_sm1,
                "sp2":exp_sp2,"sm2":exp_sm2,
                "expr":d.get("exp_50.0",-1),
                "obsr":d.get("obs",-1),
                }

    fnames = glob.glob("{}/{}".format(basedir,globber))
    infos = []
    for fname in fnames:
        d = parse_lims(open(fname,"r").readlines())
        infos.append(d)
    df = pd.DataFrame(infos)
    df = df.sort_values("mass")


    obs = (df["obs"]).values
    exp = (df["exp"]).values
    sm2 = (df["sm2"]).values
    sm1 = (df["sm1"]).values
    sp1 = (df["sp1"]).values
    sp2 = (df["sp2"]).values
    mgs = df["mass"].values
    theory = df["xsec"].values


    fig, ax = plt.subplots(gridspec_kw={"top":0.92,"bottom":0.14,"left":0.15,},figsize=(5.5,5.5))
    add_cms_info(ax, lumi="136.3")

    pe2 = ax.fill_between(mgs, sm2, sp2, linewidth=0., facecolor=YELLOW, alpha=1.0)
    pe1 = ax.fill_between(mgs, sm1, sp1, linewidth=0., facecolor=GREEN, alpha=1.0)
    pe0 = ax.plot(mgs, exp, linestyle="--", marker="",color="k",solid_capstyle="butt")
    pobs = ax.plot(mgs, obs, linestyle="-", markersize=5.,marker="o",color="k",solid_capstyle="butt")
    ptheory = ax.plot(mgs, theory, linestyle="-", marker="",color="r",solid_capstyle="butt")

    if do_tbs:
        procstr = "T1tbs"
    else:
        procstr = "T1qqqqL w/ SS requirement"

    legend = ax.legend(
            [
                (pobs[0],),
                DoubleBandObject(),
                (ptheory[0],),
                ],
            [
                "95% CL Observed",
                r"95% CL Expected $\pm$ 1 and $\pm$ 2 $\sigma_\mathrm{experiment}$",
                r"$\sigma_\mathrm{{theory}}$ ({})".format(procstr),
                ],
            handler_map={DoubleBandObject: DoubleBandObjectHandler()},
            labelspacing=0.6,
            )

    ax.set_yscale("log")
    ax.set_title("")
    if do_tbs:
        fname = "plots/rpv_t1tbs_run2.png"
    else:
        fname = "plots/rpv_t1qqqql_run2.png"
    ax.set_ylabel(r"$\sigma$ (fb)")
    ax.set_xlabel(r"$\mathrm{m}_\tilde{\mathrm{g}}$ (GeV)")
    fig.set_tight_layout(True)
    fname_png = fname.replace(".pdf",".png")
    fig.savefig(fname)
    fig.savefig(fname_png)
    os.system("ic {}".format(fname_png))
    print "Saved {}".format(fname)

if __name__ == "__main__":

    set_defaults()

    os.system("mkdir -p plots/")
    make_yukawa_plot(scaninfo="scaninfo.txt")
    make_higgs_plot(basedir="v3.24_fthiggs_v1/",globber="card_higgs*_run2.log",do_scalar=True)
    make_higgs_plot(basedir="v3.24_fthiggs_v1/",globber="card_higgs*_run2.log",do_scalar=False)
    make_rpv_plot(basedir="v3.24_sstest_v1",globber="card_rpv_t1tbs_*_all_run2.log",do_tbs=True)
    make_rpv_plot(basedir="v3.24_sstest_v1",globber="card_rpv_t1qqqql_*_all_run2.log",do_tbs=False)
