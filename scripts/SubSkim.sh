#!/bin/bash

#SAMPLES=( "DYJetsM50_HT100to200_PU_S14_POSTLS170" "DYJetsM50_HT200to400_PU_S14_POSTLS170" "DYJetsM50_HT400to600_PU_S14_POSTLS170" "DYJetsM50_HT600toInf_PU_S14_POSTLS170" "WJetsToLNu_HT100to200_PU_S14_POSTLS170" "WJetsToLNu_HT200to400_PU_S14_POSTLS170" "WJetsToLNu_HT400to600_PU_S14_POSTLS170" "WJetsToLNu_HT600toInf_PU_S14_POSTLS170" "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170" )

SAMPLES=( "DYJetsM50_HT100to200_PU_S14_POSTLS170" "DYJetsM50_HT200to400_PU_S14_POSTLS170" "DYJetsM50_HT400to600_PU_S14_POSTLS170" "DYJetsM50_HT600toInf_PU_S14_POSTLS170" "T1tttt2J_6_PU_S14_POSTLS170" "T1tttt2J_7_PU_S14_POSTLS170" "T2tt_S14" "T5Full_1200_1000_800" "T5Full_1500_800_100" "TTHnlo_S14" "TTHWWpy6_S14" "TTJets_MSDecaysCKM_central_PU_S14_POSTLS170" "TTWJets_S14" "TTZJets_S14" "WJetsToLNu_HT100to200_PU_S14_POSTLS170" "WJetsToLNu_HT200to400_PU_S14_POSTLS170" "WJetsToLNu_HT400to600_PU_S14_POSTLS170" "WJetsToLNu_HT600toInf_PU_S14_POSTLS170" "SMS_T1tttt_2J_mGl1200_mLSP800_PU_S14_POSTLS170" "SMS_T1tttt_2J_mGl1500_mLSP100_PU_S14_POSTLS170" "T1tttt_2J_mGo1300_mStop300_mCh285_mChi280_pythia8_S14" "T1tttt_2J_mGo1300_mStop300_mChi280_pythia8_S14" "T1tttt_2J_mGo800_mStop300_mCh285_mChi280_pythia8_S14" "T1tttt_2J_mGo800_mStop300_mChi280_pythia8_S14" )

#SAMPLES=( "SMS_T1tttt_2J_mGl1200_mLSP800_PU_S14_POSTLS170" "SMS_T1tttt_2J_mGl1500_mLSP100_PU_S14_POSTLS170" "T1tttt_2J_mGo1300_mStop300_mCh285_mChi280_pythia8_S14" "T1tttt_2J_mGo1300_mStop300_mChi280_pythia8_S14" "T1tttt_2J_mGo800_mStop300_mCh285_mChi280_pythia8_S14" "T1tttt_2J_mGo800_mStop300_mChi280_pythia8_S14" )

rm cfg/tmpFiles/*

for is in ${SAMPLES[@]}; do

		cp cfg/templateSkim.cfg cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg
		sed -i 's|SAMPLE|'$is'|' cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg
		#sed -i 's|YYY|'$N'|' cfg/tmpFiles/susy_${sr}.cfg
		
		qsub -q all.q -N MPAFjob -o $MPAF/workdir/logs/log_${sr}_${is}_${pt}.out -e $MPAF/workdir/logs/log_${sr}_${is}_${pt}.err $MPAF/scripts/submit.sh $MPAF/cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg
#	analysis -c $MPAF/cfg/tmpFiles/susy_${sr}_${is}_${pt}.cfg

done
