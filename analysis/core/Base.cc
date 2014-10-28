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

// this is basically the code taken from FWBaseClass.h adapted to the 
// Multi-Purpose Analysis Framework


#include "src/core/Base.hh"


using namespace std;



/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR RUNNING THE CODE                                       **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
Base::Base(TTree *tree){
	/* 
	constructs the Base Class 
	parameters: tree (ROOT TTree to be read)
	return: none
	*/

	Initialize(tree);

}


//____________________________________________________________________________
Base::~Base(){
	/* 
	destructs the Base Class
	parameters: none
	return: none
	*/

	if (!fChain) return;
	delete fChain->GetCurrentFile();

}


//____________________________________________________________________________
void Base::Initialize(TTree *tree){
	/*
	initializes the Base class and sets the branch addresses
	parameters: none
	return: none
	*/

    gettimeofday(&kStartTimeVal , NULL);
	kStartTime = clock();


   MuPt = NULL;
   MuEta = 0;
   MuPhi = 0;
   MuCharge = 0;
   MuPFIso = 0;
   MuD0 = 0;
   MuD0BS = 0;
   MuIso03SumPt = 0;
   MuIso03EmPt = 0;
   MuIso03HadPt = 0;
   MuIsGlobalMuon = 0;
   MuIsPFMuon = 0;
   MuNChi2 = 0;
   //MuNGlMuHits = 0;
   MuNMatchedStations = 0;
   MuDz = 0;
   //MuNPxHits = 0;
   MuNSiLayers = 0;
   MuIsVeto = 0;
   MuIsLoose = 0;
   MuIsTight = 0;
   MuIsPrompt = 0;
   MuID = 0;
   MuMID = 0;
   MuGMID = 0;
   //ElPt = 0;
   ElEta = 0;
   ElPhi = 0;
   ElCharge = 0;
   ElPFIso = 0;
   ElD0 = 0;
   ElChCo = 0;
   ElIsVeto = 0;
   ElIsLoose = 0;
   ElIsTight = 0;
   ElIsPrompt = 0;
   ElID = 0;
   ElMID = 0;
   ElGMID = 0;
   JetPt = 0;
   JetRawPt = 0;
   JetEta = 0;
   JetPhi = 0;
   JetEnergy = 0;
   JetCSVBTag = 0;
   JetPartonFlav = 0;
   JetBetaStar = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Run", &Run, &b_Run);
   fChain->SetBranchAddress("Lumi", &Lumi, &b_Lumi);
   fChain->SetBranchAddress("Event", &Event, &b_Event);
   fChain->SetBranchAddress("HLT_MU17_MU8", &HLT_MU17_MU8, &b_HLT_MU17_MU8);
   fChain->SetBranchAddress("HLT_MU17_MU8_PS", &HLT_MU17_MU8_PS, &b_HLT_MU17_MU8_PS);
   fChain->SetBranchAddress("HLT_MU17_TKMU8", &HLT_MU17_TKMU8, &b_HLT_MU17_TKMU8);
   fChain->SetBranchAddress("HLT_MU17_TKMU8_PS", &HLT_MU17_TKMU8_PS, &b_HLT_MU17_TKMU8_PS);
   fChain->SetBranchAddress("HLT_ELE17_ELE8_TIGHT", &HLT_ELE17_ELE8_TIGHT, &b_HLT_ELE17_ELE8_TIGHT);
   fChain->SetBranchAddress("HLT_ELE17_ELE8_TIGHT_PS", &HLT_ELE17_ELE8_TIGHT_PS, &b_HLT_ELE17_ELE8_TIGHT_PS);
   fChain->SetBranchAddress("HLT_MU8_ELE17_TIGHT", &HLT_MU8_ELE17_TIGHT, &b_HLT_MU8_ELE17_TIGHT);
   fChain->SetBranchAddress("HLT_MU8_ELE17_TIGHT_PS", &HLT_MU8_ELE17_TIGHT_PS, &b_HLT_MU8_ELE17_TIGHT_PS);
   fChain->SetBranchAddress("HLT_MU17_ELE8_TIGHT", &HLT_MU17_ELE8_TIGHT, &b_HLT_MU17_ELE8_TIGHT);
   fChain->SetBranchAddress("HLT_MU17_ELE8_TIGHT_PS", &HLT_MU17_ELE8_TIGHT_PS, &b_HLT_MU17_ELE8_TIGHT_PS);
   fChain->SetBranchAddress("HLT_MU8", &HLT_MU8, &b_HLT_MU8);
   fChain->SetBranchAddress("HLT_MU8_PS", &HLT_MU8_PS, &b_HLT_MU8_PS);
   fChain->SetBranchAddress("HLT_MU17", &HLT_MU17, &b_HLT_MU17);
   fChain->SetBranchAddress("HLT_MU17_PS", &HLT_MU17_PS, &b_HLT_MU17_PS);
   fChain->SetBranchAddress("HLT_MU5", &HLT_MU5, &b_HLT_MU5);
   fChain->SetBranchAddress("HLT_MU5_PS", &HLT_MU5_PS, &b_HLT_MU5_PS);
   fChain->SetBranchAddress("HLT_MU12", &HLT_MU12, &b_HLT_MU12);
   fChain->SetBranchAddress("HLT_MU12_PS", &HLT_MU12_PS, &b_HLT_MU12_PS);
   fChain->SetBranchAddress("HLT_MU24", &HLT_MU24, &b_HLT_MU24);
   fChain->SetBranchAddress("HLT_MU24_PS", &HLT_MU24_PS, &b_HLT_MU24_PS);
   fChain->SetBranchAddress("HLT_MU40", &HLT_MU40, &b_HLT_MU40);
   fChain->SetBranchAddress("HLT_MU40_PS", &HLT_MU40_PS, &b_HLT_MU40_PS);
   fChain->SetBranchAddress("HLT_ELE17_TIGHT", &HLT_ELE17_TIGHT, &b_HLT_ELE17_TIGHT);
   fChain->SetBranchAddress("HLT_ELE17_TIGHT_PS", &HLT_ELE17_TIGHT_PS, &b_HLT_ELE17_TIGHT_PS);
   fChain->SetBranchAddress("HLT_ELE17_JET30_TIGHT", &HLT_ELE17_JET30_TIGHT, &b_HLT_ELE17_JET30_TIGHT);
   fChain->SetBranchAddress("HLT_ELE17_JET30_TIGHT_PS", &HLT_ELE17_JET30_TIGHT_PS, &b_HLT_ELE17_JET30_TIGHT_PS);
   fChain->SetBranchAddress("HLT_ELE8_TIGHT", &HLT_ELE8_TIGHT, &b_HLT_ELE8_TIGHT);
   fChain->SetBranchAddress("HLT_ELE8_TIGHT_PS", &HLT_ELE8_TIGHT_PS, &b_HLT_ELE8_TIGHT_PS);
   fChain->SetBranchAddress("HLT_ELE8_JET30_TIGHT", &HLT_ELE8_JET30_TIGHT, &b_HLT_ELE8_JET30_TIGHT);
   fChain->SetBranchAddress("HLT_ELE8_JET30_TIGHT_PS", &HLT_ELE8_JET30_TIGHT_PS, &b_HLT_ELE8_JET30_TIGHT_PS);
   fChain->SetBranchAddress("NVrtx", &NVrtx, &b_NVrtx);
   fChain->SetBranchAddress("NTrue", &NTrue, &b_NTrue);
   fChain->SetBranchAddress("PUWeight", &PUWeight, &b_PUWeight);
   fChain->SetBranchAddress("PUWeightUp", &PUWeightUp, &b_PUWeightUp);
   fChain->SetBranchAddress("PUWeightDn", &PUWeightDn, &b_PUWeightDn);
   fChain->SetBranchAddress("GenWeight", &GenWeight, &b_GenWeight);
   fChain->SetBranchAddress("MuPt", &MuPt, &b_MuPt);
   fChain->SetBranchAddress("MuEta", &MuEta, &b_MuEta);
   fChain->SetBranchAddress("MuPhi", &MuPhi, &b_MuPhi);
   fChain->SetBranchAddress("MuCharge", &MuCharge, &b_MuCharge);
   fChain->SetBranchAddress("MuPFIso", &MuPFIso, &b_MuPFIso);
   fChain->SetBranchAddress("MuD0", &MuD0, &b_MuD0);
   fChain->SetBranchAddress("MuIsGlobalMuon", &MuIsGlobalMuon, &b_MuIsGlobalMuon);
   fChain->SetBranchAddress("MuIsPFMuon", &MuIsPFMuon, &b_MuIsPFMuon);
   fChain->SetBranchAddress("MuNChi2", &MuNChi2, &b_MuNChi2);
   //fChain->SetBranchAddress("MuNGlMuHits", &MuNGlMuHits, &b_MuNGlMuHits);
   fChain->SetBranchAddress("MuNMatchedStations", &MuNMatchedStations, &b_MuNMatchedStations);
   fChain->SetBranchAddress("MuDz", &MuDz, &b_MuDz);
   //fChain->SetBranchAddress("MuNPxHits", &MuNPxHits, &b_MuNPxHits);
   fChain->SetBranchAddress("MuNSiLayers", &MuNSiLayers, &b_MuNSiLayers);
   fChain->SetBranchAddress("MuD0BS", &MuD0BS , &b_MuD0BS);
   fChain->SetBranchAddress("MuIso03SumPt", &MuIso03SumPt , &b_MuIso03SumPt);
   fChain->SetBranchAddress("MuIso03EmPt", &MuIso03EmPt , &b_MuIso03EmPt);
   fChain->SetBranchAddress("MuIso03HadPt", &MuIso03HadPt , &b_MuIso03HadPt);
   fChain->SetBranchAddress("MuIsVeto", &MuIsVeto, &b_MuIsVeto);
   fChain->SetBranchAddress("MuIsLoose", &MuIsLoose, &b_MuIsLoose);
   fChain->SetBranchAddress("MuIsTight", &MuIsTight, &b_MuIsTight);
   fChain->SetBranchAddress("MuIsPrompt", &MuIsPrompt, &b_MuIsPrompt);
   fChain->SetBranchAddress("MuID", &MuID, &b_MuID);
   fChain->SetBranchAddress("MuMID", &MuMID, &b_MuMID);
   fChain->SetBranchAddress("MuGMID", &MuGMID, &b_MuGMID);
   fChain->SetBranchAddress("ElPt", &ElPt, &b_ElPt);
   fChain->SetBranchAddress("ElEta", &ElEta, &b_ElEta);
   fChain->SetBranchAddress("ElPhi", &ElPhi, &b_ElPhi);
   fChain->SetBranchAddress("ElCharge", &ElCharge, &b_ElCharge);
   fChain->SetBranchAddress("ElPFIso", &ElPFIso, &b_ElPFIso);
   fChain->SetBranchAddress("ElD0", &ElD0, &b_ElD0);
   fChain->SetBranchAddress("ElChCo", &ElChCo, &b_ElChCo);
   fChain->SetBranchAddress("ElIsVeto", &ElIsVeto, &b_ElIsVeto);
   fChain->SetBranchAddress("ElIsLoose", &ElIsLoose, &b_ElIsLoose);
   fChain->SetBranchAddress("ElIsTight", &ElIsTight, &b_ElIsTight);
   fChain->SetBranchAddress("ElIsPrompt", &ElIsPrompt, &b_ElIsPrompt);
   fChain->SetBranchAddress("ElID", &ElID, &b_ElID);
   fChain->SetBranchAddress("ElMID", &ElMID, &b_ElMID);
   fChain->SetBranchAddress("ElGMID", &ElGMID, &b_ElGMID);
   fChain->SetBranchAddress("pfMET", &pfMET, &b_pfMET);
   fChain->SetBranchAddress("pfMETPhi", &pfMETPhi, &b_pfMETPhi);
   fChain->SetBranchAddress("pfMET1", &pfMET1, &b_pfMET1);
   fChain->SetBranchAddress("pfMET1Phi", &pfMET1Phi, &b_pfMET1Phi);
   fChain->SetBranchAddress("JetPt", &JetPt, &b_JetPt);
   fChain->SetBranchAddress("JetRawPt", &JetRawPt, &b_JetRawPt);
   fChain->SetBranchAddress("JetEta", &JetEta, &b_JetEta);
   fChain->SetBranchAddress("JetPhi", &JetPhi, &b_JetPhi);
   fChain->SetBranchAddress("JetEnergy", &JetEnergy, &b_JetEnergy);
   fChain->SetBranchAddress("JetCSVBTag", &JetCSVBTag, &b_JetCSVBTag);
   fChain->SetBranchAddress("JetPartonFlav", &JetPartonFlav, &b_JetPartonFlav);
   fChain->SetBranchAddress("JetBetaStar", &JetBetaStar, &b_JetBetaStar);
   Notify();

}


//____________________________________________________________________________
Int_t Base::Cut(Long64_t entry){
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.

   return 1;
}


//____________________________________________________________________________
Int_t Base::GetEntry(Long64_t entry){
	/*
	read contents of an entry in the TTree
	parameters:
	return: 
	*/

	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}


//____________________________________________________________________________
time_t Base::GetTimeDifference(){
	/*
	returns the seconds since intialization of the class
	parameters: none
	return: the time
	*/

    
    
    

	return clock() - kStartTime;
}


//____________________________________________________________________________
int Base::GetTimeDifferenceMS(){
	/*
	returns the miliseconds since intialization of the class
	parameters: none
	return: the miliseconds
	*/

	struct timeval now;

	gettimeofday(&now, NULL);

    double now_ms   = (double) now.tv_sec           * 1000000 + (double) now.tv_usec;
    double start_ms = (double) kStartTimeVal.tv_sec * 1000000 + (double) kStartTimeVal.tv_usec;

	double result = (now_ms - start_ms) / 10;
    
    return (int) result;

}


//____________________________________________________________________________
Long64_t Base::LoadTree(Long64_t entry){
	/*
	set environment to read one entry in the TTree
	parameters:
	return: 
	*/

	if (!fChain) return -5;
	Long64_t centry = fChain->LoadTree(entry);
	if (centry < 0) return centry;

	if (fChain->GetTreeNumber() != fCurrent) {
		fCurrent = fChain->GetTreeNumber();
		Notify();
	}
	return centry;
}


//____________________________________________________________________________
void Base::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L FWBaseClass.C
//      Root > FWBaseClass t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}


//____________________________________________________________________________
Bool_t Base::Notify(){


   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

	return kTRUE;
}


//____________________________________________________________________________
void Base::Show(Long64_t entry){

// Print contents of entry.
// If entry is not specified, print current entry

	if (!fChain) return;
	fChain->Show(entry);
}









