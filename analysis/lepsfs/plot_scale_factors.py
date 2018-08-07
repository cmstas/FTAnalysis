import uuid
import os
import json
import pprint
import ROOT as r
import numpy as np


def get_sf_hist(funcs_sf, funcs_sferr,
        ptbins=[10.0,20,35,50,100,200,300],
        etabins=[0.,0.8,1.44,1.566,2.0,2.5],
        ):
    h = r.TH2F("h{}".format(uuid.uuid4().hex[:5]),"",len(ptbins)-1,np.array(ptbins),len(etabins)-1,np.array(etabins))
    for ptbin_lowhigh in zip(ptbins[:-1],ptbins[1:]):
        for etabin_lowhigh in zip(etabins[:-1],etabins[1:]):
            ptcenter = 0.5*(ptbin_lowhigh[0] + ptbin_lowhigh[1])
            etacenter_pos = 0.5*(etabin_lowhigh[0] + etabin_lowhigh[1])
            etacenter_neg = -etacenter_pos
            a_sf1 = np.array([f(ptcenter,etacenter_pos) for f in funcs_sf])
            a_sf2 = np.array([f(ptcenter,etacenter_neg) for f in funcs_sf])
            a_sf1err = np.array([f(ptcenter,etacenter_pos) for f in funcs_sferr])
            a_sf2err = np.array([f(ptcenter,etacenter_neg) for f in funcs_sferr])
            sf = 0.5*(np.prod(a_sf1) + np.prod(a_sf2))
            sferr = 0.5*(np.sum(a_sf1err**2)**0.5 + np.sum(a_sf2err**2)**0.5)
            ibin = h.Fill(ptcenter,etacenter_pos, sf)
            h.SetBinError(ibin, sferr)
    h.GetYaxis().SetTitle("|#eta|")
    h.GetXaxis().SetTitle("p_{T} [GeV]")
    h.GetXaxis().SetMoreLogLabels(True)
    h.SetMarkerSize(1.2)
    return h

r.gStyle.SetOptStat(0)
r.gStyle.SetPaintTextFormat(".3F")
c1 = r.TCanvas()
c1.SetLogx(1)

# r.gROOT.ProcessLine(".L lepton_sfs_el_v2_nmiss0.h")
# h = get_sf_hist(
#     funcs_sf = [r.electronScaleFactorReco_RunBCDEF, r.electronScaleFactor_RunBCDEF],
#     funcs_sferr = [r.electronScaleFactorRecoError_RunBCDEF, r.electronScaleFactorError_RunBCDEF],
#     ptbins = [10.0,20,35,50,100,200,300],
#     etabins = [0.,0.8,1.44,1.566,2.0,2.5],
#     )
# h.Draw("colztexte")
# c1.SaveAs("test.pdf")
# os.system("ic test.pdf")

r.gROOT.ProcessLine(".L lepton_sfs_el_v2_nmiss1.h")
h = get_sf_hist(
    funcs_sf = [r.electronScaleFactorReco_RunBCDEF, r.electronScaleFactor_RunBCDEF],
    funcs_sferr = [r.electronScaleFactorRecoError_RunBCDEF, r.electronScaleFactorError_RunBCDEF],
    ptbins = [10.0,20,35,50,100,200,300],
    etabins = [0.,0.8,1.44,1.566,2.0,2.5],
    )
h.Draw("colztexte")
c1.SaveAs("test.pdf")
os.system("ic test.pdf")
