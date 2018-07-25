#!/usr/bin/env bash

echo "Did you do 'make opt' instead of 'make'? It will be much faster"

final_dir="inputs"
files="
package.tar.gz 
main.exe 
liblooperBatch.so 
CORE 
LinkDef_out_rdict.pcm 
goodRunList
"
mkdir -p $final_dir
tar -czf $files
mv package.tar.gz  $final_dir
echo "I put stuff into $final_dir!"
