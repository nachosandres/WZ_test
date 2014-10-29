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


#include "analysis/analyzers/SUSYSSDL.hh"





/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR RUNNING THE CODE                                       **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
SUSYSSDL::SUSYSSDL(std::string configuration_file){
	/* 
	constructs the SUSYSSDL class 
	parameters: configuration_file
	return: none
	*/
	
	_Verbose -> Class("SUSYSSDL");
	
	std::vector<std::string> modules;
	modules.push_back("fillBREventPlots");
	modules.push_back("fillBRJetPlots");
	modules.push_back("fillBRLeptonPlots");
	modules.push_back("fillSREventPlots");
	modules.push_back("fillSRJetPlots");
	modules.push_back("fillSRLeptonPlots");

	initialize();
	setAllModules(modules);
	startExecution(configuration_file);


}


//____________________________________________________________________________
SUSYSSDL::~SUSYSSDL(){
	/* 
	destructs the SUSYSSDL class 
	parameters: none
	return: none
	*/

}


//____________________________________________________________________________
void SUSYSSDL::initialize(){
	/*
	initializes the SUSYSSDL class
	parameters: none
	return: none
	*/


	_vc -> registerVar("run"           , "I" );
	_vc -> registerVar("lumi"          , "I" );
	_vc -> registerVar("event"         , "I" );
	_vc -> registerVar("HLT_SingleMu"  , "I" );
	_vc -> registerVar("HLT_MuEG"      , "I" );
	_vc -> registerVar("HLT_TripleEl"  , "I" );
	_vc -> registerVar("HLT_DoubleEl"  , "I" );
	_vc -> registerVar("HLT_DoubleMu"  , "I" );
	_vc -> registerVar("nVert"         , "I" );
	_vc -> registerVar("nTrueInt"      , "I" );
	_vc -> registerVar("puWeight"      , "F" );
	_vc -> registerVar("genWeight"     , "F" );
	_vc -> registerVar("nel"           , "I" );
	_vc -> registerVar("el_pt"         , "AF");
	_vc -> registerVar("el_eta"        , "AF");
	_vc -> registerVar("el_phi"        , "AF");
	_vc -> registerVar("el_charge"     , "AI");
	_vc -> registerVar("el_relIso03"   , "AF");
	_vc -> registerVar("el_relIso04"   , "AF");
	_vc -> registerVar("el_dxy"        , "AF");
	_vc -> registerVar("el_dz"         , "AF");
	_vc -> registerVar("el_tightId"    , "AI");
	_vc -> registerVar("el_looseIdSusy", "AI");
	_vc -> registerVar("nmu"           , "I" );
	_vc -> registerVar("mu_pt"         , "AF");
	_vc -> registerVar("mu_eta"        , "AF");
	_vc -> registerVar("mu_phi"        , "AF");
	_vc -> registerVar("mu_charge"     , "AI");
	_vc -> registerVar("mu_relIso03"   , "AF");
	_vc -> registerVar("mu_relIso04"   , "AF");
	_vc -> registerVar("mu_dxy"        , "AF");
	_vc -> registerVar("mu_dz"         , "AF");
	_vc -> registerVar("mu_tightId"    , "AI");
	_vc -> registerVar("mu_looseIdSusy", "AI");
	_vc -> registerVar("met_pt"        , "F" );
	_vc -> registerVar("met_eta"       , "F" );
	_vc -> registerVar("met_phi"       , "F" );
	_vc -> registerVar("met_mass"      , "F" );
	_vc -> registerVar("njet"          , "I" );
	_vc -> registerVar("jet_pt"        , "AF");
	_vc -> registerVar("jet_rawPt"     , "AF");
	_vc -> registerVar("jet_eta"       , "AF");
	_vc -> registerVar("jet_phi"       , "AF");
	_vc -> registerVar("jet_mass"      , "AF");
	_vc -> registerVar("jet_btagCSV"   , "AF");

}


//____________________________________________________________________________
void SUSYSSDL::modifyWeight() {
	/*
	modifies the event weight for every entry in the tree
	parameters: none
	return: none
	*/ 
	
	//_EventWeight = (i -> second) -> GetEventWeight();
  if(_PUReweighting) 
    _EventWeight *= _vc -> getF("puWeight");

}


//____________________________________________________________________________
void SUSYSSDL::run(){

	// prepare event selection
	resetKinematicObjects();
	collectKinematicObjects();

	//kinematic, 3l lepton veto selection

	//baseline

	//signal re


	// parse br event selection
	if(brEventSelection()){

		// fill event list
		fillEventList();
		
		// calling the modules
		if(Tools::findInVector(_Modules, (std::string) "fillBREventPlots" )) fillEventPlots("BR");
		if(Tools::findInVector(_Modules, (std::string) "fillBRJetPlots"   )) fillJetPlots("BR");
		if(Tools::findInVector(_Modules, (std::string) "fillBRLeptonPlots")) fillLeptonPlots("BR");



		// parse sr event selection
		if(srSelection( HT("GoodJet"), _vc -> getF("met_pt"), _NumKinObj["GoodJet"], _NumKinObj["BJet"], )){

		
			// fill event list
			fillEventList();

			// calling the modules
			if(Tools::findInVector(_Modules, (std::string) "fillSREventPlots" )) fillEventPlots("SR");
			if(Tools::findInVector(_Modules, (std::string) "fillSRJetPlots"   )) fillJetPlots("SR");
			if(Tools::findInVector(_Modules, (std::string) "fillSRLeptonPlots")) fillLeptonPlots("SR");

		}
	}
}



/*****************************************************************************
******************************************************************************
** DEFINE INPUTS AND OUTPUTS                                                **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void SUSYSSDL::defineOutput(){
	/*
  	defines and reserves all output that is produced in this class
  	parameters: none
  	return: none
  	*/ 

	//_RootFile = new TFile(Tools::toTString(_OutputDir + "histos.root"), "RECREATE");

	// Baseline Region

	_hm -> addVariable("BR_HT"        , 1000, 0.0, 1000.0, "H_T [GeV]"            ); 
	_hm -> addVariable("BR_MET"       , 1000, 0.0, 1000.0, "#slash{E}_T [GeV]"    );
	_hm -> addVariable("BR_NBJets"    ,    4, 0.0,    4.0, "b-jet multiplicity"   );
	_hm -> addVariable("BR_NElectrons",    4, 1.0,    5.0, "electron multiplicity");
	_hm -> addVariable("BR_NJets"     ,    4, 1.0,    5.0, "jet multiplicity"     );
	_hm -> addVariable("BR_NLeps"     ,    4, 1.0,    5.0, "lepton multiplicity"  );
	_hm -> addVariable("BR_NMuons"    ,    4, 1.0,    5.0, "muon multiplicity"    );
	_hm -> addVariable("BR_NVrtx"     ,   40, 0.0,   40.0, "vertex multiplicity"  );
	_hm -> addVariable("BR_ElDXY"     ,   50, 0.0,    0.5, "#||{dxy}(e) [cm]"     );
	_hm -> addVariable("BR_ElEta"     ,   24, 0.0,    2.4, "#||{#eta(e)}"         );
	_hm -> addVariable("BR_ElIso"     ,   50, 0.0,    1.0, "PF Iso (e)"           );
	_hm -> addVariable("BR_ElMT"      , 1000, 0.0, 1000.0, "M_T(e) [GeV]"         );
	_hm -> addVariable("BR_ElPt"      , 1000, 0.0, 1000.0, "P_T(e) [GeV]"         );
	_hm -> addVariable("BR_MuDXY"     ,   50, 0.0,    0.5, "#||{dxy}(#mu) [cm]"   );
	_hm -> addVariable("BR_MuEta"     ,   24, 0.0,    2.4, "#||{#eta(#mu)}"       );
	_hm -> addVariable("BR_MuIso"     ,   50, 0.0,    1.0, "PF Iso (#mu)"         );
	_hm -> addVariable("BR_MuMT"      , 1000, 0.0, 1000.0, "M_T(#mu) [GeV]"       );
	_hm -> addVariable("BR_MuPt"      , 1000, 0.0, 1000.0, "P_T(#mu) [GeV]"       );
	_hm -> addVariable("BR_JetCSVBTag",   50, 0.0,    1.0, "jet CSV B-Tag"        );
	_hm -> addVariable("BR_JetPt"     , 1000, 0.0, 1000.0, "P_T(jet) [GeV]"       );

	// Signal Region

	_hm -> addVariable("SR_HT"        , 1000, 0.0, 1000.0, "H_T [GeV]"            ); 
	_hm -> addVariable("SR_MET"       , 1000, 0.0, 1000.0, "#slash{E}_T [GeV]"    );
	_hm -> addVariable("SR_NBJets"    ,    4, 0.0,    4.0, "b-jet multiplicity"   );
	_hm -> addVariable("SR_NElectrons",    4, 1.0,    5.0, "electron multiplicity");
	_hm -> addVariable("SR_NJets"     ,    4, 1.0,    5.0, "jet multiplicity"     );
	_hm -> addVariable("SR_NLeps"     ,    4, 1.0,    5.0, "lepton multiplicity"  );
	_hm -> addVariable("SR_NMuons"    ,    4, 1.0,    5.0, "muon multiplicity"    );
	_hm -> addVariable("SR_NVrtx"     ,   40, 0.0,   40.0, "vertex multiplicity"  );
	_hm -> addVariable("SR_ElDXY"     ,   50, 0.0,    0.5, "#||{dxy}(e) [cm]"     );
	_hm -> addVariable("SR_ElEta"     ,   24, 0.0,    2.4, "#||{#eta(e)}"         );
	_hm -> addVariable("SR_ElIso"     ,   50, 0.0,    1.0, "PF Iso (e)"           );
	_hm -> addVariable("SR_ElMT"      , 1000, 0.0, 1000.0, "M_T(e) [GeV]"         );
	_hm -> addVariable("SR_ElPt"      , 1000, 0.0, 1000.0, "P_T(e) [GeV]"         );
	_hm -> addVariable("SR_MuDXY"     ,   50, 0.0,    0.5, "#||{dxy}(#mu) [cm]"   );
	_hm -> addVariable("SR_MuEta"     ,   24, 0.0,    2.4, "#||{#eta(#mu)}"       );
	_hm -> addVariable("SR_MuIso"     ,   50, 0.0,    1.0, "PF Iso (#mu)"         );
	_hm -> addVariable("SR_MuMT"      , 1000, 0.0, 1000.0, "M_T(#mu) [GeV]"       );
	_hm -> addVariable("SR_MuPt"      , 1000, 0.0, 1000.0, "P_T(#mu) [GeV]"       );
	_hm -> addVariable("SR_JetCSVBTag",   50, 0.0,    1.0, "jet CSV B-Tag"        );
	_hm -> addVariable("SR_JetPt"     , 1000, 0.0, 1000.0, "P_T(jet) [GeV]"       );

}


//____________________________________________________________________________
void SUSYSSDL::loadInput(){
	/*
  	loads all input from the cache or from the database
  	parameters: none
 	return: none
 	*/


	// define function in MPAF for loading histograms, text files, histograms from database 

}


//____________________________________________________________________________
void SUSYSSDL::writeOutput(){
	/*
  	writes all output of this class to the disk
 	paramters: none
 	return: none
 	*/

	_hm -> saveHistos ("SUSYSSDL", "testconfigname");
	_au -> saveNumbers("SUSYSSDL", "testconfigname");

}





/*****************************************************************************
******************************************************************************
** OBJECT SELECTIONS                                                        **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool SUSYSSDL::bJetSelection(int jet_index){
	/*
  	does the selection of  b-jets
  	parameters: jet_index
  	return: true (if the jet is a b-jet), false (else)
  	*/

	if(!goodJetSelection(jet_index)) return false;
	if(_UsedCuts["BJetBTag"] && _vc -> getF("jet_btagCSV", jet_index) < _Cuts["BJetBTag"]) return false;
	
	return true;

}


//____________________________________________________________________________
bool SUSYSSDL::goodJetSelection(int jet_index){
	/*
  	does the selection of good jets, i.e. minimum selection of jets 
  	parameters: jet_index
  	return: true (if the jet is good), false (else)
  	*/

	if(_UsedCuts["GoodJetPt"]     && (_JEC == 1 ? _vc -> getF("jet_pt", jet_index) : _vc -> getF("jet_rawPt", jet_index)) < _Cuts["GoodJetPt"]     ) return false;
	if(_UsedCuts["GoodJetEtaUL"]  && fabs(_vc -> getF("jet_eta", jet_index))                                              > _Cuts["GoodJetEtaUL"]  ) return false;
	if(_UsedCuts["GoodJetEtaLL"]  && fabs(_vc -> getF("jet_eta", jet_index))                                              < _Cuts["GoodJetEtaLL"]  ) return false;

	// this is the implementation of jet lepton cleaning
	// maybe we find a better way => matthieu's library?

	if(_UsedCuts["GoodJetJLCdR"]){

		bool is_closest = false;
			
		// clean w/r/t loose electrons
		for(int el = 0; el < _NumKinObj["Electrons"]; ++el){
							
			float closest_dr  = 99.;
			int   closest_jet =  -1;
			
			for(int jet = 0; jet < _vc -> getI("njet"); ++jet){
				float dr = Tools::dR(_vc -> getF("el_eta", _KinObj["Electrons"][el]), _vc -> getF("jet_eta", jet), _vc -> getF("el_phi", _KinObj["Electrons"][el]), _vc -> getF("jet_phi", jet));
				if(dr < closest_dr) {
					closest_dr  = dr;
					closest_jet = jet;
				}
			}
			
			if(closest_jet == jet_index && closest_dr < _Cuts["GoodJetJLCdR"]) is_closest = true;
		
		}
		
		if(is_closest) return false; 
		
		// clean w/r/t loose muons 
		for(int mu = 0; mu < _NumKinObj["Muons"]; ++mu){
			
			float closest_dr  = 99.; 
			int   closest_jet =  -1; 
			
			for(int jet = 0; jet < _vc -> getI("njet"); ++jet){ 
				float dr = Tools::dR(_vc -> getF("mu_eta", _KinObj["Muons"][mu]), _vc -> getF("jet_eta", jet), _vc -> getF("mu_phi", _KinObj["Muons"][mu]), _vc -> getF("jet_phi", jet)); 
				if(dr < closest_dr) { 
					closest_dr  = dr; 
					closest_jet = jet; 
				} 
			} 
			
			if(closest_jet == jet_index && closest_dr < _Cuts["GoodJetJLCdR"]) is_closest = true; 
		
		} 
		
		if(is_closest) return false;
 
	}

	return true;

}


//____________________________________________________________________________
bool SUSYSSDL::electronSelection(int electron_index){
	/*
  	does the selection of electrons
  	parameters: electron_index
  	return: true (if the electron is an electron), false (else)
  	*/

	if( _UsedCuts["ElectronTightId"]    && _vc -> getI("el_tightId" , electron_index)  != _Cuts["ElectronTightId"]) return false;
	if( _UsedCuts["ElectronPt"]         && _vc -> getF("el_pt"      , electron_index)   < _Cuts["ElectronPt"]     ) return false;
	if( _UsedCuts["ElectronEtaUL"]      && fabs(_vc -> getF("el_eta", electron_index))  > _Cuts["ElectronEtaUL"] ) return false;
	if( _UsedCuts["ElectronEtaLL"]      && fabs(_vc -> getF("el_eta", electron_index))  < _Cuts["ElectronEtaLL"] ) return false;
	if((_UsedCuts["ElectronEtaExclUL"]  && fabs(_vc -> getF("el_eta", electron_index)) <= _Cuts["ElectronEtaExclUL"]) && (_UsedCuts["ElectronEtaExclLL"] && fabs(_vc -> getF("el_eta", electron_index)) >= _Cuts["ElectronEtaExclLL"])) return false;
	if( _UsedCuts["ElectronPtDXY"]      && fabs(_vc -> getF("el_dxy", electron_index))  < _Cuts["ElectronPtDXY"]  ) return false;

	return true;

}


//____________________________________________________________________________
bool SUSYSSDL::muonSelection(int muon_index){
	/*
  	does the selection of muons
  	parameters: muon_index
  	return: true (if the muon is a muon), false (else)
  	*/


	if(_UsedCuts["MuonTightId"] && _vc -> getI("mu_tightId" , muon_index) != _Cuts["MuonTightId"]) return false;
	if(_UsedCuts["MuonPt"]      && _vc -> getF("mu_pt"      , muon_index)  < _Cuts["MuonPt"]     ) return false;
	if(_UsedCuts["MuonEtaUL"]   && fabs(_vc -> getF("mu_eta", muon_index)) > _Cuts["MuonEtaUL"] ) return false;
	if(_UsedCuts["MuonEtaLL"]   && fabs(_vc -> getF("mu_eta", muon_index)) < _Cuts["MuonEtaLL"] ) return false;
	if(_UsedCuts["MuonPtDXY"]   && fabs(_vc -> getF("mu_dxy", muon_index)) < _Cuts["MuonPtDXY"]  ) return false;

	return true;

}


//____________________________________________________________________________
bool SUSYSSDL::vetoElectronSelection(int elIdx){
	/*
  	does the selection of veto electrons
  	parameters: electron_index
  	return: true (if the electron is a veto electron), false (else)
  	*/

	counter("elIdDenominator", "elIDs");

	if(electronSelection(elIdx)) return false;
	if(_UsedCuts["VetoElectronTightId"] && !makeCut<int>(   _vc -> getI("el_tightId", elIdx), _Cuts["VetoElectronTightId"], "=", "elId sel", 0, "elIDs" ) ) return false;
	if(_UsedCuts["VetoElectronPtLow"]   && !makeCut<float>( _vc -> getF("el_pt"     , elIdx), _Cuts["VetoElectronPtLow"]  , ">", "elPt sel", 0, "elIDs" ) ) return false;
  
	// if(_UsedCuts["VetoElectronTightId"] && _vc -> getI("el_tightId", electron_index) != _Cuts["VetoElectronTightId"]) return false;
	// if(_UsedCuts["VetoElectronPtLow"]   && _vc -> getF("el_pt"     , electron_index)  < _Cuts["VetoElectronPtLow"]  ) return false;

	return true;

}


//____________________________________________________________________________
bool SUSYSSDL::vetoMuonSelection(int muIdx){
	/*
  	does the selection of veto muons
  	parameters: muon_index
  	return: true (if the muon is a veto muon), false (else)
  	*/

	counter("muIdDenominator", "muon IDs");

	if(muonSelection(muIdx)) return false;
	if(_UsedCuts["VetoMuonTightId"] && !makeCut<int>(   _vc -> getI("mu_tightId", muIdx), _Cuts["VetoMuonTightId"], "=", "muId sel", 0, "muon IDs" ) ) return false;
	if(_UsedCuts["VetoMuonPtLow"]   && !makeCut<float>( _vc -> getF("mu_pt"     , muIdx), _Cuts["VetoMuonPtLow"]  , ">", "muPt sel", 0, "muon IDs" ) ) return false;

	return true;

}






/*****************************************************************************
******************************************************************************
** EVENT SELECTIONS                                                         **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool SUSYSSDL::brEventSelection(){
	/*
  	does the event selection in the baseline region for the SUSYSSDL class
  	parameters: none
  	return: true (if the event passes the selection), false (else)
  	*/

	counter("denominator");

	//std::cout << "br selection with " << (_NumKinObj["Electron"] + _NumKinObj["Muon"]) << " selected leptons and " << (_NumKinObj["VetoElectron"] + _NumKinObj["VetoMuon"]) << " leptons in the veto" << std::endl;


	if(_isData && _UsedCuts["BRHLT_DoubleMu"] && !makeCut<int>(_vc -> getI("HLT_DoubleMu"), _Cuts["BRHLT_DoubleMu"], "=", "BR HLT DoubleMu") ) return false;	
	if(_isData && _UsedCuts["BRHLT_DoubleEl"] && !makeCut<int>(_vc -> getI("HLT_DoubleEl"), _Cuts["BRHLT_DoubleEl"], "=", "BR HLT DoubleEl") ) return false;	
	if(_isData && _UsedCuts["BRHLT_MuEG"]     && !makeCut<int>(_vc -> getI("HLT_MuEG")    , _Cuts["BRHLT_MuEG"]    , "=", "BR HLT MuEG"    ) ) return false;	

 	bool is_3l_event = vetoLeptonSelection("Electron", "Muon", "BR");
	bool is_ss_event = ssLeptonSelection("Electron", "Muon", "BR");

 	//if(!makeCut<int>(_NumKinObj["Electron"] + _NumKinObj["Muon"], 2, "=", "lep multiplicity") ) return false;

	if(_UsedCuts["BRNumSSLeptons"] && !makeCut<int>( _NumKinObj["Electron"] + _NumKinObj["Muon"], _Cuts["BRNumSSLeptons"], "=", "BR lep mult user" ) ) return false; 
	if(_UsedCuts["BRNumSSLeptons"] && !makeCut( is_ss_event , "BR SS"     , "=") ) return false;
	if(_UsedCuts["BRNumSSLeptons"] && !makeCut( !is_3l_event, "BR veto 3l", "=") ) return false;
	   
	if(_UsedCuts["BRNumJetsLL"]    && !makeCut<int>( _NumKinObj["GoodJet"], _Cuts["BRNumJetsLL"] , ">=", "BR good jet mult ll" ) ) return false;

	if(_UsedCuts["BRNumBJets"]     && !makeCut<int>( _NumKinObj["BJet"]   , _Cuts["BRNumBJets"]  , "=" , "BR bjet mult"        ) ) return false;
	if(_UsedCuts["BRNumBJetsLL"]   && !makeCut<int>( _NumKinObj["BJet"]   , _Cuts["BRNumBJetsLL"], ">=", "BR bjet mult ll"     ) ) return false;

	if(_UsedCuts["BRHTLL"]         && !makeCut<float>( HT("GoodJet")      , _Cuts["BRHTLL"]      , ">" , "BR HT ll"            ) ) return false;
	
	if(_UsedCuts["BRMETIF0"] && _UsedCuts["BRMETIF1"] && _UsedCuts["BRMETIFHT"]){

		if( HT("GoodJet") < _Cuts["BRMETIFHT"] ){
			if(!makeCut<float>(_vc -> getF("met_pt"), _Cuts["BRMETIF0"], ">", "BR MET requirement") ) return false;
		}
		else{
			if(!makeCut<float>(_vc -> getF("met_pt"), _Cuts["BRMETIF1"], ">", "BR MET requirement") ) return false;
		}
	}

	if(_UsedCuts["BRMLL"] && !makeCut<float>(findMLL("Electron", "Muon"), _Cuts["BRMLL"], ">", "BR MLL" )                    ) return false; 

	return true;

}


//____________________________________________________________________________
void SUSYSSDL::collectKinematicObjects(){
	/*
  	collects all kinematic objects needed in the code, i.e. applies all object selections
  	parameters: none
  	return: none
  	*/

	std::vector<int> empty_vector;
	_KinObj.insert(std::pair<Label, std::vector<int> >("BJet"        , empty_vector));
	_KinObj.insert(std::pair<Label, std::vector<int> >("GoodJet"     , empty_vector));
	_KinObj.insert(std::pair<Label, std::vector<int> >("Electron"    , empty_vector));
	_KinObj.insert(std::pair<Label, std::vector<int> >("Muon"        , empty_vector));
	_KinObj.insert(std::pair<Label, std::vector<int> >("VetoElectron", empty_vector));
	_KinObj.insert(std::pair<Label, std::vector<int> >("VetoMuon"    , empty_vector));

	for(int i = 0; i < _vc -> getI("nmu"); ++i){
		if(muonSelection(i)) _KinObj["Muon"]    .push_back(i);
		if(vetoMuonSelection(i)) _KinObj["VetoMuon"].push_back(i);
	}

	for(int i = 0; i < _vc -> getI("nel"); ++i){
		if(electronSelection(i)) _KinObj["Electron"]    .push_back(i);
		if(vetoElectronSelection(i)) _KinObj["VetoElectron"].push_back(i);
	}	

	for(int i = 0; i < _vc -> getI("njet"); ++i){
		if(bJetSelection(i)   ) _KinObj["BJet"]   .push_back(i);
		if(goodJetSelection(i)) _KinObj["GoodJet"].push_back(i);
	}
	
	_NumKinObj["BJet"]         = _KinObj["BJet"]        .size();
	_NumKinObj["GoodJet"]      = _KinObj["GoodJet"]     .size();
	_NumKinObj["Electron"]     = _KinObj["Electron"]    .size();
	_NumKinObj["Muon"]         = _KinObj["Muon"]        .size();
	_NumKinObj["VetoElectron"] = _KinObj["VetoElectron"].size();
	_NumKinObj["VetoMuon"]     = _KinObj["VetoMuon"]    .size();

}


//____________________________________________________________________________
bool SUSYSSDL::ssLeptonSelection(std::string electron_label, std::string muon_label, std::string kr){
	/*
	checks, if the leptons that have been found in the kinematic region (kr) are same-sign
  	parameters: electron_label, muon_label (the labels of the selected electron and muon objects), kr
  	return: true (if the leptons all have same-sign), false (else)
  	*/

	int charge = 0;

	for(int el = 0; el < _NumKinObj[electron_label]; ++el){
		if(charge == 0) charge = _vc -> getI("el_charge", _KinObj[electron_label][el]);
		if(_vc -> getI("el_charge", _KinObj[electron_label][el]) != charge) return false;
	}
	
	for(int mu = 0; mu < _NumKinObj[muon_label]; ++mu){
		if(charge == 0) charge = _vc -> getI("mu_charge", _KinObj[muon_label][mu]);
		if(_vc -> getI("mu_charge", _KinObj[muon_label][mu]) != charge) return false;
	}

	return true;

}


//____________________________________________________________________________
bool SUSYSSDL::vetoLeptonSelection(std::string electron_label, std::string muon_label, std::string kr){
	/*
  	performs an essential part of the event selection in the 3l case, i.e.
  	events are rejected if there are 3 leptons where two leptons form an
  	opposite-sign same-flavor pair with some condition on mll and pt
  	parameters: electron_label, muon_label (the labels of the selected electron and muon objects)
  	return: true (if the event has such a pair, i.e. if the event shall be rejected), false (else)
  	*/

	counter("denominator", kr + " vetoLepSel");
	
	if(!makeCut<int>(_NumKinObj[electron_label] + _NumKinObj[muon_label], 2, "=", "lep mult"     , 0, kr + " vetoLepSel") ) return false; 
	if(!makeCut<int>(_NumKinObj["VetoElectron"] + _NumKinObj["VetoMuon"], 1, "=", "veto lep mult", 0, kr + " vetoLepSel") ) return false; 


	// two electrons
	if(_NumKinObj[electron_label] >= 1 && _NumKinObj["VetoElectron"] == 1){

		// checking charge
		int charge = _vc -> getI("el_charge", _KinObj["VetoElectron"][0]);
		bool os = false;
		int os_el_index = -1;
		for(int el = 0; el < _NumKinObj[electron_label]; ++el){
			if(_vc -> getI("el_charge", _KinObj[electron_label][el]) != charge){
				os = true;
				os_el_index = _KinObj[electron_label][el];
			}
		}
		
		// there is an os sf pair
		if(os){	
			float mll = MLL("electron", _KinObj["VetoElectron"][0], "electron", os_el_index);
			//std::cout << "os electron with mll " << mll << std::endl;
			//std::cout << "pt is " << _vc -> getF("el_pt", _KinObj["VetoElectron"][0]) << std::endl;
			//std::cout << "low cut: " << (_vc -> getF("el_pt", _KinObj["VetoElectron"][0]) > _Cuts["VetoElectronPtLow"]) << std::endl;
			//std::cout << "high cut: " << (_vc -> getF("el_pt", _KinObj["VetoElectron"][0]) > _Cuts["VetoElectronPtHigh"]) << std::endl;
			//std::cout << "first mll: " << (mll < _Cuts[kr + "VetoMLLLow"]) << " where " << _Cuts[kr + "VetoMLLLow"] << std::endl;
			//std::cout << "second mll: " << (mll > _Cuts[kr + "VetoMLLHighLL"] && mll < _Cuts[kr + "VetoMLLHighUL"]) << "where " << _Cuts[kr + "VetoMLLHighLL"] << " and " << _Cuts[kr + "VetoMLLHighUL"] << std::endl;
			//std::cout << "first: " << (_vc -> getF("el_pt", _KinObj["VetoElectron"][0]) > _Cuts["VetoElectronPtLow"]  && mll < _Cuts[kr + "VetoMLLLow"]) << std::endl;
			//std::cout << "second: " << (_vc -> getF("el_pt", _KinObj["VetoElectron"][0]) > _Cuts["VetoElectronPtHigh"] && (mll > _Cuts[kr + "VetoMLLHighLL"] && mll < _Cuts[kr + "VetoMLLHighUL"])) << std::endl;

			if(makeCut(_vc -> getF("el_pt", _KinObj["VetoElectron"][0]) > _Cuts["VetoElectronPtLow"]  && mll < _Cuts[kr + "VetoMLLLow"], "electron mll low pt veto", "=", kr + " vetoLepSel") ) return true;
			if(makeCut(_vc -> getF("el_pt", _KinObj["VetoElectron"][0]) > _Cuts["VetoElectronPtHigh"] && (mll > _Cuts[kr + "VetoMLLHighLL"] && mll < _Cuts[kr + "VetoMLLHighUL"]), "electron mll high pt veto", "=", kr + " vetoLepSel") ) return true;
			//std::cout << "returning false" << std::endl;
		}
	}

	// two muons
	if(_NumKinObj[muon_label] >= 1 && _NumKinObj["VetoMuon"] == 1){

		// checking charge
		int charge = _vc -> getI("mu_charge", _KinObj["VetoMuon"][0]);
		bool os = false;
		int os_mu_index = -1;
		for(int mu = 0; mu < _NumKinObj[muon_label]; ++mu){
			if(_vc -> getI("mu_charge", _KinObj[muon_label][mu]) != charge){
				os = true;
				os_mu_index = _KinObj[muon_label][mu];
			}
		}
		
		// there is an os sf pair
		if(os){
			float mll = MLL("muon", _KinObj["VetoMuon"][0], "muon", os_mu_index);
			//std::cout << "os muon with mll " << mll << std::endl;
			if(makeCut(_vc -> getF("mu_pt", _KinObj["VetoMuon"][0]) > _Cuts["VetoMuonPtLow"]  && mll < _Cuts[kr + "VetoMLLLow"], "muon mll low pt veto", "=", kr + " vetoLepSel") ) return true;
			if(makeCut(_vc -> getF("mu_pt", _KinObj["VetoMuon"][0]) > _Cuts["VetoMuonPtHigh"] && (mll > _Cuts[kr + "VetoMLLHighLL"] && mll < _Cuts[kr + "VetoMLLHighUL"]), "muon mll high pt veto", "=", kr + " vetoLepSel") ) return true;
			//std::cout << "returning false" << std::endl;
		}
	}
	
	//std::cout << "returning false" << std::endl;

	makeCut(true, "no veto event", "=", kr + " vetoLepSel");

	return false;


}


//____________________________________________________________________________
bool SUSYSSDL::srEventSelection(){
	/*
  	does the event selection for the SUSYSSDL class
  	parameters: none
  	return: true (if the event passes the selection), false (else)
  	*/

	if(_isData && _UsedCuts["SRHLT_DoubleMu"] && !makeCut<int>(_vc -> getI("HLT_DoubleMu"), _Cuts["SRHLT_DoubleMu"], "=", "SR HLT DoubleMu") ) return false;	
	if(_isData && _UsedCuts["SRHLT_DoubleEl"] && !makeCut<int>(_vc -> getI("HLT_DoubleEl"), _Cuts["SRHLT_DoubleEl"], "=", "SR HLT DoubleEl") ) return false;	
	if(_isData && _UsedCuts["SRHLT_MuEG"]     && !makeCut<int>(_vc -> getI("HLT_MuEG")    , _Cuts["SRHLT_MuEG"]    , "=", "SR HLT MuEG"    ) ) return false;	

	bool is_3l_event = vetoLeptonSelection("Electron", "Muon", "SR");
	bool is_ss_event = ssLeptonSelection  ("Electron", "Muon", "SR");

	if(_UsedCuts["SRNumSSLeptons"] && !makeCut<int>( _NumKinObj["Electron"] + _NumKinObj["Muon"], _Cuts["SRNumSSLeptons"], "=", "SR lep mult user" ) ) return false; 
	if(_UsedCuts["SRNumSSLeptons"] && !makeCut( is_ss_event , "SR SS"     , "=") ) return false;
	if(_UsedCuts["SRNumSSLeptons"] && !makeCut( !is_3l_event, "SR veto 3l", "=") ) return false;

	//std::cout << "event has " << _NumKinObj["GoodJet"] << " good jets" << std::endl;	  
 
	if(_UsedCuts["SRNumJetsUL"]    && !makeCut<int>( _NumKinObj["GoodJet"]  , _Cuts["SRNumJetsUL"] , "<=", "SR good jet mult ul") ) return false;
	if(_UsedCuts["SRNumJetsLL"]    && !makeCut<int>( _NumKinObj["GoodJet"]  , _Cuts["SRNumJetsLL"] , ">=", "SR good jet mult ll") ) return false;

	if(_UsedCuts["SRNumBJets"]     && !makeCut<int>( _NumKinObj["BJet"]     , _Cuts["SRNumBJets"]  , "=" , "SR bjet mult"       ) ) return false;
	if(_UsedCuts["SRNumBJetsLL"]   && !makeCut<int>( _NumKinObj["BJet"]     , _Cuts["SRNumBJetsLL"], ">=", "SR bjet mult ll"    ) ) return false;

	if(_UsedCuts["SRHT"]           && !makeCut<float>( HT("GoodJet")        , _Cuts["SRHT"]        , ">" , "SR HT"              ) ) return false;
	if(_UsedCuts["SRHTUL"]         && !makeCut<float>( HT("GoodJet")        , _Cuts["SRHTUL"]      , "<=", "SR HT ul"           ) ) return false;
	if(_UsedCuts["SRHTLL"]         && !makeCut<float>( HT("GoodJet")        , _Cuts["SRHTLL"]      , ">=", "SR HT ll"           ) ) return false;
	
	if(_UsedCuts["SRMET"]          && !makeCut<float>( _vc -> getF("met_pt"), _Cuts["SRMET"]       , ">" , "SR MET"             ) ) return false;
	if(_UsedCuts["SRMETUL"]        && !makeCut<float>( _vc -> getF("met_pt"), _Cuts["SRMETUL"]     , "<=", "SR MET ul"          ) ) return false;
	if(_UsedCuts["SRMETLL"]        && !makeCut<float>( _vc -> getF("met_pt"), _Cuts["SRMETLL"]     , ">=", "SR MET ll"          ) ) return false;

	if(_UsedCuts["SRMLL"]          && !makeCut<float>( findMLL("Electron", "Muon"), _Cuts["SRMLL"] , ">" , "SR MLL"             ) ) return false; 

	return true;

}





/*****************************************************************************
******************************************************************************
** EXECUTING TASKS                                                          **
******************************************************************************
*****************************************************************************/

//____________________________________________________________________________
void SUSYSSDL::fillEventPlots(std::string kr){
	/*
  	fills the control plots for event quantities
  	parameters: none
  	return: none
  	*/

	fill(kr + "_HT"        , HT("GoodJet")                                    , _EventWeight);
	fill(kr + "_MET"       , _vc -> getF("met_pt")                            , _EventWeight);
	fill(kr + "_NBJets"    , _NumKinObj["BJet"]                               , _EventWeight);
	fill(kr + "_NElectrons", _NumKinObj["Electron"]                           , _EventWeight);
	fill(kr + "_NJets"     , _NumKinObj["GoodJet"]                            , _EventWeight);
	fill(kr + "_NLeps"     , _NumKinObj["Electron"] + _NumKinObj["Muon"]      , _EventWeight);
	fill(kr + "_NMuons"    , _NumKinObj["Muon"]                               , _EventWeight);
	fill(kr + "_NVrtx"     , (_vc -> getI("nVert")>40)?40:_vc -> getI("nVert"), _EventWeight);

}


//____________________________________________________________________________
void SUSYSSDL::fillLeptonPlots(std::string kr){
	/*
  	fills the control plots for leptons
  	parameters: none
  	return: none
  	*/

	for(int i = 0; i < _NumKinObj["Muon"]; ++i){
		fill(kr + "_ElDXY", fabs(_vc -> getF("el_dxy"))    , _EventWeight);
		fill(kr + "_ElEta", fabs(_vc -> getF("el_eta"))    , _EventWeight);
		fill(kr + "_ElIso",      _vc -> getF("el_relIso04"), _EventWeight);
		fill(kr + "_ElPt" ,      _vc -> getF("el_pt")      , _EventWeight);
		fill(kr + "_ElMT" , MT("electron", i)              , _EventWeight);
	}

	for(int i = 0; i < _NumKinObj["Muon"]; ++i){
		fill(kr + "_MuDXY", fabs(_vc -> getF("mu_dxy"))    , _EventWeight);
		fill(kr + "_MuEta", fabs(_vc -> getF("mu_eta"))    , _EventWeight);
		fill(kr + "_MuIso",      _vc -> getF("mu_relIso04"), _EventWeight);
		fill(kr + "_MuPt" ,      _vc -> getF("mu_pt")      , _EventWeight);
		fill(kr + "_MuMT" , MT("muon", i)                  , _EventWeight);
	}

}


//____________________________________________________________________________
void SUSYSSDL::fillJetPlots(std::string kr){
	/*
  	fills the control plots for jets
  	parameters: none
  	return: none
  	*/


	for(int i = 0; i < _NumKinObj["GoodJet"]; ++i){
		fill(kr + "_JetPt"     , (_JEC == 1 ? _vc -> getF("jet_pt", i) : _vc -> getF("jet_rawPt", i)), _EventWeight);
		fill(kr + "_JetCSVBTag", _vc -> getF("jet_btagCSV", i)                                       , _EventWeight);
	}

}


void SUSYSSDL::setSignalRegion(string sr) {

  if(sr=="SR01") {
    setCut("HT", 200, "[]", 400 );
    setCut("MET", 50, "[]", 120 );
    setCut("NJets", 2, "[]", 3 );
    setCut("NBJets", 0, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR02") {
    setCut("HT", 400, ">" );
    setCut("MET", 50, "[]", 120 );
    setCut("NJets", 2, "[]", 3 );
    setCut("NBJets", 0, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR03") {
    setCut("HT", 200, "[]", 400 );
    setCut("MET", 50, "[]", 120 );
    setCut("NJets", 4, ">=" );
    setCut("NBJets", 0, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR04") {
    setCut("HT", 400, ">" );
    setCut("MET", 50, "[]", 120 );
    setCut("NJets", 4, ">=" );
    setCut("NBJets", 0, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
  if(sr=="SR05") {
    setCut("HT", 200, "[]", 400 );
    setCut("MET", 120, ">" );
    setCut("NJets", 2, "[]", 3 );
    setCut("NBJets", 0, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR06") {
    setCut("HT", 400, ">" );
    setCut("MET", 120, ">" );
    setCut("NJets", 2, "[]", 3 );
    setCut("NBJets", 0, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR07") {
    setCut("HT", 200, "[]", 400 );
    setCut("MET", 120, ">" );
    setCut("NJets", 4, ">=" );
    setCut("NBJets", 0, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR08") {
    setCut("HT", 400, ">" );
    setCut("MET", 120, ">" );
    setCut("NJets", 4, ">=" );
    setCut("NBJets", 0, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }


  if(sr=="SR11") {
    setCut("HT", 200, "[]", 400 );
    setCut("MET", 50, "[]", 120 );
    setCut("NJets", 2, "[]", 3 );
    setCut("NBJets", 1, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR12") {
    setCut("HT", 400, ">" );
    setCut("MET", 50, "[]", 120 );
    setCut("NJets", 2, "[]", 3 );
    setCut("NBJets", 1, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR13") {
    setCut("HT", 200, "[]", 400 );
    setCut("MET", 50, "[]", 120 );
    setCut("NJets", 4, ">=" );
    setCut("NBJets", 1, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR14") {
    setCut("HT", 400, ">" );
    setCut("MET", 50, "[]", 120 );
    setCut("NJets", 4, ">=" );
    setCut("NBJets", 1, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
  if(sr=="SR15") {
    setCut("HT", 200, "[]", 400 );
    setCut("MET", 120, ">" );
    setCut("NJets", 2, "[]", 3 );
    setCut("NBJets", 1, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR16") {
    setCut("HT", 400, ">" );
    setCut("MET", 120, ">" );
    setCut("NJets", 2, "[]", 3 );
    setCut("NBJets", 1, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR17") {
    setCut("HT", 200, "[]", 400 );
    setCut("MET", 120, ">" );
    setCut("NJets", 4, ">=" );
    setCut("NBJets", 1, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR18") {
    setCut("HT", 400, ">" );
    setCut("MET", 120, ">" );
    setCut("NJets", 4, ">=" );
    setCut("NBJets", 1, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }


  if(sr=="SR21") {
    setCut("HT", 200, "[]", 400 );
    setCut("MET", 50, "[]", 120 );
    setCut("NJets", 2, "[]", 3 );
    setCut("NBJets", 2, ">=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR22") {
    setCut("HT", 400, ">" );
    setCut("MET", 50, "[]", 120 );
    setCut("NJets", 2, "[]", 3 );
    setCut("NBJets", 2, ">=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR23") {
    setCut("HT", 200, "[]", 400 );
    setCut("MET", 50, "[]", 120 );
    setCut("NJets", 4, ">=" );
    setCut("NBJets", 2, ">=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR24") {
    setCut("HT", 400, ">" );
    setCut("MET", 50, "[]", 120 );
    setCut("NJets", 4, ">=" );
    setCut("NBJets", 2, ">=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
  if(sr=="SR25") {
    setCut("HT", 200, "[]", 400 );
    setCut("MET", 120, ">" );
    setCut("NJets", 2, "[]", 3 );
    setCut("NBJets", 2, ">=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR26") {
    setCut("HT", 400, ">" );
    setCut("MET", 120, ">" );
    setCut("NJets", 2, "[]", 3 );
    setCut("NBJets", 2, ">=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR27") {
    setCut("HT", 200, "[]", 400 );
    setCut("MET", 120, ">" );
    setCut("NJets", 4, ">=" );
    setCut("NBJets", 2, ">=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SR28") {
    setCut("HT", 400, ">" );
    setCut("MET", 120, ">" );
    setCut("NJets", 4, ">=" );
    setCut("NBJets", 2, ">=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }

 if(sr=="SRRPV0") {
    setCut("HT", 500, ">" );
    setCut("MET", 0, ">=" );
    setCut("NJets", 2, ">=" );
    setCut("NBJets", 0, ">=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SRRPV2") {
    setCut("HT", 500, ">" );
    setCut("MET", 0, ">=" );
    setCut("NJets", 2, ">=" );
    setCut("NBJets", 2, ">=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SRSStop1") {
    setCut("HT", 80, ">" );
    setCut("MET", 30, ">=" );
    setCut("NJets", 2, ">=" );
    setCut("NBJets", 1, "=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SRSStop1++") {
    setCut("HT", 80, ">" );
    setCut("MET", 30, ">=" );
    setCut("NJets", 2, ">=" );
    setCut("NBJets", 1, "=" );
    setCut("CH", 2, "=" );
    _srName = sr;
  }
 if(sr=="SRSStop2") {
    setCut("HT", 80, ">" );
    setCut("MET", 30, ">=" );
    setCut("NJets", 2, ">=" );
    setCut("NBJets", 2, ">=" );
    setCut("CH", -100, ">" );
    _srName = sr;
  }
 if(sr=="SRSStop2++") {
    setCut("HT", 80, ">" );
    setCut("MET", 30, ">=" );
    setCut("NJets", 2, ">=" );
    setCut("NBJets", 2, ">=" );
    setCut("CH", 2, "=" );
    _srName = sr;
  }

  // if(sr=="SRXX") {
  //   setCut("HT", 80, ">" );
  //   setCut("MET", 30, ">=" );
  //   setCut("NJets", 2, ">=" );
  //   setCut("NBJets", 2, ">=" );
  //   setCut("CH", 2, "=" );
  //   _srName = sr;
  // }

}


bool SUSYSSDL::srSelection(float HT,float MET, int NJets, int NBJets, int ch ) {

  //  counter("denominator",_srName);
  if(!makeCut<float>( HT, _valCutHT, _cTypeHT, "HT selection", _upValCutHT) ) return false;
  if(!makeCut<float>( MET, _valCutMET, _cTypeMET, "MET selection", _upValCutMET) ) return false;
  if(!makeCut<float>( NJets, _valCutHT, _cTypeHT, "jet multiplicity", _upValCutHT) ) return false;
  if(!makeCut<float>( NBJets, _valCutHT, _cTypeHT, "bjet multiplicity", _upValCutHT) ) return false;
  if(!makeCut<float>( ch, _valCutHT, _cTypeHT, "charge selection", _upValCutHT) ) return false;
  return true;
} 



void SUSYSSDL::setCut(string var, float valCut, string cType, float upValCut=0) {

  //Signal region

  if(var=="HTSR") {
    _valCutHTSR = valCut;
    _cTypeHTSR = cType;
    _upValCutHTSR = upValCut;
  } 

  if(var=="METSR") {
    _valCutMETSR = valCut;
    _cTypeMETSR = cType;
    _upValCutMETSR = upValCut;
  } 

  if(var=="NJetsSR") {
    _valCutNJetsSR = valCut;
    _cTypeNJetsSR = cType;
    _upValCutNJetsSR = upValCut;
  } 

  if(var=="NBJetsSR") {
    _valCutNBJetsSR = valCut;
    _cTypeNBJetsSR = cType;
    _upValCutNBJetsSR = upValCut;
  } 

  if(var=="CHSR") {
    _valCutCHSR = valCut;
    _cTypeCHSR = cType;
    _upValCutCHSR = upValCut;
  } 


  //baseline region
  if(var=="HTBR") {
    _valCutHTBR = valCut;
    _cTypeHTBR = cType;
    _upValCutHTBR = upValCut;
  } 

  if(var=="METBR") {
    _valCutMETBR = valCut;
    _cTypeMETBR = cType;
    _upValCutMETBR = upValCut;
  } 

  if(var=="NJetsBR") {
    _valCutNJetsBR = valCut;
    _cTypeNJetsBR = cType;
    _upValCutNJetsBR = upValCut;
  } 

  if(var=="NBJetsBR") {
    _valCutNBJetsBR = valCut;
    _cTypeNBJetsBR = cType;
    _upValCutNBJetsBR = upValCut;
  } 

  if(var=="CHBR") {
    _valCutCHBR = valCut;
    _cTypeCHBR = cType;
    _upValCutCHBR = upValCut;
  } 

}
