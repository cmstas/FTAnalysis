import os
import sys
import ROOT as r
from tqdm import tqdm
import itertools
from multiprocessing import Pool as ThreadPool

verbose_ = True

def avoid_negative_yields(h):
    for ix in range(1,h.GetNbinsX()+1):
        v = h.GetBinContent(ix)
        if v < 0:
            if verbose_: print "[!] Bin {} of {} has yield {}".format(ix,h,v)
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

def modify_central(h, fname_in, name, region, year, f_nominal=None, doss=False,others=[]):
    if name == "fakes":
        f = r.TFile(fname_in.replace("fakes","fakes_mc"))
        fhybrid = r.TFile(fname_in.replace("fakes","fakes_mchybrid"))
        h_mc = f.Get("{}_TOTAL_{}".format(region,"fakes_mc"))
        h_mchybrid = fhybrid.Get("{}_TOTAL_{}".format(region,"fakes_mchybrid"))
        h_app = f_nominal.Get("{}_UNWFR_TOTAL_{}".format(region,name))
        # h_mchybrid_app = fhybrid.Get("{}_UNWFR_TOTAL_{}".format(region,"fakes_mchybrid"))
        # print list(h_mchybrid)
        # print h.Integral(),h_mc.Integral(),h_mchybrid.Integral()
        # h_mc.Scale(h.Integral()/h_mc.Integral())
        # h_mchybrid.Scale(h.Integral()/h_mchybrid.Integral())
        f16 = r.TFile(fname_in.replace(str(year),"2016"))
        nom16unw = f16.Get("{}_UNWFR_TOTAL_{}".format(region,name))
        f17 = r.TFile(fname_in.replace(str(year),"2017"))
        nom17unw = f17.Get("{}_UNWFR_TOTAL_{}".format(region,name))
        f18 = r.TFile(fname_in.replace(str(year),"2018"))
        nom18unw = f18.Get("{}_UNWFR_TOTAL_{}".format(region,name))
        for ix in range(1,h.GetNbinsX()+1):
            v = h.GetBinContent(ix)
            ve = h.GetBinError(ix)
            v_mc = h_mc.GetBinContent(ix)
            ve_mc = h_mc.GetBinError(ix)
            v_mchybrid = h_mchybrid.GetBinContent(ix)
            ve_mchybrid = h_mchybrid.GetBinError(ix)
            v_app = h_app.GetBinContent(ix)
            if v_app == 0.: v = 0.
            # v_mchybrid_app = h_mchybrid_app.GetBinContent(ix)
            # ve_mchybrid_app = h_mchybrid_app.GetBinError(ix)
            # print ">>> Bin {} of {} has yield {:.3f}+-{:.3f} (mc has {:.3f}+-{:.3f}) (HYBRID mc has {:.3f}+-{:.3f}, app {:.1f}+-{:.1f})".format(ix,h,v,ve,v_mc,ve_mc,v_mchybrid,ve_mchybrid,v_mchybrid_app,ve_mchybrid_app)
            if doss:
                if v < 1e-6:
                    # avg weight for bins with 1 tight-loose is 0.13 (i.e., avg transfer factor)
                    avgtf = 0.13
                    if verbose_: print "[!] Bin {} of {} has yield {}+-{} and so setting value to 0 and error to avg tf of {}".format(ix,h,v,ve,avgtf)
                    ve = h.GetBinError(ix)
                    h.SetBinError(ix,avgtf)
                    h.SetBinContent(ix,1e-6)
                    for other in others:
                        other.SetBinContent(ix,1e-6)
            else:
                if v < 1e-6:
                    app16 = nom16unw.GetBinContent(ix)
                    app17 = nom17unw.GetBinContent(ix)
                    app18 = nom18unw.GetBinContent(ix)
                    apptot = app16+app17+app18
                    if apptot == 0:
                        # (6.8*35.9+10*41.5+12.7*58.8)/136.3 -- sr pred for fakes divide by app region counts weighted by lumi
                        avgtf = 0.103/(3**0.5) # since this will get compounded for the 3 years
                        if verbose_: print "[!] Bin {} of {} has NO APPLICATION REGION YIELDS in any year so setting value to 0 and error to avg tf of {}".format(ix,h,avgtf)
                        h.SetBinContent(ix,1e-6)
                        h.SetBinError(ix,avgtf)
                    elif v_mc > 1e-6:
                        if verbose_: print "[!] Bin {} of {} has yield {}+-{} (mc has {}+-{}), so setting value to 0 and error to mc value of {}".format(ix,h,v,ve,v_mc,ve_mc,v_mc)
                        # 1.5 for poisson
                        h.SetBinContent(ix,1e-6)
                        h.SetBinError(ix,1.0*v_mc)
                    elif v_mchybrid > 1e-3:
                        if verbose_: print "[!] Bin {} of {} has yield {}+-{} (HYBRID mc has {}+-{}), so setting value to 0 and error to HYBRID mc value of {}".format(ix,h,v,ve,v_mchybrid,ve_mchybrid,v_mchybrid)
                        h.SetBinContent(ix,1e-6)
                        h.SetBinError(ix,1.0*v_mchybrid)
                    else:
                        # note, shouldn't ever get here anyway
                        # avg transfer factor is 0.045
                        avgtf = 0.045
                        if verbose_: print "[!] Bin {} of {} has yield {}+-{} and no yields in MC either, so setting value to 0 and error to avg tf of {}".format(ix,h,v,ve,avgtf)
                        ve = h.GetBinError(ix)
                        h.SetBinError(ix,avgtf)

def write_one_file(fname_in, fname_out, name, region, year, doss):
    if not os.path.exists(fname_in):
        if verbose_: print "[!] {} does not exist!".format(fname_in)
        return

    fin = r.TFile(fname_in)
    fout = r.TFile(fname_out,"RECREATE")

    # nominal
    h = fin.Get("{}_TOTAL_{}".format(region,name))
    h_nominal = h.Clone("sr")
    if abs(h_nominal.Integral()) < 1e-6:
        # if verbose_: print "[!] Nominal yields for {}, {}, {} are 0, skipping".format(name, region, year)
        # fout.Close()
        # return
        if verbose_: print "[!] Nominal yields for {}, {}, {} are 0, but writing empty histograms anyway".format(name, region, year)


    avoid_negative_yields(h_nominal)
    others = []
    if name == "fakes":
        h_nb0 = fin.Get("{}_FRNORMNB0_TOTAL_{}".format(region,name))
        h_nb1 = fin.Get("{}_FRNORMNB1_TOTAL_{}".format(region,name))
        h_nb2 = fin.Get("{}_FRNORMNB2_TOTAL_{}".format(region,name))
        h_nb3 = fin.Get("{}_FRNORMNB3_TOTAL_{}".format(region,name))
        others = [h_nb0,h_nb1,h_nb2,h_nb3]
    modify_central(h_nominal, fname_in=fname_in, name=name, region=region, year=year, f_nominal=fin, doss=doss,others=others)
    # print list(h_nominal)
    # print [h_nominal.GetBinError(ibin) for ibin in range(1,h_nominal.GetNbinsX()+1)]

    # make sure we write to fout
    fout.cd()

    # # FIXME FIXME FIXME
    # if name == "fakes" and region.upper() == "SRHH":
    #     print year
    #     if year  in [2018]:
    #         print "SETTING SRHH52 and 53 in fakes to 0.01pm0.5!!!"
    #         h_nominal.SetBinContent(52,0.01)
    #         h_nominal.SetBinContent(53,0.01)
    #         h_nominal.SetBinError(52,0.5)
    #         h_nominal.SetBinError(53,0.5)

    # # FIXME FIXME FIXME
    # if name == "data" and region.upper() == "SRHH":
    #     print year
    #     if year  in [2016,2017]:
    #         print "SETTING SRHH52 and 53 in data to 0!!!"
    #         h_nominal.SetBinContent(52,0)
    #         h_nominal.SetBinContent(53,0)
    #     else:
    #         print "SETTING SRHH52 and 53 in data to 1,1!!!"
    #         h_nominal.SetBinContent(52,1)
    #         h_nominal.SetBinContent(53,1)


    # print [h_nominal.GetBinError(ibin) for ibin in range(1,h_nominal.GetNbinsX()+1)]
    h_nominal.Write()

    # only write counts for data
    if name == "data":
        fout.Close()
        return

    is_higgs = "higgs" in name
    is_fastsim = "fs_" in name

    # stat
    write_stat(h_nominal, name)

    def divz(x): return (999. if x < 1e-6 else x)

    # scale, pdf, alphas
    if name == "tttt":
        for syst in ["scale","pdf","alphas"]:
            for which in ["Up","Down"]:
                h_alt = fin.Get("{}_{}_{}_TOTAL_{}".format(region,syst.upper(),which.replace("ow","").upper(),name))
                h_syst = h_alt.Clone("{}{}".format(syst,which))
                avoid_negative_yields(h_syst)
                # preserve acceptance normalization differences for scale/pdf (assuming we've normalized out xsec from the looper & weight histogram)
                if is_fastsim or (syst not in ["scale","pdf"]):
                    h_syst.Scale(h_nominal.Integral()/divz(h_syst.Integral()))
                h_syst.Write()
    elif (name in ["ttw","ttz","tth","rares","xg","ttvv","wz","ww"]) or is_higgs or is_fastsim:
        for syst in ["scale","pdf"]:
            if is_fastsim:
                h_alt = fin.Get("{}_{}_UP_TOTAL_{}".format(region,syst.upper(),name))
                h_syst_up = h_alt.Clone("{}Up".format(syst))
                avoid_negative_yields(h_syst_up)
                h_syst_up.Scale(h_nominal.Integral()/divz(h_syst_up.Integral()))
                h_syst_down = h_alt.Clone("{}Down".format(syst))
                fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
                h_syst_up.Write()
                h_syst_down.Write()
            else:
                for which in ["Up","Down"]:
                    h_alt = fin.Get("{}_{}_{}_TOTAL_{}".format(region,syst.upper(),which.replace("ow","").upper(),name))
                    h_syst = h_alt.Clone("{}{}".format(syst,which))
                    avoid_negative_yields(h_syst)
                    # normalize for ttW ttZ because they have CRs; also normalize alphas
                    if (name in ["ttw","ttz"]):
                        h_syst.Scale(h_nominal.Integral()/divz(h_syst.Integral()))
                    h_syst.Write()
    
    # If not doing data, flips, or fakes
    if not any(x in name for x in ["data","flips","fakes"]):

        split_btagsf = True
        if split_btagsf:
            # btagSF heavy
            for which in ["Up","Down"]:
                h_alt = fin.Get("{}_{}_{}_TOTAL_{}".format(region,"BTAGSFHEAVY",which.replace("ow","").upper(),name))
                # h_alt.Scale(h_nominal.Integral()/divz(h_alt.Integral()))
                h_syst = h_alt.Clone("btaghf{}".format(which))
                h_syst.SetTitle("btaghf{}".format(which))
                avoid_zero_integrals(h_nominal,h_syst)
                h_syst.Write()

            # btagSF light
            for which in ["Up","Down"]:
                h_alt = fin.Get("{}_{}_{}_TOTAL_{}".format(region,"BTAGSFLIGHT",which.replace("ow","").upper(),name))
                # h_alt.Scale(h_nominal.Integral()/divz(h_alt.Integral()))
                h_syst = h_alt.Clone("btaglf{}".format(which))
                h_syst.SetTitle("btaglf{}".format(which))
                avoid_zero_integrals(h_nominal,h_syst)
                h_syst.Write()
        else:
            # btagSF
            for which in ["Up","Down"]:
                h_alt = fin.Get("{}_{}_{}_TOTAL_{}".format(region,"BTAGSF",which.replace("ow","").upper(),name))
                # h_alt.Scale(h_nominal.Integral()/divz(h_alt.Integral()))
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
                if syst == "jer":
                    num,den = h_nominal.Integral(), divz(h_syst.Integral())
                    sf = num/den
                    h_syst.Scale(sf)
                # if doss:
                #     num,den = h_nominal.Integral(), divz(h_syst.Integral())
                #     sf = num/den
                #     h_syst.Scale(sf)
                #     print doss,syst,which,year,region,name,num,den,sf
                h_syst.Write()

        # prefiring
        h_alt = fin.Get("{}_PREFIRE_UP_TOTAL_{}".format(region,name))
        h_syst_up = h_alt.Clone("prefireUp".format(which))
        h_syst_down = h_alt.Clone("prefireDown".format(which))
        fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
        h_syst_up.Write()
        h_syst_down.Write()

        # trigger
        h_alt = fin.Get("{}_TRIGGER_UP_TOTAL_{}".format(region,name))
        h_syst_up = h_alt.Clone("trigUp".format(which))
        h_syst_down = h_alt.Clone("trigDown".format(which))
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

        # ttbb/ttjj
        h_alt = fin.Get("{}_BB_UP_TOTAL_{}".format(region,name))
        h_syst_up = h_alt.Clone("bbUp".format(which))
        h_syst_down = h_alt.Clone("bbDown".format(which))
        # for ix in range(1,h_syst_up.GetNbinsX()+1):
        #     nomval = h_nominal.GetBinContent(ix)
        #     upval = h_syst_up.GetBinContent(ix)
        #     h_syst_up.SetBinContent(ix, nomval+(0.6/0.7)*(upval-nomval))
        fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
        h_syst_up.Write()
        h_syst_down.Write()

        if all(x not in name for x in ["fs_","rpv_"]):

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

        if "fs_" in name:

            # FastSim gen met
            h_alt = fin.Get("{}_MET_UP_TOTAL_{}".format(region,name))
            h_syst_up = h_alt.Clone("metUp".format(which))
            h_syst_down = h_alt.Clone("metDown".format(which))
            fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
            h_syst_up.Write()
            h_syst_down.Write()

            # FastSim ISR reweighting
            h_alt = fin.Get("{}_ISR_UP_TOTAL_{}".format(region,name))
            h_syst_up = h_alt.Clone("isrUp".format(which))
            h_syst_down = h_alt.Clone("isrDown".format(which))
            h_syst_up.Scale(h_nominal.Integral()/divz(h_syst_up.Integral()))
            h_syst_down.Scale(h_nominal.Integral()/divz(h_syst_down.Integral()))
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

    if name in ["fakes"]:

        # EWK subtraction
        h_alt = fin.Get("{}_FR_TOTAL_{}".format(region,name))
        h_syst_up = h_alt.Clone("fakes_EWKUp")
        h_syst_down = h_alt.Clone("fakes_EWKDown")
        fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
        h_syst_up.Write()
        h_syst_down.Write()

        if doss:
            # FR lnN 1.3 separately for Nb0,1,2,>=3
            for i,h_alt in enumerate([h_nb0,h_nb1,h_nb2,h_nb3]):
                h_syst_up = h_alt.Clone("fakes_normNB{}Up".format(i))
                h_syst_down = h_alt.Clone("fakes_normNB{}Down".format(i))
                fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
                h_syst_up.Write()
                h_syst_down.Write()

            # # FR lnN 1.3 separately for mu,el for all,low,high pt
            # for flav in ["MU","EL"]:
            #     for pt in ["A","L","H"]:
            #         n = flav+pt
            #         h_alt = fin.Get("{}_FRNORM{}_TOTAL_{}".format(region,n,name))
            #         h_syst_up = h_alt.Clone("fakes_norm{}Up".format(n))
            #         h_syst_down = h_alt.Clone("fakes_norm{}Down".format(n))
            #         fill_down_mirror_up(h_nominal,h_syst_up,h_syst_down)
            #         h_syst_up.Write()
            #         h_syst_down.Write()


        # Raw counts for gmN
        h_alt = fin.Get("{}_UNWFR_TOTAL_{}".format(region,name))
        h_counts = h_alt.Clone("fakes_unwcounts")
        h_counts.Write()

    fout.Close()

    return True

def do_one_parallel(x):
    inputdir,year,proc,region,doss = x
    fname_in = "{}/output_{}_{}.root".format(inputdir,year,proc)
    fname_out = "{}/{}_histos_{}_{}.root".format(inputdir,proc,region.lower(),year)
    if verbose_: print "Converting {} -> {}".format(fname_in,fname_out)
    ret = None
    try:
        ret = write_one_file(
                fname_in = fname_in,
                fname_out = fname_out,
                name = proc,
                region = region.upper(),
                year = year,
                doss = doss,
                )
    except:
        print "[!] Error with {}".format(fname_in)
    return ret

def make_root_files(inputdir = "outputs", outputdir = "../limits/v3.08_allyears_tmp", regions=[],verbose=True,extra_procs=[], doss=False,years=[2016,2017,2018]):
    global verbose_
    verbose_ = verbose

    def do_one(year,proc,region,doss):
        fname_in = "{}/output_{}_{}.root".format(inputdir,year,proc)
        fname_out = "{}/{}_histos_{}_{}.root".format(inputdir,proc,region.lower(),year)
        if verbose_: print "Converting {} -> {}".format(fname_in,fname_out)
        return write_one_file(
                fname_in = fname_in,
                fname_out = fname_out,
                name = proc,
                region = region.upper(),
                year = year,
                doss = doss,
                )

    procs = []
    if doss:
        procs.extend(["ttw","ww","wz", "tth", "ttz", "fakes", "fakes_mc", "data", "flips", "rares", "xg"])
        regions = ["SRHH","SRHL","SRLL","SRML","SRLM"]
    else:
        procs.extend(["tttt", "ttw", "tth", "ttz", "fakes", "fakes_mc", "data", "flips", "rares", "xg", "ttvv"])
        regions = ["SRCR","SRDISC"]
    nmade = 0
    # years = [2016, 2017, 2018]
    allprocs = procs+extra_procs

    allprocs = list(set(allprocs))

    do_parallel = True

    infos = []
    for year,proc in tqdm(list(itertools.product(years,allprocs))):
        for region in regions:
            if do_parallel:
                infos.append([inputdir,year,proc,region,doss])
            else:
                if do_one(year,proc,region,doss): nmade += 1


    if do_parallel:
        os.nice(10)
        pool = ThreadPool(15)
        # pool = ThreadPool(1)
        for res in tqdm(pool.imap_unordered(do_one_parallel,infos),total=len(infos)):
            if res: nmade += 1
            # if res: print "Wrote {}".format(res)

    # for year in [2016]:
    #     for proc in ["fs_t6ttww_m875_m775"]:
    #     # for proc in ["fs_t1tttt_m1700_m1350"]:
    #         for region in ["SRCR"]:
    #             do_one(year,proc,region)

    os.system("mkdir -p {}".format(outputdir))
    # # FIXME didn't copy
    # for _ in range(10): print "Didn't copy"
    if nmade > 5000:
        print "Copying {} files with rsync...".format(nmade)
        os.system("rsync -r --info=progress1 --include='*histos*' {}/ {}/".format(inputdir,outputdir))
        print "Done copying"
    else:
        os.system("cp {}/*.root {}/".format(inputdir,outputdir))
    print "Made {} shape histogram root files and copied them to {}".format(nmade,outputdir)

if __name__ == "__main__":

    inputdir = "outputs"
    # outputdir = "../limits/v3.05_allyears_v2"
    # outputdir = "../limits/v3.05_allyears_ttwttz1p3_v2"
    # outputdir = "../limits/v3.05_allyears_19bins_v2"
    # outputdir = "../limits/v3.05_allyears_v1"
    # outputdir = "../limits/v3.09_v0/"

    # make_root_files(inputdir, outputdir)

    # write_one_file(
    #         fname_in = "{}/output_{}_{}.root".format("outputs",2018,"fakes"),
    #         fname_out = "{}/{}_histos_{}_{}.root".format("outputs","fakes","srhh".lower(),2018),
    #         name = "fakes",
    #         region = "srhh".upper(),
    #         year = 2018,
    #         )


    verbose_ = True
    year = 2016
    inputdir = "outputs/"
    # region = "srdisc"
    region = "srcr"
    proc = "fakes"
    fname_in = "{}/output_{}_{}.root".format(inputdir,year,proc)
    fname_out = "{}/{}_histos_{}_{}.root".format(inputdir,proc,region.lower(),year)
    if verbose_: print "Converting {} -> {}".format(fname_in,fname_out)
    write_one_file(
            fname_in = fname_in,
            fname_out = fname_out,
            name = proc,
            region = region.upper(),
            year = year,
            doss = False
            )
