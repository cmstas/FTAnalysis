import os
import numpy as np
import uproot

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery.plotter import plot_stack
from matplottery.utils import Hist1D, MET_LATEX

import matplotlib

os.system("mkdir -p plots/")



def make_plots(outputdir="plots", inputfile="outputs/histos.root", prefix="", lumi=-1, year=2016):

    os.system("mkdir -p {}/".format(outputdir))

    f = uproot.open(inputfile)

    obs = Hist1D(f["clos_nbtags_plot_data"])
    pred = Hist1D(f["clos_nbtags_plot_MC"])
    obsMC = Hist1D(f["clos_nbtags_plot_MCp"])
    sf = float(obs.get_integral() / pred.get_integral())
    sf_mc = float(obsMC.get_integral() / obs.get_integral())
    print "(obs dataSS/pred dataSS) = {:.2f}, (obs MCSS/obs dataSS) = {:.2f}".format(sf, sf_mc)

    for var, xlabel in [
            ["met", MET_LATEX],
            ["rawmet", "raw "+MET_LATEX],
            ["ht", "$H_{T}$"],
            ["njets", "Njets"],
            ["nbtags", "Nbtags"],
            ["leppt", "$p_T$(lep)"],
            ["lepptbarrel", "$p_T$(barrel lep)"],
            ["lepptendcap", "$p_T$(endcap lep)"],
            ["lepeta", "$\\eta$(lep)"],
            ["lepphi", "$\\phi$(lep)"],
            ["dlepphi", "$\\Delta\\phi$"],
            ["mll", "$m_{ll}$"],
            ["nvtx", "# good vertices"],
            # ["lep1nmiss", "lep1 nmiss"],
            # ["lep2nmiss", "lep2 nmiss"],
            ]:

        obs = Hist1D(f["clos_{}_plot_data".format(var)], label="obs SS (data)", no_overflow=True)
        pred = Hist1D(f["clos_{}_plot_MC".format(var)], label="pred SS (data)", color=(28/255.,168/255.,19/255.), no_overflow=True)
        obsMC = Hist1D(f["clos_{}_plot_MCp".format(var)], label="obs SS (MC)", color=(68/255.,61/255.,165/255.), no_overflow=True)
        pred *= sf
        ratio = obs/pred

        fname = "{}/{}{}.pdf".format(outputdir,prefix,var)
        plot_stack(bgs=[pred], sigs=[obsMC],data=obs, xlabel=xlabel, ylabel="Events",filename=fname,
                cms_type = " Preliminary",
                lumi = lumi,
                ratio=ratio,
                ratio_range=[0.5,1.5],
                title=year,
                mpl_hist_params={
                    "linewidth": 0.5,
                    "linestyle": "-",
                    "edgecolor": "k",
                    },
                mpl_ratio_params={
                    "label":"obs/pred",
                    },
                mpl_sig_params={
                    "hist": False,
                    },
                ax_main_callback = lambda ax: ax.add_artist(
                    matplotlib.offsetbox.AnchoredText(
                        "Pred scaled by {:.2f}".format(sf),
                        loc="center right",
                        frameon=False,
                        prop=dict(color="red"),
                        )
                    ),
                # ax_ratio_callback = lambda ax: ax.legend_.remove(),
                )
        # os.system("ic {}".format(fname))
        print "Wrote {}".format(fname)

        data = Hist1D(f["osee_{}_plot_data".format(var)], label="Data")
        mc = Hist1D(f["osee_{}_plot_MC".format(var)], label="MC", color=(0.4, 0.6, 1.0))
        sf_os = data.get_integral() / mc.get_integral()
        fname = "{}/{}osee_{}.png".format(outputdir,prefix,var)
        ratio_range=[0.5,1.5]
        if "met" in var.lower():
            ratio_range=[0.8,1.2]
        plot_stack(data=data, bgs=[mc], xlabel=xlabel, ylabel="Events",filename=fname,
                cms_type = "Preliminary",
                lumi = lumi,
                title=year,
                ratio_range=ratio_range,
                do_bkg_syst=True,
                ax_main_callback = lambda ax: ax.add_artist(
                    matplotlib.offsetbox.AnchoredText(
                        "$\\frac{\\mathrm{data}}{\\mathrm{MC}} = %.2f$" % sf_os,
                        loc="center right",
                        frameon=False,
                        prop=dict(color="b"),
                        )
                    ),
                # ax_ratio_callback = lambda ax: ax.legend_.remove(),
                )
        # os.system("ic {}".format(fname))
        print "Wrote {}".format(fname)

if __name__ == "__main__":

    # make_plots("plots", "outputs/histos_2016.root", year=2016,  prefix="y2016_", lumi="35.9")
    # make_plots("plots", "outputs/histos_2017.root", year=2017,  prefix="y2017_", lumi="41.5")
    # make_plots("plots", "outputs/histos_2018.root", year=2018,  prefix="y2018_", lumi="58.8")

    make_plots("plots", "outputs/histos_2016.root", year=2016,  prefix="y201694x_", lumi="35.9")

    # make_plots("plots", "outputs/histos_2016_mu.root", year=2016,  prefix="mu_y2016_", lumi="35.9")
    # make_plots("plots", "outputs/histos_2017_mu.root", year=2017,  prefix="mu_y2017_", lumi="41.5")
    # make_plots("plots", "outputs/histos_2018_mu.root", year=2018,  prefix="mu_y2018_", lumi="58.8")

    # make_plots("plots", "outputs/histos_2016_94x.root", year="2016 (94x)",  prefix="y201694x_", lumi="35.9")
    # make_plots("plots", "outputs/histos_2016_tail.root", year="2016 (tail)",  prefix="y2016tail_", lumi="35.9")
    # make_plots("plots", "outputs/histos_2016_mu.root", year="2016 (mu)",  prefix="y2016mu_", lumi="35.9")
    # make_plots("plots", "outputs/histos_2017_mu.root", year="2017 (mu)",  prefix="y2017mu_", lumi="41.5")

    # make_plots("plots", "outputs/histos.root",   prefix="all_")
    # make_plots("plots", "outputs/histos_B.root", prefix="Run2017B_")
    # make_plots("plots", "outputs/histos_C.root", prefix="Run2017C_")
    # make_plots("plots", "outputs/histos_D.root", prefix="Run2017D_")
    # make_plots("plots", "outputs/histos_E.root", prefix="Run2017E_")
    # make_plots("plots", "outputs/histos_F.root", prefix="Run2017F_")
            
