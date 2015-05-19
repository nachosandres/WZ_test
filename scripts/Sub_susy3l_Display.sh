#!/bin/bash

SRS=( "SR00" ) #"SR01" "SR02" "SR03" "SR04" "SR05" "SR06" "SR07" "SR08" "SR09" "SR10" "SR11" "SR12" "SR13" "SR14" "SR15" "SR16" "SR17" "SR18" "SR19" "SR20" "SR21" "SR22" "SR23" "SR24" "SR25" "SR26" "SR27" "SR28" "SR29" )

OSSFPAIR=( "on" "off" )

for sr in ${SRS[@]}; do

    for pair in ${OSSFPAIR[@]}; do

        sregion=$sr
        pairstate=$pair

        cp cards/susy3l_auto.C cards/tmpFiles/susy3l_${sregion}_${pairstate}.C
        sed -i 's|SIGNALREGION|'$sregion'|' cards/tmpFiles/susy3l_${sregion}_${pairstate}.C
        sed -i 's|PAIRSTATE|'$pairstate'|' cards/tmpFiles/susy3l_${sregion}_${pairstate}.C

        root -l -b cards/tmpFiles/susy3l_${sregion}_${pairstate}.C

    done
done
