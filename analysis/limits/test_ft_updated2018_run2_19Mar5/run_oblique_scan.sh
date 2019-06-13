#!/usr/bin/env sh

# Make sure to run first with hhat stuff
# python make_cards.py


# then run this as ./run_oblique_scan.sh, which will automatically run gnu parallel since it's so fast
# so no need to do this: ./run_oblique_scan.sh | ../parallel --jobs 10 --bar
# and the observed rvalues will be catted out

basedir="v3.28_ft_test_Apr20_oblique_v1//"

# card="card_tttt_srdisc_run2_yukawa.txt"

# make a new card and add a line at the bottom like
# tthscale rateParam * tth 1.0 [1.0,1.0]
# cp ${basedir}/card_tttt_srdisc_run2{,_yukawa}.txt
# echo "tthscale rateParam * tth 1.0 [1.0,1.0]" >> ${basedir}/card_tttt_srdisc_run2_yukawa.txt

echo "" > commands_oblique.txt
for card in `ls -1 ${basedir}/card_hhat*_srdisc_run2.txt`; do
    echo "tthscale rateParam * tth 1.0 [1.0,1.0]" >> $card
    log=${card%.txt}.log
    hhatstr=$(echo $card | sed 's/.*hhat\(0p[0-9]*\).*/\1/g' | sed 's/p/\./')
    # scale = yt^2 = (1-hhat)^2
    scale=$(echo "(1-$hhatstr)^2" | bc -l)
    echo "combine -M AsymptoticLimits $card --setParameters tthscale=${scale} --setParameterRanges tthscale=${scale},${scale} >& $log" | tee -a commands_oblique.txt
done
../parallel --jobs 5 --bar < commands_oblique.txt

grep "Observed Limit" ${basedir}/*.log
