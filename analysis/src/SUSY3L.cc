/*****************************************************************************
******************************************************************************
******************************************************************************
**                                                                          **
** The Multi-Purpose Analysis Framework                                     **
**                                                                          **
** Jan Hoss, ETHZ, 12/2014                                  **
**                                                                          **
******************************************************************************
******************************************************************************
*****************************************************************************/

#include "analysis/src/SUSY3L.hh"



/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR RUNNING THE CODE                                       **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
SUSY3L::SUSY3L(std::string cfg){
    /* 
        constructs the SUSY3L class 
        parameters: configuration_file
        return: none
    */
	
    _verbose->Class("SUSY3L");
    startExecution(cfg);
    initialize();
}


//____________________________________________________________________________
SUSY3L::~SUSY3L(){
    /* 
        destructs the SUSY3L class 
        parameters: none
        return: none
    */

}


//____________________________________________________________________________
void SUSY3L::initialize(){
    /*
        initializes the SUSY3L class
        parameters: none
        return: none
    */

    // define variables using _vc varibale class
    // _vc->registerVar("name"           , "type");
    _vc->registerVar("nLepGood"                 , "I" );    //number of leptons in event
    _vc->registerVar("LepGood_pdgId"            , "AI");    //identifier for leptons (11: electron, 13: muon)
    _vc->registerVar("LepGood_pt"               , "AF");    //pT of leptons
    _vc->registerVar("LepGood_eta"              , "AF");    //eta of leptons
    _vc->registerVar("LepGood_phi"              , "AF");    //phi of leptons
    _vc->registerVar("LepGood_charge"           , "AI");    //charge of lepton +1 or -1

    //additional counter categories
    _au->addCategory( kElId, "el Id");
    _au->addCategory( kElVeto, "veto El");
    _au->addCategory( kMuId, "muon Id");
    _au->addCategory( kMuVeto, "veto Mu");
              




}


//____________________________________________________________________________
void SUSY3L::modifyWeight() {
    /*
        modifies the event weight for every entry in the tree
        parameters: none
        return: none
    */ 
}


//____________________________________________________________________________
void SUSY3L::run(){

    // clear object category vectors from previous event
    _els.clear();
    _mus.clear();
    _elIdx.clear();
    _muIdx.clear();
    //does veto vector has to be cleared?
    //_vEls.clear();
    //_vMus.clear();
    
    // increment event counter, used as denominator for yield calculation
    counter("denominator");

    // collect event variables to be used for later selectons and calculations
    collectKinematicObjects();

	
}



/*****************************************************************************
******************************************************************************
** DEFINE INPUTS AND OUTPUTS                                                **
******************************************************************************
*****************************************************************************/

//____________________________________________________________________________
void SUSY3L::defineOutput(){
    /*
        defines and reserves all output that is produced in this class
        parameters: none
        return: none
    */
}


//____________________________________________________________________________
void SUSY3L::loadInput(){
    /*
        loads all input from the cache or from the database
        parameters: none
        return: none
    */


    // define function in MPAF for loading histograms, text files, histograms from database 
}





//____________________________________________________________________________
void SUSY3L::writeOutput(){
    /*
        writes all output of this class to the disk
        paramters: none
        return: none
    */

}


// if adding variables int he skimming tree is needed...
void SUSY3L::modifySkimming(){

}


/******************************************************************************
*******************************************************************************
*** OBJECT SELECTIONS                                                        **
*******************************************************************************
******************************************************************************/

void SUSY3L::collectKinematicObjects(){

    /*
        collects all kinematic objects needed in the code, i.e. applies all object selections
        parameters: none
        return: none
    */

    // loop over all nLepGood leptons in this event
    for(int i = 0; i < _vc->getI("nLepGood"); ++i){
        // check which of the nLepGood leptons are electrons, identifier 11
        if(std::abs(_vc->getI("LepGood_pdgId",i)) == 11){
            //differentiate electrons for electron selecton and veto electron selection
            if(electronSelection(i)) {
                //if electron passes electron selection, create electron candidate 
                //with respective kinematic variables and append it to _els vector
                _els.push_back( Candidate::create(_vc->getF("LepGood_pt", i),
                                                  _vc->getF("LepGood_eta", i),
                                                  _vc->getF("LepGood_phi", i),
                                                  _vc->getI("LepGood_pdgId", i),
                                                  _vc->getI("LepGood_charge", i),
                                                  0.0005) );    //electron mass
                _elIdx.push_back(i);
            }
            else {
                //if electron passes veto selection, create veto electron candidate
                //and append to _vEls vector
                if(vetoElectronSelection(i))  {
                    _vEls.push_back( Candidate::create(_vc->getF("LepGood_pt", i),
                                                       _vc->getF("LepGood_eta", i),
                                                       _vc->getF("LepGood_phi", i),
                                                       _vc->getI("LepGood_pdgId", i),
                                                       _vc->getI("LepGood_charge", i),
                                                       0.0005) );   //electron mass
                }
            }
        }
        
        // check which of the nLepGood leptons are muons, identifier 13
        else if(std::abs(_vc->getI("LepGood_pdgId",i)) == 13){
            //differentiate muons for muon selecton and veto muon selection
            if(muonSelection(i)) {
                _mus.push_back( Candidate::create(_vc->getF("LepGood_pt", i),
                                                  _vc->getF("LepGood_eta", i),
                                                  _vc->getF("LepGood_phi", i),
                                                  _vc->getI("LepGood_pdgId", i),
                                                  _vc->getI("LepGood_charge", i),
                                                  0.105) );     //muon mass
                _muIdx.push_back(i);
            }
            else {
                if(vetoMuonSelection(i))  {
                    _vMus.push_back( Candidate::create(_vc->getF("LepGood_pt", i),
                                                       _vc->getF("LepGood_eta", i),
                                                       _vc->getF("LepGood_phi", i),
                                                       _vc->getI("LepGood_pdgId", i),
                                                       _vc->getI("LepGood_charge", i),
                                                       0.105) );    //muon mass
                }
            }
        }
    }
}


//____________________________________________________________________________
bool SUSY3L::electronSelection(int elIdx){
    /*
        does the selection of electrons
        parameters: elIdx
        return: true (if the electron is accepted as an electron), false (else)
    */
    
    //count electron candidates
    counter("ElectronDenominator", kElId);

    //define cuts for electrons
    float pt_cut = 10.;
    float eta_cut = 2.4;
    float eta_veto_low = 1.442;
    float eta_veto_high = 1.566;
    //if(_PT == "highpt") pt_cut = 20.;
    
    //apply the cuts
    //makeCut(variable to cut on, cut value, direction of acception, name, 2nd cut value, ?)
    if(!makeCut<float>( _vc->getF("LepGood_pt", elIdx) , pt_cut, ">"  , "pt selection"    , 0    , kElId)) return false;
    if(!makeCut<float>( std::abs(_vc->getF("LepGood_eta", elIdx)), eta_cut  , "<"  , "eta selection"   , 0    , kElId)) return false;
    if(!makeCut<float>( std::abs(_vc->getF("LepGood_eta", elIdx)), eta_veto_low, "[!]", "eta selection veto"   , eta_veto_high, kElId)) return false;

    return true;
}


//____________________________________________________________________________
bool SUSY3L::muonSelection(int muIdx){
    /*
        does the selection of muons
        parameters: muIdx
        return: true (if the muon is accepted as a muon), false (else)
    */
    
    //count muon candidates
    counter("MuonDenominator", kMuId);

    //define cut values
    float pt_cut = 10.;
    //if(_PT == "highpt") pt_cut = 20.;
    float eta_cut = 2.4;

    if(!makeCut<float>( _vc->getF("LepGood_pt", muIdx), pt_cut, ">", "pt selection"    , 0, kMuId)) return false;
    if(!makeCut<float>( std::abs( _vc->getF("LepGood_eta", muIdx)), eta_cut, "<", "eta selection", 0, kMuId)) return false;

    return true;
}


//____________________________________________________________________________
bool SUSY3L::vetoElectronSelection(int elIdx){
    /*
        does the selection of veto electrons
        parameters: elIdx
        return: true (if the electron is a veto electron), false (else)
    */
    
    //count veto electron candidates
    counter("vetoElDenominator", kElVeto);

    //define cut values
    float pt_cut = 5.;
    float eta_cut = 2.4;
    float eta_veto_low = 1.442;
    float eta_veto_high = 1.566;
 
    //cutes like electron selection, only lower pt cut
    if(!makeCut<float>( _vc->getF("LepGood_pt", elIdx) , pt_cut   , ">"  , "pt selection"    , 0    , kElVeto)) return false;
    if(!makeCut<float>( std::abs(_vc->getF("LepGood_eta", elIdx)), eta_cut   , "<"  , "eta selection"   , 0    , kElVeto)) return false;
    if(!makeCut<float>( std::abs(_vc->getF("LepGood_eta", elIdx)), eta_veto_low, "[!]", "eta selection"   , eta_veto_high, kElVeto)) return false;

    return true;
}


//____________________________________________________________________________
bool SUSY3L::vetoMuonSelection(int muIdx){
    /*
        does the selection of veto muons
        parameters: muIdx     
        return: true (if the muon is a veto muon), false (else)
    */
    
    //count veto muon candidates
    counter("VetoMuonDenominator", kMuVeto);

    //define cut values
    float pt_cut = 5.;
 
    if(!makeCut<float>(  _vc->getF("LepGood_pt"     , muIdx), pt_cut, ">", "pt selection", 0, kMuVeto ) ) return false;

    return true;
}
