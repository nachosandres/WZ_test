#include "analysis/modules/SusyModule.hh"


SusyModule::SusyModule(VarClass* vc):
  _vc(vc)
{
  defineLeptonWPS();
}

SusyModule::~SusyModule() {

  delete _vc;
}


void
SusyModule::defineLeptonWPS() {

  _sipWP.resize(kNWPs);
  _elMvaIdWP.resize(3);
  _multiIsoWP.resize(3);
  _ptWP.resize(2);
  for(int i=0;i<3;++i) {
    _elMvaIdWP[i].resize(kNWPs);
    _multiIsoWP[i].resize(kNWPs);
    if(i!=2) _ptWP[i].resize(kNWPs);
  }
  
  //conv rej ====================
  _cLostHitWP[kLoose]=1; 
  _cLostHitWP[kTight]=0; 

  //pt ============================
  _ptWP[kEl][kLoose]=7;  _ptWP[kMu][kLoose]=5;
  _ptWP[kEl][kTight]=10;  _ptWP[kMu][kTight]=10;

  //sip & IP ======================
  _sipWP[kLoose]=1000.0; 
  _sipWP[kTight]=4.0; 
  
  _dxyWP[kLoose]=0.05; //cm
  _dxyWP[kTight]=0.05; //cm
  
  _dzWP[kLoose]=0.1; //cm
  _dzWP[kTight]=0.1; //cm


  //el mva id ======================
  _elMvaIdWP[kEBC][kLoose]=-0.11;
  _elMvaIdWP[kEBF][kLoose]=-0.35;
  _elMvaIdWP[kEE][kLoose]=-0.55;

  _elMvaIdWP[kEBC][kTight]=0.73;
  _elMvaIdWP[kEBF][kTight]=0.57;
  _elMvaIdWP[kEE][kTight]=0.05;

  //multiIso =======================
  _multiIsoWP[kMiniIso][kDenom]=0.4; _multiIsoWP[kPtRatio][kDenom]=0; _multiIsoWP[kPtRel][kDenom]=0;
  _multiIsoWP[kMiniIso][kMedium]=0.14; _multiIsoWP[kPtRatio][kMedium]=0.68; _multiIsoWP[kPtRel][kMedium]=6.7; 
  _multiIsoWP[kMiniIso][kTight]=0.10; _multiIsoWP[kPtRatio][kTight]=0.70; _multiIsoWP[kPtRel][kTight]=7;
  
}


//=====================================================
// lepton selection

bool
SusyModule::multiIsoSel(int idx, int wp) const {

  if( _vc->get("LepGood_miniRelIso", idx)<_multiIsoWP[kMiniIso][wp] &&
      (_vc->get("LepGood_jetPtRatio", idx)>_multiIsoWP[kPtRatio][wp] ||
       _vc->get("LepGood_jetPtRel", idx)>_multiIsoWP[kPtRel][wp]) ) return true;
  
  return false;
}

bool 
SusyModule::elMvaSel(int idx, int wp) const {

  int etaBin=-1;

  if(std::abs(_vc->get("LepGood_eta", idx)) < 0.8) etaBin=0;
  else if(std::abs(_vc->get("LepGood_eta", idx)) < 1.479) etaBin=1;
  else if(std::abs(_vc->get("LepGood_eta", idx)) < 2.4) etaBin=2;
  
  if(_vc->get("LepGood_mvaIdPhys14", idx) <  _elMvaIdWP[etaBin][wp]  ) return false;
    
  return true;
}


bool
SusyModule::muIdSel(int idx, int wp) const {

  int wpIso=kDenom;
  
  if( _vc->get("LepGood_pt" , idx)<_ptWP[kMu][wp] ) return false;
  if( std::abs(_vc->get("LepGood_eta" , idx))>2.4 ) return false;
  
  if( _vc->get("LepGood_mediumMuonId", idx)<=0 ) return false;

  if(wp!=kLoose)
    if( _vc->get("LepGood_tightCharge" , idx)<=1 ) return false;
 
  if(_vc->get("LepGood_sip3d", idx)>_sipWP[wp]) return false;
  if( std::abs(_vc->get("LepGood_dz", idx))>_dzWP[wp] ) return false;
  if( std::abs(_vc->get("LepGood_dxy", idx))>_dxyWP[wp] ) return false;
  if( !multiIsoSel(idx, wpIso) ) return false;
  
  return true;
}


bool
SusyModule::elIdSel(int idx, int wp) const {

  int wpIso=kDenom;
  
  if(_vc->get("LepGood_pt", idx)<_ptWP[kEl][wp] ) return false;
  if(std::abs(_vc->get("LepGood_eta", idx))>2.4) return false;
  if(std::abs(_vc->get("LepGood_eta", idx))>1.4442 &&
     std::abs(_vc->get("LepGood_eta", idx))<1.566) return false;

  if(_vc->get("LepGood_convVeto", idx)<=_cLostHitWP[wp]) return false;
  if(_vc->get("LepGood_lostHits", idx)!=0) return false;

  if(wp!=kLoose)
    if(_vc->get("LepGood_tightCharge", idx)<=1) return false; 

  //always tight
  if( !elMvaSel(idx, wp) ) return false;
  if(_vc->get("LepGood_sip3d", idx)>_sipWP[wp]) return false;
  if( std::abs(_vc->get("LepGood_dz", idx))>_dzWP[wp] ) return false;
  if( std::abs(_vc->get("LepGood_dxy", idx))>_dxyWP[wp] ) return false;
  if( !multiIsoSel(idx, wpIso) ) return false;
 

  // electron cleaning ==================
  for(unsigned int il=0; il<_vc->get("nLepGood"); ++il){
    float dr = KineUtils::dR(_vc->get("LepGood_eta", il), _vc->get("LepGood_eta", idx),
                             _vc->get("LepGood_phi", il), _vc->get("LepGood_phi", idx));
    if(std::abs(_vc->get("LepGood_pdgId"))==13 && dr<0.05 ) return false;
  }

  return true;
}

//===========================================================
bool
SusyModule::jetSel( int jetIdx) const {
  
  if(_vc->get("Jet_pt", jetIdx)<40.0) return false;
  return true;
}

float 
SusyModule::HT(const CandList* jets){
  float ht = 0;
  unsigned int nJets=jets->size();
  for(unsigned int i=0; i<nJets; ++i) 
    ht += jets->at(i)->pt();
  
  return ht;
}


//=============================================================
bool
SusyModule::mllVetoSelection(const Candidate* l1, const Candidate* l2,
			     const CandList* allLeps) const {
  
  for(unsigned int i = 0; i < allLeps->size(); ++i) {
    if(l1!=allLeps->at(i) && mllZVeto(l1, allLeps->at(i))) return false;
    if(l2!=allLeps->at(i) && mllZVeto(l2, allLeps->at(i))) return false;
    if(l1!=allLeps->at(i) && mllLMGVeto(l1, allLeps->at(i))) return false;
    if(l2!=allLeps->at(i) && mllLMGVeto(l2, allLeps->at(i))) return false;
  }
  
  return true;
}

bool 
SusyModule::mllLMGVeto(const Candidate* cand, const Candidate* veto) const {
  
  float mll = Candidate::create(cand, veto)->mass();

  if(mll <= 8.0) return true;
  if(cand->charge()==veto->charge() ) return false;
  if(mll <= 12.0) return true;

  return false;
}


bool 
SusyModule::mllZVeto(const Candidate* cand, const Candidate* veto) const {
 
  if(veto->pt() < 10) return false;
  float mll = Candidate::create(cand, veto)->mass();
  
  if(cand->charge()==veto->charge() ) return false;
  if(std::abs(cand->pdgId())==std::abs(veto->pdgId()) ) {
    if(mll >= 76.0 && mll <= 106.0) return true;
  }

  return false;
}
