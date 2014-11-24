#!/bin/bash

SR=( "SR0?" "SR1?" "SR2?" "SR3?" )
LEPPT=( "highpt" "lowpt" )
LEPID=( "cut" "mva" )
JETPT=( "40" "25" "IVF" )
#SR=( "SR0?" "SR1?" )
#LEPPT=( "highpt" )
#LEPID=( "cut" )
#JETPT=( "40" )
ii=0

for isr in ${SR[@]}; do
	for ileppt in ${LEPPT[@]}; do
		for ilepid in ${LEPID[@]}; do
			for ijetpt in ${JETPT[@]}; do

				cp cards/template_susyssdl.C cards/template_susyssdl_run.C
				sed -i 's|SIGNALREGION|'$isr'|' cards/template_susyssdl_run.C
				sed -i 's|LEPPT|'$ileppt'|' cards/template_susyssdl_run.C
				sed -i 's|LEPID|'$ilepid'|' cards/template_susyssdl_run.C
				sed -i 's|JETPT|'$ijetpt'|' cards/template_susyssdl_run.C

				root -x cards/template_susyssdl_run.C 

				mv /shome/cheidegg/MPAF/workdir/plots/SUSYSSDL_ws1/png/BR_HT.png /shome/cheidegg/MPAF/workdir/plots/SUSYSSDL_ws1/png/SR${ii}_${ileppt}_${ilepid}_${ijetpt}.png

			done
		done
	done

	ii=`echo $ii +1 | bc`
done
