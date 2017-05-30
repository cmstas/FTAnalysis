# source me

# tag="v0.04"
# basedir="/nfs-7/userdata/namin/tupler_babies/merged/FT/${tag}/output/"

# tag="v9.11"
# basedir="/nfs-7/userdata/namin/tupler_babies/merged/SS/${tag}/output/"

# tag="v0.07"
# basedir="/nfs-7/userdata/namin/tupler_babies/merged//FT/${tag}/output/"

tag="v0.08"
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
