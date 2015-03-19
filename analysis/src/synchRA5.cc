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


#include "analysis/src/synchRA5.hh"





/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR RUNNING THE CODE                                       **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
synchRA5::synchRA5(std::string cfg){
  /* 
     constructs the synch class 
     parameters: configuration_file
     return: none
  */
  
  startExecution(cfg);
  initialize();

  _mvaId=false;
}


//____________________________________________________________________________
synchRA5::~synchRA5(){
  /* 
     destructs the synch class 
     parameters: none
     return: none
  */

}


//____________________________________________________________________________
void synchRA5::initialize(){
  /*
    initializes the synch class
    parameters: none
    return: none
  */


  _vc->registerVar("run"                          , "I" );
  _vc->registerVar("lumi"                         , "I" );
  _vc->registerVar("evt"                          , "I" );
  _vc->registerVar("HLT_SingleMu"                 , "D" );
  _vc->registerVar("HLT_MuEG"                     , "D" );
  _vc->registerVar("HLT_TripleEl"                 , "D" );
  _vc->registerVar("HLT_DoubleEl"                 , "D" );
  _vc->registerVar("HLT_DoubleMu"                 , "D" );
  _vc->registerVar("nVert"                        , "I" );
  _vc->registerVar("nTrueInt"                     , "I" );
  _vc->registerVar("nTrueInt"                     , "I" );
  _vc->registerVar("nBJetPt40Medium"              , "I" );
  _vc->registerVar("puWeight"                     , "D" );
  _vc->registerVar("genWeight"                    , "D" );
  _vc->registerVar("nLepGood"                     , "I" );
  _vc->registerVar("LepGood_pt"                   , "AD");
  _vc->registerVar("LepGood_eta"                  , "AD");
  _vc->registerVar("LepGood_phi"                  , "AD");
  _vc->registerVar("LepGood_charge"               , "AI");
  _vc->registerVar("LepGood_tightCharge"          , "AI");
  _vc->registerVar("LepGood_pdgId"                , "AI");
  _vc->registerVar("LepGood_relIso03"             , "AD");
  _vc->registerVar("LepGood_relIso04"             , "AD");
  _vc->registerVar("LepGood_dxy"                  , "AD");
  _vc->registerVar("LepGood_dz"                   , "AD");
  _vc->registerVar("LepGood_sip3d"                , "AD");
  _vc->registerVar("LepGood_pfMuonId"             , "AI");
  _vc->registerVar("LepGood_tightId"              , "AI");
  _vc->registerVar("LepGood_looseIdSusy"          , "AI");
  _vc->registerVar("LepGood_convVeto"             , "AI");
  _vc->registerVar("LepGood_lostHits"             , "AI");
  _vc->registerVar("LepGood_eleCutIdCSA14_50ns_v1", "AI");
  _vc->registerVar("LepGood_eleCutIdCSA14_50ns_v1", "AI");
  _vc->registerVar("LepGood_eleCutId2012_full5x5" , "AI");
  _vc->registerVar("LepGood_mvaTTH"               , "AD");
  _vc->registerVar("LepGood_mvaSusy"              , "AD");
  _vc->registerVar("LepGood_mcMatchId"            , "AI");
  _vc->registerVar("LepGood_mcMatchAny"           , "AI");
  _vc->registerVar("met_pt"                       , "D" );
  _vc->registerVar("met_eta"                      , "D" );
  _vc->registerVar("met_phi"                      , "D" );
  _vc->registerVar("met_mass"                     , "D" );
  _vc->registerVar("nJet25"                       , "I" ); 
  _vc->registerVar("nJet40"                       , "I" ); 
  _vc->registerVar("nJet"                         , "I" ); 
  _vc->registerVar("Jet_id"                       , "AI"); 
  _vc->registerVar("Jet_pt"                       , "AD"); 
  _vc->registerVar("Jet_rawPt"                    , "AD"); 
  _vc->registerVar("Jet_eta"                      , "AD"); 
  _vc->registerVar("Jet_phi"                      , "AD"); 
  _vc->registerVar("Jet_mass"                     , "AD"); 
  _vc->registerVar("Jet_btagCSV"                  , "AD");

  //generator informations
  _vc->registerVar("ngenLep"                      , "I" );
  _vc->registerVar("genLep_eta"                   , "AD");
  _vc->registerVar("genLep_phi"                   , "AD");
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
void synchRA5::modifyWeight() {
  /*
    modifies the event weight for every entry in the tree
    parameters: none
    return: none
  */ 
	
  //_weight = (i->second)->Getweight();
  // if(_PUReweighting) 
  //   _weight *= _vc->get("puWeight");

}


//____________________________________________________________________________
void synchRA5::run(){

  _leps.clear();
  _vetoleps.clear();
  _els.clear();
  _mus.clear();
  _elIdx.clear();
  _muIdx.clear();
  _jets.clear();
  _vEls.clear();
  _vMus.clear();
  _thirdleps.clear(); 

  counter("denominator");
	
  // prepare event selection
  collectKinematicObjects();

  // basic event selection (triggers, 2 ss leptons, veto)
  if(!baseSelection()) return;

  //skim right after the basic selection
  //fillSkimTree();
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
void synchRA5::defineOutput(){
  /*
    defines and reserves all output that is produced in this class
    parameters: none
    return: none
  */ 


  // this is now here but should be somewhere else!!!
  setSignalRegion();



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
void synchRA5::writeOutput(){
  /*
    writes all output of this class to the disk
    paramters: none
    return: none
  */

}


//____________________________________________________________________________
void synchRA5::modifySkimming(){
  // if adding variables in the skimming tree is needed...
}




/*****************************************************************************
******************************************************************************
** OBJECT SELECTIONS                                                        **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void synchRA5::collectKinematicObjects(){
  /*
    collects all kinematic objects needed in the code, i.e. applies all object selections
    parameters: none
    return: none
  */

  
  for(int i = 0; i < _vc->get("nLepGood"); ++i){

    // electrons
    if(std::abs(_vc->get("LepGood_pdgId", i)) == 11){		  
      if(electronSelection(i)) {
        _els.push_back( Candidate::create(_vc->get("LepGood_pt", i),
					  _vc->get("LepGood_eta", i),
					  _vc->get("LepGood_phi", i),
					  _vc->get("LepGood_pdgId", i),
					  _vc->get("LepGood_charge", i),
					  0.0005) );
        _elIdx.push_back(i);
        _leps.push_back( _els[ _els.size()-1 ] );
      }
      else {
        if(vetoElectronSelection(i))  {
          _vEls.push_back( Candidate::create(_vc->get("LepGood_pt", i),
					     _vc->get("LepGood_eta", i),
					     _vc->get("LepGood_phi", i),
					     _vc->get("LepGood_pdgId", i),
					     _vc->get("LepGood_charge", i),
					     0.0005) );
          _vetoleps.push_back( _vEls[ _vEls.size()-1 ] );
        }
      }		
    }

    // muons
    else if(std::abs(_vc->get("LepGood_pdgId", i)) == 13){
      if(muonSelection(i)) {
        _mus.push_back( Candidate::create(_vc->get("LepGood_pt", i),
					  _vc->get("LepGood_eta", i),
					  _vc->get("LepGood_phi", i),
					  _vc->get("LepGood_pdgId", i),
					  _vc->get("LepGood_charge", i),
					  0.105) );
        _muIdx.push_back(i);
        _leps.push_back( _mus[ _mus.size()-1 ] );
      }
      else {
        if(vetoMuonSelection(i))  {
          _vMus.push_back( Candidate::create(_vc->get("LepGood_pt", i),
					     _vc->get("LepGood_eta", i),
					     _vc->get("LepGood_phi", i),
					     _vc->get("LepGood_pdgId", i),
					     _vc->get("LepGood_charge", i),
					     0.105) );
          _vetoleps.push_back( _vMus[ _vMus.size()-1 ] );
        }
      }
    }
  }

  _nLeps = _leps.size();
  _nEls  = _els .size();
  _nMus  = _mus .size();
  _nVEls = _vEls.size();
  _nVMus = _vMus.size();
	
  for(int i = 0; i < _vc->get("nJet"); ++i){
    if(goodJetSelection(i)) {
      _jets.push_back( Candidate::create(_vc->get("Jet_pt", i),
					 _vc->get("Jet_eta", i),
					 _vc->get("Jet_phi", i) ) );
      
    }
  }

  _nJets  = _jets.size();

  _HT  = HT();
  _met = Candidate::create(_vc->get("met_pt"), _vc->get("met_phi") );

}


//____________________________________________________________________________
bool synchRA5::goodJetSelection(int jetIdx){
  
  counter("JetDenominator", kJetId);

  if(!makeCut<int>(_vc->get("Jet_id", jetIdx)         ,  1  , ">=", "POG CB WP-L ID", 0, kJetId) ) return false;
  if(!makeCut<float>(_vc->get("Jet_pt", jetIdx)       , 40.0, ">" , "pt selection"  , 0, kJetId) ) return false;
  if(!makeCut<float>(fabs(_vc->get("Jet_eta", jetIdx)),  2.4, "<" , "eta selection" , 0, kJetId) ) return false;

  // CH: jet lepton cleaning for loose leptons pt > 10 (all in LepGood)
  for(int il=0; il<_vc->get("nLepGood"); ++il){
    float dr = KineUtils::dR( _vc->get("LepGood_eta", il), _vc->get("Jet_eta", jetIdx),
			      _vc->get("LepGood_phi", il), _vc->get("Jet_phi", jetIdx));
    if(!makeCut<float>(dr, 0.4, ">", "jet lepton cleaning selection", 0, kJetId) ) return false;
  }	

  return true;

}


//____________________________________________________________________________
bool synchRA5::electronSelection(int elIdx){

  counter("ElectronDenominator", kElId);

  if(!makeCut<float>( _vc->get("LepGood_pt"      , elIdx)         , 10., ">" , "pt selection"    , 0, kElId)) return false;
  if(!makeCut<float>( _vc->get("LepGood_sip3d"   , elIdx)         , 4  , "<" , "SIP 3D"          , 0, kElId)) return false;
  if(!makeCut<float>( _vc->get("LepGood_relIso03", elIdx)         , 0.1, "<" , "Isolation "      , 0, kElId)) return false;
  if(!makeCut<float>(std::abs(_vc->get("LepGood_dz" , elIdx))     , 0.1, "<" , "dz selection"    , 0, kElId)) return false;
  if(!makeCut<int>( _vc->get("LepGood_tightId", elIdx)            , 2  , ">=", "POG CB WP-M Id"  , 0, kElId)) return false;
  if(!makeCut<int>( _vc->get("LepGood_tightCharge", elIdx)        , 2  , "=" , "charge selection", 0, kElId)) return false;

  bool conv= (_vc->get("LepGood_convVeto", elIdx)>0 || _vc->get("LepGood_lostHits", elIdx)>0);
  if(!makeCut( conv, "conversion rejection", "=", kElId)) return false;
  
  return true;
}


//____________________________________________________________________________
bool synchRA5::muonSelection(int muIdx){

  counter("MuonDenominator", kMuId);

  if(!makeCut<float>( _vc->get("LepGood_pt"      , muIdx)    , 10., ">", "pt selection"  , 0, kMuId)) return false;
  if(!makeCut<float>( _vc->get("LepGood_sip3d"   , muIdx)    , 4  , "<", "SIP 3D"        , 0, kMuId)) return false;
  if(!makeCut<float>( _vc->get("LepGood_relIso03", muIdx)    , 0.1, "<", "Isolation "    , 0, kMuId)) return false;
  if(!makeCut<float>(std::abs(_vc->get("LepGood_dz", muIdx)) , 0.1, "<", "dz selection"  , 0, kMuId)) return false;
  if(!makeCut<int>( _vc->get("LepGood_tightId"   , muIdx)    , 1  , "=", "POG CB WP-T Id", 0, kMuId)) return false;
	
  return true;

}


//____________________________________________________________________________
bool synchRA5::vetoElectronSelection(int elIdx){

  counter("VetoElectronDenominator", kElVeto);

  if(!makeCut<float>( _vc->get("LepGood_relIso03", elIdx)           , 0.5 , "<" , "Isolation"         , 0, kElVeto)) return false;
  if(!makeCut<float>( std::abs(_vc->get("LepGood_dz", elIdx))       , 0.1 , "<" , "dz selection"      , 0, kElVeto)) return false;
  if(!makeCut<float>( std::abs(_vc->get("LepGood_dxy", elIdx))      , 0.05, "<" , "dxy selection"     , 0, kElVeto)) return false;
  if(!makeCut<int>( _vc->get("LepGood_tightId", elIdx)              , 0   , ">=", "POG CB WP-V Id 5x5", 0, kElVeto)) return false;
  bool conv = (_vc->get("LepGood_convVeto", elIdx)>0 || _vc->get("LepGood_lostHits", elIdx)>1);
  if(!makeCut( conv, "conversion rejection", "=", kElVeto)) return false;

  return true;
}


//____________________________________________________________________________
bool synchRA5::vetoMuonSelection(int muIdx){

  counter("VetoMuonDenominator", kMuVeto);

  if(!makeCut<int>(   _vc->get("LepGood_pfMuonId", muIdx)     , 1   , "=", "POG Loose Id" , 0, kMuVeto) ) return false;
  if(!makeCut<float>( _vc->get("LepGood_relIso03", muIdx)     , 0.2 , "<", "Isolation "   , 0, kMuVeto)) return false;
  if(!makeCut<float>( std::abs(_vc->get("LepGood_dz" , muIdx)), 0.1 , "<", "dz selection" , 0, kMuVeto)) return false;
  if(!makeCut<float>( std::abs(_vc->get("LepGood_dxy", muIdx)), 0.05, "<", "dxy selection", 0, kMuVeto)) return false;

  return true;

}






/*****************************************************************************
******************************************************************************
** KINEMATIC REGION DEFINITIONS                                             **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void synchRA5::setCut(std::string var, float valCut, std::string cType, float upValCut) {
  /*
    sets the parameters (valCut, cType, upValCut) for a specific cut on a variable (var)
    parameters: var, valCut (the cut value), cType (the cut type), upValCut (the upper value
    in case one cuts the variable in a range)
    return: none
  */

  if(var == "HTSR") {
    _valCutHTSR   = valCut;
    _cTypeHTSR    = cType;
    _upValCutHTSR = upValCut;
  } 

  else if(var == "HTCondSR") {
    _valCutHTCondSR   = valCut;
    _cTypeHTCondSR    = cType;
    _upValCutHTCondSR = upValCut;
  } 
	
  else if(var == "METLowSR") {
    _valCutMETLowSR   = valCut;
    _cTypeMETLowSR    = cType;
    _upValCutMETLowSR = upValCut;
  } 
	
  else if(var == "METHighSR") {
    _valCutMETHighSR   = valCut;
    _cTypeMETHighSR    = cType;
    _upValCutMETHighSR = upValCut;
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

}



//____________________________________________________________________________
void synchRA5::setSignalRegion() {
  /*
    sets the cuts of the signal region (_SR)
    parameters: none
    return: none
  */

  if(_btag=="" || _btag=="std" || _btag=="40")
    _bvar = "nBJetMedium40";
  else if(_btag=="25" || _btag=="soft25")
    _bvar = "nBJetMedium25";
  else if(_btag=="15" || _btag=="IVF")
    _bvar = "nSoftBJetMedium25";
  else
    _bvar = "nBJetMedium40";

  if(_SR == "SR00") {
    setCut("HTSR"     ,   80, ">" );
    setCut("HTCondSR" ,  500, "<" );
    setCut("METHighSR",   30, ">" );
    setCut("METLowSR" , -100, ">" );
    setCut("NJetsSR"  ,    2, ">=");
    setCut("NBJetsSR" ,    0, ">=");
  }

  else if(_SR == "SR10") {
    setCut("HTSR"     ,   80, ">" );
    setCut("HTCondSR" ,  500, "<" );
    setCut("METHighSR",   30, ">" );
    setCut("METLowSR" , -100, ">" );
    setCut("NJetsSR"  ,    2, ">=");
    setCut("NBJetsSR" ,    1, "=" );
  }

  else if(_SR == "SR20") {
    setCut("HTSR"     ,   80, ">" );
    setCut("HTCondSR" ,  500, "<" );
    setCut("METHighSR",   30, ">" );
    setCut("METLowSR" , -100, ">" );
    setCut("NJetsSR"  ,    2, ">=");
    setCut("NBJetsSR" ,    2, "=" );
  }

  else if(_SR == "SR30") {
    setCut("HTSR"     ,   80, ">" );
    setCut("HTCondSR" ,  500, "<" );
    setCut("METHighSR",   30, ">" );
    setCut("METLowSR" , -100, ">" );
    setCut("NJetsSR"  ,    2, ">=");
    setCut("NBJetsSR" ,    3, ">=");
  }
}






/*****************************************************************************
******************************************************************************
** SELECTIONS FOR SYNCHRONIZATION AMONG RA5 MEMBERS, 2015-01-12             **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool synchRA5::baseSelection(){
	/*
    event selection only for synchronization in the RA5 group, 2015-01-12
    */
 

  // triggers 
  if(_isData && !makeCut<int>(_vc->get("HLT_DoubleMu"), 1, "=", "HLT DoubleMu") ) return false;	
  if(_isData && !makeCut<int>(_vc->get("HLT_DoubleEl"), 1, "=", "HLT DoubleEl") ) return false;	
  if(_isData && !makeCut<int>(_vc->get("HLT_MuEG")    , 1, "=", "HLT MuEG"    ) ) return false;	


  // CH: working version by SF, gives closure to CERN up to 1%, 2015-01-20
  bool is_ss_event = ssEventSelection(); 
  if(!makeCut( is_ss_event , "same-sign selection", "=") ) return false; 


  // CH: attempt to adapt ECO synch to this respect, closure less good, 2015-01-20

  //CH: retrieving high-pt and low-pt leptons
  //CH: float pt_cache = 0; 
  //CH: int   lep_idx1  = 0; 
  //CH: for(int il = 0; il < _leps.size(); ++il){ 
  //CH:   if(_leps[il]->pt() > pt_cache){ 
  //CH:     lep_idx1 = il; 
  //CH:     pt_cache = _leps[il]->pt(); 
  //CH:   } 
  //CH: } 
  //CH: 
  //CH: pt_cache = 0; 
  //CH: int lep_idx2 = 0; 
  //CH: for(int il = 0; il < _leps.size(); ++il){ 
  //CH:   if(_leps[il]->pt() > pt_cache && il != lep_idx1){ 
  //CH:     lep_idx2 = il; 
  //CH:     pt_cache = _leps[il]->pt(); 
  //CH:   } 
  //CH: } 
  //CH: 
  //CH: _first  = _leps[lep_idx1]; // the highest pt lepton 
  //CH: _second = _leps[lep_idx2]; // the second highest pt lepton 

  //CH: // any other (also tight) lepton is pushed into the veto
  //CH: for(unsigned int il = 0; il < _leps.size(); ++il){
  //CH:   if(il != lep_idx1 && il != lep_idx2) 
  //CH:     _vetoleps.push_back(_leps[il]);
  //CH: }
  //CH: 
  //CH: // same-sign
  //CH: if(!makeCut<int>( _first -> charge() == _second -> charge(), true, "=", "same-sign leptons" ) ) return false; 

  //CH: // multiplicity and flavor
  //CH: if(_lepflav=="mm" && !makeCut( fabs(_first->pdgId()) == 13 && fabs(_second->pdgId()) == 13, true, "=", "lepton multip     licity and flavor" ) ) return false; 
  //CH: if(_lepflav=="em" && !makeCut( (fabs(_first->pdgId()) == 11 && fabs(_second->pdgId()) == 13) || (fabs(_first->pdgId()     ) == 13 && fabs(_second->pdgId()) == 11), true, "=", "lepton multiplicity and flavor" ) ) return false; 
  //CH: if(_lepflav=="ee" && !makeCut( fabs(_first->pdgId()) == 11 && fabs(_second->pdgId()) == 11, true, "=", "lepton multip     licity and flavor" ) ) return false;

  //CH: // pt
  //CH: if     (_PT == "hh" && !makeCut(_first->pt() > 25. && _second->pt() > 25., true, "=", "lepton pt") ) return false;
  //CH: else if(_PT == "hl" && !makeCut(_first->pt() > 25. && _second->pt() < 25., true, "=", "lepton pt") ) return false;
  //CH: else if(_PT == "ll" && !makeCut(_first->pt() < 25. && _second->pt() < 25., true, "=", "lepton pt") ) return false;




  //CH: attempt to reproduce CERN's selection, not closing, January 2015

  //CH: // two tight leptons, same-sign
  //CH: if(_au->simpleCut( _nLeps == 2 && _leps[0]->charge() == _leps[1]->charge(), true, "=") ){

  //CH:   unsigned int fidx;
  //CH:   if(_leps[0]->pt() > _leps[1]->pt()) fidx = 0;
  //CH:   else fidx = 1;

  //CH:   _first  = _leps[fidx];
  //CH:   _second = _leps[(fidx+1) % 2];
  //CH: }

  //CH: // three or more tight leptons, search for same-sign pair
  //CH: else if(_au->simpleCut( _nLeps, 2, ">")){

  //CH:   // splitting up _leps into positively and negatively charged leps
  //CH:   vector<SortableLep> pcl, ncl;
  //CH:   for(unsigned int il = 0; il < _leps.size(); ++il) {
  //CH:     SortableLep sl = {il, _leps[il]->pdgId(), _leps[il]->pt()};
  //CH:     if(_leps[il]->charge() > 0) pcl.push_back(sl);
  //CH:     else                        ncl.push_back(sl);
  //CH:   }

  //CH:   // sorting for flavor (muons before electrons), then pt (high-pt before low-pt)
  //CH:   sort(pcl.begin(), pcl.end(), Tools::compareSL);
  //CH:   sort(ncl.begin(), ncl.end(), Tools::compareSL);

  //CH:   // selecting best same-sign tight lepton pair, building _first and _second
  //CH:   bool usePcl;
  //CH:   if     (pcl.size() >= 2 && ncl.size() < 2) usePcl = true;
  //CH:   else if(pcl.size() < 2 && ncl.size() >= 2) usePcl = false;
  //CH:   else {
  //CH:     if     (fabs(pcl[0].pdgId) > ncl[0].pdgId || fabs(pcl[1].pdgId) > ncl[1].pdgId) usePcl = true;
  //CH:     else if(pcl[0].pt + pcl[1].pt >= ncl[0].pt + ncl[1].pt                        ) usePcl = true;
  //CH:     else                                                                            usePcl = false;
  //CH:   }

  //CH:   unsigned int fidx;
  //CH:   unsigned int sidx;

  //CH:   if(usePcl){
  //CH:     _first  = _leps[pcl[0].idx];
  //CH:     _second = _leps[pcl[1].idx];
  //CH:     fidx = pcl[0].idx;
  //CH:     sidx = pcl[1].idx;
  //CH:   }
  //CH:   else {
  //CH:     _first  = _leps[ncl[0].idx];
  //CH:     _second = _leps[ncl[1].idx];
  //CH:     fidx = ncl[0].idx;
  //CH:     sidx = ncl[1].idx;
  //CH:   }

  //CH:   // any other (also tight) lepton is pushed into the veto
  //CH:   for(unsigned int il = 0; il < _leps.size(); ++il){
  //CH:     if(il != fidx && il != sidx) 
  //CH:       _vetoleps.push_back(_leps[il]);
  //CH:   }

  //CH: }
  //CH: 
  //CH: // not enough same-sign leptons
  //CH: else {
  //CH:   makeCut( true, "lepton multiplicity and charge", "=" );
  //CH:   return false;
  //CH: }


  //CH: // lepton flavor
  //CH: if(_lepflav=="ee")
  //CH:   if(!makeCut( fabs(_first->pdgId()) == 11 && fabs(_second->pdgId()) == 11, true, "=", "lepton flavor" ) ) return false; 
  //CH: if(_lepflav=="mm")
  //CH:   if(!makeCut( fabs(_first->pdgId()) == 13 && fabs(_second->pdgId()) == 13, true, "=", "lepton flavor" ) ) return false; 
  //CH: if(_lepflav=="em")	
  //CH:   if(!makeCut( (fabs(_first->pdgId()) == 11 && fabs(_second->pdgId()) == 13) || (fabs(_first->pdgId()) == 13 && fabs(_second->pdgId()) == 11), true, "=", "lepton flavor" ) ) return false; 

  //CH: // pt
  //CH: if     (_PT == "hh" && !makeCut(_first->pt() > 25 && _second->pt() > 25, true, "=", "lepton pt 25-25") ) return false;
  //CH: else if(_PT == "hl" && !makeCut(_first->pt() > 25 && _second->pt() < 25, true, "=", "lepton pt 25-10") ) return false;
  //CH: else if(_PT == "ll" && !makeCut(_first->pt() < 25 && _second->pt() < 25, true, "=", "lepton pt 10-10") ) return false;

  // Z and gammaStar veto
  bool isVetoEvent = vetoEventSelection();
  if(!makeCut( !isVetoEvent, "veto on 3 leptons"  , "=") ) return false;

  // low-mass veto
  Candidate* Z = Candidate::create( _first, _second);
  if(!makeCut<float>( Z->mass(), 8.0, ">", "MLL selection") ) return false;


  return true;

}


//____________________________________________________________________________
bool synchRA5::srSelection(){
  /*
    implements the signal region selection provided that the base selection
    already has been applied, i.e. here we just cut on the variables that are
    different among the signal region selections
    parameters: none
    return: true (if event passes selection), false (else)
  */

  if(!makeCut<float>( _HT, _valCutHTSR    , _cTypeHTSR , "SR HT selection"       , _upValCutHTSR      ) ) return false;
  if(_au->simpleCut( _HT, _valCutHTCondSR, _cTypeHTCondSR) ) {
    if(!makeCut<float>( _met->pt(), _valCutMETHighSR, _cTypeMETHighSR, "SR MET selection", _upValCutMETHighSR ) ) return false;
  }
  else {
    if(!makeCut<float>( _met->pt(), _valCutMETLowSR , _cTypeMETLowSR , "SR MET selection" , _upValCutMETLowSR ) ) return false;
  }

  if(!makeCut<int>( _nJets         , _valCutNJetsSR , _cTypeNJetsSR , "SR jet multiplicity"  , _upValCutNJetsSR ) ) return false;
  if(!makeCut<int>(_vc->get(_bvar), _valCutNBJetsSR, _cTypeNBJetsSR, "SR b-jet multiplicity", _upValCutNBJetsSR) ) return false;

  printf("%1d %9d %12d\t%2d\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\n", _vc->get("run"), _vc->get("lumi"), _vc->get("evt"), _vc->get("nLepGood"),_first->pdgId(), _first->pt(), _second->pdgId(), _second->pt(), _vc->get("nJet40"), _vc->get("nBJetMedium40"), _met->pt(), _HT);

  return true;

} 



//____________________________________________________________________________
bool synchRA5::ssEventSelection(){
  /*
    checks, if the leptons that have been found in the kinematic region are same-sign
    parameters: none
    return: true (if the leptons all have same-sign), false (else)
  */

  // SF: CHOOSE a SS PAIR, maximizing the number of muons and then pT
  int charge = 0;
  int flavor = 0;
  int flavortmp = 0;
  bool isSS = false;
  unsigned int lep_idx2(0), lep_idx1(0);
  for(unsigned int il1 = 0; il1 < _leps.size(); ++il1){
    for(unsigned int il2 = il1+1; il2 < _leps.size(); ++il2){
      charge    = _leps[il1]->charge() * _leps[il2]->charge();
      flavortmp = fabs(_leps[il1]->pdgId())+fabs( _leps[il2]->pdgId());
      if (charge < 0)         continue; // if the pair is OS skip
      if (flavor > flavortmp) continue; // if the new pair has less muons skip.

      if (_PT == "hh" && _leps[il1]->pt()<25.) continue;
      if (_PT == "hh" && _leps[il2]->pt()<25.) continue;

      if (_PT == "hl" && _leps[il1]->pt()<25.) continue;
      if (_PT == "hl" && _leps[il2]->pt()>25.) continue;

      if (_PT == "ll" && _leps[il1]->pt()>25.) continue;
      if (_PT == "ll" && _leps[il2]->pt()>25.) continue;

      flavor = flavortmp;
      _first  = _leps[il1];   lep_idx1 = il1;
      _second = _leps[il2];   lep_idx2 = il2;
      isSS = true;
    }
  }


  // any other (also tight) lepton is pushed into the veto
  for(unsigned int il = 0; il < _leps.size(); ++il){
    if(il != lep_idx1 && il != lep_idx2)
      _vetoleps.push_back(_leps[il]);
  }


  if      (_lepflav=="mm"  && flavor==26 && isSS) return true;
  else if (_lepflav=="em"  && flavor==24 && isSS) return true;
  else if (_lepflav=="ee"  && flavor==22 && isSS) return true;
  else if (_lepflav=="all" && isSS)               return true;

  return false;

}





//____________________________________________________________________________
bool synchRA5::vetoEventSelection(){
  /*
    performs an essential part of the event selection in the 3l case, i.e.
    events are rejected if there are 3 leptons where two leptons form an
    opposite-sign same-flavor pair with some condition on mll and pt
    parameters: electron_label, muon_label (the labels of the selected electron and muon objects)
    return: true (if the event has such a pair, i.e. if the event shall be rejected), false (else)
  */

  counter("denominator", kVetoLepSel);

  if(_au->simpleCut(_vetoleps.size() == 0) ){
    makeCut( true, "no veto leptons in event", "=", kVetoLepSel); 
    return false;
  }

  for(unsigned int i = 0; i < _vetoleps.size(); ++i){

    // os pair with high-pt lepton
    if(_au->simpleCut( _vetoleps[i] -> charge(), _first -> charge(), "!=") ) {
      float mll = Candidate::create(_vetoleps[i], _first) -> mass();

cout << "OS pair with highest-pt lepton" << endl;
DUMP(mll);

      // same flavor -> Z veto
      if(_au->simpleCut( _vetoleps[i] -> pdgId(), _first -> pdgId(), "=") ) {  
        if(makeCut(mll > 76.0 && mll < 106.0, "Z veto selection", "=", kVetoLepSel) ) return true;
      }

      // any flavor -> gamma star veto
      if(makeCut(mll < 12.0, "gamma star veto selection", "=", kVetoLepSel) ) return true;

    }

    // os pair with low-pt lepton
    if(_au->simpleCut( _vetoleps[i] -> charge(), _second -> charge(), "!=") ) {
      float mll = Candidate::create(_vetoleps[i], _second) -> mass();

cout << "OS pair with lowest-pt lepton" << endl;
DUMP(mll);

      // same flavor -> Z veto
      if(_au->simpleCut( _vetoleps[i] -> pdgId(), _second -> pdgId(), "=") ) {  
        if(makeCut(mll > 76.0 && mll < 106.0, "Z veto selection", "=", kVetoLepSel) ) return true;
      }

      // any flavor -> gamma star veto
      if(makeCut(mll < 12.0, "gamma star veto selection", "=", kVetoLepSel) ) return true;

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
void synchRA5::fillEventPlots(std::string kr){
  /*
    fills the control plots for event quantities
    parameters: none
    return: none
  */

  fill(kr + "_LepCharge" , eventCharge()        , _weight);
  fill(kr + "_HT"        , _HT                  , _weight);
  fill(kr + "_MET"       , _met->pt() , _weight);
  
  //ugly
  Candidate* Z = Candidate::create( _first, _second );
  
  fill(kr + "_MLL"       , Z->mass()          , _weight);
  fill(kr + "_NBJets"    , _vc->get(_bvar)    , _weight);
  fill(kr + "_NElectrons", _nEls               , _weight);
  fill(kr + "_NJets"     , _nJets              , _weight);
  fill(kr + "_NLeps"     , _nEls+_nMus         , _weight);
  fill(kr + "_NMuons"    , _nMus               , _weight);
  fill(kr + "_NVrtx"     , _vc->get("nVert")  , _weight);

}


//____________________________________________________________________________
void synchRA5::fillLeptonPlots(std::string kr){
  /*
    fills the control plots for leptons
    parameters: none
    return: none
  */

  for(int i = 0; i < _nEls; ++i){
    fill(kr + "_ElDXY", std::abs(_vc->get("LepGood_dxy"     , _elIdx[i])), _weight);
    fill(kr + "_ElEta", std::abs(_vc->get("LepGood_eta"     , _elIdx[i])), _weight);
    fill(kr + "_ElIso",      _vc->get("LepGood_relIso03", _elIdx[i]) , _weight);
    fill(kr + "_ElPt" ,      _vc->get("LepGood_pt"      , _elIdx[i]) , _weight);
    fill(kr + "_ElMT" , Candidate::create( _els[i], _met)->mass(), _weight);
  }

  for(int i = 0; i < _nMus; ++i){
    fill(kr + "_MuDXY", fabs(_vc->get("LepGood_dxy"     , _muIdx[i])), _weight);
    fill(kr + "_MuEta", fabs(_vc->get("LepGood_eta"     , _muIdx[i])), _weight);
    fill(kr + "_MuIso",      _vc->get("LepGood_relIso03", _muIdx[i]) , _weight);
    fill(kr + "_MuPt" ,      _vc->get("LepGood_pt"      , _muIdx[i]) , _weight);
    fill(kr + "_MuMT" , Candidate::create( _mus[i], _met)->mass()     , _weight);
  }

}


//____________________________________________________________________________
void synchRA5::fillJetPlots(std::string kr){
  /*
    fills the control plots for jets
    parameters: none
    return: none
  */


  for(int i = 0; i < _nJets; ++i){
    fill(kr + "_JetPt"     , _jets[i]->pt(), _weight);
    //    fill(kr + "_JetCSVBTag", _vc->get("Jet_btagCSV", i)                                       , _weight);
  }

}



//____________________________________________________________________________
int synchRA5::genMatchCateg(const Candidate* cand) {

  //loop over the number of generated leptons
  int nGenL = _vc->get("ngenLep");

  for(int ig = 0; ig < nGenL; ++ig) {
	
    if(Tools::dR(cand->eta(), _vc->get("genLep_eta", ig),
		 cand->phi(), _vc->get("genLep_phi", ig) ) < 0.05 ) { //to be tuned
		  
      // cout<<"matched lepton "<<cand.pdgId<<"  with "<<_vc->get("genLep_pdgId",ig)<<" !!! "<<cand.pt<<"  "<<cand.eta<<"   "<<cand.phi<<"   "<<Tools::dR(cand.eta, _vc->get("genLep_eta", ig),
      // cand.phi, _vc->get("genLep_phi", ig) )<<endl;
		
      if( (abs(cand->pdgId()) != abs(_vc->get("genLep_pdgId", ig)) ) && abs(_vc->get("genLep_pdgId", ig)) != 13 ) return kMisMatchPdgId; //taus are exception to the rule
      else if(cand->pdgId()*_vc->get("genLep_pdgId",ig) < 0 ) return kMisChargePdgId; //+*- = -...
      else return kGenMatched;
			
      break;
    } //dr matching
  } //gen loop
	
  return kNoGenMatch;
}

//____________________________________________________________________________
float 
synchRA5::HT(){
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
synchRA5::eventCharge(){
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


