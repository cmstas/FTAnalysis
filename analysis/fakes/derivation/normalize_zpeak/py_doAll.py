import os
import analysis.utils.pyrun as pyrun
import ROOT as r
r.gROOT.SetBatch()

r.gROOT.ProcessLine(".L CMS3.cc+")
r.gROOT.ProcessLine(".L ScanChain.C+")


years_to_consider = [
        2016,
        2017,
        2018,
        "rereco_2018",
        ]
procs_to_consider = [
        ]

basedirs ={

        2016: "/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v2.0_leptontree/output/year_2016/",
        2017: "/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v2.0_leptontree/output/year_2017/",
        2018: "/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v2.0_leptontree/output/year_2018/",
        "rereco_2018": "/nfs-7/userdata/namin/tupler_babies/merged/LeptonTree/v2.0_leptontree/output/year_2018/",
        }

outputdir = "outputs_test"

options = {

        # 2016: " Data2016 quiet ",
        # 2017: " Data2017 quiet ",
        # 2018: " Data2018 quiet ",
        # 2016: " Data2016 ",
        # 2017: " Data2017 ",
        # 2018: " Data2018 ",
        # 2016: " Data2016 doPUweight ",
        # 2017: " Data2017 doPUweight ",
        # 2018: " Data2018 doPUweight ",
        2016: " quiet Data2016 doPUweight doLeptonSF ",
        2017: " quiet Data2017 doPUweight doLeptonSF ",
        2018: " quiet Data2018 doPUweight doLeptonSF ",
        "rereco_2018": " quiet Data2018 doPUweight doLeptonSF ",

        }

def make_objs(fpatts=[],options="",treename="t"):
    if type(fpatts) == str: fpatts = [fpatts]
    ch = r.TChain(treename)
    for fpatt in fpatts:
        ch.Add(fpatt)
    return {"ch": ch, "options": options}

chs = {
        2016: {
            "dy": make_objs(basedirs[2016]+"DY*.root", options=options[2016]),
            "data_mu": make_objs(basedirs[2016]+"DataDoubleMu*.root", options=options[2016]+" onlyMu "),
            "data_el": make_objs(basedirs[2016]+"DataDoubleEG*.root", options=options[2016]+" onlyEl "),
            },
        2017: {
            "dy": make_objs(basedirs[2017]+"DY*.root", options=options[2017]),
            "data_mu": make_objs(basedirs[2017]+"DataDoubleMu*.root", options=options[2017]+" onlyMu "),
            "data_el": make_objs(basedirs[2017]+"DataDoubleEG*.root", options=options[2017]+" onlyEl "),
            },
        2018: {
            "dy": make_objs(basedirs[2018]+"DY*.root", options=options[2018]),
            "data_mu": make_objs(basedirs[2018]+"DataDoubleMu*.root", options=options[2018]+" onlyMu "),
            "data_el": make_objs(basedirs[2018]+"DataDoubleEG*.root", options=options[2018]+" onlyEl "),
            },
        "rereco_2018": {
            "dy": make_objs(basedirs[2018]+"DY*.root", options=options[2018]),
            "data_mu": make_objs([
                basedirs[2018]+"ReRecoDataDoubleMu*.root",
                basedirs[2018]+"DataDoubleMuonD*.root",
                ], options=options[2018]+" onlyMu "),
            "data_el": make_objs([
                basedirs[2018]+"ReRecoDataDoubleEG*.root",
                basedirs[2018]+"DataDoubleEGD*.root",
                ], options=options[2018]+" onlyEl "),
            },
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

# map(run_chain, enumerate(to_run))

runner = pyrun.Runner(nproc=min(len(to_run),20), func=run_chain, dot_type=2)
runner.add_args(to_run)
runner.run()

from compare import print_sfs
for year in years_to_consider:
    if "2018" in str(year):
        if "rereco" in str(year):
            print "if (year == {} and  isReReco) {{".format(str(year).replace("rereco_",""))
        else:
            print "if (year == {} and !isReReco) {{".format(str(year).replace("rereco_",""))
    else:
        print "if (year == {}) {{".format(str(year).replace("rereco_",""))
    print "// year = {}".format(year)
    print_sfs(outputdir, year=year)
    print "}"
print "Remember to apply PUw in fr derivation looper"
print "Remember to apply lepton SF to tight leptons (MTCR)"
