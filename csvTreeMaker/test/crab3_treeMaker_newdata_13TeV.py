from WMCore.Configuration import Configuration 
config = Configuration() 
config.section_("General") 
#config.General.requestName = 'DoubleEG_Run2017B_17Nov2017_Feb10th'
#config.General.requestName = 'DoubleEG_Run2017C_17Nov2017_Feb10th'
#config.General.requestName = 'DoubleEG_Run2017D_17Nov2017_Feb10th'
#config.General.requestName = 'DoubleEG_Run2017E_17Nov2017_Feb10th'
#config.General.requestName = 'DoubleEG_Run2017F_17Nov2017_Feb10th'

###################
#config.General.requestName = 'DoubleMuon_Run2017B_17Nov2017_Feb10th'
#config.General.requestName = 'DoubleMuon_Run2017C_17Nov2017_Feb10th'
#config.General.requestName = 'DoubleMuon_Run2017D_17Nov2017_Feb10th'
#config.General.requestName = 'DoubleMuon_Run2017E_17Nov2017_Feb10th'
#config.General.requestName = 'DoubleMuon_Run2017F_17Nov2017_Feb10th'

###################
#config.General.requestName = 'MuonEG_Run2017B_17Nov2017_Feb10th'
#config.General.requestName = 'MuonEG_Run2017C_17Nov2017_Feb10th'
#config.General.requestName = 'MuonEG_Run2017D_17Nov2017_Feb10th'
#config.General.requestName = 'MuonEG_Run2017E_17Nov2017_Feb10th'
config.General.requestName = 'MuonEG_Run2017F_17Nov2017_Feb10th'

config.section_("JobType") 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'data_csv_treeMaker_cfg.py'
config.JobType.allowUndistributedCMSSW = False
config.JobType.inputFiles = ['Fall17_17Nov2017C_V4_DATA.db','Fall17_17Nov2017_V4_MC.db']

config.section_("Data") 
#config.Data.inputDataset = '/DoubleEG/Run2017B-17Nov2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2017C-17Nov2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2017D-17Nov2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2017E-17Nov2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2017F-17Nov2017-v1/MINIAOD' 

###################
#config.Data.inputDataset = '/DoubleMuon/Run2017B-17Nov2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2017C-17Nov2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2017D-17Nov2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2017E-17Nov2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2017F-17Nov2017-v1/MINIAOD' 

###################
#config.Data.inputDataset = '/MuonEG/Run2017B-17Nov2017-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2017C-17Nov2017-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2017D-17Nov2017-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2017E-17Nov2017-v1/MINIAOD' 
config.Data.inputDataset = '/MuonEG/Run2017F-17Nov2017-v1/MINIAOD' 



config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/' 
config.Data.splitting = 'LumiBased' 
config.Data.unitsPerJob = 50
config.Data.lumiMask = '../data/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt' ##
#config.Data.totalUnits = 100
config.Data.publication = True 
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/' 
config.Data.outputDatasetTag = '18Feb10th_csvTree_v2' 
config.Data.outLFNDirBase = '/store/user/lwming/'
#config.Data.ignoreLocality = True

config.section_("Site") 
config.Site.storageSite = 'T3_US_FNALLPC'
