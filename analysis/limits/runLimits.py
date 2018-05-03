#!/usr/bin/env python

import sys
import os
import argparse
import createCard
import commands

# XSEC_TTTT = 0.0092 # pb
XSEC_TTTT = 0.01197 # pb

def parse_lims(lim_lines, fb=False):
    """
    return dictionary with obs, exp, sp1, sm1 cross section limits in pb
    input xsec must be in pb
    """
    d = {}
    for line in lim_lines:
        if "Observed" in line: d["obs"] = float(line.split("<")[-1])
        elif "Expected" in line: d["exp_"+line.split("%")[0].replace("Expected","").strip()] = float(line.split("<")[-1])
        elif "Significance:" in line: d["significance"] = float(line.split(":")[-1])
        elif "p-value" in line: d["pvalue"] = float(line.split("=")[-1].replace(")",""))
        elif "Best fit r:" in line:
            parts = line.split(":")[-1].split()
            rval = parts[0]
            down, up = parts[1].replace("+","").replace("-","").split("/")
            d["mu"] = float(rval)
            d["mu_up"] = float(up)
            d["mu_down"] = float(down)

    mult = 1 if not fb else 1000.0
    obs = d.get("obs",-1)*XSEC_TTTT*mult
    exp = d.get("exp_50.0",-1)*XSEC_TTTT*mult
    exp_sm1 = d.get("exp_16.0",-1)*XSEC_TTTT*mult
    exp_sp1 = d.get("exp_84.0",-1)*XSEC_TTTT*mult
    return {
            "obs":obs, "exp":exp,
            "sp1":exp_sp1,"sm1":exp_sm1,
            "significance": d.get("significance",-1.),
            "pvalue": d.get("pvalue",-1.),
            "mu": d.get("mu",-999.),
            "mu_up": d.get("mu_up",-999.),
            "mu_down": d.get("mu_down",-999.),
            }

def print_lims(d_lims, fb=False, unblinded=False):
    unit = "pb" if not fb else "fb"
    if unblinded:
        print "  Obs UL: %.2f %s" % (d_lims["obs"], unit)
    print "  Exp UL: %.2f +%.2f -%.2f %s" % (d_lims["exp"], d_lims["sp1"]-d_lims["exp"], d_lims["exp"]-d_lims["sm1"], unit)
    if d_lims.get("significance",-1) > 0.:
        print "  Sig: %.3f (p-value: %.3f)" % (d_lims["significance"], d_lims["pvalue"])
    if d_lims.get("mu",-999) > -980.:
        print "  Mu: %.3f (+%.3f -%.3f)" % (d_lims["mu"], d_lims["mu_up"], d_lims["mu_down"])

def get_lims(card, regions="srcr", redocard=True, redolimits=True, domcfakes=False, verbose=True, dolimits=True, dosignificance=True, doscan=True, unblinded=False,sig="tttt", yukawa=-1,allownegative=False, inject_tttt=False):

    if ".txt" not in card:
        card += "/card_{0}_{1}.txt".format(sig, regions)
        if verbose: print ">>> [!] no card name specified, so using {0}".format(card)

    full_card_name = "{0}".format(card)
    full_log_name = full_card_name.replace(".txt",".log")

    if not os.path.isfile(full_card_name) or redocard:
        if verbose: print ">>> Making card"
        dirname, cardname = card.rsplit("/",1)
        createCard.writeOneCard(dirname,cardname, kine=regions,domcfakes=domcfakes, signal=sig, yukawa=int(yukawa),inject_tttt=inject_tttt)
        if int(yukawa) >  0:
            print "yukawa",yukawa
    else:
        if verbose: print ">>> [!] Card already made, so reusing. Pass the --redocard flag to remake the card"

    if not dolimits: return {}

    if not os.path.isfile(full_log_name) or (redolimits or redocard):
        extra = "--noFitAsimov"
        limit_cmd = "combine -M AsymptoticLimits {0} {1}  2>&1 | tee {2}".format(full_card_name, extra, full_log_name)
        if verbose: print ">>> Running combine [{0}]".format(limit_cmd)
        stat, out = commands.getstatusoutput(limit_cmd)
        if dosignificance:
            extra = "-t -1 --expectSignal=1" if not unblinded else ""
            significance_cmd = "combine -M Significance {0} --significance {1}  2>&1 | tee -a {2}".format(full_card_name, extra, full_log_name)
            if verbose: print ">>> Running combine for significance [{0}]".format(significance_cmd)
            stat, out_sig = commands.getstatusoutput(significance_cmd)
            out += out_sig
        if doscan:
            dirname, cardname = card.rsplit("/",1)
            do_blind=not unblinded
            cardname_scan = cardname
            full_card_name_scan = full_card_name
            if do_blind:
                cardname_scan = cardname.replace(".txt","_asimov.txt")
                full_card_name_scan = full_card_name.replace(".txt","_asimov.txt")
            if redocard:
                createCard.writeOneCard(dirname,cardname_scan, do_expected_data=do_blind, kine=regions,domcfakes=domcfakes, signal=sig, yukawa=int(yukawa),inject_tttt=inject_tttt)
            # scan_cmd = "combine -M MaxLikelihoodFit {0} --saveShapes --saveWithUncertainties -n name 2>&1 | tee -a {1}".format(full_card_name_scan, full_log_name)
            if allownegative:
                extra = "--rMin -2.0 --rMax +10.0"
            else:
                extra = ""
            scan_cmd = "combine -M MaxLikelihoodFit {0} --robustFit=1 --saveShapes --saveWithUncertainties {1} -n name 2>&1 | tee -a {2}".format(full_card_name_scan, extra, full_log_name)
            if verbose: print ">>> Running combine for mu [{0}]".format(scan_cmd)
            stat, out_scan = commands.getstatusoutput(scan_cmd)
            commands.getstatusoutput("cp mlfitname.root {0}/mlfit.root".format(dirname))
            out += out_scan
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
    parser.add_argument("-X", "--noscan", help="don't do a likelihood fit/scan", action="store_true")
    parser.add_argument("-N", "--nosignificance", help="don't calculate significance", action="store_true")
    parser.add_argument("-r", "--regions", help="srcr or disc for SR+CR limits or BDT limits", default="srcr")
    parser.add_argument("-u", "--unblinded", help="show unblinded quantities", action="store_true")
    parser.add_argument("-a", "--allownegative", help="allow negative mu in maxlikelihood fit", action="store_true")
    parser.add_argument("-i", "--inject_tttt", help="inject tttt as a bkg", action="store_true")
    parser.add_argument("-s", "--sig", help="signal name", default="tttt")
    parser.add_argument("-y", "--yukawa", help="yukawa coupling int for tth", default=-1)
    args = parser.parse_args()

    # d_lims = get_lims(args)
    d_lims = get_lims(
            card=args.card,
            sig=args.sig,
            regions=args.regions,
            redocard=args.redocard,
            redolimits=args.redolimits,
            domcfakes=args.domcfakes,
            unblinded=args.unblinded,
            doscan=(not args.noscan),
            inject_tttt=args.inject_tttt,
            dosignificance=(not args.nosignificance),
            yukawa=int(args.yukawa),
            allownegative=args.allownegative
            )
    print "------------------------------"
    print_lims(d_lims, fb=True, unblinded=args.unblinded)
    print "------------------------------"
