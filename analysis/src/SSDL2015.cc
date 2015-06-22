#include "analysis/src/SSDL2015.hh"

#include <algorithm>
#include <sstream>
#include <iostream>

SSDL2015::SSDL2015(std::string cfg){
  
  startExecution(cfg);
  initialize();

}

SSDL2015::~SSDL2015(){
 
}

void
SSDL2015::initialize(){


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

  _vc->registerVar("nJetFwd"                      );
  _vc->registerVar("JetFwd_pt"                    );
  _vc->registerVar("JetFwd_phi"                   );
  

  _vc->registerVar("htJet40j"                     );
  _vc->registerVar("minMllAFAS"                   );
  _vc->registerVar("minMllAFOS"                   );
  _vc->registerVar("mZ1"                          );
  _vc->registerVar("nLepGood10"                   );

 
  //generator informations
  _vc->registerVar("nGenPart"                     );
  _vc->registerVar("GenPart_pt"                   );
  _vc->registerVar("GenPart_eta"                  );
  _vc->registerVar("GenPart_phi"                  );
  _vc->registerVar("GenPart_pdgId"                );
  _vc->registerVar("GenPart_motherId"             );

  //bjets
  _vc->registerVar("nBJetLoose25"                 );
  _vc->registerVar("nBJetMedium40"                );
  _vc->registerVar("nBJetMedium25"                );
  _vc->registerVar("nSoftBJetMedium25"            );

  //minitree variables
  _vc->registerVar("iL1TV_Mini" );
  _vc->registerVar("iL2TV_Mini" );
  _vc->registerVar("nLepGood10_Mini" );
  _vc->registerVar("nLepTightVeto10_Mini" );
  _vc->registerVar("nLepGood_Mini");
  _vc->registerVar("nLepTightVeto_Mini");
  _vc->registerVar("mZ1cut10TL_Mini" );
  _vc->registerVar("minMllAFOSTL_Mini" );
  _vc->registerVar("minMllAFASTL_Mini" );
  _vc->registerVar("nJet40_Mini" );
  _vc->registerVar("htJet40j_Mini" );
  // _vc->registerVar("_Mini" );
  _vc->registerVar("nBJetMedium25_Mini" );


  _susyMod = new SusyModule(_vc);
  
  int nCateg=156; //78 156
  _categs.resize(nCateg);
  string srs[156]={ 

    "SR1A", "SR2A", "SR3A", "SR4A", "SR5A", "SR6A", "SR7A", "SR8A",
    "SR9A", "SR10A", "SR11A", "SR12A", "SR13A", "SR14A", "SR15A", "SR16A",
    "SR17A", "SR18A", "SR19A", "SR20A", "SR21A", "SR22A", "SR23A", "SR24A",
    "SR25A", "SR26A", "SR27A", "SR28A", "SR29A", "SR30A", "SR31A", "SR32A",
    
    "SR1B", "SR2B", "SR3B", "SR4B", "SR5B", "SR6B", "SR7B", "SR8B",
    "SR9B", "SR10B", "SR11B", "SR12B", "SR13B", "SR14B", "SR15B", "SR16B",
    "SR17B", "SR18B", "SR19B", "SR20B", "SR21B", "SR22B", "SR23B", "SR24B",
    "SR25B","SR26B",
    
    "SR1C", "SR2C", "SR3C", "SR4C", "SR5C", "SR6C", "SR7C", "SR8C",

    //"BR0H", "BR0M", "BR0L",

    "BR00H", "BR00M", "BR00L",
    "BR10H", "BR10M", "BR10L",
    "BR20H", "BR20M", "BR20L",
    "BR30H", "BR30M", "BR30L",
    
    // //fake workflows ==================================================

    "SR1A_Fake", "SR2A_Fake", "SR3A_Fake", "SR4A_Fake", "SR5A_Fake", "SR6A_Fake", "SR7A_Fake", "SR8A_Fake",
    "SR9A_Fake", "SR10A_Fake", "SR11A_Fake", "SR12A_Fake", "SR13A_Fake", "SR14A_Fake", "SR15A_Fake", "SR16A_Fake",
    "SR17A_Fake", "SR18A_Fake", "SR19A_Fake", "SR20A_Fake", "SR21A_Fake", "SR22A_Fake", "SR23A_Fake", "SR24A_Fake",
    "SR25A_Fake", "SR26A_Fake", "SR27A_Fake", "SR28A_Fake", "SR29A_Fake", "SR30A_Fake", "SR31A_Fake", "SR32A_Fake",
    
    "SR1B_Fake", "SR2B_Fake", "SR3B_Fake", "SR4B_Fake", "SR5B_Fake", "SR6B_Fake", "SR7B_Fake", "SR8B_Fake",
    "SR9B_Fake", "SR10B_Fake", "SR11B_Fake", "SR12B_Fake", "SR13B_Fake", "SR14B_Fake", "SR15B_Fake", "SR16B_Fake",
    "SR17B_Fake", "SR18B_Fake", "SR19B_Fake", "SR20B_Fake", "SR21B_Fake", "SR22B_Fake", "SR23B_Fake", "SR24B_Fake",
    "SR25B_Fake","SR26B_Fake",
    
    "SR1C_Fake", "SR2C_Fake", "SR3C_Fake", "SR4C_Fake", "SR5C_Fake", "SR6C_Fake", "SR7C_Fake", "SR8C_Fake",



    "BR00H_Fake", "BR00M_Fake", "BR00L_Fake",
    "BR10H_Fake", "BR10M_Fake", "BR10L_Fake",
    "BR20H_Fake", "BR20M_Fake", "BR20L_Fake",
    "BR30H_Fake", "BR30M_Fake", "BR30L_Fake",

 
  };
  _categs.assign(srs, srs+nCateg);

  for(size_t ic=0;ic< _categs.size();ic++) {
    _SR = _categs[ic];
    setSignalRegions();
    addWorkflow( ic+1, _categs[ic] );
  }

  addWorkflow( kGlobalFake, "Fake" );

  //extra input variables
  _lepflav = getCfgVarS("LEPFLAV");
  _leppt   = getCfgVarS("LEPPT"  );
  _SR      = getCfgVarS("SR"     );
  _FR      = getCfgVarS("FR"     );
  _categorization = getCfgVarI("categorization");

  vector<string> jess;
  jess.push_back("Jet_pt");
  addSystSource("JES",SystUtils::kNone, "%", jess, "JES8TeV.db", "" );
  
  //FR databases
  if(_FR=="FO2C") {
    _dbm->loadDb("El","MaySync/CH_FRFile_090615.root","qcdel/nosel/FRisoidRElPtMIso2");
    _dbm->loadDb("Mu","MaySync/CH_FRFile_090615.root","qcdmu/nosel/FRisoRMuPtMIso2");
  }
  if(_FR=="FO4C") {
    _dbm->loadDb("El","MaySync/CH_FRFile_090615.root","tt/nosel/FRISisofo4RElPtMIso2");
    _dbm->loadDb("Mu","MaySync/CH_FRFile_090615.root","tt/nosel/FRISisofo4RMuPtMIso2");
  }

}

void
SSDL2015::modifyWeight() {

}

void
SSDL2015::modifySkimming() {

}


void
SSDL2015::defineOutput() {

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

  // _hm->addVariable("MET",500,0,1000,"#slash{E}_{T} [GeV]");
  // _hm->addVariable("MTmin",500,0,1000,"min(M_{T,1}, M_{T,2}) [GeV]");
  // _hm->addVariable("METVsMT",100,0,1000,100,0,1000,"#slash{E}_{T} [GeV]",
  // 		   "min(M_{T,1}, M_{T,2}) [GeV]");

  //lepton pT(30,0,150), HT(20,0,1000), MET(20,0,200), mTmin(20,0,200) 
  _hm->addVariable("l1Pt", 30, 0, 150,"p_{T}(l_{1}) [GeV]");
  _hm->addVariable("l2Pt", 30, 0, 150,"p_{T}(l_{2}) [GeV]");

  _hm->addVariable("HT", 20, 0, 1000,"H_{T} [GeV]");
  _hm->addVariable("MET", 20, 0, 200,"#slash{E}_{T} [GeV]");
  _hm->addVariable("MTmin", 20, 0, 200,"min(M_{T,1}, M_{T,2}) [GeV]");

  _hm->addVariable("NBJets", 4, 0, 4,"N_{b-jets} ");

}


void
SSDL2015::writeOutput() {
 
}


void
SSDL2015::run() {
  
  // if(_vc->get("evt")!=75368//  && _vc->get("evt")!=17245 && _vc->get("evt")!=82965 &&
  //    // _vc->get("evt")!=72951 && _vc->get("evt")!=142649
  //    ) return;
  //if(_vc->get("evt")!=62463 ) return;
  //if(_vc->get("evt")!=103973 )  return;
  //if(_vc->get("evt")!=142649 && _vc->get("evt")!=72951 ) return;
  //if(_vc->get("evt")!=72688 && _vc->get("evt")!=49205) return;
  //cout<<" =================================="<< _sampleName <<endl;
  counter("denominator");
  
  retrieveObjects();
  //cout<<" cui "<<endl;
  if(!ssLeptonSelection() ) return;
  //cout<<" coin "<<endl;
  fillSkimTree();
  //cout<<" pouet "<<endl;
  //===============================
  _mTmin=min( Candidate::create(_l1Cand, _met)->mass(),
	      Candidate::create(_l2Cand, _met)->mass() );
  //===============================

  if(!passGenSelection() ) return;
  counter("genselection");


  //MC check for FR --> one fake only
  if(!_isFake) {
    setWorkflow(kGlobal); //MANDATORY (otherwise double counting in other categories)
    
    // if(!genMatchedToFake(_idxL1) && genMatchedToFake(_idxL2) ) {
    //   _idxFake=_idxL2;
    // }
    // else if(genMatchedToFake(_idxL1) && !genMatchedToFake(_idxL2)) {
    //   _idxFake=_idxL1;
    // }
    // else return; //no only one fake
  }
  else setWorkflow(kGlobalFake);
  
  
  counter("lepton baseline");

  //default cuts for baseline
  if(_HT<80) return;
  if( (_HT<500 && _metPt < 30) ) return;
  if(_nJets<2) return;

  counter("std baseline");
  
  if (_leppt=="hh" && _l1Cand->pt()<25.) return;
  if (_leppt=="hh" && _l2Cand->pt()<25.) return;
  if (_leppt=="hl" && (_l1Cand->pt()<25. && _l2Cand->pt()<25.) ) return;
  if (_leppt=="hl" && (_l1Cand->pt()>25. && _l2Cand->pt()>25.) ) return;
  if (_leppt=="ll" && _l1Cand->pt()>25.) return;
  if (_leppt=="ll" && _l2Cand->pt()>25.) return;
  
  if(!_isFake) {
    if(_lepflav=="mm" && std::abs(_vc->get("LepGood_pdgId", _idxFake))!=13) return;
    if(_lepflav=="ee" && std::abs(_vc->get("LepGood_pdgId", _idxFake))!=11) return;
  }
  else {
    //setWorkflow(kGlobalFake);
    _idxFake=_idxL2;
    if(_lepflav=="mm" && std::abs(_l2Cand->pdgId())!=13 ) return;
    if(_lepflav=="ee" && std::abs(_l2Cand->pdgId())!=11 ) return;
  }
  
  counter("ptflav");
 
  if(_isFake) {
    //return;
    oneIsoSel();
  }
  
  counter("weigthing");
  

  fill("l1Pt", (_idxFake==_idxL2)?(_l1Cand->pt()):_l2Cand->pt(), _weight );
  fill("l2Pt", (_idxFake==_idxL2)?(_l2Cand->pt()):_l1Cand->pt(), _weight );
  fill("HT", _HT, _weight);
  fill("MET", _met->pt(), _weight);
  fill("MTmin", _mTmin, _weight);
  fill("NBJets", _nBJets, _weight);

  if(_categorization) {
    categorize();
    
    if(_isFake) {
      setWorkflow(getCurrentWorkflow()+kBR30L); //MM to be done in a better way
    }

    counter("region splitting");
  }
  else {
    if(!testRegion() ) return;
  }

  counter("selected");

  
  if( getCurrentWorkflow()==0) return; //getCurrentWorkflow()==100 ||

  //cout<<_vc->get("evt")<<"   "<<_jets[0]->pt()<<endl;

  // int run=_vc->get("run");
  // int lumi=_vc->get("lumi");
  // int event=_vc->get("evt");
  // int nLep = _looseLepsVeto.size();//_vc->get("nLepGood_Mini");
  // // cout<<" <====> "<<_vc->get("nLepGood_Mini")<<"  "<<_looseLepsVeto.size()<<"  "<<_looseLeps.size()<<endl;
  // int id1 = _l1Cand->pdgId();
  // double pt1 = _l1Cand->pt();
  // int id2 = _l2Cand->pdgId();
  // double pt2 = _l2Cand->pt();
  // int njet = _nJets;
  // int nbjet = _nBJets;
  // double met = _met->pt();
  // double HT = _HT;
  // int sr = ((getCurrentWorkflow()<kBR00H)?(getCurrentWorkflow()):(0));
  
  // cout << Form("%1d %9d %12d\t%2d\t%+2d %5.1f\t%+2d %5.1f\t%d\t%2d\t%5.1f\t%6.1f\t%2d",
  // 	       run, lumi, event, nLep,
  // 	       id1, pt1, id2, pt2,
  // 	       njet, nbjet, met, HT,
  // 	       sr ) << endl;

}

bool
SSDL2015::noIsoSel() {
  
  if( _fakableLepsVeto10.size()!=0 ) return false;
  
  float p=getProbAtLeastNIso( _fakableLepsVeto10 ,_fakableLepsVeto10Idx , 2);
  _weight *= p/(1-p);
  return true;
}

bool
SSDL2015::oneIsoSel() {
  
  if( _fakableLepsVeto10.size()!=1 ) return false;

  float p=getProbAtLeastNIso( _fakableLepsVeto10 ,_fakableLepsVeto10Idx , 1);
  _weight *= p/(1-p);

  return true;
}

float
SSDL2015::getProbAtLeastNIso(CandList fObjs, vector<unsigned int> fObjIdx,
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
SSDL2015::retrieveObjects(){
  
  _jets.clear();
  

  _l1Cand=nullptr;
  _l2Cand=nullptr;
  
  _looseLeps.clear();
  _looseLepsIdx.clear();

  _looseLeps10.clear();
  _looseLeps10Idx.clear();

  _looseLepsVeto.clear();
  _looseLepsVetoIdx.clear();

  _looseLepsVeto10.clear();
  _looseLepsVeto10Idx.clear();

  _fakableLeps10.clear();
  _fakableLeps10Idx.clear();

  _fakableLepsVeto10.clear();
  _fakableLepsVeto10Idx.clear();
  
  _tightLeps10.clear();
  _tightLeps10Idx.clear();

  _tightLepsVeto10.clear();
  _tightLepsVeto10Idx.clear();

  selectLeptons();

  
  _susyMod->cleanJets( &_looseLeps10, _jets, _jetsIdx, _bJets, _bJetsIdx);
  _nJets=_jets.size();
  _nBJets=_bJets.size();
  _HT=_susyMod->HT( &(_jets) );

  // _nJets  = _vc->get("nJet40_Mini");//_jets.size();
  // _nBJets = _vc->get("nBJetMedium25_Mini");
  // _HT  = _vc->get("htJet40j_Mini");//_susyMod->HT( &(_jets) );

  if(false) {
    TVector2 met = varyMET();
    _met = Candidate::create( met.Mod(), met.Phi() );
  }
  else
    _met = Candidate::create( _vc->get("met_pt"), _vc->get("met_phi") );

  _metPt = _met->pt();

  //MET and MT
 

}

//=========================================================

bool
SSDL2015::ssLeptonSelection() {
  _isFake=false;
 
  // 2Tight ===============================
  
  if(_tightLepsVeto10.size()>=2) { //main
    _isFake=false;

    CandList lepPair=_susyMod->bestSSPair( (&_tightLepsVeto10), true, false, 10, _idxL1, _idxL2);
    if(lepPair.size()<2) return false;
    _l1Cand = lepPair[0];
    _l2Cand = lepPair[1];

    if(_l1Cand==nullptr || _l2Cand==nullptr) return false; //case with less than two leptons or no valid pair

    _idxL1 = _tightLepsVeto10Idx[_idxL1];
    _idxL2 = _tightLepsVeto10Idx[_idxL2];
    
    if(!makeCut( _l1Cand->charge()*_l2Cand->charge()>0, "same sign" ) ) return false;
    if(!makeCut(_susyMod->passMllSingleVeto(_l1Cand, _l2Cand, 0, 8, false), "mll veto") ) return false;

    return true;
  } //MM: validated 2T selection -> sync with CB
   
  // 1Tight 1Loose ====================================
  if(_tightLepsVeto10.size()==1 && _fakableLepsVeto10.size()>=1) { // && _fakableLepsVeto10.size()>=1
    _isFake=true;

    CandList lepPair=_susyMod->bestSSPair(_tightLepsVeto10[0], (&_fakableLepsVeto10), true, false, 10, _idxL1, _idxL2);
    
    if(lepPair.size()<2) return false;
    //cout<<lepPair.size()<<endl;
    _l1Cand = lepPair[0];
    _l2Cand = lepPair[1];

    if(_l1Cand==nullptr || _l2Cand==nullptr) return false; //case with less than two leptons or no valid pair
    _idxL1 = _tightLepsVeto10Idx[0];
    _idxL2 = _fakableLepsVeto10Idx[_idxL2];
    
    // _l1Cand = _tightLepsVeto10[0];
    // _l2Cand = _fakableLepsVeto10[0];
      
    // if(_l1Cand==nullptr || _l2Cand==nullptr) return false;
      
    if(!genMatchedToFake(_idxL1) && !genMatchedToFake(_idxL2) ) return false;
    //if(genMatchedToFake(_idxL1) && genMatchedToFake(_idxL2) ) return false;

    if(!makeCut( _l1Cand->charge()*_l2Cand->charge()>0, "same sign" ) ) return false;
    if(!makeCut(_susyMod->passMllSingleVeto(_l1Cand, _l2Cand, 0, 8, false), "mll veto") ) return false;
     
    return true;
  }

  // 0Tight any loose ====================================
  if(_tightLepsVeto10.size()==0 && _fakableLepsVeto10.size()>=2) {

    
    CandList lepPair=_susyMod->bestSSPair( (&_fakableLepsVeto10), true, false, 10, _idxL1, _idxL2);
    if(lepPair.size()<2) return false;
    //cout<<lepPair.size()<<endl;
    _l1Cand = lepPair[0];
    _l2Cand = lepPair[1];

    if(_l1Cand==nullptr || _l2Cand==nullptr) return false; //case with less than two leptons or no valid pair

    _idxL1 = _tightLepsVeto10Idx[_idxL1];
    _idxL2 = _tightLepsVeto10Idx[_idxL2];
    
    if(!genMatchedToFake(_idxL1) || !genMatchedToFake(_idxL2) ) return false;

    if(!makeCut( _l1Cand->charge()*_l2Cand->charge()>0, "same sign" ) ) return false;
    if(!makeCut(_susyMod->passMllSingleVeto(_l1Cand, _l2Cand, 0, 8, false), "mll veto") ) return false;

    return true;

  }


  // for(unsigned int il1 = 0; il1 < _tightLeps10Veto.size(); ++il1) {
  //   for(unsigned int il2 = 0; il2 < _looseLeps10Veto.size(); ++il2){
     
  //     if(

  //     //one of the two is a fake
  //     if(!genMatchedToFake(_looseLeps10VetoIdx[il2]) || !genMatchedToFake(_tightLeps10VetoIdx[il1]) ) continue;
      
  //     if(_tightLeps10Veto[il1]->charge()*_looseLeps10Veto[il2]->charge()<0) continue;
  //     if(

  //   }
  // }


  //   //only one fake for now MM: to be fixed
  //   if(!makeCut( _l1Cand!=nullptr, "leading lepton" ) ) return false;
  //   if(!makeCut(_fakableObjects.size()==1, "trailing lepton" ) ) return false;
  //   _l2Cand = _fakableObjects[0];
  //   _idxL2 = _fakableObjectsIdx[0];

  //   //check the vetos MM: to be fixed
  //   if(!_susyMod->mllLMGVeto(_l1Cand, _l2Cand) ) return false;
  //   if(!_susyMod->mllZVeto(_l1Cand, _l2Cand) ) return false;

  //   if(!makeCut( _l1Cand->charge()*_l2Cand->charge()>0, "same sign" ) ) return false;
    
  //   return true;
  // }
  

  return false;
}

// bool
// SSDL2015::alternateSSEventSelection(bool switchWF) {
  
//   // //MM: could be done in one round, but safer and easier to understand that way 

//   // // SF: CHOOSE a SS PAIR, maximizing the number of muons and then pT
//   // int charge = 0;
//   // int flavor = 0;
//   // int flavortmp = 0;
//   // bool isSS = false;
//   // _idxL1=-1;
//   // _idxL2=-1;

//   // _nIso=-1;
//   // int nOSPairs=0;
//   // _nSelPair2Iso=0;
//   // _nSelPair1Iso=0;
//   // _nSelPair0Iso=0;

//   // //========================================================================
//   // //first fully identified leptons => signal
//   // for(unsigned int il1 = 0; il1 < _fullIdLeps.size(); ++il1){
//   //   for(unsigned int il2 = il1+1; il2 < _fullIdLeps.size(); ++il2){

//   //     // if (_leppt=="hh" && _fullIdLeps[il1]->pt()<25.) continue;
//   //     // if (_leppt=="hh" && _fullIdLeps[il2]->pt()<25.) continue;
//   //     // if (_leppt=="hl" && (_leptons[il1]->pt()>25. && _leptons[il2]->pt()>25.) ) continue;
//   //     // if (_leppt=="hl" && (_leptons[il1]->pt()<25. && _leptons[il2]->pt()<25.) ) continue;
//   //     // if (_leppt=="ll" && _fullIdLeps[il1]->pt()>25.) continue;
//   //     // if (_leppt=="ll" && _fullIdLeps[il2]->pt()>25.) continue;

//   //     charge = _fullIdLeps[il1]->charge()*_fullIdLeps[il2]->charge();
//   //     if(charge<0) continue; // if the pair is OS skip
//   //     else nOSPairs++;

//   //     flavortmp = std::abs(_fullIdLeps[il1]->pdgId())+std::abs( _fullIdLeps[il2]->pdgId());
//   //     if(_lepflav=="mm" && flavortmp!=26 ) continue;
//   //     if(_lepflav=="ee" && flavortmp!=22 ) continue;
//   //     if( (_lepflav=="em" || _lepflav=="me") && flavortmp!=24 ) continue;
//   //     //if(flavor>flavortmp) continue; // if the new pair has less muons skip.

//   //     flavor = flavortmp;
      
//   //     _l1Cand = _fullIdLeps[il1];   _idxL1 = _tmpLepIdx[il1];
//   //     _l2Cand = _fullIdLeps[il2];   _idxL2 = _tmpLepIdx[il2]; 
//   //     isSS = true;
//   //     _nSelPair2Iso++;
//   //   }
//   // }
  
//   // if(isSS) {_nIso=2; 
//   //   if(switchWF) setWorkflow(kGlobal);
//   //   return true;
//   // }
  
//   // return false; //MM: non isolated sideband disabled for now, workflows have to be fully duplicated
//   // //if(nOSPairs!=0) return false;
//   // //=======================================================================
//   // //now, mixing events with isolated and non isolated leptons
//   // for(unsigned int il1 = 0; il1 < _fullIdLeps.size(); ++il1){
//   //   for(unsigned int il2 = 0; il2 < _nonFullIdLeps.size(); ++il2){
      
//   //     // if (_leppt=="hh" && _fullIdLeps[il1]->pt()<25.) continue;
//   //     // if (_leppt=="hh" && _nonFullIdLeps[il2]->pt()<25.) continue;
//   //     // if (_leppt=="hl" && !(_fullIdLeps[il1]->pt()<25. && _nonFullIdLeps[il2]->pt()>25.) ) continue;
//   //     // if (_leppt=="hl" && !(_fullIdLeps[il1]->pt()>25. && _nonFullIdLeps[il2]->pt()<25.) ) continue;
//   //     // if (_leppt=="ll" && _fullIdLeps[il1]->pt()>25.) continue;
//   //     // if (_leppt=="ll" && _nonFullIdLeps[il2]->pt()>25.) continue;

//   //     if(!genMatchedToFake(_nonFullIdLepsIdx[il2]) && !genMatchedToFake(_tmpLepIdx[il1]) ) continue;

//   //     charge = _fullIdLeps[il1]->charge()*_nonFullIdLeps[il2]->charge();
//   //     if(charge<0) continue; // if the pair is OS skip

//   //     flavortmp = std::abs(_fullIdLeps[il1]->pdgId())+std::abs( _nonFullIdLeps[il2]->pdgId());
//   //     if(_lepflav=="mm"  && flavortmp!=26 ) continue;
//   //     if(_lepflav=="ee"  && flavortmp!=22 ) continue;
//   //     if( (_lepflav=="em" || _lepflav=="me")  && flavortmp!=24 ) continue;
  
//   //     if( find(_lepsForFR.begin(),_lepsForFR.end(),_nonFullIdLeps[il2])==_lepsForFR.end()) {
//   // 	_lepsForFR.push_back(_nonFullIdLeps[il2]);
//   // 	_lepsForFRIdx.push_back(_nonFullIdLepsIdx[il2]);
//   //     }
      
//   //     flavor = flavortmp;
      
//   //     _l1Cand = _fullIdLeps[il1];   _idxL1 = _tmpLepIdx[il1];
//   //     _l2Cand = _nonFullIdLeps[il2];   _idxL2 = _nonFullIdLepsIdx[il2]; 
    
//   //     isSS = true;
//   //     _nSelPair1Iso++;

//   //   }
//   // }
 
//   // if(isSS) { _nIso=1; 
//   //   if(switchWF) setWorkflow(kOneIso); 
//   //   return true;
//   // }

//   // //=======================================================================
//   // //now, mixing events with isolated and non isolated leptons
//   // for(unsigned int il1 = 0; il1 < _nonFullIdLeps.size(); ++il1){
//   //   for(unsigned int il2 = il1+1; il2 < _nonFullIdLeps.size(); ++il2){

//   //     // if (_leppt=="hh" && _nonFullIdLeps[il1]->pt()<25.) continue;
//   //     // if (_leppt=="hh" && _nonFullIdLeps[il2]->pt()<25.) continue;
//   //     // if (_leppt=="hl" && !(_nonFullIdLeps[il1]->pt()<25 && _nonFullIdLeps[il2]->pt()>25) ) continue;
//   //     // if (_leppt=="hl" && !(_nonFullIdLeps[il1]->pt()>25 && _nonFullIdLeps[il2]->pt()<25) ) continue;
//   //     // if (_leppt=="ll" && _nonFullIdLeps[il1]->pt()>25.) continue;
//   //     // if (_leppt=="ll" && _nonFullIdLeps[il2]->pt()>25.) continue;

//   //     charge = _nonFullIdLeps[il1]->charge()*_nonFullIdLeps[il2]->charge();
//   //     if(charge<0) continue; // if the pair is OS skip

//   //     flavortmp = std::abs(_nonFullIdLeps[il1]->pdgId())+std::abs( _nonFullIdLeps[il2]->pdgId());
//   //     if(_lepflav=="mm"  && flavortmp!=26 ) continue;
//   //     if(_lepflav=="ee"  && flavortmp!=22 ) continue;
//   //     if( (_lepflav=="em" || _lepflav=="me")  && flavortmp!=24 ) continue;
 
//   //     if( find(_lepsForFR.begin(),_lepsForFR.end(),_nonFullIdLeps[il1])==_lepsForFR.end()) {
//   // 	_lepsForFR.push_back(_nonFullIdLeps[il1]);
//   //     _lepsForFRIdx.push_back(_nonFullIdLepsIdx[il1]);
//   //   }

//   //     if( find(_lepsForFR.begin(),_lepsForFR.end(),_nonFullIdLeps[il2])==_lepsForFR.end()) {
//   // 	_lepsForFR.push_back(_nonFullIdLeps[il2]);
//   // 	_lepsForFRIdx.push_back(_nonFullIdLepsIdx[il2]);
//   //     }

//   //     flavor = flavortmp;
      
//   //     _l1Cand = _nonFullIdLeps[il1];   _idxL1 = _nonFullIdLepsIdx[il1];
//   //     _l2Cand = _nonFullIdLeps[il2];   _idxL2 = _nonFullIdLepsIdx[il2]; 
 
//   //     isSS = true;
//   //     _nSelPair0Iso++;
//   //   }
//   // }

//   // if(isSS) {_nIso=0;
//   //   if(switchWF) setWorkflow(kNoIso);
//   //   return true;
//   // }

//   return false;
// }



//=====================================================================
// signal region selection

void 
SSDL2015::setSignalRegions() {
    
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
    setSelLine("LL:=:hh|NB:=:0|MT:>=:120|MET:[]:50:500|NJ:>=:2|HT:<:300");
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
    setSelLine("LL:=:hh|NB:=:1|MT:>=:120|MET:[]:50:500|NJ:>=:2|HT:<:300");
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
    setSelLine("LL:=:hh|NB:=:2|MT:>=:120|MET:[]:50:500|NJ:>=:2|HT:<:300");
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

 else if( _SR== "BR00H" ) {
    setSelLine("LL:=:hh|NB:=:0|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:hh|NB:=:0|NJ:>=:2|HT:>=:500");
  }
  else if( _SR== "BR00M" ) {
    setSelLine("LL:=:hl|NB:=:0|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:hl|NB:=:0|NJ:>=:2|HT:>=:500");
  }
  else if( _SR== "BR00L" ) {
    setSelLine("LL:=:ll|NB:=:0|MET:>:30|NJ:>=:2|HT:<:500");
    setSelLine("LL:=:ll|NB:=:0|NJ:>=:2|HT:>=:500");
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
    setSelLine("LL:=:hh|NB:=:2|MET:>:30|NJ:>=:2|HT:<:500");
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
SSDL2015::setSelLine(string str) {

  //MM could be done in a better way

  //parsing full selection into variable selections
  stringstream ss(str);
  string item;
  string tk;
  
  vector<vector<string> > sel;
  while (std::getline(ss, item, '|')) {
    
    vector<string> vars(4,"");
    stringstream sssel( item );
  
    int n=0;
    while (std::getline(sssel, tk, ':')) {
      vars[n]= (tk);
      n++;
    }

    if(vars.size()==3)
      vars.push_back("");

    sel.push_back(vars);
  }

  _sels[_SR].push_back( sel );
}


//================================================
bool 
SSDL2015::genMatchedToFake(int idx) {

  int id1  = _vc->get("LepGood_mcMatchId" ,idx); 
  if(id1==0) return true;

  return false;
}

bool
SSDL2015::genMatchedMisCharge() {
    
  int nGenL=_vc->get("nGenPart");
  int pdgId1=0;
  int pdgId2=0;
  
  for(int ig=0;ig<nGenL;++ig) {
    
    if(pdgId1==0 && KineUtils::dR(_vc->get("LepGood_eta",_idxL1), _vc->get("GenPart_eta", ig),
		     _vc->get("LepGood_phi",_idxL1), _vc->get("GenPart_phi", ig))<0.05) { 
      pdgId1 = _vc->get("GenPart_pdgId",ig);
    }
    if(pdgId2==0 &&KineUtils::dR(_vc->get("LepGood_eta",_idxL2), _vc->get("GenPart_eta", ig),
		     _vc->get("LepGood_phi",_idxL2), _vc->get("GenPart_phi", ig))<0.05) { 
      pdgId2 = _vc->get("GenPart_pdgId",ig);
    }
    
    if(pdgId1!=0 && pdgId2!=0)
      break;
  }
  if(pdgId1*pdgId2<0) return true;
  
  return false;
}

int 
SSDL2015::genMatchCateg(const Candidate* cand) {

  // //loop over the number of generated leptons
  // int nGenL = _vc->get("nGenPart");

  // for(int ig = 0; ig < nGenL; ++ig) {
	
  //   if(Tools::dR(cand->eta(), _vc->get("GenPart_eta", ig),
  // 		 cand->phi(), _vc->get("GenPart_phi", ig) ) < 0.05 ) { //to be tuned
		  		
  //     //if( genMatchedToFake(idx) ) return kMisMatchPdgId; //taus are exception to the rule
  //     else if(cand->pdgId()*_vc->get("GenPart_pdgId",ig) < 0 ) return kMisChargePdgId; //+*- = -...
  //     else return kGenMatched;
			
  //     break;
  //   } //dr matching
  // } //gen loop
	
  return kNoGenMatch;
}

bool
SSDL2015::passGenSelection() {

  if( _sampleName.find("DYJets")!=(size_t)-1 || _sampleName.find("TTJets")!=(size_t)-1 ) {
    //ugly
    int lep1Id=genMatchCateg(_l1Cand);
    int lep2Id=genMatchCateg(_l1Cand);
    
    if(_sampleName.find("charge")!=(size_t)-1) {
      if( !genMatchedMisCharge() )
	  //( (lep1Id == kMisChargePdgId && lep2Id >= kMisChargePdgId) || 
	  //  (lep2Id == kMisChargePdgId && lep1Id >= kMisChargePdgId) ) ) 
	return false;
    }
    else if(_sampleName.find("fake")!=(size_t)-1) {
      // if( lep1Id > kMisMatchPdgId &&
      // 	  lep2Id > kMisMatchPdgId ) return false;
      if(!genMatchedToFake(_idxL1) && !genMatchedToFake(_idxL2) ) return false;

    }
    else {
      if( (!genMatchedToFake(_idxL1) && !genMatchedToFake(_idxL2) ) && !genMatchedMisCharge() ) {
	// // cout<<" coincoin "<<"  "<<genMatchedMisCharge()<<endl;
	// // cout<<_vc->get("LepGood_mcMatchId" ,_idxL1)<<"  "<<_vc->get("LepGood_mcMatchId" ,_idxL2)<<" --> "<<endl;
	// int nGenL=_vc->get("nGenPart");
	// for(int ig=0;ig<nGenL;++ig) {
	//   cout<<_vc->get("GenPart_pdgId",ig)<<"   "<<_vc->get("LepGood_eta",_idxL1)<<" <> "<<_vc->get("LepGood_eta",_idxL2)<<" <> "<<_vc->get("GenPart_eta", ig)<<" : "<<_vc->get("GenPart_pt", ig)<<" -> "<<_vc->get("GenPart_motherId", ig)<<endl;
	// }
      }
    }
    // if(_sampleName.find("prompt")!=(size_t)-1) {
    //   if( lep1Id != kGenMatched ||
    // 	  lep2Id != kGenMatched ) return false;
    // }
  }

  return true;
}


float 
SSDL2015::getFR(Candidate* cand, int idx) {
  string db;
  //int wp=SusyModule::kTight;
  
  if( std::abs(cand->pdgId())==13) db="Mu";
  else  db="El";
  

  float ptVal=cand->pt();
  float etaVal=std::abs(cand->eta());
  
  if(_FR.find("C")!=string::npos) ptVal=std::max(_susyMod->conePt(idx), (float)10.);
  if(_FR.find("J")!=string::npos) ptVal/=_vc->get("LepGood_jetPtRatio", idx);

  ptVal=std::max(ptVal, (float)10.);
  // cout<<" ====> "<<cand->pt()<<" / "<<cand->eta()<<" => "<<_dbm->getDBValue(db, std::min( ptVal,(float)69.9),
  // 									    std::min(std::abs(cand->eta()),(float)2.49) )<<endl;

  // if(_dbm->getDBValue(db, std::min( ptVal,(float)69.9),
  // 		      std::min(std::abs(cand->eta()),(float)2.49) ) == 1)
  //   cout<<cand->pt()<<"  "<<ptVal<<"   "<<_dbm->getDBValue(db, std::min( ptVal,(float)69.9),
  // 							   std::min(std::abs(cand->eta()),(float)2.49) )<<endl;

  return _dbm->getDBValue(db, std::min( ptVal,(float)69.9),
			  std::min(etaVal,(float)2.49) );
}




//===============================================================
//===============================================================
bool
SSDL2015::testRegion() {

  bool passSel=true;

  for(size_t is=0;is<_sels[_SR].size();is++) {
    passSel=true;
    //cout<<"new region ================== "<<_SR<<endl;
    for(size_t ii=0;ii<_sels[_SR][is].size();ii++) {
    
      //cout<<_sels[_SR][is][ii][0]<<" >> "<<endl;

      if(_sels[_SR][is][ii][0]=="LL") { //lepton pt scheme, specific case 
	if(_sels[_SR][is][ii][2]=="hh" && 
	   (_l1Cand->pt()<25 || _l2Cand->pt()<25) ) {passSel=false;break;}
	if(_sels[_SR][is][ii][2]=="hl" &&
	   ( (_l1Cand->pt()<25 && _l2Cand->pt()<25) ||
	     (_l1Cand->pt()>=25 && _l2Cand->pt()>=25) ) ) {passSel=false;break;}
	if(_sels[_SR][is][ii][2]=="ll" && 
	   (_l1Cand->pt()>=25 || _l2Cand->pt()>=25) ) {passSel=false;break;}
      
      }
      else { //all other selections

	//cout<<":: "<<_sels[_SR][is][ii][0]<<" :: "<<(*(_val[_sels[_SR][is][ii][0] ]))<<" "<<_sels[_SR][is][ii][1]<<" "<<atof(_sels[_SR][is][ii][2].c_str() )<<"  "<<atof(_sels[_SR][is][ii][3].c_str())<<endl;

	// bool dec=(_au->simpleCut<float>( (*(_val[_sels[_SR][is][ii][0] ])) , atof(_sels[_SR][is][ii][2].c_str() ), _sels[_SR][is][ii][1], atof(_sels[_SR][is][ii][3].c_str()) ));
	if(!_au->simpleCut<float>( (*(_val[_sels[_SR][is][ii][0] ])) , atof(_sels[_SR][is][ii][2].c_str() ),
				   _sels[_SR][is][ii][1], atof(_sels[_SR][is][ii][3].c_str()) ) ) 
	  {passSel=false;break;}
      }
      
    }
    //cout<<" --> passing selection? "<<passSel<<endl;
    if(passSel) return true;
  }

  return false;
}


void
SSDL2015::categorize() {
  
  int offset=1;
  string categ="";
  for(size_t ic=0;ic< _categs.size();ic++) {
    _SR = _categs[ic];
    if(testRegion() ) {setWorkflow(ic+offset); return;}
  }
  setWorkflow(kGlobal);
}


bool 
SSDL2015::looseLepton(int idx, int pdgId) {

  if(abs(pdgId)==13) {//mu case
    if(!_susyMod->muIdSel(idx, SusyModule::kLoose) ) return false;
    if(!_susyMod->multiIsoSel(idx, SusyModule::kLoose) ) return false;
  }
  else {
    if(!_susyMod->elIdSel(idx, SusyModule::kLoose, SusyModule::kLoose) ) return false;
    if(!_susyMod->multiIsoSel(idx, SusyModule::kLoose) ) return false;
  }

  return true;
}

bool 
SSDL2015::tightLepton(int idx, int pdgId) {

  if(abs(pdgId)==13) {//mu case
    if(!_susyMod->muIdSel(idx, SusyModule::kTight) ) return false;
    if(!_susyMod->multiIsoSel(idx, SusyModule::kMedium) ) return false;
  }
  else {
    if(!_susyMod->elIdSel(idx, SusyModule::kTight, SusyModule::kTight) ) return false;
    if(!_susyMod->multiIsoSel(idx, SusyModule::kTight) ) return false;
  }

  return true;
}


bool 
SSDL2015::fakableLepton(int idx, int pdgId) {

  if(abs(pdgId)==13) {//mu case
    if(!_susyMod->muIdSel(idx, SusyModule::kTight) ) return false;
    if(!_susyMod->multiIsoSel(idx, SusyModule::kLoose) ) return false;
    
    if(_FR.find("FO4")!=string::npos && !_susyMod->invMultiIsoSel(idx, SusyModule::kSpecFakeMu) ) return false;
  }
  else {
    if(_FR.find("FO2")==string::npos && !_susyMod->elIdSel(idx, SusyModule::kTight, SusyModule::kTight )) return false;
    if(_FR.find("FO2")!=string::npos && !_susyMod->elIdSel(idx, SusyModule::kTight, SusyModule::kLoose )) return false;
    if(!_susyMod->multiIsoSel(idx, SusyModule::kLoose) ) return false;
    
    if(_FR.find("FO4")!=string::npos && !_susyMod->invMultiIsoSel(idx, SusyModule::kSpecFakeEl) ) return false;
  }

  return true;
}




bool
SSDL2015::passCERNSelection() {

  if(!makeCut( _vc->get("nLepGood10_Mini"), "CERN lepmult" ) ) return false;
  if(!makeCut<float>( _vc->get("mZ1cut10TL_Mini"), 76 , "]![", "CERN Z veto",  106 ) ) return false;
  if(!makeCut<float>( _vc->get("minMllAFOSTL_Mini"), 0 , "]!]", "CERN g veto OS", 12) ) return false;
  if(!makeCut<float>( _vc->get("minMllAFASTL_Mini"), 8 , ">", "CERN g veto AS" ) ) return false;
     
// > = 2 good leptons: nLepGood10 >= 2
// minMllAS8:  minMllAFAS > 8
// minMllOS12: minMllAFOS <= 0 || minMllAFOS > 12
// zveto3l:    mZ1 < 76 || mZ1 > 106

     if(!makeCut( abs(_l1Cand->pdgId()) > 0 && abs(_l2Cand->pdgId()) > 0, "pdgId") ) return false;
     if(!makeCut( _l1Cand->charge()*_l2Cand->charge()>0, "charge") ) return false;
     if(!makeCut( _l1Cand->pt()>25 && _l2Cand->pt()>25, "pt") ) return false;
     
     cout<<_l1Cand->pdgId()<<"  "<<_l2Cand->pdgId()<<"  "<<SusyModule::kMedium<<"  "<<SusyModule::kTight<<endl;

     int wp1 = SusyModule::kMedium;//((abs(_l1Cand->pdgId())==13)?(SusyModule::kMedium):SusyModule::kTight);
     int wp2 = SusyModule::kMedium;//((abs(_l2Cand->pdgId())==13)?(SusyModule::kMedium):SusyModule::kTight);
     if(abs(_l1Cand->pdgId())==13) wp1 = SusyModule::kTight;
     if(abs(_l2Cand->pdgId())==13) wp2 = SusyModule::kTight;


     // cout<<wp1<<"  "<<wp2<<"   "<<_idxL1<<"  "<<_idxL2<<"   "<< _susyMod->multiIsoSel(_idxL1, wp1 )
     // 	 <<"   "<<_susyMod->multiIsoSel(_idxL2, wp2 )<<endl;

     if(!makeCut( _susyMod->multiIsoSel(_idxL1, wp1 ), "iso1") ) return false;
     if(!makeCut( _susyMod->multiIsoSel(_idxL2, wp2 ), "iso2") ) return false;

     bool id= _vc->get("LepGood_mediumMuonId", _idxL1)>0 && 
              _vc->get("LepGood_mediumMuonId", _idxL2)>0;

     if(!makeCut(id, "id") ) return false;

     bool elId= ( _vc->get("LepGood_mvaIdPhys14",_idxL1) >=0.73+(0.57-0.73)*(abs(_l1Cand->eta())>0.8)+(+0.05-0.57)*(abs(_l1Cand->eta())>1.479) || abs(_l1Cand->pdgId()) == 13) && 
     ( _vc->get("LepGood_mvaIdPhys14",_idxL2) >=0.73+(0.57-0.73)*(abs(_l2Cand->eta())>0.8)+(+0.05-0.57)*(abs(_l2Cand->eta())>1.479) || abs(_l2Cand->pdgId()) == 13); 

     cout<<_vc->get("LepGood_mvaIdPhys14",_idxL1)<<"   "<<_l1Cand->eta()<<"   "<<_vc->get("LepGood_mvaIdPhys14",_idxL2)<<"   "<<_l2Cand->eta()<<endl;

     if(!makeCut(elId, "elId") ) return false;

// exclusive:  nLepGood10 == 2
// anyll: abs(LepGood1_pdgId) > 0 && abs(LepGood2_pdgId) > 0
// same-sign: LepGood1_charge*LepGood2_charge > 0
// lep1_pt25: LepGood1_pt > 25 
// lep2_pt25: LepGood2_pt > 25 
// lep iso: multiIso_multiWP(LepGood1_pdgId,LepGood1_pt,LepGood1_eta,LepGood1_miniRelIso,LepGood1_jetPtRatio,LepGood1_jetPtRel,2) > 0 && 
//          multiIso_multiWP(LepGood2_pdgId,LepGood2_pt,LepGood2_eta,LepGood2_miniRelIso,LepGood2_jetPtRatio,LepGood2_jetPtRel,2) > 0
//lep mu id: LepGood1_mediumMuonId > 0 && LepGood2_mediumMuonId > 0 
// lep el id: ( LepGood1_mvaIdPhys14 >=0.73+(0.57-0.73)*(abs(LepGood1_eta)>0.8)+(+0.05-0.57)*(abs(LepGood1_eta)>1.479) || abs(LepGood1_pdgId) == 13) && 
//            ( LepGood2_mvaIdPhys14 >=0.73+(0.57-0.73)*(abs(LepGood2_eta)>0.8)+(+0.05-0.57)*(abs(LepGood2_eta)>1.479) || abs(LepGood2_pdgId) == 13) 

     // cout<<_idxL1<<"  "<<_idxL2<<"   "<<_vc->get("LepGood_sip3d",_idxL1)<<"   "<<_vc->get("LepGood_sip3d",_idxL2)<<endl;

     if(!makeCut( max(_vc->get("LepGood_sip3d",_idxL1),_vc->get("LepGood_sip3d",_idxL2)) < 4, "sip") ) return false;

     bool conv= (abs(_l1Cand->pdgId())==13 || (_vc->get("LepGood_convVeto",_idxL1) && _vc->get("LepGood_lostHits",_idxL1) == 0)) &&
                (abs(_l2Cand->pdgId())==13 || (_vc->get("LepGood_convVeto",_idxL2) && _vc->get("LepGood_lostHits",_idxL2) == 0));

     if(!makeCut(  conv, "conversion") ) return false;

     bool charge= (_vc->get("LepGood_tightCharge",_idxL1)> (abs(_l1Cand->pdgId())==11)) &&
		   (_vc->get("LepGood_tightCharge",_idxL2)> (abs(_l2Cand->pdgId())==11));

     if(!makeCut(  charge, "tight charge") ) return false;
     
  return true;

}


//===========================================================================
void
SSDL2015::selectLeptons() {
  for(size_t il=0;il<_vc->get("nLepGood"); il++) {

    bool isMu=std::abs(_vc->get("LepGood_pdgId", il))==13;

    Candidate* cand=Candidate::create(_vc->get("LepGood_pt", il),
				      _vc->get("LepGood_eta", il),
				      _vc->get("LepGood_phi", il),
				      _vc->get("LepGood_pdgId", il),
				      _vc->get("LepGood_charge", il),
				      isMu?0.105:0.0005);

    // cout<<" pt: "<<cand->pt()<<"  eta: "<<cand->eta()<<"   phi: "<<cand->phi()<<"  pdgId: "<<_vc->get("LepGood_pdgId", il)<<"   dxy: "<<_vc->get("LepGood_dxy",il)<<"  dz: "<<_vc->get("LepGood_dz",il)<<endl;

    if(!looseLepton(il, cand->pdgId() ) ) continue;
    _looseLeps.push_back(cand);
    _looseLepsIdx.push_back(il);
    
    //cout<<" -> selected"<<endl;

    if(cand->pt()<10) continue;
    _looseLeps10.push_back(cand);
    _looseLeps10Idx.push_back(il);
    
  }

  //veto on loose leptons =====================
  for(size_t il=0;il<_looseLeps.size();il++) {

    // cout<<" pt:"<<_looseLeps[il]->pt()<<"  selected"<<"  "<<_susyMod->passMllMultiVeto( _looseLeps[il], &_looseLeps, 76, 106, true)<<"  "<<_susyMod->passMllMultiVeto( _looseLeps[il], &_looseLeps, 0, 12, true)<<endl;

    if(!_susyMod->passMllMultiVeto( _looseLeps[il], &_looseLeps, 76, 106, true) ||
       !_susyMod->passMllMultiVeto( _looseLeps[il], &_looseLeps, 0, 12, true) ) continue;

   

    _looseLepsVeto.push_back( _looseLeps[il]);
    _looseLepsVetoIdx.push_back(_looseLepsIdx[il]);
  }

  //veto on loose leptons 10, likely the one to use for the fakes =====================
  for(size_t il=0;il<_looseLeps10.size();il++) {
    if(!_susyMod->passMllMultiVeto( _looseLeps10[il], &_looseLeps, 76, 106, true) ||
       !_susyMod->passMllMultiVeto( _looseLeps10[il], &_looseLeps, 0, 12, true) ) continue;

    _looseLepsVeto10.push_back(_looseLeps10[il]);
    _looseLepsVeto10Idx.push_back(_looseLeps10Idx[il]);
  }

  //tight leptons definitions =================

  for(size_t il=0;il<_looseLeps10.size();il++) {

    if(!tightLepton(_looseLeps10Idx[il], _looseLeps10[il]->pdgId()) ) {
      if(!fakableLepton(_looseLeps10Idx[il], _looseLeps10[il]->pdgId()) ) continue; //not a fakable object

      _fakableLeps10.push_back(_looseLeps10[il]);
      _fakableLeps10Idx.push_back(_looseLeps10Idx[il]);
      
      if(!_susyMod->passMllMultiVeto( _looseLeps10[il], &_looseLeps, 76, 106, true) ||
	 !_susyMod->passMllMultiVeto( _looseLeps10[il], &_looseLeps, 0, 12, true) ) continue;

      _fakableLepsVeto10.push_back(_looseLeps10[il]);
      _fakableLepsVeto10Idx.push_back(_looseLeps10Idx[il]);

      continue;
    }
    
    _tightLeps10.push_back(_looseLeps10[il]);
    _tightLeps10Idx.push_back(_looseLeps10Idx[il]);
    
    
    if(!_susyMod->passMllMultiVeto( _looseLeps10[il], &_looseLeps, 76, 106, true) ||
       !_susyMod->passMllMultiVeto( _looseLeps10[il], &_looseLeps, 0, 12, true) ) continue;
    
    _tightLepsVeto10.push_back(_looseLeps10[il]);
    _tightLepsVeto10Idx.push_back(_looseLeps10Idx[il]);
  }

}


//==============================================
//MET uncertainty variation
TVector2
SSDL2015::varyMET() {

  unsigned int nJets=_vc->get("nJet");
  unsigned int nFwdJets=_vc->get("nJetFwd");
  if(!isInUncProc() ) {//first, store the jets
    _uncleanJets.clear();
    _uncleanFwdJets.clear();
    for(unsigned int ij=0;ij<nJets;ij++) { 
      TVector2 jet; jet.SetMagPhi( _vc->get("Jet_pt", ij), _vc->get("Jet_phi", ij)   );
      _uncleanJets.push_back(jet);
    }
    for(unsigned int ij=0;ij<nFwdJets;ij++) { 
      TVector2 jet; jet.SetMagPhi(_vc->get("JetFwd_pt", ij),_vc->get("JetFwd_phi", ij));
      _uncleanFwdJets.push_back(jet);
    }
  }
  
  TVector2 met; met.SetMagPhi(_vc->get("met_pt"), _vc->get("met_phi") );
  for(unsigned int ij=0;ij<nJets;ij++) { 
    //add back the standard jets
    met += _uncleanJets[ij];
    //JES varied jets
    TVector2 jet; jet.SetMagPhi( _vc->get("Jet_pt", ij), _vc->get("Jet_phi", ij)   );
    met -= jet;
  }
  for(unsigned int ij=0;ij<nFwdJets;ij++) { 
    //add back the standard jets
    met += _uncleanFwdJets[ij];
    //JES varied jets
    TVector2 jet; jet.SetMagPhi(_vc->get("JetFwd_pt", ij), _vc->get("JetFwd_phi", ij) );
    met -= jet;
  }
  
  return met;
}



