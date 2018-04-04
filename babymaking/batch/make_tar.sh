#!/usr/bin/env bash

final_dir="inputs"
files="
package.tar.gz 
main.exe 
liblooperBatch.so 
libyaml-cpp.so.0.5 
CORE 
LinkDef_out_rdict.pcm 
goodRunList
config.yaml
"
mkdir -p $final_dir
cp -H ../../common/yaml-cpp/build/libyaml-cpp.so.0.5 .
tar -chzf $files
rm libyaml-cpp.so.0.5
mv package.tar.gz  $final_dir
echo "I put stuff into $final_dir!"
