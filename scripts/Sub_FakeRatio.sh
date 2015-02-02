#!/bin/bash

MRS=( "MR00" "MR01" "MR02" "MR03" )
#LIDS=( "mvanotcut" "cutnotmva" )
#DIRS=( "FRskimsMVA" "FRskimsCUT" )
LIDS=( "mva" "mvanotcut" "mvaandcut" "cut" "cutnotmva" "cutandmva" )
DIRS=( "FRskimsMVA" "FRskimsMVA" "FRskimsMVA" "FRskimsCUT" "FRskimsCUT" "FRskimsCUT" )

#rm cfg/tmpFiles/*

for mr in ${MRS[@]}; do

  ii=0

  for lid in ${LIDS[@]}; do

    mregion=$mr 
    lepid=$lid
    skdir=${DIRS[$ii]}

    cp cfg/template_FakeRatio.cfg cfg/tmpFiles/FakeRatio_${mregion}_${lepid}.cfg
    sed -i 's|MEASUREMENTREGION|'$mregion'|' cfg/tmpFiles/FakeRatio_${mregion}_${lepid}.cfg
    sed -i 's|LEPTONID|'$lepid'|' cfg/tmpFiles/FakeRatio_${mregion}_${lepid}.cfg 
    sed -i 's|SKIMDIR|'$skdir'|' cfg/tmpFiles/FakeRatio_${mregion}_${lepid}.cfg

    #analysis -c cfg/tmpFiles/FakeRatio_${mregion}_${lepid}.cfg
    qsub -q all.q -N MPAFjob -o $MPAF/workdir/logs/FakeRatio/FakeRatio_${mregion}_${lepid}.out -e $MPAF/workdir/logs/FakeRatio/FakeRatio_${mregion}_${lepid}.err $MPAF/scripts/submit.sh $MPAF/cfg/tmpFiles/FakeRatio_${mregion}_${lepid}.cfg

  done
done
