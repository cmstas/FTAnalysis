import os
import ROOT as r

def avoid_negative_yields(h):
    for ix in range(1,h.GetNbinsX()+1):
        v = h.GetBinContent(ix)
        if v < 0:
            print "[!] Bin {} of {} has yield {}".format(ix,h,v)
            # h.SetBinContent(ix,1.0e-6)
            h.SetBinContent(ix,0.0)

def write_stat_up_down(h,name,isdown):
    updown = "Down" if isdown else "Up"
    tmp = "{}_stat{}".format(name,updown)
    sud = r.TH1F(tmp,tmp,h.GetNbinsX(),h.GetXaxis().GetXmin(),h.GetXaxis().GetXmax())
    for ix in range(1,h.GetNbinsX()+1):
        v = h.GetBinContent(ix)
        e = h.GetBinError(ix)
        if v >= 0: sud.SetBinContent(ix, max(v-e,0.) if isdown else v+e)
        else: sud.SetBinContent(ix,0.)
    sud.Write()

def write_stat(h,name):
    write_stat_up_down(h,name,isdown=False)
    write_stat_up_down(h,name,isdown=True)

def write_hthlt_syst(h,name,val=1.02):
    syst_up = h.Clone("hthltUp")
    syst_up.SetTitle("hthltUp")
    for ix in range(1,h.GetNbinsX()+1):
        v = h.GetBinContent(ix)
        if v > 0: syst_up.SetBinContent(ix, v*val)
    syst_down = h.Clone("hthltDown")
    syst_down.SetTitle("hthltUp")
    fill_down_mirror_up(h,syst_up,syst_down)
    syst_up.Write()
    syst_down.Write()

def fill_down_mirror_up(h,hup,hdown):
    hdown.Reset()
    hdown.Add(hup)
    hdown.Scale(-1)
    hdown.Add(h)
    hdown.Add(h)
    avoid_negative_yields(hdown)

def avoid_zero_integrals(h,h_var):
    if h_var.Integral() <= 0:
        h_var.Reset()
        h_var.Add(h)
        h_var.Scale(-1)
    avoid_negative_yields(h_var)

def modify_central(h, fname_in, name, region, year):
    if name == "fakes":
        f = r.TFile(fname_in.replace("fakes","fakes_mc"))
        fhybrid = r.TFile(fname_in.replace("fakes","fakes_mchybrid"))
        h_mc = f.Get("{}_TOTAL_{}".format(region,"fakes_mc"))
        h_mchybrid = fhybrid.Get("{}_TOTAL_{}".format(region,"fakes_mchybrid"))
        h_mchybrid_app = fhybrid.Get("{}_UNWFR_TOTAL_{}".format(region,"fakes_mchybrid"))
        # print list(h_mchybrid)
        # print h.Integral(),h_mc.Integral(),h_mchybrid.Integral()
        h_mc.Scale(h.Integral()/h_mc.Integral())
        h_mchybrid.Scale(h.Integral()/h_mchybrid.Integral())
        for ix in range(1,h.GetNbinsX()+1):
            v = h.GetBinContent(ix)
            ve = h.GetBinError(ix)
            v_mc = h_mc.GetBinContent(ix)
            ve_mc = h_mc.GetBinError(ix)
            v_mchybrid = h_mchybrid.GetBinContent(ix)
            ve_mchybrid = h_mchybrid.GetBinError(ix)
            v_mchybrid_app = h_mchybrid_app.GetBinContent(ix)
            ve_mchybrid_app = h_mchybrid_app.GetBinError(ix)
            # print ">>> Bin {} of {} has yield {:.3f}+-{:.3f} (mc has {:.3f}+-{:.3f}) (HYBRID mc has {:.3f}+-{:.3f}, app {:.1f}+-{:.1f})".format(ix,h,v,ve,v_mc,ve_mc,v_mchybrid,ve_mchybrid,v_mchybrid_app,ve_mchybrid_app)
            if v < 1e-6:
                if v_mc > 1e-6:
                    print "[!] Bin {} of {} has yield {}+-{} (mc has {}+-{}), so setting value to 0 and error to mc value of {}".format(ix,h,v,ve,v_mc,ve_mc,v_mc)
                    h.SetBinContent(ix,0.)
                    h.SetBinError(ix,v_mc)
                elif v_mchybrid > 1e-6:
                    print "[!] Bin {} of {} has yield {}+-{} (HYBRID mc has {}+-{}), so setting value to 0 and error to HYBRID mc value of {}".format(ix,h,v,ve,v_mchybrid,ve_mchybrid,v_mchybrid)
                    h.SetBinContent(ix,0.)
                    h.SetBinError(ix,v_mchybrid)
                else:
                    print "[!] Bin {} of {} has yield {}+-{} and no yields in MC either".format(ix,h,v,ve)

def write_one_file(fname_in, fname_out, name, region, year):
    if not os.path.exists(fname_in):
        print "[!] {} does not exist!".format(fname_in)
        return

    fin = r.TFile(fname_in)
    fout = r.TFile(fname_out,"RECREATE")

    # nominal
    h = fin.Get("{}_TOTAL_{}".format(region,name))
    h_nominal = h.Clone("sr")
    avoid_negative_yields(h_nominal)
    modify_central(h_nominal, fname_in=fname_in, name=name, region=region, year=year)

    # make sure we write to fout
    fout.cd()

    h_nominal.Write()

    # only write counts for data
    if name == "data":
        fout.Close()
        return

    # stat
    write_stat(h_nominal, name)

    # scale, pdf, alphas
    if name == "tttt":
        for syst in ["scale","pdf","alphas"]:
            for which in ["Up","Down"]:
                h_alt = fin.Get("{}_{}_{}_TOTAL_{}".format(region,syst.upper(),which.replace("ow","").upper(),name))
                h_syst = h_alt.Clone("{}{}".format(syst,which))
                avoid_negative_yields(h_syst)
                h_syst.Scale(h_nominal.Integral()/h_syst.Integral())
                h_syst.Write()
    elif name in ["ttw","ttz","tth"]:
        for syst in ["scale","pdf","alphas"]:
            for which in ["Up","Down"]:
                h_alt = fin.Get("{}_{}_{}_TOTAL_{}".format(region,syst.upper(),which.replace("ow","").upper(),name))
                h_syst = h_alt.Clone("{}{}".format(syst,which))
                avoid_negative_yields(h_syst)
                h_syst.Scale(h_nominal.Integral()/h_syst.Integral())
                h_syst.Write()
    
    # If not doing data, flips, or fakes
    if not any(x in name for x in ["data","flips","fakes"]):
        # btagSF
        for which in ["Up","Down"]:
            h_alt = fin.Get("{}_{}_{}_TOTAL_{}".format(region,"BTAGSF",which.replace("ow","").upper(),name))
            h_alt.Scale(h_nominal.Integral()/h_alt.Integral())
            h_syst = h_alt.Clone("btag{}".format(which))
            h_syst.SetTitle("btag{}".format(which))
            avoid_zero_integrals(h_nominal,h_syst)
            h_syst.Write()

        # PU, JES, JER
        for syst in ["pu","jes","jer"]:
            for which in ["Up","Down"]:
                h_alt = fin.Get("{}_{}_{}_TOTAL_{}".format(region,syst.upper(),which.replace("ow","").upper(),name))
                h_syst = h_alt.Clone("{}{}".format(syst,which))
                h_syst.SetTitle("{}{}".format(syst,which))
                avoid_zero_integrals(h_nominal,h_syst)
                h_syst.Write()

        # FastSim gen met
        h_alt = fin.Get("{}_MET_UP_TOTAL_{}".format(region,name))
        h_syst_up = h_alt.Clone("metUp".format(which))
        h_syst_down = h_alt.Clone("metDown".format(which))
        fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
        h_syst_up.Write()
        h_syst_down.Write()

        # lepton SFs
        h_alt = fin.Get("{}_LEP_UP_TOTAL_{}".format(region,name))
        h_syst_up = h_alt.Clone("lepUp".format(which))
        h_syst_down = h_alt.Clone("lepDown".format(which))
        fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
        h_syst_up.Write()
        h_syst_down.Write()

        # ttbb/ttjj ~ 1.7 uncertainty
        h_alt = fin.Get("{}_BB_UP_TOTAL_{}".format(region,name))
        h_syst_up = h_alt.Clone("bbUp".format(which))
        h_syst_down = h_alt.Clone("bbDown".format(which))
        for ix in range(1,h_syst_up.GetNbinsX()+1):
            nomval = h_nominal.GetBinContent(ix)
            upval = h_syst_up.GetBinContent(ix)
            h_syst_up.SetBinContent(ix, nomval+(0.6/0.7)*(upval-nomval))
        fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
        h_syst_up.Write()
        h_syst_down.Write()

        # ISR - take half the deviation
        h_alt = fin.Get("{}_ISR_UP_TOTAL_{}".format(region,name))
        h_syst_up = h_alt.Clone("isrUp".format(which))
        h_syst_down = h_alt.Clone("isrDown".format(which))
        for ix in range(1,h_syst_up.GetNbinsX()+1):
            nomval = h_nominal.GetBinContent(ix)
            upval = h_syst_up.GetBinContent(ix)
            h_syst_up.SetBinContent(ix, 0.5*(nomval+upval))
        fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
        h_syst_up.Write()
        h_syst_down.Write()

        # HLT syst
        write_hthlt_syst(h_nominal,name)

    # ISR/FSR up and down variations for tttt
    if name == "tttt":
        if year in [2017, 2018]:
            for syst in ["isrvar","fsrvar"]:
                for which in ["Up","Down"]:
                    # ISR/FSR variations are stored in the ntuple
                    h_alt = fin.Get("{}_{}_{}_TOTAL_{}".format(region,syst.upper(),which.replace("ow","").upper(),name))
                    h_alt.Scale(h_nominal.Integral()/h_alt.Integral())
                    h_syst = h_alt.Clone("{}{}".format(syst,which))
                    avoid_negative_yields(h_syst)
                    h_syst.Write()
        else:
            # ISR/FSR variations come from 4 other samples
            f_isrup = r.TFile(fname_in.replace("tttt","ttttisrup"))
            f_isrdn = r.TFile(fname_in.replace("tttt","ttttisrdn"))
            f_fsrup = r.TFile(fname_in.replace("tttt","ttttfsrup"))
            f_fsrdn = r.TFile(fname_in.replace("tttt","ttttfsrdn"))
            h_isrup_tmp = f_isrup.Get("{}_TOTAL_ttttisrup".format(region))
            h_isrdn_tmp = f_isrdn.Get("{}_TOTAL_ttttisrdn".format(region))
            h_fsrup_tmp = f_fsrup.Get("{}_TOTAL_ttttfsrup".format(region))
            h_fsrdn_tmp = f_fsrdn.Get("{}_TOTAL_ttttfsrdn".format(region))
            h_isrup = h_isrup_tmp.Clone("isrvarUp")
            h_isrdn = h_isrdn_tmp.Clone("isrvarDown")
            h_fsrup = h_fsrup_tmp.Clone("fsrvarUp")
            h_fsrdn = h_fsrdn_tmp.Clone("fsrvarDown")
            h_isrup.Scale(h_nominal.Integral()/h_isrup.Integral())
            h_isrdn.Scale(h_nominal.Integral()/h_isrdn.Integral())
            h_fsrup.Scale(h_nominal.Integral()/h_fsrup.Integral())
            h_fsrdn.Scale(h_nominal.Integral()/h_fsrdn.Integral())
            for ix in range(1,h_nominal.GetNbinsX()+1):
                nomval = h_nominal.GetBinContent(ix)
                h_isrup.SetBinContent(ix,h_isrup.GetBinContent(ix))
                h_isrdn.SetBinContent(ix,h_isrdn.GetBinContent(ix))
                # scale fsr variations closer to central by sqrt(2) according to
                # https://twiki.cern.ch/twiki/bin/viewauth/CMS/TopSystematics
                varup = h_fsrup.GetBinContent(ix)
                vardn = h_fsrdn.GetBinContent(ix)
                nom = h_nominal.GetBinContent(ix)
                newup = nom+(varup-nom)/(2.**0.5)
                newdn = nom+(vardn-nom)/(2.**0.5)
                if (nom <= 1.0e-6): continue
                h_fsrup.SetBinContent(ix,newup)
                h_fsrdn.SetBinContent(ix,newdn)
            fout.cd()
            map(lambda x: x.Write(), [h_isrup,h_isrdn,h_fsrup,h_fsrdn])
            map(lambda x: x.Close(), [f_isrup,f_isrdn,f_fsrup,f_fsrdn])

    # EWK subtraction
    if name in ["fakes"]:
        h_alt = fin.Get("{}_FR_TOTAL_{}".format(region,name))
        h_syst_up = h_alt.Clone("fakes_EWKUp")
        h_syst_down = h_alt.Clone("fakes_EWKDown")
        fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
        h_syst_up.Write()
        h_syst_down.Write()


    fout.Close()

def make_root_files(inputdir = "outputs", outputdir = "../limits/v3.08_allyears_tmp"):
    for year in [2016, 2017, 2018]:
        for proc in ["tttt", "ttw", "tth", "ttz", "fakes", "fakes_mc", "data", "flips", "rares", "xg", "ttvv"]:
    # for year in [2018]: # FIXME
    #     for proc in ["fakes"]: # FIXME
            for region in ["SRCR","SRDISC"]:
                fname_in = "{}/output_{}_{}.root".format(inputdir,year,proc)
                fname_out = "{}/{}_histos_{}_{}.root".format(inputdir,proc,region.lower(),year)
                print "Converting {} -> {}".format(fname_in,fname_out)
                write_one_file(
                        fname_in = fname_in,
                        fname_out = fname_out,
                        name = proc,
                        region = region.upper(),
                        year = year,
                        )

    os.system("mkdir -p {}".format(outputdir))
    # # FIXME didn't copy
    # for _ in range(10): print "Didn't copy"
    os.system("cp {}/*.root {}/".format(inputdir,outputdir))

if __name__ == "__main__":

    inputdir = "outputs"
    # outputdir = "../limits/v3.05_allyears_v2"
    # outputdir = "../limits/v3.05_allyears_ttwttz1p3_v2"
    # outputdir = "../limits/v3.05_allyears_19bins_v2"
    # outputdir = "../limits/v3.05_allyears_v1"
    outputdir = "../limits/v3.09_v0/"

    make_root_files(inputdir, outputdir)

    # for year in [2016, 2017, 2018]:
    #     for proc in ["tttt", "ttw", "tth", "ttz", "fakes", "fakes_mc", "data", "flips", "rares", "xg", "ttvv"]:
    #         for region in ["SRCR","SRDISC"]:
    #             fname_in = "{}/output_{}_{}.root".format(inputdir,year,proc)
    #             fname_out = "{}/{}_histos_{}_{}.root".format(inputdir,proc,region.lower(),year)
    #             print "Converting {} -> {}".format(fname_in,fname_out)
    #             write_one_file(
    #                     fname_in = fname_in,
    #                     fname_out = fname_out,
    #                     name = proc,
    #                     region = region.upper(),
    #                     year = year,
    #                     )
    # os.system("mkdir -p {}".format(outputdir))
    # os.system("cp {}/*.root {}/".format(inputdir,outputdir))

#     write_one_file(
#             fname_in = "outputs/output_2017_tttt.root",
#             fname_out = "tttt_histos_srcr.root",
#             name = "tttt",
#             # name = "ttw",
#             # name = "fakes",
#             # name = "fakes_mc",
#             region = "SRCR",
#             year = 2017,
#             )
