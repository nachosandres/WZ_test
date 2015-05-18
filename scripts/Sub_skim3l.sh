#!/bin/bash

#rm cfg/tmpFiles/*


cp cfg/template3L.cfg cfg/tmpFiles/susy3l_skim.cfg

#run locally
#analysis -c $MPAF/cfg/tmpFiles/susy3l_skim.cfg

#submit jobs to batch
qsub -q all.q -N MPAFjob -o $MPAF/workdir/logs/susy3l/susy3l_skim.out -e $MPAF/workdir/logs/susy3l/susy3l_skim.err $MPAF/scripts/submit.sh $MPAF/cfg/tmpFiles/susy3l_skim.cfg
