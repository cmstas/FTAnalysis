#!/usr/bin/env sh


# basedir=v3.26_feb22_ssallsigs_v1
# basedir=v3.26_feb27_allsigs_v1
# basedir=v3.27_mar4_t5tttt_v1/
# basedir="v3.28_ss_mar9small_v1/"
# basedir="v3.28_ss_mar9_allsignals_v1/"
# basedir="v3.28_ss_mar12allsigs_v1/"
basedir="v3.28_ss_mar13allsigs_v1/"

# procs=$(ls $basedir/fs_*histos*srhh*.root | head -n 10 | rev | cut -d '/' -f1 | rev | awk -F "_histos_" '{print $1}')
procs=$(ls $basedir/{fs,rpv}_*histos*srhh*.root | rev | cut -d '/' -f1 | rev | awk -F "_histos_" '{print $1}' | sort | uniq)
# echo $procs

for proc in $procs; do
    # echo "timeout 25m python runSSLimits.py ${basedir} -s ${proc} >& /dev/null"
    echo "python runSSLimits.py ${basedir} -s ${proc} --redolimits --cardonly --workspace >& /dev/null"
done

echo "# Run:"
echo "# ./parallel --jobs 30 --bar < commands.txt"
