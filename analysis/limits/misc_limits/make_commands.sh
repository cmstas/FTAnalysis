#!/usr/bin/env sh


# basedir=v3.26_feb22_ssallsigs_v1
# basedir=v3.26_feb27_allsigs_v1
# basedir=v3.27_mar4_t5tttt_v1/
# basedir="v3.28_ss_mar9small_v1/"
# basedir="v3.28_ss_mar9_allsignals_v1/"
# basedir="v3.28_ss_mar12allsigs_v1/"
# basedir="v3.28_ss_mar13allsigs_v1/"
# basedir="v3.31_ss_Jun26_v1/"
basedir="v3.31_ss_test_v0/"

# procs=$(ls $basedir/fs_*histos*srhh*.root | head -n 10 | rev | cut -d '/' -f1 | rev | awk -F "_histos_" '{print $1}')
# echo $procs

proc="fs_t1tttt_m1800_m1000"

echo "python runSSMiscLimits.py ${basedir} -s ${proc} --redolimits --cardonly --workspace >& /dev/null"

echo "# Run the first one by hand (because of a race condition), then you can do"
echo "# ./parallel --jobs 30 --bar < commands.txt"
