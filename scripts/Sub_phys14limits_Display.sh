#!/bin/bash

# to be executed in MPAF/display directory

SRS=( "SR00" "SR10" "SR20" "SR30" "SR01" "SR02" "SR03" "SR04" "SR05" "SR06" "SR07" "SR08" "SR11" "SR12" "SR13" "SR14" "SR15" "SR16" "SR17" "SR18" "SR21" "SR22" "SR23" "SR24" "SR25" "SR26" "SR27" "SR28" )
PTS=( "hh" "hl" "ll" )
FLS=( "ee" "em" "mm" )

#rm cards/tmpFiles/*

for sr in ${SRS[@]}; do

  for pt in ${PTS[@]}; do

    for fl in ${FLS[@]}; do

      sregion=$sr
      leppt=$pt
      flavor=$fl

      cp cards/template_datacard.C cards/tmpFiles/datacard_${sregion}_${leppt}_${flavor}.C
      sed -i 's|SIGNALREGION|'$sregion'|' cards/tmpFiles/datacard_${sregion}_${leppt}_${flavor}.C
      sed -i 's|PTSCENARIO|'$leppt'|' cards/tmpFiles/datacard_${sregion}_${leppt}_${flavor}.C
      sed -i 's|FLAVORS|'$flavor'|' cards/tmpFiles/datacard_${sregion}_${leppt}_${flavor}.C

      root -l -b cards/tmpFiles/datacard_${sregion}_${leppt}_${flavor}.C

    done
  done
done
