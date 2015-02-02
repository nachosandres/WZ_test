#!/bin/bash

MVAS="cut"
SAMPLES=( "WJetsToLNu_13TeV-madgraph-pythia8-tauola" )
#SAMPLES=( "DYJetsToLL_M-50_13TeV-madgraph-pythia8" "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola" "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola" "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola" "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola" "TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola" "TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola" "Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola" "TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola" "TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola" "TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola" "TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola" "T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola" "TTWJets_Tune4C_13TeV-madgraph-tauola" "TTZJets_Tune4C_13TeV-madgraph-tauola" "WJetsToLNu_13TeV-madgraph-pythia8-tauola" "WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola" "WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola" "WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola" "WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola" "WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola" )

#rm cfg/tmpFiles/*

for sample in ${SAMPLES[@]}; do

  ds=$sample

  cp cfg/template_FakeRatio_Skim.cfg cfg/tmpFiles/FakeRatioSkim_${ds}.cfg
  sed -i 's|LEPTONID|'$MVAS'|' cfg/tmpFiles/FakeRatioSkim_${ds}.cfg
  sed -i 's|SAMPLE|'$ds'|' cfg/tmpFiles/FakeRatioSkim_${ds}.cfg

  analysis -c $MPAF/cfg/tmpFiles/FakeRatioSkim_${ds}.cfg
  #qsub -q all.q -N MPAFjob -o $MPAF/workdir/logs/FakeRatio/FakeRatioSkim_${ds}.out -e $MPAF/workdir/logs/FakeRatio/FakeRatioSkim_${ds}.err $MPAF/scripts/submit.sh $MPAF/cfg/tmpFiles/FakeRatioSkim_${ds}.cfg

done
