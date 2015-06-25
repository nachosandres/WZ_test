#!/bin/bash
source /swshare/ROOT/root_v5.34.18_slc6_amd64/bin/thisroot.sh

cd /shome/jhoss/analysis/MPAF

source setup.sh
analysis -c $1
