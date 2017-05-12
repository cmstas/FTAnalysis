#!/usr/bin/env python

import ROOT as r
import os
import glob
import runLimits
import argparse
from multiprocessing.dummy import Pool as ThreadPool

def reduce_bins(fname, outdir, outbins):
    fin = r.TFile(fname)
    indir,basename = fname.rsplit("/",1)
    fout = r.TFile(outdir+"/"+basename,"RECREATE")
    for hname in [k.GetName() for k in fin.GetListOfKeys()]:
        h_in = fin.Get(hname)
        h_out = r.TH1F(hname, hname, len(outbins), 0.5, len(outbins)+0.5)
        binvals = list(h_in) # includes under and overflow, so bin 1 is index 1
        for ival,val in enumerate([binvals[ib] for ib in outbins]):
            h_out.SetBinContent(ival+1,val)
        h_out.Write()
    fin.Close()
    fout.Close()



def get_singlebin_limits(thedir, redo=False):

    rfiles = glob.glob(thedir+"/*_*.root")

    pool = ThreadPool(11)
    # pool = ThreadPool(1)

    bins = [
        [1,2,3],
        [1,2,4],
        [1,2,5],
        [1,2,6],
        [1,2,7],
        [1,2,8],
        [1,2,9],
        [1,2,10],
        ]

    def do_bin_setup(inp,rfiles):
        # not threadsafe - just make hists and cards
        ibin,outbins = inp
        outdir = "{0}/bin_{1}/".format(thedir,ibin+1)
        os.system("mkdir -p {0}".format(outdir))
        for rfile in rfiles:
            reduce_bins(rfile,outdir,outbins=outbins)
        runLimits.get_lims(outdir, redocard=True, redolimits=True, verbose=False, dolimits=False)


    def do_bin_limits(inp):
        # threadsafe - just run limits
        ibin,outbins = inp
        outdir = "{0}/bin_{1}/".format(thedir,ibin+1)
        try: return [inp, runLimits.get_lims(outdir, redocard=False, redolimits=redo, verbose=False)]
        except: return {}

    if redo:
        [do_bin_setup(inp,rfiles) for inp in enumerate(bins)]

    vals = []
    for result in pool.imap_unordered(do_bin_limits, enumerate(bins)):
        vals.append(result)

    return map(lambda x: (x[0][0]+1,x[1]),vals)

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("dir", help="directory")
    args = parser.parse_args()

    # thedir = "v0.04_May10_singlebins"
    print get_singlebin_limits(args.dir, redo=True)

