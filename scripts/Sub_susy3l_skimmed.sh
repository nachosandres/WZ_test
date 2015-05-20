#!/bin/bash

SRS=( "SR00" "SR01" "SR02" "SR03" "SR04" "SR05" "SR06" "SR07" "SR08" "SR09" "SR10" "SR11" "SR12" "SR13" "SR14" "SR15" "SR16" "SR17" "SR18" "SR19" "SR20" "SR21" "SR22" "SR23" "SR24" "SR25" "SR26" "SR27" "SR28" "SR29" "SR30" "SR31" "SR32" "SR33" "SR34" "SR35" "SR36" "SR37" "SR38" "SR39" "SR40" "SR41" "SR42" "SR43" "SR44" "SR45" "SR46" "SR47" "SR48" "SR49" "SR50" "SR51" "SR52" "SR53" "SR54" "SR55" "SR56" "SR57" "SR58" "SR59" "SR60" "SR61" "SR62" "SR63" "SR64" "SR65" "SR66" "SR67" "SR68" "SR69" "SR70" "SR71" "SR72" "SR73" "SR74" "SR75" "SR76" "SR77" "SR78" "SR79" "SR80" )

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
        analysis -c $MPAF/cfg/tmpFiles/susy3l_${sregion}_${pairstate}.cfg
        
        #submit jobs to batch
        #qsub -q all.q -N MPAFjob -o $MPAF/workdir/logs/susy3l/susy3l_${sregion}_${pairstate}.out -e $MPAF/workdir/logs/susy3l/susy3l_${sregion}_${pairstate}.err $MPAF/scripts/submit.sh $MPAF/cfg/tmpFiles/susy3l_${sregion}_${pairstate}.cfg
    done
done
