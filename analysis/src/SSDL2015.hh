#ifndef SSDL2015_HH
#define SSDL2015_HH

#include "analysis/core/MPAF.hh"

#include "analysis/modules/SusyModule.hh"

class SSDL2015: public MPAF {

public:

  // Member Functions

  SSDL2015(std::string);
  virtual ~SSDL2015();


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
  int genMatchCateg(const Candidate* cand);
  bool genMatchedToFake(int id);

  bool passGenSelection();
  
  float getFR(Candidate* cand, int idx);
  
  TVector2 varyMET();

  //============================
  void retrieveObjects();
  bool ssLeptonSelection();
  void selectLeptons();

  //==============================
  void wzCRSelection();
  void setSignalRegions();
  void setSelLine(string str);
  //void setCut(string var, float valCut, string cType, float upValCut=0);
  
  //=============================
  //advanced fast selection
  bool testRegion();
  void categorize();

  bool passCERNSelection();
  bool looseLepton(int idx, int pdgId);
  bool tightLepton(int idx, int pdgId);
  bool fakableLepton(int idx, int pdgId);

private: 

  //counter categories, 0 is ALWAYS global (even if not specified later
  //enum {kGlobal=0,kLowMETMT,kGenFake,kGenMisCharge,kOneIso,kNoIso, kSelId};
  enum {kGlobal=0,
	
	kSR1A, kSR2A, kSR3A, kSR4A, kSR5A, kSR6A, kSR7A, kSR8A,
	kSR9A, kSR10A, kSR11A, kSR12A, kSR13A, kSR14A, kSR15A, kSR16A,
	kSR17A, kSR18A, kSR19A, kSR20A, kSR21A, kSR22A, kSR23A, kSR24A,
	kSR25A, kSR26A, kSR27A, kSR28A, kSR29A, kSR30A, kSR31A, kSR32A,

	kSR1B, kSR2B, kSR3B, kSR4B, kSR5B, kSR6B, kSR7B, kSR8B,
	kSR9B, kSR10B, kSR11B, kSR12B, kSR13B, kSR14B, kSR15B, kSR16B,
	kSR17B, kSR18B, kSR19B, kSR20B, kSR21B, kSR22B, kSR23B, kSR24B,
	kSR25B, kSR26B,

	kSR1C, kSR2C, kSR3C, kSR4C, kSR5C, kSR6C, kSR7C, kSR8C,
    
	//kBR0H, kBR0M, kBR0L,
	kBR00H, kBR00M, kBR00L,
	kBR10H, kBR10M, kBR10L,
	kBR20H, kBR20M, kBR20L,
	kBR30H, kBR30M, kBR30L,
	
	kSR1A_Fake, kSR2A_Fake, kSR3A_Fake, kSR4A_Fake, kSR5A_Fake, kSR6A_Fake, kSR7A_Fake, kSR8A_Fake,
	kSR9A_Fake, kSR10A_Fake, kSR11A_Fake, kSR12A_Fake, kSR13A_Fake, kSR14A_Fake, kSR15A_Fake, kSR16A_Fake,
	kSR17A_Fake, kSR18A_Fake, kSR19A_Fake, kSR20A_Fake, kSR21A_Fake, kSR22A_Fake, kSR23A_Fake, kSR24A_Fake,
	kSR25A_Fake, kSR26A_Fake, kSR27A_Fake, kSR28A_Fake, kSR29A_Fake, kSR30A_Fake, kSR31A_Fake, kSR32A_Fake,

	kSR1B_Fake, kSR2B_Fake, kSR3B_Fake, kSR4B_Fake, kSR5B_Fake, kSR6B_Fake, kSR7B_Fake, kSR8B_Fake,
	kSR9B_Fake, kSR10B_Fake, kSR11B_Fake, kSR12B_Fake, kSR13B_Fake, kSR14B_Fake, kSR15B_Fake, kSR16B_Fake,
	kSR17B_Fake, kSR18B_Fake, kSR19B_Fake, kSR20B_Fake, kSR21B_Fake, kSR22B_Fake, kSR23B_Fake, kSR24B_Fake,
	kSR25B_Fake, kSR26B_Fake,

	kSR1C_Fake, kSR2C_Fake, kSR3C_Fake, kSR4C_Fake, kSR5C_Fake, kSR6C_Fake, kSR7C_Fake, kSR8C_Fake,

	
	kBR00H_Fake, kBR00M_Fake, kBR00L_Fake,
	kBR10H_Fake, kBR10M_Fake, kBR10L_Fake,
	kBR20H_Fake, kBR20M_Fake, kBR20L_Fake,
	kBR30H_Fake, kBR30M_Fake, kBR30L_Fake,

	kGlobalFake,
	
	kWZCR
  };

  enum {kNoGenMatch=0, kMisMatchPdgId,
	kMisChargePdgId, kGenMatched};
  // enum {kLoose=0,kTight,kVTight,kHTight,kNWPs};
  
  SusyModule* _susyMod;
  
  Candidate* _met;
  
  std::map<std::string,float*> _val;
  float _nBJets;
  float _mTmin;
  float _metPt;
  float _nJets;
  float _HT;
  
  //MM ugly
  std::map<std::string, std::vector<std::vector<std::vector<std::string> > > > _sels;

  //fakes
  bool _isFake;
  int _idxFake;

  CandList _looseLeps;
  std::vector<unsigned int>  _looseLepsIdx;

  CandList _looseLeps10;
  std::vector<unsigned int>  _looseLeps10Idx;

  CandList _looseLepsVeto;
  std::vector<unsigned int>  _looseLepsVetoIdx;

  CandList _looseLepsVeto10;
  std::vector<unsigned int>  _looseLepsVeto10Idx;

  CandList _fakableLeps10;
  std::vector<unsigned int>  _fakableLeps10Idx;

  CandList _fakableLepsVeto10;
  std::vector<unsigned int>  _fakableLepsVeto10Idx;
  
  CandList _tightLeps10;
  std::vector<unsigned int>  _tightLeps10Idx;

  CandList _tightLepsVeto10;
  std::vector<unsigned int>  _tightLepsVeto10Idx;

  CandList _jets;
  std::vector<unsigned int>  _jetsIdx;
  
  CandList _bJets;
  std::vector<unsigned int>  _bJetsIdx;
  
  unsigned int _nLooseLeps;
 

  int _nIso;
  int _nSelPair2Iso;
  int _nSelPair1Iso;
  int _nSelPair0Iso;

  Candidate * _l1Cand;
  Candidate * _l2Cand;
  int _idxL1;
  int _idxL2; 
 

  string _lepflav;
  string _leppt;
  string _leptl;
  string _SR;
  string _FR;

  int _fakeEl;
  int _fakeMu;
  int _nDFake;
  int _nSFake;
  int _nCharge;
  int _nOther;


  vector<string> _categs;
  bool _categorization;

  vector<TVector2> _uncleanJets;
  vector<TVector2> _uncleanFwdJets;

};



#endif
