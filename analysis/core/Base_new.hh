/*****************************************************************************
******************************************************************************
******************************************************************************
**                                                                          **
** The Dileptons Analysis Framework                                         **
**                                                                          **
** Constantin Heidegger, CERN, Summer 2014                                  **
**                                                                          **
******************************************************************************
******************************************************************************
*****************************************************************************/

// this is basically the code taken from FWBaseClass.h adapted to the 
// Dileptons Analysis Framework

#ifndef BASE_HH 
#define BASE_HH 

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <time.h>
#include <sys/time.h>


class Base {

public :

	Base(TTree *tree=0);
	virtual ~Base();
	virtual void     Initialize(TTree *tree);
	virtual Int_t    Cut(Long64_t entry);
	time_t GetTimeDifference();
	int GetTimeDifferenceMS();
	virtual Int_t    GetEntry(Long64_t entry);
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Loop();
	virtual Bool_t   Notify();
	virtual void     Show(Long64_t entry = -1);

	time_t kStartTime;
	struct timeval kStartTimeVal;

	TTree          *fChain;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrent; //!current Tree number in a TChain
	
	// Declaration of leaf types
	Int_t           run;
	Int_t           lumi;
	Int_t           event;
	Int_t           HLT_SingleMu;
	Int_t           HLT_MuEG;
	Int_t           HLT_TripleEl;
	Int_t           HLT_DoubleEl;
	Int_t           HLT_DoubleMu;
	Int_t           nVert;
	Int_t           nTrueInt;
	Float_t         puWeight;
	Float_t         genWeight;
	std::vector<float>   *el_pt;
	std::vector<float>   *el_eta;
	std::vector<float>   *el_phi;
	std::vector<int>     *el_charge;
	std::vector<float>   *el_relIso03;
	std::vector<float>   *el_relIso04;
	std::vector<float>   *el_dxy;
	std::vector<float>   *el_dz;
	std::vector<int>     *el_tightId;
	std::vector<int>     *el_looseIdSusy;
	std::vector<float>   *mu_pt;
	std::vector<float>   *mu_eta;
	std::vector<float>   *mu_phi;
	std::vector<int>     *mu_charge;
	std::vector<float>   *mu_relIso03;
	std::vector<float>   *mu_relIso04;
	std::vector<float>   *mu_dxy;
	std::vector<float>   *mu_dz;
	std::vector<int>     *mu_tightId;
	std::vector<int>     *mu_looseIdSusy;
	Float_t         met_pt;
	Float_t         met_eta;
	Float_t         met_phi;
	Float_t         met_mass;
	std::vector<float>   *jet_pt;
	std::vector<float>   *jet_rawPt;
	std::vector<float>   *jet_eta;
	std::vector<float>   *jet_phi;
	std::vector<float>   *jet_mass;
	std::vector<float>   *jet_btagCSV;
	
	// List of branches
	TBranch        *b_run;   //!
	TBranch        *b_lumi;   //!
	TBranch        *b_event;   //!
	TBranch        *b_HLT_SingleMu;
	TBranch        *b_HLT_MuEG;
	TBranch        *b_HLT_TripleEl;
	TBranch        *b_HLT_DoubleEl;
	TBranch        *b_HLT_DoubleMu;
	TBranch        *b_nVert;   //!
	TBranch        *b_nTrueInt;   //!
	TBranch        *b_puWeight;   //!
	TBranch        *b_genWeight;   //!
	TBranch        *b_mu_pt;
	TBranch        *b_mu_eta;
	TBranch        *b_mu_phi;
	TBranch        *b_mu_charge;
	TBranch        *b_mu_relIso03;
	TBranch        *b_mu_relIso04;
	TBranch        *b_mu_dxy;
	TBranch        *b_mu_dz;
	TBranch        *b_mu_tightId;
	TBranch        *b_mu_looseIdSusy;
	TBranch        *b_el_pt;
	TBranch        *b_el_eta;
	TBranch        *b_el_phi;
	TBranch        *b_el_charge;
	TBranch        *b_el_relIso03;
	TBranch        *b_el_relIso04;
	TBranch        *b_el_dxy;
	TBranch        *b_el_dz;
	TBranch        *b_el_tightId;
	TBranch        *b_el_looseIdSusy;
	TBranch        *b_met_pt;
	TBranch        *b_met_eta;
	TBranch        *b_met_phi;
	TBranch        *b_met_mass;
	TBranch        *b_jet_pt;
	TBranch        *b_jet_rawPt;
	TBranch        *b_jet_eta;
	TBranch        *b_jet_phi;
	TBranch        *b_jet_mass;
	TBranch        *b_jet_btagCSV;

};

#endif



