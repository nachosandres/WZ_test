#ifndef FakeEstim_HH
#define FakeEstim_HH

#include "analysis/core/MPAF.hh"

class FakeEstim: public MPAF {

public:

  // Member Functions

  FakeEstim(std::string);
  virtual ~FakeEstim();


private:

  void initialize();

  void run();
  void defineOutput();
  void modifyWeight();
  void writeOutput();

  void modifySkimming();

  //============================
  bool noIsoSel();
  bool oneIsoSel();
  bool genFake();
  float getProbAtLeastNIso(CandList fObjs, vector<unsigned int> idxs, int nIso);
  bool genMatchedMisCharge();
  bool genMatchedToFake(int id);
  float getFR(Candidate* cand, int idx);

  //============================
  bool electronMvaCut(int elIdx, int wp);
  // bool electronCutId(int elIdx, float sip, bool mvatight);
  // bool muonCutId(int muIdx, float sip);
  // bool looseElectronSelection(int elIdx);
  // bool looseMuonSelection(int muIdx);
  // bool tightElectronSelection(int elIdx);
  // bool tightMuonSelection(int muIdx);
  bool isMuSelected(int idx, int wp);
  bool isElSelected(int idx, int wp);
  bool lepMvaId(int idx, int wp);
  bool passFakeId(int idx, bool isMu); 
  bool isIsolated(int idx, int wp);
  bool goodJetSelection(int jetIdx);
  float HT();
  void retrieveObjects();
  bool signalSelection();
  bool ssEventSelection();
  bool alternateSSEventSelection();
  bool mllVetoSelection();
  bool mllLMGVeto(Candidate* cand, Candidate* veto);
  bool mllZVeto(Candidate* cand, Candidate* veto);

//==============================
  void defineLeptonWPS();
  void setSignalRegion();
  void setCut(string var, float valCut, string cType, float upValCut=0);
  

private: 

  //counter categories, 0 is ALWAYS global (even if not specified later
  enum {kGlobal=0,kGenFake,kGenMisCharge,kOneIso,kNoIso, kSelId};
  enum {kLoose=0,kTight,kVTight,kHTight,kNWPs};
  enum {kMiniIso=0,kPtRatio,kPtRel};
  
  CandList _allLeps;
  CandList _looseLeps;
  CandList _leptons;
  CandList _nonFullIdLeps;
  CandList _fullIdLeps;
  CandList _lepsForFR;
  CandList _jets;

  Candidate* _met;
  
  std::vector<unsigned int> _looseLepsIdx;
  std::vector<unsigned int> _leptonsIdx;
  std::vector<unsigned int> _nonFullIdLepsIdx;
  std::vector<unsigned int> _fullIdLepsIdx;
  std::vector<unsigned int> _tmpLepIdx;
  std::vector<unsigned int> _lepsForFRIdx;
 
  vector<float> _sipWP;
  vector<vector<float> > _elMvaIdWP;
  vector<float> _mvaIdLPtWP;
  vector<float> _mvaIdHPtWP;
  vector<float> _isoWP;
  vector<float> _miniIsoWP;
  vector<float> _ptRelWP;
  vector<vector<float> > _multiIsoWP;
  
  unsigned int _nLooseLeps;
  unsigned int _nJets;

  float _valCutHTSR;
  float _valCutMETSR;
  float _valCutHTCondSR;
  float _valCutMETHighSR;
  float _valCutMETLowSR;
  float _valCutNJetsSR;
  float _valCutNBJetsSR;

  std::string _cTypeHTSR;
  std::string _cTypeMETSR;
  std::string _cTypeHTCondSR;
  std::string _cTypeMETHighSR;
  std::string _cTypeMETLowSR;
  std::string _cTypeNJetsSR;
  std::string _cTypeNBJetsSR;

  float _upValCutHTSR;
  float _upValCutMETSR;
  float _upValCutHTCondSR;
  float _upValCutMETHighSR;
  float _upValCutMETLowSR;
  float _upValCutNJetsSR;
  float _upValCutNBJetsSR;

  int _nIso;
  int _nSelPair2Iso;
  int _nSelPair1Iso;
  int _nSelPair0Iso;

  Candidate * _l1Cand;
  Candidate * _l2Cand;
  int _lep_idx1;
  int _lep_idx2; 
 
  float _HT;


  string _btag;
  string _fakes;
  string _lepflav;
  string _lepId;
  string _lepiso;
  string _leppt;
  string _leptl;
  string _extScheme;
  string _SR;
  string _FR;


  int _fakeEl;
  int _fakeMu;
  int _nDFake;
  int _nSFake;
  int _nCharge;
  int _nOther;
};



#endif
