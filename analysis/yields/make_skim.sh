# source me, don't do ./make_skim.sh
# will have to reinstate FT environment afterwards :(

# tag="v0.10"
tag="v0.10_fix"
basedir="/nfs-7/userdata/namin/tupler_babies/merged//FT/${tag}/output/"

cut="njets>=2"

mkdir -p ${basedir}/skim

cd /cvmfs/cms.cern.ch/slc6_amd64_gcc493/cms/cmssw/CMSSW_8_0_20/; cmsenv; cd -

for sample in $(ls -1 ${basedir}/*.root); do
    sample=$(basename $sample);
    outname=${basedir}/skim/${sample}
    [ -e $outname ] && continue

    echo skim.py -t t -c "${cut}" "${basedir}/${sample}" -o ${outname}
    skim.py -t t -c "${cut}" "${basedir}/${sample}" -o ${outname} &
done
