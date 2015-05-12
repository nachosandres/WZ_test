#include "analysis/src/FakeEstim.hh"

#include <algorithm>
#include <sstream>
#include <iostream>

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

  _susyMod = new SusyModule(_vc);
  

  //extra input variables
  _lepflav = getCfgVarS("LEPFLAV");
  //_leppt   = getCfgVarS("LEPPT"  );
  _SR      = getCfgVarS("SR"     );
  _FR      = getCfgVarS("FR"     );
  _categorization = getCfgVarI("categorization");
 
  setSignalRegions();

  
  if(_FR=="mIsoCor") {
    _dbm->loadDb("AllElT","v3/FakeRatio_all_cut_mixisoVT_none_iso_all_all_out.root","MR_RElMapPtMIso_qcd_all_cut_mixisoVT_none_iso_all_all");
    _dbm->loadDb("AllElVT","v3/FakeRatio_all_cut_mixisoHT_none_iso_all_all_out.root","MR_RElMapPtMIso_qcd_all_cut_mixisoHT_none_iso_all_all");
    
    _dbm->loadDb("AllMuT","v3/FakeRatio_all_cut_mixisoT_none_iso_all_all_out.root","MR_RMuMapPtMIso_qcd_all_cut_mixisoT_none_iso_all_all");
    _dbm->loadDb("AllMuVT","v3/FakeRatio_all_cut_mixisoVT_none_iso_all_all_out.root","MR_RMuMapPtMIso_qcd_all_cut_mixisoVT_none_iso_all_all");
  }
  else if(_FR=="mIsoAlCor") {
    _dbm->loadDb("AllElT","v3/FakeRatio_all_cut_mixisoVT_none_iso_all_all_out.root","MR_RElMapPtMIso_qcd_all_cut_mixisoVT_none_iso_all_all");
    _dbm->loadDb("AllElVT","v3/FakeRatio_all_cut_mixisoHT_none_iso_all_all_out.root","MR_RElMapPtMIso_qcd_all_cut_mixisoHT_none_iso_all_all");
    
    _dbm->loadDb("AllMuT","v3/FakeRatio_all_cut_mixisoT_none_iso_all_all_out.root","MR_RMuMapPtMIso_qcd_all_cut_mixisoT_none_iso_all_all");
    _dbm->loadDb("AllMuVT","v3/FakeRatio_all_cut_mixisoVT_none_iso_all_all_out.root","MR_RMuMapPtMIso_qcd_all_cut_mixisoVT_none_iso_all_all");
  }
  else if(_FR=="mIsoptJCor") {
    _dbm->loadDb("AllElT","v3/FakeRatio_all_cut_mixisoVT_none_iso_all_all_out.root","MR_RElMapPtMIso_qcd_all_cut_mixisoVT_none_iso_all_all");
    _dbm->loadDb("AllElVT","v3/FakeRatio_all_cut_mixisoHT_none_iso_all_all_out.root","MR_RElMapPtMIso_qcd_all_cut_mixisoHT_none_iso_all_all");
    
    _dbm->loadDb("AllMuT","v3/FakeRatio_all_cut_mixisoT_none_iso_all_all_out.root","MR_RMuMapPtMIso_qcd_all_cut_mixisoT_none_iso_all_all");
    _dbm->loadDb("AllMuVT","v3/FakeRatio_all_cut_mixisoVT_none_iso_all_all_out.root","MR_RMuMapPtMIso_qcd_all_cut_mixisoVT_none_iso_all_all");
  }
  else { //default pt one
    _dbm->loadDb("AllElT","v3/FakeRatio_all_cut_mixisoVT_none_iso_all_all_out.root","MR_RElMapPtMIso_qcd_all_cut_mixisoVT_none_iso_all_all");
    _dbm->loadDb("AllElVT","v3/FakeRatio_all_cut_mixisoHT_none_iso_all_all_out.root","MR_RElMapPtMIso_qcd_all_cut_mixisoHT_none_iso_all_all");
    
    _dbm->loadDb("AllMuT","v3/FakeRatio_all_cut_mixisoT_none_iso_all_all_out.root","MR_RMuMapPtMIso_qcd_all_cut_mixisoT_none_iso_all_all");
    _dbm->loadDb("AllMuVT","v3/FakeRatio_all_cut_mixisoVT_none_iso_all_all_out.root","MR_RMuMapPtMIso_qcd_all_cut_mixisoVT_none_iso_all_all");
  }


  //additional workflow
  //addWorkflow( kGenMisCharge, "GenMisCharge" );
  // addWorkflow( kGenFake, "GenFake" );
  // addWorkflow( kOneIso, "OneFake" );
  // addWorkflow( kNoIso, "DoubleFake" );

  //addWorkflow( kLowMETMT, "LowMETMT" );

  //_au->addCategory(kSelId,"selection ID");


  _fakeEl=0;
  _fakeMu=0;
  _nDFake=0;
  _nSFake=0;
  _nCharge=0;
  _nOther=0;

  int nCateg=33; //78
  _categs.resize(nCateg);
  string srs[33]={ 
    "SR1A", "SR2A", "SR3A", "SR4A", "SR5A", "SR6A", "SR7A", "SR8A",
    "SR9A", "SR10A", "SR11A", "SR12A", "SR13A", "SR14A", "SR15A", "SR16A",
    "SR17A", "SR18A", "SR19A", "SR20A", "SR21A", "SR22A", "SR23A", "SR24A",
    "SR25A", "SR26A", "SR27A", "SR28A", "SR29A", "SR30A", "SR31A", "SR32A",

    // "SR1B", "SR2B", "SR3B", "SR4B", "SR5B", "SR6B", "SR7B", "SR8B",
    // "SR9B", "SR10B", "SR11B", "SR12B", "SR13B", "SR14B", "SR15B", "SR16B",
    // "SR17B", "SR18B", "SR19B", "SR20B", "SR21B", "SR22B", "SR23B", "SR24B",
    // "SR25B","SR26B"

    // "SR1C", "SR2C", "SR3C", "SR4C", "SR5C", "SR6C", "SR7C", "SR8C",
    
    "BR0H", //"BR0M", "BR0L",
    //"BR10H", //"BR10M", "BR10L",
    //"BR20H", //"BR20M", "BR20L",
    //"BR30H" //, "BR30M", "BR30L"
  };
  _categs.assign(srs, srs+nCateg);

  for(size_t ic=0;ic< _categs.size();ic++) {
    addWorkflow( ic+1, _categs[ic] );
  }

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

  _hm->addVariable("srcFake", 10, 0,10,"srcFake");

  _hm->addVariable("MET",500,0,1000,"#slash{E}_{T} [GeV]");
  _hm->addVariable("MTmin",500,0,1000,"min(M_{T,1}, M_{T,2}) [GeV]");
  _hm->addVariable("METVsMT",100,0,1000,100,0,1000,"#slash{E}_{T} [GeV]",
		   "min(M_{T,1}, M_{T,2}) [GeV]");

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
  // if(_isData && !makeCut<int>(_vc->get("HLT_DoubleMu"), 1, "=", "HLT DoubleMu") ) return;	
  // if(_isData && !makeCut<int>(_vc->get("HLT_DoubleEl"), 1, "=", "HLT DoubleEl") ) return;	
  // if(_isData && !makeCut<int>(_vc->get("HLT_MuEG")    , 1, "=", "HLT MuEG"    ) ) return;
  
  
  // lepton multiplicity
  //if(!makeCut( _leptons.size()>=2, "lepMult" ) ) return;
  if(!makeCut( _vc->get("nLepGood10_Mini")>=2, "lepMult" ) ) return;

  if(!makeCut<float>( _vc->get("mZ1cut10TL_Mini"), 76 , "]![", "Z veto",  106 ) ) return;
  if(!makeCut<float>( _vc->get("minMllAFOSTL_Mini"), 12 , ">", "g veto OS") ) return;
  if(!makeCut<float>( _vc->get("minMllAFASTL_Mini"), 8 , ">", "g veto AS" ) ) return;
  

  // // selecting best same-sign pair 
  // bool is_ss_event = alternateSSEventSelection(false);//ssEventSelection();
  // if(!makeCut( is_ss_event , "same-sign selection", "=") ) return;

  fillSkimTree();


  //MET and MT
  _mTmin=min( Candidate::create(_l1Cand, _met)->mass(),
		   Candidate::create(_l2Cand, _met)->mass() );

  fill("MET", _met->pt() );
  fill("MTmin", _mTmin );
  fill("METVsMT", _met->pt(), _mTmin );
  

  counter("change of weigth");
  
  if(_categorization) {
    categorize();
    counter("region splitting");
  }
  else {
    testRegion();

  }

  fill("NSelPair2Iso", _nSelPair2Iso );
  fill("NSelPair1Iso", _nSelPair1Iso );
  fill("NSelPair0Iso", _nSelPair0Iso );
  

  fill("NIso",_fullIdLeps.size() );
  fill("NNonIso",_nonFullIdLeps.size() );
  

  cout << Form("%1d %9d %12d\t%2d\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%2d",
	       _vc->get("run"),
	       _vc->get("lumi"),
	       _vc->get("event"),
	       _vc->get("nLepGood"),
	       _l1Cand->pdgId(),
	       _l1Cand->pt(),
	       _l2Cand->pdgId(),
	       _l2Cand->pt(),
	       _nJets,
	       _nBJets,
	       _met->pt(),
	       _HT,
	       _categs[ getCurrentWorkflow()-1 ].substr(2, _categs[ getCurrentWorkflow()-1 ].size()-3-1)
	       ) << endl;


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
  // bool isMu = false;
  // for(int i=0; i<_vc->get("nLepGood"); ++i){

  //   isMu=std::abs(_vc->get("LepGood_pdgId", i))==13;
    
  //   Candidate* lepCand= Candidate::create(_vc->get("LepGood_pt",i),
  // 					  _vc->get("LepGood_eta",i),
  // 					  _vc->get("LepGood_phi",i),
  // 					  _vc->get("LepGood_pdgId",i),
  // 					  _vc->get("LepGood_charge",i),
  // 					  isMu?0.105:0.0005);

  //   _allLeps.push_back( lepCand );

  //   if( !(isMu?_susyMod->muIdSel(i, SusyModule::kLoose):_susyMod->elIdSel(i, SusyModule::kLoose)) ) continue;
  //   _looseLeps.push_back(lepCand);
  //   _looseLepsIdx.push_back(i);
      
  //   if( !(isMu?_susyMod->muIdSel(i, SusyModule::kTight):_susyMod->elIdSel(i, SusyModule::kTight)) ) continue;
  //   _leptons.push_back(lepCand);
  //   _leptonsIdx.push_back(i);

  //   fill("jetPtRatio", _vc->get("LepGood_jetPtRatio",i) );
  //   fill("jetPtRel", _vc->get("LepGood_jetPtRel",i) );
  //   fill("miniIso", _vc->get("LepGood_miniRelIso",i) );
    

  //   if( ! _susyMod->multiIsoSel(i, isMu?SusyModule::kMedium:SusyModule::kTight) ) {//non fully identified leptons
  //     _nonFullIdLeps.push_back(lepCand);
  //     _nonFullIdLepsIdx.push_back(i);
  //   }
  //   else {
  //     _fullIdLeps.push_back(lepCand);
  //     _tmpLepIdx.push_back(i);
  //   }

  // }

  _lep_idx1 = _vc->get("iL1T");
  _lep_idx2 = _vc->get("iL2T");

  bool isMu=std::abs(_vc->get("LepGood_pdgId", _lep_idx1))==13;
  _l1Cand = Candidate::create(_vc->get("LepGood_pt", _lep_idx1),
			      _vc->get("LepGood_eta", _lep_idx1),
			      _vc->get("LepGood_phi", _lep_idx1),
			      _vc->get("LepGood_pdgId", _lep_idx1),
			      _vc->get("LepGood_charge", _lep_idx1),
			      isMu?0.105:0.0005);
  
  isMu=std::abs(_vc->get("LepGood_pdgId", _lep_idx2))==13;
  _l2Cand = Candidate::create(_vc->get("LepGood_pt", _lep_idx2),
			      _vc->get("LepGood_eta",_lep_idx2),
			      _vc->get("LepGood_phi",_lep_idx2),
			      _vc->get("LepGood_pdgId",_lep_idx2),
			      _vc->get("LepGood_charge",_lep_idx2),
			      isMu?0.105:0.0005);

  _nLooseLeps=_looseLeps.size();
    
  // for(int i = 0; i < _vc->get("nJet40"); ++i){
  //   if(_susyMod->jetSel(i)) {
  //     _jets.push_back( Candidate::create(_vc->get("Jet_pt", i),
  // 					 _vc->get("Jet_eta", i),
  // 					 _vc->get("Jet_phi", i) ) );
  //   }
  // }

  _nJets  = _vc->get("nJet40_Mini");//_jets.size();
  _nBJets = _vc->get("nBJetMedium25_Mini");

  _HT  = _vc->get("htJet40j_Mini");//_susyMod->HT( &(_jets) );
  _met = Candidate::create(_vc->get("met_pt"), _vc->get("met_phi") );
  _metPt = _met->pt();

}

//=========================================================

bool
FakeEstim::alternateSSEventSelection(bool switchWF) {
  
  //MM: could be done in one round, but safer and easier to understand that way 

  // SF: CHOOSE a SS PAIR, maximizing the number of muons and then pT
  int charge = 0;
  int flavor = 0;
  int flavortmp = 0;
  bool isSS = false;
  _lep_idx1=-1;
  _lep_idx2=-1;

  _nIso=-1;
  int nOSPairs=0;
  _nSelPair2Iso=0;
  _nSelPair1Iso=0;
  _nSelPair0Iso=0;

  //========================================================================
  //first fully identified leptons => signal
  for(unsigned int il1 = 0; il1 < _fullIdLeps.size(); ++il1){
    for(unsigned int il2 = il1+1; il2 < _fullIdLeps.size(); ++il2){

      // if (_leppt=="hh" && _fullIdLeps[il1]->pt()<25.) continue;
      // if (_leppt=="hh" && _fullIdLeps[il2]->pt()<25.) continue;
      // if (_leppt=="hl" && (_leptons[il1]->pt()>25. && _leptons[il2]->pt()>25.) ) continue;
      // if (_leppt=="hl" && (_leptons[il1]->pt()<25. && _leptons[il2]->pt()<25.) ) continue;
      // if (_leppt=="ll" && _fullIdLeps[il1]->pt()>25.) continue;
      // if (_leppt=="ll" && _fullIdLeps[il2]->pt()>25.) continue;

      charge = _fullIdLeps[il1]->charge()*_fullIdLeps[il2]->charge();
      if(charge<0) continue; // if the pair is OS skip
      else nOSPairs++;

      flavortmp = std::abs(_fullIdLeps[il1]->pdgId())+std::abs( _fullIdLeps[il2]->pdgId());
      if(_lepflav=="mm" && flavortmp!=26 ) continue;
      if(_lepflav=="ee" && flavortmp!=22 ) continue;
      if( (_lepflav=="em" || _lepflav=="me") && flavortmp!=24 ) continue;
      //if(flavor>flavortmp) continue; // if the new pair has less muons skip.

      flavor = flavortmp;
      
      _l1Cand = _fullIdLeps[il1];   _lep_idx1 = _tmpLepIdx[il1];
      _l2Cand = _fullIdLeps[il2];   _lep_idx2 = _tmpLepIdx[il2]; 
      isSS = true;
      _nSelPair2Iso++;
    }
  }
  
  if(isSS) {_nIso=2; 
    if(switchWF) setWorkflow(kGlobal);
    return true;
  }
  
  return false; //MM: non isolated sideband disabled for now, workflows have to be fully duplicated
  //if(nOSPairs!=0) return false;
  //=======================================================================
  //now, mixing events with isolated and non isolated leptons
  for(unsigned int il1 = 0; il1 < _fullIdLeps.size(); ++il1){
    for(unsigned int il2 = 0; il2 < _nonFullIdLeps.size(); ++il2){
      
      // if (_leppt=="hh" && _fullIdLeps[il1]->pt()<25.) continue;
      // if (_leppt=="hh" && _nonFullIdLeps[il2]->pt()<25.) continue;
      // if (_leppt=="hl" && !(_fullIdLeps[il1]->pt()<25. && _nonFullIdLeps[il2]->pt()>25.) ) continue;
      // if (_leppt=="hl" && !(_fullIdLeps[il1]->pt()>25. && _nonFullIdLeps[il2]->pt()<25.) ) continue;
      // if (_leppt=="ll" && _fullIdLeps[il1]->pt()>25.) continue;
      // if (_leppt=="ll" && _nonFullIdLeps[il2]->pt()>25.) continue;

      if(!genMatchedToFake(_nonFullIdLepsIdx[il2]) && !genMatchedToFake(_tmpLepIdx[il1]) ) continue;

      charge = _fullIdLeps[il1]->charge()*_nonFullIdLeps[il2]->charge();
      if(charge<0) continue; // if the pair is OS skip

      flavortmp = std::abs(_fullIdLeps[il1]->pdgId())+std::abs( _nonFullIdLeps[il2]->pdgId());
      if(_lepflav=="mm"  && flavortmp!=26 ) continue;
      if(_lepflav=="ee"  && flavortmp!=22 ) continue;
      if( (_lepflav=="em" || _lepflav=="me")  && flavortmp!=24 ) continue;
  
      if( find(_lepsForFR.begin(),_lepsForFR.end(),_nonFullIdLeps[il2])==_lepsForFR.end()) {
	_lepsForFR.push_back(_nonFullIdLeps[il2]);
	_lepsForFRIdx.push_back(_nonFullIdLepsIdx[il2]);
      }
      
      flavor = flavortmp;
      
      _l1Cand = _fullIdLeps[il1];   _lep_idx1 = _tmpLepIdx[il1];
      _l2Cand = _nonFullIdLeps[il2];   _lep_idx2 = _nonFullIdLepsIdx[il2]; 
    
      isSS = true;
      _nSelPair1Iso++;

    }
  }
 
  if(isSS) { _nIso=1; 
    if(switchWF) setWorkflow(kOneIso); 
    return true;
  }

  //=======================================================================
  //now, mixing events with isolated and non isolated leptons
  for(unsigned int il1 = 0; il1 < _nonFullIdLeps.size(); ++il1){
    for(unsigned int il2 = il1+1; il2 < _nonFullIdLeps.size(); ++il2){

      // if (_leppt=="hh" && _nonFullIdLeps[il1]->pt()<25.) continue;
      // if (_leppt=="hh" && _nonFullIdLeps[il2]->pt()<25.) continue;
      // if (_leppt=="hl" && !(_nonFullIdLeps[il1]->pt()<25 && _nonFullIdLeps[il2]->pt()>25) ) continue;
      // if (_leppt=="hl" && !(_nonFullIdLeps[il1]->pt()>25 && _nonFullIdLeps[il2]->pt()<25) ) continue;
      // if (_leppt=="ll" && _nonFullIdLeps[il1]->pt()>25.) continue;
      // if (_leppt=="ll" && _nonFullIdLeps[il2]->pt()>25.) continue;

      charge = _nonFullIdLeps[il1]->charge()*_nonFullIdLeps[il2]->charge();
      if(charge<0) continue; // if the pair is OS skip

      flavortmp = std::abs(_nonFullIdLeps[il1]->pdgId())+std::abs( _nonFullIdLeps[il2]->pdgId());
      if(_lepflav=="mm"  && flavortmp!=26 ) continue;
      if(_lepflav=="ee"  && flavortmp!=22 ) continue;
      if( (_lepflav=="em" || _lepflav=="me")  && flavortmp!=24 ) continue;
 
      if( find(_lepsForFR.begin(),_lepsForFR.end(),_nonFullIdLeps[il1])==_lepsForFR.end()) {
	_lepsForFR.push_back(_nonFullIdLeps[il1]);
      _lepsForFRIdx.push_back(_nonFullIdLepsIdx[il1]);
    }

      if( find(_lepsForFR.begin(),_lepsForFR.end(),_nonFullIdLeps[il2])==_lepsForFR.end()) {
	_lepsForFR.push_back(_nonFullIdLeps[il2]);
	_lepsForFRIdx.push_back(_nonFullIdLepsIdx[il2]);
      }

      flavor = flavortmp;
      
      _l1Cand = _nonFullIdLeps[il1];   _lep_idx1 = _nonFullIdLepsIdx[il1];
      _l2Cand = _nonFullIdLeps[il2];   _lep_idx2 = _nonFullIdLepsIdx[il2]; 
 
      isSS = true;
      _nSelPair0Iso++;
    }
  }

  if(isSS) {_nIso=0;
    if(switchWF) setWorkflow(kNoIso);
    return true;
  }

  return false;
}



//=====================================================================
// signal region selection

void 
FakeEstim::setSignalRegions() {
    
  //objects ===========================================
  _val["NB"]  = &(_nBJets);
  _val["MT"]  = &(_mTmin);
  _val["MET"] = &(_metPt);
  _val["NJ"]  = &(_nJets);
  _val["HT"]  = &(_HT);

  //HH-regions =============================================================
  //0b-jet =================================================================
  
  if( _SR== "SR1A" ) {
    setSelLine("LL:=:hh|NB:=:0|MT:<:120|MET:[]:50:200|NJ:[]:2:4|HT:<:300");
  }
  else if( _SR== "SR2A" ) {
    setSelLine("LL:=:hh|NB:=:0|MT:<:120|MET:[]:50:200|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR3A" ) {
    setSelLine("LL:=:hh|NB:=:0|MT:<:120|MET:[]:50:200|NJ:>=:5|HT:<:300");
    setSelLine("LL:=:hh|NB:=:0|MT:<:120|MET:[]:200:500|NJ:>=:2|HT:<:300");
    setSelLine("LL:=:hh|NB:=:0|MT:>=:120|MET:50:[]:500|NJ:>=:2|HT:<:300");
  }
  else if( _SR== "SR4A" ) {
    setSelLine("LL:=:hh|NB:=:0|MT:<:120|MET:[]:50:200|NJ:>=:5|HT:[]:300:1600");
  }
  else if( _SR== "SR5A" ) {
    setSelLine("LL:=:hh|NB:=:0|MT:<:120|MET:[]:200:500|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR6A" ) {
    setSelLine("LL:=:hh|NB:=:0|MT:<:120|MET:[]:200:500|NJ:>=:5|HT:[]:300:1600");
  }
  else if( _SR== "SR7A" ) {
    setSelLine("LL:=:hh|NB:=:0|MT:>=:120|MET:[]:50:200|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR8A" ) {
    setSelLine("LL:=:hh|NB:=:0|MT:>=:120|MET:[]:50:200|NJ:>=:5|HT:[]:300:1600");
    setSelLine("LL:=:hh|NB:=:0|MT:>=:120|MET:[]:200:500|NJ:>=:2|HT:[]:300:1600");
  }

 //1b-jet =================================================================
  else if( _SR== "SR9A" ) {
    setSelLine("LL:=:hh|NB:=:1|MT:<:120|MET:[]:50:200|NJ:[]:2:4|HT:<:300");
  }
  else if( _SR== "SR10A" ) {
    setSelLine("LL:=:hh|NB:=:1|MT:<:120|MET:[]:50:200|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR11A" ) {
    setSelLine("LL:=:hh|NB:=:1|MT:<:120|MET:[]:50:200|NJ:>=:5|HT:<:300");
    setSelLine("LL:=:hh|NB:=:1|MT:<:120|MET:[]:200:500|NJ:>=:2|HT:<:300");
    setSelLine("LL:=:hh|NB:=:1|MT:>=:120|MET:50:[]:500|NJ:>=:2|HT:<:300");
  }
  else if( _SR== "SR12A" ) {
    setSelLine("LL:=:hh|NB:=:1|MT:<:120|MET:[]:50:200|NJ:>=:5|HT:[]:300:1600");
  }
  else if( _SR== "SR13A" ) {
    setSelLine("LL:=:hh|NB:=:1|MT:<:120|MET:[]:200:500|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR14A" ) {
    setSelLine("LL:=:hh|NB:=:1|MT:<:120|MET:[]:200:500|NJ:>=:5|HT:[]:300:1600");
  }
  else if( _SR== "SR15A" ) {
    setSelLine("LL:=:hh|NB:=:1|MT:>=:120|MET:[]:50:200|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR16A" ) {
    setSelLine("LL:=:hh|NB:=:1|MT:>=:120|MET:[]:50:200|NJ:>=:5|HT:[]:300:1600");
    setSelLine("LL:=:hh|NB:=:1|MT:>=:120|MET:[]:200:500|NJ:>=:2|HT:[]:300:1600");
  }
  
  //2b-jet =================================================================
 else if( _SR== "SR17A" ) {
    setSelLine("LL:=:hh|NB:=:2|MT:<:120|MET:[]:50:200|NJ:[]:2:4|HT:<:300");
  }
  else if( _SR== "SR18A" ) {
    setSelLine("LL:=:hh|NB:=:2|MT:<:120|MET:[]:50:200|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR19A" ) {
    setSelLine("LL:=:hh|NB:=:2|MT:<:120|MET:[]:50:200|NJ:>=:5|HT:<:300");
    setSelLine("LL:=:hh|NB:=:2|MT:<:120|MET:[]:200:500|NJ:>=:2|HT:<:300");
    setSelLine("LL:=:hh|NB:=:2|MT:>=:120|MET:50:[]:500|NJ:>=:2|HT:<:300");
  }
  else if( _SR== "SR20A" ) {
    setSelLine("LL:=:hh|NB:=:2|MT:<:120|MET:[]:50:200|NJ:>=:5|HT:[]:300:1600");
  }
  else if( _SR== "SR21A" ) {
    setSelLine("LL:=:hh|NB:=:2|MT:<:120|MET:[]:200:500|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR22A" ) {
    setSelLine("LL:=:hh|NB:=:2|MT:<:120|MET:[]:200:500|NJ:>=:5|HT:[]:300:1600");
  }
  else if( _SR== "SR23A" ) {
    setSelLine("LL:=:hh|NB:=:2|MT:>=:120|MET:[]:50:200|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR24A" ) {
    setSelLine("LL:=:hh|NB:=:2|MT:>=:120|MET:[]:50:200|NJ:>=:5|HT:[]:300:1600");
    setSelLine("LL:=:hh|NB:=:2|MT:>=:120|MET:[]:200:500|NJ:>=:2|HT:[]:300:1600");
  }
  
  //3b-jet =================================================================
  else if( _SR== "SR25A" ) {
    setSelLine("LL:=:hh|NB:>=:3|MT:<:120|MET:[]:50:200|NJ:>=:2|HT:<:300");
  }
  else if( _SR== "SR26A" ) {
    setSelLine("LL:=:hh|NB:>=:3|MT:<:120|MET:[]:50:200|NJ:>=:2|HT:[]:300:1600");
  }
  else if( _SR== "SR27A" ) {
    setSelLine("LL:=:hh|NB:>=:3|MT:<:120|MET:[]:200:500|NJ:>=:2|HT:<:300");
  }
  else if( _SR== "SR28A" ) {
    setSelLine("LL:=:hh|NB:>=:3|MT:<:120|MET:[]:200:500|NJ:>=:2|HT:[]:300:1600");
  }
  else if( _SR== "SR29A" ) {
    setSelLine("LL:=:hh|NB:>=:3|MT:>=:120|MET:[]:50:500|NJ:>=:2|HT:<:300");
  }
  else if( _SR== "SR30A" ) {
    setSelLine("LL:=:hh|NB:>=:3|MT:>=:120|MET:[]:50:500|NJ:>=:2|HT:[]:300:1600");
  }
  
  //inclusive H-MET ==========================================================
  else if( _SR== "SR31A" ) {
    setSelLine("LL:=:hh|MET:>=:500|NJ:>=:2|HT:>=:300");
  }

  //inclusive H-HT ==========================================================
  else if( _SR== "SR32A" ) {
    setSelLine("LL:=:hh|MET:[]:50:500|NJ:>=:2|HT:>=:1600");
  }

  
  //HL-regions =============================================================
  //0b-jet =================================================================
  else if( _SR== "SR1B" ) {
    setSelLine("LL:=:hl|NB:=:0|MT:<:120|MET:[]:50:200|NJ:[]:2:4|HT:<:300");
  }
  else if( _SR== "SR2B" ) {
    setSelLine("LL:=:hl|NB:=:0|MT:<:120|MET:[]:50:200|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR3B" ) {
    setSelLine("LL:=:hl|NB:=:0|MT:<:120|MET:[]:50:200|NJ:>=:5|HT:<:300");
    setSelLine("LL:=:hl|NB:=:0|MT:<:120|MET:[]:200:500|NJ:>=:2|HT:<:300");
  }
  else if( _SR== "SR4B" ) {
    setSelLine("LL:=:hl|NB:=:0|MT:<:120|MET:[]:50:200|NJ:>=:5|HT:[]:300:1600");
  }
  else if( _SR== "SR5B" ) {
    setSelLine("LL:=:hl|NB:=:0|MT:<:120|MET:[]:200:500|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR6B" ) {
    setSelLine("LL:=:hl|NB:=:0|MT:<:120|MET:[]:200:500|NJ:>=:5|HT:[]:300:1600");
  }
  
  //1b-jet =================================================================
 else if( _SR== "SR7B" ) {
    setSelLine("LL:=:hl|NB:=:1|MT:<:120|MET:[]:50:200|NJ:[]:2:4|HT:<:300");
  }
  else if( _SR== "SR8B" ) {
    setSelLine("LL:=:hl|NB:=:1|MT:<:120|MET:[]:50:200|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR9B" ) {
    setSelLine("LL:=:hl|NB:=:1|MT:<:120|MET:[]:50:200|NJ:>=:5|HT:<:300");
    setSelLine("LL:=:hl|NB:=:1|MT:<:120|MET:[]:200:500|NJ:>=:2|HT:<:300");
  }
  else if( _SR== "SR10B" ) {
    setSelLine("LL:=:hl|NB:=:1|MT:<:120|MET:[]:50:200|NJ:>=:5|HT:[]:300:1600");
  }
  else if( _SR== "SR11B" ) {
    setSelLine("LL:=:hl|NB:=:1|MT:<:120|MET:[]:200:500|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR12B" ) {
    setSelLine("LL:=:hl|NB:=:1|MT:<:120|MET:[]:200:500|NJ:>=:5|HT:[]:300:1600");
  }
  
  //2b-jet =================================================================
  else if( _SR== "SR13B" ) {
    setSelLine("LL:=:hl|NB:=:2|MT:<:120|MET:[]:50:200|NJ:[]:2:4|HT:<:300");
  }
  else if( _SR== "SR14B" ) {
    setSelLine("LL:=:hl|NB:=:2|MT:<:120|MET:[]:50:200|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR15B" ) {
    setSelLine("LL:=:hl|NB:=:2|MT:<:120|MET:[]:50:200|NJ:>=:5|HT:<:300");
    setSelLine("LL:=:hl|NB:=:2|MT:<:120|MET:[]:200:500|NJ:>=:2|HT:<:300");
  }
  else if( _SR== "SR16B" ) {
    setSelLine("LL:=:hl|NB:=:2|MT:<:120|MET:[]:50:200|NJ:>=:5|HT:[]:300:1600");
  }
  else if( _SR== "SR17B" ) {
    setSelLine("LL:=:hl|NB:=:2|MT:<:120|MET:[]:200:500|NJ:[]:2:4|HT:[]:300:1600");
  }
  else if( _SR== "SR18B" ) {
    setSelLine("LL:=:hl|NB:=:2|MT:<:120|MET:[]:200:500|NJ:>=:5|HT:[]:300:1600");
  }

  //3+b-jet =================================================================
  else if( _SR== "SR19B" ) {
    setSelLine("LL:=:hl|NB:>=:3|MT:<:120|MET:[]:50:200|NJ:>=:2|HT:<:300");
  }
  else if( _SR== "SR20B" ) {
    setSelLine("LL:=:hl|NB:>=:3|MT:<:120|MET:[]:50:200|NJ:>=:2|HT:[]:300:1600");
  }
  else if( _SR== "SR21B" ) {
    setSelLine("LL:=:hl|NB:>=:3|MT:<:120|MET:[]:200:500|NJ:>=:2|HT:<:300");
  }
  else if( _SR== "SR22B" ) {
    setSelLine("LL:=:hl|NB:>=:3|MT:<:120|MET:[]:200:500|NJ:>=:2|HT:[]:300:1600");
  }

  //inclusive H-MT =============================================================
  else if( _SR== "SR23B" ) {
    setSelLine("LL:=:hl|MT:>=:120|MET:[]:50:500|NJ:>=:2|HT:<:300");
  }
  else if( _SR== "SR24B" ) {
    setSelLine("LL:=:hl|MT:>=:120|MET:[]:50:500|NJ:>=:2|HT:[]:300:1600");
  }
  
  //inclusive H-MET ==========================================================
  else if( _SR== "SR25B" ) {
    setSelLine("LL:=:hl|MET:>=:500|NJ:>=:2|HT:>=:300");
  }
  
  //inclusive H-HT ==========================================================
  else if( _SR== "SR26B" ) {
    setSelLine("LL:=:hl|MET:[]:50:500|NJ:>=:2|HT:>=:1600");
  }


  //LL-regions =============================================================
  else if( _SR== "SR1C" ) {
    setSelLine("LL:=:ll|NB:=:0|MT:<:120|MET:[]:50:200|NJ:>=:2|HT:>:300");
  }
  else if( _SR== "SR2C" ) {
    setSelLine("LL:=:ll|NB:=:0|MT:<:120|MET:>:200|NJ:>=:2|HT:>:300");
  }
  else if( _SR== "SR3C" ) {
    setSelLine("LL:=:ll|NB:=:1|MT:<:120|MET:[]:50:200|NJ:>=:2|HT:>:300");
  }
  else if( _SR== "SR4C" ) {
    setSelLine("LL:=:ll|NB:=:1|MT:<:120|MET:>:200|NJ:>=:2|HT:>:300");
  }
  else if( _SR== "SR5C" ) {
    setSelLine("LL:=:ll|NB:=:2|MT:<:120|MET:[]:50:200|NJ:>=:2|HT:>:300");
  }
  else if( _SR== "SR6C" ) {
    setSelLine("LL:=:ll|NB:=:2|MT:<:120|MET:>:200|NJ:>=:2|HT:>:300");
  }
 else if( _SR== "SR7C" ) {
    setSelLine("LL:=:ll|NB:>=:3|MT:<:120|MET:>=:50|NJ:>=:2|HT:>:300");
  }
  else if( _SR== "SR8C" ) {
    setSelLine("LL:=:ll|MT:>=:120|MET:>=:50|NJ:>=:2|HT:>:300");
  }



  //baselines =============================================================
  //so stupid.....
  else if( _SR== "BR0H" ) {
    setSelLine("LL:=:hh|NB:>=:0|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:hh|NB:>=:0|NJ:>=:2|HT:>=:500");
  }
  else if( _SR== "BR0M" ) {
    setSelLine("LL:=:hl|NB:>=:0|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:hl|NB:>=:0|NJ:>=:2|HT:>=:500");
  }
  else if( _SR== "BR0L" ) {
    setSelLine("LL:=:ll|NB:>=:0|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:ll|NB:>=:0|NJ:>=:2|HT:>=:500");
  }

 else if( _SR== "BR10H" ) {
    setSelLine("LL:=:hh|NB:=:1|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:hh|NB:=:1|NJ:>=:2|HT:>=:500");
  }
  else if( _SR== "BR10M" ) {
    setSelLine("LL:=:hl|NB:=:1|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:hl|NB:=:1|NJ:>=:2|HT:>=:500");
  }
  else if( _SR== "BR10L" ) {
    setSelLine("LL:=:ll|NB:=:1|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:ll|NB:=:1|NJ:>=:2|HT:>=:500");
  }

  else if( _SR== "BR20H" ) {
    setSelLine("LL:=:ll|NB:=:2|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:hh|NB:=:2|NJ:>=:2|HT:>=:500");
  }
  else if( _SR== "BR20M" ) {
    setSelLine("LL:=:hl|NB:=:2|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:hl|NB:=:2|NJ:>=:2|HT:>=:500");
  }
  else if( _SR== "BR20L" ) {
    setSelLine("LL:=:ll|NB:=:2|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:ll|NB:=:2|NJ:>=:2|HT:>=:500");
  }

 else if( _SR== "BR30H" ) {
    setSelLine("LL:=:hh|NB:>=:3|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:hh|NB:>=:3|NJ:>=:2|HT:>=:500");
  }
  else if( _SR== "BR30M" ) {
    setSelLine("LL:=:hl|NB:>=:3|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:hl|NB:>=:3|NJ:>=:2|HT:>=:500");
  }
  else if( _SR== "BR30L" ) {
    setSelLine("LL:=:ll|NB:>=:3|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:ll|NB:>=:3|NJ:>=:2|HT:>=:500");
  }




}

void
FakeEstim::setSelLine(string str) {

  //MM could be done in a better way

  //parsing full selection into variable selections
  stringstream ss(str);
  string item;
  string tk;
  vector<string> vars(4,"");
  vector<vector<string> > sel;
  while (std::getline(ss, item, '|')) {
    
    vars[3]="";
    stringstream sssel( item );
   
    while (std::getline(sssel, tk, ':')) {
      vars.push_back(tk);
    }
    sel.push_back(vars);
  }

  _sels[_SR].push_back( sel );
}


//================================================
bool 
FakeEstim::genMatchedToFake(int idx) {

  int id1  = _vc->get("LepGood_mcMatchId" ,idx); 
  if(id1==0) return true;

  return false;
}

bool
FakeEstim::genMatchedMisCharge() {
    
  int nGenL=_vc->get("nGenPart");
  int pdgId1=0;
  int pdgId2=0;
  
  for(int ig=0;ig<nGenL;++ig) {
    
    if(pdgId1==0 && KineUtils::dR(_vc->get("LepGood_eta",_lep_idx1), _vc->get("GenPart_eta", ig),
		     _vc->get("LepGood_phi",_lep_idx1), _vc->get("GenPart_phi", ig))<0.05) { 
      pdgId1 = _vc->get("GenPart_pdgId",ig);
    }
    if(pdgId2==0 &&KineUtils::dR(_vc->get("LepGood_eta",_lep_idx2), _vc->get("GenPart_eta", ig),
		     _vc->get("LepGood_phi",_lep_idx2), _vc->get("GenPart_phi", ig))<0.05) { 
      pdgId2 = _vc->get("GenPart_pdgId",ig);
    }
    
    if(pdgId1!=0 && pdgId2!=0)
      break;
  }
  if(pdgId1*pdgId2<0) return true;
  
  return false;
}

float 
FakeEstim::getFR(Candidate* cand, int idx) {
  string db;
  int wp=SusyModule::kTight;
  
  if( std::abs(cand->pdgId())==13) db="AllMu";
  else  db="AllEl";

  //  if(_lepiso == "multiIso") db+="T";


  float ptVal=cand->pt();
  // if(_extScheme=="mIsoCor") {
  //   ptVal = cand->pt()*(1+ max((float)0.,_vc->get("LepGood_miniRelIso",idx )-_multiIsoWP[kMiniIso][wp]));
  // }
  // if(_extScheme=="mIsoAlCor") {
  //   ptVal = (_vc->get("LepGood_pTrel",idx)>_multiIsoWP[kPtRel][wp])?(cand->pt()*(1+ max((float)0.,_vc->get("LepGood_miniRelIso",idx )-_multiIsoWP[kMiniIso][wp]))):
  //     (_vc->get("LepGood_jetPtRatio",idx)/cand->pt()*0.8 );
  // }
  // if(_extScheme=="mIsoPtJCor") {
  //   ptVal = (_vc->get("LepGood_pTrel",idx)>_multiIsoWP[kPtRel][wp])?(cand->pt()*(1+ max((float)0.,_vc->get("LepGood_miniRelIso",idx )-_multiIsoWP[kMiniIso][wp]))):
  //     ( max(cand->pt(), _vc->get("LepGood_jetPtRatio",idx)/cand->pt()*_multiIsoWP[kPtRatio][wp] ) );
  // }

  
  return _dbm->getDBValue(db, std::min( ptVal,(float)99.9),
			  std::min(std::abs(cand->eta()),(float)2.39) );
}




//===============================================================
//===============================================================
bool
FakeEstim::testRegion() {

  bool passSel=true;

  for(size_t is=0;is<_sels[_SR].size();is++) {
    
    passSel=true;
    for(size_t ii=0;ii<_sels[_SR][is].size();ii++) {

      if(_sels[_SR][is][ii][0]=="LL") { //lepton pt scheme, specific case 
	if(_sels[_SR][is][ii][2]=="hh" && 
	   (_l1Cand->pt()<25 || _l2Cand->pt()<25) ) {passSel=false;break;}
	if(_sels[_SR][is][ii][2]=="hl" &&
	   ( (_l1Cand->pt()<25 && _l2Cand->pt()<25) |
	     (_l1Cand->pt()>=25 && _l2Cand->pt()>=25) ) ) {passSel=false;break;}
	if(_sels[_SR][is][ii][2]=="ll" && 
	   (_l1Cand->pt()>=25 || _l2Cand->pt()>=25) ) {passSel=false;break;}
      
      }
      else { //all other selections
	if(!_au->simpleCut<float>( (*(_val[_sels[_SR][is][ii][0] ])) , atof(_sels[_SR][is][ii][1].c_str() ),
			    _sels[_SR][is][ii][2], atof(_sels[_SR][is][ii][3].c_str()) ) ) 
	  {passSel=false;break;}
      }
      
    }
    if(passSel) return true;
    else return false;
  }

  return true;
}


void
FakeEstim::categorize() {
  
  int offSet=1;
  string categ="";
  for(size_t ic=0;ic< _categs.size();ic++) {
    _SR = _categs[ic];
    //setSignalRegion();
    if(testRegion() ) {setWorkflow(ic+offSet); return;}
  }
  setWorkflow(kGlobal);
}
