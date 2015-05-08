#ifndef SusyModule_hh
#define SusyModule_hh

#include "analysis/core/VarClass.hh"
#include "analysis/tools/Candidate.hh"
#include "analysis/utils/KineUtils.hh"

class SusyModule {

public:

  SusyModule(VarClass* vc);
  ~SusyModule();

  
  bool elMvaSel(int elIdx, int wp) const;
  bool muIdSel(int idx) const;
  bool elIdSel(int idx) const;
  bool multiIsoSel(int idx, int wp) const;
  bool jetSel(int jetIdx) const;
  float HT(const CandList* jets);

  bool mllVetoSelection(const Candidate* l1, const Candidate* l2,
			const CandList* allLeps) const ;
  bool mllLMGVeto(const Candidate* cand, const Candidate* veto) const;
  bool mllZVeto(const Candidate* cand, const Candidate* veto) const;
  

  enum {kDenom=0,
	kLoose,
	kMedium,
	kTight,
	kNWPs};

  enum {kMiniIso=0,kPtRatio,kPtRel};
  enum {kEBC=0,kEBF,kEE};
  

private:

  void defineLeptonWPS();
  //const
  VarClass* _vc;

  vector<float> _sipWP;
  vector<float> _dxyWP;
  vector<float> _dzWP;
  vector<vector<float> > _elMvaIdWP;
  vector<float> _isoWP;
  vector<float> _miniIsoWP;
  vector<float> _ptRelWP;
  vector<vector<float> > _multiIsoWP;


};

#endif
