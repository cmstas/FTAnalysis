import itertools
import array
import os
import numpy as np
import re
import glob
import pandas as pd

import ROOT as r
r.gROOT.SetBatch(True)

np.set_printoptions(suppress=True,threshold=200,precision=5)

def parse_log(fname):
    d = {}
    with open(fname,"r") as fhin:
        for line in fhin:
            if "Observed" in line: d["obs"] = float(line.split("<")[-1])
            elif "Expected" in line: d["exp_"+line.split("%")[0].replace("Expected","").strip()] = float(line.split("<")[-1])

    obs = d.get("obs",-1)
    exp = d.get("exp_50.0",-1)
    exp_sm1 = d.get("exp_16.0",-1)
    exp_sp1 = d.get("exp_84.0",-1)
    exp_sm2 = d.get("exp_2.5",-1)
    exp_sp2 = d.get("exp_97.5",-1)
    return {
            "obs":obs, "exp":exp,
            "sp1":exp_sp1,"sm1":exp_sm1,
            "sp2":exp_sp2,"sm2":exp_sm2,
            }

def set_susy_palette():
    r.gStyle.SetOptStat(0)
    mypalette = array.array('i')
    NRGBs = 5
    NCont = 255
    stops = array.array('d', [0.00, 0.34, 0.61, 0.84, 1.00])
    red   = array.array('d', [0.50, 0.50, 1.00, 1.00, 1.00])
    green = array.array('d', [0.50, 1.00, 1.00, 0.60, 0.50])
    blue  = array.array('d', [1.00, 1.00, 0.50, 0.40, 0.50])
    FI = r.TColor.CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont)
    for i in range (0,NCont): mypalette.append(FI+i)
    r.gStyle.SetPalette(NCont,mypalette)
    r.gStyle.SetNumberContours(NCont)

def get_smoothed(graph, nsmooth=3, algo="k5b", diagonal_fudge=50, return_hist=False, persist=[]):
    graph_ = graph.Clone()
    # Turn TGraph into TH2F
    h2 = graph_.GetHistogram()

    # Keep list of (xbin,ybin)s for empty bins
    emptybins = set([])
    for xbin in range(1,h2.GetNbinsX()+1):
        for ybin in range(1,h2.GetNbinsY()+1):
            if h2.GetBinContent(xbin,ybin) < 1.e-6:
                emptybins.add((xbin,ybin))

    if nsmooth:
        # Now smooth 5x5 3 times
        h2.Smooth(nsmooth,algo)

    # Reset empty bins to 0, since smoothing spilled over diagonal
    for xbin in range(1,h2.GetNbinsX()+1):
        for ybin in range(1,h2.GetNbinsY()+1):
            if (xbin,ybin) in emptybins:
                h2.SetBinContent(xbin,ybin,0)

    # Want to be able to give a TGraph2D and get back a smoothed (and cleaned) histogram back
    if return_hist:
        persist.append(graph_)
        persist.append(h2)
        return h2

    # Trick to avoid zeros above the diagonal that would affect the smoothing
    # Smear diagonal straight upwards so that contour will go up vertically rather than
    # to the top right (parallel to diagonal)
    for xbin in range(1,h2.GetNbinsX()+1):
        for ybin in range(2,h2.GetNbinsY()+1):
            if h2.GetBinContent(xbin,ybin) < 1.e-6:
                h2.SetBinContent(xbin,ybin,h2.GetBinContent(xbin,ybin-1))

    # Now get the contour
    cont = r.TGraph2D(h2).GetContourList(1.)

    # Then reset empty bins back to 0 to fix that trick from above
    for xbin in range(1,h2.GetNbinsX()+1):
        for ybin in range(1,h2.GetNbinsY()+1):
            if (xbin,ybin) in emptybins:
                h2.SetBinContent(xbin,ybin,0)

    # Get list of x,y points for contour
    points = zip(list(cont[0].GetX()),list(cont[0].GetY()))

    # Go through and delete points close to diagonal if they are messed up
    newpoints = []
    # how many GeV below diagonal to delete contour poinst for - may need to tune
    diagonal_deletion_amount = diagonal_fudge
    for pt in points:
        xbin = h2.GetXaxis().FindBin(pt[0])
        ybin = h2.GetYaxis().FindBin(pt[1]+diagonal_deletion_amount)
        val = h2.GetBinContent(xbin,ybin)
        if val > 1.0e-6:
            newpoints.append(pt)

    # Make new temporary TGraph and put in the "good" points from above
    gtest = r.TGraph(len(newpoints))
    for i,(x,y) in enumerate(newpoints):
        gtest.SetPoint(i,x,y)
    return gtest

def get_rvals(
        indir = "Scan_v9.06_Aug21_150-35p9_HHL_38-Full/",
        glob_pattern = "*.log",
        is_gluino = True,
        blinded = True,
        ):

    lognames = glob.glob(indir + "/" + glob_pattern)
    fxsec = r.TFile("xsecs/xsec_susy_13tev.root")
    hxsec = fxsec.Get("h_xsec_gluino" if is_gluino else "h_xsec_stop")

    d_vals = {
            "glu_xsec": [],
            "xsec": [],
            "mglu": [],
            "mlsp": [],
            "obs": [],
            "om1": [],
            "op1": [],
            "exp": [],
            "sp1": [],
            "sm1": [],
            "sp2": [],
            "sm2": [],
            }
    for logname in lognames:
        mglu, mlsp = map(int,re.findall(r"_m([0-9]+)", logname))
        lims = parse_log(logname)

        # if any of the numbers are -1 (dummy default), skip bad point
        if any(lims[k]<0. for k in ["exp", "sm1", "sp1", "sm2", "sp2"]): continue

        for k in ["obs", "exp", "sm1", "sp1", "sm2", "sp2"]:
            d_vals[k].append(lims[k])
        glu_xsec = hxsec.GetBinContent(hxsec.FindBin(mglu))
        glu_xsec_err = hxsec.GetBinError(hxsec.FindBin(mglu))
        if blinded:
            lims["obs"] = lims["exp"]
        plane = lims["obs"]
        xsec = plane*glu_xsec
        d_vals["om1"].append(lims["obs"]*(glu_xsec-glu_xsec_err)/glu_xsec)
        d_vals["op1"].append(lims["obs"]*(glu_xsec+glu_xsec_err)/glu_xsec)
        d_vals["mglu"].append(1.0*mglu)
        d_vals["mlsp"].append(1.0*mlsp)
        d_vals["glu_xsec"].append(1.0*glu_xsec)
        d_vals["xsec"].append(1.0*xsec)

    # convert all arrays to numpy
    for k in d_vals.keys():
        d_vals[k] = np.array(d_vals[k])

    # cnames = ['mglu', 'mlsp', 'exp', 'sm1', 'sp1', 'sm2', 'sp2', 'obs', 'op1', 'om1', 'xsec', 'glu_xsec']
    # rec = np.rec.fromarrays([d_vals[k] for k in cnames], dtype=[(k, float) for k in cnames])
    # rec.sort(order=["mglu","mlsp"])
    # return rec

    return d_vals

def compare_two_scans_usingtgraph(dir_old,dir_new,glob_pattern="*fs_t6ttww*.log",fname="compare.png",nbinsx=12,nbinsy=12,palette=r.kCherry):
    vals_old = get_rvals(indir=dir_old, glob_pattern=glob_pattern)
    g_exp_old = r.TGraph2D("g_exp_old","g_exp_old",len(vals_old["mglu"]),vals_old["mglu"],vals_old["mlsp"],vals_old["exp"])
    vals_new = get_rvals(indir=dir_new, glob_pattern=glob_pattern)
    g_exp_new = r.TGraph2D("g_exp_new","g_exp_new",len(vals_new["mglu"]),vals_new["mglu"],vals_new["mlsp"],vals_new["exp"])

    minx = max(vals_old["mglu"].min(),vals_new["mglu"].min())
    maxx = min(vals_old["mglu"].max(),vals_new["mglu"].max())
    miny = max(vals_old["mlsp"].min(),vals_new["mlsp"].min())
    maxy = min(vals_old["mlsp"].max(),vals_new["mlsp"].max())

    h_old = r.TH2D("h_old","h_old",nbinsx,minx,maxx,nbinsy,miny,maxy)
    g_exp_old.SetHistogram(h_old)
    h_new = r.TH2D("h_new","h_new",nbinsx,minx,maxx,nbinsy,miny,maxy)
    g_exp_new.SetHistogram(h_new)
    h_old = g_exp_old.GetHistogram()
    h_new = g_exp_new.GetHistogram()

    r.gStyle.SetPalette(palette)
    r.gStyle.SetNumberContours(255)
    
    c1 = r.TCanvas()
    r.gStyle.SetOptStat(0)
    r.gStyle.SetPaintTextFormat("0.2f")

    h_new.SetMarkerSize(1.5)
    h_new.Divide(h_old)
    h_new.Draw("colztext")
    h_new.SetTitle("{}/{} ({})".format(dir_new,dir_old,glob_pattern))
    h_new.GetZaxis().SetRangeUser(0.5,1.05)

    c1.SaveAs(fname)

def compare_two_scans(dir_old,dir_new,glob_pattern="*fs_t6ttww*.log",fname="compare.png",nbinsx=12,nbinsy=12,palette=r.kCherry, zrange=[0.5,1.05], title=None):

    vals_old = get_rvals(indir=dir_old, glob_pattern=glob_pattern)
    vals_new = get_rvals(indir=dir_new, glob_pattern=glob_pattern)
    df_old = pd.DataFrame(np.c_[vals_old["mglu"],vals_old["mlsp"],vals_old["exp"]], columns=["mglu","mlsp","exp"])
    df_new = pd.DataFrame(np.c_[vals_new["mglu"],vals_new["mlsp"],vals_new["exp"]], columns=["mglu","mlsp","exp"])

    df = df_old.merge(df_new, on=["mglu","mlsp"], suffixes=["_old","_new"], how="inner")
    ratio = (df["exp_new"]/df["exp_old"]).values

    minx = df["mglu"].min()
    maxx = df["mglu"].max()
    miny = df["mlsp"].min()
    maxy = df["mlsp"].max()

    h_ratio = r.TProfile2D("h_ratio","h_ratio",nbinsx,minx,maxx,nbinsy,miny,maxy)

    for x,y,rat in zip(df["mglu"].values,df["mlsp"].values,ratio):
        h_ratio.Fill(x,y,rat)

    r.gStyle.SetPalette(palette)
    r.gStyle.SetNumberContours(255)
    
    c1 = r.TCanvas()
    r.gStyle.SetOptStat(0)
    r.gStyle.SetPaintTextFormat("0.3f")

    h_ratio.SetMarkerSize(1.5)
    h_ratio.Draw("colztext")
    h_ratio.SetTitle(title or "{}/{} ({})".format(dir_new,dir_old,glob_pattern))
    h_ratio.GetZaxis().SetRangeUser(*zrange)

    c1.SaveAs(fname)

def draw_limits(
        blinded=True,
        indir = "Scan_v9.06_Aug21_150-35p9_HHL_38-Full/",
        indir_secondary = "",
        label_secondary = "",
        glob_pattern = "*.log",
        is_gluino=True,
        minx = 600,
        maxx = 2300,
        miny = 0,
        maxy = 1800,
        minz = 1.0e-3,
        maxz = 2.0,
        diag_x1 = 600,
        diag_y1 = 430,
        diag_x2 = 1700,
        diag_y2 = 1530,
        lumi = 150,
        label_mass = "mass label",
        label_diag = "m_{#tilde{g}}-m_{#tilde{#chi}_{1}^{0}} = 2 #upoint (m_{W} + m_{b})",
        label_xaxis = "m_{#tilde{g}} (GeV)",
        label_yaxis = "m_{#tilde{#chi}_{1}^{0}} (GeV)",
        label_process = "pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow t#bar{t}#tilde{#chi}^{0}_{1}        ",
        outname = "test.pdf",
        ):

    d_vals = get_rvals(indir=indir, glob_pattern=glob_pattern, is_gluino=is_gluino, blinded=blinded)

    set_susy_palette()

    # Make the main canvas/pad
    c1 = r.TCanvas("c1", "", 800, 800)
    c1.cd()
    padt = r.TPad("p_tex", "p_tex", 0.0, 0.0, 1.0, 1.0);
    padt.SetTopMargin(0.1)
    padt.SetBottomMargin(0.1)
    padt.SetRightMargin(0.17)
    padt.SetLeftMargin(0.15)
    padt.Draw()
    padt.cd()
    padt.SetLogz()

    # Draw the frame first, which will control the axis ranges
    h_frame = r.TH2F("frame","",1,minx,maxx,1,miny,maxy)
    h_frame.Draw()

    # Since h_frame is the one with the x and y axes, set these properties here
    h_frame.GetXaxis().SetLabelSize(0.035)
    h_frame.GetXaxis().SetTitle(label_xaxis)
    h_frame.GetXaxis().SetTitleOffset(1.0)
    h_frame.GetXaxis().SetTitleSize(0.045)
    h_frame.GetYaxis().SetLabelSize(0.04)
    h_frame.GetYaxis().SetTitle(label_yaxis)
    h_frame.GetYaxis().SetTitleOffset(1.6)
    h_frame.GetYaxis().SetTitleSize(0.045)

    # Now draw the main color 2d hist (xsec UL), and since it's the one with the z-axis,
    # set z-axis properties on it
    g_xsec = r.TGraph2D("g_xsec","g_xsec",len(d_vals["mglu"]),d_vals["mglu"],d_vals["mlsp"],d_vals["xsec"])
    h_xsec = get_smoothed(g_xsec, return_hist=True)
    h_xsec.Draw("colzsame") # FIXME NOTE. If colors don't show up, the line below fixes it but I don't know why. it also screws everything else up btw
    # h_xsec.Draw("colz")

    h_xsec.GetZaxis().SetLabelSize(0.04)
    h_xsec.GetZaxis().SetRangeUser(minz,maxz)
    h_frame.GetZaxis().SetRangeUser(minz,maxz)
    h_xsec.GetZaxis().SetTitle("95% CL upper limit on cross section (pb)")
    h_xsec.GetZaxis().SetTitleOffset(1.3)
    h_xsec.GetZaxis().SetTitleSize(0.045)


    # Draw all the contours, skipping observed quantities if blinded
    # Store stuff in dictionaries to persist them and protect from ROOT garbage collection
    d_graphs = {}
    d_contours = {}
    for name,color,width,style in [
            ("obs", r.kBlack, 4, 1),
            ("om1", r.kBlack, 2, 1),
            ("op1", r.kBlack, 2, 1),
            ("exp", r.kRed, 4, 2),
            ("sm1", r.kRed, 2, 2),
            ("sp1", r.kRed, 2, 2),
            ("sm2", r.kRed, 2, 3),
            ("sp2", r.kRed, 2, 3),
            ]:
        g = r.TGraph2D("g_{}".format(name), "", len(d_vals["mglu"]), d_vals["mglu"], d_vals["mlsp"], d_vals[name])
        c = get_smoothed(g, nsmooth=0, diagonal_fudge=0.)
        c.SetLineWidth(width)
        c.SetLineStyle(style)
        c.SetLineColor(color)
        if (not blinded) or (name not in ["obs", "om1","op1"]):
            c.Draw("L same")
        d_graphs[name] = g
        d_contours[name] = c
    
    if indir_secondary:
        d_vals_secondary = get_rvals(indir=indir_secondary, glob_pattern=glob_pattern, is_gluino=is_gluino, blinded=blinded)
        for name,color,width,style in [
                ("exp", r.kBlue, 4, 2),
                ("sm1", r.kBlue, 2, 2),
                ("sp1", r.kBlue, 2, 2),
                # ("sm2", r.kBlue, 2, 3),
                # ("sp2", r.kBlue, 2, 3),
                ]:
            g = r.TGraph2D("g_{}_secondary".format(name), "", len(d_vals_secondary["mglu"]), d_vals_secondary["mglu"], d_vals_secondary["mlsp"], d_vals_secondary[name])
            c = get_smoothed(g, nsmooth=0, diagonal_fudge=0.)
            c.SetLineWidth(width)
            c.SetLineStyle(style)
            c.SetLineColor(color)
            if (not blinded) or (name not in ["obs", "om1","op1"]):
                c.Draw("L same")
            d_graphs[name+"_secondary"] = g
            d_contours[name+"_secondary"] = c

    # Get axis ticks back on top
    padt.Update()
    padt.RedrawAxis()

    # Draw diagonal line, and text, computing proper angle from aspect ratio/margins
    pix_per_xval = 1.0*padt.GetWw()*(1.0-padt.GetLeftMargin()-padt.GetRightMargin())/(maxx-minx)
    pix_per_yval = 1.0*padt.GetWh()*(1.0-padt.GetTopMargin()-padt.GetBottomMargin())/(maxy-miny)
    angle = np.arctan2((diag_y2-diag_y1)*pix_per_yval,(diag_x2-diag_x1)*pix_per_xval)*180./np.pi

    def xy_to_ndc(x,y):
        return (padt.GetLeftMargin() + 1.0*(x-minx)/(maxx-minx)*(1.0-padt.GetLeftMargin()-padt.GetRightMargin()),
                padt.GetBottomMargin() + 1.0*(y-miny)/(maxy-miny)*(1.0-padt.GetBottomMargin()-padt.GetTopMargin()))

    diag = r.TLine(diag_x1,diag_y1,diag_x2,diag_y2)
    diag.SetLineWidth(1)
    diag.SetLineStyle(2)
    k = 0.7
    x,y = xy_to_ndc(k*diag_x1+(1-k)*diag_x2,k*diag_y1+(1-k)*diag_y2)
    diagtex = r.TLatex(x,y,label_diag)
    diagtex.SetNDC()
    diagtex.SetTextSize(0.03)
    diagtex.SetTextAngle(angle)
    diagtex.SetTextAlign(20)
    diagtex.SetLineWidth(2)
    diagtex.SetTextFont(42)
    diag.Draw("same")
    diagtex.Draw("same")

    # CMS text, energy/lumi
    cmstex = r.TLatex(0.575,0.91, "{} fb^{{-1}} (13 TeV)".format(lumi) )
    cmstex.SetNDC()
    cmstex.SetTextSize(0.04)
    cmstex.SetLineWidth(2)
    cmstex.SetTextFont(42)
    cmstexbold = r.TLatex(0.16,0.91, "CMS" )
    cmstexbold.SetNDC()
    cmstexbold.SetTextSize(0.05)
    cmstexbold.SetLineWidth(2)
    cmstexbold.SetTextFont(61)
    cmstexprel = r.TLatex(0.28,0.91, "Preliminary" )
    cmstexprel.SetNDC()
    cmstexprel.SetTextSize(0.04)
    cmstexprel.SetLineWidth(2)
    cmstexprel.SetTextFont(52)
    cmstex.Draw("same")
    cmstexbold.Draw("same")
    cmstexprel.Draw("same")

    yshift = 0.
    if indir_secondary:
        yshift = 0.065

    if label_mass:
        masstex = r.TLatex(0.18,0.67-yshift, label_mass)
        masstex.SetNDC()
        masstex.SetTextSize(0.03)
        masstex.SetLineWidth(2)
        masstex.SetTextFont(42)
        masstex.Draw("same")

    # Draw legend at top
    l1 = r.TLegend(0.15, 0.70-yshift, 0.83, 0.90)
    l1.SetTextFont(42)
    l1.SetTextSize(0.036)
    l1.SetShadowColor(r.kWhite)
    # l1.SetFillColorAlpha(r.kWhite, 0.5)
    l1.SetFillColorAlpha(r.kWhite, 1.0)
    l1.SetHeader("{}  NLO+NLL exclusion".format(label_process))
    l1.AddEntry(d_contours["obs"] , "Observed #pm 1 #sigma_{theory}", "l")
    l1.AddEntry(d_contours["exp"] , "Expected #pm 1 and 2 #sigma_{experiment}", "l")
    l1.Draw("same")

    # Draw all the little lines for the legend markers using many magic numbers
    d_legendlines = {}
    for name,color,width,style,magicy in [
            ["LExpM2", r.kRed, 2, 3, 2.33],
            ["LExpM1", r.kRed, 2, 2, 2.23],
            ["LExpP1", r.kRed, 2, 2, 1.93],
            ["LExpP2", r.kRed, 2, 3, 1.83],
            ["LObsM1", r.kBlack, 2, 1, 1.10],
            ["LObsP1", r.kBlack, 2, 1, 1.40],
            ]:
        line = r.TGraph(2)
        line.SetName(name)
        line.SetTitle(name)
        line.SetLineColor(color)
        line.SetLineStyle(style)
        line.SetLineWidth(width)
        line.SetMarkerStyle(20)
        line.SetPoint(0,minx+ 3.8*(maxx-minx)/100, maxy-magicy*(maxy-miny)/100*10)
        line.SetPoint(1,minx+21.2*(maxx-minx)/100, maxy-magicy*(maxy-miny)/100*10)
        line.Draw("LSAME")
        d_legendlines[name] = line

    if indir_secondary:
        l1.AddEntry(d_contours["exp_secondary"] , label_secondary, "l")
        for name,color,width,style,magicy in [
                # ["LExpM2", r.kBlue, 2, 3, 2.33+0.82],
                ["LExpM1", r.kBlue, 2, 2, 2.23+0.82],
                ["LExpP1", r.kBlue, 2, 2, 1.93+0.82],
                # ["LExpP2", r.kBlue, 2, 3, 1.83+0.82],
                ]:
            line = r.TGraph(2)
            line.SetName(name)
            line.SetTitle(name)
            line.SetLineColor(color)
            line.SetLineStyle(style)
            line.SetLineWidth(width)
            line.SetMarkerStyle(20)
            line.SetPoint(0,minx+ 3.8*(maxx-minx)/100, maxy-magicy*(maxy-miny)/100*10)
            line.SetPoint(1,minx+21.2*(maxx-minx)/100, maxy-magicy*(maxy-miny)/100*10)
            line.Draw("LSAME")
            d_legendlines[name+"_secondary"] = line

    c1.SaveAs(outname)
    os.system("test ic && ic {}".format(outname))


if __name__ == "__main__":

    # draw_limits(
    #     indir = "Scan_v9.06_Aug21_150-35p9_HHL_38-Full/",
    #     glob_pattern = "*.log",
    #     outname = "test.pdf",
    #     # outname = "test.png",
    #         )

    if False:
        draw_limits(

            # indir = "v3.09_ML_fullscan",
            # indir_secondary = "v3.09_AN_fullscan",
            # label_secondary = "AN",

            indir = "v3.09_AN_fullscan",
            indir_secondary = "v3.09_AN_newfullscan",
            label_secondary = "new",

            # indir = "Scan_v9.06_Mar6_35p9_reminiaodfilt_t5qqqqt6ttww_met/",

            # indir = "v3.09_AN_fullscan",
            # indir_secondary = "Scan_v9.06_Mar6_35p9_reminiaodfilt_t5qqqqt6ttww_met/",
            # label_secondary = "2016 analysis",

            glob_pattern = "*fs_t6ttww_*.log",
            is_gluino = False,
            outname = "test.pdf",
            minx = 300,
            maxx = 1025,
            miny = 75,
            maxy = 1175,
            diag_x1 = 300,
            diag_y1 = 300-85,
            diag_x2 = 960,
            diag_y2 = 970-85,
            lumi = 35.9,
            label_mass = "m_{#tilde{#chi}^{0}_{1}} = 50 GeV",
            label_diag = "m_{#tilde{b}_{1}} - m_{#tilde{#chi}_{1}^{#pm}} = m_{W} + m_{b}",
            label_xaxis = "m_{#tilde{b}_{1}} (GeV)",
            label_yaxis = "m_{#tilde{#chi}_{1}^{#pm}} (GeV)",
            label_process = "pp #rightarrow #tilde{b}_{1}#bar{#tilde{b}}_{1}, #tilde{b}_{1}#rightarrow tW#tilde{#chi}^{0}_{1}      ",
            )

    # fname = "test.png"
    # compare_two_scans("v3.09_AN_fullscan", "v3.09_ML_fullscan", glob_pattern="*fs_t6ttww_*.log", fname=fname)
    # os.system("ic "+fname)

    fname = "test.pdf"
    # compare_two_scans("v3.09_ML_fullscan", "v3.09_prefire2017_v2", glob_pattern="*fs_t1tttt_*.log", fname=fname)
    compare_two_scans("v3.09_nominal_v2", "v3.09_prefire2017_v2", glob_pattern="*fs_t1tttt_*.log", fname=fname, nbinsx=12,nbinsy=12, zrange=[0.95,1.05], palette=r.kLightTemperature, title="prefire SF / nominal")
    os.system("ic "+fname)
