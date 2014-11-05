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

	setAllModules(modules);
	startExecution(configuration_file);
	initialize();
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


	_vc -> registerVar("run"                          , "I" );
	_vc -> registerVar("lumi"                         , "I" );
	_vc -> registerVar("event"                        , "I" );
	_vc -> registerVar("HLT_SingleMu"                 , "I" );
	_vc -> registerVar("HLT_MuEG"                     , "I" );
	_vc -> registerVar("HLT_TripleEl"                 , "I" );
	_vc -> registerVar("HLT_DoubleEl"                 , "I" );
	_vc -> registerVar("HLT_DoubleMu"                 , "I" );
	_vc -> registerVar("nVert"                        , "I" );
	_vc -> registerVar("nTrueInt"                     , "I" );
	_vc -> registerVar("puWeight"                     , "F" );
	_vc -> registerVar("genWeight"                    , "F" );
	_vc -> registerVar(_lepnum                        , "I" );
	_vc -> registerVar(_lep + "_pt"                   , "AF");
	_vc -> registerVar(_lep + "_eta"                  , "AF");
	_vc -> registerVar(_lep + "_phi"                  , "AF");
	_vc -> registerVar(_lep + "_charge"               , "AI");
	_vc -> registerVar(_lep + "_pdgId"                , "AI");
	_vc -> registerVar(_lep + "_relIso03"             , "AF");
	_vc -> registerVar(_lep + "_relIso04"             , "AF");
	_vc -> registerVar(_lep + "_dxy"                  , "AF");
	_vc -> registerVar(_lep + "_dz"                   , "AF");
	_vc -> registerVar(_lep + "_tightId"              , "AI");
	_vc -> registerVar(_lep + "_looseIdSusy"          , "AI");
	_vc -> registerVar(_lep + "_eleCutIdCSA14_50ns_v1", "AI");
	_vc -> registerVar(_lep + "_mvaNew"               , "AF");
	//_vc -> registerVar("nel"                          , "I" );
	//_vc -> registerVar("el_pt"                        , "AF");
	//_vc -> registerVar("el_eta"                       , "AF");
	//_vc -> registerVar("el_phi"                       , "AF");
	//_vc -> registerVar("el_charge"                    , "AI");
	//_vc -> registerVar("el_pdgId"                     , "AI");
	//_vc -> registerVar("el_relIso03"                  , "AF");
	//_vc -> registerVar("el_relIso04"                  , "AF");
	//_vc -> registerVar("el_dxy"                       , "AF");
	//_vc -> registerVar("el_dz"                        , "AF");
	//_vc -> registerVar("el_tightId"                   , "AI");
	//_vc -> registerVar("el_looseIdSusy"               , "AI");
	//_vc -> registerVar("nmu"                          , "I" );
	//_vc -> registerVar("mu_pt"                        , "AF");
	//_vc -> registerVar("mu_eta"                       , "AF");
	//_vc -> registerVar("mu_phi"                       , "AF");
	//_vc -> registerVar("mu_charge"                    , "AI");
	//_vc -> registerVar("mu_pdgId"                     , "AI");
	//_vc -> registerVar("mu_relIso03"                  , "AF");
	//_vc -> registerVar("mu_relIso04"                  , "AF");
	//_vc -> registerVar("mu_dxy"                       , "AF");
	//_vc -> registerVar("mu_dz"                        , "AF");
	//_vc -> registerVar("mu_tightId"                   , "AI");
	//_vc -> registerVar("mu_looseIdSusy"               , "AI");
	_vc -> registerVar("met_pt"                       , "F" );
	_vc -> registerVar("met_eta"                      , "F" );
	_vc -> registerVar("met_phi"                      , "F" );
	_vc -> registerVar("met_mass"                     , "F" );
	_vc -> registerVar(_jetnum                        , "I" ); 
	_vc -> registerVar(_jet + "_pt"                   , "AF"); 
	_vc -> registerVar(_jet + "_rawPt"                , "AF"); 
	_vc -> registerVar(_jet + "_eta"                  , "AF"); 
	_vc -> registerVar(_jet + "_phi"                  , "AF"); 
	_vc -> registerVar(_jet + "_mass"                 , "AF"); 
	_vc -> registerVar(_jet + "_btagCSV"              , "AF");

	//generator informations
	_vc -> registerVar("ngenLep"                      , "I" );
	_vc -> registerVar("genLep_eta"                   , "AF");
	_vc -> registerVar("genLep_phi"                   , "AF");
	_vc -> registerVar("genLep_pdgId"                 , "AI");




	//additional counter categories
	_au->addCategory( kElId, "el Id");
	_au->addCategory( kElVeto, "veto El");
	_au->addCategory( kMuId, "muon Id");
	_au->addCategory( kMuVeto, "veto Mu");
	_au->addCategory( kJetId, "jet Id");
	_au->addCategory( kBJetId, "b-jet Id");
	_au->addCategory( kVetoLepSel, "vetoLepSel" );


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

  _leptons.clear();
  
	// prepare event selection
	resetKinematicObjects();
	collectKinematicObjects();


	// basic event selection (triggers, 2 ss leptons, veto)
	if(!baseSelection()) return;

	//skim right after the basic selection
	//fillSkimTree();
	//return;
	
	//cout<<" ************************************************** new event "<<_SampleName<<"  "<<_EntryIterator<<endl;
	//splitting the samples into categories
	// cout<<"============= lepton 1"<<endl;
	// cout<<" ==> "<<genMatchCateg( _leptons[0] )<<endl;
	// cout<<"============= lepton 2"<<endl;
	// cout<<" ==> "<<genMatchCateg( _leptons[1] )<<endl;
if(_SampleName.find("TTJets")!=(size_t)-1 || _SampleName.find("DYJets")!=(size_t)-1 ||
	   _SampleName.find("WJets")!=(size_t)-1 ) {
	 int lep1Id = genMatchCateg( _leptons[0] );
	 int lep2Id = genMatchCateg( _leptons[1] );
	
	 if(_SampleName.find("misId")!=(size_t)-1) {
	   if( ! ( (lep1Id == kMisChargePdgId && lep2Id >= kMisChargePdgId) || 
	 	  (lep2Id == kMisChargePdgId && lep1Id >= kMisChargePdgId) ) ) return;
	 }
	 if(_SampleName.find("fake")!=(size_t)-1) {
	   if( lep1Id > kMisMatchPdgId &&
	       lep2Id > kMisMatchPdgId ) return;
	 }
	 if(_SampleName.find("prompt")!=(size_t)-1) {
	   if( genMatchCateg( _leptons[0] ) != kGenMatched ||
	       genMatchCateg( _leptons[1] ) != kGenMatched ) return;
	 }
	 }
	 counter("genCateg selection");
	
	//cout<<" ============> selected"<<endl;

	// br event selection
	if(!brSelection()) return;

	// fill event list
	fillEventList();
		
	// calling the modules
	if(Tools::findInVector(_Modules, (std::string) "fillBREventPlots" )) fillEventPlots("BR");
	if(Tools::findInVector(_Modules, (std::string) "fillBRJetPlots"   )) fillJetPlots("BR");
	if(Tools::findInVector(_Modules, (std::string) "fillBRLeptonPlots")) fillLeptonPlots("BR");
	
	
	// sr event selection
	if(!srSelection()) return;
		
	// fill event list
	fillEventList();
	
	// calling the modules
	if(Tools::findInVector(_Modules, (std::string) "fillSREventPlots" )) fillEventPlots("SR");
	if(Tools::findInVector(_Modules, (std::string) "fillSRJetPlots"   )) fillJetPlots("SR");
	if(Tools::findInVector(_Modules, (std::string) "fillSRLeptonPlots")) fillLeptonPlots("SR");

	
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


	// this is now here but should be somewhere else!!!
	setSignalRegion();
	setBaselineRegion();


	// Baseline Region

	_hm -> addVariable("BR_LepCharge" ,   20, -10.0,   10.0, "lepton charge"        );
	_hm -> addVariable("BR_HT"        , 1000,   0.0, 1000.0, "H_T [GeV]"            ); 
	_hm -> addVariable("BR_MET"       , 1000,   0.0, 1000.0, "#slash{E}_T [GeV]"    );
	_hm -> addVariable("BR_MLL"       , 1000,   0.0, 1000.0, "m_{ll} [GeV]"         ); 
	_hm -> addVariable("BR_NBJets"    ,   20,   0.0,   20.0, "b-jet multiplicity"   );
	_hm -> addVariable("BR_NElectrons",   20,   0.0,   20.0, "electron multiplicity");
	_hm -> addVariable("BR_NJets"     ,   20,   0.0,   20.0, "jet multiplicity"     );
	_hm -> addVariable("BR_NLeps"     ,   20,   0.0,   20.0, "lepton multiplicity"  );
	_hm -> addVariable("BR_NMuons"    ,   20,   0.0,   20.0, "muon multiplicity"    );
	_hm -> addVariable("BR_NVrtx"     ,   40,   0.0,   40.0, "vertex multiplicity"  );
	_hm -> addVariable("BR_ElDXY"     ,   50,   0.0,    0.5, "#||{dxy}(e) [cm]"     );
	_hm -> addVariable("BR_ElAbsEta"  ,  240,   0.0,    2.4, "#||{#eta(e)}"         );
	_hm -> addVariable("BR_ElEta"     ,  480,  -2.4,    2.4, "#eta(e)"              );
	_hm -> addVariable("BR_ElIso"     ,   50,   0.0,    1.0, "PF Iso (e)"           );
	_hm -> addVariable("BR_ElMT"      , 1000,   0.0, 1000.0, "M_T(e) [GeV]"         );
	_hm -> addVariable("BR_ElPt"      , 1000,   0.0, 1000.0, "P_T(e) [GeV]"         );
	_hm -> addVariable("BR_MuDXY"     ,   50,   0.0,    0.5, "#||{dxy}(#mu) [cm]"   );
	_hm -> addVariable("BR_MuAbsEta"  ,  240,   0.0,    2.4, "#||{#eta(#mu)}"       );
	_hm -> addVariable("BR_MuEta"     ,  480,  -2.4,    2.4, "#eta(#mu)"            );
	_hm -> addVariable("BR_MuIso"     ,   50,   0.0,    1.0, "PF Iso (#mu)"         );
	_hm -> addVariable("BR_MuMT"      , 1000,   0.0, 1000.0, "M_T(#mu) [GeV]"       );
	_hm -> addVariable("BR_MuPt"      , 1000,   0.0, 1000.0, "P_T(#mu) [GeV]"       );
	_hm -> addVariable("BR_JetCSVBTag",   50,   0.0,    1.0, "jet CSV B-Tag"        );
	_hm -> addVariable("BR_JetPt"     , 1000,   0.0, 1000.0, "P_T(jet) [GeV]"       );

	// Signal Region

	_hm -> addVariable("SR_LepCharge" ,   20, -10.0,   10.0, "lepton charge"        );
	_hm -> addVariable("SR_HT"        , 1000,   0.0, 1000.0, "H_T [GeV]"            ); 
	_hm -> addVariable("SR_MET"       , 1000,   0.0, 1000.0, "#slash{E}_T [GeV]"    );
	_hm -> addVariable("SR_MLL"       , 1000,   0.0, 1000.0, "m_{ll} [GeV]"         ); 
	_hm -> addVariable("SR_NBJets"    ,   20,   0.0,   20.0, "b-jet multiplicity"   );
	_hm -> addVariable("SR_NElectrons",   20,   0.0,   20.0, "electron multiplicity");
	_hm -> addVariable("SR_NJets"     ,   20,   0.0,   20.0, "jet multiplicity"     );
	_hm -> addVariable("SR_NLeps"     ,   20,   0.0,   20.0, "lepton multiplicity"  );
	_hm -> addVariable("SR_NMuons"    ,   20,   0.0,   20.0, "muon multiplicity"    );
	_hm -> addVariable("SR_NVrtx"     ,   40,   0.0,   40.0, "vertex multiplicity"  );
	_hm -> addVariable("SR_ElDXY"     ,   50,   0.0,    0.5, "#||{dxy}(e) [cm]"     );
	_hm -> addVariable("SR_ElEta"     ,  240,   0.0,    2.4, "#||{#eta(e)}"         );
	_hm -> addVariable("SR_ElIso"     ,   50,   0.0,    1.0, "PF Iso (e)"           );
	_hm -> addVariable("SR_ElMT"      , 1000,   0.0, 1000.0, "M_T(e) [GeV]"         );
	_hm -> addVariable("SR_ElPt"      , 1000,   0.0, 1000.0, "P_T(e) [GeV]"         );
	_hm -> addVariable("SR_MuDXY"     ,   50,   0.0,    0.5, "#||{dxy}(#mu) [cm]"   );
	_hm -> addVariable("SR_MuEta"     ,  240,   0.0,    2.4, "#||{#eta(#mu)}"       );
	_hm -> addVariable("SR_MuIso"     ,   50,   0.0,    1.0, "PF Iso (#mu)"         );
	_hm -> addVariable("SR_MuMT"      , 1000,   0.0, 1000.0, "M_T(#mu) [GeV]"       );
	_hm -> addVariable("SR_MuPt"      , 1000,   0.0, 1000.0, "P_T(#mu) [GeV]"       );
	_hm -> addVariable("SR_JetCSVBTag",   50,   0.0,    1.0, "jet CSV B-Tag"        );
	_hm -> addVariable("SR_JetPt"     , 1000,   0.0, 1000.0, "P_T(jet) [GeV]"       );

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

	_hm -> saveHistos ("SUSYSSDL", _ConfigName);
	_au -> saveNumbers("SUSYSSDL", _ConfigName);

}


// if adding variables int he skimming tree is needed...
void SUSYSSDL::modifySkimming(){
}


/*****************************************************************************
******************************************************************************
** OBJECT SELECTIONS                                                        **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool SUSYSSDL::bJetSelection(int jetIdx){
	/*
  	does the selection of  b-jets
  	parameters: jetIdx
  	return: true (if the jet is a b-jet), false (else)
  	*/
	
  counter("BJetDenominator", kBJetId);

	if(!makeCut(goodJetSelection(jetIdx), "jet Id", "=", kBJetId) ) return false;
	if(!makeCut<float>(_vc -> getF(_jet + "_btagCSV", jetIdx), 0.679, ">=", "csv btag selection", 0, kBJetId) ) return false;
	
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
	
	for(int i = 0; i < _vc -> getI(_lepnum); ++i){
		if(fabs(_vc -> getI(_lep + "_pdgId", i)) == 11){
			if(electronSelection(i))     _KinObj["Electron"]    .push_back(i);
			if(vetoElectronSelection(i)) _KinObj["VetoElectron"].push_back(i);
		 }
		if(fabs(_vc -> getI(_lep + "_pdgId", i)) == 13){
		   if(muonSelection(i))     _KinObj["Muon"]    .push_back(i);
		   if(vetoMuonSelection(i)) _KinObj["VetoMuon"].push_back(i);
		}
	}
	
	for(int i = 0; i < _vc -> getI(_jetnum); ++i){
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
bool SUSYSSDL::goodJetSelection(int jetIdx){
	/*
  	does the selection of good jets, i.e. minimum selection of jets 
  	parameters: jetIdx
  	return: true (if the jet is good), false (else)
  	*/

	float jet_pt = (_JEC == 1 ? _vc -> getF(_jet + "_pt", jetIdx) : _vc -> getF(_jet + "_rawPt", jetIdx));

	counter("JetDenominator", kJetId);

	if(!makeCut<float>(jet_pt                                  , 40.0, ">", "pt selection" , 0, kJetId) ) return false;
	if(!makeCut<float>(fabs(_vc -> getF(_jet + "_eta", jetIdx)),  2.4, "<", "eta selection", 0, kJetId) ) return false;


	// CH: here we require dR(j, selected lep) > 0.4, which
	// makes jet-lepton cleaning obsolete
	for(int el = 0; el < _NumKinObj["Electron"]; ++el){
		float dr = Tools::dR(_vc -> getF(_lep + "_eta", _KinObj["Electron"][el]), _vc -> getF(_jet + "_eta", jetIdx), _vc -> getF(_lep + "_phi", _KinObj["Electron"][el]), _vc -> getF(_jet + "_phi", jetIdx));
		if(!makeCut<float>(dr, 0.4, ">", "dR selection (el)", 0, kJetId) ) return false;
	}	

	for(int mu = 0; mu < _NumKinObj["Muon"]; ++mu){
		float dr = Tools::dR(_vc -> getF(_lep + "_eta", _KinObj["Muon"][mu]), _vc -> getF(_jet + "_eta", jetIdx), _vc -> getF(_lep + "_phi", _KinObj["Muon"][mu]), _vc -> getF(_jet + "_phi", jetIdx)); 
		if(!makeCut<float>(dr, 0.4, ">", "dR selection (mu)", 0, kJetId) ) return false;
	}


	// CH: this is the implementation of jet lepton cleaning
	// maybe we find a better way => matthieu's library?

//	bool is_closest = false;
//		
//	// clean w/r/t loose electrons
//	for(int el = 0; el < _NumKinObj["Electron"]; ++el){
//						
//		float closest_dr  = 99.;
//		int   closest_jet =  -1;
//		
//		for(int jet = 0; jet < _vc -> getI(_jetnum); ++jet){
//			float dr = Tools::dR(_vc -> getF(_lep + "_eta", _KinObj["Electron"][el]), _vc -> getF(_jet + "_eta", jet), _vc -> getF(_lep + "_phi", _KinObj["Electrons"][el]), _vc -> getF(_jet + "_phi", jet));
//			if(dr < closest_dr) {
//				closest_dr  = dr;
//				closest_jet = jet;
//			}
//		}
//		
//		if(closest_jet == jetIdx && closest_dr < min_dR) is_closest = true;
//	
//	}
//
//	if(makeCut(is_closest, "jet-lepton cleaning (el)", "=", kJetId)) return false;
//	
//	// clean w/r/t loose muons 
//	for(int mu = 0; mu < _NumKinObj["Muon"]; ++mu){
//		
//		float closest_dr  = 99.; 
//		int   closest_jet =  -1; 
//		
//		for(int jet = 0; jet < _vc -> getI(_jetnum); ++jet){ 
//			float dr = Tools::dR(_vc -> getF(_lep + "_eta", _KinObj["Muon"][mu]), _vc -> getF(_jet + "_eta", jet), _vc -> getF(_lep + "_phi", _KinObj["Muon"][mu]), _vc -> getF(_jet + "_phi", jet)); 
//			if(dr < closest_dr) { 
//				closest_dr  = dr; 
//				closest_jet = jet; 
//			} 
//		} 
//		
//		if(closest_jet == jetIdx && closest_dr < min_dR) is_closest = true; 
//	
//	} 
//		
//	if(makeCut(is_closest, "jet-lepton cleaning (mu)", "=", kJetId)) return false;

	return true;

}


//____________________________________________________________________________
bool SUSYSSDL::electronSelection(int elIdx){
	/*
  	does the selection of electrons
  	parameters: elIdx
  	return: true (if the electron is an electron), false (else)
  	*/

	counter("ElectronDenominator", kElId);

	float pt_cut = 10.;
	if(_PT == "highpt") pt_cut = 20.;

	if(!makeCut<int>(       _vc -> getI(_lep + "_eleCutIdCSA14_50ns_v1", elIdx) ,  3     , ">=" , "POG CB WP-M Id ", 0    , kElId)) return false;
	//if(!makeCut<int>(       _vc -> getI(_lep + "_mvaNew"               , elIdx) ,  0.93  , ">=" , "POG MVA Id "    , 0    , kElId)) return false;
	if(!makeCut<float>(     _vc -> getF(_lep + "_pt"                   , elIdx) , pt_cut , ">"  , "pt selection"   , 0    , kElId)) return false;
	if(!makeCut<float>(fabs(_vc -> getF(_lep + "_eta"                  , elIdx)),  2.4   , "<"  , "eta selection"  , 0    , kElId)) return false;
	if(!makeCut<float>(fabs(_vc -> getF(_lep + "_eta"                  , elIdx)),  1.4442, "[!]", "eta selection"  , 1.566, kElId)) return false;
	if(!makeCut<float>(fabs(_vc -> getF(_lep + "_dz"                   , elIdx)),  0.2   , "<"  , "dz selection"   , 0    , kElId)) return false;
	if(!makeCut<float>(fabs(_vc -> getF(_lep + "_dxy"                  , elIdx)),  0.01  , "<"  , "dxy selection"  , 0    , kElId)) return false;

	return true;

}


//____________________________________________________________________________
bool SUSYSSDL::muonSelection(int muIdx){
	/*
  	does the selection of muons
  	parameters: muIdx
  	return: true (if the muon is a muon), false (else)
  	*/

	counter("MuonDenominator", kMuId);
	
	float pt_cut = 10.;
	if(_PT == "highpt") pt_cut = 20.;

	if(!makeCut<int>(       _vc -> getI(_lep + "_tightId", muIdx) ,  1    , "=", "POG Tight Id ", 0, kMuId)) return false;
	if(!makeCut<float>(     _vc -> getF(_lep + "_pt"     , muIdx) , pt_cut, ">", "pt selection" , 0, kMuId)) return false;
	if(!makeCut<float>(fabs(_vc -> getF(_lep + "_eta"    , muIdx)),  2.4  , "<", "eta selection", 0, kMuId)) return false;
	if(!makeCut<float>(fabs(_vc -> getF(_lep + "_dz"     , muIdx)),  0.2  , "<", "dz selection" , 0, kMuId)) return false;
	if(!makeCut<float>(fabs(_vc -> getF(_lep + "_dxy"    , muIdx)),  0.005, "<", "dxy selection", 0, kMuId)) return false;  

	return true;

}


//____________________________________________________________________________
bool SUSYSSDL::vetoElectronSelection(int elIdx){
	/*
  	does the selection of veto electrons
  	parameters: elIdx
  	return: true (if the electron is a veto electron), false (else)
  	*/

	counter("vetoElDenominator", kElVeto);

	if(!makeCut(!electronSelection(elIdx), "no veto electron", "=", kElVeto) ) return false;

	if(!makeCut<int>(   _vc -> getI(_lep + "_eleCutIdCSA14_50ns_v1", elIdx), 3.0 , ">=", "POG CB WP-M Id ", 0, kElVeto)) return false;
	//if(!makeCut<int>(   _vc -> getI(_lep + "_mvaNew"               , elIdx), 0.93, ">=", "POG MVA Id "    , 0, kElVeto)) return false;
	if(!makeCut<float>( _vc -> getF(_lep + "_pt"                   , elIdx), 5.0 , ">" , "pt selection"   , 0, kElVeto)) return false;
  
	return true;

}


//____________________________________________________________________________
bool SUSYSSDL::vetoMuonSelection(int muIdx){
	/*
  	does the selection of veto muons
  	parameters: muIdx     
  	return: true (if the muon is a veto muon), false (else)
  	*/

	counter("VetoMuonDenominator", kMuVeto);

	if(!makeCut(!muonSelection(muIdx), "no veto muon", "=", kMuVeto) ) return false;
	if(!makeCut<int>(   _vc -> getI(_lep + "_tightId", muIdx), 1  , "=", "POG Tight Id", 0, kMuVeto) ) return false;
	if(!makeCut<float>( _vc -> getF(_lep + "_pt"     , muIdx), 5.0, ">", "pt selection", 0, kMuVeto ) ) return false;

	return true;

}






/*****************************************************************************
******************************************************************************
** KINEMATIC REGION DEFINITIONS                                             **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void SUSYSSDL::setBaselineRegion(){
	/*
	sets the cuts of the baseline region (_BR) for high- and low-pt analyses (_PT)
	parameters: none
	return: none
	*/
  
 	if(_BR == "BR00" && _PT == "lowpt") {
		setCut("HTBR"     ,  250, ">" );
		setCut("HTCondBR" ,  500, "<" );
		setCut("METHighBR",   30, ">=");
		setCut("METLowBR" ,    0, ">=");
		setCut("NJetsBR"  ,    2, ">=");
		setCut("NBJetsBR" ,    0, "=" );
		setCut("CHBR"     , -100, ">" );
	}	
	else if(_BR == "BR01" && _PT == "lowpt") {
		setCut("HTBR"     ,  250, ">" );
		setCut("HTCondBR" ,  500, "<" );
		setCut("METHighBR",   30, ">=");
		setCut("METLowBR" ,    0, ">=");
		setCut("NJetsBR"  ,    2, ">=");
		setCut("NBJetsBR" ,    1, "=" );
		setCut("CHBR"     , -100, ">" );
	}
	else if(_BR == "BR02" && _PT == "lowpt") {
		setCut("HTBR"     ,  250, ">" );
		setCut("HTCondBR" ,  500, "<" );
		setCut("METHighBR",   30, ">=");
		setCut("METLowBR" ,    0, ">=");
		setCut("NJetsBR"  ,    2, ">=");
		setCut("NBJetsBR" ,    2, ">=");
		setCut("CHBR"     , -100, ">" );
	}

 	else if(_BR == "BR00" && _PT == "highpt") {
		setCut("HTBR"     ,   80, ">" );
		setCut("HTCondBR" ,  500, "<" );
		setCut("METHighBR",   30, ">=");
		setCut("METLowBR" ,    0, ">=");
		setCut("NJetsBR"  ,    2, ">=");
		setCut("NBJetsBR" ,    0, "=" );
		setCut("CHBR"     , -100, ">" );
	}	
	else if(_BR == "BR01" && _PT == "highpt") {
		setCut("HTBR"     ,   80, ">" );
		setCut("HTCondBR" ,  500, "<" );
		setCut("METHighBR",   30, ">=");
		setCut("METLowBR" ,    0, ">=");
		setCut("NJetsBR"  ,    2, ">=");
		setCut("NBJetsBR" ,    1, "=" );
		setCut("CHBR"     , -100, ">" );
	}
	else if(_BR == "BR02" && _PT == "highpt") {
		setCut("HTBR"     ,   80, ">" );
		setCut("HTCondBR" ,  500, "<" );
		setCut("METHighBR",   30, ">=");
		setCut("METLowBR" ,    0, ">=");
		setCut("NJetsBR"  ,    2, ">=");
		setCut("NBJetsBR" ,    2, ">=");
		setCut("CHBR"     , -100, ">" );
	}
}


//____________________________________________________________________________
void SUSYSSDL::setCut(std::string var, float valCut, std::string cType, float upValCut) {
	/*
	sets the parameters (valCut, cType, upValCut) for a specific cut on a variable (var)
	parameters: var, valCut (the cut value), cType (the cut type), upValCut (the upper value
	            in case one cuts the variable in a range)
	return: none
	*/


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
	
	else if(var == "CHSR") {
		_valCutCHSR   = valCut;
		_cTypeCHSR    = cType;
		_upValCutCHSR = upValCut;
	} 


	// baseline region

	else if(var == "HTBR") {
		_valCutHTBR   = valCut;
		_cTypeHTBR    = cType;
		_upValCutHTBR = upValCut;
	} 

	else if(var == "HTCondBR") {
		_valCutHTCondBR   = valCut;
		_cTypeHTCondBR    = cType;
		_upValCutHTCondBR = upValCut;
	} 
	
	else if(var == "METLowBR") {
		_valCutMETLowBR   = valCut;
		_cTypeMETLowBR    = cType;
		_upValCutMETLowBR = upValCut;
	} 
	
	else if(var == "METHighBR") {
		_valCutMETHighBR   = valCut;
		_cTypeMETHighBR    = cType;
		_upValCutMETHighBR = upValCut;
	} 
	
	else if(var == "NJetsBR") {
		_valCutNJetsBR   = valCut;
		_cTypeNJetsBR    = cType;
		_upValCutNJetsBR = upValCut;
	} 
	
	else if(var == "NBJetsBR") {
		_valCutNBJetsBR   = valCut;
		_cTypeNBJetsBR    = cType;
		_upValCutNBJetsBR = upValCut;
	} 
	
	else if(var == "CHBR") {
		_valCutCHBR  = valCut;
		_cTypeCHBR    = cType;
		_upValCutCHBR = upValCut;
	} 

}



//____________________________________________________________________________
void SUSYSSDL::setSignalRegion() {
	/*
	sets the cuts of the signal region (_SR)
	parameters: none
	return: none
	*/

 	if(_SR == "SR01") {
		setCut("HTSR", 200, "[]", 400 );
		setCut("METSR", 50, "[]", 120 );
		setCut("NJetsSR", 2, "[]", 3 );
		setCut("NBJetsSR", 0, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR02") {
		setCut("HTSR", 400, ">" );
		setCut("METSR", 50, "[]", 120 );
		setCut("NJetsSR", 2, "[]", 3 );
		setCut("NBJetsSR", 0, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR03") {
		setCut("HTSR", 200, "[]", 400 );
		setCut("METSR", 50, "[]", 120 );
		setCut("NJetsSR", 4, ">=" );
		setCut("NBJetsSR", 0, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR04") {
		setCut("HTSR", 400, ">" );
		setCut("METSR", 50, "[]", 120 );
		setCut("NJetsSR", 4, ">=" );
		setCut("NBJetsSR", 0, "=" );
		setCut("CHSR", -100, ">" );
	}
 	else if(_SR == "SR05") {
		setCut("HTSR", 200, "[]", 400 );
		setCut("METSR", 120, ">" );
		setCut("NJetsSR", 2, "[]", 3 );
		setCut("NBJetsSR", 0, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR06") {
		setCut("HTSR", 400, ">" );
		setCut("METSR", 120, ">" );
		setCut("NJetsSR", 2, "[]", 3 );
		setCut("NBJetsSR", 0, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR07") {
		setCut("HTSR", 200, "[]", 400 );
		setCut("METSR", 120, ">" );
		setCut("NJetsSR", 4, ">=" );
		setCut("NBJetsSR", 0, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR08") {
		setCut("HTSR", 400, ">" );
		setCut("METSR", 120, ">" );
		setCut("NJetsSR", 4, ">=" );
		setCut("NBJetsSR", 0, "=" );
		setCut("CHSR", -100, ">" );
	}



 	else if(_SR == "SR11") {
		setCut("HTSR", 200, "[]", 400 );
		setCut("METSR", 50, "[]", 120 );
		setCut("NJetsSR", 2, "[]", 3 );
		setCut("NBJetsSR", 1, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR12") {
		setCut("HTSR", 400, ">" );
		setCut("METSR", 50, "[]", 120 );
		setCut("NJetsSR", 2, "[]", 3 );
		setCut("NBJetsSR", 1, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR13") {
		setCut("HTSR", 200, "[]", 400 );
		setCut("METSR", 50, "[]", 120 );
		setCut("NJetsSR", 4, ">=" );
		setCut("NBJetsSR", 1, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR14") {
		setCut("HTSR", 400, ">" );
		setCut("METSR", 50, "[]", 120 );
		setCut("NJetsSR", 4, ">=" );
		setCut("NBJetsSR", 1, "=" );
		setCut("CHSR", -100, ">" );
	}
 	else if(_SR == "SR15") {
		setCut("HTSR", 200, "[]", 400 );
		setCut("METSR", 120, ">" );
		setCut("NJetsSR", 2, "[]", 3 );
		setCut("NBJetsSR", 1, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR16") {
		setCut("HTSR", 400, ">" );
		setCut("METSR", 120, ">" );
		setCut("NJetsSR", 2, "[]", 3 );
		setCut("NBJetsSR", 1, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR17") {
		setCut("HTSR", 200, "[]", 400 );
		setCut("METSR", 120, ">" );
		setCut("NJetsSR", 4, ">=" );
		setCut("NBJetsSR", 1, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR18") {
		setCut("HTSR", 400, ">" );
		setCut("METSR", 120, ">" );
		setCut("NJetsSR", 4, ">=" );
		setCut("NBJetsSR", 1, "=" );
		setCut("CHSR", -100, ">" );
	}



 	else if(_SR == "SR21") {
		setCut("HTSR", 200, "[]", 400 );
		setCut("METSR", 50, "[]", 120 );
		setCut("NJetsSR", 2, "[]", 3 );
		setCut("NBJetsSR", 2, ">=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR22") {
		setCut("HTSR", 400, ">" );
		setCut("METSR", 50, "[]", 120 );
		setCut("NJetsSR", 2, "[]", 3 );
		setCut("NBJetsSR", 2, ">=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR23") {
		setCut("HTSR", 200, "[]", 400 );
		setCut("METSR", 50, "[]", 120 );
		setCut("NJetsSR", 4, ">=" );
		setCut("NBJetsSR", 2, ">=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR24") {
		setCut("HTSR", 400, ">" );
		setCut("METSR", 50, "[]", 120 );
		setCut("NJetsSR", 4, ">=" );
		setCut("NBJetsSR", 2, ">=" );
		setCut("CHSR", -100, ">" );
	}
 	else if(_SR == "SR25") {
		setCut("HTSR", 200, "[]", 400 );
		setCut("METSR", 120, ">" );
		setCut("NJetsSR", 2, "[]", 3 );
		setCut("NBJetsSR", 2, ">=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR26") {
		setCut("HTSR", 400, ">" );
		setCut("METSR", 120, ">" );
		setCut("NJetsSR", 2, "[]", 3 );
		setCut("NBJetsSR", 2, ">=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR27") {
		setCut("HTSR", 200, "[]", 400 );
		setCut("METSR", 120, ">" );
		setCut("NJetsSR", 4, ">=" );
		setCut("NBJetsSR", 2, ">=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SR28") {
		setCut("HTSR", 400, ">" );
		setCut("METSR", 120, ">" );
		setCut("NJetsSR", 4, ">=" );
		setCut("NBJetsSR", 2, ">=" );
		setCut("CHSR", -100, ">" );
	}



	else if(_SR == "SRRPV0") {
		setCut("HTSR", 500, ">" );
		setCut("METSR", 0, ">=" );
		setCut("NJetsSR", 2, ">=" );
		setCut("NBJetsSR", 0, ">=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SRRPV2") {
		setCut("HTSR", 500, ">" );
		setCut("METSR", 0, ">=" );
		setCut("NJetsSR", 2, ">=" );
		setCut("NBJetsSR", 2, ">=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SRSStop1") {
		setCut("HTSR", 80, ">" );
		setCut("METSR", 30, ">=" );
		setCut("NJetsSR", 2, ">=" );
		setCut("NBJetsSR", 1, "=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SRSStop1++") {
		setCut("HTSR", 80, ">" );
		setCut("METSR", 30, ">=" );
		setCut("NJetsSR", 2, ">=" );
		setCut("NBJetsSR", 1, "=" );
		setCut("CHSR", 2, "=" );
	}
	else if(_SR == "SRSStop2") {
		setCut("HTSR", 80, ">" );
		setCut("METSR", 30, ">=" );
		setCut("NJetsSR", 2, ">=" );
		setCut("NBJetsSR", 2, ">=" );
		setCut("CHSR", -100, ">" );
	}
	else if(_SR == "SRSStop2++") {
		setCut("HTSR", 80, ">" );
		setCut("METSR", 30, ">=" );
		setCut("NJetsSR", 2, ">=" );
		setCut("NBJetsSR", 2, ">=" );
		setCut("CHSR", 2, "=" );
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





/*****************************************************************************
******************************************************************************
** EVENT SELECTIONS                                                         **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool SUSYSSDL::baseSelection(){
	/*
	implements the base selection that is fundamental for both the baseline 
	and the signal region selections
	parameters: none
	return: true (if event passes selection), false (else)
	*/
  
  counter("denominator");

	if(_isData && !makeCut<int>(_vc -> getI("HLT_DoubleMu"), 1, "=", "HLT DoubleMu") ) return false;	
	if(_isData && !makeCut<int>(_vc -> getI("HLT_DoubleEl"), 1, "=", "HLT DoubleEl") ) return false;	
	if(_isData && !makeCut<int>(_vc -> getI("HLT_MuEG")    , 1, "=", "HLT MuEG"    ) ) return false;	

	if(!makeCut<int>( _NumKinObj["Electron"] + _NumKinObj["Muon"], 2, "=", "lepton multiplicity" ) ) return false; 
	
	bool is_3l_event = vetoEventSelection("Electron", "Muon");
	bool is_ss_event = ssEventSelection  ("Electron", "Muon");
	//bool is_lowpt_event = lowptEventSelection("Electron", "Muon");

	if(!makeCut( is_ss_event , "same-sign selection", "=") ) return false;
	if(!makeCut( !is_3l_event, "veto on 3 leptons"  , "=") ) return false;
	//if(_PT == "lowpt" && !makeCut( is_lowpt_event, "lowpt 20-10 leptons", "=") ) return false;
	
	if(!makeCut<float>( findMLL("Electron", "Muon"), 8.0, ">", "MLL selection") ) return false;

	return true;

}


//____________________________________________________________________________
bool SUSYSSDL::brSelection(){
	/*
	implements the baseline region selection provided that the base selection
	already has been applied, i.e. here we just cut on the variables that are
	different among the baseline region selections
	parameters: none
	return: true (if event passes selection), false (else)
	*/

	if(!makeCut<float>( HT("GoodJet")                 , _valCutHTBR    , _cTypeHTBR , "BR HT selection"       , _upValCutHTBR      ) ) return false;
	if(_au -> simpleCut(HT("GoodJet"), _valCutHTCondBR, _cTypeHTCondBR) ) {
		if(!makeCut<float>( _vc -> getF("met_pt"), _valCutMETHighBR, _cTypeMETHighBR, "BR MET high selection", _upValCutMETHighBR ) ) return false;
	}
	else {
		if(!makeCut<float>( _vc -> getF("met_pt"), _valCutMETLowBR , _cTypeMETLowBR , "BR MET low selection" , _upValCutMETLowBR ) ) return false;
	}

	if(!makeCut<int>( _NumKinObj["GoodJet"]         , _valCutNJetsBR , _cTypeNJetsBR , "BR jet multiplicity"  , _upValCutNJetsBR ) ) return false;
	if(!makeCut<int>( _NumKinObj["BJet"]            , _valCutNBJetsBR, _cTypeNBJetsBR, "BR b-jet multiplicity", _upValCutNBJetsBR) ) return false;
	if(!makeCut<int>( findCharge("Electron", "Muon"), _valCutCHBR    , _cTypeCHBR    , "BR charge selection"  , _upValCutCHBR    ) ) return false;

	return true;

} 


//____________________________________________________________________________
bool SUSYSSDL::lowptEventSelection(std::string electron_label, std::string muon_label, std::string kr){
	/*
	checks, in the case of lowpt analysis, that the leptons are 20-10 in terms of pt
  	parameters: electron_label, muon_label (the labels of the selected electron and muon objects), kr
  	return: true (if the leptons are 20-10), false (else)
  	*/

	if(_PT != "lowpt") return false;
	if(_NumKinObj[electron_label] + _NumKinObj[muon_label] != 2) return false;

	float pts[2] = {0.0, 0.0};
	int idx = 0;

	for(int ie = 0; ie < _NumKinObj[electron_label]; ++ie){
		pts[ie] = _vc -> getF(_lep + "_pt", _KinObj[electron_label][ie]);
		++idx;
	}

	for(int im = 0; im < _NumKinObj[muon_label]; ++im)
		pts[idx + im] = _vc -> getF(_lep + "_pt", _KinObj[muon_label][im]);

	std::sort(pts, pts + sizeof(pts)/sizeof(pts[0]));

	// we did the > 10 selection already in the object selection
	// due to sort, the last element is the high-pt lepton in the pair
	if(pts[1] > 20) return true; 

	return false;

}

//____________________________________________________________________________
bool SUSYSSDL::srSelection(){
	/*
	implements the signal region selection provided that the base selection
	already has been applied, i.e. here we just cut on the variables that are
	different among the signal region selections
	parameters: none
	return: true (if event passes selection), false (else)
	*/


	//  counter("denominator",_srName);
	if(!makeCut<float>( HT("GoodJet")                 , _valCutHTSR    , _cTypeHTSR    , "SR HT selection"     , _upValCutHTSR    ) ) return false;
	if(!makeCut<float>( _vc -> getF("met_pt")         , _valCutMETSR   , _cTypeMETSR   , "SR MET selection"    , _upValCutMETSR   ) ) return false;
	if(!makeCut<float>( _NumKinObj["GoodJet"]         , _valCutNJetsSR , _cTypeNJetsSR , "SR jet multiplicity" , _upValCutNJetsSR ) ) return false;
	if(!makeCut<float>( _NumKinObj["BJet"]            , _valCutNBJetsSR, _cTypeNBJetsSR, "SR bjet multiplicity", _upValCutNBJetsSR) ) return false;
	if(!makeCut<float>( findCharge("Electron", "Muon"), _valCutCHSR    , _cTypeCHSR    , "SR charge selection" , _upValCutCHSR    ) ) return false;

	return true;

} 


//____________________________________________________________________________
bool SUSYSSDL::ssEventSelection(std::string electron_label, std::string muon_label, std::string kr){
	/*
	checks, if the leptons that have been found in the kinematic region (kr) are same-sign
  	parameters: electron_label, muon_label (the labels of the selected electron and muon objects), kr
  	return: true (if the leptons all have same-sign), false (else)
  	*/

	int charge = 0;

	for(int ie = 0; ie < _NumKinObj[electron_label]; ++ie){
		if(charge == 0) charge = _vc -> getI(_lep + "_charge", _KinObj[electron_label][ie]);
		if(_vc -> getI(_lep + "_charge", _KinObj[electron_label][ie]) != charge) return false;

		CandStruct el;
		el.eta = _vc -> getF(_lep + "_eta", _KinObj[electron_label][ie]);
		el.phi = _vc -> getF(_lep + "_phi", _KinObj[electron_label][ie]);
		el.pt = _vc -> getF(_lep + "_pt", _KinObj[electron_label][ie]);
		el.charge = _vc -> getI(_lep + "_charge", _KinObj[electron_label][ie]);
		el.pdgId = _vc -> getI(_lep + "_pdgId", _KinObj[electron_label][ie]);
		_leptons.push_back(el);
	}
	
	for(int im = 0; im < _NumKinObj[muon_label]; ++im){
		if(charge == 0) charge = _vc -> getI(_lep + "_charge", _KinObj[muon_label][im]);
		if(_vc -> getI(_lep + "_charge", _KinObj[muon_label][im]) != charge) return false;

		CandStruct mu;
		mu.eta = _vc -> getF(_lep + "_eta", _KinObj[muon_label][im]);
		mu.phi = _vc -> getF(_lep + "_phi", _KinObj[muon_label][im]);
		mu.pt = _vc -> getF(_lep + "_pt", _KinObj[muon_label][im]);
		mu.charge = _vc -> getI(_lep + "_charge", _KinObj[muon_label][im]);
		mu.pdgId = _vc -> getI(_lep + "_pdgId", _KinObj[muon_label][im]);
		_leptons.push_back(mu);
	}

	return true;

}


//____________________________________________________________________________
bool SUSYSSDL::vetoEventSelection(std::string electron_label, std::string muon_label, std::string kr){
	/*
  	performs an essential part of the event selection in the 3l case, i.e.
  	events are rejected if there are 3 leptons where two leptons form an
  	opposite-sign same-flavor pair with some condition on mll and pt
  	parameters: electron_label, muon_label (the labels of the selected electron and muon objects)
  	return: true (if the event has such a pair, i.e. if the event shall be rejected), false (else)
  	*/

	counter("denominator", kVetoLepSel);
	
	if(!makeCut<int>(_NumKinObj[electron_label] + _NumKinObj[muon_label], 2, "=", "lep mult"     , 0, kVetoLepSel) ) return false; 
	if(!makeCut<int>(_NumKinObj["VetoElectron"] + _NumKinObj["VetoMuon"], 1, "=", "veto lep mult", 0, kVetoLepSel) ) return false; 


	// two electrons
	if(_NumKinObj[electron_label] >= 1 && _NumKinObj["VetoElectron"] == 1){

		// checking charge
		int charge = _vc -> getI(_lep + "_charge", _KinObj["VetoElectron"][0]);
		bool os = false;
		int os_el_index = -1;
		for(int el = 0; el < _NumKinObj[electron_label]; ++el){
			if(_vc -> getI(_lep + "_charge", _KinObj[electron_label][el]) != charge){
				os = true;
				os_el_index = _KinObj[electron_label][el];
			}
		}
		
		// there is an os sf pair
		if(os){	
			float mll = MLL("electron", _KinObj["VetoElectron"][0], "electron", os_el_index);
			if(makeCut(_vc -> getF(_lep + "_pt", _KinObj["VetoElectron"][0]) > 5.0  &&  mll < 12.0, "electron mll low pt veto", "=", kVetoLepSel) ) return true;
			if(makeCut(_vc -> getF(_lep + "_pt", _KinObj["VetoElectron"][0]) > 10.0 && (mll > 76.0 && mll < 106.0), "electron mll high pt veto", "=", kVetoLepSel) ) return true;
		}
	}

	// two muons
	if(_NumKinObj[muon_label] >= 1 && _NumKinObj["VetoMuon"] == 1){

		// checking charge
		int charge = _vc -> getI(_lep + "_charge", _KinObj["VetoMuon"][0]);
		bool os = false;
		int os_mu_index = -1;
		for(int mu = 0; mu < _NumKinObj[muon_label]; ++mu){
			if(_vc -> getI(_lep + "_charge", _KinObj[muon_label][mu]) != charge){
				os = true;
				os_mu_index = _KinObj[muon_label][mu];
			}
		}
		
		// there is an os sf pair
		if(os){
			float mll = MLL("muon", _KinObj["VetoMuon"][0], "muon", os_mu_index);
			if(makeCut(_vc -> getF(_lep + "_pt", _KinObj["VetoMuon"][0]) > 5.0  && mll < 12.0, "muon mll low pt veto", "=", kVetoLepSel) ) return true;
			if(makeCut(_vc -> getF(_lep + "_pt", _KinObj["VetoMuon"][0]) > 10.0 && (mll > 76.0 && mll < 106.0), "muon mll high pt veto", "=", kVetoLepSel) ) return true;
		}
	}
	

	makeCut(true, "no veto event", "=", kVetoLepSel);

	return false;


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

	fill(kr + "_LepCharge" , findCharge("Electron", "Muon")                   , _EventWeight);
	fill(kr + "_HT"        , HT("GoodJet")                                    , _EventWeight);
	fill(kr + "_MET"       , _vc -> getF("met_pt")                            , _EventWeight);
	fill(kr + "_MLL"       , findMLL("Electron", "Muon")                      , _EventWeight);
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

	for(int i = 0; i < _NumKinObj["Electron"]; ++i){
		fill(kr + "_ElDXY", fabs(_vc -> getF(_lep + "_dxy"))    , _EventWeight);
		fill(kr + "_ElEta", fabs(_vc -> getF(_lep + "_eta"))    , _EventWeight);
		fill(kr + "_ElIso",      _vc -> getF(_lep + "_relIso04"), _EventWeight);
		fill(kr + "_ElPt" ,      _vc -> getF(_lep + "_pt")      , _EventWeight);
		fill(kr + "_ElMT" , MT("electron", i)               , _EventWeight);
	}

	for(int i = 0; i < _NumKinObj["Muon"]; ++i){
		fill(kr + "_MuDXY", fabs(_vc -> getF(_lep + "_dxy"))    , _EventWeight);
		fill(kr + "_MuEta", fabs(_vc -> getF(_lep + "_eta"))    , _EventWeight);
		fill(kr + "_MuIso",      _vc -> getF(_lep + "_relIso04"), _EventWeight);
		fill(kr + "_MuPt" ,      _vc -> getF(_lep + "_pt")      , _EventWeight);
		fill(kr + "_MuMT" , MT("muon", i)                   , _EventWeight);
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
		fill(kr + "_JetPt"     , (_JEC == 1 ? _vc -> getF(_jet + "_pt", i) : _vc -> getF(_jet + "_rawPt", i)), _EventWeight);
		fill(kr + "_JetCSVBTag", _vc -> getF(_jet + "_btagCSV", i)                                       , _EventWeight);
	}

}


//____________________________________________________________________________
int SUSYSSDL::genMatchCateg(CandStruct cand) {

	//loop over the number of generated leptons
	int nGenL = _vc -> getI("ngenLep");

	for(int ig = 0; ig < nGenL; ++ig) {
	
		if(Tools::dR(cand.eta, _vc -> getF("genLep_eta", ig),
			 cand.phi, _vc -> getF("genLep_phi", ig) ) < 0.05 ) { //to be tuned
		  
		  // cout<<"matched lepton "<<cand.pdgId<<"  with "<<_vc->getI("genLep_pdgId",ig)<<" !!! "<<cand.pt<<"  "<<cand.eta<<"   "<<cand.phi<<"   "<<Tools::dR(cand.eta, _vc->getF("genLep_eta", ig),
		  // cand.phi, _vc->getF("genLep_phi", ig) )<<endl;
		
			if( (abs(cand.pdgId) != abs(_vc -> getI("genLep_pdgId", ig)) ) && abs(_vc -> getI("genLep_pdgId", ig)) != 13 ) return kMisMatchPdgId; //taus are exception to the rule
			else if(cand.pdgId*_vc -> getI("genLep_pdgId",ig) < 0 ) return kMisChargePdgId; //+*- = -...
			else return kGenMatched;
			
			break;
		} //dr matching
	} //gen loop
	
	return kNoGenMatch;
}
