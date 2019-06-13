#!/usr/bin/env bash

# # basedir=v3.27_ft_mar5_v1/
# basedir="v3.28_ft_mar18nominal_v1/"
# # basedir=v3.27_ft_mar5jes_v1//
# for region in srcr srdisc ; do
# # for region in srcr ; do
#     for which in exp obs ; do
#     # for which in exp ; do
#         if [[ ${which} = *"obs"* ]]; then
#             expectflags=""
#         else
#             expectflags="-t -1 --expectSignal=1"
#         fi
#         cardname=${basedir}/card_tttt_${region}_run2.txt
#         cardroot=${cardname/txt/root}
#         name=impacts_run2_${region}_${which}
#         text2workspace.py ${cardname}
#         # do initial fit (~20 secs)
#         combineTool.py -d ${cardroot} -M Impacts --robustFit 1 ${expectflags} -m 125 --doInitialFit
#         # do 15 parallel fits for all ~200 nuisances (~2 min)
#         combineTool.py -d ${cardroot} -M Impacts --robustFit 1 ${expectflags} -m 125 --doFits --parallel 15
#         # there will be ~200 root files here, but we'll clean them up later
#         combineTool.py -M Impacts -d ${cardroot} -m 125 -o ${name}.json
#         # make plots in `impacts.pdf`
#         plotImpacts.py -i ${name}.json -o ${name}
#         # clean up
#         rm *_paramFit_*
#     done
# done



basedir="v3.28_ft_mar18correlateall_v1//"
for region in srdisc ; do
    for which in exp ; do
        if [[ ${which} = *"obs"* ]]; then
            expectflags=""
        else
            expectflags="-t -1 --expectSignal=1"
        fi
        cardname=${basedir}/card_tttt_${region}_run2.txt
        # cardname=${basedir}/card_tttt_${region}_run2_nomcstat.txt
        cardroot=${cardname/txt/root}
        name=corrall_impacts_run2_${region}_${which}
        # name=corrall_nomcstat_impacts_run2_${region}_${which}
        text2workspace.py ${cardname}
        combineTool.py -d ${cardroot} -M Impacts --robustFit 1 ${expectflags} -m 125 --doInitialFit
        combineTool.py -d ${cardroot} -M Impacts --robustFit 1 ${expectflags} -m 125 --doFits --parallel 25
        combineTool.py -M Impacts -d ${cardroot} -m 125 -o ${name}.json
        plotImpacts.py -i ${name}.json -o ${name}
        rm *_paramFit_*
    done
done



