#!/usr/bin/env zsh

config="config.yaml"

echo "Compiling code"
source /code/osgcode/cmssoft/cms/cmsset_default.sh
cd ~/FTAnalysis && source setup.sh && cd studies/tau/
datasetIdx="-1" root -b -q -l doAll.C  # just compliles the code

if [ "$?" -eq 0 ]; then
    echo "Code compiled, running in new window"
    tmux new-window -n  'Run' \
                           "source /code/osgcode/cmssoft/cms/cmsset_default.sh && \
                            cd ~/FTAnalysis && \
                            source setup.sh && \
                            cd studies/tau/ && clear && \
                            config=$config dataset=tttt root -b -q -l doAll.C"

    tmux split-window -h  "source /code/osgcode/cmssoft/cms/cmsset_default.sh && \
                           cd ~/FTAnalysis && \
                           source setup.sh && \
                           cd studies/tau/ && clear && \
                           config=$config dataset=ttw root -b -q -l doAll.C"

    tmux split-window -h  "source /code/osgcode/cmssoft/cms/cmsset_default.sh && \
                           cd ~/FTAnalysis && \
                           source setup.sh && \
                           cd studies/tau/ && clear && \
                           config=$config dataset=ttz root -b -q -l doAll.C"

    tmux split-window -h  "source /code/osgcode/cmssoft/cms/cmsset_default.sh && \
                           cd ~/FTAnalysis && \
                           source setup.sh && \
                           cd studies/tau/ && clear && \
                           config=$config dataset=tth root -b -q -l doAll.C"

    tmux select-layout even-vertical
fi
