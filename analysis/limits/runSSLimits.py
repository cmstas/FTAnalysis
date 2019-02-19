#!/usr/bin/env python

import sys
import os
import argparse
import createCard
import commands
import subprocess

# from createCard import writeOneCard
# from makeSingleCombinedCard import do_combine
import makeSingleCombinedCard

def make_card(basedir,sig,finalcard,verbose=True):

    # # CLEAN before rerunning. Once per SCAN, not per MASS POINT
    # os.system("rm {}/*run2*.root".format(basedir))

    cnames = []
    for kine in ["srhh","srhl","srll","srml","srlm"]:

        # Make cards per year
        outname_pattern = "card_{signal}_{kine}_{year}.txt"
        for year in [2016,2017,2018]:
            outname =outname_pattern.format(signal=sig,kine=kine,year=year)
            createCard.writeOneCard(basedir, outname, year=year, analysis="ss", kine=kine, signal=sig)
            if verbose: print "Wrote {}/{}".format(basedir,outname)

        # Combine the 3 years into one card
        cardname_per_kine = "card_{signal}_{kine}_run2.txt".format(signal=sig,kine=kine)
        cnames.append(cardname_per_kine)
        makeSingleCombinedCard.do_combine(
                basedir,
                regions=kine,
                inputcard_pattern = outname_pattern.replace("{year}","{{year}}").format(signal=sig,kine=kine), # all but year in pattern
                cardname=cardname_per_kine,
                signal = sig,
                verbose = verbose,
                skiptext2workspace = True,
                # autothresh = 3.0,
                to_correlate = [
                        "scale",
                        "pdf",
                        "TTH",
                        "TTWSF",
                        "TTZSF",
                        "WZSF", # NOTE
                        "WW", # NOTE
                        "fakes",
                        "rares",
                        "XG",
                        "TTVV",
                        ],
                )

    # Combine the cards per kinematic region into one...so one card per signal point
    # finalcard = "card_{signal}_all_run2.txt".format(signal=sig)
    cmd = "( cd {} ; combineCards.py {} > {} )".format(basedir, " ".join(cnames), finalcard)
    if verbose: print cmd
    os.system(cmd)
    fullfinalcard = "{}/{}".format(basedir,finalcard)
    if verbose: print "Wrote {}".format(fullfinalcard)
    return fullfinalcard


def parse_lims(lim_lines):
    """
    return dictionary with obs, exp, sp1, sm1 cross section limits in pb
    input xsec must be in pb
    """
    d = {}
    for line in lim_lines:
        if "Observed" in line: d["obs"] = float(line.split("<")[-1])
        elif "Expected" in line: d["exp_"+line.split("%")[0].replace("Expected","").strip()] = float(line.split("<")[-1])
        elif "Significance:" in line: d["significance"] = float(line.split(":")[-1])

    obs = d.get("obs",-1)
    exp = d.get("exp_50.0",-1)
    exp_sm1 = d.get("exp_16.0",-1)
    exp_sp1 = d.get("exp_84.0",-1)
    return {
            "obs":obs, "exp":exp,
            "sp1":exp_sp1,"sm1":exp_sm1,
            }

def print_lims(d_lims, fb=False, unblinded=False):
    if unblinded:
        print "  Obs UL: r = {:.3f}".format(d_lims["obs"])
    print "  Exp UL: r = {:.3f} (+{:.3f} -{:.3f})".format(d_lims["exp"], d_lims["sp1"]-d_lims["exp"], d_lims["exp"]-d_lims["sm1"])

def get_lims(basedir, doupperlimit=True, redolimits=True,
        verbose=True, unblinded=False,sig="fs_t1tttt_m1600_m500",
        ):

    params = locals()


    card = "card_{signal}_all_run2.txt".format(signal=sig)
    full_card_name = "{}/{}".format(basedir,card)
    full_log_name = full_card_name.replace(".txt",".log")


    extra_base = ""
    did_run_limits = False
    if not os.path.isfile(full_log_name) or redolimits:
        make_card(basedir,sig,card,verbose=verbose)
        extra = extra_base[:]
        if not unblinded:
            extra = extra_base + " --noFitAsimov"
        limit_cmd = "combine -M AsymptoticLimits {0} {1}  2>&1 | tee {2}".format(full_card_name, extra, full_log_name)
        if verbose: print ">>> Running combine for upper limit [{0}]".format(limit_cmd)
        out = ""
        if doupperlimit:
            stat, out = commands.getstatusoutput(limit_cmd)
            did_run_limits = True
    else:
        if verbose: print ">>> [!] Limits already run, so reusing. Pass the --redolimits flag to redo the limits"
        stat, out = 0, open(full_log_name,"r").read()

    d_lims = parse_lims(out.splitlines())

    # now check to see if it's legit
    if d_lims["exp"] < 0. and doupperlimit:
        if verbose: print ">>> [!] Uh-oh, error with combine? Here's what the output of combine was:"
        os.system("cat {0}".format(full_log_name))
        raise Exception("Combine error!")

    # if not unblinded: d_lims["obs"] = d_lims["exp"]

    if did_run_limits and os.path.exists(full_log_name):
        with open(full_log_name,"a") as fh:
            fh.write("\n")
            fh.write("PARAMS: {}\n".format(str(params)))
            fh.write("RESULT: {}\n".format(str(d_lims)))

    return d_lims

if __name__ == "__main__":
    pass

    parser = argparse.ArgumentParser(formatter_class=lambda prog: argparse.HelpFormatter(prog,max_help_position=35, width=150))
    parser.add_argument("basedir", help="basedir")
    parser.add_argument("-s", "--sig", help="signal name (default: %(default)s)", default="tttt")
    parser.add_argument(      "--clean", help="if using same directory, clean the old root files to remake them (default: %(default)s)", action="store_true")
    args = parser.parse_args()

    basedir = args.basedir
    sig= args.sig
    if args.clean:
        print "[!] Removing {}/*_run2.root files from the directory so they can get remade".format(basedir)
        os.system("rm {}/*_run2.root".format(basedir))
    print get_lims(basedir,sig=sig,verbose=True,redolimits=True,unblinded=True)

    """
    # basedir = "v3.23_testss_v1"
    # basedir = "v3.24_unblindpartialrereco_v1"
    basedir = "v3.24_unblindfull_v1"
    # basedir = "v3.24_ssnominal_v1/"
    sig = "fs_t1tttt_m1600_m100"
    # sig = "fs_t1tttt_m1800_m1250"

    basedir = "v3.24_fullsignals_v1"
    # sig = "fs_t1tttt_m1800_m1"
    sig = "fs_t1tttt_m1800_m1250"

    # print "[!] Removing {}/*_run2.root files from the directory so they can get remade".format(basedir)
    # os.system("rm {}/*_run2.root".format(basedir))


    # print get_lims(basedir,sig=sig,verbose=False,redolimits=False)
    print get_lims(basedir,sig=sig,verbose=True,redolimits=True)
    """

    """
    basedir = "v3.24_testss_v1/"
    # basedir = "v3.24_testssisr_v1/"
    # basedir = "v3.24_unblindfull_v1"
    sig = "fs_t1tttt_m1600_m100"
    # sig = "fs_t1tttt_m1800_m100"

    print "[!] Removing {}/*_run2.root files from the directory so they can get remade".format(basedir)
    os.system("rm {}/*_run2.root".format(basedir))


    # print get_lims(basedir,sig=sig,verbose=False,redolimits=False)
    print get_lims(basedir,sig=sig,verbose=True,redolimits=True)
    """

    # parser = argparse.ArgumentParser(formatter_class=lambda prog: argparse.HelpFormatter(prog,max_help_position=35, width=150))
    # parser.add_argument("card", help="card name in directory")
    # parser.add_argument("-C", "--redocard", help="force the recreation of the card (default: %(default)s)", action="store_true")
    # parser.add_argument("-L", "--redolimits", help="force the rerunning of limits (default: %(default)s)", action="store_true")
    # parser.add_argument("-M", "--domcfakes", help="use fakes from MC (default: %(default)s)", action="store_true")
    # parser.add_argument("-X", "--noscan", help="don't do a likelihood fit/scan (default: %(default)s)", action="store_true")
    # parser.add_argument("-N", "--nosignificance", help="don't calculate significance (default: %(default)s)", action="store_true")
    # parser.add_argument("-r", "--regions", help="srcr or disc for SR+CR limits or BDT limits (default: %(default)s)", default="srcr")
    # parser.add_argument("-u", "--unblinded", help="show unblinded quantities (default: %(default)s)", action="store_true")
    # parser.add_argument("-a", "--allownegative", help="allow negative mu in maxlikelihood fit (default: %(default)s)", action="store_true")
    # parser.add_argument("-i", "--inject_tttt", help="inject tttt as a bkg (default: %(default)s)", action="store_true")
    # parser.add_argument("-m", "--noautostats", help="don't use autoMCStats feature (default: %(default)s)", action="store_true")
    # parser.add_argument("-t", "--thresh", help="threshold for autoMCstats (default: %(default)s)", default=0.0)
    # parser.add_argument("-s", "--sig", help="signal name (default: %(default)s)", default="tttt")
    # parser.add_argument("-y", "--year", help="year (default: %(default)s)", default=-1, type=int)
    # parser.add_argument(      "--scalelumi", help="scale luminosity (default: %(default)s)", default=1.0, type=float)
    # parser.add_argument(      "--scaletth", help="scale tth (default: %(default)s)", default=1.0, type=float)
    # parser.add_argument(      "--nosyst", help="no systs at all, but note autoMCStats might be included (default: %(default)s)", action="store_true")
    # parser.add_argument(      "--ignorefakes", help="ignore fake background entirely (default: %(default)s)", action="store_true")
    # parser.add_argument(      "--includeml", help="include multilep channel (default: %(default)s)", action="store_true")
    # args = parser.parse_args()

    # if "fs_" in args.sig:
    #     args.noscan = True
    #     args.nosignificance = True

    # # d_lims = get_lims(args)
    # d_lims = get_lims(
    #         card=args.card,
    #         sig=args.sig,
    #         regions=args.regions,
    #         redocard=args.redocard,
    #         redolimits=args.redolimits,
    #         domcfakes=args.domcfakes,
    #         unblinded=args.unblinded,
    #         doscan=(not args.noscan),
    #         inject_tttt=args.inject_tttt,
    #         dosignificance=(not args.nosignificance),
    #         allownegative=args.allownegative,
    #         use_autostats=(not args.noautostats),
    #         thresh=args.thresh,
    #         scalelumi=args.scalelumi,
    #         scaletth=args.scaletth,
    #         year=args.year,
    #         nosyst=args.nosyst,
    #         ignorefakes=args.ignorefakes,
    #         includeml=args.includeml,
    #         )
    # print "card: {}".format(args.card.strip())
    # print "------------------------------"
    # print_lims(d_lims, unblinded=args.unblinded)
    # print "------------------------------"
