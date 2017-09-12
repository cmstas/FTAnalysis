#!/usr/bin/env python

from getPostFit import get_dict
import glob
import ROOT as r
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

def print_table(d_yields):
    nbins = len(d_yields["ttz"]["central"])
    # colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","$\\ttVV$","X+$\\gamma$","Rares","Flips","Fakes","Total","Data","$\\tttt$"]
    # procs = ["ttw","ttz","tth","ttvv","xg","rares","flips","fakes","total_background","data","tttt"]

    colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","$\\ttVV$","X+$\\gamma$","Rares","Flips","Fakes","Total","Data","$\\tttt$", "tot s+b"]
    allprocs = [["ttw"],["ttz"],["tth"],["ttvv"],["xg"],["rares"],["flips"],["fakes"],["total_background"],["data"],["tttt"],["total"]]

    # colnames = ["","$\\ttW$","$\\ttZ$","$\\ttH$","Others","Total","Data","$\\tttt$"]
    # allprocs = [["ttw"],["ttz"],["tth"],["ttvv","xg","rares","flips","fakes"],["total_background"],["data"],["tttt"]]

    srnames = ["CRZ","CRW","SR1","SR2","SR3","SR4","SR5","SR6","SR7","SR8"]
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
                tojoin.append("{0:.0f}".format(cent))
            else:
                tojoin.append("{0:5.2f}$\\pm${1:5.2f}".format(cent,err))
        print " & ".join(tojoin),
        print r"\\"



if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("filename", help="mlfit.root file location")
    args = parser.parse_args()

    filename = args.filename
    # filename = "v0.10_Jul20/mlfit.root"

    d_splusbfit = get_dict(filename,"shapes_fit_s")
    d_prefit = get_dict(filename,"shapes_prefit")

    datafile = r.TFile(glob.glob(filename.rsplit("/",1)[0]+"/data_*srcr*.root")[0])


    d_prefityields = parse_yields(d_prefit)
    d_prefityields["data"] = {
            "central": list(datafile.Get("sr"))[1:-1],
            "error": list(datafile.Get("sr"))[1:-1],
            }

    d_splusbfityields = parse_yields(d_splusbfit)
    d_splusbfityields["data"] = {
            "central": list(datafile.Get("sr"))[1:-1],
            "error": list(datafile.Get("sr"))[1:-1],
            }


    # print d_splusbfit
    # print d_prefit

    # pprint(d_yields)

    print "--- prefit ----"
    print_table(d_prefityields)

    print "--- s+b fit ----"
    print_table(d_splusbfityields)
    
    # d_yields["bgtot"] = {}
    # d_yields["bgtot"]["central"] = bg_sum_central
    # d_yields["bgtot"]["error"] = bg_sum_error**0.5

    # # data
    # data_hname = d_shapes["data_obs"]["hname"]
    # data_rootfile = d_shapes["data_obs"]["rootfile"]
    # fdata = r.TFile(data_rootfile)
    # d_yields["data"] = {}
    # d_yields["data"]["central"] = np.array(list(fdata.Get(data_hname))[1:-1])
    # d_yields["data"]["error"] = d_yields["data"]["central"]**0.5
