from WMCore.Configuration import Configuration 
config = Configuration() 
config.section_("General") 
#config.General.requestName = 'DoubleEG_Run2016B_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleEG_Run2016C_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleEG_Run2016D_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleEG_Run2016E_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleEG_Run2016F_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleEG_Run2016G_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleEG_Run2016H2_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleEG_Run2016H3_03Feb2017_3rdMarch'

###################
#config.General.requestName = 'DoubleMuon_Run2016B_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleMuon_Run2016C_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleMuon_Run2016D_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleMuon_Run2016E_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleMuon_Run2016F_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleMuon_Run2016G_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleMuon_Run2016H2_03Feb2017_3rdMarch'
#config.General.requestName = 'DoubleMuon_Run2016H3_03Feb2017_3rdMarch'

###################
config.General.requestName = 'MuonEG_Run2016B_03Feb2017_3rdMarch'
#config.General.requestName = 'MuonEG_Run2016C_03Feb2017_3rdMarch'
#config.General.requestName = 'MuonEG_Run2016D_03Feb2017_3rdMarch'
#config.General.requestName = 'MuonEG_Run2016E_03Feb2017_3rdMarch'
#config.General.requestName = 'MuonEG_Run2016F_03Feb2017_3rdMarch'
#config.General.requestName = 'MuonEG_Run2016G_03Feb2017_3rdMarch'
#config.General.requestName = 'MuonEG_Run2016H2_03Feb2017_3rdMarch'
#config.General.requestName = 'MuonEG_Run2016H3_03Feb2017_3rdMarch'

config.section_("JobType") 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'data_csv_treeMaker_cfg.py'
config.JobType.allowUndistributedCMSSW = False
#config.JobType.inputFiles = ['Fall15_25nsV2_DATA.db','Fall15_25nsV2_MC.db']

config.section_("Data") 
#config.Data.inputDataset = '/DoubleEG/Run2016B-03Feb2017_ver2-v2/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016C-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016D-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016E-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016F-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016G-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016H-03Feb2017_ver2-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleEG/Run2016H-03Feb2017_ver3-v1/MINIAOD'

###################
#config.Data.inputDataset = '/DoubleMuon/Run2016B-03Feb2017_ver2-v2/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016C-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016D-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016E-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016F-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016G-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016H-03Feb2017_ver2-v1/MINIAOD' 
#config.Data.inputDataset = '/DoubleMuon/Run2016H-03Feb2017_ver3-v1/MINIAOD'

###################
config.Data.inputDataset = '/MuonEG/Run2016B-03Feb2017_ver2-v2/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016C-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016D-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016E-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016F-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016G-03Feb2017-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016H-03Feb2017_ver2-v1/MINIAOD' 
#config.Data.inputDataset = '/MuonEG/Run2016H-03Feb2017_ver3-v1/MINIAOD'



config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/' 
config.Data.splitting = 'LumiBased' 
config.Data.unitsPerJob = 50
config.Data.lumiMask = '../data/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt' ##
#config.Data.totalUnits = 100
config.Data.publication = True 
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/' 
config.Data.outputDatasetTag = '3rdMarch_2017_csvTree' 
config.Data.outLFNDirBase = '/store/user/lwming/'
config.Data.ignoreLocality = True

config.section_("Site") 
config.Site.storageSite = 'T3_US_FNALLPC'
