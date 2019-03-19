import itertools
import pickle
import array
import os
import numpy as np
import re
import fnmatch
import glob
import pandas as pd

import ROOT as r
r.gROOT.SetBatch(True)

import argparse
import sys

np.set_printoptions(suppress=True,threshold=200,precision=5)

def parse_log(fname):
    d = {}
    with open(fname,"r") as fhin:
        for line in fhin:
            if line.startswith("Observed"): d["obs"] = float(line.split("<")[-1])
            elif line.startswith("Expected") and "<" in line: d["exp_"+line.split("%")[0].replace("Expected","").strip()] = float(line.split("<")[-1])
            elif line.startswith("Significance"): d["sigma"] = float(line.split(":")[-1])

    obs = d.get("obs",-1)
    exp = d.get("exp_50.0",-1)
    exp_sm1 = d.get("exp_16.0",-1)
    exp_sp1 = d.get("exp_84.0",-1)
    exp_sm2 = d.get("exp_2.5",-1)
    exp_sp2 = d.get("exp_97.5",-1)
    sigma = d.get("sigma",-1)
    # FIXME unblind later
    # obs = exp
    return {
            "sigma":sigma,
            "obs":obs, "exp":exp,
            "sp1":exp_sp1,"sm1":exp_sm1,
            "sp2":exp_sp2,"sm2":exp_sm2,
            }

def parse_logs(biglog):
    logbase = biglog.replace(".txt","").replace(".pkl","")
    if not os.path.exists("{}.pkl".format(logbase)):
        with open("{}.txt".format(logbase)) as fh:
            proc = None
            d = {}
            for line in fh:
                if line.startswith("Start working") and ("upper limit" in line):
                    proc = line.split("Start working on",1)[1].split()[0].strip().replace(".root",".log")
                    d[proc] = {}
                elif line.startswith("Observed"): d[proc]["obs"] = float(line.split("<")[-1])
                elif line.startswith("Expected") and "<" in line: d[proc]["exp_"+line.split("%")[0].replace("Expected","").strip()] = float(line.split("<")[-1])
                elif line.startswith("Significance"): d[proc]["sigma"] = float(line.split(":")[-1])
            dnew = {}
            for k,v in d.items():
                obs = v.get("obs",-1)
                exp = v.get("exp_50.0",-1)
                exp_sm1 = v.get("exp_16.0",-1)
                exp_sp1 = v.get("exp_84.0",-1)
                exp_sm2 = v.get("exp_2.5",-1)
                exp_sp2 = v.get("exp_97.5",-1)
                sigma = v.get("sigma",-1)
                dnew[k] = {
                        "sigma":sigma,
                        "obs":obs, "exp":exp,
                        "sp1":exp_sp1,"sm1":exp_sm1,
                        "sp2":exp_sp2,"sm2":exp_sm2,
                        }
        with open("{}.pkl".format(logbase), "w") as fh:
            pickle.dump(dnew,fh)
    with open("{}.pkl".format(logbase), "r") as fh:
        d = pickle.load(fh)
    return d

def set_susy_palette():
    r.gStyle.SetOptStat(0)
    mypalette = array.array('i')
    NRGBs = 5
    NCont = 255
    stops = array.array('d', [0.00, 0.34, 0.61, 0.84, 1.00])
    red   = array.array('d', [0.50, 0.50, 1.00, 1.00, 1.00])
    green = array.array('d', [0.50, 1.00, 1.00, 0.60, 0.50])
    blue  = array.array('d', [1.00, 1.00, 0.50, 0.40, 0.50])
    # NRGBs = 8
    # NCont = 255
    # stops = array.array('d', [0.00, 0.34, 0.499, 0.50, 0.501, 0.61, 0.84, 1.00])
    # red   = array.array('d', [0.50, 0.50, 0.75, 1.00, 0.75, 1.00, 1.00, 1.00])
    # green = array.array('d', [0.50, 1.00, 1.00, 1.00, 1.00, 1.00, 0.60, 0.50])
    # blue  = array.array('d', [1.00, 1.00, 0.75, 1.00, 0.75, 0.50, 0.40, 0.50])
    FI = r.TColor.CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont)
    for i in range (0,NCont): mypalette.append(FI+i)
    r.gStyle.SetPalette(NCont,mypalette)
    r.gStyle.SetNumberContours(NCont)

def get_smoothed(graph, nsmooth=0, algo="k5b", diagonal_fudge=0, return_hist=False, cval=1.0, persist=[],set_high_above_diag=True):
    graph_ = graph.Clone()
    # Turn TGraph into TH2F
    h2 = graph_.GetHistogram()

    # Keep list of (xbin,ybin)s for empty bins
    emptybins = set([])
    for xbin in range(1,h2.GetNbinsX()+1):
        for ybin in range(1,h2.GetNbinsY()+1):
            if h2.GetBinContent(xbin,ybin) < 1.e-6:
                if h2.GetBinError(xbin,ybin) < 1.e-6:
                    emptybins.add((xbin,ybin))

    # Smear to the left and up
    for ybin in range(2,h2.GetNbinsY()+1):
        for xbin in range(1,h2.GetNbinsX()+1)[::-1]:
            if h2.GetBinContent(xbin,ybin) < 1.e-6:
                h2.SetBinContent(xbin,ybin,h2.GetBinContent(xbin+1,ybin-1))
                # h2.SetBinContent(xbin,ybin,h2.GetBinContent(xbin+1,ybin))
                # h2.SetBinContent(xbin,ybin,h2.GetBinContent(xbin,ybin-1))

    # If nsmooth positive, make root do that many smooths
    # if negative, make root do "1" smooth, but we call the functnion nsmooth times
    # apparently they are different...
    if nsmooth != 0:
        if nsmooth > 0:
            h2.Smooth(1,algo)
        else:
            for i in range(abs(nsmooth)):
                h2.Smooth(1,algo)

    hspill = h2.Clone("spill")
    hspill.Reset()

    # Reset empty bins to 0, since smoothing spilled over diagonal
    for xbin in range(1,h2.GetNbinsX()+1):
        for ybin in range(1,h2.GetNbinsY()+1):
            if (xbin,ybin) in emptybins:
                if set_high_above_diag:
                    h2.SetBinContent(xbin,ybin,(0 if return_hist else 1.1))
                hspill.SetBinContent(xbin,ybin,1.) # keep track of bins we cleaned up

    # Want to be able to give a TGraph2D and get back a smoothed (and cleaned) histogram back
    if return_hist:
        persist.append(graph_)
        persist.append(h2)
        return h2

    # Now get the contour
    cont = r.TGraph2D(h2).GetContourList(cval)
    if not cont:
        return None

    # Get list of x,y points for contour
    points = zip(list(cont[0].GetX()),list(cont[0].GetY()))

    # Go through and delete points close to diagonal if they are messed up
    newpoints = []
    # how many GeV below diagonal to delete contour poinst for - may need to tune
    diagonal_deletion_amount = diagonal_fudge
    for pt in points:
        xbin = h2.GetXaxis().FindBin(pt[0])
        # ybin = h2.GetYaxis().FindBin(pt[1]+diagonal_deletion_amount)
        ybin = h2.GetYaxis().FindBin(pt[1])
        ybinhigher = h2.GetYaxis().FindBin(pt[1]+12.5)
        val = h2.GetBinContent(xbin,ybin)
        # Delete points that are in a bin we cleaned up due to spillover
        # or if it's up to 12.5GeV lower than a point that is within such a bin
        if (hspill.GetBinContent(xbin,ybin) < 0.5) and (hspill.GetBinContent(xbin,ybinhigher) < 0.5):
            newpoints.append(pt)
        # else:
        #     print pt[0], pt[1]

    # Make new temporary TGraph and put in the "good" points from above
    gtest = r.TGraph(len(newpoints))
    for i,(x,y) in enumerate(newpoints):
        gtest.SetPoint(i,x,y)
    return gtest

def get_rvals(
        # indir = "Scan_v9.06_Aug21_150-35p9_HHL_38-Full/",
        biglog = "biglog_v1.txt",
        glob_pattern = "*.log",
        is_gluino = True,
        blinded = True,
        ):

    # lognames = glob.glob(indir + "/" + glob_pattern)
    parsed_logs = parse_logs(biglog)
    lognames = filter(lambda x: fnmatch.fnmatch(x,glob_pattern), parsed_logs.keys())
    fxsec = r.TFile("xsecs/xsec_susy_13tev.root")
    hxsec = fxsec.Get("h_xsec_gluino" if is_gluino else "h_xsec_stop")

    rscale = 1.
    is_t6tthzbrz = False
    if "t6tthzbrz" in lognames[0]:
        is_t6tthzbrz = True
        # Some mass points only have ZZ to begin with, so when we scaled invFilterEff in the looper, we made the exclusion too strong for them
        print "-"*50
        print "This is T6ttHZ with BR(Z)=100%, so scaling some r values by 1/(0.4*0.4)"
        print "-"*50

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
            "sigma": [],
            }
    for logname in lognames:
        mglu, mlsp = map(int,re.findall(r"_m([0-9]+)", logname))
        # lims = parse_log(logname)
        lims = parsed_logs[logname]

        # if any of the numbers are -1 (dummy default), skip bad point
        if any(lims[k]<0. for k in ["exp", "sm1", "sp1", "sm2", "sp2"]): continue

        rscale = 1.
        if is_t6tthzbrz and mglu-mlsp < 101:
            rscale = 1/(0.4*0.4)

        for k in ["obs", "exp", "sm1", "sp1", "sm2", "sp2"]:
            d_vals[k].append(rscale*lims[k])
        glu_xsec = hxsec.GetBinContent(hxsec.FindBin(mglu))
        glu_xsec_err = hxsec.GetBinError(hxsec.FindBin(mglu))
        if blinded:
            lims["obs"] = lims["exp"]
        plane = rscale*lims["obs"]
        xsec = plane*glu_xsec
        d_vals["om1"].append(rscale*lims["obs"]*(glu_xsec-glu_xsec_err)/glu_xsec)
        d_vals["op1"].append(rscale*lims["obs"]*(glu_xsec+glu_xsec_err)/glu_xsec)
        d_vals["mglu"].append(1.0*mglu)
        d_vals["mlsp"].append(1.0*mlsp)
        d_vals["glu_xsec"].append(1.0*glu_xsec)
        d_vals["xsec"].append(1.0*xsec)
        d_vals["sigma"].append(lims["sigma"])

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
        do_significance = False,
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

    # d_vals = get_rvals(indir=indir, glob_pattern=glob_pattern, is_gluino=is_gluino, blinded=blinded)
    d_vals = get_rvals(biglog=indir, glob_pattern=glob_pattern, is_gluino=is_gluino, blinded=blinded)
    # Delete points where obs-exp is larger than 6*(2sigmaup-expected)
    bad = (d_vals["obs"]-(d_vals["exp"]))/(d_vals["sp2"]-d_vals["exp"]) > 6.
    for k in d_vals:
        d_vals[k] = d_vals[k][~bad]

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
    if not do_significance:
        padt.SetLogz()
    else:
        minz, maxz = -3., 3.
        outname = outname.replace("_xsec_","_signif_")

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
    if not do_significance:
        g_xsec = r.TGraph2D("g_xsec","g_xsec",len(d_vals["mglu"]),d_vals["mglu"],d_vals["mlsp"],d_vals["xsec"])
        h_xsec = get_smoothed(g_xsec, nsmooth=2, return_hist=True)
        h_xsec.Draw("colzsame") # FIXME NOTE. If colors don't show up, the line below fixes it but I don't know why. it also screws everything else up btw
        # h_xsec.Draw("colz")

        r_value_overlay = False
        if r_value_overlay:
            m1s = d_vals["mglu"]
            m2s = d_vals["mlsp"]
            rvals = d_vals["obs"]
            lut = np.c_[m1s,m2s,rvals]
            nx = h_xsec.GetNbinsX()
            ny = h_xsec.GetNbinsY()
            xlow = h_xsec.GetXaxis().GetBinLowEdge(1)
            ylow = h_xsec.GetYaxis().GetBinLowEdge(1)
            xhigh = h_xsec.GetXaxis().GetBinUpEdge(nx)
            yhigh = h_xsec.GetYaxis().GetBinUpEdge(nx)
            hnew = r.TH2F("hnew","hnew",nx//4,xlow+25.,xhigh-25.,ny//4,ylow+25.,yhigh-25.)
            def find_nearest(x,y):
                dists = np.hypot(lut[:,0]-x,lut[:,1]-y)
                return lut[np.argmin(dists)][-1]
            for ix in range(1,hnew.GetNbinsX()+1):
                for iy in range(1,hnew.GetNbinsY()+1):
                    x,y = hnew.GetXaxis().GetBinCenter(ix), hnew.GetYaxis().GetBinCenter(iy)
                    if h_xsec.GetBinContent(h_xsec.FindBin(x,y)) < 1e-6: continue
                    v = find_nearest(x,y)
                    hnew.SetBinContent(ix,iy,v)
            r.gStyle.SetPaintTextFormat("0.2f")
            hnew.Draw("sametext")
    else:
        g_xsec = r.TGraph2D("g_xsec","g_xsec",len(d_vals["mglu"]),d_vals["mglu"],d_vals["mlsp"],d_vals["sigma"])
        h_xsec = get_smoothed(g_xsec, nsmooth=2, return_hist=True)
        # opt 1 suppresses empty bins (0)
        h_xsec.Draw("colz1same")
        # h_xsec.Draw("colzsame")

        significance_value_overlay = True
        if significance_value_overlay:
            nx = h_xsec.GetNbinsX()
            ny = h_xsec.GetNbinsY()
            xlow = h_xsec.GetXaxis().GetBinLowEdge(1)
            ylow = h_xsec.GetYaxis().GetBinLowEdge(1)
            xhigh = h_xsec.GetXaxis().GetBinUpEdge(nx)
            yhigh = h_xsec.GetYaxis().GetBinUpEdge(nx)
            hnew = r.TH2F("hnew","hnew",nx//4,xlow+25.,xhigh-25.,ny//4,ylow+25.,yhigh-25.)
            for ix in range(1,hnew.GetNbinsX()+1):
                for iy in range(1,hnew.GetNbinsY()+1):
                    x,y = hnew.GetXaxis().GetBinCenter(ix), hnew.GetYaxis().GetBinCenter(iy)
                    hnew.SetBinContent(ix,iy,h_xsec.GetBinContent(h_xsec.FindBin(x,y)))
            r.gStyle.SetPaintTextFormat("0.2f")
            hnew.Draw("sametext")

    h_xsec.GetZaxis().SetLabelSize(0.04)
    h_xsec.GetZaxis().SetRangeUser(minz,maxz)
    h_frame.GetZaxis().SetRangeUser(minz,maxz)
    h_xsec.GetZaxis().SetTitle("95% CL upper limit on cross section (pb)" if not do_significance else "Significance [#sigma]")
    h_xsec.GetZaxis().SetTitleOffset(1.3)
    h_xsec.GetZaxis().SetTitleSize(0.045)


    # Draw all the contours, skipping observed quantities if blinded
    # Store stuff in dictionaries to persist them and protect from ROOT garbage collection
    d_graphs = {}
    d_contours = {}
    if not do_significance:
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
            extra = {"nsmooth":1}
            if "dm20" in outname: extra["nsmooth"] = 4
            if "t6tthzbrz" in outname: extra["set_high_above_diag"] = False
            if "t6tthzbrb" in outname:
                extra["nsmooth"] = -4
                extra["algo"] = "k5b"
            if "t6tthzbrh" in outname:
                extra["nsmooth"] = -5
                extra["algo"] = "k5b"
            c = get_smoothed(g, **extra)
            if not c: 
                print "Error with contour for {}".format(name)
                continue
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
                c = get_smoothed(g, nsmooth=0)
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
        y_mass = 0.67
        if "splitline" in label_mass: y_mass -= 0.04
        masstex = r.TLatex(0.18,y_mass-yshift, label_mass)
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
    if not do_significance:
        if len(label_process) > 50:
            l1.SetHeader("{}    #scale[0.85]{{#splitline{{NNLO+NNLL}}{{exclusion}}}}".format(label_process))
        else:
            l1.SetHeader("{}  NNLO+NNLL exclusion".format(label_process))
        l1.AddEntry(d_contours["obs"] , "Observed #pm 1 #sigma_{theory}", "l")
        l1.AddEntry(d_contours["exp"] , "Expected #pm 1 and 2 #sigma_{experiment}", "l")
    else:
        l1.SetHeader("{}  significance".format(label_process))
    l1.Draw("same")

    if not do_significance:
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

    if not do_significance:
        outdir = outname.rsplit("/",1)[0]
        tag = outname.rsplit("/",1)[1].split("_scan")[0]
        fout = r.TFile("{}/SUS-19-008_{}.root".format(outdir,tag),"RECREATE")
        hxsecwrite = h_xsec.Clone("xsec")
        hxsecwrite.Write()
        cobswrite = d_contours["obs"].Clone("ssobs")
        cobswrite.Write()
        com1write = d_contours["om1"].Clone("ssobs_m1s")
        com1write.Write()
        cop1write = d_contours["op1"].Clone("ssobs_p1s")
        cop1write.Write()
        cexpwrite = d_contours["exp"].Clone("ssexp")
        cexpwrite.Write()
        csm1write = d_contours["sm1"].Clone("ssexp_m1s")
        csm1write.Write()
        csp1write = d_contours["sp1"].Clone("ssexp_p1s")
        csp1write.Write()
        csm2write = d_contours["sm2"].Clone("ssexp_m2s")
        csm2write.Write()
        csp2write = d_contours["sp2"].Clone("ssexp_p2s")
        csp2write.Write()
        fout.Close()

    do_significance_1d = False
    if do_significance and do_significance_1d:
        sigmas = d_vals["sigma"]
        h = r.TH1F("significance_1d","significance_1d",15,-3,3.)
        for s in sigmas:
            h.Fill(s)
        h.SetTitle("#splitline{{1d significances: {}}}{{mean = {:.1f}, median = {:.1f}}}".format(label_process,sigmas.mean(),np.median(sigmas)))
        h.Draw("histpetext")
        # cmstex.Draw("same")
        # cmstexbold.Draw("same")
        # cmstexprel.Draw("same")
        fname = outname.replace(".pdf","_1d.pdf")
        c1.SaveAs(fname)
        os.system("test ic && ic {}".format(fname))


if __name__ == "__main__":

    parser = argparse.ArgumentParser(formatter_class=lambda prog: argparse.HelpFormatter(prog,max_help_position=35, width=150))
    parser.add_argument("-s", "--sig", help="do significance", action="store_true")
    parser.add_argument("-m", "--model", help="model str", default="", type=str)
    args = parser.parse_args()

    # print parse_log("v3.23_ss_fastsim_19Jan22/card_fs_t1tttt_m1750_m400_all_run2.log")

    # indir = "v3.23_ss_fastsim_19Jan22"
    # indir = "v3.24_fullsignals_v1/"

    # indir = "v3.26_feb15_sst1t5rpv_v1/"
    # outdir = "scanplots"

    # indir = "v3.26_feb22_ssallsigs_v1/"
    # indir = "v3.26_feb27_allsigs_v1"
    # indir2 = "v3.27_mar4_t5tttt_v1/"
    # indir = "batch/biglog_v1.txt"
    indir = "batch/biglog_v3.txt"
    # indir2 = "v3.27_mar4_t5tttt_v1/"
    outdir = "scanplots_test"

    dos = args.sig or False
    modstr = args.model or "t1tttt"
    # modstr = args.model or "t6ttww"
    # modstr = args.model or "t6tthzbrh"
    os.system("mkdir -p {}".format(outdir))

    # dos = True
    # modstr = "t5tttt"
    # modstr = "t6ttww"
    # modstr = "t5qqqqvv"
    # modstr = "t5qqqqww"
    # modstr = "t5qqqqvvdm20"
    # modstr = "t5qqqqwwdm20"
    # modstr = "t5tttt"

    # modstr = "t6tthzbrb"
    # modstr = "t6tthzbrh"
    # modstr = "t6tthzbrz"

    # FIXME can only plot one at a time because pyROOT has weird memory management

    if modstr == "t1tttt":
        draw_limits(
            indir = indir,
            glob_pattern = "*fs_t1tttt_m*.log",
            is_gluino = True,
            outname = "{}/t1tttt_scan_xsec_run2.pdf".format(outdir),
            do_significance = dos,
            minx = 600,
            maxx = 2200,
            miny = 0,
            maxy = 1800,
            diag_x1 = 600,
            diag_y1 = 600-170,
            diag_x2 = 1700,
            diag_y2 = 1700-170,
            lumi = 137,
            label_mass = "",
            label_diag = "m_{#tilde{g}}-m_{#tilde{#chi}_{1}^{0}} = 2 #upoint (m_{W} + m_{b})",
            label_xaxis = "m_{#tilde{g}} (GeV)",
            label_yaxis = "m_{#tilde{#chi}_{1}^{0}} (GeV)",
            label_process = "pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow t#bar{t}#tilde{#chi}^{0}_{1}        ",
            blinded=False,
            )

    if modstr == "t6ttww":
        draw_limits(
            indir = indir,
            glob_pattern = "*fs_t6ttww_m*.log",
            is_gluino = False,
            outname = "{}/t6ttww_scan_xsec_run2.pdf".format(outdir),
            do_significance = dos,
            minx = 300,
            maxx = 1325,
            miny = 75,
            maxy = 1175,
            diag_x1 = 300,
            diag_y1 = 300-85,
            diag_x2 = 960,
            diag_y2 = 970-85,
            lumi = 137,
            label_mass = "m_{#tilde{#chi}^{0}_{1}} = 50 GeV",
            label_diag = "m_{#tilde{b}_{1}} - m_{#tilde{#chi}_{1}^{#pm}} = m_{W} + m_{b}",
            label_xaxis = "m_{#tilde{b}_{1}} (GeV)",
            label_yaxis = "m_{#tilde{#chi}_{1}^{#pm}} (GeV)",
            label_process = "pp #rightarrow #tilde{b}_{1}#bar{#tilde{b}}_{1}, #tilde{b}_{1}#rightarrow tW#tilde{#chi}^{0}_{1}      ",
            blinded=False,
            )

    if modstr == "t5qqqqvv":
        draw_limits(
            indir = indir,
            glob_pattern = "*fs_t5qqqqvv_m*.log",
            is_gluino = True,
            do_significance = dos,
            outname = "{}/t5qqqqvv_scan_xsec_run2.pdf".format(outdir),
            minx = 600,
            maxx = 2000,
            miny = 0,
            maxy = 1800,
            diag_x1 = 600,
            diag_y1 = 600,
            diag_x2 = 1600,
            diag_y2 = 1600,
            lumi = 137,
            label_mass = "",
            label_diag = "m_{#tilde{g}} = m_{#tilde{#chi}_{1}^{0}}",
            label_xaxis = "m_{#tilde{g}} (GeV)",
            label_yaxis = "m_{#tilde{#chi}_{1}^{0}} (GeV)",
            label_process = "pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow q#bar{q}'V#tilde{#chi}^{0}_{1}     ",
            blinded=False,
            )

    if modstr == "t5qqqqvvdm20":
        draw_limits(
            indir = indir,
            glob_pattern = "*fs_t5qqqqvvdm20_m*.log",
            is_gluino = True,
            do_significance = dos,
            outname = "{}/t5qqqqvvdm20_scan_xsec_run2.pdf".format(outdir),
            minx = 600,
            maxx = 1700,
            miny = 0,
            maxy = 1600,
            diag_x1 = 600,
            diag_y1 = 600,
            diag_x2 = 1600,
            diag_y2 = 1600,
            lumi = 137,
            label_mass = "m_{#tilde{#chi}^{#pm}_{1}} = m_{#tilde{#chi}^{0}_{1}} + 20 GeV",
            label_diag = "m_{#tilde{g}} = m_{#tilde{#chi}_{1}^{0}}",
            label_xaxis = "m_{#tilde{g}} (GeV)",
            label_yaxis = "m_{#tilde{#chi}_{1}^{0}} (GeV)",
            label_process = "pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow q#bar{q}'V*#tilde{#chi}^{0}_{1}    ",
            blinded=False,
            )

    if modstr == "t5qqqqww":
        draw_limits(
            indir = indir,
            glob_pattern = "*fs_t5qqqqww_m*.log",
            is_gluino = True,
            do_significance = dos,
            outname = "{}/t5qqqqww_scan_xsec_run2.pdf".format(outdir),
            minx = 600,
            maxx = 2000,
            miny = 0,
            maxy = 1800,
            diag_x1 = 600,
            diag_y1 = 600,
            diag_x2 = 1600,
            diag_y2 = 1600,
            lumi = 137,
            label_mass = "",
            label_diag = "m_{#tilde{g}} = m_{#tilde{#chi}_{1}^{0}}",
            label_xaxis = "m_{#tilde{g}} (GeV)",
            label_yaxis = "m_{#tilde{#chi}_{1}^{0}} (GeV)",
            label_process = "pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow q#bar{q}'W#tilde{#chi}^{0}_{1}     ",
            blinded=False,
            )

    if modstr == "t5qqqqwwdm20":
        draw_limits(
            indir = indir,
            glob_pattern = "*fs_t5qqqqwwdm20_m*.log",
            is_gluino = True,
            do_significance = dos,
            outname = "{}/t5qqqqwwdm20_scan_xsec_run2.pdf".format(outdir),
            minx = 600,
            maxx = 1700,
            miny = 0,
            maxy = 1600,
            diag_x1 = 600,
            diag_y1 = 600,
            diag_x2 = 1600,
            diag_y2 = 1600,
            lumi = 137,
            label_mass = "m_{#tilde{#chi}^{#pm}_{1}} = m_{#tilde{#chi}^{0}_{1}} + 20 GeV",
            label_diag = "m_{#tilde{g}} = m_{#tilde{#chi}_{1}^{0}}",
            label_xaxis = "m_{#tilde{g}} (GeV)",
            label_yaxis = "m_{#tilde{#chi}_{1}^{0}} (GeV)",
            label_process = "pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow q#bar{q}'W*#tilde{#chi}^{0}_{1}    ",
            blinded=False,
            )

    if modstr == "t6tthzbrh":
        # pg 20 https://arxiv.org/pdf/1710.09154.pdf
        draw_limits(
            indir = indir,
            glob_pattern = "*fs_t6tthzbrh_m*.log",
            is_gluino = False,
            outname = "{}/t6tthzbrh_scan_xsec_run2.pdf".format(outdir),
            do_significance = dos,
            minx = 300,
            maxx = 1300,
            miny = 200,
            maxy = 1200,
            diag_x1 = 350,
            diag_y1 = 200,
            diag_x2 = 350+900,
            diag_y2 = 200+900,
            lumi = 137,
            label_mass = "#splitline{BR(#tilde{t}_{2}#rightarrow#tilde{t}_{1} Z)=0%}{m_{#tilde{t}_{1}}-m_{#tilde{#chi}_{1}^{0}}=175 GeV}",
            label_diag = "",
            label_xaxis = "m_{#tilde{t}_{2}} (GeV)",
            label_yaxis = "m_{#tilde{t}_{1}} (GeV)",
            label_process = "pp #rightarrow #tilde{t}_{2}#bar{#tilde{t}}_{2}, #tilde{t}_{2}#rightarrow #tilde{t}_{1}Z(H), #tilde{t}_{1}#rightarrow t#tilde{#chi}_{1}^{0} ",
            blinded=False,
            )

    if modstr == "t6tthzbrb":
        # pg 20 https://arxiv.org/pdf/1710.09154.pdf
        draw_limits(
            indir = indir,
            glob_pattern = "*fs_t6tthzbrb_m*.log",
            is_gluino = False,
            outname = "{}/t6tthzbrb_scan_xsec_run2.pdf".format(outdir),
            do_significance = dos,
            minx = 300,
            maxx = 1300,
            miny = 200,
            maxy = 1200,
            diag_x1 = 350,
            diag_y1 = 200,
            diag_x2 = 350+900,
            diag_y2 = 200+900,
            lumi = 137,
            label_mass = "#splitline{BR(#tilde{t}_{2}#rightarrow#tilde{t}_{1} Z)=50%}{m_{#tilde{t}_{1}}-m_{#tilde{#chi}_{1}^{0}}=175 GeV}",
            label_diag = "",
            label_xaxis = "m_{#tilde{t}_{2}} (GeV)",
            label_yaxis = "m_{#tilde{t}_{1}} (GeV)",
            label_process = "pp #rightarrow #tilde{t}_{2}#bar{#tilde{t}}_{2}, #tilde{t}_{2}#rightarrow #tilde{t}_{1}Z(H), #tilde{t}_{1}#rightarrow t#tilde{#chi}_{1}^{0} ",
            blinded=False,
            )

    if modstr == "t6tthzbrz":
        # pg 20 https://arxiv.org/pdf/1710.09154.pdf
        draw_limits(
            indir = indir,
            glob_pattern = "*fs_t6tthzbrz_m*.log",
            is_gluino = False,
            outname = "{}/t6tthzbrz_scan_xsec_run2.pdf".format(outdir),
            do_significance = dos,
            minx = 300,
            maxx = 1300,
            miny = 200,
            maxy = 1200,
            diag_x1 = 300,
            diag_y1 = 300,
            diag_x2 = 300+900,
            diag_y2 = 300+900,
            lumi = 137,
            label_mass = "#splitline{BR(#tilde{t}_{2}#rightarrow#tilde{t}_{1} Z)=100%}{m_{#tilde{t}_{1}}-m_{#tilde{#chi}_{1}^{0}}=175 GeV}",
            label_diag = "",
            label_xaxis = "m_{#tilde{t}_{2}} (GeV)",
            label_yaxis = "m_{#tilde{t}_{1}} (GeV)",
            label_process = "pp #rightarrow #tilde{t}_{2}#bar{#tilde{t}}_{2}, #tilde{t}_{2}#rightarrow #tilde{t}_{1}Z(H), #tilde{t}_{1}#rightarrow t#tilde{#chi}_{1}^{0} ",
            blinded=False,
            )

    if modstr == "t5tttt":
        draw_limits(
            indir = indir,
            glob_pattern = "*fs_t5tttt_m*.log",
            is_gluino = True,
            outname = "{}/t5tttt_scan_xsec_run2.pdf".format(outdir),
            do_significance = dos,
            minx = 800,
            maxx = 2300,
            miny = 0,
            maxy = 1900,
            diag_x1 = 800,
            diag_y1 = 540,
            diag_x2 = 800+1400,
            diag_y2 = 540+1400,
            lumi = 137,
            label_mass = "m_{#kern[0.5]{#tilde{t}_{1}}} = m_{#tilde{#chi}^{0}_{1}} + m_{#kern[0.3]{t}}",
            label_diag = "m_{#tilde{g}} - m_{#tilde{#chi}^{0}_{1}} = m_{t} + m_{W} + m_{b}",
            label_xaxis = "m_{#tilde{g}} (GeV)",
            label_yaxis = "m_{#tilde{#chi}_{1}^{0}} (GeV)",
            label_process = "pp #rightarrow #tilde{g}#tilde{g}, #tilde{g}#rightarrow #tilde{t}_{1}#kern[0.3]{t}, #tilde{t}_{1}#rightarrow t#tilde{#chi}^{0}_{1} ",
            blinded=False,
            )

#     fname = "test.pdf"
#     # compare_two_scans("v3.09_ML_fullscan", "v3.09_prefire2017_v2", glob_pattern="*fs_t1tttt_*.log", fname=fname)
#     compare_two_scans("v3.09_nominal_v2", "v3.09_prefire2017_v2", glob_pattern="*fs_t1tttt_*.log", fname=fname, nbinsx=12,nbinsy=12, zrange=[0.95,1.05], palette=r.kLightTemperature, title="prefire SF / nominal")
#     os.system("ic "+fname)
