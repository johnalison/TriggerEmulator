# TriggerEmulator

Emulate b-jet trigger decisions 

From a CMSSW/src release area

>#(only do this once the first time) cmsrel CMSSW_10_2_0

>cd CMSSW_10_2_0/src

>cmsenv

Checkout the nTupleAnalysis base class repo

>git clone git@github.com:johnalison/nTupleAnalysis.git

Checkout the AnaExample repo

>git clone git@github.com:johnalison/AnaExample.git

>scram b -j 5

>voms-proxy-init -voms cms

Run the following with --help to see what command line argument you can specify. Otherwise it runs with the default input files and default output directory. 

>exampleAna AnaExample/nTupleAnalysis/scripts/exampleAna_cfg.py  -i AnaExample/nTupleAnalysis/fileLists/ZToMuMu.txt -o $PWD -y 2018 -l 59.6e3 --histogramming 1 --histFile hists.root --isMC --nevents 1000

