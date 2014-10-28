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

#ifndef FAKERATIO_HH
#define FAKERATIO_HH

#include "analysis/core/MPAF.hh"




class FakeRatio: public MPAF {

public:


	// Member Functions

	FakeRatio(std::string);
	virtual ~FakeRatio();
	void initialize();

	void run();
	void defineOutput();
	void loadInput();
	void writeOutput();

	bool awayJetSelection(int);
	bool goodJetSelection(int);
	bool bJetSelection(int);
	bool looseElectronSelection(int);
	bool looseMuonSelection(int);
	bool tightElectronSelection(int);
	bool tightMuonSelection(int);
	bool vetoElectronSelection(int);
	bool vetoMuonSelection(int);

	void collectKinematicObjects();
	bool eventSelection();

	void fillFakeRatioMaps();




private:

	TFile * _RootFile;
	std::vector<float> bins_eta;
	std::vector<float> bins_pt;

	TH2F * h_MFR;
	TH2F * h_TM;
	TH2F * h_LM;


	
};


#endif
