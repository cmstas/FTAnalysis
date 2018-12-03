import ROOT as r
from analysis.limits.errors import E
import plottery.plottery as ply

el_trigs = [
        "HLT_Ele17_CaloIdM_TrackIdM_PFJet30",
        "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30",
        "HLT_Ele8_CaloIdM_TrackIdM_PFJet30",
        "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30",
        ]

mu_trigs = [
        "HLT_Mu17_TrkIsoVVL",
        "HLT_Mu17",
        "HLT_Mu8_TrkIsoVVL",
        "HLT_Mu8",
        "HLT_IsoMu27",
        ]

# version = "v8"
# version = "v9"
# version = "v13"
# version = "v22"
version = "v23"
year = 2017
f_dy = r.TFile("zpeaks/{version}/hists_dy_{year}.root".format(version=version,year=year))
f_data_el = r.TFile("zpeaks/{version}/hists_e_{year}.root".format(version=version,year=year))
f_data_mu = r.TFile("zpeaks/{version}/hists_mu_{year}.root".format(version=version,year=year))

print f_dy, f_data_el, f_data_mu

hacky_ZSF = 1
# hacky_ZSF = 41.5/30.4 # scale down DY for 2018 lumi (because DY hist filled with 2017 lumi scaling)

def get_sf(h_dy, h_data):

    mc_err = r.Double()
    data_err = r.Double()
    mc = h_dy.IntegralAndError(h_dy.FindBin(75),h_dy.FindBin(105),mc_err)
    data = h_data.IntegralAndError(h_data.FindBin(75),h_data.FindBin(105),data_err)
    data_entries = h_data.GetEntries()

    # ratio is MC/data since we are scaling data!
    ratio = 1.0*E(mc,mc_err)/E(data,data_err)

    return data, mc, ratio, data_entries

sfs = []  # these are MC/data, so we multiply data by these
for trigname in el_trigs+mu_trigs:
    # h_dy = f_dy.Get("mll_{}".format(trigname))
    h_dy = f_dy.Get("{}".format(trigname))
    h_dy.Scale(1./hacky_ZSF)
    is_el = "_Ele" in trigname
    # if is_el: h_data = f_data_el.Get("mll_{}".format(trigname))
    # else: h_data = f_data_mu.Get("mll_{}".format(trigname))
    if is_el: h_data = f_data_el.Get("{}".format(trigname))
    else: h_data = f_data_mu.Get("{}".format(trigname))
    data, mc, ratio, data_entries = get_sf(h_dy, h_data)
    h_data.Sumw2()

    sfs.append([trigname,ratio[0]])

    h_data.Scale(ratio[0])

    inv_ratio = 1./ratio

    color = r.kAzure-2 if is_el else r.kOrange+1
    ply.plot_hist(
            data=h_data,
            bgs=[h_dy],
            colors = [color],
            legend_labels = ["DY"],
            options = {
                "do_stack": True,
                "legend_scalex": 0.8,
                "legend_scaley": 0.6,
                "extra_text": [
                    trigname,
                    "scaled",
                    "#frac{{data}}{{MC}} = {:.2f}".format(inv_ratio[0]),
                    "unw. data entries: {:.0f}".format(data_entries),
                    ],
                "extra_text_xpos": 0.17,
                "extra_text_ypos": 0.85,
                "legend_alignment": "bottom right",
                "legend_smart": False,
                # "xaxis_range":[40.,140.],
                "ratio_range":[0.5,2.0],
                "output_name": "plots/zsf_{}.pdf".format(trigname),
                "legend_percentageinbox": True,
                "cms_label": "Preliminary",
                "lumi_value": "41.5",
                # "output_ic": True,
                }
            )
    # break

for tn,sf in sfs:
    print "float sf_{} = {:.4f};".format(tn,sf)
