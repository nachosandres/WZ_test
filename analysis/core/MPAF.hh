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

#include "TFile.h"
#include "TH1I.h"
#include "TTree.h"
#include "TStopwatch.h"

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
#include <boost/timer.hpp>
#include <boost/progress.hpp>

#include "analysis/core/VarClass.hh"
#include "analysis/utils/CustomTypes.cc"
#include "analysis/utils/Debug.cc"
#include "analysis/utils/KineUtils.hh"
#include "analysis/utils/Parser.hh"
#include "analysis/utils/Tools.hh"
#include "analysis/utils/Verbose.hh"


#include "analysis/tools/Candidate.hh"
#include "analysis/tools/CandFwd.hh"

#include "tools/src/DataBaseManager.hh"
#include "tools/src/Dataset.hh"
#include "tools/src/HistoManager.hh"
#include "tools/src/AnaUtils.hh"



class MPAF {


  // Public Non-Template Methods

public:

  MPAF();
  MPAF(string cfg);
  virtual ~MPAF();

  void checkResources();
  void checkConfiguration();
  void createOutputStructure();
  void endExecution();
  void loadConfigurationFile(std::string);
  //void setAllModules(std::vector<std::string>);
  void setConfigName(std::string);
  void startExecution(std::string);

  void analyze();	
  void fillEventList();
  void resetKinematicObjects();

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
 
  string getCfgVarS(string n);
  int getCfgVarI(string n);
  float getCfgVarF(string n);

  //counters and selection functions
  template <typename T> inline
  bool makeCut(T value, T valcut, string type, string cName, T seccut=0, int eCateg = AUtils::kGlobal) {
    return _au -> makeCut<T>(value, valcut, type, _inds
			     , cName, _weight, seccut, eCateg, false);
    //_SampleName+_SampleOption[_SampleName]
  };
  bool makeCut(bool decision, string cName, string type = "=", int eCateg = AUtils::kGlobal);
  void counter(string cName, int eCateg = AUtils::kGlobal);

  int mucounter;

  // virtual functions for the classes
  virtual void defineOutput()=0;
  virtual void modifyWeight()=0;
  virtual void run()=0;
  virtual void writeOutput()=0;
  
  //skimming functions
  virtual void modifySkimming()=0;
  template < typename T > void addSkimBranch(string name,T* val) {
    _skimTree->Branch( name.c_str(), val );
  };
  void fillSkimTree() { if(_skim) _skimTree->Fill();};


  // Private Non-Template Methods
	
private:

  void initialize();

  void initSkimming();
  void finalizeSkimming();

  void internalWriteOutput();

  // Protected Members

protected:

  bool _isData;
  TTree * _RootTree;
  VarClass * _vc;
  DataBaseManager* _dbm;
  HistoManager* _hm;
  AnaUtils* _au;

  MIPar _inputVars;

  size_t _ie;
  std::string _sampleName;
  float _weight;

  std::string _cfgName;

  // Private Members

private:
  
	
  int _inds;

  unsigned int _nEvtMax; 
  unsigned int _nSkip; 

  std::map<std::string, std::string> _sampleOption;
  
  //skimming variables
  TFile* _oFile;
  TTree* _skimTree;
  TH1I* _hnSkim;

  bool _skim;
  bool _fullSkim;

  std::vector <Dataset*> _datasets;

  // Configuration File Variables
  std::string _inputPath;
  std::string _className;

};


#endif
