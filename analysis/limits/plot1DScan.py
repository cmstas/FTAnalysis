#!/usr/bin/env python

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

import os
import numpy as np
import argparse

from scipy.interpolate import CubicSpline

from matplottery.plotter import set_defaults, add_cms_info

def plot_one(ax,fname,poi_name="r",poi_scale=1.,fmt=".3f",unit="",label="Expected"):
    f1 = r.TFile(fname)
    t = f1.Get("limit")

    points = []
    for evt in t:
        if 2*evt.deltaNLL > 8.: continue
        if evt.quantileExpected > -1.5:
            points.append((evt.r*poi_scale, 2*evt.deltaNLL))
    points = np.array(sorted(points))
    def get_roots(points,k):
        return sorted(CubicSpline(points[:,0],points[:,1]-k).roots())
    y_1sigma = 1.
    y_2sigma = 4.
    r_center = get_roots(points,0.)[1]
    r_1sigma = get_roots(points,y_1sigma)
    r_2sigma = get_roots(points,y_2sigma)

    rtext = "%%s = ${%%%s}^{+%%%s}_{-%%%s}$ %s" % (fmt,fmt,fmt,unit if unit else "")
    # rtext = "%s = ${%.3f}^{+%.3f}_{-%.3f}$" % (poi_name, r_center,r_center-r_1sigma[0],r_1sigma[1]-r_center)
    rtext = rtext % (poi_name, r_center,r_center-r_1sigma[0],r_1sigma[1]-r_center)
    ax.text(0.99, 0.97,rtext, horizontalalignment='right', verticalalignment='top', transform = ax.transAxes, size="xx-large")

    ax.plot(points[:,0],points[:,1], color="black",marker="o",markersize=3,linewidth=1.5, label=label)
    line_opts = {"color":"gray","linewidth":0.5,"linestyle":"-"}
    ax.axhline(y_1sigma,**line_opts)
    ax.axhline(y_2sigma,**line_opts)
    ax.plot([r_1sigma[0],r_1sigma[0]],[0.,y_1sigma],**line_opts)
    ax.plot([r_1sigma[1],r_1sigma[1]],[0.,y_1sigma],**line_opts)
    ax.plot([r_2sigma[0],r_2sigma[0]],[0.,y_2sigma],**line_opts)
    ax.plot([r_2sigma[1],r_2sigma[1]],[0.,y_2sigma],**line_opts)
    xmin,xmax = ax.get_xlim()
    ax.set_ylim([0., points[:,1].max()*1.3])
    ax.set_xlim([0., xmax])
    ax.set_xlabel(poi_name+(" [{}]".format(unit) if unit else ""))
    ax.set_ylabel(r"$-2\Delta$ ln L")
    ax.set_title(" ")

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("scandata", help="scandata.root file name")
    parser.add_argument("-l", "--label", help="label", default="Expected",type=str)
    parser.add_argument("-L", "--lumi", help="label", default="136.3",type=str)
    parser.add_argument("-x", "--xsec", help="scale and rename POI for xsec", action="store_true")
    args = parser.parse_args()

    import ROOT as r

    poi_name = "r"
    poi_scale = 1.
    unit = ""
    fmt = ".3f"
    if args.xsec:
        poi_name = r"$\sigma$"
        poi_scale = 11.97
        unit = "fb"
        fmt = ".2f"

    set_defaults()

    fig, ax = plt.subplots(nrows=1, ncols=1, figsize=(6,5))
    # fig, ax = plt.subplots(nrows=1, ncols=1)

    # plot_one(ax,fname="original_cards_2017_94X_mcfakes_75ifb/scandata.root", poi_name=poi_name, poi_scale=poi_scale,fmt=".3f",unit="",label="Expected")
    plot_one(ax,fname=args.scandata, poi_name=poi_name, poi_scale=poi_scale,fmt=fmt,unit=unit,label=args.label)

    add_cms_info(ax, typ="Preliminary", lumi=args.lumi)

    ax.legend(loc="upper left",fontsize="x-large")

    if args.xsec:
        ax.xaxis.set_minor_locator(mpl.ticker.MultipleLocator(base=1))
        ax.yaxis.set_minor_locator(mpl.ticker.MultipleLocator(base=0.5))
    else:
        ax.xaxis.set_minor_locator(mpl.ticker.MultipleLocator(base=0.1))
        ax.yaxis.set_minor_locator(mpl.ticker.MultipleLocator(base=0.5))



    fig.tight_layout()
    basedir = args.scandata.rsplit("/",1)[0]
    fname = "./{}/scanplot.pdf".format(basedir)
    print "Saved {}".format(fname)
    fig.savefig(fname)
    os.system("ic "+fname)
