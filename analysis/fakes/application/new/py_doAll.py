import os
import analysis.utils.pyrun as pyrun
import ROOT as r
r.gROOT.SetBatch()

r.gROOT.ProcessLine(".L ../../../misc/class_files/v8.02/SS.cc+")
r.gROOT.ProcessLine(".L ScanChain.C+")


years_to_consider = [ # FIXME
        2016,
        2017,
        2018,
        ]
# procs_to_consider = []
procs_to_consider = [ # FIXME
        # "ttdl0jet",
        # "ttdl1jet",
        # "fakes",
        # "flips",
        # "data",
        ]

# years_to_consider = [2016]
# procs_to_consider = ["wz","vvnowz"]

basedirs ={

        2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2016/",
        2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2017/",
        2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2018/",

        }

outputdir = "outputs"
# plotdir = "plots_SS_19Jan25"
plotdir = "plots_FT_19Jan25"

options = {

        # 2016: " doSS Data2016 quiet ",
        # 2017: " doSS Data2017 quiet ",
        # 2018: " doSS Data2018 quiet ",

        # 2016: " doSS Data2016 ",
        # 2017: " doSS Data2017 ",
        # 2018: " doSS Data2018 ",

        2016: " Data2016 ",
        2017: " Data2017 ",
        2018: " Data2018 ",

        }

def make_objs(fpatts=[],options="",treename="t"):
    if type(fpatts) == str: fpatts = [fpatts]
    ch = r.TChain(treename)
    for fpatt in fpatts:
        ch.Add(fpatt)
    return {"ch": ch, "options": options}

chs = {
        2016: {
            "tt": make_objs(basedirs[2016]+"TTBAR*.root", options=options[2016]),
            "wjets": make_objs(basedirs[2016]+"WJets_HT*.root", options=options[2016]),
            },
        2017: {
            "tt": make_objs(basedirs[2017]+"TTSL.root", options=options[2017]),
            "wjets": make_objs(basedirs[2017]+"WJets_HT*.root", options=options[2017]),
            },
        2018: {
            "tt": make_objs(basedirs[2018]+"TTSL.root", options=options[2018]),
            "wjets": make_objs(basedirs[2018]+"WJets_HT*.root", options=options[2018]),
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
        if procs_to_consider and (proc not in procs_to_consider): continue
        to_run.append([obj["ch"], obj["options"], outputdir])

os.system("mkdir -p {}".format(outputdir))

map(run_chain,enumerate(to_run))

# os.nice(10)
# runner = pyrun.Runner(nproc=min(len(to_run),25), func=run_chain, dot_type=2)
# runner.add_args(to_run)
# runner.run()

# see note in analysis/yields/py_doAll.py
import sys
sys.modules["numba"] = None
import plot
plot.make_plots(outputdir=plotdir)
