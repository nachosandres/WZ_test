#!/bin/bash

SRS=( "SR00" "SR01" "SR02" "SR03" "SR04" "SR05" "SR06" "SR07" "SR08" "SR11" "SR12" "SR13" "SR14" "SR15" "SR16" "SR17" "SR18" "SR21" "SR22" "SR23" "SR24" "SR25" "SR26" "SR27" "SR28" "SR31" "SR32" "SR33" "SR34" "SR35" "SR36" "SR37" "SR38" )
BRS=( "BRAll" "BR00" "BR00" "BR00" "BR00" "BR00" "BR00" "BR00" "BR00" "BR01" "BR01" "BR01" "BR01" "BR01" "BR01" "BR01" "BR01" "BR02" "BR02" "BR02" "BR02" "BR02" "BR02" "BR02" "BR02" "BR03" "BR03" "BR03" "BR03" "BR03" "BR03" "BR03" "BR03" )
#SRS=( "SR01" "SR11" "SR21" )
#BRS=( "BR00" "BR01" "BR02" )
PTS=( "highpt" "lowpt" )
MVAs=( "cut" "mva" )
BTAGs=( "40" "25" "IVF" )

#SAMPLES=( "DYJetsM50_HT100to200_PU_S14_POSTLS170_skim" "DYJetsM50_HT200to400_PU_S14_POSTLS170_skim" "DYJetsM50_HT400to600_PU_S14_POSTLS170_skim" "DYJetsM50_HT600toInf_PU_S14_POSTLS170_skim" "WJetsToLNu_HT100to200_PU_S14_POSTLS170_skim" "WJetsToLNu_HT200to400_PU_S14_POSTLS170_skim" "WJetsToLNu_HT400to600_PU_S14_POSTLS170_skim" "WJetsToLNu_HT600toInf_PU_S14_POSTLS170_skim" "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170_skim" "TTWJets_S14_skim" "TTZJets_S14_skim" "TTHnlo_S14_skim" "T1tttt2J_6_PU_S14_POSTLS170_skim" "T1tttt2J_7_PU_S14_POSTLS170_skim" "T5Full_1200_1000_800_skim" "T5Full_1500_800_100_skim"  )
#SAMPLES=( "DYJetsM50_HT100to200_PU_S14_POSTLS170_skim" "DYJetsM50_HT200to400_PU_S14_POSTLS170_skim" "DYJetsM50_HT400to600_PU_S14_POSTLS170_skim" "DYJetsM50_HT600toInf_PU_S14_POSTLS170_skim" "WJetsToLNu_HT100to200_PU_S14_POSTLS170_skim" "WJetsToLNu_HT200to400_PU_S14_POSTLS170_skim" "WJetsToLNu_HT400to600_PU_S14_POSTLS170_skim" "WJetsToLNu_HT600toInf_PU_S14_POSTLS170_skim" "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170_skim" )
#SAMPLES=( "TTHnlo_S14_skim" ) # "TTWJets_S14_skim" "TTZJets_S14_skim" "T1tttt2J_6_PU_S14_POSTLS170_skim" "T1tttt2J_7_PU_S14_POSTLS170_skim" "T5Full_1200_1000_800_skim" "T5Full_1500_800_100_skim"  )

rm cfg/tmpFilesS/*

for im in ${MVAs[@]}; do

    mva=$im

    for ij in ${BTAGs[@]}; do

	btag=$ij


	for ip in ${PTS[@]}; do

	    pt=$ip

#for is in ${SAMPLES[@]}; do

	    ii=0
	#ip=${PTS[$ii]}

	    for sr in ${SRS[@]}; do

#		if [ "$btag" != "IVF" ] && [ -e /shome/mmarionn/MPAF/workdir/stats/SUSYSSDL/susy_${sr}_${pt}_${mva}_${btag}.dat ]; then
#		    continue
#		fi

		ib=${BRS[$ii]}
		cp cfg/templateSUSY.cfg cfg/tmpFilesS/susy_${sr}_${pt}_${mva}_${btag}.cfg
		sed -i 's|SIGNALREGION|'$sr'|' cfg/tmpFilesS/susy_${sr}_${pt}_${mva}_${btag}.cfg
		sed -i 's|BASELINEREGION|'$ib'|' cfg/tmpFilesS/susy_${sr}_${pt}_${mva}_${btag}.cfg
		sed -i 's|PTSELECTION|'$pt'|' cfg/tmpFilesS/susy_${sr}_${pt}_${mva}_${btag}.cfg
		sed -i 's|MVALEP|'$mva'|' cfg/tmpFilesS/susy_${sr}_${pt}_${mva}_${btag}.cfg
		sed -i 's|BTAG|'$btag'|' cfg/tmpFilesS/susy_${sr}_${pt}_${mva}_${btag}.cfg
	#	sed -i 's|SAMPLE|'$is'|' cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg
		#sed -i 's|YYY|'$N'|' cfg/tmpFiles/susy_${sr}.cfg
		
		qsub -q all.q -N MPAFjob -o $MPAF/workdir/logs/log_${sr}_${pt}_${mva}_${btag}.out -e $MPAF/workdir/logs/log_${sr}_${pt}_${mva}_${btag}.err $MPAF/scripts/submit.sh $MPAF/cfg/tmpFilesS/susy_${sr}_${pt}_${mva}_${btag}.cfg
#		analysis -c $MPAF/cfg/tmpFilesS/susy_${sr}_${pt}_${mva}_${btag}.cfg
		ii=`echo $ii +1 | bc`

	    done

	done
    done

done
