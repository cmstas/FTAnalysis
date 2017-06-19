#!/usr/bin/env python

import argparse
import createCard
import os


# # thedir = "v0.04_May10_test"
# thedir = "v0.04_May10_nottrw/"
# cardname =  "card_tttt_asimov.txt"
# # createCard.writeOneCard(thedir,cardname, do_expected_data=True)

def make_scan(thedir, noredocard=False):
    cardname =  "card_tttt_asimov.txt"
    # thedir, cardname = fullcardname.rsplit("/",1)
    if not noredocard:
        createCard.writeOneCard(thedir,cardname, do_expected_data=True)
    # os.system("combine  -M MultiDimFit --algo grid --setPhysicsModelParameterRanges r=0,8.5 --redefineSignalPOI r {0} ; python plot1DScan.py  higgsCombineTest.MultiDimFit.mH120.root --main-label Expected ".format(thedir+"/"+cardname))
    fullcard = thedir+"/"+cardname
    cmd = "combine  -M MultiDimFit --algo grid --setPhysicsModelParameterRanges r=0,8.5 --saveFitResult --redefineSignalPOI r {0} ; python ../limits/plot1DScan.py  higgsCombineTest.MultiDimFit.mH120.root --main-label Expected ".format(fullcard)
    print "[!] Running: {0}".format(cmd)
    os.system(cmd)
    print "made {0}/scan.pdf".format(thedir)

    cmd = "combine -M MaxLikelihoodFit {0} --saveShapes --saveWithUncertainties -n name".format(fullcard)
    print "[!] Running: {0}".format(cmd)
    os.system(cmd)
    # os.system("ic scan.pdf")

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("dir", help="directory")
    parser.add_argument("-N", "--noredocard", help="*don't* remake the card", action="store_true")
    args = parser.parse_args()

    make_scan(args.dir, args.noredocard)
