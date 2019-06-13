#!/usr/bin/env bash

basedir=v3.27_ft_mar5_v1/
model=tttt

# year="2017"
# blindstr=" --unblinded "
# year="2018"
year="run2"
blindstr="  "

# region=srhh
# ./ss_printPrefitTable.py ${basedir}/card_${model}_${region}_${year}.txt ${blindstr}

echo " " > table_dump.txt
for region in srcr srdisc ; do
    echo $region >> table_dump.txt
    ../printPrefitTable.py ${basedir}/card_${model}_${region}_${year}.txt --regions ${region} ${blindstr} >> table_dump.txt
    echo "" >> table_dump.txt
done
