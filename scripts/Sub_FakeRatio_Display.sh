#!/bin/bash

# to be executed in MPAF/display directory

MRS=( "MR00" "MR01" "MR02" "MR03" )
IDS=( "cut" "cutnotmva" "cutandmva" "mva" "mvanotcut" "mvaandcut" )
OBS=( "LLepMap" "TLepMap" )

rm cards/tmpFiles/*

for mr in ${MRS[@]}; do

  for id in ${IDS[@]}; do

    for ob in ${OBS[@]}; do

      mregion=$mr
      lepid=$id
      observable=$ob

      cp cards/template_FakeRatio.C cards/tmpFiles/FakeRatio_${mregion}_${lepid}_${observable}.C
      sed -i 's|MEASUREMENTREGION|'$mregion'|' cards/tmpFiles/FakeRatio_${mregion}_${lepid}_${observable}.C
      sed -i 's|LEPID|'$lepid'|' cards/tmpFiles/FakeRatio_${mregion}_${lepid}_${observable}.C
      sed -i 's|VARTOPLOT|'$observable'|' cards/tmpFiles/FakeRatio_${mregion}_${lepid}_${observable}.C

      root -l -b cards/tmpFiles/FakeRatio_${mregion}_${lepid}_${observable}.C

    done
  done
done
