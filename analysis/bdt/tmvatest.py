import ROOT
import sys
import os
import numpy as np

import argparse

parser = argparse.ArgumentParser()

parser.add_argument("input", help="input root file")
parser.add_argument("-o", "--output", help="output ROOT file name", default="LearningOutput.root")
parser.add_argument("-m", "--maxvars", help="maximum number of input variables", default=999)
parser.add_argument("-t", "--train", help="do training", action="store_true")

args = parser.parse_args()
 
# dotrain = True
dotrain = args.train
# inname = "output_toptag_75ifb_18sr_jet2020.root"
# outname = "LearningOutput_2020_notoptag.root"

# inname = "output_75ifb_18sr_nominal.root"
# outname = "LearningOutput_nominal.root"

inname = args.input
outname = args.output 
max_vars = int(args.maxvars)

if dotrain:
    f1 = ROOT.TFile.Open(inname)
    t1 = f1.Get('t')
     
    # Useful output information will be stored in a new root file:
    f_out = ROOT.TFile(outname,"RECREATE")
     
    # Create the TMVA factory
    ROOT.TMVA.Tools.Instance()
    factory = ROOT.TMVA.Factory("TMVAClassification", f_out,"AnalysisType=Classification")
    print dir(factory)

    loader = ROOT.TMVA.DataLoader("dataset")
     
    # Add the six variables to the TMVA factory as floats
    # all_vars = ["l1id", "l2id", "nbtags", "njets", "nleps", "wcands", "met", "ht", "htb", "mt1", "mt2", "dphil1j1", "dphil2j2", "dphil1j2", "dphil2j1", "dphil1l2", "dphij1j2", "dphil1met", "dphil2met", "dphij1met", "dphij2met", "detal1j1", "detal2j2", "detal1j2", "detal2j1", "detal1l2", "detaj1j2", "ml1j1", "ml2j2", "ml1j2", "ml2j1", "ml1l2", "mj1j2", "ptl1", "ptl2", "ptj1", "ptj2", "nlb40", "nmb40", "ntb40", "q1", "q2", "q3", "ht4ratio"]
    # nominal_vars = ["njets", "nbtags", "mt1", "mt2", "met", "ml1l2", "htb", "nleps", "ht", "mj1j2", "dphij1j2", "ptj1", "ptj2", "ml1j2", "ml1j1", "wcands", "dphil1j1", "detal1l2","nlb40","nmb40","ntb40","q1","q2","ht4ratio"]
    # nominal_vars = ["njets", "nbtags", "mt1", "mt2", "met", "ml1l2", "htb", "nleps", "ht", "mj1j2", "dphij1j2", "ptj1", "ptj2", "ml1j2", "ml1j1", "wcands", "dphil1j1", "detal1l2","nlb40","nmb40","ntb40","q1","ht4ratio"]
    new_vars = ["nbtags", "njets", "met", "ptl2", "nlb40", "ntb40", "nleps", "htb", "q1", "ptj1", "ptj6", "ptj7", "ml1j1", "dphil1l2", "maxmjoverpt", "ptl1", "detal1l2", "ptj8", "ptl3"]

    new_vars = new_vars[:max_vars]
    # small_vars = ["nbtags"]
    # test_vars = ["ht"]
    # for x in all_vars:
    # for x in new_vars:
    for x in new_vars:
    # for x in small_vars:
    # for x in test_vars:
        typ = "F"
        if x in ["nleps","wcands","q1","q2","q3","nlb40","nmb40","ntb40","njets","nbtags"]: typ = "I"
        loader.AddVariable(x,typ)
    loader.AddSpectator("weight","F")
    loader.AddSpectator("ptl1","F")
    loader.AddSpectator("ptl2","F")
    loader.AddSpectator("SR","F")
    loader.AddSpectator("br","F")
     
    # Link the signal and background to the root_data ntuple
    loader.AddBackgroundTree(t1)
    loader.AddSignalTree(t1)
    # factory.SetBackgroundWeightExpression("weight")
    # factory.SetSignalWeightExpression("weight")
    loader.SetBackgroundWeightExpression("weight")
    loader.SetSignalWeightExpression("weight")
     
    # cuts defining the signal and background sample
    extra = "1"
    # # extra = "(ht>300)"
    # extra = "(ht>600)"
    # sigCut = ROOT.TCut("(stype == 7) && "+extra)
    # bgCut = ROOT.TCut("(stype >= 0 && stype < 7) && "+extra) 
    sigCut = ROOT.TCut("(stype == 0) && "+extra)
    bgCut = ROOT.TCut("(stype > 0) && "+extra) 
    # Prepare the training/testing signal/background  
    loader.PrepareTrainingAndTestTree(sigCut,bgCut,"SplitMode=Random:NormMode=NumEvents:!V") 
     
    method = factory.BookMethod(loader, ROOT.TMVA.Types.kBDT, "BDT",
                       ":".join([
                           "!H",
                           "!V",
                           # "NTrees=2000", # orig 500
                           "NTrees=500", # orig 500
                           # "NTrees=150",
                           "nEventsMin=150",
                           "MaxDepth=5", # orig 4
                           "BoostType=AdaBoost",
                           "AdaBoostBeta=0.25", # orig 0.5
                           "SeparationType=GiniIndex",
                           "nCuts=20",
                           "PruneMethod=NoPruning",
                           # "MinNodeSize=5", # in %
                           ]))

    factory.TrainAllMethods() 
    factory.TestAllMethods() 
    factory.EvaluateAllMethods() 

    f_out.Close()

# f_out = ROOT.TFile("LearningOutput_2020_yestoptag.root")
f_out = ROOT.TFile(outname)
 
normfact = 10
# xmin,xmax = -0.55, 0.95
# xmin,xmax = -0.4, 0.7
xmin,xmax = -0.45, 0.95
Histo_training_S = ROOT.TH1D('Histo_training_S' , '%i x S (Train)'%normfact , 25 , xmin,xmax)
Histo_training_B = ROOT.TH1D('Histo_training_B' , 'B (Train)'               , 25 , xmin,xmax)
Histo_testing_S = ROOT.TH1D('Histo_testing_S'   , '%i x S (Test)'%normfact  , 25 , xmin,xmax)
Histo_testing_B = ROOT.TH1D('Histo_testing_B'   , 'B (Test)'                , 25 , xmin,xmax)
 
# Fetch the trees of events from the root file 
TrainTree = f_out.Get("dataset/TrainTree") 
TestTree = f_out.Get("dataset/TestTree") 
 
# Cutting on these objects in the trees will allow to separate true S/B SCut_Tree = 'classID>0.5'
BCut_Tree = 'classID<0.5 && (br)'
SCut_Tree = 'classID>0.5 && (br)'
 
TrainTree.Draw("BDT>>Histo_training_S",("%i*weight*("%normfact)+SCut_Tree+")")
TrainTree.Draw("BDT>>Histo_training_B","weight*("+BCut_Tree+")")
TestTree.Draw( "BDT>>Histo_testing_S",("%i*weight*("%normfact)+SCut_Tree+")")
TestTree.Draw( "BDT>>Histo_testing_B","weight*("+BCut_Tree+")")
 
# Create the color styles
Histo_training_S.SetLineColor(2)
Histo_training_S.SetMarkerColor(2)
Histo_training_S.SetFillColor(2)
Histo_testing_S.SetLineColor(2)
Histo_testing_S.SetMarkerColor(2)
Histo_testing_S.SetFillColor(2)
 
Histo_training_B.SetLineColor(4)
Histo_training_B.SetMarkerColor(4)
Histo_training_B.SetFillColor(4)
Histo_testing_B.SetLineColor(4)
Histo_testing_B.SetMarkerColor(4)
Histo_testing_B.SetFillColor(4)
 
# Histogram fill styles
Histo_training_S.SetFillStyle(4501)
Histo_training_B.SetFillStyle(4501)
Histo_training_S.SetFillColorAlpha(Histo_training_S.GetLineColor(),0.2)
Histo_training_B.SetFillColorAlpha(Histo_training_B.GetLineColor(),0.2)
Histo_testing_S.SetFillStyle(0)
Histo_testing_B.SetFillStyle(0)
 
# Histogram marker styles
Histo_testing_S.SetMarkerStyle(20)
Histo_testing_B.SetMarkerStyle(20)
Histo_testing_S.SetMarkerSize(0.7)
Histo_testing_B.SetMarkerSize(0.7)
 
# Set titles
Histo_training_S.GetXaxis().SetTitle("Discriminant")
Histo_training_S.GetYaxis().SetTitle("Counts/Bin")
 
# Draw the objects
# c1 = ROOT.TCanvas("c1","",800,600)
c1 = ROOT.TCanvas("c1","",400,400)
p1 = ROOT.TPad("p1","p1",0., 0.23, 1.0, 1.0)
p2 = ROOT.TPad("p2","p2",0., 0.0, 1.0, 0.23)
p2.Draw()
# p2.cd()
p1.Draw()
# p1.SetLogy(1)
p1.SetLogy(0)
p1.cd()
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptTitle(0)
Histo_training_S.Draw("HIST")
Histo_training_B.Draw("HISTSAME")
Histo_testing_S.Draw("EPSAME")
Histo_testing_B.Draw("EPSAME")

# Reset the y-max of the plot
ymax = max([h.GetMaximum() for h in [Histo_training_S,Histo_training_B,Histo_testing_S,Histo_testing_B] ])
ymax *=1.4
Histo_training_S.SetMaximum(ymax)
Histo_training_S.SetMinimum(0.01)
 
# Create Legend
c1.cd(1).BuildLegend( 0.42+0.3,  0.72,  0.57+0.3,  0.88).SetFillColor(0)
 

auc = -1.

# make soverb pad
soverb_cumulative = Histo_testing_S.Clone("soverb_cumulative")
soverb_cumulative.GetYaxis().SetRangeUser(0.01,9.)
soverb_cumulative.GetYaxis().SetNdivisions(505)
soverb_cumulative.GetYaxis().SetTitleSize(0.11)
soverb_cumulative.GetYaxis().SetTitleOffset(0.31)
soverb_cumulative.GetYaxis().SetLabelSize(0.13)
soverb_cumulative.GetYaxis().CenterTitle()
soverb_cumulative.GetXaxis().SetLabelSize(0.0)
soverb_cumulative.GetXaxis().SetTitle("")
soverb_cumulative.GetXaxis().SetTickSize(0.06)
soverb_cumulative.SetMarkerStyle(20)
soverb_cumulative.SetMarkerSize(0.7)    

p2.cd()
# arrs = np.array(list(Histo_training_S))/normfact
# arrb = np.array(list(Histo_training_B))
arrs = np.array(list(Histo_testing_S))/normfact
arrb = np.array(list(Histo_testing_B))
cumsum_s = np.cumsum(arrs[::-1])[::-1]
cumsum_b = np.cumsum(arrb[::-1])[::-1]
auc = abs(np.trapz(cumsum_s/np.sum(arrs),cumsum_b/np.sum(arrb)))
cumsum = (cumsum_s/cumsum_b)
cumsum[np.abs(cumsum)>1e5] = 1000.


cumsum_sqrtsb = cumsum_s/np.sqrt(cumsum_s+cumsum_b)
cumsum_sqrtsb[np.abs(cumsum_sqrtsb)>1e5] = 1000.
# for ibz, val in enumerate(cumsum):
#     soverb_cumulative.SetBinContent(ibz,val)
#     soverb_cumulative.SetBinError(ibz,0.)
for ibz, val in enumerate(cumsum_sqrtsb):
    if np.isnan(val): continue
    soverb_cumulative.SetBinContent(ibz,val)
    soverb_cumulative.SetBinError(ibz,0.)
maxsb = max(list(soverb_cumulative))
# print "GREP", maxsb, auc, max_vars, ",".join(new_vars)
soverb_cumulative.SetLineColor(ROOT.kBlue-2)
soverb_cumulative.SetMarkerColor(ROOT.kBlue-2)
# soverb_cumulative.GetYaxis().SetTitle("Cumulative s/b")
soverb_cumulative.GetYaxis().SetTitle("Cumulative s/#sqrt{s+b}")
soverb_cumulative.GetYaxis().SetRangeUser(0.01,2.)
soverb_cumulative.Draw("samepe")

p1.cd()
l1=ROOT.TLatex()
l1.SetNDC();
l1.DrawLatex(0.26,0.93,"BDT [AUC = %.3f] [%.3f]" % (auc,maxsb))

l1.SetTextSize(l1.GetTextSize()*0.5)
l1.DrawLatex(0.76,0.63,"N^{sig}_{train} = %.1f" % (Histo_training_S.Integral()/normfact))
l1.DrawLatex(0.76,0.58,"N^{sig}_{test} = %.1f" % (Histo_testing_S.Integral()/normfact))
l1.DrawLatex(0.76,0.53,"N^{bg}_{train} = %.1f" % Histo_training_B.Integral())
l1.DrawLatex(0.76,0.48,"N^{bg}_{test} = %.1f" % Histo_testing_B.Integral())
 
pname = 'plots/validation_bdt_{}.pdf'.format(outname.rsplit(".",1)[0].split("/")[-1])
c1.Print(pname)
os.system("ic "+pname)
