import glob
import os
import os
import runSSLimits

import analysis.utils.pyrun as pyrun
import argparse
import fnmatch
import ast
import time
import sys


if __name__ == "__main__":

    # basedir = "v3.23_ss_fastsim_19Jan22"
    # sig = "fs_t1tttt_m1600_m100"
    # print runSSLimits.get_lims(basedir,sig=sig,verbose=True,redolimits=False)

    clean = False
    basedir = "v3.26_feb15_sst1t5rpv_v1"
    skip_already_done = True

    def run((index,info)):
        basedir,sig = info
        try:
            # runSSLimits.get_lims(basedir,sig=sig,verbose=False,redolimits=True,unblinded=True)
            runSSLimits.get_lims(basedir,sig=sig,verbose=False,redolimits=False,unblinded=True)
        except:
            pass
            print "Skipping error with {}, {}".format(basedir, sig)
        return index,0

    def get_args(basedir, skip_already_done=False):
        fnames = glob.glob("{}/fs_*histos*.root".format(basedir))
        sigs = set(map(lambda x: x.rsplit("/")[-1].split("_histos")[0], fnames))
        if skip_already_done:
            log_fnames = glob.glob("{}/card_fs_*_all_run2.log".format(basedir))
            # print log_fnames
            done_sigs = set(map(lambda x: x.rsplit("/")[-1].split("_all_run2")[0].replace("card_",""), log_fnames))
            sigs = sigs - done_sigs
        ret = [[basedir,sig] for sig in sigs]
        return ret

    # basedir = "v3.23_ss_fastsim_19Jan22"
    # basedir = "v3.24_fullsignals_v1/" # AN from before preapproval
    args = get_args(basedir, skip_already_done=skip_already_done)
    print "Running {} points".format(len(args))
    # args = args[:10]
    # print args[:10]
    # print len(args)

    if clean:
        print "[!] Removing {}/*_run2.root files from the directory so they can get remade".format(basedir)
        os.system("rm {}/*_run2.root".format(basedir))

    # run the first one serially to make root files and stuff without worrying about race conditions
    map(run,enumerate(args[:1]))

    os.nice(10)
    runner = pyrun.Runner(nproc=min(30,len(args)), func=run, dot_type=(3 if len(args)<500 else 0))
    runner.add_args(args)
    runner.run()

