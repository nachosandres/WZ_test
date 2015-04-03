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

#ifndef FakeRatio_HH
#define FakeRatio_HH

#include "analysis/core/MPAF.hh"

class FakeRatio: public MPAF {

public:


  // Member Functions

  FakeRatio(std::string);
  virtual ~FakeRatio();


private:

  void initialize();

  void run();
  void defineOutput();
  void modifyWeight();
  void writeOutput();

  void modifySkimming();

  void collectKinematicObjects();
  bool goodJetSelection(int);
  bool leptonCutId(int lepIdx, int cat, float iso, float dxy, string cbid, int id);
  bool leptonMvaId(int lepIdx, int cat, float iso, float dxy, bool tch = false);
  bool looseElectronSelection(int);
  bool looseMuonSelection(int);
  bool tightElectronSelection(int);
  bool tightMuonSelection(int);
  bool vetoElectronSelection(int);
  bool vetoMuonSelection(int);

  void setCut(std::string, float, std::string, float = 0); 
  void setMeasurementRegion();

  bool genMRSelection();
  bool mrSelection();
  bool skimSelection();

  bool bJetMatching(int);
  void fillEventPlots(std::string);
  void fillLeptonPlots(std::string);
  void fillJetPlots(std::string);
  void fillFakeRatioMaps(std::string);

  int genParticleOrigin(int mid, int gmid);
  int genMatchIsFake(const Candidate*);
  int genMatchCateg(const Candidate*, string matchhist);
  void fillMatchHist(string matchhist, int pdgid);

  float HT();
  int eventCharge();

private: 

  //counter categories, 0 is ALWAYS global (even if not specified later
  enum {kGlobal=0, kTElId, kLElId, kVElId, kTMuId, kLMuId, kVMuId, kJetId};

  enum {kNoGenMatch=0, kMisMatchPdgId,
	kMisChargePdgId, kGenMatched};

  float _valCutNBJetsMR;
  std::string _cTypeNBJetsMR;
  float _upValCutNBJetsMR;

  int _nLEls;
  int _nLNTEls;
  int _nTEls;
  int _nVEls;
  int _nLLeps;
  int _nLNTLeps;
  int _nTLeps;
  int _nVLeps;
  int _nLMus;
  int _nLNTMus;
  int _nTMus;
  int _nVMus;
  int _nJets;

  CandList _lEls;
  CandList _lntEls;
  CandList _tEls;
  CandList _vEls;
  CandList _lMus;
  CandList _lntMus;
  CandList _tMus;
  CandList _vMus;
  CandList _lLeps;
  CandList _lntLeps;
  CandList _tLeps;
  CandList _vLeps;
  CandList _jets;

  vector<int> _lElIdx;
  vector<int> _lntElIdx;
  vector<int> _tElIdx;
  vector<int> _vElIdx;
  vector<int> _lLepIdx;
  vector<int> _lntLepIdx;
  vector<int> _tLepIdx;
  vector<int> _vLepIdx;
  vector<int> _lMuIdx;
  vector<int> _lntMuIdx;
  vector<int> _tMuIdx;
  vector<int> _vMuIdx;


  Candidate * _met; 
  float _HT;

  bool _mvaId;
  string _bvar;
  string _lepflav;
  
  string _mva;
  string _btag;
  string _PT;
  string _MR; 

};


#endif
