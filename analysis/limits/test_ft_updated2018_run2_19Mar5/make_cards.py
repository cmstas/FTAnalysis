#!/usr/bin/env python

import sys
import os
import argparse
import analysis.limits.createCard as createCard
import commands
import subprocess
import glob

import analysis.limits.makeSingleCombinedCard as makeSingleCombinedCard

def make_card(basedir,kine,finalcard,verbose=True,sig="tttt",domcfakes=False):

    # # CLEAN before rerunning. Once per SCAN, not per MASS POINT
    # os.system("rm {}/*run2*.root".format(basedir))

    cnames = []
    # for kine in ["srcr","srdisc"]:

    # Make cards per year
    outname_pattern = "card_{signal}_{kine}_{year}.txt"
    # for year in [2016,2017]:
    for year in [2016,2017,2018]:
        outname =outname_pattern.format(signal=sig,kine=kine,year=year)
        createCard.writeOneCard(basedir, outname, year=year, analysis="ft", kine=kine, signal=sig, domcfakes=domcfakes)
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
            # years=[2016,2017],
            years=[2016,2017,2018],
            skiptext2workspace = True,
            # autothresh = 3.0,
            to_correlate = [
                "scale",
                "pdf",
                "alphas",
                "bb",
                "TTH",
                "TTWSF",
                "TTZSF",
                "rares",
                "XG",
                "TTVV",
                "fakes",
                "TTTT",
                    ],
            )

    fullfinalcard = "{}/{}".format(basedir,finalcard)
    if verbose: print "Wrote {}".format(fullfinalcard)
    return fullfinalcard


def make_cards(basedir, kine,verbose=True, sig="tttt",domcfakes=False):
    card = "card_{signal}_{kine}_run2.txt".format(signal=sig,kine=kine)
    return make_card(basedir,kine,card,sig=sig,verbose=verbose,domcfakes=domcfakes)

if __name__ == "__main__":
    pass

    # parser = argparse.ArgumentParser(formatter_class=lambda prog: argparse.HelpFormatter(prog,max_help_position=35, width=150))
    # parser.add_argument("basedir", help="basedir")
    # parser.add_argument("-s", "--sig", help="signal name (default: %(default)s)", default="tttt")
    # parser.add_argument(      "--clean", help="if using same directory, clean the old root files to remake them (default: %(default)s)", action="store_true")
    # args = parser.parse_args()

    # basedir = args.basedir
    # sig= args.sig
    # basedir = "v3.27_ft_mar5_v1/"
    # basedir = "v3.28_ft_mar6_v1/"
    # basedir = "v3.28_ft_mar6unblind_v1/"
    # basedir = "v3.28_ft_mar12_v1/"
    # basedir = "v3.28_ft_mar12checktrig_v1/"

    basedir = "v3.28_ft_mar13ttwbb_v1/"
    # basedir = "v3.28_ft_mar13test_v1"
    # basedir = "v3.28_ft_mar12ttwbb_v1/"
    # basedir = "v3.28_ft_mar12mcfakeshybrid_v1/"
    # basedir = "v3.28_ft_mar6oldbdt_v1/"
    # basedir = "v3.28_ft_mar6oldbdtnewbins_v1/"
    # basedir = "v3.27_ft_mar5oldbdt_v1/"
    # basedir = "v3.27_ft_mar5newfr_v1/"
    # basedir = "v3.27_ft_mar5jes_v1/"
    if len(sys.argv) >= 2:
        basedir = sys.argv[-1]
        print "Using overriden basedir of",basedir
    clean = True
    if clean:
        print "[!] Removing {}/*_run2.root files from the directory so they can get remade".format(basedir)
        os.system("rm {}/*_run2.root".format(basedir))
    for kine in ["srcr","srdisc"]:
        # make_cards(basedir,kine)
        if "mcfakeshybrid" in basedir:
            make_cards(basedir,kine,domcfakes=True)
        else:
            make_cards(basedir,kine)

    # basedir="v3.28_ft_test_Apr30_higgs_v1/"
    # clean = True
    # if clean:
    #     print "[!] Removing {}/*_run2.root files from the directory so they can get remade".format(basedir)
    #     os.system("rm {}/*_run2.root".format(basedir))
    # sigs = sorted(list(set(map(lambda x: x.rsplit("/",1)[-1].split("_")[0],glob.glob("{}/higgs*histos*.root".format(basedir))))))
    # for kine in ["srdisc"]:
    #     for sig in sigs:
    #         make_cards(basedir,kine,sig=sig)

#     basedir="v3.28_ft_test_Apr20_oblique_v1/"
#     clean = True
#     if clean:
#         print "[!] Removing {}/*_run2.root files from the directory so they can get remade".format(basedir)
#         os.system("rm {}/*_run2.root".format(basedir))
#     sigs = sorted(list(set(map(lambda x: x.rsplit("/",1)[-1].split("_")[0],glob.glob("{}/hhat*histos*.root".format(basedir))))))
#     for kine in ["srdisc"]:
#         for sig in sigs:
#             make_cards(basedir,kine,sig=sig)
