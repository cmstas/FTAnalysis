#!/usr/bin/env python

import ROOT
import sys
import os
import subprocess
import math
import glob
import argparse

do_uncorrfakes = False

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
        self.bb  = "-"
        self.met  = "-"
        self.scale  = "-"
        self.alphas  = "-"
        self.isrvar  = "-"
        self.fsrvar  = "-"
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
        self.TTTT = "-"
        self.TTVV = "-"
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

def writeUncorrelatedFakesForProcess(thedir, card, kine, process, processes, lnN=1.3):
    if process.name=="sig": return #fake signal for MI limits does not need stat unc.
    ra5 = ""
    # open file first to read. if one of the hists exists, then we've already called this function
    # and no need to open the file again and write
    f = ROOT.TFile(process.rootf)

    name = "_uncorrfakes_"

    h_test = "%s%s%s%s%sUp" % (ra5,process.name,name,kine,1)
    bypass = True
    if not f.Get(h_test) or bypass:
        f = ROOT.TFile(process.rootf,"UPDATE")

    h = f.Get("sr")

    for ibin in range(1,h.GetNbinsX()+1):
        card.write ("%-40s %-5s " % (("%s%s%s%s%s" % (ra5,process.name,name,kine,ibin)), "shape") )
        for myprocess in processes:
            if myprocess.count == process.count:
                card.write("%-15s " % "1")
                cent = h.GetBinContent(ibin)
                hupnewtest = f.Get("%s%s%s%s%sUp" % (ra5,process.name,name,kine,ibin))
                if not hupnewtest or bypass:
                    hupnew = h.Clone("%s%s%s%s%sUp" % (ra5,process.name,name,kine,ibin))
                    hupnew.SetBinContent(ibin,cent*lnN)
                    hupnew.Write()
                hdnnewtest = f.Get("%s%s%s%s%sDown" % (ra5,process.name,name,kine,ibin))
                if not hdnnewtest or bypass:
                    hdnnew = h.Clone("%s%s%s%s%sDown" % (ra5,process.name,name,kine,ibin))
                    hdnnew.SetBinContent(ibin,max(cent/lnN,1.e-6))
                    hdnnew.Write()
            else:  card.write("%-15s " % ("-"))
        card.write("\n")

    if not h_test:
        f.Close()

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
    hup = f.Get(("%s_statUp" % (process.name.replace("_bkg",""))))
    hdn = f.Get(("%s_statDown" % (process.name.replace("_bkg",""))))

    if not hup:
        # print "%s_statUp" % (process.name), hup, process.rootf
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

    #nuisance TTTT
    card.write("%-40s %-5s " % ("TTTT","lnN"))
    for process in processes: card.write("%-15s " % (process.TTTT))
    card.write("\n")
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
    #nuisance bb
    card.write("%-40s %-5s " % ("bb","shape"))
    for process in processes: card.write("%-15s " % (process.bb))
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

    #nuisance isrvar
    card.write("%-40s %-5s " % ("isrvar","shape"))
    for process in processes: card.write("%-15s " % (process.isrvar))
    card.write("\n")
    #nuisance fsrvar
    card.write("%-40s %-5s " % ("fsrvar","shape"))
    for process in processes: card.write("%-15s " % (process.fsrvar))
    card.write("\n")
    #nuisance scale
    card.write("%-40s %-5s " % ("scale","shape"))
    for process in processes: card.write("%-15s " % (process.scale))
    card.write("\n")
    #nuisance pdf
    card.write("%-40s %-5s " % ("pdf","shape"))
    for process in processes: card.write("%-15s " % (process.pdf))
    card.write("\n")
    #nuisance alphas
    card.write("%-40s %-5s " % ("alphas","shape"))
    for process in processes: card.write("%-15s " % (process.alphas))
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

    #nuisance TTVV
    card.write("%-40s %-5s " % ("TTVV","lnN"))
    for process in processes: card.write("%-15s " % (process.TTVV))
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
        if process.name in ["ttw","ttz","tth"]:
            statshape = 0.15 # ttw, ttz, tth, wz, tttt
            # if process.name in ["ww"]: statshape = 0.20
            process.statshape = 1.+statshape
            writeStatForProcess(thedir,card,kine,process,processes, statshape=statshape)
        else:
            writeStatForProcess(thedir,card,kine,process,processes)
        if process.name in ["fakes"] and do_uncorrfakes:
            writeUncorrelatedFakesForProcess(thedir,card,kine,process,processes,lnN=1.6)

    return

def writeOneCard(thedir, output, signal="tttt", kine="srcr", plot="sr", domcfakes=False, do_expected_data=False, yukawa=-1, inject_tttt=False):
    # if we're not using tttt as the signal, then want to include tttt as a bg (--> inject_tttt = True) 
    inject_tttt = (signal != "tttt") or inject_tttt
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
    if yukawa > 0:
        TTH = Process(3,"tth_yt"+str(yukawa),"tth_yt"+str(yukawa)+"_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    else:
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
    TTVV = Process(8,"ttvv","ttvv_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    if inject_tttt:
        os.system("cp {} {}".format(signal.rootf,signal.rootf.replace("tttt_","tttt_bkg_")))
        TTTT = Process(9,"tttt_bkg","tttt_bkg_histos_"+kine+"_"+"*"+"ifb.root",plot,thedir)
    #overwrite nuisances
    lumiunc = "1.025"
    signal.lumi  = "1"
    signal.lephlt  = "1.04"
    signal.hlt  = "1.03"
    signal.jes  = "1"
    signal.jer  = "1"
    signal.lepeff  = "1.0"
    signal.hthlt  = "1"
    signal.btag = "1"
    signal.pu = "1"
    signal.scale = "1"
    signal.alphas = "1"
    signal.isrvar = "1"
    signal.fsrvar = "1"
    signal.pdf = "1"
    ttz_sf = "1.40"
    ttw_sf = "1.40"
    wz_sf = "1.15"
    TTW.TTWSF          = ttw_sf
    TTW.lumi         = lumiunc
    TTW.isr         = "1"
    TTW.bb         = "1"
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
    TTZ.bb  = "1"
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
    # TTH.TTH          = "0.936/1.099" # FIXME FIXME FIXME
    TTH.lumi          = lumiunc
    TTH.bb  = "1"
    TTH.jes  = "1"
    TTH.jer  = "1"
    TTH.lepeff  = "1.0"
    TTH.lephlt  = "1.04"
    TTH.hlt  = "1.03"
    TTH.hthlt  = "1"
    TTH.btag = "1"
    TTH.pu = "1"
    TTVV.TTVV          = "1.5"
    TTVV.lumi          = lumiunc
    TTVV.jes  = "1"
    TTVV.jer  = "1"
    TTVV.lepeff  = "1.0"
    TTVV.lephlt  = "1.04"
    TTVV.hlt  = "1.03"
    TTVV.hthlt  = "1"
    TTVV.btag = "1"
    TTVV.pu = "1"
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
    if inject_tttt:
        TTTT.TTTT = "0.79/1.18" # - In 2017, latest NLO(QCD+EWK) calculation (arxiv:1711.02116).  - 11.97 +2.15 -2.51
        TTTT.lumi  = "1"
        TTTT.lephlt  = "1.04"
        TTTT.hlt  = "1.03"
        TTTT.jes  = "1"
        TTTT.jer  = "1"
        TTTT.lepeff  = "1.0"
        TTTT.hthlt  = "1"
        TTTT.btag = "1"
        TTTT.pu = "1"
        TTTT.scale = "1"
        TTTT.alphas = "1"
        TTTT.pdf = "1"
    if not domcfakes:
        if not do_uncorrfakes:
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
    processes.append(TTVV)
    if inject_tttt:
        processes.append(TTTT)

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
