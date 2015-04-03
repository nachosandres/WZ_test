#ifndef SSDLBoosted_HH
#define SSDLBoosted_HH

#include "analysis/core/MPAF.hh"

class SSDLBoosted: public MPAF {

public:

  // Member Functions

  SSDLBoosted(std::string);
  virtual ~SSDLBoosted();


private:

  void initialize();

  void run();
  void defineOutput();
  void modifyWeight();
  void writeOutput();

  void modifySkimming();

  void fillObjLists();
  int genMatchId(const Candidate* cand);
  bool goodJetSelection(int jetIdx);
  bool bJetSelection(int jetIdx);
  int eventCharge();
  Candidate* closestObj(const Candidate* ref, 
			const CandList objs, int& idx);
  Candidate* matchSubJetLep(const Candidate* ref, 
			    const CandList fatJets,
			    const CandList jets);


private: 

  //counter categories, 0 is ALWAYS global (even if not specified later
  enum {kGlobal=0, kElId, kElVeto, kMuId, kMuVeto, kJetId, kBJetId, kVetoLepSel};
	
  std::vector<int> _elIdx;
  std::vector<int> _muIdx;
  std::vector<int> _lepIdx;

  int _nEls;
  int _nVEls;
  int _nMus;
  int _nVMus;
  int _nJets;
  int _nBJets;
  int _nFJets;

  CandList _leps;
  CandList _els;
  CandList _vEls;
  CandList _mus;
  CandList _vMus;
  CandList _jets;
  CandList _bJets;
  CandList _fJets;
  Candidate* _met;
  CandList _gTops;
  
  float _HT;


  bool _mvaId;
  string _bvar;
  string _lepflav;
  
};


#endif
