/*****************************************************************************
******************************************************************************
******************************************************************************
**                                                                          **
** The Multi-Purpose Analysis Framework                                     **
**                                                                          **
** Constantin Heidegger, CERN, Summer 2014                                  **
**                                                                          **
******************************************************************************
******************************************************************************
*****************************************************************************/

#ifndef SYNCHRA5_HH
#define SYNCHRA5_HH

#include "analysis/core/MPAF.hh"

class synchRA5: public MPAF {

public:


  // Member Functions

  synchRA5(std::string);
  virtual ~synchRA5();


private:

  void initialize();

  void run();
  void defineOutput();
  void loadInput();
  void modifyWeight();
  void writeOutput();

  void modifySkimming();

  void collectKinematicObjects();
  bool electronSelection(int);
  bool goodJetSelection(int);
  bool muonSelection(int);
  bool vetoElectronSelection(int);
  bool vetoMuonSelection(int);

  void setCut(std::string, float, std::string, float = 0); 
  void setSignalRegion();

  bool baseSelection();
  bool srSelection();
  bool vetoEventSelection();

  void fillEventPlots(std::string);
  void fillLeptonPlots(std::string);
  void fillJetPlots(std::string);

  int genMatchCateg(const Candidate*);

  float HT();
  int eventCharge();

private: 

  //counter categories, 0 is ALWAYS global (even if not specified later
  enum {kGlobal=0, kElId, kElVeto, kMuId, kMuVeto, kJetId, kBJetId, kVetoLepSel};

  enum {kNoGenMatch=0, kMisMatchPdgId,
	kMisChargePdgId, kGenMatched};

  float _valCutHTSR;
  float _valCutHTCondSR;
  float _valCutMETLowSR;
  float _valCutMETHighSR;
  float _valCutNJetsSR;
  float _valCutNBJetsSR;

  std::string _cTypeHTSR;
  std::string _cTypeHTCondSR;
  std::string _cTypeMETLowSR;
  std::string _cTypeMETHighSR;
  std::string _cTypeNJetsSR;
  std::string _cTypeNBJetsSR;

  float _upValCutHTSR;
  float _upValCutHTCondSR;
  float _upValCutMETLowSR;
  float _upValCutMETHighSR;
  float _upValCutNJetsSR;
  float _upValCutNBJetsSR;
	
  std::vector<int> _elIdx;
  std::vector<int> _muIdx;

  int _nLeps;
  int _nEls;
  int _nVEls;
  int _nMus;
  int _nVMus;
  int _nJets;
  int _nBJets;

  CandList _leps;
  CandList _els;
  CandList _vetoleps;
  CandList _vEls;
  CandList _mus;
  CandList _vMus;
  CandList _jets;
  CandList _bJets;
  Candidate* _met;

  Candidate * _first;
  Candidate * _second;  
  CandList _thirdleps;  
  float _HT;


  bool _mvaId;
  string _bvar;
  string _lepflav;
  
  string _mva;
  string _btag;
  string _PT;
  string _BR;
  string _SR;
  

};


#endif
