#!/usr/bin/env python

import ROOT
import sys
import os
import subprocess
import math
import glob
import argparse

do_ttww = True

class Process(object):
    def __init__(self, mycount, myname, myrootf, myplot, thedir):
        self.count = mycount
        self.name = myname
        self.rootf = glob.glob(thedir+"/"+myrootf)[0]
        self.plot = myplot
        self.lumi  = "-"
        self.jes  = "-"
        self.jer  = "-"
        self.isr  = "-"
        self.met  = "-"
        self.scale  = "-"
        self.lepeff  = "-"
        self.hlt  = "-"
        self.lephlt  = "-"
        self.hthlt  = "-"
        self.btag = "-"
        self.pu = "-"
        self.TTWSF = "-"
        self.TTZSF = "-"
        # self.WZSF = "-"
        self.TTH = "-"
        self.TTWW = "-"
        # self.WW = "-"
        self.XG = "-"
        self.rares = "-"
        self.fakes = "-"
        self.fakes_EWK = "-"
        self.flips = "-"
        self.stat = "x"
        self.statshape = "-"
        self.pdf = "-"
        self.ratecache = None
        self.binscache = []
    def rate(self):
        if self.ratecache is not None:
            return self.ratecache
        else:
            f = ROOT.TFile(self.rootf)
            if f.Get(self.plot):
                self.ratecache = f.Get(self.plot).Integral()
                return self.ratecache
            else:
                print self.plot+" not found in "+self.rootf
                return 0

    def bins(self):
        if self.binscache:
            return self.binscache
        else:
            f = ROOT.TFile(self.rootf)
            if f.Get(self.plot):
                self.binscache = list(f.Get(self.plot))[1:-1]
                return self.binscache
            else:
                print self.plot+" not found in "+self.rootf
                return []

def writeStatForProcess(thedir, card, kine, process, processes, statshape=None):
    if process.name=="sig": return #fake signal for MI limits does not need stat unc.
    ra5 = ""
    # open file first to read. if one of the hists exists, then we've already called this function
    # and no need to open the file again and write
    f = ROOT.TFile(process.rootf)

    name = "_stat_"
    if statshape is not None:
        name = "_statshape_"

    h_test = "%s%s%s%s%sUp" % (ra5,process.name,name,kine,1)
    bypass = False
    if not f.Get(h_test) or bypass:
        f = ROOT.TFile(process.rootf,"UPDATE")

    h = f.Get("sr")
    hup = f.Get(("%s_statUp" % (process.name)))
    hdn = f.Get(("%s_statDown" % (process.name)))

    if not hup:
        print hup.GetName()+" not found in "+process.rootf
        return 0

    for ibin in range(1,hup.GetNbinsX()+1):
        card.write ("%-40s %-5s " % (("%s%s%s%s%s" % (ra5,process.name,name,kine,ibin)), "shape") )
        for myprocess in processes:
            if myprocess.count == process.count:
                card.write("%-15s " % "1")
                hupnewtest = f.Get("%s%s%s%s%sUp" % (ra5,process.name,name,kine,ibin))
                if not hupnewtest or bypass:
                    hupnew = h.Clone("%s%s%s%s%sUp" % (ra5,process.name,name,kine,ibin))
                    if statshape is None:
                        hupnew.SetBinContent(ibin,hup.GetBinContent(ibin))
                    else:
                        cent = h.GetBinContent(ibin)
                        up_stat = hup.GetBinContent(ibin)
                        dup_stat = up_stat-cent
                        dup_scale = statshape*cent
                        up = cent + (dup_stat**2.0+dup_scale**2.0)**0.5
                        hupnew.SetBinContent(ibin,up)
                    hupnew.Write()
                hdnnewtest = f.Get("%s%s%s%s%sDown" % (ra5,process.name,name,kine,ibin))
                if not hdnnewtest or bypass:
                    hdnnew = h.Clone("%s%s%s%s%sDown" % (ra5,process.name,name,kine,ibin))
                    if statshape is None:
                        if hdn.GetBinContent(ibin) > 0: hdnnew.SetBinContent(ibin,hdn.GetBinContent(ibin))
                        else: hdnnew.SetBinContent(ibin,0)
                    else:
                        cent = h.GetBinContent(ibin)
                        down_stat = hdn.GetBinContent(ibin)
                        ddown_stat = cent-down_stat
                        ddown_scale = statshape*cent
                        down = cent - (ddown_stat**2.0+ddown_scale**2.0)**0.5
                        hdnnew.SetBinContent(ibin,max(down,1.e-6))
                    hdnnew.Write()
            else:  card.write("%-15s " % ("-"))
        card.write("\n")

    if not h_test:
        f.Close()


#write card regardless of number of processes (but make sure signal is first in list)
def writeOneCardFromProcesses(thedir, kine, plot, output, data, processes):

    line = "---------------------------------------------------------------"
    binname = "SS"
    card = open(str(thedir)+'/'+str(output), 'w')
    card.write("imax 1  number of channels \n")
    card.write("jmax *  number of backgrounds \n")
    card.write("kmax *  number of nuisance parameters \n")
    card.write(line+"\n")
    for process in processes:
        card.write("shapes "+process.name+" * "+process.rootf+" "+plot+" $SYSTEMATIC\n")
    card.write("shapes data_obs * "+data.rootf+" "+plot+" "+plot+"\n")
    card.write(line+"\n")
    card.write("bin "+str(binname)+"\n")
    #pseudo data for now
    card.write("observation %-15.3f \n" % (data.rate()))
    card.write(line+"\n")
    #bin
    card.write("%-40s %-5s " % ("bin",""))
    for process in processes: card.write("%-15s " % (binname))
    card.write("\n")
    #process count
    card.write("%-40s %-5s " % ("process",""))
    for process in processes: card.write("%-15s " % (process.count))
    card.write("\n")
    #process name
    card.write("%-40s %-5s " % ("process",""))
    for process in processes: card.write("%-15s " % (process.name))
    card.write("\n")
    #process rate
    card.write("%-40s %-5s " % ("rate",""))
    for process in processes: card.write("%-15.3f " % (process.rate()))
    card.write("\n")
    #separate
    card.write(line+"\n")

    #nuisance lumi
    card.write("%-40s %-5s " % ("lumi","lnN"))
    for process in processes: card.write("%-15s " % (process.lumi))
    card.write("\n")
    #nuisance jes
    card.write("%-40s %-5s " % ("jes","shape"))
    for process in processes: card.write("%-15s " % (process.jes))
    card.write("\n")
    #nuisance jer
    card.write("%-40s %-5s " % ("jer","shape"))
    for process in processes: card.write("%-15s " % (process.jer))
    card.write("\n")
    #nuisance isr
    card.write("%-40s %-5s " % ("isr","shape"))
    for process in processes: card.write("%-15s " % (process.isr))
    card.write("\n")
    #nuisance lepeff
    card.write("%-40s %-5s " % ("lep","shape"))
    for process in processes: card.write("%-15s " % (process.lepeff))
    card.write("\n")
    #nuisance lephlt
    card.write("%-40s %-5s " % ("lephlt","lnN"))
    for process in processes: card.write("%-15s " % (process.lephlt))
    card.write("\n")
    #nuisance hthlt
    card.write("%-40s %-5s " % ("hthlt","shape"))
    for process in processes: card.write("%-15s " % (process.hthlt))
    card.write("\n")
    #nuisance btag
    card.write("%-40s %-5s " % ("btag","shape"))
    for process in processes: card.write("%-15s " % (process.btag))
    card.write("\n")

    #nuisance scale
    card.write("%-40s %-5s " % ("scale","shape"))
    for process in processes: card.write("%-15s " % (process.scale))
    card.write("\n")
    #nuisance pdf
    card.write("%-40s %-5s " % ("pdf","shape"))
    for process in processes: card.write("%-15s " % (process.pdf))
    card.write("\n")

    #nuisance pu
    card.write("%-40s %-5s " % ("pu","shape"))
    for process in processes: card.write("%-15s " % (process.pu))
    card.write("\n")

    #nuisance TTW
    card.write("%-40s %-5s " % ("TTWSF","lnN"))
    for process in processes: card.write("%-15s " % (process.TTWSF))
    card.write("\n")

    #nuisance TTZ
    card.write("%-40s %-5s " % ("TTZSF","lnN"))
    for process in processes: card.write("%-15s " % (process.TTZSF))
    card.write("\n")

    #nuisance TTH
    card.write("%-40s %-5s " % ("TTH","lnN"))
    for process in processes: card.write("%-15s " % (process.TTH))
    card.write("\n")

    #nuisance TTWW
    card.write("%-40s %-5s " % ("TTWW","lnN"))
    for process in processes: card.write("%-15s " % (process.TTWW))
    card.write("\n")

    # #nuisance WZ
    # card.write("%-40s %-5s " % ("WZSF","lnN"))
    # for process in processes: card.write("%-15s " % (process.WZSF))
    # card.write("\n")


    # #nuisance WW
    # card.write("%-40s %-5s " % ("WW","lnN"))
    # for process in processes: card.write("%-15s " % (process.WW))
    # card.write("\n")

    #nuisance XG
    card.write("%-40s %-5s " % ("XG","lnN"))
    for process in processes: card.write("%-15s " % (process.XG))
    card.write("\n")

    #nuisance RARES
    card.write("%-40s %-5s " % ("rares","lnN"))
    for process in processes: card.write("%-15s " % (process.rares))
    card.write("\n")

    #nuisance fakes
    card.write("%-40s %-5s " % ("fakes","lnN"))
    for process in processes: card.write("%-15s " % (process.fakes))
    card.write("\n")
    #nuisance fakes EWK
    card.write("%-40s %-5s " % ("fakes_EWK","shape"))
    for process in processes: card.write("%-15s " % (process.fakes_EWK))
    card.write("\n")

    #nuisance flips
    card.write("%-40s %-5s " % ("flips","lnN"))
    for process in processes: card.write("%-15s " % (process.flips))
    card.write("\n")

    for process in processes:
        # if process.name in ["ttw","ttz","tth","wz","ww"]:
        if process.name in ["ttw","ttz","tth","ttww"]:
            statshape = 0.15 # ttw, ttz, tth, wz, tttt
            # if process.name in ["ww"]: statshape = 0.20
            process.statshape = 1.+statshape
            writeStatForProcess(thedir,card,kine,process,processes, statshape=statshape)
        else:
            writeStatForProcess(thedir,card,kine,process,processes)

    return

def writeOneCard(thedir, output, signal="tttt", kine="srcr", plot="sr", domcfakes=False, do_expected_data=False):
    #define processes (signal first)
    # if pseudoData:
    #     print "Using pseudo data!"
    #     data = Process(-1,"data","pdata_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    # else:
        # print "Using real data!"
    data = Process(-1,"data","data_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    signal = Process(0,signal,signal+"_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    TTW = Process(1,"ttw","ttw_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    TTZ = Process(2,"ttz","ttz_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    TTH = Process(3,"tth","tth_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    # WZ  = Process(4,"wz","wz_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    # WW  = Process(5,"ww","ww_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    XG  = Process(4,"xg","xg_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    rares = Process(5,"rares","rares_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    if not domcfakes:
        fakes = Process(6,"fakes","fakes_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    else:
        fakes = Process(6,"fakes_mc","fakes_mc_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    flips = Process(7,"flips","flips_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    if do_ttww:
        TTWW = Process(8,"ttww","ttww_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    #overwrite nuisances
    lumiunc = "1.025"
    signal.lumi  = lumiunc
    signal.lephlt  = "1.04"
    signal.hlt  = "1.03"
    signal.jes  = "1"
    signal.jer  = "1"
    signal.lepeff  = "1.0"
    signal.hthlt  = "1"
    signal.btag = "1"
    signal.pu = "1"
    signal.scale = "1"
    signal.pdf = "1"
    ttz_sf = "1.40"
    ttw_sf = "1.40"
    wz_sf = "1.15"
    TTW.TTWSF          = ttw_sf
    TTW.lumi         = lumiunc
    TTW.isr         = "1"
    TTW.jes  = "1"
    TTW.jer  = "1"
    # TTW.lepeff  = "1.0"
    TTW.lephlt  = "1.04"
    TTW.hlt  = "1.03"
    # TTW.hthlt  = "1"
    TTW.btag = "1"
    TTW.pu = "1"
    TTZ.TTZSF          = ttz_sf
    TTZ.lumi          = lumiunc
    TTZ.isr  = "1"
    TTZ.jes  = "1"
    TTZ.jer  = "1"
    # TTZ.lepeff  = "1.0"
    TTZ.lephlt  = "1.04"
    TTZ.hlt  = "1.03"
    # TTZ.hthlt  = "1"
    TTZ.btag = "1"
    TTZ.pu = "1"
    # TTH.TTH          = "1.0"
    TTH.TTH          = "1.5"
    TTH.lumi          = lumiunc
    TTH.jes  = "1"
    TTH.jer  = "1"
    TTH.lepeff  = "1.0"
    TTH.lephlt  = "1.04"
    TTH.hlt  = "1.03"
    TTH.hthlt  = "1"
    TTH.btag = "1"
    TTH.pu = "1"
    TTWW.TTWW          = "2.0"
    TTWW.lumi          = lumiunc
    TTWW.jes  = "1"
    TTWW.jer  = "1"
    TTWW.lepeff  = "1.0"
    TTWW.lephlt  = "1.04"
    TTWW.hlt  = "1.03"
    TTWW.hthlt  = "1"
    TTWW.btag = "1"
    TTWW.pu = "1"
    # WZ.WZSF = wz_sf
    # WZ.jes  = "1"
    # WZ.btag = "1"
    # WZ.pu = "1"
    # WW.WW = "1.20"
    # WW.lumi = lumiunc
    # WW.jes  = "1"
    # if kine is "srcr": WW.lepeff  = "1.04"
    # WW.lephlt  = "1.04"
    # WW.hlt  = "1.03"
    # WW.hthlt  = "1"
    # WW.btag = "1"
    # WW.pu = "1"
    XG.XG = "1.50"
    XG.lumi = lumiunc
    XG.jes  = "1"
    XG.jer  = "1"
    XG.lepeff  = "1.0"
    XG.lephlt  = "1.04"
    XG.hlt  = "1.03"
    XG.hthlt  = "1"
    XG.btag = "1"
    XG.pu = "1"
    rares.rares = "1.50"
    rares.lumi = lumiunc
    rares.jes  = "1"
    rares.jer  = "1"
    rares.lepeff  = "1.0"
    rares.lephlt  = "1.04"
    rares.hlt  = "1.03"
    rares.hthlt  = "1"
    rares.btag = "1"
    rares.pu = "1"
    if not domcfakes:
        fakes.fakes = "1.30"
        fakes.fakes_EWK = "1"
    else:
        fakes.fakes = "1.20"
        fakes.fakes_EWK = "-"
    flips.flips = "1.20"

    #fill list of processes
    processes = []
    processes.append(signal)
    processes.append(TTW)
    processes.append(TTZ)
    processes.append(TTH)
    # processes.append(WZ)
    # processes.append(WW)
    processes.append(XG)
    processes.append(rares)
    processes.append(fakes)
    processes.append(flips)
    if do_ttww:
        processes.append(TTWW)

    if do_expected_data:
        # for proc in processes:
        #     print proc.name, proc.rate(), proc.bins()
        # tot_bins = map(sum,zip(*[proc.bins() for proc in processes]))
        tot_bins = map(sum,zip(*[map(lambda x: max(x,0.),proc.bins()) for proc in processes]))
        # print tot_bins, sum(tot_bins)
        # print data.rootf, data.name, data.rate()
        newdatafile = data.rootf.replace("data_","dataasimov_")
        fnew = ROOT.TFile(newdatafile,"RECREATE")
        newsr = ROOT.TH1F(plot,plot,len(tot_bins),0,len(tot_bins))
        for ibc,bc in enumerate(tot_bins): newsr.SetBinContent(ibc+1,bc)
        newsr.Write()
        fnew.Close()
        newdata = Process(-1,"data",newdatafile.rsplit("/",1)[-1],plot,thedir)
        data = newdata

    #create it
    writeOneCardFromProcesses(thedir, kine, plot, output, data, processes )

    # for proc in processes:
    #     print "-->", proc.name, proc.rate()
    #     for nuisance in dir(proc):
    #         if "__" in nuisance: continue
    #         if nuisance in ["rootf","rate","ratecache","plot","name","count"]: continue
    #         val = proc.__getattribute__(nuisance)
    #         if val == "-": continue
    #         print "\t", proc.name, nuisance, val


    return

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("directory", help="card directory")
    parser.add_argument("-o", "--output", help="output card name", default="card_tttt.txt")
    args = parser.parse_args()

    # thedir = "v0.02_Apr26_test"
    writeOneCard(args.directory,args.output)
