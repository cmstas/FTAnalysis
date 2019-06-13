#!/usr/bin/env sh

# extra=""
extra="--redolimits"
# extra="--redolimits --noscan"
# basedir=v3.27_ft_mar5_v1//
# basedir=v3.28_ft_mar6_v1///
# basedir=v3.28_ft_mar6unblind_v1///
# basedir="v3.28_ft_mar12_v1/"
# basedir="v3.28_ft_mar12checktrig_v1/"
# basedir="v3.28_ft_mar13ttwbb_v1/"
# basedir="v3.28_ft_mar18musyst_v1"

# basedir="v3.28_ft_test_Apr20_v1/"
# basedir="v3.28_ft_May3_94x_v1/"
# basedir="v3.28_ft_test_May13_lumi137_v1/"
# basedir="v3.28_ft_test_May13_lumi300_v1/"
basedir="v3.28_ft_test_May13_lumi400_v1/"

# basedir="v3.28_ft_mar12ttwbb_v1/"
# basedir="v3.28_ft_mar12mcfakeshybrid_v1//"
# basedir=v3.28_ft_mar6oldbdt_v1///
# basedir=v3.27_ft_mar5jes_v1//
python make_cards.py ${basedir}
# ../runLimits.py ${basedir}/card_tttt_srcr_2016.txt -r srcr ${extra}  --unblinded --tagfit y2016_srcr_obs | tee y2016_lims_srcr_obs.txt
# ../runLimits.py ${basedir}/card_tttt_srdisc_2016.txt -r srdisc ${extra}  --unblinded --tagfit y2016_srdisc_obs | tee y2016_lims_srdisc_obs.txt
# ../runLimits.py ${basedir}/card_tttt_srcr_2016.txt -r srcr ${extra}  --tagfit y2016_srcr_exp | tee y2016_lims_srcr_exp.txt
# ../runLimits.py ${basedir}/card_tttt_srdisc_2016.txt -r srdisc ${extra}  --tagfit y2016_srdisc_exp | tee y2016_lims_srdisc_exp.txt
# ../runLimits.py ${basedir}/card_tttt_srcr_2017.txt -r srcr ${extra}  --unblinded --tagfit y2017_srcr_obs | tee y2017_lims_srcr_obs.txt
# ../runLimits.py ${basedir}/card_tttt_srdisc_2017.txt -r srdisc ${extra}  --unblinded --tagfit y2017_srdisc_obs | tee y2017_lims_srdisc_obs.txt
# ../runLimits.py ${basedir}/card_tttt_srcr_2017.txt -r srcr ${extra}  --tagfit y2017_srcr_exp | tee y2017_lims_srcr_exp.txt
# ../runLimits.py ${basedir}/card_tttt_srdisc_2017.txt -r srdisc ${extra}  --tagfit y2017_srdisc_exp | tee y2017_lims_srdisc_exp.txt
# ../runLimits.py ${basedir}/card_tttt_srcr_2018.txt -r srcr ${extra}  --unblinded --tagfit y2018_srcr_obs | tee y2018_lims_srcr_obs.txt
# ../runLimits.py ${basedir}/card_tttt_srdisc_2018.txt -r srdisc ${extra}  --unblinded --tagfit y2018_srdisc_obs | tee y2018_lims_srdisc_obs.txt
# ../runLimits.py ${basedir}/card_tttt_srcr_2018.txt -r srcr ${extra}  --tagfit y2018_srcr_exp | tee y2018_lims_srcr_exp.txt
# ../runLimits.py ${basedir}/card_tttt_srdisc_2018.txt -r srdisc ${extra}  --tagfit y2018_srdisc_exp | tee y2018_lims_srdisc_exp.txt
../runLimits.py ${basedir}/card_tttt_srdisc_run2.txt -r srdisc ${extra}  --tagfit run2_srdisc_exp | tee lims_srdisc_exp.txt
../runLimits.py ${basedir}/card_tttt_srdisc_run2.txt -r srdisc ${extra}  --unblinded --tagfit run2_srdisc_obs | tee lims_srdisc_obs.txt
../runLimits.py ${basedir}/card_tttt_srcr_run2.txt -r srcr ${extra}  --tagfit run2_srcr_exp | tee lims_srcr_exp.txt
../runLimits.py ${basedir}/card_tttt_srcr_run2.txt -r srcr ${extra}  --unblinded --tagfit run2_srcr_obs | tee lims_srcr_obs.txt
outname=nuisance_output.root

# python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex fitDiagnosticsrun2_srcr_exp.root | grep -v 'with the following' > nuisances_run2_srcr_exp.tex
# python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex fitDiagnosticsrun2_srcr_obs.root | grep -v 'with the following' > nuisances_run2_srcr_obs.tex
# python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex fitDiagnosticsrun2_srdisc_exp.root | grep -v 'with the following' > nuisances_run2_srdisc_exp.tex
# python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex fitDiagnosticsrun2_srdisc_obs.root | grep -v 'with the following' > nuisances_run2_srdisc_obs.tex
# # python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex fitDiagnosticsy2018_srcr_exp.root | grep -v 'with the following' > nuisances_y2018_srcr_exp.tex
# # python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex fitDiagnosticsy2018_srdisc_exp.root | grep -v 'with the following' > nuisances_y2018_srdisc_exp.tex
# mkdir -p plots
# python parse_fit.py
# python ../plot1DScan.py ${basedir}/scandatarun2_srdisc_obs.root -l Observed --xsec -e ${basedir}/scandatarun2_srdisc_exp.root

# # extra="--redolimits --noscan"
# extra="--redolimits "
# # basedir="v3.28_ft_mar13ttwbb_v1//"
# basedir="v3.28_ft_mar18bb1p3_v1/"
# python make_cards.py ${basedir}
# ../runLimits.py ${basedir}/card_tttt_srcr_run2.txt -r srcr ${extra}  --tagfit run2_srcr_exp | tee lims_srcr_exp.txt
# ../runLimits.py ${basedir}/card_tttt_srcr_run2.txt -r srcr ${extra}  --unblinded --tagfit run2_srcr_obs | tee lims_srcr_obs.txt
# ../runLimits.py ${basedir}/card_tttt_srdisc_run2.txt -r srdisc ${extra}  --tagfit run2_srdisc_exp | tee lims_srdisc_exp.txt
# ../runLimits.py ${basedir}/card_tttt_srdisc_run2.txt -r srdisc ${extra}  --unblinded --tagfit run2_srdisc_obs | tee lims_srdisc_obs.txt

# # python make_cards.py
# # extra=""
# extra="--redolimits"
# # basedir=v3.27_ft_mar5_v1//
# # basedir=v3.28_ft_mar6_v1///
# basedir=v3.28_ft_mar6unblind_v1///
# # basedir=v3.28_ft_mar6oldbdt_v1///
# # basedir=v3.27_ft_mar5jes_v1//
# # ../runLimits.py ${basedir}/card_tttt_srdisc_2016.txt -r srdisc ${extra}  --unblinded --tagfit y2016_srdisc_obs | tee y2016_lims_srdisc_obs.txt
# # ../runLimits.py ${basedir}/card_tttt_srdisc_2016.txt -r srdisc ${extra}  --tagfit y2016_srdisc_exp | tee y2016_lims_srdisc_exp.txt
# # ../runLimits.py ${basedir}/card_tttt_srdisc_2017.txt -r srdisc ${extra}  --unblinded --tagfit y2017_srdisc_obs | tee y2017_lims_srdisc_obs.txt
# # ../runLimits.py ${basedir}/card_tttt_srdisc_2017.txt -r srdisc ${extra}  --tagfit y2017_srdisc_exp | tee y2017_lims_srdisc_exp.txt
# # ../runLimits.py ${basedir}/card_tttt_srdisc_2018.txt -r srdisc ${extra}  --unblinded --tagfit y2018_srdisc_obs | tee y2018_lims_srdisc_obs.txt
# # ../runLimits.py ${basedir}/card_tttt_srdisc_2018.txt -r srdisc ${extra}  --tagfit y2018_srdisc_exp | tee y2018_lims_srdisc_exp.txt
# ../runLimits.py ${basedir}/card_tttt_srcr_run2.txt -r srcr ${extra}  --tagfit run2_srcr_exp | tee lims_srcr_exp.txt
# ../runLimits.py ${basedir}/card_tttt_srcr_run2.txt -r srcr ${extra}  --unblinded --tagfit run2_srcr_obs | tee lims_srcr_obs.txt
# # ../runLimits.py ${basedir}/card_tttt_srcr_2016.txt -r srcr ${extra}  --unblinded --tagfit y2016_srcr_obs | tee y2016_lims_srcr_obs.txt
# # ../runLimits.py ${basedir}/card_tttt_srcr_2016.txt -r srcr ${extra}  --tagfit y2016_srcr_exp | tee y2016_lims_srcr_exp.txt
# # ../runLimits.py ${basedir}/card_tttt_srcr_2017.txt -r srcr ${extra}  --unblinded --tagfit y2017_srcr_obs | tee y2017_lims_srcr_obs.txt
# # ../runLimits.py ${basedir}/card_tttt_srcr_2017.txt -r srcr ${extra}  --tagfit y2017_srcr_exp | tee y2017_lims_srcr_exp.txt
# # ../runLimits.py ${basedir}/card_tttt_srcr_2018.txt -r srcr ${extra}  --unblinded --tagfit y2018_srcr_obs | tee y2018_lims_srcr_obs.txt
# # ../runLimits.py ${basedir}/card_tttt_srcr_2018.txt -r srcr ${extra}  --tagfit y2018_srcr_exp | tee y2018_lims_srcr_exp.txt
# ../runLimits.py ${basedir}/card_tttt_srdisc_run2.txt -r srdisc ${extra}  --tagfit run2_srdisc_exp | tee lims_srdisc_exp.txt
# ../runLimits.py ${basedir}/card_tttt_srdisc_run2.txt -r srdisc ${extra}  --unblinded --tagfit run2_srdisc_obs | tee lims_srdisc_obs.txt
# # outname=nuisance_output.root
# # python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex fitDiagnosticsrun2_srcr_exp.root | grep -v 'with the following' > nuisances_run2_srcr_exp.tex
# # python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex fitDiagnosticsrun2_srcr_obs.root | grep -v 'with the following' > nuisances_run2_srcr_obs.tex
# # python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex fitDiagnosticsrun2_srdisc_exp.root | grep -v 'with the following' > nuisances_run2_srdisc_exp.tex
# # python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex fitDiagnosticsrun2_srdisc_obs.root | grep -v 'with the following' > nuisances_run2_srdisc_obs.tex
# # # python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex fitDiagnosticsy2018_srcr_exp.root | grep -v 'with the following' > nuisances_y2018_srcr_exp.tex
# # # python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a -g ${outname} -f latex fitDiagnosticsy2018_srdisc_exp.root | grep -v 'with the following' > nuisances_y2018_srdisc_exp.tex
# # mkdir -p plots
# # python parse_fit.py


# python ../plot1DScan.py v3.28_ft_mar6unblind_v1/scandatarun2_srdisc_obs.root -l Observed --xsec

# python make_cards.py
# ../runLimits.py v3.27_ft_mar5newfr_v1///card_tttt_srcr_run2.txt -r srcr ${extra}  --tagfit run2_srcr_exp | tee newfr_lims_srcr_exp.txt
# ../runLimits.py v3.27_ft_mar5newfr_v1///card_tttt_srdisc_run2.txt -r srdisc ${extra}  --tagfit run2_srdisc_exp | tee newfr_lims_srdisc_exp.txt

# python make_cards.py
# # extra=""
# extra="--redolimits"
# ../runLimits.py v3.27_ft_mar5_v1///card_tttt_srcr_run2.txt -r srcr ${extra}  --tagfit run2_srcr_exp | tee test_expuncorr.txt

# # python make_cards.py
# # extra=""
# extra="--redolimits"
# basedir=v3.27_ft_mar5oldbdt_v1//
# ../runLimits.py ${basedir}/card_tttt_srcr_run2.txt -r srcr ${extra}  --tagfit run2_srcr_exp | tee oldbdt_lims_srcr_exp.txt
# ../runLimits.py ${basedir}/card_tttt_srdisc_run2.txt -r srdisc ${extra}  --tagfit run2_srdisc_exp | tee oldbdt_lims_srdisc_exp.txt

# python make_cards.py
# # extra=""
# extra="--redolimits"
# # basedir=v3.28_ft_mar6oldbdt_v1/
# basedir=v3.28_ft_mar6oldbdtnewbins_v1/
# ../runLimits.py ${basedir}/card_tttt_srdisc_run2.txt -r srdisc ${extra}  --tagfit oldbdtrun2_srdisc_exp | tee oldbdt_lims_srdisc_exp.txt

# combine -M FitDiagnostics v3.28_ft_mar6_v1//card_tttt_srcr_run2.txt --expectSignal=1 -t -1 --robustFit=1 --saveShapes --saveWithUncertainties --saveOverallShapes -n _exp_corrs_srcr --plots --numToysForShapes 200 >& /dev/null &
# combine -M FitDiagnostics v3.28_ft_mar6_v1//card_tttt_srdisc_run2.txt --expectSignal=1 -t -1 --robustFit=1 --saveShapes --saveWithUncertainties --saveOverallShapes -n _exp_corrs_srdisc --plots --numToysForShapes 200 >& /dev/null &
