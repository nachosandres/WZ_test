#ifndef _DBManager_
#define _DBManager_

#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "THnSparse.h"
#include "TFile.h"
#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TGraph.h"

#include "tools/src/StatUtils.hh"

class DataBaseManager {

public:

  DataBaseManager(); 
  ~DataBaseManager();

  void loadDb(std::string key, std::string dbName);
  void loadDb(std::string key, std::string dbName, std::string hname);
  float getDBValue(std::string key, float v1=-1000000, float v2=-1000000,
		   float v3=-1000000, float v4=-1000000, float v5=-1000000,
		   float v6=-1000000,float v7=-1000000, float v8=-1000000, 
		   float v9=-1000000, float v10=-1000000);

  float getDBErrL(std::string key, float v1=-1000000, float v2=-1000000,
		   float v3=-1000000, float v4=-1000000, float v5=-1000000,
		   float v6=-1000000,float v7=-1000000, float v8=-1000000, 
		   float v9=-1000000, float v10=-1000000);

  float getDBErrH(std::string key, float v1=-1000000, float v2=-1000000,
		   float v3=-1000000, float v4=-1000000, float v5=-1000000,
		   float v6=-1000000,float v7=-1000000, float v8=-1000000, 
		   float v9=-1000000, float v10=-1000000);

  std::vector<std::vector<float> > getDB(std::string key);

private:
  
  void readDb(std::string key, std::string dbName);
  void readDbHisto(std::string key, std::string dbName, std::string hname);

  std::map<std::string,std::vector<std::vector<float> > > _cDbLim;
  std::map<std::string,std::map<int, int* > > _cDbIdx;
  std::map<std::string, THnSparseF*> _mDBs;
  std::map<std::string, THnSparseF*> _mDBEHs;
  std::map<std::string, THnSparseF*> _mDBELs;

};

#endif
