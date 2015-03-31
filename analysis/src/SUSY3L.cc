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
    _vc->registerVar("lumi"                             , "I" );    //lumi section number
    _vc->registerVar("evt"                              , "I" );    //event number
    
    _vc->registerVar("nLepGood"                         , "I" );    //number of leptons in event
    _vc->registerVar("LepGood_pdgId"                    , "AI");    //identifier for leptons (11: electron, 13: muon)
    _vc->registerVar("LepGood_pt"                       , "AD");    //pT of leptons
    _vc->registerVar("LepGood_eta"                      , "AD");    //eta of leptons track
    _vc->registerVar("LepGood_etaSc"                    , "AD");    //eta of leptons super cluster in Ecal
    _vc->registerVar("LepGood_phi"                      , "AD");    //phi of leptons
    _vc->registerVar("LepGood_charge"                   , "AI");    //charge of lepton +1 or -1
    _vc->registerVar("LepGood_relIso03"                 , "AD");    //relative isolation of the lepton, cone dimensions?
    _vc->registerVar("LepGood_dz"                       , "AD");    //difference to reconstructed primary vertex in z direction
    _vc->registerVar("LepGood_dxy"                      , "AD");    //difference to reconstructed primary vertex in xy plane
    _vc->registerVar("LepGood_sip3d"                    , "AD");    //similar observable as dxy, also vertex cut
    _vc->registerVar("LepGood_tightCharge"              , "AI");    //indicates reliability of charge measurement, values 0,1,2
    _vc->registerVar("LepGood_eleCutIdCSA14_50ns_v1"    , "AI");    //indicates reliability of electron identification [-1;4]
    _vc->registerVar("LepGood_convVeto"                 , "AI");    //0 (veto) or 1 (no veto), calculated from partner track
    _vc->registerVar("LepGood_lostHits"                 , "AI");    //number of missing hits in pixel detector
    _vc->registerVar("LepGood_tightId"                  , "AI");    //0 or 1 (loose and tight criteria?)
    _vc->registerVar("LepGood_dEtaScTrkIn"              , "AD");    //delta eta between track and supercluster in Ecal
    _vc->registerVar("LepGood_dPhiScTrkIn"              , "AD");    //delta phi between track and supercluster in Ecal
    _vc->registerVar("LepGood_hadronicOverEm"           , "AD");    //
    _vc->registerVar("LepGood_sigmaIEtaIEta"            , "AD");    //
    _vc->registerVar("LepGood_eInvMinusPInv"            , "AD");    //
    /* 
    _vc->registerVar("nTauGood"                         , "I" );    //number of taus in event
    _vc->registerVar("TauGood_pdgId"                    , "AI");    //identifier for taus (15)
    _vc->registerVar("TauGood_pt"                       , "AD");    //pT of tau
    _vc->registerVar("TauGood_eta"                      , "AD");    //eta of tau
    _vc->registerVar("TauGood_phi"                      , "AD");    //phi of tau
    _vc->registerVar("TauGood_charge"                   , "AI");    //charge of tau +1 or -1
    _vc->registerVar("TauGood_idAntiMu"                 , "I");     //tau muon discriminator
    _vc->registerVar("TauGood_idAntiE"                  , "I");     //tau electron discriminator
    _vc->registerVar("TauGood_idDecayMode"              , "I");     //
    _vc->registerVar("TauGood_isoCI3hit"                , "F");     //
    */
    _vc->registerVar("nJet"                             , "I" );    //number of jets in the event
    _vc->registerVar("Jet_pt"                           , "AD");    //pT of each of the nJet jets
    _vc->registerVar("Jet_eta"                          , "AD");    //eta of each of the nJet jets
    _vc->registerVar("Jet_phi"                          , "AD");    //phi of each of the nJet jets
    _vc->registerVar("Jet_id"                           , "AI");    //jet identifier (>=1: 8TeV loose recommendation)
    _vc->registerVar("Jet_btagCSV"                      , "AD" );     //b-tagging quantity (-1 or [0;1]

    _vc->registerVar("met_pt"                           , "F" );     //missing tranvers momentum
    _vc->registerVar("met_phi"                          , "F" );     //phi of missing transvers momentum

    //additional counter categories
    _au->addCategory( kElId, "el Id");
    _au->addCategory( kElVeto, "veto El");
    _au->addCategory( kMuId, "muon Id");
    _au->addCategory( kMuVeto, "veto Mu");
    _au->addCategory( kTauId, "tau Id");
    _au->addCategory( kTauVeto, "veto tau");
    _au->addCategory( kJetId, "jet Id");
    _au->addCategory( kBJetId, "b-jet Id");
    _au->addCategory( conZEvents, "Z events");
                 
    //config file input variables
    _pairmass = getCfgVarS("pairMass");
    _selectTaus = getCfgVarS("selectTaus");
    _BR = getCfgVarS("baselineRegion");
    _SR = getCfgVarS("signalRegion");


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
    _taus.clear();
    _elIdx.clear();
    _muIdx.clear();
    _tauIdx.clear();
    _jets.clear();
    _bJets.clear();
    
    // increment event counter, used as denominator for yield calculation
    counter("denominator");

    // do the minimal selection and collect kinematic variables for events passing it
    collectKinematicObjects();

    // initialization of baseline region cuts, baseline event selection, and filling of
    // event based observables in plots
    setBaselineRegion();
    if(!baseSelection()) return;	
    fillEventPlots("BR");

    // initialization of signal region cuts, categorization of events passing the baseline 
    // selection into different signal regions, and filling of plots
    setSignalRegion();
    if(!srSelection()) return;	
    fillEventPlots("SR");

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
    
    //event based observables for baseline region 
    _hm->addVariable("BR_HT"        , 1000,   0.0, 1000.0, "H_T [GeV]"                      );
    _hm->addVariable("BR_MET"       , 1000,   0.0, 1000.0, "#slash{E}_T [GeV]"              );
    _hm->addVariable("BR_NBJets"    ,   20,   0.0,   20.0, "b-jet multiplicity"             );
    _hm->addVariable("BR_NJets"     ,   20,   0.0,   20.0, "jet multiplicity"               ); 

    //event based observables for signal region 
    _hm->addVariable("SR_HT"        , 1000,   0.0, 1000.0, "H_T [GeV]"                      );
    _hm->addVariable("SR_MET"       , 1000,   0.0, 1000.0, "#slash{E}_T [GeV]"              );
    _hm->addVariable("SR_NBJets"    ,   20,   0.0,   20.0, "b-jet multiplicity"             );
    _hm->addVariable("SR_NJets"     ,   20,   0.0,   20.0, "jet multiplicity"               ); 

    //additional observables
    _hm->addVariable("Zmass"        ,  150,   0.0,  150.0, "Z candidate mass [GeV]"         );
    _hm->addVariable("deltaR_elmu"  ,  500,   0.0,  10.0, "delta R between el and mu"         );
}


//____________________________________________________________________________
void SUSY3L::loadInput(){
    /*
        loads all input from the cache or from the database
        parameters: none
        return: none
    */


    // define function in MPAD for loading histograms, text files, histograms from database 
}





//____________________________________________________________________________
void SUSY3L::writeOutput(){
    /*
        writes all output of this class to the disk
        paramters: none
        return: none
    */

    _hm -> saveHistos("SUSY3L", _cfgName);
    _au -> saveNumbers("SUSY3L", _cfgName);

}


//____________________________________________________________________________
void SUSY3L::modifySkimming(){
// if adding variables int he skimming tree is needed...

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
    
    // loop over all nLepGood leptons in this event and select muons
    for(int i = 0; i < _vc->getI("nLepGood"); ++i){
        // check which of the nLepGood leptons are muons, identifier 13
        if(std::abs(_vc->getI("LepGood_pdgId",i)) == 13){
            //differentiate muons for muon selecton and veto muon selection
            if(muonSelection(i)) {
                _mus.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
                                                  _vc->getD("LepGood_eta", i),
                                                  _vc->getD("LepGood_phi", i),
                                                  _vc->getI("LepGood_pdgId", i),
                                                  _vc->getI("LepGood_charge", i),
                                                  0.105) );     //muon mass
                _muIdx.push_back(i);
            }
            /*
            else {
                if(vetoMuonSelection(i))  {
                    _vMus.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
                                                       _vc->getD("LepGood_eta", i),
                                                       _vc->getD("LepGood_phi", i),
                                                       _vc->getI("LepGood_pdgId", i),
                                                       _vc->getI("LepGood_charge", i),
                                                       0.105) );    //muon mass
                }
            }
            */
        }
    }
    //number of muons in event   
    _nMus = _mus.size();
    //_nVMus = _vMus.size();

    // loop over all nLepGood leptons in this event and select electrons
    for(int i = 0; i < _vc->getI("nLepGood"); ++i){
        // check which of the nLepGood leptons are electrons, identifier 11
        if(std::abs(_vc->getI("LepGood_pdgId",i)) == 11){
            //differentiate electrons for electron selecton and veto electron selection
            if(electronSelection(i)) {
                //if electron passes electron selection, create electron candidate 
                //with respective kinematic variables and append it to _els vector
                _els.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
                                                  _vc->getD("LepGood_eta", i),
                                                  _vc->getD("LepGood_phi", i),
                                                  _vc->getI("LepGood_pdgId", i),
                                                  _vc->getI("LepGood_charge", i),
                                                  0.0005) );    //electron mass
                _elIdx.push_back(i);
            }
            /*
            else {
                //if electron passes veto selection, create veto electron candidate
                //and append to _vEls vector
                if(vetoElectronSelection(i))  {
                    _vEls.push_back( Candidate::create(_vc->getD("LepGood_pt", i),
                                                       _vc->getD("LepGood_eta", i),
                                                       _vc->getD("LepGood_phi", i),
                                                       _vc->getI("LepGood_pdgId", i),
                                                       _vc->getI("LepGood_charge", i),
                                                       0.0005) );   //electron mass
                }
            }
            */
        }
    }
  
    //number of electrons in the event
    _nEls = _els.size();
    //_nVEls = _vEls.size();
    
    /*
    // loop over all taus and apply selection
    for(int i = 0; i < _vc->getI("nTauGood"); ++i){
        // check which of the taus have tau identifier 15 (actually not needed)
        if(std::abs(_vc->getI("TauGood_pdgId",i)) == 15){
            //differentiate taus for tau selecton and veto tau selection
            if(tauSelection(i)){
                _taus.push_back( Candidate::create(_vc->getD("TauGood_pt", i),
                                                   _vc->getD("TauGood_eta", i),
                                                   _vc->getD("TauGood_phi", i),
                                                   _vc->getI("TauGood_pdgId", i),
                                                   _vc->getI("TauGood_charge", i),
                                                   1.777) );     //tau mass
                _tauIdx.push_back(i);
            }
            
            else {
                if(vetotauSelection(i)){
                    _vTaus.push_back( Candidate::create(_vc->getD("TauGood_pt", i),
                                                        _vc->getD("TauGood_eta", i),
                                                        _vc->getD("TauGood_phi", i),
                                                        _vc->getI("TauGood_pdgId", i),
                                                        _vc->getI("TauGood_charge", i),
                                                        1.777) );    //tau mass
                }
            }
        }
    }
    */
    //number of taus in the event
    _nTaus = _taus.size();

    // loop over all jets of the event
    for(int i = 0; i < _vc->getI("nJet"); ++i){
        //if jet passes good jet selection, create a jet candidate and fetch kinematics  
        if(goodJetSelection(i)) {
            _jets.push_back( Candidate::create(_vc->getD("Jet_pt", i),
                                               _vc->getD("Jet_eta", i),
                                               _vc->getD("Jet_phi", i)));
        }
    }
    //number of jets in event
    _nJets = _jets.size();
    
    // loop over all jets of the event
    for(int i = 0; i < _vc->getI("nJet"); ++i){
        //if jet passes bjet selection, create a b-jet candidate and fetch kinematics  
        if(bJetSelection(i) ) {
            _bJets.push_back( Candidate::create(_vc->getD("Jet_pt", i),
                                                _vc->getD("Jet_eta", i),
                                                _vc->getD("Jet_phi", i)));
        }
    }

    //number of (b-)jets in the event
    _nBJets = _bJets.size();
    _nJets = _jets.size();
   
    //compute sum of jet pT's 
    _HT = HT();

    //create met candidate for every event
    _met = Candidate::create(_vc->getD("met_pt"), _vc->getD("met_phi") );

}


//____________________________________________________________________________
bool SUSY3L::electronSelection(int elIdx){
    /*
        selection of electrons
        parameters: elIdx
        return: true (if the electron is accepted as an electron), false (else)
    */
    
    //count electron candidates
    counter("ElectronDenominator", kElId);

    //define cuts for electrons
    float pt_cut = 10.;
    float eta_cut = 2.4;
    float eta_veto_low = 1.4442;
    float eta_veto_high = 1.566;
    float isolation_cut = 0.15;
    float vertex_dz_cut = 0.1;      //in cm
    float vertex_dxy_cut = 0.05;    //in cm
    float sip3d_cut = 4;
    float deltaR = 0.1;
   
    //apply the cuts
    //makeCut(variable to cut on, cut value, direction of acception, name, 2nd cut value, counter)
    if(!makeCut<float>( _vc->getD("LepGood_pt", elIdx) , pt_cut, ">"  , "pt selection"    , 0    , kElId)) return false;
    if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", elIdx)), eta_cut  , "<"  , "eta selection"   , 0    , kElId)) return false;
    if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", elIdx)), eta_veto_low, "[!]", "eta selection veto"   , eta_veto_high, kElId)) return false;
    if(!makeCut<int>( _vc->getI("LepGood_eleCutIdCSA14_50ns_v1", elIdx) , 3     , ">=" , "POG CB WP-M Id " , 0    , kElId)) return false;
    if(!makeCut<float>( _vc->getD("LepGood_relIso03", elIdx) , isolation_cut   , "<"  , "isolation "      , 0    , kElId)) return false;
    if(!makeCut<float>( std::abs(_vc->getD("LepGood_dz", elIdx)), vertex_dz_cut   , "<"  , "dz selection"    , 0    , kElId)) return false;
    if(!makeCut<float>( std::abs(_vc->getD("LepGood_dxy", elIdx)), vertex_dxy_cut  , "<"  , "dxy selection"   , 0    , kElId)) return false;
    if(!makeCut<float>( std::abs(_vc->getD("LepGood_sip3d", elIdx)), sip3d_cut  , "<"  , "sip3d selection"   , 0    , kElId)) return false;
    if(!makeCut<int>( _vc->getI("LepGood_tightCharge", elIdx) , 1     , ">"  , "charge selection", 0    , kElId)) return false;
    //boolian variable if electron comes from gamme conversion or not (true if not from conversion)
    bool not_conv = (_vc->getI("LepGood_convVeto", elIdx)>0 && _vc->getI("LepGood_lostHits", elIdx)==0);
    if(!makeCut( not_conv, "conversion rejection", "=", kElId)) return false;
    
    //reject electrons which are within a cone of delta R around a muon candidate (potentially final state radiation, bremsstrahlung)
    bool muMatch = false;
    for(int im=0; im<_nMus; ++im){
        float dr = KineUtils::dR( _mus[im]->eta(), _vc->getD("LepGood_eta", elIdx), _mus[im]->phi(), _vc->getD("LepGood_phi", elIdx));
        //_deltaR = dr;
        //fill("deltaR_elmu" , _deltaR        , _weight);
        if(dr<deltaR){
            muMatch = true;
            break;
        }
    }
    if(!makeCut( !muMatch, "dR selection (mu)", "=", kElId) ) return false;

    return true;
}


//____________________________________________________________________________
bool SUSY3L::muonSelection(int muIdx){
    /*
        selection of muons
        parameters: muIdx
        return: true (if the muon is accepted as a muon), false (else)
    */
    
    //count muon candidates
    counter("MuonDenominator", kMuId);

    //define cut values
    float pt_cut = 10.;
    float eta_cut = 2.4;
    float isolation_cut = 0.15;
    float vertex_dz_cut = 0.1;
    float vertex_dxy_cut = 0.05;
    float sip3d_cut = 4;
    
    //apply the cuts
    if(!makeCut<float>( _vc->getD("LepGood_pt", muIdx), pt_cut, ">", "pt selection"    , 0, kMuId)) return false;
    if(!makeCut<float>( std::abs( _vc->getD("LepGood_eta", muIdx)), eta_cut, "<", "eta selection", 0, kMuId)) return false;
    if(!makeCut<float>( _vc->getD("LepGood_relIso03", muIdx) , isolation_cut   , "<", "isolation "      , 0, kMuId)) return false;
    if(!makeCut<int>( _vc->getI("LepGood_tightId", muIdx) , 1     , "=", "POG Tight Id "   , 0, kMuId)) return false;
    if(!makeCut<float>(std::abs(_vc->getD("LepGood_dz", muIdx)), vertex_dz_cut   , "<", "dz selection"    , 0, kMuId)) return false;
    if(!makeCut<float>(std::abs(_vc->getD("LepGood_dxy", muIdx)), vertex_dxy_cut , "<", "dxy selection"   , 0, kMuId)) return false;
    if(!makeCut<float>( std::abs(_vc->getD("LepGood_sip3d", muIdx)), sip3d_cut  , "<"  , "sip3d selection"   , 0    , kMuId)) return false;
 
    return true;
}


//____________________________________________________________________________
bool SUSY3L::tauSelection(int tauIdx){
    /*
        selection of taus
        parameters: tauIdx
        return: true (if the lepton is identified as (hadronic) tau), false (else)
    */
   
    //count tau candidates
    counter("TauDenominator", kTauId);

    //define cuts for electrons
    float pt_cut = 20.;
    float eta_cut = 2.4;
    float deltaR = 0.3;
    
    //apply the cuts
    if(!makeCut<float>( _vc->getD("TauGood_pt", tauIdx) , pt_cut, ">"  , "pt selection"    , 0    , kTauId)) return false;
    if(!makeCut<float>( std::abs(_vc->getD("TauGood_eta", tauIdx)), eta_cut  , "<"  , "eta selection"   , 0    , kTauId)) return false;
    if(!makeCut<int>( _vc->getI("TauGood_idAntiMu", tauIdx) , 2     , "=" , "anti muon" , 0    , kTauId)) return false;
    if(!makeCut<int>( _vc->getI("TauGood_idAntiE", tauIdx) , 4     , ">=" , "anti electron" , 0    , kTauId)) return false;
    if(!makeCut<int>( _vc->getI("TauGood_idDecayMode", tauIdx) , 1     , "=" , "decay mode" , 0    , kTauId)) return false;
    if(!makeCut<int>( _vc->getD("TauGood_isoCI3hit", tauIdx) , 1     , ">=" , "ci3hit" , 0    , kTauId)) return false;
    //remove taus which are within a cone of deltaR around selected electrons or muons
    //loop over all electron candidates
    bool lepMatch = false;
    for(int ie=0; ie<_nEls; ++ie){
        //calculate delta R, input eta1, eta2, phi1, phi2
        float dr = KineUtils::dR( _els[ie]->eta(), _vc->getD("TauGood_eta", tauIdx), _els[ie]->phi(), _vc->getD("TauGood_phi", tauIdx));
        if(dr < deltaR){
            lepMatch = true; 
            break;
        }
    }
    //loop over all muon candidates
    for(int im=0; im<_nMus; ++im){
        //calculate delta R, input eta1, eta2, phi1, phi2
        float dr = KineUtils::dR( _mus[im]->eta(), _vc->getD("TauGood_eta", tauIdx), _mus[im]->phi(), _vc->getD("TauGood_phi", tauIdx));
        if(dr < deltaR) {
            lepMatch = true; 
            break;
        }
    }
    if(!makeCut(!lepMatch,  "lepton cleaning", "=", kTauId) ) return false;
    return true;
}


//____________________________________________________________________________
bool SUSY3L::vetoElectronSelection(int elIdx){
    /*
        selection of veto electrons
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
    float isolation_cut = 0.2;
 
    //cuts like electron selection, only lower pt cut, looser electron identification id, looser isolation cut
    if(!makeCut<float>( _vc->getD("LepGood_pt", elIdx) , pt_cut   , ">"  , "pt selection"    , 0    , kElVeto)) return false;
    if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", elIdx)), eta_cut   , "<"  , "eta selection"   , 0    , kElVeto)) return false;
    if(!makeCut<float>( std::abs(_vc->getD("LepGood_eta", elIdx)), eta_veto_low, "[!]", "eta selection"   , eta_veto_high, kElVeto)) return false;
    if(!makeCut<int>( _vc->getI("LepGood_eleCutIdCSA14_50ns_v1", elIdx), 1, ">=" , "POG CB WP-L Id " , 0    , kElVeto)) return false;
    if(!makeCut<float>( _vc->getD("LepGood_relIso03", elIdx), isolation_cut    , "<"  , "isolation "      , 0    , kElVeto)) return false;

    return true;
}


//____________________________________________________________________________
bool SUSY3L::vetoMuonSelection(int muIdx){
    /*
        selection of veto muons
        parameters: muIdx     
        return: true (if the muon is a veto muon), false (else)
    */
    
    //count veto muon candidates
    counter("VetoMuonDenominator", kMuVeto);

    //define cut values
    float pt_cut = 5.;
    float isolation_cut = 0.2;

    if(!makeCut<int>(   _vc->getI("LepGood_tightId", muIdx), 1  , "=", "POG Tight Id", 0, kMuVeto) ) return false;
    if(!makeCut<float>( _vc->getD("LepGood_relIso03", muIdx),  isolation_cut  , "<", "isolation "   , 0, kMuVeto)) return false;
    if(!makeCut<float>(  _vc->getD("LepGood_pt"     , muIdx), pt_cut, ">", "pt selection", 0, kMuVeto ) ) return false;
 
    return true;
}

//____________________________________________________________________________
bool SUSY3L::goodJetSelection(int jetIdx){
    /*
        selection of jets
        parameters: jetIdx
        return: true (if the jet is good), false (else)
    */
    
    counter("JetDenominator", kJetId);

    //define cut values
    float pt_cut = 30.;
    float eta_cut = 2.4;
    float deltaR = 0.4;

    if(!makeCut<float>(_vc->getD("Jet_pt", jetIdx)       , pt_cut, ">", "pt selection" , 0, kJetId) ) return false;
    if(!makeCut<float>(std::abs(_vc->getD("Jet_eta", jetIdx)),  eta_cut, "<", "eta selection", 0, kJetId) ) return false;
    if(!makeCut<float>(_vc->getI("Jet_id", jetIdx),  1, ">=", "jet id", 0, kJetId) ) return false;

    //exclude jets which are within a cone of deltaR around lepton candidates or taus
    //loop over all electron candidates
    bool lepMatch = false;
    for(int ie=0; ie<_nEls; ++ie){
        //calculate delta R, input eta1, eta2, phi1, phi2
        float dr = KineUtils::dR( _els[ie]->eta(), _vc->getD("Jet_eta", jetIdx), _els[ie]->phi(), _vc->getD("Jet_phi", jetIdx));
        if(dr < deltaR){
            lepMatch = true; 
            break;
        }
    }
    
    //loop over all muon candidates
    for(int im=0; im<_nMus; ++im){
        //calculate delta R, input eta1, eta2, phi1, phi2
        float dr = KineUtils::dR( _mus[im]->eta(), _vc->getD("Jet_eta", jetIdx), _mus[im]->phi(), _vc->getD("Jet_phi", jetIdx));
        if(dr < deltaR) {
            lepMatch = true; 
            break;
        }
    }
    
    //loop over all tau candidates
    for(int it=0; it<_nTaus; ++it){
        //calculate delta R, input eta1, eta2, phi1, phi2
        float dr = KineUtils::dR( _taus[it]->eta(), _vc->getD("Jet_eta", jetIdx), _taus[it]->phi(), _vc->getD("Jet_phi", jetIdx));
        if(dr < deltaR) {
            lepMatch = true; 
            break;
        }
    }
    
    if(!makeCut(!lepMatch,  "lepton cleaning", "=", kJetId) ) return false;
    
    return true;
}



//____________________________________________________________________________
bool SUSY3L::bJetSelection(int jetIdx){
    /*
        does the selection of  b-jets
        parameters: jetIdx
        return: true (if the jet is a b-jet), false (else)
    */
    
    counter("BJetDenominator", kBJetId);

    float btagCSV_cut = 0.814;

    //b-jet needs to fulfill criteria for jets
    if(!makeCut(goodJetSelection(jetIdx), "jet Id", "=", kBJetId) ) return false;
    //cut on b-tagger parameter
    if(!makeCut<float>(_vc->getD("Jet_btagCSV", jetIdx), btagCSV_cut, ">", "csv btag selection", 0, kBJetId) ) return false;

    return true;

}



/*******************************************************************************
* ******************************************************************************
* ** KINEMATIC REGION DEFINITIONS                                             **
* ******************************************************************************
* *****************************************************************************/

//____________________________________________________________________________
void SUSY3L::setBaselineRegion(){
    /*
        sets the cuts of the baseline region (_BR)
        parameters: none
        return: none
    */

    if(_BR == "BR0"){
        setCut("LepMultiplicity"    ,    3, "="  )  ;     //number of isolated leptons
        _pt_cut_hard_leg              = 20          ;     //harsher pT requirement on one of the leptons
        _M_T_3rdLep_MET_cut           =  0          ;     //minimum transverse mass of 3rd lepton and met in On-Z events
        setCut("NJets"              ,    2, ">=" )  ;     //number of jets in event
        setCut("NBJets"             ,    1, ">=" )  ;     //number of b-tagged jets in event
        _ZMassWindow                  = 15.         ;     //width around Z mass to define on- or off-Z events
        _lowMllCut                    = 12.         ;     //low invariant mass cut for ossf leptoin pairs
        setCut("HT"                 ,   60, ">=" )  ;     //sum of jet pT's
        setCut("MET"                ,   50, ">=" )  ;     //missing transverse energy
    }

}


//____________________________________________________________________________
void SUSY3L::setSignalRegion() {
  
    /*
        sets the cuts of the signal region (_SR)
        parameters: none
        return: none
    */

    if(_SR == "SR00") {
        setCut("NBJetsSR", 1, "=" );
        setCut("NJetsSR", 2, "[]", 3 );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 50, "[[", 100 );
    }
 
    if(_SR == "SR01") {
        setCut("NBJetsSR", 1, "=" );
        setCut("NJetsSR", 2, "[]", 3 );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 100, "[[", 200 );
    }   
 
    if(_SR == "SR02") {
        setCut("NBJetsSR", 1, "=" );
        setCut("NJetsSR", 2, "[]", 3 );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 200, ">=" );
    } 

    if(_SR == "SR03") {
        setCut("NBJetsSR", 1, "=" );
        setCut("NJetsSR", 2, "[]", 3 );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 50, "[[", 100 );
    }
 
    if(_SR == "SR04") {
        setCut("NBJetsSR", 1, "=" );
        setCut("NJetsSR", 2, "[]", 3 );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 100, "[[", 200 );
    }   
 
    if(_SR == "SR05") {
        setCut("NBJetsSR", 1, "=" );
        setCut("NJetsSR", 2, "[]", 3 );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 200, ">=" );
    }

    if(_SR == "SR06") {
        setCut("NBJetsSR", 1, "=" );
        setCut("NJetsSR", 4, ">=" );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 50, "[[", 100 );
    }
 
    if(_SR == "SR07") {
        setCut("NBJetsSR", 1, "=" );
        setCut("NJetsSR", 4, ">=" );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 100, "[[", 200 );
    }   
 
    if(_SR == "SR08") {
        setCut("NBJetsSR", 1, "=" );
        setCut("NJetsSR", 4, ">=" );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 200, ">=" );
    } 

    if(_SR == "SR09") {
        setCut("NBJetsSR", 1, "=" );
        setCut("NJetsSR", 4, ">=" );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 50, "[[", 100 );
    }
 
    if(_SR == "SR10") {
        setCut("NBJetsSR", 1, "=" );
        setCut("NJetsSR", 4, ">=" );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 100, "[[", 200 );
    }   
 
    if(_SR == "SR11") {
        setCut("NBJetsSR", 1, "=" );
        setCut("NJetsSR", 4, ">=" );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 200, ">=" );
    }

    if(_SR == "SR12") {
        setCut("NBJetsSR", 2, "=" );
        setCut("NJetsSR", 2, "[]", 3 );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 50, "[[", 100 );
    }
 
    if(_SR == "SR13") {
        setCut("NBJetsSR", 2, "=" );
        setCut("NJetsSR", 2, "[]", 3 );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 100, "[[", 200 );
    }   
 
    if(_SR == "SR14") {
        setCut("NBJetsSR", 2, "=" );
        setCut("NJetsSR", 2, "[]", 3 );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 200, ">=" );
    } 

    if(_SR == "SR15") {
        setCut("NBJetsSR", 2, "=" );
        setCut("NJetsSR", 2, "[]", 3 );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 50, "[[", 100 );
    }
 
    if(_SR == "SR16") {
        setCut("NBJetsSR", 2, "=" );
        setCut("NJetsSR", 2, "[]", 3 );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 100, "[[", 200 );
    }   
 
    if(_SR == "SR17") {
        setCut("NBJetsSR", 2, "=" );
        setCut("NJetsSR", 2, "[]", 3 );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 200, ">=" );
    }

    if(_SR == "SR18") {
        setCut("NBJetsSR", 2, "=" );
        setCut("NJetsSR", 4, ">=" );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 50, "[[", 100 );
    }
 
    if(_SR == "SR19") {
        setCut("NBJetsSR", 2, "=" );
        setCut("NJetsSR", 4, ">=" );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 100, "[[", 200 );
    }   
 
    if(_SR == "SR20") {
        setCut("NBJetsSR", 2, "=" );
        setCut("NJetsSR", 4, ">=" );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 200, ">=" );
    } 

    if(_SR == "SR21") {
        setCut("NBJetsSR", 2, "=" );
        setCut("NJetsSR", 4, ">=" );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 50, "[[", 100 );
    }
 
    if(_SR == "SR22") {
        setCut("NBJetsSR", 2, "=" );
        setCut("NJetsSR", 4, ">=" );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 100, "[[", 200 );
    }   
 
    if(_SR == "SR23") {
        setCut("NBJetsSR", 2, "=" );
        setCut("NJetsSR", 4, ">=" );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 200, ">=" );
    }

    if(_SR == "SR24") {
        setCut("NBJetsSR", 3, ">=" );
        setCut("NJetsSR", 2, ">=" );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 50, "[[", 100 );
    }
 
    if(_SR == "SR25") {
        setCut("NBJetsSR", 3, ">=" );
        setCut("NJetsSR", 2, ">=" );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 100, "[[", 200 );
    }   
 
    if(_SR == "SR26") {
        setCut("NBJetsSR", 3, ">=" );
        setCut("NJetsSR", 2, ">=" );
        setCut("HTSR", 60, "[[", 200 );
        setCut("METSR", 200, ">=" );
    } 

    if(_SR == "SR27") {
        setCut("NBJetsSR", 3, ">=" );
        setCut("NJetsSR", 2, ">=" );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 50, "[[", 100 );
    }
 
    if(_SR == "SR28") {
        setCut("NBJetsSR", 3, ">=" );
        setCut("NJetsSR", 2, ">=" );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 100, "[[", 200 );
    }   
 
    if(_SR == "SR29") {
        setCut("NBJetsSR", 3, ">=" );
        setCut("NJetsSR", 2, ">=" );
        setCut("HTSR", 200, ">=" );
        setCut("METSR", 200, ">=" );
    }

}

//____________________________________________________________________________
void SUSY3L::setCut(std::string var, float valCut, std::string cType, float upValCut) {
    /*
        sets the parameters (valCut, cType, upValCut) for a specific cut on a variable (var)
        parameters: var, valCut (the cut value), cType (the cut type), upValCut (the upper value
        in case one cuts the variable in a range)
        return: none
    */

    //baseline region
    if(var == "LepMultiplicity") {
        _valCutLepMultiplicityBR   = valCut;
        _cTypeLepMultiplicityBR    = cType;
        _upValCutLepMultiplicityBR = upValCut;
    }
    else if(var == "NJets") {
        _valCutNJetsBR   = valCut;
        _cTypeNJetsBR    = cType;
        _upValCutNJetsBR = upValCut;
    }
    else if(var == "NBJets") {
        _valCutNBJetsBR   = valCut;
        _cTypeNBJetsBR    = cType;
        _upValCutNBJetsBR = upValCut;
    }
    else if(var == "HT") {
        _valCutHTBR   = valCut;
        _cTypeHTBR    = cType;
        _upValCutHTBR = upValCut;
    }
    else if(var == "MET") {
        _valCutMETBR   = valCut;
        _cTypeMETBR    = cType;
        _upValCutMETBR = upValCut;
    }


    // signal region

    if(var == "HTSR") {
        _valCutHTSR   = valCut;
        _cTypeHTSR    = cType;
        _upValCutHTSR = upValCut;
    }

    else if(var == "METSR") {
        _valCutMETSR   = valCut;
        _cTypeMETSR    = cType;
        _upValCutMETSR = upValCut;
    }

    else if(var == "NJetsSR") {
        _valCutNJetsSR   = valCut;
        _cTypeNJetsSR    = cType;
        _upValCutNJetsSR = upValCut;
    }

    else if(var == "NBJetsSR") {
        _valCutNBJetsSR   = valCut;
        _cTypeNBJetsSR    = cType;
        _upValCutNBJetsSR = upValCut;
    }

}



/*******************************************************************************
* ******************************************************************************
* ** EVENT SELECTIONS                                                         **
* ******************************************************************************
* *****************************************************************************/

//____________________________________________________________________________
bool SUSY3L::baseSelection(){
    /*
        implements the basic selection that is fundamental for both the baseline 
        and the signal region selections
        parameters: none
        return: true (if event passes selection), false (else)
    */
    
    //select events with certain lepton multiplicity of all flavor combinations
    if(!makeCut<int>( _nEls + _nMus, _valCutLepMultiplicityBR, _cTypeLepMultiplicityBR, "lepton multiplicity", _upValCutLepMultiplicityBR ) ) return false;

    //require at least 1 of the leptons to have higher pT than original cut
    bool has_hard_leg = hardLegSelection();
    if(!makeCut( has_hard_leg , "hard leg selection", "=") ) return false;

    //require minimum number of jets
    if(!makeCut<int>( _nJets, _valCutNJetsBR, _cTypeNJetsBR, "jet multiplicity", _upValCutNJetsBR) ) return false;

    //require minimum number of b-tagged jets
    if(!makeCut<int>( _nBJets, _valCutNBJetsBR, _cTypeNBJetsBR, "b-jet multiplicity", _upValCutNBJetsBR) ) return false;

    //require minimum hadronic activity (sum of jet pT's)
    if(!makeCut<float>( _HT, _valCutHTBR, _cTypeHTBR, "hadronic activity", _upValCutHTBR) ) return false;

    //require minimum missing transvers energy (actually missing momentum)
    if(!makeCut<float>( _met->pt(), _valCutMETBR, _cTypeMETBR, "missing transverse energy", _upValCutMETBR) ) return false;

    //reject event if ossf lepton pair with low invariant mass is found
    bool has_low_mll = lowMllPair();
    if(!makeCut( !has_low_mll , "low mll rejection", "=") ) return false;

    //select on or off-Z events according to specification in config file
    //bool is_reconstructed_Z = !ZEventSelection();
    bool is_reconstructed_Z = ZEventSelectionLoop();

    //if(is_reconstructed_Z){
    //    fill("Zmass" , _Z->mass()        , _weight);
    //}
    
    if(_pairmass == "off"){
        if(!makeCut( !is_reconstructed_Z, "mll selection", "=") ) return false;
    }
    else if(_pairmass == "on"){
        if(!makeCut( is_reconstructed_Z, "mll selection", "=") ) return false;
    }

    return true;
}

//____________________________________________________________________________
bool SUSY3L::hardLegSelection(){
    /*
        Checks if the selected event with at least 3 leptons has at least one lepton 
        fullfilling a harsher pT cut 
        return: true (if the event has such a lepton with higher pT), false (else)
    */

    //check if one of the electrons fullfils hard pt cut
    for(int ie=0; ie<_nEls; ++ie){
        if(_els[ie]->pt()>_pt_cut_hard_leg) return true;
    }

    //check if one of the muons fullfils hard pt cut
    for(int im=0; im<_nMus; ++im){
        if(_mus[im]->pt()>_pt_cut_hard_leg) return true;
    }

    //check if one of the taus fullfils hard pt cut
    //for(int it=0; it<_nTaus; ++it){
    //    if(_taus[it]->pt()>_pt_cut_hard_leg) return true;
    //}


    return false;
}

//____________________________________________________________________________
bool SUSY3L::lowMllPair(){
    /*
        Checks if event has ossf lepton pair with low invariant mass 
        return: true (if the event has such a lepton pair), false (else)
    */

    //loop over all possible combination of two electrons
    for(int ie1=0; ie1 < _nEls; ie1++) {
        for(int ie2 = ie1; ie2 < _nEls; ie2++) {
            //continue if not an ossf pair
            if(_vc->getI("LepGood_pdgId", ie1) != - _vc->getI("LepGood_pdgId", ie2) ) continue;
            //return true if low mass pair is found
            float mll = Candidate::create(_els[ie1], _els[ie2])->mass();
            if(mll < _lowMllCut) return true;
        }
    }

    //loop over all possible combination of two muons
    for(int im1=0; im1 < _nMus; im1++) {
        for(int im2 = im1; im2 < _nMus; im2++) {
            //continue if not an ossf pair
            if(_vc->getI("LepGood_pdgId", im1) != - _vc->getI("LepGood_pdgId", im2) ) continue;
            //return true if low mass pair is found
            float mll = Candidate::create(_mus[im1], _mus[im2])->mass();
            if(mll < _lowMllCut) return true;
           }
        }
 
    //loop over all possible combination of two taus
    //for(int it1=0; it1 < _nTaus; it1++) {
    //    for(int it2 = it1; it2 < _nTaus; it2++) {
    //        //continue if not an ossf pair
    //        if(_vc->getI("LepGood_pdgId", it1) != - _vc->getI("LepGood_pdgId", it2) ) continue;
    //        //return true if low mass pair is found
    //       float mll = Candidate::create(_taus[it1], _taus[it2])->mass();
    //        if(mll < _lowMllCut) return true;
    //       }
    //    }
 
    return false;
}


//____________________________________________________________________________
//bool SUSY3L::ZEventSelection(){
    /*
        Checks if there is a same-flavor opposite-charge pair with an invariant 
        mass around the Z mass among the 3 leptons. Faster than ZEventSelectionLoop 
        but no Z candidate extraction, just immediate rejection of event
        return: true (if a Z can not be reconstructed from 2 leptons), false (else)
    */
/*    
    //TODO: modify for more than 3 leptons
    //count reconstructed Z bosons
    counter("denominator", conZEvents);

    //Z mass
    float Zmass = 91.1876;

    //three electrons
    if(_nEls == 3){
        if(_els[0]->charge() != _els[1]->charge()){
            float mll = Candidate::create(_els[0], _els[1])->mass();
            //if(!makeCut<float>(mll, _lowMllCut, "<", "low mll veto", 0, conZEvents) ) return true;  
            if(!makeCut<float>(mll, Zmass - _ZMassWindow, "<", "mll Z veto", Zmass + _ZMassWindow, conZEvents) ) return false;
        }
        if(_els[0]->charge() != _els[2]->charge()){
            float mll = Candidate::create(_els[0], _els[2])->mass();
            //if(!makeCut<float>(mll, _lowMllCut, "<", "low mll veto", 0, conZEvents) ) return true;  
            if(!makeCut<float>(mll, Zmass - _ZMassWindow, "<", "mll Z veto", Zmass + _ZMassWindow, conZEvents) ) return false;
        }
        if(_els[1]->charge() != _els[2]->charge()){
            float mll = Candidate::create(_els[1], _els[2])->mass();
            //if(!makeCut<float>(mll, _lowMllCut, "<", "low mll veto", 0, conZEvents) ) return true;  
            if(!makeCut<float>(mll, Zmass - _ZMassWindow, "<", "mll Z veto", Zmass + _ZMassWindow, conZEvents) ) return false;
        }
    }
     
    return false;
}
*/

//____________________________________________________________________________
bool SUSY3L::ZEventSelectionLoop(){
    /*
        Checks if there is a same-flavor opposite-charge lepton pair with an invariant 
        mass around the Z mass. The ossf pair with an invariant mass closest to the 
        Z mass is added as Z candidate. Additionally a requirement on the transverse mass
        of the 3rd lepton and the met is checked
        return: true (if a Z can be reconstructed from 2 leptons and tranverse mass 
        requirement is fulfilled), false (else)
    */
    
    //count reconstructed Z bosons
    counter("denominator", conZEvents);

    //Z mass
    float Zmass = 91.1876;
    float diff = 1000000;
    bool Zevent = false;
    float pt_3rdLeg = 0;
    float phi_3rdLeg = 0;
    float mt = 0;

    bool el_Zcand = false;
    //loop over all possible combination of two electrons
    for(int ie1=0; ie1 < _nEls; ie1++){
        for(int ie2 = ie1; ie2 < _nEls; ie2++) {
            //continue if not an ossf pair
            if( _els[ie1]->pdgId() != - _els[ie2]->pdgId()) continue;
            //create new Z candidate
            Candidate* Ztmp = Candidate::create(_els[ie1], _els[ie2]);
            //keep Z candidate if smallest difference to Z mass
            if((std::abs(Ztmp->mass()-Zmass) < _ZMassWindow) && (std::abs(Ztmp->mass()-Zmass)<diff) ) {
                _Z = Ztmp;
                diff = std::abs(_Z->mass()-Zmass);
                el_Zcand = true;
            }
            else{
                continue;
            }
            //measure pt and phi of 3rd lepton
            if(_nEls == 3){
                if(ie1+ie2 == 1){
                    pt_3rdLeg = _els[2]->pt();
                    phi_3rdLeg = _els[2]->phi();
                }
                if(ie1+ie2 == 2){
                    pt_3rdLeg = _els[1]->pt();
                    phi_3rdLeg = _els[1]->phi();
                }
                if(ie1+ie2 == 3){
                    pt_3rdLeg = _els[0]->pt();
                    phi_3rdLeg = _els[0]->phi();
                }                 
            }
            if(_nMus == 1){
                pt_3rdLeg = _mus[0]->pt();
                phi_3rdLeg = _mus[0]->phi();
            }
        }
    }
    
    if(el_Zcand == true){
            //calculate transverse mass of 3rd lepton and met
        mt = M_T(pt_3rdLeg, _vc->getD("met_pt"), phi_3rdLeg, _vc->getD("met_phi"));
        //accept event if Z candidate exists and mt critirion is fulfilled
                    
        if( (mt > _M_T_3rdLep_MET_cut) && (std::abs(_Z->mass()-Zmass) < _ZMassWindow)){
            Zevent = true;
        }
        mt = 0.;
        pt_3rdLeg = 0.;
        phi_3rdLeg = 0.;
    }

    bool mu_Zcand = false;
    //loop over all possible combination of two muons
    for(int im1=0; im1 < _nMus; im1++) {
        for(int im2 = im1; im2 < _nMus; im2++) {
            //continue if not an ossf pair
            if( _mus[im1]->pdgId() != - _mus[im2]->pdgId()) continue;
            //create new Z candidate
            Candidate* Ztmp = Candidate::create(_mus[im1], _mus[im2]);
            //keep Z candidate if smallest difference to Z mass
            if((std::abs(Ztmp->mass()-Zmass) < _ZMassWindow) && (std::abs(Ztmp->mass()-Zmass)<diff) ) {
                _Z = Ztmp;
                diff = std::abs(_Z->mass()-Zmass);
            }
            else{
                continue;
            }
            
            //measure pt and phi of 3rd lepton
            if(_nMus == 3){
                if(im1+im2 == 1){
                    pt_3rdLeg = _mus[2]->pt();
                    phi_3rdLeg = _mus[2]->phi();
                }
                if(im1+im2 == 2){
                    pt_3rdLeg = _mus[1]->pt();
                    phi_3rdLeg = _mus[1]->phi();
                }
                if(im1+im2 == 3){
                    pt_3rdLeg = _mus[0]->pt();
                    phi_3rdLeg = _mus[0]->phi();
                }                 
            }
            if(_nEls == 1){
                pt_3rdLeg = _els[0]->pt();
                phi_3rdLeg = _els[0]->phi();
            }        
        }
    }

    if(mu_Zcand == true){
        //calculate transverse mass of 3rd lepton and met
        mt = M_T(pt_3rdLeg, _vc->getD("met_pt"), phi_3rdLeg, _vc->getD("met_phi"));
        //accept event if Z candidate exists and mt critirion is fulfilled
        
        if( (mt > _M_T_3rdLep_MET_cut) && (std::abs(_Z->mass()-Zmass) < _ZMassWindow)){
            Zevent = true;
        }
        mt = 0.;
        pt_3rdLeg = 0.;
        phi_3rdLeg = 0.;
    }
       
    return Zevent;
}


//____________________________________________________________________________
bool SUSY3L::srSelection(){
    /*
        implements the signal region selection provided that the base selection
        already has been applied, i.e. here we just cut on the variables that 
        distinguish the different signal regions
        parameters: none
        return: true (if event passes selection), false (else)
    */

    // cut on the variables distriminating the signal regions
    if(!makeCut<float>( _nBJets     , _valCutNBJetsSR, _cTypeNBJetsSR, "SR bjet multiplicity", _upValCutNBJetsSR) ) return false;
    if(!makeCut<int>( _nJets       , _valCutNJetsSR , _cTypeNJetsSR , "SR jet multiplicity" , _upValCutNJetsSR ) ) return false;
    if(!makeCut<float>( _HT          , _valCutHTSR    , _cTypeHTSR    , "SR HT selection"     , _upValCutHTSR    ) ) return false;
    if(!makeCut<float>( _met->pt()   , _valCutMETSR   , _cTypeMETSR   , "SR MET selection"    , _upValCutMETSR   ) ) return false;

    return true;

}



/*******************************************************************************
* ******************************************************************************
* ** EXECUTING TASKS                                                          **
* ******************************************************************************
* *****************************************************************************/

//____________________________________________________________________________
void SUSY3L::fillEventPlots(std::string kr){
    /*
        fills the control plots for event quantities
        parameters: none
        return: none
    */

    fill(kr + "_HT"        , _HT                    , _weight);
    fill(kr + "_MET"       , _met->pt()             , _weight);
    fill(kr + "_NBJets"    , _nBJets                , _weight);
    fill(kr + "_NJets"     , _nJets                 , _weight);

}




//____________________________________________________________________________
float SUSY3L::DeltaPhi(float phi1, float phi2){
    /*
        computes delta phi for two giveb phis taking into account the phi range
        parameters: phi1, phi2
        return: delta phi
    */

        float result = phi1 - phi2;
        while( result >   TMath::Pi() ) result -= TMath::TwoPi();
        while( result <= -TMath::Pi() ) result += TMath::TwoPi();
        
        return TMath::Abs(result);
       
       
}

//____________________________________________________________________________
float SUSY3L::HT(){
    /*
        computes HT for a given list of selected jets
        parameters: jet_label
        return: HT
    */

        float ht = 0;
        for(int i = 0; i < _nJets; ++i){
            ht += _jets[i]->pt();
        }
        return ht;
}

//____________________________________________________________________________
float SUSY3L::M_T(float pt_lepton, float met, float phi_lepton, float phi_met){
    /*
        computes the transverse mass for a lepton and met
        parameters: pt_lepton, met, phi_lepton, phi_met
        return: transverse mass M_T
    */

        float deltaPhi = DeltaPhi(phi_lepton, phi_met);
        float m_t = 0;
        m_t = sqrt(2 * pt_lepton * met * (1 - cos(deltaPhi) ));
        return m_t;
}

