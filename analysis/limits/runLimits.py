#!/usr/bin/env python

import sys
import os
import argparse
import createCard
import commands

XSEC_TTTT = 0.0092 # pb

def parse_lims(lim_lines, fb=False):
    """
    return dictionary with obs, exp, sp1, sm1 cross section limits in pb
    input xsec must be in pb
    """
    d = {}
    for line in lim_lines:
        if "Observed" in line: d["obs"] = float(line.split("<")[-1])
        elif "Expected" in line: d["exp_"+line.split("%")[0].replace("Expected","").strip()] = float(line.split("<")[-1])

    mult = 1 if not fb else 1000.0
    obs = d.get("obs",-1)*XSEC_TTTT*mult
    exp = d.get("exp_50.0",-1)*XSEC_TTTT*mult
    exp_sm1 = d.get("exp_16.0",-1)*XSEC_TTTT*mult
    exp_sp1 = d.get("exp_84.0",-1)*XSEC_TTTT*mult
    return {
            "obs":obs, "exp":exp,
            "sp1":exp_sp1,"sm1":exp_sm1,
            }

def print_lims(d_lims, fb=False):
    unit = "pb" if not fb else "fb"
    print "  Obs: %.2f %s" % (d_lims["obs"], unit)
    print "  Exp: %.2f + %.2f - %.2f %s" % (d_lims["exp"], d_lims["sp1"]-d_lims["exp"], d_lims["exp"]-d_lims["sm1"], unit)

def get_lims(card, regions="srcr", redocard=True, redolimits=True, domcfakes=False, verbose=True, dolimits=True):

    if ".txt" not in card:
        card += "/card_tttt_{0}.txt".format(regions)
        if verbose: print ">>> [!] no card name specified, so using {0}".format(card)

    full_card_name = "{0}".format(card)
    full_log_name = full_card_name.replace(".txt",".log")

    if not os.path.isfile(full_card_name) or redocard:
        if verbose: print ">>> Making card"
        dirname, cardname = card.rsplit("/",1)
        createCard.writeOneCard(dirname,cardname, kine=regions,domcfakes=domcfakes)
    else:
        if verbose: print ">>> [!] Card already made, so reusing. Pass the --redocard flag to remake the card"

    if not dolimits: return {}

    if not os.path.isfile(full_log_name) or (redolimits or redocard):
        combine_cmd = "combine -M Asymptotic {0} --noFitAsimov  2>&1 | tee {1}".format(full_card_name, full_log_name)
        if verbose: print ">>> Running combine [{0}]".format(combine_cmd)
        stat, out = commands.getstatusoutput(combine_cmd)
    else:
        if verbose: print ">>> [!] Limits already run, so reusing. Pass the --redolimits flag to redo the limits"
        stat, out = 0, open(full_log_name,"r").read()

    d_lims = parse_lims(out.splitlines(), fb=True)

    # now check to see if it's legit
    if d_lims["exp"] < 0.:
        if verbose: print ">>> [!] Uh-oh, error with combine? Here's what the output of combine was:"
        os.system("cat {0}".format(full_log_name))
        raise Exception("Combine error!")

    return d_lims

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("card", help="card name in directory")
    parser.add_argument("-C", "--redocard", help="force the recreation of the card", action="store_true")
    parser.add_argument("-L", "--redolimits", help="force the rerunning of limits", action="store_true")
    parser.add_argument("-M", "--domcfakes", help="use fakes from MC", action="store_true")
    parser.add_argument("-r", "--regions", help="srcr or disc for SR+CR limits or BDT limits", default="srcr")
    args = parser.parse_args()

    # d_lims = get_lims(args)
    d_lims = get_lims(
            card=args.card,
            regions=args.regions,
            redocard=args.redocard,
            redolimits=args.redolimits,
            domcfakes=args.domcfakes,
            )
    print "------------------------------"
    print_lims(d_lims, fb=True)
    print "------------------------------"
