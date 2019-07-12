import commands
import ROOT as r
import os
import sys
import glob
import argparse
import re
from multiprocessing.dummy import Pool as ThreadPool
import itertools
from tqdm import tqdm

def reduce_bins(fname_in, fname_out, bins_to_keep=[],set_content_to_one=False):
    fin = r.TFile(fname_in)
    do_move_tmp = False
    if fname_out == fname_in:
        do_move_tmp = True
        fname_out = fname_in + ".tmp"
    fout = r.TFile(fname_out,"RECREATE")
    for hname in [k.GetName() for k in fin.GetListOfKeys() if k.ReadObj().InheritsFrom(r.TH1.Class())]:
        h_in = fin.Get(hname)
        h_out = r.TH1F(hname, hname, len(bins_to_keep), 0.5, len(bins_to_keep)+0.5)
        binvals = list(h_in) # includes under and overflow, so bin 1 is index 1
        binerrs = [h_in.GetBinError(ibin) for ibin in range(0,h_in.GetNbinsX()+2)]
        for ival,(val,err) in enumerate([(binvals[ib],binerrs[ib]) for ib in bins_to_keep]):
            if val < 1e-6:
                # print fname_in,ival,val,err
                val = 1e-6
                err = 1e-6
            if set_content_to_one:
                h_out.SetBinContent(ival+1,1)
                h_out.SetBinError(ival+1,0)
            else:
                h_out.SetBinContent(ival+1,val)
                h_out.SetBinError(ival+1,err)
        h_out.Write()
    # print "Wrote {} with bin indices ({}): {}".format(fname_out,len(bins_to_keep),str(bins_to_keep))
    fin.Close()
    fout.Close()
    if do_move_tmp:
        os.system("mv {} {}".format(fname_out,fname_in))

if __name__ == "__main__":

    basedir = "v3.31_ss_test_v0/"

    for which,nbins in [
            ["srmi1",10],
            ["srmi2",11],
            ["srincl",17],
            ]:
        for ibin in tqdm(range(1,nbins+1)):
            # break

            # # which = "srmi1" # 10 bins
            # # which = "srmi2" # 11 bins
            # which = "srincl" # 19 bins
            # ibin = 1


            tempdir = "temp/cards_{}_bin{}/".format(which,ibin)
            os.system("mkdir -p {}".format(tempdir))

            os.system("cp -rp {}/{{fs_t1tttt_m1800_m1000,ttz,ttw,xg,fakes,ww,tth,rares,flips,data,wz}}_histos_{}_run2.root {}/".format(basedir,which,tempdir))
            os.system("cp -rp {}/card_fs_t1tttt_m1800_m1000_{}_run2.txt {}/".format(basedir,which,tempdir))

            # for fname in tqdm(glob.glob(tempdir+"/*.root")):
            for fname in (glob.glob(tempdir+"/*.root")):
                reduce_bins(
                        fname_in=fname,
                        fname_out=fname,
                        bins_to_keep=[ibin],
                        set_content_to_one = ("fs_" in fname),
                        )

            cmd = "sed -i -s 's#{basedir}/#{tempdir}/#' {tempdir}/*.txt ;".format(basedir=basedir.rstrip("/"),tempdir=tempdir)
            cmd += " sed -i -s '/\(^observation\|^rate\)/s/[0-9\.]\+/-1/g' {tempdir}/*.txt ".format(tempdir=tempdir)
            if "srmi1" in tempdir and "bin8" in tempdir:
                cmd += " ; sed -i '/fakes_EWK/d' {tempdir}/*.txt".format(tempdir=tempdir)
            # print cmd
            commands.getoutput(cmd)


            # combine -M FitDiagnostics temp/cards_srincl_bin1/card_fs_t1tttt_m1800_m1000_srincl_run2.txt --saveShapes --saveWithUncertainties --saveOverallShapes -n test

    cardnames = glob.glob("temp/*/*_run2.txt")
    print "### COMMANDS:"
    for cardname in cardnames:
        logname_lim = cardname.replace(".txt","_lim.log")
        print "combine -M AsymptoticLimits {cardname} >& {logname}".format(cardname=cardname,logname=logname_lim)

        if "srincl" in cardname:
            logname_fit = cardname.replace(".txt","_fit.log")
            label = cardname.split("temp/",1)[-1].split("/",1)[0].replace("cards","")
            print "combine -M FitDiagnostics {cardname} --saveShapes --saveWithUncertainties --saveOverallShapes -n {label} >& {logname}".format(cardname=cardname,label=label,logname=logname_fit)
        else:
            print "combine -M HybridNew --frequentist --testStat LHC --seed -1 -T 400 {cardname}  -v 0 --rAbsAcc=0.05 --rRelAcc=0.02 >& {logname}".format(
                    cardname=cardname,
                    logname=logname_lim.replace(".log","_toys_obs.log"),
                    )
            for quantile in [0.025,0.16,0.5,0.84,0.975]:
                print "combine -M HybridNew --frequentist --testStat LHC --seed -1 -T 400 {cardname}  -v 0 --rAbsAcc=0.05 --rRelAcc=0.02 --expectedFromGrid={quantile} >& {logname}".format(
                        cardname=cardname,
                        logname=logname_lim.replace(".log","_toys_exp{}.log".format(str(quantile).replace(".","p"))),
                        quantile=quantile,
                        )
    print "###"
    print "###"
    print "### Then run: cp *.root root_files/"
