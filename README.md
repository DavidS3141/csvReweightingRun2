### Prerequisite: the lepton and jet object selections are based on MiniAOD, check it out first

source /cvmfs/cms.cern.ch/cmsset_default.sh

setenv SCRAM_ARCH slc6_amd64_gcc530

cmsrel CMSSW_8_0_26_patch1

cd CMSSW_8_0_26_patch1/src/

cmsenv

git cms-merge-topic ikrav:egm_id_80X_v2
scram b -j 10 

git clone https://github.com/lwming/MiniAOD.git -b Moriond17_EleID_JER

git clone https://github.com/cms-ttH/csvReweightingRun2.git

git clone https://github.com/cms-ttH/TriggerRun2.git -b csvSF_correction

scram b -j 10


### the SF calculations are done in 3 steps

1. making trees from MiniAOD samples(both MC and Data), only saving DIL events, as well as all the necessary variables. The main script is "plugins/csvTreeMaker.cc". Recompile is needed everytime you modify it.

   Run a test job: cmsRun test/csv_treeMaker_cfg.py

   Submit the full set of jobs on CRAB: crab submit -c crab3*.py

2. Collecting all the necessary histograms for the SFs calculations. The main script is "macros/csvSF_treeReader_13TeV.C". 

   root -b -q macros/head13TeV.C 'macros/csvSF_treeReader_13TeV.C+('','','','','')';

   the output file will contain histograms h_Data_jet_csv, h_MC_b_jet_csv, and h_MC_nonb_jet_csv

3. calculating the nominal SFs alone with all the systematics ???
