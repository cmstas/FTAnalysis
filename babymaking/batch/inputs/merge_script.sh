#!/bin/bash

eval "$1" # Beautiful trick - http://unix.stackexchange.com/a/185467

echo "Args:"
echo "OUTPUT_NAMES: $OUTPUT_NAMES" # OUTPUT_NAMES=ttW_lnu,skim
echo "BABY_DIR: $BABY_DIR" # /hadoop/cms/store/user/namin/AutoTwopler_babies/Stop_1l_v10/TestCMS3_RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/
echo "OUTPUT_DIR: $OUTPUT_DIR" # /nfs-7/userdata/namin/tupler_babies/merged/
echo "ANALYSIS: $ANALYSIS" # Stop_1l
echo "BABY_TAG: $BABY_TAG" # v10
echo "DATASET: $DATASET" # /TestCMS3/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAOD
echo "SHORTNAME: $SHORTNAME" # Test_ntuple.root
echo "EXTRA_ARGS: $EXTRA_ARGS" # Extra Args in instructions.txt file

# convert comma separated list into array of output files
OUTPUT_NAMES=(`echo $OUTPUT_NAMES | sed s/,/" "/g`)
echo "OUTPUT_NAMES=${OUTPUT_NAMES[*]}"

# same for extra args
EXTRA_ARGS=(`echo $EXTRA_ARGS | sed s/,/" "/g`)
echo "EXTRA_ARGS=${EXTRA_ARGS[*]}"

for output_name in "${OUTPUT_NAMES[@]}"; do

    out_folder=$OUTPUT_DIR/$ANALYSIS/$BABY_TAG/${output_name}
    out_file=${out_folder}/${SHORTNAME}
    in_folder=${BABY_DIR}/${output_name}

    mkdir -p $out_folder

    ### User merge part. 
    ### Don't redirect to log file. That is done outside the scope of this script.

    echo "Will merge $in_folder/*.root into $out_file"
    echo root -l -n -b -q merge_macro.C\(\"${in_folder}\",\"${out_file}\"\)
    root -l -n -b -q merge_macro.C\(\"${in_folder}\",\"${out_file}\"\)

done

exit 0 # good 
# exit 1 # bad (anything other than 0)

