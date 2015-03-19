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
  _vc->registerVar("HLT_SingleMu"                 , "F" );
  _vc->registerVar("HLT_MuEG"                     , "F" );
  _vc->registerVar("HLT_TripleEl"                 , "F" );
  _vc->registerVar("HLT_DoubleEl"                 , "F" );
  _vc->registerVar("HLT_DoubleMu"                 , "F" );
  _vc->registerVar("nVert"                        , "I" );
  _vc->registerVar("nTrueInt"                     , "I" );
  _vc->registerVar("nTrueInt"                     , "I" );
  _vc->registerVar("nBJetPt40Medium"              , "I" );
  _vc->registerVar("puWeight"                     , "F" );
  _vc->registerVar("genWeight"                    , "F" );
  _vc->registerVar("nLepGood"                     , "I" );
  _vc->registerVar("LepGood_pt"                   , "AF");
  _vc->registerVar("LepGood_eta"                  , "AF");
  _vc->registerVar("LepGood_phi"                  , "AF");
  _vc->registerVar("LepGood_charge"               , "AI");
  _vc->registerVar("LepGood_tightCharge"          , "AI");
  _vc->registerVar("LepGood_mediumMuonId"         , "AI");
  _vc->registerVar("LepGood_pdgId"                , "AI");
  _vc->registerVar("LepGood_relIso03"             , "AF");
  _vc->registerVar("LepGood_relIso04"             , "AF");
  _vc->registerVar("LepGood_dxy"                  , "AF");
  _vc->registerVar("LepGood_dz"                   , "AF");
  _vc->registerVar("LepGood_sip3d"                , "AF");
  _vc->registerVar("LepGood_pfMuonId"             , "AI");
  _vc->registerVar("LepGood_tightId"              , "AI");
  _vc->registerVar("LepGood_looseIdSusy"          , "AI");
  _vc->registerVar("LepGood_convVeto"             , "AI");
  _vc->registerVar("LepGood_lostHits"             , "AI");
  _vc->registerVar("LepGood_eleCutIdCSA14_50ns_v1", "AI");
  _vc->registerVar("LepGood_eleCutIdCSA14_50ns_v1", "AI");
  _vc->registerVar("LepGood_eleCutId2012_full5x5" , "AI");
  _vc->registerVar("LepGood_mvaSusyPHYS14"        , "AF");
  _vc->registerVar("LepGood_mvaSusy"              , "AF");
  _vc->registerVar("LepGood_mcMatchId"            , "AI");
  _vc->registerVar("LepGood_mcMatchAny"           , "AI");
  _vc->registerVar("met_pt"                       , "F" );
  _vc->registerVar("met_eta"                      , "F" );
  _vc->registerVar("met_phi"                      , "F" );
  _vc->registerVar("met_mass"                     , "F" );
  _vc->registerVar("nJet25"                       , "I" );
  _vc->registerVar("nJet40"                       , "I" );
  _vc->registerVar("nJet"                         , "I" );
  _vc->registerVar("Jet_id"                       , "AI");
  _vc->registerVar("Jet_pt"                       , "AF");
  _vc->registerVar("Jet_rawPt"                    , "AF");
  _vc->registerVar("Jet_eta"                      , "AF");
  _vc->registerVar("Jet_phi"                      , "AF");
  _vc->registerVar("Jet_mass"                     , "AF");
  _vc->registerVar("Jet_btagCSV"                  , "AF");
  _vc->registerVar("nDiscJet"                     , "I" );
  _vc->registerVar("DiscJet_id"                   , "AI");
  _vc->registerVar("DiscJet_pt"                   , "AF");
  _vc->registerVar("DiscJet_rawPt"                , "AF");
  _vc->registerVar("DiscJet_eta"                  , "AF");
  _vc->registerVar("DiscJet_phi"                  , "AF");
  _vc->registerVar("DiscJet_mass"                 , "AF");
  _vc->registerVar("DiscJet_btagCSV"              , "AF");
  
  //generator informations
  _vc->registerVar("nGenPart"                     , "I" );
  _vc->registerVar("GenPart_pt"                   , "AF");
  _vc->registerVar("GenPart_eta"                  , "AF");
  _vc->registerVar("GenPart_phi"                  , "AF");
  _vc->registerVar("GenPart_pdgId"                , "AI");
  _vc->registerVar("GenPart_motherId"             , "AI");
  _vc->registerVar("GenPart_grandmotherId"        , "AI");

  _vc->registerVar("nGenBHad"                     , "I" );
  _vc->registerVar("GenBHad_pt"                   , "AF"); 
  _vc->registerVar("GenBHad_eta"                  , "AF"); 
  _vc->registerVar("GenBHad_phi"                  , "AF"); 
  _vc->registerVar("GenBHad_pdgId"                , "AI"); 
 
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
  //   _weight *= _vc->get("puWeight");

}


//____________________________________________________________________________
void FakeRatio::run(){



  _lEls   .clear();
  _lntEls .clear();
  _tEls   .clear();
  _vEls   .clear();
  _lLeps  .clear();
  _lntLeps.clear();
  _tLeps  .clear();
  _vLeps  .clear();
  _lMus   .clear();
  _lntMus .clear();
  _tMus   .clear();
  _vMus   .clear();
  _jets   .clear();

  _lElIdx   .clear();
  _lntElIdx .clear();
  _tElIdx   .clear();
  _vElIdx   .clear();
  _lLepIdx  .clear();
  _lntLepIdx.clear();
  _tLepIdx  .clear();
  _vLepIdx  .clear();
  _lMuIdx   .clear();
  _lntMuIdx .clear();
  _tMuIdx   .clear();
  _vMuIdx   .clear();


  counter("denominator");
	
  // prepare event selection
  collectKinematicObjects();
	
	
  // measurement region selection
  //if(!skimSelection()) return;
  //if(!genMRSelection()) return;
  if(!mrSelection()) return;

  //skim right after the basic selection
  //fillSkimTree();
  //return;
 
	
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
  _hm->addVariable("MR_HT"          , 1000,   0.0, 1000.0, "H_T [GeV]"                  ); 
  _hm->addVariable("MR_MET"         , 1000,   0.0, 1000.0, "#slash{E}_T [GeV]"          );
  _hm->addVariable("MR_NBJets"      ,   20,   0.0,   20.0, "b-jet multiplicity"         );
  _hm->addVariable("MR_NLEls"       ,   20,   0.0,   20.0, "loose electron multiplicity");
  _hm->addVariable("MR_NTEls"       ,   20,   0.0,   20.0, "tight electron multiplicity");
  _hm->addVariable("MR_NVEls"       ,   20,   0.0,   20.0, "veto electron multiplicity" );
  _hm->addVariable("MR_NJets"       ,   20,   0.0,   20.0, "jet multiplicity"           );
  _hm->addVariable("MR_NLLeps"      ,   20,   0.0,   20.0, "loose lepton multiplicity"  );
  _hm->addVariable("MR_NTLeps"      ,   20,   0.0,   20.0, "tight lepton multiplicity"  );
  _hm->addVariable("MR_NVLeps"      ,   20,   0.0,   20.0, "veto lepton multiplicity"   );
  _hm->addVariable("MR_NLMus"       ,   20,   0.0,   20.0, "loose muon multiplicity"    );
  _hm->addVariable("MR_NTMus"       ,   20,   0.0,   20.0, "tight muon multiplicity"    );
  _hm->addVariable("MR_NVMus"       ,   20,   0.0,   20.0, "veto muon multiplicity"     );
  _hm->addVariable("MR_NVrtx"       ,   40,   0.0,   40.0, "vertex multiplicity"        );
  _hm->addVariable("MR_LElDXY"      ,   50,   0.0,    0.5, "#||{dxy}(e) [cm]"           );
  _hm->addVariable("MR_LElEta"      ,  240,   0.0,    2.4, "#||{#eta(e)}"               );
  _hm->addVariable("MR_LElIso"      ,   50,   0.0,    1.0, "PF Iso (e)"                 );
  _hm->addVariable("MR_LElMT"       , 1000,   0.0, 1000.0, "M_T(e) [GeV]"               );
  _hm->addVariable("MR_LElPt"       , 1000,   0.0, 1000.0, "P_T(e) [GeV]"               );
  _hm->addVariable("MR_LElMPdg"     ,    8,   0.0,    8.0, "match category"             );
  _hm->addVariable("MR_LNTElDXY"    ,   50,   0.0,    0.5, "#||{dxy}(e) [cm]"           );
  _hm->addVariable("MR_LNTElEta"    ,  240,   0.0,    2.4, "#||{#eta(e)}"               );
  _hm->addVariable("MR_LNTElIso"    ,   50,   0.0,    1.0, "PF Iso (e)"                 );
  _hm->addVariable("MR_LNTElMT"     , 1000,   0.0, 1000.0, "M_T(e) [GeV]"               );
  _hm->addVariable("MR_LNTElPt"     , 1000,   0.0, 1000.0, "P_T(e) [GeV]"               );
  _hm->addVariable("MR_LNTElMPdg"   ,    8,   0.0,    8.0, "match category"             );
  _hm->addVariable("MR_TElDXY"      ,   50,   0.0,    0.5, "#||{dxy}(e) [cm]"           );
  _hm->addVariable("MR_TElEta"      ,  240,   0.0,    2.4, "#||{#eta(e)}"               );
  _hm->addVariable("MR_TElIso"      ,   50,   0.0,    1.0, "PF Iso (e)"                 );
  _hm->addVariable("MR_TElMT"       , 1000,   0.0, 1000.0, "M_T(e) [GeV]"               );
  _hm->addVariable("MR_TElPt"       , 1000,   0.0, 1000.0, "P_T(e) [GeV]"               );
  _hm->addVariable("MR_TElMPdg"     ,    8,   0.0,    8.0, "match category"             );
  _hm->addVariable("MR_LLepDXY"     ,   50,   0.0,    0.5, "#||{dxy}(lep) [cm]"         );
  _hm->addVariable("MR_LLepEta"     ,  240,   0.0,    2.4, "#||{#eta(lep)}"             );
  _hm->addVariable("MR_LLepIso"     ,   50,   0.0,    1.0, "PF Iso (lep)"               );
  _hm->addVariable("MR_LLepMT"      , 1000,   0.0, 1000.0, "M_T(lep) [GeV]"             );
  _hm->addVariable("MR_LLepPt"      , 1000,   0.0, 1000.0, "P_T(lep) [GeV]"             );
  _hm->addVariable("MR_LLepMPdg"    ,    8,   0.0,    8.0, "match category"             );
  _hm->addVariable("MR_LNTLepDXY"   ,   50,   0.0,    0.5, "#||{dxy}(lep) [cm]"         );
  _hm->addVariable("MR_LNTLepEta"   ,  240,   0.0,    2.4, "#||{#eta(lep)}"             );
  _hm->addVariable("MR_LNTLepIso"   ,   50,   0.0,    1.0, "PF Iso (lep)"               );
  _hm->addVariable("MR_LNTLepMT"    , 1000,   0.0, 1000.0, "M_T(lep) [GeV]"             );
  _hm->addVariable("MR_LNTLepPt"    , 1000,   0.0, 1000.0, "P_T(lep) [GeV]"             );
  _hm->addVariable("MR_LNTLepMPdg"  ,    8,   0.0,    8.0, "match category"             );
  _hm->addVariable("MR_TLepDXY"     ,   50,   0.0,    0.5, "#||{dxy}(lep) [cm]"         );
  _hm->addVariable("MR_TLepEta"     ,  240,   0.0,    2.4, "#||{#eta(lep)}"             );
  _hm->addVariable("MR_TLepIso"     ,   50,   0.0,    1.0, "PF Iso (lep)"               );
  _hm->addVariable("MR_TLepMT"      , 1000,   0.0, 1000.0, "M_T(lep) [GeV]"             );
  _hm->addVariable("MR_TLepPt"      , 1000,   0.0, 1000.0, "P_T(lep) [GeV]"             );
  _hm->addVariable("MR_TLepMPdg"    ,    8,   0.0,    8.0, "match category"             );
  _hm->addVariable("MR_LMuDXY"      ,   50,   0.0,    0.5, "#||{dxy}(#mu) [cm]"         );
  _hm->addVariable("MR_LMuEta"      ,  240,   0.0,    2.4, "#||{#eta(#mu)}"             );
  _hm->addVariable("MR_LMuIso"      ,   50,   0.0,    1.0, "PF Iso (#mu)"               );
  _hm->addVariable("MR_LMuMT"       , 1000,   0.0, 1000.0, "M_T(#mu) [GeV]"             );
  _hm->addVariable("MR_LMuPt"       , 1000,   0.0, 1000.0, "P_T(#mu) [GeV]"             );
  _hm->addVariable("MR_LMuMPdg"     ,    8,   0.0,    8.0, "match category"             );
  _hm->addVariable("MR_LNTMuDXY"    ,   50,   0.0,    0.5, "#||{dxy}(#mu) [cm]"         );
  _hm->addVariable("MR_LNTMuEta"    ,  240,   0.0,    2.4, "#||{#eta(#mu)}"             );
  _hm->addVariable("MR_LNTMuIso"    ,   50,   0.0,    1.0, "PF Iso (#mu)"               );
  _hm->addVariable("MR_LNTMuMT"     , 1000,   0.0, 1000.0, "M_T(#mu) [GeV]"             );
  _hm->addVariable("MR_LNTMuPt"     , 1000,   0.0, 1000.0, "P_T(#mu) [GeV]"             );
  _hm->addVariable("MR_LNTMuMPdg"   ,    8,   0.0,    8.0, "match category"             );
  _hm->addVariable("MR_TMuDXY"      ,   50,   0.0,    0.5, "#||{dxy}(#mu) [cm]"         );
  _hm->addVariable("MR_TMuEta"      ,  240,   0.0,    2.4, "#||{#eta(#mu)}"             );
  _hm->addVariable("MR_TMuIso"      ,   50,   0.0,    1.0, "PF Iso (#mu)"               );
  _hm->addVariable("MR_TMuMT"       , 1000,   0.0, 1000.0, "M_T(#mu) [GeV]"             );
  _hm->addVariable("MR_TMuPt"       , 1000,   0.0, 1000.0, "P_T(#mu) [GeV]"             );
  _hm->addVariable("MR_TMuMPdg"     ,    8,   0.0,    8.0, "match category"             );
  _hm->addVariable("MR_JetCSVBTag"  ,   50,   0.0,    1.0, "jet CSV B-Tag"              );
  _hm->addVariable("MR_JetPt"       , 1000,   0.0, 1000.0, "P_T(jet) [GeV]"             );
  _hm->addVariable("MR_LElMap"      , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)"  );
  _hm->addVariable("MR_LElMapBJM"   , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)"  );
  _hm->addVariable("MR_LLepMap"     , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(lep)", "#||{#eta}(lep)");
  _hm->addVariable("MR_LLepMapBJM"  , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(lep)", "#||{#eta}(lep)");
  _hm->addVariable("MR_LMuMap"      , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");
  _hm->addVariable("MR_LMuMapBJM"   , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");
  _hm->addVariable("MR_LNTElMap"    , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)"  );
  _hm->addVariable("MR_LNTElMapBJM" , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)"  );
  _hm->addVariable("MR_LNTLepMap"   , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(lep)", "#||{#eta}(lep)");
  _hm->addVariable("MR_LNTLepMapBJM", bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(lep)", "#||{#eta}(lep)");
  _hm->addVariable("MR_LNTMuMap"    , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");
  _hm->addVariable("MR_LNTMuMapBJM" , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");
  _hm->addVariable("MR_TElMap"      , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)"  );
  _hm->addVariable("MR_TElMapBJM"   , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)"  );
  _hm->addVariable("MR_TLepMap"     , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(lep)", "#||{#eta}(lep)");
  _hm->addVariable("MR_TLepMapBJM"  , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(lep)", "#||{#eta}(lep)");
  _hm->addVariable("MR_TMuMap"      , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");
  _hm->addVariable("MR_TMuMapBJM"   , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");
  
}



//____________________________________________________________________________
void FakeRatio::writeOutput(){
  /*
    writes all output of this class to the disk
    paramters: none
    return: none
  */
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
  
  for(int i = 0; i < _vc->get("nLepGood"); ++i){

    bool loose = false;
    bool tight = false;

    // electrons
    if(std::abs(_vc->get("LepGood_pdgId", i)) == 11){		  
      if(looseElectronSelection(i)) {
        _lEls.push_back( Candidate::create(_vc->get("LepGood_pt", i),
					  _vc->get("LepGood_eta", i),
					  _vc->get("LepGood_phi", i),
					  _vc->get("LepGood_pdgId", i),
					  _vc->get("LepGood_charge", i),
					  0.0005) );
        _lElIdx.push_back(i);
        _lLeps.push_back( _lEls[ _lEls.size()-1 ] );
        _lLepIdx.push_back(i);
        loose = true;
      }
      if(tightElectronSelection(i)) {
        _tEls.push_back( Candidate::create(_vc->get("LepGood_pt", i),
					  _vc->get("LepGood_eta", i),
					  _vc->get("LepGood_phi", i),
					  _vc->get("LepGood_pdgId", i),
					  _vc->get("LepGood_charge", i),
					  0.0005) );
        _tElIdx.push_back(i);
        _tLeps.push_back( _tEls[ _tEls.size()-1 ] );
        _tLepIdx.push_back(i);
        tight = true;
      }
      if(loose && !tight) {
        _lntEls.push_back( Candidate::create(_vc->get("LepGood_pt", i),
					  _vc->get("LepGood_eta", i),
					  _vc->get("LepGood_phi", i),
					  _vc->get("LepGood_pdgId", i),
					  _vc->get("LepGood_charge", i),
					  0.0005) );
        _lntElIdx.push_back(i);
        _lntLeps.push_back( _lntEls[ _lntEls.size()-1 ] );
        _lntLepIdx.push_back(i);
      }
      if(!loose && !tight) {
        if(vetoElectronSelection(i))  {
          _vEls.push_back( Candidate::create(_vc->get("LepGood_pt", i),
					     _vc->get("LepGood_eta", i),
					     _vc->get("LepGood_phi", i),
					     _vc->get("LepGood_pdgId", i),
					     _vc->get("LepGood_charge", i),
					     0.0005) );
          _vLeps.push_back( _vEls[ _vEls.size()-1 ] );
        }
      }		
    }

    // muons
    else if(std::abs(_vc->get("LepGood_pdgId", i)) == 13){
      if(looseMuonSelection(i)) {
        _lMus.push_back( Candidate::create(_vc->get("LepGood_pt", i),
					  _vc->get("LepGood_eta", i),
					  _vc->get("LepGood_phi", i),
					  _vc->get("LepGood_pdgId", i),
					  _vc->get("LepGood_charge", i),
					  0.105) );
        _lMuIdx.push_back(i);
        _lLeps.push_back( _lMus[ _lMus.size()-1 ] );
        _lLepIdx.push_back(i);
        loose = true;
      }
      if(tightMuonSelection(i)) {
        _tMus.push_back( Candidate::create(_vc->get("LepGood_pt", i),
					  _vc->get("LepGood_eta", i),
					  _vc->get("LepGood_phi", i),
					  _vc->get("LepGood_pdgId", i),
					  _vc->get("LepGood_charge", i),
					  0.105) );
        _tMuIdx.push_back(i);
        _tLeps.push_back( _tMus[ _tMus.size()-1 ] );
        _tLepIdx.push_back(i);
        tight = true;
      }
      if(loose && !tight) {
        _lntMus.push_back( Candidate::create(_vc->get("LepGood_pt", i),
					  _vc->get("LepGood_eta", i),
					  _vc->get("LepGood_phi", i),
					  _vc->get("LepGood_pdgId", i),
					  _vc->get("LepGood_charge", i),
					  0.105) );
        _lntMuIdx.push_back(i);
        _lntLeps.push_back( _lntMus[ _lntMus.size()-1 ] );
        _lntLepIdx.push_back(i);
      }
      if(!loose && !tight) {
        if(vetoMuonSelection(i))  {
          _vMus.push_back( Candidate::create(_vc->get("LepGood_pt", i),
					     _vc->get("LepGood_eta", i),
					     _vc->get("LepGood_phi", i),
					     _vc->get("LepGood_pdgId", i),
					     _vc->get("LepGood_charge", i),
					     0.105) );
          _vLeps.push_back( _vMus[ _vMus.size()-1 ] );
        }
      }
    }
  }

  _nLEls    = _lEls   .size();
  _nLNTEls  = _lntEls .size();
  _nTEls    = _tEls   .size();
  _nVEls    = _vEls   .size();
  _nLLeps   = _lLeps  .size();
  _nLNTLeps = _lntLeps.size();
  _nTLeps   = _tLeps  .size();
  _nVLeps   = _vLeps  .size();
  _nLMus    = _lMus   .size();
  _nLNTMus  = _lntMus .size();
  _nTMus    = _tMus   .size();
  _nVMus    = _vMus   .size();
  
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
bool FakeRatio::goodJetSelection(int jetIdx){
  /*
    does the selection of good jets, i.e. minimum selection of jets 
    parameters: jetIdx
    return: true (if the jet is good), false (else)
  */
  
  counter("JetDenominator", kJetId);

  if(!makeCut<float>(_vc->get("Jet_pt", jetIdx)       , 40.0, ">", "pt selection" , 0, kJetId) ) return false;
  if(!makeCut<float>(fabs(_vc->get("Jet_eta", jetIdx)),  2.4, "<", "eta selection", 0, kJetId) ) return false;

  // CH: the requirement that dR(jet, every loose leption) > 1.0 in combination with
  // requirering EXACTLY one loose lepton in the measurement region, makes jet-lepton
  // cleaning obsolete
  
  for(unsigned int il=0; il<_lLeps.size(); ++il){
    float dr = KineUtils::dR(_lLeps[il]->eta(), _vc->get("Jet_eta", jetIdx),
			                 _lLeps[il]->phi(), _vc->get("Jet_phi", jetIdx));
    if(!makeCut<float>(dr, 1.0, ">", "dR selection", 0, kJetId) ) return false;
  }

  // CH: this is jet-lepton cleaning: removing the closest jet to the loose lepton if dR < 0.4
  float dr_cache = 999999.;
  for(unsigned int il=0; il<_lLeps.size(); ++il){
    float dr = KineUtils::dR(_lLeps[il]->eta(), _vc->get("Jet_eta", jetIdx),
			                 _lLeps[il]->phi(), _vc->get("Jet_phi", jetIdx));
    if(dr < dr_cache)
      dr_cache = dr;
  }
  for(unsigned int il=0; il<_vLeps.size(); ++il){
    float dr = KineUtils::dR(_vLeps[il]->eta(), _vc->get("Jet_eta", jetIdx),
			                 _vLeps[il]->phi(), _vc->get("Jet_phi", jetIdx));
    if(dr < dr_cache)
      dr_cache = dr;
  }
  if(!makeCut<float>(dr_cache, 0.4, ">", "dR selection, jet-lepton cleaning", 0, kJetId) ) return false;

  return true;

}


//____________________________________________________________________________
bool FakeRatio::leptonMvaId(int lepIdx, int cat, float iso, float dxy, bool tch){

  //if(!makeCut<float>( _vc->get("LepGood_mvaSusyPHYS14", lepIdx)      , 0.53, ">" , "MVA POG WP"      , 0, cat)) return false;
  bool mvawp = false;
  if((_vc->get("LepGood_pt", lepIdx) > 25 && _vc->get("LepGood_mvaSusy", lepIdx) > 0) || _vc->get("LepGood_mvaSusy", lepIdx) > 0.53) mvawp = true;

  if(!makeCut( mvawp, "MVA WP", "=", cat)) return false;
  if(tch){
    if(!makeCut<int>( _vc->get("LepGood_tightCharge"  , lepIdx)      , 1   , ">" , "charge selection", 0, cat)) return false;
    if(!makeCut<int>( _vc->get("LepGood_mediumMuonId" , lepIdx)      , 1   , "=" , "medium muon ID"  , 0, cat)) return false;
  }
  if(!makeCut<float>( _vc->get("LepGood_relIso03"     , lepIdx)      , iso , "<" , "isolation "      , 0, cat)) return false;
  if(!makeCut<float>(std::abs(_vc->get("LepGood_dxy"  , lepIdx))     , dxy , "<" , "dxy selection"   , 0, cat)) return false;
  if(!makeCut<float>( _vc->get("LepGood_sip3d"        , lepIdx)      , 4   , "<" , "SIP 3D"          , 0, cat)) return false;

  return true;

}


//____________________________________________________________________________
bool FakeRatio::leptonCutId(int lepIdx, int cat, float iso, float dxy, string cbid, int id){

  if(!makeCut<int>(       _vc->get(cbid              , lepIdx) , id , ">=", "cut-based WP" , 0, cat)) return false;
  if(!makeCut<float>(     _vc->get("LepGood_relIso03", lepIdx) , iso, "<" , "isolation "   , 0, cat)) return false;
  if(!makeCut<float>(fabs(_vc->get("LepGood_dxy"     , lepIdx)), dxy, "<" , "dxy selection", 0, cat)) return false;
  if(!makeCut<float>(fabs(_vc->get("LepGood_dz"      , lepIdx)), 0.1, "<" , "dz selection" , 0, cat)) return false;
  if(!makeCut<float>(     _vc->get("LepGood_sip3d"   , lepIdx) , 4  , "<" , "SIP 3D"       , 0, cat)) return false;

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



  //CH: denominator is cut-based denominator for all IDs
  //CH: actually, we only take the tight ID (muons) / medium ID (electrons) on top of the LepGood preselection
  
  if(!makeCut<float>( _vc->get("LepGood_pt"      , elIdx)            , 10.   , ">"  , "pt selection"      , 0    , kLElId)) return false;
  if(!makeCut<float>( std::abs(_vc->get("LepGood_eta", elIdx))       , 2.4   , "<"  , "eta selection"     , 0    , kLElId)) return false;
  if(!makeCut<float>( std::abs(_vc->get("LepGood_eta", elIdx))       , 1.4442, "[!]", "eta selection veto", 1.566, kLElId)) return false;

  //CH: using iso<0.5, dxy<0.05 as they are in the tree 
  //CH: denominator is cut-based denominator for all IDs
  //bool mvaid = leptonMvaId(elIdx, kLElId, 999.0, 999.0);
  bool cutid = leptonCutId(elIdx, kLElId, 0.5, 0.05, "LepGood_eleCutId2012_full5x5", 3);
  bool lepid = false;

  //if     (_mva == "mva"      ) lepid = mvaid;
  //else if(_mva == "mvanotcut") lepid = mvaid && !cutid;
  //else if(_mva == "mvaandcut") lepid = mvaid && cutid;
  //else if(_mva == "cut"      ) lepid = cutid;
  //else if(_mva == "cutnotmva") lepid = cutid && !mvaid;
  //else if(_mva == "cutandmva") lepid = cutid && mvaid;
 
  lepid = cutid;
  if(!makeCut( lepid, "lepton id", "=", kLElId)) return false;

  if(!makeCut<int>( _vc->get("LepGood_tightCharge", elIdx)           , 1     , ">"  , "charge selection"  , 0    , kLElId)) return false;
  
  bool conv= (_vc->get("LepGood_convVeto", elIdx) > 0 && _vc->get("LepGood_lostHits", elIdx)==0);
  if(!makeCut( conv, "conversion rejection", "=", kLElId)) return false;

  // electron cleaning 
  for(unsigned int il=elIdx+1; il<_vc->get("nLepGood"); ++il){
    float dr = KineUtils::dR(_vc->get("LepGood_eta", il), _vc->get("LepGood_eta", elIdx),
			                 _vc->get("LepGood_phi", il), _vc->get("LepGood_phi", elIdx));
    if(std::abs(_vc->get("LepGood_pdgId")) == 13 && !makeCut<float>(dr, 0.2, ">", "electron cleaning selection", 0, kLElId) ) return false;
  }
  
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
  
  //CH: denominator is cut-based denominator for all IDs
  //CH: actually, we only take the tight ID (muons) / medium ID (electrons) on top of the LepGood preselection

  if(!makeCut<float>( _vc->get("LepGood_pt"         , muIdx)    , 10. , ">" , "pt selection"    , 0, kLMuId)) return false;
  if(!makeCut<float>( _vc->get("LepGood_eta"        , muIdx)    , 2.4 , "<" , "eta selection"   , 0, kLMuId)) return false;

  //CH: using iso<0.5, dxy<0.05 as they are in the tree 
  //bool mvaid = leptonMvaId(muIdx, kLMuId, 999.0, 999.0);
  bool cutid = leptonCutId(muIdx, kLMuId, 0.5, 0.05, "LepGood_tightId", 1);
  bool lepid = false;

  //if     (_mva == "mva"      ) lepid = mvaid;
  //else if(_mva == "mvanotcut") lepid = mvaid && !cutid;
  //else if(_mva == "mvaandcut") lepid = mvaid && cutid;
  //else if(_mva == "cut"      ) lepid = cutid;
  //else if(_mva == "cutnotmva") lepid = cutid && !mvaid;
  //else if(_mva == "cutandmva") lepid = cutid && mvaid;
 
  lepid = cutid;
 
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
 
  if(!makeCut<float>( _vc->get("LepGood_pt"      , elIdx)            , 10.   , ">"  , "pt selection"      , 0    , kTElId)) return false;
  if(!makeCut<float>( std::abs(_vc->get("LepGood_eta", elIdx))       , 2.4   , "<"  , "eta selection"     , 0    , kTElId)) return false;
  if(!makeCut<float>( std::abs(_vc->get("LepGood_eta", elIdx))       , 1.4442, "[!]", "eta selection veto", 1.566, kTElId)) return false;

  bool mvaid = leptonMvaId(elIdx, kTElId, 0.1, 0.01, false);
  bool cutid = leptonCutId(elIdx, kTElId, 0.1, 0.01, "LepGood_eleCutId2012_full5x5", 3);
  bool lepid = false;

  if     (_mva == "mva"      ) lepid = mvaid;
  else if(_mva == "mvanotcut") lepid = mvaid && !cutid;
  else if(_mva == "mvaandcut") lepid = mvaid && cutid;
  else if(_mva == "cut"      ) lepid = cutid;
  else if(_mva == "cutnotmva") lepid = cutid && !mvaid;
  else if(_mva == "cutandmva") lepid = cutid && mvaid;
 
  if(!makeCut( lepid, "lepton id", "=", kTElId)) return false;

  if(!makeCut<int>( _vc->get("LepGood_tightCharge", elIdx)           , 1     , ">"  , "charge selection"  , 0    , kTElId)) return false;

  bool conv= (_vc->get("LepGood_convVeto", elIdx) > 0 && _vc->get("LepGood_lostHits", elIdx)==0);
  if(!makeCut( conv, "conversion rejection", "=", kTElId)) return false; 

  // electron cleaning 
  for(unsigned int il=elIdx+1; il<_vc->get("nLepGood"); ++il){
    float dr = KineUtils::dR(_vc->get("LepGood_eta", il), _vc->get("LepGood_eta", elIdx),
			                 _vc->get("LepGood_phi", il), _vc->get("LepGood_phi", elIdx));
    if(std::abs(_vc->get("LepGood_pdgId")) == 13 && !makeCut<float>(dr, 0.2, ">", "electron cleaning selection", 0, kTElId) ) return false;
  }
  
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
  
  if(!makeCut<float>( _vc->get("LepGood_pt"      , muIdx)      , 10. , ">" , "pt selection"  , 0, kTMuId)) return false;
  if(!makeCut<float>( _vc->get("LepGood_eta"     , muIdx)      , 2.4 , "<" , "eta selection" , 0, kTMuId)) return false;

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

  if(!makeCut<float>( _vc->get("LepGood_pt", elIdx), 10.0, "<", "pt selection", 0, kVElId)) return false; 

  // electron cleaning 
  for(unsigned int il=elIdx+1; il<_vc->get("nLepGood"); ++il){
    float dr = KineUtils::dR(_vc->get("LepGood_eta", il), _vc->get("LepGood_eta", elIdx),
			                 _vc->get("LepGood_phi", il), _vc->get("LepGood_phi", elIdx));
    if(std::abs(_vc->get("LepGood_pdgId")) == 13 && !makeCut<float>(dr, 0.2, ">", "electron cleaning selection", 0, kTElId) ) return false;
  }
  
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
 
  if(!makeCut<float>( _vc->get("LepGood_pt", muIdx), 10.0, "<", "pt selection", 0, kVMuId)) return false; 
  
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

  if(_mva=="mva" || _mva == "mvanotcut") _mvaId=true;


  if(_btag=="" || _btag=="std" || _btag=="40")
    _bvar = "nBJetMedium40";
  else if(_btag=="25" || _btag=="soft25")
    _bvar = "nBJetMedium25";
  else if(_btag=="15" || _btag=="IVF")
    _bvar = "nSoftBJetMedium25";
  else
    _bvar = "nBJetMedium40";
  
  if(_MR == "MR00") {
    setCut("NBJetsMR", 0, "=");
  }
  else if(_MR == "MR01") {
    setCut("NBJetsMR", 1, "=");
  }
  else if(_MR == "MR02") {
    setCut("NBJetsMR", 2, "=");
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
bool FakeRatio::genMRSelection(){


//cout << "-------- new event (" << _vc->get("run") << ":" << _vc->get("lumi") << ":" << _vc->get("evt") << ") -------" << endl;

  // gen matching: if defined, only use the fake lepton, and remove all others from the collection
  if( _sampleName.find("fake") != (size_t) -1){
  //if( _sampleName.find("DYJets")!=(size_t)-1 || _sampleName.find("TTJets")!=(size_t)-1  || _sampleName.find("WJets")!=(size_t)-1 ) {

//for(int i = 0; i < _lLeps.size(); ++i){
//  cout << "printing reco lepton " << _lLepIdx[i] << ":" << _lLeps[i]->pdgId() << ":" << _lLeps[i]->pt() << ":" << _lLeps[i]->eta() << ":" << _lLeps[i]->phi() << endl;
//}

    CandList n_lLeps   = _lLeps;
    CandList n_lntLeps = _lntLeps;
    CandList n_tLeps   = _tLeps;
    vector<int> n_lLepIdx   = _lLepIdx;
    vector<int> n_lntLepIdx = _lntLepIdx;
    vector<int> n_tLepIdx   = _tLepIdx;
  
    CandList n_lEls   = _lEls;
    CandList n_lntEls = _lntEls;
    CandList n_tEls   = _tEls;
    vector<int> n_lElIdx   = _lElIdx;
    vector<int> n_lntElIdx = _lntElIdx;
    vector<int> n_tElIdx   = _tElIdx;

    CandList n_lMus   = _lMus;
    CandList n_lntMus = _lntMus;
    CandList n_tMus   = _tMus;
    vector<int> n_lMuIdx   = _lMuIdx;
    vector<int> n_lntMuIdx = _lntMuIdx;
    vector<int> n_tMuIdx   = _tMuIdx;

    _lLeps  .clear();
    _lntLeps.clear();
    _tLeps  .clear();
    _lLepIdx  .clear();
    _lntLepIdx.clear();
    _tLepIdx  .clear();

    _lEls  .clear();
    _lntEls.clear();
    _tEls  .clear();
    _lElIdx  .clear();
    _lntElIdx.clear();
    _tElIdx  .clear();

    _lMus  .clear();
    _lntMus.clear();
    _tMus  .clear();
    _lMuIdx  .clear();
    _lntMuIdx.clear();
    _tMuIdx  .clear();



	for(unsigned int i = 0; i < n_lEls.size(); ++i){
      //int lep_id = 0;
      if(_vc->get("LepGood_mcMatchId", n_lElIdx[i]) == 0){ 
      //lep_id = genMatchCateg( n_lEls[i], "MR_LElMPdg");
      //if(lep_id == kMisMatchPdgId){
        _lEls  .push_back(n_lEls[i]  );
        _lElIdx.push_back(n_lElIdx[i]);
      }
    }
	for(unsigned int i = 0; i < n_lntEls.size(); ++i){
      //int lep_id = 0; 
      if(_vc->get("LepGood_mcMatchId", n_lntElIdx[i]) == 0){ 
      //lep_id = genMatchCateg( n_lntEls[i], "MR_LNTElMPdg");
      //if(lep_id == kMisMatchPdgId){
        _lntEls  .push_back(n_lntEls[i]  );
        _lntElIdx.push_back(n_lntElIdx[i]);
      }
    }
	for(unsigned int i = 0; i < n_tEls.size(); ++i){
	  ////int lep_id = 0; 
      if(_vc->get("LepGood_mcMatchId", n_tElIdx[i]) == 0){ 
      //lep_id = genMatchCateg( n_tEls[i], "MR_TElMPdg" );
      //if(lep_id == kMisMatchPdgId){
        _tEls  .push_back(n_tEls[i]  );
        _tElIdx.push_back(n_tElIdx[i]);
      }
    }
	for(unsigned int i = 0; i < n_lLeps.size(); ++i){
      //int lep_id = 0; 
      if(_vc->get("LepGood_mcMatchId", n_lLepIdx[i]) == 0){ 
      //lep_id = genMatchCateg( n_lLeps[i], "MR_LLepMPdg");
      //if(lep_id == kMisMatchPdgId){
        _lLeps  .push_back(n_lLeps[i]  );
        _lLepIdx.push_back(n_lLepIdx[i]);
      }
    }
//for(int i = 0; i < _vc->get("nGenPart"); ++i){
//  cout << "printing gen lepton " << i << ":" << _vc->get("GenPart_pdgId", i) << ":" << _vc->get("GenPart_pt", i) << ":" << _vc->get("GenPart_eta", i) << ":" << _vc->get("GenPart_phi", i) << endl;
//}
//for(int i = 0; i < _lLeps.size(); ++i){
//  cout << "printing matched lepton " << _lLepIdx[i] << ":" << _lLeps[i]->pdgId() << ":" << _lLeps[i]->pt() << ":" << _lLeps[i]->eta() << ":" << _lLeps[i]->phi() << endl;
//}
//for(int i = 0; i < n_lLeps.size(); ++i){
//  if(find(_lLepIdx.begin(), _lLepIdx.end(), n_lLepIdx[i]) == _lLepIdx.end())
//  cout << "printing removed lepton " << n_lLepIdx[i] << ":" << n_lLeps[i]->pdgId() << ":" << n_lLeps[i]->pt() << ":" << n_lLeps[i]->eta() << ":" << n_lLeps[i]->phi() << endl;
//}
	for(unsigned int i = 0; i < n_lntLeps.size(); ++i){
      //int lep_id = 0; 
      if(_vc->get("LepGood_mcMatchId", n_lntLepIdx[i]) == 0){ 
      //lep_id = genMatchCateg( n_lntLeps[i], "MR_LNTLepMPdg");
      //if(lep_id == kMisMatchPdgId){
        _lntLeps  .push_back(n_lntLeps[i]  );
        _lntLepIdx.push_back(n_lntLepIdx[i]);
      }
    }
	for(unsigned int i = 0; i < n_tLeps.size(); ++i){
      //int lep_id = 0; 
      if(_vc->get("LepGood_mcMatchId", n_tLepIdx[i]) == 0){ 
      //lep_id = genMatchCateg( n_tLeps[i], "MR_TLepMPdg" );
      //if(lep_id == kMisMatchPdgId){
        _tLeps  .push_back(n_tLeps[i]  );
        _tLepIdx.push_back(n_tLepIdx[i]);
      }
    }
	for(unsigned int i = 0; i < n_lMus.size(); ++i){
      //int lep_id = 0; 
      if(_vc->get("LepGood_mcMatchId", n_lMuIdx[i]) == 0){ 
      //lep_id = genMatchCateg( n_lMus[i], "MR_LMuMPdg");
      //if(lep_id == kMisMatchPdgId){
        _lMus  .push_back(n_lMus[i]  );
        _lMuIdx.push_back(n_lMuIdx[i]);
      }
    }
	for(unsigned int i = 0; i < n_lntMus.size(); ++i){
      //int lep_id = 0; 
      if(_vc->get("LepGood_mcMatchId", n_lntMuIdx[i]) == 0){ 
      //lep_id = genMatchCateg( n_lntMus[i], "MR_LNTMuMPdg");
      //if(lep_id == kMisMatchPdgId){
        _lntMus  .push_back(n_lntMus[i]  );
        _lntMuIdx.push_back(n_lntMuIdx[i]);
      }
    }
	for(unsigned int i = 0; i < n_tMus.size(); ++i){ //int lep_id = 0; 
      if(_vc->get("LepGood_mcMatchId", _tMuIdx[i]) == 0){ 
      //lep_id = genMatchCateg( n_tMus[i], "MR_TMuMPdg" );
      //if(lep_id == kMisMatchPdgId){
        _tMus  .push_back(n_tMus[i]  );
        _tMuIdx.push_back(n_tMuIdx[i]);
      }
    }

    _nLEls    = _lEls  .size();
    _nLNTEls  = _lntEls.size();
    _nTEls    = _tEls  .size();
    _nLLeps   = _lLeps  .size();
    _nLNTLeps = _lntLeps.size();
    _nTLeps   = _tLeps  .size();
    _nLMus    = _lMus  .size();
    _nLNTMus  = _lntMus.size();
    _nTMus    = _tMus  .size();
  }  
  counter("genCateg selection");

  // lepton multiplicity
  if     (_lepflav=="all" && !makeCut<int>( _nLLeps          , 1   , "=", "lepton multiplicity and flavor" ) ) return false; 
  else if(_lepflav=="e"   && !makeCut( _nLEls==1 && _nLMus==0, true, "=", "lepton multiplicity and flavor" ) ) return false;
  else if(_lepflav=="m"   && !makeCut( _nLEls==0 && _nLMus==1, true, "=", "lepton multiplicity and flavor" ) ) return false;

  // jet multiplicity
  if(!makeCut<int>( _nJets       , 1   , ">=", "jet multiplicity") ) return false; 

  // b-jet multiplicity
  if(!makeCut<int>(_vc->get(_bvar), _valCutNBJetsMR, _cTypeNBJetsMR, "b-jet multiplicity", _upValCutNBJetsMR) ) return false;


// counting fakes
  if(std::abs(_lLeps[0]->pdgId()) == 11) counter("event with one fake electron");
  if(std::abs(_lLeps[0]->pdgId()) == 13) counter("event with one fake muon");
bool foundZ = false;
bool foundtau = false;
bool foundtaup = false;
bool foundW = false;
bool foundWp = false;
bool foundtaun = false;
bool foundtaunp = false;
for(int i = 0; i < _vc->get("nGenPart"); ++i){
  if(_vc->get("GenPart_pdgId", i) == 15) foundtau = true;
  if(_vc->get("GenPart_pdgId", i) == -15) foundtaup = true;
  if(_vc->get("GenPart_pdgId", i) == 23) foundZ = true;
  if(_vc->get("GenPart_pdgId", i) == 24) foundW = true;
  if(_vc->get("GenPart_pdgId", i) == -24) foundWp = true;
  if(_vc->get("GenPart_pdgId", i) == 16) foundtaun = true;
  if(_vc->get("GenPart_pdgId", i) == -16) foundtaunp = true;
}
if(foundZ && foundtau && foundtaup){ 
  if(std::abs(_lLeps[0]->pdgId()) == 11) counter("event with one fake electron and Z->tautau");
  if(std::abs(_lLeps[0]->pdgId()) == 13) counter("event with one fake muon and Z->tautau");
}

  // no Z->tautau or W->taunu events
  if(!makeCut((!foundZ || !foundtau || !foundtaup) && (!foundW || !foundtaup || !foundtaun) && (!foundWp || !foundtau || !foundtaunp), true, "=", "no Z->tautau and W->taunu events") ) return false;

// looking for the source of muons
//if(std::abs(_lLeps[0]->pdgId()) == 13){
//cout << "-------- new event (" << _vc->get("run") << ":" << _vc->get("lumi") << ":" << _vc->get("evt") << ") -------" << endl;
//cout << "reco lepton: " << _lLepIdx[0] << ":" << _lLeps[0]->pdgId() << ":" << _lLeps[0]->pt() << ":" << _lLeps[0]->eta() << ":" << _lLeps[0]->phi() << endl;
//++mucounter;
//for(int i = 0; i < _vc->get("nGenPart"); ++i){
//  if(std::abs(_vc->get("GenPart_pdgId", i)) != 15) continue;
//  float dr = Tools::dR(_lLeps[0]->eta(), _vc->get("GenPart_eta",i),
//                       _lLeps[0]->phi(), _vc->get("GenPart_phi",i));
//  cout << "looping on GenPart " << i << ":" << _vc->get("GenPart_pdgId", i) << ":" << _vc->get("GenPart_pt",i) << ":" << _vc->get("GenPart_eta",i) << ":" << _vc->get("GenPart_phi",i) << ":" << dr << endl;
//}
//for(int i = 0; i < _vc->get("nGenBHad"); ++i){
//  float dr = Tools::dR(_lLeps[0]->eta(), _vc->get("GenBHad_eta",i),
//                       _lLeps[0]->phi(), _vc->get("GenBHad_phi",i));
//  cout << "looping on GenBHad " << i << ":" << _vc->get("GenBHad_pdgId", i) << ":" << _vc->get("GenBHad_pt",i) << ":" << _vc->get("GenBHad_eta",i) << ":" << _vc->get("GenBHad_phi",i) << ":" << dr << endl;
//}
//}

  return true;

}


//____________________________________________________________________________
bool FakeRatio::mrSelection(){


  // triggers  
  if(_isData && !makeCut<int>(_vc->get("HLT_DoubleMu"), 1, "=", "HLT DoubleMu") ) return false;	
  if(_isData && !makeCut<int>(_vc->get("HLT_DoubleEl"), 1, "=", "HLT DoubleEl") ) return false;	
  if(_isData && !makeCut<int>(_vc->get("HLT_MuEG")    , 1, "=", "HLT MuEG"    ) ) return false;	

  // lepton multiplicity
  if     (_lepflav=="all" && !makeCut<int>( _nLLeps          , 1   , "=", "lepton multiplicity and flavor" ) ) return false;  
  else if(_lepflav=="e"   && !makeCut( _nLEls==1 && _nLMus==0, true, "=", "lepton multiplicity and flavor" ) ) return false;
  else if(_lepflav=="m"   && !makeCut( _nLEls==0 && _nLMus==1, true, "=", "lepton multiplicity and flavor" ) ) return false;

  // veto leptons 
  if(!makeCut<int>( _nVLeps      , 0   , "=" , "lepton veto"     ) ) return false; 

  // jet multiplicity
  if(!makeCut<int>( _nJets       , 1   , ">=", "jet multiplicity") ) return false; 

  // b-jet multiplicity
  if(!makeCut<int>(_vc->get(_bvar), _valCutNBJetsMR, _cTypeNBJetsMR, "b-jet multiplicity", _upValCutNBJetsMR) ) return false;

  // MET 
  if(!makeCut<float>( _met->pt() , 20.0, "<" , "MET selection"   ) ) return false;

  // MT
  Candidate* MT = nullptr;
  MT = Candidate::create( _lLeps[0], _met);
  if(!makeCut<float>( MT->mass() , 20.0, "<" , "MT selection"    ) ) return false;

  return true;

}

//____________________________________________________________________________
bool FakeRatio::skimSelection(){

  // triggers  
  if(_isData && !makeCut<int>(_vc->get("HLT_DoubleMu"), 1, "=", "HLT DoubleMu") ) return false;	
  if(_isData && !makeCut<int>(_vc->get("HLT_DoubleEl"), 1, "=", "HLT DoubleEl") ) return false;	
  if(_isData && !makeCut<int>(_vc->get("HLT_MuEG")    , 1, "=", "HLT MuEG"    ) ) return false;	

  // lepton multiplicity
  if     (_lepflav=="all" && !makeCut<int>( _nLLeps          , 1   , "=", "lepton multiplicity and flavor" ) ) return false; 
  else if(_lepflav=="e"   && !makeCut( _nLEls==1 && _nLMus==0, true, "=", "lepton multiplicity and flavor" ) ) return false;
  else if(_lepflav=="m"   && !makeCut( _nLEls==0 && _nLMus==1, true, "=", "lepton multiplicity and flavor" ) ) return false;

  // veto leptons 
  if(!makeCut<int>( _nVLeps      , 0   , "=" , "lepton veto"     ) ) return false; 

  // jet multiplicity
  if(!makeCut<int>( _nJets       , 1   , ">=", "jet multiplicity") ) return false; 

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

  // need to look in the DiscJet collection as those are the
  // jets that have been cleaned in the JetAnalyzer of CMGTools
  for(int j = 0; j < _vc->get("nDiscJet"); ++j){
    float dr = KineUtils::dR(_vc->get("LepGood_eta", lepIdx), _vc->get("DiscJet_eta", j),
                             _vc->get("LepGood_phi", lepIdx), _vc->get("DiscJet_phi", j));
    if(dr < dr_cache){
      dr_cache   = dr;
      btag_cache = _vc->get("DiscJet_btagCSV", j);
    }
  }

  // combinedInclusiveSecondaryVertexV2BJetTags medium WP
  if(btag_cache > 0.814) return true;

  return false;

}


//____________________________________________________________________________
void FakeRatio::fillEventPlots(std::string kr){
  /*
    fills the control plots for event quantities
    parameters: none
    return: none
  */

  fill(kr + "_HT"        , _HT                 , _weight);
  fill(kr + "_MET"       , _met->pt()          , _weight);
  fill(kr + "_NBJets"    , _vc->get(_bvar)    , _weight);
  fill(kr + "_NLEls"     , _lEls.size()        , _weight);
  fill(kr + "_NTEls"     , _tEls.size()        , _weight);
  fill(kr + "_NVEls"     , _vEls.size()        , _weight);
  fill(kr + "_NJets"     , _nJets              , _weight);
  fill(kr + "_NLLeps"    , _lLeps.size()       , _weight);
  fill(kr + "_NTLeps"    , _tLeps.size()       , _weight);
  fill(kr + "_NVLeps"    , _vLeps.size()       , _weight);
  fill(kr + "_NLMus"     , _lMus.size()        , _weight);
  fill(kr + "_NTMus"     , _tMus.size()        , _weight);
  fill(kr + "_NVMus"     , _vMus.size()        , _weight);
  fill(kr + "_NVrtx"     , _vc->get("nVert")  , _weight);

}


//____________________________________________________________________________
void FakeRatio::fillFakeRatioMaps(std::string kr){
  /*
    fills the fake ratio maps for leptons
    parameters: none
    return: none
  */


  for(unsigned int i = 0; i < _lEls.size(); ++i){
    fill(kr + "_LElMap" , _vc->get("LepGood_pt", _lElIdx[i] ), fabs(_vc->get("LepGood_eta", _lElIdx[i] )), _weight);
    if(bJetMatching(_lElIdx[i]))
	  fill(kr + "_LElMapBJM" , _vc->get("LepGood_pt", _lElIdx[i] ), fabs(_vc->get("LepGood_eta", _lElIdx[i] )), _weight);
  }

  for(unsigned int i = 0; i < _lntEls.size(); ++i){
    fill(kr + "_LNTElMap" , _vc->get("LepGood_pt", _lntElIdx[i] ), fabs(_vc->get("LepGood_eta", _lntElIdx[i] )), _weight);
    if(bJetMatching(_lntElIdx[i]))
      fill(kr + "_LNTElMapBJM" , _vc->get("LepGood_pt", _lntElIdx[i] ), fabs(_vc->get("LepGood_eta", _lntElIdx[i] )), _weight);
  }

  for(unsigned int i = 0; i < _tEls.size(); ++i){
    fill(kr + "_TElMap" , _vc->get("LepGood_pt", _tElIdx[i] ), fabs(_vc->get("LepGood_eta", _tElIdx[i] )), _weight);
    if(bJetMatching(_tElIdx[i]))
      fill(kr + "_TElMapBJM" , _vc->get("LepGood_pt", _tElIdx[i] ), fabs(_vc->get("LepGood_eta", _tElIdx[i] )), _weight);
  }

  for(unsigned int i = 0; i < _lLeps.size(); ++i){
    fill(kr + "_LLepMap", _vc->get("LepGood_pt", _lLepIdx[i]), fabs(_vc->get("LepGood_eta", _lLepIdx[i])), _weight);
    if(bJetMatching(_lLepIdx[i]))
      fill(kr + "_LLepMapBJM", _vc->get("LepGood_pt", _lLepIdx[i]), fabs(_vc->get("LepGood_eta", _lLepIdx[i])), _weight);
  }

  for(unsigned int i = 0; i < _lntLeps.size(); ++i){
    fill(kr + "_LNTLepMap", _vc->get("LepGood_pt", _lntLepIdx[i]), fabs(_vc->get("LepGood_eta", _lntLepIdx[i])), _weight);
    if(bJetMatching(_lntLepIdx[i]))
      fill(kr + "_LNTLepMapBJM", _vc->get("LepGood_pt", _lntLepIdx[i]), fabs(_vc->get("LepGood_eta", _lntLepIdx[i])), _weight);
  }

  for(unsigned int i = 0; i < _tLeps.size(); ++i){
    fill(kr + "_TLepMap", _vc->get("LepGood_pt", _tLepIdx[i]), fabs(_vc->get("LepGood_eta", _tLepIdx[i])), _weight);
    if(bJetMatching(_tLepIdx[i]))
      fill(kr + "_TLepMapBJM", _vc->get("LepGood_pt", _tLepIdx[i]), fabs(_vc->get("LepGood_eta", _tLepIdx[i])), _weight);
  }

  for(unsigned int i = 0; i < _lMus.size(); ++i){
    fill(kr + "_LMuMap" , _vc->get("LepGood_pt", _lMuIdx[i] ), fabs(_vc->get("LepGood_eta", _lMuIdx[i] )), _weight);
    if(bJetMatching(_lMuIdx[i]))
      fill(kr + "_LMuMapBJM" , _vc->get("LepGood_pt", _lMuIdx[i] ), fabs(_vc->get("LepGood_eta", _lMuIdx[i] )), _weight);
  }

  for(unsigned int i = 0; i < _lntMus.size(); ++i){
    fill(kr + "_LNTMuMap" , _vc->get("LepGood_pt", _lntMuIdx[i] ), fabs(_vc->get("LepGood_eta", _lntMuIdx[i] )), _weight);
    if(bJetMatching(_lntMuIdx[i]))
      fill(kr + "_LNTMuMapBJM" , _vc->get("LepGood_pt", _lntMuIdx[i] ), fabs(_vc->get("LepGood_eta", _lntMuIdx[i] )), _weight);
  }

  for(unsigned int i = 0; i < _tMus.size(); ++i){
    fill(kr + "_TMuMap" , _vc->get("LepGood_pt", _tMuIdx[i] ), fabs(_vc->get("LepGood_eta", _tMuIdx[i] )), _weight);
    if(bJetMatching(_tMuIdx[i]))
      fill(kr + "_TMuMapBJM" , _vc->get("LepGood_pt", _tMuIdx[i] ), fabs(_vc->get("LepGood_eta", _tMuIdx[i] )), _weight);
  }

}


//____________________________________________________________________________
void FakeRatio::fillLeptonPlots(std::string kr){
  /*
    fills the control plots for leptons
    parameters: none
    return: none
  */


  for(unsigned int i = 0; i < _lEls.size(); ++i){
    fill(kr + "_LElDXY", fabs(_vc->get("LepGood_dxy"     , _lElIdx[i])), _weight);
    fill(kr + "_LElEta", fabs(_vc->get("LepGood_eta"     , _lElIdx[i])), _weight);
    fill(kr + "_LElIso",      _vc->get("LepGood_relIso03", _lElIdx[i]) , _weight);
    fill(kr + "_LElPt" ,      _vc->get("LepGood_pt"      , _lElIdx[i]) , _weight);
    fill(kr + "_LElMT" , Candidate::create( _lEls[i], _met)->mass()     , _weight);
  }

  for(unsigned int i = 0; i < _lntEls.size(); ++i){
    fill(kr + "_LNTElDXY", fabs(_vc->get("LepGood_dxy"     , _lntElIdx[i])), _weight);
    fill(kr + "_LNTElEta", fabs(_vc->get("LepGood_eta"     , _lntElIdx[i])), _weight);
    fill(kr + "_LNTElIso",      _vc->get("LepGood_relIso03", _lntElIdx[i]) , _weight);
    fill(kr + "_LNTElPt" ,      _vc->get("LepGood_pt"      , _lntElIdx[i]) , _weight);
    //fill(kr + "_LNTElMT" , Candidate::create( _lntEls[i], _met)->mass()     , _weight);
  }

  for(unsigned int i = 0; i < _tEls.size(); ++i){
    fill(kr + "_TElDXY", fabs(_vc->get("LepGood_dxy"     , _tElIdx[i])), _weight);
    fill(kr + "_TElEta", fabs(_vc->get("LepGood_eta"     , _tElIdx[i])), _weight);
    fill(kr + "_TElIso",      _vc->get("LepGood_relIso03", _tElIdx[i]) , _weight);
    fill(kr + "_TElPt" ,      _vc->get("LepGood_pt"      , _tElIdx[i]) , _weight);
    fill(kr + "_TElMT" , Candidate::create( _tEls[i], _met)->mass()     , _weight);
  }

  for(unsigned int i = 0; i < _lLeps.size(); ++i){
    fill(kr + "_LLepDXY", fabs(_vc->get("LepGood_dxy"     , _lLepIdx[i])), _weight);
    fill(kr + "_LLepEta", fabs(_vc->get("LepGood_eta"     , _lLepIdx[i])), _weight);
    fill(kr + "_LLepIso",      _vc->get("LepGood_relIso03", _lLepIdx[i]) , _weight);
    fill(kr + "_LLepPt" ,      _vc->get("LepGood_pt"      , _lLepIdx[i]) , _weight);
    fill(kr + "_LLepMT" , Candidate::create( _lLeps[i], _met)->mass()     , _weight);
  }

  for(unsigned int i = 0; i < _lntLeps.size(); ++i){
    fill(kr + "_LNTLepDXY", fabs(_vc->get("LepGood_dxy"     , _lntLepIdx[i])), _weight);
    fill(kr + "_LNTLepEta", fabs(_vc->get("LepGood_eta"     , _lntLepIdx[i])), _weight);
    fill(kr + "_LNTLepIso",      _vc->get("LepGood_relIso03", _lntLepIdx[i]) , _weight);
    fill(kr + "_LNTLepPt" ,      _vc->get("LepGood_pt"      , _lntLepIdx[i]) , _weight);
    //fill(kr + "_LNTLepMT" , Candidate::create( _lntLeps[i], _met)->mass()     , _weight);
  }

  for(unsigned int i = 0; i < _tLeps.size(); ++i){
    fill(kr + "_TLepDXY", fabs(_vc->get("LepGood_dxy"     , _tLepIdx[i])), _weight);
    fill(kr + "_TLepEta", fabs(_vc->get("LepGood_eta"     , _tLepIdx[i])), _weight);
    fill(kr + "_TLepIso",      _vc->get("LepGood_relIso03", _tLepIdx[i]) , _weight);
    fill(kr + "_TLepPt" ,      _vc->get("LepGood_pt"      , _tLepIdx[i]) , _weight);
    fill(kr + "_TLepMT" , Candidate::create( _tLeps[i], _met)->mass()     , _weight);
  }

  for(unsigned int i = 0; i < _lMus.size(); ++i){
    fill(kr + "_LMuDXY", fabs(_vc->get("LepGood_dxy"     , _lMuIdx[i])), _weight);
    fill(kr + "_LMuEta", fabs(_vc->get("LepGood_eta"     , _lMuIdx[i])), _weight);
    fill(kr + "_LMuIso",      _vc->get("LepGood_relIso03", _lMuIdx[i]) , _weight);
    fill(kr + "_LMuPt" ,      _vc->get("LepGood_pt"      , _lMuIdx[i]) , _weight);
    fill(kr + "_LMuMT" , Candidate::create( _lMus[i], _met)->mass()     , _weight);
  }

  for(unsigned int i = 0; i < _lntMus.size(); ++i){
    fill(kr + "_LNTMuDXY", fabs(_vc->get("LepGood_dxy"     , _lntMuIdx[i])), _weight);
    fill(kr + "_LNTMuEta", fabs(_vc->get("LepGood_eta"     , _lntMuIdx[i])), _weight);
    fill(kr + "_LNTMuIso",      _vc->get("LepGood_relIso03", _lntMuIdx[i]) , _weight);
    fill(kr + "_LNTMuPt" ,      _vc->get("LepGood_pt"      , _lntMuIdx[i]) , _weight);
    //fill(kr + "_LNTMuMT" , Candidate::create( _lntMus[i], _met)->mass()     , _weight);
  }

  for(unsigned int i = 0; i < _tMus.size(); ++i){
    fill(kr + "_TMuDXY", fabs(_vc->get("LepGood_dxy"     , _tMuIdx[i])), _weight);
    fill(kr + "_TMuEta", fabs(_vc->get("LepGood_eta"     , _tMuIdx[i])), _weight);
    fill(kr + "_TMuIso",      _vc->get("LepGood_relIso03", _tMuIdx[i]) , _weight);
    fill(kr + "_TMuPt" ,      _vc->get("LepGood_pt"      , _tMuIdx[i]) , _weight);
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
int FakeRatio::genParticleOrigin(int mid, int gmid){

  // returns:
  // -1: unmatched
  //  0: Z
  //  1: W
  //  2: electron
  //  3: muon
  //  4: tau
  //  5: neutrinos
  //  6: light flavor u/d/s
  //  7: charm c
  //  8: bottom b

  unsigned int mother           = std::abs(mid);
  unsigned int grandmother      = std::abs(gmid);
  unsigned int mother_3dig      = mother % 1000;
  unsigned int grandmother_3dig = grandmother % 1000;

  if     (mother == 23 || grandmother == 23                                                                 ) return 0;
  else if(mother == 24 || grandmother == 24                                                                 ) return 1;
  else if(mother == 5 || grandmother == 5                                                                   ) return 8;
  else if(grandmother >= 5000 && grandmother <= 5999                                                        ) return 8;
  else if((grandmother < 1000 || grandmother > 9999) && grandmother_3dig >= 500 && grandmother_3dig <= 599  ) return 8;
  else if(mother >= 5000 && mother <= 5999                                                                  ) return 8;
  else if((mother < 1000 || mother > 9999) && mother_3dig >= 500 && mother_3dig <= 599                      ) return 8;
  else if(mother == 4 || grandmother == 4                                                                   ) return 7;
  else if(grandmother >= 4000 && grandmother <= 4999                                                        ) return 7;
  else if((grandmother < 1000 || grandmother > 9999) && grandmother_3dig >= 400 && grandmother_3dig <= 499  ) return 7;
  else if(mother >= 4000 && mother <= 4999                                                                  ) return 7;
  else if((mother < 1000 || mother > 9999) && mother_3dig >= 400 && mother_3dig <= 499                      ) return 7;
  else if(grandmother == 1 || grandmother == 2 || grandmother == 3                                          ) return 6;
  else if(mother == 1 || mother == 2 || mother == 3                                                         ) return 6;
  else if((mother > 999 || mother < 10000) && mother_3dig >= 100 && mother_3dig <= 399                      ) return 6;
  else if(grandmother == 12 || grandmother == 14 || grandmother == 16                                       ) return 5;
  else if(mother == 12 || mother == 14 || mother == 16                                                      ) return 5;
  else if(mother == 15 || grandmother == 15                                                                 ) return 4;
  else if(mother == 13 || grandmother == 13                                                                 ) return 3;
  else if(mother == 11 || grandmother == 11                                                                 ) return 2;

  return -1;
}


//____________________________________________________________________________
int FakeRatio::genMatchIsFake(const Candidate * cand){

  bool isFake = true;
  vector<unsigned int> mpdg_cache;

  for(int ig = 0; ig < _vc->get("nGenPart"); ++ig){

    if(Tools::dR(cand->eta(), _vc->get("GenPart_eta", ig),
                 cand->phi(), _vc->get("GenPart_phi", ig) ) < 0.3){

      unsigned int partPdg = std::abs(_vc->get("GenPart_pdgId", ig));

      //if(partOrigin == 0){
      //cout << "found a Z for " << partPdg << " (" << _vc->get("GenPart_phi", ig) << ", " << _vc->get("GenPart_eta", ig) << ") for reco particle " << cand ->pdgId() << " (" << cand ->phi() << ", " << cand ->eta() << ")" << endl;
      //}
      //if(partPdg == 23 || (partOrigin >= 0 && partOrigin <= 5)){
      //cout << "gen pdg: " << partPdg << " and " << isFake << endl;
      if(partPdg >= 11 && partPdg <= 16){
        isFake = false;
        break; 
      }
    }
  }
  //cout << "is fake? " << isFake << endl;

  return isFake;

}


//____________________________________________________________________________
int FakeRatio::genMatchCateg(const Candidate* cand, string matchhist) {

  //loop over the number of generated leptons
  //int nGenL = _vc->get("nGenPart");
  //bool isFake = true;
  //vector<unsigned int> mpdg_cache;

  //for(int ig = 0; ig < nGenL; ++ig) {
  //  
  //  if(Tools::dR(cand->eta(), _vc->get("GenPart_eta", ig),
  //  	         cand->phi(), _vc->get("GenPart_phi", ig) ) < 0.3 && 
  //     _vc->get("GenPart_pt", ig) > 0.5 * cand->pt()) { //to be tuned	

  //    //if( (abs(cand->pdgId()) != abs(_vc->get("GenPart_pdgId", ig)) )){

  //     //cout<<"matched lepton "<<cand->pdgId()<<"  with "<<_vc->get("GenPart_pdgId",ig)<<" !!! "<<cand->pt()<<"  "<<cand->eta()<<"   "<<cand->phi()<<"   "<<Tools::dR(cand->eta(), _vc->get("GenPart_eta", ig), cand->phi(), _vc->get("GenPart_phi", ig) )<<endl;
  //
  //      if(abs(_vc->get("GenPart_pdgId", ig)) >= 11 &&
  //         abs(_vc->get("GenPart_pdgId", ig)) <= 16 ) {
  //      //if(abs(_vc->get("GenPart_pdgId", ig)) == 11 ||
  //      //   abs(_vc->get("GenPart_pdgId", ig)) == 13 || 
  //      //   abs(_vc->get("GenPart_pdgId", ig)) == 15 ) {

  //        isFake = false;
  //        break;
  //      }
  //      else {
  //        mpdg_cache.push_back(abs(_vc->get("GenPart_pdgId", ig)));
  //        //fillMatchHist(matchhist, abs(_vc->get("GenPart_pdgId", ig)));
  //      }
  //    //}
  //  } //dr matching
  //} //gen loop

  //if(isFake){
  //  for(int i = 0; i < mpdg_cache.size(); ++i)
  //    fillMatchHist(matchhist, mpdg_cache[i]);
  //}

  bool isFake = genMatchIsFake(cand);

  // CH: checking the source in this way does not make too much sense,
  // the GenPart collection does not contain all particles!
  //if(isFake){

  //  int pdg_cache  = 0;
  //  int mid_cache  = 0;
  //  int gmid_cache = 0;
  //  float dr_cache = 999.;

  //  for(int ig = 0; ig < _vc->get("nGenPart"); ++ig){

  //    if(Tools::dR(cand->eta(), _vc->get("GenPart_eta", ig),
  //                 cand->phi(), _vc->get("GenPart_phi", ig) ) < dr_cache){
  //    
  //    //int partOrigin = genParticleOrigin(_vc->get("GenPart_motherId", ig), 
  //    //                                   _vc->get("GenPart_grandmotherId", ig));

  //      pdg_cache  = abs(_vc->get("GenPart_pdgId", ig));
  //      mid_cache  = abs(_vc->get("GenPart_motherId", ig));
  //      gmid_cache = abs(_vc->get("GenPart_grandmotherId", ig));
  //      dr_cache  = Tools::dR(cand->eta(), _vc->get("GenPart_eta", ig), cand->phi(), _vc->get("GenPart_phi", ig) );
  //    }
  //  }
  //  cout << "pdg: " << pdg_cache << ", mid: " << mid_cache << ", gmid: " << gmid_cache << ", dR: " << dr_cache << endl; 
  //  fillMatchHist(matchhist, pdg_cache);

  //}

  if(isFake) return kMisMatchPdgId;
  else       return kNoGenMatch;
}




void FakeRatio::fillMatchHist(string matchhist, int pdgid){

  // bin 0: W, Z
  // bin 1: leptons
  // bin 2: photons
  // bin 3: gluons
  // bin 4: light-flavor
  // bin 5: c
  // bin 6: b
  // bin 7: others

  unsigned int fillbin = 7;
  unsigned int apdgid = fabs(pdgid);
  unsigned int apdgid3 = apdgid % 1000;

  if     (apdgid == 23 || apdgid == 24   ) fillbin = 0;
  else if(apdgid >= 11 && apdgid <= 16   ) fillbin = 1;
  else if(apdgid == 22                   ) fillbin = 2;
  else if(apdgid == 21                   ) fillbin = 3;
  else if(apdgid == 1 || apdgid == 2 || apdgid == 3 || (apdgid >= 1000 && apdgid <= 3999) || (apdgid >= 111 && apdgid <= 399) || (apdgid3 >= 111 && apdgid3 <= 399)) fillbin = 4;
  else if(apdgid == 4 || (apdgid >= 4000 && apdgid <= 4999) || (apdgid >= 401 && apdgid <= 499) || (apdgid3 >= 401 && apdgid3 <= 499)) fillbin = 5;
  else if(apdgid == 5 || (apdgid >= 5000 && apdgid <= 5999) || (apdgid >= 501 && apdgid <= 599) || (apdgid3 >= 501 && apdgid3 <= 599)) fillbin = 6;

  //cout << "filling " << apdgid << " in bin " << fillbin << endl;


  fill(matchhist, fillbin, _weight);


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

