import os
import sys
import ROOT as r

r.gStyle.SetPaintTextFormat(".3F")
r.gStyle.SetPadLeftMargin(0.175)
r.gStyle.SetPadBottomMargin(0.14)

el_x_bin_labels = ["15-20","20-25","25-30","30-40","40-50","50-200"]
el_y_bin_labels = ["0-0.8","0.8-1.442","1.442-1.556","1.556-2","2-2.5"]
mu_x_bin_labels = ["10-15","15-20","20-25","25-30","30-40","40-50","50-200"]
mu_y_bin_labels = ["0-0.9","0.9-1.2","1.2-2.1","2.1-2.4"]

def do_draw(h,is_el=True):
    hnew = r.TH2D(h.GetName()+"new","",h.GetNbinsX(),0,h.GetNbinsX(),h.GetNbinsY(),0,h.GetNbinsY())
    for ix in range(1,h.GetNbinsX()+1):
        for iy in range(1,h.GetNbinsY()+1):
            hnew.SetBinContent(ix,iy,h.GetBinContent(ix,iy))
            hnew.SetBinError(ix,iy,h.GetBinError(ix,iy))
    h = hnew
    if is_el:
        for ix,label in enumerate(el_x_bin_labels):
            h.GetXaxis().SetBinLabel(ix+1,label)
        for iy,label in enumerate(el_y_bin_labels):
            h.GetYaxis().SetBinLabel(iy+1,label)
    else:
        for ix,label in enumerate(mu_x_bin_labels):
            h.GetXaxis().SetBinLabel(ix+1,label)
        for iy,label in enumerate(mu_y_bin_labels):
            h.GetYaxis().SetBinLabel(iy+1,label)
    c1 = r.TCanvas()
    h.Draw("colztexte")
    h.GetXaxis().SetTitle("{} p_{{T}} (GeV)".format("Electron" if is_el else "Muon"))
    h.GetYaxis().SetTitle("#eta")
    h.GetXaxis().SetMoreLogLabels()
    h.GetXaxis().SetNoExponent()
    h.GetYaxis().SetLabelSize(0.06)
    h.GetXaxis().SetLabelSize(0.06)
    h.GetXaxis().SetTitleSize(0.05)
    h.GetYaxis().SetTitleSize(0.05)
    h.GetYaxis().SetTitleOffset(1.55)
    h.GetXaxis().SetTitleOffset(1.1)
    h.SetMarkerSize(2)
    r.gStyle.SetPalette(r.kRainBow)
    r.gStyle.SetOptStat(0)
    h.GetZaxis().SetRangeUser(0.,1)
    # c1.SetLogx()
    h.SetTitle("Efficiency")
    os.system("mkdir -p plots/")
    fname = "plots/eff_{}.pdf".format("el" if is_el else "mu")
    c1.SaveAs(fname)
    os.system("ic "+fname)
    return h

# f_2016 = r.TFile("outputs/output_2016_tt.root")
# f_2017 = r.TFile("outputs/output_2017_tt.root")
# f_2018 = r.TFile("outputs/output_2018_tt.root")


hists = {}
fs = []
for year in [2016,2017,2018]:
    hists[year] = {}
    f = r.TFile("outputs_Jul8//output_{}_tt.root".format(year))
    fs.append(f)
    el_num = f.Get("h2d_el_pt_eta_pass")
    el_den = f.Get("h2d_el_pt_eta_all")
    mu_num = f.Get("h2d_mu_pt_eta_pass")
    mu_den = f.Get("h2d_mu_pt_eta_all")
    count = f.Get("h_cnt_eff").Integral() # FIXME change to h_cnt_eff
    lumi={2016:35.9,2017:41.5,2018:59.7}[year]
    scale = lumi/count
    for h in [el_num,el_den,mu_num,mu_den]: h.Scale(scale)
    hists[year] = dict(
            el_num=el_num,
            mu_num=mu_num,
            el_den=el_den,
            mu_den=mu_den,
            )



print hists

for flav in ["el","mu"]:
    h_num = hists[2016]["{}_num".format(flav)].Clone()
    h_den = hists[2016]["{}_den".format(flav)].Clone()
    h_num.Reset()
    h_den.Reset()
    years = [2016,2017,2018]
    # years = [2016,2017]
    for year in years: h_num.Add(hists[year]["{}_num".format(flav)])
    for year in years: h_den.Add(hists[year]["{}_den".format(flav)])
    h_num.Divide(h_den)
    h = do_draw(h_num,is_el=(flav=="el"))

    f = r.TFile("eff_{}_008.root".format(flav),"RECREATE")
    hout = h.Clone("histo2D")
    hout.Write()
    f.Close()

# h_2016_el = f_2016.Get("eff_el")
# h_2016_mu = f_2016.Get("eff_mu")
# h_2017_el = f_2017.Get("eff_el")
# h_2017_mu = f_2017.Get("eff_mu")
# h_2018_el = f_2018.Get("eff_el")
# h_2018_mu = f_2018.Get("eff_mu")

# do_draw(h_2016_el)
