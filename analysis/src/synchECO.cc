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


#include "analysis/src/synchECO.hh"





/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR RUNNING THE CODE                                       **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
synchECO::synchECO(std::string cfg){
  /* 
     constructs the synchECO class 
     parameters: configuration_file
     return: none
  */
	
  _verbose->Class("synchECO");
  
  startExecution(cfg);
  initialize();

  _mvaId=false;
}


//____________________________________________________________________________
synchECO::~synchECO(){
  /* 
     destructs the synchECO class 
     parameters: none
     return: none
  */

}


//____________________________________________________________________________
void synchECO::initialize(){
  /*
    initializes the synchECO class
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
void synchECO::modifyWeight() {
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
void synchECO::run(){

  _els.clear();
  _mus.clear();
  _elIdx.clear();
  _muIdx.clear();
  

  counter("denominator");
	
  // prepare event selection
  collectKinematicObjects();

  // basic event selection (triggers, 2 ss leptons, veto)
  if(!baseSelection()) return;

  //skim right after the basic selection
  //fillSkimTree();
  //return;

}



/*****************************************************************************
******************************************************************************
** DEFINE INPUTS AND OUTPUTS                                                **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void synchECO::defineOutput(){
  /*
    defines and reserves all output that is produced in this class
    parameters: none
    return: none
  */ 

}


//____________________________________________________________________________
void synchECO::loadInput(){
  /*
    loads all input from the cache or from the database
    parameters: none
    return: none
  */


  // define function in MPAF for loading histograms, text files, histograms from database 

}


//____________________________________________________________________________
void synchECO::writeOutput(){
  /*
    writes all output of this class to the disk
    paramters: none
    return: none
  */

  _hm->saveHistos ("synchECO", _cfgName);
  _au->saveNumbers("synchECO", _cfgName);

}


//____________________________________________________________________________
void synchECO::modifySkimming(){
  // if adding variables in the skimming tree is needed...
}


/*****************************************************************************
******************************************************************************
** OBJECT SELECTIONS                                                        **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void synchECO::collectKinematicObjects(){
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
    }
  }

  _nEls  = _els .size();
  _nMus  = _mus .size();


}

//____________________________________________________________________________
bool synchECO::electronSelection(int elIdx){
  /*
    does the selection of electrons
    parameters: elIdx
    return: true (if the electron is an electron), false (else)
  */

  counter("ElectronDenominator", kElId);

  if(!makeCut<float>( _vc->getF("LepGood_pt", elIdx)           , 5.0, ">", "pt selection" , 0, kElId)) return false;
  if(!makeCut<float>( std::abs(_vc->getF("LepGood_eta", elIdx)), 2.4, "<", "eta selection", 0, kElId)) return false;
  
  return true;
}


//____________________________________________________________________________
bool synchECO::muonSelection(int muIdx){
  /*
    does the selection of muons
    parameters: muIdx
    return: true (if the muon is a muon), false (else)
  */

  counter("MuonDenominator", kMuId);
	
  if(!makeCut<float>( _vc->getF("LepGood_pt", muIdx)            , 5.0, ">", "pt selection" , 0, kMuId)) return false;
  if(!makeCut<float>( std::abs( _vc->getF("LepGood_eta", muIdx)), 2.4, "<", "eta selection", 0, kMuId)) return false;

  return true;

}





/*****************************************************************************
******************************************************************************
** SELECTIONS FOR SYNCHRONIZATION AMONG ECO MEMBERS, 2014-12-01             **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool synchECO::baseSelection(){
	/*
    event selection only for synchronization in the ECO group, 2014-12-01
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


