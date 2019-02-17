import glob
import os
import os
import runSSLimits

import analysis.utils.pyrun as pyrun
import argparse
import fnmatch
import ast
import time


if __name__ == "__main__":

    # basedir = "v3.23_ss_fastsim_19Jan22"
    # sig = "fs_t1tttt_m1600_m100"
    # print runSSLimits.get_lims(basedir,sig=sig,verbose=True,redolimits=False)

    def run((index,info)):
        basedir,sig = info
        try:
            runSSLimits.get_lims(basedir,sig=sig,verbose=False,redolimits=False,unblinded=True)
        except:
            pass
            print "Skipping error with {}, {}".format(basedir, sig)
        return index,0

    def get_args(basedir):
        fnames = glob.glob("{}/fs_*histos*.root".format(basedir))
        sigs = set(map(lambda x: x.rsplit("/")[-1].split("_histos")[0], fnames))
        return [[basedir,sig] for sig in sigs]

    # basedir = "v3.23_ss_fastsim_19Jan22"
    basedir = "v3.24_fullsignals_v1/"
    args = get_args(basedir)
    # args = args[:10]
    # print args[:10]
    # print len(args)

    print "[!] Removing {}/*_run2.root files from the directory so they can get remade".format(basedir)
    os.system("rm {}/*_run2.root".format(basedir))

    # run the first one serially to make root files and stuff without worrying about race conditions
    map(run,enumerate(args[:1]))

    os.nice(10)
    runner = pyrun.Runner(nproc=min(25,len(args)), func=run, dot_type=(3 if len(args)<500 else 0))
    runner.add_args(args)
    runner.run()

