#!/usr/bin/env bash

final_dir="inputs"
files="
package.tar.gz 
main.exe 
liblooperBatch.so 
CORE 
LinkDef_out_rdict.pcm 
btagsf
goodRunList
"
mkdir -p $final_dir
tar -czf $files
cp condor_executable.sh $final_dir
mv package.tar.gz  $final_dir
echo "I put stuff into $final_dir!"
