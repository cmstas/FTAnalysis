import os
import ROOT as r


tot_num = None
tot_den = None
tot_num_94x = None
tot_den_94x = None
fs = []
mcrate = {}
# for y in [2016,2017,2018,"2016_94x"]:
for y in [2016,2017,2018]:
    f = r.TFile("outputs/histos_{}.root".format(y))
    fs.append(f)
    nss_ = f.Get("nSS_run_data_plot")
    nos_ = f.Get("nOS_run_data_plot")
    nss_.Sumw2()
    nos_.Sumw2()
    print nss_, nos_
    print nss_.Integral(), nos_.Integral()
    mc_nss_ = f.Get("clos_mll_plot_MC").Integral()
    mc_nos_ = f.Get("osee_mll_plot_MC").Integral()
    mcrate[y] = mc_nss_/mc_nos_

    if "94x" in str(y):
        tot_num_94x = nss_.Clone("numer_{}".format(y))
        tot_den_94x = nos_.Clone("denom_{}".format(y))

    if not tot_num:
        tot_num = nss_.Clone("numer_{}".format(y))
        tot_den = nos_.Clone("denom_{}".format(y))
    else:
        tot_num.Add(nss_)
        tot_den.Add(nos_)

tot_num.Rebin(5)
tot_den.Rebin(5)
tot_num.Divide(tot_num,tot_den,1,1,"B")

if tot_num_94x:
    tot_num_94x.Rebin(5)
    tot_den_94x.Rebin(5)
    tot_num_94x.Divide(tot_num_94x,tot_den_94x,1,1,"B")

c1 = r.TCanvas()
tot_num.SetLineColor(r.kBlack)
tot_num.Draw("PE")

if tot_num_94x:
    tot_num_94x.SetLineColor(r.kBlue)
    # tot_num_94x.Draw("PESAME")
    tot_num_94x.SetLineWidth(2)

tot_num.SetLineWidth(2)
tot_num.GetXaxis().SetTitle("Run number")
# tot_num.GetYaxis().SetTitle("Flip rate")
tot_num.SetTitle("Flip rate vs time")

# tot_num.Scale(1000000)

r.gStyle.SetOptStat(0)
# c1.BuildLegend()
line = r.TLine()
line.SetLineColor(r.kRed)
line.SetLineWidth(2)
line.DrawLine(272007,mcrate[2016],284044,mcrate[2016])
line.DrawLine(297020,mcrate[2017],306462,mcrate[2017])
line.DrawLine(315252,mcrate[2018],325175,mcrate[2018])
line.SetLineColor(r.kBlue)
# line.DrawLine(272007,mcrate["2016_94x"],284044,mcrate["2016_94x"])

t = r.TLatex()
t.SetTextAlign(22)
t.SetTextFont(42)
t.SetTextColor(r.kBlack)
t.SetTextSize(0.05)
t.DrawLatex(0.5*(272007+284044),0.0002,"2016")
t.DrawLatex(0.5*(297020+306462),0.0002,"2017")
t.DrawLatex(0.5*(315252+325175),0.0002,"2018")

k = 0.01
leg = r.TLegend(0.5+k,0.75,0.83+k,0.85)
# leg.SetNColumns(2)
leg.SetTextFont(42)
leg.SetBorderSize(0)
# leg.AddEntry(tot_num, "nominal data, #color[4]{94X 2016}","LPE")
leg.AddEntry(tot_num, "data (80X, 94X, 102X)","LPE")
tmp = tot_num.Clone("tmp")
tmp.SetLineWidth(3)
tmp.SetMarkerColor(r.kRed)
tmp.SetLineColor(r.kRed)
# leg.AddEntry(tmp, "nominal MC, #color[4]{94X 2016}")
leg.AddEntry(tmp, "MC (80X, 94X, 102X)")
leg.Draw()

c1.SaveAs("fliprate_vs_time.pdf")
os.system("ic fliprate_vs_time.pdf")

