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

    os.system("mkdir -p plots")

    r.gROOT.SetBatch(1)

    bginfo =   [
               ("flips", "Charge misid.",       r.kGray+2,     0.3),
               ("rares", "Rare SM",             r.kMagenta-7,  0.5),
               ("xg",    "X+#gamma",            r.kViolet+2,   0.5),
               ("ttvv",  "t#bar{t}VV",  r.kAzure-4,     0.2),
               ("ttz",   "t#bar{t}Z",           r.kGreen-6,    0.35),
               ("fakes", "Nonprompt lep.",      18,            0.35),
               ("tth",   "t#bar{t}H",           r.kBlue-5,     0.35),
               ("ttw",   "t#bar{t}W",           r.kGreen+3,    0.35),
               ]

    bgnames, titles, colors, systs = map(list,zip(*bginfo))

    f1 = r.TFile("histos.root")

    cards_dir = "../limits/{0}".format(f1.Get("metadata").GetTitle())

    commonopts = "--darkColorLines --lumi 35.9 --topYaxisTitle Data/Pred. --type Preliminary --poissonErrorsNoZeros --dataName Data --outOfFrame --systInclStat --systFillStyle 3344 "
    d_opts_br = {

          # "metnm1"        : [("br",), commonopts+"     --xAxisLabel N-1 E_{T}^{miss} --isLinear --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset -0.0  --makeTable "],

          "ht"         : [("ttzcr","ttwcr","sr","br"), commonopts+"     --ratioUpperBound 4.0 --xAxisLabel H_{T} --isLinear --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset -0.0  "],
          "met"        : [("ttzcr","ttwcr","sr","br"), commonopts+"     --xAxisLabel E_{T}^{miss} --isLinear --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset -0.0  "],
          "mvis"       : [("ttzcr","ttwcr","sr","br"), commonopts+"     --xAxisLabel m^{vis} --isLinear --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset -0.0  "],
          "mtvis"      : [("ttzcr","ttwcr","sr","br"), commonopts+"     --xAxisLabel m_{T}^{vis} --isLinear --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset -0.0  "],
          "njets"      : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel N_{jets} --noXaxisUnit --nDivisions 6 --noDivisionLabel --isLinear --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset 0.1  "],
          "nbtags"     : [("ttzcr","ttwcr","sr","br"), commonopts+"  --noDivisionLabel --noXaxisUnit --xAxisLabel N_{b} --nDivisions 4 --noXaxisUnit --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset 0.1  --makeTable "],
          "mtmin"      : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel m_{T}^{min} --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "mll"        : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel m_{ll} --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "mllos"        : [("ttzcr",), commonopts+"   --xAxisLabel Z cand m_{ll} --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "type"       : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel type --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "charge"     : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel charge --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "nleps"      : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel Nleps --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "l1pt"       : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel ordered l1pt --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "l2pt"       : [("ttzcr","ttwcr","sr","br"), commonopts+"   --xAxisLabel ordered l2pt --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "l3pt"       : [("ttzcr",), commonopts+"   --xAxisLabel ordered l3pt --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "SR_TOTAL"   : [("",),     commonopts+"   --xAxisLabel SR --noDivisionLabel --noXaxisUnit --isLinear --noOverflow --legendUp -.03 --legendRight -0.05    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable    --percentageInBox --xAxisBinLabels SR1,SR2,SR3,SR4,SR5,SR6,SR7,SR8 "],
          "SRCR_TOTAL" : [("",),     commonopts+"  --xAxisLabel Region   --noDivisionLabel --noXaxisUnit --isLinear --noOverflow --legendUp -.03 --legendRight -0.05    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable    --percentageInBox --xAxisBinLabels CRZ,CRW,SR1,SR2,SR3,SR4,SR5,SR6,SR7,SR8 "],
          "mtop1"        : [("sr",), commonopts+"     --xAxisLabel m_{top,1} --isLinear --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset -0.0  "],
          "mtop2"        : [("sr",), commonopts+"     --xAxisLabel m_{top,2} --isLinear --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset -0.0  "],

          "mva"       : [("sr","br"), commonopts+"   --xAxisLabel lep1,2 el MVA --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "sip3d_mu_lep1"       : [("sr","br"), commonopts+"   --xAxisLabel lep1 mu sip3d --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "sip3d_mu_lep2"       : [("sr","br"), commonopts+"   --xAxisLabel lep2 mu sip3d --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "mu_l1pt"       : [("sr","br"), commonopts+"   --xAxisLabel lep1 mu pt --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "mu_l2pt"       : [("sr","br"), commonopts+"   --xAxisLabel lep2 mu pt --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "mu_l3pt"       : [("sr","br"), commonopts+"   --xAxisLabel lep3 mu pt --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "l1eta_mu"       : [("sr","br"), commonopts+"   --xAxisLabel lep1 mu eta --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "l2eta_mu"       : [("sr","br"), commonopts+"   --xAxisLabel lep2 mu eta --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "lep1_mu_miniIso"       : [("sr","br"), commonopts+"   --xAxisLabel lep1 mu miniIso --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "lep2_mu_miniIso"       : [("sr","br"), commonopts+"   --xAxisLabel lep2 mu miniIso --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "lep1_mu_ptRel"       : [("sr","br"), commonopts+"   --xAxisLabel lep1 mu ptRel --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "lep1_mu_ptRel"       : [("sr","br"), commonopts+"   --xAxisLabel lep2 mu ptRel --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],

          "sip3d_el_lep1"       : [("sr","br"), commonopts+"   --xAxisLabel lep1 el sip3d --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "sip3d_el_lep2"       : [("sr","br"), commonopts+"   --xAxisLabel lep2 el sip3d --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "el_l1pt"       : [("sr","br"), commonopts+"   --xAxisLabel lep1 el pt --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "el_l2pt"       : [("sr","br"), commonopts+"   --xAxisLabel lep2 el pt --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "el_l3pt"       : [("sr","br"), commonopts+"   --xAxisLabel lep3 el pt --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "l1eta_el"       : [("sr","br"), commonopts+"   --xAxisLabel lep1 el eta --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "l2eta_el"       : [("sr","br"), commonopts+"   --xAxisLabel lep2 el eta --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "lep1_el_miniIso"       : [("sr","br"), commonopts+"   --xAxisLabel lep1 el miniIso --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "lep2_el_miniIso"       : [("sr","br"), commonopts+"   --xAxisLabel lep2 el miniIso --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "lep1_el_ptRel"       : [("sr","br"), commonopts+"   --xAxisLabel lep1 el ptRel --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "lep1_el_ptRel"       : [("sr","br"), commonopts+"   --xAxisLabel lep2 el ptRel --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],

          "bjetpt"       : [("sr","br"), commonopts+"   --xAxisLabel p_{T}(bjets) --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],
          "jetpt"       : [("sr","br"), commonopts+"   --xAxisLabel p_{T}(jets) --isLinear --legendUp -.15 --legendRight -0.08    --legendTaller 0.15 --yTitleOffset -0.1  "],

          # "disc"       : [("br",),   commonopts+"  --isLinear  --xAxisLabel disc  --legendUp .0 --legendRight -0.08    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable "],
          # "disc2"       : [("br",),  commonopts+" --isLinear  --xAxisLabel disc2  --legendUp .0 --legendRight -0.08    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable "],
          # "SRDISC_TOTAL"   : [("",), commonopts+"   --xAxisLabel SR_{disc} --noDivisionLabel --noXaxisUnit --isLinear --noOverflow --legendUp -.03 --legendRight -0.05    --legendTaller 0.05 --yTitleOffset -0.1  --makeTable    --percentageInBox "],
          # "ntops"      : [("sr",), commonopts+"   --xAxisLabel N_{tops} --noXaxisUnit --nDivisions 5 --noDivisionLabel  --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset 0.1  --makeTable "],
          # "ntopness"      : [("sr",), commonopts+"     --xAxisLabel N_{tops}ness --isLinear --legendUp -.15 --legendRight -0.08   --legendTaller 0.15 --yTitleOffset -0.0  "],

          }

    do_stats = False
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
            h_tttt.Scale(5.0)

            do_unblind = True

            if do_stats and key == "SRCR_TOTAL":
            # if key == "SRCR_TOTAL":
                make_scan(cards_dir, do_blind=not do_unblind)
                os.system("cp scan.pdf plots/scan.pdf")

            if do_stats and key in ["SRCR_TOTAL"]:
                regions="srcr"
                if "DISC" in key: regions="srdisc"
                d_lims = get_lims(card=cards_dir, regions=regions, redocard=True, redolimits=True, domcfakes=False)
                exp, expp1, expm1 = d_lims["exp"], d_lims["sp1"]-d_lims["exp"], d_lims["exp"]-d_lims["sm1"]
                subtitle = "#sigma^{UL}_{exp} = %.2f^{+%.1f}_{-%.1f} fb" % (exp, expp1, expm1)

            # do_unblind = typ in ["ttwcr","ttzcr", "sr"]
            do_blind = not do_unblind
            if do_unblind:
                d_newopts["noTextBetweenPads"] = True
                dataMCplot(h_data, bgs=bgs, sigs=[h_tttt], sigtitles=["t#bar{t}t#bar{t} x 5"], systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)

            if key in ["SR_TOTAL","SRCR_TOTAL"]:
                new_d_newopts = d_newopts.copy()
                new_h_tttt = h_tttt.Clone("new_tttt")
                new_h_tttt.Scale(0.1)
                new_bgs = bgs+[new_h_tttt]
                new_colors = colors+[r.kPink-1]
                new_systs = systs+[0.0]
                new_titles = titles+["t#bar{t}t#bar{t}"]
                new_d_newopts["poissonErrorsNoZeros"] = False
                new_d_newopts["noTextBetweenPads"] = False
                new_d_newopts["preserveBackgroundOrder"] = True

            if key == "SR_TOTAL":
                new_d_newopts["outputName"] = d_newopts["outputName"].replace(".pdf","_blindstack.pdf")
                dataMCplot(h_data_empty, bgs=new_bgs, systs=new_systs, titles=new_titles, title="Prefit", subtitle=subtitle, colors=new_colors, opts=new_d_newopts, opts_str=opts_str)

            # if do_stats and key == "SRCR_TOTAL":
            #     d_postfit = get_postfit_dict("mlfitname.root")
            #     new_d_newopts["outputName"] = d_newopts["outputName"].replace(".pdf","_blindstackpostfit.pdf")
            #     postfit_bgs = [d_postfit[bg.GetName().rsplit("_",1)[-1]] for bg in new_bgs]
            #     h_totalbgsyst = d_postfit["total_background"]
            #     dataMCplot(h_data_empty, bgs=postfit_bgs, titles=new_titles, title="Postfit", subtitle=subtitle, colors=new_colors, opts=new_d_newopts, opts_str=opts_str, total_syst=h_totalbgsyst)

            if do_blind:
                d_newopts["outputName"] = d_newopts["outputName"].replace(".pdf","_blind.pdf")
                d_newopts["poissonErrorsNoZeros"] = False
                d_newopts["noTextBetweenPads"] = False
                # For SRCR, "blind" is actually partially blind (first two bins -- CRZ,CRW -- are unblinded)
                # make data with only CR unblinded (first two bins)
                h_data_cronly = h_data.Clone("cronly")
                for i in range(1,h_data.GetNbinsX()+1):
                    if i in [1,2]: h_data_cronly.SetBinContent(i, h_data.GetBinContent(i))
                    else: h_data_cronly.SetBinContent(i, 0)
                if key == "SRCR_TOTAL":
                    dataMCplot(h_data_cronly, bgs=bgs, sigs=[h_tttt], sigtitles=["t#bar{t}t#bar{t} x 5"], systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)
                else:
                    dataMCplot(h_data_empty, bgs=bgs, sigs=[h_tttt], sigtitles=["t#bar{t}t#bar{t} x 5"], systs=systs, titles=titles, title=title, subtitle=subtitle, colors=colors, opts=d_newopts, opts_str=opts_str)


# os.system("niceplots plots plots_tttt_Jun30_secret")
os.system("niceplots plots plots_tttt_Jul20_unblind")
