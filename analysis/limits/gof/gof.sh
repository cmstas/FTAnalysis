#!/usr/bin/env bash

card=v3.24_unblindfull_v1/card_fs_t1tttt_m1600_m100_all_run2.txt

# cp ../v3.24_unblindfull_v1/*_run2.root v3.24_unblindfull_v1/
# cp ../v3.24_unblindfull_v1/card_fs_t1tttt_m1600_m100_all_run2.txt v3.24_unblindfull_v1/

njobs=25
ntoys=100
# njobs=15
# ntoys=5

# run toys
for seed in $(seq 1 ${njobs}); do

    # bg only
    echo "nice -10 combine -M GoodnessOfFit ${card} --fixedSignalStrength 0 --algo=saturated -t ${ntoys} -s ${seed} >& log_gof_${seed}.txt &"
    nice -10 combine -M GoodnessOfFit ${card} --fixedSignalStrength 0 --algo=saturated -t ${ntoys} -s ${seed} >& log_gof_${seed}.txt &

    # echo "nice -10 combine -M GoodnessOfFit ${card} --algo=saturated -t ${ntoys} -s ${seed} >& log_gof_${seed}.txt &"
    # nice -10 combine -M GoodnessOfFit ${card} --algo=saturated -t ${ntoys} -s ${seed} >& log_gof_${seed}.txt &

done

# bg only
# get observed
combine -M GoodnessOfFit ${card} --fixedSignalStrength 0 --algo=saturated >& log_gof_observed.txt &

# # get observed
# combine -M GoodnessOfFit ${card} --algo=saturated >& log_gof_observed.txt &

# when done, do
# hadd -k -f all_toys.root higgsCombineTest.GoodnessOfFit.mH120.*.root
# rm log_gof_[0-9]*.txt
# rm higgsCombineTest.*.root
# ./plot_gof.py
