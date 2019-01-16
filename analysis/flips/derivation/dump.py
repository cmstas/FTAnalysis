# https://stackoverflow.com/questions/15760712/python-readline-module-prints-escape-character-during-import
import sys
if not sys.stdout.isatty():
    import os
    oldTerm = os.environ['TERM'] # remember the original setting
    os.environ['TERM'] = ''
    import readline
    os.environ['TERM'] = oldTerm # restore the orignal TERM setting
    del oldTerm

import ROOT as r

def print_flip_rate(hist, do_errors=False, scale=1.0):
  print "float flipRate%s(float pt, float eta) {" % ("" if not do_errors else "Error")
  print "    float scale = {};".format(scale)
  for xbin in range(1,ratio.GetNbinsX()+1):
      for ybin in range(1,ratio.GetNbinsY()+1):
          val = ratio.GetBinContent(xbin,ybin)
          extra = ""
          if do_errors:
              val = ratio.GetBinError(xbin,ybin)
              extra = " // {:.1f}%".format(100.0*val/max(1e-6,ratio.GetBinContent(xbin,ybin)))
          if xbin != ratio.GetNbinsX():
              print "   if (pt>={} && pt<{} && fabs(eta)>={:.3f} && fabs(eta)<{:.3f} ) return {}*scale;{}".format(
                      ratio.GetXaxis().GetBinLowEdge(xbin),
                      ratio.GetXaxis().GetBinUpEdge(xbin),
                      ratio.GetYaxis().GetBinLowEdge(ybin),
                      ratio.GetYaxis().GetBinUpEdge(ybin),
                      val,
                      extra,
                      )
          else:
              print "   if (pt>={} && fabs(eta)>={:.3f} && fabs(eta)<{:.3f} ) return {}*scale;{}".format(
                      ratio.GetXaxis().GetBinLowEdge(xbin),
                      ratio.GetYaxis().GetBinLowEdge(ybin),
                      ratio.GetYaxis().GetBinUpEdge(ybin),
                      val,
                      extra,
                      )
  print "   return 0.;"
  print "}"

if __name__ == "__main__":

    # f = r.TFile("outputs//histos_2017.root")
    # ratio = f.Get("ratio")
    # predscale = 1.45
    # print_flip_rate(ratio, do_errors=False, scale=predscale)
    # print_flip_rate(ratio, do_errors=True, scale=predscale)

    year = 2018
    f = r.TFile("outputs//histos_{}.root".format(year))
    ratio = f.Get("ratio")
    predscale = dict(
            y2016 = 1.01, # 2016
            y2017 = 1.44, # 2017
            y2018 = 1.41, # 2018
            )["y"+str(year)]
    print_flip_rate(ratio, do_errors=False, scale=predscale)
    print_flip_rate(ratio, do_errors=True, scale=predscale)
