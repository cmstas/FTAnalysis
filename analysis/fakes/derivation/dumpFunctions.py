import os
import sys
import ROOT as r

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
            if do_errors: buff += "{};\n".format(h.GetBinError(xbin,ybin))
            else: buff += "{}; // +-{:.1f}%;\n".format(h.GetBinContent(xbin,ybin),100.0*h.GetBinError(xbin,ybin)/max(1.0e-6,h.GetBinContent(xbin,ybin)))
    buff += "   return 0.;" + "\n"
    buff += "}" + "\n"
    return buff

if __name__ == "__main__":


    thedir = "./outputs_FT_2017/"
    prefix = "QCDMC"

    for suffix in [

            "",

            ]:

        # suffix = ""
        for do_mu in [True,False]:
            lep = "muon" if do_mu else "electron"
            l = "mu" if do_mu else "e"
            ll = "mu" if do_mu else "el"

            fname = "rate_histos_qcd_{}{}_LooseEMVA_IsoTrigs.root".format(ll,suffix)

            full_fname = "{}/{}".format(thedir,fname)
            f = r.TFile(full_fname)


            h = f.Get("rate_cone_histo_{}".format(l))
            name = "{}{}FakeRate{}".format(lep,prefix,suffix)
            print dump_hist(h,name)
            name = "{}{}FakeRateError{}".format(lep,prefix,suffix)
            print dump_hist(h,name,do_errors=True)

            # h = f.Get("rate_fine_cone_histo_{}".format(l))
            # name = "{}{}FakeRate{}Fine".format(lep,prefix,suffix)
            # print dump_hist(h,name)
            # name = "{}{}FakeRateError{}Fine".format(lep,prefix,suffix)
            # print dump_hist(h,name,do_errors=True)

