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







   MuPt = 0;
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

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("HLT_SingleMu", &HLT_SingleMu, &b_HLT_SingleMu);
   fChain->SetBranchAddress("HLT_MuEG", &HLT_MuEG, &b_HLT_MuEG);
   fChain->SetBranchAddress("HLT_TripleEl", &HLT_TripleEl, &b_HLT_TripleEl);
   fChain->SetBranchAddress("HLT_DoubleEl", &HLT_DoubleEl, &b_HLT_DoubleEl);
   fChain->SetBranchAddress("HLT_DoubleMu", &HLT_DoubleMu, &b_HLT_DoubleMu);
   fChain->SetBranchAddress("nVrtx", &nVrtx, &b_nVrtx);
   fChain->SetBranchAddress("nTrueInt", &nTrueInt, &b_nTrueInt);
   fChain->SetBranchAddress("puWeight", &puWeight, &b_puWeight);
   fChain->SetBranchAddress("genWeight", &genWeight, &b_genWeight);
   fChain->SetBranchAddress("el_pt", &el_pt, &b_el_pt);
   fChain->SetBranchAddress("el_eta", &el_eta, &b_el_eta);
   fChain->SetBranchAddress("el_phi", &el_phi, &b_el_phi);
   fChain->SetBranchAddress("el_charge", &el_charge, &b_el_charge);
   fChain->SetBranchAddress("el_relIso03", &el_relIso03, &b_el_relIso03);
   fChain->SetBranchAddress("el_relIso04", &el_relIso04, &b_el_relIso04);
   fChain->SetBranchAddress("el_dxy", &el_dxy, &b_el_dxy);
   fChain->SetBranchAddress("el_dz", &el_dz, &b_el_dz);
   fChain->SetBranchAddress("el_tightId", &el_tightId, &b_el_tightId);
   fChain->SetBranchAddress("el_looseIdSusy", &el_looseIdSusy, &b_el_looseIdSusy);


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









