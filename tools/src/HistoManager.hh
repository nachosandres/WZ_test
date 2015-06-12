/*****************************************************************************
******************************************************************************
****************************************************************************** 
**                                                                          ** 
** The Multi-Purpose Analysis Framework                                     ** 
**                                                                          ** 
** Constantin Heidegger, Matthieu Marionneau                                ** 
**                                                                          ** 
****************************************************************************** 
****************************************************************************** 
*****************************************************************************/ 

#ifndef _HistoMan_
#define _HistoMan_

#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>

#include "TError.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH1.h"
#include "TH2F.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"

#include "tools/src/Format.cc"
#include "tools/src/HistoUtils.hh"


using namespace std;



class HistoManager {

private:
  
  //histograms
  map< string, hObs > _variables;
  map< string, hObs >::iterator _itVar; 
  map< string, hObs >::const_iterator _cItVar; 
  
  vector<string> _dsNames;
  size_t _nds;
  
  string _hname;
  
  //==============================================
  //Functions

public:

  //cosntructor & destructor

  HistoManager();
  virtual ~HistoManager();

  //initilaization
  void configAnalysis(vector<string> datasets);
  
  //reset
  void reset();

  //Histogram access and booking
  
  void addVariable(string var, int nBin, float min, float max, 
		   string Xleg, bool prof=false, string type="m");
  void addVariable(string var, int nBin, vector<float> bins,
		   string Xleg, bool prof=false, string type="m");
  void addVariable(string var, int nBinX, float minX, float maxX,
		   int nBinY, float minY,
		   float maxY, string Xleg,string Yleg,
		   bool prof=false, string type="m");
  void addVariable(string var, int nBinX, vector<float> binsX, int nBinY,
		   vector<float> binsY,
		   string Xleg,string Yleg,bool prof=false, string type="m");
  
  void addVariableFromTemplate(string var, TH1* h, bool prof, 
			       bool is2D, string type);

  void fill(string var, int ds, float val, float weight=1.);
  void fill(string var, int ds, float valx, float valy, float weight);
  void fill(string var, string type, float value, float weight=1.,string dir="");
  
  void copyHisto( string var, int ds, TH1* h);

  TH1* getHisto(string obs, int ds);
  const hObs* getHObs(string obs);
  
  vector<string> getObservables(bool allObs=false);
  
  //Prepare observables
  hObs preparehObs(string var, int nbinX, vector<float> bins, 
		   string Xleg,string Yleg,
		   string type, bool prof,
		   int nbinsY=-1, 
		   vector<float> binsY=vector<float>(0,0) );
   
  hObs preparehObsFromTemplate(string var, TH1* h, bool prof,
			       bool is2D, string type );

  //saving plots
  void savePlots(string path, const TCanvas* c, string advname);

  //systematic uncertainties
  vector<systM> getSystObs(string obs);
  systM findSysts(string var,string type);


  void saveHistos(string anName, string conName, std::map<std::string, int> cnts);

  ClassDef(HistoManager,0)
};

#endif
  
