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

#ifndef SYNCHECO_HH
#define SYNCHECO_HH

#include "analysis/core/MPAF.hh"

class synchECO: public MPAF {

public:


  // Member Functions

  synchECO(std::string);
  virtual ~synchECO();


private:

  void initialize();

  void run();
  void defineOutput();
  void modifyWeight();
  void writeOutput();

  void modifySkimming();

  void collectKinematicObjects();
  bool electronSelection(int);
  bool muonSelection(int);

  bool baseSelection();

private: 

  //counter categories, 0 is ALWAYS global (even if not specified later
  enum {kGlobal=0, kElId, kElVeto, kMuId, kMuVeto, kJetId, kBJetId, kVetoLepSel};

  enum {kNoGenMatch=0, kMisMatchPdgId,
	kMisChargePdgId, kGenMatched};

  float _valCutHTSR;
  float _valCutMETSR;
  float _valCutNJetsSR;
  float _valCutNBJetsSR;
  float _valCutCHSR;
  float _valCutHTBR;
  float _valCutHTCondBR;
  float _valCutMETLowBR;
  float _valCutMETHighBR;
  float _valCutNJetsBR;
  float _valCutNBJetsBR;
  float _valCutCHBR;

  std::string _cTypeHTSR;
  std::string _cTypeMETSR;
  std::string _cTypeNJetsSR;
  std::string _cTypeNBJetsSR;
  std::string _cTypeCHSR;
  std::string _cTypeHTBR;
  std::string _cTypeHTCondBR;
  std::string _cTypeMETLowBR;
  std::string _cTypeMETHighBR;
  std::string _cTypeNJetsBR;
  std::string _cTypeNBJetsBR;
  std::string _cTypeCHBR;

  float _upValCutHTSR;
  float _upValCutMETSR;
  float _upValCutNJetsSR;
  float _upValCutNBJetsSR;
  float _upValCutCHSR;
  float _upValCutHTBR;
  float _upValCutHTCondBR;
  float _upValCutMETLowBR;
  float _upValCutMETHighBR;
  float _upValCutNJetsBR;
  float _upValCutNBJetsBR;
  float _upValCutCHBR;
	
  std::vector<int> _elIdx;
  std::vector<int> _muIdx;

  int _nEls;
  int _nVEls;
  int _nMus;
  int _nVMus;
  int _nJets;
  int _nBJets;

  CandList _els;
  CandList _vEls;
  CandList _mus;
  CandList _vMus;
  CandList _jets;
  CandList _bJets;
  Candidate* _met;
  
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
