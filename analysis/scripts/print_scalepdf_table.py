import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')

import os
import numpy as np
import pandas as pd

import matplotlib as mpl
mpl.use('Agg')
from matplotlib import rcParams
import matplotlib.pyplot as plt
from matplottery.utils import Hist1D

import uproot
import itertools

np.set_printoptions(linewidth=150)

def hist(ax, h,**kwargs):
    counts = h.counts
    yerrs = h.errors
    xerrs = 0.5*h.get_bin_widths()
    centers = h.get_bin_centers()
    do_text = kwargs.pop("text",False)
    good = counts != 0.
    patches = ax.errorbar(centers[good],counts[good],xerr=xerrs[good],yerr=yerrs[good],**kwargs)
    if do_text:
        for x,y,yerr in zip(centers[good],counts[good],yerrs[good]):
            ax.text(x,y+yerr,"{:.2f}".format(y), horizontalalignment="center",verticalalignment="bottom", fontsize=12, color=patches[0].get_color())
    return patches

# year = 2016
# # year = 2017
# # sel = arrs["br"] & (arrs["hyp_class"]==3)
# # get 3 numbers -- BR, SRCR, SR
# # for pdf, scale
# # for 2016, 2017
# # for all processes
# which = "scale"
# region = "br"
# # region = "sr"
# sample = "TTWnlo"
# # region = "srcr"
# # region = "sr"
# # varname = "sr"
# varname = "njets"
# # which = "pdf"

def make_plots_and_df():
    data = []
    for year in [2016,2017]:
        for sample,nicesample in [
                ("TTTTnew", "tttt"),
                ("TTWnlo", "ttW"),
                ("TTZnlo", "ttZ"),
                ("TTHtoNonBB", "ttH"),
                ("TTTJ", "tttJ"),
                ("TTTW", "tttW"),
                ("TTWH", "ttWH"),
                ("TTWW", "ttWW"),
                ("TTWZ", "ttWZ"),
                ("TTZH", "ttZH"),
                ("TTZZ", "ttZZ"),
                ("TTHH", "ttHH"),
                ("WWW", "WWW"),
                ("WWZ", "WWZ"),
                ("WZZ", "WZZ"),
                ("ZZZ", "ZZZ"),
                ("WWDPS", "WWDPS"),
                ("QQWW", "WpWpJJ"),
                ("ZZ", "ZZ"),
                ("TZQ", "tZq"),
                ("TWZ", "tWZ"),
                ("GGHtoZZto4L", "HtoZZ"),
                ("VHtoNonBB", "V+H"),
                ("WWG", "WWGamma"),
                ("WZG", "WZgamma"),
                ("TGext", "tGamma"),
                ("TTGdilep", "ttGamma2L"),
                ("TTGsinglelep", "ttGamma1L"),
                ("TTGsinglelepbar", "tGamma1Lbar"),
                ]:

            for which,region,varname in list(itertools.product(
                ["scale","pdf"],
                ["br","sr","srcr"],
                ["njets","sr"],
                # ["scale"],
                # ["br"],
                # ["njets"],
                )):


                # f = uproot.open("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.11_scaleunc/output/year_2017/TTWnlo.root")
                # f = uproot.open("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.11_scaleunc/output/year_2016/TTWnlo.root")
                # f = uproot.open("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.11_scalepdfunc/output/year_2017/TTWnlo.root")
                # fname_in = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.11_scalepdfunc/output/year_{}/{}.root".format(year,sample)
                fname_in = "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.13_all/output/year_{}/{}.root".format(year,sample)
                if not os.path.exists(fname_in):
                    print "File for {} doesn't exist in {}".format(sample,year)
                    continue
                f = uproot.open(fname_in)
                norms = Hist1D(f["weight"])
                t = f["t"]
                arrs = t.arrays(["br","scale1fb","weight_scale_UP","weight_scale_DN","hyp_class","sr","weight_pdf_UP","weight_pdf_DN"]+[varname])

                if region == "srcr":
                    sel = (arrs["br"]) & (arrs["sr"] > 0) & (arrs["hyp_class"]==3)
                elif region == "sr":
                    sel = (arrs["br"]) & (arrs["sr"] > 2) & (arrs["hyp_class"]==3)
                elif region == "br":
                    sel = (arrs["br"]) & (arrs["hyp_class"]==3)

                var = arrs[varname][sel]
                # var = arrs["ht"][sel]
                lumi = 35.9 if year == 2016 else 41.5
                weights_nom = lumi*arrs["scale1fb"][sel]
                if which == "scale":
                    wup = arrs["weight_scale_UP"][sel]
                    wdown = arrs["weight_scale_DN"][sel]
                    wup[wup < -9000] = 1.
                    wdown[wdown < -9000] = 1.
                    weights_up = lumi*arrs["scale1fb"][sel]* wup * norms.counts[0]/norms.counts[1]
                    weights_down = lumi*arrs["scale1fb"][sel]* wdown * norms.counts[0]/norms.counts[2]
                elif which == "pdf":
                    wup = arrs["weight_pdf_UP"][sel]
                    wdown = arrs["weight_pdf_DN"][sel]
                    wup[wup < -9000] = 1.
                    wdown[wdown < -9000] = 1.
                    weights_up = lumi*arrs["scale1fb"][sel]* wup * norms.counts[0]/norms.counts[3]
                    weights_down = lumi*arrs["scale1fb"][sel]* wdown * norms.counts[0]/norms.counts[4]

                if not len(var):
                    print "no events for {}".format(sample)
                    continue

                bins = np.arange(0.5,max(var.max()+1.5,5.5),1.)
                # bins = np.linspace(0.5,var.max()+1.5,50)
                var_nom = Hist1D(var, weights=weights_nom, bins=bins)
                var_up = Hist1D(var, weights=weights_up, bins=bins)
                var_down = Hist1D(var, weights=weights_down, bins=bins)

                if var_nom.get_integral() < 1.e-6:
                    print "no events for {}".format(sample)
                    continue

                # var_up *= var_nom.get_integral()/var_up.get_integral()
                # var_down *= var_nom.get_integral()/var_down.get_integral()

                int_nom = var_nom.get_integral()
                int_up = var_up.get_integral()
                int_down = var_down.get_integral()
                pct_up = 100.*(int_up/int_nom-1.)
                pct_down = 100.*(int_down/int_nom-1.)
                normfact_up = var_nom.counts.sum()/var_up.counts.sum()
                normfact_down = var_nom.counts.sum()/var_down.counts.sum()
                pctresids = 100.0*np.r_[np.abs(normfact_up*var_up.counts/var_nom.counts-1),np.abs(normfact_down*var_down.counts/var_nom.counts-1)]
                medpctresids = np.median(pctresids[np.isfinite(pctresids)])


                fig, ax = plt.subplots()

                # ax.set_yscale("log",nonposy="clip")

                hist(ax,var_nom,lw=1.5,fmt="o",markersize=5.0,label="nominal [N={:.2f}]".format(int_nom),text=False,color="k")
                hist(ax,var_up,lw=1.5,fmt="o",markersize=5.0,label="{} up [{:+.1f}%]".format(which,pct_up),text=False,color="C0")
                hist(ax,var_down,lw=1.5,fmt="o",markersize=5.0,label="{} down [{:+.1f}%]".format(which,pct_down),text=False,color="C3")
                # print var_nom
                ax.set_xlabel(varname)
                ax.set_title("[{}] {} acceptance+shape, {}".format(year,which,nicesample))
                ax.legend()
                fig.tight_layout()
                fname = "plots/y{}_{}_{}_{}.pdf".format(year,which,region,sample)
                fig.savefig(fname)
                fig.savefig(fname.replace(".pdf",".png"))
                # os.system("ic "+fname)

                print varname, which, sample, nicesample, year, region, int_nom, pct_up, pct_down, medpctresids
                if varname == "sr":
                    data.append([
                        year, sample, nicesample, region, which, int_nom, pct_up, pct_down, medpctresids
                        ])

    df = pd.DataFrame(data, columns=["year", "sample", "nicesample", "region", "which", "yield", "accup", "accdown", "shapemedian"])
    df.to_pickle("df.pkl")

if __name__ == "__main__":

    if not os.path.exists("df.pkl"):
        make_plots_and_df()
    df = pd.read_pickle("df.pkl")
    df = df.round(pd.Series([3,1,1,1],index=["yield","accup","accdown","shapemedian"]))
    df = df.drop(columns=["sample"])
    # print df.head()
    # print df
    # df = df[df["region"] == "sr"]
    df = df[df["region"] == "srcr"]
    df_pdf_2016 = df[(df["year"] == 2016) & (df["which"] == "pdf")].drop(columns=["year"])
    df_pdf_2017 = df[(df["year"] == 2017) & (df["which"] == "pdf")].drop(columns=["year"])
    df_scale_2016 = df[(df["year"] == 2016) & (df["which"] == "scale")].drop(columns=["year"])
    df_scale_2017 = df[(df["year"] == 2017) & (df["which"] == "scale")].drop(columns=["year"])
    # print df.sort_values("int_nom")
    # print df_pdf_2016.sort_values("int_nom",ascending=False)
    df_pdf = df_pdf_2016.merge(df_pdf_2017,suffixes=["_pdf_2016","_pdf_2017"],on=["region","which","nicesample"],how="outer")
    df_scale = df_scale_2016.merge(df_scale_2017,suffixes=["_scale_2016","_scale_2017"],on=["region","which","nicesample"],how="outer")
    print df_pdf
    print df_scale

    df_merged = df_pdf.merge(df_scale,on=["nicesample"])

    from analysis.utils.pytable import Table
    tab = Table()
    # tab.set_theme_basic()
    # tab.set_theme_fancy()
    tab.set_theme_csv()

    for index,row in df_merged.iterrows():
        outrow = [
                row["nicesample"],
                row["yield_scale_2016"],
                row["accup_scale_2016"],
                row["accdown_scale_2016"],
                row["shapemedian_scale_2016"],
                row["accup_pdf_2016"],
                row["accdown_pdf_2016"],
                row["shapemedian_pdf_2016"],
                row["yield_scale_2017"],
                row["accup_scale_2017"],
                row["accdown_scale_2017"],
                row["shapemedian_scale_2017"],
                row["accup_pdf_2017"],
                row["accdown_pdf_2017"],
                row["shapemedian_pdf_2017"],
                ]
        tab.add_row(outrow)

    tab.print_table(show_row_separators=False,show_alternating=False)

    # print df.sort_values("pct_up")
