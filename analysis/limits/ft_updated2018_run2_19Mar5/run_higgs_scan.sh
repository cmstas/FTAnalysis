#!/usr/bin/env sh

# Make sure to run first with higgs stuff
# python make_cards.py


# then pipe this into GNU parallel
# ./run_higgs_scan.sh | ../parallel --jobs 10 --bar

# basedir=v3.28_ft_mar6higgs_v1/
# basedir="v3.28_ft_mar13ttwbbhiggs_v1/"
# basedir="v3.28_ft_mar18higgs_v1//"
basedir="v3.28_ft_mar18higgs_v2//"
for card in `ls -1 ${basedir}/card_higgs*_srdisc_run2.txt`; do
    log=${card%.txt}.log
    echo "combine -M AsymptoticLimits $card >& $log"
done

