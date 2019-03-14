#!/bin/bash

OUTPUTDIR=$1
TAG=$2
IFILE=$3

export SCRAM_ARCH=slc6_amd64_gcc630
export CMSSWVERSION=CMSSW_9_4_9

function getjobad {
    grep -i "^$1" "$_CONDOR_JOB_AD" | cut -d= -f2- | xargs echo
}

echo -e "\n--- begin header output ---\n" #                     <----- section division
echo "OUTPUTDIR: $OUTPUTDIR"
echo "TAG: $TAG"
echo "IFILE: $IFILE"

echo "GLIDEIN_CMSSite: $GLIDEIN_CMSSite"
echo "hostname: $(hostname)"
echo "uname -a: $(uname -a)"
echo "time: $(date +%s)"
echo "args: $@"

echo -e "\n--- end header output ---\n" #                       <----- section division

if [ -r "$OSGVO_CMSSW_Path"/cmsset_default.sh ]; then
    echo "sourcing environment: source $OSGVO_CMSSW_Path/cmsset_default.sh"
    source "$OSGVO_CMSSW_Path"/cmsset_default.sh
elif [ -r "$OSG_APP"/cmssoft/cms/cmsset_default.sh ]; then
    echo "sourcing environment: source $OSG_APP/cmssoft/cms/cmsset_default.sh"
    source "$OSG_APP"/cmssoft/cms/cmsset_default.sh
elif [ -r /cvmfs/cms.cern.ch/cmsset_default.sh ]; then
    echo "sourcing environment: source /cvmfs/cms.cern.ch/cmsset_default.sh"
    source /cvmfs/cms.cern.ch/cmsset_default.sh
else
    echo "ERROR! Couldn't find $OSGVO_CMSSW_Path/cmsset_default.sh or /cvmfs/cms.cern.ch/cmsset_default.sh or $OSG_APP/cmssoft/cms/cmsset_default.sh"
    exit 1
fi

tarfile=package.tar.xz
# cardstar=cards.tar.gz
echo "this is a selective cmssw tar file"
eval `scramv1 project CMSSW $CMSSWVERSION`
cd $CMSSWVERSION
eval `scramv1 runtime -sh`
if [ -e ../${tarfile} ]; then
    mv ../${tarfile} ${tarfile};
    # mv ../${cardstar} ${cardstar};
    tar xf ${tarfile};
    mv ../card_*.root .
    # tar xf ${cardstar};
fi
scram b


echo -e "\n--- begin running ---\n" #                           <----- section division

echo "" > output.txt
for card in card_*.root ; do
    echo $card
    echo -e "\n\n\n######" >> output.txt
    echo "Start working on ${card} upper limit" >> output.txt
    timeout 30m combine -M AsymptoticLimits ${card} >> output.txt
    echo "Start working on ${card} significance" >> output.txt
    timeout 15m combine -M Significance --uncapped 1 --rMin=-3 ${card} >> output.txt
    echo -e "######\n\n\n" >> output.txt
done

echo -e "\n--- end running ---\n" #                             <----- section division

echo -e "\n--- start cat output.txt ---\n" #                             <----- section division
cat output.txt
echo -e "\n--- end cat output.txt ---\n" #                             <----- section division

echo -e "\n--- begin copying output ---\n" #                    <----- section division

echo "time before copy: $(date +%s)"

OUTPUTNAME="output"

COPY_SRC="file://`pwd`/${OUTPUTNAME}.txt"
COPY_DEST="gsiftp://gftp.t2.ucsd.edu${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}.txt"
echo "Running: env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-copy -p -f -t 7200 --verbose --checksum ADLER32 ${COPY_SRC} ${COPY_DEST}"
env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-copy -p -f -t 7200 --verbose --checksum ADLER32 ${COPY_SRC} ${COPY_DEST} 
COPY_STATUS=$?
if [[ $COPY_STATUS != 0 ]]; then
    echo "Removing output file because gfal-copy crashed with code $COPY_STATUS"
    env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-rm --verbose ${COPY_DEST}
    REMOVE_STATUS=$?
    if [[ $REMOVE_STATUS != 0 ]]; then
        echo "Uhh, gfal-copy crashed and then the gfal-rm also crashed with code $REMOVE_STATUS"
        echo "You probably have a corrupt file sitting on hadoop now."
        exit 1
    fi
fi

echo -e "\n--- end copying output ---\n" #                      <----- section division

echo "time at end: $(date +%s)"

