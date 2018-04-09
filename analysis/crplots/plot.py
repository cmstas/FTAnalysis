import os
import numpy as np
import uproot

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery.plotter import plot_stack
from matplottery.utils import Hist1D, MET_LATEX

def make_plots(outputdir="plots", inputdir="outputs"):

    os.system("mkdir -p {}/".format(outputdir))

    bginfo = [
            # ("flips"    , "Charge misid." , [0.4, 0.4, 0.4]),
            # ("rares"    , "Rare"          , [1.0, 0.4, 1.0]),
            ("wjets"       , "W+jets"    , [0.4, 0.0, 0.8]),
            # ("xg"       , "$X\\gamma$"    , [0.4, 0.0, 0.8]),
            ("dy"     , "DY+jets" , [0.4, 0.6, 1.0]),
            ("ttz"      , "$t\\bar{t}Z$"  , [0.4, 0.8, 0.4]),
            # ("tt" , "$t\\bar{t}$", [0.9, 0.9, 0.9]),
            ("tt" , "$t\\bar{t}$", [0.8, 0.8, 0.8]),
            # ("tth"      , "$t\\bar{t}H$"  , [0.4, 0.4, 0.6]),
            ("ttw"      , "$t\\bar{t}W$"  , [0.0, 0.4, 0.0]),
            ]

    procs = [p[0] for p in bginfo]
    files = { proc:uproot.open("{}/histos_{}.root".format(inputdir,proc)) for proc in (procs+["data"]) }

    for which in ["tl", "os"]:
        title = "Tight-Loose" if which == "tl" else "OS"
        for var, xlabel in [
                ["met", MET_LATEX],
                ["ht", "$H_{T}$"],
                ["mtmin", "$m_{T}^\\mathrm{min}$"],
                ["njets", "Njets"],
                ["nbtags", "Nbtags"],
                ["pt1", "$p_T$(lep1)"],
                ["pt2", "$p_T$(lep2)"],
                ["pte", "$p_T$(e)"],
                ["ptm", "$p_T$(m)"],
                ["eta1", "$\\eta$(lep1)"],
                ["eta2", "$\\eta$(lep2)"],
                ["mll", "$m_{ll}$"],
                ["type", "hyp type (mm, em, ee)"],
                ["nvtx", "# good vertices"],
                ]:

            hname = "{}_{}".format(which,var)

            bgs = [Hist1D(files[proc][hname], label=label,color=color) for proc,label,color in bginfo]
            bgs = sorted(bgs, key=lambda bg: bg.get_integral())
            data = Hist1D(files["data"][hname], label="Data")
            # figure out why this is even needed. did we sumw2?
            data._errors = data._counts ** 0.5

            fname = "{}/{}_{}.png".format(outputdir,which,var)
            plot_stack(bgs=bgs, data=data, title=title, xlabel=xlabel, filename=fname,
                    cms_type = "Preliminary",
                    lumi = "41.3",
                    ratio_range=[0.5,1.5],
                    )
            print "Wrote {}".format(fname)

if __name__ == "__main__":
    make_plots(
            outputdir="plots",
            inputdir="outputs",
            )
            
