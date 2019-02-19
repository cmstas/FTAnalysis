import os
import numpy as np

import uproot

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
from matplottery.utils import Hist1D
from matplottery.plotter import plot_stack

np.set_printoptions(linewidth=150)

# # ttZ has 0 extra partons
# https://github.com/cms-sw/genproductions/blob/f2df23bf13242fd796b3a282593f40044527fc0a/bin/MadGraph5_aMCatNLO/cards/production/13TeV/TTZJets/TTZJetsToLLNuNu_5f_NLO/TTZJetsToLLNuNu_5f_NLO_proc_card.dat
# # ttW has 1 extra parton
# https://github.com/cms-sw/genproductions/blob/5fb3762c8be13dabb89a8580863856201d56f49c/bin/MadGraph5_aMCatNLO/cards/production/2017/13TeV/TTWJets/TTWJetsToLNu_5f_NLO_FXFX/TTWJetsToLNu_5f_NLO_FXFX_proc_card.dat

def do(inputdir,outputdir,which,year):
    # which = "ttdl0jet"
    # which = "ttdl1jet"
    # which = "tt"
    others = ["wjets"]
    procs = ["dy", "ttz", "tth", "ttw", "vv", "rares", "singletop", "tttt"]+others

    # inputdir = "outputs_Nov29/"
    # inputdir = "outputs_19Jan4_v3p21/"
    # inputdir = "outputs_19Jan18_v3p24_ft//"
    # year = 2017
    # year = 2018
    files = { proc:uproot.open("{}/histos_{}_{}.root".format(inputdir,proc,year)) for proc in procs+["data",which] }

    # # add up 2017, 2018 to check
    # # dir=outputs_19Jan4_v3p21/; for proc in wjets tttt singletop ttdl0jet dy ttw tth ttz ttdl1jet vv rares tt data ; do echo hadd -k -f ${dir}/histos_${proc}_1718.root ${dir}/histos_${proc}_{2017,2018}.root ; done
    # year = 1718
    # files = { proc:uproot.open("{}/histos_{}_{}.root".format(inputdir,proc,year)) for proc in procs+["data",which] }

    # print files
    # files = { proc:uproot.open("{}/histos_{}_{}.root".format(inputdir,proc,year)) for proc in (list(set(sum(map(lambda x:x.keys(),bginfo.values()),[])))+["data"]) }
        # hname = "{}_{}_{}".format(region,var,flav)

    # hname = "tt_isr_reweight_check_nisrjets_in"
    hname = "tt_isr_nisrjets_in"
    # hname = "tt_isr_nisrjets_em"
    # hname = "tt_isr_ht_in"
    # hname = "tt_isr_etae_in"
    # hname = "tt_isr_nisrmatch_in"
    # hname = "tt_isr_met_in"
    otherbgs = []
    for newname,tosum,color in [
            ["other",others, [0.4,0.8,0.8]],
            ["rare", ["rares","vv","ttz","tth","ttw","tttt"], [1.0,0.4,1.0]],
            ["dy", ["dy"], [0.4,0.6,1.0]],
            ["singletop", ["singletop"], [1.0,0.4,0.0]],
            ]:
        groupbg = sum([Hist1D(files[proc][hname]) for proc in tosum])
        # if newname == "singletop": groupbg *= 15.
        groupbg.set_attr("label", newname)
        groupbg.set_attr("color",color)
        otherbgs.append(groupbg)
    data = Hist1D(files["data"][hname],label="data")
    mc_isr = Hist1D(files[which][hname],label=which)

    # scale ISR sample to data (well, to data-otherBGs)
    mc_isr *= (data-sum(otherbgs)).get_integral() / mc_isr.get_integral()

    bgs = otherbgs + [mc_isr]
    # for bg in bgs:
    #     print bg.get_attr("label").ljust(15), bg
    # print data

    ibg = sum(bgs).get_integral()
    idata = data.get_integral()

    data_isr = (data-sum(otherbgs))
    hsfs = data_isr/mc_isr / (data_isr.get_integral()/mc_isr.get_integral())
    hsfs.set_attr("label","isrw")
    print hsfs

    # # Test -- if we scale mc_isr by the sfs above, do
    # # we get an equal data/MC in each bin? (we should!)
    # print "before", mc_isr.get_integral()
    # mc_isr._counts = mc_isr._counts * hsfs._counts
    # print "after", mc_isr.get_integral()

    from analysis.utils import pytable
    from analysis.utils.plotting_utils import write_table
    tab = write_table(data, otherbgs+[mc_isr], extra_hists=[hsfs], binedge_fmt="{:.0f}", fix_negative=False)
    # tab.set_theme_fancy()
    tab.set_theme_csv()
    tab.print_table()

    mc_isr.set_attr("color",[0.8, 0.8, 0.8])

    # ratio = data/sum(otherbgs+[mc_isr])
    ratio = Hist1D(hsfs,label=r"$\frac{\mathrm{Data}-\mathrm{non}t\bar{t}}{t\bar{t}}$")
    def ax_ratio_callback(ratio):
        def f(ax):
            for x,y,ye in zip(ratio.get_bin_centers(),ratio.get_counts(),ratio.get_errors()):
                if not (0. < y < 2.): continue
                ax.text(x-0.32,y-0.1,"{:.2f}$\pm${:.2f}".format(y,ye),
                        color="black", ha="center", va="center", fontsize=7.0,
                        wrap=True)
        return f

    pstr = which.replace("jet","")[-1]
    if pstr == "t": pstr = "2"
    fname = "{}/y{}_isrrw_nisrjets{}.pdf".format(outputdir,year,pstr)
    # mc_isr._counts[mc_isr._counts < 0.] = 0.
    plot_stack(bgs=otherbgs+[mc_isr], data=data, title="nisrjets", xlabel="nisrjets", filename=fname,
    # plot_stack(bgs=[data], data=data, title="nisrjets", xlabel="nisrjets", filename=fname,
               cms_type = "Preliminary",
               lumi = {2016:"35.9",2017:"41.5",2018:"58.8"}[year],
               ratio_range=[0.0,3.0],
               ratio=ratio,
               ax_ratio_callback=ax_ratio_callback(ratio),
               # do_bkg_syst=True,
               )
    # os.system("ic {}".format(fname))

    print ratio

    # # ttZ has 0 extra partons
    # https://github.com/cms-sw/genproductions/blob/f2df23bf13242fd796b3a282593f40044527fc0a/bin/MadGraph5_aMCatNLO/cards/production/13TeV/TTZJets/TTZJetsToLLNuNu_5f_NLO/TTZJetsToLLNuNu_5f_NLO_proc_card.dat
    # # ttW has 1 extra parton
    # https://github.com/cms-sw/genproductions/blob/5fb3762c8be13dabb89a8580863856201d56f49c/bin/MadGraph5_aMCatNLO/cards/production/2017/13TeV/TTWJets/TTWJetsToLNu_5f_NLO_FXFX/TTWJetsToLNu_5f_NLO_FXFX_proc_card.dat
    if "0jet" in which:
        vec = hsfs.get_counts().tolist()[:4]+[1., 1., 1., 1., 1.]
    else:
        vec = [1.,1.,1.,1.]+hsfs.get_counts().tolist()[:5]
    print """
    float isrWeight(int nisrmatch, int sample) {{
        // see instructions in analysis/misc/isr_weights/calc.py and script in analysis/crplots/calc_isr.py
        float scale = 1.;
        if (sample == 1) {{ // ttW -- 0 extra partons
            scale = 1.;
            if (nisrmatch == 0) {{ return scale*{:.3f}; }}
            if (nisrmatch == 1) {{ return scale*{:.3f}; }}
            if (nisrmatch == 2) {{ return scale*{:.3f}; }}
            if (nisrmatch >= 3) {{ return scale*{:.3f}; }}
        }} else if (sample == 2) {{ // ttZ -- 1 extra parton
            scale = 1.;
            if (nisrmatch == 0) {{ return scale*{:.3f}; }}
            if (nisrmatch == 1) {{ return scale*{:.3f}; }}
            if (nisrmatch == 2) {{ return scale*{:.3f}; }}
            if (nisrmatch == 3) {{ return scale*{:.3f}; }}
            if (nisrmatch >= 4) {{ return scale*{:.3f}; }}
        }}
        return scale;
    }}
    """.format(*vec)

if __name__ == "__main__":

    # for year in [2017,2018]:
    #     for which in ["tt","ttdl0jet","ttdl1jet"]:
    # # for year in [2017]:
    # #     for which in ["ttdl0jet"]:
    # # for year in [2016]:
    # #     for which in ["tt"]:
    #         do(
    #                 # inputdir="outputs_19Jan18_v3p24_ft",
    #                 # outputdir="plots_ft_3p24_19Jan18",
    #                 inputdir="outputs_19Feb16_v3p26_ft",
    #                 outputdir="plots_ft_3p26_19Feb16",
    #                 which=which,
    #                 year=year,
    #                 )

    # do( inputdir="outputs_19Feb16_v3p26_ft", outputdir="plots_ft_3p26_19Feb16", which="tt", year=2016,)
    do( inputdir="outputs_19Feb16_v3p26_ft", outputdir="plots_ft_3p26_19Feb16", which="ttmg", year=2016,)
