#!/bin/bash

#usage from MPAF folder: source scripts/phys14exerc_Display.sh
templateDISPLAY=template_phys14exerc.C


outFOLDER=$MPAF/display/tmpFiles/Plots_phys14exerc

#for each SR use corresponding BR
SRS=( "SR00" "SR10" "SR20" "SR30" )
PTS=( "hh" "hl" ) #hh, hl, ll
MVAs=( "cut" ) #"cut, mva
BTAGs=( "40" )
FLAVs=( "ee" "mm" "em" "all" )
VARs=( "SR_HT" "SR_MLL" )

if [ ! -d "$MPAF/display/tmpFiles" ]; then
  mkdir $MPAF/display/tmpFiles
fi

if [ ! -d "$MPAF/workdir/logs" ]; then
  mkdir $MPAF/workdir/logs
fi

mkdir $outFOLDER
mkdir $outFOLDER/eps
mkdir $outFOLDER/png
mkdir $outFOLDER/pdf/
mkdir $outFOLDER/root/


for im in ${MVAs[@]}; do

  mva=$im

  for ij in ${BTAGs[@]}; do

    btag=$ij

    for ip in ${PTS[@]}; do

      pt=$ip
      ii=0

      for sr in ${SRS[@]}; do

	  
	for iflav in ${FLAVs[@]}; do  
	  flav=$iflav
	  
	  for ivar in ${VARs[@]}; do
	  
	    var=$ivar

            cp $MPAF/display/cards/$templateDISPLAY $MPAF/display/tmpFiles/display_${sr}_${pt}_${mva}_${btag}_${flav}_${var}.C
            sed -i 's|replaceSIGNALREGION|'$sr'|' $MPAF/display/tmpFiles/display_${sr}_${pt}_${mva}_${btag}_${flav}_${var}.C
            sed -i 's|replaceLEPPT|'$pt'|' $MPAF/display/tmpFiles/display_${sr}_${pt}_${mva}_${btag}_${flav}_${var}.C
            sed -i 's|replaceLEPID|'$mva'|' $MPAF/display/tmpFiles/display_${sr}_${pt}_${mva}_${btag}_${flav}_${var}.C
            sed -i 's|replaceBTAG|'$btag'|' $MPAF/display/tmpFiles/display_${sr}_${pt}_${mva}_${btag}_${flav}_${var}.C
            sed -i 's|replaceLEPFLAV|'$flav'|' $MPAF/display/tmpFiles/display_${sr}_${pt}_${mva}_${btag}_${flav}_${var}.C
            sed -i 's|replaceVARIABLE|'$var'|' $MPAF/display/tmpFiles/display_${sr}_${pt}_${mva}_${btag}_${flav}_${var}.C
   	
            echo running: display_${sr}_${pt}_${mva}_${btag}_${flav}_${var}.C
            cd $MPAF/display
	    root -q -b $MPAF/display/tmpFiles/display_${sr}_${pt}_${mva}_${btag}_${flav}_${var}.C >& $MPAF/workdir/logs/display_${sr}_${pt}_${mva}_${btag}_${flav}_${var}.log
            mv $MPAF/workdir/plots/phys14exerc/eps/${var}.eps $outFOLDER/eps/${var}_${sr}_${pt}_${mva}_${btag}_${flav}.eps
	    mv $MPAF/workdir/plots/phys14exerc/png/${var}.png $outFOLDER/png/${var}_${sr}_${pt}_${mva}_${btag}_${flav}.png
	    mv $MPAF/workdir/plots/phys14exerc/pdf/${var}.pdf $outFOLDER/pdf/${var}_${sr}_${pt}_${mva}_${btag}_${flav}.pdf
	    mv $MPAF/workdir/plots/phys14exerc/root/${var}.root $outFOLDER/root/${var}_${sr}_${pt}_${mva}_${btag}_${flav}.root
	    
	    cd -
          done
	done
      done
    done
  done
done
