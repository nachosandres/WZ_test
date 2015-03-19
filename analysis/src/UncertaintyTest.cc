#include "analysis/src/UncertaintyTest.hh"

#include <algorithm>

UncertaintyTest::UncertaintyTest(std::string cfg){
  
  startExecution(cfg);
  initialize();

}

UncertaintyTest::~UncertaintyTest(){
 
}

void
UncertaintyTest::initialize(){

 _vc->registerVar("nLepGood");
 _vc->registerVar("LepGood_pdgId");
 _vc->registerVar("LepGood_pt");
 _vc->registerVar("LepGood_eta");
 _vc->registerVar("LepGood_phi");
 _vc->registerVar("LepGood_mass");

}

void
UncertaintyTest::modifyWeight() {

}

void
UncertaintyTest::modifySkimming() {

}


void
UncertaintyTest::defineOutput() {

 string leps[2]={"l1","l2"};
 for(int il=0;il<2;il++) {
   _hm->addVariable(leps[il]+"Pt", 200, 0., 200.0,"p_{T}("+leps[il]+") [GeV]");
   // _hm->addVariable(leps[il]+"Eta", 60, -3.0, 3.0,"#eta("+leps[il]+") ");
   // _hm->addVariable(leps[il]+"Phi", 60, -3.1416, 3.1416,"#phi("+leps[il]+") ");
 }

}


void
UncertaintyTest::writeOutput() {
 
}


void
UncertaintyTest::run() {

counter("denominator");

 if(!makeCut( _vc->get("nLepGood")==2,"nLep") ) return;

 if(!makeCut( _vc->get("LepGood_pt")>20,"L1 > 20 GeV") ) return;

 fill("l1Pt", _vc->get("LepGood_pt") );
 fill("l2Pt", _vc->get("LepGood_pt",1) );

 //example of shape variation 
 // fillUnc("l1Pt", "LES", _vc->get("LepGood_pt",0)*0.90, 1., "Do");
 // fillUnc("l1Pt", "LES", _vc->get("LepGood_pt",0)*1.10, 1., "Up");

 //error on normalization (e.g scale factor uncertainties)
 fillUnc("l1Pt", "SFEff", _vc->get("LepGood_pt",0), 0.9, "Do");
 fillUnc("l1Pt", "SFEff", _vc->get("LepGood_pt",0), 1.1, "Up");
 // //can also be done that way if the central value is not already defined and we have everything under the hand
 // fillUnc("l1Pt", "SFOther", _vc->get("LepGood_pt",0), 1, 1.1, 0.9);

}
