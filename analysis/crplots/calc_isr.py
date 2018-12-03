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

which = "ttdl0jet"
# which = "ttdl1jet"
# which = "tt"
procs = ["dy", "ttz", "tth", "ttw", "vv", "rares", "singletop"]

inputdir = "outputs_isr2017/"
year = 2017
files = { proc:uproot.open("{}/histos_{}_{}.root".format(inputdir,proc,year)) for proc in procs+["data",which] }
# print files
# files = { proc:uproot.open("{}/histos_{}_{}.root".format(inputdir,proc,year)) for proc in (list(set(sum(map(lambda x:x.keys(),bginfo.values()),[])))+["data"]) }
    # hname = "{}_{}_{}".format(region,var,flav)

hname = "tt_isr_nisrjets_in"
# hname = "tt_isr_met_in"
otherbgs = []
for newname,tosum in [
        ["rare", ["rares","vv","ttz","tth","ttw"]],
        ["dy", ["dy"]],
        ["singletop", ["singletop"]],
        ]:
    groupbg = sum([Hist1D(files[proc][hname]) for proc in tosum])
    groupbg.set_attr("label", newname)
    otherbgs.append(groupbg)
data = Hist1D(files["data"][hname],label="data")
mc_isr = Hist1D(files[which][hname],label=which)

# FIXME
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

otherbgs[0].set_attr("color", [1.0, 0.4, 1.0])
otherbgs[1].set_attr("color", [0.4, 0.6, 1.0])
otherbgs[2].set_attr("color", [1.0, 0.4, 0.0])
mc_isr.set_attr("color",[0.8, 0.8, 0.8])

ratio = data/sum(otherbgs+[mc_isr])
def ax_ratio_callback(ratio):
    def f(ax):
        for x,y in zip(ratio.get_bin_centers(),ratio.get_counts()):
            if not (0. < y < 2.): continue
            ax.text(x-0.3,y-0.1,"{:.2f}".format(y),
                    color="black", ha="center", va="center", fontsize=7.0,
                    wrap=True)
    return f

fname = "test.pdf"
# mc_isr._counts[mc_isr._counts < 0.] = 0.
plot_stack(bgs=otherbgs+[mc_isr], data=data, title="nisrjets", xlabel="nisrjets", filename=fname,
# plot_stack(bgs=[data], data=data, title="nisrjets", xlabel="nisrjets", filename=fname,
           cms_type = "Preliminary",
           lumi = "41.5",
           ratio_range=[0.0,2.0],
           ax_ratio_callback=ax_ratio_callback(ratio),
           )
os.system("ic {}".format(fname))

