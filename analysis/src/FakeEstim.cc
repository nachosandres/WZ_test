#include "analysis/src/FakeEstim.hh"

#include <algorithm>

FakeEstim::FakeEstim(std::string cfg){
  
  startExecution(cfg);
  initialize();

}

FakeEstim::~FakeEstim(){
 
}

void
FakeEstim::initialize(){


  _vc->registerVar("run"                          );
  _vc->registerVar("lumi"                         );
  _vc->registerVar("evt"                          );
  _vc->registerVar("HLT_SingleMu"                 );
  _vc->registerVar("HLT_MuEG"                     );
  _vc->registerVar("HLT_TripleEl"                 );
  _vc->registerVar("HLT_DoubleEl"                 );
  _vc->registerVar("HLT_DoubleMu"                 );
  _vc->registerVar("nVert"                        );
  _vc->registerVar("nTrueInt"                     );
  _vc->registerVar("nTrueInt"                     );
  _vc->registerVar("nBJetPt40Medium"              );
  _vc->registerVar("puWeight"                     );
  _vc->registerVar("genWeight"                    );
  _vc->registerVar("nLepGood"                     );
  _vc->registerVar("LepGood_pt"                   );
  _vc->registerVar("LepGood_eta"                  );
  _vc->registerVar("LepGood_phi"                  );
  _vc->registerVar("LepGood_charge"               );
  _vc->registerVar("LepGood_tightCharge"          );
  _vc->registerVar("LepGood_mediumMuonId"         );
  _vc->registerVar("LepGood_mvaIdPhys14"          );
  _vc->registerVar("LepGood_pdgId"                );
  _vc->registerVar("LepGood_relIso03"             );
  _vc->registerVar("LepGood_relIso04"             );
  _vc->registerVar("LepGood_jetPtRatio"           );
  _vc->registerVar("LepGood_jetPtRel"             );
  _vc->registerVar("LepGood_jetBTagCSV"           );
  _vc->registerVar("LepGood_miniRelIso"           );
  _vc->registerVar("LepGood_dxy"                  );
  _vc->registerVar("LepGood_dz"                   );
  _vc->registerVar("LepGood_sip3d"                );
  _vc->registerVar("LepGood_convVeto"             );
  _vc->registerVar("LepGood_lostHits"             );
  _vc->registerVar("LepGood_mvaSusy"              );
  _vc->registerVar("LepGood_mcMatchId"            );
  _vc->registerVar("LepGood_mcMatchAny"           );
  _vc->registerVar("met_pt"                       );
  _vc->registerVar("met_eta"                      );
  _vc->registerVar("met_phi"                      );
  _vc->registerVar("met_mass"                     );
  _vc->registerVar("nJet25"                       );
  _vc->registerVar("nJet40"                       );
  _vc->registerVar("nJet"                         );
  _vc->registerVar("Jet_id"                       );
  _vc->registerVar("Jet_pt"                       );
  _vc->registerVar("Jet_rawPt"                    );
  _vc->registerVar("Jet_eta"                      );
  _vc->registerVar("Jet_phi"                      );
  _vc->registerVar("Jet_mass"                     );
  _vc->registerVar("Jet_btagCSV"                  );

  _vc->registerVar("htJet40j"                     );
  _vc->registerVar("minMllAFAS"                   );
  _vc->registerVar("minMllAFOS"                   );
  _vc->registerVar("mZ1"                          );
  _vc->registerVar("nLepGood10"                   );

 
  //generator informations
  _vc->registerVar("nGenPart"                     );
  _vc->registerVar("GenPart_eta"                  );
  _vc->registerVar("GenPart_phi"                  );
  _vc->registerVar("GenPart_pdgId"                );
  
  //bjets
  _vc->registerVar("nBJetLoose25"                 );
  _vc->registerVar("nBJetMedium40"                );
  _vc->registerVar("nBJetMedium25"                );
  _vc->registerVar("nSoftBJetMedium25"            );


  //extra input variables
  _lepflav = getCfgVarS("LEPFLAV");
  _lepId   = getCfgVarS("LEPID"  );
  _lepiso  = getCfgVarS("LEPISO" );
  _leppt   = getCfgVarS("LEPPT"  );
  // _leptl   = getCfgVarS("LEPTL"  );
  _extScheme  = getCfgVarS("FRSCHEME");
  _SR      = getCfgVarS("SR"     );
  _FR      = getCfgVarS("FR"     );
 
  //database
  // if(_extScheme=="iso") {
  //   if(_lepiso=="pTrel") {
  //     // _dbm->loadDb("AllEl","FakeRatio_all_cut_pTrel_none_iso_all_all_out.root","MR_RElMap_qcd_all_cut_pTrel_none_iso_all_all");
  //     // _dbm->loadDb("AllMu","FakeRatio_all_cut_pTrel_none_iso_all_all_out.root","MR_RMuMap_qcd_all_cut_pTrel_none_iso_all_all");
  //     // _dbm->loadDb("0BEl","FakeRatio_MR00_cut_pTrel_none_iso_all_all_out.root","MR_RElMap_qcd_MR00_cut_pTrel_none_iso_all_all");
  //     // _dbm->loadDb("0BMu","FakeRatio_MR00_cut_pTrel_none_iso_all_all_out.root","MR_RMuMap_qcd_MR00_cut_pTrel_none_iso_all_all");
  //     // _dbm->loadDb("1BEl","FakeRatio_MR01_cut_pTrel_none_iso_all_all_out.root","MR_RElMap_qcd_MR01_cut_pTrel_none_iso_all_all");
  //     // _dbm->loadDb("1BMu","FakeRatio_MR01_cut_pTrel_none_iso_all_all_out.root","MR_RMuMap_qcd_MR01_cut_pTrel_none_iso_all_all");
  //     // _dbm->loadDb("2BEl","FakeRatio_MR02_cut_pTrel_none_iso_all_all_out.root","MR_RElMap_qcd_MR02_cut_pTrel_none_iso_all_all");
  //     // _dbm->loadDb("2BMu","FakeRatio_MR02_cut_pTrel_none_iso_all_all_out.root","MR_RMuMap_qcd_MR02_cut_pTrel_none_iso_all_all");
  //     // _dbm->loadDb("3BEl","FakeRatio_MR03_cut_pTrel_none_iso_all_all_out.root","MR_RElMap_qcd_MR03_cut_pTrel_none_iso_all_all");
  //     // _dbm->loadDb("3BMu","FakeRatio_MR03_cut_pTrel_none_iso_all_all_out.root","MR_RMuMap_qcd_MR03_cut_pTrel_none_iso_all_all");
  //   }
  // }
  // else if(_extSchem=="")

  if(_lepId=="mva") {
    _lepiso="relIso";
    _extScheme="iso";
  }
  if(_lepiso!="relIso")
    _extScheme="iso";

  // _dbm->loadDb("AllEl","FakeRatio_all_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all_out.root","MR_RElMap_qcd_all_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all");
  // _dbm->loadDb("AllMu","FakeRatio_all_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all_out.root","MR_RMuMap_qcd_all_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all");
  // _dbm->loadDb("0BEl","FakeRatio_MR00_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all_out.root","MR_RElMap_qcd_MR00_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all");
  // _dbm->loadDb("0BMu","FakeRatio_MR00_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all_out.root","MR_RMuMap_qcd_MR00_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all");
  // _dbm->loadDb("1BEl","FakeRatio_MR01_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all_out.root","MR_RElMap_qcd_MR01_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all");
  // _dbm->loadDb("1BMu","FakeRatio_MR01_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all_out.root","MR_RMuMap_qcd_MR01_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all");
  // _dbm->loadDb("2BEl","FakeRatio_MR02_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all_out.root","MR_RElMap_qcd_MR02_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all");
  // _dbm->loadDb("2BMu","FakeRatio_MR02_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all_out.root","MR_RMuMap_qcd_MR02_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all");
  // _dbm->loadDb("3BEl","FakeRatio_MR03_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all_out.root","MR_RElMap_qcd_MR03_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all");
  // _dbm->loadDb("3BMu","FakeRatio_MR03_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all_out.root","MR_RMuMap_qcd_MR03_"+_lepId+"_"+_lepiso+"_none_"+_extScheme+"_all_all");


  _dbm->loadDb("AllMuT","v3/FakeRatio_all_cut_"+_lepiso+"_none_iso_all_all.root","MR_RMuMapPtMIso/QCD_Mu15_skimfake");
  _dbm->loadDb("AllMuVT","v3/FakeRatio_all_cut_"+_lepiso+"_none_iso_all_all.root","MR_RMuMapPtMIso/QCD_Mu15_skimfake");

  //_dbm->loadDb("AllMuT","v3/FakeRatio_all_cut_mixisoT_none_iso_all_all_out.root","MR_RMuMapPtMIso_qcd_all_cut_mixisoT_none_iso_all_all");
  //_dbm->loadDb("AllMuVT","v3/FakeRatio_all_cut_mixisoVT_none_iso_all_all_out.root","MR_RMuMapPtMIso_qcd_all_cut_mixisoVT_none_iso_all_all");


  //additional workflow
  addWorkflow( kGenMisCharge, "GenMisCharge" );
  addWorkflow( kGenFake, "GenFake" );
  addWorkflow( kOneIso, "OneFake" );
  addWorkflow( kNoIso, "DoubleFake" );

  _au->addCategory(kSelId,"selection ID");

  defineLeptonWPS();
  setSignalRegion();


  _fakeEl=0;
  _fakeMu=0;
  _nDFake=0;
  _nSFake=0;
  _nCharge=0;
  _nOther=0;

}

void
FakeEstim::defineLeptonWPS() {

  _sipWP.resize(kNWPs);
  _elMvaIdWP.resize(3);
  _multiIsoWP.resize(3);
  for(int i=0;i<3;++i) {
    _elMvaIdWP[i].resize(kNWPs);
    _multiIsoWP[i].resize(kNWPs);
  }
  
  _mvaIdLPtWP.resize(kNWPs);
  _mvaIdHPtWP.resize(kNWPs);
  _isoWP.resize(kNWPs);
  _miniIsoWP.resize(kNWPs);
  _ptRelWP.resize(kNWPs);
  
  //sip ============================
  _sipWP[kLoose]=10.0;  _sipWP[kTight]=4.0; 
  
  //el mva id ======================
  _elMvaIdWP[0][kLoose]=0.35; _elMvaIdWP[0][kTight]=0.73;
  _elMvaIdWP[1][kLoose]=0.20; _elMvaIdWP[1][kTight]=0.57;
  _elMvaIdWP[2][kLoose]=-0.52; _elMvaIdWP[2][kTight]=0.05;

  //mva id =========================
  _mvaIdLPtWP[kLoose]=0;  _mvaIdLPtWP[kTight]=0.53; 
  _mvaIdHPtWP[kLoose]=-0.68;  _mvaIdHPtWP[kTight]=0.; 

  //isolation ======================
  _isoWP[kLoose]=0.5;  _isoWP[kTight]=0.1; _isoWP[kVTight]=0.1; _isoWP[kHTight]=0.1;

  //miniIso ========================
  _miniIsoWP[kLoose]=0.4; _miniIsoWP[kTight]=0.05; _miniIsoWP[kVTight]=0.05; _miniIsoWP[kHTight]=0.05;

  //ptRel ==========================
  _ptRelWP[kLoose]=5.;  _ptRelWP[kTight]=14.; _ptRelWP[kVTight]=14.; _ptRelWP[kHTight]=14.;

  //multiIso =======================
  _multiIsoWP[kMiniIso][kLoose]=0.4; _multiIsoWP[kMiniIso][kTight]=0.10; _multiIsoWP[kMiniIso][kVTight]=0.075; 
  _multiIsoWP[kPtRatio][kLoose]=0; _multiIsoWP[kPtRatio][kTight]=0.70; _multiIsoWP[kPtRatio][kVTight]=0.725; 
  _multiIsoWP[kPtRel][kLoose]=0; _multiIsoWP[kPtRel][kTight]=7; _multiIsoWP[kPtRel][kVTight]=7; 

  
  _multiIsoWP[kMiniIso][kHTight]=0.05; _multiIsoWP[kPtRatio][kHTight]=0.725; _multiIsoWP[kPtRel][kHTight]=8; 

}

void
FakeEstim::modifyWeight() {

}

void
FakeEstim::modifySkimming() {

}


void
FakeEstim::defineOutput() {

  string leps[2]={"l1","l2"};
  for(int il=0;il<2;il++) {
    _hm->addVariable(leps[il]+"Pt", 200, 0., 200.0,"p_{T}("+leps[il]+") [GeV]");
    // _hm->addVariable(leps[il]+"Eta", 60, -3.0, 3.0,"#eta("+leps[il]+") ");
    // _hm->addVariable(leps[il]+"Phi", 60, -3.1416, 3.1416,"#phi("+leps[il]+") ");
  }

  _hm->addVariable("pdgId1", 20, 0., 20,"");
  _hm->addVariable("pdgId2", 20, 0., 20,"");

  _hm->addVariable("NIso",5,0,5,"NIso");
  _hm->addVariable("NNonIso",5,0,5,"NIso");
  _hm->addVariable("l1Id",100,-10,10,"NIso");
  _hm->addVariable("l2Id",100,-10,10,"NIso");

  _hm->addVariable("isolation",100,0,1,"iso");
  _hm->addVariable("NSelPair2Iso",5,0,5,"iso");
  _hm->addVariable("NSelPair1Iso",5,0,5,"iso");
  _hm->addVariable("NSelPair0Iso",5,0,5,"iso");

  _hm->addVariable("NBJet",10,0,10,"nbjet");

  _hm->addVariable("jetPtRatio", 110, 0., 1.1,"jetPtRatio");
  _hm->addVariable("miniIso", 110, 0., 1.1,"miniIso");
  _hm->addVariable("jetPtRel", 110, 0., 22,"jetPtRel");

  //  _hm->add
}


void
FakeEstim::writeOutput() {
 
  cout<< " fake electrons: "<<_fakeEl<<" fake muons: "<<_fakeMu<<endl;
  cout<<" dFake: "<<_nDFake<<" sFake: "<<_nSFake<<" charge: "<<_nCharge
      <<" other: "<<_nOther<<endl;
 
}


void
FakeEstim::run() {
   
  retrieveObjects();
  
  counter("denominator");
 
  // triggers  
  if(_isData && !makeCut<int>(_vc->get("HLT_DoubleMu"), 1, "=", "HLT DoubleMu") ) return;	
  if(_isData && !makeCut<int>(_vc->get("HLT_DoubleEl"), 1, "=", "HLT DoubleEl") ) return;	
  if(_isData && !makeCut<int>(_vc->get("HLT_MuEG")    , 1, "=", "HLT MuEG"    ) ) return;	
  
  //cout<<_nLooseLeps<<endl;
  // _nLooseLeps=0;
  // lepton multiplicity
  //cout<<getCurrentWorkflow()<<endl;
  //if(!makeCut( _fullIdLeps.size()>=2, "lepMult" ) ) return;
  if(!makeCut( _leptons.size()>=2, "lepMult" ) ) return;
  //return;

  // selecting best same-sign pair 
  bool is_ss_event = alternateSSEventSelection();//ssEventSelection();
  if(!makeCut( is_ss_event , "same-sign selection", "=") ) return;

  //if(_fullIdLeps.size()!=1 || _nonFullIdLeps.size()!=1) return;

  if(_nIso!=1)
    fill("isolation", _vc->get("LepGood_sip3d",_lep_idx1)/20. );
  fill("isolation", _vc->get("LepGood_sip3d",_lep_idx2)/20. );
  
  fill("pdgId1", std::abs(_vc->get("LepGood_pdgId", _lep_idx1)) ); 
  fill("pdgId2", std::abs(_vc->get("LepGood_pdgId", _lep_idx2)) ); 

  fill("l1Pt", std::abs(_vc->get("LepGood_pt", _lep_idx1)) ); 
  fill("l2Pt", std::abs(_vc->get("LepGood_pt", _lep_idx2)) ); 

  // cout<<" new event=============================== "<<_ie<<endl;
  // cout<<" NLep="<<_leptons.size()<<"   nIdLep="<<_fullIdLeps.size()<<"="<<_fullIdLepsIdx.size()<<"  nNonIdLep="
  //     <<_nonFullIdLeps.size()<<" // "<<_lepsForFR.size()<<" ==> nIso="<<_nIso<<endl;

  // for(int i=0;i<_lepsForFR.size();i++) {
  //   cout<<_lepsForFR[i]->pdgId()<<" <> "<<_nonFullIdLeps[i]->pdgId()<<" || "
  // 	<<_lepsForFR[i]->charge()<<" <> "<<_nonFullIdLeps[i]->charge()<<" || "
  // 	<<passFakeId(_nonFullIdLepsIdx[i])<<" || "
  // 	<<_lepsForFR[i]->pt()<<" <> "<<_nonFullIdLeps[i]->pt()<<" || "<<_lepsForFR[i]->eta()<<endl;
  //     }
 
  // cout<<" l1Cand: "<<_l1Cand->pdgId()<<" || "<<_l1Cand->charge()<<" || "
  //     <<passFakeId(_lep_idx1)<<" || "<<_l1Cand->pt()<<endl;
  // cout<<" l2Cand: "<<_l2Cand->pdgId()<<" || "<<_l2Cand->charge()<<" || "
  //     <<passFakeId(_lep_idx2)<<" || "<<_l2Cand->pt()<<endl;

  // cout<<" prob2: "<<getProbAtLeastNIso(_lepsForFR,2)<<endl<<" prob1: "<<getProbAtLeastNIso(_lepsForFR,1)<<endl;

  //cout<<" ==> "<<_fullIdLeps.size()<<endl;

  // if(_nIso==2) {
  //   // if(!_isData) {
  //   //   //if( genMatchedToFake() ) setWorkflow(kGenFake);
  //   //   // if( genMatchedMisCharge() ) setWorkflow(kGenMisCharge);
  //   //   // else setWorkflow(kGlobal);
  //   // }
  //   // else setWorkflow(kGlobal);
    
  // }
  // else
 if(_nIso!=2)  { //fake ratio
   float p=getProbAtLeastNIso( _lepsForFR ,_lepsForFRIdx, 2-_nIso);
   _weight *= p/(1-p);
  }

  
  //if( _fullIdLepsIdx.size()>=2 ) {
  //   //return;
  //   //setWorkflow(kGlobal);
  //   if(!_isData) {
  //     if( genMatchedToFake() ) setWorkflow(kGenFake);
  //     else if( genMatchedMisCharge() ) setWorkflow(kGenMisCharge);
  //     else setWorkflow(kGlobal);
  //   }
  //   else setWorkflow(kGlobal);
  // }
  // else {
  //   if( noIsoSel() ) setWorkflow(kNoIso);
  //   else if( oneIsoSel() ) setWorkflow(kOneIso);
  //   else return;
  // }
  //and global should be 0

  counter("change of weigth");
  //cout<<"workflow = "<<getCurrentWorkflow()<<"   "<<_l1Cand<<"   "<<_l2Cand<<endl;

  // baseline selection ======================================

  
  // veto on third lepton using all selected leptons
  if(!makeCut( mllVetoSelection(), "mll vetos", "=") ) return;


  //gen composition
  if(getCurrentWorkflow()==kGlobal) {
    if( genMatchedToFake(_lep_idx1) && std::abs(_l1Cand->pdgId())==11 ) _fakeEl++;
    if( genMatchedToFake(_lep_idx1) && std::abs(_l1Cand->pdgId())==13 ) _fakeMu++;
    
    if( genMatchedToFake(_lep_idx2) && std::abs(_l2Cand->pdgId())==11 ) _fakeEl++;
    if( genMatchedToFake(_lep_idx2) && std::abs(_l2Cand->pdgId())==13 ) _fakeMu++;
    
    if( genMatchedToFake(_lep_idx1)&&genMatchedToFake(_lep_idx2) ) _nDFake++;
    else if( genMatchedToFake(_lep_idx1)||genMatchedToFake(_lep_idx2) ) _nSFake++;
    else if( genMatchedMisCharge() ) _nCharge++;
    else _nOther++;
  }

    //if( genMatchedMisCharge() ) se


  // signal selection ======================================
  
  if(!makeCut<float>( _HT, _valCutHTSR, _cTypeHTSR, "SRHTSel", _upValCutHTSR) ) return;
  if(!makeCut<float>( _met->pt(), _valCutMETSR, _cTypeMETSR, "SR MET selection", _upValCutMETSR) ) return;
  if(_au->simpleCut( _HT, _valCutHTCondSR, _cTypeHTCondSR) ) {
    if(!makeCut<float>(_met->pt(), _valCutMETHighSR, _cTypeMETHighSR, "SR METAdv selection", _upValCutMETHighSR) ) return;
  }
  else {
    if(!makeCut<float>(_met->pt(), _valCutMETLowSR , _cTypeMETLowSR , "SR METAdv selection", _upValCutMETLowSR) ) return;
  }

  if(!makeCut<int>( _nJets, _valCutNJetsSR, _cTypeNJetsSR, "SR jet multiplicity", _upValCutNJetsSR) ) return;
  
  fill("NBJet", _vc->get(_btag) , _weight);

  if(!makeCut<int>(_vc->get(_btag), _valCutNBJetsSR, _cTypeNBJetsSR, "SR b-jet multiplicity", _upValCutNBJetsSR) ) return;



  //what are the events in the global category?
  if(getCurrentWorkflow()==kGlobal) {
    int id1  = _vc->get("LepGood_mcMatchId" , _lep_idx1); 
    int id2  = _vc->get("LepGood_mcMatchId" , _lep_idx2); 
    int ix1  = _vc->get("LepGood_mcMatchAny" , _lep_idx1); 
    int ix2  = _vc->get("LepGood_mcMatchAny" , _lep_idx2); 
    
    // cout<<" ===================== > " <<id1<<"   "<<id2<<"   "
    //  	<<_l1Cand->charge()<<"   "<<_l2Cand->charge()<<"   "
    //  	<<_l1Cand->pdgId()<<"   "<<_l2Cand->pdgId()<<"    "
    //  	<<ix1<<"    "<<ix2<<endl;

    // cout<<_tmpLepIdx[0]<<"<>"<<_fullIdLepsIdx[0]<<" || "<<_tmpLepIdx[1]<<"<>"<<_fullIdLepsIdx[1]
    // 	<<" ==>  "<< _fullIdLeps[0]->pt()<<" <> "<<_allLeps[_fullIdLepsIdx[0]]->pt()<<"   "

    // 	<<" || "<<_fullIdLeps[1]->pt()<<" <> "<<_allLeps[_fullIdLepsIdx[1]]->pt()
    // 	<<" ==>  "<< _fullIdLeps[0]->charge()<<" <> "<<_allLeps[_fullIdLepsIdx[0]]->charge()
    // 	<<" || "<< _fullIdLeps[1]->charge()<<" <> "<<_allLeps[_fullIdLepsIdx[1]]->charge()
    // 	<<"   +===> "
    // 	<<_fullIdLeps.size()<<"  ++  "<<_nonFullIdLeps.size()<<"  == "<<_leptons.size()<<endl;

    // fill("l1Id", _fullIdLeps[0]->pt()-_allLeps[_fullIdLepsIdx[0]]->pt() );
    // fill("l2Id", _fullIdLeps[1]->pt()-_allLeps[_fullIdLepsIdx[1]]->pt() );
    //abort();
  }

  fill("NSelPair2Iso", _nSelPair2Iso );
  fill("NSelPair1Iso", _nSelPair1Iso );
  fill("NSelPair0Iso", _nSelPair0Iso );
  

  fill("NIso",_fullIdLeps.size() );
  fill("NNonIso",_nonFullIdLeps.size() );
  
}

bool
FakeEstim::noIsoSel() {
  
  if( _fullIdLepsIdx.size()!=0 ) return false;
  
  float p=getProbAtLeastNIso( _lepsForFR ,_lepsForFRIdx , 2);
  _weight *= p/(1-p);
  return true;
}

bool
FakeEstim::oneIsoSel() {
  
  if( _fullIdLepsIdx.size()!=1 ) return false;

  float p=getProbAtLeastNIso( _lepsForFR ,_lepsForFRIdx , 1);
  _weight *= p/(1-p);
  return true;
}

bool
FakeEstim::genFake() {
  
  //if( !genMatchedToFake() ) return false;

  return true;
}



float
FakeEstim::getProbAtLeastNIso(CandList fObjs, vector<unsigned int> fObjIdx,
			      int nIso) {

  //MM tirage Bernoulli sans remise non ordonne avec probas individuelles
  //formule generale probablement tres compliquee, et surtout pas trouvee

  if(fObjs.size()<(size_t)nIso) {cout<<"not enough leptons for your configuration : "<<fObjs.size()<<" vs  "<<nIso<<endl; return 0;}
  if(nIso>2) {cout<<"not implemented for nIso>2"<<endl; return 0;}

  vector<float> frs(fObjs.size(),0);

  float pNoIso = 1;
  for(size_t io=0;io<fObjs.size(); io++) {
    frs[io] = getFR(fObjs[io], fObjIdx[io]);
    pNoIso *= (1-frs[io]);
  }

  if(nIso==0) return pNoIso;// no isolated lepton among all leptons
  if(nIso==1) return (1-pNoIso);// at least one isolated lepton among all leptons

  //at least 2 iso case : computes proba to have only 1 iso within N
  float p1Iso = 0;
  for(size_t ii=0;ii<fObjs.size(); ii++) {
    float pii = frs[ii];
    for(size_t ij=0;ij<fObjs.size(); ij++) {
      if(ij!=ii)
	pii *= 1-frs[ij];
    }
    p1Iso += pii;
  }

  if(nIso==2) return (1-p1Iso-pNoIso);

  return 0;
}


//=====================================================
// lepton selection

bool 
FakeEstim::electronMvaCut(int idx, int wp){

  int etaBin=-1;

  if(std::abs(_vc->get("LepGood_eta", idx)) < 0.8) etaBin=0;
  else if(std::abs(_vc->get("LepGood_eta", idx)) < 1.479) etaBin=1;
  else if(std::abs(_vc->get("LepGood_eta", idx)) < 2.4) etaBin=2;
  
  if(_vc->get("LepGood_mvaIdPhys14", idx) <  _elMvaIdWP[etaBin][wp]  ) return false;
    
  return true;
}


// bool 
// FakeEstim::electronCutId(int elIdx, float sip, bool mvatight){

//   if( !electronMvaCut(elIdx, mvatight) ) return false;
//   if(_vc->get("LepGood_sip3d", elIdx)>sip) return false;
//   if( std::abs(_vc->get("LepGood_dz", elIdx))>0.1 ) return false;
//   if( std::abs(_vc->get("LepGood_dxy", elIdx))>0.05 ) return false;

//   return true;
// }

// bool
// FakeEstim::muonCutId(int muIdx, float sip){

//   if( _vc->get("LepGood_mediumMuonId", muIdx)<=0 ) return false;
//   if( _vc->get("LepGood_tightCharge" , muIdx)<=1 ) return false;
//   if( _vc->get("LepGood_sip3d", muIdx)>sip ) return false;
//   if( std::abs(_vc->get("LepGood_dz", muIdx))>0.1 ) return false;
//   if( std::abs(_vc->get("LepGood_dxy", muIdx))>0.05 ) return false;

//   return true;

// }


// bool 
// FakeEstim::looseElectronSelection(int elIdx){


//   if(_vc->get("LepGood_pt", elIdx)<10.) return false;
//   if(std::abs(_vc->get("LepGood_eta", elIdx))>2.4) return false;
//   if(std::abs(_vc->get("LepGood_eta", elIdx))>1.4442 &&
//      std::abs(_vc->get("LepGood_eta", elIdx))<1.566) return false;
//   if(_vc->get("LepGood_tightCharge", elIdx)<=1) return false; 
//   if(_vc->get("LepGood_convVeto", elIdx)<0) return false;
//   if(_vc->get("LepGood_lostHits", elIdx)!=0) return false;

//   bool lepId=false;
//   if(_lepId != "mva") lepId=electronCutId(elIdx, 10.0, true);
//   else lepId=electronMvaId(elIdx, false);

//   if( !lepId ) return false;
//   if( !isolationCut(elIdx, false) ) return false;

//   // electron cleaning 
//   for(unsigned int il=0; il<_vc->get("nLepGood"); ++il){
//     float dr = KineUtils::dR(_vc->get("LepGood_eta", il), _vc->get("LepGood_eta", elIdx),
//                              _vc->get("LepGood_phi", il), _vc->get("LepGood_phi", elIdx));
//     if(std::abs(_vc->get("LepGood_pdgId"))==13 && dr<0.05 ) return false;
//   }

//   return true;
// }


// bool 
// FakeEstim::looseMuonSelection(int muIdx){
 
//   if( !stdMuSelection(muIdx) ) return false;


//   bool lepId=false;
//   if(_lepId != "mva") lepId=muonCutId(muIdx, 10.0);
//   else lepId=muonMvaId(elIdx, false);

//   if( !lepId ) return false;

//   if( !isolationCut(muIdx, false) ) return false;

//   return true;
// }


// bool 
// FakeEstim::tightElectronSelection(int elIdx){
  
//   if(_vc->get("LepGood_pt", elIdx)<10.) return false;
//   if(std::abs(_vc->get("LepGood_eta", elIdx))>2.4) return false;
//   if(std::abs(_vc->get("LepGood_eta", elIdx))>1.4442 &&
//      std::abs(_vc->get("LepGood_eta", elIdx))<1.566) return false;
//   if(_vc->get("LepGood_tightCharge", elIdx)<=1) return false; 
//   if(_vc->get("LepGood_convVeto", elIdx)<0) return false;
//   if(_vc->get("LepGood_lostHits", elIdx)!=0) return false;

//   bool lepId=false;
//   if(_lepId != "mva") lepId=electronCutId(elIdx, 4.0, true);
//   else lepId=electronMvaId(elIdx, true);

//   if( !lepId ) return false;

//   if( !electronCutId(elIdx,4.0, true) ) return false;
  
//   // electron cleaning 
//   for(unsigned int il=0; il<_vc->get("nLepGood"); ++il){
//     float dr = KineUtils::dR(_vc->get("LepGood_eta", il), _vc->get("LepGood_eta", elIdx),
//                              _vc->get("LepGood_phi", il), _vc->get("LepGood_phi", elIdx));
//     if(std::abs(_vc->get("LepGood_pdgId")) == 13 && dr<0.05) return false;
//   }

//   return true;
// }

// bool 
// FakeEstim::tightMuonSelection(int muIdx){
  
//   if( _vc->get("LepGood_pt" , muIdx)<10 ) return false;
//   if( std::abs(_vc->get("LepGood_eta" , muIdx))>2.4 ) return false;
//   if( !muonCutId(muIdx,4.0) ) return false;

//   bool lepId=false;
//   if(_lepId != "mva") lepId=muonCutId(muIdx, 10.0);
//   else lepId=muonMvaId(elIdx, false);
  
//   if( !lepId ) return false;



//   return true;
// }

bool
FakeEstim::isMuSelected(int idx, int wp) {

  int wpIso=wp;
  int wpSIP=wp;
  int wpMva=wp;
  if(_lepId!="mva") {
    if(_extScheme=="iso") {wpIso=kLoose;}
    if(_extScheme=="isosip") {wpIso=kLoose; wpSIP=kLoose;}
  }
  else {wpMva=kLoose;}
  
  if( _vc->get("LepGood_pt" , idx)<10 ) return false;
  if( std::abs(_vc->get("LepGood_eta" , idx))>2.4 ) return false;
  
  if( _vc->get("LepGood_mediumMuonId", idx)<=0 ) return false;
  if( _vc->get("LepGood_tightCharge" , idx)<=1 ) return false;
 
  if(_lepId=="mva") {
    if( !lepMvaId(idx, wpMva) ) return false;
  }
  else {
    //if(_vc->get("LepGood_tightId"   , idx)<1) return false;
    if(_vc->get("LepGood_sip3d", idx)>_sipWP[wpSIP]) return false;
    if( std::abs(_vc->get("LepGood_dz", idx))>0.1 ) return false;
    if( std::abs(_vc->get("LepGood_dxy", idx))>0.05 ) return false;
    if( !isIsolated(idx, wpIso) ) return false;
  }
  
  return true;
}

bool
FakeEstim::isElSelected(int idx, int wp) {

  int wpIso=wp;
  int wpSIP=wp;
  int wpMva=wp;
  if(_lepId!="mva") {
    if(_extScheme=="iso") {wpIso=kLoose;}
    if(_extScheme=="isosip") {wpIso=kLoose; wpSIP=kLoose;}
  }
  else {wpMva=kLoose;}


  if(_vc->get("LepGood_pt", idx)<10.) return false;
  if(std::abs(_vc->get("LepGood_eta", idx))>2.4) return false;
  if(std::abs(_vc->get("LepGood_eta", idx))>1.4442 &&
     std::abs(_vc->get("LepGood_eta", idx))<1.566) return false;
  if(_vc->get("LepGood_tightCharge", idx)<=1) return false; 
  if(_vc->get("LepGood_convVeto", idx)<0) return false;
  if(_vc->get("LepGood_lostHits", idx)!=0) return false;

  if(_lepId=="mva") {
    if( !lepMvaId(idx, wpMva) ) return false;
  }
  else {
    //always tight
    if( !electronMvaCut(idx, kTight) ) return false;
    if(_vc->get("LepGood_sip3d", idx)>_sipWP[wpSIP]) return false;
    if( std::abs(_vc->get("LepGood_dz", idx))>0.1 ) return false;
    if( std::abs(_vc->get("LepGood_dxy", idx))>0.05 ) return false;
    if( !isIsolated(idx, wpIso) ) return false;
  }

 

  // electron cleaning ==================
  for(unsigned int il=0; il<_vc->get("nLepGood"); ++il){
    float dr = KineUtils::dR(_vc->get("LepGood_eta", il), _vc->get("LepGood_eta", idx),
                             _vc->get("LepGood_phi", il), _vc->get("LepGood_phi", idx));
    if(std::abs(_vc->get("LepGood_pdgId"))==13 && dr<0.05 ) return false;
  }

  return true;
}

bool 
FakeEstim::lepMvaId(int idx, int wp) {
  
  if(_vc->get("LepGood_pt", idx)>25)
    return _vc->get("LepGood_mvaSusy",idx)>_mvaIdHPtWP[wp];
  else
    return _vc->get("LepGood_mvaSusy", idx)>_mvaIdLPtWP[wp];

}


bool 
FakeEstim::isIsolated(int idx, int wp) {
 
  if(_lepiso=="pTrel") {
    if((_vc->get("LepGood_jetPtRel", idx)>_ptRelWP[wp] || _vc->get("LepGood_relIso03", idx)<_isoWP[wp])) return true;
  }
  else if(_lepiso == "miniiso"){
    if(_vc->get("LepGood_miniRelIso", idx)<_miniIsoWP[wp]) return true;
  }
  else if(_lepiso == "multiIso") {
    if( _vc->get("LepGood_miniRelIso", idx)<_multiIsoWP[kMiniIso][wp] &&
	(_vc->get("LepGood_jetPtRatio", idx)>_multiIsoWP[kPtRatio][wp] ||
	 _vc->get("LepGood_jetPtRel", idx)>_multiIsoWP[kPtRel][wp]) ) return true;
  }
  else if(_lepiso == "multiIsoHTight" && wp!=kLoose) {
    if( _vc->get("LepGood_miniRelIso", idx)<_multiIsoWP[kMiniIso][wp+1] &&
	(_vc->get("LepGood_jetPtRatio", idx)>_multiIsoWP[kPtRatio][wp+1] ||
	 _vc->get("LepGood_jetPtRel", idx)>_multiIsoWP[kPtRel][wp+1]) ) return true;
  }
  else if(_lepiso == "multiIsoGiuseppe") {
    if( _vc->get("LepGood_jetPtRel", idx)>6) { //miniiso case
      if( _vc->get("LepGood_miniRelIso", idx)<0.05) return true;
    }
    else {
      if( _vc->get("LepGood_relIso03", idx)<0.1) return true;
    }
  }
  else {
    if(_vc->get("LepGood_relIso03", idx)<_isoWP[wp]) return true;
  }

  return false;
}

bool
FakeEstim::passFakeId(int idx, bool isMu) {
  
  if(_lepId!="mva") {
    if(_extScheme=="iso") { //iso only
      if( !isIsolated(idx, isMu?kTight:kVTight) ) return false;
    }
    if(_extScheme=="isosip") { //iso + sip
      if( !isIsolated(idx, isMu?kTight:kVTight) ) return false;
      if( _vc->get("LepGood_sip3d", idx)>_sipWP[kTight]) return false;
    }
    if(_extScheme=="isoptRel") { //iso + ptRel
      if((_vc->get("LepGood_jetPtRel", idx)<_ptRelWP[kTight] && _vc->get("LepGood_relIso03", idx)>_isoWP[kTight])) return false;
    }
  }
  else { //mva case
    if( !lepMvaId(idx, kTight) ) return false;
  }
  return true;
}


bool 
FakeEstim::goodJetSelection(int jetIdx){ 
  if(_vc->get("Jet_pt", jetIdx)<40.0) return false;
  return true;
}

float 
FakeEstim::HT(){
  float ht = 0;
  for(unsigned int i=0; i<_nJets; ++i) 
    ht += _jets[i]->pt();
  
  return ht;
}


//==================================================================================================
void 
FakeEstim::retrieveObjects(){

  //clear collections
  _allLeps.clear();
  _looseLeps.clear();
  _leptons.clear();
  _nonFullIdLeps.clear();
  _fullIdLeps.clear();
  _lepsForFR.clear();
  _lepsForFRIdx.clear();
  
  _jets.clear();
  
  _looseLepsIdx.clear();
  _leptonsIdx.clear();
  _nonFullIdLepsIdx.clear();
  _fullIdLepsIdx.clear();
  _tmpLepIdx.clear();

  //========================
  bool isMu = false;
  for(int i=0; i<_vc->get("nLepGood"); ++i){

    isMu=std::abs(_vc->get("LepGood_pdgId", i))==13;
    
    // if(!isMu) continue;
    // if(std::abs( _vc->get("LepGood_eta",i))>1.5) continue;

    Candidate* lepCand= Candidate::create(_vc->get("LepGood_pt",i),
					  _vc->get("LepGood_eta",i),
					  _vc->get("LepGood_phi",i),
					  _vc->get("LepGood_pdgId",i),
					  _vc->get("LepGood_charge",i),
					  isMu?0.105:0.0005);

    _allLeps.push_back( lepCand );

    if( !(isMu?isMuSelected(i,kLoose):isElSelected(i,kLoose)) ) continue;
    _looseLeps.push_back(lepCand);
    _looseLepsIdx.push_back(i);
      
    if( !(isMu?isMuSelected(i,kTight):isElSelected(i,kTight)) ) continue;
    _leptons.push_back(lepCand);
    _leptonsIdx.push_back(i);

    //cout<<i<<" ==> "<<lepCand->charge()<<"   "<<lepCand->pt()<<endl;

    //cout<<i<<" ===> "<<_vc->get("LepGood_sip3d", i)<<"   "<<_vc->get("LepGood_relIso03", i)<<"   "<<_vc->get("LepGood_charge",i)<<endl;

    fill("jetPtRatio", _vc->get("LepGood_jetPtRatio",i) );
    fill("jetPtRel", _vc->get("LepGood_jetPtRel",i) );
    fill("miniIso", _vc->get("LepGood_miniRelIso",i) );
    

    if( !passFakeId(i, isMu) ) {//non fully identified leptons
      _nonFullIdLeps.push_back(lepCand);
      _nonFullIdLepsIdx.push_back(i);
    }
    else {
      _fullIdLeps.push_back(lepCand);
      _tmpLepIdx.push_back(i);
    }

  }

  
  // cout<<_looseLeps.size()<<"    "<<_leptons.size()
  //     <<"  ==>  "<<_nonFullIdLeps.size()<<"    "<<_fullIdLeps.size()<<endl;

  _nLooseLeps=_looseLeps.size();
    
  for(int i = 0; i < _vc->get("nJet40"); ++i){
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

//============================================================
bool 
FakeEstim::signalSelection(){
  
  if(!makeCut<float>( _HT       , _valCutHTSR    , _cTypeHTSR  , "SR HT selection" , _upValCutHTSR      ) ) return false;
  if(!makeCut<float>( _met->pt(), _valCutMETSR   , _cTypeMETSR , "SR MET selection", _upValCutMETSR      ) ) return false;

  if(_au->simpleCut( _HT, _valCutHTCondSR, _cTypeHTCondSR) ) {
    if(!makeCut<float>( _met->pt(), _valCutMETHighSR, _cTypeMETHighSR, "SR MET selection", _upValCutMETHighSR ) ) return false;
  }
  else {
    if(!makeCut<float>( _met->pt(), _valCutMETLowSR , _cTypeMETLowSR , "SR MET selection" , _upValCutMETLowSR ) ) return false;
  }

  if(!makeCut<int>( _nJets         , _valCutNJetsSR , _cTypeNJetsSR , "SR jet multiplicity"  , _upValCutNJetsSR ) ) return false;
  if(!makeCut<int>(_vc->get(_btag), _valCutNBJetsSR, _cTypeNBJetsSR, "SR b-jet multiplicity", _upValCutNBJetsSR) ) return false;

  return true;
} 

//=========================================================

bool 
FakeEstim::ssEventSelection(){

  // SF: CHOOSE a SS PAIR, maximizing the number of muons and then pT
  int charge = 0;
  int flavor = 0;
  int flavortmp = 0;
  int id = 0;
  int idtmp = 0;
  bool isSS = false;
  _lep_idx1=-1;
  _lep_idx2=-1;

  int id1,id2;

  for(unsigned int il1 = 0; il1 < _leptons.size(); ++il1){
    for(unsigned int il2 = il1+1; il2 < _leptons.size(); ++il2){
      
      counter("denom",kSelId);

      if (_leppt=="hh" && _leptons[il1]->pt()<25.) continue;
      if (_leppt=="hh" && _leptons[il2]->pt()<25.) continue;
      if (_leppt=="hl" && !(_leptons[il1]->pt()<25. && _leptons[il2]->pt()>25.) ) continue;
      if (_leppt=="hl" && !(_leptons[il1]->pt()>25. && _leptons[il2]->pt()<25.) ) continue;
      if (_leppt=="ll" && _leptons[il1]->pt()>25.) continue;
      if (_leppt=="ll" && _leptons[il2]->pt()>25.) continue;

      counter("ptSel",kSelId);

      charge = _leptons[il1]->charge()*_leptons[il2]->charge();
      if(charge<0)         continue; // if the pair is OS skip

      counter("charge",kSelId);

      flavortmp = std::abs(_leptons[il1]->pdgId())+std::abs( _leptons[il2]->pdgId());
      
      if(_lepflav=="mm"  && flavortmp!=26 ) continue;
      if(_lepflav=="ee"  && flavortmp!=22 ) continue;
      if( (_lepflav=="em" || _lepflav=="me")  && flavortmp!=24 ) continue;
      
      counter("flavor",kSelId);

      
      id2=passFakeId( _leptonsIdx[il2], std::abs(_leptons[il1]->pdgId())==13 );
      id1=passFakeId( _leptonsIdx[il1], std::abs(_leptons[il1]->pdgId())==13 );
      idtmp= id1+id2;
      if(id>idtmp) continue; //giving priority to isolated objects
      
      if(flavor>flavortmp) continue; // if the new pair has less muons skip.

      // fiding leptons which can participate to the fake bkg estimation
      if(!id1)  _lepsForFR.push_back( _leptons[il1] ); 
      if(!id2)  _lepsForFR.push_back( _leptons[il2] ); 
      // cout<<il1<<"   "<<il2<<" ===> "<<id1<<"   "<<id2<<" |  "
      // 	  <<_leptons[il1]->charge()<<"   "<<_leptons[il2]->charge()<<" | "
      // 	  <<passFakeId( _leptonsIdx[il1] )<<"  "<<passFakeId( _leptonsIdx[il2] )<<" |  "<<_lepsForFR.size()<<endl;
      // cout<<il1<<"/"<<il2<<" ==> "<<_leptonsIdx[il1]<<"/"<<_leptonsIdx[il2]<<" ==> "<<passFakeId(_leptonsIdx[il1])<<"/"<<passFakeId(_leptonsIdx[il2])<<endl;
      //cout<<" <> "<<id<<"   "<<idtmp<<endl;
      
      //and retrieve the best pair of the event
      counter("endprocess",kSelId);

      flavor = flavortmp;
      id = idtmp;
      _l1Cand = _leptons[il1];   _lep_idx1 = _leptonsIdx[il1];
      _l2Cand = _leptons[il2];   _lep_idx2 = _leptonsIdx[il2]; 
      isSS = true;
    }
  }

  if(_lep_idx1!=-1 && passFakeId(_lep_idx1, std::abs(_leptons[_lep_idx1]->pdgId())==13 )) {
    _fullIdLepsIdx.push_back(_lep_idx1);
  }
  if(_lep_idx2!=-1 && passFakeId(_lep_idx2, std::abs(_leptons[_lep_idx2]->pdgId())==13 )) {
    _fullIdLepsIdx.push_back(_lep_idx2);
  }


  if(isSS) return true;
  
  
  // cout<<"   =========>   "<<_lep_idx1<<"   "<<_lep_idx2<<"   "<<passFakeId(_lep_idx1)<<"   "<<passFakeId(_lep_idx2)<<endl;
  // cout<<_fullIdLepsIdx.size()<<" ||  "<<_fullIdLeps.size()<<" +  "<<_nonFullIdLeps.size()
  //     <<" =  "<<_leptons.size()<<endl;

  // if      (_lepflav=="mm"  && flavor==26 && isSS) return true;
  // else if (_lepflav=="em"  && flavor==24 && isSS) return true;
  // else if (_lepflav=="ee"  && flavor==22 && isSS) return true;
  // else if (_lepflav=="all" && isSS)               return true;

  return false;
}

bool
FakeEstim::alternateSSEventSelection() {
  
  // SF: CHOOSE a SS PAIR, maximizing the number of muons and then pT
  int charge = 0;
  int flavor = 0;
  int flavortmp = 0;
  int id = 0;
  int idtmp = 0;
  bool isSS = false;
  _lep_idx1=-1;
  _lep_idx2=-1;

  _nIso=-1;
  int nOSPairs=0;
  _nSelPair2Iso=0;
  _nSelPair1Iso=0;
  _nSelPair0Iso=0;

  int id1,id2;

  //========================================================================
  //first fully identified leptons => signal
  for(unsigned int il1 = 0; il1 < _fullIdLeps.size(); ++il1){
    for(unsigned int il2 = il1+1; il2 < _fullIdLeps.size(); ++il2){

      if (_leppt=="hh" && _fullIdLeps[il1]->pt()<25.) continue;
      if (_leppt=="hh" && _fullIdLeps[il2]->pt()<25.) continue;
      if (_leppt=="hl" && !(_fullIdLeps[il1]->pt()<25. && _fullIdLeps[il2]->pt()>25.) ) continue;
      if (_leppt=="hl" && !(_fullIdLeps[il1]->pt()>25. && _fullIdLeps[il2]->pt()<25.) ) continue;
      if (_leppt=="ll" && _fullIdLeps[il1]->pt()>25.) continue;
      if (_leppt=="ll" && _fullIdLeps[il2]->pt()>25.) continue;

      charge = _fullIdLeps[il1]->charge()*_fullIdLeps[il2]->charge();
      if(charge<0) continue; // if the pair is OS skip
      else nOSPairs++;

      flavortmp = std::abs(_fullIdLeps[il1]->pdgId())+std::abs( _fullIdLeps[il2]->pdgId());
      if(_lepflav=="mm" && flavortmp!=26 ) continue;
      if(_lepflav=="ee" && flavortmp!=22 ) continue;
      if( (_lepflav=="em" || _lepflav=="me") && flavortmp!=24 ) continue;
      if(flavor>flavortmp) continue; // if the new pair has less muons skip.

      flavor = flavortmp;
      id = idtmp;
      _l1Cand = _fullIdLeps[il1];   _lep_idx1 = _tmpLepIdx[il1];
      _l2Cand = _fullIdLeps[il2];   _lep_idx2 = _tmpLepIdx[il2]; 
      isSS = true;
      _nSelPair2Iso++;
    }
  }
  
  if(isSS) {_nIso=2; setWorkflow(kGlobal); return true;}

  //if(nOSPairs!=0) return false;
  //=======================================================================
  //now, mixing events with isolated and non isolated leptons
  for(unsigned int il1 = 0; il1 < _fullIdLeps.size(); ++il1){
    for(unsigned int il2 = 0; il2 < _nonFullIdLeps.size(); ++il2){
      
      //cout<<"non full Id mIso: "<<_vc->get("LepGood_miniRelIso",_nonFullIdLepsIdx[il2])<<endl;

      if (_leppt=="hh" && _fullIdLeps[il1]->pt()<25.) continue;
      if (_leppt=="hh" && _nonFullIdLeps[il2]->pt()<25.) continue;
      if (_leppt=="hl" && !(_fullIdLeps[il1]->pt()<25. && _nonFullIdLeps[il2]->pt()>25.) ) continue;
      if (_leppt=="hl" && !(_fullIdLeps[il1]->pt()>25. && _nonFullIdLeps[il2]->pt()<25.) ) continue;
      if (_leppt=="ll" && _fullIdLeps[il1]->pt()>25.) continue;
      if (_leppt=="ll" && _nonFullIdLeps[il2]->pt()>25.) continue;

      if(!genMatchedToFake(_nonFullIdLepsIdx[il2]) && !genMatchedToFake(_tmpLepIdx[il1]) ) continue;

      charge = _fullIdLeps[il1]->charge()*_nonFullIdLeps[il2]->charge();
      if(charge<0) continue; // if the pair is OS skip

      flavortmp = std::abs(_fullIdLeps[il1]->pdgId())+std::abs( _nonFullIdLeps[il2]->pdgId());
      if(_lepflav=="mm"  && flavortmp!=26 ) continue;
      if(_lepflav=="ee"  && flavortmp!=22 ) continue;
      if( (_lepflav=="em" || _lepflav=="me")  && flavortmp!=24 ) continue;
      //if(flavor>flavortmp) continue; // if the new pair has less muons skip.

      if( find(_lepsForFR.begin(),_lepsForFR.end(),_nonFullIdLeps[il2])==_lepsForFR.end()) {
	_lepsForFR.push_back(_nonFullIdLeps[il2]);
	_lepsForFRIdx.push_back(_nonFullIdLepsIdx[il2]);
      }
      
      flavor = flavortmp;
      id = idtmp;
      _l1Cand = _fullIdLeps[il1];   _lep_idx1 = _tmpLepIdx[il1];
      _l2Cand = _nonFullIdLeps[il2];   _lep_idx2 = _nonFullIdLepsIdx[il2]; 
      //_lepsForFR.push_back(_l2Cand);
      isSS = true;
      _nSelPair1Iso++;

    }
  }
 
  if(isSS) { _nIso=1; setWorkflow(kOneIso); return true;}

  //=======================================================================
  //now, mixing events with isolated and non isolated leptons
  for(unsigned int il1 = 0; il1 < _nonFullIdLeps.size(); ++il1){
    for(unsigned int il2 = il1+1; il2 < _nonFullIdLeps.size(); ++il2){

      if (_leppt=="hh" && _nonFullIdLeps[il1]->pt()<25.) continue;
      if (_leppt=="hh" && _nonFullIdLeps[il2]->pt()<25.) continue;
      if (_leppt=="hl" && !(_nonFullIdLeps[il1]->pt()<25 && _nonFullIdLeps[il2]->pt()>25) ) continue;
      if (_leppt=="hl" && !(_nonFullIdLeps[il1]->pt()>25 && _nonFullIdLeps[il2]->pt()<25) ) continue;
      if (_leppt=="ll" && _nonFullIdLeps[il1]->pt()>25.) continue;
      if (_leppt=="ll" && _nonFullIdLeps[il2]->pt()>25.) continue;

      charge = _nonFullIdLeps[il1]->charge()*_nonFullIdLeps[il2]->charge();
      if(charge<0) continue; // if the pair is OS skip

      flavortmp = std::abs(_nonFullIdLeps[il1]->pdgId())+std::abs( _nonFullIdLeps[il2]->pdgId());
      if(_lepflav=="mm"  && flavortmp!=26 ) continue;
      if(_lepflav=="ee"  && flavortmp!=22 ) continue;
      if( (_lepflav=="em" || _lepflav=="me")  && flavortmp!=24 ) continue;
      //if(flavor>flavortmp) continue; // if the new pair has less muons skip.

      if( find(_lepsForFR.begin(),_lepsForFR.end(),_nonFullIdLeps[il1])==_lepsForFR.end()) {
	_lepsForFR.push_back(_nonFullIdLeps[il1]);
      _lepsForFRIdx.push_back(_nonFullIdLepsIdx[il1]);
    }

      if( find(_lepsForFR.begin(),_lepsForFR.end(),_nonFullIdLeps[il2])==_lepsForFR.end()) {
	_lepsForFR.push_back(_nonFullIdLeps[il2]);
	_lepsForFRIdx.push_back(_nonFullIdLepsIdx[il2]);
      }

      flavor = flavortmp;
      id = idtmp;
      _l1Cand = _nonFullIdLeps[il1];   _lep_idx1 = _nonFullIdLepsIdx[il1];
      _l2Cand = _nonFullIdLeps[il2];   _lep_idx2 = _nonFullIdLepsIdx[il2]; 
      // _lepsForFR.push_back(_l1Cand);
      // _lepsForFR.push_back(_l2Cand);

      isSS = true;
      _nSelPair0Iso++;
    }
  }

  if(isSS) {_nIso=0; setWorkflow(kNoIso); return true;}

  return false;
}



bool
FakeEstim::mllVetoSelection(){

  // for(unsigned int i = 0; i < _tightVetoLeps.size(); ++i){
  //   if(_l1Cand!=_tightVetoLeps[i] && mllZVeto(_l1Cand, _tightVetoLeps[i])) return false;
  //   if(_l2Cand!=_tightVetoLeps[i] && mllZVeto(_l2Cand, _tightVetoLeps[i])) return false;
  // }
  //cout<<" aqui "<<endl;
  for(unsigned int i = 0; i < _allLeps.size(); ++i) {
    if(_l1Cand!=_allLeps[i] && mllZVeto(_l1Cand, _allLeps[i])) return false;
    if(_l2Cand!=_allLeps[i] && mllZVeto(_l2Cand, _allLeps[i])) return false;
    if(_l1Cand!=_allLeps[i] && mllLMGVeto(_l1Cand, _allLeps[i])) return false;
    if(_l2Cand!=_allLeps[i] && mllLMGVeto(_l2Cand, _allLeps[i])) return false;
  }
  
  return true;
}

bool 
FakeEstim::mllLMGVeto(Candidate* cand, Candidate* veto){
  
  if((std::abs(veto->pdgId()) == 11 && veto->pt() < 7) || (std::abs(veto->pdgId()) == 13 && veto->pt() < 5)) return false;
  float mll = Candidate::create(cand, veto)->mass();

  //cout<<" mll "<<mll<<endl;

  if(mll <= 8.0) return true;
  if(cand->charge()==veto->charge() ) return false;
  if(mll <= 12.0) return true;

  return false;
}


bool 
FakeEstim::mllZVeto(Candidate* cand, Candidate* veto){
 
  if(cand->pt() < 10 || veto->pt() < 10) return false;
  float mll = Candidate::create(cand, veto)->mass();
  
  //cout<<" dwdmll "<<mll<<endl;

  if(cand->charge()==veto->charge() ) return false;
  if(std::abs(cand->pdgId())==std::abs(veto->pdgId()) ) {
    if(mll >= 76.0 && mll <= 106.0) return true;
  }

  return false;
}


//=====================================================================
// signal region selection

void 
FakeEstim::setSignalRegion() {
  
  _btag = "nBJetMedium25";
  
  if(_SR == "SR00") {
    setCut("HTSR"     ,   80, ">" );
    setCut("METSR"    , -100, ">" );
    setCut("HTCondSR" ,  500, "<" );
    setCut("METHighSR",   30, ">" );
    setCut("METLowSR" , -100, ">" );
    setCut("NJetsSR"  ,    2, ">=");
    setCut("NBJetsSR" ,    0, "=" );
  }
  
  else if(_SR == "SR10") {
    setCut("HTSR"     ,   80, ">" );
    setCut("METSR"    , -100, ">" );
    setCut("HTCondSR" ,  500, "<" );
    setCut("METHighSR",   30, ">" );
    setCut("METLowSR" , -100, ">" );
    setCut("NJetsSR"  ,    2, ">=");
    setCut("NBJetsSR" ,    1, "=" );
  }
  
  else if(_SR == "SR20") {
    setCut("HTSR"     ,   80, ">" );
    setCut("METSR"    , -100, ">" );
    setCut("HTCondSR" ,  500, "<" );
    setCut("METHighSR",   30, ">" );
    setCut("METLowSR" , -100, ">" );
    setCut("NJetsSR"  ,    2, ">=");
    setCut("NBJetsSR" ,    2, "=" );
  }
  else if(_SR == "SR30") {
    setCut("HTSR"     ,   80, ">" );
    setCut("METSR"    , -100, ">" );
    setCut("HTCondSR" ,  500, "<" );
    setCut("METHighSR",   30, ">" );
    setCut("METLowSR" , -100, ">" );
    setCut("NJetsSR"  ,    2, ">=");
    setCut("NBJetsSR" ,    3, ">=");
  }

  // CH: SUS-13-013 cuts
  else if(_SR == "SR01") {
    setCut("HTSR"     ,  200, "[]", 400);
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    0, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR02") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    0, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR03") {
    setCut("HTSR"     ,  200, "[]", 400 );
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    0, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR04") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    0, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR05") {
    setCut("HTSR"     ,  200, "[]", 400);
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    0, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR06") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    0, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR07") {
    setCut("HTSR"     ,  200, "[]", 400 );
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    0, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR08") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    0, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
 

  else if(_SR == "SR11") {
    setCut("HTSR"     ,  200, "[]", 400);
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    1, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR12") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    1, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR13") {
    setCut("HTSR"     ,  200, "[]", 400 );
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    1, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR14") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    1, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR15") {
    setCut("HTSR"     ,  200, "[]", 400);
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    1, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR16") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    1, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR17") {
    setCut("HTSR"     ,  200, "[]", 400 );
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    1, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR18") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    1, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
 

  else if(_SR == "SR21") {
    setCut("HTSR"     ,  200, "[]", 400);
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    2, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR22") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    2, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR23") {
    setCut("HTSR"     ,  200, "[]", 400 );
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    2, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR24") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    2, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR25") {
    setCut("HTSR"     ,  200, "[]", 400);
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    2, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR26") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    2, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR27") {
    setCut("HTSR"     ,  200, "[]", 400 );
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    2, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR28") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    2, "=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }

  else if(_SR == "SR31") {
    setCut("HTSR"     ,  200, "[]", 400);
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    3, ">=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR32") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    3, ">=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR33") {
    setCut("HTSR"     ,  200, "[]", 400 );
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    3, ">=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR34") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,   50, "[]", 120);
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    3, ">=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR35") {
    setCut("HTSR"     ,  200, "[]", 400);
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    3, ">=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR36") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    2, "[]",   3);
    setCut("NBJetsSR" ,    3, ">=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR37") {
    setCut("HTSR"     ,  200, "[]", 400 );
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    3, ">=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }
  else if(_SR == "SR38") {
    setCut("HTSR"     ,  400, ">" );
    setCut("METSR"    ,  120, ">" );
    setCut("NJetsSR"  ,    4, ">=");
    setCut("NBJetsSR" ,    3, ">=" );
    setCut("HTCondSR" ,    0, "<" );
    setCut("METHighSR", -100, ">" );
    setCut("METLowSR" , -100, ">" );
  }


}


void 
FakeEstim::setCut(string var, float valCut, 
		  string cType, float upValCut) {

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

//================================================
bool 
FakeEstim::genMatchedToFake(int idx) {

  int id1  = _vc->get("LepGood_mcMatchId" ,idx); 
  //int id2  = _vc->get("LepGood_mcMatchId" , _lep_idx2); 
  // int any1 = _vc->get("LepGood_mcMatchAny", _lep_idx1); 
  // int any2 = _vc->get("LepGood_mcMatchAny", _lep_idx2);
  
  // cout<<id1<<"  "<<id2<<endl;


  if(id1==0) return true; // || id2==0

  // if(id1==0){
  //   //	if     (fromHadrTau(_lep_idx1)) counter("fakes from tau");
  //   if(any1 == 5             ) counter("fakes from b");
  //   else if(any1 == 4             ) counter("fakes from c");
  //   else if(any1 <= 3             ) counter("fakes from light");
  // }
  // else if(id2 == 0){
  //   //	if     (fromHadrTau(_lep_idx2)) counter("fakes from tau");
  //   if(any2 == 5             ) counter("fakes from b");
  //   else if(any2 == 4             ) counter("fakes from c");
  //   else if(any2 <= 3             ) counter("fakes from light");
  // }

  return false;
}

bool
FakeEstim::genMatchedMisCharge() {
  
  // int id1  = _vc->get("LepGood_mcMatchId" , _lep_idx1); 
  // int id2  = _vc->get("LepGood_mcMatchId" , _lep_idx2); 
  // int any1 = _vc->get("LepGood_mcMatchAny", _lep_idx1); 
  // int any2 = _vc->get("LepGood_mcMatchAny", _lep_idx2); 
  // if(id1*id2<0 ) return true;

  //manual charge check ==================
  // int lep=(id1==100)?_lep_idx1:_lep_idx2;
  // int lep2=(id2==100)?_lep_idx2:_lep_idx1;
  
  int nGenL=_vc->get("nGenPart");
  int pdgId1=0;
  int pdgId2=0;
  
  for(int ig=0;ig<nGenL;++ig) {
    // cout<< _vc->get("GenPart_pdgId",ig)<<"   "<<KineUtils::dR(_vc->get("LepGood_eta",_lep_idx1), _vc->get("GenPart_eta", ig),
    // 							      _vc->get("LepGood_phi",_lep_idx1), _vc->get("GenPart_phi", ig))<<"   "<<KineUtils::dR(_vc->get("LepGood_eta",_lep_idx2), _vc->get("GenPart_eta", ig),
    // 																		    _vc->get("LepGood_phi",_lep_idx2), _vc->get("GenPart_phi", ig))<<endl;
    if(pdgId1==0 && KineUtils::dR(_vc->get("LepGood_eta",_lep_idx1), _vc->get("GenPart_eta", ig),
		     _vc->get("LepGood_phi",_lep_idx1), _vc->get("GenPart_phi", ig))<0.05) { 
      pdgId1 = _vc->get("GenPart_pdgId",ig);
    }
    if(pdgId2==0 &&KineUtils::dR(_vc->get("LepGood_eta",_lep_idx2), _vc->get("GenPart_eta", ig),
		     _vc->get("LepGood_phi",_lep_idx2), _vc->get("GenPart_phi", ig))<0.05) { 
      pdgId2 = _vc->get("GenPart_pdgId",ig);
    }
      // cout<<_vc->get("LepGood_pdgId",lep)<<"   "<<_vc->get("GenPart_pdgId",ig)<<"   "<<_lep_idx1<<endl;
      // if(_vc->get("LepGood_pdgId",lep)*_vc->get("GenPart_pdgId",ig)<0) return true;
      // if(_vc->get("LepGood_pdgId",lep2)*_vc->get("GenPart_pdgId",ig)<0) return true;
    if(pdgId1!=0 && pdgId2!=0)
      break;
  }
  // cout<<pdgId1<<"   "<<pdgId2<<endl;
  if(pdgId1*pdgId2<0) return true;
  
  return false;
}

// vector<float>
// FakeEstim::getFakeRatios() {
//   vector<float> frs;
//   for(size_t i=0;i<_lepsForFR.size();i++) {
//     frs.push_back(getFR(_lepsForFR[i]));
//   }
    
// }

float 
FakeEstim::getFR(Candidate* cand, int idx) {
  string db;
  int wp=kTight;
  
  //if(_FR=="inc") {
    // if(std::abs(cand->pdgId())==11) db="AllEl";
    // if(std::abs(cand->pdgId())==13) db="AllMu";

  if(_lepiso == "multiIso") db="AllMuT";
  else if(_lepiso == "multiIsoHTight") {db="AllMuVT"; wp=kVTight;}
  else db="AllMuT";

  // }
  // else {
  //   if(std::abs(cand->pdgId())==11) db=_SR.substr(2,1)+"BEl";
  //   if(std::abs(cand->pdgId())==13) db=_SR.substr(2,1)+"BMu";
  // }
  
  //cout<<" fr: "<<std::min(cand->pt(),(float)49.9)<<"  "<<std::min(std::abs(cand->eta()),(float)2.4)<<" ==> "<<_dbm->getDBValue(db, std::min(cand->pt(),(float)49.9), std::min(std::abs(cand->eta()),(float)2.4) )<<endl;

  return _dbm->getDBValue(db, std::min( cand->pt()*(1+ max((float)0.,_vc->get("LepGood_miniRelIso",idx )-_multiIsoWP[kMiniIso][wp])),(float)99.9),
			  std::min(std::abs(cand->eta()),(float)2.39) );
}


// CandList
// FakeEstim::buildNonIsoLepList(int nIso) {

//   CandList lepsForFR;
//   size_t nLeps=_leptons.size();
  
//   bool notId1, notId2;
//   bool flav;
//   int flavor;
  
//   for(int il1=0;il1<nLeps;il1++) {
    
//     notId1=1-passFakeId( _leptonsIdx[il1] );
    
//     for(int il2=il1+1;il2<nLeps;il2++) {

//       if(_leptons[il1]->charge()*_leptons[il2]->charge()<0) continue;

//       flav=true;
//       flavor = std::abs(_leptons[il1]->pdgId())+std::abs( _leptons[il2]->pdgId());
//       if(_lepflav=="mm"  && flavor!=26 ) flav=false; 
//       if(_lepflav=="ee"  && flavor!=22 ) flav=false; 
//       if( (_lepflav=="em" || _lepflav=="me")  && flavor!=24 ) flav=false; 
//       if(!flav) continue;

      
//       notId2=1-passFakeId( _leptonsIdx[il2] );
      
//       if(nIso==2 && (!notId1 || !notId2) ) continue; 
//       if(nIso==1 && ( (notId1 && notId2) || (!notId1 && !notId2) )) continue;

//       if(notId1)  lepsForFR.push_back( _leptons[il1] ); 
//       if(notId2)  lepsForFR.push_back( _leptons[il2] ); 


//       if(ssPair && flav && notId1 && _nonFullIdLeps[il1]!=_l1Cand &&
// 	 _nonFullIdLeps[il1]!=_l2Cand )


//     }
//   }



// }

