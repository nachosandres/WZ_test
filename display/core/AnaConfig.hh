#ifndef _AnaConfig_
#define _AnaConfig_


#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <map>
#include <math.h>
#include <sstream>

#include "tools/src/Dataset.hh"
#include "tools/src/DataBaseManager.hh"

using namespace std;

class AnaConfig {

private:
  
  string _dir;
  string _path;
  
  string _rootFile;
  vector<string> _statFileList;
  string _hname;

  vector<string> _samplenames;
  vector<string> _dsnames;

  map<int, string > _numDS;
  map<int, string >::iterator _itNDS;
  map<string, Dataset* > _datasets;
  map<string, Dataset* >::const_iterator _itDs;
  
  
  map<string,float> _kFactors;
  map<string,float> _xSecLumis;
  map<string,float>::iterator _itKF;
  map<string,float>::iterator _itXS;
  
  bool _useXSect;

  double _lumi;
  bool _runFilter;
  int _runNum;

  vector<string> _ddCuts;
  vector<pair<string, float> > _csData;

  bool _noData;

  size_t _testNMax;

  bool _skiptree;

  DataBaseManager* _dbm;

public:

  AnaConfig();
  ~AnaConfig();

  void reset();

  void configureLumi(map<string,float> Lum ,
		     map<string,float> Kfac,
		     float l,bool useXS=true );
  void configureData(bool runfilter, int runnum,bool MCOnly);
  void configureNames(string dir, string objName, string objList);

  void loadXSDB(string dbname);
  void loadKFDB(string dbname);

  string getDir() {return _dir;};
  vector<string> getObjList() {return _statFileList;};
  float getLumi() const {return _lumi;};
  vector<string> getSampleNames() {return _samplenames;};
  vector<string> getDsNames() {return _dsnames;};

  // float getWeight(int channel, int evt);
  // float getWeight(string chan, int evt);

  // string getSample(int channel, int evt);
  // string getSample(string chan, int evt);
  
  vector<string > getDSNames();

  vector<string> getDDCuts(){return _ddCuts;};

  //vector<int> getDDDSStatus();  
  vector<pair<string, float> > getCSData();

  Dataset* findDS(string channel);
  Dataset* findDS(string channel, string crName);
  string findDS(int channel);
  string findDSName(string channel);
  string findDSName(string channel, string crName);
  int findChan(string ds);

  void addSample(string str, string sname, int col, bool loadH=true );

  bool passRunFilter(int run);

  Dataset* getDataset(string name);
  // TTree* getTree(string name) {return _datasets[ name ]->getTree();};
  // int getNEvents(string name) {return _datasets[ name ]->getNEvents();};
  
  bool isNoDataAna();

  string getHName();
  
  //void setNMax(size_t testNMax);

  void isHistoAnalysis();
  


private:

  vector<string> listFiles(string dir, string files);
  
  SampleId parseSampleId(string str);


};

#endif
