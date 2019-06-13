from tqdm import tqdm
import json
import os
import numpy as np
import itertools
import uproot

from analysis.utils.plotting_utils import write_table

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery.plotter import plot_stack
from matplottery.utils import Hist1D, MET_LATEX

np.set_printoptions(linewidth=200)

labels = {

    "ht": "$H_{T}$",
    "met": MET_LATEX,
    "mll": "$m_{ll}$",
    "m3l": "$m_{lll}$",
    "njets": "Njets",
    "nbtags": "Nbtags",
    "type": "hyp type (mm, em, ee)",
    "nleps": "nleps",
    "pt1": "$p_T$(lep1)",
    "pt2": "$p_T$(lep2)",
    "pt3": "$p_T$(lep3)",
    "pte": "$p_T$(e)",
    "ptm": "$p_T$(m)",
    "mtmin": "$m_{T}^\\mathrm{min}$",
    "zmll": "$m_{ll} (Z-cand)$",
    "nvtx": "# good vertices",
    "eta1": r"$\eta$(lep1)",
    "eta2": r"$\eta$(lep2)",
    "phie": r"$\phi$(e)",
    "phim": r"$\phi(\mu)$",
    "etae": r"$\eta$(e)",
    "etam": r"$\eta(\mu)$",

    # "ptj1": "$p_T$ - jet 1",
    # "ptj2": "$p_T$ - jet 2",
    # "ptj3": "$p_T$ - jet 3",
    # "ptj4": "$p_T$ - jet 4",

    "dphil1l2": r"$\Delta\phi(l_1,l_2)$",
    "dphil1met": r"$\Delta\phi(l_1,${}$)$".format(MET_LATEX),
    "dphil2met": r"$\Delta\phi(l_2,${}$)$".format(MET_LATEX),
    "q1": "charge - lep 1",
    "htb": r"$H_{T}$(b-jets)",
    "nlb40": r"N-loose b-tags, $p_{T}>40$",
    "ntb40": r"N-tight b-tags, $p_{T}>40$",
    "detal1l2": r"$\Delta\eta(l_1,l_2)$",
    "maxmjoverpt": r"max($m_j/p_T$)",
    "ml1j1": r"m$(l_1,j_2)$",
    "ptj1": "$p_T$ - jet 1",
    "ptj6": "$p_T$ - jet 6",
    "ptj7": "$p_T$ - jet 7",
    "ptj8": "$p_T$ - jet 8",
    "eventbdt": "raw BDT discriminant",

    "zbb_mbbmin": "ZBB mbbmin",
    "zbb_disc_both": "ZBB disc both",
    "zbb_disc_ttbb": "ZBB disc ttbb",
    "zbb_disc_ttjets": "ZBB disc ttjets",

    # "nbnj": r"bin%5=Nj, bin//5=Nb",
    # "nisrjets": "Njets (ISR/FSR)",
    # "nisrmatch": "Njets (ISR/FSR - Truth Matched)",
    # "bdisc1": "Disc leading bjet",
    # "etaelnt": r"loose leg $\eta$(e)",
    # "etamlnt": r"loose leg $\eta(\mu)$",


    # "btagid": r"$20 N_\mathrm{b} + 5 N_\mathrm{charm} + N_\mathrm{light}$",
    # "avgcdisc": "average DeepCSV c disc",
    # "nforwardjets20": "number of jets with $2.4<\eta<3.0$, $p_{T}>20$",
    # "ntrijets": "number of trijets",
    # "trijet_meandisc": "mean trijet disc",
    # "trijet_numhigh": "number of trijet with disc>0.3",
    # "trijet_frachigh": "fraction of trijet with disc>0.3",
    # "trijet_leadingdisc": "leading trijet disc",
    # "trijet_subleadingdisc": "subleading trijet disc",

    # "rawmet": "Raw "+MET_LATEX,
    # "ptrele": "$p_T$rel - e",
    # "ptrelm": r"$p_T$rel - $\mu$",
    # "ptratioe": "$p_T$ratio - e",
    # "ptratiom": r"$p_T$ratio - $\mu$",
    # "ptrelfaile": "$p_T$rel - e, fail $p_T$ratio",
    # "ptrelfailm": r"$p_T$rel - $\mu$, fail $p_T$ratio",
    # "miniisoe": "mini-iso - e",
    # "miniisom": r"mini-iso - $\mu$",
    # "nmiss1": "exp miss inner hits - lep 1",
    # "nmiss2": "exp miss inner hits - lep 2",

    # "ptrel1": "$p_T$rel - lep 1",
    # "ptrel2": "$p_T$rel - lep 2",
    # "ptratio1": "$p_T$ratio - lep 1",
    # "ptratio2": "$p_T$ratio - lep 2",
    # "ptrelfail1": "$p_T$rel - lep 1, fail $p_T$ratio",
    # "ptrelfail2": "$p_T$rel - lep 2, fail $p_T$ratio",
    # "miniiso1": "mini-iso - lep 1",
    # "miniiso2": "mini-iso - lep 2",

    # "ptrellnt": "$p_T$rel - loose lep",
    # "ptratiolnt": "$p_T$ratio - loose lep",
    # "ptrelfaillnt": "$p_T$rel - loose lep, failing $p_T$ratio",
    # "miniisolnt": "mini-iso - loose lep",

    # "htb": "$H_{T} B-Jets$",
    # "nlb40": "N-loose Btags, $p_T$>40",
    # "ntb40": "N-tight Btags, $p_T$>40",
    # "type3l": "3l type (mmm, mme, mee, eee)",



}

d_label_colors = {
            "dy":                      (r"DY+jets",        [0.4, 0.6, 1.0]),
            "fakes":                   (r"Nonprompt lep.", [0.85, 0.85, 0.85]),
            "flips":                   (r"Charge misid.",  [0.4, 0.4, 0.4]),
            "rares":                   ("Rare",            [1.0, 0.4, 1.0]),
            "singletop":               ("Single Top",      [1.0, 0.4, 0.0]),
            "tt":                      (r"$t\bar{t}$",     [0.8, 0.8, 0.8]),
            "ttfake":                   (r"$t\bar{t}$ Nonprompt", [0.85, 0.85, 0.85]),
            "wjets":                   (r"W+jets",         [113./255,151./255,44./255]),
            "tth":                     (r"$t\bar{t}H$",    [0.4, 0.4, 0.6]),
            "ttw":                     (r"$t\bar{t}W$",    [0.0, 0.4, 0.0]),
            "ttz":                     (r"$t\bar{t}Z$",    [0.4, 0.8, 0.4]),
            "zbb":                     (r"$t\bar{t}Z$(bb)",    [0.3, 0.6, 0.4]),
            "wz":                      (r"WZ" ,             [1.0,0.8,0.0]),
            "vv":                      (r"VV",             [0.0, 0.4, 0.8]),
            "vvnowz":                   (r"VV",             [0.0, 0.4, 0.8]),
            "tttt":                      (r"$t\bar{t}t\bar{t}$",             [0.786,0.147,0.022]),
            "wgamma":                   (r"W+$\gamma$",             "#9D7ABF"),
            "zgamma":                   (r"Z+$\gamma$",             "#8154AD"),
            "othergamma":                   (r"Other X+$\gamma$",             "#54267F"),
            "raresnoxg":                   ("Rare",            [1.0, 0.4, 1.0]),
        }

bginfo = {
        "hhos": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        "hhosloose": { k:d_label_colors[k] for k in [ "flips", "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        "hhtl": { k:d_label_colors[k] for k in [ "dy", "tt", "vv", "wjets", "rares", "singletop", ] },
        "osnbrw": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop","tttt" ] },
        "os_noht": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        "osloose": { k:d_label_colors[k] for k in [ "flips", "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        "nj2": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "nj2mc": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "ttfake", "rares", ] },
        "htnb1": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "htnb1mu": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "htnb1mc": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "tt", "wjets", "rares", "singletop", ] },
        "htnb1mcmu": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "ttfake", "rares", "singletop", ] },
        "crz": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "crw": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "br": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "sshh": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "fakes", "rares", ] },
        "sshhmc": { k:d_label_colors[k] for k in [ "flips", "ttz", "tth", "ttw", "vv", "ttfake", "rares", ] },
        "tt_isr": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        "tt_isr_reweight_check": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop", ] },
        "tl": { k:d_label_colors[k] for k in [ "dy", "tt", "vv", "wjets", "rares", "singletop", ] },
        "os": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop","tttt" ] },
        "zbb": { k:d_label_colors[k] for k in [ "dy", "ttz", "zbb", "tt", "tth", "ttw", "vv", "rares", "singletop","tttt" ] },
        # "tl": { k:d_label_colors[k] for k in [ "dy", "tt", "vv", "wjets", "rares", "singletop", ] },
        # "os": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop" ] },
        "tlmet": { k:d_label_colors[k] for k in [ "dy", "tt", "vv", "wjets", "rares", "singletop", ] },
        "osmet": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop" ] },
        "osmet100": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop" ] },
        "osmet100mtmin200": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop" ] },
        "tlnomet": { k:d_label_colors[k] for k in [ "dy", "tt", "vv", "wjets", "rares", "singletop", ] },
        "osnomet": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop" ] },
        "osnomethighmt": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop" ] },
        "osnometmtlt120": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop" ] },
        "osnometmtgt120": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop" ] },
        "mlonz": { k:d_label_colors[k] for k in [ "dy", "ttz", "tth", "ttw", "vvnowz","wz", "rares", "fakes" ] },
        "oshtnb3": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "ttw", "vv", "rares", "singletop","tttt" ] },
        "lowmetlowht": { k:d_label_colors[k] for k in [ "fakes", "flips", "vv", "ttz","tth","ttw", "rares", ] }, # NOTE data-driven fakes
        "lowmetallht": { k:d_label_colors[k] for k in [ "fakes", "flips", "vv", "ttz","tth","ttw", "rares", ] }, # NOTE data-driven fakes
        "xgcr": { k:d_label_colors[k] for k in [ "dy", "ttz", "tt", "tth", "wjets", "ttw", "vv","zgamma","wgamma","othergamma","raresnoxg", "singletop" ] },
        }

# yuck, gotta make these global for multiprocessing since uproot file objects can't be pickled
files, other_files = {}, {}

def worker(info):
    global files, other_files

    outputdir, year, lumi, region, flav, var = info
    title = region.upper()
    xlabel = labels[var]
    hname = "{}_{}_{}".format(region,var,flav)

    # htnb1mc is htnb1 but with ttbar MC fakes instead of data-driven
    # we're using the original htnb1 histograms though, so fix hname
    if "htnb1mc" in region:
        hname = hname.replace("htnb1mc","htnb1")
    if "sshhmc" in region:
        hname = hname.replace("sshhmc","sshh")
    if "nj2mc" in region:
        hname = hname.replace("nj2mc","nj2")

    if other_files:
        bgs = [
                sum([Hist1D(files[proc][hname],label=label,color=color)] + [Hist1D(other_files[y][proc][hname],label=label,color=color) for y in other_files.keys()])
                for proc,(label,color) in sorted(bginfo[region].items())
                ]
        data = sum([Hist1D(files["data"][hname])] + [Hist1D(other_files[y]["data"][hname]) for y in other_files.keys()])
    else:
        bgs = [Hist1D(files[proc][hname], label=label,color=color) for proc,(label,color) in sorted(bginfo[region].items())]
        data = Hist1D(files["data"][hname])
    data.set_attr("label", "Data [{}]".format(int(data.get_integral())))
    if data.get_integral() < 1e-6: return
    if abs(sum(bgs).get_integral()) < 1e-6: return


    do_bkg_syst = True
    try:
        hname_up = hname.replace("{}_".format(var), "{}_btagheavyup_".format(var))
        bgs_btagheavyup = [ sum([Hist1D(files[proc][hname_up],label=label,color=color)] + [Hist1D(other_files[y][proc][hname_up],label=label,color=color) for y in other_files.keys()]) for proc,(label,color) in sorted(bginfo[region].items()) ]
        hname_up = hname.replace("{}_".format(var), "{}_btaglightup_".format(var))
        bgs_btaglightup = [ sum([Hist1D(files[proc][hname_up],label=label,color=color)] + [Hist1D(other_files[y][proc][hname_up],label=label,color=color) for y in other_files.keys()]) for proc,(label,color) in sorted(bginfo[region].items()) ]
        hname_up = hname.replace("{}_".format(var), "{}_jecup_".format(var))
        bgs_jecup = [ sum([Hist1D(files[proc][hname_up],label=label,color=color)] + [Hist1D(other_files[y][proc][hname_up],label=label,color=color) for y in other_files.keys()]) for proc,(label,color) in sorted(bginfo[region].items()) ]
        hname_up = hname.replace("{}_".format(var), "{}_jerup_".format(var))
        bgs_jerup = [ sum([Hist1D(files[proc][hname_up],label=label,color=color)] + [Hist1D(other_files[y][proc][hname_up],label=label,color=color) for y in other_files.keys()]) for proc,(label,color) in sorted(bginfo[region].items()) ]
        hname_up = hname.replace("{}_".format(var), "{}_scaledn_".format(var))
        bgs_scaleup = [ sum([Hist1D(files[proc][hname_up],label=label,color=color)] + [Hist1D(other_files[y][proc][hname_up],label=label,color=color) for y in other_files.keys()]) for proc,(label,color) in sorted(bginfo[region].items()) ]
        hname_up = hname.replace("{}_".format(var), "{}_bbup_".format(var))
        bgs_bbup = [ sum([Hist1D(files[proc][hname_up],label=label,color=color)] + [Hist1D(other_files[y][proc][hname_up],label=label,color=color) for y in other_files.keys()]) for proc,(label,color) in sorted(bginfo[region].items()) ]
        def relative_variation(hcent, hvar):
            if hvar.get_integral() > 1.0e-5:
                tmp = (hvar.counts/hcent.counts) * hcent.get_integral()/hvar.get_integral()
                # tmp = (hvar.counts/hcent.counts)
                tmp[np.isnan(tmp)] = 1.
                tmp[tmp < 1.] = tmp[tmp<1.]**-1.0
                tmp[np.isnan(tmp)] = 1.
                tmp -= 1.
                return tmp
            return np.zeros(len(hvar.counts))
        bgs_btagheavyup = map(relative_variation, bgs,bgs_btagheavyup)
        bgs_btaglightup = map(relative_variation, bgs,bgs_btaglightup)
        bgs_jecup = map(relative_variation, bgs,bgs_jecup)
        bgs_jerup = map(relative_variation, bgs,bgs_jerup)
        bgs_scaleup = map(relative_variation, bgs,bgs_scaleup)
        bgs_bbup = map(relative_variation, bgs,bgs_bbup)
        def augment_errors(hcent, *relerrs):
            hcent._errors = hcent.counts*((hcent.errors/hcent.counts)**2. + np.sum(np.array(relerrs)**2.,axis=0))**0.5
            hcent._errors[np.isnan(hcent._errors)] = hcent._counts[np.isnan(hcent._errors)]
        map(augment_errors, bgs, bgs_btagheavyup,bgs_btaglightup,bgs_jecup,bgs_jerup,bgs_scaleup,bgs_bbup)
        do_bkg_syst = True
    except KeyError:
        # didn't find btagup/btagdown
        pass

    bgs = sorted(bgs, key=lambda bg: bg.get_integral())
    sf = data.get_integral()/sum(bgs).get_integral()
    bgs = [bg*sf for bg in bgs]
    # bgs = [bg*1 for bg in bgs]
    title += " data/MC={:.2f}".format(sf)
    if other_files:
        fname = "{}/run2_{}_{}_{}.pdf".format(outputdir,region,var,flav)
    else:
        fname = "{}/y{}_{}_{}_{}.pdf".format(outputdir,year,region,var,flav)

    plot_stack(bgs=bgs, data=data, title=title, xlabel=xlabel, filename=fname,
               cms_type = "Preliminary",
               # do_log=True,
               do_bkg_syst=do_bkg_syst,
               lumi = lumi,
               ratio_range=[0.0,2.0],
               mpl_title_params=dict(fontsize=(8 if len(str(lumi))>=5 else 9)),
               # ratio_range=[0.5,1.5],
               )

    fname_log = fname.replace(".pdf","_log.pdf").replace(".png","_log.png")
    plot_stack(bgs=bgs, data=data, title=title, xlabel=xlabel, filename=fname_log,
               cms_type = "Preliminary",
               do_log=True,
               do_bkg_syst=do_bkg_syst,
               lumi = lumi,
               ratio_range=[0.0,2.0],
               mpl_title_params=dict(fontsize=(8 if len(str(lumi))>=5 else 9)),
               # ratio_range=[0.5,1.5],
               )

    # os.system("ic {}".format(fname))
    # table_info = write_table(data,bgs,outname=fname.replace(".pdf",".txt"))
    write_table(data,bgs,outname=fname.replace(".pdf",".txt"))
    return fname

def make_plots(outputdir="plots", inputdir="outputs", year=2017, lumi="41.5", other_years=[], regions=[], flavs=["ee","em","mm","in"]):
    global files, other_files

    os.system("mkdir -p {}/".format(outputdir))

    files = { proc:uproot.open("{}/histos_{}_{}.root".format(inputdir,proc,year)) for proc in (list(set(sum(map(lambda x:x.keys(),bginfo.values()),[])))+["data"]) }
    other_files = {}
    for y in other_years:
        other_files[y] = { proc:uproot.open("{}/histos_{}_{}.root".format(inputdir,proc,y)) for proc in (list(set(sum(map(lambda x:x.keys(),bginfo.values()),[])))+["data"]) }


    # for region in ["htnb1mc","htnb1","os","osloose","br","crw","crz","tt_isr_reweight_check"]:
    # regions = ["htnb1mc","htnb1","htnb1mcmu","htnb1mu","os","os_noht","osloose","br","crw","crz"]
    regions = regions or ["htnb1mc","htnb1","os","sshh","sshhmc","osloose","br","crw","crz"]
    flavs = flavs or ["ee","em","mm","in"]
    varss = labels.keys()
    infos = [[outputdir,year,lumi]+list(x) for x in itertools.product(regions,flavs,varss)]

    os.nice(10)
    from multiprocessing import Pool as ThreadPool
    pool = ThreadPool(25)
    for res in pool.imap_unordered(worker,infos):
        if res:
            print "Wrote {}".format(res)


if __name__ == "__main__":

    # make_plots(
    #         outputdir="plots_20172018_nj2",
    #         inputdir="outputs_temp",
    #         year=2018,
    #         lumi="77.1", # 2017+2018 --> 41.53+35.53 = 77.06
    #         other_years = [2017],
    #         regions = ["nj2","nj2mc"],
    #         )

    # make_plots(
    #         outputdir="plots_temp",
    #         inputdir="outputs_temp",
    #         regions = ["htnb1","htnb1mu"],
    #         year=2018,
    #         lumi="112.9", # 2016+2017+2018 --> 35.87+41.53+35.53 = 112.9
    #         other_years = [2016,2017],
    #         )

    # make_plots(
    #         outputdir="plots_ss2017",
    #         inputdir="outputs_ss20172018",
    #         regions = ["hhos","hhosloose","hhtl"],
    #         flavs = ["ee","mm","in"],
    #         year=2017,
    #         lumi="41.5",
    #         )

    # make_plots(
    #         outputdir="plots_ss2018",
    #         inputdir="outputs_ss20172018",
    #         regions = ["hhos","hhosloose","hhtl"],
    #         flavs = ["ee","mm","in"],
    #         year=2018,
    #         lumi="51.0",
    #         )

    # make_plots(
    #         outputdir="plots_2017_nmiss0",
    #         inputdir="outputs_2017_nmiss0",
    #         year=2017, lumi="41.5",
    #         )
    # make_plots(
    #         outputdir="plots_2017_nmiss1",
    #         inputdir="outputs_2017_nmiss1",
    #         year=2017, lumi="41.5",
    #         )

    # inputdir = "outputs_Nov29"
    # inputdir = "outputs_Dec10"
    # inputdir = "outputs_Dec19_v6"
    # inputdir = "outputs_Dec19_v32"
    # inputdir = "outputs_Dec17toptag"
    # regions = ["osloose","os","tl","tt_isr"]
    # regions = ["osloose","os","tl","osnbrw"]
    # regions = ["osloose","os","tl", "osnbrw"] #,"osnbrw"]
    # regions = ["os","osnbrw"]
    # regions = ["os","osloose", "tl", "htnb1", "htnb1mc"]
    # regions = ["osnomet","tlnomet","osmet","tlmet","mlonz"] #,"mlonzhigh","mlonz0j"]

    # # 2018 alone
    # make_plots(
    #         outputdir="plots_zbb_2018_19Mar31",
    #         inputdir="outputs_19Mar31_v3p28_2018_zbb",
    #         regions = ["zbb"], flavs = ["in"],
    #         year=2018,
    #         lumi="59.7",
    #         )
    # sys.exit()

    ## SAME SIGN NOTE
    regions = [
            "osmet",
            "osnomet",
            "tlmet",
            "tlnomet",
            "mlonz",
            "lowmetallht",
            "lowmetlowht",
            ]
    flavs = ["in"]
    # flavs = ["ee","em","mm","in"]
    inputdir = "outputs_19Mar9_v3p28_ss/"
    outputdir = "plots_ss_3p28_19Mar9"

    # ## FOUR TOP NOTE
    # regions = [
    #         "os",
    #         "tl",
    #         "htnb1",
    #         "htnb1mc",
    #         ]
    # flavs = ["in"]
    # # inputdir = "outputs_19Jan18_v3p24_ft"
    # # outputdir = "plots_ft_3p24_19Jan18"
    # # inputdir = "outputs_19Mar4_v3p27_ft/"
    # # outputdir = "plots_ft_3p27_19Mar4"
    # inputdir = "outputs_19Mar4_v3p28_ft/"
    # outputdir = "plots_ft_3p28_19Mar4"

    # 2016 alone
    # outputdir = "plots_ft_jecv6_2017"
    # outputdir = "plots_ft_jecv32_2017"
    # outputdir = "plots_ft_2017_toptag"
    make_plots(
            outputdir=outputdir,
            inputdir=inputdir,
            regions = regions, flavs = flavs,
            year=2016,
            lumi="35.9",
            )

    # 2017 alone
    # outputdir = "plots_ft_jecv6_2017"
    # outputdir = "plots_ft_jecv32_2017"
    # outputdir = "plots_ft_2017_toptag"
    make_plots(
            outputdir=outputdir,
            inputdir=inputdir,
            regions = regions, flavs = flavs,
            year=2017,
            lumi="41.5",
            )

    # 2018 alone
    # outputdir = "plots_ft_jecv6_2018"
    # outputdir = "plots_ft_jecv32_2018"
    # outputdir = "plots_ft_2018_toptag"
    make_plots(
            outputdir=outputdir,
            inputdir=inputdir,
            regions = regions, flavs = flavs,
            year=2018,
            lumi="59.7",
            )

    # # 2016 + 2018 + 2017
    # outputdir = "plots_ft_run2"
    # # outputdir = "plots_ft_run2_toptag"
    make_plots(
            outputdir=outputdir,
            inputdir=inputdir,
            regions = regions, flavs = flavs,
            year=2018,
            lumi="137.2",
            other_years = [2016,2017],
            )

    # os.system("niceplots plots_2017 plots_crs2017_Sep3")
    # os.system("niceplots plots_2018 plots_crs2018_Sep3")
    # os.system("niceplots plots_20172018 plots_crs20172018_Sep3")
    # os.system("niceplots plots_run2 plots_crsrun2_Sep3")
