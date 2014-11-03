#!/bin/bash

#SAMPLES=( "DYJetsM50_HT100to200_PU_S14_POSTLS170" "DYJetsM50_HT200to400_PU_S14_POSTLS170" "DYJetsM50_HT400to600_PU_S14_POSTLS170" "DYJetsM50_HT600toInf_PU_S14_POSTLS170" "WJetsToLNu_HT100to200_PU_S14_POSTLS170" "WJetsToLNu_HT200to400_PU_S14_POSTLS170" "WJetsToLNu_HT400to600_PU_S14_POSTLS170" "WJetsToLNu_HT600toInf_PU_S14_POSTLS170" "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170" )

SAMPLES=( "DYJetsM50_HT100to200_PU_S14_POSTLS170" "DYJetsM50_HT200to400_PU_S14_POSTLS170" "DYJetsM50_HT400to600_PU_S14_POSTLS170" "DYJetsM50_HT600toInf_PU_S14_POSTLS170" "T1tttt2J_6_PU_S14_POSTLS170" "T1tttt2J_7_PU_S14_POSTLS170" "T2tt_S14" "T5Full_1200_1000_800" "T5Full_1500_800_100" "TTHnlo_S14" "TTHWWpy6_S14" "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170" "TTWJets_S14" "TTZJets_S14" "WJetsToLNu_HT100to200_PU_S14_POSTLS170" "WJetsToLNu_HT200to400_PU_S14_POSTLS170" "WJetsToLNu_HT400to600_PU_S14_POSTLS170" "WJetsToLNu_HT600toInf_PU_S14_POSTLS170" )


for is in ${SAMPLES[@]}; do

		cp cfg/templateSkim.cfg cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg
		sed -i 's|SAMPLE|'$is'|' cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg
		#sed -i 's|YYY|'$N'|' cfg/tmpFiles/susy_${sr}.cfg
		
		qsub -q all.q -N MPAFjob -o /shome/cheidegg/MPAF/workdir/logs/log_${sr}_${is}_${pt}.out -e /shome/cheidegg/MPAF/workdir/logs/log_${sr}_${is}_${pt}.err /shome/cheidegg/MPAF/scripts/submit.sh /shome/cheidegg/MPAF/cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg
	

done
