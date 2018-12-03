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
        print "  Sig: %.3f (p-value: %.4f)" % (d_lims["significance"], d_lims["pvalue"])
    if d_lims.get("mu",-999) > -980.:
        print "  Mu: %.3f (+%.3f -%.3f) | (+%.1f%% -%.1f%%)" % (d_lims["mu"], d_lims["mu_up"], d_lims["mu_down"], 100.*(d_lims["mu_up"]/d_lims["mu"]), 100.*(d_lims["mu_down"]/d_lims["mu"]))

def get_lims(card, regions="srcr", doupperlimit=True, redocard=True, redolimits=True, domcfakes=False, ignorefakes=False,
        verbose=True, dolimits=True, dosignificance=True, doscan=True,
        unblinded=False,sig="tttt", allownegative=False, inject_tttt=False,
        use_autostats=False, thresh=0.0, scalelumi=1.0, scaletth=1.0, year=-1, nosyst=False):

    params = locals()

    extra_base = ""
    # NOTE, can't do both or else need to combine the flags
    if scalelumi != 1.0:
        extra_base += " --setParameters lumiscale={0} --setParameterRanges lumiscale={0},{0} ".format(scalelumi)
    elif scaletth != 1.0:
        extra_base = " --setParameters tthscale={0} --setParameterRanges tthscale={0},{0} ".format(scaletth)

    if nosyst:
        extra_base += " -S 0 "


    if ".txt" not in card:
        card += "/card_{0}_{1}.txt".format(sig, regions)
        if verbose: print ">>> [!] no card name specified, so using {0}".format(card)

    full_card_name = "{0}".format(card)
    full_card_name_root = "{0}".format(card.replace(".txt",".root"))
    full_log_name = full_card_name.replace(".txt",".log")

    if not os.path.isfile(full_card_name) or redocard:
        if verbose: print ">>> Making card"
        dirname, cardname = card.rsplit("/",1)
        createCard.writeOneCard(dirname,cardname, kine=regions,domcfakes=domcfakes, signal=sig, inject_tttt=inject_tttt, use_autostats=use_autostats, thresh=thresh, year=year, ignorefakes=ignorefakes)
        if verbose: print ">>> Making workspace"
        # There was a bug with combine (https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit/issues/491) which needed text2workspace to be explicitly
        # run before using FitDiagnostics (for mu scan & pre/postfit), but this is fixed in the 81x branch (cherry pick 69c180a04c05d0bad6a6c38d97cdc431cfe4fb49 in 94x if
        # needed). So we don't need this anymore and can switch .root -> .txt in all the below commands. Though, it's faster to run text2workspace first and not do it
        # implicitly 3 times total.
        cmd = "text2workspace.py {0} -o {1}".format(full_card_name,full_card_name_root)
        stat, out = commands.getstatusoutput(cmd)
    else:
        if verbose: print ">>> [!] Card already made, so reusing. Pass the --redocard flag to remake the card"

    if not dolimits: return {}

    if not os.path.isfile(full_log_name) or (redolimits or redocard):
        extra = extra_base[:]
        if not unblinded:
            extra = extra_base + " --noFitAsimov"
        limit_cmd = "combine -M AsymptoticLimits {0} {1}  2>&1 | tee {2}".format(full_card_name_root, extra, full_log_name)
        if verbose: print ">>> Running combine for upper limit [{0}]".format(limit_cmd)
        out = ""
        if doupperlimit:
            stat, out = commands.getstatusoutput(limit_cmd)
        if dosignificance:
            extra = extra_base + (" -t -1 --expectSignal=1" if not unblinded else "")
            # verbose mode or else we can't see p-value
            significance_cmd = "combine -M Significance {0} --significance {1} -v 1 2>&1 | tee -a {2}".format(full_card_name_root, extra, full_log_name)
            if verbose: print ">>> Running combine for significance [{0}]".format(significance_cmd)
            stat, out_sig = commands.getstatusoutput(significance_cmd)
            out += out_sig
        if doscan:
            dirname, cardname = card.rsplit("/",1)
            cardname_mu = cardname
            extra = extra_base[:]
            if not unblinded:
                extra += " --expectSignal=1 -t -1 "
            if allownegative:
                extra += " --rMin -2.0 --rMax +10.0"
            mu_cmd = "combine -M FitDiagnostics {0} --robustFit=1 --saveShapes --saveWithUncertainties {1} -n name --freezeParameters lumiscale,tthscale 2>&1 | tee -a {2}".format(full_card_name_root, extra, full_log_name)
            if verbose: print ">>> Running combine for mu [{0}]".format(mu_cmd)
            stat, out_mu = commands.getstatusoutput(mu_cmd)
            commands.getstatusoutput("cp fitDiagnosticsname.root {0}/mlfitname.root".format(dirname))
            out += out_mu
            # While FitDiagnostics gives a mu (and pre/postfit stuff), it does not give the *scan*, so need to do this manually with MultiDimFit
            scan_cmd = "combine  -M MultiDimFit {0} --algo grid --centeredRange=2.0 --saveFitResult --redefineSignalPOI r --robustFit=1 --freezeParameters lumiscale,tthscale -n name --saveNLL {1} 2>&1 | tee -a {2}".format(full_card_name_root, extra, full_log_name)
            if verbose: print ">>> Running combine for scan [{0}]".format(scan_cmd)
            stat, out_scan = commands.getstatusoutput(scan_cmd)
            commands.getstatusoutput("cp higgsCombinename.MultiDimFit.mH120.root {0}/scandata.root".format(dirname))
            out += out_scan
    else:
        if verbose: print ">>> [!] Limits already run, so reusing. Pass the --redolimits flag to redo the limits"
        stat, out = 0, open(full_log_name,"r").read()

    d_lims = parse_lims(out.splitlines(), fb=True)

    # now check to see if it's legit
    if d_lims["exp"] < 0. and doupperlimit:
        if verbose: print ">>> [!] Uh-oh, error with combine? Here's what the output of combine was:"
        os.system("cat {0}".format(full_log_name))
        raise Exception("Combine error!")

    if os.path.exists(full_log_name):
        with open(full_log_name,"a") as fh:
            fh.write("\n")
            fh.write("PARAMS: {}\n".format(str(params)))
            fh.write("RESULT: {}\n".format(str(d_lims)))

    return d_lims

if __name__ == "__main__":

    parser = argparse.ArgumentParser(formatter_class=lambda prog: argparse.HelpFormatter(prog,max_help_position=35, width=150))
    parser.add_argument("card", help="card name in directory")
    parser.add_argument("-C", "--redocard", help="force the recreation of the card (default: %(default)s)", action="store_true")
    parser.add_argument("-L", "--redolimits", help="force the rerunning of limits (default: %(default)s)", action="store_true")
    parser.add_argument("-M", "--domcfakes", help="use fakes from MC (default: %(default)s)", action="store_true")
    parser.add_argument("-X", "--noscan", help="don't do a likelihood fit/scan (default: %(default)s)", action="store_true")
    parser.add_argument("-N", "--nosignificance", help="don't calculate significance (default: %(default)s)", action="store_true")
    parser.add_argument("-r", "--regions", help="srcr or disc for SR+CR limits or BDT limits (default: %(default)s)", default="srcr")
    parser.add_argument("-u", "--unblinded", help="show unblinded quantities (default: %(default)s)", action="store_true")
    parser.add_argument("-a", "--allownegative", help="allow negative mu in maxlikelihood fit (default: %(default)s)", action="store_true")
    parser.add_argument("-i", "--inject_tttt", help="inject tttt as a bkg (default: %(default)s)", action="store_true")
    parser.add_argument("-m", "--noautostats", help="don't use autoMCStats feature (default: %(default)s)", action="store_true")
    parser.add_argument("-t", "--thresh", help="threshold for autoMCstats (default: %(default)s)", default=0.0)
    parser.add_argument("-s", "--sig", help="signal name (default: %(default)s)", default="tttt")
    parser.add_argument("-y", "--year", help="year (default: %(default)s)", default=-1, type=int)
    parser.add_argument(      "--scalelumi", help="scale luminosity (default: %(default)s)", default=1.0, type=float)
    parser.add_argument(      "--scaletth", help="scale tth (default: %(default)s)", default=1.0, type=float)
    parser.add_argument(      "--nosyst", help="no systs at all, but note autoMCStats might be included (default: %(default)s)", action="store_true")
    parser.add_argument(      "--ignorefakes", help="ignore fake background entirely (default: %(default)s)", action="store_true")
    # args = parser.parse_args()
    args, unknown = parser.parse_known_args()
    print "[?] Found some unknown args, but just ignoring them:", unknown

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
            allownegative=args.allownegative,
            use_autostats=(not args.noautostats),
            thresh=args.thresh,
            scalelumi=args.scalelumi,
            scaletth=args.scaletth,
            year=args.year,
            nosyst=args.nosyst,
            ignorefakes=args.ignorefakes,
            )
    print "card: {}".format(args.card.strip())
    print "------------------------------"
    print_lims(d_lims, fb=True, unblinded=args.unblinded)
    print "------------------------------"
