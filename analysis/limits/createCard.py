import ROOT
import sys
import os
import subprocess
import math

lumi = "35.9"

if "LUMI" in os.environ:
    lumi = os.environ["LUMI"]

pseudoData = 0

doComb = False
doMetUnc = False

class Process(object):
    def __init__(self, mycount, myname, myrootf, myplot):
        self.count = mycount
        self.name = myname
        self.rootf = myrootf
        self.plot = myplot
        self.lumi  = "-"
        self.jes  = "-"
        self.isr  = "-"
        self.met  = "-"
        self.scale  = "-"
        self.fs_lep  = "-"
        self.fs_hlt  = "-"
        self.lepeff  = "-"
        self.hlt  = "-"
        self.lephlt  = "-"
        self.hthlt  = "-"
        self.btag = "-"
        self.pu = "-"
        self.TTWSF = "-"
        self.TTZSF = "-"
        self.WZSF = "-"
        self.WW = "-"
        self.XG = "-"
        self.rares = "-"
        self.fakes = "-"
        self.fakes_EWK = "-"
        self.flips = "-"
        self.stat = "x"
        self.statshape = "-"
        self.ratecache = None
    def rate(self):
        if self.ratecache is not None:
            return self.ratecache
        else:
            f = ROOT.TFile(thedir+"/"+self.rootf)
            if f.Get(self.plot):
                self.ratecache = f.Get(self.plot).Integral()
                return self.ratecache
            else:
                print self.plot+" not found in "+self.rootf
                return 0

def writeStatForProcess(thedir, card, kine, process, processes, statshape=None):
    if process.name=="sig": return #fake signal for MI limits does not need stat unc.
    ra5 = ""
    if doComb: ra5 = "ra5_"
    # open file first to read. if one of the hists exists, then we've already called this function
    # and no need to open the file again and write
    f = ROOT.TFile(thedir+"/"+process.rootf)

    name = "_stat_"
    if statshape is not None:
        name = "_statshape_"

    h_test = "%s%s%s%s%sUp" % (ra5,process.name,name,kine,1)
    bypass = False
    if not f.Get(h_test) or bypass:
        f = ROOT.TFile(thedir+"/"+process.rootf,"UPDATE")

    h = f.Get("sr")
    hup = f.Get(("%s_statUp" % (process.name)))
    hdn = f.Get(("%s_statDown" % (process.name)))

    if hup:
        for bin in range(1,hup.GetNbinsX()+1):
            card.write ("%-40s %-5s " % (("%s%s%s%s%s" % (ra5,process.name,name,kine,bin)), "shape") )
            for myprocess in processes:
                if myprocess.count == process.count:
                    card.write("%-15s " % "1")
                    hupnewtest = f.Get("%s%s%s%s%sUp" % (ra5,process.name,name,kine,bin))
                    if not hupnewtest or bypass:
                        hupnew = h.Clone("%s%s%s%s%sUp" % (ra5,process.name,name,kine,bin))
                        if statshape is None:
                            hupnew.SetBinContent(bin,hup.GetBinContent(bin))
                        else:
                            cent = h.GetBinContent(bin)
                            up_stat = hup.GetBinContent(bin)
                            dup_stat = up_stat-cent
                            dup_scale = statshape*cent # statshape must be 0.3 for 30% up or something
                            up = cent + (dup_stat**2.0+dup_scale**2.0)**0.5
                            hupnew.SetBinContent(bin,up)
                        hupnew.Write()
                    hdnnewtest = f.Get("%s%s%s%s%sDown" % (ra5,process.name,name,kine,bin))
                    if not hdnnewtest or bypass:
                        hdnnew = h.Clone("%s%s%s%s%sDown" % (ra5,process.name,name,kine,bin))
                        if statshape is None:
                            if hdn.GetBinContent(bin) > 0: hdnnew.SetBinContent(bin,hdn.GetBinContent(bin))
                            else: hdnnew.SetBinContent(bin,0)
                        else:
                            cent = h.GetBinContent(bin)
                            down_stat = hdn.GetBinContent(bin)
                            ddown_stat = cent-down_stat
                            ddown_scale = statshape*cent # statshape must be 0.3 for 30% down or something
                            down = cent - (ddown_stat**2.0+ddown_scale**2.0)**0.5
                            hdnnew.SetBinContent(bin,max(down,1.e-6))
                        hdnnew.Write()
                else:  card.write("%-15s " % ("-"))
            card.write("\n")
    else:
        print hup.GetName()+" not found in "+process.rootf
        return 0
    if not h_test:
        f.Close()


#write card regardless of number of processes (but make sure signal is first in list)
def writeOneCardFromProcesses(thedir, kine, plot, output, data, processes):
    line = "---------------------------------------------------------------"
    bin = "SS"
    card = open(str(thedir)+'/'+str(output), 'w')
    card.write("imax 1  number of channels \n")
    card.write("jmax *  number of backgrounds \n")
    card.write("kmax *  number of nuisance parameters \n")
    card.write(line+"\n")
    for process in processes:
        card.write("shapes "+process.name+" * "+thedir+"/"+process.rootf+" "+plot+" $SYSTEMATIC\n")
    card.write("shapes data_obs * "+thedir+"/"+data.rootf+" "+plot+" "+plot+"\n")
    card.write(line+"\n")
    card.write("bin "+str(bin)+"\n")
    #pseudo data for now
    card.write("observation %-15.3f \n" % (data.rate()))
    card.write(line+"\n")
    #bin
    card.write("%-40s %-5s " % ("bin",""))
    for process in processes: card.write("%-15s " % (bin))
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
    #nuisance isr
    card.write("%-40s %-5s " % ("isr","shape"))
    for process in processes: card.write("%-15s " % (process.isr))
    card.write("\n")
    #nuisance scale
    card.write("%-40s %-5s " % ("scale","shape"))
    for process in processes: card.write("%-15s " % (process.scale))
    card.write("\n")
    #nuisance fs_lep
    card.write("%-40s %-5s " % ("fs_lep","lnN"))
    for process in processes: card.write("%-15s " % (process.fs_lep))
    card.write("\n")
    #nuisance fs_hlt
    card.write("%-40s %-5s " % ("fs_hlt","lnN"))
    for process in processes: card.write("%-15s " % (process.fs_hlt))
    card.write("\n")
    #nuisance lepeff
    card.write("%-40s %-5s " % ("lepeff","lnN"))
    for process in processes: card.write("%-15s " % (process.lepeff))
    card.write("\n")
    if doComb == True:
        #nuisance hlt
        card.write("%-40s %-5s " % ("hlt","lnN"))
        for process in processes: card.write("%-15s " % (process.hlt))
        card.write("\n")
    else:
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
        if process.name in ["ttw","ttz","tth","wz","ww"]:
            statshape = 0.15 # ttw, ttz, tth, wz, tttt
            if process.name in ["ww"]: statshape = 0.20
            process.statshape = 1.+statshape
            writeStatForProcess(thedir,card,kine,process,processes, statshape=statshape)
        else:
            writeStatForProcess(thedir,card,kine,process,processes)

    return

def writeOneCard(thedir, signal, kine, plot, output):
    #define processes (signal first)
    if pseudoData:
        print "Using pseudo data!"
        data = Process(-1,"data","pdata_histos_"+kine+"_"+lumi+"ifb.root",plot)
    else:
        # print "Using real data!"
        data = Process(-1,"data","data_histos_"+kine+"_"+lumi+"ifb.root",plot)
    signal = Process(0,signal,signal+"_histos_"+kine+"_"+lumi+"ifb.root",plot)
    TTW = Process(1,"ttw","ttw_histos_"+kine+"_"+lumi+"ifb.root",plot)
    TTZ = Process(2,"ttz","ttz_histos_"+kine+"_"+lumi+"ifb.root",plot)
    TTH = Process(3,"tth","tth_histos_"+kine+"_"+lumi+"ifb.root",plot)
    WZ  = Process(4,"wz","wz_histos_"+kine+"_"+lumi+"ifb.root",plot)
    WW  = Process(5,"ww","ww_histos_"+kine+"_"+lumi+"ifb.root",plot)
    XG  = Process(6,"xg","xg_histos_"+kine+"_"+lumi+"ifb.root",plot)
    rares = Process(7,"rares","rares_histos_"+kine+"_"+lumi+"ifb.root",plot)
    fakes = Process(8,"fakes","fakes_histos_"+kine+"_"+lumi+"ifb.root",plot)
    flips = Process(9,"flips","flips_histos_"+kine+"_"+lumi+"ifb.root",plot)
    #overwrite nuisances
    lumiunc = "1.026"
    signal.lumi  = lumiunc
    signal.lephlt  = "1.02"
    signal.hlt  = "1.03"
    signal.jes  = "1"
    signal.lepeff  = "1.04"
    signal.hthlt  = "1"
    signal.btag = "1"
    signal.pu = "1"
    ttz_sf = "1.40"
    ttw_sf = "1.40"
    wz_sf = "1.15"
    TTW.TTWSF          = ttw_sf
    TTW.lumi         = lumiunc
    TTW.jes  = "1"
    if kine is "srcr": TTW.lepeff  = "1.04"
    TTW.lephlt  = "1.02"
    TTW.hlt  = "1.03"
    # TTW.hthlt  = "1"
    TTW.btag = "1"
    TTW.pu = "1"
    TTZ.TTZSF          = ttz_sf
    TTZ.lumi          = lumiunc
    TTZ.jes  = "1"
    if kine is "srcr": TTZ.lepeff  = "1.04"
    TTZ.lephlt  = "1.02"
    TTZ.hlt  = "1.03"
    # TTZ.hthlt  = "1"
    TTZ.btag = "1"
    TTZ.pu = "1"
    TTH.TTH          = "1.0"
    TTH.lumi          = lumiunc
    TTH.jes  = "1"
    if kine is "srcr": TTH.lepeff  = "1.04"
    TTH.lephlt  = "1.02"
    TTH.hlt  = "1.03"
    TTH.hthlt  = "1"
    TTH.btag = "1"
    TTH.pu = "1"
    WZ.WZSF = wz_sf
    WZ.jes  = "1"
    WZ.btag = "1"
    WZ.pu = "1"
    WW.WW = "1.20"
    WW.lumi = lumiunc
    WW.jes  = "1"
    if kine is "srcr": WW.lepeff  = "1.04"
    WW.lephlt  = "1.02"
    WW.hlt  = "1.03"
    WW.hthlt  = "1"
    WW.btag = "1"
    WW.pu = "1"
    XG.XG = "1.50"
    XG.lumi = lumiunc
    XG.jes  = "1"
    if kine is "srcr": XG.lepeff  = "1.04"
    XG.lephlt  = "1.02"
    XG.hlt  = "1.03"
    XG.hthlt  = "1"
    XG.btag = "1"
    XG.pu = "1"
    rares.rares = "1.50"
    rares.lumi = lumiunc
    rares.jes  = "1"
    if kine is "srcr": rares.lepeff  = "1.04"
    rares.lephlt  = "1.02"
    rares.hlt  = "1.03"
    rares.hthlt  = "1"
    rares.btag = "1"
    rares.pu = "1"
    fakes.fakes = "1.30"
    fakes.fakes_EWK = "1"
    flips.flips = "1.20"

    #fill list of processes
    processes = []
    processes.append(signal)
    processes.append(TTW)
    processes.append(TTZ)
    processes.append(WZ)
    processes.append(WW)
    processes.append(XG)
    processes.append(rares)
    processes.append(fakes)
    processes.append(flips)


    #create it
    writeOneCardFromProcesses(thedir, kine, plot, output, data, processes )

    for proc in processes:
        print "-->", proc.name, proc.rate()
        for nuisance in dir(proc):
            if "__" in nuisance: continue
            if nuisance in ["rootf","rate","ratecache","plot","name","count"]: continue
            val = proc.__getattribute__(nuisance)
            if val == "-": continue
            print "\t", proc.name, nuisance, val


    return

def writeAllCardsForOneKine(thedir, signal, kine, suffix=""):
    print "Writing all cards for one kinematic region"
    writeOneCard(thedir, signal, kine,   "sr"+suffix, "card_"+signal+suffix+"_"+lumi+"ifb-"+kine+".txt" )
    oldthedir = os.getcwd()
    os.chdir(dir)
    f = open('card_'+signal+suffix+'_'+lumi+'ifb-all.txt', 'wb')
    subprocess.call(["combineCards.py","card_"+signal+suffix+"_"+lumi+"ifb-"+kine+".txt"],stdout=f)
    os.chdir(olddir)

# #main body
# if len(sys.argv)==3:
#     thedir = sys.argv[1]
#     signal = sys.argv[2]
#     writeAllCards( thedir, signal )
# elif len(sys.argv)==4:
#     thedir = sys.argv[1]
#     signal = sys.argv[2]
#     suffix = sys.argv[3]
#     writeAllCards( thedir, signal, suffix )
# elif len(sys.argv)==5:
#     thedir = sys.argv[1]
#     signal = sys.argv[2]
#     suffix = sys.argv[3]
#     kine = sys.argv[4]
#     suffix = "" # override suffix. we only need it to make the number of arguments greater than the previous elif
#     writeAllCardsForOneKine( thedir, signal, kine, suffix )
# elif len(sys.argv)==6:
#     thedir = sys.argv[1]
#     signal = sys.argv[2]
#     kine = sys.argv[3]
#     plot = sys.argv[4]
#     output = sys.argv[5]
#     writeOneCard( thedir, signal, kine, plot, output )
# else: print "number of arguments not supported"


if __name__ == "__main__":
    thedir = "v0.01_Apr25_test"
    writeOneCard("v0.01_Apr25_test","tttt","srcr","sr","card_test.txt")
    print "here"
