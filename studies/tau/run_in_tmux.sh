#!/usr/bin/env zsh


echo "Compiling code"
source /code/osgcode/cmssoft/cms/cmsset_default.sh
cd ~/FTAnalysis && source setup.sh && cd studies/tau/
datasetIdx="-1" root -b -q -l doAll.C  # just compliles the code

if [ "$?" -eq 0 ]; then
    echo "Code compiled, running in new window w/ events_max=$events_max"
    tmux new-window -n  'Run' \
                           "source /code/osgcode/cmssoft/cms/cmsset_default.sh && \
                            cd ~/FTAnalysis && \
                            source setup.sh && \
                            cd studies/tau/ && clear && \
                            dataset=tttt root -b -q -l doAll.C"

    tmux split-window -h  "source /code/osgcode/cmssoft/cms/cmsset_default.sh && \
                           cd ~/FTAnalysis && \
                           source setup.sh && \
                           cd studies/tau/ && clear && \
                           dataset=ttw root -b -q -l doAll.C"

    tmux split-window -h  "source /code/osgcode/cmssoft/cms/cmsset_default.sh && \
                           cd ~/FTAnalysis && \
                           source setup.sh && \
                           cd studies/tau/ && clear && \
                           dataset=ttz root -b -q -l doAll.C"

    tmux split-window -h  "source /code/osgcode/cmssoft/cms/cmsset_default.sh && \
                           cd ~/FTAnalysis && \
                           source setup.sh && \
                           cd studies/tau/ && clear && \
                           dataset=tth root -b -q -l doAll.C"

    tmux select-layout even-vertical
fi
