#!/usr/bin/env bash

# basedir=v3.05_allyears_crwsplit_v1
# basedir=v3.05_allyears_v1
# basedir=v3.05_allyears_v2
# basedir=v3.05_allyears_ttwttz1p3_v2
[ -z "$1" ] && {
    echo "Need a base directory!"
    exit
}
basedir=$1
# extra=" --unblinded "
for y in 2016 2017 2018 ; do
    ./runLimits.py ${basedir}/card_tttt_srcr_${y}.txt --year ${y} --redocard --noscan ${extra} &
done
wait
./makeCombinedCard.py ${basedir}
./runLimits.py ${basedir}/combined_card.txt --redolimits ${extra}
