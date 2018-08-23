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
              extra = " // {:.1f}%".format(100.0*val/ratio.GetBinContent(xbin,ybin))
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

    f = r.TFile("outputs//histos_both.root")
    ratio = f.Get("ratio")
    predscale = 1.51
    print_flip_rate(ratio, do_errors=False, scale=predscale)
    print_flip_rate(ratio, do_errors=True, scale=predscale)
