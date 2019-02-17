#!/usr/bin/env bash

# run makeScan.py to do the max likelihood fit for the input to this

mkdir -p plots

# basedir=v3.05_allyears_v2
# inpfile=${basedir}/mlfitname.root
inpfile=fitDiagnosticsss.root
outname=nuisance_output.root

python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f html ${inpfile} > nuisances.html
web nuisances.html

# python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex ${inpfile} | grep -v 'with the following' > nuisances.tex
# echo "\\documentclass{article} \\begin{document}\\pagenumbering{gobble} $(cat nuisances.tex) \\end{document}" > nuisances.tex
# pdflatex -interaction=nonstopmode nuisances.tex
# pdfcrop nuisances.pdf
# mv nuisances-crop.pdf nuisances.pdf
# web nuisances.pdf
