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

#ifndef SUSYSSDL_HH
#define SUSYSSDL_HH

#include "analysis/core/MPAF.hh"




class SUSYSSDL: public MPAF {

public:


	// Member Functions

	SUSYSSDL(std::string);
	virtual ~SUSYSSDL();
	void initialize();

	void run();
	void defineOutput();
	void loadInput();
	void modifyWeight();
	void writeOutput();

	bool bJetSelection(int);
	bool electronSelection(int);
	bool goodJetSelection(int);
	bool muonSelection(int);
	bool vetoElectronSelection(int);
	bool vetoMuonSelection(int);

	bool brEventSelection();
	void collectKinematicObjects();
	bool vetoLeptonSelection(std::string, std::string, std::string = "");
	bool srEventSelection();
	bool ssLeptonSelection(std::string, std::string, std::string = "");

	void fillEventPlots(std::string);
	void fillLeptonPlots(std::string);
	void fillJetPlots(std::string);




private:

	std::string _OutputDir;
	TFile * _RootFile;
	
	TH1F * h_HT;
	TH1F * h_MET;
	TH1F * h_NBJets;
	TH1F * h_NElectrons;
	TH1F * h_NJets;
	TH1F * h_NLeps;
	TH1F * h_NMuons;
	TH1F * h_NVrtx;
	
	TH1F * h_ElD0;
	TH1F * h_ElEta;
	TH1F * h_ElIso;
	TH1F * h_ElMT;
	TH1F * h_ElPt;
	TH1F * h_MuD0;
	TH1F * h_MuEta;
	TH1F * h_MuIso;
	TH1F * h_MuMT;
	TH1F * h_MuPt;
	
	TH1F * h_JetCSVBTag;
	TH1F * h_JetPt;




	
};


#endif
