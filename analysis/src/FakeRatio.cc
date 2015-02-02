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


#include "analysis/src/FakeRatio.hh"





/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR RUNNING THE CODE                                       **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
FakeRatio::FakeRatio(std::string cfg){
  /* 
     constructs the FakeRatio class 
     parameters: configuration_file
     return: none
  */
	
  _verbose->Class("FakeRatio");
  
  startExecution(cfg);
  initialize();

  _mvaId=false;
}


//____________________________________________________________________________
FakeRatio::~FakeRatio(){
  /* 
     destructs the FakeRatio class 
     parameters: none
     return: none
  */

}


//____________________________________________________________________________
void FakeRatio::initialize(){
  /*
    initializes the FakeRatio class
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
  _au->addCategory( kLElId, "loose El Id");
  _au->addCategory( kTElId, "tight El Id");
  _au->addCategory( kVElId, "veto El Id" );
  _au->addCategory( kLMuId, "loose Mu Id");
  _au->addCategory( kTMuId, "tight Mu Id");
  _au->addCategory( kVMuId, "veto Mu Id" );
  _au->addCategory( kJetId, "jet Id"     );

  //extra input variables
  _lepflav = getCfgVarS("LEPFLAV");
  _mva     = getCfgVarS("LEPID"  );
  _btag    = getCfgVarS("BTAG"   );
  _MR      = getCfgVarS("MR"     );
  
}


//____________________________________________________________________________
void FakeRatio::modifyWeight() {
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
void FakeRatio::run(){



  _lEls.clear();
  _tEls.clear();
  _vEls.clear();
  _lLeps.clear();
  _tLeps.clear();
  _vLeps.clear();
  _lMus.clear();
  _tMus.clear();
  _vMus.clear();

  _lElIdx.clear();
  _tElIdx.clear();
  _vElIdx.clear();
  _lLepIdx.clear();
  _tLepIdx.clear();
  _vLepIdx.clear();
  _lMuIdx.clear();
  _tMuIdx.clear();
  _vMuIdx.clear();

  vector<int>().swap(_lElIdx);
  vector<int>().swap(_tElIdx);
  vector<int>().swap(_vElIdx);
  vector<int>().swap(_lLepIdx);
  vector<int>().swap(_tLepIdx);
  vector<int>().swap(_vLepIdx);
  vector<int>().swap(_lMuIdx);
  vector<int>().swap(_tMuIdx);
  vector<int>().swap(_vMuIdx);


  counter("denominator");
	
  // prepare event selection
  collectKinematicObjects();
	
  // measurement region selection
  //if(!skimSelection()) return;
  if(!mrSelection()) return;

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
  
	
  // calling the modules
  fillEventPlots("MR");
  fillFakeRatioMaps("MR");
  fillJetPlots("MR");
  fillLeptonPlots("MR");

	
}



/*****************************************************************************
******************************************************************************
** DEFINE INPUTS AND OUTPUTS                                                **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void FakeRatio::defineOutput(){
  /*
    defines and reserves all output that is produced in this class
    parameters: none
    return: none
  */ 


  float FR_bins_eta[6] = { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5};
  float FR_bins_pt[9] = {10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0};
  vector<float> bins_eta = Tools::toVector(FR_bins_eta);
  vector<float> bins_pt = Tools::toVector(FR_bins_pt);

  // Setting Measurement Region
  setMeasurementRegion();

  // Measurement Region
  _hm->addVariable("MR_HT"         , 1000,   0.0, 1000.0, "H_T [GeV]"                  ); 
  _hm->addVariable("MR_MET"        , 1000,   0.0, 1000.0, "#slash{E}_T [GeV]"          );
  _hm->addVariable("MR_NBJets"     ,   20,   0.0,   20.0, "b-jet multiplicity"         );
  _hm->addVariable("MR_NLEls"      ,   20,   0.0,   20.0, "loose electron multiplicity");
  _hm->addVariable("MR_NTEls"      ,   20,   0.0,   20.0, "tight electron multiplicity");
  _hm->addVariable("MR_NVEls"      ,   20,   0.0,   20.0, "veto electron multiplicity" );
  _hm->addVariable("MR_NJets"      ,   20,   0.0,   20.0, "jet multiplicity"           );
  _hm->addVariable("MR_NLLeps"     ,   20,   0.0,   20.0, "loose lepton multiplicity"  );
  _hm->addVariable("MR_NTLeps"     ,   20,   0.0,   20.0, "tight lepton multiplicity"  );
  _hm->addVariable("MR_NVLeps"     ,   20,   0.0,   20.0, "veto lepton multiplicity"   );
  _hm->addVariable("MR_NLMus"      ,   20,   0.0,   20.0, "loose muon multiplicity"    );
  _hm->addVariable("MR_NTMus"      ,   20,   0.0,   20.0, "tight muon multiplicity"    );
  _hm->addVariable("MR_NVMus"      ,   20,   0.0,   20.0, "veto muon multiplicity"     );
  _hm->addVariable("MR_NVrtx"      ,   40,   0.0,   40.0, "vertex multiplicity"        );
  _hm->addVariable("MR_LElDXY"     ,   50,   0.0,    0.5, "#||{dxy}(e) [cm]"           );
  _hm->addVariable("MR_LElEta"     ,  240,   0.0,    2.4, "#||{#eta(e)}"               );
  _hm->addVariable("MR_LElIso"     ,   50,   0.0,    1.0, "PF Iso (e)"                 );
  _hm->addVariable("MR_LElMT"      , 1000,   0.0, 1000.0, "M_T(e) [GeV]"               );
  _hm->addVariable("MR_LElPt"      , 1000,   0.0, 1000.0, "P_T(e) [GeV]"               );
  _hm->addVariable("MR_TElDXY"     ,   50,   0.0,    0.5, "#||{dxy}(e) [cm]"           );
  _hm->addVariable("MR_TElEta"     ,  240,   0.0,    2.4, "#||{#eta(e)}"               );
  _hm->addVariable("MR_TElIso"     ,   50,   0.0,    1.0, "PF Iso (e)"                 );
  _hm->addVariable("MR_TElMT"      , 1000,   0.0, 1000.0, "M_T(e) [GeV]"               );
  _hm->addVariable("MR_TElPt"      , 1000,   0.0, 1000.0, "P_T(e) [GeV]"               );
  _hm->addVariable("MR_LMuDXY"     ,   50,   0.0,    0.5, "#||{dxy}(#mu) [cm]"         );
  _hm->addVariable("MR_LMuEta"     ,  240,   0.0,    2.4, "#||{#eta(#mu)}"             );
  _hm->addVariable("MR_LMuIso"     ,   50,   0.0,    1.0, "PF Iso (#mu)"               );
  _hm->addVariable("MR_LMuMT"      , 1000,   0.0, 1000.0, "M_T(#mu) [GeV]"             );
  _hm->addVariable("MR_LMuPt"      , 1000,   0.0, 1000.0, "P_T(#mu) [GeV]"             );
  _hm->addVariable("MR_TMuDXY"     ,   50,   0.0,    0.5, "#||{dxy}(#mu) [cm]"         );
  _hm->addVariable("MR_TMuEta"     ,  240,   0.0,    2.4, "#||{#eta(#mu)}"             );
  _hm->addVariable("MR_TMuIso"     ,   50,   0.0,    1.0, "PF Iso (#mu)"               );
  _hm->addVariable("MR_TMuMT"      , 1000,   0.0, 1000.0, "M_T(#mu) [GeV]"             );
  _hm->addVariable("MR_TMuPt"      , 1000,   0.0, 1000.0, "P_T(#mu) [GeV]"             );
  _hm->addVariable("MR_JetCSVBTag" ,   50,   0.0,    1.0, "jet CSV B-Tag"              );
  _hm->addVariable("MR_JetPt"      , 1000,   0.0, 1000.0, "P_T(jet) [GeV]"             );
  _hm->addVariable("MR_LElMap"     , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)"  );
  _hm->addVariable("MR_LElMapBJM"  , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)"  );
  _hm->addVariable("MR_LLepMap"    , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(lep)", "#||{#eta}(lep)");
  _hm->addVariable("MR_LLepMapBJM" , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(lep)", "#||{#eta}(lep)");
  _hm->addVariable("MR_LMuMap"     , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");
  _hm->addVariable("MR_LMuMapBJM"  , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");
  _hm->addVariable("MR_TElMap"     , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)"  );
  _hm->addVariable("MR_TElMapBJM"  , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)"  );
  _hm->addVariable("MR_TLepMap"    , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(lep)", "#||{#eta}(lep)");
  _hm->addVariable("MR_TLepMapBJM" , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(lep)", "#||{#eta}(lep)");
  _hm->addVariable("MR_TMuMap"     , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");
  _hm->addVariable("MR_TMuMapBJM"  , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");
  

}


//____________________________________________________________________________
void FakeRatio::loadInput(){
  /*
    loads all input from the cache or from the database
    parameters: none
    return: none
  */


  // define function in MPAF for loading histograms, text files, histograms from database 

}


//____________________________________________________________________________
void FakeRatio::writeOutput(){
  /*
    writes all output of this class to the disk
    paramters: none
    return: none
  */

  _hm->saveHistos ("FakeRatio", _cfgName);
  _au->saveNumbers("FakeRatio", _cfgName);

}


//____________________________________________________________________________
void FakeRatio::modifySkimming(){
  // if adding variables in the skimming tree is needed...
}


/*****************************************************************************
******************************************************************************
** OBJECT SELECTIONS                                                        **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void FakeRatio::collectKinematicObjects(){
  /*
    collects all kinematic objects needed in the code, i.e. applies all object selections
    parameters: none
    return: none
  */
  
  for(int i = 0; i < _vc->getI("nLepGood"); ++i){

    // electrons
    if(std::abs(_vc->getI("LepGood_pdgId", i)) == 11){		  
      if(looseElectronSelection(i)) {
        _lEls.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
					  _vc->getD("LepGood_eta", i),
					  _vc->getD("LepGood_phi", i),
					  _vc->getI("LepGood_pdgId", i),
					  _vc->getI("LepGood_charge", i),
					  0.0005) );
        _lElIdx.push_back(i);
        _lLeps.push_back( _lEls[ _lEls.size()-1 ] );
        _lLepIdx.push_back(i);
      }
      else if(tightElectronSelection(i)) {
        _tEls.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
					  _vc->getD("LepGood_eta", i),
					  _vc->getD("LepGood_phi", i),
					  _vc->getI("LepGood_pdgId", i),
					  _vc->getI("LepGood_charge", i),
					  0.0005) );
        _tElIdx.push_back(i);
        _tLeps.push_back( _tEls[ _tEls.size()-1 ] );
        _tLepIdx.push_back(i);
      }
      else {
        if(vetoElectronSelection(i))  {
          _vEls.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
					     _vc->getD("LepGood_eta", i),
					     _vc->getD("LepGood_phi", i),
					     _vc->getI("LepGood_pdgId", i),
					     _vc->getI("LepGood_charge", i),
					     0.0005) );
          _vLeps.push_back( _vEls[ _vEls.size()-1 ] );
        }
      }		
    }

    // muons
    else if(std::abs(_vc->getI("LepGood_pdgId", i)) == 13){
      if(looseMuonSelection(i)) {
        _lMus.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
					  _vc->getD("LepGood_eta", i),
					  _vc->getD("LepGood_phi", i),
					  _vc->getI("LepGood_pdgId", i),
					  _vc->getI("LepGood_charge", i),
					  0.105) );
        _lMuIdx.push_back(i);
        _lLeps.push_back( _lMus[ _lMus.size()-1 ] );
        _lLepIdx.push_back(i);
      }
      else if(tightMuonSelection(i)) {
        _tMus.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
					  _vc->getD("LepGood_eta", i),
					  _vc->getD("LepGood_phi", i),
					  _vc->getI("LepGood_pdgId", i),
					  _vc->getI("LepGood_charge", i),
					  0.105) );
        _tMuIdx.push_back(i);
        _tLeps.push_back( _tMus[ _tMus.size()-1 ] );
        _tLepIdx.push_back(i);
      }
      else {
        if(vetoMuonSelection(i))  {
          _vMus.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
					     _vc->getD("LepGood_eta", i),
					     _vc->getD("LepGood_phi", i),
					     _vc->getI("LepGood_pdgId", i),
					     _vc->getI("LepGood_charge", i),
					     0.105) );
          _vLeps.push_back( _vMus[ _vMus.size()-1 ] );
        }
      }
    }
  }

  _nLEls  = _lEls .size();
  _nTEls  = _tEls .size();
  _nVEls  = _vEls .size();
  _nLLeps = _lLeps.size();
  _nTLeps = _tLeps.size();
  _nVLeps = _vLeps.size();
  _nLMus  = _lMus .size();
  _nTMus  = _tMus .size();
  _nVMus  = _vMus .size();
  
  for(int i = 0; i < _vc->getI("nJet"); ++i){
    if(goodJetSelection(i)) {
      _jets.push_back( Candidate::create(_vc->getD("Jet_pt", i),
					                     _vc->getD("Jet_eta", i),
					                     _vc->getD("Jet_phi", i) ) );
      
    }
  }

  _nJets  = _jets.size();
  _HT  = HT();
  _met = Candidate::create(_vc->getD("met_pt"), _vc->getD("met_phi") );

}


//____________________________________________________________________________
bool FakeRatio::goodJetSelection(int jetIdx){
  /*
    does the selection of good jets, i.e. minimum selection of jets 
    parameters: jetIdx
    return: true (if the jet is good), false (else)
  */
  
  counter("JetDenominator", kJetId);

  if(!makeCut<float>(_vc->getD("Jet_pt", jetIdx)       , 40.0, ">", "pt selection" , 0, kJetId) ) return false;
  if(!makeCut<float>(fabs(_vc->getD("Jet_eta", jetIdx)),  2.4, "<", "eta selection", 0, kJetId) ) return false;

  // CH: the requirement that dR(jet, every loose leption) > 1.0 in combination with
  // requirering EXACTLY one loose lepton in the measurement region, makes jet-lepton
  // cleaning obsolete
  
  for(unsigned int il=0; il<_lLeps.size(); ++il){
    float dr = KineUtils::dR(_lLeps[il]->eta(), _vc->getD("Jet_eta", jetIdx),
			                 _lLeps[il]->phi(), _vc->getD("Jet_phi", jetIdx));
    if(!makeCut<float>(dr, 1.0, ">", "dR selection", 0, kJetId) ) return false;
  }

  // CH: this is jet-lepton cleaning: removing the closest jet to the loose lepton if dR < 0.4
  float dr_cache = 999999.;
  for(unsigned int il=0; il<_lLeps.size(); ++il){
    float dr = KineUtils::dR(_lLeps[il]->eta(), _vc->getD("Jet_eta", jetIdx),
			                 _lLeps[il]->phi(), _vc->getD("Jet_phi", jetIdx));
    if(dr < dr_cache)
      dr_cache = dr;
  }
  for(unsigned int il=0; il<_vLeps.size(); ++il){
    float dr = KineUtils::dR(_vLeps[il]->eta(), _vc->getD("Jet_eta", jetIdx),
			                 _vLeps[il]->phi(), _vc->getD("Jet_phi", jetIdx));
    if(dr < dr_cache)
      dr_cache = dr;
  }
  if(!makeCut<float>(dr_cache, 0.4, ">", "dR selection, jet-lepton cleaning", 0, kJetId) ) return false;

  return true;

}


//____________________________________________________________________________
bool FakeRatio::leptonMvaId(int lepIdx, int cat, float iso, float dxy, bool tch){

  if(!makeCut<float>( _vc->getD("LepGood_mvaSusy", lepIdx)            , 0.93, ">" , "MVA POG Tight Id", 0, cat)) return false;
  if(tch) 
    if(!makeCut<int>( _vc->getI("LepGood_tightCharge", lepIdx)        , 1   , ">" , "charge selection", 0, cat)) return false;
  // CH: this is for categorization between numerator and denominator
  //if(!makeCut<float>( _vc->getD("LepGood_relIso03", lepIdx)         , iso  , "<"  , "isolation "        , 0 , kLElId)) return false;
  //if(!makeCut<float>(std::abs(_vc->getD("LepGood_dxy", lepIdx))     , dxy  , "<"  , "dxy selection"     , 0 , kLElId)) return false;

  return true;

}


//____________________________________________________________________________
bool FakeRatio::leptonCutId(int lepIdx, int cat, float iso, float dxy, string cbid, int id){

  if(!makeCut<int>(       _vc->getI(cbid              , lepIdx) , id , ">=", "cut-based WP" , 0, cat)) return false;
  if(!makeCut<float>(     _vc->getD("LepGood_relIso03", lepIdx) , iso, "<" , "isolation "   , 0, cat)) return false;
  if(!makeCut<float>(fabs(_vc->getD("LepGood_dxy"     , lepIdx)), dxy, "<" , "dxy selection", 0, cat)) return false;
  if(!makeCut<float>(fabs(_vc->getD("LepGood_dz"      , lepIdx)), 0.1, "<" , "dz selection" , 0, cat)) return false;

  return true;
} 


//____________________________________________________________________________
bool FakeRatio::looseElectronSelection(int elIdx){
  /*
    does the selection of loose electrons
    parameters: elIdx
    return: true (if the electron is a loose electron), false (else)
  */

  counter("LooseElectronDenominator", kLElId);
  
  if(!makeCut<float>( _vc->getD("LepGood_pt"      , elIdx)            , 10.   , ">"  , "pt selection"      , 0    , kLElId)) return false;
  if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", elIdx))       , 2.4   , "<"  , "eta selection"     , 0    , kLElId)) return false;
  if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", elIdx))       , 1.4442, "[!]", "eta selection veto", 1.566, kLElId)) return false;

  bool mvaid = leptonMvaId(elIdx, kLElId, 0.6, 999.);
  bool cutid = leptonCutId(elIdx, kLElId, 0.6, 999., "LepGood_eleCutId2012_full5x5", 3);
  bool lepid = false;

  if     (_mva == "mva"      ) lepid = mvaid;
  else if(_mva == "mvanotcut") lepid = mvaid && !cutid;
  else if(_mva == "mvaandcut") lepid = mvaid && cutid;
  else if(_mva == "cut"      ) lepid = cutid;
  else if(_mva == "cutnotmva") lepid = cutid && !mvaid;
  else if(_mva == "cutandmva") lepid = cutid && mvaid;
 
  if(!makeCut( lepid, "lepton id", "=", kLElId)) return false;

  if(!makeCut<float>( _vc->getD("LepGood_sip3d"   , elIdx)            , 4     , "<"  , "SIP 3D"            , 0    , kLElId)) return false;
  if(!makeCut<int>( _vc->getI("LepGood_tightCharge", elIdx)           , 1     , ">"  , "charge selection"  , 0    , kLElId)) return false;
  
  bool conv= (_vc->getI("LepGood_convVeto", elIdx) > 0 && _vc->getI("LepGood_lostHits", elIdx)==0);
  if(!makeCut( conv, "conversion rejection", "=", kLElId)) return false;
  
  return true;

}


//____________________________________________________________________________
bool FakeRatio::looseMuonSelection(int muIdx){
  /*
    does the selection of loose muons
    parameters: muIdx
    return: true (if the muon is a loose muon), false (else)
  */

  counter("LooseMuonDenominator", kLMuId);
  
  if(!makeCut<float>( _vc->getD("LepGood_pt"         , muIdx)    , 10. , ">" , "pt selection"    , 0, kLMuId)) return false;
  if(!makeCut<float>( _vc->getD("LepGood_eta"        , muIdx)    , 2.4 , "<" , "eta selection"   , 0, kLMuId)) return false;
  if(!makeCut<float>( _vc->getD("LepGood_sip3d"      , muIdx)    , 4   , "<" , "SIP 3D"          , 0, kLMuId)) return false;

  bool mvaid = leptonMvaId(muIdx, kLMuId, 0.6, 0.01, true);
  bool cutid = leptonCutId(muIdx, kLMuId, 1.0, 0.2 , "LepGood_tightId", 1);
  bool lepid = false;

  if     (_mva == "mva"      ) lepid = mvaid;
  else if(_mva == "mvanotcut") lepid = mvaid && !cutid;
  else if(_mva == "mvaandcut") lepid = mvaid && cutid;
  else if(_mva == "cut"      ) lepid = cutid;
  else if(_mva == "cutnotmva") lepid = cutid && !mvaid;
  else if(_mva == "cutandmva") lepid = cutid && mvaid;
 
  if(!makeCut( lepid, "lepton id", "=", kLMuId)) return false;
  
  return true;

}


//____________________________________________________________________________
bool FakeRatio::tightElectronSelection(int elIdx){
  /*
    does the selection of tight electrons
    parameters: elIdx
    return: true (if the electron is a tight electron), false (else)
  */


  counter("TightElectronDenominator", kTElId);
 
  if(!makeCut<float>( _vc->getD("LepGood_pt"      , elIdx)            , 10.   , ">"  , "pt selection"      , 0    , kTElId)) return false;
  if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", elIdx))       , 2.4   , "<"  , "eta selection"     , 0    , kTElId)) return false;
  if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", elIdx))       , 1.4442, "[!]", "eta selection veto", 1.566, kTElId)) return false;

  bool mvaid = leptonMvaId(elIdx, kTElId, 0.1, 0.01);
  bool cutid = leptonCutId(elIdx, kTElId, 0.1, 0.01, "LepGood_eleCutId2012_full5x5", 3);
  bool lepid = false;

  if     (_mva == "mva"      ) lepid = mvaid;
  else if(_mva == "mvanotcut") lepid = mvaid && !cutid;
  else if(_mva == "mvaandcut") lepid = mvaid && cutid;
  else if(_mva == "cut"      ) lepid = cutid;
  else if(_mva == "cutnotmva") lepid = cutid && !mvaid;
  else if(_mva == "cutandmva") lepid = cutid && mvaid;
 
  if(!makeCut( lepid, "lepton id", "=", kTElId)) return false;

  if(!makeCut<float>( _vc->getD("LepGood_sip3d"   , elIdx)            , 4     , "<"  , "SIP 3D"            , 0    , kTElId)) return false;
  if(!makeCut<int>( _vc->getI("LepGood_tightCharge", elIdx)           , 1     , ">"  , "charge selection"  , 0    , kTElId)) return false;
  
  bool conv= (_vc->getI("LepGood_convVeto", elIdx) > 0 && _vc->getI("LepGood_lostHits", elIdx)==0);
  if(!makeCut( conv, "conversion rejection", "=", kTElId)) return false;
  
  return true;

}


//____________________________________________________________________________
bool FakeRatio::tightMuonSelection(int muIdx){
  /*
    does the selection of tight muons
    parameters: muIdx
    return: true (if the muon is a tight muon), false (else)
  */

  counter("TightMuonDenominator", kTMuId);
  
  if(!makeCut<float>( _vc->getD("LepGood_pt"      , muIdx)      , 10. , ">" , "pt selection"  , 0, kTMuId)) return false;
  if(!makeCut<float>( _vc->getD("LepGood_eta"     , muIdx)      , 2.4 , "<" , "eta selection" , 0, kTMuId)) return false;
  if(!makeCut<float>( _vc->getD("LepGood_sip3d"   , muIdx)      , 4   , "<" , "SIP 3D"        , 0, kTMuId)) return false;

  bool mvaid = leptonMvaId(muIdx, kTMuId, 0.1, 0.01, true);
  bool cutid = leptonCutId(muIdx, kTMuId, 0.1, 0.01, "LepGood_tightId", 1);
  bool lepid = false;

  if     (_mva == "mva"      ) lepid = mvaid;
  else if(_mva == "mvanotcut") lepid = mvaid && !cutid;
  else if(_mva == "mvaandcut") lepid = mvaid && cutid;
  else if(_mva == "cut"      ) lepid = cutid;
  else if(_mva == "cutnotmva") lepid = cutid && !mvaid;
  else if(_mva == "cutandmva") lepid = cutid && mvaid;
 
  if(!makeCut( lepid, "lepton id", "=", kTMuId)) return false;
  
  return true;

}


//____________________________________________________________________________
bool FakeRatio::vetoElectronSelection(int elIdx){
  /*
    does the selection of veto electrons
    parameters: elIdx
    return: true (if the electron is a veto electron), false (else)
  */

  counter("VetoElectronDenominator", kVElId);

  if(!makeCut<float>( _vc->getD("LepGood_pt", elIdx), 10.0, "<", "pt selection", 0, kVElId)) return false; 
  
  return true;

}


//____________________________________________________________________________
bool FakeRatio::vetoMuonSelection(int muIdx){
  /*
    does the selection of veto muons
    parameters: muIdx     
    return: true (if the muon is a veto muon), false (else)
  */

  counter("VetoMuonDenominator", kVMuId);
 
  if(!makeCut<float>( _vc->getD("LepGood_pt", muIdx), 10.0, "<", "pt selection", 0, kVMuId)) return false; 
  
  return true;

}






/*****************************************************************************
******************************************************************************
** KINEMATIC REGION DEFINITIONS                                             **
******************************************************************************
*****************************************************************************/



//____________________________________________________________________________
void FakeRatio::setCut(std::string var, float valCut, std::string cType, float upValCut) {
  /*
    sets the parameters (valCut, cType, upValCut) for a specific cut on a variable (var)
    parameters: var, valCut (the cut value), cType (the cut type), upValCut (the upper value
    in case one cuts the variable in a range)
    return: none
  */


  // measurement region

  if(var == "NBJetsMR") {
    _valCutNBJetsMR   = valCut;
    _cTypeNBJetsMR    = cType;
    _upValCutNBJetsMR = upValCut;
  } 
	

}



//____________________________________________________________________________
void FakeRatio::setMeasurementRegion() {
  /*
    sets the cuts of the measurement region (MR)
    parameters: none
    return: none
  */

  if(_mva=="mva" || _mva == "mvanotcb") _mvaId=true;


  if(_btag=="" || _btag=="std" || _btag=="40")
    _bvar = "nBJetMedium40";
  else if(_btag=="25" || _btag=="soft25")
    _bvar = "nBJetMedium25";
  else if(_btag=="15" || _btag=="IVF")
    _bvar = "nSoftBJetMedium25";
  else
    _bvar = "nBJetMedium40";
  
  if(_MR == "MR00") {
    setCut("NBJetsSR", 0, ">=");
  }
  else if(_MR == "MR01") {
    setCut("NBJetsMR", 1, ">=");
  }
  else if(_MR == "MR02") {
    setCut("NBJetsMR", 2, ">=");
  }
  else if(_MR == "MR03") {
    setCut("NBJetsMR", 3, ">=");
  }
}



/*****************************************************************************
******************************************************************************
** EVENT SELECTIONS                                                         **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool FakeRatio::mrSelection(){


  // triggers  
  if(_isData && !makeCut<int>(_vc->getI("HLT_DoubleMu"), 1, "=", "HLT DoubleMu") ) return false;	
  if(_isData && !makeCut<int>(_vc->getI("HLT_DoubleEl"), 1, "=", "HLT DoubleEl") ) return false;	
  if(_isData && !makeCut<int>(_vc->getI("HLT_MuEG")    , 1, "=", "HLT MuEG"    ) ) return false;	

  // lepton multiplicity
  if     (_lepflav=="all" && !makeCut<int>( _nLLeps          , 1   , "=", "lepton multiplicity and flavor" ) ) return false;  
  else if(_lepflav=="e"   && !makeCut( _nLEls==1 && _nLMus==0, true, "=", "lepton multiplicity and flavor" ) ) return false;
  else if(_lepflav=="m"   && !makeCut( _nLEls==0 && _nLMus==1, true, "=", "lepton multiplicity and flavor" ) ) return false;

  // veto leptons 
  if(!makeCut<int>( _nVLeps      , 0   , "=" , "lepton veto"     ) ) return false; 

  // jet multiplicity
  if(!makeCut<int>( _nJets       , 1   , ">=", "jet multiplicity") ) return false; 

  // MET 
  if(!makeCut<float>( _met->pt() , 20.0, ">" , "MET selection"   ) ) return false;

  // MT
  Candidate* MT = nullptr;
  MT = Candidate::create( _lLeps[0], _met);
  if(!makeCut<float>( MT->mass() , 20.0, ">" , "MT selection"    ) ) return false;

  return true;

}

//____________________________________________________________________________
bool FakeRatio::skimSelection(){

  // triggers  
  if(_isData && !makeCut<int>(_vc->getI("HLT_DoubleMu"), 1, "=", "HLT DoubleMu") ) return false;	
  if(_isData && !makeCut<int>(_vc->getI("HLT_DoubleEl"), 1, "=", "HLT DoubleEl") ) return false;	
  if(_isData && !makeCut<int>(_vc->getI("HLT_MuEG")    , 1, "=", "HLT MuEG"    ) ) return false;	

  // lepton multiplicity
  if     (_lepflav=="all" && !makeCut<int>( _nLLeps          , 1   , "=", "lepton multiplicity and flavor" ) ) return false; 
  else if(_lepflav=="e"   && !makeCut( _nLEls==1 && _nLMus==0, true, "=", "lepton multiplicity and flavor" ) ) return false;
  else if(_lepflav=="m"   && !makeCut( _nLEls==0 && _nLMus==1, true, "=", "lepton multiplicity and flavor" ) ) return false;

  return true;

}




/*****************************************************************************
******************************************************************************
** EXECUTING TASKS                                                          **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool FakeRatio::bJetMatching(int lepIdx){
  /*
	matches the lepton to the closest jet (w.r.t. dR) and returns true if 
	this jet is a bJet (i.e. the lepton is a fake)
  */

  float dr_cache = 99999999.;
  float btag_cache = 0.;

  for(int j = 0; j < _vc->getI("nJet"); ++j){
    float dr = KineUtils::dR(_vc->getD("LepGood_eta", lepIdx), _vc->getD("Jet_eta", j),
                             _vc->getD("LepGood_phi", lepIdx), _vc->getD("Jet_phi", j));
    if(dr < dr_cache){
      dr_cache   = dr;
      btag_cache = _vc->getD("Jet_btagCSV", j);
    }
  }

  if(btag_cache > 0.679) return true;

  return false;

}


//____________________________________________________________________________
void FakeRatio::fillEventPlots(std::string kr){
  /*
    fills the control plots for event quantities
    parameters: none
    return: none
  */

  fill(kr + "_HT"        , _HT                  , _weight);
  fill(kr + "_MET"       , _met->pt() , _weight);
  fill(kr + "_NBJets"    , _vc->getI(_bvar)    , _weight);
  fill(kr + "_NLEls"     , _nLEls              , _weight);
  fill(kr + "_NTEls"     , _nTEls              , _weight);
  fill(kr + "_NVEls"     , _nVEls              , _weight);
  fill(kr + "_NJets"     , _nJets              , _weight);
  fill(kr + "_NLLeps"    , _nLLeps             , _weight);
  fill(kr + "_NTLeps"    , _nTLeps             , _weight);
  fill(kr + "_NVLeps"    , _nVLeps             , _weight);
  fill(kr + "_NLMus"     , _nLMus              , _weight);
  fill(kr + "_NTMus"     , _nTMus              , _weight);
  fill(kr + "_NVMus"     , _nVMus              , _weight);
  fill(kr + "_NVrtx"     , _vc->getI("nVert")  , _weight);

}


//____________________________________________________________________________
void FakeRatio::fillFakeRatioMaps(std::string kr){
  /*
    fills the fake ratio maps for leptons
    parameters: none
    return: none
  */


  for(int i = 0; i < _nLEls; ++i){
    fill(kr + "_LElMap" , _vc->getD("LepGood_pt", _lElIdx[i] ), fabs(_vc->getD("LepGood_eta", _lElIdx[i] )), _weight);
    if(bJetMatching(_lElIdx[i]))
	  fill(kr + "_LElMapBJM" , _vc->getD("LepGood_pt", _lElIdx[i] ), fabs(_vc->getD("LepGood_eta", _lElIdx[i] )), _weight);
  }

  for(int i = 0; i < _nTEls; ++i){
    fill(kr + "_TElMap" , _vc->getD("LepGood_pt", _tElIdx[i] ), fabs(_vc->getD("LepGood_eta", _tElIdx[i] )), _weight);
    if(bJetMatching(_tElIdx[i]))
      fill(kr + "_TElMapBJM" , _vc->getD("LepGood_pt", _tElIdx[i] ), fabs(_vc->getD("LepGood_eta", _tElIdx[i] )), _weight);
  }

  for(int i = 0; i < _nLLeps; ++i){
    fill(kr + "_LLepMap", _vc->getD("LepGood_pt", _lLepIdx[i]), fabs(_vc->getD("LepGood_eta", _lLepIdx[i])), _weight);
    if(bJetMatching(_lLepIdx[i]))
      fill(kr + "_LLepMapBJM", _vc->getD("LepGood_pt", _lLepIdx[i]), fabs(_vc->getD("LepGood_eta", _lLepIdx[i])), _weight);
  }

  for(int i = 0; i < _nTLeps; ++i){
    fill(kr + "_TLepMap", _vc->getD("LepGood_pt", _tLepIdx[i]), fabs(_vc->getD("LepGood_eta", _tLepIdx[i])), _weight);
    if(bJetMatching(_tLepIdx[i]))
      fill(kr + "_TLepMapBJM", _vc->getD("LepGood_pt", _tLepIdx[i]), fabs(_vc->getD("LepGood_eta", _tLepIdx[i])), _weight);
  }

  for(int i = 0; i < _nLMus; ++i){
    fill(kr + "_LMuMap" , _vc->getD("LepGood_pt", _lMuIdx[i] ), fabs(_vc->getD("LepGood_eta", _lMuIdx[i] )), _weight);
    if(bJetMatching(_lMuIdx[i]))
      fill(kr + "_LMuMapBJM" , _vc->getD("LepGood_pt", _lMuIdx[i] ), fabs(_vc->getD("LepGood_eta", _lMuIdx[i] )), _weight);
  }

  for(int i = 0; i < _nTMus; ++i){
    fill(kr + "_TMuMap" , _vc->getD("LepGood_pt", _tMuIdx[i] ), fabs(_vc->getD("LepGood_eta", _tMuIdx[i] )), _weight);
    if(bJetMatching(_tMuIdx[i]))
      fill(kr + "_TMuMapBJM" , _vc->getD("LepGood_pt", _tMuIdx[i] ), fabs(_vc->getD("LepGood_eta", _tMuIdx[i] )), _weight);
  }

}


//____________________________________________________________________________
void FakeRatio::fillLeptonPlots(std::string kr){
  /*
    fills the control plots for leptons
    parameters: none
    return: none
  */


  for(int i = 0; i < _nLEls; ++i){
    fill(kr + "_LElDXY", fabs(_vc->getD("LepGood_dxy"     , _lElIdx[i])), _weight);
    fill(kr + "_LElEta", fabs(_vc->getD("LepGood_eta"     , _lElIdx[i])), _weight);
    fill(kr + "_LElIso",      _vc->getD("LepGood_relIso03", _lElIdx[i]) , _weight);
    fill(kr + "_LElPt" ,      _vc->getD("LepGood_pt"      , _lElIdx[i]) , _weight);
    fill(kr + "_LElMT" , Candidate::create( _lEls[i], _met)->mass()     , _weight);
  }

  for(int i = 0; i < _nTEls; ++i){
    fill(kr + "_TElDXY", fabs(_vc->getD("LepGood_dxy"     , _tElIdx[i])), _weight);
    fill(kr + "_TElEta", fabs(_vc->getD("LepGood_eta"     , _tElIdx[i])), _weight);
    fill(kr + "_TElIso",      _vc->getD("LepGood_relIso03", _tElIdx[i]) , _weight);
    fill(kr + "_TElPt" ,      _vc->getD("LepGood_pt"      , _tElIdx[i]) , _weight);
    fill(kr + "_TElMT" , Candidate::create( _tEls[i], _met)->mass()     , _weight);
  }

  for(int i = 0; i < _nLMus; ++i){
    fill(kr + "_LMuDXY", fabs(_vc->getD("LepGood_dxy"     , _lMuIdx[i])), _weight);
    fill(kr + "_LMuEta", fabs(_vc->getD("LepGood_eta"     , _lMuIdx[i])), _weight);
    fill(kr + "_LMuIso",      _vc->getD("LepGood_relIso03", _lMuIdx[i]) , _weight);
    fill(kr + "_LMuPt" ,      _vc->getD("LepGood_pt"      , _lMuIdx[i]) , _weight);
    fill(kr + "_LMuMT" , Candidate::create( _lMus[i], _met)->mass()     , _weight);
  }

  for(int i = 0; i < _nTMus; ++i){
    fill(kr + "_TMuDXY", fabs(_vc->getD("LepGood_dxy"     , _tMuIdx[i])), _weight);
    fill(kr + "_TMuEta", fabs(_vc->getD("LepGood_eta"     , _tMuIdx[i])), _weight);
    fill(kr + "_TMuIso",      _vc->getD("LepGood_relIso03", _tMuIdx[i]) , _weight);
    fill(kr + "_TMuPt" ,      _vc->getD("LepGood_pt"      , _tMuIdx[i]) , _weight);
    fill(kr + "_TMuMT" , Candidate::create( _tMus[i], _met)->mass()     , _weight);
  }

}


//____________________________________________________________________________
void FakeRatio::fillJetPlots(std::string kr){
  /*
    fills the control plots for jets
    parameters: none
    return: none
  */


  for(int i = 0; i < _nJets; ++i){
    fill(kr + "_JetPt"     , _jets[i]->pt(), _weight);
  }

}


//____________________________________________________________________________
int FakeRatio::genMatchCateg(const Candidate* cand) {

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
FakeRatio::HT(){
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
//int 
//FakeRatio::eventCharge(){
//  /*
//    returns the total charge in the event as calculated from the electrons and
//    leptons that have been selected
//    parameters: electron_label, muon_label
//    return: the total charge as int
//  */
//
//  int charge = 0;
//
//  for(int i=0;i<_nEls;++i)
//    charge += _els[i]->charge();
//
//  for(int i=0;i<_nMus;++i)
//    charge += _mus[i]->charge();
//
//  return charge; 
//
//}

