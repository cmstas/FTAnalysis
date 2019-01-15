#!/usr/bin/env python

import os
import analysis.utils.pyrun as pyrun
import argparse
import fnmatch
import time

def make_obj(fpatts=[],options="",treename="t"):
    if type(fpatts) == str: fpatts = [fpatts]
    ch = r.TChain(treename)
    for fpatt in fpatts:
        ch.Add(fpatt)
    return {"ch": ch, "options": options}

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--out", help="output directory", default="outputs")
    parser.add_argument("-e", "--extra_options", help="quoted string of extra options", default="")
    parser.add_argument("-t", "--tag", help="tag for bookkeeping and output directory location", default="v3.08_allyears_tmp")

    parser.add_argument(      "--year", help="year, if you only want to run one", default="")
    parser.add_argument(      "--proc", help="process, if you only want to run one/some. accepts wildcards if quoted.", default="", type=str)

    parser.add_argument("-n", "--noloop", help="skip looping/scanchain", action="store_true")
    parser.add_argument("-s", "--shapes", help="make shape hists and copy to limit directory tag folder", action="store_true")
    parser.add_argument("-p", "--plots", help="make plots and copy to the limit directory tag folder", action="store_true")
    parser.add_argument("-f", "--fastsim", help="include fastsim scans", action="store_true")
    parser.add_argument("-v", "--verbosity", help="verbosity level (0 = default,1,2)", default=0, type=int)

    args = parser.parse_args()

    try:
        args.year = int(args.year)
    except:
        pass

    import ROOT as r
    r.gROOT.SetBatch()

    if not args.noloop:
        r.gROOT.ProcessLine(".L ../misc/class_files/v8.02/SS.cc+")
        r.gROOT.ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc+")
        r.gROOT.ProcessLine(".L yieldMaker.C+")


    years_to_consider = [
            # "2016_94x",
            2016,
            2017,
            2018,
            ]

    basedirs = {
            # # 2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2016/",
            # # 2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2017/",
            # # 2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2018/",
            # "2016_94x": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_all/output/year_2016_94x/",
            # # 2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_all/output/year_2016/",
            # 2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_lowpt3/output/year_2016/", # FIXME
            # 2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_all/output/year_2017/",
            # 2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_all/output/year_2018/",

            "2016_94x": "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_all/output/year_2016_94x/",
            # 2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.11_all/output/year_2016/",
            # 2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.11_all/output/year_2017/",
            # 2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.11_all/output/year_2018/",

            # # 2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.13_all/output/year_2016/",
            # 2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.14_all/output/year_2016/",
            # # 2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.13_all/output/year_2017/",
            # # 2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.13_all/output/year_2018/",
            # 2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.20_jec32/output/year_2017/",
            # 2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.20_jec32/output/year_2018/",

            2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.21/output/year_2016/",
            2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.21_fix2017/output/year_2017/",
            2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.22/output/year_2018/",

            # secondary basedirs for debugging
            20162: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.14_all/output/year_2016/",
            20172: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.14_all/output/year_2017/",
            20182: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.14_all/output/year_2018/",

            # 2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.15_all/output/year_2016/",
            # 2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.15_all/output/year_2017/",
            # 2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.15_all/output/year_2018/",
            # # secondary basedirs for debugging
            # 20162: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.15_all/output/year_2016/",
            # 20172: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.15_all/output/year_2017/",
            # 20182: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.15_all/output/year_2018/",

            }

    outputdir = args.out
    extra_global_options = args.extra_options
    options = {
            "2016_94x": "Data2016 94x quiet {} evaluateBDT ".format(extra_global_options),
            2016: "Data2016 quiet {} evaluateBDT ".format(extra_global_options),
            2017: "Data2017 quiet {} evaluateBDT minPtFake18 ".format(extra_global_options),
            2018: "Data2018 quiet {} evaluateBDT minPtFake18 ".format(extra_global_options),
            }


    chs = {

            "2016_94x": {

                "fakes": make_obj([
                    basedirs["2016_94x"]+"Data*.root", # FIXME eventually need to add TTV,TTH for EWK subtraction
                    ] , options=options["2016_94x"]+" doFakes doData "),
                "flips": make_obj(basedirs["2016_94x"]+"Data*.root", options=options["2016_94x"]+" doFlips doData "),
                "data": make_obj(basedirs["2016_94x"]+"Data*.root", options=options["2016_94x"] + " doData "),
                "tttt": make_obj(basedirs["2016_94x"]+"TTTTnew.root", options=options["2016_94x"]),

                },
            2016: {

                "fakes": make_obj([
                    basedirs[2016]+"Data*.root",
                    basedirs[2016]+"TTWnlo.root",
                    basedirs[2016]+"TTZnlo.root",
                    basedirs[2016]+"TTHtoNonBB.root",
                    ] , options=options[2016]+" doFakes doData "),
                "fakes_app": make_obj([
                    basedirs[2016]+"Data*.root",
                    # basedirs[2016]+"TTWnlo.root",
                    # basedirs[2016]+"TTZnlo.root",
                    # basedirs[2016]+"TTHtoNonBB.root",
                    ] , options=options[2016]+" doFakesUnw doData "),
                "flips": make_obj(basedirs[2016]+"Data*.root", options=options[2016]+" doFlips doData "),
                "data": make_obj(basedirs[2016]+"Data*.root", options=options[2016] + " doData "),
                "tttt": make_obj(basedirs[2016]+"TTTTnew.root", options=options[2016]),
                "ttttisrup": make_obj(basedirs[2016]+"TTTTisrup.root", options=options[2016]),
                "ttttisrdn": make_obj(basedirs[2016]+"TTTTisrdown.root", options=options[2016]),
                "ttttfsrup": make_obj(basedirs[2016]+"TTTTfsrup.root", options=options[2016]),
                "ttttfsrdn": make_obj(basedirs[2016]+"TTTTfsrdown.root", options=options[2016]),
                # "fakes_mc": make_obj(basedirs[2016]+"TTBAR*.root", options=options[2016]+ " doFakesMC "),
                "fakes_mc": make_obj([
                    basedirs[2016]+"TTBAR_PH.root",
                    basedirs[2016]+"TTSLht500.root",
                    basedirs[2016]+"TTDLht500.root",
                    ] , options=options[2016]+ " doTruthFake doStitch "),
                "fakes_mchybrid": make_obj([
                    basedirs[2016]+"TTBAR_PH.root",
                    basedirs[2016]+"TTSLht500.root",
                    basedirs[2016]+"TTDLht500.root",
                    ] , options=options[2016]+ " doFakesMC doStitch "),
                "fakes_mchybrid_app": make_obj([
                    basedirs[2016]+"TTBAR_PH.root",
                    basedirs[2016]+"TTSLht500.root",
                    basedirs[2016]+"TTDLht500.root",
                    ] , options=options[2016]+ " doFakesMCUnw doStitch "),
                "ttw": make_obj(basedirs[2016]+"TTWnlo.root", options=options[2016]),
                "tth": make_obj(basedirs[2016]+"TTHtoNonBB.root", options=options[2016]),
                "ttz": make_obj([
                    basedirs[2016]+"TTZnlo.root",
                    basedirs[2016]+"TTZLOW.root",
                    ] , options=options[2016]),
                "xg": make_obj([
                    basedirs[2016]+"TGext.root",
                    basedirs[2016]+"TTGdilep.root",
                    basedirs[2016]+"TTGsinglelepbar.root",
                    basedirs[2016]+"TTGsinglelep.root",
                    basedirs[2016]+"WGToLNuGext.root",
                    basedirs[2016]+"ZG.root",
                    ],options=options[2016] + " doXgamma "),
                "ttvv": make_obj([
                    basedirs[2016]+"TTHH.root",
                    basedirs[2016]+"TTWH.root",
                    basedirs[2016]+"TTWW.root",
                    basedirs[2016]+"TTWZ.root",
                    basedirs[2016]+"TTZH.root",
                    basedirs[2016]+"TTZZ.root",
                    ],options=options[2016]),
                "rares": make_obj([
                    basedirs[2016]+"GGHtoZZto4L.root",
                    basedirs[2016]+"QQWW.root",
                    basedirs[2016]+"TWZ.root",
                    basedirs[2016]+"TZQ.root",
                    basedirs[2016]+"VHtoNonBB.root",
                    basedirs[2016]+"WWDPS.root",
                    basedirs[2016]+"WWW.root",
                    basedirs[2016]+"WWZ.root",
                    basedirs[2016]+"WZ.root",
                    basedirs[2016]+"WZG.root",
                    basedirs[2016]+"WWG.root",
                    basedirs[2016]+"WZZ.root",
                    basedirs[2016]+"ZZ.root",
                    basedirs[2016]+"ZZZ.root",
                    basedirs[2016]+"TTTJ.root",
                    basedirs[2016]+"TTTW.root",
                    ],options=options[2016]),

                # "higgsh350": make_obj([ basedirs[20162]+"Higgs_ttH_350.root", basedirs[20162]+"Higgs_tHW_350.root", basedirs[20162]+"Higgs_tHq_350.root", ],options=options[2016]),
                # "higgsh370": make_obj([ basedirs[20162]+"Higgs_ttH_370.root", basedirs[20162]+"Higgs_tHW_370.root", basedirs[20162]+"Higgs_tHq_370.root", ],options=options[2016]),
                # "higgsh390": make_obj([ basedirs[20162]+"Higgs_ttH_390.root", basedirs[20162]+"Higgs_tHW_390.root", basedirs[20162]+"Higgs_tHq_390.root", ],options=options[2016]),
                # "higgsh410": make_obj([ basedirs[20162]+"Higgs_ttH_410.root", basedirs[20162]+"Higgs_tHW_410.root", basedirs[20162]+"Higgs_tHq_410.root", ],options=options[2016]),
                # "higgsh430": make_obj([ basedirs[20162]+"Higgs_ttH_430.root", basedirs[20162]+"Higgs_tHW_430.root", basedirs[20162]+"Higgs_tHq_410.root", ],options=options[2016]),
                # "higgsh450": make_obj([ basedirs[20162]+"Higgs_ttH_450.root", basedirs[20162]+"Higgs_tHW_450.root", basedirs[20162]+"Higgs_tHq_450.root", ],options=options[2016]),
                # "higgsh470": make_obj([ basedirs[20162]+"Higgs_ttH_470.root", basedirs[20162]+"Higgs_tHW_470.root", basedirs[20162]+"Higgs_tHq_470.root", ],options=options[2016]),
                # "higgsh490": make_obj([ basedirs[20162]+"Higgs_ttH_490.root", basedirs[20162]+"Higgs_tHW_490.root", basedirs[20162]+"Higgs_tHq_490.root", ],options=options[2016]),
                # "higgsh510": make_obj([ basedirs[20162]+"Higgs_ttH_510.root", basedirs[20162]+"Higgs_tHW_510.root", basedirs[20162]+"Higgs_tHq_510.root", ],options=options[2016]),
                # "higgsh530": make_obj([ basedirs[20162]+"Higgs_ttH_530.root", basedirs[20162]+"Higgs_tHW_530.root", basedirs[20162]+"Higgs_tHq_530.root", ],options=options[2016]),
                # "higgsh550": make_obj([ basedirs[20162]+"Higgs_ttH_550.root", basedirs[20162]+"Higgs_tHW_550.root", basedirs[20162]+"Higgs_tHq_550.root", ],options=options[2016]),
                # "higgsh570": make_obj([ basedirs[20162]+"Higgs_ttH_570.root", basedirs[20162]+"Higgs_tHW_570.root", basedirs[20162]+"Higgs_tHq_570.root", ],options=options[2016]),
                # "higgsh590": make_obj([ basedirs[20162]+"Higgs_ttH_590.root", basedirs[20162]+"Higgs_tHW_590.root", basedirs[20162]+"Higgs_tHq_590.root", ],options=options[2016]),
                # "higgsh610": make_obj([ basedirs[20162]+"Higgs_ttH_610.root", basedirs[20162]+"Higgs_tHW_610.root", basedirs[20162]+"Higgs_tHq_610.root", ],options=options[2016]),
                # "higgsh630": make_obj([ basedirs[20162]+"Higgs_ttH_610.root", basedirs[20162]+"Higgs_tHW_610.root", basedirs[20162]+"Higgs_tHq_610.root", ],options=options[2016]),
                # "higgsh650": make_obj([ basedirs[20162]+"Higgs_ttH_610.root", basedirs[20162]+"Higgs_tHW_610.root", basedirs[20162]+"Higgs_tHq_610.root", ],options=options[2016]),

                # "higgsa350": make_obj([ basedirs[20162]+"Higgs_ttH_350.root", basedirs[20162]+"Higgs_tHW_350.root", basedirs[20162]+"Higgs_tHq_350.root", ],options=options[2016]),
                # "higgsa370": make_obj([ basedirs[20162]+"Higgs_ttH_370.root", basedirs[20162]+"Higgs_tHW_370.root", basedirs[20162]+"Higgs_tHq_370.root", ],options=options[2016]),
                # "higgsa390": make_obj([ basedirs[20162]+"Higgs_ttH_390.root", basedirs[20162]+"Higgs_tHW_390.root", basedirs[20162]+"Higgs_tHq_390.root", ],options=options[2016]),
                # "higgsa410": make_obj([ basedirs[20162]+"Higgs_ttH_410.root", basedirs[20162]+"Higgs_tHW_410.root", basedirs[20162]+"Higgs_tHq_410.root", ],options=options[2016]),
                # "higgsa430": make_obj([ basedirs[20162]+"Higgs_ttH_430.root", basedirs[20162]+"Higgs_tHW_430.root", basedirs[20162]+"Higgs_tHq_410.root", ],options=options[2016]),
                # "higgsa450": make_obj([ basedirs[20162]+"Higgs_ttH_450.root", basedirs[20162]+"Higgs_tHW_450.root", basedirs[20162]+"Higgs_tHq_450.root", ],options=options[2016]),
                # "higgsa470": make_obj([ basedirs[20162]+"Higgs_ttH_470.root", basedirs[20162]+"Higgs_tHW_470.root", basedirs[20162]+"Higgs_tHq_470.root", ],options=options[2016]),
                # "higgsa490": make_obj([ basedirs[20162]+"Higgs_ttH_490.root", basedirs[20162]+"Higgs_tHW_490.root", basedirs[20162]+"Higgs_tHq_490.root", ],options=options[2016]),
                # "higgsa510": make_obj([ basedirs[20162]+"Higgs_ttH_510.root", basedirs[20162]+"Higgs_tHW_510.root", basedirs[20162]+"Higgs_tHq_510.root", ],options=options[2016]),
                # "higgsa530": make_obj([ basedirs[20162]+"Higgs_ttH_530.root", basedirs[20162]+"Higgs_tHW_530.root", basedirs[20162]+"Higgs_tHq_530.root", ],options=options[2016]),
                # "higgsa550": make_obj([ basedirs[20162]+"Higgs_ttH_550.root", basedirs[20162]+"Higgs_tHW_550.root", basedirs[20162]+"Higgs_tHq_550.root", ],options=options[2016]),
                # "higgsa570": make_obj([ basedirs[20162]+"Higgs_ttH_570.root", basedirs[20162]+"Higgs_tHW_570.root", basedirs[20162]+"Higgs_tHq_570.root", ],options=options[2016]),
                # "higgsa590": make_obj([ basedirs[20162]+"Higgs_ttH_590.root", basedirs[20162]+"Higgs_tHW_590.root", basedirs[20162]+"Higgs_tHq_590.root", ],options=options[2016]),
                # "higgsa610": make_obj([ basedirs[20162]+"Higgs_ttH_610.root", basedirs[20162]+"Higgs_tHW_610.root", basedirs[20162]+"Higgs_tHq_610.root", ],options=options[2016]),
                # "higgsa630": make_obj([ basedirs[20162]+"Higgs_ttH_610.root", basedirs[20162]+"Higgs_tHW_610.root", basedirs[20162]+"Higgs_tHq_610.root", ],options=options[2016]),
                # "higgsa650": make_obj([ basedirs[20162]+"Higgs_ttH_610.root", basedirs[20162]+"Higgs_tHW_610.root", basedirs[20162]+"Higgs_tHq_610.root", ],options=options[2016]),

                },
            2017: {

                "fakes": make_obj([
                    basedirs[2017]+"Data*.root",
                    basedirs[2017]+"TTWnlo.root",
                    basedirs[2017]+"TTZnlo.root",
                    basedirs[2017]+"TTHtoNonBB.root",
                    ] , options=options[2017]+" doFakes doData "),
                "fakes_app": make_obj([
                    basedirs[2017]+"Data*.root",
                    # basedirs[2017]+"TTWnlo.root",
                    # basedirs[2017]+"TTZnlo.root",
                    # basedirs[2017]+"TTHtoNonBB.root",
                    ] , options=options[2017]+" doFakesUnw doData "),
                "flips": make_obj(basedirs[2017]+"Data*.root", options=options[2017]+" doFlips "),
                "data": make_obj(basedirs[2017]+"Data*.root", options=options[2017]+" doData "),
                # "fakes_mc": make_obj(basedirs[2017]+"TTBAR*.root", options=options[2017]+ " doFakesMC "),
                "fakes_mc": make_obj([
                    basedirs[2017]+"TTDL.root",
                    basedirs[2017]+"TTSLtop.root",
                    basedirs[2017]+"TTSLtopbar.root",
                    ] , options=options[2017]+ " doTruthFake "),
                "fakes_mchybrid": make_obj([
                    basedirs[2017]+"TTDL.root",
                    basedirs[2017]+"TTSLtop.root",
                    basedirs[2017]+"TTSLtopbar.root",
                    ] , options=options[2017]+ " doFakesMC "),
                "fakes_mchybrid_app": make_obj([
                    basedirs[2017]+"TTDL.root",
                    basedirs[2017]+"TTSLtop.root",
                    basedirs[2017]+"TTSLtopbar.root",
                    ] , options=options[2017]+ " doFakesMCUnw "),

                "tttt": make_obj(basedirs[2017]+"TTTTnew.root", options=options[2017]),
                "ttw": make_obj(basedirs[2017]+"TTWnlo.root", options=options[2017]),
                "tth": make_obj(basedirs[2017]+"TTHtoNonBB.root", options=options[2017]),
                "ttz": make_obj([
                    basedirs[2017]+"TTZnlo.root",
                    basedirs[2017]+"TTZLOW.root",
                    ] , options=options[2017]),

                # "tttt": make_obj("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_newdeepflavv2//output/year_2017/TTTTnew.root", options=options[2017]),
                # "ttw": make_obj("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_newdeepflavv2//output/year_2017/TTWnlo.root", options=options[2017]),
                # "tth": make_obj("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_newdeepflavv2//output/year_2017/TTHtoNonBB.root", options=options[2017]),
                # "ttz": make_obj([
                #     "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_newdeepflavv2//output/year_2017/TTZnlo.root",
                #     basedirs[2017]+"TTZLOW.root",
                #     ] , options=options[2017]),

                "xg": make_obj([
                    basedirs[2017]+"TGext.root",
                    basedirs[2017]+"TTGdilep.root", # TODO This is LO since NLO doesn't exist yet
                    basedirs[2017]+"TTGsinglelepbar.root",
                    basedirs[2017]+"TTGsinglelep.root",
                    basedirs[2017]+"WGToLNuGext.root", # TODO This is LO since NLO doesn't exist yet
                    # basedirs[2017]+"ZG.root", # TODO Don't have this yet
                    ],options=options[2017] + " doXgamma "),
                "ttvv": make_obj([
                    basedirs[2017]+"TTHH.root",
                    basedirs[2017]+"TTWH.root",
                    basedirs[2017]+"TTWW.root",
                    basedirs[2017]+"TTWZ.root",
                    basedirs[2017]+"TTZH.root",
                    basedirs[2017]+"TTZZ.root",
                    ],options=options[2017]),
                "rares": make_obj([
                    basedirs[2017]+"GGHtoZZto4L.root",
                    basedirs[2017]+"QQWW.root",
                    basedirs[2017]+"TWZ.root",
                    basedirs[2017]+"TZQ.root",
                    basedirs[2017]+"VHtoNonBB.root",
                    basedirs[2017]+"WWDPS.root",
                    basedirs[2017]+"WWW.root",
                    basedirs[2017]+"WWZ.root",
                    basedirs[2017]+"WZ.root",
                    basedirs[2017]+"WGToLNuGext.root",
                    basedirs[2017]+"WZG.root",
                    basedirs[2017]+"WWG.root",
                    basedirs[2017]+"WZZ.root",
                    basedirs[2017]+"ZZ.root",
                    basedirs[2017]+"ZZZ.root",
                    basedirs[2017]+"TTTJ.root",
                    basedirs[2017]+"TTTW.root",
                    ],options=options[2017]),

                # "higgsh350": make_obj([ basedirs[20172]+"Higgs_ttH_350.root", basedirs[20172]+"Higgs_tHW_350.root", basedirs[20172]+"Higgs_tHq_350.root", ],options=options[2017]),
                # "higgsh370": make_obj([ basedirs[20172]+"Higgs_ttH_375.root", basedirs[20172]+"Higgs_tHW_375.root", basedirs[20172]+"Higgs_tHq_375.root", ],options=options[2017]),
                # "higgsh390": make_obj([ basedirs[20172]+"Higgs_ttH_400.root", basedirs[20172]+"Higgs_tHW_400.root", basedirs[20172]+"Higgs_tHq_400.root", ],options=options[2017]),
                # "higgsh410": make_obj([ basedirs[20172]+"Higgs_ttH_400.root", basedirs[20172]+"Higgs_tHW_400.root", basedirs[20172]+"Higgs_tHq_400.root", ],options=options[2017]),
                # "higgsh430": make_obj([ basedirs[20172]+"Higgs_ttH_425.root", basedirs[20172]+"Higgs_tHW_425.root", basedirs[20172]+"Higgs_tHq_425.root", ],options=options[2017]),
                # "higgsh450": make_obj([ basedirs[20172]+"Higgs_ttH_450.root", basedirs[20172]+"Higgs_tHW_450.root", basedirs[20172]+"Higgs_tHq_450.root", ],options=options[2017]),
                # "higgsh470": make_obj([ basedirs[20172]+"Higgs_ttH_475.root", basedirs[20172]+"Higgs_tHW_475.root", basedirs[20172]+"Higgs_tHq_475.root", ],options=options[2017]),
                # "higgsh490": make_obj([ basedirs[20172]+"Higgs_ttH_475.root", basedirs[20172]+"Higgs_tHW_500.root", basedirs[20172]+"Higgs_tHq_500.root", ],options=options[2017]),
                # "higgsh510": make_obj([ basedirs[20172]+"Higgs_ttH_475.root", basedirs[20172]+"Higgs_tHW_500.root", basedirs[20172]+"Higgs_tHq_500.root", ],options=options[2017]),
                # "higgsh530": make_obj([ basedirs[20172]+"Higgs_ttH_525.root", basedirs[20172]+"Higgs_tHW_525.root", basedirs[20172]+"Higgs_tHq_525.root", ],options=options[2017]),
                # "higgsh550": make_obj([ basedirs[20172]+"Higgs_ttH_550.root", basedirs[20172]+"Higgs_tHW_550.root", basedirs[20172]+"Higgs_tHq_550.root", ],options=options[2017]),
                # "higgsh570": make_obj([ basedirs[20172]+"Higgs_ttH_575.root", basedirs[20172]+"Higgs_tHW_575.root", basedirs[20172]+"Higgs_tHq_575.root", ],options=options[2017]),
                # "higgsh590": make_obj([ basedirs[20172]+"Higgs_ttH_600.root", basedirs[20172]+"Higgs_tHW_600.root", basedirs[20172]+"Higgs_tHq_600.root", ],options=options[2017]),
                # "higgsh610": make_obj([ basedirs[20172]+"Higgs_ttH_600.root", basedirs[20172]+"Higgs_tHW_600.root", basedirs[20172]+"Higgs_tHq_600.root", ],options=options[2017]),
                # "higgsh630": make_obj([ basedirs[20172]+"Higgs_ttH_625.root", basedirs[20172]+"Higgs_tHW_625.root", basedirs[20172]+"Higgs_tHq_625.root", ],options=options[2017]),
                # "higgsh650": make_obj([ basedirs[20172]+"Higgs_ttH_650.root", basedirs[20172]+"Higgs_tHW_650.root", basedirs[20172]+"Higgs_tHq_650.root", ],options=options[2017]),

                # "higgsa350": make_obj([ basedirs[20172]+"Higgs_ttH_350.root", basedirs[20172]+"Higgs_tHW_350.root", basedirs[20172]+"Higgs_tHq_350.root", ],options=options[2017]),
                # "higgsa370": make_obj([ basedirs[20172]+"Higgs_ttH_375.root", basedirs[20172]+"Higgs_tHW_375.root", basedirs[20172]+"Higgs_tHq_375.root", ],options=options[2017]),
                # "higgsa390": make_obj([ basedirs[20172]+"Higgs_ttH_400.root", basedirs[20172]+"Higgs_tHW_400.root", basedirs[20172]+"Higgs_tHq_400.root", ],options=options[2017]),
                # "higgsa410": make_obj([ basedirs[20172]+"Higgs_ttH_400.root", basedirs[20172]+"Higgs_tHW_400.root", basedirs[20172]+"Higgs_tHq_400.root", ],options=options[2017]),
                # "higgsa430": make_obj([ basedirs[20172]+"Higgs_ttH_425.root", basedirs[20172]+"Higgs_tHW_425.root", basedirs[20172]+"Higgs_tHq_425.root", ],options=options[2017]),
                # "higgsa450": make_obj([ basedirs[20172]+"Higgs_ttH_450.root", basedirs[20172]+"Higgs_tHW_450.root", basedirs[20172]+"Higgs_tHq_450.root", ],options=options[2017]),
                # "higgsa470": make_obj([ basedirs[20172]+"Higgs_ttH_475.root", basedirs[20172]+"Higgs_tHW_475.root", basedirs[20172]+"Higgs_tHq_475.root", ],options=options[2017]),
                # "higgsa490": make_obj([ basedirs[20172]+"Higgs_ttH_475.root", basedirs[20172]+"Higgs_tHW_500.root", basedirs[20172]+"Higgs_tHq_500.root", ],options=options[2017]),
                # "higgsa510": make_obj([ basedirs[20172]+"Higgs_ttH_475.root", basedirs[20172]+"Higgs_tHW_500.root", basedirs[20172]+"Higgs_tHq_500.root", ],options=options[2017]),
                # "higgsa530": make_obj([ basedirs[20172]+"Higgs_ttH_525.root", basedirs[20172]+"Higgs_tHW_525.root", basedirs[20172]+"Higgs_tHq_525.root", ],options=options[2017]),
                # "higgsa550": make_obj([ basedirs[20172]+"Higgs_ttH_550.root", basedirs[20172]+"Higgs_tHW_550.root", basedirs[20172]+"Higgs_tHq_550.root", ],options=options[2017]),
                # "higgsa570": make_obj([ basedirs[20172]+"Higgs_ttH_575.root", basedirs[20172]+"Higgs_tHW_575.root", basedirs[20172]+"Higgs_tHq_575.root", ],options=options[2017]),
                # "higgsa590": make_obj([ basedirs[20172]+"Higgs_ttH_600.root", basedirs[20172]+"Higgs_tHW_600.root", basedirs[20172]+"Higgs_tHq_600.root", ],options=options[2017]),
                # "higgsa610": make_obj([ basedirs[20172]+"Higgs_ttH_600.root", basedirs[20172]+"Higgs_tHW_600.root", basedirs[20172]+"Higgs_tHq_600.root", ],options=options[2017]),
                # "higgsa630": make_obj([ basedirs[20172]+"Higgs_ttH_625.root", basedirs[20172]+"Higgs_tHW_625.root", basedirs[20172]+"Higgs_tHq_625.root", ],options=options[2017]),
                # "higgsa650": make_obj([ basedirs[20172]+"Higgs_ttH_650.root", basedirs[20172]+"Higgs_tHW_650.root", basedirs[20172]+"Higgs_tHq_650.root", ],options=options[2017]),

                },
            2018: {

                # 2017 for everything other than data for now, because...uh...there's no usable 2018 MC, even though it's almost 2019 :)
                "fakes": make_obj([
                    basedirs[2018]+"Data*.root",
                    basedirs[2018]+"TTWnlo.root",
                    basedirs[2018]+"TTZnlo.root",
                    basedirs[2018]+"TTHtoNonBB.root",
                    ] , options=options[2018]+" doFakes doData "),
                "fakes_app": make_obj([
                    basedirs[2018]+"Data*.root",
                    # basedirs[2017]+"TTWnlo.root",
                    # basedirs[2017]+"TTZnlo.root",
                    # basedirs[2017]+"TTHtoNonBB.root",
                    ] , options=options[2018]+" doFakesUnw doData "),
                "flips": make_obj(basedirs[2018]+"Data*.root", options=options[2018]+" doFlips "),
                "data": make_obj(basedirs[2018]+"Data*.root", options=options[2018]+" doData "),
                "tttt": make_obj(basedirs[2018]+"TTTTnew.root", options=options[2018]),
                # "fakes_mc": make_obj(basedirs[2017]+"TTBAR*.root", options=options[2018]+ " doFakesMC "),
                "fakes_mc": make_obj([
                    basedirs[2018]+"TTDL.root",
                    basedirs[2018]+"TTSLtop.root",
                    basedirs[2018]+"TTSLtopbar.root",
                    ] , options=options[2018]+ " doTruthFake "),
                "fakes_mchybrid": make_obj([
                    basedirs[2018]+"TTDL.root",
                    basedirs[2018]+"TTSLtop.root",
                    basedirs[2018]+"TTSLtopbar.root",
                    ] , options=options[2018]+ " doFakesMC "),
                "fakes_mchybrid_app": make_obj([
                    basedirs[2018]+"TTDL.root",
                    basedirs[2018]+"TTSLtop.root",
                    basedirs[2018]+"TTSLtopbar.root",
                    ] , options=options[2018]+ " doFakesMCUnw "),
                "ttw": make_obj(basedirs[2018]+"TTWnlo.root", options=options[2018]),
                "tth": make_obj(basedirs[2018]+"TTHtoNonBB.root", options=options[2018]),
                "ttz": make_obj([
                    basedirs[2018]+"TTZnlo.root",
                    basedirs[2018]+"TTZLOW.root",
                    ] , options=options[2018]),
                "xg": make_obj([
                    basedirs[2018]+"TGext.root",
                    basedirs[2018]+"TTGdilep.root",
                    basedirs[2018]+"TTGsinglelepbar.root",
                    basedirs[2018]+"TTGsinglelep.root",
                    basedirs[2018]+"WGToLNuGext.root",
                    basedirs[2018]+"ZG.root",
                    ],options=options[2018] + " doXgamma "),
                "ttvv": make_obj([
                    basedirs[2018]+"TTHH.root",
                    basedirs[2018]+"TTWH.root",
                    basedirs[2018]+"TTWW.root",
                    basedirs[2018]+"TTWZ.root",
                    basedirs[2018]+"TTZZ.root",
                    ],options=options[2018]),
                "rares": make_obj([
                    basedirs[2018]+"GGHtoZZto4L.root",
                    basedirs[2018]+"QQWW.root",
                    basedirs[2018]+"TWZ.root",
                    # basedirs[2018]+"TZQ.root",
                    basedirs[2018]+"VHtoNonBB.root",
                    basedirs[2018]+"WWDPS.root",
                    basedirs[2018]+"WWW.root",
                    basedirs[2018]+"WWZ.root",
                    basedirs[2018]+"WZ.root",
                    basedirs[2018]+"WZG.root",
                    basedirs[2018]+"WGToLNuGext.root",
                    basedirs[2018]+"WZZ.root",
                    basedirs[2018]+"ZZ.root",
                    basedirs[2018]+"ZZZ.root",
                    basedirs[2018]+"TTTJ.root",
                    basedirs[2018]+"TTTW.root",
                    ],options=options[2018]),

                # "higgsh350": make_obj([ basedirs[20172]+"Higgs_ttH_350.root", basedirs[20172]+"Higgs_tHW_350.root", basedirs[20172]+"Higgs_tHq_350.root", ],options=options[2018]),
                # "higgsh370": make_obj([ basedirs[20172]+"Higgs_ttH_375.root", basedirs[20172]+"Higgs_tHW_375.root", basedirs[20172]+"Higgs_tHq_375.root", ],options=options[2018]),
                # "higgsh390": make_obj([ basedirs[20172]+"Higgs_ttH_400.root", basedirs[20172]+"Higgs_tHW_400.root", basedirs[20172]+"Higgs_tHq_400.root", ],options=options[2018]),
                # "higgsh410": make_obj([ basedirs[20172]+"Higgs_ttH_400.root", basedirs[20172]+"Higgs_tHW_400.root", basedirs[20172]+"Higgs_tHq_400.root", ],options=options[2018]),
                # "higgsh430": make_obj([ basedirs[20172]+"Higgs_ttH_425.root", basedirs[20172]+"Higgs_tHW_425.root", basedirs[20172]+"Higgs_tHq_425.root", ],options=options[2018]),
                # "higgsh450": make_obj([ basedirs[20172]+"Higgs_ttH_450.root", basedirs[20172]+"Higgs_tHW_450.root", basedirs[20172]+"Higgs_tHq_450.root", ],options=options[2018]),
                # "higgsh470": make_obj([ basedirs[20172]+"Higgs_ttH_475.root", basedirs[20172]+"Higgs_tHW_475.root", basedirs[20172]+"Higgs_tHq_475.root", ],options=options[2018]),
                # "higgsh490": make_obj([ basedirs[20172]+"Higgs_ttH_475.root", basedirs[20172]+"Higgs_tHW_500.root", basedirs[20172]+"Higgs_tHq_500.root", ],options=options[2018]),
                # "higgsh510": make_obj([ basedirs[20172]+"Higgs_ttH_475.root", basedirs[20172]+"Higgs_tHW_500.root", basedirs[20172]+"Higgs_tHq_500.root", ],options=options[2018]),
                # "higgsh530": make_obj([ basedirs[20172]+"Higgs_ttH_525.root", basedirs[20172]+"Higgs_tHW_525.root", basedirs[20172]+"Higgs_tHq_525.root", ],options=options[2018]),
                # "higgsh550": make_obj([ basedirs[20172]+"Higgs_ttH_550.root", basedirs[20172]+"Higgs_tHW_550.root", basedirs[20172]+"Higgs_tHq_550.root", ],options=options[2018]),
                # "higgsh570": make_obj([ basedirs[20172]+"Higgs_ttH_575.root", basedirs[20172]+"Higgs_tHW_575.root", basedirs[20172]+"Higgs_tHq_575.root", ],options=options[2018]),
                # "higgsh590": make_obj([ basedirs[20172]+"Higgs_ttH_600.root", basedirs[20172]+"Higgs_tHW_600.root", basedirs[20172]+"Higgs_tHq_600.root", ],options=options[2018]),
                # "higgsh610": make_obj([ basedirs[20172]+"Higgs_ttH_600.root", basedirs[20172]+"Higgs_tHW_600.root", basedirs[20172]+"Higgs_tHq_600.root", ],options=options[2018]),
                # "higgsh630": make_obj([ basedirs[20172]+"Higgs_ttH_625.root", basedirs[20172]+"Higgs_tHW_625.root", basedirs[20172]+"Higgs_tHq_625.root", ],options=options[2018]),
                # "higgsh650": make_obj([ basedirs[20172]+"Higgs_ttH_650.root", basedirs[20172]+"Higgs_tHW_650.root", basedirs[20172]+"Higgs_tHq_650.root", ],options=options[2018]),

                # "higgsa350": make_obj([ basedirs[20172]+"Higgs_ttH_350.root", basedirs[20172]+"Higgs_tHW_350.root", basedirs[20172]+"Higgs_tHq_350.root", ],options=options[2018]),
                # "higgsa370": make_obj([ basedirs[20172]+"Higgs_ttH_375.root", basedirs[20172]+"Higgs_tHW_375.root", basedirs[20172]+"Higgs_tHq_375.root", ],options=options[2018]),
                # "higgsa390": make_obj([ basedirs[20172]+"Higgs_ttH_400.root", basedirs[20172]+"Higgs_tHW_400.root", basedirs[20172]+"Higgs_tHq_400.root", ],options=options[2018]),
                # "higgsa410": make_obj([ basedirs[20172]+"Higgs_ttH_400.root", basedirs[20172]+"Higgs_tHW_400.root", basedirs[20172]+"Higgs_tHq_400.root", ],options=options[2018]),
                # "higgsa430": make_obj([ basedirs[20172]+"Higgs_ttH_425.root", basedirs[20172]+"Higgs_tHW_425.root", basedirs[20172]+"Higgs_tHq_425.root", ],options=options[2018]),
                # "higgsa450": make_obj([ basedirs[20172]+"Higgs_ttH_450.root", basedirs[20172]+"Higgs_tHW_450.root", basedirs[20172]+"Higgs_tHq_450.root", ],options=options[2018]),
                # "higgsa470": make_obj([ basedirs[20172]+"Higgs_ttH_475.root", basedirs[20172]+"Higgs_tHW_475.root", basedirs[20172]+"Higgs_tHq_475.root", ],options=options[2018]),
                # "higgsa490": make_obj([ basedirs[20172]+"Higgs_ttH_475.root", basedirs[20172]+"Higgs_tHW_500.root", basedirs[20172]+"Higgs_tHq_500.root", ],options=options[2018]),
                # "higgsa510": make_obj([ basedirs[20172]+"Higgs_ttH_475.root", basedirs[20172]+"Higgs_tHW_500.root", basedirs[20172]+"Higgs_tHq_500.root", ],options=options[2018]),
                # "higgsa530": make_obj([ basedirs[20172]+"Higgs_ttH_525.root", basedirs[20172]+"Higgs_tHW_525.root", basedirs[20172]+"Higgs_tHq_525.root", ],options=options[2018]),
                # "higgsa550": make_obj([ basedirs[20172]+"Higgs_ttH_550.root", basedirs[20172]+"Higgs_tHW_550.root", basedirs[20172]+"Higgs_tHq_550.root", ],options=options[2018]),
                # "higgsa570": make_obj([ basedirs[20172]+"Higgs_ttH_575.root", basedirs[20172]+"Higgs_tHW_575.root", basedirs[20172]+"Higgs_tHq_575.root", ],options=options[2018]),
                # "higgsa590": make_obj([ basedirs[20172]+"Higgs_ttH_600.root", basedirs[20172]+"Higgs_tHW_600.root", basedirs[20172]+"Higgs_tHq_600.root", ],options=options[2018]),
                # "higgsa610": make_obj([ basedirs[20172]+"Higgs_ttH_600.root", basedirs[20172]+"Higgs_tHW_600.root", basedirs[20172]+"Higgs_tHq_600.root", ],options=options[2018]),
                # "higgsa630": make_obj([ basedirs[20172]+"Higgs_ttH_625.root", basedirs[20172]+"Higgs_tHW_625.root", basedirs[20172]+"Higgs_tHq_625.root", ],options=options[2018]),
                # "higgsa650": make_obj([ basedirs[20172]+"Higgs_ttH_650.root", basedirs[20172]+"Higgs_tHW_650.root", basedirs[20172]+"Higgs_tHq_650.root", ],options=options[2018]),

                }
            }

    def get_fastsim_procnames(fname, procbase="fs_t1tttt"):
        f = r.TFile(fname)
        counts = f.Get("counts")
        xaxis = counts.GetXaxis()
        yaxis = counts.GetYaxis()
        valid_points = []
        for ix in range(1,counts.GetNbinsX()+1):
            for iy in range(1,counts.GetNbinsY()+1):
                v = counts.GetBinContent(ix,iy)
                if v < 100: continue
                ex = xaxis.GetBinLowEdge(ix)
                ey = yaxis.GetBinLowEdge(iy)
                valid_points.append([int(ex),int(max(ey,1))])
        strs = map(lambda x:"{}_m{}_m{}".format(procbase,x[0],x[1]), valid_points)
        return strs

    fastsim_procnames = []
    if args.fastsim:
        fn = basedirs[2016]+"T1TTTT.root"
        procnames = get_fastsim_procnames(fn, procbase="fs_t1tttt")
        procnames = [pn for pn in procnames if any(x in pn for x in [
            # "m1600_m100",
            # "m1550_m100",
            # "m1450_m1050",
            # "m1400_m1000",
            # "m1400_m1200",
            # "m1500_m1150",
            # "m1200_m700",
            # "m1200_m100",
            ])]
        for pn in procnames: 
            chs[2016][pn] = make_obj(fn, options=options[2016])
            fastsim_procnames.append(pn)

        fn = basedirs[2016]+"T6TTWW.root"
        procnames = get_fastsim_procnames(fn, procbase="fs_t6ttww")
        procnames = [pn for pn in procnames if any(x in pn for x in [
        #     "m850_m75",
            # "m850_m750",
            "m850_m700",
            ])]
        for pn in procnames:
            chs[2016][pn] = make_obj(fn, options=options[2016])
            fastsim_procnames.append(pn)

    def run_chain((index,info)):
        ch, options, outputdir = info
        t0 = time.time()
        ret = r.getyields(ch, options, outputdir)
        t1 = time.time()
        return index, [ret, ch.GetTitle(), t1-t0]

    to_run = []
    for year in years_to_consider:
        if (args.year) and (year != args.year): continue
        for proc,obj in chs[year].items():
            # if (len(args.proc) > 0) and (proc != args.proc): continue
            if (len(args.proc) > 0) and not fnmatch.fnmatch(proc,args.proc): continue
            options = obj["options"]
            if args.verbosity >= 1:
                options = options.replace("quiet","")
            if args.year and args.proc and "*" not in args.proc:  # if one process, show the progress bar
                options = options.replace("quiet","")
            # Change chain titles to proc so that we output the right root file name
            obj["ch"].SetTitle("{}".format(proc))
            to_run.append([obj["ch"], options, outputdir])
            if args.verbosity >= 2:
                print "Adding:", obj["ch"].GetTitle(), options, outputdir

    os.system("mkdir -p {}".format(outputdir))

    # Make sure all the requested root files actually exist
    for ch,opts,_ in to_run:
        for fname in [str(x.GetTitle()) for x in ch.GetListOfFiles()]:
            if not os.path.exists(fname):
                print "[!] {} does not exist!".format(fname)

    # to_run = [t for t in to_run if "doXgamma" in t[1]]
    # print to_run

    # to_run = [t for t in to_run if ("doData " in t[1] and "2016" in t[1] and "Fake" not in t[1] and "Flip" not in t[1])]
    # to_run[0][1] = to_run[0][1].replace("quiet","")
    # print to_run
    # run_chain((0,to_run[0]))

    # to_run = [t for t in to_run if ("doFakes " in t[1] and "2016" in t[1])]
    # to_run[0][1] = to_run[0][1].replace("quiet","")
    # print to_run
    # run_chain((0,to_run[0]))

    procnames = list(set([x[0].GetTitle() for x in to_run]))
    higgs_procnames = [pn for pn in procnames if "higgs" in pn]
    

    if not args.noloop:

        def callback(ret):
            pass
        # if len(to_run) < 30:
        #     def callback(ret):
        #         ret, proc, t = ret
        #         print "Processed {} in {:.1f} seconds".format(proc,t)
        # else:
        #     def callback(ret):
        #         pass

        # Now run them
        if len(to_run) == 1:
            print "Running one chain"
            map(run_chain,enumerate(to_run))
            print "Done"
        else:
            runner = pyrun.Runner(nproc=min(25,len(to_run)), func=run_chain, dot_type=3)
            runner.add_args(to_run)
            runner.run(print_callback=callback)

    if args.tag:
        outdir_limits = "../limits/{}/".format(args.tag)
        os.system("mkdir -p {}".format(outdir_limits))
        os.system("cp ../misc/signal_regions.h {}/".format(outdir_limits))
        os.system("cp ../misc/bdt.h {}/".format(outdir_limits))
        os.system("cp yieldMaker.C make_shape_hists.py py_doAll.py plot_all.py {}/".format(outdir_limits))

        if args.shapes:
            import make_shape_hists
            regions = ["SRCR","SRDISC"] if not "noBDT" in extra_global_options else ["SRCR"]
            make_shape_hists.make_root_files(
                    outputdir=outdir_limits,
                    inputdir=args.out,
                    extra_procs=fastsim_procnames+higgs_procnames,
                    regions=regions,
                    verbose=(args.verbosity >= 2)
                    )

        if args.plots:
            # numba njit causes segfault when pyroot has been imported (more specifically, once we've gotten a root MethodProxy somehow)
            # so prevent uproot from importing it (which prevents the njit)
            # https://github.com/scikit-hep/uproot/issues/58
            # https://stackoverflow.com/questions/1350466/preventing-python-code-from-importing-certain-modules
            import sys
            sys.modules["numba"] = None
            import plot_all

            plot_all.make_plots(
                    outputdir="{}/plots/".format(outdir_limits),
                    inputdir=args.out,
                    year=2016,
                    lumi="35.9",
                    other_years = []
                    )

            plot_all.make_plots(
                    outputdir="{}/plots/".format(outdir_limits),
                    inputdir=args.out,
                    year=2017,
                    lumi="41.5",
                    other_years = []
                    )

            plot_all.make_plots(
                    outputdir="{}/plots/".format(outdir_limits),
                    inputdir=args.out,
                    year=2018,
                    lumi="58.8",
                    other_years = [],
                    )

            plot_all.make_plots(
                    outputdir="{}/plots/".format(outdir_limits),
                    inputdir=args.out,
                    year=2018,
                    lumi="136.3", # 2016+2017+2018 --> 35.922+41.53+58.83 = 136.3
                    other_years = [2016,2017],
                    )
