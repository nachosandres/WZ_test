#!/bin/bash

#SRS=( "SR01" "SR02" "SR03" "SR04" "SR05" "SR06" "SR07" "SR08" "SR11" "SR12" "SR13" "SR14" "SR15" "SR16" "SR17" "SR18" "SR21" "SR22" "SR23" "SR24" "SR25" "SR26" "SR27" "SR28" )
#BRS=( "BR00" "BR00" "BR00" "BR00" "BR00" "BR00" "BR00" "BR00" "BR01" "BR01" "BR01" "BR01" "BR01" "BR01" "BR01" "BR01" "BR02" "BR02" "BR02" "BR02" "BR02" "BR02" "BR02" "BR02" )
SRS=( "SR11" "SR21" )
BRS=( "BR01" "BR02" )
pt="lowpt"

SAMPLES=( "DYJetsM50_HT100to200_PU_S14_POSTLS170" "DYJetsM50_HT200to400_PU_S14_POSTLS170" "DYJetsM50_HT400to600_PU_S14_POSTLS170" "DYJetsM50_HT600toInf_PU_S14_POSTLS170" "WJetsToLNu_HT100to200_PU_S14_POSTLS170" "WJetsToLNu_HT200to400_PU_S14_POSTLS170" "WJetsToLNu_HT400to600_PU_S14_POSTLS170" "WJetsToLNu_HT600toInf_PU_S14_POSTLS170" "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170" )

for is in ${SAMPLES[@]}; do

	ii=0
	#ip=${PTS[$ii]}

    for sr in ${SRS[@]}; do

		ib=${BRS[$ii]}
		cp cfg/templateSUSY.cfg cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg
		sed -i 's|SIGNALREGION|'$sr'|' cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg
		sed -i 's|BASELINEREGION|'$ib'|' cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg
		sed -i 's|PTSELECTION|'$pt'|' cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg
		sed -i 's|SAMPLE|'$is'|' cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg
		#sed -i 's|YYY|'$N'|' cfg/tmpFiles/susy_${sr}.cfg
		
		qsub -q all.q -N MPAFjob -o /shome/cheidegg/MPAF/workdir/logs/log_${sr}_${is}_${pt}.out -e /shome/cheidegg/MPAF/workdir/logs/log_${sr}_${is}_${pt}.err /shome/cheidegg/MPAF/scripts/submit.sh /shome/cheidegg/MPAF/cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg

		ii=`echo $ii +1 | bc`

    done

done
