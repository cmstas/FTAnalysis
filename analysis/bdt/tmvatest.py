import ROOT
import sys
import os
import numpy as np
 
dotrain = False
if dotrain:
    f1 = ROOT.TFile.Open('output.root')
    t1 = f1.Get('t')
     
    # Useful output information will be stored in a new root file:
    f_out = ROOT.TFile("LearningOutput.root","RECREATE")
     
    # Create the TMVA factory
    ROOT.TMVA.Tools.Instance()
    factory = ROOT.TMVA.Factory("TMVAClassification", f_out,"AnalysisType=Classification")
     
    # Add the six variables to the TMVA factory as floats
    all_vars = ["l1id", "l2id", "nbtags", "njets", "nleps", "wcands", "met", "ht", "htb", "mt1", "mt2", "dphil1j1", "dphil2j2", "dphil1j2", "dphil2j1", "dphil1l2", "dphij1j2", "dphil1met", "dphil2met", "dphij1met", "dphij2met", "detal1j1", "detal2j2", "detal1j2", "detal2j1", "detal1l2", "detaj1j2", "ml1j1", "ml2j2", "ml1j2", "ml2j1", "ml1l2", "mj1j2", "ptl1", "ptl2", "ptj1", "ptj2", "nlb40", "nmb40", "ntb40", "q1", "q2", "q3", "ht4ratio"]
    # nominal_vars = ["njets", "nbtags", "mt1", "mt2", "met", "ml1l2", "htb", "nleps", "ht", "mj1j2", "dphij1j2", "ptj1", "ptj2", "ml1j2", "ml1j1", "wcands", "dphil1j1", "detal1l2","nlb40","nmb40","ntb40","q1","q2","ht4ratio"]
    nominal_vars = ["njets", "nbtags", "mt1", "mt2", "met", "ml1l2", "htb", "nleps", "ht", "mj1j2", "dphij1j2", "ptj1", "ptj2", "ml1j2", "ml1j1", "wcands", "dphil1j1", "detal1l2","nlb40","nmb40","ntb40","q1","ht4ratio"]
    small_vars = ["nbtags"]
    test_vars = ["ht"]
    # for x in all_vars:
    for x in nominal_vars:
    # for x in small_vars:
    # for x in test_vars:
        typ = "F"
        if x in ["nleps","wcands","q1","q2","q3","nlb40","nmb40","ntb40","njets","nbtags"]: typ = "I"
        factory.AddVariable(x,typ)
    factory.AddSpectator("weight","F")
    factory.AddSpectator("ptl1","F")
    factory.AddSpectator("ptl2","F")
    factory.AddSpectator("SR","F")
     
    # Link the signal and background to the root_data ntuple
    factory.AddBackgroundTree(t1)
    factory.AddSignalTree(t1)
    # factory.SetBackgroundWeightExpression("weight")
    # factory.SetSignalWeightExpression("weight")
    factory.SetBackgroundWeightExpression("weight")
    factory.SetSignalWeightExpression("weight")
     
    # cuts defining the signal and background sample
    extra = "1"
    # # extra = "(ht>300)"
    # extra = "(ht>600)"
    sigCut = ROOT.TCut("name == \"tttt\" && "+extra)
    # bgCut = ROOT.TCut("name != \"tttt\" && "+extra) 
    bgCut = ROOT.TCut("name == \"ttw\" && "+extra)  # FIXME go back to previous line
    # Prepare the training/testing signal/background  
    factory.PrepareTrainingAndTestTree(sigCut,bgCut,"SplitMode=Random:NormMode=NumEvents:!V") 
     
    method = factory.BookMethod(ROOT.TMVA.Types.kBDT, "BDT",
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

f_out = ROOT.TFile("LearningOutput.root")
 
normfact = 10
# xmin,xmax = -0.55, 0.95
# xmin,xmax = -0.4, 0.7
xmin,xmax = -0.45, 0.95
Histo_training_S = ROOT.TH1D('Histo_training_S' , '%i x S (Train)'%normfact , 25 , xmin,xmax)
Histo_training_B = ROOT.TH1D('Histo_training_B' , 'B (Train)'               , 25 , xmin,xmax)
Histo_testing_S = ROOT.TH1D('Histo_testing_S'   , '%i x S (Test)'%normfact  , 25 , xmin,xmax)
Histo_testing_B = ROOT.TH1D('Histo_testing_B'   , 'B (Test)'                , 25 , xmin,xmax)
 
# Fetch the trees of events from the root file 
TrainTree = f_out.Get("TrainTree") 
TestTree = f_out.Get("TestTree") 
 
# Cutting on these objects in the trees will allow to separate true S/B SCut_Tree = 'classID>0.5'
BCut_Tree = 'classID<0.5 && (SR>0)'
SCut_Tree = 'classID>0.5 && (SR>0)'
 
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
arrs = np.array(list(Histo_training_S))/normfact
arrb = np.array(list(Histo_training_B))
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
    soverb_cumulative.SetBinContent(ibz,val)
    soverb_cumulative.SetBinError(ibz,0.)
soverb_cumulative.SetLineColor(ROOT.kBlue-2)
soverb_cumulative.SetMarkerColor(ROOT.kBlue-2)
# soverb_cumulative.GetYaxis().SetTitle("Cumulative s/b")
soverb_cumulative.GetYaxis().SetTitle("Cumulative s/#sqrt{s+b}")
soverb_cumulative.GetYaxis().SetRangeUser(0.01,1.)
soverb_cumulative.Draw("samepe")

p1.cd()
l1=ROOT.TLatex()
l1.SetNDC();
l1.DrawLatex(0.26,0.93,"BDT [AUC = %.2f]" % auc)

l1.SetTextSize(l1.GetTextSize()*0.5)
l1.DrawLatex(0.76,0.63,"N^{sig}_{train} = %.1f" % (Histo_training_S.Integral()/normfact))
l1.DrawLatex(0.76,0.58,"N^{sig}_{test} = %.1f" % (Histo_testing_S.Integral()/normfact))
l1.DrawLatex(0.76,0.53,"N^{bg}_{train} = %.1f" % Histo_training_B.Integral())
l1.DrawLatex(0.76,0.48,"N^{bg}_{test} = %.1f" % Histo_testing_B.Integral())
 
pname = 'validation_bdt.pdf'
c1.Print(pname)
os.system("ic "+pname)
