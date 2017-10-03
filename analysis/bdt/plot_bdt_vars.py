import os
import sys
import ROOT as r



if __name__ == "__main__":
    os.system("mkdir -p plots")

    r.gROOT.SetBatch(1)

    ch = r.TChain("t")
    ch.Add("output_withdata.root")
    c1 = r.TCanvas()

    triplets = [
        ("l1id","30,-15,15",""),
        ("l2id","30,-15,15",""),
        ("nbtags","6,0,6",""),
        ("njets","10,0,10",""),
        ("nleps","",""),
        ("wcands","",""),
        ("met","50,0,500",""),
        ("ht","50,0,2000",""),
        ("htb","50,0,500",""),
        ("mt1","30,0,300",""),
        ("mt2","30,0,300",""),
        ("dphil1j1", "30,0,3.2",""),
        ("dphil2j2", "30,0,3.2",""),
        ("dphil1j2", "30,0,3.2",""),
        ("dphil2j1", "30,0,3.2",""),
        ("dphil1l2", "30,0,3.2",""),
        ("dphij1j2", "30,0,3.2",""),
        ("dphil1met","30,0,3.2",""),
        ("dphil2met","30,0,3.2",""),
        ("dphij1met","30,0,3.2",""),
        ("dphij2met","30,0,3.2",""),
        ("detal1j1","25,0,5",""),
        ("detal2j2","25,0,5",""),
        ("detal1j2","25,0,5",""),
        ("detal2j1","25,0,5",""),
        ("detal1l2","25,0,5",""),
        ("detaj1j2","25,0,5",""),
        ("ml1j1","25,0,500",""),
        ("ml2j2","25,0,500",""),
        ("ml1j2","25,0,500",""),
        ("ml2j1","25,0,500",""),
        ("ml1l2","25,0,500",""),
        ("mj1j2","25,0,1000",""),
        ("ptl1","25,0,500",""),
        ("ptl2","25,0,500",""),
        ("ptj1","25,0,500",""),
        ("ptj2","25,0,500",""),
        ("stb","50,0,500",""),
        ("nlb40","8,0,8",""),
        ("nmb40","8,0,8",""),
        ("ntb40","8,0,8",""),
        ("q1","3,-1,2",""),
        ("q2","3,-1,2",""),
        ("q3","3,-1,2",""),
        ("ht4ratio","20,0,20","njets>4"),
        ("SR","10,1,11",""),
        ("disc","10,0,1",""),
    ]

    nominal_vars = ["njets", "nbtags", "mt1", "mt2", "met", "ml1l2", "htb", "nleps", "ht", "mj1j2", "dphij1j2", "ptj1", "ptj2", "ml1j2", "ml1j1", "wcands", "dphil1j1", "detal1l2","nlb40","nmb40","ntb40","q1","ht4ratio"]


    # triplet = ("ht4ratio", "(20,0,20)", "njets>4")
    for triplet in triplets:

        var, bins, sel = triplet

        if var not in nominal_vars: continue
        if not sel: sel = "1"

        print triplet


        hnamebg = var+"_bg"
        hnamesig = var+"_sig"
        hnamedata = var+"_data"
        bgstr = "(name==\"ttw\" || name==\"ttz\" || name==\"tth\" || name==\"xg\" || name==\"rares\" || name==\"ttvv\" || name==\"flips\" || name==\"fakes\")"
        sigstr = "(name==\"tttt\")"
        datastr = "(name==\"data\")"
        ch.Draw("{}>>{}({})".format(var,hnamebg,bins), "weight*(({}) && ({}))".format(sel,bgstr), "goff")
        ch.Draw("{}>>{}({})".format(var,hnamesig,bins),"weight*(({}) && ({}))".format(sel,sigstr), "goff")
        ch.Draw("{}>>{}({})".format(var,hnamedata,bins),"weight*(({}) && ({}))".format(sel,datastr), "goff")
        hbg = r.gDirectory.Get(hnamebg)
        hsig = r.gDirectory.Get(hnamesig)
        hdata = r.gDirectory.Get(hnamedata)
        # print hbg, hsig

        hdata.SetLineColor(r.kBlack)
        hbg.SetLineColor(r.kBlue)
        hsig.SetLineColor(r.kRed)

        hdata.SetMarkerSize(1.2)

        hbg.SetTitle("%s {%s} [both normed]" % (var,sel))
        hbg.SetMaximum(1.3*max(hbg.GetMaximum(),hsig.GetMaximum()))
        hsig.SetMaximum(1.3*max(hbg.GetMaximum(),hsig.GetMaximum()))
        hbg.DrawNormalized("histe")
        hsig.DrawNormalized("samehiste")
        hdata.DrawNormalized("samePE")


        c1.SaveAs("plots/{}.pdf".format(var))


    os.system("niceplots plots plots_bdt_Sep26")



