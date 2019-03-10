import json
import pprint
import ROOT as r

def get_first_hist(f1):
    return f1.Get(f1.GetListOfKeys()[0].GetName())

def dump_bins(h2s, name, transpose=False, nofabseta=False, fallthrough=0., do_err=False,onebinfirsthist=False):
    buff = "float %s(float pt, float eta) {\n" % name
    if type(h2s) is not list:
        h2s = [h2s]
    for ih2,h2 in enumerate(h2s):
        if transpose:
            for iy in range(1,h2.GetNbinsY()+1):
                for ix in range(1,h2.GetNbinsX()+1):
                    if do_err:
                        val = h2.GetBinError(ix,iy)
                    else:
                        val = h2.GetBinContent(ix,iy)
                    if iy != h2.GetNbinsY() or h2.GetNbinsY() == 1:
                        buff += "  if (pt >= %.0f && pt < %.0f && fabs(eta) >= %.3f && fabs(eta) < %.3f) return %.4f;\n" \
                                % (h2.GetYaxis().GetBinLowEdge(iy), h2.GetYaxis().GetBinUpEdge(iy), h2.GetXaxis().GetBinLowEdge(ix), h2.GetXaxis().GetBinUpEdge(ix), val)
                    else:
                        buff += "  if (pt >= %.0f  && fabs(eta) >= %.3f && fabs(eta) < %.3f) return %.4f;\n" \
                                % (h2.GetYaxis().GetBinLowEdge(iy), h2.GetXaxis().GetBinLowEdge(ix), h2.GetXaxis().GetBinUpEdge(ix), val)
                if onebinfirsthist and ih2 == 0: break
        else:
            for ix in range(1,h2.GetNbinsX()+1):
                for iy in range(1,h2.GetNbinsY()+1):
                    if do_err:
                        val = h2.GetBinError(ix,iy)
                    else:
                        val = h2.GetBinContent(ix,iy)
                    if ix != h2.GetNbinsX() or h2.GetNbinsX() == 1:
                        buff += "  if (pt >= %.0f && pt < %.0f && fabs(eta) >= %.3f && fabs(eta) < %.3f) return %.4f;\n" \
                                % (h2.GetXaxis().GetBinLowEdge(ix), h2.GetXaxis().GetBinUpEdge(ix), h2.GetYaxis().GetBinLowEdge(iy), h2.GetYaxis().GetBinUpEdge(iy), val)
                    else:
                        buff += "  if (pt >= %.0f  && fabs(eta) >= %.3f && fabs(eta) < %.3f) return %.4f;\n" \
                                % (h2.GetXaxis().GetBinLowEdge(ix), h2.GetYaxis().GetBinLowEdge(iy), h2.GetYaxis().GetBinUpEdge(iy), val)
            if onebinfirsthist and ih2 == 0: break
    buff += "  return {};\n".format(fallthrough)
    buff += "}\n"
    if nofabseta:
        buff = buff.replace("fabs(eta)", "eta")
    return buff

def dump_bins_json(jsname, name, do_err=False):
    buff = "float %s(float pt, float eta) {\n" % name

    data = json.load(open(jsname))

    d_sane = {}
    for s_aetarange,pts in data["NUM_MediumID_DEN_genTracks"]["abseta_pt"].items():
        for s_ptrange,valerr in pts.items():
            aetarange = tuple(eval(s_aetarange.split(":")[-1]))
            ptrange = tuple(eval(s_ptrange.split(":")[-1]))
            val = valerr["value"]
            err = valerr["error"]
            if ptrange not in d_sane:
                d_sane[ptrange] = {}
            d_sane[ptrange][aetarange] = (val,err)
    for ipt,ptrange in enumerate(sorted(d_sane.keys())):
        for ieta, (etarange, (val,err)) in enumerate(sorted(d_sane[ptrange].items())):
            if do_err:
                thing = err
            else:
                thing = val
            if ipt != len(d_sane.keys())-1:
                buff += "  if (pt >= %.0f && pt < %.0f && fabs(eta) >= %.3f && fabs(eta) < %.3f) return %.4f;\n" \
                        % (ptrange[0], ptrange[1], etarange[0], etarange[1], thing)
            else:
                buff += "  if (pt >= %.0f  && fabs(eta) >= %.3f && fabs(eta) < %.3f) return %.4f;\n" \
                        % (ptrange[0], etarange[0], etarange[1], thing)
    buff += "  return 0.;\n"
    buff += "}\n"
    return buff

if __name__ == "__main__":

    outdir = "run2/"

    # # Electrons
    # # 2017
    # # Reco SF from EGM Twiki (https://twiki.cern.ch/twiki/bin/view/CMS/Egamma2017DataRecommendations#Electron_Reconstruction_Scale_Fa)
    # # - pT>20 from https://twiki.cern.ch/twiki/pub/CMS/Egamma2017DataRecommendations/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root
    # # - pT<20 from https://twiki.cern.ch/twiki/pub/CMS/Egamma2017DataRecommendations/egammaEffi.txt_EGM2D_runBCDEF_passingRECO_lowEt.root
    # # Specific to our ID from SUS Twiki (https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF#Scale_Factors_for_2017_Data, https://twiki.cern.ch/twiki/pub/CMS/SUSLeptonSF/ElectronScaleFactors_Run2017.root)
    # # - Run2017_MVATightIP2D3DIDEmu (MVA Tight ID + ID Emu + TightIP2D + TightIP3D wrt Reco)
    # # - Run2017_MultiIsoEmuJECv32 (MultiIso + IsoEmu (JECv32) wrt MVA Tight ID + ID Emu + TightIP2D + TightIP3D)
    # # - Run2017_ConvIHit0 (ConvVeto + MissHits = 0 wrt MVA Tight ID + ID Emu + TightIP2D + TightIP3D)
    # # - Run2017_3Qagree (3ChargeAgreement wrt MVA Tight ID + ID Emu + TightIP2D + TightIP3D + ConvVeto + MissHits = 0)
    # fh = open("{}/lepton_sfs_el_2017.h".format(outdir),"w")
    # f_el_reco_high = r.TFile("rootfiles_run2/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root")
    # f_el_reco_low = r.TFile("rootfiles_run2/egammaEffi.txt_EGM2D_runBCDEF_passingRECO_lowEt.root")
    # f_el_susy = r.TFile("rootfiles_run2/ElectronScaleFactors_Run2017.root")
    # h_el_reco_high = f_el_reco_high.Get("EGamma_SF2D")
    # h_el_reco_low = f_el_reco_low.Get("EGamma_SF2D")
    # h_el_mvatight = f_el_susy.Get("Run2017_MVATightIP2D3DIDEmu")
    # h_el_multiiso = f_el_susy.Get("Run2017_MultiIsoEmuJECv32")
    # h_el_convhit = f_el_susy.Get("Run2017_ConvIHit0")
    # h_el_3charge = f_el_susy.Get("Run2017_3Qagree")
    # h_el_mvatight.Multiply(h_el_multiiso)
    # h_el_mvatight.Multiply(h_el_convhit)
    # h_el_mvatight.Multiply(h_el_3charge)
    # # print dump_bins(h_el_mvatight, "electronScaleFactor_RunBCDEF", transpose=True, nofabseta=True)
    # # print dump_bins(h_el_mvatight, "electronScaleFactorError_RunBCDEF", transpose=True, nofabseta=True, do_err=True)
    # fh.write( dump_bins(h_el_mvatight, "electronScaleFactor_RunBCDEF", transpose=True, nofabseta=True) + "\n" )
    # fh.write( dump_bins(h_el_mvatight, "electronScaleFactorError_RunBCDEF", transpose=True, nofabseta=True, do_err=True) + "\n" )
    # # print dump_bins([h_el_reco_low,h_el_reco_high], "electronScaleFactorReco_RunBCDEF", transpose=True, nofabseta=True)
    # # print dump_bins([h_el_reco_low,h_el_reco_high], "electronScaleFactorRecoError_RunBCDEF", transpose=True, nofabseta=True, do_err=True)
    # fh.write( dump_bins([h_el_reco_low,h_el_reco_high], "electronScaleFactorReco_RunBCDEF", transpose=True, nofabseta=True) + "\n" )
    # fh.write( dump_bins([h_el_reco_low,h_el_reco_high], "electronScaleFactorRecoError_RunBCDEF", transpose=True, nofabseta=True, do_err=True) + "\n" )
    # fh.close()

    # 2018
    # Reco SF from EGM Twiki (https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2)
    # - pT>20 from https://twiki.cern.ch/twiki/pub/CMS/EgammaIDRecipesRun2/egammaEffi.txt_EGM2D.root
    # - pT<20 from https://twiki.cern.ch/twiki/pub/CMS/EgammaIDRecipesRun2/egammaEffi.txt_EGM2D_low.root
    # Specific to our ID from SUS Twiki (https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF#Scale_Factors_for_2018_Data, https://twiki.cern.ch/twiki/pub/CMS/SUSLeptonSF/ElectronScaleFactors_Run2018.root)
    # - Run2018_MVATightIP2D3DIDEmu (MVA Tight ID + ID Emu + TightIP2D + TightIP3D wrt Reco)
    # - Run2018_ConvIHit0 (ConvVeto + MissHits = 0 wrt MVA Tight ID + ID Emu + TightIP2D + TightIP3D)
    # - NOTE missing MultiIso part because they're waiting on JECs for 2018
    # - Run2018_3Qagree (3ChargeAgreement wrt MVA Tight ID + ID Emu + TightIP2D + TightIP3D + ConvVeto + MissHits = 0)

    # fh = open("{}/lepton_sfs_el_2018.h".format(outdir),"w")
    # f_el_reco_high = r.TFile("rootfiles_run2/egammaEffi.txt_EGM2D.root")
    # f_el_reco_low = r.TFile("rootfiles_run2/egammaEffi.txt_EGM2D_low.root")
    # f_el_susy_mva = r.TFile("rootfiles_run2/el2018/mvatightip2d3didemu_wrt_reco/egammaEffi.txt_EGM2D.root")
    # f_el_susy_multiiso = r.TFile("rootfiles_run2/el2018/multiisoemu_wrt_mvatightipd2d3didemu//egammaEffi.txt_EGM2D.root")
    # f_el_susy_convhit = r.TFile("rootfiles_run2/el2018/convhit0_wrt_mvatightipd2d3didemu//egammaEffi.txt_EGM2D.root")
    # f_el_susy_3charge = r.TFile("rootfiles_run2/el2018/threeq_wrt_everything//egammaEffi.txt_EGM2D.root")
    # h_el_reco_high = f_el_reco_high.Get("EGamma_SF2D")
    # h_el_reco_low = f_el_reco_low.Get("EGamma_SF2D")
    # h_el_mvatight = f_el_susy_mva.Get("EGamma_SF2D")
    # h_el_multiiso = f_el_susy_multiiso.Get("EGamma_SF2D")
    # h_el_convhit = f_el_susy_convhit.Get("EGamma_SF2D")
    # h_el_3charge = f_el_susy_3charge.Get("EGamma_SF2D")
    # h_el_mvatight.Multiply(h_el_multiiso)
    # h_el_mvatight.Multiply(h_el_convhit)
    # h_el_mvatight.Multiply(h_el_3charge)
    # # print dump_bins(h_el_mvatight, "electronScaleFactor_RunABCD", transpose=True, nofabseta=True)
    # # print dump_bins(h_el_mvatight, "electronScaleFactorError_RunABCD", transpose=True, nofabseta=True, do_err=True)
    # fh.write( dump_bins(h_el_mvatight, "electronScaleFactor_RunABCD", transpose=True, nofabseta=True) + "\n" )
    # fh.write( dump_bins(h_el_mvatight, "electronScaleFactorError_RunABCD", transpose=True, nofabseta=True, do_err=True) + "\n" )
    # # print dump_bins([h_el_reco_low,h_el_reco_high], "electronScaleFactorReco_RunABCD", transpose=True, nofabseta=True, onebinfirsthist=True) # only print first pt bin for first histogram
    # # print dump_bins([h_el_reco_low,h_el_reco_high], "electronScaleFactorRecoError_RunABCD", transpose=True, nofabseta=True, do_err=True, onebinfirsthist=True) # only print first pt bin for first histogram
    # fh.write( dump_bins([h_el_reco_low,h_el_reco_high], "electronScaleFactorReco_RunABCD", transpose=True, nofabseta=True, onebinfirsthist=True) + "\n" ) # only print first pt bin for first histogram
    # fh.write( dump_bins([h_el_reco_low,h_el_reco_high], "electronScaleFactorRecoError_RunABCD", transpose=True, nofabseta=True, do_err=True, onebinfirsthist=True) + "\n" ) # only print first pt bin for first histogram
    # fh.close()

    # Muons
    # 2017
    # Medium MuonID from MuonPOG (https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffs2017)
    # - https://twiki.cern.ch/twiki/pub/CMS/MuonReferenceEffs2017/RunBCDEF_SF_ID.root
    # Specific to our ID from SUS Twiki (https://twiki.cern.ch/twiki/bin/view/CMS/SUSLeptonSF#Muon)
    # Multi Iso Medium from http://jrgonzal.web.cern.ch/jrgonzal/MuonSF2017/v2/passMultiIsoM2017v2_MediumID/
    # NOTE missing other stuff like dpt,dxy,dz, ...
    fh = open("{}/lepton_sfs_mu_2017.h".format(outdir),"w")
    f_mu_medium = r.TFile("rootfiles_run2/RunBCDEF_SF_ID.root")
    f_mu_susy = r.TFile("rootfiles_run2/SF_new.root")
    h_mu_medium = f_mu_medium.Get("NUM_MediumID_DEN_genTracks_pt_abseta")
    h_mu_multiiso = f_mu_susy.Get("SF2D")
    x = dump_bins(h_mu_medium, "muonScaleFactor_Medium", fallthrough=1.) + "\n" 
    xe = dump_bins(h_mu_medium, "muonScaleFactorError_Medium", fallthrough=0.02, do_err=True) + "\n" 
    fh.write(x)
    fh.write(xe)
    x = dump_bins(h_mu_multiiso, "muonScaleFactor_RunBCDEF", fallthrough=1.,transpose=True) + "\n" 
    xe = dump_bins(h_mu_multiiso, "muonScaleFactorError_RunBCDEF", fallthrough=0.02, transpose=True, do_err=True) + "\n" 
    fh.write(x)
    fh.write(xe)
    fh.close()

    # 2018
    # NOTE only have the POG ones, rest is copied from 2017
    # https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffs2018
    fh = open("{}/lepton_sfs_mu_2018.h".format(outdir),"w")
    f_mu_medium = r.TFile("rootfiles_run2/RunABCD_SF_ID.root")
    f_mu_susy = r.TFile("rootfiles_run2/SF_new.root")
    h_mu_medium = f_mu_medium.Get("NUM_MediumID_DEN_genTracks_pt_abseta")
    h_mu_multiiso = f_mu_susy.Get("SF2D")
    x = dump_bins(h_mu_medium, "muonScaleFactor_Medium", fallthrough=1.) + "\n" 
    xe = dump_bins(h_mu_medium, "muonScaleFactorError_Medium", fallthrough=0.02, do_err=True) + "\n" 
    fh.write(x)
    fh.write(xe)
    x = dump_bins(h_mu_multiiso, "muonScaleFactor_RunABCD", fallthrough=1.,transpose=True) + "\n" 
    xe = dump_bins(h_mu_multiiso, "muonScaleFactorError_RunABCD", fallthrough=0.02, transpose=True, do_err=True) + "\n" 
    fh.write(x)
    fh.write(xe)
    fh.close()
