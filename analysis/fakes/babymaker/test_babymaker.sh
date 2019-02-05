#!/usr/bin/env bash

function get_dataset() {
    which=$1
    sampstr=$2
    python -c 'from samples import '${which}' ; y = [x for x in '${which}' if "'${sampstr}'" in x[0]]; print y[len(y)//2][0]';
}

function get_random_file() {
    datasetname=$1;
    dir=$(dis_client.py -t snt "${datasetname} | grep location" | head -1);
    last=$(ls -1 ${dir}/*.root | sort | shuf --random-source=<(yes) | head -n 1)
    echo $last
}

function get_file() {
    get_random_file $(get_dataset $1 "$2")
}

function check() {
    fname=$1
    badstr="\e[91mx\e[0m"
    goodstr="\e[92mo\e[0m"
    pgood=$([ -n "$(grep "Processed [1-9]" ${fname})" ] && echo $goodstr || echo $badstr);
    sgood=$([ -n "$(grep "Stored [1-9]" ${fname})" ] && echo $goodstr || echo $badstr);
    echo -e "$pgood$sgood $fname"
}

function run() {
    echo Running ./main.exe $(get_file $1 $2) -o $3 -n 10000 '>&' $4
    ./main.exe $(get_file $1 $2) -o $3 -n 5000 >& $4
    check $4
}

outputdir=outputs_test/
mkdir -p ${outputdir}
rm ${outputdir}/log*.txt

run   mc_2016     "/DYJetsToLL"                       ${outputdir}/test_2016_dy.root              ${outputdir}/log_2016_dy.txt              &
run   mc_2017     "/DYJetsToLL"                       ${outputdir}/test_2017_dy.root              ${outputdir}/log_2017_dy.txt              &
run   mc_2017     "MuEnriched"                        ${outputdir}/test_2017_qcdmu.root           ${outputdir}/log_2017_qcdmu.txt           &
run   mc_2017     "bcToE"                             ${outputdir}/test_2017_qcdbctoe.root        ${outputdir}/log_2017_qcdbctoe.txt        &
run   data_2016   "/DoubleMuon/Run2016C"              ${outputdir}/test_2016_datamuc.root         ${outputdir}/log_2016_datamuc.txt         &
run   data_2016   "/DoubleEG/Run2016C"                ${outputdir}/test_2016_dataelc.root         ${outputdir}/log_2016_dataelc.txt         &
run   data_2017   "/DoubleMuon/Run2017C"              ${outputdir}/test_2017_datamuc.root         ${outputdir}/log_2017_datamuc.txt         &
run   data_2017   "/SingleElectron/Run2017C"          ${outputdir}/test_2017_dataelc.root         ${outputdir}/log_2017_dataelc.txt         &
run   mc_2018     "/DYJetsToLL"                       ${outputdir}/test_2018_dy.root              ${outputdir}/log_2018_dy.txt              &
run   data_2018   "/DoubleMuon/Run2018D-PromptReco"   ${outputdir}/test_2018_datamud.root         ${outputdir}/log_2018_datamud.txt         &
run   data_2018   "/EGamma/Run2018B-17Sep2018"        ${outputdir}/test_2018_dataelbrereco.root   ${outputdir}/log_2018_dataelbrereco.txt   &
run   data_2018   "/EGamma/Run2018D-PromptReco"       ${outputdir}/test_2018_dataeldprompt.root   ${outputdir}/log_2018_dataeldprompt.txt   &

wait
