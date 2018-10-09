import numpy as np

from matplottery.utils import Hist1D
import uproot

np.set_printoptions(linewidth=150)

# # ttZ has 0 extra partons
# https://github.com/cms-sw/genproductions/blob/f2df23bf13242fd796b3a282593f40044527fc0a/bin/MadGraph5_aMCatNLO/cards/production/13TeV/TTZJets/TTZJetsToLLNuNu_5f_NLO/TTZJetsToLLNuNu_5f_NLO_proc_card.dat
# # ttW has 1 extra parton
# https://github.com/cms-sw/genproductions/blob/5fb3762c8be13dabb89a8580863856201d56f49c/bin/MadGraph5_aMCatNLO/cards/production/2017/13TeV/TTWJets/TTWJetsToLNu_5f_NLO_FXFX/TTWJetsToLNu_5f_NLO_FXFX_proc_card.dat

# which = "ttdl0jet"
which = "ttdl1jet"
# which = "tt"
procs = ["dy", "ttz", "tth", "ttw", "vv", "rares", "singletop"]

inputdir = "outputs_temp"
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
tab = write_table(data, otherbgs+[mc_isr], extra_hists=[hsfs], binedge_fmt="{:.0f}")
# tab.set_theme_fancy()
tab.set_theme_csv()
tab.print_table()

