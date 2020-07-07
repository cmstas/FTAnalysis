import sys
import os
import numpy as np
import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.lines as mlines
import matplotlib.patheffects as meffects
import matplotlib.font_manager as mfm
import pandas as pd
import pickle
import fnmatch
import glob
import commands
import ast
from matplotlib.ticker import MultipleLocator, FuncFormatter

import os
import matplotlib.pyplot as plt


# SIGH
import matplotlib
matplotlib.rcParams['xtick.direction'] = 'in'
matplotlib.rcParams['ytick.direction'] = 'in'

XSEC_TTTT = 11.97
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
    # print rcParams
    # rcParams["pdf.fonttype"] = 42
    # rcParams["pdf.use14corefonts"] = True # this one uses helvetica for pdf saving
    # pdf.fonttype       : 3         ## Output Type 3 (Type3) or Type 42 (TrueType)
    # pdf.use14corefonts : False

def add_cms_info(ax, typ="", lumi="137", xtype=0.12):
    ax.text(0.0, 1.01,"CMS", horizontalalignment='left', verticalalignment='bottom', transform = ax.transAxes, weight="bold", size=20)
    ax.text(xtype, 1.01,typ, horizontalalignment='left', verticalalignment='bottom', transform = ax.transAxes, style="italic", size="x-large")
    ax.text(0.99, 1.01,"%s fb${}^\mathregular{-1}$ (13 TeV)" % (lumi), horizontalalignment='right', verticalalignment='bottom', transform = ax.transAxes, size="x-large")

def get_fig_ax():
    fig, ax = plt.subplots(gridspec_kw={"top":0.92,"bottom":0.14,"left":0.15,"right":0.95},figsize=(5.5,5.5))
    return fig, ax

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

def make_miplot(globber,outdir="plots",do_met=True):


    if False:
        def parse_lims(fname):
            d = {}
            params = {}
            lim_lines = open(fname).readlines()
            for line in lim_lines:
                if "Observed" in line: d["obs"] = float(line.split("<")[-1])
                elif "Expected" in line: d["exp_"+line.split("%")[0].replace("Expected","").strip()] = float(line.split("<")[-1])
            lumi = 137.2
            obs = d.get("obs",-1)/lumi
            exp = d.get("exp_50.0",-1)/lumi
            exp_sm1 = d.get("exp_16.0",-1)/lumi
            exp_sp1 = d.get("exp_84.0",-1)/lumi
            exp_sm2 = d.get("exp_2.5",-1)/lumi
            exp_sp2 = d.get("exp_97.5",-1)/lumi
            ibin = int(fname.split("_bin",1)[1].rsplit("/")[0])
            return {
                    "ibin": ibin,
                    "obs":obs, "exp":exp,
                    "sp1":exp_sp1,"sm1":exp_sm1,
                    "sp2":exp_sp2,"sm2":exp_sm2,
                    }
        fnames = glob.glob(globber)
        data = map(parse_lims,fnames)
        df = pd.DataFrame(data)
        print df
        obs = (df["obs"]).values
        exp = (df["exp"]).values
        sm2 = (df["sm2"]).values
        sm1 = (df["sm1"]).values
        sp1 = (df["sp1"]).values
        sp2 = (df["sp2"]).values
        xs = df["ibin"].values


    content = commands.getstatusoutput('grep "r < " temp/cards_srmi*/card_*_run2_lim*log')[1]
    data = {}
    for line in content.splitlines():
        if not line.strip(): continue
        logname,rest = line.split(":",1)
        tag = logname.split("temp/",1)[1].split("/",1)[0]
        lumi = 137.2
        num = float(rest.split("r < ",1)[1].split()[0])/lumi
        if tag not in data: data[tag] = dict(tag=tag)
        info = data[tag]
        if "_toys" not in logname:
            if "Observed" in rest: info["obs"] = num
            elif "2.5%" in rest: info["exp_sm2"] = num
            elif "16.0%" in rest: info["exp_sm1"] = num
            elif "50.0%" in rest: info["exp"] = num
            elif "84.0%" in rest: info["exp_sp1"] = num
            elif "97.5%" in rest: info["exp_sp2"] = num
        else:
            if "_obs" in logname: info["toy_obs"] = num
            elif "_exp0p025" in logname: info["toy_exp_sm2"] = num
            elif "_exp0p16" in logname: info["toy_exp_sm1"] = num
            elif "_exp0p5" in logname: info["toy_exp"] = num
            elif "_exp0p84" in logname: info["toy_exp_sp1"] = num
            elif "_exp0p975" in logname: info["toy_exp_sp2"] = num
        # data.append(info)
    df = pd.DataFrame(data.values())
    df["which"] = df["tag"].str.split("_").str[1]
    df["ibin"] = df["tag"].str.split("_").str[2].str.replace("bin","").astype(int)

    if do_met:
        df = df[df["which"] == "srmi1"]
    else:
        df = df[df["which"] == "srmi2"]
    df = df.sort_values(["ibin"])
    obs = (df["toy_obs"]).values
    exp = (df["toy_exp"]).values
    sm2 = (df["toy_exp_sm2"]).values
    sm1 = (df["toy_exp_sm1"]).values
    sp1 = (df["toy_exp_sp1"]).values
    sp2 = (df["toy_exp_sp2"]).values
    xs = df["ibin"].values
    print xs


    fig,ax = get_fig_ax()
    add_cms_info(ax, lumi="137")


    if do_met:
        xs = (xs-1)*100 + 300
    else:
        xs = (xs-1)*100 + 1400
    print xs


    pe2 = ax.fill_between(xs, sm2, sp2, linewidth=0., facecolor=YELLOW, alpha=1.0)
    pe1 = ax.fill_between(xs, sm1, sp1, linewidth=0., facecolor=GREEN, alpha=1.0)
    pe0 = ax.plot(xs, exp, linestyle="--", marker="",color="k",solid_capstyle="butt")
    pobs = ax.plot(xs, obs, linestyle="-", markersize=5.,marker="o",color="k",solid_capstyle="butt")

    if do_met:
        ax.set_ylim([0.,0.5])
    else:
        ax.set_ylim([0.,0.5])
        # ax.set_yscale("log")
    # else:
    #     procstr = "T1qqqqL"
    #     ax.set_yscale("log")
    #     ax.set_ylim([0.07,200.])

    legend = ax.legend(
            [
                (pobs[0],),
                DoubleBandObject(),
                ],
            [
                "Observed",
                r"Median expected, $\pm$1 and 2 s.d.${}_\mathrm{experiment}$",
                ],
            handler_map={DoubleBandObject: DoubleBandObjectHandler()},
            labelspacing=0.6,
            loc="upper center",
            framealpha=0.0,
            # title="95% CL upper limits",
            # mode="expand",
            )
    legend.get_title().set_fontsize(legend.get_texts()[0].get_fontsize())

    ax.set_title("")

    xtext = np.array(ax.get_xlim()).mean()
    ytext = np.array(ax.get_ylim())[-1]*0.65

    ax.set_ylabel(r"95% CL upper limit on $\sigma A \epsilon$ (fb)", horizontalalignment="right", y=1.)
    if do_met:
        fname = "{}/milimits_met.pdf".format(outdir)
        ax.set_xlabel(r"$p_\mathrm{T}^{\mathrm{miss}}$ (GeV)", horizontalalignment="right", x=1.)
        ax.text(xtext,ytext,"$H_\mathrm{T}$ > 300 GeV",fontsize=13,horizontalalignment="center",verticalalignment="center",color="k")
    else:
        fname = "{}/milimits_ht.pdf".format(outdir)
        ax.set_xlabel(r"$H_\mathrm{T}$ (GeV)", horizontalalignment="right", x=1.)
        ax.text(xtext,ytext,"$p_\mathrm{T}^{\mathrm{miss}}$ < 300 GeV",fontsize=13,horizontalalignment="center",verticalalignment="center",color="k")
    ax.yaxis.set_minor_locator(MultipleLocator(0.02))
    ax.xaxis.set_minor_locator(MultipleLocator(100))
    ax.tick_params(axis="x",pad=8.0)

    # ax.xaxis.set_major_formatter(FuncFormatter(lambda x,y:">"+str(int(x))))

    # fig.canvas.draw()
    # labels = ["$>$"+item.get_text() for item in ax.get_xticklabels()]
    # print labels
    # ax.set_xticklabels(labels)

    fig.set_tight_layout(True)
    fname_png = fname.replace(".pdf",".png")
    fig.savefig(fname)
    fig.savefig(fname_png)
    os.system("ic {}".format(fname_png))
    # os.system("ic {}".format(fname))
    print "Saved {}".format(fname)

if __name__ == "__main__":

    set_defaults()

    os.system("mkdir -p plots/")

    make_miplot(globber="temp/*srmi1*/*_lim.log",outdir="plots/",do_met=True)
    make_miplot(globber="temp/*srmi2*/*_lim.log",outdir="plots/",do_met=False)
