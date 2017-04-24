export LD_LIBRARY_PATH=$PWD/babymaking/batch/:$LD_LIBRARY_PATH

[[ -d babymaking/batch/NtupleTools/ ]] || git clone git@github.com/cmstas/NtupleTools babymaking/batch/NtupleTools/
[[ -d common/Software/ ]] || git clone git@github.com/cmstas/Software common/Software/
[[ -d common/CORE/ ]] || git clone git@github.com/cmstas/CORE common/CORE/
