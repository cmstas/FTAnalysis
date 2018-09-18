import os
import ROOT as r

def avoid_negative_yields(h):
    for ix in range(1,h.GetNbinsX()+1):
        v = h.GetBinContent(ix)
        if v < 0:
            print "[!] Bin {} of {} has yield {}".format(ix,h,v)
            h.SetBinContent(ix,1.0e-6)

def write_stat_up_down(h,name,isdown):
    updown = "Down" if isdown else "Up"
    tmp = "{}_stat{}".format(name,updown)
    sud = r.TH1F(tmp,tmp,h.GetNbinsX(),h.GetXaxis().GetXmin(),h.GetXaxis().GetXmax())
    for ix in range(1,h.GetNbinsX()+1):
        v = h.GetBinContent(ix)
        e = h.GetBinError(ix)
        if v > 0: sud.SetBinContent(ix, v-e if isdown else v+e)
        else: sud.SetBinContent(ix,1.0e-6)
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


def write_one_file(fname_in, fname_out, name, region, year):
    if not os.path.exists(fname_in):
        print "[!] {} does not exist!".format(fname_in)
        return

    fin = r.TFile(fname_in)
    fout = r.TFile(fname_out,"RECREATE")

    # make sure we write to fout
    fout.cd()

    # nominal
    h = fin.Get("{}_TOTAL_{}".format(region,name))
    h_nominal = h.Clone("sr")
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
    if "fakes" in name:
        h_alt = fin.Get("{}_FR_TOTAL_{}".format(region,name))
        h_syst_up = h_alt.Clone("fakes_EWKUp")
        h_syst_down = h_alt.Clone("fakes_EWKDown")
        fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
        h_syst_up.Write()
        h_syst_down.Write()


    fout.Close()

if __name__ == "__main__":

    indir = "outputs"
    copy_to = "../limits/v3.05_allyears_v2"
    # copy_to = "../limits/v3.05_allyears_v1"
    # copy_to = "../limits/v3.05_allyears_tmp"

    for year in [2016, 2017, 2018]:
        for proc in ["tttt", "ttw", "tth", "ttz", "fakes", "fakes_mc", "data", "flips", "rares", "xg", "ttvv"]:
            for region in ["SRCR","SRDISC"]:
                fname_in = "{}/output_{}_{}.root".format(indir,year,proc)
                fname_out = "{}/{}_histos_{}_{}.root".format(indir,proc,region.lower(),year)
                print "Converting {} -> {}".format(fname_in,fname_out)
                write_one_file(
                        fname_in = fname_in,
                        fname_out = fname_out,
                        name = proc,
                        region = region.upper(),
                        year = year,
                        )

    os.system("mkdir -p {}".format(copy_to))
    os.system("cp {}/*.root {}/".format(indir,copy_to))

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
