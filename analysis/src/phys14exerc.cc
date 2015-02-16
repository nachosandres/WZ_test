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


#include "analysis/src/phys14exerc.hh"





/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR RUNNING THE CODE                                       **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
phys14exerc::phys14exerc(std::string cfg){
  /* 
     constructs the phys14exerc class 
     parameters: configuration_file
     return: none
  */
	
  _verbose->Class("phys14exerc");
  
  startExecution(cfg);
  initialize();

  _mvaId=false;
}


//____________________________________________________________________________
phys14exerc::~phys14exerc(){
  /* 
     destructs the phys14exerc class 
     parameters: none
     return: none
  */

}


//____________________________________________________________________________
void phys14exerc::initialize(){
  /*
    initializes the phys14exerc class
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
  //_mva     = getCfgVarS("LEPID"  );
  _btag    = getCfgVarS("BTAG"   );
  _PT      = getCfgVarS("PT"     );
  //_BR      = getCfgVarS("BR"     );
  _SR      = getCfgVarS("SR"     );
  
  //add some protection against non-supported values
  if (_PT != "hh" && _PT != "hl" && _PT != "ll") {
    std::cout << "Unkown value for _PT=" << _PT
              << " valid values are <<hh>>, <<<hl>>, and <<ll>>" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (_btag != "40" && _btag != "25") {
    std::cout << "Unkown value for _btag=" << _btag
              << " valid values are <<40>>, and <<25>>" << std::endl;
    exit(EXIT_FAILURE);
  }

}


//____________________________________________________________________________
void phys14exerc::modifyWeight() {
  /*
    modifies the event weight for every entry in the tree
    parameters: none
    return: none
  */ 
	
  //_weight = (i->second)->Getweight();
  // if(_PUReweighting) 
  //   _weight *= _vc->getD("puWeight");

}


//____________________________________________________________________________
void phys14exerc::run(){

  _leps    .clear();
  _vetoleps.clear();
  _els     .clear();
  _mus     .clear();
  _jets    .clear();
  _bJets   .clear();
  _elIdx   .clear();
  _muIdx   .clear();
  _vEls    .clear();
  _vMus    .clear();

  counter("denominator");
	
  // prepare event selection
  collectKinematicObjects();
	
  // basic event selection (triggers, 2 ss leptons, veto)
  //if(!skimSelection()) return;
  if(!baseSelection()) return;

  //skim right after the basic selection
  //fillSkimTree();
  //return;
	
  ////splitting the samples into categories
  //if( _sampleName.find("DYJets")!=(size_t)-1 || _sampleName.find("TTJets")!=(size_t)-1 ) {
  //  //_sampleName.find("WJets")!=(size_t)-1 ) {
  //  
  //  //ugly
  //  int lep1Id=0;
  //  int lep2Id=0;
  //  if(_nEls==2) {
  //    lep1Id = genMatchCateg( _els[0] );
  //    lep2Id = genMatchCateg( _els[1] );
  //  }
  //  if(_nEls==1) {
  //    lep1Id = genMatchCateg( _els[0] );
  //    lep2Id = genMatchCateg( _mus[0] );
  //  }
  //  if(_nEls==0) {
  //    lep1Id = genMatchCateg( _mus[0] );
  //    lep2Id = genMatchCateg( _mus[1] );
  //  }


  //  if(_sampleName.find("misId")!=(size_t)-1) {
  //    if( ! ( (lep1Id == kMisChargePdgId && lep2Id >= kMisChargePdgId) || 
  //        (lep2Id == kMisChargePdgId && lep1Id >= kMisChargePdgId) ) ) return;
  //  }
  //  if(_sampleName.find("fake")!=(size_t)-1) {
  //    if( lep1Id > kMisMatchPdgId &&
  //    lep2Id > kMisMatchPdgId ) return;
  //  }
  //  if(_sampleName.find("prompt")!=(size_t)-1) {
  //    if( lep1Id != kGenMatched ||
  //    lep2Id != kGenMatched ) return;
  //  }
  //}
  //counter("genCateg selection");
  
	
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
void phys14exerc::defineOutput(){
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
void phys14exerc::loadInput(){
  /*
    loads all input from the cache or from the database
    parameters: none
    return: none
  */


  // define function in MPAF for loading histograms, text files, histograms from database 

}


//____________________________________________________________________________
void phys14exerc::writeOutput(){
  /*
    writes all output of this class to the disk
    paramters: none
    return: none
  */

  _hm->saveHistos ("phys14exerc", _cfgName);
  _au->saveNumbers("phys14exerc", _cfgName);

}


//____________________________________________________________________________
void phys14exerc::modifySkimming(){
  // if adding variables in the skimming tree is needed...
}


/*****************************************************************************
******************************************************************************
** OBJECT SELECTIONS                                                        **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool phys14exerc::bJetSelection(int jetIdx){
  /*
    does the selection of  b-jets
    parameters: jetIdx
    return: true (if the jet is a b-jet), false (else)
  */
	
  counter("BJetDenominator", kBJetId);

  if(!makeCut(goodJetSelection(jetIdx), "jet Id", "=", kBJetId) ) return false;
  if(!makeCut<float>(_vc->getD("Jet_btagCSV", jetIdx), 0.679, ">=", "csv btag selection", 0, kBJetId) ) return false;

  return true;

}


//____________________________________________________________________________
void phys14exerc::collectKinematicObjects(){
  /*
    collects all kinematic objects needed in the code, i.e. applies all object selections
    parameters: none
    return: none
  */
  
  for(int i = 0; i < _vc->getI("nLepGood"); ++i){

    // electrons
    if(std::abs(_vc->getI("LepGood_pdgId", i)) == 11){		  
      if(electronSelection(i)) {
        _els.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
					  _vc->getD("LepGood_eta", i),
					  _vc->getD("LepGood_phi", i),
					  _vc->getI("LepGood_pdgId", i),
					  _vc->getI("LepGood_charge", i),
					  0.0005) );
        _elIdx.push_back(i);
        _leps.push_back( _els[ _els.size()-1 ] );
      }
      else {
        if(vetoElectronSelection(i))  {
          _vEls.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
					     _vc->getD("LepGood_eta", i),
					     _vc->getD("LepGood_phi", i),
					     _vc->getI("LepGood_pdgId", i),
					     _vc->getI("LepGood_charge", i),
					     0.0005) );
          _vetoleps.push_back( _vEls[ _vEls.size()-1 ] );
        }
      }		
    }

    // muons
    else if(std::abs(_vc->getI("LepGood_pdgId", i)) == 13){
      if(muonSelection(i)) {
        _mus.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
					  _vc->getD("LepGood_eta", i),
					  _vc->getD("LepGood_phi", i),
					  _vc->getI("LepGood_pdgId", i),
					  _vc->getI("LepGood_charge", i),
					  0.105) );
        _muIdx.push_back(i);
        _leps.push_back( _mus[ _mus.size()-1 ] );
      }
      else {
        if(vetoMuonSelection(i))  {
          _vMus.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
					     _vc->getD("LepGood_eta", i),
					     _vc->getD("LepGood_phi", i),
					     _vc->getI("LepGood_pdgId", i),
					     _vc->getI("LepGood_charge", i),
					     0.105) );
          _vetoleps.push_back( _vMus[ _vMus.size()-1 ] );
        }
      }
    }
  }

  _nEls  = _els .size();
  _nMus  = _mus .size();
  _nVEls = _vEls.size();
  _nVMus = _vMus.size();
  
  for(int i = 0; i < _vc->getI("nJet25"); ++i){
	//CH: replaced by tree variables 
    //if(bJetSelection(i) ) {
    //  _bJets.push_back( Candidate::create(_vc->getD("Jet_pt", i),
	//				  _vc->getD("Jet_eta", i),
	//				  _vc->getD("Jet_phi", i) ) );
    //}

    if(goodJetSelection(i)) {
      _jets.push_back( Candidate::create(_vc->getD("Jet_pt", i),
					 _vc->getD("Jet_eta", i),
					 _vc->getD("Jet_phi", i) ) );
      
    }
  }

  //_nBJets = _bJets.size();
  _nJets  = _jets.size();

  _HT  = HT();
  _met = Candidate::create(_vc->getD("met_pt"), _vc->getD("met_phi") );

}


//____________________________________________________________________________
bool phys14exerc::goodJetSelection(int jetIdx){
  /*
    does the selection of good jets, i.e. minimum selection of jets 
    parameters: jetIdx
    return: true (if the jet is good), false (else)
  */
  
  counter("JetDenominator", kJetId);
  
  double jetCut = 25;
  if (_btag == "40") jetCut = 40;

  if(!makeCut<float>(_vc->getD("Jet_pt", jetIdx)       , jetCut, ">", "pt selection" , 0, kJetId) ) return false;
  if(!makeCut<float>(fabs(_vc->getD("Jet_eta", jetIdx)),  2.4, "<", "eta selection", 0, kJetId) ) return false;

  // SF: here we require dR(j,every loose lepton) > 0.4
  for(unsigned int il=0; il<_leps.size(); ++il){
    float dr = KineUtils::dR(_leps[il]->eta(), _vc->getD("Jet_eta", jetIdx),
			     _leps[il]->phi(), _vc->getD("Jet_phi", jetIdx));
    if(!makeCut<float>(dr, 0.4, ">", "dR selection", 0, kJetId) ) return false;
  }

  for(unsigned int il=0; il<_vetoleps.size(); ++il){
    float dr = KineUtils::dR(_vetoleps[il]->eta(), _vc->getD("Jet_eta", jetIdx),
			     _vetoleps[il]->phi(), _vc->getD("Jet_phi", jetIdx));
    if(!makeCut<float>(dr, 0.4, ">", "dR selection", 0, kJetId) ) return false;
  }

  return true;

}


//____________________________________________________________________________
bool phys14exerc::electronSelection(int elIdx){
  /*
    does the selection of electrons
    parameters: elIdx
    return: true (if the electron is an electron), false (else)
  */


  counter("ElectronDenominator", kElId);
  
  if(!makeCut<float>( _vc->getD("LepGood_pt"      , elIdx)          , 10.   , ">"  , "pt selection"      , 0    , kElId)) return false;
  if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", elIdx))     , 2.4   , "<"  , "eta selection"     , 0    , kElId)) return false;
  if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", elIdx))     , 1.4442, "[!]", "eta selection veto", 1.566, kElId)) return false;
  if(!makeCut<float>( _vc->getD("LepGood_sip3d"   , elIdx)          , 4     , "<"  , "SIP 3D"            , 0    , kElId)) return false;
  if(!makeCut<float>( _vc->getD("LepGood_relIso03", elIdx)          , 0.1   , "<"  , "Isolation "        , 0    , kElId)) return false;
  if(!makeCut<float>(std::abs(_vc->getD("LepGood_dz" , elIdx))      , 0.1   , "<"  , "dz selection"      , 0    , kElId)) return false;
  if(!makeCut<int>( _vc->getI("LepGood_eleCutId2012_full5x5", elIdx), 3     , ">=" , "POG CB WP-M Id "   , 0    , kElId)) return false;
  if(!makeCut<int>( _vc->getI("LepGood_tightCharge", elIdx)         , 1     , ">"  , "charge selection"  , 0    , kElId)) return false;
  
  bool conv= (_vc->getI("LepGood_convVeto", elIdx) > 0 && _vc->getI("LepGood_lostHits", elIdx)==0);
  if(!makeCut( conv, "conversion rejection", "=", kElId)) return false;
  
  return true;

}


//____________________________________________________________________________
bool phys14exerc::muonSelection(int muIdx){
  /*
    does the selection of muons
    parameters: muIdx
    return: true (if the muon is a muon), false (else)
  */

  counter("MuonDenominator", kMuId);
  
  if(!makeCut<float>( _vc->getD("LepGood_pt"      , muIdx)    , 10., ">", "pt selection"  , 0, kMuId)) return false;
  if(!makeCut<float>( _vc->getD("LepGood_sip3d"   , muIdx)    , 4  , "<", "SIP 3D"        , 0, kMuId)) return false;
  if(!makeCut<float>( _vc->getD("LepGood_relIso03", muIdx)    , 0.1, "<", "Isolation "    , 0, kMuId)) return false;
  if(!makeCut<float>(std::abs(_vc->getD("LepGood_dz", muIdx)) , 0.1, "<", "dz selection"  , 0, kMuId)) return false;
  if(!makeCut<int>( _vc->getI("LepGood_tightId"   , muIdx)    , 1  , ">=", "POG CB WP-T Id", 0, kMuId)) return false;
  
  return true;

}


//____________________________________________________________________________
bool phys14exerc::vetoElectronSelection(int elIdx){
  /*
    does the selection of veto electrons
    parameters: elIdx
    return: true (if the electron is a veto electron), false (else)
  */

  counter("VetoElectronDenominator", kElVeto);

  if(!makeCut<float>( _vc->getD("LepGood_pt", elIdx) , 10.0   , ">"  , "pt selection"    , 0    , kElVeto)) return false; 
  if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", elIdx)), 2.4   , "<"  , "eta selection"   , 0    , kElVeto)) return false;
  if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", elIdx)), 1.4442, "[!]", "eta selection veto" , 1.566, kElVeto)) return false;

  bool conv = (_vc->getI("LepGood_convVeto", elIdx) > 0 && _vc->getI("LepGood_lostHits", elIdx)==0);
  if(!makeCut( conv, "conversion rejection", "=", kElVeto)) return false;

  
  return true;

}


//____________________________________________________________________________
bool phys14exerc::vetoMuonSelection(int muIdx){
  /*
    does the selection of veto muons
    parameters: muIdx     
    return: true (if the muon is a veto muon), false (else)
  */

  counter("VetoMuonDenominator", kMuVeto);

  if(!makeCut<float>(  _vc->getD("LepGood_pt"     , muIdx), 10.0, ">", "pt selection", 0, kMuVeto ) ) return false;
  if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", muIdx)), 2.4   , "<"  , "eta selection"   , 0    , kMuVeto)) return false;
 
  //if(!makeCut<int>(   _vc->getI("LepGood_pfMuonId", muIdx)     , 1   , "=", "POG Loose Id" , 0, kMuVeto) ) return false;
  //if(!makeCut<float>( _vc->getD("LepGood_relIso03", muIdx)     , 0.2 , "<", "Isolation "   , 0, kMuVeto)) return false;
  //if(!makeCut<float>( std::abs(_vc->getD("LepGood_dz" , muIdx)), 0.1 , "<", "dz selection" , 0, kMuVeto)) return false;
  //if(!makeCut<float>( std::abs(_vc->getD("LepGood_dxy", muIdx)), 0.05, "<", "dxy selection", 0, kMuVeto)) return false;
  
  return true;

}






/*****************************************************************************
******************************************************************************
** KINEMATIC REGION DEFINITIONS                                             **
******************************************************************************
*****************************************************************************/



//____________________________________________________________________________
void phys14exerc::setCut(std::string var, float valCut, std::string cType, float upValCut) {
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
void phys14exerc::setSignalRegion() {
  /*
    sets the cuts of the signal region (_SR)
    parameters: none
    return: none
  */

  _bvar = "nBJetMedium25";
  if (_btag == "40") _bvar = "nBJetMedium40";
  
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
** EVENT SELECTIONS                                                         **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool phys14exerc::baseSelection(){
  /*
    implements the base selection that is fundamental for both the baseline 
    and the signal region selections
    parameters: none
    return: true (if event passes selection), false (else)
  */


  // triggers  
  if(_isData && !makeCut<int>(_vc->getI("HLT_DoubleMu"), 1, "=", "HLT DoubleMu") ) return false;	
  if(_isData && !makeCut<int>(_vc->getI("HLT_DoubleEl"), 1, "=", "HLT DoubleEl") ) return false;	
  if(_isData && !makeCut<int>(_vc->getI("HLT_MuEG")    , 1, "=", "HLT MuEG"    ) ) return false;	


  // lepton multiplicity
  if(!makeCut<int>( _nEls + _nMus, 2, ">=", "lepton multiplicity and flavor" ) ) return false; 


  bool is_ss_event = ssEventSelection();
  if(!makeCut( is_ss_event , "same-sign selection", "=") ) return false;

  // retrieving high-pt and low-pt leptons
  //SF: float pt_cache = 0; 
  //SF: int   lep_idx1  = 0; 
  //SF: for(int il = 0; il < _leps.size(); ++il){ 
  //SF:   if(_leps[il]->pt() > pt_cache){ 
  //SF:     lep_idx1 = il; 
  //SF:     pt_cache = _leps[il]->pt(); 
  //SF:   } 
  //SF: } 
  //SF: 
  //SF: pt_cache = 0; 
  //SF: int lep_idx2 = 0; 
  //SF: for(int il = 0; il < _leps.size(); ++il){ 
  //SF:   if(_leps[il]->pt() > pt_cache && il != lep_idx1){ 
  //SF:     lep_idx2 = il; 
  //SF:     pt_cache = _leps[il]->pt(); 
  //SF:   } 
  //SF: } 
  //SF: 
  //SF: _first  = _leps[lep_idx1]; // the highest pt lepton 
  //SF: _second = _leps[lep_idx2]; // the second highest pt lepton 


  //SF: // any other (also tight) lepton is pushed into the veto
  //SF: for(unsigned int il = 0; il < _leps.size(); ++il){
  //SF:   if(il != lep_idx1 && il != lep_idx2) 
  //SF:     _vetoleps.push_back(_leps[il]);
  //SF: }
  //SF: 
  //SF: // same-sign
  //SF: if(!makeCut<int>( _first -> charge() == _second -> charge(), true, "=", "same-sign leptons" ) ) return false; 

  //SF: // multiplicity and flavor
  //SF: if(_lepflav=="mm" && !makeCut( fabs(_first->pdgId()) == 13 && fabs(_second->pdgId()) == 13, true, "=", "lepton multiplicity and flavor" ) ) return false; 
  //SF: if(_lepflav=="em" && !makeCut( (fabs(_first->pdgId()) == 11 && fabs(_second->pdgId()) == 13) || (fabs(_first->pdgId()) == 13 && fabs(_second->pdgId()) == 11), true, "=", "lepton multiplicity and flavor" ) ) return false; 
  //SF: if(_lepflav=="ee" && !makeCut( fabs(_first->pdgId()) == 11 && fabs(_second->pdgId()) == 11, true, "=", "lepton multiplicity and flavor" ) ) return false;

  //SF: // pt
  //SF: if     (_PT == "hh" && !makeCut(_first->pt() > 25. && _second->pt() > 25., true, "=", "lepton pt") ) return false;
  //SF: else if(_PT == "hl" && !makeCut(_first->pt() > 25. && _second->pt() < 25., true, "=", "lepton pt") ) return false;
  //SF: else if(_PT == "ll" && !makeCut(_first->pt() < 25. && _second->pt() < 25., true, "=", "lepton pt") ) return false;

  // veto on third lepton
  bool is_3l_event = vetoEventSelection("Electron", "Muon");
  if(!makeCut( !is_3l_event, "veto on 3 leptons"  , "=") ) return false;
	
  // mll
  Candidate* Z = Candidate::create( _first, _second);
  if(!makeCut<float>( Z->mass() , 8.0, ">", "MLL selection") ) return false;

  return true;

}


//____________________________________________________________________________
bool phys14exerc::skimSelection(){


  // lepton multiplicity
  if(!makeCut<int>( _nEls + _nMus, 2 , ">=", "lepton multiplicity and flavor" ) ) return false; 
  if(!makeCut<float>( _HT        , 80, ">" , "SR HT selection"                ) ) return false;
  if(!makeCut<int>( _nJets       , 2 , ">=", "SR jet multiplicity"            ) ) return false;

  return true;

}

//____________________________________________________________________________
bool phys14exerc::srSelection(){

  if(!makeCut<float>( _HT, _valCutHTSR    , _cTypeHTSR , "SR HT selection"       , _upValCutHTSR      ) ) return false;
  if(_au->simpleCut( _HT, _valCutHTCondSR, _cTypeHTCondSR) ) {
    if(!makeCut<float>( _met->pt(), _valCutMETHighSR, _cTypeMETHighSR, "SR MET selection", _upValCutMETHighSR ) ) return false;
  }
  else {
    if(!makeCut<float>( _met->pt(), _valCutMETLowSR , _cTypeMETLowSR , "SR MET selection" , _upValCutMETLowSR ) ) return false;
  }

  if(!makeCut<int>( _nJets         , _valCutNJetsSR , _cTypeNJetsSR , "SR jet multiplicity"  , _upValCutNJetsSR ) ) return false;
  if(!makeCut<int>(_vc->getI(_bvar), _valCutNBJetsSR, _cTypeNBJetsSR, "SR b-jet multiplicity", _upValCutNBJetsSR) ) return false;
 

  return true;

} 



//____________________________________________________________________________
bool phys14exerc::ssEventSelection(){
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
bool phys14exerc::vetoEventSelection(std::string electron_label, std::string muon_label, std::string kr){
  /*
    performs an essential part of the event selection in the 3l case, i.e.
    events are rejected if there are 3 leptons where two leptons form an
    opposite-sign same-flavor pair with some condition on mll and pt
    parameters: electron_label, muon_label (the labels of the selected electron and muon objects)
    return: true (if the event has such a pair, i.e. if the event shall be rejected), false (else)
  */

  counter("denominator", kVetoLepSel);
	
  for(unsigned int i = 0; i < _vetoleps.size(); ++i){

    // search for OS pair with high-pt lepton
    if (_first->charge() == _vetoleps[i] -> charge()) continue;

    float mll = 0;
    mll = Candidate::create(_vetoleps[i], _first) -> mass();

    // same flavor -> Z veto
    if(_au->simpleCut( fabs(_vetoleps[i] -> pdgId()), fabs(_first -> pdgId()), "=") ) {
      if(makeCut(mll > 76.0 && mll < 106.0, "Z veto selection", "=", kVetoLepSel) ) return true;
    }

    // any flavor -> gamma star veto
    if(makeCut(mll < 12.0, "gamma star veto selection", "=", kVetoLepSel) ) return true;

    mll = Candidate::create(_vetoleps[i], _second) -> mass(); 
 
    // same flavor -> Z veto 
    if(_au->simpleCut( fabs(_vetoleps[i] -> pdgId()), fabs(_second -> pdgId()), "=") ) {   
      if(makeCut(mll > 76.0 && mll < 106.0, "Z veto selection", "=", kVetoLepSel) ) return true; 
    } 
 
    // any flavor -> gamma star veto 
    if(makeCut(mll < 12.0, "gamma star veto selection", "=", kVetoLepSel) ) return true; 
 
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
void phys14exerc::fillEventPlots(std::string kr){
  /*
    fills the control plots for event quantities
    parameters: none
    return: none
  */

  fill(kr + "_LepCharge" , eventCharge()        , _weight);
  fill(kr + "_HT"        , _HT                  , _weight);
  fill(kr + "_MET"       , _met->pt() , _weight);
  
  //ugly
  Candidate* Z = Candidate::create( _first, _second);
  
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
void phys14exerc::fillLeptonPlots(std::string kr){
  /*
    fills the control plots for leptons
    parameters: none
    return: none
  */

  for(int i = 0; i < _nEls; ++i){
    fill(kr + "_ElDXY", std::abs(_vc->getD("LepGood_dxy"     , _elIdx[i])), _weight);
    fill(kr + "_ElEta", std::abs(_vc->getD("LepGood_eta"     , _elIdx[i])), _weight);
    fill(kr + "_ElIso",      _vc->getD("LepGood_relIso03", _elIdx[i]) , _weight);
    fill(kr + "_ElPt" ,      _vc->getD("LepGood_pt"      , _elIdx[i]) , _weight);
    fill(kr + "_ElMT" , Candidate::create( _els[i], _met)->mass(), _weight);
  }

  for(int i = 0; i < _nMus; ++i){
    fill(kr + "_MuDXY", fabs(_vc->getD("LepGood_dxy"     , _muIdx[i])), _weight);
    fill(kr + "_MuEta", fabs(_vc->getD("LepGood_eta"     , _muIdx[i])), _weight);
    fill(kr + "_MuIso",      _vc->getD("LepGood_relIso03", _muIdx[i]) , _weight);
    fill(kr + "_MuPt" ,      _vc->getD("LepGood_pt"      , _muIdx[i]) , _weight);
    fill(kr + "_MuMT" , Candidate::create( _mus[i], _met)->mass()     , _weight);
  }

}


//____________________________________________________________________________
void phys14exerc::fillJetPlots(std::string kr){
  /*
    fills the control plots for jets
    parameters: none
    return: none
  */


  for(int i = 0; i < _nJets; ++i){
    fill(kr + "_JetPt"     , _jets[i]->pt(), _weight);
    //    fill(kr + "_JetCSVBTag", _vc->getD("Jet_btagCSV", i)                                       , _weight);
  }

}


//____________________________________________________________________________
int phys14exerc::genMatchCateg(const Candidate* cand) {

  //loop over the number of generated leptons
  int nGenL = _vc->getI("ngenLep");

  for(int ig = 0; ig < nGenL; ++ig) {
	
    if(Tools::dR(cand->eta(), _vc->getD("genLep_eta", ig),
		 cand->phi(), _vc->getD("genLep_phi", ig) ) < 0.05 ) { //to be tuned
		  
      // cout<<"matched lepton "<<cand.pdgId<<"  with "<<_vc->getI("genLep_pdgId",ig)<<" !!! "<<cand.pt<<"  "<<cand.eta<<"   "<<cand.phi<<"   "<<Tools::dR(cand.eta, _vc->getD("genLep_eta", ig),
      // cand.phi, _vc->getD("genLep_phi", ig) )<<endl;
		
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
phys14exerc::HT(){
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
phys14exerc::eventCharge(){
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

