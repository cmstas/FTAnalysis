#!/usr/bin/env zsh

# events_max=0 -> all events
events_max=1000000
# events_max=100
# events_max=0

output_path="old_output/"

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
                            output_path=$output_path events_max=$events_max datasetIdx=0 root -b -q -l doAll.C"

    tmux split-window -h  "source /code/osgcode/cmssoft/cms/cmsset_default.sh && \
                           cd ~/FTAnalysis && \
                           source setup.sh && \
                           cd studies/tau/ && clear && \
                           output_path=$output_path events_max=$events_max datasetIdx=1 root -b -q -l doAll.C"

    tmux split-window -h  "source /code/osgcode/cmssoft/cms/cmsset_default.sh && \
                           cd ~/FTAnalysis && \
                           source setup.sh && \
                           cd studies/tau/ && clear && \
                           output_path=$output_path events_max=$events_max datasetIdx=2 root -b -q -l doAll.C"

    tmux split-window -h  "source /code/osgcode/cmssoft/cms/cmsset_default.sh && \
                           cd ~/FTAnalysis && \
                           source setup.sh && \
                           cd studies/tau/ && clear && \
                           output_path=$output_path events_max=$events_max datasetIdx=3 root -b -q -l doAll.C"

    tmux select-layout even-vertical
fi
