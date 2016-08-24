import FWCore.ParameterSet.Config as cms

process = cms.Process("MAOD")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

#### caution: use the correct global tag for MC or Data 
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_ICHEP16JEC_v0'  ##Data

# Load the producer for MVA IDs. Make sure it is also added to the sequence!
process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
#process.options.allowUnscheduled = cms.untracked.bool(True)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

from JetMETCorrections.Configuration.JetCorrectionServices_cff import *

process.ak4PFCHSL1Fastjet = cms.ESProducer(
    'L1FastjetCorrectionESProducer',
    level       = cms.string('L1FastJet'),
    algorithm   = cms.string('AK4PFchs'),
    srcRho      = cms.InputTag( 'fixedGridRhoFastjetAll' )
    )

process.ak4PFchsL2Relative = ak4CaloL2Relative.clone( algorithm = 'AK4PFchs' )
process.ak4PFchsL3Absolute = ak4CaloL3Absolute.clone( algorithm = 'AK4PFchs' )
process.ak4PFchsResidual  = ak4CaloResidual.clone( algorithm = 'AK4PFchs' ) ##data

process.ak4PFchsL1L2L3 = cms.ESProducer("JetCorrectionESChain",
    correctors = cms.vstring(
	'ak4PFCHSL1Fastjet', 
        'ak4PFchsL2Relative', 
        'ak4PFchsL3Absolute',
	'ak4PFchsResidual' ##data
	)
)

######## update JetCollection with the HIP mitigation
##process.load("Configuration.StandardSequences.MagneticField_cff")
##process.load("Configuration.Geometry.GeometryRecoDB_cff")
##
##jetSource = 'slimmedJets'
##jetCorrectionsAK4 = ('AK4PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual'], 'None')
##bTagDiscriminators = ['pfCombinedInclusiveSecondaryVertexV2BJetTags', 'pfCombinedMVAV2BJetTags']
##
##from PhysicsTools.PatAlgos.tools.jetTools import *
##updateJetCollection(
##        process,
##        jetSource = cms.InputTag(jetSource),
##        jetCorrections = jetCorrectionsAK4,
####        btagInfos = bTagInfos, #list of bTagInfos
##        btagDiscriminators = bTagDiscriminators, #list of taggers
##        runIVF = True,
##        hipMitigation = True,
##        explicitJTA = False,
####        postfix = postfix
##)
########

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
#        '/store/data/Run2015D/MuonEG/MINIAOD/05Oct2015-v2/60000/106398EB-C173-E511-8746-0025905A60B0.root'
        '/store/data/Run2016B/MuonEG/MINIAOD/PromptReco-v2/000/273/158/00000/A2E8DB05-371A-E611-9B55-02163E01413C.root'
#        '/store/data/Run2016B/MuonEG/MINIAOD/PromptReco-v1/000/273/017/00000/2E5181C8-5319-E611-88A2-02163E011ADA.root'
            )
)



process.ttHTreeMaker = cms.EDAnalyzer('csvTreeMaker',
#    inSample = cms.int32(-100),##
#    sampleName = cms.string("DoubleEG"),##
#    inSample = cms.int32(-200),##
#    sampleName = cms.string("DoubleMuon"),##
    inSample = cms.int32(-300),##
    sampleName = cms.string("MuonEG"),##

    XS = cms.double(1.),
    nGen = cms.double(1.),
    lumi = cms.double(10000),

    )

process.TFileService = cms.Service("TFileService",
	fileName = cms.string('csv_treeMaker.root')
)

process.p = cms.Path(process.electronMVAValueMapProducer * process.ttHTreeMaker)
