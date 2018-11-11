from tqdm import tqdm
import glob
import os
import sys
import ROOT as r

def get_names(basedir):
    fnames = glob.glob("{}/*/*.root".format(basedir))
    return [fname.split("output/")[-1] for fname in fnames]

tags = [
# "v3.05_all",
# "v3.06_all",
# "v3.07_all",
"v3.08_all", # Good
"v3.09_all",
]

d_names = {}
d_basedirs = {}

for tag in tags:
    basedir = "/nfs-7/userdata/namin/tupler_babies/merged/FT/{}/output/".format(tag)
    print basedir
    d_basedirs[tag] = basedir
    d_names[tag] = get_names(basedir)

# print d_names

all_names = sorted(list(reduce(lambda x,y: x|y, map(set,d_names.values()))))

def get_metric(fname):
    if not fname: return 0
    try:
        f1 = r.TFile(fname)
        # t = f1.Get("t")
        # return t.GetEntries()
        h = f1.Get("counts")
        return int(h.GetEntries())
    except:
        return -1
    # return "x"

from analysis.utils.pytable import Table

tab = Table()
# tab.set_theme_basic()
# tab.set_theme_latex()
tab.set_column_names(["name"]+tags+["difference"])
for name in tqdm(all_names):
    # if "Data" not in name: continue
    # if "2017" not in name: continue
    metrics = [get_metric("{}/{}".format(d_basedirs[tag],name) if name in d_names[tag] else None) for tag in tags]
    nicename = name.replace(".root","").replace("year_","")
    difference = metrics[1] - metrics[0]
    color = "green"
    dstr = str(difference)
    if abs(difference)>0: # and "Data" in name:
        color = "red"
        dstr = "{} ({:.1f}%)".format(difference, 0 if metrics[0]==0 else 100.0*difference/metrics[0])
    if difference>0:
        color = "green"
    tab.add_row([nicename]+metrics+[dstr], color=color)
    # tab.add_row(row,color=color)
    # if row[0] == "Pam":
    #     tab.add_line()

tab.print_table(show_row_separators=False,show_alternating=False)