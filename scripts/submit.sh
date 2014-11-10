#!/bin/bash
#source $VO_CMS_SW_DIR/cmsset_default.sh
#cd /shome/cheidegg/CMSSW_7_0_6_patch3/src
#eval `scramv1 runtime -sh`
source /swshare/ROOT/root_v5.34.18_slc6_amd64/bin/thisroot.sh
cd /shome/mmarionn/MPAF
source setup.sh
analysis -c $1
