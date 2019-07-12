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
import ast
from matplotlib.ticker import MultipleLocator



import os
import matplotlib.pyplot as plt


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
    ax.text(0.0, 1.01,"CMS", horizontalalignment='left', verticalalignment='bottom', transform = ax.transAxes, weight="bold", size="x-large")
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

def parse_yukawa_log(fname):
    d = {}
    tthscale = None
    for line in open(fname,"r").readlines():
        line = line.strip()
        if line.startswith("Set Default Value of"):
            tthscale = float(line.split()[-1])
            if tthscale not in d:
                d[tthscale] = {}
        if line.startswith("Observed Limit:"):
            limit = float(line.split("<")[-1].strip())
            d[tthscale]["limit"] = XSEC_TTTT*limit
        if line.startswith("Best fit r:"):
            parts = line.split(":")[-1].strip()
            central = float(parts.split()[0])
            down,up = map(float,parts.split()[1].split("/"))
            d[tthscale]["central"] = XSEC_TTTT*central
            d[tthscale]["up"] = XSEC_TTTT*up
            d[tthscale]["down"] = XSEC_TTTT*down
    keys = sorted(d.keys())
    ktvals = map(lambda x: x**.5, keys)
    ul = map(lambda x: d[x].get("limit",-1), keys)
    central = map(lambda x: d[x].get("central",-1), keys)
    ups = map(lambda x: d[x].get("up",-1), keys)
    downs = map(lambda x: d[x].get("down",-1), keys)
    return ktvals, ul, central, ups, downs


def make_yukawa_plot(scaninfo):

    kts, ul, central, ups, downs = parse_yukawa_log(scaninfo)
    kts = np.array(kts)
    uls = np.array(ul)

    # print central
    # print ups
    # print downs
    obs_cent = np.array(central)
    obs_up = obs_cent+np.abs(np.array(ups))
    obs_down = obs_cent-np.abs(np.array(downs))

    # good = kts<2.5
    good = kts<3.0
    kts = kts[good]
    uls = uls[good]
    obs_cent = obs_cent[good]
    obs_up = obs_up[good]
    obs_down = obs_down[good]

    print obs_cent
    print obs_up
    print obs_down

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

    fig,ax = get_fig_ax()
    add_cms_info(ax, lumi="137")
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
                "Observed upper limit",
                "Observed cross section",
                "Predicted cross section,\nPhys. Rev. D 95 (2017) 053004",
                ],
            handler_map={OneSideHatchObject: OneSideHatchObjectHandler()},
            labelspacing=0.6,
            fontsize=12,
            )

    ax.xaxis.set_minor_locator(MultipleLocator(0.1))
    ax.yaxis.set_minor_locator(MultipleLocator(2))

    ax.set_ylim([0.,55.])
    ax.set_ylabel(r"$\sigma_{t\bar{t}t\bar{t}}$ (fb)")
    ax.set_title("")
    ax.set_xlabel(r"$|y_\mathrm{t}/y_\mathrm{t}^\mathrm{SM}|$")
    fig.set_tight_layout(True)
    fname = "plots/yukawa_run2.pdf"
    fname_png = fname.replace(".pdf",".png")
    fig.savefig(fname)
    fig.savefig(fname_png)
    os.system("ic {}".format(fname_png))
    print "Saved {}".format(fname)

def make_higgs_plot(basedir,globber,do_scalar=True,do_sum=False):

    logs = glob.glob("{}/{}".format(basedir,globber))

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
                "obsr": d.get("obs",-1),
                "exp84": d.get("exp_50.0",-1),
                "obs":obs, "exp":exp,
                "sp1":exp_sp1,"sm1":exp_sm1,
                "significance": d.get("significance",-1.),
                "pvalue": d.get("pvalue",-1.),
                "mu": d.get("mu",-999.),
                "mu_up": d.get("mu_up",-999.),
                "mu_down": d.get("mu_down",-999.),
                }

    # # Old -- nate's MG4 model
    # d_xsec = {
    #     "tth": { 350: 0.001*24.2, 370: 0.001*20.1, 390: 0.001*17.8, 410: 0.001*15.8, 430: 0.001*14.9, 450: 0.001*11.9, 470: 0.001*11.2, 490: 0.001*9.87, 510: 0.001*9.75, 530: 0.001*7.78, 550: 0.001*7.49, 570: 0.007207, 590: 0.006467, 610: 0.005803, 630: 0.005222, 650: 0.004728, 670: 0.004325, 690: 0.004020, },
    #     "thw": { 350: 0.001*8.37, 370: 0.001*7.61, 390: 0.001*7.07, 410: 0.001*6.58, 430: 0.001*5.72, 450: 0.001*5.16, 470: 0.001*4.81, 490: 0.001*4.34, 510: 0.001*4.19, 530: 0.001*3.86, 550: 0.001*3.41, 570: 0.003378, 590: 0.003095, 610: 0.002830, 630: 0.002585, 650: 0.002361, 670: 0.002157, 690: 0.001976, },
    #     "thq": { 350: 0.001*15.8, 370: 0.001*13.5, 390: 0.001*11.7, 410: 0.001*10.2, 430: 0.001*8.82, 450: 0.001*7.73, 470: 0.001*6.78, 490: 0.001*5.93, 510: 0.001*5.23, 530: 0.001*4.59, 550: 0.001*4.11, 570: 0.003786, 590: 0.003403, 610: 0.003071, 630: 0.002778, 650: 0.002514, 670: 0.002267, 690: 0.002024, },
    #     "tta": { 350: 0.001*33.7, 370: 0.001*28.6, 390: 0.001*25.3, 410: 0.001*22.0, 430: 0.001*19.9, 450: 0.001*17.7, 470: 0.001*15.1, 490: 0.001*14.9, 510: 0.001*12.6, 530: 0.001*11.5, 550: 0.001*10.0, 570: 0.009233, 590: 0.008270, 610: 0.007430, 630: 0.006705, 650: 0.006086, 670: 0.005564, 690: 0.005131, },
    #     "taw": { 350: 0.001*7.97, 370: 0.001*7.09, 390: 0.001*6.88, 410: 0.001*5.92, 430: 0.001*5.39, 450: 0.001*4.97, 470: 0.001*4.59, 490: 0.001*4.27, 510: 0.001*3.87, 530: 0.001*3.82, 550: 0.001*3.26, 570: 0.003167, 590: 0.002946, 610: 0.002739, 630: 0.002542, 650: 0.002348, 670: 0.002155, 690: 0.001958, },
    #     "taq": { 350: 0.001*9.28, 370: 0.001*8.14, 390: 0.001*7.19, 410: 0.001*6.43, 430: 0.001*5.72, 450: 0.001*5.07, 470: 0.001*4.55, 490: 0.001*4.09, 510: 0.001*3.67, 530: 0.001*3.27, 550: 0.001*2.94, 570: 0.002753, 590: 0.002508, 610: 0.002289, 630: 0.002090, 650: 0.001903, 670: 0.001723, 690: 0.001543, },
    #     }

    # New MG5 2HDMtII model
    d_xsec = {

        # "tth": { 350: 0.02097, 370: 0.01941, 390: 0.01753, 410: 0.01584, 430: 0.01427, 450: 0.01291, 470: 0.01171, 490: 0.01064, 510: 0.00967, 530: 0.00881, 550: 0.00803, 570: 0.00735, 590: 0.00672, 610: 0.00615, 630: 0.00564, 650: 0.00517, },
        # "thw": { 350: 0.01170, 370: 0.01110, 390: 0.01026, 410: 0.00948, 430: 0.00870, 450: 0.00800, 470: 0.00734, 490: 0.00673, 510: 0.00620, 530: 0.00569, 550: 0.00524, 570: 0.00483, 590: 0.00446, 610: 0.00413, 630: 0.00382, 650: 0.00354, },
        # "thq": { 350: 0.01795, 370: 0.01620, 390: 0.01430, 410: 0.01257, 430: 0.01103, 450: 0.00972, 470: 0.00860, 490: 0.00759, 510: 0.00674, 530: 0.00600, 550: 0.00537, 570: 0.00480, 590: 0.00431, 610: 0.00386, 630: 0.00348, 650: 0.00313, },
        # "tta": { 350: 0.03586, 370: 0.03201, 390: 0.02842, 410: 0.02514, 430: 0.02233, 450: 0.01994, 470: 0.01780, 490: 0.01595, 510: 0.01428, 530: 0.01282, 550: 0.01150, 570: 0.01039, 590: 0.00938, 610: 0.00845, 630: 0.00763, 650: 0.00692, },
        # "taw": { 350: 0.01204, 370: 0.01111, 390: 0.01017, 410: 0.00931, 430: 0.00854, 450: 0.00781, 470: 0.00717, 490: 0.00661, 510: 0.00607, 530: 0.00560, 550: 0.00517, 570: 0.00478, 590: 0.00441, 610: 0.00406, 630: 0.00375, 650: 0.00347, },
        # "taq": { 350: 0.01165, 370: 0.01041, 390: 0.00922, 410: 0.00823, 430: 0.00737, 450: 0.00660, 470: 0.00592, 490: 0.00531, 510: 0.00479, 530: 0.00432, 550: 0.00390, 570: 0.00353, 590: 0.00320, 610: 0.00291, 630: 0.00264, 650: 0.00239, },

        # "tth": { 350: 0.02097, 370: 0.01941, 390: 0.01753, 410: 0.01584, 430: 0.01427, 450: 0.01291, 470: 0.01171, 490: 0.01064, 510: 0.00967, 530: 0.00881, 550: 0.00803, 570: 0.00735, 590: 0.00672, 610: 0.00615, 630: 0.00564, 650: 0.00517, },
        # "thw": { 350: 0.01170, 370: 0.01110, 390: 0.01026, 410: 0.00948, 430: 0.00870, 450: 0.00800, 470: 0.00734, 490: 0.00673, 510: 0.00620, 530: 0.00569, 550: 0.00524, 570: 0.00483, 590: 0.00446, 610: 0.00413, 630: 0.00382, 650: 0.00354, },
        # "thq": { 350: 0.01795, 370: 0.01620, 390: 0.01430, 410: 0.01257, 430: 0.01103, 450: 0.00972, 470: 0.00860, 490: 0.00759, 510: 0.00674, 530: 0.00600, 550: 0.00537, 570: 0.00480, 590: 0.00431, 610: 0.00386, 630: 0.00348, 650: 0.00313, },
        # "tta": { 350: 0.03304, 370: 0.03069, 390: 0.02763, 410: 0.02462, 430: 0.02196, 450: 0.01967, 470: 0.01759, 490: 0.01578, 510: 0.01415, 530: 0.01272, 550: 0.01142, 570: 0.01032, 590: 0.00932, 610: 0.00841, 630: 0.00759, 650: 0.00688, },
        # "taw": { 350: 0.01110, 370: 0.01065, 390: 0.00989, 410: 0.00912, 430: 0.00840, 450: 0.00771, 470: 0.00709, 490: 0.00654, 510: 0.00602, 530: 0.00556, 550: 0.00513, 570: 0.00475, 590: 0.00438, 610: 0.00403, 630: 0.00373, 650: 0.00346, },
        # "taq": { 350: 0.01074, 370: 0.00998, 390: 0.00897, 410: 0.00806, 430: 0.00724, 450: 0.00651, 470: 0.00585, 490: 0.00526, 510: 0.00474, 530: 0.00428, 550: 0.00387, 570: 0.00351, 590: 0.00318, 610: 0.00289, 630: 0.00262, 650: 0.00238, },

        # NNPDF30_lo_as_0130
        "tth": {450: 0.010682752, 550: 0.0063392135, 390: 0.015096416100000001, 490: 0.008632358500000001, 630: 0.0042931789, 530: 0.0069951436, 430: 0.0119420745, 370: 0.0169809566, 610: 0.0047297227, 510: 0.007765844300000001, 470: 0.009613482000000001, 410: 0.0134279684, 590: 0.005202127, 570: 0.0057455038, 350: 0.0185143819, 650: 0.0039104657},
        "thw": {450: 0.0056768322, 550: 0.0035618743, 390: 0.007494867200000001, 490: 0.0046984308, 630: 0.0025047125000000003, 530: 0.003914093400000001, 430: 0.0062351308000000005, 370: 0.0081869037, 610: 0.0027237802, 510: 0.0042829036000000004, 470: 0.0051794518, 410: 0.006845303200000001, 590: 0.0029802590000000003, 570: 0.0032522894, 350: 0.008661526400000001, 650: 0.002284925},
        "thq": {450: 0.009315778, 550: 0.0050025702000000005, 390: 0.0138925995, 490: 0.0071878093, 630: 0.0031643444, 530: 0.0056235926, 430: 0.0106258163, 370: 0.015883702200000002, 610: 0.0035355493, 510: 0.006353862, 470: 0.0082177972, 410: 0.0121690891, 590: 0.0039580287, 570: 0.0044437551, 350: 0.017739056, 650: 0.0028334996},
        "tta": {450: 0.0155234338, 550: 0.0087135532, 390: 0.0226198931, 490: 0.012246641400000001, 630: 0.0056337246, 530: 0.0097404993, 430: 0.017593939200000002, 370: 0.0256368625, 610: 0.0062696396, 510: 0.0109298962, 470: 0.0137758844, 410: 0.0198962237, 590: 0.0069905502, 570: 0.007804414900000001, 350: 0.0289885125, 650: 0.0050780903},
        "taw": {450: 0.0055611045000000005, 550: 0.0035126836, 390: 0.007434729, 490: 0.0046146355, 630: 0.0024661255, 530: 0.0038467407, 430: 0.006131804, 370: 0.0081844304, 610: 0.0026852096000000002, 510: 0.0042009149, 470: 0.0050620913, 410: 0.0067569432000000006, 590: 0.0029403872, 570: 0.0032054519, 350: 0.0089778315, 650: 0.0022517830000000003},
        "taq": {450: 0.0062621178000000005, 550: 0.0036001531, 390: 0.0088503291, 490: 0.0049840919, 630: 0.0023751163, 530: 0.0040093756, 430: 0.007004922, 370: 0.0100224284, 610: 0.0026336461, 510: 0.0044720565, 470: 0.0055733218, 410: 0.0078737072, 590: 0.0029177462, 570: 0.0032320544, 350: 0.0112952294, 650: 0.0021521164},

        }

    d_xsec["xsech"] = { mass:sum([d_xsec[proc].get(mass,0) for proc in ["tth","thq","thw"]]) for mass in set(sum([d_xsec[proc].keys() for proc in ["tth","thq","thw"]],[])) }
    d_xsec["xseca"] = { mass:sum([d_xsec[proc].get(mass,0) for proc in ["tta","taq","taw"]]) for mass in set(sum([d_xsec[proc].keys() for proc in ["tta","taq","taw"]],[])) }
    d_xsec["xsecb"] = { mass:(d_xsec["xsech"][mass]+d_xsec["xseca"][mass]) for mass in d_xsec["xsech"].keys() }
    # print d_xsec["xsech"]

    infos = []
    for log in logs:
        tmp = log.rsplit("/",1)[-1].split("_")[1].replace("higgs","")
        d = parse_lims(open(log).readlines())
        d["which"] = tmp[0]
        d["mass"] = int(tmp[1:])
        infos.append(d)
    df = pd.DataFrame(infos)
    # df["obs"] *= 0.
    df = df.sort_values("mass")

    if do_scalar:
        which = "h"
    else:
        which = "a"
    if do_sum:
        which = "b"
    df = df[df["which"]==which]
    df["xsec"] = df["mass"].apply(lambda x: 1000.*d_xsec["xsec"+which][x])
    # obs = (df["expr"]*df["xsec"]).values
    obs = (df["obsr"]*df["xsec"]).values
    exp = (df["expr"]*df["xsec"]).values
    sm2 = (df["exprm2"]*df["xsec"]).values
    sm1 = (df["exprm1"]*df["xsec"]).values
    sp1 = (df["exprp1"]*df["xsec"]).values
    sp2 = (df["exprp2"]*df["xsec"]).values
    mhs = df["mass"].values
    theory = df["xsec"].values

    df.to_json("dump_higgs_{}.json".format(which))

    df_dm = pd.read_json("../../studies/FTInterpretations/plotting/dump_dmhiggs.json")
    if do_scalar:
        theory_dm = df_dm[df_dm.which == "dmscalar"][["massmed","xsec_totsm"]].sort_values(["massmed"],ascending=True)
    else:
        theory_dm = df_dm[df_dm.which == "dmpseudo"][["massmed","xsec_totsm"]].sort_values(["massmed"],ascending=True)

    # smooth the theory curve.
    from scipy.signal import savgol_filter
    theory = savgol_filter(theory,5,2)
    # sp2 = savgol_filter(sp2,5,2)
    # sp1 = savgol_filter(sp1,5,2)
    # sm2 = savgol_filter(sm2,5,2)
    # sm1 = savgol_filter(sm1,5,2)

    fig,ax = get_fig_ax()
    add_cms_info(ax, lumi="137")
    pe2 = ax.fill_between(mhs, sm2, sp2, linewidth=0., facecolor=YELLOW, alpha=1.0)
    pe1 = ax.fill_between(mhs, sm1, sp1, linewidth=0., facecolor=GREEN, alpha=1.0)
    pe0 = ax.plot(mhs, exp, linestyle="--", marker="",color="k",solid_capstyle="butt")
    # pobs = ax.plot(mhs, exp*0., linestyle="-", markersize=5.,marker="o",color="k",solid_capstyle="butt")
    pobs = ax.plot(mhs, obs, linestyle="-", markersize=5.,marker="o",color="k",solid_capstyle="butt")
    ptheory_2hdm = ax.plot(mhs, theory, linestyle="-", marker="",color="r",solid_capstyle="butt")
    ptheory_dm = ax.plot(theory_dm.massmed, theory_dm.xsec_totsm*1e3, linestyle="-", marker="",color="b",solid_capstyle="butt")
    # ptheorysmooth = ax.plot(mhs, savgol_filter(theory,5,2), linestyle="-", marker="",color="b",solid_capstyle="butt")
    legend = ax.legend(
            [
                (pobs[0],),
                DoubleBandObject(),
                (ptheory_2hdm[0],),
                (ptheory_dm[0],),
                ],
            [
                "Observed",
                r"Median expected, $\pm$1 and $\pm$2 $\sigma_\mathrm{experiment}$",
                # r"$\sigma^\mathrm{%s}_\mathrm{theory}$" % ("pseudoscalar" if which == "a" else "scalar"),
                r"$\sigma^\mathrm{%s}_\mathrm{theory}$ (2HDM, $\tan\beta$=1)" % ("pseudoscalar" if which == "a" else "scalar"),
                r"$\sigma^\mathrm{%s}_\mathrm{theory}$ (DM, $m_\chi$=600 GeV, $g_\mathrm{SM}$=$g_\mathrm{DM}$=1)" % ("pseudoscalar" if which == "a" else "scalar"),
                ],
            handler_map={DoubleBandObject: DoubleBandObjectHandler()},
            labelspacing=0.4,
            title="95% CL upper limits",
            fontsize=12,
            )
    legend.get_title().set_fontsize(legend.get_texts()[0].get_fontsize())
    ax.yaxis.set_minor_locator(MultipleLocator(5.))
    ax.xaxis.set_minor_locator(MultipleLocator(10.))
    ax.set_ylim([0.,100.])
    # ax.set_ylim([1.,120.])
    # ax.set_yscale("log")
    ax.set_title("")
    fname = "scalar.png"
    if which == "h":
        fname = "plots/ft_higgs_scan_limit_run2.pdf"
        ax.set_ylabel(r"$\sigma$(pp$\rightarrow$ (t$\bar{\mathrm{t}}$,tW,tq)+H) $\times$ BR(H$\rightarrow$ t$\bar{\mathrm{t}}$) (fb)")
        ax.set_xlabel(r"$m_\mathrm{H}$ (GeV)")
    elif which == "a":
        fname = "plots/ft_higgs_ps_scan_limit_run2.pdf"
        ax.set_ylabel(r"$\sigma$(pp$\rightarrow$ (t$\bar{\mathrm{t}}$,tW,tq)+A) $\times$ BR(A$\rightarrow$ t$\bar{\mathrm{t}}$) (fb)")
        ax.set_xlabel(r"$m_\mathrm{A}$ (GeV)")
    elif which == "b":
        fname = "plots/ft_higgs_both_scan_limit_run2.pdf"
        ax.set_ylabel(r"$\sigma$(pp$\rightarrow$ (t$\bar{\mathrm{t}}$,tW,tq)+H/A) $\times$ BR(H/A$\rightarrow$ t$\bar{\mathrm{t}}$) (fb)")
        ax.set_xlabel(r"$m_\mathrm{H/A}$ (GeV)")
    fig.set_tight_layout(True)
    fname_png = fname.replace(".pdf",".png")
    fig.savefig(fname)
    fig.savefig(fname_png)
    os.system("ic {}".format(fname_png))
    print "Saved {}".format(fname)

def make_rpv_plot(biglog,globber,outdir="scanplots",do_tbs=True):
    # xsecs in pb -- https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVgluglu
    gluino_xsecs = {
            1000 : 0.385 ,
            1050 : 0.27 ,
            1100 : 0.191 ,
            1150 : 0.137 ,
            1200 : 0.0985 ,
            1250 : 0.0715 ,
            1300 : 0.0522 ,
            1350 : 0.0384 ,
            1400 : 0.0284 ,
            1450 : 0.0211 ,
            1500 : 0.0157 ,
            1550 : 0.0118 ,
            1600 : 0.00887 ,
            1650 : 0.0067 ,
            1700 : 0.00507 ,
            1750 : 0.00385 ,
            1800 : 0.00293 ,
            1850 : 0.00224 ,
            1900 : 0.00171 ,
            1950 : 0.00131 ,
            2000 : 0.00101 ,
            2050 : 0.000776 ,
            2100 : 0.000598 ,
            2150 : 0.000461 ,
            2200 : 0.000356 ,
            2250 : 0.000275 ,
            2300 : 0.000213 ,
            2350 : 0.000165 ,
            2400 : 0.000128 ,
            2450 : 9.91e-05 ,
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

    def augment_lims(name,d):
        mass = int(name.split("_m")[1].split("_",1)[0])
        mult = 1.e3
        xsec = gluino_xsecs[mass]*mult
        obs = d.get("obs",-1)*xsec
        exp = d.get("exp",-1)*xsec
        exp_sm1 = d.get("sm1",-1)*xsec
        exp_sp1 = d.get("sp1",-1)*xsec
        exp_sm2 = d.get("sm2",-1)*xsec
        exp_sp2 = d.get("sp2",-1)*xsec
        return {
                "mass": mass,
                "xsec": xsec,
                "obs":obs, "exp":exp,
                "sp1":exp_sp1,"sm1":exp_sm1,
                "sp2":exp_sp2,"sm2":exp_sm2,
                "expr":d.get("exp",-1),
                "obsr":d.get("obs",-1),
                }


    with open(biglog, "r") as fh:
        parsed_logs = pickle.load(fh)
    keys = filter(lambda x: fnmatch.fnmatch(x,globber), parsed_logs.keys())
    infos = []
    for key in keys:
        d = parsed_logs[key]
        d = augment_lims(key,parsed_logs[key])
        print d
        infos.append(d)
    df = pd.DataFrame(infos)
    df = df.sort_values("mass")

    df["eodiff"] = ((df["obsr"]-df["expr"])/df["expr"])

    # print df["eodiff"].mean()
    # print df
    if "t1qqqql" in globber:
        print("EXCLUDING 1900 and 2300 points for t1qqqql")
        df = df[(df.mass != 1900)&(df.mass != 2300)]


    obs = (df["obs"]).values
    exp = (df["exp"]).values
    sm2 = (df["sm2"]).values
    sm1 = (df["sm1"]).values
    sp1 = (df["sp1"]).values
    sp2 = (df["sp2"]).values
    mgs = df["mass"].values
    theory = df["xsec"].values


    fig,ax = get_fig_ax()
    add_cms_info(ax, lumi="137")

    pe2 = ax.fill_between(mgs, sm2, sp2, linewidth=0., facecolor=YELLOW, alpha=1.0)
    pe1 = ax.fill_between(mgs, sm1, sp1, linewidth=0., facecolor=GREEN, alpha=1.0)
    pe0 = ax.plot(mgs, exp, linestyle="--", marker="",color="k",solid_capstyle="butt")
    pobs = ax.plot(mgs, obs, linestyle="-", markersize=5.,marker="o",color="k",solid_capstyle="butt")
    ptheory = ax.plot(mgs, theory, linestyle="-", marker="",color="r",solid_capstyle="butt")

    if do_tbs:
        procstr = "T1tbs"
        ax.set_yscale("log")
    else:
        procstr = "T1qqqqL"
        ax.set_yscale("log")
        ax.set_ylim([0.07,200.])

    legend = ax.legend(
            [
                (pobs[0],),
                DoubleBandObject(),
                (ptheory[0],),
                ],
            [
                "95% CL Observed",
                r"95% CL Expected $\pm$ 1 and $\pm$ 2 $\sigma_\mathrm{experiment}$",
                r"$\sigma_\mathrm{{theory}}$ (pp$\rightarrow\tilde{\mathrm{g}}\tilde{\mathrm{g}}$)",
                ],
            handler_map={DoubleBandObject: DoubleBandObjectHandler()},
            labelspacing=0.6,
            loc="upper center",
            )

    ax.set_title("")

    # fpath = os.path.join("/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/limits/arial.ttf")
    # prop = mfm.FontProperties(fname=fpath)
    # print prop
    # # ax.set_title("title", fontproperties=prop)
    # # ax.set_title("title", fontname="Helvetica")
    # # ax.set_title("title")

    if do_tbs:
        fname = "{}/rpv_t1tbs_run2.pdf".format(outdir)
    else:
        fname = "{}/rpv_t1qqqql_run2.pdf".format(outdir)
    ax.set_ylabel(r"$\sigma$ (fb)")
    ax.set_xlabel(r"$\mathrm{m}_\tilde{\mathrm{g}}$ (GeV)")
    fig.set_tight_layout(True)
    fname_png = fname.replace(".pdf",".png")
    fig.savefig(fname)
    fig.savefig(fname_png)
    os.system("ic {}".format(fname_png))
    # os.system("ic {}".format(fname))
    print "Saved {}".format(fname)

if __name__ == "__main__":

    set_defaults()

    # os.system("mkdir -p plots/")
    # make_yukawa_plot(scaninfo="ft_updated2018_run2_19Mar5/v3.28_ft_mar6unblind_v1//log_yukawa_scan.txt")
    # make_higgs_plot(basedir="v3.24_fthiggs_v1/",globber="card_higgs*_run2.log",do_scalar=True)
    # make_higgs_plot(basedir="v3.24_fthiggs_v1/",globber="card_higgs*_run2.log",do_scalar=False)
    # make_higgs_plot(basedir="ft_updated2018_run2_19Mar5/v3.28_ft_mar6higgs_v1/",globber="card_higgs*_run2.log",do_scalar=True)
    # make_higgs_plot(basedir="ft_updated2018_run2_19Mar5/v3.28_ft_mar6higgs_v1/",globber="card_higgs*_run2.log",do_scalar=False)

    # make_higgs_plot(basedir="ft_updated2018_run2_19Mar5/v3.28_ft_mar13ttwbbhiggs_v1//",globber="card_higgs*_run2.log",do_scalar=True)
    # make_higgs_plot(basedir="ft_updated2018_run2_19Mar5/v3.28_ft_mar13ttwbbhiggs_v1//",globber="card_higgs*_run2.log",do_scalar=False)
    # make_yukawa_plot(scaninfo="ft_updated2018_run2_19Mar5/v3.28_ft_mar13ttwbb_v1/log_yukawa_scan.txt")

    # os.system("mkdir -p scanplots/")
    # make_rpv_plot(basedir="v3.26_feb15_sst1t5rpv_v1/",globber="card_rpv_t1tbs_*_all_run2.log",do_tbs=True)
    # make_rpv_plot(basedir="v3.26_feb15_sst1t5rpv_v1/",globber="card_rpv_t1qqqql_*_all_run2.log",do_tbs=False)

#     os.system("mkdir -p scanplots_test/")
#     make_rpv_plot(basedir="v3.26_feb27_allsigs_v1",outdir="scanplots_test/",globber="card_rpv_t1tbs_*_all_run2.log",do_tbs=True)
#     make_rpv_plot(basedir="v3.26_feb27_allsigs_v1",outdir="scanplots_test/",globber="card_fs_t1qqqql_*_all_run2.log",do_tbs=False)

    # # Mar 13, latest -- had 1.7pm0.85 ttbb/ttjj (extra 30% on ttw+bb)
    # make_higgs_plot(basedir="ft_updated2018_run2_19Mar5/v3.28_ft_mar13ttwbbhiggs_v1//",globber="card_higgs*_run2.log",do_scalar=True)
    # make_higgs_plot(basedir="ft_updated2018_run2_19Mar5/v3.28_ft_mar13ttwbbhiggs_v1//",globber="card_higgs*_run2.log",do_scalar=False)
    # make_yukawa_plot(scaninfo="ft_updated2018_run2_19Mar5/v3.28_ft_mar13ttwbb_v1/log_yukawa_scan.txt")

    # # NOTE PAS
    # # Mar 18, latest -- back to nominal 1.7pm0.6 ttbb/ttjj
    # make_higgs_plot(basedir="ft_updated2018_run2_19Mar5/v3.28_ft_mar18higgs_v2/",globber="card_higgs*_run2.log",do_scalar=True)
    # make_higgs_plot(basedir="ft_updated2018_run2_19Mar5/v3.28_ft_mar18higgs_v2/",globber="card_higgs*_run2.log",do_scalar=False)
    # make_yukawa_plot(scaninfo="ft_updated2018_run2_19Mar5/v3.28_ft_mar18nominal_v1/log_yukawa_scan.txt")

    # make_rpv_plot(biglog="batch/biglog_v3.pkl",outdir="scanplots_test/",globber="*rpv_t1tbs_*",do_tbs=True)
    # make_rpv_plot(biglog="batch/biglog_v3.pkl",outdir="scanplots_test/",globber="*fs_t1qqqql_*",do_tbs=False)

    # from plotSMS import parse_logs
    # parse_logs("batch/biglog_v4.txt")
    # make_rpv_plot(biglog="batch/biglog_v4.pkl",outdir="scanplots_test/",globber="*rpv_t1tbs_*",do_tbs=True)
    # make_rpv_plot(biglog="batch/biglog_v4.pkl",outdir="scanplots_test/",globber="*fs_t1qqqql_*",do_tbs=False)

    # # new Higgs xsecs (May6) -- CWR/paper for four top
    # make_higgs_plot(basedir="test_ft_updated2018_run2_19Mar5/v3.28_ft_test_Apr30_higgs_v1/",globber="card_higgs*_run2.log",do_scalar=True)
    # make_higgs_plot(basedir="test_ft_updated2018_run2_19Mar5/v3.28_ft_test_Apr30_higgs_v1/",globber="card_higgs*_run2.log",do_scalar=False)
    # make_higgs_plot(basedir="test_ft_updated2018_run2_19Mar5/v3.28_ft_test_Apr30_higgs_v1/",globber="card_higgs*_run2.log",do_sum=True)
    # make_yukawa_plot(scaninfo="ft_updated2018_run2_19Mar5/v3.28_ft_mar18nominal_v1/log_yukawa_scan.txt")

    # from plotSMS import parse_logs
    # parse_logs("batch/biglog_v6.txt")
    make_rpv_plot(biglog="batch/biglog_v6.pkl",outdir="scanplots_Jun28/",globber="*rpv_t1tbs_*",do_tbs=True)
    make_rpv_plot(biglog="batch/biglog_v6.pkl",outdir="scanplots_Jun28/",globber="*fs_t1qqqql_*",do_tbs=False)
