import ROOT as r
import numpy as np
import array
import sys, glob
import os
import scipy.optimize

r.gROOT.SetBatch(1)

# xvals=[0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0]
# xvals=[0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.2]
xvals = [0.1*i for i in range(1,29)]
lumi = 35.9
xlow = min(xvals)
xhigh = max(xvals)
xspace = xvals[1]-xvals[0]
yhigh = 65.
xsm = 1.0
ylow = 0.
nbinsx = len(xvals)
nbinsy = 10
magic2 = 2.08


from parse import get_stuff
ktvals, ul, central, ups, downs = get_stuff("scaninfo.txt")
# ktvals, ul, central, ups, downs = get_stuff("scaninfo_negative_theorylnN.txt")
# ktvals, ul, central, ups, downs = get_stuff("scaninfo_negative.txt")

obs=[16.9 for _ in xvals]
theory = [0.05*i*i+0.25 for i in range(len(xvals))]

# A: gza
# B: int 
# F: higgs
def calc_sigma(kt,A,B,F):
    return A + B*kt**2 + F*kt**4

# NLO/LO kfactor from 14TeV
kfactor = 1.27

gza_13tev = 9.997*kfactor
int_13tev = -1.547*kfactor
higgs_13tev = 1.168*kfactor
# scale variations for 13 tev numbers
gza_13tev_up = 14.104*kfactor
int_13tev_up = -2.152*kfactor
higgs_13tev_up = 1.625*kfactor
gza_13tev_down = 6.378*kfactor
int_13tev_down = -0.999*kfactor
higgs_13tev_down = 0.7655*kfactor

def get_kt(sigma, A,B,F):
    # A: gza
    # B: interference
    # F: higgs
    return np.sqrt(np.sqrt(B*B-4.*A*F+4.*F*sigma)/F  - B/F)/np.sqrt(2.0)
sigma_obs_ul = 41.7
print "Central: kt<{0:.2f} using obs UL of {1:.1f} for 13TeV with kfactor".format(get_kt(sigma_obs_ul, gza_13tev, int_13tev, higgs_13tev), sigma_obs_ul)
print "Up: kt<{0:.2f} using obs UL of {1:.1f} for 13TeV with kfactor".format(get_kt(sigma_obs_ul, gza_13tev_up, int_13tev_up, higgs_13tev_up), sigma_obs_ul)
print "Down: kt<{0:.2f} using obs UL of {1:.1f} for 13TeV with kfactor".format(get_kt(sigma_obs_ul, gza_13tev_down, int_13tev_down, higgs_13tev_down), sigma_obs_ul)
print "Using lnN 0.639/1.1412 in combine: kt<{0:.2f} using obs UL of {1:.1f} for 13TeV with kfactor".format(get_kt(58.01, gza_13tev, int_13tev, higgs_13tev), 58.01)


theory = [calc_sigma(kt,gza_13tev,int_13tev,higgs_13tev) for kt in xvals]
theorylow = [calc_sigma(kt,gza_13tev_down,int_13tev_down,higgs_13tev_down) for kt in xvals]
theoryhigh = [calc_sigma(kt,gza_13tev_up,int_13tev_up,higgs_13tev_up) for kt in xvals]

# print theory
# print theorylow
# print theoryhigh


# m2s=[0.7,0.7,0.7,0.7,0.7,0.7,0.7,0.7]
# m1s=[0.8,0.8,0.8,0.8,0.8,0.8,0.8,0.8]
# exp=[0.9,0.9,0.9,0.9,0.9,0.9,0.9,0.9]
# p1s=[1.1,1.1,1.1,1.1,1.1,1.1,1.1,1.1]
# p2s=[1.2,1.2,1.2,1.2,1.2,1.2,1.2,1.2]
# x0e=[0.,0.,0.,0.,0.,0.,0.,0.]

def decrease(vals,minus):
    return [v-minus for v in vals]

# ul= [41.7 for _ in xvals]


obs=decrease(obs,.0)
exp=obs[:]
m1s=decrease(exp,11.4)
p1s=decrease(exp,-13.8)
# m2s=decrease(obs,.3)
# p2s=decrease(obs,-0.25)
thm1s = list(np.array(theory)-np.array(theorylow))
thp1s = list(np.array(theoryhigh)-np.array(theory))
# thp1s = [0.2 for _ in theory]
x0e=[0. for _ in obs]

#TGraphAsymmErrors needs the delta
for i in range(0,len(exp)):
    m1s[i]=exp[i]-m1s[i]
    p1s[i]=p1s[i]-exp[i]

r.gStyle.SetOptStat(0)
c1 = r.TCanvas("c1", "", 800, 800)
c1.cd()
padt = r.TPad("p_tex", "p_tex", 0.0, 0.0, 1.0, 1.0)
padt.SetTopMargin(0.1)
padt.SetBottomMargin(0.12)
padt.SetRightMargin(0.05)
padt.SetLeftMargin(0.15)
padt.SetTickx()
padt.SetTicky()
padt.Draw()
padt.cd()

h = r.TH2F("h","h",nbinsx,xlow,xhigh,nbinsy,ylow,yhigh)
h.SetTitle("")
# h.GetXaxis().SetTitle("|y_{T}/y_{T}^{SM}|")
h.GetXaxis().SetTitle("|#it{y}_{t}/#it{y}_{t}^{SM}|")
h.GetYaxis().SetTitle("#sigma(t#bar{t}t#bar{t}) (fb)")
h.GetXaxis().SetLabelSize(0.04)
h.GetYaxis().SetLabelSize(0.04)
h.GetXaxis().SetTitleSize(0.045)
h.GetYaxis().SetTitleSize(0.045)
h.GetXaxis().SetTitleOffset(1.05)
h.GetYaxis().SetTitleOffset(1.5)

h.GetXaxis().SetLabelSize(0.045)
h.Draw()


# gr_s1b = r.TGraphAsymmErrors(len(xvals),array.array('d', xvals),array.array('d', exp),array.array('d', x0e),array.array('d', x0e),array.array('d', m1s),array.array('d', p1s))
# print zip([ktvals,central, downs, ups])
gr_s1b = r.TGraphAsymmErrors(
        len(ktvals),
        array.array('d', ktvals),
        array.array('d', central),
        array.array('d', [0. for _ in ktvals]),
        array.array('d', [0. for _ in ktvals]),
        array.array('d', downs),
        array.array('d', ups),
        )
gr_s1b.SetFillColor(r.kGray)
gr_s1b.SetLineStyle(1)
gr_s1b.SetLineWidth(3)
gr_s1b.SetLineColor(r.kBlack)
gr_s1b.Draw("3L")

# gexp = r.TGraph(len(xvals), array.array('d', xvals), array.array('d', exp))
gexp = r.TGraph(len(ktvals), array.array('d', ktvals), array.array('d', central))
gexp.SetLineStyle(1)
gexp.SetLineWidth(3)
gexp.SetLineColor(r.kBlack)
gexp.Draw("L")


r.gPad.Update()
r.gPad.RedrawAxis()

# print ktvals
# print ul
# print theory
# print thp1s
# print thm1s
# print xvals

gr_ulb = r.TGraphAsymmErrors(
        len(ktvals),
        array.array('d', ktvals),
        array.array('d', ul),
        array.array('d', [0. for _ in ul]),
        array.array('d', [0. for _ in ul]),
        array.array('d', [0.0 for _ in ul]),
        array.array('d', [3.5 for _ in ul]
            ))
gr_ulb.SetLineWidth(3)
gr_ulb.SetLineColor(r.kGray+2)
gr_ulb.SetFillStyle(3554)
gr_ulb.SetFillColor(r.kGray+2)
gr_ulb.Draw("3L")

gr_theory = r.TGraphAsymmErrors(len(xvals),array.array('d', xvals),array.array('d', theory),array.array('d', x0e),array.array('d', x0e),array.array('d', thm1s),array.array('d', thp1s))
gr_theory.SetFillColorAlpha(r.kAzure+2,0.4)
# gr_theory.SetFillColorAlpha(r.kViolet+5,0.5)
gr_theory.SetLineStyle(1)
gr_theory.SetLineWidth(2)
gr_theory.Draw("3C")

higcent = 1.43
higup = higcent+0.23
higdown = higcent-0.22
arxiv = r.TGraphAsymmErrors(
        2,
        array.array('d', [higcent,higcent]),
        array.array('d', [ylow,yhigh]),
        array.array('d', [0.22,0.22]),
        array.array('d', [0.23,0.23]),
        array.array('d', [100.,100.]),
        array.array('d', [0.,0.])
        )
arxiv.SetFillColorAlpha(r.kRed+2,0.5)
arxiv.SetLineColor(r.kRed-2)
arxiv.SetLineStyle(1)
arxiv.SetLineWidth(2)
arxiv.Draw("2LPC")

line = r.TLine(xsm,yhigh,xsm,ylow)
line.SetLineWidth(2)
line.SetLineStyle(7)
line.SetLineColor(r.kRed)
line.Draw()


xshift = 0.1
yshift = 0.
# leg = r.TLegend(0.6, 0.7, 0.90, 0.85)
# leg = r.TLegend(0.23, 0.68, 0.63, 0.88)
leg = r.TLegend(0.23+xshift, 0.72+yshift, 0.63+xshift, 0.87+yshift)
leg.AddEntry(None,"Obs. UL","")
leg.AddEntry(None,"Obs. #pm#sigma_{experiment}","")
leg.AddEntry(None,"LHC Run 1, arXiv:1606.02266","")
leg.AddEntry(None,"SM (LO #times 1.27)","")
leg.SetFillColorAlpha(r.kWhite, 0.80)

leg.Draw()

leg.SetTextSize(0.027)

leg.SetMargin(leg.GetMargin()*0.76)
# leg.SetMargin(leg.GetMargin()*1.25)
legHeight=abs(leg.GetY1()-leg.GetY2());
entryHeight=legHeight/leg.GetNRows();
legWidth=abs(leg.GetX2()-leg.GetX1());
halfFillWidth = legWidth*leg.GetMargin()/2;
nRows = leg.GetNRows();

yRowOffset = 0.40 - nRows*0.05
xPctFudge = 0.01
yPctFudge = 0.005+0.008;

pctTex = r.TLatex();
coords = []
for i in range(nRows):
  xPctNDC = xPctFudge+leg.GetX1()+halfFillWidth*0.8;
  yPctNDC = yPctFudge+leg.GetY1()+(i+yRowOffset)*entryHeight;
  coords.append([xPctNDC,yPctNDC])
  pctTex.SetTextSize(0.022);
  pctTex.SetTextAlign(22);
  pctTex.SetTextFont(42);
  # pctTex.DrawLatexNDC(xPctNDC,yPctNDC,"%s#scale[0.5]{#lower[-0.3]{%%}}"%str(i));
coords = coords[::-1]

def ndc_to_user(nx,ny):
    x1 = padt.GetX1()
    y1 = padt.GetY1()
    x2 = padt.GetX2()
    y2 = padt.GetY2()
    xuser = x1 + nx*(x2-x1)
    yuser = y1 + ny*(y2-y1)
    return xuser,yuser

# ux1 = 1.2
# ux2 = 1.4
# uy1 = 35.
# uy2 = 35.

dx = 0.15
dy = 1.3

idx = 1
ux1, uy1 = ndc_to_user(coords[idx][0],coords[idx][1])
ux1 -= 0.1
ux2 = ux1+dx
uy2 = uy1

LObs1 = r.TGraphAsymmErrors(2)
LObs1.SetFillColor(gr_s1b.GetFillColor())
LObs1.SetPoint(0,ux1,uy1)
LObs1.SetPointError(0,0.,0.,dy,dy)
LObs1.SetPoint(1,ux2,uy2)
LObs1.SetPointError(1,0.,0.,dy,dy)
LObs1.Draw("3L")
LObs2 = r.TGraphAsymmErrors(2)
LObs2.SetLineColor(gexp.GetLineColor())
LObs2.SetLineWidth(gexp.GetLineWidth())
LObs2.SetFillStyle(0)
LObs2.SetPoint(0,ux1,uy1)
LObs2.SetPoint(1,ux2,uy2)
LObs2.Draw("3L")

idx = 3
ux1, uy1 = ndc_to_user(coords[idx][0],coords[idx][1])
ux1 -= 0.1
ux2 = ux1+dx
uy2 = uy1

LTh1 = r.TGraphAsymmErrors(2)
LTh1.SetFillColor(gr_theory.GetFillColor())
LTh1.SetLineColor(gr_theory.GetLineColor())
LTh1.SetLineWidth(gr_theory.GetLineWidth())
LTh1.SetPoint(0,ux1,uy1)
LTh1.SetPointError(0,0.,0.,dy,dy)
LTh1.SetPoint(1,ux2,uy2)
LTh1.SetPointError(1,0.,0.,dy,dy)
LTh1.Draw("3L")


idx = 0
ux1, uy1 = ndc_to_user(coords[idx][0],coords[idx][1])
uy1 -= 0.5
ux1 -= 0.1
ux2 = ux1+dx
uy2 = uy1

LUpper1 = r.TGraphAsymmErrors(2)
LUpper1.SetFillStyle(gr_ulb.GetFillStyle())
LUpper1.SetFillColor(gr_ulb.GetFillColor())
LUpper1.SetLineColor(gr_ulb.GetLineColor())
LUpper1.SetLineWidth(gr_ulb.GetLineWidth())
LUpper1.SetPoint(0,ux1,uy1)
LUpper1.SetPointError(0,0.,0.,0,dy)
LUpper1.SetPoint(1,ux2,uy2)
LUpper1.SetPointError(1,0.,0.,0,dy)
LUpper1.Draw("3L")

idx = 2
ux1, uy1 = ndc_to_user(coords[idx][0],coords[idx][1])
ux1 -= 0.1
ux2 = ux1+dx
uy2 = uy1

LArxiv1 = r.TGraphAsymmErrors(2)
LArxiv1.SetFillColor(arxiv.GetFillColor())
LArxiv1.SetLineColor(arxiv.GetLineColor())
LArxiv1.SetLineWidth(arxiv.GetLineWidth())
LArxiv1.SetPoint(0,ux1,uy1)
LArxiv1.SetPointError(0,0.,0.,dy,dy)
LArxiv1.SetPoint(1,ux2,uy2)
LArxiv1.SetPointError(1,0.,0.,dy,dy)
LArxiv1.Draw("3L")



offset = 0.00

# masstex = r.TLatex(0.70,0.79, "H_{T} > 300 GeV" )
# smtex = r.TLatex(0.25,0.79, "SM" )
# smtex = r.TLatex(0.16,0.52, "SM" ) # horizontal to left of line
smtex = r.TLatex(0.36,0.52, "SM" ) # new position with ktvals
# smtex = r.TLatex(0.20,0.52, "SM" ) # vertical to left of line
smtex.SetNDC()
smtex.SetTextColor(r.kRed)
smtex.SetTextSize(0.036)
# smtex.SetTextAngle(90)
smtex.SetLineWidth(2)
smtex.SetTextFont(42)
smtex.Draw()

cmstex = r.TLatex(0.670,0.91-offset, "%.1f fb^{-1} (13 TeV)" % lumi)
cmstex.SetNDC()
cmstex.SetTextSize(0.04)
cmstex.SetLineWidth(2)
cmstex.SetTextFont(42)
cmstex.Draw()

cmstexbold = r.TLatex(0.16,0.91-offset, "CMS" )
cmstexbold.SetNDC()
cmstexbold.SetTextSize(0.05)
cmstexbold.SetLineWidth(2)
cmstexbold.SetTextFont(61)
cmstexbold.Draw()

cmstexprel = r.TLatex(0.28,0.91-offset, "Preliminary" )
cmstexprel.SetNDC()
cmstexprel.SetTextSize(0.04)
cmstexprel.SetLineWidth(2)
cmstexprel.SetTextFont(52)
cmstexprel.Draw()

fname = "plots/ihateroot.pdf"
c1.SaveAs(fname)
os.system("ic " + fname)

calculate = True
if calculate:
    ktvals = np.array(ktvals) # for non theory y values
    ul = np.array(ul)
    theory = np.array(theory)
    theorylow = np.array(theorylow)
    theoryhigh = np.array(theoryhigh)
    xvals = np.array(xvals) # for theory y values

    def fcrossing_main(xtest):
        return (np.interp(xtest,ktvals,ul) - np.interp(xtest,xvals,theory))**2.
    def fcrossing_plus(xtest):
        return (np.interp(xtest,ktvals,ul) - np.interp(xtest,xvals,theorylow))**2.
    def fcrossing_minus(xtest):
        return (np.interp(xtest,ktvals,ul) - np.interp(xtest,xvals,theoryhigh))**2.


    cent = scipy.optimize.minimize(fcrossing_main, 2.5)["x"][0]
    high = scipy.optimize.minimize(fcrossing_plus, 2.5)["x"][0]
    low = scipy.optimize.minimize(fcrossing_minus, 2.5)["x"][0]
    print "{:.2f},{:.2f},{:.2f}".format(cent, low, high)
    print "yt/ytSM < {:.2f} (+{:.2f} -{:.2f})".format(cent, cent-low, high-cent)
