#!/bin/bash

#USEDIR="/pnfs/psi.ch/cms/trivcat/store/user/cheidegg/sea/trees/2015-01-04_phys14-first-production/root/"
#SAMPLES=( "DYJetsToLL_M-50_13TeV-madgraph-pythia8" "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola" "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola" "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola" "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola" "SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola" "SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola" "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola" "TTWJets_Tune4C_13TeV-madgraph-tauola" "TTZJets_Tune4C_13TeV-madgraph-tauola" "WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola" "WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola" "WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola" "WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola" )
#USEDIR="myroot/"
#SAMPLES=( "T1ttbbWW_mGo1000_mCh725_mChi720" "T1ttbbWW_mGo1300_mCh300_mChi290" "T5qqqqWW_mGo1200_mCh1000_mChi800" "T5ttttDeg_mGo1000_mStop300_mCh285_mChi280" "T5ttttDeg_mGo1000_mStop300_mChi280" "T6ttWW_mSbot600_mCh425_mChi50" "T6ttWW_mSbot650_mCh150_mChi50" "WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola" )
USEDIR="/pnfs/psi.ch/cms/trivcat/store/user/cheidegg/temp/"
SAMPLES=( "WJetsToLNu_13TeV-madgraph-pythia8-tauola" )

rm cfg/tmpFiles/*

for sample in ${SAMPLES[@]}; do

  ds=$sample

  cp cfg/template_phys14exerc_Skim.cfg cfg/tmpFiles/phys14exercSkim_${ds}.cfg
  sed -i 's|USEDIR|'$USEDIR'|' cfg/tmpFiles/phys14exercSkim_${ds}.cfg
  sed -i 's|SAMPLE|'$ds'|' cfg/tmpFiles/phys14exercSkim_${ds}.cfg

  #analysis -c $MPAF/cfg/tmpFiles/phys14exercSkim_${ds}.cfg
  qsub -q all.q -N MPAFjob -o $MPAF/workdir/logs/phys14exerc/phys14exercSkim_${ds}.out -e $MPAF/workdir/logs/phys14exerc/phys14exercSkim_${ds}.err $MPAF/scripts/submit.sh $MPAF/cfg/tmpFiles/phys14exercSkim_${ds}.cfg

done
