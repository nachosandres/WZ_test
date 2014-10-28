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


#include "analysis/core/MPAF.hh"
#include "analysis/general/FakeRatio.hh"





/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR RUNNING THE CODE                                       **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
FakeRatio::FakeRatio(std::string configuration_file){
	/* 
	constructs the FakeRatio class 
	parameters: configuration_file
	return: none
	*/

	_Verbose -> Class("FakeRatio");

	std::vector<std::string> modules;
	modules.push_back("fillFakeRatioMaps");

	initialize();
	setAllModules(modules);
	startExecution(configuration_file);

}


//____________________________________________________________________________
FakeRatio::~FakeRatio(){
	/* 
	destructs the FakeRatio class 
	parameters: none
	return: none
	*/

}


//____________________________________________________________________________
void FakeRatio::initialize(){
	/*
	initializes the FakeRatio Class
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
	_vc -> registerVar("el_pt"         , "VF");
	_vc -> registerVar("el_eta"        , "VF");
	_vc -> registerVar("el_phi"        , "VF");
	_vc -> registerVar("el_charge"     , "VI");
	_vc -> registerVar("el_relIso03"   , "VF");
	_vc -> registerVar("el_relIso04"   , "VF");
	_vc -> registerVar("el_dxy"        , "VF");
	_vc -> registerVar("el_dz"         , "VF");
	_vc -> registerVar("el_tightId"    , "VI");
	_vc -> registerVar("el_looseIdSusy", "VI");
	_vc -> registerVar("mu_pt"         , "VF");
	_vc -> registerVar("mu_eta"        , "VF");
	_vc -> registerVar("mu_phi"        , "VF");
	_vc -> registerVar("mu_charge"     , "VI");
	_vc -> registerVar("mu_relIso03"   , "VF");
	_vc -> registerVar("mu_relIso04"   , "VF");
	_vc -> registerVar("mu_dxy"        , "VF");
	_vc -> registerVar("mu_dz"         , "VF");
	_vc -> registerVar("mu_tightId"    , "VI");
	_vc -> registerVar("mu_looseIdSusy", "VI");
	_vc -> registerVar("met_pt"        , "F" );
	_vc -> registerVar("met_eta"       , "F" );
	_vc -> registerVar("met_phi"       , "F" );
	_vc -> registerVar("met_mass"      , "F" );
	_vc -> registerVar("jet_pt"        , "VF");
	_vc -> registerVar("jet_rawPt"     , "VF");
	_vc -> registerVar("jet_eta"       , "VF");
	_vc -> registerVar("jet_phi"       , "VF");
	_vc -> registerVar("jet_mass"      , "VF");
	_vc -> registerVar("jet_btagCSV"   , "VF");



}


//____________________________________________________________________________
void FakeRatio::run(){


	// prepare event selection
	resetKinematicObjects();
	collectKinematicObjects();

	// parse event selection, if true we fill event list, event tree (later) and call the kernel
	if(eventSelection()){
	
		fillEventList();

		// calling the modules
		if(Tools::findInVector(_Modules, (std::string) "fillFakeRatioMaps")) fillFakeRatioMaps();

	}

}



/*****************************************************************************
******************************************************************************
** DEFINE INPUTS AND OUTPUTS                                                **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void FakeRatio::defineOutput(){
	/*
  	defines and reserves all output that is produced in this class
  	parameters: none
  	return: none
  	*/ 

	float FR_bins_eta[6] = { 0.0,  0.5,  1.0,  1.5,  2.0,  2.5};
	float FR_bins_pt[9]  = {10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0};

	bins_eta = Tools::toVector(FR_bins_eta);
	bins_pt  = Tools::toVector(FR_bins_pt);

	_hm -> addVariable("ElFR"    , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)");
	_hm -> addVariable("ElLoose" , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)");
	_hm -> addVariable("ElTight" , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(e)"  , "#||{#eta}(e)");

	_hm -> addVariable("MuFR"    , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");
	_hm -> addVariable("MuLoose" , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");
	_hm -> addVariable("MuTight" , bins_pt.size()-1, bins_pt, bins_eta.size()-1, bins_eta, "P_T(#mu)", "#||{#eta}(#mu)");

}


//____________________________________________________________________________
void FakeRatio::loadInput(){
	/*
  	loads all input from the cache or from the database
  	parameters: none
 	return: none
 	*/


	// define function in MPAF for loading histograms, text files, histograms from database 

}


//____________________________________________________________________________
void FakeRatio::writeOutput(){
	/*
  	writes all output of this class to the disk
 	paramters: none
 	return: none
 	*/


	//ElFR 
	//MuFR

	_hm -> saveHistos("FakeRatio", "testoutput");

}





/*****************************************************************************
******************************************************************************
** OBJECT SELECTIONS                                                        **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
bool FakeRatio::awayJetSelection(int jet_index){
	/*
  	does the selection of away jets
  	parameters: jet_index
  	return: true (if the jet is away), false (else)
  	*/

	if(!goodJetSelection(jet_index)) return false;

	for(unsigned int i = 0; i < _vc -> getSize("mu_eta"); ++i)
		if(_UsedCuts["AwayJetDeltaR"] && Tools::dR(_vc -> getF("jet_eta", jet_index), _vc -> getF("mu_eta", i), _vc -> getF("jet_phi", jet_index), _vc -> getF("mu_phi", i)) < _Cuts["AwayJetDeltaR"]) 
			return false;

	for(unsigned int i = 0; i < _vc -> getSize("el_eta"); ++i) 
		if(_UsedCuts["AwayJetDeltaR"] && Tools::dR(_vc -> getF("jet_eta", jet_index), _vc -> getF("el_eta", i), _vc -> getF("jet_phi", jet_index), _vc -> getF("el_phi", i)) < _Cuts["AwayJetDeltaR"]) 
			return false;
	
	return true;

}


//____________________________________________________________________________
bool FakeRatio::goodJetSelection(int jet_index){
	/*
  	does the selection of good jets, i.e. minimum selection of jets 
  	parameters: jet_index
  	return: true (if the jet is good), false (else)
  	*/

	if(_UsedCuts["GoodJetPt"]   && (_JEC == 1 ? _vc -> getF("jet_pt", jet_index) : _vc -> getF("jet_rawPt", jet_index)) < _Cuts["GoodJetPt"]  ) return false;
	if(_UsedCuts["GoodJetBTag"] &&      _vc -> getF("jet_btagCSV", jet_index)                                           < _Cuts["GoodJetBTag"]) return false;
	if(_UsedCuts["GoodJetEta"]  && fabs(_vc -> getF("jet_eta"    , jet_index))                                          > _Cuts["GoodJetEta"] ) return false;


	// this is the implementation of jet lepton cleaning
	// maybe we find a better way => matthieu's library?
	
	if(_UsedCuts["GoodJetJLCdR"]){
	
		bool is_closest = false;
		
		// clean w/r/t loose electrons
		for(int el = 0; el < _NumKinObj["Electrons"]; ++el){
		
			float closest_dr  = 99.;
			int   closest_jet =  -1;
			
			for(unsigned int jet = 0; jet < _vc -> getSize("jet_pt"); ++jet){
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
		
			for(unsigned int jet = 0; jet < _vc -> getSize("jet_pt"); ++jet){
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
bool FakeRatio::looseElectronSelection(int electron_index){
	/*
  	does the selection of loose electrons
  	parameters: electron_index
  	return: true (if the electron is loose), false (else)
  	*/


	// CH: this is the selection used in ASAnalysis for 8TeV
	// make sure that we use the same for 13TeV.

	//float epVariable = fabs(1/fTR->ElCaloEnergy[ind] - fTR->ElESuperClusterOverP[ind]/fTR->ElCaloEnergy[ind]);
	//if( fabs(fTR->ElSCEta[ind]) <= 1.479 ){ // Barrel
	//	if(fabs(fTR->ElDeltaEtaSuperClusterAtVtx [ind]) > 0.004) return false;
	//	if(fabs(fTR->ElDeltaPhiSuperClusterAtVtx [ind]) > 0.06 ) return false;
	//	if(fTR->ElSigmaIetaIeta [ind] > 0.01 ) return false;
	//	if(fTR->ElHcalOverEcal [ind] > 0.12 ) return false;
	//	if(epVariable > 0.05 ) return false;
	//}
	//if( fabs(fTR->ElSCEta[ind]) > 1.479 && fabs(fTR->ElSCEta[ind]) < 2.5 ){ // Endcap
	//	if(fabs(fTR->ElDeltaEtaSuperClusterAtVtx [ind]) > 0.007) return false;
	//	if(fabs(fTR->ElDeltaPhiSuperClusterAtVtx [ind]) > 0.03 ) return false;
	//	if(fTR->ElSigmaIetaIeta [ind] > 0.03 ) return false;
	//	if(fTR->ElHcalOverEcal [ind] > 0.10 ) return false;
	//	if(epVariable > 0.05 ) return false;
	//}
	//// add the conversion rejection here
	//// if (!fTR->ElPassConversionVeto[ind] ) return false;
	//// if (fTR->ElNumberOfMissingInnerHits[ind] > 0 ) return false;
	//// if (fTR->ElCInfoIsGsfCtfScPixCons[ind] != 1 ) return false;
	//// if(fabs(fTR->ElDzPV[ind]) > 0.1) return false; 


	if(_UsedCuts["LooseElectronPt"]     && _vc -> getF("el_pt"      , electron_index) < _Cuts["LooseElectronPt"]    ) return false;
	if(_UsedCuts["LooseElectronRelIso"] && _vc -> getF("el_relIso04", electron_index) > _Cuts["LooseElectronRelIso"]) return false;
	
	return true;

}


//____________________________________________________________________________
bool FakeRatio::looseMuonSelection(int muon_index){
	/*
  	does the selection of loose muons
  	parameters: muon_index
  	return: true (if the muon is loose), false (else)
  	*/


	if(_UsedCuts["LooseMuonTightId"] &&      _vc -> getI("mu_tightId" , muon_index) != _Cuts["LooseMuonTightId"]) return false;
	if(_UsedCuts["LooseMuonPt"]      &&      _vc -> getF("mu_pt"      , muon_index)  < _Cuts["LooseMuonPt"]     ) return false;
	if(_UsedCuts["LooseMuonRelIso"]  &&      _vc -> getF("mu_relIso04", muon_index)  > _Cuts["LooseMuonRelIso"] ) return false;
	if(_UsedCuts["LooseMuonDXY"]     && fabs(_vc -> getF("mu_dxy"     , muon_index)) > _Cuts["LooseMuonDXY"]    ) return false;
	if(_UsedCuts["LooseMuonDZ"]      && fabs(_vc -> getF("mu_dz"      , muon_index)) > _Cuts["LooseMuonDZ"]     ) return false;
	
	return true;

}


//____________________________________________________________________________
bool FakeRatio::tightElectronSelection(int electron_index){
	/*
  	does the selection of tight electrons
  	parameters: electron_index
  	return: true (if the electron is tight), false (else)
  	*/

	if(!looseElectronSelection(electron_index)) return false; 
	if(_UsedCuts["TightElectronDXY"]    && fabs(_vc -> getF("el_dxy"     , electron_index)) > _Cuts["TightElectronDXY"]   ) return false;
	if(_UsedCuts["TightElectronRelIso"] &&      _vc -> getF("el_relIso04", electron_index)  > _Cuts["LooseElectronRelIso"]) return false;
	
	return true;

}


//____________________________________________________________________________
bool FakeRatio::tightMuonSelection(int muon_index){
	/*
  	does the selection of tight muons
  	parameters: muon_index
  	return: true (if the muon is tight), false (else)
  	*/

	if(!looseMuonSelection(muon_index)) return false; 
	if(_UsedCuts["TightMuonDXY"]    && fabs(_vc -> getF("mu_dxy"     , muon_index)) > _Cuts["TightMuonDXY"]   ) return false;
	if(_UsedCuts["TightMuonRelIso"] &&      _vc -> getF("mu_RelIso04", muon_index)  > _Cuts["TightMuonRelIso"]) return false;

	return true; 

}


//____________________________________________________________________________
bool FakeRatio::vetoElectronSelection(int electron_index){
	/*
  	does the selection of veto electrons
  	parameters: electron_index
  	return: true (if the electron is veto), false (else)
  	*/

	if(!looseElectronSelection(electron_index)) return false; 
	if(_UsedCuts["VetoElectronPt"] && _vc -> getF("el_pt", electron_index) < _Cuts["VetoElectronPt"]) return false;
	
	return true;

}


//____________________________________________________________________________
bool FakeRatio::vetoMuonSelection(int muon_index){
	/*
  	does the selection of veto muons
  	parameters: muon_index
  	return: true (if the muon is veto), false (else)
  	*/

	if(!looseMuonSelection(muon_index)) return false; 
	if(_UsedCuts["VetoMuonPt"] && _vc -> getF("mu_pt", muon_index) < _Cuts["VetoMuonPt"]) return false; 

	return true; 

}






/*****************************************************************************
******************************************************************************
** EVENT SELECTIONS                                                         **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void FakeRatio::collectKinematicObjects(){
	/*
  	collects all kinematic objects needed in the code, i.e. applies all object selections
  	parameters: none
  	return: none
  	*/

	std::vector<int> empty_vector;
	_KinObj.insert(std::pair<Label, std::vector<int> >("AwayJet"      , empty_vector));
	_KinObj.insert(std::pair<Label, std::vector<int> >("GoodJet"      , empty_vector));
	_KinObj.insert(std::pair<Label, std::vector<int> >("LooseElectron", empty_vector));
	_KinObj.insert(std::pair<Label, std::vector<int> >("LooseMuon"    , empty_vector));
	_KinObj.insert(std::pair<Label, std::vector<int> >("TightElectron", empty_vector));
	_KinObj.insert(std::pair<Label, std::vector<int> >("TightMuon"    , empty_vector));
	_KinObj.insert(std::pair<Label, std::vector<int> >("VetoMuon"     , empty_vector));
	_KinObj.insert(std::pair<Label, std::vector<int> >("VetoElectron" , empty_vector));

	for(unsigned int i = 0; i < _vc -> getSize("jet_pt"); ++i){
		if(awayJetSelection(i)) _KinObj["AwayJet"].push_back(i);
		if(goodJetSelection(i)) _KinObj["GoodJet"].push_back(i);
	}

	for(unsigned int i = 0; i < _vc -> getSize("mu_pt"); ++i){
		if(looseMuonSelection(i)) _KinObj["LooseMuon"].push_back(i);
		if(tightMuonSelection(i)) _KinObj["TightMuon"].push_back(i);
		if(vetoMuonSelection(i) ) _KinObj["VetoMuon"] .push_back(i);
	}

	for(unsigned int i = 0; i < _vc -> getSize("el_pt"); ++i){
		if(looseElectronSelection(i)) _KinObj["LooseElectron"].push_back(i);
		if(tightElectronSelection(i)) _KinObj["TightElectron"].push_back(i);
		if(vetoElectronSelection(i) ) _KinObj["VetoElectron"] .push_back(i);
	}	
	
	_NumKinObj["AwayJet"]       = _KinObj["AwayJet"]      .size();
	_NumKinObj["GoodJet"]       = _KinObj["GoodJet"]      .size();
	_NumKinObj["LooseElectron"] = _KinObj["LooseElectron"].size();
	_NumKinObj["LooseMuon"]     = _KinObj["LooseMuon"]    .size();
	_NumKinObj["TightElectron"] = _KinObj["TightElectron"].size();
	_NumKinObj["TightMuon"]     = _KinObj["TightMuon"]    .size();
	_NumKinObj["VetoElectron"]  = _KinObj["VetoElectron"] .size();
	_NumKinObj["VetoMuon"]      = _KinObj["VetoMuon"]     .size();

}


//____________________________________________________________________________
bool FakeRatio::eventSelection(){
	/*
  	does the event selection for the FakeRatio class
  	parameters: none
  	return: true (if the event passes the selection), false (else)
  	*/


	if(_UsedCuts["HLT_Mu17"]             && _vc -> getI("HLT_MU17")                              != _Cuts["HLT_MU17"]            ) return false;	
	if(_UsedCuts["NumberOfLooseLeptons"] && _NumKinObj["LooseElectron"] + _NumKinObj["LooseMuon"] < _Cuts["NumberOfLooseLeptons"]) return false; 
	if(_UsedCuts["NumberOfVetoLeptons"]  && _NumKinObj["VetoElectron"]  + _NumKinObj["VetoMuon"]  < _Cuts["NumberOfVetoLeptons"] ) return false; 
	if(_UsedCuts["NumberOfAwayJets"]     && _NumKinObj["AwayJet"]                                 < _Cuts["NumberOfAwayJets"]    ) return false; 
	if(_UsedCuts["MET"]                  && _vc -> getF("met_pt")                                 < _Cuts["MET"]                 ) return false;

	return true;

}





/*****************************************************************************
******************************************************************************
** EXECUTING TASKS                                                          **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void FakeRatio::fillFakeRatioMaps(){
	/*
  	fills the fake ratio maps
  	parameters: none
  	return: none
  	*/


	// electrons

	for(int i = 0; i < _NumKinObj["LooseElectron"]; ++i){

		fill("ElLoose", _vc -> getF("el_pt", _KinObj["LooseElectron"][i]), fabs(_vc -> getF("el_eta", _KinObj["LooseElectron"][i])), _EventWeight);

		if(Tools::findInVector(_KinObj["TightElectron"], _KinObj["LooseElectron"][i]))
			fill("ElTight", _vc -> getF("el_pt", _KinObj["LooseElectron"][i]), fabs(_vc -> getF("el_eta", _KinObj["LooseElectron"][i])), _EventWeight);

	}
	

	// muons

	for(int i = 0; i < _NumKinObj["LooseMuon"]; ++i){

		fill("MuLoose", _vc -> getF("mu_pt", _KinObj["LooseMuon"][i]), fabs(_vc -> getF("mu_eta", _KinObj["LooseMuon"][i])), _EventWeight);

		if(Tools::findInVector(_KinObj["TightMuon"], _KinObj["LooseMuon"][i]))
			fill("MuTight", _vc -> getF("mu_pt", _KinObj["LooseMuon"][i]), fabs(_vc -> getF("mu_eta", _KinObj["LooseMuon"][i])), _EventWeight);


		// denominator
		//if(_vc -> getF("mu_pt", lm) > bins_pt.back()){
		//	int fillbin = h_LM -> FindBin(bins_pt.back() - 0.5, fabs(_vc -> getF("mu_eta", lm)));
		//	h_LM -> AddBinContent(fillbin, _EventWeight);
		//}
		//else
		//	h_LM -> Fill(_vc -> getF("mu_pt", lm), fabs(_vc -> getF("mu_eta", lm)), _EventWeight);


		//// numerator
		//if(Tools::findInVector(_KinObj["TightMuon"], lm)){

		//	if(_vc -> getF("mu_pt", lm) > bins_pt.back()){
		//		int fillbin = h_TM -> FindBin(bins_pt.back() - 0.5, fabs(MuEta -> at(lm)));
		//		h_TM -> AddBinContent(fillbin, _EventWeight);
		//	}
		//	else
		//		h_TM -> Fill(MuPt -> at(lm), fabs(MuEta -> at(lm)), _EventWeight);

		//}
	}



	

}

