therelease=CMSSW_8_0_26_patch2
export SCRAM_ARCH=slc6_amd64_gcc530
mkdir -p common
if [ ! -d common/$therelease ]; then 
    cd common/ ;
    cmsrel $therelease;
    cd -
fi
cd common/$therelease/src
eval `scram ru -sh`
cd -

export LD_LIBRARY_PATH=$PWD/babymaking/batch/:$LD_LIBRARY_PATH

[[ -d babymaking/batch/NtupleTools/ ]] || git clone git@github.com:cmstas/NtupleTools.git babymaking/batch/NtupleTools/
[[ -d common/Software/ ]] || git clone git@github.com:cmstas/Software.git common/Software/
[[ -d common/CORE/ ]] || {
    git clone git@github.com:cmstas/CORE.git common/CORE/;
    cd common/CORE; make -j10 >&/dev/null &
    cd -;
}
