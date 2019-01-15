import os
import sys
import pickle

import runLimits
import makeCombinedCard

if __name__ == "__main__":

    data = []
    basedir = "v3.08_allyears_tmp/"
    for which in ["a","h"]:
        for mass in [350,370,390,410,430,450,470,490,510,530,550]:
        # for mass in [350,370]:
            # proc = "higgsh510"
            proc = "higgs{}{}".format(which,mass)
            for year in [2016,2017,2018]:
                d_lims = runLimits.get_lims(
                        card="{}/card_tttt_srcr_{}.txt".format(basedir,year),
                        sig = proc,
                        redocard=True,
                        dolimits=False,
                        doscan=False,
                        year=year,
                        )
                # runLimits.print_lims(d_lims)

            makeCombinedCard.do_combine(
                    basedir,
                    extra_procs = [proc],
                    )

            d_lims = runLimits.get_lims(
                    card="{}/combined_card.txt".format(basedir),
                    sig = proc,
                    redocard=False,
                    doscan=False,
                    redolimits=True,
                    dosignificance=False,
                    )
            d_lims["which"] = which
            d_lims["mass"] = mass
            data.append(d_lims)

    pickle.dump(data,open("higgs_scan_data.pkl","w"))
    print "Dumped pickle higgs_scan_data.pkl"






        # ./runLimits.py v3.08_allyears_tmp/card_tttt_srcr_2016.txt --year 2016 --noscan --sig higgsh350 --redocard --nolimits
