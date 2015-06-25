#!/bin/bash

SRS=( "SR00" "SR01" "SR02" "SR03" "SR04" "SR05" "SR06" "SR07" "SR08" "SR09" "SR10" "SR11" "SR12" "SR13" "SR14" "SR15" "SR16" "SR17" "SR18" "SR19" "SR20" "SR21" "SR22" "SR23" "SR24" "SR25" "SR26" "SR27" "SR28" "SR29" )

OSSFPAIR=( "on" "off" )

#rm cfg/tmpFiles/*

for sr in ${SRS[@]}; do

    for pair in ${OSSFPAIR[@]}; do

        sregion=$sr
        pairstate=$pair

        cp cfg/template3L_auto_skimmed.cfg cfg/tmpFiles/susy3l_${sregion}_${pairstate}.cfg
        sed -i 's|SIGNALREGION|'$sregion'|' cfg/tmpFiles/susy3l_${sregion}_${pairstate}.cfg
        sed -i 's|PAIRSTATE|'$pairstate'|'  cfg/tmpFiles/susy3l_${sregion}_${pairstate}.cfg

        #run locally
        #analysis -c $MPAF/cfg/tmpFiles/susy3l_${sregion}_${pairstate}.cfg
        
        #submit jobs to batch
        qsub -q all.q -N MPAFjob -o $MPAF/workdir/logs/susy3l/susy3l_${sregion}_${pairstate}.out -e $MPAF/workdir/logs/susy3l/susy3l_${sregion}_${pairstate}.err $MPAF/scripts/submit.sh $MPAF/cfg/tmpFiles/susy3l_${sregion}_${pairstate}.cfg
    done
done
