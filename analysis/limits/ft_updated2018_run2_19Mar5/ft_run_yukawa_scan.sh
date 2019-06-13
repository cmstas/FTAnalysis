#!/usr/bin/env bash

# basedir=v3.28_ft_mar6unblind_v1/
# basedir="v3.28_ft_mar13ttwbb_v1/"
basedir="v3.28_ft_mar18nominal_v1/"

# make a new card and add a line at the bottom like
# tthscale rateParam * tth 1.0 [1.0,1.0]
cp ${basedir}/card_tttt_srdisc_run2{,_yukawa}.txt
echo "tthscale rateParam * tth 1.0 [1.0,1.0]" >> ${basedir}/card_tttt_srdisc_run2_yukawa.txt

card="card_tttt_srdisc_run2_yukawa.txt"
scales="
0.0
0.1
0.2
0.3
0.4
0.5
0.6
0.7
0.8
0.9
1.0
1.1
1.2
1.3
1.4
1.5
1.6
1.7
1.8
1.9
2.0
2.1
2.2
2.3
2.4
2.5
2.7
3.0
3.5
4.0
5.0
6.0
7.0
8.0
"
> ${basedir}/log_yukawa_scan.txt
for scale in ${scales}; do

    # echo combine -M Significance --expectSignal=1 -t -1 ${basedir}/${card} --setParameters tthscale=${scale} --setParameterRanges tthscale=${scale},${scale}
    # combine -M Significance --expectSignal=1 -t -1 ${basedir}/${card} --setParameters tthscale=${scale} --setParameterRanges tthscale=${scale},${scale} >> ${basedir}/log_yukawa_scan.txt

    echo combine -M AsymptoticLimits ${basedir}/${card} --setParameters tthscale=${scale} --setParameterRanges tthscale=${scale},${scale}
    combine -M AsymptoticLimits ${basedir}/${card} --setParameters tthscale=${scale} --setParameterRanges tthscale=${scale},${scale} >>  ${basedir}/log_yukawa_scan.txt

    echo combine -M FitDiagnostics ${basedir}/${card} --robustFit=1 -n name --setParameters tthscale=${scale} --setParameterRanges tthscale=${scale},${scale}
    combine -M FitDiagnostics ${basedir}/${card} --robustFit=1 -n name --setParameters tthscale=${scale} --setParameterRanges tthscale=${scale},${scale} >>  ${basedir}/log_yukawa_scan.txt

done

# NOTE afterwards, do
# grep -E "(Set Default Value|Expected 50.0|Best fit r)" v3.28_ft_mar6unblind_v1/log_yukawa_scan.txt | xargs -L 2
