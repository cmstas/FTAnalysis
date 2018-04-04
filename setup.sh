therelease=CMSSW_7_4_7_patch1
combineversion=74x-root6
# export SCRAM_ARCH=slc6_amd64_gcc530

if [ ! -d "common/$therelease" ]; then
    (cd common/
     cmsrel "$therelease")
    mkdir "common/$therelease/src/HiggsAnalysis/"
    git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git "common/$therelease/src/HiggsAnalysis/CombinedLimit/"
    (cd "common/$therelease/src/HiggsAnalysis/CombinedLimit/"
     git checkout $combineversion)
fi

cd "common/$therelease/src"
cmsenv
cd ../../..

if [ "$1" = "build" ]; then
    echo "Building CMSSW Release"
    (cd "common/$therelease/src" ; scram b -j 15)

    echo "Building yaml-cpp"
    (mkdir -p common/yaml-cpp/build
     cd common/yaml-cpp/build
     cmake .. -DBUILD_SHARED_LIBS=ON
     make -j15)

    echo "Building CORE"
    make -j15 -C common/CORE
fi

export ROOT_INCLUDE_PATH="$PWD/common/yaml-cpp/include:$ROOT_INCLUDE_PATH"

export LD_LIBRARY_PATH="$PWD/babymaking/batch:$PWD/common/yaml-cpp/build/:$LD_LIBRARY_PATH"
export PYTHONPATH="$PWD:$PYTHONPATH"
