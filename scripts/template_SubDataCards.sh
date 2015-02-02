#!/bin/bash

# to be executed in MPAF/display directory

SRS=( "SR00" "SR10" "SR20" "SR30" )
PTS=( "hh" "hl" "ll" )
FLS=( "ee" "em" "mm" )

rm cards/tmpFiles/*

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
