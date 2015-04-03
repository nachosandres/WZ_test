#!/bin/bash

#usage from MPAF folder: source scripts/phys14exerc_SubJobs.sh
templateCFG=template_phys14exerc.cfg

#for each SR use corresponding BR
SRS=( "SR00" "SR10" "SR20" "SR30" )
BRS=( "noBR" "noBR" "noBR" "noBR" )
PTS=( "hh" "hl" ) #hh, hl, ll
MVAs=( "cut" ) #"cut, mva
BTAGs=( "40" )
FLAVs=( "ee" "mm" "em" "all" )

if [ ! -d "cfg/tmpFiles" ]; then
  mkdir cfg/tmpFiles
fi

if [ ! -d "workdir/logs" ]; then
  mkdir workdir/logs
fi


for im in ${MVAs[@]}; do

  mva=$im

  for ij in ${BTAGs[@]}; do

    btag=$ij

    for ip in ${PTS[@]}; do

      pt=$ip
      ii=0

      for sr in ${SRS[@]}; do

        ib=${BRS[$ii]}
	  
	for iflav in ${FLAVs[@]}; do  
	  flav=$iflav

          cp cfg/$templateCFG cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}_${flav}.cfg
          sed -i 's|replaceSIGNALREGION|'$sr'|' cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}_${flav}.cfg
          sed -i 's|replaceBASELINEREGION|'$ib'|' cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}_${flav}.cfg
          sed -i 's|replaceLEPPT|'$pt'|' cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}_${flav}.cfg
          sed -i 's|replaceLEPID|'$mva'|' cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}_${flav}.cfg
          sed -i 's|replaceBTAG|'$btag'|' cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}_${flav}.cfg
          sed -i 's|replaceLEPFLAV|'$flav'|' cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}_${flav}.cfg
   	
          echo running: susy_${sr}_${pt}_${mva}_${btag}_${flav}.cfg
	  #qsub -q all.q -N MPAFjob -o $MPAF/workdir/logs/log_${sr}_${pt}_${mva}_${btag}_${flav}.out -e $MPAF/workdir/logs/log_${sr}_${pt}_${mva}_${btag}_${flav}.err $MPAF/scripts/submit.sh $MPAF/cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}_${flav}.cfg
          analysis -c cfg/tmpFiles/susy_${sr}_${pt}_${mva}_${btag}_${flav}.cfg >& $MPAF/workdir/logs/log_${sr}_${pt}_${mva}_${btag}_${flav}.log

          #ii=`echo $ii +1 | bc`

        done
      done
    done
  done
done
