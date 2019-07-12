#!/usr/bin/env python

import sys
import os
import argparse
from analysis.limits import createCard
import commands
import subprocess

# from createCard import writeOneCard
# from makeSingleCombinedCard import do_combine
from analysis.limits import makeSingleCombinedCard

def make_card(basedir,sig,finalcard,verbose=True,workspace=False):

    # # CLEAN before rerunning. Once per SCAN, not per MASS POINT
    # os.system("rm {}/*run2*.root".format(basedir))

    cnames = []
    for kine in ["srincl","srmi1","srmi2"]:

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
                skiptext2workspace = not workspace,
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
                        "bb",
                "fakes_normNB0",
                "fakes_normNB1",
                "fakes_normNB2",
                "fakes_normNB3",
                        ],
                )

    # # Combine the cards per kinematic region into one...so one card per signal point
    # # finalcard = "card_{signal}_all_run2.txt".format(signal=sig)
    # cmd = "( cd {} ; combineCards.py {} > {} )".format(basedir, " ".join(cnames), finalcard)
    # if verbose: print cmd
    # os.system(cmd)
    fullfinalcard = "{}/{}".format(basedir,finalcard)
    if verbose: print "Wrote {}".format(fullfinalcard)


    return fullfinalcard

def setup_cards(basedir, doupperlimit=True, dosig=True,redolimits=True,
        verbose=True, unblinded=False,sig="fs_t1tttt_m1600_m500",
        cardonly=False,
        workspace=False,
        ):

    params = locals()


    card = "card_{signal}_all_run2.txt".format(signal=sig)
    full_card_name = "{}/{}".format(basedir,card)
    full_log_name = full_card_name.replace(".txt",".log")
    fullfinalcard = make_card(basedir,sig,card,verbose=verbose,workspace=workspace)

if __name__ == "__main__":
    pass

    parser = argparse.ArgumentParser(formatter_class=lambda prog: argparse.HelpFormatter(prog,max_help_position=35, width=150))
    parser.add_argument("basedir", help="basedir")
    parser.add_argument("-s", "--sig", help="signal name (default: %(default)s)", default="tttt")
    parser.add_argument(      "--clean", help="if using same directory, clean the old root files to remake them (default: %(default)s)", action="store_true")
    parser.add_argument(      "--cardonly", help="only make card", action="store_true")
    parser.add_argument(      "--workspace", help="also make a combine workspace", action="store_true")
    parser.add_argument(      "--redolimits", help="redo limits (ignore log file presence)", action="store_true")
    args = parser.parse_args()

    basedir = args.basedir
    sig= args.sig
    if args.clean:
        print "[!] Removing {}/*_run2.root files from the directory so they can get remade".format(basedir)
        os.system("rm {}/*_run2.root".format(basedir))
    print setup_cards(basedir,sig=sig,verbose=True,redolimits=args.redolimits,unblinded=True,cardonly=args.cardonly,workspace=args.workspace)
