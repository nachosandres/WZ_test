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
	Int_t           Run;
	Int_t           Lumi;
	Int_t           Event;
	Int_t           HLT_MU17_MU8;
	Int_t           HLT_MU17_MU8_PS;
	Int_t           HLT_MU17_TKMU8;
	Int_t           HLT_MU17_TKMU8_PS;
	Int_t           HLT_ELE17_ELE8_TIGHT;
	Int_t           HLT_ELE17_ELE8_TIGHT_PS;
	Int_t           HLT_MU8_ELE17_TIGHT;
	Int_t           HLT_MU8_ELE17_TIGHT_PS;
	Int_t           HLT_MU17_ELE8_TIGHT;
	Int_t           HLT_MU17_ELE8_TIGHT_PS;
	Int_t           HLT_MU8;
	Int_t           HLT_MU8_PS;
	Int_t           HLT_MU17;
	Int_t           HLT_MU17_PS;
	Int_t           HLT_MU5;
	Int_t           HLT_MU5_PS;
	Int_t           HLT_MU12;
	Int_t           HLT_MU12_PS;
	Int_t           HLT_MU24;
	Int_t           HLT_MU24_PS;
	Int_t           HLT_MU40;
	Int_t           HLT_MU40_PS;
	Int_t           HLT_ELE17_TIGHT;
	Int_t           HLT_ELE17_TIGHT_PS;
	Int_t           HLT_ELE17_JET30_TIGHT;
	Int_t           HLT_ELE17_JET30_TIGHT_PS;
	Int_t           HLT_ELE8_TIGHT;
	Int_t           HLT_ELE8_TIGHT_PS;
	Int_t           HLT_ELE8_JET30_TIGHT;
	Int_t           HLT_ELE8_JET30_TIGHT_PS;
	Int_t           NVrtx;
	Int_t           NTrue;
	Float_t         PUWeight;
	Float_t         PUWeightUp;
	Float_t         PUWeightDn;
	Float_t         GenWeight;
	std::vector<float>   *MuPt;
	std::vector<float>   *MuEta;
	std::vector<float>   *MuPhi;
	std::vector<int>     *MuCharge;
	std::vector<float>   *MuPFIso;
	std::vector<float>   *MuD0;
	std::vector<int>     *MuIsGlobalMuon;
	std::vector<int>     *MuIsPFMuon;
	std::vector<float>   *MuNChi2;
	//std::vector<int>     *MuNGlMuHits;
	std::vector<int>     *MuNMatchedStations;
	std::vector<float>   *MuDz;
	//std::vector<int>     *MuNPxHits;
	std::vector<int>     *MuNSiLayers;
	std::vector<float>   *MuD0BS;
	std::vector<float>   *MuIso03SumPt;
	std::vector<float>   *MuIso03EmPt;
	std::vector<float>   *MuIso03HadPt;
	std::vector<bool>    *MuIsVeto;
	std::vector<bool>    *MuIsLoose;
	std::vector<bool>    *MuIsTight;
	std::vector<bool>    *MuIsPrompt;
	std::vector<int>     *MuID;
	std::vector<int>     *MuMID;
	std::vector<int>     *MuGMID;
	std::vector<float>   *ElPt;
	std::vector<float>   *PhPt;
	std::vector<float>   *TauPt;
	std::vector<float>   *ElEta;
	std::vector<float>   *ElPhi;
	std::vector<int>     *ElCharge;
	std::vector<float>   *ElPFIso;
	std::vector<float>   *ElD0;
	std::vector<float>   *ElChCo;
	std::vector<bool>    *ElIsVeto;
	std::vector<bool>    *ElIsLoose;
	std::vector<bool>    *ElIsTight;
	std::vector<bool>    *ElIsPrompt;
	std::vector<int>     *ElID;
	std::vector<int>     *ElMID;
	std::vector<int>     *ElGMID;
	Float_t         pfMET;
	Float_t         pfMETPhi;
	Float_t         pfMET1;
	Float_t         pfMET1Phi;
	std::vector<float>   *JetPt;
	std::vector<float>   *JetRawPt;
	std::vector<float>   *JetEta;
	std::vector<float>   *JetPhi;
	std::vector<float>   *JetEnergy;
	std::vector<float>   *JetCSVBTag;
	std::vector<int>     *JetPartonFlav;
	std::vector<float>   *JetBetaStar;
	
	// List of branches
	TBranch        *b_Run;   //!
	TBranch        *b_Lumi;   //!
	TBranch        *b_Event;   //!
	TBranch        *b_HLT_MU17_MU8;   //!
	TBranch        *b_HLT_MU17_MU8_PS;   //!
	TBranch        *b_HLT_MU17_TKMU8;   //!
	TBranch        *b_HLT_MU17_TKMU8_PS;   //!
	TBranch        *b_HLT_ELE17_ELE8_TIGHT;   //!
	TBranch        *b_HLT_ELE17_ELE8_TIGHT_PS;   //!
	TBranch        *b_HLT_MU8_ELE17_TIGHT;   //!
	TBranch        *b_HLT_MU8_ELE17_TIGHT_PS;   //!
	TBranch        *b_HLT_MU17_ELE8_TIGHT;   //!
	TBranch        *b_HLT_MU17_ELE8_TIGHT_PS;   //!
	TBranch        *b_HLT_MU8;   //!
	TBranch        *b_HLT_MU8_PS;   //!
	TBranch        *b_HLT_MU17;   //!
	TBranch        *b_HLT_MU17_PS;   //!
	TBranch        *b_HLT_MU5;   //!
	TBranch        *b_HLT_MU5_PS;   //!
	TBranch        *b_HLT_MU12;   //!
	TBranch        *b_HLT_MU12_PS;   //!
	TBranch        *b_HLT_MU24;   //!
	TBranch        *b_HLT_MU24_PS;   //!
	TBranch        *b_HLT_MU40;   //!
	TBranch        *b_HLT_MU40_PS;   //!
	TBranch        *b_HLT_ELE17_TIGHT;   //!
	TBranch        *b_HLT_ELE17_TIGHT_PS;   //!
	TBranch        *b_HLT_ELE17_JET30_TIGHT;   //!
	TBranch        *b_HLT_ELE17_JET30_TIGHT_PS;   //!
	TBranch        *b_HLT_ELE8_TIGHT;   //!
	TBranch        *b_HLT_ELE8_TIGHT_PS;   //!
	TBranch        *b_HLT_ELE8_JET30_TIGHT;   //!
	TBranch        *b_HLT_ELE8_JET30_TIGHT_PS;   //!
	TBranch        *b_NVrtx;   //!
	TBranch        *b_NTrue;   //!
	TBranch        *b_PUWeight;   //!
	TBranch        *b_PUWeightUp;   //!
	TBranch        *b_PUWeightDn;   //!
	TBranch        *b_GenWeight;   //!
	TBranch        *b_MuPt;   //!
	TBranch        *b_MuEta;   //!
	TBranch        *b_MuPhi;   //!
	TBranch        *b_MuCharge;   //!
	TBranch        *b_MuPFIso;   //!
	TBranch        *b_MuD0;   //!
	TBranch        *b_MuIsGlobalMuon;   //!
	TBranch        *b_MuIsPFMuon;   //!
	TBranch        *b_MuNChi2;   //!
	//TBranch        *b_MuNGlMuHits;   //!
	TBranch        *b_MuNMatchedStations;   //!
	TBranch        *b_MuDz;   //!
	//TBranch        *b_MuNPxHits;   //!
	TBranch        *b_MuNSiLayers;   //!
	TBranch        *b_MuD0BS;
	TBranch        *b_MuIso03SumPt;
	TBranch        *b_MuIso03EmPt;
	TBranch        *b_MuIso03HadPt;
	TBranch        *b_MuIsVeto;   //!
	TBranch        *b_MuIsLoose;   //!
	TBranch        *b_MuIsTight;   //!
	TBranch        *b_MuIsPrompt;   //!
	TBranch        *b_MuID;   //!
	TBranch        *b_MuMID;   //!
	TBranch        *b_MuGMID;   //!
	TBranch        *b_ElPt;   //!
	TBranch        *b_PhPt;   //!
	TBranch        *b_TauPt;   //!
	TBranch        *b_ElEta;   //!
	TBranch        *b_ElPhi;   //!
	TBranch        *b_ElCharge;   //!
	TBranch        *b_ElPFIso;   //!
	TBranch        *b_ElD0;   //!
	TBranch        *b_ElChCo;   //!
	TBranch        *b_ElIsVeto;   //!
	TBranch        *b_ElIsLoose;   //!
	TBranch        *b_ElIsTight;   //!
	TBranch        *b_ElIsPrompt;   //!
	TBranch        *b_ElID;   //!
	TBranch        *b_ElMID;   //!
	TBranch        *b_ElGMID;   //!
	TBranch        *b_pfMET;   //!
	TBranch        *b_pfMETPhi;   //!
	TBranch        *b_pfMET1;   //!
	TBranch        *b_pfMET1Phi;   //!
	TBranch        *b_JetPt;   //!
	TBranch        *b_JetRawPt;   //!
	TBranch        *b_JetEta;   //!
	TBranch        *b_JetPhi;   //!
	TBranch        *b_JetEnergy;   //!
	TBranch        *b_JetCSVBTag;   //!
	TBranch        *b_JetPartonFlav;   //!
	TBranch        *b_JetBetaStar;   //!

};

#endif



