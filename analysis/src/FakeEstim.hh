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
  float getProbAtLeastNIso(CandList fObjs, vector<unsigned int> idxs, int nIso);
  bool genMatchedMisCharge();
  bool genMatchedToFake(int id);
  float getFR(Candidate* cand, int idx);

  //============================
  bool electronMvaCut(int elIdx, int wp);
  bool isMuSelected(int idx, int wp);
  bool isElSelected(int idx, int wp);
  bool lepMvaId(int idx, int wp);
  bool passFakeId(int idx, bool isMu); 
  bool isIsolated(int idx, int wp);
  bool goodJetSelection(int jetIdx);
  float HT();
  void retrieveObjects();
  bool alternateSSEventSelection(bool switchWF=true);
  bool mllVetoSelection();
  bool mllLMGVeto(Candidate* cand, Candidate* veto);
  bool mllZVeto(Candidate* cand, Candidate* veto);

//==============================
  void defineLeptonWPS();
  void setSignalRegion();
  void setCut(string var, float valCut, string cType, float upValCut=0);
  
  //=============================
  //advanced fast selection
  bool testRegion();
  void categorize();


private: 

  //counter categories, 0 is ALWAYS global (even if not specified later
  //enum {kGlobal=0,kLowMETMT,kGenFake,kGenMisCharge,kOneIso,kNoIso, kSelId};
  enum {kGlobal=0,
	kSR1A,kSR2A,kSR3A,kSR4A,kSR7A,kSR8A,kSR9A,kSR10A,
	kSR13A,kSR14A,kSR15A,kSR16A,
	kSR1B,
	kSR3B,
	kSR5B,
	kSR1AL,kSR2AL,kSR3AL,kSR4AL,kSR1AH,kSR2AH,kSR3AH,kSR4AH,kSR5A,kSR6A,
	kSR7AL,kSR8AL,kSR9AL,kSR10AL,kSR7AH,kSR8AH,kSR9AH,kSR10AH,kSR11A,kSR12A,
	kSR13AL,kSR14AL,kSR15AL,kSR16AL,kSR13AH,kSR14AH,kSR15AH,kSR16AH,kSR17A,kSR18A,
	kSR19AL,kSR19AH,kSR20A,
	kSR1BL,kSR1BH,kSR2B,kSR2BL,kSR2BH,
	kSR3BL,kSR3BH,kSR4B,kSR4BL,kSR4BH,
	kSR5BL,kSR5BH,kSR6B,kSR6BL,kSR6BH,
	kSR7BL,kSR7BH,kSR8B,
	kOneIso,kNoIso, kSelId};


  // enum {kLoose=0,kTight,kVTight,kHTight,kNWPs};
  enum {kDenom=0,
	kLoose,
	kMedium,
	kTight,
	kVTight,
	kHTight,
	k60,
	k70,
	k75,
	k775,
	k80,
	k825,
	k85,
	k875,
	k90,
	k925,
	k95,
	k975,
	kNWPs};


  enum {kMiniIso=0,kPtRatio,kPtRel};
  
  CandList _allLeps;
  CandList _looseLeps;
  CandList _leptons;
  CandList _nonFullIdLeps;
  CandList _fullIdLeps;
  CandList _lepsForFR;
  CandList _jets;

  Candidate* _met;

  float _mTmin;
  
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
  float _valCutMTSR;
  float _valCutNJetsSR;
  float _valCutNBJetsSR;

  float _valCutNJetsCond;
  float _valCutMETCond;

  std::string _cTypeHTSR;
  std::string _cTypeMETSR;
  std::string _cTypeMTSR;
  std::string _cTypeNJetsSR;
  std::string _cTypeNBJetsSR;

  std::string _cTypeNJetsCond;
  std::string _cTypeMETCond;

  float _upValCutHTSR;
  float _upValCutMETSR;
  float _upValCutMTSR;
  float _upValCutNJetsSR;
  float _upValCutNBJetsSR;

  float _upValCutNJetsCond;
  float _upValCutMETCond;


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

  float _metCut;
  int _isoLvl;


  int _fakeEl;
  int _fakeMu;
  int _nDFake;
  int _nSFake;
  int _nCharge;
  int _nOther;


  vector<string> _categs;
  bool _categorization;

};



#endif
