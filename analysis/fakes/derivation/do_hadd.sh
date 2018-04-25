#!/usr/bin/env bash

tag="v6"
outfolder=zpeaks/${tag}
basedirinputs=/hadoop/cms/store/user/namin/fakerate/2017/ProjectMetis/
mkdir -p $outfolder
for dataset in SingleElectron DoubleMuon DYJets WJets; do
    echo $dataset
    hadd -k -f ${outfolder}/$dataset.root ${basedirinputs}/FakeRate_${dataset}*HIST_${tag}/*.root
done

