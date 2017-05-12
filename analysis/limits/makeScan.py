#!/usr/bin/env python

import argparse
import createCard
import os


# # thedir = "v0.04_May10_test"
# thedir = "v0.04_May10_nottrw/"
# cardname =  "card_tttt_asimov.txt"
# # createCard.writeOneCard(thedir,cardname, do_expected_data=True)

def main(thedir):
    cardname =  "card_tttt_asimov.txt"
    # thedir, cardname = fullcardname.rsplit("/",1)
    createCard.writeOneCard(thedir,cardname, do_expected_data=True)
    os.system("combine  -M MultiDimFit --algo grid --setPhysicsModelParameterRanges r=0,8.5 --redefineSignalPOI r {0} ; python plot1DScan.py  higgsCombineTest.MultiDimFit.mH120.root --main-label Expected ; cp scan.pdf {0}/scan.pdf".format(thedir+"/"+cardname))
    os.system("ic scan.pdf")
    print "made {0}/scan.pdf".format(thedir)

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("dir", help="directory")
    args = parser.parse_args()

    main(args.dir)
