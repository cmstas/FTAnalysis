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
        # self.rootf = glob.glob(thedir+"/"+myrootf)[0]
        self.rootf = thedir+"/"+myrootf
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
        # self.lephlt  = "-"
        self.hthlt  = "-"
        self.trigsf  = "-"
        self.prefire  = "-"
        self.fs_hlt  = "-"
        self.fs_lepeff  = "-"
        self.btag = "-"
        self.btagheavy = "-"
        self.btaglight = "-"
        self.pu = "-"
        self.TTWSF = "-"
        self.TTZSF = "-"
        self.WZSF = "-"
        self.TTH = "-"
        self.TTTT = "-"
        self.TTVV = "-"
        self.WW = "-"
        self.XG = "-"
        self.rares = "-"
        self.fakes = "-"
        self.fakes_EWK = "-"
        self.fakesnb0 = "-"
        self.fakesnb1 = "-"
        self.fakesnb2 = "-"
        self.fakesnb3 = "-"
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

def writeAutoMCStats(card, thresh=0.0):
    card.write("SS autoMCStats {}\n".format(thresh))

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
def writeOneCardFromProcesses(thedir, kine, plot, output, data, processes, thresh=0.0, use_autostats=False, ignorefakes=False, analysis="ft", inject_tttt=False):

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

    if inject_tttt:
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
    if analysis == "ft":
        #nuisance jer
        card.write("%-40s %-5s " % ("jer","shape"))
        for process in processes: card.write("%-15s " % (process.jer))
        card.write("\n")
    #nuisance isr
    card.write("%-40s %-5s " % ("isr","shape"))
    for process in processes: card.write("%-15s " % (process.isr))
    card.write("\n")
    # if analysis == "ft":
    #nuisance bb
    card.write("%-40s %-5s " % ("bb","shape"))
    for process in processes: card.write("%-15s " % (process.bb))
    card.write("\n")
    if analysis == "ss":
        # met genmet for fastsim
        card.write("%-40s %-5s " % ("met","shape"))
        for process in processes: card.write("%-15s " % (process.met))
        card.write("\n")
    #nuisance lepeff
    card.write("%-40s %-5s " % ("lep","shape"))
    for process in processes: card.write("%-15s " % (process.lepeff))
    card.write("\n")
    ##nuisance lephlt
    #card.write("%-40s %-5s " % ("lephlt","lnN"))
    #for process in processes: card.write("%-15s " % (process.lephlt))
    #card.write("\n")
    if analysis == "ss":
        #nuisance fs_lepeff
        card.write("%-40s %-5s " % ("fs_lepeff","lnN"))
        for process in processes: card.write("%-15s " % (process.fs_lepeff))
        card.write("\n")
        #nuisance fs_hlt
        card.write("%-40s %-5s " % ("fs_hlt","lnN"))
        for process in processes: card.write("%-15s " % (process.fs_hlt))
        card.write("\n")

    use_trigsf = True
    if use_trigsf:
        #nuisance trigsf
        card.write("%-40s %-5s " % ("trig","shape"))
        for process in processes: card.write("%-15s " % (process.trigsf))
        card.write("\n")
    else:
        #nuisance hthlt
        card.write("%-40s %-5s " % ("hthlt","shape"))
        for process in processes: card.write("%-15s " % (process.hthlt))
        card.write("\n")

    use_prefireunc = True
    if use_prefireunc:
        #nuisance prefire
        card.write("%-40s %-5s " % ("prefire","shape"))
        for process in processes: card.write("%-15s " % (process.prefire))
        card.write("\n")

    split_btagsf = True
    if split_btagsf:
        #nuisance btagheavy
        card.write("%-40s %-5s " % ("btaghf","shape"))
        for process in processes: card.write("%-15s " % (process.btagheavy))
        card.write("\n")
        #nuisance btaglight
        card.write("%-40s %-5s " % ("btaglf","shape"))
        for process in processes: card.write("%-15s " % (process.btaglight))
        card.write("\n")
    else:
        #nuisance btag
        card.write("%-40s %-5s " % ("btag","shape"))
        for process in processes: card.write("%-15s " % (process.btag))
        card.write("\n")

    if analysis == "ft":
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
    if analysis == "ft":
        #nuisance pdf
        card.write("%-40s %-5s " % ("pdf","shape"))
        for process in processes: card.write("%-15s " % (process.pdf))
        card.write("\n")
        #nuisance alphas
        card.write("%-40s %-5s " % ("alphas","shape"))
        for process in processes: card.write("%-15s " % (process.alphas))
        card.write("\n")
    else:
        #nuisance pdf
        card.write("%-40s %-5s " % ("pdf","lnN"))
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

    if analysis == "ft":
        #nuisance TTVV
        card.write("%-40s %-5s " % ("TTVV","lnN"))
        for process in processes: card.write("%-15s " % (process.TTVV))
        card.write("\n")

    if analysis == "ss":

        #nuisance WZ
        card.write("%-40s %-5s " % ("WZSF","lnN"))
        for process in processes: card.write("%-15s " % (process.WZSF))
        card.write("\n")


        #nuisance WW
        card.write("%-40s %-5s " % ("WW","lnN"))
        for process in processes: card.write("%-15s " % (process.WW))
        card.write("\n")

    #nuisance XG
    card.write("%-40s %-5s " % ("XG","lnN"))
    for process in processes: card.write("%-15s " % (process.XG))
    card.write("\n")

    #nuisance RARES
    card.write("%-40s %-5s " % ("rares","lnN"))
    for process in processes: card.write("%-15s " % (process.rares))
    card.write("\n")

    add_fakes_total = True
    add_fakes_nb = False
    if analysis == "ss":
        # FIXME
        add_fakes_total = False
        add_fakes_nb = True

    #nuisance fakes EWK
    card.write("%-40s %-5s " % ("fakes_EWK","shape"))
    for process in processes: card.write("%-15s " % (process.fakes_EWK))
    card.write("\n")
    if add_fakes_total:
        #nuisance fakes
        card.write("%-40s %-5s " % ("fakes","lnN"))
        for process in processes: card.write("%-15s " % (process.fakes))
        card.write("\n")
    if add_fakes_nb:
        #nuisance fakes
        card.write("%-40s %-5s " % ("fakes_normNB0","shape"))
        for process in processes: card.write("%-15s " % (process.fakesnb0))
        card.write("\n")
        card.write("%-40s %-5s " % ("fakes_normNB1","shape"))
        for process in processes: card.write("%-15s " % (process.fakesnb1))
        card.write("\n")
        card.write("%-40s %-5s " % ("fakes_normNB2","shape"))
        for process in processes: card.write("%-15s " % (process.fakesnb2))
        card.write("\n")
        card.write("%-40s %-5s " % ("fakes_normNB3","shape"))
        for process in processes: card.write("%-15s " % (process.fakesnb3))
        card.write("\n")

    #nuisance flips
    card.write("%-40s %-5s " % ("flips","lnN"))
    for process in processes: card.write("%-15s " % (process.flips))
    card.write("\n")

    if use_autostats:
        writeAutoMCStats(card,thresh=thresh)
    else:
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

    # if rateparams:
    #     card.write("tthscale rateParam * tth 1.0 [1.0,1.0]\n")
    #     card.write("lumiscale rateParam * * 1.0 [1.0,1.0]\n")

    return

def writeOneCard(thedir, output, signal="tttt", kine="srcr", plot="sr", domcfakes=False, do_expected_data=False, inject_tttt=False, use_autostats=True, thresh=0.0,year=-1, ignorefakes=False,analysis="ft"):
    signame = signal[:]
    # For SS, tttt is already in rares # NOTE
    if analysis == "ft":
        # if we're not using tttt as the signal, then want to include tttt as a bg (--> do_tttt = True) 
        inject_tttt = (signal != "tttt") or inject_tttt
    # do_tttt = True
    #define processes (signal first)
    # if pseudoData:
    #     print "Using pseudo data!"
    #     data = Process(-1,"data","pdata_histos_"+kine+"_"+"ifb.root",plot,thedir)
    # else:
        # print "Using real data!"
    tomatch = "ifb"
    if year > 0:
        tomatch = str(year)
    num = -1
    data = Process(num,"data","data_histos_"+kine+"_"+tomatch+".root",plot,thedir)
    num += 1
    signal = Process(num,signame,signame+"_histos_"+kine+"_"+tomatch+".root",plot,thedir)
    num += 1
    TTW = Process(num,"ttw","ttw_histos_"+kine+"_"+tomatch+".root",plot,thedir)
    num += 1
    TTZ = Process(num,"ttz","ttz_histos_"+kine+"_"+tomatch+".root",plot,thedir)
    num += 1
    TTH = Process(num,"tth","tth_histos_"+kine+"_"+tomatch+".root",plot,thedir)
    num += 1
    if analysis == "ss":
        WZ  = Process(num,"wz","wz_histos_"+kine+"_"+tomatch+".root",plot,thedir)
        num += 1
        WW  = Process(num,"ww","ww_histos_"+kine+"_"+tomatch+".root",plot,thedir)
        num += 1
    XG  = Process(num,"xg","xg_histos_"+kine+"_"+tomatch+".root",plot,thedir)
    num += 1
    rares = Process(num,"rares","rares_histos_"+kine+"_"+tomatch+".root",plot,thedir)
    num += 1
    if not domcfakes:
        fakes = Process(num,"fakes","fakes_histos_"+kine+"_"+tomatch+".root",plot,thedir)
        num += 1
    else:
        fakes = Process(num,"fakes_mc","fakes_mc_histos_"+kine+"_"+tomatch+".root",plot,thedir)
        num += 1
    flips = Process(num,"flips","flips_histos_"+kine+"_"+tomatch+".root",plot,thedir)
    num += 1
    if analysis == "ft":
        TTVV = Process(num,"ttvv","ttvv_histos_"+kine+"_"+tomatch+".root",plot,thedir)
        num += 1
    if inject_tttt:
        if analysis == "ss":
            os.system("cp {} {}".format(signal.rootf,signal.rootf.replace("tttt_","tttt_bkg_")))
            TTTT = Process(9,"tttt_bkg","tttt_bkg_histos_"+kine+"_"+tomatch+".root",plot,thedir)
        else:
            # os.system("cp {} {}".format(signal.rootf,signal.rootf.replace("tttt_","tttt_bkg_")))
            # TTTT = Process(9,"tttt_bkg","tttt_bkg_histos_"+kine+"_"+tomatch+".root",plot,thedir)
            TTTT = Process(9,"tttt","tttt_histos_"+kine+"_"+tomatch+".root",plot,thedir)
        num += 1
    #overwrite nuisances
    if year == 2016: lumiunc = "1.025"
    if year == 2017: lumiunc = "1.023"
    if year == 2018: lumiunc = "1.025"
    signal.lumi  = "1"
    # signal.lephlt  = "1.04"
    signal.hlt  = "1.03"
    signal.jes  = "1"
    signal.jer  = "1"
    signal.lepeff  = "1"
    signal.hthlt  = "1"
    signal.trigsf  = "1"
    if year != 2018: signal.prefire  = "1"
    signal.btag = "1"
    signal.btagheavy = "1"
    signal.btaglight = "1"
    signal.pu = "1"
    if analysis == "ft":
        signal.scale = "1"
        signal.pdf = "1"
        signal.alphas = "1"
    if analysis == "ss" and ("fs_" in signame):
        signal.fs_hlt = "1.05"
        signal.isr = "1"
        if kine == "srhh": signal.fs_lepeff = "1.06"
        if kine == "srhl": signal.fs_lepeff = "1.10"
        if kine == "srll": signal.fs_lepeff = "1.12"
        if kine == "srml": signal.fs_lepeff = "1.10"
        if kine == "srlm": signal.fs_lepeff = "1.12"
        signal.met = "1"
        signal.scale = "1"
        signal.pdf = "1.02"
    signal.isrvar = "1"
    signal.fsrvar = "1"
    if analysis == "ss":
        ttz_sf = "1.30"
        wz_sf = "1.30"
        ttw_sf = "1.30"
    else:
        ttz_sf = "1.40"
        ttw_sf = "1.40"
    TTW.TTWSF          = ttw_sf
    TTW.lumi         = lumiunc
    if analysis == "ft":
        TTW.isr         = "1"
    TTW.scale         = "1"
    if analysis == "ft":
        TTW.pdf         = "1"
    # TTW.alphas         = "1"
    TTW.bb         = "1"
    TTW.jes  = "1"
    TTW.jer  = "1"
    TTW.lepeff  = "1"
    # TTW.lephlt  = "1.04"
    TTW.hlt  = "1.03"
    if analysis == "ss":
        TTW.hthlt  = "1"
    TTW.trigsf  = "1"
    if year != 2018: TTW.prefire  = "1"
    TTW.btag = "1"
    TTW.btagheavy = "1"
    TTW.btaglight = "1"
    TTW.pu = "1"
    TTZ.TTZSF          = ttz_sf
    TTZ.lumi          = lumiunc
    if analysis == "ft":
        TTZ.isr  = "1"
    TTZ.bb  = "1"
    TTZ.jes  = "1"
    TTZ.jer  = "1"
    TTZ.scale  = "1"
    if analysis == "ft":
        TTZ.pdf  = "1"
    # TTZ.alphas  = "1"
    TTZ.lepeff  = "1"
    # TTZ.lephlt  = "1.04"
    TTZ.hlt  = "1.03"
    if analysis == "ss":
        TTZ.hthlt  = "1"
    TTZ.trigsf  = "1"
    if year != 2018: TTZ.prefire  = "1"
    TTZ.btag = "1"
    TTZ.btagheavy = "1"
    TTZ.btaglight = "1"
    TTZ.pu = "1"
    # TTH.TTH          = "1.0"
    if analysis == "ss":
        TTH.TTH          = "1.3"
    else:
        TTH.TTH          = "1.25"
    TTH.lumi          = lumiunc
    TTH.bb  = "1"
    TTH.jes  = "1"
    TTH.jer  = "1"
    TTH.lepeff  = "1"
    # TTH.lephlt  = "1.04"
    TTH.scale  = "1"
    if analysis == "ft":
        TTH.pdf  = "1"
    # TTH.alphas  = "1"
    TTH.hlt  = "1.03"
    TTH.hthlt  = "1"
    TTH.trigsf  = "1"
    if year != 2018: TTH.prefire  = "1"
    TTH.btag = "1"
    TTH.btagheavy = "1"
    TTH.btaglight = "1"
    TTH.pu = "1"
    if analysis == "ft":
        TTVV.TTVV          = "1.11"
        TTVV.lumi          = lumiunc
        TTVV.jes  = "1"
        TTVV.jer  = "1"
        TTVV.lepeff  = "1"
        # TTVV.lephlt  = "1.04"
        TTVV.hlt  = "1.03"
        TTVV.hthlt  = "1"
        TTVV.trigsf  = "1"
        if year != 2018: TTVV.prefire  = "1"
        TTVV.btag = "1"
        TTVV.btagheavy = "1"
        TTVV.btaglight = "1"
        TTVV.pu = "1"
        TTVV.scale  = "1"
        TTVV.pdf  = "1"
        XG.XG = "1.11"
    if analysis == "ss":
        WZ.WZSF = wz_sf
        WZ.lumi = lumiunc
        WZ.jes  = "1"
        WZ.btag = "1"
        WZ.btagheavy = "1"
        WZ.btaglight = "1"
        WZ.pu = "1"
        WZ.trigsf  = "1"
        if year != 2018: WZ.prefire  = "1"
        WW.WW = "1.30"
        WW.lumi = lumiunc
        WW.jes  = "1"
        WW.jer  = "1"
        WW.lepeff  = "1"
        # if kine is "srcr": WW.lepeff  = "1.04"
        # WW.lephlt  = "1.04"
        WW.hlt  = "1.03"
        WW.hthlt  = "1"
        WW.trigsf  = "1"
        if year != 2018: WW.prefire  = "1"
        WW.btag = "1"
        WW.btagheavy = "1"
        WW.btaglight = "1"
        WW.pu = "1"
        XG.XG = "1.50"
    XG.lumi = lumiunc
    XG.jes  = "1"
    XG.jer  = "1"
    XG.lepeff  = "1"
    # XG.lephlt  = "1.04"
    XG.hlt  = "1.03"
    XG.hthlt  = "1"
    XG.trigsf  = "1"
    if year != 2018: XG.prefire  = "1"
    XG.btag = "1"
    XG.btagheavy = "1"
    XG.btaglight = "1"
    XG.pu = "1"
    if analysis == "ft":
        XG.scale  = "1"
        XG.pdf  = "1"
        rares.rares = "1.20"
    else:
        rares.rares = "1.50"
    rares.lumi = lumiunc
    rares.jes  = "1"
    rares.jer  = "1"
    rares.lepeff  = "1"
    # rares.lephlt  = "1.04"
    rares.hlt  = "1.03"
    rares.hthlt  = "1"
    rares.trigsf  = "1"
    if year != 2018: rares.prefire  = "1"
    rares.btag = "1"
    rares.btagheavy = "1"
    rares.btaglight = "1"
    rares.pu = "1"
    if analysis == "ft":
        rares.scale  = "1"
        rares.pdf  = "1"
    if inject_tttt:
        # TTTT.TTTT = "0.79/1.18" # - In 2017, latest NLO(QCD+EWK) calculation (arxiv:1711.02116).  - 11.97 +2.15 -2.51
        TTTT.TTTT = "1.2" # - In 2017, latest NLO(QCD+EWK) calculation (arxiv:1711.02116).  - 11.97 +2.15 -2.51
        TTTT.lumi  = lumiunc
        # TTTT.lephlt  = "1.04"
        TTTT.hlt  = "1.03"
        TTTT.jes  = "1"
        TTTT.jer  = "1"
        TTTT.lepeff  = "1"
        TTTT.hthlt  = "1"
        TTTT.trigsf  = "1"
        if year != 2018: TTTT.prefire  = "1"
        TTTT.btag = "1"
        TTTT.btagheavy = "1"
        TTTT.btaglight = "1"
        TTTT.pu = "1"
        TTTT.scale = "1"
        TTTT.alphas = "1"
        TTTT.pdf = "1"
        if analysis == "ft":
            signal.isrvar = "-"
            signal.fsrvar = "-"
            signal.alphas = "-"
    if not domcfakes:
        if not do_uncorrfakes:
            fakes.fakes = "1.30"
        fakes.fakes_EWK = "1"
        fakes.fakesnb0 = "1"
        fakes.fakesnb1 = "1"
        fakes.fakesnb2 = "1"
        fakes.fakesnb3 = "1"
    else:
        fakes.fakes = "1.30"
        # fakes.fakes_EWK = "-"
    flips.flips = "1.20"

    #fill list of processes
    processes = []
    processes.append(signal)
    processes.append(TTW)
    processes.append(TTZ)
    processes.append(TTH)
    if analysis == "ss":
        processes.append(WZ)
        processes.append(WW)
    processes.append(XG)
    processes.append(rares)
    if not ignorefakes: processes.append(fakes)
    processes.append(flips)
    if analysis == "ft":
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
    writeOneCardFromProcesses(thedir, kine, plot, output, data, processes, thresh=thresh, use_autostats=use_autostats, ignorefakes=ignorefakes, analysis=analysis,inject_tttt=inject_tttt )

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
    parser.add_argument("-o", "--output", help="output card name", default="card.txt")
    parser.add_argument("-y", "--year", help="year", default=-1, type=int)
    parser.add_argument("-a", "--analysis", help="analysis: 'ss'/'ft'", default="ft")
    parser.add_argument("-r", "--regions", help="srcr, srdisc, srhh, srhl, etc (default: %(default)s)", default="srcr")
    # parser.add_argument("-s", "--sig", help="signal name (default: %(default)s)", default="fs_t1tttt_m1600_m500")
    parser.add_argument("-s", "--sig", help="signal name (default: %(default)s)", default="tttt")
    args = parser.parse_args()

    regions = str(args.regions)
    outputname = str(args.output)

    # thedir = "v0.02_Apr26_test"
    writeOneCard(args.directory,args.output,year=args.year,analysis=args.analysis,signal=args.sig,kine=args.regions)
