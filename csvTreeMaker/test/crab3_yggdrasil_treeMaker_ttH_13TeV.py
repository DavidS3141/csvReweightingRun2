from WMCore.Configuration import Configuration 
config = Configuration() 
config.section_("General") 
config.General.requestName = 'treeMaker_SPRING15_v2' ## change

config.section_("JobType") 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'csv_treeMaker_cfg.py' 
config.JobType.allowUndistributedCMSSW = False

config.section_("Data") 
config.Data.inputDataset = '/ttHTobb_M125_13TeV_powheg_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM' ## change
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/' 
config.Data.splitting = 'FileBased' 
config.Data.unitsPerJob = 1
#config.Data.totalUnits = 100
config.Data.publication = True 
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/' 
config.Data.publishDataName = 'Validation_v1'
### change user Space 
config.Data.outLFNDirBase = '' # change '/store/user/lwming/'
config.Data.ignoreLocality = True

config.section_("Site") 
config.Site.storageSite = 'T3_US_FNALLPC' #change
