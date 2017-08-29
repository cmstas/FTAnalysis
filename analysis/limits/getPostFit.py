#!/usr/bin/env python

# import argparse
import createCard
import os
import ROOT as r

def reduce_bins(h_in, nbins):
    h_tmp = r.TH1F(h_in.GetName(), h_in.GetTitle(), nbins, 0.5, nbins+0.5);
    for ibin in range(1,h_in.GetNbinsX()+1):
        h_tmp.SetBinContent( ibin, h_in.GetBinContent(ibin) )
        h_tmp.SetBinError( ibin, h_in.GetBinError(ibin) )
    return h_tmp

def get_dict(fname,typ="shapes_fit_s", nbins=10, _tostore=[]):
    f1 = r.TFile(fname)
    if not f1: 
        print "[!] Error, couldn't find file {0}".format(fname)
        raise Exception

    d = {}
    # typ = "shapes_fit_s"
    g_data = f1.Get("{0}/SS/data".format(typ))
    tkeys = f1.Get("{0}/SS".format(typ)).GetListOfKeys()
    for tkey in tkeys:
        key = tkey.GetName()
        tmpobj = f1.Get("{0}/SS/{1}".format(typ,key))
        # clone so that we're not tied to the TFile, as it gets closed
        obj = tmpobj.Clone(key)
        if tkey.ReadObj().InheritsFrom(r.TH1F.Class()):
            reduce_bins(obj, nbins)

        if key == "data":
            h_data = r.TH1F("h_data", "h_data", nbins, 0.5, nbins+0.5);
            for ipt in range(0,obj.GetN()):
                x = r.Double()
                y = r.Double()
                g_data.GetPoint(ipt, x, y);
                h_data.SetBinContent(ipt+1, y);
            d[key] = h_data
        else:
            d[key] = obj
        _tostore.append(d[key])
    f1.Close()
    return d
# shapes_prefit

def get_postfit_dict(fname="mlfitname.root"):
    d_splusbfit = get_dict(fname,"shapes_fit_s")
    d_prefit = get_dict(fname,"shapes_prefit")
    # for key in d_splusbfit:
    ratios = {} # postfit SRCR / prefit SRCR integrals
    for key in d_prefit:
        if "covar" in key: continue
        h_spb = d_splusbfit.get(key, None)
        h_prefit = d_prefit.get(key, None)
        if not h_spb or not h_prefit: continue
        ratios[key] = 1.0*h_spb.Integral()/h_prefit.Integral()
    return d_splusbfit, ratios
    # return d_prefit


if __name__ == "__main__":

    # parser = argparse.ArgumentParser()
    # parser.add_argument("dir", help="directory")
    # args = parser.parse_args()

    # print get_postfit_dict()
    d_postfit, ratios = get_postfit_dict("v0.10_Jul20/mlfit.root")
    print d_postfit
    print d_postfit["tttt"].Integral()
    print ratios
