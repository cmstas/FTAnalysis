#!/usr/bin/env python

from getPostFit import get_dict
import glob
import ROOT as r
import numpy as np
import argparse

from pprint import pprint

from errors import E


def parse_yields(d_in):
    d_yields = {}
    for key in d_in:
        h1 = d_in[key]
        if not h1: continue
        vals,errs = zip(*[[h1.GetBinContent(ib),h1.GetBinError(ib)] for ib in range(1,h1.GetNbinsX()+1)])
        d_yields[key] = {}
        d_yields[key]["central"] = vals
        d_yields[key]["error"] = errs
    return d_yields

def print_table(d_yields, regions="srcr", pretty=False,unblinded=False):
    nbins = len(d_yields["ttz"]["central"])
    # colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","$\\ttVV$","X+$\\gamma$","Rares","Flips","Fakes","Total","Data","$\\tttt$"]
    # procs = ["ttw","ttz","tth","ttvv","xg","rares","flips","fakes","total_background","data","tttt"]

    # colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","$\\ttVV$","X+$\\gamma$","Rares","Flips","Fakes","Total","Data","$\\tttt$", "tot s+b"]
    # allprocs = [["ttw"],["ttz"],["tth"],["ttvv"],["xg"],["rares"],["flips"],["fakes"],["total_background"],["data"],["tttt"],["total"]]

    # colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","$\\ttVV$","X+$\\gamma$","Rares","Flips","FakesMC","Total","Data","$\\tttt$", "tot s+b"]
    # allprocs = [["ttw"],["ttz"],["tth"],["ttvv"],["xg"],["rares"],["flips"],["fakes_mc"],["total_background"],["data"],["tttt"],["total"]]

    colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","Others","Total","Data","$\\tttt$", "tot s+b"]
    allprocs = [["ttw"],["ttz"],["tth"],["ttvv","xg","rares","flips","fakes_mc"],["total_background"],["data"],["tttt"], ["total"]]

    # srnames = ["CRZ","CRW","SR1","SR2","SR3","SR4","SR5","SR6","SR7","SR8"]
    if regions == "srcr":
        srnames = ["CRZ","CRW"]+["SR{}".format(i) for i in range(1,17)]
    elif regions == "srdisc":
        # srnames = ["SR{}".format(i) for i in range(1,15)]
        srnames = ["SR{}".format(i) for i in range(1,14)]

    if not unblinded:
        d_yields["data"]["central"] *= 0
        d_yields["data"]["error"] *= 0

    if not pretty:

        for ibin in range(nbins):
            # print ibin
            if ibin == 0:
                print "&".join(map(lambda x: "{0:12s}".format(x),colnames)),
                print r"\\"
                print r"\hline\hline"

            tojoin = [srnames[ibin]]
            for procs in allprocs:
                tot_ve = E(0.,0.)
                for subproc in procs:
                    ve = E(max(d_yields[subproc]["central"][ibin],0.), d_yields[subproc]["error"][ibin])
                    tot_ve += ve
                cent,err = tot_ve
                if "data" in procs:
                    if unblinded:
                        tojoin.append("{0:.0f}".format(cent))
                    else:
                        tojoin.append("-")
                else:
                    tojoin.append("{0:5.2f}$\\pm${1:5.2f}".format(cent,err))
            print " & ".join(tojoin),
            print r"\\"

    else:

        from analysis.utils.pytable import Table
        tab = Table()
        tab.set_column_names([cname.replace("$","").replace("\\","") for cname in colnames])
        tab.set_theme_basic()
        sep = u"\u00B1".encode("utf-8")
        precision = 2
        # sep = "+-"

        for ibin in range(nbins):
            row = [srnames[ibin]]
            for procs in allprocs:
                tot_ve = E(0.,0.)
                for subproc in procs:
                    ve = E(max(d_yields[subproc]["central"][ibin],0.), d_yields[subproc]["error"][ibin])
                    tot_ve += ve
                cent,err = tot_ve
                if "data" in procs:
                    if unblinded:
                        row.append(("{0:.0f}").format(cent,sep,err))
                    else:
                        row.append("-")
                else:
                    row.append(("{0:5.%if} {1}{2:%i.%if}" % (precision,precision+3,precision)).format(cent,sep,err))
            tab.add_row(row)
        tab.print_table(show_row_separators=False,show_alternating=False)


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("filename", help="mlfit.root file location")
    parser.add_argument("-r", "--regions", help="srcr or srdisc for SR+CR limits or BDT limits", default="srcr")
    parser.add_argument("-p", "--pretty", help="use pretty pytable", action="store_true")
    parser.add_argument("-u", "--unblinded", help="unblinded?", action="store_true")
    args = parser.parse_args()

    filename = args.filename
    # filename = "v0.10_Jul20/mlfit.root"

    datafile = r.TFile(glob.glob(filename.rsplit("/",1)[0]+"/data_*{}*.root".format(args.regions))[0])

    for name,hname in [
            ("b-only fit", "shapes_fit_b"),
            ("prefit", "shapes_prefit"),
            ("s+b fit", "shapes_fit_s"),
            ]:
        d = get_dict(filename,hname)
        # covar = d["total_covar"]
        # print list(covar)
        print "--- {} ({}) ----".format(name, hname)
        yields = parse_yields(d)
        yields["data"] = {
                "central": np.array(list(datafile.Get("sr"))[1:-1]),
                "error": np.array(list(datafile.Get("sr"))[1:-1]),
                }
        print_table(yields, regions=args.regions, pretty=args.pretty, unblinded=args.unblinded)
