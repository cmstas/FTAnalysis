#!/usr/bin/env python
import numpy as np
from tqdm import tqdm
import math
import ROOT as r

from variables import cols


d_name_map = {
        "ttw": 0,
        "ttz": 1,
        "tth": 2,
        "fakes_mc": 3,
        "xg": 4,
        "flips": 5,
        "qqww": 6,
        "rares": 6,
        "tttt": 7,
        }

if __name__ == "__main__":

    ch = r.TChain("t")
    ch.Add("output.root")
    # ch.Add("output_1lb.root")


    # cnt = 0



    xdata = []
    ydata = []
    extra = []
    for event in tqdm(ch,total=ch.GetEntries()):

        # if event.SR == 1: continue
        # if event.SR == 2: continue
        # if event.SR <= 0: continue

        extra.append([event.weight,event.SR])
        # print event.name == "ttw"
        ydata.append( d_name_map[str(event.name)] )
        row = []
        for col in cols: row.append(event.__getattr__(col))
        xdata.append(row)

        # break


    np.array(xdata, dtype=np.float32).dump("dump_xdata.npa")
    np.array(ydata, dtype=np.float32).dump("dump_ydata.npa")
    np.array(extra, dtype=np.float32).dump("dump_extra.npa")
