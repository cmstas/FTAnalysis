import ROOT as r
import os

def dump_hist(h,name,do_errors=False):
    buff = ""
    buff += "float {}(float pt, float eta) {{\n".format(name)
    for xbin in range(1,h.GetNbinsX()+1):
        for ybin in range(1,h.GetNbinsY()+1):
            if xbin != h.GetNbinsX():
                buff += "   if (pt>={:.0f} && pt<{:.0f}".format(h.GetXaxis().GetBinLowEdge(xbin), h.GetXaxis().GetBinUpEdge(xbin) )
                buff += " && fabs(eta)>={:.3f} && fabs(eta)<{:.3f}) return ".format(h.GetYaxis().GetBinLowEdge(ybin), h.GetYaxis().GetBinUpEdge(ybin))
            else:
                buff += "   if (pt>={:.0f}".format(h.GetXaxis().GetBinLowEdge(xbin))
                buff += " && fabs(eta)>={:.3f} && fabs(eta)<{:.3f}) return ".format(h.GetYaxis().GetBinLowEdge(ybin), h.GetYaxis().GetBinUpEdge(ybin))
            if do_errors: buff += "{:.4f};\n".format(h.GetBinError(xbin,ybin))
            else: buff += "{:.4f}; // +-{:.1f}%;\n".format(h.GetBinContent(xbin,ybin),100.0*h.GetBinError(xbin,ybin)/max(1.0e-6,h.GetBinContent(xbin,ybin)))
    buff += "   return 0.;" + "\n"
    buff += "}" + "\n"
    return buff

def dump_functions(inputdir,year):

    buff = ""
    for iso,isostr in [(False,""),(True,"_IsoTrigs")]:
        suffix = isostr
        for lep,l,ll in [("electron","e","el"),("muon","mu","mu")]:

            ystr = "y{}_".format(year)
            fdata = r.TFile("{}/pdfs/{}ewkCorFR_{}{}.root".format(inputdir,ystr,lep,suffix))

            cent = fdata.Get("Nt_histo_{}".format(l))
            buff += dump_hist(cent,"{}FakeRate{}".format(lep,suffix))
            buff += dump_hist(cent,"{}FakeRateError{}".format(lep,suffix),do_errors=True)
            buff += "\n"

            alt = fdata.Get("num_data_zp")
            buff += dump_hist(alt,"{}AlternativeFakeRate{}".format(lep,suffix))
            buff += dump_hist(alt,"{}AlternativeFakeRateError{}".format(lep,suffix),do_errors=True)
            buff += "\n"

            fqcd = r.TFile("{}/{}rate_histos_qcd_{}_LooseEMVA{}.root".format(inputdir,ystr,ll,suffix))
            qcd = fqcd.Get("rate_cone_histo_{}".format(l))
            buff += dump_hist(qcd, "{}QCDMCFakeRate{}".format(lep,suffix))
            buff += dump_hist(qcd, "{}QCDMCFakeRateError{}".format(lep,suffix),do_errors=True)
            buff += "\n"

            fdata.Close()
            fqcd.Close()
    return buff

if __name__ == "__main__":

    r.gROOT.ProcessLine(".L ../../misc/common_utils.h");
    r.gROOT.ProcessLine(".L make1DplotFR.C");
    r.gROOT.ProcessLine(".L plotEWKCorFR.C");
    r.gROOT.ProcessLine(".L plotFR.C");

    do_ss = True

    if do_ss:
        # inputdir = "outputs_19Jan28/";
        inputdir = "outputs_ssrereco_19Jan28/";
    else:
        inputdir = "outputs_ft_19Feb2//";

    import do_fits
    d_infos = do_fits.fit_and_plot(inputdir=inputdir,outputdir="{}/plots_mtfits/".format(inputdir))
    do_fits.print_infos(d_infos)
    print "FIXME pinning 2016 MTSFs to certain values"
    d_infos[2016][("el",False,True)][0] = 1.28 # from http://uaf-1.t2.ucsd.edu/~namin/dump/AN-16-386_temp.pdf pg 37
    d_infos[2016][("el",True,True)][0] = 1.28
    d_infos[2016][("mu",False,True)][0] = 1.17
    d_infos[2016][("mu",True,True)][0] = 1.19
    print "FIXME pinning 2018 MTSFs to certain values"
    d_infos[2018][("el",False,True)][0] = 1.1
    d_infos[2018][("el",True,True)][0] = 1.1
    years = [2016,2017,2018]
    for year in years:
        r.plotFR(inputdir, year)
        for iso in [True,False]:
            mt_sf_el = d_infos[year][("el",iso,True)][0]
            mt_sf_mu = d_infos[year][("mu",iso,True)][0]
            print "Making plots for year={} isotrigs={} with EWKSF el/mu = {:.2f}/{:.2f}".format(year,iso,mt_sf_el,mt_sf_mu)
            r.make1DplotFR(inputdir, 1.0, 1.0, mt_sf_el, mt_sf_mu, iso, year)
            r.plotEWKCorFR(inputdir, 1.0, 1.0, mt_sf_el, mt_sf_mu, iso, year)
    os.system("mkdir -p {}/pdfs/".format(inputdir))
    os.system("cp pdfs/* {}/pdfs/".format(inputdir))

    if do_ss:
        for year in [2016,2017,2018]:
            with open("frs_{}_temp.h".format(year),"w") as fh:
                fh.write(dump_functions(inputdir,year))
    else:
        for year in [2017,2018]:
            with open("frs_ft_{}_temp.h".format(year),"w") as fh:
                fh.write(dump_functions(inputdir,year))

