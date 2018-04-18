import ROOT as r

def get_first_hist(f1):
    return f1.Get(f1.GetListOfKeys()[0].GetName())

def dump_bins(h2s, name, transpose=False, nofabseta=False, default1=False, do_err=False):
    buff = "float %s(float pt, float eta) {\n" % name
    if type(h2s) is not list:
        h2s = [h2s]
    for h2 in h2s:
        if transpose:
            for iy in range(1,h2.GetNbinsY()+1):
                for ix in range(1,h2.GetNbinsX()+1):
                    if do_err:
                        val = h2.GetBinError(ix,iy)
                    else:
                        val = h2.GetBinContent(ix,iy)
                    if iy != h2.GetNbinsY():
                        buff += "  if (pt >= %.0f && pt < %.0f && eta >= %.3f && eta < %.3f) return %.4f;\n" \
                                % (h2.GetYaxis().GetBinLowEdge(iy), h2.GetYaxis().GetBinUpEdge(iy), h2.GetXaxis().GetBinLowEdge(ix), h2.GetXaxis().GetBinUpEdge(ix), val)
                    else:
                        buff += "  if (pt >= %.0f  && eta >= %.3f && eta < %.3f) return %.4f;\n" \
                                % (h2.GetYaxis().GetBinLowEdge(iy), h2.GetXaxis().GetBinLowEdge(ix), h2.GetXaxis().GetBinUpEdge(ix), val)
        else:
            for ix in range(1,h2.GetNbinsX()+1):
                for iy in range(1,h2.GetNbinsY()+1):
                    if do_err:
                        val = h2.GetBinError(ix,iy)
                    else:
                        val = h2.GetBinContent(ix,iy)
                    if ix != h2.GetNbinsX():
                        buff += "  if (pt >= %.0f && pt < %.0f && fabs(eta) >= %.3f && fabs(eta) < %.3f) return %.4f;\n" \
                                % (h2.GetXaxis().GetBinLowEdge(ix), h2.GetXaxis().GetBinUpEdge(ix), h2.GetYaxis().GetBinLowEdge(iy), h2.GetYaxis().GetBinUpEdge(iy), val)
                    else:
                        buff += "  if (pt >= %.0f  && fabs(eta) >= %.3f && fabs(eta) < %.3f) return %.4f;\n" \
                                % (h2.GetXaxis().GetBinLowEdge(ix), h2.GetYaxis().GetBinLowEdge(iy), h2.GetYaxis().GetBinUpEdge(iy), val)
    if not default1:
        buff += "  return 0.;\n"
    else:
        buff += "  return 1.;\n"
    buff += "}\n"
    if nofabseta:
        buff = buff.replace("fabs(eta)", "eta")
    return buff

if __name__ == "__main__":
    # from: https://twiki.cern.ch/twiki/bin/viewauth/CMS/Egamma2017DataRecommendations


    for X in ["B","C","D","E","F","BCDEF"]:
        f_el_medium_runX = r.TFile("rootfiles/egammaEffi.txt_EGM2D_run{}_passingMedium94X.root".format(X))
        h_el_medium_runX = f_el_medium_runX.Get("EGamma_SF2D")
        print dump_bins(h_el_medium_runX, "electronScaleFactor_Run{}".format(X), transpose=True)
        print dump_bins(h_el_medium_runX, "electronScaleFactorError_Run{}".format(X), transpose=True, do_err=True)


    f_el_reco_low = r.TFile("rootfiles/egammaEffi.txt_EGM2D_runBCDEF_passingRECO_lowEt.root")
    h_el_reco_low = f_el_reco_low.Get("EGamma_SF2D")
    for X in ["B","C","D","E","F","BCDEF"]:
        f_el_reco_high_runX = r.TFile("rootfiles/egammaEffi.txt_EGM2D_run{}_passingRECO.root".format(X))
        h_el_reco_high_runX = f_el_reco_high_runX.Get("EGamma_SF2D")
        print dump_bins([h_el_reco_low,h_el_reco_high_runX], "electronScaleFactorReco_Run{}".format(X), transpose=True)
        print dump_bins([h_el_reco_low,h_el_reco_high_runX], "electronScaleFactorRecoError_Run{}".format(X), transpose=True, do_err=True)

    # # FULLSIM ELECTRONS
    # electron_file = r.TFile("rootfiles/scaleFactors.root") # from http://tomc.web.cern.ch/tomc/tagAndProbe/20160726/output/scaleFactors.root
    # # electron_file = r.TFile("rootfiles/el_sfs.root") 

    # tightid2d3d = electron_file.Get("GsfElectronToTightID2D3D")
    # multi = electron_file.Get("MVATightElectronToMultiIsoT")
    # convhit0chg = electron_file.Get("MVATightConvIHit0ElectronToConvIHit0Chg")
    # multiemu = electron_file.Get("MVATightElectronToMultiIsoEmu")

    # el_noiso = tightid2d3d.Clone("el_noiso")
    # el_noiso.Multiply(multi)
    # el_noiso.Multiply(convhit0chg)

    # el_iso = tightid2d3d.Clone("el_noiso")
    # el_iso.Multiply(multiemu)
    # el_iso.Multiply(convhit0chg)

    # f_gsf = r.TFile("rootfiles/egammaEffi.txt_SF2D.root")
    # h_gsf = get_first_hist(f_gsf)

    # # FULLSIM MUONS
    # muon_file_dxy = r.TFile("rootfiles/mu_dxy0p05dz0p1.root")
    # muon_file_mediumid = r.TFile("rootfiles/mu_mediumID.root")
    # muon_file_sip3d = r.TFile("rootfiles/mu_sip3dlt4.root")
    # muon_file_multiiso = r.TFile("rootfiles/mu_multiiso.root") 
    # muon_file_dpt = r.TFile("rootfiles/mu_dpt0p2.root") 

    # dxy0p05dz0p1 = get_first_hist(muon_file_dxy)
    # mediumID = get_first_hist(muon_file_mediumid)
    # sip3dlt4 = get_first_hist(muon_file_sip3d)
    # multiiso = get_first_hist(muon_file_multiiso) 
    # dpt = get_first_hist(muon_file_dpt) 

    # mu = dxy0p05dz0p1.Clone("mu")
    # mu.Multiply(mediumID)
    # mu.Multiply(sip3dlt4)
    # mu.Multiply(multiiso)
    # mu.Multiply(dpt) 

    # # FASTSIM ELECTRONS
    # el_fastsim_file = r.TFile("rootfiles/el_fs.root")
    # el_fastsim = el_fastsim_file.Get("histo2D")

    # el_fastsim_iso_file = r.TFile("rootfiles/el_fs_iso.root")
    # el_fastsim_iso = el_fastsim_iso_file.Get("histo2D")


    # # FASTSIM MUONS
    # muon_fs_file_dxy = r.TFile("rootfiles/mu_fs_dxy0p05dz0p1.root")
    # muon_fs_file_mediumid = r.TFile("rootfiles/mu_fs_mediumID.root")
    # muon_fs_file_sip3d = r.TFile("rootfiles/mu_fs_sip3dlt4.root")
    # muon_fs_file_multiiso = r.TFile("rootfiles/mu_fs_multiiso.root") 
    # muon_fs_file_dpt = r.TFile("rootfiles/mu_fs_dpt0p2.root") 

    # dxy0p05dz0p1_fs = get_first_hist(muon_fs_file_dxy)
    # mediumID_fs = get_first_hist(muon_fs_file_mediumid)
    # sip3dlt4_fs = get_first_hist(muon_fs_file_sip3d)
    # multiiso_fs = get_first_hist(muon_fs_file_multiiso) 
    # dpt_fs = get_first_hist(muon_fs_file_dpt) 

    # mu_fastsim = dxy0p05dz0p1_fs.Clone("mu")
    # mu_fastsim.Multiply(mediumID_fs)
    # mu_fastsim.Multiply(sip3dlt4_fs)
    # mu_fastsim.Multiply(multiiso_fs)
    # mu_fastsim.Multiply(dpt_fs) 

    # ### DO THE DUMPING

    # dump_bins(el_noiso, name="electronScaleFactorHighHT", nofabseta=True)
    # dump_bins(el_iso, name="electronScaleFactorLowHT", nofabseta=True)
    # dump_bins(mu, name="muonScaleFactor")
    # dump_bins(h_gsf,"electronGSF", stupid=True, default1=True)
    # dump_bins(el_fastsim, name="electronScaleFactorFastSimHighHT")
    # dump_bins(el_fastsim_iso, name="electronScaleFactorFastSimLowHT")
    # dump_bins(mu_fastsim, name="muonScaleFactorFastSim")
