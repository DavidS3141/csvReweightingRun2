from WMCore.Configuration import Configuration 
config = Configuration() 
config.section_("General") 
#config.General.requestName = 'DoubleEG_Run2016B_23Sep2016_6thJan'
#config.General.requestName = 'DoubleEG_Run2016C_23Sep2016_6thJan'
#config.General.requestName = 'DoubleEG_Run2016D_23Sep2016_6thJan'
#config.General.requestName = 'DoubleEG_Run2016E_23Sep2016_6thJan'
#config.General.requestName = 'DoubleEG_Run2016F_23Sep2016_6thJan'
#config.General.requestName = 'DoubleEG_Run2016G_23Sep2016_6thJan'
#config.General.requestName = 'DoubleEG_Run2016H2_23Sep2016_6thJan'
#config.General.requestName = 'DoubleEG_Run2016H3_23Sep2016_6thJan'

###################
#config.General.requestName = 'DoubleMuon_Run2016B_23Sep2016_6thJan'
#config.General.requestName = 'DoubleMuon_Run2016C_23Sep2016_6thJan'
#config.General.requestName = 'DoubleMuon_Run2016D_23Sep2016_6thJan'
#config.General.requestName = 'DoubleMuon_Run2016E_23Sep2016_6thJan'
#config.General.requestName = 'DoubleMuon_Run2016F_23Sep2016_6thJan'
#config.General.requestName = 'DoubleMuon_Run2016G_23Sep2016_6thJan'
#config.General.requestName = 'DoubleMuon_Run2016H2_23Sep2016_6thJan'
#config.General.requestName = 'DoubleMuon_Run2016H3_23Sep2016_6thJan'

###################
config.General.requestName = 'MuonEG_Run2016B_23Sep2016_6thJan'
#config.General.requestName = 'MuonEG_Run2016C_23Sep2016_6thJan'
#config.General.requestName = 'MuonEG_Run2016D_23Sep2016_6thJan'
#config.General.requestName = 'MuonEG_Run2016E_23Sep2016_6thJan'
#config.General.requestName = 'MuonEG_Run2016F_23Sep2016_6thJan'
#config.General.requestName = 'MuonEG_Run2016G_23Sep2016_6thJan'
#config.General.requestName = 'MuonEG_Run2016H2_23Sep2016_6thJan'
#config.General.requestName = 'MuonEG_Run2016H3_23Sep2016_6thJan'

config.section_("JobType") 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'data_csv_treeMaker_cfg.py'
config.JobType.allowUndistributedCMSSW = False
#config.JobType.inputFiles = ['Fall15_25nsV2_DATA.db','Fall15_25nsV2_MC.db']

config.section_("Data") 
#config.Data.inputDataset = '/DoubleEG/Run2016B-23Sep2016-v3/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016C-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016D-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016E-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016F-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016G-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016H-PromptReco-v2/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016H-PromptReco-v3/MINIAOD'

###################
#config.Data.inputDataset = '/DoubleMuon/Run2016B-23Sep2016-v3/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016C-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016D-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016E-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016F-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016G-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016H-PromptReco-v2/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016H-PromptReco-v3/MINIAOD'

###################
config.Data.inputDataset = '/MuonEG/Run2016B-23Sep2016-v3/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016C-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016D-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016E-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016F-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016G-23Sep2016-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016H-PromptReco-v2/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016H-PromptReco-v3/MINIAOD'


config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/' 
config.Data.splitting = 'LumiBased' 
config.Data.unitsPerJob = 20
config.Data.lumiMask = '../data/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt' ##
#config.Data.totalUnits = 100
config.Data.publication = True 
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/' 
config.Data.outputDatasetTag = '6thJan_2017_csvTree' 
config.Data.outLFNDirBase = '/store/user/lwming/'
config.Data.ignoreLocality = True

config.section_("Site") 
config.Site.storageSite = 'T3_US_FNALLPC'
