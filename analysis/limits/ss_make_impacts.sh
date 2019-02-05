#!/usr/bin/env bash

basedir=v3.23_testss_v1
model=fs_t1tttt_m1600_m100

region="all"

# year="2016"
# blindstr=" --unblinded "
# expectflags=""

year="run2"
blindstr=""
expectflags="-t -1 --expectSignal=1"

cardname=${basedir}/card_${model}_${region}_${year}.txt
cardroot=${cardname/txt/root}

# region=srhh
# ./ss_printPrefitTable.py ${basedir}/card_${model}_${region}_${year}.txt ${blindstr}

# echo " " > table_dump.txt
# for region in srhh srhl srll srml srlm ; do
#     echo $region >> table_dump.txt
#     ./ss_printPrefitTable.py  ${blindstr} >> table_dump.txt
#     echo "" >> table_dump.txt
# done

text2workspace.py ${cardname}
# do initial fit (~20 secs)
combineTool.py -d ${cardroot} -M Impacts --robustFit 1 ${expectflags} -m 125 --doInitialFit
# do 15 parallel fits for all ~200 nuisances (~2 min)
combineTool.py -d ${cardroot} -M Impacts --robustFit 1 ${expectflags} -m 125 --doFits --parallel 15
# there will be ~200 root files here, but we'll clean them up later
combineTool.py -M Impacts -d ${cardroot} -m 125 -o impacts.json
# make plots in `impacts.pdf`
plotImpacts.py -i impacts.json -o impacts
# clean up
rm *_paramFit_*
