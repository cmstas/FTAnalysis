import os
import analysis.utils.pyrun as pyrun
import ROOT as r
r.gROOT.SetBatch()

r.gROOT.ProcessLine(".L ../misc/class_files/v8.02/SS.cc+")
r.gROOT.ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc+")
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
        # "tt",
        ]

# years_to_consider = [2016]
# procs_to_consider = ["wz","vvnowz"]

basedirs ={

        2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2016/",
        2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2017/",
        # 2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2018/",
        # 2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.26/output/year_2018/", # Autumn18V3 JECs with dummy residuals
        # 2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.27_fixbtag//output/year_2018/", # Autumn18V3 JECs with Fall17 V32 F residuals, new btag SF
        2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.28//output/year_2018/", # Autumn18V8 "final" jecs (with residuals)

        }

# outputdir = "outputs_Dec10" # has isr rw derived from tt on 2017 2018
# outputdir = "outputs_Dec19_v6" # v6 jec + wps, htnb1 region to check fake counts and compare with new ptratio points
# outputdir = "outputs_Dec19_v32" # v32 jec + wps, htnb1 region to check fake counts and compare with new ptratio points
# outputdir = "outputs_Dec17toptag" # 

# outputdir = "outputs_19Jan4_v3p21" # ISR REWEIGHTING

# outputdir = "outputs_19Jan18_v3p24_ss" # FOR SS
# outputdir = "outputs_19Jan18_v3p24_ss_promptreco" # FOR SS
# outputdir = "outputs_19Jan18_v3p24_ft" # FOR FT

# outputdir = "outputs_19Jan18_v3p24_ss_2018nvtx" # FOR SS

# outputdir = "outputs_19Feb16_v3p26_ft" # FOR FT

# outputdir = "outputs_19Feb16_v3p26_ss" # FOR SS
# outputdir = "outputs_19Feb27_v3p27_ss_resid" # FOR SS
# outputdir = "outputs_19Feb16_v3p26_ss_noisrweights" # FOR SS
# outputdir = "outputs_19Mar4_v3p27_ss" # FOR SS
# outputdir = "outputs_19Mar4_v3p27_ft" # FOR FT
# outputdir = "outputs_19Mar4_v3p28_ft" # FOR FT
outputdir = "outputs_19Mar9_v3p28_ss" # FOR FT

# outputdir = "outputs_19Mar31_v3p28_2018_zbb" # FOR ZBB

options = {

        # 2016: "useInclusiveSFs doBDT Data2016 quiet ",
        # 2017: "useInclusiveSFs doBDT Data2017 minPtFake18 quiet ",
        # 2018: "useInclusiveSFs doBDT Data2018 minPtFake18 quiet ",

        # # for SS
        # 2016: " noISRWeights noLeptonPtCut doSS Data2016 new2016FRBins quiet ",
        # 2017: " noISRWeights noLeptonPtCut doSS Data2017 quiet ",
        # 2018: " noISRWeights noLeptonPtCut doSS Data2018 quiet ",
        
        # for SS
        2016: "  noLeptonPtCut doSS Data2016 new2016FRBins quiet ",
        2017: "  noLeptonPtCut doSS Data2017 quiet ",
        2018: "  noLeptonPtCut doSS Data2018 quiet ",

        # # for FT
        # 2016: " Data2016 new2016FRBins quiet ",
        # 2017: " Data2017 minPtFake18 quiet ",
        # 2018: " Data2018 minPtFake18 quiet ",

        # 2018: "useInclusiveSFs Data2018 minPtFake18 quiet doHEMAfter ", # FIXME
        # 2018: "useInclusiveSFs Data2018 minPtFake18 quiet doHEMBefore ", # FIXME

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
            "ttnonhf": make_objs(basedirs[2016]+"TTBAR*.root", options=options[2016]+" doNonTTHF"),
            "tthf": make_objs(basedirs[2016]+"TTBAR*.root", options=options[2016]+" doTTHF"),
            "fakes": make_objs(basedirs[2016]+"Data*.root", options=options[2016]+" doFakes"),
            "flips": make_objs(basedirs[2016]+"Data*.root", options=options[2016]+" doFlips"),
            "data": make_objs(basedirs[2016]+"Data*.root", options=options[2016]),
            "ttw": make_objs(basedirs[2016]+"TTWnlo.root", options=options[2016]),
            "tth": make_objs(basedirs[2016]+"TTHtoNonBB.root", options=options[2016]),
            "dy": make_objs([
                basedirs[2016]+"DY_high.root",
                basedirs[2016]+"DY_low.root",
                ], options=options[2016]),
            "wjets": make_objs(basedirs[2016]+"WJets_HT*.root", options=options[2016]),
            "ttfake": make_objs(basedirs[2016]+"TTBAR*.root", options=options[2016]+ "doTruthFake"),
            "ttmg": make_objs([
                basedirs[2016]+"TTDL.root",
                basedirs[2016]+"TTSLtop.root",
                basedirs[2016]+"TTSLtopbar.root",
                ], options=options[2016]),
            # "ttlomg": make_objs(basedirs[2016]+"TTLOMG.root", options=options[2016]),
            # "ttslph": make_objs(basedirs[2016]+"TTSLPH.root", options=options[2016]),
            "ttz": make_objs([
                basedirs[2016]+"TTZnlo.root",
                basedirs[2016]+"TTZLOW.root",
                ]
                , options=options[2016]),
            "wz": make_objs([
                basedirs[2016]+"WZ.root",
                ],options=options[2016]),
            "vvnowz": make_objs([
                basedirs[2016]+"QQWW.root",
                basedirs[2016]+"WWDPS.root",
                basedirs[2016]+"ZZ.root",
                ],options=options[2016]),
            "vv": make_objs([
                basedirs[2016]+"WZ.root",
                basedirs[2016]+"QQWW.root",
                basedirs[2016]+"WWDPS.root",
                basedirs[2016]+"ZZ.root",
                ],options=options[2016]),
            "tttt": make_objs([
                basedirs[2016]+"TTTTnew.root",
                ],options=options[2016]),
            "zgamma": make_objs([
                basedirs[2016]+"ZG.root",
                ],options=options[2016]),
            "wgamma": make_objs([
                basedirs[2016]+"WGToLNuGext.root",
                ],options=options[2016]),
            "othergamma": make_objs([
                basedirs[2016]+"TGext.root",
                basedirs[2016]+"TTG.root",
                basedirs[2016]+"WZG.root",
                ],options=options[2016]),
            "raresnoxg": make_objs([
                basedirs[2016]+"VHtoNonBB.root",
                basedirs[2016]+"GGHtoZZto4L.root",
                basedirs[2016]+"WZZ.root",
                basedirs[2016]+"ZZZ.root",
                basedirs[2016]+"WWZ.root",
                basedirs[2016]+"WWW.root",
                basedirs[2016]+"TZQ.root",
                basedirs[2016]+"TWZ.root",
                basedirs[2016]+"TTZZ.root",
                basedirs[2016]+"TTWZ.root",
                basedirs[2016]+"TTWW.root",
                basedirs[2016]+"TTWH.root",
                basedirs[2016]+"TTHH.root",
                basedirs[2016]+"TTTW.root",
                basedirs[2016]+"TTTJ.root",
                ],options=options[2016]),
            "rares": make_objs([
                basedirs[2016]+"TGext.root",
                basedirs[2016]+"TTG.root",
                basedirs[2016]+"WZG.root",
                basedirs[2016]+"WGToLNuGext.root",
                basedirs[2016]+"VHtoNonBB.root",
                basedirs[2016]+"GGHtoZZto4L.root",
                basedirs[2016]+"ZG.root",
                basedirs[2016]+"WZZ.root",
                basedirs[2016]+"ZZZ.root",
                basedirs[2016]+"WWZ.root",
                basedirs[2016]+"WWW.root",
                basedirs[2016]+"TZQ.root",
                basedirs[2016]+"TWZ.root",
                basedirs[2016]+"TTZZ.root",
                basedirs[2016]+"TTWZ.root",
                basedirs[2016]+"TTWW.root",
                basedirs[2016]+"TTWH.root",
                basedirs[2016]+"TTHH.root",
                basedirs[2016]+"TTTW.root",
                basedirs[2016]+"TTTJ.root",
                ],options=options[2016]),
            "singletop": make_objs([
                basedirs[2016]+"ST1.root",
                basedirs[2016]+"ST2.root",
                ],options=options[2016]),
            },
        2017: {

            "tt": make_objs(basedirs[2017]+"TTBAR*.root", options=options[2017]),
            "ttnonhf": make_objs(basedirs[2017]+"TTBAR*.root", options=options[2017]+" doNonTTHF"),
            "tthf": make_objs(basedirs[2017]+"TTBAR*.root", options=options[2017]+" doTTHF"),
            "fakes": make_objs(basedirs[2017]+"Data*.root", options=options[2017]+" doFakes"),
            "flips": make_objs(basedirs[2017]+"Data*.root", options=options[2017]+" doFlips"),
            "data": make_objs(basedirs[2017]+"Data*.root", options=options[2017]),
            "ttw": make_objs(basedirs[2017]+"TTWnlo.root", options=options[2017]),
            "tth": make_objs(basedirs[2017]+"TTHtoNonBB.root", options=options[2017]),
            "dy": make_objs([
                basedirs[2017]+"DY_high.root",
                basedirs[2017]+"DY_low.root",
                ], options=options[2017]),
            "wjets": make_objs(basedirs[2017]+"WJets_HT*.root", options=options[2017]),
            # "wjets": make_objs([
            #     "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.10_prefire/output/year_2017/WJets_HT*.root" # FIXME
            #     ], options=options[2017]),
            "ttfake": make_objs(basedirs[2017]+"TTBAR*.root", options=options[2017]+ "doTruthFake"),
            # "ttlomg": make_objs(basedirs[2017]+"TTLOMG.root", options=options[2017]),
            # "ttslph": make_objs(basedirs[2017]+"TTSLPH.root", options=options[2017]),
            "ttdl0jet": make_objs([
                basedirs[2017]+"TTdilep0jet*.root",
                ], options=options[2017]),
            "ttdl1jet": make_objs([
                basedirs[2017]+"TTdilep1jet*.root",
                ], options=options[2017]),
            "ttz": make_objs([
                basedirs[2017]+"TTZnlo.root",
                basedirs[2017]+"TTZLOW.root",
                ]
                , options=options[2017]),
            "wz": make_objs([
                basedirs[2017]+"WZ.root",
                ],options=options[2017]),
            "vvnowz": make_objs([
                basedirs[2017]+"QQWW.root",
                basedirs[2017]+"WW.root",
                basedirs[2017]+"WWDPS.root",
                basedirs[2017]+"ZZ.root",
                ],options=options[2017]),
            "vv": make_objs([
                basedirs[2017]+"WZ.root",
                basedirs[2017]+"QQWW.root",
                basedirs[2017]+"WW.root",
                basedirs[2017]+"WWDPS.root",
                basedirs[2017]+"ZZ.root",
                ],options=options[2017]),
            "tttt": make_objs([
                basedirs[2017]+"TTTTnew.root",
                ],options=options[2017]),
            "zgamma": make_objs([
                basedirs[2017]+"ZG.root",
                ],options=options[2017]),
            "wgamma": make_objs([
                basedirs[2017]+"WGToLNuGext.root",
                ],options=options[2017]),
            "othergamma": make_objs([
                basedirs[2017]+"TGext.root",
                basedirs[2017]+"TTGdilep.root",
                basedirs[2017]+"TTGsinglelepbar.root",
                basedirs[2017]+"TTGsinglelep.root",
                basedirs[2017]+"WZG.root",
                ],options=options[2016]),
            "raresnoxg": make_objs([
                basedirs[2017]+"VHtoNonBB.root",
                basedirs[2017]+"GGHtoZZto4L.root",
                basedirs[2017]+"WZZ.root",
                basedirs[2017]+"ZZZ.root",
                basedirs[2017]+"WWZ.root",
                basedirs[2017]+"WWW.root",
                basedirs[2017]+"TZQ.root",
                basedirs[2017]+"TWZ.root",
                basedirs[2017]+"TTZZ.root",
                basedirs[2017]+"TTWZ.root",
                basedirs[2017]+"TTWW.root",
                basedirs[2017]+"TTWH.root",
                basedirs[2017]+"TTHH.root",
                basedirs[2017]+"TTTW.root",
                basedirs[2017]+"TTTJ.root",
                ],options=options[2017]),
            "rares": make_objs([
                basedirs[2017]+"TGext.root",
                basedirs[2017]+"TTGdilep.root",
                basedirs[2017]+"TTGsinglelepbar.root",
                basedirs[2017]+"TTGsinglelep.root",
                basedirs[2017]+"WZG.root",
                basedirs[2017]+"WZZ.root",
                basedirs[2017]+"ZZZ.root",
                basedirs[2017]+"WWZ.root",
                basedirs[2017]+"WWW.root",
                basedirs[2017]+"TWZ.root",
                basedirs[2017]+"TZQ.root",
                basedirs[2017]+"TTZZ.root",
                basedirs[2017]+"TTWZ.root",
                basedirs[2017]+"TTWW.root",
                basedirs[2017]+"TTWH.root",
                basedirs[2017]+"TTHH.root",
                basedirs[2017]+"TTTW.root",
                basedirs[2017]+"TTTJ.root",
                basedirs[2017]+"GGHtoZZto4L.root",
                basedirs[2017]+"VHtoNonBB.root",
                basedirs[2017]+"WGToLNuGext.root",
                basedirs[2017]+"ZG.root",
                ],options=options[2017]),
            "singletop": make_objs([
                basedirs[2017]+"STtop.root",
                basedirs[2017]+"STantitop.root",
                ],options=options[2017]),

            },
        2018: {

            "fakes": make_objs([basedirs[2018]+"ReRecoData*.root",basedirs[2018]+"Data*Dv2.root"], options=options[2018]+" doFakes"),
            "flips": make_objs([basedirs[2018]+"ReRecoData*.root",basedirs[2018]+"Data*Dv2.root"], options=options[2018]+" doFlips"),
            "data": make_objs([basedirs[2018]+"ReRecoData*.root",basedirs[2018]+"Data*Dv2.root"], options=options[2018]),

            # "fakes": make_objs([basedirs[2018]+"Data*.root"], options=options[2018]+" doFakes"),
            # "flips": make_objs([basedirs[2018]+"Data*.root"], options=options[2018]+" doFlips"),
            # "data": make_objs([basedirs[2018]+"Data*.root"], options=options[2018]),

            "tt": make_objs([
                basedirs[2018]+"TTBAR*.root",
                ], options=options[2018]),
            "ttnonhf": make_objs(basedirs[2018]+"TTBAR*.root", options=options[2018]+" doNonTTHF"),
            "tthf": make_objs(basedirs[2018]+"TTBAR*.root", options=options[2018]+" doTTHF"),
            "ttw": make_objs(basedirs[2018]+"TTWnlo.root", options=options[2018]),
            "tth": make_objs(basedirs[2018]+"TTHtoNonBB.root", options=options[2018]),
            "dy": make_objs([
                basedirs[2018]+"DY_high.root",
                basedirs[2018]+"DY_low.root",
                ], options=options[2018]),
            "wjets": make_objs(basedirs[2018]+"WJets*.root", options=options[2018]),
            "ttfake": make_objs(basedirs[2018]+"TTBAR*.root", options=options[2018]+ "doTruthFake"),
            "ttdl0jet": make_objs([
                basedirs[2018]+"TTdilep0jet*.root",
                ], options=options[2018]),
            "ttdl1jet": make_objs([
                basedirs[2018]+"TTdilep1jet*.root",
                ], options=options[2018]),
            "ttz": make_objs([
                basedirs[2018]+"TTZnlo.root",
                basedirs[2018]+"TTZLOW.root",
                ], options=options[2018]),
            "wz": make_objs([
                basedirs[2018]+"WZ.root",
                ],options=options[2018]),
            "vvnowz": make_objs([
                basedirs[2018]+"QQWW.root",
                basedirs[2018]+"WW.root",
                basedirs[2018]+"WWDPS.root",
                basedirs[2018]+"ZZ.root",
                ],options=options[2018]),
            "vv": make_objs([
                basedirs[2018]+"WZ.root",
                basedirs[2018]+"QQWW.root",
                basedirs[2018]+"WW.root",
                basedirs[2018]+"WWDPS.root",
                basedirs[2018]+"ZZ.root",
                ],options=options[2018]),
            "tttt": make_objs([
                basedirs[2018]+"TTTTnew.root",
                ],options=options[2018]),
            "zgamma": make_objs([
                basedirs[2018]+"ZG.root",
                ],options=options[2018]),
            "wgamma": make_objs([
                basedirs[2018]+"WGToLNuGext.root",
                ],options=options[2018]),
            "othergamma": make_objs([
                basedirs[2018]+"TGext.root",
                basedirs[2018]+"TTGdilep.root",
                basedirs[2018]+"TTGsinglelepbar.root",
                basedirs[2018]+"TTGsinglelep.root",
                basedirs[2018]+"WZG.root",
                ],options=options[2018]),
            "raresnoxg": make_objs([
                basedirs[2018]+"VHtoNonBB.root",
                basedirs[2018]+"GGHtoZZto4L.root",
                basedirs[2018]+"WZZ.root",
                basedirs[2018]+"ZZZ.root",
                basedirs[2018]+"WWZ.root",
                basedirs[2018]+"WWW.root",
                basedirs[2018]+"TZQ.root",
                basedirs[2018]+"TWZ.root",
                basedirs[2018]+"TTZZ.root",
                basedirs[2018]+"TTWZ.root",
                basedirs[2018]+"TTWW.root",
                basedirs[2018]+"TTWH.root",
                basedirs[2018]+"TTHH.root",
                basedirs[2018]+"TTTW.root",
                basedirs[2018]+"TTTJ.root",
                ],options=options[2018]),
            "rares": make_objs([
                basedirs[2018]+"TGext.root",
                basedirs[2018]+"TTGdilep.root",
                basedirs[2018]+"TTGsinglelepbar.root",
                basedirs[2018]+"TTGsinglelep.root",
                basedirs[2018]+"WZG.root",
                basedirs[2018]+"WZZ.root",
                basedirs[2018]+"ZZZ.root",
                basedirs[2018]+"WWZ.root",
                basedirs[2018]+"WWW.root",
                basedirs[2018]+"TWZ.root",
                basedirs[2018]+"TZQ.root",
                basedirs[2018]+"TTZZ.root",
                basedirs[2018]+"TTWZ.root",
                basedirs[2018]+"TTWW.root",
                basedirs[2018]+"TTWH.root",
                basedirs[2018]+"TTHH.root",
                basedirs[2018]+"TTTW.root",
                basedirs[2018]+"TTTJ.root",
                basedirs[2018]+"VHtoNonBB.root",
                basedirs[2018]+"GGHtoZZto4L.root",
                basedirs[2018]+"WGToLNuGext.root",
                basedirs[2018]+"ZG.root",
                ],options=options[2018]),
            "singletop": make_objs([
                basedirs[2018]+"STtop.root",
                basedirs[2018]+"STantitop.root",
                ],options=options[2018]),
            # "zbb": make_objs([
            #     "/home/users/namin/2018/fourtop/all/FTAnalysis/babymaking/batch/output_TTZBB_2018.root",
            #     ],options=options[2018])

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

os.nice(10)
runner = pyrun.Runner(nproc=min(len(to_run),25), func=run_chain, dot_type=2)
runner.add_args(to_run)
runner.run()
