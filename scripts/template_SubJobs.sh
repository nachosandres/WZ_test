#!/bin/bash

SRS=( "SR00" "SR01" "SR02" "SR03" "SR04" "SR05" "SR06" "SR07" "SR08" "SR11" "SR12" "SR13" "SR14" "SR15" "SR16" "SR17" "SR18" "SR21" "SR22" "SR23" "SR24" "SR25" "SR26" "SR27" "SR28" "SR31" "SR32" "SR33" "SR34" "SR35" "SR36" "SR37" "SR38" )
BRS=( "BRAll" "BR00" "BR00" "BR00" "BR00" "BR00" "BR00" "BR00" "BR00" "BR01" "BR01" "BR01" "BR01" "BR01" "BR01" "BR01" "BR01" "BR02" "BR02" "BR02" "BR02" "BR02" "BR02" "BR02" "BR02" "BR03" "BR03" "BR03" "BR03" "BR03" "BR03" "BR03" "BR03" )
PTS=( "highpt" "lowpt" )
MVAs=( "cut" "mva" )
BTAGs=( "40" "25" "IVF" )

rm cfg/tmpFiles/*

for im in ${MVAs[@]}; do

  mva=$im

  for ij in ${BTAGs[@]}; do

    btag=$ij

    for ip in ${PTS[@]}; do

      pt=$ip
      ii=0

      for sr in ${SRS[@]}; do

        ib=${BRS[$ii]}

        cp cfg/template_SUSYSSDL.cfg cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}.cfg
        sed -i 's|SIGNALREGION|'$sr'|' cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}.cfg
        sed -i 's|BASELINEREGION|'$ib'|' cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}.cfg
        sed -i 's|PTSELECTION|'$pt'|' cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}.cfg
        sed -i 's|MVALEP|'$mva'|' cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}.cfg
        sed -i 's|BTAG|'$btag'|' cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}.cfg
   	
        qsub -q all.q -N MPAFjob -o $MPAF/workdir/logs/log_${sr}_${pt}_${mva}_${btag}.out -e $MPAF/workdir/logs/log_${sr}_${pt}_${mva}_${btag}.err $MPAF/scripts/submit.sh $MPAF/cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}.cfg

        ii=`echo $ii +1 | bc`

      done
    done
  done
done
