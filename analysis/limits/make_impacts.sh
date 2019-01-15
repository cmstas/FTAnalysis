#!/usr/bin/env bash

[ -z "$1" ] && {
    echo "Need a base directory as the first argument!"
    exit
}
[ ! -d "$1" ] && {
    echo "Need a *valid* base directory as the first argument!"
    exit
}
basedir="$1"
extra="${@:2}"
region="srcr"
expectflags="-t -1 --expectSignal=1";
if [[ ${extra} = *"srdisc"* ]]; then
    region="srdisc";
fi
if [[ ${extra} = *"unblinded"* ]]; then
    expectflags=""
fi
echo ">>> base directory: ${basedir}"
echo ">>> extra options: ${extra}"
echo ">>> region: ${region}"

cardname=${basedir}/card_tttt_${region}_run2.txt
cardroot=${cardname/txt/root}

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
