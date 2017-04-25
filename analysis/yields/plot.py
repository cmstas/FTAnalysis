import os
import sys
import ROOT as r
import sys
sys.path.insert(0,'../../common/Software/dataMCplotMaker/')
from dataMCplotMaker import dataMCplot

if __name__ == "__main__":

    os.system("mkdir -p plots")

    r.gROOT.SetBatch(1)

    # systs = [0.2, 0.2, 0.3, 0.3, 0.5, 0.5, 0.3, 0.35]
    # colors = [r.kGreen+3, r.kGreen-6, r.kOrange, r.kOrange-3, r.kViolet+2, r.kMagenta-7, r.kGray+2, 18]
    # titles = ["t#bar{t}W", "t#bar{t}Z/H", "WZ", "WW", "X+#gamma", "Rare SM", "Charge misid.", "Nonprompt lep."]
    systs = [0.2, 0.2, 0.2, 0.3, 0.3, 0.5, 0.5, 0.3, 0.35]
    colors = [r.kGreen+3, r.kGreen-6, r.kBlue-5, r.kOrange, r.kOrange-3, r.kViolet+2, r.kMagenta-7, r.kGray+2, 18]
    titles = ["t#bar{t}W", "t#bar{t}Z", "t#bar{t}H", "WZ", "WW", "X+#gamma", "Rare SM", "Charge misid.", "Nonprompt lep."]
    # f1 = r.TFile("histos_Mar29.root")
    f1 = r.TFile("histos.root")
    # f1 = r.TFile("histos_nonttwcr.root")
    # f1 = r.TFile("histos_nominal.root")
    # f1 = r.TFile("histos_ttwisr.root")
    # f1 = r.TFile("histos_ttzisr.root")

    d_opts_br = {

          "ht"         : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9     --ratioUpperBound 4.0 --xAxisLabel H_{T} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred. --legendTaller 0.15 --yTitleOffset -0.0 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "met"        : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9     --xAxisLabel E_{T}^{miss} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred. --legendTaller 0.15 --yTitleOffset -0.0 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "njets"      : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel N_{jets} --noXaxisUnit --nDivisions 6 --noDivisionLabel --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred. --legendTaller 0.15 --yTitleOffset 0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros   --outOfFrame "],
          "nbtags"     : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9  --noDivisionLabel --noXaxisUnit --xAxisLabel N_{b} --nDivisions 4 --noXaxisUnit --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset 0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros   --outOfFrame --makeTable "],
          "mtmin"      : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel m_{T}^{min} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "mll"        : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel m_{ll} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "mlle"       : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel m_{ee} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "mllmu"      : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel m_{#mu#mu} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "mllem"      : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel m_{e#mu} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "type"       : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel type --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "charge"     : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel charge --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "mt2real"    : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel m_{T}^{2} --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "nleps"      : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel Nleps --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "wcands"     : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel Wcands --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "l1pt"       : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel ordered l1pt --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "l2pt"       : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel ordered l2pt --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "l3pt"       : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel ordered l3pt --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "el_l1pt"    : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel ordered el_l1pt --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "el_l2pt"    : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel ordered el_l2pt --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "el_l3pt"    : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel ordered el_l3pt --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "mu_l1pt"    : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel ordered mu_l1pt --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "mu_l2pt"    : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel ordered mu_l2pt --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "mu_l3pt"    : [("ttzcr","ttwcr","sr","br"), "--lumi 35.9   --xAxisLabel ordered mu_l3pt --isLinear --legendUp -.15 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.15 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame "],
          "disc"       : [("br",), "--lumi 35.9   --xAxisLabel disc  --legendUp .0 --legendRight -0.08 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.05 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame --makeTable "],

          "SRCR_TOTAL" : [("",), "--lumi 35.9   --xAxisLabel SR --isLinear --noOverflow --legendUp -.01 --legendRight -0.05 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.05 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame --makeTable    --percentageInBox --xAxisBinLabels CRZ,CRW,SR1,SR2,SR3,SR4,SR5,SR6,SR7 "],
          "SR_TOTAL"   : [("",), "--lumi 35.9   --xAxisLabel SR --isLinear --noOverflow --legendUp -.01 --legendRight -0.05 --systInclStat --topYaxisTitle Data/Pred.  --legendTaller 0.05 --yTitleOffset -0.1 --dataName Data  --type Preliminary --poissonErrorsNoZeros  --outOfFrame --makeTable    --percentageInBox --xAxisBinLabels SR1,SR2,SR3,SR4,SR5,SR6,SR7 "],


          }
    # for name in ["met", "ht", "mtmin", "njets", "nbtags","nleps","wcands", "topcands", \
    #         "mll", "mlle", "mllmu", "mllem", "type", "charge", "mt2real", "SR_TOTAL"]:

    # 0    nb0 l1
    # 7.5  nb3 l1
    # 12.5 nb4 l1
    # 16.5 nb2 l2
    # 21.5 nb3 l3
    # 23.5 nb4 l4

    # for name in ["SR_TOTAL"]:
    for key in d_opts_br.keys():
        types, opts_str = d_opts_br[key]
        for typ in types:
            if len(typ) == 0:
                name = key[:]
            else:
                name = "{}_{}".format(typ,key)
            # print name

            # name = name.split("_",1)[-1]
            # print name
            oname = "plots/%s.pdf" % name.replace("_TOTAL","")

            title = typ.upper()
            subtitle = ""
            d_newopts = {
                "outputName": oname,
            }

            # print oname
            # continue

            # opts_str = d_opts_br[name]


            # bgs = map(lambda x: f1.Get("{0}_{1}".format(name,x)), ["data","tttt","ttw","ttzh","wz","ww","xg","rares","flips","fakes"])
            bgs = map(lambda x: f1.Get("{0}_{1}".format(name,x)), ["data","tttt","ttw","ttz","tth","wz","ww","xg","rares","flips","fakes"])
            h_data,h_tttt,bgs = bgs[0], bgs[1], bgs[2:]
            # print h_data
            # print h_tttt
            h_data_empty = h_data.Clone("empty")
            h_data_empty.Reset()

            print name
            # print h_data, h_tttt, bgs

            h_tttt.Sumw2()
            h_tttt.Scale(10.0)

            dataMCplot(h_data, bgs=bgs, sigs=[h_tttt], sigtitles=["t#bar{t}t#bar{t} x 10"], systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)

            d_newopts["outputName"] = d_newopts["outputName"].replace(".pdf","_blind.pdf")
            d_newopts["poissonErrorsNoZeros"] = False
            dataMCplot(h_data_empty, bgs=bgs, sigs=[h_tttt], sigtitles=["t#bar{t}t#bar{t} x 10"], systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)

        # os.system("ic " + d_newopts["outputName"])

# os.system("ic plots/SRCR_blind.pdf")
os.system("cp ../../test.h plots/SR.extra")
os.system("niceplots plots plots_tttt_Apr22")

# os.system("niceplots plots plots_tttt_Apr11_nonttwcr")
# os.system("niceplots plots plots_tttt_Apr11_ttwcr")
# os.system("niceplots plots plots_tttt_Apr11_ttwisr")
# os.system("niceplots plots plots_tttt_Apr11_ttzisr")
