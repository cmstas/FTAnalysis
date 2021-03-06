#!/usr/bin/env bash

# basedir=v3.05_allyears_crwsplit_v1
# basedir=v3.05_allyears_v1
# basedir=v3.05_allyears_v2
# basedir=v3.05_allyears_ttwttz1p3_v2
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
if [[ ${extra} = *"srdisc"* ]]; then
    region="srdisc";
fi
echo ">>> base directory: ${basedir}"
echo ">>> extra options: ${extra}"
echo ">>> region: ${region}"
# extra=" --unblinded "

for y in 2016 2017 2018 ; do
    # ./runLimits.py ${basedir}/card_tttt_${region}_${y}.txt --year ${y} --redocard --noscan ${extra} &
    ./runLimits.py ${basedir}/card_tttt_${region}_${y}.txt --year ${y} --redocard --noscan ${extra} --nolimits &
done
wait

# if [[ ${extra} = *"--single"* ]]; then
    echo "Removing *${region}_run2.root files from the directory so they can get remade"
    rm ${basedir}/*${region}_run2.root
    ./makeSingleCombinedCard.py ${basedir} ${extra} --cardname card_tttt_${region}_run2.txt
    # NOTE make sure there's no --redocard, because we just made it
    # ./runLimits.py ${basedir}/combined_card.txt --redolimits ${extra}
    # ./runLimits.py ${basedir}/card_tttt_${region}_run2.txt --redolimits --noscan ${extra}
    ./runLimits.py ${basedir}/card_tttt_${region}_run2.txt --redolimits ${extra}
# else
#     ./makeCombinedCard.py ${basedir} ${extra} --cardname card_tttt_${region}_run2.txt
#     # NOTE make sure there's no --redocard, because we just made it
#     # ./runLimits.py ${basedir}/combined_card.txt --redolimits ${extra}
#     # ./runLimits.py ${basedir}/card_tttt_${region}_run2.txt --redolimits --noscan ${extra}
#     ./runLimits.py ${basedir}/card_tttt_${region}_run2.txt --redolimits ${extra}
# fi
