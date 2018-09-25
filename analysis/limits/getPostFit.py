#!/usr/bin/env python

# import argparse
import createCard
import os
import ROOT as r
from errors import E

def reduce_bins(h_in, nbins):
    h_tmp = r.TH1F(h_in.GetName(), h_in.GetTitle(), nbins, 0.5, nbins+0.5);
    for ibin in range(1,h_in.GetNbinsX()+1):
        h_tmp.SetBinContent( ibin, h_in.GetBinContent(ibin) )
        h_tmp.SetBinError( ibin, h_in.GetBinError(ibin) )
    return h_tmp

def reduce_bins_2d(h_in):
    h_tmp = r.TH2F(h_in.GetName(), h_in.GetTitle(), h_in.GetNbinsX(), 0.5, h_in.GetNbinsX()+0.5, h_in.GetNbinsY(), 0.5, h_in.GetNbinsY()+0.5);
    for iy in range(1,h_in.GetNbinsY()+1):
        for ix in range(1,h_in.GetNbinsX()+1):
            h_tmp.SetBinContent( ix,iy, h_in.GetBinContent(ix,iy) )
            h_tmp.SetBinError( ix,iy, h_in.GetBinError(ix,iy) )
    return h_tmp

def get_dict(fname,typ="shapes_fit_s", nbins=10, _tostore=[], channel="SS"):
    f1 = r.TFile(fname)
    if not f1: 
        print "[!] Error, couldn't find file {0}".format(fname)
        raise Exception

    d = {}
    # typ = "shapes_fit_s"
    g_data = f1.Get("{0}/{1}/data".format(typ,channel))
    tkeys = f1.Get("{0}/{1}".format(typ,channel)).GetListOfKeys()
    for tkey in tkeys:
        key = tkey.GetName()
        tmpobj = f1.Get("{0}/{1}/{2}".format(typ,channel,key))
        # clone so that we're not tied to the TFile, as it gets closed
        obj = tmpobj.Clone(key)
        # For some reason, need to explicitly make new histograms to keep them detached from the TFile
        if tkey.ReadObj().InheritsFrom(r.TH1F.Class()):
            reduce_bins(obj, obj.GetNbinsX())
        if tkey.ReadObj().InheritsFrom(r.TH2F.Class()):
            reduce_bins_2d(obj)

        if key == "data":
            h_data = r.TH1F("h_data", "h_data", obj.GetN(), 0.5, obj.GetN()+0.5);
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

def get_postfit_dict(fname="mlfitname.root",channels=["SS"], bonly=False):
    which_postfit = "shapes_fit_s" # s+b
    if bonly: which_postfit = "shapes_fit_b" # bonly
    d_postfit = get_dict(fname,which_postfit,channel=channels[0])
    d_prefit = get_dict(fname,"shapes_prefit",channel=channels[0])
    if len(channels) > 1:
        for ch in channels[1:]:
            for k,v in get_dict(fname,which_postfit,channel=ch).items():
                if not v: continue
                d_postfit[k].Add(v)
            for k,v in get_dict(fname,"shapes_prefit",channel=ch).items():
                if not v: continue
                d_prefit[k].Add(v)
    # for key in d_postfit:
    ratios = {} # postfit SRCR / prefit SRCR integrals
    ratios_errors = {} # errors in the above SFs (gaussian propagated)
    for key in d_prefit:
        if "covar" in key: continue
        h_postfit = d_postfit.get(key, None)
        h_prefit = d_prefit.get(key, None)
        if not h_postfit or not h_prefit: continue
        epostfit = r.Double(0.0)
        eprefit = r.Double(0.0)
        vpostfit = h_postfit.IntegralAndError(0,-1,epostfit)
        vprefit = h_prefit.IntegralAndError(0,-1,eprefit)
        ratios[key] = 1.0*vpostfit/vprefit
        ratios_errors[key] = (E(vpostfit,epostfit)/E(vprefit,eprefit))[1]
    return d_postfit, ratios, ratios_errors
    # return d_prefit


if __name__ == "__main__":

    # parser = argparse.ArgumentParser()
    # parser.add_argument("dir", help="directory")
    # args = parser.parse_args()

    # print get_postfit_dict()
    from pprint import pprint
    d_postfit, ratios, ratios_errors = get_postfit_dict("v3.05_allyears_crwsplit_3bins_v1/mlfitname.root",channels=["y2016","y2017","y2018"], bonly=True)
    # d_postfit, ratios, ratios_errors = get_postfit_dict("v3.05_allyears_2bins_v1/mlfitname.root",channels=["y2016","y2017","y2018"], bonly=True)
    print d_postfit["tttt"].Integral()
    pprint(d_postfit)
    pprint(ratios)
    pprint(ratios_errors)

    for k in ratios.keys():
        print k, E(ratios[k],ratios_errors[k]).round(2)
