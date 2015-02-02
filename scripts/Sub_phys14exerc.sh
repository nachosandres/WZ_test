#!/bin/bash

SRS=( "SR00" "SR10" "SR20" "SR30" )
PTS=( "hh" "hl" "ll" )
FLS=( "ee" "em" "mm" )

#rm cfg/tmpFiles/*

for sr in ${SRS[@]}; do

  for pt in ${PTS[@]}; do

    for fl in ${FLS[@]}; do

      sregion=$sr
      leppt=$pt
      flavor=$fl

      cp cfg/template_phys14exerc.cfg cfg/tmpFiles/phys14exerc_${sregion}_${leppt}_${flavor}.cfg
      sed -i 's|SIGNALREGION|'$sregion'|' cfg/tmpFiles/phys14exerc_${sregion}_${leppt}_${flavor}.cfg
      sed -i 's|PTSCENARIO|'$leppt'|' cfg/tmpFiles/phys14exerc_${sregion}_${leppt}_${flavor}.cfg
      sed -i 's|FLAVORS|'$flavor'|' cfg/tmpFiles/phys14exerc_${sregion}_${leppt}_${flavor}.cfg

      #analysis -c $MPAF/cfg/tmpFiles/phys14exerc_${sregion}_${leppt}_${flavor}.cfg
      qsub -q all.q -N MPAFjob -o $MPAF/workdir/logs/phys14exerc/phys14exerc_${sregion}_${leppt}_${flavor}.out -e $MPAF/workdir/logs/phys14exerc/phys14exerc_${sregion}_${leppt}_${flavor}.err $MPAF/scripts/submit.sh $MPAF/cfg/tmpFiles/phys14exerc_${sregion}_${leppt}_${flavor}.cfg

    done
  done
done
