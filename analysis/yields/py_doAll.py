#!/usr/bin/env python

import os
import analysis.utils.pyrun as pyrun
import argparse

def make_objs(fpatts=[],options="",treename="t"):
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

    parser.add_argument(      "--year", help="year, if you only want to run one", default=-1, type=int)
    parser.add_argument(      "--proc", help="process, if you only want to run one", default="", type=str)

    parser.add_argument("-n", "--noloop", help="skip looping/scanchain", action="store_true")
    parser.add_argument("-s", "--shapes", help="make shape hists and copy to limit directory tag folder", action="store_true")
    parser.add_argument("-p", "--plots", help="make plots and copy to the limit directory tag folder", action="store_true")

    args = parser.parse_args()

    import ROOT as r
    r.gROOT.SetBatch()

    r.gROOT.ProcessLine(".L ../misc/class_files/v8.02/SS.cc+")
    r.gROOT.ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc+")
    r.gROOT.ProcessLine(".L yieldMaker.C+")


    years_to_consider = [
            2016,
            2017,
            2018,
            ]

    basedirs ={
            # 2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2016/",
            # 2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2017/",
            # 2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.08_all/output/year_2018/",
            2016: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_all/output/year_2016/",
            2017: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_all/output/year_2017/",
            2018: "/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.09_all/output/year_2018/",
            }

    outputdir = args.out
    extra_global_options = args.extra_options
    options = {
            2016: "Data2016 quiet {} evaluateBDT ".format(extra_global_options),
            2017: "Data2017 quiet {} evaluateBDT minPtFake18 ".format(extra_global_options),
            2018: "Data2018 quiet {} evaluateBDT minPtFake18 ".format(extra_global_options),
            }


    chs = {
            2016: {

                "fakes": make_objs([
                    basedirs[2016]+"Data*.root",
                    basedirs[2016]+"TTWnlo.root",
                    basedirs[2016]+"TTZnlo.root",
                    basedirs[2016]+"TTHtoNonBB.root",
                    ] , options=options[2016]+" doFakes doData "),
                "flips": make_objs(basedirs[2016]+"Data*.root", options=options[2016]+" doFlips doData "),
                "data": make_objs(basedirs[2016]+"Data*.root", options=options[2016] + " doData "),
                "tttt": make_objs(basedirs[2016]+"TTTTnew.root", options=options[2016]),
                "ttttisrup": make_objs(basedirs[2016]+"TTTTisrup.root", options=options[2016]),
                "ttttisrdn": make_objs(basedirs[2016]+"TTTTisrdown.root", options=options[2016]),
                "ttttfsrup": make_objs(basedirs[2016]+"TTTTfsrup.root", options=options[2016]),
                "ttttfsrdn": make_objs(basedirs[2016]+"TTTTfsrdown.root", options=options[2016]),
                # "fakes_mc": make_objs(basedirs[2016]+"TTBAR*.root", options=options[2016]+ " doFakesMC "),
                "fakes_mc": make_objs([
                    basedirs[2016]+"TTDL.root",
                    basedirs[2016]+"TTSLtop.root",
                    basedirs[2016]+"TTSLtopbar.root",
                    ] , options=options[2016]+ " doTruthFake "),
                "fakes_mchybrid": make_objs([
                    basedirs[2016]+"TTDL.root",
                    basedirs[2016]+"TTSLtop.root",
                    basedirs[2016]+"TTSLtopbar.root",
                    basedirs[2016]+"TTWnlo.root",
                    basedirs[2016]+"TTZnlo.root",
                    basedirs[2016]+"TTHtoNonBB.root",
                    ] , options=options[2016]+ " doFakesMC "),
                "ttw": make_objs(basedirs[2016]+"TTWnlo.root", options=options[2016]),
                "tth": make_objs(basedirs[2016]+"TTHtoNonBB.root", options=options[2016]),
                "ttz": make_objs([
                    basedirs[2016]+"TTZnlo.root",
                    basedirs[2016]+"TTZLOW.root",
                    ] , options=options[2016]),
                "xg": make_objs([
                    basedirs[2016]+"TGext.root",
                    basedirs[2016]+"TTGdilep.root",
                    basedirs[2016]+"TTGsinglelepbar.root",
                    basedirs[2016]+"TTGsinglelep.root",
                    basedirs[2016]+"WGToLNuGext.root",
                    basedirs[2016]+"ZG.root",
                    ],options=options[2016] + " doXgamma "),
                "ttvv": make_objs([
                    basedirs[2016]+"TTHH.root",
                    basedirs[2016]+"TTWH.root",
                    basedirs[2016]+"TTWW.root",
                    basedirs[2016]+"TTWZ.root",
                    basedirs[2016]+"TTZZ.root",
                    ],options=options[2016]),
                "rares": make_objs([
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
                    basedirs[2016]+"WZZ.root",
                    basedirs[2016]+"ZZ.root",
                    basedirs[2016]+"ZZZ.root",
                    basedirs[2016]+"TTTJ.root",
                    basedirs[2016]+"TTTW.root",
                    ],options=options[2016]),

                },
            2017: {

                "fakes": make_objs([
                    basedirs[2017]+"Data*.root",
                    basedirs[2017]+"TTWnlo.root",
                    basedirs[2017]+"TTZnlo.root",
                    basedirs[2017]+"TTHtoNonBB.root",
                    ] , options=options[2017]+" doFakes doData "),
                "flips": make_objs(basedirs[2017]+"Data*.root", options=options[2017]+" doFlips "),
                "data": make_objs(basedirs[2017]+"Data*.root", options=options[2017]+" doData "),
                "tttt": make_objs(basedirs[2017]+"TTTTnew.root", options=options[2017]),
                # "fakes_mc": make_objs(basedirs[2017]+"TTBAR*.root", options=options[2017]+ " doFakesMC "),
                "fakes_mc": make_objs([
                    basedirs[2017]+"TTDL.root",
                    basedirs[2017]+"TTSLtop.root",
                    basedirs[2017]+"TTSLtopbar.root",
                    ] , options=options[2017]+ " doTruthFake "),
                "fakes_mchybrid": make_objs([
                    basedirs[2017]+"TTDL.root",
                    basedirs[2017]+"TTSLtop.root",
                    basedirs[2017]+"TTSLtopbar.root",
                    basedirs[2017]+"TTWnlo.root",
                    basedirs[2017]+"TTZnlo.root",
                    basedirs[2017]+"TTHtoNonBB.root",
                    ] , options=options[2017]+ " doFakesMC "),
                "ttw": make_objs(basedirs[2017]+"TTWnlo.root", options=options[2017]),
                "tth": make_objs(basedirs[2017]+"TTHtoNonBB.root", options=options[2017]),
                "ttz": make_objs([
                    basedirs[2017]+"TTZnlo.root",
                    basedirs[2017]+"TTZLOW.root",
                    ] , options=options[2017]),
                "xg": make_objs([
                    basedirs[2017]+"TGext.root",
                    # basedirs[2017]+"TTGdilep.root", # TODO Don't have this yet
                    basedirs[2017]+"TTGsinglelepbar.root",
                    basedirs[2017]+"TTGsinglelep.root",
                    # basedirs[2017]+"WGToLNuGext.root", # TODO Don't have this yet
                    # basedirs[2017]+"ZG.root", # TODO Don't have this yet
                    ],options=options[2017] + " doXgamma "),
                "ttvv": make_objs([
                    basedirs[2017]+"TTHH.root",
                    basedirs[2017]+"TTWH.root",
                    basedirs[2017]+"TTWW.root",
                    basedirs[2017]+"TTWZ.root",
                    basedirs[2017]+"TTZZ.root",
                    ],options=options[2017]),
                "rares": make_objs([
                    # basedirs[2017]+"GGHtoZZto4L.root", # TODO Don't have this sample yet
                    basedirs[2017]+"QQWW.root",
                    # basedirs[2017]+"TWZ.root", # TODO Don't have this yet
                    basedirs[2017]+"TZQ.root",
                    basedirs[2017]+"VHtoNonBB.root",
                    basedirs[2017]+"WWDPS.root",
                    basedirs[2017]+"WWW.root",
                    basedirs[2017]+"WWZ.root",
                    basedirs[2017]+"WZ.root",
                    basedirs[2017]+"WZG.root",
                    basedirs[2017]+"WZZ.root",
                    basedirs[2017]+"ZZ.root",
                    basedirs[2017]+"ZZZ.root",
                    basedirs[2017]+"TTTJ.root",
                    basedirs[2017]+"TTTW.root",
                    ],options=options[2017]),

                },
            2018: {

                # 2017 for everything other than data for now, because...uh...there's no usable 2018 MC, even though it's almost 2019 :)
                "fakes": make_objs([
                    basedirs[2018]+"Data*.root",
                    basedirs[2017]+"TTWnlo.root",
                    basedirs[2017]+"TTZnlo.root",
                    basedirs[2017]+"TTHtoNonBB.root",
                    ] , options=options[2018]+" doFakes doData "),
                "flips": make_objs(basedirs[2018]+"Data*.root", options=options[2018]+" doFlips "),
                "data": make_objs(basedirs[2018]+"Data*.root", options=options[2018]+" doData "),
                "tttt": make_objs(basedirs[2017]+"TTTTnew.root", options=options[2018]),
                # "fakes_mc": make_objs(basedirs[2017]+"TTBAR*.root", options=options[2018]+ " doFakesMC "),
                "fakes_mc": make_objs([
                    basedirs[2017]+"TTDL.root",
                    basedirs[2017]+"TTSLtop.root",
                    basedirs[2017]+"TTSLtopbar.root",
                    ] , options=options[2018]+ " doTruthFake "),
                "fakes_mchybrid": make_objs([
                    basedirs[2017]+"TTDL.root",
                    basedirs[2017]+"TTSLtop.root",
                    basedirs[2017]+"TTSLtopbar.root",
                    basedirs[2017]+"TTWnlo.root",
                    basedirs[2017]+"TTZnlo.root",
                    basedirs[2017]+"TTHtoNonBB.root",
                    ] , options=options[2018]+ " doFakesMC "),
                "ttw": make_objs(basedirs[2017]+"TTWnlo.root", options=options[2018]),
                "tth": make_objs(basedirs[2017]+"TTHtoNonBB.root", options=options[2018]),
                "ttz": make_objs([
                    basedirs[2017]+"TTZnlo.root",
                    basedirs[2017]+"TTZLOW.root",
                    ] , options=options[2018]),
                "xg": make_objs([
                    basedirs[2017]+"TGext.root",
                    # basedirs[2017]+"TTGdilep.root", # TODO Don't have this yet
                    basedirs[2017]+"TTGsinglelepbar.root",
                    basedirs[2017]+"TTGsinglelep.root",
                    # basedirs[2017]+"WGToLNuGext.root", # TODO Don't have this yet
                    # basedirs[2017]+"ZG.root", # TODO Don't have this yet
                    ],options=options[2018] + " doXgamma "),
                "ttvv": make_objs([
                    basedirs[2017]+"TTHH.root",
                    basedirs[2017]+"TTWH.root",
                    basedirs[2017]+"TTWW.root",
                    basedirs[2017]+"TTWZ.root",
                    basedirs[2017]+"TTZZ.root",
                    ],options=options[2018]),
                "rares": make_objs([
                    # basedirs[2017]+"GGHtoZZto4L.root", # TODO Don't have this sample yet
                    basedirs[2017]+"QQWW.root",
                    # basedirs[2017]+"TWZ.root", # TODO Don't have this yet
                    basedirs[2017]+"TZQ.root",
                    basedirs[2017]+"VHtoNonBB.root",
                    basedirs[2017]+"WWDPS.root",
                    basedirs[2017]+"WWW.root",
                    basedirs[2017]+"WWZ.root",
                    basedirs[2017]+"WZ.root",
                    basedirs[2017]+"WZG.root",
                    basedirs[2017]+"WZZ.root",
                    basedirs[2017]+"ZZ.root",
                    basedirs[2017]+"ZZZ.root",
                    basedirs[2017]+"TTTJ.root",
                    basedirs[2017]+"TTTW.root",
                    ],options=options[2018]),

                }
            }

    # Change chain titles to proc so that we output the right root file name
    for year in chs:
        [obj["ch"].SetTitle("{}".format(proc)) for proc,obj in chs[year].items()]

    def run_chain((index,info)):
        ch, options, outputdir = info
        ret = r.getyields(ch, options, outputdir)
        return index, ret

    to_run = []
    for year in years_to_consider:
        if (args.year > 0) and (year != args.year): continue
        for proc,obj in chs[year].items():
            if (len(args.proc) > 0) and (proc != args.proc): continue
            options = obj["options"]
            if (args.year > 0) and len(args.proc):  # if one process, show the progress bar
                options = options.replace("quiet","")
            to_run.append([obj["ch"], options, outputdir])

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

    # print to_run

    if not args.noloop:
        # Now run them
        runner = pyrun.Runner(nproc=20, func=run_chain)
        runner.add_args(to_run)
        runner.run()

    if args.tag:
        outdir_limits = "../limits/{}/".format(args.tag)
        os.system("mkdir -p {}".format(outdir_limits))

        if args.shapes:
            import make_shape_hists
            make_shape_hists.make_root_files(
                    outputdir=outdir_limits,
                    inputdir=args.out,
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
                    year=2018,
                    lumi="112.9", # 2016+2017+2018 --> 35.87+41.53+35.53 = 112.9
                    other_years = [2016,2017],
                    )

