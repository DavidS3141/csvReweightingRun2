// -*- C++ -*-
//
// Package:    csvReweightingRun2/csvTreeMaker
// Class:      csvTreeMaker
// 
/**\class csvTreeMaker csvTreeMaker.cc csvReweightingRun2/csvTreeMaker/plugins/csvTreeMaker.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Wuming Luo
//         Created:  Thu, 24 Sep 2015 15:46:22 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Lepton.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/strbitset.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/JetReco/interface/PileupJetIdentifier.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "MiniAOD/MiniAODHelper/interface/MiniAODHelper.h"
#include "csvReweightingRun2/csvTreeMaker/interface/csvEventVars.h"

// #include "EgammaAnalysis/ElectronTools/interface/EGammaMvaEleEstimatorCSA14.h"



//
// class declaration
//

class csvTreeMaker : public edm::EDAnalyzer {
   public:
      explicit csvTreeMaker(const edm::ParameterSet&);
      ~csvTreeMaker();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      virtual void beginRun(edm::Run const& iRun,edm::EventSetup const& iSetup) override;
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  
      // ----------member data ---------------------------
  
  //----------
  edm::EDGetTokenT<reco::GenJetCollection> genJetsToken;

  edm::EDGetTokenT <edm::TriggerResults> triggerResultsToken;
  edm::EDGetTokenT <edm::TriggerResults> filterResultsToken;

  // new MVAelectron
  // edm::EDGetTokenT< edm::View<pat::Electron> > EDMElectronsToken;
  // MVA values and categories
  // edm::EDGetTokenT<edm::ValueMap<float> > EDMeleMVAvaluesToken;
  // edm::EDGetTokenT<edm::ValueMap<int> > EDMeleMVAcategoriesToken;

  edm::EDGetTokenT <reco::VertexCollection> vertexToken;
  edm::EDGetTokenT <pat::ElectronCollection> electronToken;
  edm::EDGetTokenT <pat::MuonCollection> muonToken;
  edm::EDGetTokenT <pat::JetCollection> jetToken;
  // edm::EDGetTokenT <edm::View<pat::Jet > > jetToken;

  edm::EDGetTokenT <pat::METCollection> metToken;
  // edm::EDGetTokenT <pat::METCollection> metNoHFToken;

  edm::EDGetTokenT <pat::PackedCandidateCollection> packedpfToken;

  edm::EDGetTokenT <reco::BeamSpot> beamspotToken;
  // edm::EDGetTokenT <reco::ConversionCollection> EDMConversionCollectionToken;
  edm::EDGetTokenT <double> rhoToken;
  edm::EDGetTokenT <reco::GenParticleCollection> mcparicleToken;
  edm::EDGetTokenT <std::vector< PileupSummaryInfo > > puInfoToken;

  edm::EDGetTokenT <GenEventInfoProduct> genInfoProductToken;


  HLTConfigProvider hlt_config_;

  bool verbose_;
  bool isData;
  const int insample_;
  const std::string sampleName_;
  const double xSec_;
  const double nGen_;
  const double intLumi_;

  std::string hltTag;
  std::string filterTag;


  int nevents;
  double nevents_wgt;

  int nevents_clean;
  double nevents_clean_wgt;


  TTree *worldTree;
  EventVars *eve; 
  TH1F* h_nEvents;
  TH1F* h_numPV;

  // EGammaMvaEleEstimatorCSA14* myMVATrig;
 
  MiniAODHelper miniAODhelper;


};

//
// constants, enums and typedefs
//
typedef std::vector<std::vector<double> >      vvdouble;
typedef std::vector<std::vector<std::string> > vvstring;
typedef std::vector<double>                    vdouble;
typedef std::vector<string>                    vstring;
typedef std::vector<bool>                      vbool;
typedef std::vector<int>                       vint;
typedef std::vector< TLorentzVector >          vecTLorentzVector;

//
// static data member definitions
//

//
// constructors and destructor
//
csvTreeMaker::csvTreeMaker(const edm::ParameterSet& iConfig):
  insample_(iConfig.getParameter<int>("inSample")),
  sampleName_(iConfig.getParameter<std::string>("sampleName")),
  xSec_(iConfig.getParameter<double>("XS")),
  nGen_(iConfig.getParameter<double>("nGen")),
  intLumi_(iConfig.getParameter<double>("lumi"))
{
   //now do what ever initialization is needed
  verbose_ = false;

  hltTag = "HLT";
  filterTag = "PAT";

  triggerResultsToken = consumes <edm::TriggerResults> (edm::InputTag(std::string("TriggerResults"), std::string(""), hltTag));
  filterResultsToken = consumes <edm::TriggerResults> (edm::InputTag(std::string("TriggerResults"), std::string(""), filterTag));

  // new MVAelectron
  // EDMElectronsToken = consumes< edm::View<pat::Electron> >(edm::InputTag("slimmedElectrons","",""));
  // EDMeleMVAvaluesToken           = consumes<edm::ValueMap<float> >(edm::InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Values",""));
  // EDMeleMVAcategoriesToken       = consumes<edm::ValueMap<int> >(edm::InputTag("electronMVAValueMapProducer","ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Categories",""));

  vertexToken = consumes <reco::VertexCollection> (edm::InputTag(std::string("offlineSlimmedPrimaryVertices")));
  electronToken = consumes <pat::ElectronCollection> (edm::InputTag(std::string("slimmedElectrons")));
  muonToken = consumes <pat::MuonCollection> (edm::InputTag(std::string("slimmedMuons")));
  // jetToken = consumes <pat::JetCollection> (edm::InputTag(std::string("selectedUpdatedPatJets")));
  jetToken = consumes <pat::JetCollection> (edm::InputTag(std::string("slimmedJets")));
  // jetToken = consumes <edm::View<pat::Jet > > (edm::InputTag(std::string("slimmedJets")));
  metToken = consumes <pat::METCollection> (edm::InputTag(std::string("slimmedMETs")));
  // metNoHFToken = consumes <pat::METCollection> (edm::InputTag(std::string("slimmedMETsNoHF")));


  packedpfToken = consumes <pat::PackedCandidateCollection> (edm::InputTag(std::string("packedPFCandidates")));

  beamspotToken = consumes <reco::BeamSpot> (edm::InputTag(std::string("offlineBeamSpot")));
  //  rhoToken = consumes <double> (edm::InputTag(std::string("fixedGridRhoFastjetCentral")));
  rhoToken = consumes <double> (edm::InputTag(std::string("fixedGridRhoFastjetAll")));
  mcparicleToken = consumes <reco::GenParticleCollection> (edm::InputTag(std::string("prunedGenParticles")));
  puInfoToken = consumes <std::vector< PileupSummaryInfo > > (edm::InputTag(std::string("slimmedAddPileupInfo"))); //
  // EDMConversionCollectionToken = consumes <reco::ConversionCollection > (edm::InputTag("reducedEgamma","reducedConversions",""));
  genInfoProductToken = consumes <GenEventInfoProduct> (edm::InputTag(std::string("generator")));
  
  genJetsToken = consumes <reco::GenJetCollection> (edm::InputTag(std::string("slimmedGenJets")) );


  ////
  edm::Service<TFileService> fs_;
  worldTree = fs_->make<TTree>("worldTree", "worldTree");
  eve=0; 
  worldTree->Branch("eve.", "EventVars", &eve, 8000, 1);
  h_nEvents = fs_->make<TH1F>("nEvents", "nEvents", 3, 0, 3);
  h_nEvents->GetXaxis()->SetBinLabel(1,"All");
  h_nEvents->GetXaxis()->SetBinLabel(2,"Pos");
  h_nEvents->GetXaxis()->SetBinLabel(3,"Neg");

  h_numPV = fs_->make<TH1F>("numPVs", "numPVs", 100, 0, 100);

  nevents=0;
  nevents_wgt=0;

  nevents_clean=0;
  nevents_clean_wgt=0;

  /// sample specific
  std::string era = "2012_53x";
  // insample_ = 2500;
  // sampleName_ = "TTJets";
  // xSec_ = 689.1;
  // nGen_ = 25474122;
  // intLumi_ = 20000;

  analysisType::analysisType iAnalysisType = analysisType::LJ;
  isData = ( insample_<0 );

  miniAODhelper.SetUp(era, insample_, iAnalysisType, isData);

  //  miniAODhelper.SetJetCorrectorUncertainty();
  // miniAODhelper.SetUpElectronMVA("MiniAOD/MiniAODHelper/data/ElectronMVA/EIDmva_EB1_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml","MiniAOD/MiniAODHelper/data/ElectronMVA/EIDmva_EB2_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml","MiniAOD/MiniAODHelper/data/ElectronMVA/EIDmva_EE_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml");

}


csvTreeMaker::~csvTreeMaker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called for each event  ------------
void
csvTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   
   eve->initialize();


   double minTightLeptonPt = 25.;// increased due to trigger Pt threshold
  double looseLeptonPt = 15.;//20.;

  edm::Handle<reco::VertexCollection> vtxHandle;
  iEvent.getByToken(vertexToken,vtxHandle);
  reco::VertexCollection vtxs = *vtxHandle;

  /// old electrons
  edm::Handle<pat::ElectronCollection> electrons;
  iEvent.getByToken(electronToken,electrons);
  //// MVAelectrons
  // edm::Handle< edm::View<pat::Electron> > h_electrons;
  // iEvent.getByToken( EDMElectronsToken,h_electrons );
  // // add electron mva info to electrons
  // edm::Handle<edm::ValueMap<float> > h_mvaValues; 
  // edm::Handle<edm::ValueMap<int> > h_mvaCategories;
  // iEvent.getByToken(EDMeleMVAvaluesToken,h_mvaValues);
  // iEvent.getByToken(EDMeleMVAcategoriesToken,h_mvaCategories);  
  // std::vector<pat::Electron> electrons = miniAODhelper.GetElectronsWithMVAid(h_electrons,h_mvaValues,h_mvaCategories);

  ////
  edm::Handle<pat::MuonCollection> muons;
  iEvent.getByToken(muonToken,muons);

  edm::Handle<pat::JetCollection> pfjets;
  iEvent.getByToken(jetToken,pfjets);

  //////
  /// updated PU jet ID if any
  ////
  // edm::Handle<edm::View<pat::Jet > > pfjets_noPUID;
  // iEvent.getByToken(jetToken,pfjets_noPUID);

  // Handle<ValueMap<float> > puJetIdMVA;
  // iEvent.getByLabel("pileupJetIdUpdated:fullDiscriminant",puJetIdMVA); ///pileupJetIdUpdated

  // Handle<ValueMap<int> > puJetIdFlag;
  // iEvent.getByLabel("pileupJetIdUpdated:fullId",puJetIdFlag); ///

  //----------
  edm::Handle<pat::METCollection> pfmet;
  iEvent.getByToken(metToken,pfmet);

  // edm::Handle<pat::METCollection> pfmetNoHF;
  // iEvent.getByToken(metNoHFToken,pfmetNoHF);

  edm::Handle<pat::PackedCandidateCollection> packedPFcands;
  iEvent.getByToken(packedpfToken,packedPFcands);


  edm::Handle<reco::BeamSpot> bsHandle;
  iEvent.getByToken(beamspotToken,bsHandle);

  // edm::Handle<reco::ConversionCollection> h_conversioncollection;
  // iEvent.getByToken( EDMConversionCollectionToken,h_conversioncollection );

  edm::Handle<reco::GenParticleCollection> mcparticles;
  if ( !isData ) iEvent.getByToken(mcparicleToken,mcparticles);

  edm::Handle<reco::GenJetCollection> genjetCollection;
  if ( !isData ) iEvent.getByToken( genJetsToken , genjetCollection );

  edm::Handle<double> rhoHandle;
  iEvent.getByToken(rhoToken,rhoHandle);
  ////------- set up rho for lepton effArea Isolation correction
  double rho_event = ( (rhoHandle.isValid()) ) ? *rhoHandle : -99;
  miniAODhelper.SetRho(rho_event);

  edm::Handle<std::vector< PileupSummaryInfo > > PupInfo;
  if ( !isData ) iEvent.getByToken(puInfoToken,PupInfo);

  edm::Handle<GenEventInfoProduct> GenEventInfoHandle;
  if ( !isData ) iEvent.getByToken(genInfoProductToken,GenEventInfoHandle);

  double GenEventInfoWeight = 1;
  if ( !isData ) GenEventInfoWeight = GenEventInfoHandle.product()->weight();
  h_nEvents->Fill(0.5);
  if (GenEventInfoWeight > 0)   h_nEvents->Fill(1.5);
  else   h_nEvents->Fill(2.5);

  edm::Handle<edm::TriggerResults> triggerResults;
  iEvent.getByToken(triggerResultsToken, triggerResults);

  // bool passDoubleElectronTrigger = false;
  bool passDoubleMuonTrigger = false;
  // bool passElectronMuonTrigger = false;

  bool passDoubleElectronTriggerNew = false;
  bool passDoubleMuonTriggerNew = false;
  bool passElectronMuonTriggerNew = false;

  // vstring triggerPaths;
  // vint triggerAcceps;
  if( triggerResults.isValid() ){
    std::vector<std::string> triggerNames = hlt_config_.triggerNames();

    for( unsigned int iPath=0; iPath<triggerNames.size(); iPath++ ){
      std::string pathName = triggerNames[iPath];
      unsigned int hltIndex = hlt_config_.triggerIndex(pathName);

      if( hltIndex >= triggerResults->size() ) continue;

      int accept = triggerResults->accept(hltIndex);
      /// saving trigger infos in to vectors
      // triggerPaths.push_back(pathName);
      // triggerAcceps.push_back(accept);

      if( accept ){
	// if( pathName.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") !=std::string::npos ) passDoubleElectronTrigger = true;

	if( pathName.find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v") !=std::string::npos ) passDoubleMuonTrigger = true;

	// if( pathName.find("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") !=std::string::npos ||
	//     pathName.find("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v") !=std::string::npos ) passElectronMuonTrigger = true;

	///1E34 menu
	if( pathName.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") !=std::string::npos ||
	    pathName.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v") !=std::string::npos ) passDoubleElectronTriggerNew = true;

	if( pathName.find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v") !=std::string::npos ||
	    pathName.find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v") !=std::string::npos ) passDoubleMuonTriggerNew = true;

	if( pathName.find("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") !=std::string::npos ||
	    pathName.find("HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v") !=std::string::npos ||
	    pathName.find("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") !=std::string::npos ||
	    pathName.find("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v") !=std::string::npos ) passElectronMuonTriggerNew = true;
	
      }
    }
  }

  // eve->TriggerPaths_ = triggerPaths;
  // eve->TriggerAcceps_ = triggerAcceps;
  // eve->passDoubleElectronTrigger_ = ( passDoubleElectronTrigger ) ? 1 : 0;
  eve->passDoubleMuonTrigger_     = ( passDoubleMuonTrigger ) ? 1 : 0;
  // eve->passElectronMuonTrigger_   = ( passElectronMuonTrigger ) ? 1 : 0;

  eve->passDoubleElectronTriggerNew_ = ( passDoubleElectronTriggerNew ) ? 1 : 0;
  eve->passDoubleMuonTriggerNew_     = ( passDoubleMuonTriggerNew ) ? 1 : 0;
  eve->passElectronMuonTriggerNew_   = ( passElectronMuonTriggerNew ) ? 1 : 0;


  ////-------

  math::XYZPoint beamSpotPosition;
  beamSpotPosition.SetCoordinates(0,0,0);
  double BSx=0,BSy=0,BSz=0;
  if( (bsHandle.isValid()) ){
    reco::BeamSpot bs = *bsHandle;
    BSx = bs.x0();
    BSy = bs.y0();
    BSz = bs.z0();
    beamSpotPosition = bsHandle->position();
  }


  if( verbose_ ) printf("\t BeamSpot: x = %.2f,\t y = %.2f,\t z = %.2f \n", BSx, BSy, BSz );

  int numpv=0; int iPV=0;
  bool firstGoodPV = false;
  reco::Vertex vertex;
  if( vtxHandle.isValid() ){
    for( reco::VertexCollection::const_iterator vtx = vtxs.begin(); vtx!=vtxs.end(); ++vtx ){
      
      iPV++;
      bool isGood = ( !(vtx->isFake()) &&
		      (vtx->ndof() >= 4.0) &&
		      (abs(vtx->z()) <= 24.0) &&
		      (abs(vtx->position().Rho()) <= 2.0) 
		      );
		      
      if( !isGood ) continue;

      if( iPV==1 ){
	firstGoodPV = true;
	vertex = (*vtx);
      }

      numpv++;
    }
  }

  eve->GoodFirstPV_=firstGoodPV;

  // if( !firstGoodPV ){
  //   std::cout << "ERROR!! First PV does not pass requirements! Skipping event" << std::endl;
  //   return;
  // }

  if( numpv>0 ) miniAODhelper.SetVertex(vertex);

  eve->numPVs_ = numpv;

  if (GenEventInfoWeight > 0)   h_numPV->Fill(numpv, 1);
  else   h_numPV->Fill(numpv, -1);


  double numTruePV = -1;
  double numGenPV = -1;
  if( (PupInfo.isValid()) ){
    for( std::vector<PileupSummaryInfo>::const_iterator PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI ) {
      int BX = PVI->getBunchCrossing();
      if( BX==0 ){
	numTruePV = PVI->getTrueNumInteractions();
	numGenPV  = PVI->getPU_NumInteractions();
      }
    }
  }

  eve->numTruePV_ = numTruePV;
  eve->numGenPV_ = numGenPV;



  double wgt_lumi = 1.;
  if( insample_>=0 ) wgt_lumi = xSec_ * intLumi_ *1./ nGen_;

  // Increment event counter
  nevents++;
  nevents_wgt+=wgt_lumi;

  // Number of events after filtering, triggers, etc. (now nothing)
  nevents_clean++;
  nevents_clean_wgt+=wgt_lumi;

  int run  = iEvent.id().run();
  int lumi = iEvent.luminosityBlock();
  long evt = iEvent.id().event();

  eve->run_ = run;
  eve->lumi_ = lumi;
  eve->evt_ = evt;

  const JetCorrector* corrector = JetCorrector::getJetCorrector( "ak4PFchsL1L2L3", iSetup );   //Get the jet corrector from the event setup

  miniAODhelper.SetJetCorrector(corrector);


  /////////
  ///
  /// Electrons
  ///
  ////////
  // miniAODhelper.SetElectronMVAinfo(h_conversioncollection, bsHandle);
  // /// Ele MVA ID 
  // std::vector<pat::Electron> selectedElectrons_tight = miniAODhelper.GetSelectedElectrons( electrons, minTightLeptonPt, electronID::electronEndOf15MVA80iso0p15, 2.4 );
  // std::vector<pat::Electron> selectedElectrons_loose = miniAODhelper.GetSelectedElectrons( electrons, looseLeptonPt, electronID::electronEndOf15MVA80iso0p15, 2.4 );

  /// Ele Cut Based ID 
  std::vector<pat::Electron> selectedElectrons_tight = miniAODhelper.GetSelectedElectrons( *electrons, minTightLeptonPt, electronID::electron80XCutBasedM, 2.4 );
  std::vector<pat::Electron> selectedElectrons_loose = miniAODhelper.GetSelectedElectrons( *electrons, looseLeptonPt, electronID::electron80XCutBasedM, 2.4 );

  int numTightElectrons = int(selectedElectrons_tight.size());
  int numLooseElectrons = int(selectedElectrons_loose.size());// - numTightElectrons;


  /////////
  ///
  /// Muons
  ///
  ////////
  std::vector<pat::Muon> selectedMuons_tight = miniAODhelper.GetSelectedMuons( *muons, minTightLeptonPt, muonID::muonTight, coneSize::R04, corrType::deltaBeta, 2.4 );
  std::vector<pat::Muon> selectedMuons_loose = miniAODhelper.GetSelectedMuons( *muons, looseLeptonPt, muonID::muonTight, coneSize::R04, corrType::deltaBeta, 2.4 );
  // std::vector<pat::Muon> selectedMuons_loose = miniAODhelper.GetSelectedMuons( *muons, looseLeptonPt, muonID::muonLoose );

  int numTightMuons = int(selectedMuons_tight.size());
  int numLooseMuons = int(selectedMuons_loose.size());// - numTightMuons;
  
  eve->numTightMuons_ = numTightMuons;
  eve->numLooseMuons_ = numLooseMuons;
  eve->numTightElectrons_ = numTightElectrons;
  eve->numLooseElectrons_ = numLooseElectrons;


  ///////////// 
  // --- Pass exactly two leptons cuts
  // ---------- the lepton selections might change in the future -------
  // 
  /////////////

  bool passDIL = ( (numTightMuons+numTightElectrons)>=1 &&
  		   (numLooseMuons+numLooseElectrons)==2 );
  // bool passDIL = (numTightMuons + numTightElectrons) == 2; //old selections

  // Event must be DIL, as requested
  if( ! passDIL ) return;


  eve->PassDIL_ = ( passDIL ) ? 1 : 0;

  //// DIL subcategories: ee, mumu, emu
  int TwoMuon = 0, TwoElectron = 0, MuonElectron = 0; 
  if ( numTightMuons>=1 && numLooseMuons==2 && numLooseElectrons==0 ) TwoMuon = 1;
  if ( numTightElectrons>=1 && numLooseElectrons==2 && numLooseMuons==0 ) TwoElectron = 1;
  if ( (numTightMuons + numTightElectrons)>=1 && numLooseMuons==1 && numLooseElectrons==1 ) MuonElectron = 1;
  // if ( numTightMuons==2 && numTightElectrons==0 ) TwoMuon = 1; //old selections
  // if ( numTightElectrons==2 && numTightMuons==0 ) TwoElectron = 1;
  // if ( numTightMuons==1 && numTightElectrons==1 ) MuonElectron = 1;

  eve->TwoMuon_ = TwoMuon;
  eve->TwoElectron_ = TwoElectron;
  eve->MuonElectron_ = MuonElectron;

  
  ///////////
  /////  extra lepton variables: opposite charge, dR_leplep and mass_leplep
  //////////
  vint lepton_trkCharge, lepton_isMuon;//, lepton_isTight, lepton_isLoose;

  std::vector<TLorentzVector> vec_TLV_lep;
  TLorentzVector sum_lepton_vect;
  sum_lepton_vect.SetPxPyPzE(0., 0., 0., 0.);

  ///// --- loop over muons
  for( std::vector<pat::Muon>::const_iterator iMu = selectedMuons_loose.begin(); iMu != selectedMuons_loose.end(); iMu++ ){ 
    // Get muon 4Vector and add to vecTLorentzVector for muons
    TLorentzVector leptonP4;  
    leptonP4.SetPxPyPzE(iMu->px(),iMu->py(),iMu->pz(),iMu->energy());
    vec_TLV_lep.push_back(leptonP4);

    sum_lepton_vect += leptonP4;

    lepton_isMuon.push_back(1);

    int trkCharge = -99;
    if( iMu->muonBestTrack().isAvailable() ) trkCharge = iMu->muonBestTrack()->charge();
    lepton_trkCharge.push_back(trkCharge);

  }

  ///// --- loop over electrons
  for( std::vector<pat::Electron>::const_iterator iEle = selectedElectrons_loose.begin(); iEle != selectedElectrons_loose.end(); iEle++ ){ 
    // Get electron 4Vector and add to vecTLorentzVector for electrons
    TLorentzVector leptonP4;	  
    leptonP4.SetPxPyPzE(iEle->px(),iEle->py(),iEle->pz(),iEle->energy());
    vec_TLV_lep.push_back(leptonP4);

    sum_lepton_vect += leptonP4;

    lepton_isMuon.push_back(0);

    int trkCharge = -99;
    if( iEle->gsfTrack().isAvailable() ) trkCharge = iEle->gsfTrack()->charge();
    lepton_trkCharge.push_back(trkCharge);

  }

  eve->lepton_trkCharge_        = lepton_trkCharge;
  eve->lepton_vect_TLV_        = vec_TLV_lep; /// non existing yet
  eve->lepton_isMuon_ = lepton_isMuon;

  /// DIL specific: opposite charged lepton pair
  int oppositeLepCharge = -9;
  if( lepton_trkCharge.size()==2 ){
    int chg0 = lepton_trkCharge[0];
    int chg1 = lepton_trkCharge[1];

    if( (chg0 * chg1)==-1 )     oppositeLepCharge = 1;
    else if( (chg0 * chg1)==1 ) oppositeLepCharge = 0;
    else if( chg0==-99 )        oppositeLepCharge = -1;
    else if( chg1==-99 )        oppositeLepCharge = -2;
    else                        oppositeLepCharge = -3;
  }
  eve->oppositeLepCharge_ = oppositeLepCharge;


  /// DIL specific: dR and mass of the dilepton pair
  double mass_leplep = -99;
  if( vec_TLV_lep.size()==2 ){
    mass_leplep = sum_lepton_vect.M();
    eve->mass_leplep_ = mass_leplep;
    eve->dR_leplep_ = vec_TLV_lep[0].DeltaR(vec_TLV_lep[1]);
  }


  ///////////
  ///
  /// various weights: PU, lepton SF, trigger SF, etc
  ///
  //////////
  // PU scale factor
  double wgtPU = 1;
  eve->wgt_pu_     = wgtPU;

  // Lepton scale factor
  float leptonSF = 1.0;
  eve->wgt_lepSF_  = leptonSF;

  eve->wgt_lumi_  = intLumi_;
  eve->wgt_xs_    = xSec_;//mySample.xSec;
  eve->wgt_nGen_  = nGen_;//mySample.nGen;

  eve->wgt_generator_ = GenEventInfoWeight;


  /////////
  ///
  /// Jets
  ///
  ////////
  // Do jets stuff

  /// checking PU jet ID
  // for ( unsigned int i=0; i<pfjets->size(); ++i ) {
  //   const pat::Jet & patjet = pfjets->at(i);

  //   if(!patjet.hasUserFloat("pileupJetId:fullDiscriminant") || !patjet.hasUserInt("pileupJetId:fullId")) {
  // 	std::cout << "not mvaValue or idflag info for the jet ---" << std::endl;
  // 	continue;
  //   }

  //   float mvaValue   = patjet.userFloat("pileupJetId:fullDiscriminant");
  //   int    idflag = patjet.userInt("pileupJetId:fullId");

  //   cout << "jet " << i << " pt " << patjet.pt() << " eta " << patjet.eta() << " PU JetID MVA " << mvaValue << " PU JetID flag " << idflag;
  //   if( PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kLoose ) ){
  //   	cout << " pass loose wp";
  //   }
  //   if( PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kMedium ) ){
  //   	  cout << " pass medium wp";
  //   }
  //   if( PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kTight ) ){
  //     cout << " pass tight wp";
  //   }
  //   cout << endl;

  // }
  // cout << endl;

  //-------
  std::vector<pat::Jet> pfJets_ID = miniAODhelper.GetSelectedJets(*pfjets, 0., 999, jetID::jetLoose, '-');
  /// lepton-jet overlap cleanning, might change the input lepton collections in the future
  std::vector<pat::Jet> pfJets_ID_clean = miniAODhelper.GetDeltaRCleanedJets( pfJets_ID, selectedMuons_loose, selectedElectrons_loose, 0.4);

  std::vector<pat::Jet> unCorrectedJets = miniAODhelper.GetUncorrectedJets(pfJets_ID_clean);
  std::vector<pat::Jet> correctedJets_noSys = miniAODhelper.GetCorrectedJets(unCorrectedJets, iEvent, iSetup, genjetCollection);
  /// jet pt threshold, 20GeV for now
  std::vector<pat::Jet> selectedJets_noSys_unsorted = miniAODhelper.GetSelectedJets(correctedJets_noSys, 20., 2.5, jetID::none, '-' );
  std::vector<pat::Jet> selectedJets_tag_noSys_unsorted = miniAODhelper.GetSelectedJets( correctedJets_noSys, 20., 2.5, jetID::none, 'M' );

  // std::vector<pat::Jet> selectedJets_loose_noSys_unsorted = miniAODhelper.GetSelectedJets(correctedJets_noSys, 20., 3.0, jetID::none, '-' );
  // std::vector<pat::Jet> selectedJets_loose_tag_noSys_unsorted = miniAODhelper.GetSelectedJets( correctedJets_noSys, 20., 3.0, jetID::none, 'M' );


  // bool for passing jet/tag cuts, first is for tree filling, second is for event counting
  // bool hasNumJetNumTag = false;

  double totalWgt = leptonSF*wgtPU; //wgt_lumi;

  ///////
  // Loop over systematics: nominal, JESUp, JESDown
  ///////
  bool passingTwoJet = false;
  for( int iSys=0; iSys<rNumSys; iSys++ ){

    //no sys for data, 
    if( isData ){
      if( iSys!=0 ) continue;
    } 

    eve->wgt_[iSys] = totalWgt;

    sysType::sysType iSysType = sysType::NA;
    switch( iSys ){
    case 1 : iSysType = sysType::JESup;    break;
    case 2 : iSysType = sysType::JESdown;  break;

    case 3 : iSysType = sysType::JESFlavorQCDup;    break;
    case 4 : iSysType = sysType::JESFlavorQCDdown;  break;

    case 5 : iSysType = sysType::JESSinglePionHCALup;    break;
    case 6 : iSysType = sysType::JESSinglePionHCALdown;  break;

    case 7 : iSysType = sysType::JESAbsoluteScaleup;    break;
    case 8 : iSysType = sysType::JESAbsoluteScaledown;  break;

    case 9 : iSysType = sysType::JESAbsoluteMPFBiasup;    break;
    case 10 : iSysType = sysType::JESAbsoluteMPFBiasdown;  break;

    case 11 : iSysType = sysType::JESPileUpPtRefup;    break;
    case 12 : iSysType = sysType::JESPileUpPtRefdown;  break;
      ////////
    case 13 : iSysType = sysType::JESSinglePionECALup;    break;
    case 14 : iSysType = sysType::JESSinglePionECALdown;  break;

    case 15 : iSysType = sysType::JESPileUpPtBBup;    break;
    case 16 : iSysType = sysType::JESPileUpPtBBdown;  break;

    case 17 : iSysType = sysType::JESPileUpPtEC1up;    break;
    case 18 : iSysType = sysType::JESPileUpPtEC1down;  break;

    case 19 : iSysType = sysType::JESPileUpDataMCup;    break;
    case 20 : iSysType = sysType::JESPileUpDataMCdown;  break;

    case 21 : iSysType = sysType::JESRelativeFSRup;    break;
    case 22 : iSysType = sysType::JESRelativeFSRdown;  break;

    case 23 : iSysType = sysType::JESTimePtEtaup;    break;
    case 24 : iSysType = sysType::JESTimePtEtadown;  break;

    default: iSysType = sysType::NA;       break;
    }


    // nJets/Tags
    // THESE MUST BE INITIALIZED TO -1
    eve->numJets_float_[iSys] = -1;
    eve->numTags_float_[iSys] = -1;


    /////////
    ///
    /// Pfjets
    ///
    ////////

    std::vector<pat::Jet> correctedJets = ( iSys==0 ) ? correctedJets_noSys : miniAODhelper.GetCorrectedJets(unCorrectedJets, iEvent, iSetup, genjetCollection, iSysType);
    std::vector<pat::Jet> selectedJets_unsorted = ( iSys==0 ) ? selectedJets_noSys_unsorted : miniAODhelper.GetSelectedJets(correctedJets, 20., 2.5, jetID::none, '-' );
    std::vector<pat::Jet> selectedJets_tag_unsorted = ( iSys==0 ) ? selectedJets_tag_noSys_unsorted : miniAODhelper.GetSelectedJets( correctedJets, 20., 2.5, jetID::none, 'M' );

    // Sort jet collections by pT
    std::vector<pat::Jet> selectedJets       = miniAODhelper.GetSortedByPt( selectedJets_unsorted );
    std::vector<pat::Jet> selectedJets_tag   = miniAODhelper.GetSortedByPt( selectedJets_tag_unsorted );

    // Get numJets, numTags
    int numJet = int( selectedJets.size() );
    int numTag = int( selectedJets_tag.size() );

    if (numJet < 2) continue; //return;    
    passingTwoJet = true;
    // Get Corrected MET (propagating JEC and JER)
    std::vector<pat::Jet> oldJetsForMET = miniAODhelper.GetSelectedJets(*pfjets, 0., 999, jetID::jetMETcorrection, '-' );
    std::vector<pat::Jet> oldJetsForMET_uncorr = miniAODhelper.GetUncorrectedJets(oldJetsForMET);
    std::vector<pat::Jet> newJetsForMET = miniAODhelper.GetCorrectedJets(oldJetsForMET_uncorr, iEvent, iSetup, genjetCollection, iSysType);
    std::vector<pat::MET> newMETs = miniAODhelper.CorrectMET(oldJetsForMET, newJetsForMET, *pfmet);
    // std::vector<pat::MET> newMETs_NoHF = miniAODhelper.CorrectMET(oldJetsForMET, newJetsForMET, *pfmetNoHF);

    pat::MET correctedMET = newMETs.at(0); 
    // TLorentzVector metV(correctedMET.px(),correctedMET.py(),0.0,correctedMET.pt());
    // pat::MET correctedMET_NoHF = newMETs_NoHF.at(0); 

       
    // Initialize event vars, selected jets
    double mht_px = 0.;
    double mht_py = 0.;


    vecTLorentzVector jetV;
    std::vector<double> csvV;
    std::vector<double> cMVAV;
    std::vector<double> jet_ptV; /// non existing
    std::vector<double> jet_etaV;
    vint jet_flavour_vect;
    vint jet_partonflavour_vect;

    vdouble jet_PUID_mvaV;
    vint jet_PUID_flagV;
    vint jet_PUID_passWPLooseV;

    // int jeti = 0;
    // Loop over selected jets
    for( std::vector<pat::Jet>::const_iterator iJet = selectedJets.begin(); iJet != selectedJets.end(); iJet++ ){ 

      jet_ptV.push_back(iJet->pt());
      jet_etaV.push_back(iJet->eta());
      jet_partonflavour_vect.push_back(iJet->partonFlavour());
      jet_flavour_vect.push_back(iJet->hadronFlavour());

      // Get jet 4Vector and add to vecTLorentzVector for jets
      TLorentzVector jet0p4;	  
      jet0p4.SetPxPyPzE(iJet->px(),iJet->py(),iJet->pz(),iJet->energy());
      jetV.push_back(jet0p4);

      // MHT
      mht_px += - iJet->px();
      mht_py += - iJet->py();

      //      double myCSV = miniAODhelper.GetJetCSV(*iJet, "pfCombinedInclusiveSecondaryVertexV2BJetTags");
      double myCSV = iJet->bDiscriminator("pfDeepCSVJetTags:probb") + iJet->bDiscriminator("pfDeepCSVJetTags:probbb");
      csvV.push_back(myCSV);

      //      double mycMVA = miniAODhelper.GetJetCSV(*iJet, "pfCombinedMVABJetTags");
      double mycMVA = iJet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
      cMVAV.push_back(mycMVA);
    
      //PU jet ID
      if(!iJet->hasUserFloat("pileupJetId:fullDiscriminant") || !iJet->hasUserInt("pileupJetId:fullId")) {
	std::cout << "no mvaValue or idflag info for the jet ---" << std::endl;
      }

      float mvaValue   = iJet->userFloat("pileupJetId:fullDiscriminant");
      int    idflag = iJet->userInt("pileupJetId:fullId");

      // cout << "Sys " << iSys << ": jet " << jeti << " pt " << iJet->pt() << " eta " << iJet->eta() << " PU JetID MVA " << mvaValue << " PU JetID flag " << idflag << endl;;
      // jeti++;
      bool passPUIDLoose = PileupJetIdentifier::passJetId( idflag, PileupJetIdentifier::kLoose ); 


      jet_PUID_mvaV.push_back(mvaValue);
      jet_PUID_flagV.push_back(idflag);
      jet_PUID_passWPLooseV.push_back(passPUIDLoose);

    }// end loop over iJet

    ////
    eve->jet_vect_TLV_[iSys]   = jetV;
    eve->jet_CSV_[iSys]        = csvV;
    eve->jet_cMVA_[iSys]        = cMVAV;
    eve->jet_pt_[iSys]        = jet_ptV;
    eve->jet_eta_[iSys]        = jet_etaV;
    eve->jet_flavour_[iSys]          = jet_flavour_vect;
    eve->jet_partonflavour_[iSys]          = jet_partonflavour_vect;

    //PU jet ID
    eve->jet_PUID_mva_[iSys] = jet_PUID_mvaV;
    eve->jet_PUID_flag_[iSys] = jet_PUID_flagV;
    eve->jet_PUID_passWPLoose_[iSys] = jet_PUID_passWPLooseV;

    // Add lepton 4Vector quantities to MHT
    mht_px += - sum_lepton_vect.Px();
    mht_py += - sum_lepton_vect.Py();
    double MHT = sqrt( mht_px*mht_px + mht_py*mht_py );

    bool PassBigDiamondZmask = ( MuonElectron || 
				 (mass_leplep < (65.5 + 3*MHT/8)) || 
				 (mass_leplep > (108 - MHT/4)) || 
				 (mass_leplep < (79 - 3*MHT/4)) || 
				 (mass_leplep > (99 + MHT/2)) 
				 );
    eve->PassZmask_ = ( PassBigDiamondZmask ) ? 1 : 0;


    double MET = correctedMET.pt();
    bool PassBigDiamondZmaskMET = ( MuonElectron || 
				    (mass_leplep < (65.5 + 3*MET/8)) || 
				    (mass_leplep > (108 - MET/4)) || 
				    (mass_leplep < (79 - 3*MET/4)) || 
				    (mass_leplep > (99 + MET/2)) 
				    );
    eve->PassZmaskMET_ = ( PassBigDiamondZmaskMET ) ? 1 : 0;


    // Compute MHT_
    eve->MHT_[iSys] = MHT;
    // MET
    eve->MET_[iSys]      = correctedMET.pt();
    eve->MET_phi_[iSys]  = correctedMET.phi();
    // MET_NoHF
    // eve->METNoHF_[iSys]      = correctedMET_NoHF.pt();
    // eve->METNoHF_phi_[iSys]  = correctedMET_NoHF.phi();


    // nJets/Tags
    eve->numJets_float_[iSys] = numJet;
    eve->numTags_float_[iSys] = numTag;

    // // jet1-4 pT
    // if( selectedJets.size()>0 ) eve->first_jet_pt_[iSys]  = selectedJets.at(0).pt();
    // if( selectedJets.size()>1 ) eve->second_jet_pt_[iSys] = selectedJets.at(1).pt();



  } // end loop over systematics


  // Fill tree if pass full selection
  if(passingTwoJet) worldTree->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
csvTreeMaker::beginJob()
{

}

// ------------ method called once each job just after ending the event loop  ------------
void 
csvTreeMaker::endJob() 
{

  std::cout << " *********************************************************** " << std::endl;
  std::cout << " ** Opposite sign dilepton event selection ** " << std::endl;
  std::cout << " " << std::endl;
  std::cout << "\t Event information: " << std::endl;
  std::cout << "\t\t Sample = " << sampleName_ << std::endl;
  std::cout << "\t\t Integrated lumi (pb^-1) = " << intLumi_ << std::endl;
  std::cout << "\t\t Cross section (pb)      = " << xSec_ << std::endl;
  std::cout << "\t\t Number generated        = " << nGen_ << std::endl;
  std::cout << " " << std::endl;
  std::cout << "   Number of Events Processed  = " << nevents << std::endl;
  std::cout << "   Number of Events Processed (wgt) = " << nevents_wgt << std::endl;

  std::cout << "   Number of Events Pass Trigger  = " << nevents_clean << std::endl;
  std::cout << "   Number of Events Pass Trigger (wgt) = " << nevents_clean_wgt << std::endl;


  std::cout << " *********************************************************** " << std::endl;

}

// ------------ method called when starting to processes a run  ------------
/*
void 
csvTreeMaker::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
void 
csvTreeMaker::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
  bool hltchanged = true;
  if (!hlt_config_.init(iRun, iSetup, hltTag, hltchanged)) {
    std::cout << "Warning, didn't find trigger process HLT,\t" << hltTag << std::endl;
    return;
  }

}


// ------------ method called when ending the processing of a run  ------------
/*
void 
csvTreeMaker::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
csvTreeMaker::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
csvTreeMaker::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
csvTreeMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


//define this as a plug-in
DEFINE_FWK_MODULE(csvTreeMaker);
