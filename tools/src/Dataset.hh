#ifndef _Dataset_
#define _Dataset_

#include <vector>
#include <stdlib.h>
#include <string>
#include <map>
#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TChain.h"
#include "TObject.h"
#include "TKey.h"

#include "tools/src/Sample.hh"

class Dataset {

private:

  std::string _name;
  bool _isData;
 
  std::vector<Sample> _samples;
  vector<pair<int,int> > _events;
  std::vector<float> _weights;
  std::vector<int> _nprocs;
	
  int _color;
	
  //for tChain part
  TChain* _chain;
	
  //for histogram part
  std::map<std::string, std::map<std::string, TH1*> > _histos;
	
  int _isFromCS;
  bool _isGhost;
	
  int _dsContentType;


public:

  enum {
    kTree = 0,
    kHisto
  };

  Dataset();
  Dataset(std::string name);
  Dataset(std::string name, int color);
	
  ~Dataset();
	
  void freeMemory();

  void config(std::string name, int color, int content);
	
  void setName(std::string name){ _name=name;};
  void setColor(int ncol){ _color=ncol;};
	
  void addSample(std::string sfullname, std::string path, std::string dir, 
		 std::string objName, std::string hname, float xSect,
		 float kFact, float lumi, float eqLumi);


  //access functions 
  std::string getName(){ return _name;};
  int getColor(){ return _color;};
	
  bool isDataset(std::string name){return _name==name;};
  bool isPPcolDataset(){ return _isData;};

  int hasSample(string sname);
	
  bool isTreeType() { return _dsContentType==kTree;};
  bool isHistoType() { return _dsContentType==kHisto;};
	
  //bool isDataDriven(){return _isDataDriven;};
  bool isFromCS(){return _isFromCS!=0;};
  int csCode(){return _isFromCS;};
	
  bool isGhost(){return _isGhost;};
	
  //std::string findProcess(int event);
	
  //float findWeight(int event);
	
  float getWeight(int is) {return _samples[is].getLumW(); };
  float getWeight(string sname);
	
  TTree* getTree() {return _chain;};
  int getNEvents() { return _chain->GetEntries(); };
	
  int getNProcEvent(int evt);
	
  //void setNMax(size_t nmax);
	
  std::vector<std::string> getSamples();
  std::vector<std::string> getObservables();
  TH1* getHisto( std::string varName, std::string sName);

private:

  void loadTree(std::string path, std::string dir, std::string sname, std::string objName);
  void loadHistos(std::string path, std::string dir, std::string sname);

  // float computeWeight(float nEvts, float xSect, float kFact,
  // 		      float lumi, float& eqLumi);
  
  int getNProcEvents(string path, string dir, 
   		     string sname, string hname);
  

  ClassDef(Dataset,0)
  
};

#endif
