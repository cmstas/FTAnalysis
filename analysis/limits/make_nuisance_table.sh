#!/usr/bin/env bash

# run makeScan.py to do the max likelihood fit for the input to this

mkdir -p plots

python diffNuisances.py -a -g nuisance_output.root -f latex mlfitname.root > nuisance_output.tex
