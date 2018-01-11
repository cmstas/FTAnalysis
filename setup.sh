therelease=CMSSW_7_4_7_patch1
combineversion=74x-root6
# export SCRAM_ARCH=slc6_amd64_gcc530

if [ ! -d "common/$therelease" ]; then
    (cd common/
     cmsrel "$therelease")
    mkdir "common/$therelease/src/HiggsAnalysis/"
    combine_path=
    git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git "common/$therelease/src/HiggsAnalysis/CombinedLimit/"
    (cd "common/$therelease/src/HiggsAnalysis/CombinedLimit/"
     git checkout $combineversion)
fi

cd "common/$therelease/src"
cmsenv
cd ../../..

export LD_LIBRARY_PATH=$PWD/babymaking/batch:$LD_LIBRARY_PATH
export PYTHONPATH=$PWD:$PYTHONPATH

if [ "$1" = "build" ]; then
    (cd "common/$therelease/src" ; scram b -j 15)
    make -j15 -C common/CORE
fi
