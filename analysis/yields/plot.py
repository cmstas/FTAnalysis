import os
import sys
import ROOT as r
import sys
sys.path.insert(0,'../../')
from common.Software.dataMCplotMaker.dataMCplotMaker import dataMCplot
from analysis.limits.runLimits import get_lims
from analysis.limits.singleBinLimits import get_singlebin_limits
from analysis.limits.makeScan import make_scan
from analysis.limits.getPostFit import get_postfit_dict

if __name__ == "__main__":

    # # from https://github.com/sgnoohc/Ditto/blob/master/python/makeplot.py#L193-L200
    # mycolors = []
    # mycolors.append(r.TColor(7000 ,   0/255. ,   0/255. ,   0/255.))
    # mycolors.append(r.TColor(7001 , 213/255. ,  94/255. ,   0/255.)) #r
    # mycolors.append(r.TColor(7002 , 230/255. , 159/255. ,   0/255.)) #o
    # mycolors.append(r.TColor(7003 , 240/255. , 228/255. ,  66/255.)) #y
    # mycolors.append(r.TColor(7004 ,   0/255. , 158/255. , 115/255.)) #g
    # mycolors.append(r.TColor(7005 ,   0/255. , 114/255. , 178/255.)) #b
    # mycolors.append(r.TColor(7006 ,  86/255. , 180/255. , 233/255.)) #k
    # mycolors.append(r.TColor(7007 , 204/255. , 121/255. , 167/255.)) #p

    os.system("mkdir -p plots")

    r.gROOT.SetBatch(1)

    bginfo =   [
               ("flips", "Charge misid.",       r.kGray+2,     0.3),
               ("rares", "Rare SM",             r.kMagenta-7,  0.5),
               ("xg",    "X+#gamma",            r.kViolet+2,   0.5),
               ("ttww",  "t#bar{t}VV",  r.kAzure-4,     0.2),
               ("ttz",   "t#bar{t}Z",           r.kGreen-6,    0.2),
               ("fakes", "Nonprompt lep.",      18,            0.35),
               ("tth",   "t#bar{t}H",           r.kBlue-5,     0.2),
               ("ttw",   "t#bar{t}W",           r.kGreen+3,    0.2),
               # ("ttww",  "t#bar{t}W^{+}W^{-}",  r.kOrange,     0.2),
               ]

    bgnames, titles, colors, systs = map(list,zip(*bginfo))

    f1 = r.TFile("histos.root")

    cards_dir = "../limits/{0}".format(f1.Get("metadata").GetTitle())

    # commonopts = "--darkColorLines --lumi 35.9 --topYaxisTitle Data/Pred. --type Preliminary --poissonErrorsNoZeros --dataName Data --outOfFrame --systInclStat "
    commonopts = "--darkColorLines --lumi 35.9 --topYaxisTitle Data/Pred. --type Preliminary --poissonErrorsNoZeros --dataName Data --outOfFrame --systInclStat --systFillStyle 3344 "
    d_opts_br = {

          "ht"         : [("ttzcr","ttwcr","sr","br"), commonopts+"     --ratioUpperBound 4.0 --xAxisLabel H_{T} --isLinear --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset -0.0  "],
          "met"        : [("ttzcr","ttwcr","sr","br"), commonopts+"     --xAxisLabel E_{T}^{miss} --isLinear --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset -0.0  "],
          "njets"      : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel N_{jets} --noXaxisUnit --nDivisions 6 --noDivisionLabel --isLinear --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset 0.1  "],
          "nbtags"     : [("ttzcr","ttwcr","sr","br"), commonopts+"  --noDivisionLabel --noXaxisUnit --xAxisLabel N_{b} --nDivisions 4 --noXaxisUnit --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset 0.1  --makeTable "],
          "mtmin"      : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel m_{T}^{min} --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "mll"        : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel m_{ll} --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "type"       : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel type --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "charge"     : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel charge --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "nleps"      : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel Nleps --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "l1pt"       : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel ordered l1pt --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "l2pt"       : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel ordered l2pt --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "SRCR_TOTAL" : [("",),     commonopts+"  --xAxisLabel Region   --noDivisionLabel --noXaxisUnit --isLinear --noOverflow --legendUp -.03 --legendRight -0.05    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable    --percentageInBox --xAxisBinLabels CRZ,CRW,SR1,SR2,SR3,SR4,SR5,SR6,SR7,SR8 "],

          "SR_TOTAL"   : [("",),     commonopts+"   --xAxisLabel SR --noDivisionLabel --noXaxisUnit --isLinear --noOverflow --legendUp -.03 --legendRight -0.05    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable    --percentageInBox --xAxisBinLabels SR1,SR2,SR3,SR4,SR5,SR6,SR7,SR8 "],

          # "l3pt"       : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel ordered l3pt --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          # "disc"       : [("br",),   commonopts+"  --isLinear  --xAxisLabel disc  --legendUp .0 --legendRight -0.08    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable "],
          # "disc2"       : [("br",),  commonopts+" --isLinear  --xAxisLabel disc2  --legendUp .0 --legendRight -0.08    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable "],
          # "SRDISC_TOTAL"   : [("",), commonopts+"   --xAxisLabel SR_{disc} --noDivisionLabel --noXaxisUnit --isLinear --noOverflow --legendUp -.03 --legendRight -0.05    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable    --percentageInBox "],

          }

    for key in d_opts_br.keys():
        types, opts_str = d_opts_br[key]
        for typ in types:
            if len(typ) == 0:
                name = key[:]
            else:
                name = "{}_{}".format(typ,key)

            oname = "plots/%s.pdf" % name.replace("_TOTAL","")

            title = typ.upper()
            subtitle = ""
            d_newopts = {
                "outputName": oname,
            }

            print name, typ

            bgs = map(lambda x: f1.Get("{0}_{1}".format(name,x)), ["data", "tttt"]+bgnames)
            h_data,h_tttt,bgs = bgs[0], bgs[1], bgs[2:]
            h_data_empty = h_data.Clone("empty")
            h_data_empty.Reset()

            h_tttt.Sumw2()
            h_tttt.Scale(10.0)

            # if key == "SRCR_TOTAL":
            #     pairs = zip(*sorted(get_singlebin_limits(cards_dir, redo=True)))[1]
            #     buff = ["CRZ","CRW"]
            #     buff += ["SR%i#scale[0.5]{#color[4]{ #downarrow%.0ffb}}" % (ib,pairs[ib-1].get("exp",-1)) for ib in range(1,8+1)]
            #     d_newopts["xAxisBinLabels"] = ",".join(buff)

            if key == "SRCR_TOTAL":
                make_scan(cards_dir)
                os.system("cp scan.pdf plots/scan.pdf")

            # if key in ["SRCR_TOTAL", "SRDISC_TOTAL"]:
            if key in ["SRCR_TOTAL"]:
                regions="srcr"
                if "DISC" in key: regions="srdisc"
                d_lims = get_lims(card=cards_dir, regions=regions, redocard=True, redolimits=True, domcfakes=False)
                exp, expp1, expm1 = d_lims["exp"], d_lims["sp1"]-d_lims["exp"], d_lims["exp"]-d_lims["sm1"]
                subtitle = "#sigma^{UL}_{exp} = %.2f^{+%.1f}_{-%.1f} fb" % (exp, expp1, expm1)

            do_unblind = typ in ["ttwcr","ttzcr"]
            do_blind = not do_unblind
            if do_unblind:
                d_newopts["noTextBetweenPads"] = True
                dataMCplot(h_data, bgs=bgs, sigs=[h_tttt], sigtitles=["t#bar{t}t#bar{t} x 10"], systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)

            if key == "SR_TOTAL":
                new_h_tttt = h_tttt.Clone("new_tttt")
                new_h_tttt.Scale(0.1)
                new_bgs = bgs+[new_h_tttt]
                new_colors = colors+[r.kPink-1]
                new_systs = systs+[0.0]
                # new_titles = titles+["#color[2]{t#bar{t}t#bar{t}}"]
                new_titles = titles+["t#bar{t}t#bar{t}"]
                new_d_newopts = d_newopts.copy()
                new_d_newopts["outputName"] = d_newopts["outputName"].replace(".pdf","_blindstack.pdf")
                new_d_newopts["poissonErrorsNoZeros"] = False
                new_d_newopts["noTextBetweenPads"] = False
                new_d_newopts["preserveBackgroundOrder"] = True
                # new_d_newopts["flagLocation"] = "0.6,0.965,0.07"
                # new_d_newopts["flagLocation"] = "0.21,0.96,0.08"
                dataMCplot(h_data_empty, bgs=new_bgs, systs=new_systs, titles=new_titles, title="Prefit", subtitle=subtitle, colors=new_colors, opts=new_d_newopts, opts_str=opts_str)

                # os.system("ic {0}".format(new_d_newopts["outputName"]))
                # os.system("web {0}".format(new_d_newopts["outputName"]))

            if key == "SRCR_TOTAL":
                d_postfit = get_postfit_dict("mlfitname.root")
                new_d_newopts["outputName"] = d_newopts["outputName"].replace(".pdf","_blindstackpostfit.pdf")
                postfit_bgs = [d_postfit[bg.GetName().rsplit("_",1)[-1]] for bg in new_bgs]
                h_totalbgsyst = d_postfit["total_background"]
                dataMCplot(h_data_empty, bgs=postfit_bgs, titles=new_titles, title="Postfit", subtitle=subtitle, colors=new_colors, opts=new_d_newopts, opts_str=opts_str, total_syst=h_totalbgsyst)

            if do_blind:
                # d_newopts["flagLocation"] = "0.5,0.7,0.1" if do_unblind else "0.6,0.965,0.07"
                d_newopts["outputName"] = d_newopts["outputName"].replace(".pdf","_blind.pdf")
                d_newopts["poissonErrorsNoZeros"] = False
                d_newopts["noTextBetweenPads"] = False
                dataMCplot(h_data_empty, bgs=bgs, sigs=[h_tttt], sigtitles=["t#bar{t}t#bar{t} x 10"], systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)


            # os.system("ic {0}".format(d_newopts["outputName"]))

# os.system("cp ../misc/signal_regions.h plots/SR.extra")
# os.system("cp ../misc/signal_regions.h plots/SR_blind.extra")
# os.system("cp ../misc/signal_regions.h plots/SRCR_blind.extra")
# os.system("niceplots plots plots_tttt_Jun16")
os.system("niceplots plots plots_tttt_Jun18")
