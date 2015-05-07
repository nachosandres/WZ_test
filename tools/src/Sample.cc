#include "src/Sample.hh"
#include <iostream>

using namespace std;

ClassImp(Sample)

Sample::Sample() {
}

Sample::Sample(string name, int n, int nPE, float xSect,
	       float kfact, float eqLumi) {

  _name = name;
  _nEvents=n;
  _nProcEvents=nPE;
  _xSection=xSect;
  _kFactor=kfact;
  _eqLumi=eqLumi;

  _weight=1.;

  computeWeight();

}

Sample::~Sample() {
}


void
Sample::computeWeight() {
  //means that Xsection used for reweigthing instead of equivalentLumi
  // and means that the number of processed events is available
  if(_eqLumi==-1 && _nProcEvents!=-1) {
    _eqLumi = _nProcEvents/_xSection;
  }
  
  _weight =(1./_eqLumi)*_kFactor;
}
