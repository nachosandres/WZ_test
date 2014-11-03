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

#ifndef MPAF_HH
#define MPAF_HH

#include "TMath.h"
#include "TString.h"
#include "TObject.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TROOT.h"
#include "TVirtualPad.h"
#include "TLorentzVector.h"
#include "TPaveStats.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <map>
#include <time.h>

#include "analysis/core/VarClass.hh"
#include "analysis/helper/CustomTypes.cc"
#include "analysis/helper/Debug.cc"
#include "analysis/helper/Tools.hh"
#include "analysis/helper/Verbose.hh"

#include "tools/src/DataBaseManager.hh"
#include "tools/src/Dataset.hh"
#include "tools/src/HistoManager.hh"
#include "tools/src/AnaUtils.hh"

class MPAF {


	// Public Non-Template Methods

public:

	MPAF();
	virtual ~MPAF();

	void checkResources();
	void checkConfiguration();
	void createOutputStructure();
	void endExecution();
	void loadConfigurationFile(std::string);
	void setAllModules(std::vector<std::string>);
	void setConfigName(std::string);
	void startExecution(std::string);

	void analyze();	
	void fillEventList();
	void resetKinematicObjects();

	int findCharge(std::string, std::string);
	float findMLL(std::string, std::string);
	float HT(std::string);
	float MLL(std::string, int, std::string, int);
	float MT(std::string, int);




	// Protected Non-Template Methods

protected:


	// varclass functions
	void fill(string var, float valx, float weight = 1.);
	void fill(string var, float valx, float valy, float weight);
	void fillUnc(string var, string unc, float val, float weight, string dir = "");
	void fillUnc(string var, string unc, float val, float weight, float wup, float wdo);

	// database functions
	void loadDb(string key, string dbfile, string hname = "");
	float getDBVal(string db, float v1 = -100000, float v2 = -100000, float v3 = -100000, 
	                          float v4 = -100000, float v5 = -100000, float v6 = -100000, 
	                          float v7 = -100000, float v8 = -100000, float v9 = -100000, 
	                          float v10 = -100000);
	float getDBErrL(string db, float v1 = -100000, float v2 = -100000, float v3 = -100000,
	                           float v4 = -100000, float v5 = -100000, float v6 = -100000,
	                           float v7 = -100000, float v8 = -100000, float v9 = -100000, 
	                           float v10 = -100000);
	float getDBErrH(string db, float v1 = -100000, float v2 = -100000, float v3 = -100000,
	                           float v4 = -100000, float v5 = -100000, float v6 = -100000,
	                           float v7 = -100000, float v8 = -100000, float v9 = -100000,
	                           float v10 = -100000);
 
	//counters and selection functions
	template <typename T> inline
	bool makeCut(T value, T valcut, string type, string cName, T seccut=0, int eCateg = AUtils::kGlobal) {
	  return _au -> makeCut<T>(value, valcut, type, _inds
				   , cName, _EventWeight, seccut, eCateg, false);
	  //_SampleName+_SampleOption[_SampleName]
	};
  bool makeCut(bool decision, string cName, string type = "=", int eCateg = AUtils::kGlobal);
  void counter(string cName, int eCateg = AUtils::kGlobal);


	// virtual functions for the classes
	virtual void defineOutput();
	virtual void loadInput();
	virtual void modifyWeight();
	virtual void run();
	virtual void writeOutput();
  
  //skimming functions
  virtual void modifySkimming();
  template < typename T > void addSkimBranch(string name,T* val) {
    _skimTree->Branch( name.c_str(), val );
  };
  void fillSkimTree() { _skimTree->Fill();};


	// Private Non-Template Methods
	
private:

	void initialize();

  void initSkimming();
  void finalizeSkimming();


	// Public Members

public:


	std::string _lep;
	std::string _lepnum;
	std::string _jet;
	std::string _jetnum;


	// Configuration File Variables

	std::string _UserName;
	std::string _InputPath;
	std::string _OutputPath;
	std::string _AFSPath;
	VerbosityLevel _VerbosityLevel;
	RunMode _Mode;
	std::vector<std::string> _RunOn;
	std::vector<std::string> _Modules;
	float _Luminosity;
	int _JEC;
	bool _PUReweighting;
	std::string _SR;
	std::string _BR;
	std::string _PT;
	std::map <std::string, bool> _UsedCuts;
	std::map <std::string, float> _Cuts;
	std::vector <Dataset*> _Samples;



	// Other Member Variables

	std::string _ConfigName;
	std::vector<std::string> _AllModules;
	std::map<std::string, std::vector<int> > _KinObj;
	std::map<std::string, int> _NumKinObj;

	Long64_t _EntryIterator;
	std::string _SampleName;
	float _EventWeight;



	// Protected Members

protected:

	bool _isData;
	TTree * _RootTree;
	VarClass * _vc;
	Verbose * _Verbose;
	DataBaseManager* _dbm;
	HistoManager* _hm;
  AnaUtils* _au;

  string _option;

	// Private Members

private:
  
	
	int _inds;
	bool _TestNEvt;
	unsigned int _TestNEvtMax; 

        std::map<std::string, std::string> _SampleOption;
  
  //skimming variables
  TFile* _oFile;
  TTree* _skimTree;
  TH1I* _hnSkim;

  bool _skim;
  bool _fullSkim;


};


#endif
