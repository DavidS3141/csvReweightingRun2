from WMCore.Configuration import Configuration 
config = Configuration() 
config.section_("General") 
#config.General.requestName = 'DoubleEG_Run2016B_PromptReco_v2_26thJuly'
#config.General.requestName = 'DoubleEG_Run2016C_PromptReco_v2_26thJuly'
#config.General.requestName = 'DoubleEG_Run2016D_PromptReco_v2_26thJuly'
#config.General.requestName = 'DoubleEG_Run2016E_PromptReco_v2_26thJuly'

#config.General.requestName = 'DoubleMuon_Run2016B_PromptReco_v2_26thJuly'
#config.General.requestName = 'DoubleMuon_Run2016C_PromptReco_v2_26thJuly'
#config.General.requestName = 'DoubleMuon_Run2016D_PromptReco_v2_26thJuly'
#config.General.requestName = 'DoubleMuon_Run2016E_PromptReco_v2_26thJuly'

#config.General.requestName = 'MuonEG_Run2016B_PromptReco_v2_26thJuly'
#config.General.requestName = 'MuonEG_Run2016C_PromptReco_v2_26thJuly'
#config.General.requestName = 'MuonEG_Run2016D_PromptReco_v2_26thJuly'
config.General.requestName = 'MuonEG_Run2016E_PromptReco_v2_26thJuly'

config.section_("JobType") 
config.JobType.pluginName = 'Analysis' 
config.JobType.psetName = 'data_csv_treeMaker_cfg.py'
config.JobType.allowUndistributedCMSSW = False
#config.JobType.inputFiles = ['Fall15_25nsV2_DATA.db','Fall15_25nsV2_MC.db']

config.section_("Data") 
#config.Data.inputDataset = '/DoubleEG/Run2016B-PromptReco-v2/MINIAOD'
#config.Data.inputDataset = '/DoubleEG/Run2016C-PromptReco-v2/MINIAOD'
#config.Data.inputDataset = '/DoubleEG/Run2016D-PromptReco-v2/MINIAOD'
#config.Data.inputDataset = '/DoubleEG/Run2016E-PromptReco-v2/MINIAOD'

#config.Data.inputDataset = '/DoubleMuon/Run2016B-PromptReco-v2/MINIAOD'
#config.Data.inputDataset = '/DoubleMuon/Run2016C-PromptReco-v2/MINIAOD'
#config.Data.inputDataset = '/DoubleMuon/Run2016D-PromptReco-v2/MINIAOD'
#config.Data.inputDataset = '/DoubleMuon/Run2016E-PromptReco-v2/MINIAOD'

#config.Data.inputDataset = '/MuonEG/Run2016B-PromptReco-v2/MINIAOD'
#config.Data.inputDataset = '/MuonEG/Run2016C-PromptReco-v2/MINIAOD'
#config.Data.inputDataset = '/MuonEG/Run2016D-PromptReco-v2/MINIAOD'
config.Data.inputDataset = '/MuonEG/Run2016E-PromptReco-v2/MINIAOD'

config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/' 
config.Data.splitting = 'LumiBased' 
config.Data.unitsPerJob = 20
config.Data.lumiMask = '../data/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt' ##
#config.Data.totalUnits = 100
config.Data.publication = True 
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/' 
config.Data.outputDatasetTag = 'ICHEP_26thJuly_Run2016_csvTree' 
config.Data.outLFNDirBase = '/store/user/lwming/'
config.Data.ignoreLocality = True

config.section_("Site") 
config.Site.storageSite = 'T3_US_FNALLPC'
