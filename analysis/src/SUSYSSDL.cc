/*****************************************************************************
******************************************************************************
******************************************************************************
**                                                                          **
** The Multi-Purpose Analysis Framework                                     **
**                                                                          **
** Constantin Heidegger, Matthieu Marionneau                                **
** CERN, Fall 2014                                                          **
**                                                                          **
******************************************************************************
******************************************************************************
*****************************************************************************/


#include "analysis/src/SUSYSSDL.hh"





/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR RUNNING THE CODE                                       **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
SUSYSSDL::SUSYSSDL(std::string cfg){
  /* 
     constructs the SUSYSSDL class 
     parameters: configuration_file
     return: none
  */
	
  _verbose->Class("SUSYSSDL");
  
  startExecution(cfg);
  initialize();

  _mvaId=false;
}


//____________________________________________________________________________
SUSYSSDL::~SUSYSSDL(){
  /* 
     destructs the SUSYSSDL class 
     parameters: none
     return: none
  */

}


//____________________________________________________________________________
void SUSYSSDL::initialize(){
  /*
    initializes the SUSYSSDL class
    parameters: none
    return: none
  */


  _vc->registerVar("run"                          , "I" );
  _vc->registerVar("lumi"                         , "I" );
  _vc->registerVar("evt"                          , "I" );
  _vc->registerVar("HLT_SingleMu"                 , "I" );
  _vc->registerVar("HLT_MuEG"                     , "I" );
  _vc->registerVar("HLT_TripleEl"                 , "I" );
  _vc->registerVar("HLT_DoubleEl"                 , "I" );
  _vc->registerVar("HLT_DoubleMu"                 , "I" );
  _vc->registerVar("nVert"                        , "I" );
  _vc->registerVar("nTrueInt"                     , "I" );
  _vc->registerVar("puWeight"                     , "F" );
  _vc->registerVar("genWeight"                    , "F" );
  _vc->registerVar("nLepGood"                     , "I" );
  _vc->registerVar("LepGood_pt"                   , "AF");
  _vc->registerVar("LepGood_eta"                  , "AF");
  _vc->registerVar("LepGood_phi"                  , "AF");
  _vc->registerVar("LepGood_charge"               , "AI");
  _vc->registerVar("LepGood_tightCharge"          , "AI");
  _vc->registerVar("LepGood_pdgId"                , "AI");
  _vc->registerVar("LepGood_relIso03"             , "AF");
  _vc->registerVar("LepGood_relIso04"             , "AF");
  _vc->registerVar("LepGood_dxy"                  , "AF");
  _vc->registerVar("LepGood_dz"                   , "AF");
  _vc->registerVar("LepGood_tightId"              , "AI");
  _vc->registerVar("LepGood_looseIdSusy"          , "AI");
  _vc->registerVar("LepGood_convVeto"             , "AI");
  _vc->registerVar("LepGood_lostHits"             , "AI");
  _vc->registerVar("LepGood_eleCutIdCSA14_50ns_v1", "AI");
  _vc->registerVar("LepGood_mvaTTH"               , "AF");
  _vc->registerVar("LepGood_mvaSusy"              , "AF");
  _vc->registerVar("LepGood_mcMatchId"            , "AI");
  _vc->registerVar("LepGood_mcMatchAny"           , "AI");
  _vc->registerVar("met_pt"                       , "F" );
  _vc->registerVar("met_eta"                      , "F" );
  _vc->registerVar("met_phi"                      , "F" );
  _vc->registerVar("met_mass"                     , "F" );
  _vc->registerVar("nJet25"                       , "I" ); 
  _vc->registerVar("nJet"                         , "I" ); 
  _vc->registerVar("Jet_pt"                       , "AF"); 
  _vc->registerVar("Jet_rawPt"                    , "AF"); 
  _vc->registerVar("Jet_eta"                      , "AF"); 
  _vc->registerVar("Jet_phi"                      , "AF"); 
  _vc->registerVar("Jet_mass"                     , "AF"); 
  _vc->registerVar("Jet_btagCSV"                  , "AF");

  //generator informations
  _vc->registerVar("ngenLep"                      , "I" );
  _vc->registerVar("genLep_eta"                   , "AF");
  _vc->registerVar("genLep_phi"                   , "AF");
  _vc->registerVar("genLep_pdgId"                 , "AI");

  //bjets
  _vc->registerVar("nBJetLoose25"                 , "I" ); 
  _vc->registerVar("nBJetMedium40"                , "I" );
  _vc->registerVar("nBJetMedium25"                , "I" );
  _vc->registerVar("nSoftBJetMedium25"            , "I" );



  //additional counter categories
  _au->addCategory( kElId      , "el Id"      );
  _au->addCategory( kElVeto    , "veto El"    );
  _au->addCategory( kMuId      , "muon Id"    );
  _au->addCategory( kMuVeto    , "veto Mu"    );
  _au->addCategory( kJetId     , "jet Id"     );
  _au->addCategory( kBJetId    , "b-jet Id"   );
  _au->addCategory( kVetoLepSel, "vetoLepSel" );

  //extra input variables
  _lepflav = getCfgVarS("LEPFLAV");
  _mva     = getCfgVarS("LEPID"  );
  _btag    = getCfgVarS("BTAG"   );
  _PT      = getCfgVarS("PT"     );
  _BR      = getCfgVarS("BR"     );
  _SR      = getCfgVarS("SR"     );
  
}


//____________________________________________________________________________
void SUSYSSDL::modifyWeight() {
  /*
    modifies the event weight for every entry in the tree
    parameters: none
    return: none
  */ 
	
  //_weight = (i->second)->Getweight();
  // if(_PUReweighting) 
  //   _weight *= _vc->getF("puWeight");

}


//____________________________________________________________________________
void SUSYSSDL::run(){

  _els.clear();
  _mus.clear();
  _jets.clear();
  _bJets.clear();
  _elIdx.clear();
  _muIdx.clear();
  

  counter("denominator");
	
  // prepare event selection
  collectKinematicObjects();

  // CH: event selection for synchronization
  if(!someSelection()) return;

  return;
	
  // basic event selection (triggers, 2 ss leptons, veto)
  if(!baseSelection()) return;

  //skim right after the basic selection
  fillSkimTree();
  //return;
	
  //splitting the samples into categories
  if( _sampleName.find("DYJets")!=(size_t)-1 || _sampleName.find("TTJets")!=(size_t)-1 ) {
    //_sampleName.find("WJets")!=(size_t)-1 ) {
    
    //ugly
    int lep1Id=0;
    int lep2Id=0;
    if(_nEls==2) {
      lep1Id = genMatchCateg( _els[0] );
      lep2Id = genMatchCateg( _els[1] );
    }
    if(_nEls==1) {
      lep1Id = genMatchCateg( _els[0] );
      lep2Id = genMatchCateg( _mus[0] );
    }
    if(_nEls==0) {
      lep1Id = genMatchCateg( _mus[0] );
      lep2Id = genMatchCateg( _mus[1] );
    }


    if(_sampleName.find("misId")!=(size_t)-1) {
      if( ! ( (lep1Id == kMisChargePdgId && lep2Id >= kMisChargePdgId) || 
	      (lep2Id == kMisChargePdgId && lep1Id >= kMisChargePdgId) ) ) return;
    }
    if(_sampleName.find("fake")!=(size_t)-1) {
      if( lep1Id > kMisMatchPdgId &&
	  lep2Id > kMisMatchPdgId ) return;
    }
    if(_sampleName.find("prompt")!=(size_t)-1) {
      if( lep1Id != kGenMatched ||
	  lep2Id != kGenMatched ) return;
    }
  }
  counter("genCateg selection");
  
  
  // br event selection
  if(!brSelection()) return;
  
  // calling the modules
  fillEventPlots("BR");
  fillJetPlots("BR");
  fillLeptonPlots("BR");
	
	
  // sr event selection
  if(!srSelection()) return;
	
  // calling the modules
  fillEventPlots("SR");
  fillJetPlots("SR");
  fillLeptonPlots("SR");

	
}



/*****************************************************************************
******************************************************************************
** DEFINE INPUTS AND OUTPUTS                                                **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void SUSYSSDL::defineOutput(){
  /*
    defines and reserves all output that is produced in this class
    parameters: none
    return: none
  */ 


  // this is now here but should be somewhere else!!!
  setSignalRegion();
  setBaselineRegion();


  // Baseline Region

  _hm->addVariable("BR_LepCharge" ,   20, -10.0,   10.0, "lepton charge"        );
  _hm->addVariable("BR_HT"        , 1000,   0.0, 1000.0, "H_T [GeV]"            ); 
  _hm->addVariable("BR_MET"       , 1000,   0.0, 1000.0, "#slash{E}_T [GeV]"    );
  _hm->addVariable("BR_MLL"       , 1000,   0.0, 1000.0, "m_{ll} [GeV]"         ); 
  _hm->addVariable("BR_NBJets"    ,   20,   0.0,   20.0, "b-jet multiplicity"   );
  _hm->addVariable("BR_NElectrons",   20,   0.0,   20.0, "electron multiplicity");
  _hm->addVariable("BR_NJets"     ,   20,   0.0,   20.0, "jet multiplicity"     );
  _hm->addVariable("BR_NLeps"     ,   20,   0.0,   20.0, "lepton multiplicity"  );
  _hm->addVariable("BR_NMuons"    ,   20,   0.0,   20.0, "muon multiplicity"    );
  _hm->addVariable("BR_NVrtx"     ,   40,   0.0,   40.0, "vertex multiplicity"  );
  _hm->addVariable("BR_ElDXY"     ,   50,   0.0,    0.5, "#||{dxy}(e) [cm]"     );
  _hm->addVariable("BR_ElAbsEta"  ,  240,   0.0,    2.4, "#||{#eta(e)}"         );
  _hm->addVariable("BR_ElEta"     ,  480,  -2.4,    2.4, "#eta(e)"              );
  _hm->addVariable("BR_ElIso"     ,   50,   0.0,    1.0, "PF Iso (e)"           );
  _hm->addVariable("BR_ElMT"      , 1000,   0.0, 1000.0, "M_T(e) [GeV]"         );
  _hm->addVariable("BR_ElPt"      , 1000,   0.0, 1000.0, "P_T(e) [GeV]"         );
  _hm->addVariable("BR_MuDXY"     ,   50,   0.0,    0.5, "#||{dxy}(#mu) [cm]"   );
  _hm->addVariable("BR_MuAbsEta"  ,  240,   0.0,    2.4, "#||{#eta(#mu)}"       );
  _hm->addVariable("BR_MuEta"     ,  480,  -2.4,    2.4, "#eta(#mu)"            );
  _hm->addVariable("BR_MuIso"     ,   50,   0.0,    1.0, "PF Iso (#mu)"         );
  _hm->addVariable("BR_MuMT"      , 1000,   0.0, 1000.0, "M_T(#mu) [GeV]"       );
  _hm->addVariable("BR_MuPt"      , 1000,   0.0, 1000.0, "P_T(#mu) [GeV]"       );
  _hm->addVariable("BR_JetCSVBTag",   50,   0.0,    1.0, "jet CSV B-Tag"        );
  _hm->addVariable("BR_JetPt"     , 1000,   0.0, 1000.0, "P_T(jet) [GeV]"       );

  // Signal Region

  _hm->addVariable("SR_LepCharge" ,   20, -10.0,   10.0, "lepton charge"        );
  _hm->addVariable("SR_HT"        , 1000,   0.0, 1000.0, "H_T [GeV]"            ); 
  _hm->addVariable("SR_MET"       , 1000,   0.0, 1000.0, "#slash{E}_T [GeV]"    );
  _hm->addVariable("SR_MLL"       , 1000,   0.0, 1000.0, "m_{ll} [GeV]"         ); 
  _hm->addVariable("SR_NBJets"    ,   20,   0.0,   20.0, "b-jet multiplicity"   );
  _hm->addVariable("SR_NElectrons",   20,   0.0,   20.0, "electron multiplicity");
  _hm->addVariable("SR_NJets"     ,   20,   0.0,   20.0, "jet multiplicity"     );
  _hm->addVariable("SR_NLeps"     ,   20,   0.0,   20.0, "lepton multiplicity"  );
  _hm->addVariable("SR_NMuons"    ,   20,   0.0,   20.0, "muon multiplicity"    );
  _hm->addVariable("SR_NVrtx"     ,   40,   0.0,   40.0, "vertex multiplicity"  );
  _hm->addVariable("SR_ElDXY"     ,   50,   0.0,    0.5, "#||{dxy}(e) [cm]"     );
  _hm->addVariable("SR_ElEta"     ,  240,   0.0,    2.4, "#||{#eta(e)}"         );
  _hm->addVariable("SR_ElIso"     ,   50,   0.0,    1.0, "PF Iso (e)"           );
  _hm->addVariable("SR_ElMT"      , 1000,   0.0, 1000.0, "M_T(e) [GeV]"         );
  _hm->addVariable("SR_ElPt"      , 1000,   0.0, 1000.0, "P_T(e) [GeV]"         );
  _hm->addVariable("SR_MuDXY"     ,   50,   0.0,    0.5, "#||{dxy}(#mu) [cm]"   );
  _hm->addVariable("SR_MuEta"     ,  240,   0.0,    2.4, "#||{#eta(#mu)}"       );
  _hm->addVariable("SR_MuIso"     ,   50,   0.0,    1.0, "PF Iso (#mu)"         );
  _hm->addVariable("SR_MuMT"      , 1000,   0.0, 1000.0, "M_T(#mu) [GeV]"       );
  _hm->addVariable("SR_MuPt"      , 1000,   0.0, 1000.0, "P_T(#mu) [GeV]"       );
  _hm->addVariable("SR_JetCSVBTag",   50,   0.0,    1.0, "jet CSV B-Tag"        );
  _hm->addVariable("SR_JetPt"     , 1000,   0.0, 1000.0, "P_T(jet) [GeV]"       );

}


//____________________________________________________________________________
void SUSYSSDL::loadInput(){
  /*
    loads all input from the cache or from the database
    parameters: none
    return: none
  */


  // define function in MPAF for loading histograms, text files, histograms from database 

}


//____________________________________________________________________________
void SUSYSSDL::writeOutput(){
  /*
    writes all output of this class to the disk
    paramters: none
    return: none
  */

  _hm->saveHistos ("SUSYSSDL", _cfgName);
  _au->saveNumbers("SUSYSSDL", _cfgName);

}


//____________________________________________________________________________
void SUSYSSDL::modifySkimming(){
  // if adding variables in the skimming tree is needed...
}


/*****************************************************************************
******************************************************************************
** OBJECT SELECTIONS                                                        **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool SUSYSSDL::bJetSelection(int jetIdx){
  /*
    does the selection of  b-jets
    parameters: jetIdx
    return: true (if the jet is a b-jet), false (else)
  */
	
  //counter("BJetDenominator", kBJetId);

  //if(!makeCut(goodJetSelection(jetIdx), "jet Id", "=", kBJetId) ) return false;
  //if(!makeCut<float>(_vc->getF("Jet_btagCSV", jetIdx), 0.679, ">=", "csv btag selection", 0, kBJetId) ) return false;

  return true;

}


//____________________________________________________________________________
void SUSYSSDL::collectKinematicObjects(){
  /*
    collects all kinematic objects needed in the code, i.e. applies all object selections
    parameters: none
    return: none
  */
  
  for(int i = 0; i < _vc->getI("nLepGood"); ++i){

    // electrons
    if(std::abs(_vc->getI("LepGood_pdgId", i)) == 11){		  
      if(electronSelection(i)) {
        _els.push_back( Candidate::create(_vc->getF("LepGood_pt", i),
					  _vc->getF("LepGood_eta", i),
					  _vc->getF("LepGood_phi", i),
					  _vc->getI("LepGood_pdgId", i),
					  _vc->getI("LepGood_charge", i),
					  0.0005) );
        _elIdx.push_back(i);
      }
      else {
        if(vetoElectronSelection(i))  {
          _vEls.push_back( Candidate::create(_vc->getF("LepGood_pt", i),
					     _vc->getF("LepGood_eta", i),
					     _vc->getF("LepGood_phi", i),
					     _vc->getI("LepGood_pdgId", i),
					     _vc->getI("LepGood_charge", i),
					     0.0005) );
        }
      }		
    }

    // muons
    else if(std::abs(_vc->getI("LepGood_pdgId", i)) == 13){
      if(muonSelection(i)) {
        _mus.push_back( Candidate::create(_vc->getF("LepGood_pt", i),
					  _vc->getF("LepGood_eta", i),
					  _vc->getF("LepGood_phi", i),
					  _vc->getI("LepGood_pdgId", i),
					  _vc->getI("LepGood_charge", i),
					  0.105) );
        _muIdx.push_back(i);
      }
      else {
        if(vetoMuonSelection(i))  {
          _vMus.push_back( Candidate::create(_vc->getF("LepGood_pt", i),
					     _vc->getF("LepGood_eta", i),
					     _vc->getF("LepGood_phi", i),
					     _vc->getI("LepGood_pdgId", i),
					     _vc->getI("LepGood_charge", i),
					     0.105) );
        }
      }
    }
  }

  _nEls  = _els .size();
  _nMus  = _mus .size();
  _nVEls = _vEls.size();
  _nVMus = _vMus.size();
	
  for(int i = 0; i < _vc->getI("nJet"); ++i){
    if(bJetSelection(i) ) {
      _bJets.push_back( Candidate::create(_vc->getF("Jet_pt", i),
					  _vc->getF("Jet_eta", i),
					  _vc->getF("Jet_phi", i) ) );
    }

    if(goodJetSelection(i)) {
      _jets.push_back( Candidate::create(_vc->getF("Jet_pt", i),
					 _vc->getF("Jet_eta", i),
					 _vc->getF("Jet_phi", i) ) );
      
    }
  }

  _nBJets = _bJets.size();
  _nJets  = _jets.size();

  _HT  = HT();
  _met = Candidate::create(_vc->getF("met_pt"), _vc->getF("met_phi") );

}


//____________________________________________________________________________
bool SUSYSSDL::goodJetSelection(int jetIdx){
  /*
    does the selection of good jets, i.e. minimum selection of jets 
    parameters: jetIdx
    return: true (if the jet is good), false (else)
  */
  
  counter("JetDenominator", kJetId);

  if(!makeCut<float>(_vc->getF("Jet_pt", jetIdx)       , 40.0, ">", "pt selection" , 0, kJetId) ) return false;
  if(!makeCut<float>(fabs(_vc->getF("Jet_eta", jetIdx)),  2.4, "<", "eta selection", 0, kJetId) ) return false;


  // CH: here we require dR(j, selected lep) > 0.4, which
  // makes jet-lepton cleaning obsolete
  for(int ie=0; ie<_nEls; ++ie){
    float dr = KineUtils::dR( _els[ie]->eta(), _vc->getF("Jet_eta", jetIdx),
			      _els[ie]->phi(), _vc->getF("Jet_phi", jetIdx));
    if(!makeCut<float>(dr, 0.4, ">", "dR selection (el)", 0, kJetId) ) return false;
  }	

  for(int im=0; im<_nMus; ++im){
    float dr = Tools::dR( _mus[im]->eta(), _vc->getF("Jet_eta", jetIdx),
			  _mus[im]->phi(), _vc->getF("Jet_phi", jetIdx)); 
    if(!makeCut<float>(dr, 0.4, ">", "dR selection (mu)", 0, kJetId) ) return false;
  }

  return true;

}


//____________________________________________________________________________
bool SUSYSSDL::electronSelection(int elIdx){
  /*
    does the selection of electrons
    parameters: elIdx
    return: true (if the electron is an electron), false (else)
  */

  counter("ElectronDenominator", kElId);

  float pt_cut = 10.;
  if(_PT == "highpt") pt_cut = 20.;
  // CH: we use 5GeV for ECO synching
  pt_cut = 5.;

  if(!makeCut<float>( _vc->getF("LepGood_pt", elIdx) , pt_cut, ">"  , "pt selection"    , 0    , kElId)) return false;
  if(!makeCut<float>( std::abs(_vc->getF("LepGood_eta", elIdx)), 2.4   , "<"  , "eta selection"   , 0    , kElId)) return false;
  //if(!makeCut<float>( std::abs(_vc->getF("LepGood_eta", elIdx)), 1.4442, "[!]", "eta selection veto"   , 1.566, kElId)) return false;

  // CH: leave this commented for ECO synching
  //if(_mvaId) {
  //  if(!makeCut<float>( _vc->getF("LepGood_mvaSusy", elIdx) , 0.93, ">", "MVA POG Tight Id", 0, kElId)) return false;
  //}
  //else {
  //  if(!makeCut<int>( _vc->getI("LepGood_eleCutIdCSA14_50ns_v1", elIdx) , 3     , ">=" , "POG CB WP-M Id " , 0    , kElId)) return false;
  //  if(!makeCut<float>( _vc->getF("LepGood_relIso03", elIdx) , 0.1   , "<"  , "Isolation "      , 0    , kElId)) return false;
  //  if(!makeCut<float>( std::abs(_vc->getF("LepGood_dz", elIdx)), 0.1   , "<"  , "dz selection"    , 0    , kElId)) return false;
  //  if(!makeCut<float>( std::abs(_vc->getF("LepGood_dxy", elIdx)), 0.01  , "<"  , "dxy selection"   , 0    , kElId)) return false;
  //}
  //  
  //if(!makeCut<int>( _vc->getI("LepGood_tightCharge", elIdx) , 1     , ">"  , "charge selection", 0    , kElId)) return false;
  //bool conv= (_vc->getI("LepGood_convVeto", elIdx)>0 && _vc->getI("LepGood_lostHits", elIdx)==0);
  //if(!makeCut( conv, "conversion rejection", "=", kElId)) return false;
  
  
  return true;
}


//____________________________________________________________________________
bool SUSYSSDL::muonSelection(int muIdx){
  /*
    does the selection of muons
    parameters: muIdx
    return: true (if the muon is a muon), false (else)
  */

  counter("MuonDenominator", kMuId);
	
  float pt_cut = 10.;
  if(_PT == "highpt") pt_cut = 20.;
  // CH: we use 5GeV for ECO synching
  pt_cut = 5.;

  if(!makeCut<float>( _vc->getF("LepGood_pt", muIdx), pt_cut, ">", "pt selection"    , 0, kMuId)) return false;
  if(!makeCut<float>( std::abs( _vc->getF("LepGood_eta", muIdx)), 2.4, "<", "eta selection", 0, kMuId)) return false;

  //if(_mvaId) {
  //  if(!makeCut<float>( _vc->getF("LepGood_mvaSusy", muIdx) , 0.93  , ">", "MVA POG Tight Id", 0, kMuId)) return false;
  //  if(!makeCut<int>( _vc->getI("LepGood_tightCharge", muIdx) , 1     , ">", "charge selection", 0, kMuId)) return false;
  //}
  //else {
  //  if(!makeCut<int>( _vc->getI("LepGood_tightId", muIdx) , 1     , "=", "POG Tight Id "   , 0, kMuId)) return false;
  //  if(!makeCut<float>( _vc->getF("LepGood_relIso03", muIdx) , 0.1   , "<", "Isolation "      , 0, kMuId)) return false;
  //    
  //  if(!makeCut<float>(std::abs(_vc->getF("LepGood_dz", muIdx)), 0.1   , "<", "dz selection"    , 0, kMuId)) return false;
  //  if(!makeCut<float>(std::abs(_vc->getF("LepGood_dxy", muIdx)), 0.005 , "<", "dxy selection"   , 0, kMuId)) return false;  
  //}


  return true;

}


//____________________________________________________________________________
bool SUSYSSDL::vetoElectronSelection(int elIdx){
  /*
    does the selection of veto electrons
    parameters: elIdx
    return: true (if the electron is a veto electron), false (else)
  */

  counter("vetoElDenominator", kElVeto);

  if(!makeCut<float>( _vc->getF("LepGood_pt", elIdx) , 5.0   , ">"  , "pt selection"    , 0    , kElVeto)) return false;
  if(!makeCut<float>( std::abs(_vc->getF("LepGood_eta", elIdx)), 2.4   , "<"  , "eta selection"   , 0    , kElVeto)) return false;
  if(!makeCut<float>( std::abs(_vc->getF("LepGood_eta", elIdx)), 1.4442, "[!]", "eta selection"   , 1.566, kElVeto)) return false;

  if(_mvaId) {
    if(!makeCut<float>( _vc->getF("LepGood_mvaSusy", elIdx), 0.93   , ">"  , "MVA POG Loose Id", 0    , kElVeto)) return false;
  }
  else {
    if(!makeCut<int>( _vc->getI("LepGood_eleCutIdCSA14_50ns_v1", elIdx), 1, ">=" , "POG CB WP-L Id " , 0    , kElVeto)) return false;
    if(!makeCut<float>( _vc->getF("LepGood_relIso03", elIdx), 0.2    , "<"  , "Isolation "      , 0    , kElVeto)) return false;
  }
  
  return true;

}


//____________________________________________________________________________
bool SUSYSSDL::vetoMuonSelection(int muIdx){
  /*
    does the selection of veto muons
    parameters: muIdx     
    return: true (if the muon is a veto muon), false (else)
  */

  counter("VetoMuonDenominator", kMuVeto);

  if(!makeCut<int>(   _vc->getI("LepGood_tightId", muIdx), 1  , "=", "POG Tight Id", 0, kMuVeto) ) return false;
  if(!makeCut<float>( _vc->getF("LepGood_relIso03", muIdx),  0.2  , "<", "Isolation "   , 0, kMuVeto)) return false;
  // if(!makeCut<int>( _vc->getI("LepGood_tightCharge", muIdx), 1 , ">", "charge selection"  , 0    , kMuId)) return false;
  if(!makeCut<float>(  _vc->getF("LepGood_pt"     , muIdx), 5.0, ">", "pt selection", 0, kMuVeto ) ) return false;

  return true;

}






/*****************************************************************************
******************************************************************************
** KINEMATIC REGION DEFINITIONS                                             **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void SUSYSSDL::setBaselineRegion(){
  /*
    sets the cuts of the baseline region (_BR) for high- and low-pt analyses (_PT)
    parameters: none
    return: none
  */
  
  if(_mva=="mva") _mvaId=true;

  //btagging scheme
  if(_btag=="" || _btag=="std" || _btag=="40")
    _bvar = "nBJetMedium40";
  else if(_btag=="25" || _btag=="soft25")
    _bvar = "nBJetMedium25";
  else if(_btag=="15" || _btag=="IVF")
    _bvar = "nSoftBJetMedium25";
  else
    _bvar = "nBJetMedium40";


  if(_BR == "BRAll" && _PT == "lowpt") {
    setCut("HTBR"     ,  250, ">" );
    setCut("HTCondBR" ,  500, "<" );
    setCut("METHighBR",   30, ">=");
    setCut("METLowBR" ,    0, ">=");
    setCut("NJetsBR"  ,    2, ">=");
    setCut("NBJetsBR" , -100, ">" );
    setCut("CHBR"     , -100, ">" );
  }
  if(_BR == "BR00" && _PT == "lowpt") {
    setCut("HTBR"     ,  250, ">" );
    setCut("HTCondBR" ,  500, "<" );
    setCut("METHighBR",   30, ">=");
    setCut("METLowBR" ,    0, ">=");
    setCut("NJetsBR"  ,    2, ">=");
    setCut("NBJetsBR" ,    0, "=" );
    setCut("CHBR"     , -100, ">" );
  }	
  else if(_BR == "BR01" && _PT == "lowpt") {
    setCut("HTBR"     ,  250, ">" );
    setCut("HTCondBR" ,  500, "<" );
    setCut("METHighBR",   30, ">=");
    setCut("METLowBR" ,    0, ">=");
    setCut("NJetsBR"  ,    2, ">=");
    setCut("NBJetsBR" ,    1, "=" );
    setCut("CHBR"     , -100, ">" );
  }
  else if(_BR == "BR02" && _PT == "lowpt") {
    setCut("HTBR"     ,  250, ">" );
    setCut("HTCondBR" ,  500, "<" );
    setCut("METHighBR",   30, ">=");
    setCut("METLowBR" ,    0, ">=");
    setCut("NJetsBR"  ,    2, ">=");
    setCut("NBJetsBR" ,    2, "=");
    setCut("CHBR"     , -100, ">" );
  }
  else if(_BR == "BR03" && _PT == "lowpt") {
    setCut("HTBR"     ,  250, ">" );
    setCut("HTCondBR" ,  500, "<" );
    setCut("METHighBR",   30, ">=");
    setCut("METLowBR" ,    0, ">=");
    setCut("NJetsBR"  ,    2, ">=");
    setCut("NBJetsBR" ,    3, ">=");
    setCut("CHBR"     , -100, ">" );
  }

  else if(_BR == "BRAll" && _PT == "highpt") {
    setCut("HTBR"     ,   80, ">" );
    setCut("HTCondBR" ,  500, "<" );
    setCut("METHighBR",   30, ">=");
    setCut("METLowBR" ,    0, ">=");
    setCut("NJetsBR"  ,    2, ">=");
    setCut("NBJetsBR" , -100, ">" );
    setCut("CHBR"     , -100, ">" );
  }	
  else if(_BR == "BR00" && _PT == "highpt") {
    setCut("HTBR"     ,   80, ">" );
    setCut("HTCondBR" ,  500, "<" );
    setCut("METHighBR",   30, ">=");
    setCut("METLowBR" ,    0, ">=");
    setCut("NJetsBR"  ,    2, ">=");
    setCut("NBJetsBR" ,    0, "=" );
    setCut("CHBR"     , -100, ">" );
  }	
  else if(_BR == "BR01" && _PT == "highpt") {
    setCut("HTBR"     ,   80, ">" );
    setCut("HTCondBR" ,  500, "<" );
    setCut("METHighBR",   30, ">=");
    setCut("METLowBR" ,    0, ">=");
    setCut("NJetsBR"  ,    2, ">=");
    setCut("NBJetsBR" ,    1, "=" );
    setCut("CHBR"     , -100, ">" );
  }
  else if(_BR == "BR02" && _PT == "highpt") {
    setCut("HTBR"     ,   80, ">" );
    setCut("HTCondBR" ,  500, "<" );
    setCut("METHighBR",   30, ">=");
    setCut("METLowBR" ,    0, ">=");
    setCut("NJetsBR"  ,    2, ">=");
    setCut("NBJetsBR" ,    2, "=");
    setCut("CHBR"     , -100, ">" );
  }
  else if(_BR == "BR02" && _PT == "highpt") {
    setCut("HTBR"     ,   80, ">" );
    setCut("HTCondBR" ,  500, "<" );
    setCut("METHighBR",   30, ">=");
    setCut("METLowBR" ,    0, ">=");
    setCut("NJetsBR"  ,    2, ">=");
    setCut("NBJetsBR" ,    3, ">=");
    setCut("CHBR"     , -100, ">" );
  }
}


//____________________________________________________________________________
void SUSYSSDL::setCut(std::string var, float valCut, std::string cType, float upValCut) {
  /*
    sets the parameters (valCut, cType, upValCut) for a specific cut on a variable (var)
    parameters: var, valCut (the cut value), cType (the cut type), upValCut (the upper value
    in case one cuts the variable in a range)
    return: none
  */


  // signal region
	
  if(var == "HTSR") {
    _valCutHTSR   = valCut;
    _cTypeHTSR    = cType;
    _upValCutHTSR = upValCut;
  } 
	
  else if(var == "METSR") {
    _valCutMETSR   = valCut;
    _cTypeMETSR    = cType;
    _upValCutMETSR = upValCut;
  } 
	
  else if(var == "NJetsSR") {
    _valCutNJetsSR   = valCut;
    _cTypeNJetsSR    = cType;
    _upValCutNJetsSR = upValCut;
  } 
	
  else if(var == "NBJetsSR") {
    _valCutNBJetsSR   = valCut;
    _cTypeNBJetsSR    = cType;
    _upValCutNBJetsSR = upValCut;
  } 
	
  else if(var == "CHSR") {
    _valCutCHSR   = valCut;
    _cTypeCHSR    = cType;
    _upValCutCHSR = upValCut;
  } 


  // baseline region

  else if(var == "HTBR") {
    _valCutHTBR   = valCut;
    _cTypeHTBR    = cType;
    _upValCutHTBR = upValCut;
  } 

  else if(var == "HTCondBR") {
    _valCutHTCondBR   = valCut;
    _cTypeHTCondBR    = cType;
    _upValCutHTCondBR = upValCut;
  } 
	
  else if(var == "METLowBR") {
    _valCutMETLowBR   = valCut;
    _cTypeMETLowBR    = cType;
    _upValCutMETLowBR = upValCut;
  } 
	
  else if(var == "METHighBR") {
    _valCutMETHighBR   = valCut;
    _cTypeMETHighBR    = cType;
    _upValCutMETHighBR = upValCut;
  } 
	
  else if(var == "NJetsBR") {
    _valCutNJetsBR   = valCut;
    _cTypeNJetsBR    = cType;
    _upValCutNJetsBR = upValCut;
  } 
	
  else if(var == "NBJetsBR") {
    _valCutNBJetsBR   = valCut;
    _cTypeNBJetsBR    = cType;
    _upValCutNBJetsBR = upValCut;
  } 
	
  else if(var == "CHBR") {
    _valCutCHBR  = valCut;
    _cTypeCHBR    = cType;
    _upValCutCHBR = upValCut;
  } 

}



//____________________________________________________________________________
void SUSYSSDL::setSignalRegion() {
  /*
    sets the cuts of the signal region (_SR)
    parameters: none
    return: none
  */
  if(_SR == "SR00") {
    setCut("HTSR", -100, ">" );
    setCut("METSR", -100, ">" );
    setCut("NJetsSR", -100, ">" );
    setCut("NBJetsSR", -100, ">" );
    setCut("CHSR", -100, ">" );
  }
  if(_SR == "SR01") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 0, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR02") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 0, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR03") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 0, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR04") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 0, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR05") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 0, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR06") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 0, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR07") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 0, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR08") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 0, "=" );
    setCut("CHSR", -100, ">" );
  }



  else if(_SR == "SR11") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 1, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR12") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 1, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR13") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 1, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR14") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 1, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR15") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 1, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR16") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 1, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR17") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 1, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR18") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 1, "=" );
    setCut("CHSR", -100, ">" );
  }



  else if(_SR == "SR21") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 2, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR22") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 2, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR23") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 2, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR24") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 2, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR25") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 2, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR26") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 2, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR27") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 4, "=" );
    setCut("NBJetsSR", 2, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR28") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 2, "=" );
    setCut("CHSR", -100, ">" );
  }


	
  else if(_SR == "SR31") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 3, ">=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR32") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 3, ">=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR33") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 3, ">=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR34") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 50, "[]", 120 );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 3, ">=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR35") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 3, ">=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR36") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 2, "[]", 3 );
    setCut("NBJetsSR", 3, ">=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR37") {
    setCut("HTSR", 200, "[]", 400 );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 4, "=" );
    setCut("NBJetsSR", 3, ">=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SR38") {
    setCut("HTSR", 400, ">" );
    setCut("METSR", 120, ">" );
    setCut("NJetsSR", 4, ">=" );
    setCut("NBJetsSR", 3, ">=" );
    setCut("CHSR", -100, ">" );
  }


  else if(_SR == "SRRPV0") {
    setCut("HTSR", 500, ">" );
    setCut("METSR", 0, ">=" );
    setCut("NJetsSR", 2, ">=" );
    setCut("NBJetsSR", 0, ">=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SRRPV2") {
    setCut("HTSR", 500, ">" );
    setCut("METSR", 0, ">=" );
    setCut("NJetsSR", 2, ">=" );
    setCut("NBJetsSR", 2, ">=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SRSStop1") {
    setCut("HTSR", 80, ">" );
    setCut("METSR", 30, ">=" );
    setCut("NJetsSR", 2, ">=" );
    setCut("NBJetsSR", 1, "=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SRSStop1++") {
    setCut("HTSR", 80, ">" );
    setCut("METSR", 30, ">=" );
    setCut("NJetsSR", 2, ">=" );
    setCut("NBJetsSR", 1, "=" );
    setCut("CHSR", 2, "=" );
  }
  else if(_SR == "SRSStop2") {
    setCut("HTSR", 80, ">" );
    setCut("METSR", 30, ">=" );
    setCut("NJetsSR", 2, ">=" );
    setCut("NBJetsSR", 2, ">=" );
    setCut("CHSR", -100, ">" );
  }
  else if(_SR == "SRSStop2++") {
    setCut("HTSR", 80, ">" );
    setCut("METSR", 30, ">=" );
    setCut("NJetsSR", 2, ">=" );
    setCut("NBJetsSR", 2, ">=" );
    setCut("CHSR", 2, "=" );
  }


  // if(sr=="SRXX") {
  //   setCut("HT", 80, ">" );
  //   setCut("MET", 30, ">=" );
  //   setCut("NJets", 2, ">=" );
  //   setCut("NBJets", 2, ">=" );
  //   setCut("CH", 2, "=" );
  //   _srName = sr;
  // }

}





/*****************************************************************************
******************************************************************************
** EVENT SELECTIONS                                                         **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool SUSYSSDL::baseSelection(){
  /*
    implements the base selection that is fundamental for both the baseline 
    and the signal region selections
    parameters: none
    return: true (if event passes selection), false (else)
  */
  
  if(_isData && !makeCut<int>(_vc->getI("HLT_DoubleMu"), 1, "=", "HLT DoubleMu") ) return false;	
  if(_isData && !makeCut<int>(_vc->getI("HLT_DoubleEl"), 1, "=", "HLT DoubleEl") ) return false;	
  if(_isData && !makeCut<int>(_vc->getI("HLT_MuEG")    , 1, "=", "HLT MuEG"    ) ) return false;	

  if(_lepflav=="all")
    if(!makeCut<int>( _nEls + _nMus, 2, "=", "lepton multiplicity" ) ) return false; 
  if(_lepflav=="ee")
    if(!makeCut( _nEls==2 && _nMus==0, true, "=", "lepton multiplicity" ) ) return false; 
  if(_lepflav=="mm")
    if(!makeCut( _nEls==0 && _nMus==2, true, "=", "lepton multiplicity" ) ) return false; 
  if(_lepflav=="em")	
    if(!makeCut( _nEls==1 && _nMus==1, true, "=", "lepton multiplicity" ) ) return false; 

  bool is_3l_event = vetoEventSelection("Electron", "Muon");
  bool is_ss_event = ssEventSelection  ();
  //bool is_lowpt_event = lowptEventSelection("Electron", "Muon");

  if(!makeCut( is_ss_event , "same-sign selection", "=") ) return false;
  if(!makeCut( !is_3l_event, "veto on 3 leptons"  , "=") ) return false;
  //if(_PT == "lowpt" && !makeCut( is_lowpt_event, "lowpt 20-10 leptons", "=") ) return false;
	
  //ugly
  Candidate* Z=nullptr;
  if(_nEls==2) Z=Candidate::create( _els[0], _els[1] );
  if(_nEls==1) Z=Candidate::create( _els[0], _mus[0] );
  if(_nEls==0) Z=Candidate::create( _mus[0], _mus[1] );

  if(!makeCut<float>( Z->mass() , 8.0, ">", "MLL selection") ) return false;

  return true;

}


//____________________________________________________________________________
bool SUSYSSDL::brSelection(){
  /*
    implements the baseline region selection provided that the base selection
    already has been applied, i.e. here we just cut on the variables that are
    different among the baseline region selections
    parameters: none
    return: true (if event passes selection), false (else)
  */

  
  if(!makeCut<float>( _HT, _valCutHTBR    , _cTypeHTBR , "BR HT selection"       , _upValCutHTBR      ) ) return false;
  if(_au->simpleCut( _HT, _valCutHTCondBR, _cTypeHTCondBR) ) {
    if(!makeCut<float>( _met->pt(), _valCutMETHighBR, _cTypeMETHighBR, "BR MET selection", _upValCutMETHighBR ) ) return false;
  }
  else {
    if(!makeCut<float>( _met->pt(), _valCutMETLowBR , _cTypeMETLowBR , "BR MET selection" , _upValCutMETLowBR ) ) return false;
  }

  if(!makeCut<int>( _nJets, _valCutNJetsBR , _cTypeNJetsBR , "BR jet multiplicity"  , _upValCutNJetsBR ) ) return false;
  if(!makeCut<int>(_vc->getI(_bvar), _valCutNBJetsBR, _cTypeNBJetsBR, "BR b-jet multiplicity", _upValCutNBJetsBR) ) return false;
  //if(!makeCut<int>( findCharge("Electron", "Muon"), _valCutCHBR    , _cTypeCHBR    , "BR charge selection"  , _upValCutCHBR    ) ) return false;

  return true;

} 


// //____________________________________________________________________________
// bool SUSYSSDL::lowptEventSelection(std::string electron_label, std::string muon_label, std::string kr){
//   /*
//     checks, in the case of lowpt analysis, that the leptons are 20-10 in terms of pt
//     parameters: electron_label, muon_label (the labels of the selected electron and muon objects), kr
//     return: true (if the leptons are 20-10), false (else)
//   */

//   if(_PT != "lowpt") return false;
//   if(_NumKinObj[electron_label] + _NumKinObj[muon_label] != 2) return false;

//   float pts[2] = {0.0, 0.0};
//   int idx = 0;

//   for(int ie = 0; ie < _NumKinObj[electron_label]; ++ie){
//     pts[ie] = _vc->getF("LepGood_pt", _KinObj[electron_label][ie]);
//     ++idx;
//   }

//   for(int im = 0; im < _NumKinObj[muon_label]; ++im)
//     pts[idx + im] = _vc->getF("LepGood_pt", _KinObj[muon_label][im]);

//   std::sort(pts, pts + sizeof(pts)/sizeof(pts[0]));

//   // we did the > 10 selection already in the object selection
//   // due to sort, the last element is the high-pt lepton in the pair
//   if(pts[1] > 20) return true; 

//   return false;

// }


//____________________________________________________________________________
bool SUSYSSDL::someSelection(){
	/*
    event selection only for synchronization
    */
  

  // number of leptons
  if(!makeCut<int>( _nEls + _nMus, 2, ">=", "lepton multiplicity" ) ) return false; 


  // retrieving the lepton quantities
  CandList leps;
  vector<int> indices;
  for(int ie = 0; ie < _nEls; ++ie) {
    leps   .push_back(_els[ie]);
    indices.push_back(_elIdx[ie]);
  }
  for(int im = 0; im < _nMus; ++im) {
    leps   .push_back(_mus[im]);
    indices.push_back(_muIdx[im]);
  }

  float pt_cache = 0;
  int   lep_idx1  = 0;
  for(int il = 0; il < leps.size(); ++il){
    if(leps[il]->pt() > pt_cache){
      lep_idx1 = il;
      pt_cache = leps[il]->pt();
    }
  }

  Candidate * first = leps[lep_idx1]; // the highest pt lepton
  int tree_idx1 = indices[lep_idx1];
 
  pt_cache = 0;
  int lep_idx2 = 0;
  for(int il = 0; il < leps.size(); ++il){
    if(leps[il]->pt() > pt_cache && il != lep_idx1){
      lep_idx2 = il;
      pt_cache = leps[il]->pt();
    }
  }

  Candidate * second = leps[lep_idx2]; // the second highest pt lepton
  int tree_idx2 = indices[lep_idx2];

  // same sign
  int pdg = 11;
  if(_lepflav == "mm") pdg = 13;
  if(!makeCut(fabs(first->pdgId()) == pdg && fabs(first->pdgId()) == fabs(second->pdgId()) &&  first->charge() == second->charge(), true, "=", "lepton flavor and charge" ) ) return false; 

  // fiducial volume
  if(pdg == 11){
    if(!makeCut( (fabs(first->eta()) < 1.4442 || fabs(first->eta()) > 1.566) && (fabs(second->eta()) < 1.4442 || (fabs(second->eta())>1.566 )), true, "=", "fiducial volume")) return false;
  }
  else{
    if(!makeCut(true, true, "=", "fiducial volume") ) return false;
  }

  // pt
  if(!makeCut(first->pt() > 20 && second->pt()>20, true, "=", "lepton pt 20-20") ) return false;

  // mva
  if(!makeCut(_vc->getF("LepGood_mvaTTH", tree_idx1)>0.7 && _vc->getF("LepGood_mvaTTH", tree_idx2)>0.7, true, "=", "lepton mva") ) return false;

  // charge flip
  bool first_ch, second_ch;
  if(first->pdgId() == 11)
    first_ch = ((_vc->getI("LepGood_tightCharge", tree_idx1)>1) && (_vc->getI("LepGood_convVeto", tree_idx1)>0) && (_vc->getI("LepGood_lostHits", tree_idx1)==0));
  else
    first_ch = _vc->getI("LepGood_tightCharge", tree_idx1)>1;

  if(second->pdgId() == 11)
    second_ch = ((_vc->getI("LepGood_tightCharge", tree_idx2)>1) && (_vc->getI("LepGood_convVeto", tree_idx2)>0) && (_vc->getI("LepGood_lostHits", tree_idx2)==0));
  else
    second_ch = _vc->getI("LepGood_tightCharge", tree_idx2)>1;

  if(!makeCut(first_ch && second_ch, true, "=", "lepton charge flip") ) return false;   

  // number of b jets
  if(!makeCut<int>(_vc->getI("nBJetLoose25"), 2, ">=", "B-jet multiplicity") ) return false;

  // number of jets
  if(!makeCut<int>(_vc->getI("nJet25")      , 4, ">=", "Jet multiplicity"  ) ) return false;

  //printf("%d:%3.3f:%3.3f:%d:%d\t", first->pdgId(), first->eta(), first->pt(), _vc->getI("LepGood_mcMatchId", tree_idx1), _vc->getI("LepGood_mcMatchAny", tree_idx1));
  //printf("%d:%d:%d:%3.3f:%3.3f:%3.3f:%3.3f:%d:%d\n", _vc->getI("run"), _vc->getI("lumi"), _vc->getI("evt"), first->pt(), _vc->getF("LepGood_mvaTTH", tree_idx1), second->pt(), _vc->getF("LepGood_mvaTTH", tree_idx2), _vc->getI("nBJetLoose25"), _vc->getI("nJet25"));

  return true;

}


//____________________________________________________________________________
bool SUSYSSDL::srSelection(){
  /*
    implements the signal region selection provided that the base selection
    already has been applied, i.e. here we just cut on the variables that are
    different among the signal region selections
    parameters: none
    return: true (if event passes selection), false (else)
  */


  //  counter("denominator",_srName);
  if(!makeCut<float>( _HT          , _valCutHTSR    , _cTypeHTSR    , "SR HT selection"     , _upValCutHTSR    ) ) return false;
  if(!makeCut<float>( _met->pt()   , _valCutMETSR   , _cTypeMETSR   , "SR MET selection"    , _upValCutMETSR   ) ) return false;
  if(!makeCut<int>( _nJets       , _valCutNJetsSR , _cTypeNJetsSR , "SR jet multiplicity" , _upValCutNJetsSR ) ) return false;
  //if(!makeCut<float>( _vc->getI(_bvar)            , _valCutNBJetsSR, _cTypeNBJetsSR, "SR bjet multiplicity", _upValCutNBJetsSR) ) return false;
  if(!makeCut<float>( eventCharge(), _valCutCHSR    , _cTypeCHSR    , "SR charge selection" , _upValCutCHSR    ) ) return false;

  return true;

} 


//____________________________________________________________________________
bool SUSYSSDL::ssEventSelection(){
  /*
    checks, if the leptons that have been found in the kinematic region are same-sign
    parameters: none
    return: true (if the leptons all have same-sign), false (else)
  */

  int charge = 0;

  for(int ie = 0; ie < _nEls; ++ie){
    if(charge == 0) charge = _els[ie]->charge();
    if( _els[ie]->charge() != charge) return false;
  }
	
  for(int im = 0; im < _nMus; ++im){
    if(charge == 0) charge = _mus[im]->charge();
    if( _mus[im]->charge() != charge) return false;
  }

  return true;

}


//____________________________________________________________________________
bool SUSYSSDL::vetoEventSelection(std::string electron_label, std::string muon_label, std::string kr){
  /*
    performs an essential part of the event selection in the 3l case, i.e.
    events are rejected if there are 3 leptons where two leptons form an
    opposite-sign same-flavor pair with some condition on mll and pt
    parameters: electron_label, muon_label (the labels of the selected electron and muon objects)
    return: true (if the event has such a pair, i.e. if the event shall be rejected), false (else)
  */

  counter("denominator", kVetoLepSel);
	
  if(!makeCut<int>( _nEls+_nMus, 2, "=", "lep mult"     , 0, kVetoLepSel) ) return false; 
  if(!makeCut<int>( _nVEls+_nVMus, 1, "=", "veto lep mult", 0, kVetoLepSel) ) return false; //FIXME =


  // two electrons
  if(_nEls >= 1 && _nVEls == 1){

    // checking charge
    int charge = _vEls[0]->charge();
    bool os = false;
    int os_el_index = -1;
    for(int ie=0; ie<_nEls; ++ie){
      if(_els[ie]->charge() != charge){
	os = true;
	os_el_index = ie;
      }
    }
		
    // there is an os sf pair
    if(os){	
      float mll = Candidate::create(_vEls[0], _els[os_el_index] )->mass();
      if(makeCut(_vEls[0]->pt() > 5.0  &&  mll < 12.0, "mll low pt veto", "=", kVetoLepSel) ) return true;
      if(makeCut(_vEls[0]->pt() > 10.0 && (mll > 76.0 && mll < 106.0), "mll high pt veto", "=", kVetoLepSel) ) return true;
    }
  }

  // two muons
  if(_nMus >= 1 && _nVMus == 1){

    // checking charge
    int charge = _vMus[0]->charge();
    bool os = false;
    int os_mu_index = -1;
    for(int im=0; im<_nMus; ++im){
      if( _vMus[0]->charge() != charge){
	os = true;
	os_mu_index = im;
      }
    }
		
    // there is an os sf pair
    if(os){
      float mll = Candidate::create(_vMus[0], _mus[os_mu_index] )->mass();
      if(makeCut(_vMus[0]->pt() > 5.0  && mll < 12.0, "mll low pt veto", "=", kVetoLepSel) ) return true;
      if(makeCut(_vMus[0]->pt() > 10.0 && (mll > 76.0 && mll < 106.0), "mll high pt veto", "=", kVetoLepSel) ) return true;
    }
  }
	

  makeCut(true, "no veto event", "=", kVetoLepSel);

  return false;


}





/*****************************************************************************
******************************************************************************
** EXECUTING TASKS                                                          **
******************************************************************************
*****************************************************************************/

//____________________________________________________________________________
void SUSYSSDL::fillEventPlots(std::string kr){
  /*
    fills the control plots for event quantities
    parameters: none
    return: none
  */

  fill(kr + "_LepCharge" , eventCharge()        , _weight);
  fill(kr + "_HT"        , _HT                  , _weight);
  fill(kr + "_MET"       , _met->pt() , _weight);
  
  //ugly
  Candidate* Z=nullptr;
  if(_nEls==2) Z=Candidate::create( _els[0], _els[1] );
  if(_nEls==1) Z=Candidate::create( _els[0], _mus[0] );
  if(_nEls==0) Z=Candidate::create( _mus[0], _mus[1] );
  
  fill(kr + "_MLL"       , Z->mass()          , _weight);
  //fill(kr + "_NBJets"    , _NumKinObj["BJet"]                               , _weight);
  fill(kr + "_NBJets"    , _vc->getI(_bvar)    , _weight);
  fill(kr + "_NElectrons", _nEls               , _weight);
  fill(kr + "_NJets"     , _nJets              , _weight);
  fill(kr + "_NLeps"     , _nEls+_nMus         , _weight);
  fill(kr + "_NMuons"    , _nMus               , _weight);
  fill(kr + "_NVrtx"     , _vc->getI("nVert")  , _weight);

}


//____________________________________________________________________________
void SUSYSSDL::fillLeptonPlots(std::string kr){
  /*
    fills the control plots for leptons
    parameters: none
    return: none
  */

  for(int i = 0; i < _nEls; ++i){
    fill(kr + "_ElDXY", std::abs(_vc->getF("LepGood_dxy"     , _elIdx[i])), _weight);
    fill(kr + "_ElEta", std::abs(_vc->getF("LepGood_eta"     , _elIdx[i])), _weight);
    fill(kr + "_ElIso",      _vc->getF("LepGood_relIso03", _elIdx[i]) , _weight);
    fill(kr + "_ElPt" ,      _vc->getF("LepGood_pt"      , _elIdx[i]) , _weight);
    fill(kr + "_ElMT" , Candidate::create( _els[i], _met)->mass(), _weight);
  }

  for(int i = 0; i < _nMus; ++i){
    fill(kr + "_MuDXY", fabs(_vc->getF("LepGood_dxy"     , _muIdx[i])), _weight);
    fill(kr + "_MuEta", fabs(_vc->getF("LepGood_eta"     , _muIdx[i])), _weight);
    fill(kr + "_MuIso",      _vc->getF("LepGood_relIso03", _muIdx[i]) , _weight);
    fill(kr + "_MuPt" ,      _vc->getF("LepGood_pt"      , _muIdx[i]) , _weight);
    fill(kr + "_MuMT" , Candidate::create( _mus[i], _met)->mass()     , _weight);
  }

}


//____________________________________________________________________________
void SUSYSSDL::fillJetPlots(std::string kr){
  /*
    fills the control plots for jets
    parameters: none
    return: none
  */


  for(int i = 0; i < _nJets; ++i){
    fill(kr + "_JetPt"     , _jets[i]->pt(), _weight);
    //    fill(kr + "_JetCSVBTag", _vc->getF("Jet_btagCSV", i)                                       , _weight);
  }

}


//____________________________________________________________________________
int SUSYSSDL::genMatchCateg(const Candidate* cand) {

  //loop over the number of generated leptons
  int nGenL = _vc->getI("ngenLep");

  for(int ig = 0; ig < nGenL; ++ig) {
	
    if(Tools::dR(cand->eta(), _vc->getF("genLep_eta", ig),
		 cand->phi(), _vc->getF("genLep_phi", ig) ) < 0.05 ) { //to be tuned
		  
      // cout<<"matched lepton "<<cand.pdgId<<"  with "<<_vc->getI("genLep_pdgId",ig)<<" !!! "<<cand.pt<<"  "<<cand.eta<<"   "<<cand.phi<<"   "<<Tools::dR(cand.eta, _vc->getF("genLep_eta", ig),
      // cand.phi, _vc->getF("genLep_phi", ig) )<<endl;
		
      if( (abs(cand->pdgId()) != abs(_vc->getI("genLep_pdgId", ig)) ) && abs(_vc->getI("genLep_pdgId", ig)) != 13 ) return kMisMatchPdgId; //taus are exception to the rule
      else if(cand->pdgId()*_vc->getI("genLep_pdgId",ig) < 0 ) return kMisChargePdgId; //+*- = -...
      else return kGenMatched;
			
      break;
    } //dr matching
  } //gen loop
	
  return kNoGenMatch;
}

//____________________________________________________________________________
float 
SUSYSSDL::HT(){
  /*
    computes HT for a given list of selected jets
    parameters: jet_label
    return: HT
  */

  float ht = 0;

  for(int i = 0; i < _nJets; ++i) 
    ht += _jets[i]->pt();

  return ht;

}


//____________________________________________________________________________
int 
SUSYSSDL::eventCharge(){
  /*
    returns the total charge in the event as calculated from the electrons and
    leptons that have been selected
    parameters: electron_label, muon_label
    return: the total charge as int
  */

  int charge = 0;

  for(int i=0;i<_nEls;++i)
    charge += _els[i]->charge();

  for(int i=0;i<_nMus;++i)
    charge += _mus[i]->charge();

  return charge; 

}

