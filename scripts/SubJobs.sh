#!/bin/bash

SRS=( "SR01" "SR02" )

SAMPLES=( "DYJetsM50_HT100to200_PU_S14_POSTLS170" "DYJetsM50_HT200to400_PU_S14_POSTLS170" )

for is in ${SAMPLES[@]}; do

    for sr in ${SRS[@]}; do
	
	cp cfg/templateSUSY.cfg cfg/tmpFiles/susy_${sr}_${is}.cfg
	sed -i 's|SIGNALREGION|'$sr'|' cfg/tmpFiles/susy_${sr}_${is}.cfg
	sed -i 's|SAMPLE|'$sr'|' cfg/tmpFiles/susy_${sr}_${is}.cfg
   # sed -i 's|YYY|'$N'|' cfg/tmpFiles/susy_${sr}.cfg
	
	qsub -q all.q submit.sh cfg/tmpFiles/susy_${sr}_${is}.cfg

    done

done