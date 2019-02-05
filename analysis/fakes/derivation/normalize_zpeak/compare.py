from errors import E
import ROOT as r
from pprint import pprint

def get_val_err(h):
    err = r.Double()
    val = h.IntegralAndError(h.FindBin(91.2-15),h.FindBin(91.2+15),err)
    return val, err

def print_sfs(outdir, year=2017, _persist=[], prefix="    "):
    d_hists = {}
    for which in ["data_el","data_mu","dy"]:
        f = r.TFile("{}/histos_{}_{}.root".format(outdir,which,year))
        _persist.append(f)
        for k in [k.GetName() for k in f.GetListOfKeys()]:
            if k not in d_hists:
                d_hists[k] = {}
            d_hists[k][which] = f.Get(k)
    # pprint(d_hists)
    # print map(lambda x: x.Integral(), d_hists.values())
    # h = d_hists["mll_HLT_IsoMu27"]["dy"]
    sfs = {}
    for k,dproc in d_hists.items():
        ve_dy = E(*get_val_err(dproc["dy"]))
        ve_data = E(*get_val_err(dproc["data_el"]))+E(*get_val_err(dproc["data_mu"]))
        if ve_dy[0] < 1e-6: continue
        sf = (ve_dy/ve_data)
        sfs[k.replace("mll_","")] = sf[0]
    for trig,sf in sorted(sfs.items()):
        print "{} sf_{} = {:.4f};".format(prefix,trig,sf)


if __name__ == "__main__":
    print_sfs()
    # print get_val_err(h)
