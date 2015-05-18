#ifndef FakeEstim_HH
#define FakeEstim_HH

#include "analysis/core/MPAF.hh"

#include "analysis/modules/SusyModule.hh"

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
  void retrieveObjects();
  bool alternateSSEventSelection(bool switchWF=true);
 
  //==============================
  void setSignalRegions();
  void setSelLine(string str);
  //void setCut(string var, float valCut, string cType, float upValCut=0);
  
  //=============================
  //advanced fast selection
  bool testRegion();
  void categorize();

  bool passCERNSelection();
  bool goodLepton(int idx, int pdgId);

private: 

  //counter categories, 0 is ALWAYS global (even if not specified later
  //enum {kGlobal=0,kLowMETMT,kGenFake,kGenMisCharge,kOneIso,kNoIso, kSelId};
  enum {kGlobal=0,
	
	kSR1A, kSR2A, kSR3A, kSR4A, kSR5A, kSR6A, kSR7A, kSR8A,
	kSR9A, kSR10A, kSR11A, kSR12A, kSR13A, kSR14A, kSR15A, kSR16A,
	kSR17A, kSR18A, kSR19A, kSR20A, kSR21A, kSR22A, kSR23A, kSR24A,
	kSR25A, kSR26A, kSR27A, kSR28A, kSR29A, kSR30A, kSR31A, kSR32A,

	// kSR1B, kSR2B, kSR3B, kSR4B, kSR5B, kSR6B, kSR7B, kSR8B,
	// kSR9B, kSR10B, kSR11B, kSR12B, kSR13B, kSR14B, kSR15B, kSR16B,
	// kSR17B, kSR18B, kSR19B, kSR20B, kSR21B, kSR22B, kSR23B, kSR24B,
	// kSR25B, kSR26B,

	// kSR1C, kSR2C, kSR3C, kSR4C, kSR5C, kSR6C, kSR7C, kSR8C,
    
	kBR0H, kBR0M, kBR0L,
	kBR10H, kBR10M, kBR10L,
	kBR20H, kBR20M, kBR20L,
	kBR30H, kBR30M, kBR30L,

	kOneIso,kNoIso, kSelId};


  // enum {kLoose=0,kTight,kVTight,kHTight,kNWPs};
  
  SusyModule* _susyMod;
 
  CandList _allLeps;
  CandList _looseLeps;
  CandList _leptons;
  CandList _nonFullIdLeps;
  CandList _fullIdLeps;
  CandList _lepsForFR;
  CandList _jets;

  Candidate* _met;
  
  std::map<std::string,float*> _val;
  float _nBJets;
  float _mTmin;
  float _metPt;
  float _nJets;
  float _HT;
  
  //MM ugly
  std::map<std::string, std::vector<std::vector<std::vector<std::string> > > > _sels;




  std::vector<unsigned int> _looseLepsIdx;
  std::vector<unsigned int> _leptonsIdx;
  std::vector<unsigned int> _nonFullIdLepsIdx;
  std::vector<unsigned int> _fullIdLepsIdx;
  std::vector<unsigned int> _tmpLepIdx;
  std::vector<unsigned int> _lepsForFRIdx;
 
 
  
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

};



#endif
