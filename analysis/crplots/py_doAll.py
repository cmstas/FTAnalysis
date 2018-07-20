import os
import pyrun
import ROOT as r
r.gROOT.SetBatch()

r.gROOT.ProcessLine(".L ../misc/class_files/v8.02/SS.cc+")
r.gROOT.ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc+")
r.gROOT.ProcessLine(".L ScanChain.C+")


years_to_consider = [
        # 2016,
        # 2017,
        2018,
        ]
# outputdir = "outputs_20172018_newWP_newmet"
# outputdir = "outputs_2017_oldmet"
# outputdir = "outputs_2017_newmet"


basedirs ={
        2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/{}/output/".format("v1.00_80x_baseline_full_v5"),
        # 2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/{}/output/".format("v1.04_v1"),  # old multiiso WP
        2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/{}/output/".format("v1.06_v2"),  # new multiiso WP, old,new met
        # 2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/{}/output/".format("2018_v1.03"),
        2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/{}/output/".format("2018_v1.05"),  # new multiiso WP, old,new met
        }

# outputdir = "outputs_20172018_newWP_oldmet"
# options = {
#         2016: "useIsoTriggers2016 useInclusiveSFs Data2016 quiet ",
#         2017: "useInclusiveSFs Data2017 quiet ",
#         2018: "useInclusiveSFs Data2018 quiet ",
#         }

outputdir = "outputs_20172018_newWP_newmet"
options = {
        2016: "useIsoTriggers2016 useInclusiveSFs Data2016 useNewMET quiet ",
        2017: "useInclusiveSFs Data2017 useNewMET quiet ",
        2018: "useInclusiveSFs Data2018 useNewMET quiet ",
        }

def make_objs(fpatts=[],options="",treename="t"):
    if type(fpatts) == str: fpatts = [fpatts]
    ch = r.TChain(treename)
    for fpatt in fpatts:
        ch.Add(fpatt)
    return {"ch": ch, "options": options}

chs = {
        2017: {
            "fakes": make_objs(basedirs[2017]+"Data*.root", options=options[2017]+" doFakes"),
            "flips": make_objs(basedirs[2017]+"Data*.root", options=options[2017]+" doFlips"),
            "data": make_objs(basedirs[2017]+"Data*.root", options=options[2017]),
            "ttw": make_objs(basedirs[2017]+"TTWnlo.root", options=options[2017]),
            "ttz": make_objs(basedirs[2017]+"TTZnlo.root", options=options[2017]),
            "tth": make_objs(basedirs[2017]+"TTHtoNonBB.root", options=options[2017]),
            "dy": make_objs(basedirs[2017]+"DY_high.root", options=options[2017]),
            "wjets": make_objs(basedirs[2017]+"WJets*.root", options=options[2017]),
            "tt": make_objs(basedirs[2017]+"TTBAR*.root", options=options[2017]),
            "ttfake": make_objs(basedirs[2017]+"TTBAR*.root", options=options[2017]+ "doTruthFake"),
            "vv": make_objs([
                basedirs[2017]+"WZ.root",
                # basedirs[2017]+"WW.root",
                basedirs[2017]+"WWDPS.root",
                basedirs[2017]+"ZZ.root",
                ],options=options[2017]),
            "rares": make_objs([
                basedirs[2017]+"WZG.root",
                basedirs[2017]+"WZZ.root",
                basedirs[2017]+"ZZZ.root",
                basedirs[2017]+"WWZ.root",
                basedirs[2017]+"WWW.root",
                basedirs[2017]+"TZQ.root",
                basedirs[2017]+"TTZZ.root",
                basedirs[2017]+"TTWZ.root",
                basedirs[2017]+"TTWW.root",
                basedirs[2017]+"TTWH.root",
                basedirs[2017]+"TTHH.root",
                basedirs[2017]+"TTTW.root",
                basedirs[2017]+"TTTJ.root",
                ],options=options[2017]),
            "singletop": make_objs([
                basedirs[2017]+"STtop.root",
                basedirs[2017]+"STantitop.root",
                ],options=options[2017]),
            },
        2018: {
            "fakes": make_objs(basedirs[2018]+"Data*.root", options=options[2018]+" doFakes"),
            "flips": make_objs(basedirs[2018]+"Data*.root", options=options[2018]+" doFlips"),
            "data": make_objs(basedirs[2018]+"Data*.root", options=options[2018]),
            "ttw": make_objs(options=options[2018]),
            "ttz": make_objs(options=options[2018]),
            "tth": make_objs(options=options[2018]),
            "dy": make_objs(basedirs[2018]+"DY_high.root", options=options[2018]),
            "wjets": make_objs(options=options[2018]),
            "tt": make_objs(basedirs[2018]+"TTDilep*.root", options=options[2018]),
            "ttfake": make_objs(basedirs[2018]+"TTDilep*.root", options=options[2018]+ "doTruthFake"),
            "vv": make_objs(options=options[2018]),
            "rares": make_objs(options=options[2018]),
            "singletop": make_objs(options=options[2018]),
            }
        }

# Change chain titles to proc_year so that we output the right root file name
for year in chs:
    [obj["ch"].SetTitle("{}_{}".format(proc,year)) for proc,obj in chs[year].items()]


def run_chain((index,info)):
    ch, options, outputdir = info
    ret = r.ScanChain(ch, options, outputdir)
    return index, ret

to_run = []
for year in years_to_consider:
    for proc,obj in chs[year].items():
        to_run.append([obj["ch"], obj["options"], outputdir])

# for tr in to_run:
#     print tr

os.system("mkdir -p {}".format(outputdir))

runner = pyrun.Runner(nproc=10, func=run_chain)
runner.add_args(to_run)
runner.run()
