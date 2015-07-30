 #!/bin/bash

#usage from MPAF folder: source scripts/phys14exerc_SubJobs.sh
#templateCFG=template_fakeEstim.cfg
templateCFG=template_3L.cfg

NAME="WZ3L"
#FLAVs=( "all" "eee" "eem" "mme" "mmm" )
FLAVs=( "all" )
#FLAVs=( "eee" "eem" "mme" "mmm" )
#WZSTEPs=( "4" "3" "2" "1" "0" )
WZSTEPs=( "5" )

QSQRs=( "1001" )
#QSQRs=( "1001" "1002" "1003" "1004" "1005" "1006" "1007" "1008" "1009" )
#PDFrange=( "2001" "2100" )

LHEs=()
nQSQR=0
if [ ! -z "${QSQRs[0]}" ]; then
  LHEs=${QSQRs[@]}
  nQSQR=${#QSQRs[@]}
fi


if [ ! -z "${PDFrange[0]}" ]; then
  echo variable set
  for (( iter=${PDFrange[0]}; iter<=${PDFrange[1]}; iter++ )); do
    LHEs+=( ${iter} )
  done
fi


if [ ! -d "cfg/tmpFiles" ]; then
  mkdir cfg/tmpFiles
fi

if [ ! -d "workdir/logs" ]; then
  mkdir workdir/logs
fi


for iwzstep in ${WZSTEPs[@]}; do

      wzstep=$iwzstep
      ii=0


	  
	for iflav in ${FLAVs[@]}; do  
	  flav=$iflav
	  
	  ilhe=0
	  for lhe in ${LHEs[@]}; do
	      
	      LHESYS=""
	      replaceLHESYS=""
	      if (( ilhe < nQSQR )); then
	         LHESYS="LHE"
	         replaceLHESYS="replaceLHESYS"
              else
                 LHESYS="LHE"
	         replaceLHESYS="replaceLHESYS"
              fi

              cp cfg/$templateCFG cfg/tmpFiles/${NAME}_WZSTEP${wzstep}_LEPFLAV${flav}_${LHESYS}${lhe}.cfg
              sed -i 's|replaceLEPPT|'all'|' cfg/tmpFiles/${NAME}_WZSTEP${wzstep}_LEPFLAV${flav}_${LHESYS}${lhe}.cfg
              sed -i 's|replaceWZstep|'$wzstep'|' cfg/tmpFiles/${NAME}_WZSTEP${wzstep}_LEPFLAV${flav}_${LHESYS}${lhe}.cfg
              sed -i 's|replaceLEPFLAV|'$flav'|' cfg/tmpFiles/${NAME}_WZSTEP${wzstep}_LEPFLAV${flav}_${LHESYS}${lhe}.cfg
              #sed -i 's|replaceQsqr|'$lhe'|' cfg/tmpFiles/${NAME}_WZSTEP${wzstep}_LEPFLAV${flav}_${LHESYS}${lhe}.cfg
              sed -i 's|'$replaceLHESYS'|'$lhe'|' cfg/tmpFiles/${NAME}_WZSTEP${wzstep}_LEPFLAV${flav}_${LHESYS}${lhe}.cfg
   	
              echo running: ${NAME}_WZSTEP${wzstep}_LEPFLAV${flav}_${LHESYS}${lhe}.cfg

	      #qsub -q all.q -N MPAFjob -o $MPAF/workdir/logs/log_${sr}_${pt}_${mva}_${btag}_${flav}_${LHESYS}${lhe}.out -e $MPAF/workdir/logs/log_${sr}_${pt}_${mva}_${btag}_${flav}_${LHESYS}${lhe}.err $MPAF/scripts/submit.sh $MPAF/cfg/tmpFiles/${NAME}_WZSTEP${wzstep}_LEPFLAV${flav}_${LHESYS}${lhe}.cfg              
	      analysis -c cfg/tmpFiles/${NAME}_WZSTEP${wzstep}_LEPFLAV${flav}_${LHESYS}${lhe}.cfg >& $MPAF/workdir/logs/log_${NAME}_WZSTEP${wzstep}_LEPFLAV${flav}_${LHESYS}${lhe}.log

              ilhe=`echo $ilhe +1 | bc`

              #ii=`echo $ii +1 | bc`

          done
	done
done

