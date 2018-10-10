#!/usr/bin/env bash

# tag="v8"
# tag="v9"
tag="v23"
outfolder=zpeaks/${tag}
basedirinputs=/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/
mkdir -p $outfolder
# for dataset in SingleElectron DoubleMuon DYJets WJets; do
for dataset in EGamma DoubleMuon ; do
    echo $dataset
    # hadd -k -f ${outfolder}/$dataset.root ${basedirinputs}/FakeRate_${dataset}*HIST_${tag}/*.root
    hadd -k -f ${outfolder}/$dataset.root ${basedirinputs}/FakeRate_${dataset}*Run2018*HIST_${tag}/*.root
done

for dataset in DYJets WJets ; do
    echo $dataset
    # hadd -k -f ${outfolder}/$dataset.root ${basedirinputs}/FakeRate_${dataset}*HIST_${tag}/*.root
    hadd -k -f ${outfolder}/$dataset.root ${basedirinputs}/FakeRate_${dataset}*HIST_${tag}/*.root
done

