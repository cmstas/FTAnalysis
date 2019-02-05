#!/usr/bin/env bash

# python py_doAll.py --tag v3.23_ss_fastsim_19Jan22 --ss --plots --shapes --fastsim --plot_kwargs '{"show_mcfakes":False}'
# python py_doAll.py --tag v3.23_ss_fastsim_19Jan22 --ss --plots --shapes --fastsim --plot_kwargs '{"show_mcfakes":False}' --excludeproc "fs_*"



basedir=v3.23_testss_v1
model=fs_t1tttt_m1600_m100

# year="2016"
# blindstr=" --unblinded "

year="run2"
blindstr=""

# region=srhh
# ./ss_printPrefitTable.py ${basedir}/card_${model}_${region}_${year}.txt ${blindstr}

echo " " > table_dump.txt
for region in srhh srhl srll srml srlm ; do
    echo $region >> table_dump.txt
    ./ss_printPrefitTable.py ${basedir}/card_${model}_${region}_${year}.txt ${blindstr} >> table_dump.txt
    echo "" >> table_dump.txt
done
