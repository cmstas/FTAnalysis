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
    fin = r.TFile(fname_in)
    fout = r.TFile(fname_out,"RECREATE")

    # make sure we write to fout
    fout.cd()

    # name = "tttt"
    # region = "SRCR"

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
            h_isrup = f_isrup.Get("{}_TOTAL_{}".format(region,name))
            h_isrdn = f_isrdn.Get("{}_TOTAL_{}".format(region,name))
            h_fsrup = f_fsrup.Get("{}_TOTAL_{}".format(region,name))
            h_fsrdn = f_fsrdn.Get("{}_TOTAL_{}".format(region,name))
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
                nom = h_sr.GetBinContent(ix)
                newup = nom+(varup-nom)/(2.**0.5)
                newdn = nom+(vardn-nom)/(2.**0.5)
                if (nom <= 1.0e-6): continue
                h_fsrup.SetBinContent(ix,newup)
                h_fsrdn.SetBinContent(ix,newdn)
            map(lambda x: x.Close(), [f_isrup,f_isrdn,f_fsrup,f_fsrdn])
            fout.cd()
            map(lambda x: x.Write(), [h_isrup,h_isrdn,h_fsrup,h_fsrdn])

    # EWK subtraction
    if "fakes" in name:
        h_alt = fin.Get("{}_FR_TOTAL_{}".format(region,name))
        h_syst_up = h_alt.Clone("fakes_EWKUp".format(which))
        h_syst_down = h_alt.Clone("fakes_EWKDown".format(which))
        fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
        h_syst_up.Write()
        h_syst_down.Write()

    # HLT syst
    write_hthlt_syst(h_nominal,name)

    fout.Close()

if __name__ == "__main__":

    write_one_file(
            fname_in = "histos.root",
            fname_out = "test.root",
            name = "tttt",
            region = "SRCR",
            year = 2017,
            )
