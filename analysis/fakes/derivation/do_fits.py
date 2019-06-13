import os
# import ROOT as r
import itertools
import numpy as np

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
import matplotlib
from matplottery.plotter import plot_2d, plot_stack
from matplottery.utils import Hist2D, Hist1D
import uproot
# from plottery.utils import move_in_overflows
from scipy.optimize import minimize

np.set_printoptions(linewidth=200, precision=2)


def fit_and_plot(inputdir="outputs_19Jan28", outputdir="plots/",years=[2016,2017,2018]):

    # flav = "mu"
    # proc = "data_{}".format(flav)
    d = {}
    for iso in [True,False]:
        d[iso] = {}
        isostr = "_IsoTrigs" if iso else ""
        for year in years:
        # for year in [2018]:
            d[iso][year] = {}
            for flav in ["mu","el"]:
            # for flav in ["mu"]:
                for proc in [
                        "data_{}".format(flav),
                        "qcd_{}".format(flav),
                        "qcddata_{}".format(flav),
                        "dy",
                        "wjets",
                        "ttjets",
                        ]:
                    if "qcddata" in proc:
                        f = uproot.open("{}/y{}_rate_histos_{}_LooseEMVA{}.root".format(inputdir,year,proc.replace("qcddata","qcd"),isostr))
                        h = Hist1D(f["histo_mt_cr_noiso_{}".format(flav)],label=proc)
                    else:
                        f = uproot.open("{}/y{}_rate_histos_{}_LooseEMVA{}.root".format(inputdir, year,proc,isostr))
                        h = Hist1D(f["histo_mt_cr_{}".format(flav)],label=proc)
                    proc = proc.split("_")[0]
                    if proc not in d[iso][year]: d[iso][year][proc] = {}
                    d[iso][year][proc][flav] = h

    d_infos = {}

    for iso, year, flav, qcd_template in itertools.product(
            [True,False],
            years,
            ["mu","el"],
            ["qcd","qcddata"],
            # [True],
            # [2017],
            # ["mu"],
            # ["qcd"],
            ):

        # flav = "mu"
        # qcd_template = "qcd"
        # # qcd_template = "qcddata"
        # year = 2017
        # iso = True

        hdata = Hist1D(d[iso][year]["data"][flav],label="Data")
        hqcd = Hist1D(d[iso][year][qcd_template][flav],label="QCD template ({})".format("data" if "data" in qcd_template else "MC"),color=(0.9,0.9,0.9))
        hewk = Hist1D(d[iso][year]["dy"][flav]+d[iso][year]["wjets"][flav]+d[iso][year]["ttjets"][flav],label=r"EWK (W+DY+$t\bar{t}$)",color=(1.,0.6,0.2))
        hdy = Hist1D(d[iso][year]["dy"][flav],label=r"DY",color=(0.3,0.8,0.8))
        hwjets = Hist1D(d[iso][year]["wjets"][flav],label=r"W+jets",color=(1.,0.6,0.2))
        httjets = Hist1D(d[iso][year]["ttjets"][flav],label=r"$t\bar{t}$",color=(1.,0.4,0.4))
        # hewk = Hist1D(hdy+hwjets+httjets,label=r"EWK (W+DY+$t\bar{t}$)",color=(1.,0.6,0.2))
        hewk = Hist1D(hdy+hwjets,label=r"EWK (W+DY)",color=(1.,0.6,0.2))
        # hists = [hqcd,hewk]

        def do_fit(hdata,hqcd,hewk):
            def calc_chi2(args):
                sf_qcd,sf_ewk = args
                nbinlow,nbinhigh = None,None
                # nbinlow,nbinhigh = 5,None
                bgerrs = (sf_qcd*hqcd.errors)**2. + (sf_ewk*hewk.errors)**2.
                bgcounts = (sf_qcd*hqcd.counts+sf_ewk*hewk.counts)
                datacounts = hdata.counts
                dataerrs = hdata.errors
                # tot_err2 = (dataerrs**2. + bgerrs**2.)
                tot_err2 = (dataerrs**2.) # ignore bg errs
                tot_diff2 = (datacounts - bgcounts)**2.
                # chi2 = np.log(np.sum(tot_diff2[nbinlow:nbinhigh]/tot_err2[nbinlow:nbinhigh]**.5))
                chi2 = (np.sum(tot_diff2[nbinlow:nbinhigh]/tot_err2[nbinlow:nbinhigh]**.5))
                return chi2

            # triplets = []
            # for x in np.arange(0.7,1.4,0.005):
            #     for y in np.arange(0.7,1.4,0.005):
            #         z = calc_chi2([x,y])
            #         triplets.append([x,y,z])
            # triplets = np.array(triplets)
            # print triplets[triplets[:,2].argsort()]


            res = minimize(calc_chi2,
                    [
                        1.0,
                        1.31,
                        ],
                    # bounds=[
                    #     # (0.3,3.0),
                    #     # (0.8,1.5),
                    #     ],
                    # method="SLSQP",
                    method='Nelder-Mead',
                    # method='Powell',
                    tol=1e-6,
                    )
            # print res
            # print res.x
            sf_qcd,sf_ewk = res.x
            return [sf_qcd, sf_ewk]

        sf_qcd,sf_ewk = do_fit(hdata,hqcd,hewk)
        print year,iso,flav,qcd_template,sf_ewk,sf_qcd
        # sf_qcd,sf_ewk = 0.3,1.17
        if year not in d_infos: d_infos[year] = {}
        d_infos[year][(flav,iso,("data" in qcd_template))] = [sf_ewk, sf_qcd]
        # hewk.set_attr("label", "{:.2f} x {}".format(sf_ewk,hewk.get_attr("label")))
        # hqcd.set_attr("label", "{:.2f} x {}".format(sf_qcd,hqcd.get_attr("label")))
        hists = [
                # sf_ewk*hdy,
                # sf_ewk*httjets,
                sf_qcd*hqcd,
                # sf_ewk*hwjets,
                sf_ewk*hewk,
                ]
        # print hdata
        title = "{}, {}".format(
                "$\mu$" if flav == "mu" else "e",
                "iso" if iso else "noniso",
                )
        fname = "{}/y{}_{}_{}_mt_lin_postfit_{}temp.pdf".format(
                outputdir,
                year,
                flav,
                "iso" if iso else "noniso",
                "data" if "data" in qcd_template else "mc",
                )
        opts = dict(
                xlabel = "$m_{T}$ [GeV]",
                ylabel = "Entries",
                mpl_hist_params = dict(),
                cms_type = "Preliminary",
                filename = fname,
                title = title,
                ratio_range = [0.0,2.0],
                lumi = {"2016":"35.9","2017":"41.5","2018":"58.8"}[str(year)],
                do_bkg_syst = True,
                # do_log=True,
                ax_main_callback = lambda ax: ax.add_artist(
                    matplotlib.offsetbox.AnchoredText(
                        # "QCD scaled by {:.2f}\nEWK scaled by {:.2f}".format(sf_qcd,sf_ewk),
                        "EWK scaled by {:.2f}\nQCD scaled by {:.2f}".format(sf_ewk,sf_qcd),
                        loc="center right",
                        frameon=False,
                        prop=dict(color="red"),
                        )
                    ),
                )
        plot_stack(data=hdata, bgs=hists, **opts)
        # os.system("ic {}".format(fname))
    return d_infos

def print_infos(d_infos):

    for year,d_sfs in sorted(d_infos.items()):
        # d_sfs = d_infos[year]
        # print "TString dir = \"{}\";".format(basedir)
        print "int year = {};".format(year)
        print "float mt_sf_el_iso =    {:.3f};".format(d_sfs[("el",True,False)][0])
        print "float mt_sf_el_noniso = {:.3f};".format(d_sfs[("el",False,False)][0])
        print "float mt_sf_mu_iso =    {:.3f};".format(d_sfs[("mu",True,False)][0])
        print "float mt_sf_mu_noniso = {:.3f};".format(d_sfs[("mu",False,False)][0])

    for year,d_sfs in sorted(d_infos.items()):
        # d_sfs[(flavstr,iso,use_inviso_data_template)]
        print r"""
\multirow{{2}}{{*}}{{{year}}} & MC        & {mcisoe:.3f} & {mcisom:.3f} & {mcnonisoe:.3f} & {mcnonisom:.3f} \\
        & data      & {dataisoe:.3f} & {dataisom:.3f} & {datanonisoe:.3f} & {datanonisom:.3f} \\ \hline
    """.format(
            year=year,
            mcisoe=d_sfs[("el",True,False)][0],
            mcisom=d_sfs[("mu",True,False)][0],
            mcnonisoe=d_sfs[("el",False,False)][0],
            mcnonisom=d_sfs[("mu",False,False)][0],
            dataisoe=d_sfs[("el",True,True)][0],
            dataisom=d_sfs[("mu",True,True)][0],
            datanonisoe=d_sfs[("el",False,True)][0],
            datanonisom=d_sfs[("mu",False,True)][0],
            )

if __name__ == "__main__":
    d_infos = fit_and_plot(inputdir="outputs_19Jan28/",outputdir="outputs_19Jan28/plots_mtfits/")
    # print_infos(d_infos)
