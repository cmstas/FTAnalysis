from tqdm import tqdm
import json
import os
import numpy as np
import uproot
from analysis.limits.errors import E

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery.plotter import plot_stack
from matplottery.utils import Hist1D, MET_LATEX

labels = {

    "ht": "$H_{T}$",
    "met": MET_LATEX,
    "mll": "$m_{ll}$",
    "njets": "Njets",
    "nbtags": "Nbtags",
    "type": "hyp type (mm, em, ee)",
    "matchtype": "unmatched/light, b, c",
    "mtmin": "$m_{T}^\\mathrm{min}$",
    "nleps": "nleps",
    "nmiss1": "exp miss inner hits - lep 1",
    "nmiss2": "exp miss inner hits - lep 2",
    "pt1": "$p_T$(lep1)",
    "pt2": "$p_T$(lep2)",
    "pt3": "$p_T$(lep3)",
    "pte": "$p_T$(e)",
    "ptm": "$p_T$(m)",
    "eta1": r"$\eta$(lep1)",
    "eta2": r"$\eta$(lep2)",
    "etae": r"$\eta$(e)",
    "etam": r"$\eta(\mu)$",
    "ptrel1": "$p_T$rel - lep 1",
    "ptrel2": "$p_T$rel - lep 2",
    "ptrele": "$p_T$rel - e",
    "ptrelm": r"$p_T$rel - $\mu$",
    "ptratio1": "$p_T$ratio - lep 1",
    "ptratio2": "$p_T$ratio - lep 2",
    "ptratioe": "$p_T$ratio - e",
    "ptratiom": r"$p_T$ratio - $\mu$",
    "ptrelfail1": "$p_T$rel - lep 1, fail $p_T$ratio",
    "ptrelfail2": "$p_T$rel - lep 2, fail $p_T$ratio",
    "ptrelfaile": "$p_T$rel - e, fail $p_T$ratio",
    "ptrelfailm": r"$p_T$rel - $\mu$, fail $p_T$ratio",
    "miniiso1": "mini-iso - lep 1",
    "miniiso2": "mini-iso - lep 2",
    "miniisoe": "mini-iso - e",
    "miniisom": r"mini-iso - $\mu$",
    "q1": "charge - lep 1",
    "nvtx": "# good vertices",
    "nisrjets": "Njets (ISR)",
    "nisrmatch": "Njets (ISR - Truth Matched)",

    # "rawmet": "Raw "+MET_LATEX,
    # "htb": "$H_{T} B-Jets$",
    # "zmll": "$m_{ll} (Z-cand)$",
    # "nlb40": "N-loose Btags, $p_T$>40",
    # "ntb40": "N-tight Btags, $p_T$>40",
    # "type3l": "3l type (mmm, mme, mee, eee)",

}

bginfo = {
        "os": {
            "flips":                   (r"Charge misid.",  [0.4, 0.4, 0.4]),
            "dy":                      (r"DY+jets",        [0.4, 0.6, 1.0]),
            "ttz":                     (r"$t\bar{t}Z$",    [0.4, 0.8, 0.4]),
            "tt":                      (r"$t\bar{t}$",     [0.8, 0.8, 0.8]),
            "tth":                     (r"$t\bar{t}H$",    [0.4, 0.4, 0.6]),
            "ttw":                     (r"$t\bar{t}W$",    [0.0, 0.4, 0.0]),
            "vv":                      (r"VV",             [0.0, 0.4, 0.8]),
            "rares":                   ("Rare",            [1.0, 0.4, 1.0]),
            "singletop":               ("Single Top",      [1.0, 0.4, 0.0]),
            },
        "osloose": {
            "flips":                   (r"Charge misid.",  [0.4, 0.4, 0.4]),
            "dy":                      (r"DY+jets",        [0.4, 0.6, 1.0]),
            "ttz":                     (r"$t\bar{t}Z$",    [0.4, 0.8, 0.4]),
            "tt":                      (r"$t\bar{t}$",     [0.8, 0.8, 0.8]),
            "tth":                     (r"$t\bar{t}H$",    [0.4, 0.4, 0.6]),
            "ttw":                     (r"$t\bar{t}W$",    [0.0, 0.4, 0.0]),
            "vv":                      (r"VV",             [0.0, 0.4, 0.8]),
            "rares":                   ("Rare",            [1.0, 0.4, 1.0]),
            "singletop":               ("Single Top",      [1.0, 0.4, 0.0]),
            },
        "htnb1": {
            "flips":                   (r"Charge misid.",  [0.4, 0.4, 0.4]),
            "ttz":                     (r"$t\bar{t}Z$",    [0.4, 0.8, 0.4]),
            "tth":                     (r"$t\bar{t}H$",    [0.4, 0.4, 0.6]),
            "ttw":                     (r"$t\bar{t}W$",    [0.0, 0.4, 0.0]),
            "vv":                      (r"VV",             [0.0, 0.4, 0.8]),
            "fakes":                   (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
            # "ttfake":                   (r"$t\bar{t}$ Nonprompt", [0.85, 0.85, 0.85]),
            "rares":                   ("Rare",            [1.0, 0.4, 1.0]),
            # "singletop":               ("Single Top",      [1.0, 0.4, 0.0]),
            },
        "htnb1mc": {
            "flips":                   (r"Charge misid.",  [0.4, 0.4, 0.4]),
            "ttz":                     (r"$t\bar{t}Z$",    [0.4, 0.8, 0.4]),
            "tth":                     (r"$t\bar{t}H$",    [0.4, 0.4, 0.6]),
            "ttw":                     (r"$t\bar{t}W$",    [0.0, 0.4, 0.0]),
            "vv":                      (r"VV",             [0.0, 0.4, 0.8]),
            # "fakes":                   (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
            "ttfake":                   (r"$t\bar{t}$ Nonprompt", [0.85, 0.85, 0.85]),
            "rares":                   ("Rare",            [1.0, 0.4, 1.0]),
            "singletop":               ("Single Top",      [1.0, 0.4, 0.0]),
            },
        "crz": {
            "flips":                   (r"Charge misid.",  [0.4, 0.4, 0.4]),
            "ttz":                     (r"$t\bar{t}Z$",    [0.4, 0.8, 0.4]),
            "tth":                     (r"$t\bar{t}H$",    [0.4, 0.4, 0.6]),
            "ttw":                     (r"$t\bar{t}W$",    [0.0, 0.4, 0.0]),
            "vv":                      (r"VV",             [0.0, 0.4, 0.8]),
            "fakes":                   (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
            "rares":                   ("Rare",            [1.0, 0.4, 1.0]),
            # "singletop":               ("Single Top",      [1.0, 0.4, 0.0]),
            },
        "crw": {
            "flips":                   (r"Charge misid.",  [0.4, 0.4, 0.4]),
            "ttz":                     (r"$t\bar{t}Z$",    [0.4, 0.8, 0.4]),
            "tth":                     (r"$t\bar{t}H$",    [0.4, 0.4, 0.6]),
            "ttw":                     (r"$t\bar{t}W$",    [0.0, 0.4, 0.0]),
            "vv":                      (r"VV",             [0.0, 0.4, 0.8]),
            "fakes":                   (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
            "rares":                   ("Rare",            [1.0, 0.4, 1.0]),
            # "singletop":               ("Single Top",      [1.0, 0.4, 0.0]),
            },
        "br": {
            "flips":                   (r"Charge misid.",  [0.4, 0.4, 0.4]),
            "ttz":                     (r"$t\bar{t}Z$",    [0.4, 0.8, 0.4]),
            "tth":                     (r"$t\bar{t}H$",    [0.4, 0.4, 0.6]),
            "ttw":                     (r"$t\bar{t}W$",    [0.0, 0.4, 0.0]),
            "vv":                      (r"VV",             [0.0, 0.4, 0.8]),
            "fakes":                   (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
            "rares":                   ("Rare",            [1.0, 0.4, 1.0]),
            # "singletop":               ("Single Top",      [1.0, 0.4, 0.0]),
            },
        "tt_isr": {
            "dy":                      (r"DY+jets",        [0.4, 0.6, 1.0]),
            "ttz":                     (r"$t\bar{t}Z$",    [0.4, 0.8, 0.4]),
            "tt":                      (r"$t\bar{t}$",     [0.8, 0.8, 0.8]),
            "tth":                     (r"$t\bar{t}H$",    [0.4, 0.4, 0.6]),
            "ttw":                     (r"$t\bar{t}W$",    [0.0, 0.4, 0.0]),
            "vv":                      (r"VV",             [0.0, 0.4, 0.8]),
            "rares":                   ("Rare",            [1.0, 0.4, 1.0]),
            "singletop":               ("Single Top",      [1.0, 0.4, 0.0]),
            },
        "tt_isr_reweight_check": {
            "dy":                      (r"DY+jets",        [0.4, 0.6, 1.0]),
            "ttz":                     (r"$t\bar{t}Z$",    [0.4, 0.8, 0.4]),
            "tt":                      (r"$t\bar{t}$",     [0.8, 0.8, 0.8]),
            "tth":                     (r"$t\bar{t}H$",    [0.4, 0.4, 0.6]),
            "ttw":                     (r"$t\bar{t}W$",    [0.0, 0.4, 0.0]),
            "vv":                      (r"VV",             [0.0, 0.4, 0.8]),
            "rares":                   ("Rare",            [1.0, 0.4, 1.0]),
            "singletop":               ("Single Top",      [1.0, 0.4, 0.0]),
            },
}

def write_table(data, bgs, outname=None):
    from pytable import Table
    tab = Table()
    sumbgs = sum(bgs)
    databg = data/sumbgs
    procs = bgs+[sumbgs,data,databg]
    cnames = [bg.get_attr("label") for bg in bgs] + ["Total bkg","Data", "Data/bkg"]
    tab.set_column_names(["bin"]+cnames)
    sep = "+-"
    precision = 2
    if "tt_isr" in outname and "nisrjets" in outname:
        precision = 5
    binpairs = zip(data.edges[:-1],data.edges[1:])
    tab.set_theme_basic()
    for ibin,binrow in enumerate(binpairs):
        row = ["[{},{}]".format(*binrow)]
        for iproc,proc in enumerate(procs):
            cent = max(proc.counts[ibin],0.)
            err = proc.errors[ibin]
            row.append(("{0:5.%if} {1}{2:%i.%if}" % (precision,precision+3,precision)).format(cent,sep,err))
        tab.add_row(row)
    tab.add_line()

    row = ["total"]
    for iproc,proc in enumerate(procs):
        if iproc == len(procs)-1:
            totbg = E(sum(sumbgs.counts), np.sum(sumbgs.errors**2.)**0.5)
            totdata = E(sum(data.counts))
            ratio = totdata/ totbg
            cent, err = ratio[0], ratio[1]
        else:
            cent = sum(proc.counts)
            err = np.sum(proc.errors**2.)**0.5
        row.append(("{0:5.%if} {1}{2:%i.%if}" % (precision,precision+3,precision)).format(cent,sep,err))
    tab.add_row(row)

    if outname:
        with open(outname,"w") as fhout:
            towrite = "".join(tab.get_table_string(show_row_separators=False,show_alternating=False))
            fhout.write(towrite)
            parts = towrite.split("\n")
            header = parts[:3]
            binparts = parts[3:-4]
            total = parts[-4:-1]
            table_info = { "header":"<br>".join(header), "bins":binparts, "total":"<br>".join(total) }
            return table_info

def get_info(bin_coords, table_info):
    info = { "xedges": [], "yedges": [], "table": table_info }
    for ipair,pair in enumerate(zip(bin_coords[::2],bin_coords[1::2])):
        xlow,xhigh = round(pair[0][0],5), round(pair[1][0],5)
        ylow,yhigh = round(pair[0][1],5), round(pair[1][1],5)
        info["xedges"].append([xlow,xhigh])
        info["yedges"].append([ylow,yhigh])
    return info


def make_plots(outputdir="plots", inputdir="outputs", year="2017", lumi="41.3"):
    bin_json = {}

    os.system("mkdir -p {}/".format(outputdir))

    files = { proc:uproot.open("{}/histos_{}_{}.root".format(inputdir,proc,year)) for proc in (list(set(sum(map(lambda x:x.keys(),bginfo.values()),[])))+["data"]) }
    # files = { proc:uproot.open("{}/histos_{}.root".format(inputdir,proc)) for proc in (list(set(sum(map(lambda x:x.keys(),bginfo.values()),[])))+["data"]) }

    for which in ["htnb1mc","htnb1","os","osloose","br","crw","crz","tt_isr_reweight_check"]:
    # for which in ["os"]:
        # title = "CRW ({})".format(year) if which == "crw" else "NB==1, all HT ({})".format(year)
        title = which.upper()
        for var, xlabel in labels.items():
            # for flav in ["ee","em","mm","in"]:
            # for flav in ["ee","mm","in"]:
            for flav in ["in"]:

                hname = "{}_{}_{}".format(which,var,flav)

                # htnb1mc is htnb1 but with ttbar MC fakes instead of data-driven
                # we're using the original htnb1 histograms though, so fix hname
                if which == "htnb1mc":
                    hname = hname.replace("htnb1mc","htnb1")

                bgs = [Hist1D(files[proc][hname], label=label,color=color) for proc,(label,color) in bginfo[which].items()]
                bgs = sorted(bgs, key=lambda bg: bg.get_integral())
                data = Hist1D(files["data"][hname])
                data.set_attr("label", "Data [{}]".format(int(data.get_integral())))
                if data.get_integral() < 1e-6: continue
                if abs(sum(bgs).get_integral()) < 1e-6: continue

                # bgtt = next(bg for bg in bgs if bg.get_attr("label") == r"$t\bar{t}$")
                # relerrs = np.array([0.13672627, 0.05453066, 0.08452154, 0.17839078, 0.20806332])
                # bgtt._errors = ((bgtt._counts*relerrs)**2. + bgtt._errors**2.)**0.5
                # sbgs = sum(bgs)
                # sbgs._errors *= 0.
                # ratio = data/sbgs

                # map(lambda x:x.rebin(2), bgs+[data])

                # promptbgs = sum(bg for bg in bgs if "Nonprompt" not in bg.get_attr("label"))
                # fpurity = promptbgs.get_integral() / data.get_integral()
                # hpurity = promptbgs / data

                fname = "{}/y{}_{}_{}_{}.png".format(outputdir,year,which,var,flav)
                _, _, bin_coords = plot_stack(bgs=bgs, data=data, title=title, xlabel=xlabel, filename=fname,
                           cms_type = "Preliminary",
                           lumi = lumi,
                           return_bin_coordinates=True,
                           ratio_range=[0.0,2.0],
                           # ratio_range=[0.75,1.25],
                           # do_bkg_syst = True,
                           # ratio = ratio,
                           # ratio_range=[0.0,1.0],
                           # ratio = hpurity,
                           # mpl_ratio_params = {
                           #     "label": "prompt purity: {:.2f}".format(fpurity),
                           #     "color": "C3",
                           #     },
                           )
                # os.system("ic {}".format(fname))
                table_info = write_table(data,bgs,outname=fname.replace(".png",".txt"))
                bin_json[fname.split("/")[-1].split(".")[0]] = get_info(bin_coords,table_info)
                print "Wrote {}".format(fname)

    return bin_json

if __name__ == "__main__":

    lumi = "41.3"
    # inputdir = "outputs2017"
    # outputdir = "plots"

    # for val in [10,12,14,16,20]:
    #     inputdir = "outputs_2017_ptfake{}".format(val)
    #     outputdir = "plots_ptfake{}".format(val)
    #     total_bin_json = {}
    #     total_bin_json.update(make_plots(
    #             outputdir=outputdir,
    #             inputdir=inputdir,
    #             year="2017",
    #             lumi=lumi,
    #             ))

    # inputdir = "outputs_2017_all"
    inputdir = "outputs_2017_isr"
    outputdir = "plots_isr"
    total_bin_json = {}
    total_bin_json.update(make_plots(
            outputdir=outputdir,
            inputdir=inputdir,
            year="2017",
            lumi=lumi,
            ))

    # total_bin_json.update(make_plots(
    #         outputdir=outputdir,
    #         inputdir=inputdir,
    #         year="2016",
    #         lumi="35.9",
    #         ))
    # json.dump(total_bin_json, open("{}/binInfo.json".format(outputdir),"w"))
