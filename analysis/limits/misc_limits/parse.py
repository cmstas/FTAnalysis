import ROOT as r


def parse_log(fname):
    d = {}
    with open(fname,"r") as fhin:
        for line in fhin:
            if line.startswith("Observed"): d["obs"] = float(line.split("<")[-1])
            elif line.startswith("Expected") and "<" in line: d["exp_"+line.split("%")[0].replace("Expected","").strip()] = float(line.split("<")[-1])
            elif line.startswith("Significance"): d["sigma"] = float(line.split(":")[-1])

    obs = d.get("obs",-1)
    exp = d.get("exp_50.0",-1)
    exp_sm1 = d.get("exp_16.0",-1)
    exp_sp1 = d.get("exp_84.0",-1)
    exp_sm2 = d.get("exp_2.5",-1)
    exp_sp2 = d.get("exp_97.5",-1)
    sigma = d.get("sigma",-1)
    # FIXME unblind later
    # obs = exp
    return {
            "sigma":sigma,
            "obs":obs, "exp":exp,
            "sp1":exp_sp1,"sm1":exp_sm1,
            "sp2":exp_sp2,"sm2":exp_sm2,
            }


# def get_dict(fname,typ="shapes_prefit", nbins=10, _tostore=[], channel="SS"):
#     f1 = r.TFile(fname)
#     if not f1: 
#         print "[!] Error, couldn't find file {0}".format(fname)
#         raise Exception

#     f1.ls()

#     d = {}
#     # typ = "shapes_fit_s"
#     g_data = f1.Get("{0}/{1}/data".format(typ,channel))
#     tkeys = f1.Get("{0}/{1}".format(typ,channel)).GetListOfKeys()
#     for tkey in tkeys:
#         key = tkey.GetName()
#         tmpobj = f1.Get("{0}/{1}/{2}".format(typ,channel,key))
#         # clone so that we're not tied to the TFile, as it gets closed
#         obj = tmpobj.Clone(key)
#         # For some reason, need to explicitly make new histograms to keep them detached from the TFile
#         if tkey.ReadObj().InheritsFrom(r.TH1F.Class()):
#             reduce_bins(obj, obj.GetNbinsX())
#         if tkey.ReadObj().InheritsFrom(r.TH2F.Class()):
#             reduce_bins_2d(obj)

#         if key == "data":
#             h_data = r.TH1F("h_data", "h_data", obj.GetN(), 0.5, obj.GetN()+0.5);
#             for ipt in range(0,obj.GetN()):
#                 x = r.Double()
#                 y = r.Double()
#                 g_data.GetPoint(ipt, x, y);
#                 h_data.SetBinContent(ipt+1, y);
#             d[key] = h_data
#         else:
#             d[key] = obj
#         _tostore.append(d[key])
#     f1.Close()
#     return d
# # shapes_prefit

import math
# from https://github.com/cmstas/MT2Analysis/blob/master/scripts/makeSRYieldsTable.py#L119-L131
def GetRoundedValues(rate, unc, unc_sig_fig=2, prec_cap=-2):
    if unc == 0:
        prec = 0
    else:
        prec = int(math.floor(math.log10(unc))) - (unc_sig_fig - 1)
    prec = max(prec, prec_cap)
    ndec = max(0,-prec)
    rnd = lambda x: "{{0:.{0}f}}".format(ndec).format(int(round(float(x)/10**prec)) * 10**prec)
    return rnd(rate), rnd(unc)

import uproot

# for ibin in range(1,19+1):
for ibin in range(1,17+1):
    fit_fname = "root_files/fitDiagnostics_srincl_bin{}.root".format(ibin)
    log_fname = "temp/cards_srincl_bin{}/card_fs_t1tttt_m1800_m1000_srincl_run2_lim.log".format(ibin)
    # print get_dict(fname)
    f = uproot.open(fit_fname)
    # print dir(f["shapes_prefit"]["total_background"])
    val = f["shapes_prefit"]["total_background"].values[0]
    err = min(f["shapes_prefit"]["total_background"]._fSumw2[1],val)
    data = f["shapes_prefit"]["total_data"].yvalues[0]
    robs = parse_log(log_fname)["obs"]

    val, err = GetRoundedValues(val,err)
    # print "{} | {} +- {} | {:.0f} | {:.2f}".format(ibin, val,err,data,robs)
    print r"{} & {} $\pm$ {} & {:.0f} & {:.2f}".format(ibin, val,err,data,robs)

#     def get_val(h):
#         return round(h.values[0],3),round(h._fSumw2[1],3)
#     ys = dict([(k,get_val(v)) for k,v in f["shapes_prefit"]["SS"].items() if ("data" not in k and "total" not in k)])
#     if err/val > 0.7:
#         print ys
