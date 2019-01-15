import os
import sys
import pickle
import array
import re
import time
import random

import pandas as pd
import numpy as np

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

import ROOT as r

d_xsec = {
        "tth": {
         350: 0.001*24.2,
         370: 0.001*20.1,
         390: 0.001*17.8,
         410: 0.001*15.8,
         430: 0.001*14.9,
         450: 0.001*11.9,
         470: 0.001*11.2,
         490: 0.001*9.87,
         510: 0.001*9.75,
         530: 0.001*7.78,
         550: 0.001*7.49,
            },
        "thw": {
            350: 0.001*8.37,
            370: 0.001*7.61,
            390: 0.001*7.07,
            410: 0.001*6.58,
            430: 0.001*5.72,
            450: 0.001*5.16,
            470: 0.001*4.81,
            490: 0.001*4.34,
            510: 0.001*4.19,
            530: 0.001*3.86,
            550: 0.001*3.41,
            },
        "thq": {
            350: 0.001*15.8,
            370: 0.001*13.5,
            390: 0.001*11.7,
            410: 0.001*10.2,
            430: 0.001*8.82,
            450: 0.001*7.73,
            470: 0.001*6.78,
            490: 0.001*5.93,
            510: 0.001*5.23,
            530: 0.001*4.59,
            550: 0.001*4.11,
            },
        "tta": {
            350: 0.001*33.7,
            370: 0.001*28.6,
            390: 0.001*25.3,
            410: 0.001*22.0,
            430: 0.001*19.9,
            450: 0.001*17.7,
            470: 0.001*15.1,
            490: 0.001*14.9,
            510: 0.001*12.6,
            530: 0.001*11.5,
            550: 0.001*10.0,
            },
        "taw": {
                350: 0.001*7.97,
                370: 0.001*7.09,
                390: 0.001*6.88,
                410: 0.001*5.92,
                430: 0.001*5.39,
                450: 0.001*4.97,
                470: 0.001*4.59,
                490: 0.001*4.27,
                510: 0.001*3.87,
                530: 0.001*3.82,
                550: 0.001*3.26,
            },
        "taq": {
                350: 0.001*9.28,
                370: 0.001*8.14,
                390: 0.001*7.19,
                410: 0.001*6.43,
                430: 0.001*5.72,
                450: 0.001*5.07,
                470: 0.001*4.55,
                490: 0.001*4.09,
                510: 0.001*3.67,
                530: 0.001*3.27,
                550: 0.001*2.94,
            }
        }
d_xsec["xsech"] = { mass:sum([d_xsec[proc].get(mass,0) for proc in ["tth","thq","thw"]]) for mass in set(sum([d_xsec[proc].keys() for proc in ["tth","thq","thw"]],[])) }
d_xsec["xseca"] = { mass:sum([d_xsec[proc].get(mass,0) for proc in ["tta","taq","taw"]]) for mass in set(sum([d_xsec[proc].keys() for proc in ["tta","taq","taw"]],[])) }


def set_defaults():
    from matplotlib import rcParams
    rcParams["font.family"] = "sans-serif"
    rcParams["font.sans-serif"] = ["Helvetica", "Arial", "Liberation Sans", "Bitstream Vera Sans", "DejaVu Sans"]
    # rcParams['mathtext.fontset'] = 'custom'
    # rcParams['mathtext.rm'] = 'Liberation Sans'
    # rcParams['mathtext.it'] = 'Liberation Sans:italic'
    # rcParams['mathtext.bf'] = 'Liberation Sans:bold'
    rcParams['legend.fontsize'] = 11
    rcParams['legend.labelspacing'] = 0.2
    # rcParams['axes.xmargin'] = 0.0 # rootlike, no extra padding within x axis
    rcParams['axes.labelsize'] = 'x-large'
    rcParams['axes.formatter.use_mathtext'] = True
    rcParams['legend.framealpha'] = 0.65
    rcParams['axes.labelsize'] = 'x-large'
    rcParams['axes.titlesize'] = 'large'
    rcParams['xtick.labelsize'] = 'large'
    rcParams['ytick.labelsize'] = 'large'
    rcParams['figure.subplot.hspace'] = 0.1
    rcParams['figure.subplot.wspace'] = 0.1
    rcParams['figure.subplot.right'] = 0.96
    rcParams['figure.max_open_warning'] = 0
    rcParams['figure.dpi'] = 125
    rcParams["axes.formatter.limits"] = [-5,4] # scientific notation if log(y) outside this

def make_plot(ds,d):

    lumi = float(d["mylumi"])
    if d["overridelumi"]:
        lumi = float(d["overridelumi"])


    scalelumi = 1.0
    if d["scaletolumi"]:
        scalelumi = float(d["scaletolumi"])/lumi

    # xmin,xmax = 250,1050
    # ymax = 30

    xmin = d["xmin"]
    xmax = d["xmax"]
    ymax = d["ymax"]

    obs = (ds["expr"]*ds["xsec"]).values.tolist()
    exp = (ds["expr"]*ds["xsec"]).values.tolist()
    sm2 = (ds["exprm2"]*ds["xsec"]).values.tolist()
    sm1 = (ds["exprm1"]*ds["xsec"]).values.tolist()
    sp1 = (ds["exprp1"]*ds["xsec"]).values.tolist()
    sp2 = (ds["exprp2"]*ds["xsec"]).values.tolist()
    mhs = ds["mass"].values.tolist()
    x0e = [0. for _ in mhs]

    # theory = [1000.0*d_xsec[typ][int(mass)]/(scalelumi**0.5) for mass in mhs]
    theory = ds["xsec"].values.tolist()
    # print theory

    # print mhs
    # print obs
    # print exp

    #TGraphAsymmErrors needs the delta
    for i in range(0,len(exp)):
        sm2[i]=exp[i]-sm2[i]
        sm1[i]=exp[i]-sm1[i]
        sp1[i]=sp1[i]-exp[i]
        sp2[i]=sp2[i]-exp[i]

    r.gStyle.SetOptStat(0)
    c1 = r.TCanvas("c1", "", 800, 800)
    c1.cd()
    padt = r.TPad("p_tex", "p_tex", 0.0, 0.0, 1.0, 1.0)
    padt.SetTopMargin(0.1)
    padt.SetBottomMargin(0.1)
    padt.SetRightMargin(0.05)
    padt.SetLeftMargin(0.15)
    padt.SetTickx()
    padt.SetTicky()
    padt.Draw()
    padt.cd()

    h = r.TH2F("h","h",16,xmin,xmax,ymax,0,ymax)
    h.SetTitle("")
    # h.GetXaxis().SetTitle("m_{H/A} [GeV]")
    h.GetXaxis().SetTitle(d.get("xaxistitle","m_{H/A} (GeV)"))
    # h.GetYaxis().SetTitle("N^{UL}/N^{exp} limit at 95% CL")
    h.GetYaxis().SetTitle(d["yaxistitle"])
    h.GetXaxis().SetLabelSize(0.04)
    h.GetYaxis().SetLabelSize(0.04)
    h.GetXaxis().SetLabelOffset(0.01)
    h.GetYaxis().SetLabelOffset(0.015)
    h.GetXaxis().SetTitleSize(0.045)
    h.GetYaxis().SetTitleSize(0.045)
    h.GetXaxis().SetTitleOffset(1.0)
    h.GetYaxis().SetTitleOffset(1.5)
    h.Draw()

    gr_s2b = r.TGraphAsymmErrors(len(mhs),array.array('d', mhs),array.array('d', exp),array.array('d', x0e),array.array('d', x0e),array.array('d', sm2),array.array('d', sp2))
    gr_s2b.SetFillColor(r.kYellow)
    gr_s2b.Draw("3")
      
    gr_s1b = r.TGraphAsymmErrors(len(mhs),array.array('d', mhs),array.array('d', exp),array.array('d', x0e),array.array('d', x0e),array.array('d', sm1),array.array('d', sp1))
    gr_s1b.SetFillColor(r.kGreen)
    gr_s1b.Draw("3")

    gexp = r.TGraph(len(mhs), array.array('d', mhs), array.array('d', exp))
    gexp.SetLineStyle(7)
    gexp.SetLineWidth(3)
    gexp.SetLineColor(r.kBlack)
    gexp.Draw("L")

    gtheory = r.TGraph(len(mhs), array.array('d', mhs), array.array('d', theory))
    # gexp.SetLineStyle(7)
    gtheory.SetLineWidth(4)
    gtheory.SetLineColor(r.kRed)
    gtheory.Draw("L")

    # if "combined" in typ:
    #     if not d.get("hidepseudo",False):
    #         gtheory2 = r.TGraph(len(mhs), array.array('d', mhs), array.array('d', theory_ps))
    #         gtheory2.SetLineStyle(7)
    #         gtheory2.SetLineWidth(4)
    #         gtheory2.SetLineColor(r.kRed)
    #         gtheory2.Draw("L")

    if d["showobserved"]:
        gobs = r.TGraph(len(mhs), array.array('d', mhs), array.array('d', obs))
    else:
        gobs = r.TGraph()
    gobs.SetMarkerStyle(r.kFullCircle)
    gobs.SetMarkerSize(1.5)
    gobs.SetMarkerColor(r.kBlack)
    gobs.SetLineWidth(3)
    gobs.SetLineColor(r.kBlack)
    if d["showobserved"]: gobs.Draw("PL")

    r.gPad.Update()
    r.gPad.RedrawAxis()

    if d["nolegendheader"]:
        l1 = r.TLegend(0.15, 0.75, 0.95, 0.90)
        # l1 = r.TLegend(0.15, 0.70, 0.95, 0.90)
        # l1.AddEntry(None , "", "")
        # l1.AddEntry(None , "", "")
    else:
        l1 = r.TLegend(0.15, 0.70, 0.95, 0.90)
    l1.SetTextFont(42)
    l1.SetTextSize(0.029)
    l1.SetNColumns(2)
    l1.SetShadowColor(r.kWhite)
    l1.SetFillColor(r.kWhite)
    if not d["nolegendheader"]:
        l1.SetHeader(d["legendheader"])
    l1.AddEntry(gobs , "95% CL Observed", "lp")
    if "_ps_" in d["sig"]:
        l1.AddEntry(gtheory , "#sigma_{theory}^{pseudoscalar}", "l")
    else:
        l1.AddEntry(gtheory , "#sigma_{theory}^{scalar}", "l")
    l1.AddEntry(gexp , "95% CL Expected #pm 1 and #pm 2 #sigma_{experiment}", "l")
    # if "combined" in typ:
    #     if not d.get("hidepseudo",False):
    #         l1.AddEntry(gtheory2 , "#sigma_{theory}^{pseudoscalar}", "l")
    l1.Draw()
    
    magic1 = 1.8
    if d["nolegendheader"]:
        # magic2 = 1.38
        magic2 = 2.08
    else:
        magic2 = 2.08
    magic3 = 0.15
    magic4 = 0.08
    magic5 = 11.2

    magic2 = 1.38

    LExp1 = r.TGraphAsymmErrors(2)
    LExp1.SetFillColor(r.kYellow)
    LExp1.SetPoint(0,xmin+ magic1*(xmax-xmin)/100, ymax-magic2*(ymax-0)/100*10)
    LExp1.SetPointError(0,0.,0.,magic3*ymax/5,magic3*ymax/5)
    LExp1.SetPoint(1,xmin+magic5*(xmax-xmin)/100, ymax-magic2*(ymax-0)/100*10)
    LExp1.SetPointError(1,0.,0.,magic3*ymax/5,magic3*ymax/5)
    LExp1.Draw("3")

    LExp2 = r.TGraphAsymmErrors(2)
    LExp2.SetFillColor(r.kGreen)
    LExp2.SetPoint(0,xmin+ magic1*(xmax-xmin)/100, ymax-magic2*(ymax-0)/100*10)
    LExp2.SetPointError(0,0.,0.,magic4*ymax/5,magic4*ymax/5)
    LExp2.SetPoint(1,xmin+magic5*(xmax-xmin)/100, ymax-magic2*(ymax-0)/100*10)
    LExp2.SetPointError(1,0.,0.,magic4*ymax/5,magic4*ymax/5)
    LExp2.Draw("L3")

    LExp = r.TGraph(2)
    LExp.SetLineColor(r.kBlack)
    LExp.SetLineStyle(7)
    LExp.SetLineWidth(3)
    LExp.SetPoint(0,xmin+ magic1*(xmax-xmin)/100, ymax-magic2*(ymax-0)/100*10)
    LExp.SetPoint(1,xmin+magic5*(xmax-xmin)/100, ymax-magic2*(ymax-0)/100*10)
    LExp.Draw("L")

    # offset = 0.058
    offset = 0.00
    cmstex = r.TLatex(0.670,0.91-offset, "%.1f fb^{-1} (13 TeV)" % (lumi*scalelumi))
    cmstex.SetNDC()
    cmstex.SetTextSize(0.04)
    cmstex.SetLineWidth(2)
    cmstex.SetTextFont(42)
    cmstex.Draw()

    cmstexbold = r.TLatex(0.16,0.91-offset, "CMS" )
    cmstexbold.SetNDC()
    cmstexbold.SetTextSize(0.05)
    cmstexbold.SetLineWidth(2)
    cmstexbold.SetTextFont(61)
    cmstexbold.Draw()

    cmstexprel = r.TLatex(0.28,0.91-offset, " Preliminary" )
    cmstexprel.SetNDC()
    cmstexprel.SetTextSize(0.04)
    cmstexprel.SetLineWidth(2)
    cmstexprel.SetTextFont(52)
    cmstexprel.Draw()

    if d["stupid_label"]:
        stupidtex = r.TLatex(0.19,0.63, d["stupid_label"])
        stupidtex.SetNDC()
        stupidtex.SetTextSize(0.045)
        stupidtex.SetLineWidth(2)
        stupidtex.SetTextFont(42)
        stupidtex.Draw()

    # masstex = r.TLatex(0.60,0.79, "pp#rightarrow t#bar{t}H, H #rightarrow t#bar{t}" )
    # masstex.SetNDC()
    # masstex.SetTextSize(0.036)
    # masstex.SetLineWidth(2)
    # masstex.SetTextFont(42)
    # masstex.Draw()
    
    # exclLine = r.TLine()
    # exclLine.SetLineColor(r.kRed)
    # exclLine.DrawLine(xmin,1.0,xmax,1.0)

    fname_out = "ft_%s_limit%s.pdf" % (d["sig"],d["postfix"])
    c1.SaveAs(fname_out)
    # os.system("web %s" % fname_out)
    os.system("ic %s" % fname_out)

if __name__ == "__main__":

    df = pd.DataFrame(pickle.load(open("higgs_scan_data.pkl")))
    df["obs"] *= 0.
    df = df.sort_values("mass")
    print df

    # print df[df["which"]=="h"]["expr"]

    # which = "h"
    # ds = df[df["which"]==which]
    # ds["xsec"] = ds["mass"].apply(lambda x: 1000.*d_xsec["xsec"+which][x])
    # print ds
    # obs = (ds["expr"]*ds["xsec"]).values.tolist()
    # exp = (ds["expr"]*ds["xsec"]).values.tolist()
    # sm2 = (ds["exprm2"]*ds["xsec"]).values.tolist()
    # sm1 = (ds["exprm1"]*ds["xsec"]).values.tolist()
    # sp1 = (ds["exprp1"]*ds["xsec"]).values.tolist()
    # sp2 = (ds["exprp2"]*ds["xsec"]).values.tolist()
    # mhs = ds["mass"].values.tolist()
    # x0e = [0. for _ in mhs]
    # print obs
    # print exp
    # print sm2
    # print sm1
    # print sp1
    # print sp2
    # print mhs
    # sys.exit()


    # set_defaults()
    # mpl.rcParams.update({'legend.fontsize': 'large',
    #          'axes.labelsize': 'x-large',
    #          'axes.titlesize':'x-large',
    #          'xtick.labelsize':'large',
    #          'ytick.labelsize':'large'})

    # fig, (ax,ax_ratio) = plt.subplots(1,1, gridspec_kw={"top":0.94})
    # c = "C0"

    

    # p1 = ax.fill_between(kts, theorylow, theoryhigh, linewidth=0., facecolor=c, alpha=0.4)
    # p2 = ax.plot(kts, theory, linestyle="--", marker=".",color=c)
    # c = "C3"
    # p3 = ax.plot(data_mg[:,0], data_mg[:,1], linestyle="-", marker=".",color=c)
    # # p4 = ax.plot(data_mg_withb[:,0], data_mg_withb[:,1], linestyle="-", marker=".",color="C4")
    # ax.legend([(p1,p2[0]), p3[0]], ["Cao et al eqn (x 1.27)","QED+QCD MG5 (x 1.27)"])
    # # ax.legend([(p1,p2[0]), p3[0], p4[0]], ["Cao et al eqn (x 1.27)","QED+QCD MG5 (x 1.27)", "QED+QCD MG5 w/ b (x 1.27)"])
    # ax.set_ylabel(r"$\sigma_{t\bar{t}t\bar{t}}$ [fb]")
    # ax_ratio.set_xlabel(r"$y_{t}/y_{t}^{SM}$")
    # ratio = data_mg[:,1]/calc_sigma(data_mg[:,0],gza_13tev,int_13tev,higgs_13tev)
    # ax_ratio.plot(data_mg[:,0],ratio,lw=1.,markersize=5.,marker="o",color=c)
    # ax_ratio.set_ylabel(r"MG5/Equation")
    # ax.set_title("")
    # fig.set_tight_layout(True)
    # # map(lambda x:x.set_fontsize(16), [ax.legend, ax.title, ax.xaxis.label, ax.yaxis.label])
    # fig.savefig("test.pdf")
    # os.system("ic test.pdf")


    tth = {
            "sig": "tth_scan",
            "redolimits": False,
            "mydir": "v9.06_Mar6_35p9_reminiaod_higgs",
            "legendheader": "pp#rightarrow t#bar{t}H, BR(H #rightarrow t#bar{t})=1: exclusion limit",
            "nolegendheader": False,
            "mylumi": "17.3",
            "verbose": False,
            "scaletolumi": None,
            "showobserved": True,
            "postfix": "",
            # "xmin": 250,
            # "xmax": 1050,
            # "ymax": 200,
            "xmin": 300,
            "xmax": 550,
            "ymax": 200,
            "overridelumi": None,
            "stupid_label": "",
            "yaxistitle": "#sigma^{UL} limit at 95% CL (fb)",
            }


    ### SCALAR

    tth["mylumi"] = "136.3"
    tth["redolimits"] = False




    ### PSEUDOSCALAR


    # thedir = "v0.10_Jul6_WSF"
    # thedir = "v0.10_Jul10_higgstest"
    # thedir = "v0.10_Jul20_tmp"
    thedir = "v0.10_Jul20_fastsimhiggs"


    higgs = tth.copy()
    higgs["sig"] = "higgs_scan"
    higgs["stupid_label"] = "   "
    higgs["mydir"] = thedir
    higgs["xmin"] = 349.9
    higgs["xmax"] = 550
    higgs["ymax"] = 160
    higgs["legendheader"] = "pp#rightarrow (t#bar{t}H/A,tWH/A,tqH/A), BR(H/A #rightarrow t#bar{t})=1: exclusion"
    higgs["nolegendheader"] = True
    # higgs["yaxistitle"] = "#sigma(pp#rightarrow (t#bar{t},tW,tq)+H/A) #times BR(H/A#rightarrow t#bar{t}) [fb]"
    higgs["yaxistitle"] = "#sigma(pp#rightarrow (t#bar{t},tW,tq)+H) #times BR(H#rightarrow t#bar{t}) (fb)"
    higgs["xaxistitle"] = "m_{H} (GeV)"
    higgs["hidepseudo"] = True
    higgs["redolimits"] = True
    higgs["showobserved"] = False

    higgs_ps = tth.copy()
    higgs_ps["sig"] = "higgs_ps_scan"
    higgs_ps["stupid_label"] = "   "
    higgs_ps["mydir"] = thedir
    higgs_ps["xmin"] = 349.9
    higgs_ps["xmax"] = 550
    higgs_ps["ymax"] = 160
    higgs_ps["legendheader"] = "pp#rightarrow (t#bar{t}H/A,tWH/A,tqH/A), BR(H/A #rightarrow t#bar{t})=1: exclusion"
    higgs_ps["nolegendheader"] = True
    # higgs_ps["yaxistitle"] = "#sigma(pp#rightarrow (t#bar{t},tW,tq)+H/A) #times BR(H/A#rightarrow t#bar{t}) [fb]"
    higgs_ps["yaxistitle"] = "#sigma(pp#rightarrow (t#bar{t},tW,tq)+A) #times BR(A#rightarrow t#bar{t}) (fb)"
    higgs_ps["xaxistitle"] = "m_{A} (GeV)"
    higgs_ps["hidepseudo"] = True
    higgs_ps["redolimits"] = True
    higgs_ps["showobserved"] = False

    # which = "a"
    # ds = df[df["which"]==which]
    # ds["xsec"] = ds["mass"].apply(lambda x: 1000.*d_xsec["xsec"+which][x])
    # make_plot(ds, higgs_ps)

    which = "h"
    ds = df[df["which"]==which]
    ds["xsec"] = ds["mass"].apply(lambda x: 1000.*d_xsec["xsec"+which][x])
    make_plot(ds, higgs)
    


