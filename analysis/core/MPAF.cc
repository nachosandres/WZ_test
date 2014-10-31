/*****************************************************************************
******************************************************************************
******************************************************************************
**                                                                          **
** The Multi-Purpose Analysis Framework                                     **
**                                                                          **
** Constantin Heidegger, Matthieu Marionneau                                **
**                                                                          **
******************************************************************************
******************************************************************************
*****************************************************************************/


#include "analysis/core/MPAF.hh"

#include <boost/timer.hpp>
#include <boost/progress.hpp>

#include "TStopwatch.h"



/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR RUNNING THE CODE                                       **
******************************************************************************
*****************************************************************************/

//____________________________________________________________________________
MPAF::MPAF(){
	/* 
	constructs the MPAF Class 
	parameters: none
	return: none
	*/
	
	initialize();
	
}


//____________________________________________________________________________
MPAF::~MPAF(){
	/* 
	destructs the MPAF class
	parameters: none
	return: none
	*/

	_Verbose -> ExecutionTime();
	_Verbose -> WriteLogFile();

	delete _Verbose;
	delete _vc;
	delete _hm;
	delete _dbm;
	delete _au;

}


//____________________________________________________________________________
void MPAF::initialize(){
	/*
	initializes the MPAF class, defining standard values of certain variables
	parameters: none
	return: none
	*/

	// CH: we need to think of a better way to do this
	// these are the names of the lepton and jet objects as stored in the tree
	// unfortunately they are different for SSDL and Multilepton trees
	_lep = "LepGood";
	_lepnum = "nLepGood";
	_jet = "Jet";
	_jetnum = "nJet";


	_Verbose = new Verbose((VerbosityLevel) 0);
	_Verbose -> Class("MPAF");

	_vc  = new VarClass();
	_hm  = new HistoManager();
	_dbm = new DataBaseManager();
	_au  = new AnaUtils();

	_option="";

}


//____________________________________________________________________________
void MPAF::modifyWeight() {
	/*
	modifies the event weight for every entry in the tree
	parameters: none
	return: none
	*/  

}


//____________________________________________________________________________
void MPAF::analyze(){
	/*
	does the loop over the data samples, reads the tree and calls run() at every
	iteration
	parameters: none
	return: none
	*/


  TStopwatch stw;
  float timeCPU=0;
  float timeWall=0;
  int nE=0;

	// load data from database
	loadInput();

	// define and book all outputs
	defineOutput();

	// loop over given samples
	for(unsigned int i = 0; i < _Samples.size(); ++i){
		
	  //dataset number, depends how we want to store the datasets
	  //_inds = getLabel( i -> first );

		// open file
		_SampleName = _Samples[i] -> getName();
		_inds = i;
		_isData = _Samples[i] -> isPPcolDataset();

		_Verbose -> Sample(_SampleName);
		//TFile * root_file = TFile::Open( (_Samples[i] -> getPath() ).c_str() );
		//if(root_file == NULL) _Verbose -> ErrorAndExit();
		//
		//// open tree
		//_RootTree = (TTree *) root_file -> Get("treeProducerSusySSDL");
		//_RootTree -> ResetBranchAddresses();
		//Base::Initialize(_RootTree);
		
		_vc -> reset();
		_vc -> buildTree( _Samples[i] -> getTree() , false ); //MM : boolean prepared for skimming

	
		// loop over entries
		unsigned int nEvts = _Samples[i] -> getNEvents();
		if(_TestNEvt) nEvts =  min(_TestNEvtMax,nEvts);
		cout<<" Starting processing dataset : "<<_SampleName<<"  (running on "<<nEvts<<" events)"<<endl;

		boost::progress_display show_progress( nEvts );
		for(_EntryIterator = 0; _EntryIterator < nEvts; ++_EntryIterator) {
		  ++show_progress;
		  stw.Start();
		  
		  //MM : preparation for uncertainty variation over one variable
		  // keeping line for future development
		  // _vc->applySystVar( _vc->_su->getSystInfos(_unc, _uDir) );

			_EventWeight = 1.;

			// get tree entry, i.e. load branches
			//std::cout << "trying to read entry " << _EntryIterator << std::endl;
			_Samples[i] -> getTree() -> GetEntry(_EntryIterator);
			
			// get event weight, PU reweight it if needed 
			modifyWeight();
		
			// do something at every entry	
			run();

			timeWall+=stw.RealTime();
			timeCPU+=stw.CpuTime();
			nE++;
		}

		//cleaning memory
		_Samples[i]->freeMemory();
	}


	cout<<" +++ Execution frequency +++ "<<endl;
	cout<<"   Real time = "<<timeWall/nE<<" s/evt "<<" -> "<<nE/timeWall<<" Hz"<<endl;
	cout<<"   CPU time = "<<timeCPU/nE<<" s/evt "<<" -> "<<nE/timeCPU<<" Hz"<<endl<<endl;

	// write all outputs to disk
	writeOutput();

	_au->printNumbers();

}


//____________________________________________________________________________
void MPAF::checkConfiguration(){

	/*
	checks if the given configuration obeys all rules, if all variables are 
	given and if their values make at least some sense (i.e. if their values are 
	within allowed limits), and returns error message and exists the code in case 
	something is illegal
	parameters: none
	return: none
	*/

	// RunOn, Mode and Verbose have been checked already or set to default value.
	// Samples has been checked in the DataSamples class.

	
	// check user name
	if(_UserName.size() == 0) _Verbose -> ErrorAndExit(3);

	// check directories
	if(_InputPath .size() > 0 && (access(_InputPath .c_str(), 0) != 0 || _InputPath .substr(_InputPath .size() - 1) != "/")) _Verbose -> ErrorAndExit(4);
	if(_OutputPath.size() > 0 && (access(_OutputPath.c_str(), 0) != 0 || _OutputPath.substr(_OutputPath.size() - 1) != "/")) _Verbose -> ErrorAndExit(4);

	// check write permission in output folder
	if(!Tools::checkDirWritePermission(_OutputPath)) _Verbose -> ErrorAndExit(1);
}


//____________________________________________________________________________
void MPAF::createOutputStructure(){
	/*
	creates the entire output structure in the local output folder
	parameters: none
	return: none
	*/

	Tools::execCmd("mkdir " + _OutputPath + "MPAF");

	//for(int i = 0; i < _RunOn.size(); ++i)
	//	Tools::execCmd("mkdir " + _OutputPath + _RunOn[i]);

	_Verbose -> SetLogFilePath(_OutputPath + "MPAF/log.out");

}


//____________________________________________________________________________
void MPAF::endExecution(){
	/*
	ends the execution, writes the log file
	parameters: none
	return: none
	*/

}


//____________________________________________________________________________
void MPAF::loadConfigurationFile(std::string configuration_file){
	/*
	loads the configuration file and reads the configuration variables
	parameters: configuratoin_file (path to the configuration file)
	return: none
	*/

	char buffer[500];
	char symbol_char[1]="", variable_char[100]="", value_char[200]="", tree_char[100]="", option_char[100]="";

	_TestNEvt    = false;
	_TestNEvtMax = 0;

	ifstream IN(Tools::toTString(configuration_file));
	if(!IN.is_open()) _Verbose -> ErrorAndExit(2);

	while(IN.getline(buffer, 1000, '\n')){

		if(strlen(buffer) == 0) continue;
		if(buffer[0] == '#') continue;
		if(buffer[0] == ' ') continue;

		//option reset
		strcpy(tree_char  ,"");
		strcpy(option_char,"");
		
		if(sscanf(buffer, "%s\t%s\t%s\t%s\t%s", symbol_char, variable_char, value_char, tree_char, option_char) < 3) continue;
		
		std::string symbol   = symbol_char;
		std::string variable = variable_char;
		std::string value    = value_char;
		std::string tree     = tree_char;
		std::string option   = option_char;

		// non-configuration variables
		if(symbol == "n"){
			if      (variable == "UserName"  ) _UserName   = value;
			else if (variable == "InputPath" ) _InputPath  = value;
			else if (variable == "OutputPath") _OutputPath = value;
			else if (variable == "Mode"      ) _Mode       = Tools::toRunMode(value);
			else if (variable == "RunOn"     ) _RunOn      = Tools::explodeString(value, ",");
			else if (variable == "Modules"   ) _Modules    = Tools::explodeString(value, ",");
		}

		// configuration variables
		else if(symbol == "v"){
			if      (variable == "JEC"          ) _JEC           = atoi(value.c_str());
			else if (variable == "PUReweighting") _PUReweighting = (bool) atoi(value.c_str());
			else if (variable == "SR"           ) _SR            = value;
			else if (variable == "BR"           ) _BR            = value;
			else if (variable == "PT"           ) _PT            = value;
		}

		// event or object selections
		else if(symbol == "o" || symbol == "e"){
			_UsedCuts[variable] = true;
			_Cuts[variable]     = Tools::interpretCut(value);
		}

		// setting verbosity
		if(symbol == "n" && variable == "Verbose") {
			_VerbosityLevel = Tools::toVerbosityLevel(value);
			_Verbose -> SetVerbosityLevel(_VerbosityLevel);
		}

		// setting max number of events to loop on
		if(symbol == "v" && variable == "TestNEvtMax"){
			_TestNEvt    = true;
			_TestNEvtMax = atoi(value.c_str());
		}


		// loading data samples
		if(symbol == "s" && variable != ""){	
		  string dsName=variable;
		  dsName+=option;

		  _Samples.push_back(new Dataset(dsName));
		  _Samples.back() -> addSample(variable, _InputPath, value_char, tree, "", 1.0, 1.0, 1.0, 1.0);
		  //_SampleOption[ variable ] = opt;
		}
				
	}

	if(_Modules.size() == 0 || _Modules[0].find("All") != std::string::npos) 
		_Modules = _AllModules;

	_Verbose -> SetNumberOfModules(_Modules.size());
}


//_____________________________________________________________________________________
void MPAF::setAllModules(std::vector<std::string> all_modules){
	/*
  	sets the list of all modules the daughter class inhabits, i.e. the list of all
  	modules that could possibly be executed
	parameters: all_modules
	return: none 
 	*/

	_AllModules = all_modules;

}


//____________________________________________________________________________
void MPAF::setConfigName(std::string configuration_file){
	/*
	saves the name of the configuration
	parameters: configuration_file
	return: none
	*/

	size_t p = configuration_file.find_last_of("/");
	size_t q = configuration_file.find(".cfg");

	_ConfigName = configuration_file.substr(p + 1, q - p - 1);

}


//____________________________________________________________________________
void MPAF::startExecution(std::string configuration_file){
	/*
	starts MPAF, i.e. loads the configuration file, checks the configuration,
	loads and sets some variables and prepares the output structure
	parameters: configuration_file (path to the configuration file)
	return: none
	*/

	setConfigName(configuration_file);
	loadConfigurationFile(configuration_file);
	//checkConfiguration(); // CH: uncomment again as soon as config file and things are fixed
	//createOutputStructure();

	std::vector<std::string> dsNames;
	for(unsigned int i = 0; i < _Samples.size(); ++i) {
		dsNames.push_back(_Samples[i] -> getName());
	
		// if(_SampleOption[ _Samples[i] -> getName() ]!="") //MM FIXME, ugly
		//   dsNames.back() += _SampleOption[ _Samples[i] -> getName() ];
	}

	_hm -> configAnalysis( dsNames );

}





/*****************************************************************************
******************************************************************************
** PLACEHOLDERS AND FUNCTIONS TO BE USED BY SUB CLASSES                     **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void MPAF::defineOutput(){
	/*
  	placeholder for defining all outputs per class
  	parameters: none
  	return: none
  	*/

}


//____________________________________________________________________________
void MPAF::fillEventList(){
	/*
	fills the event lists cache with a print-out about the current event
	parameters: none
	return: none
	*/

	//std::string line = Form("%d\t%d\t%d", Run, Lumi, Event);
	//std::cout << line << std::endl;

}


//____________________________________________________________________________
void MPAF::loadInput(){
	/*
  	placeholder for loading input per class
  	parameters: none
  	return: none
  	*/

}


//____________________________________________________________________________
void MPAF::resetKinematicObjects(){
	/*
  	resets the maps and vectors of the kinematic objects
  	parameters: none
  	return: none
  	*/
	
	_KinObj   .clear();
	_NumKinObj.clear();

}


//____________________________________________________________________________
void MPAF::run(){
	/*
  	placeholder for running the analysis, performing the event selection and
  	filling histograms per tree entry
  	parameters: none
  	return: none
  	*/

}


//____________________________________________________________________________
void MPAF::writeOutput(){
	/*
  	placeholder for writing the output per class
  	parameters: none
  	return: none
  	*/

}






/*****************************************************************************
******************************************************************************
** METHODS FOR DOING PHYSICS                                                **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
int MPAF::findCharge(std::string electron_label, std::string muon_label){
	/*
	returns the total charge in the event as calculated from the electrons and
	leptons that have been selected
	parameters: electron_label, muon_label
	return: the total charge as int
	*/

	int charge = 0;

	for(int i = 0; i < _NumKinObj[electron_label]; ++i)
		charge += _vc -> getI(_lep + "_charge", _KinObj[electron_label][i]);

	for(int i = 0; i < _NumKinObj[muon_label]; ++i)
		charge += _vc -> getI(_lep + "_charge", _KinObj[muon_label][i]);

	return charge; 

}


//____________________________________________________________________________
float MPAF::findMLL(std::string electron_label, std::string muon_label){
	/*
	returns the invariant mass MLL of the two leptons that are stored in
	the _KinObj lists given by electron_label and muon_label; i.e. we 
	assume that the overall number of leptons stored in those two lists
	is 2, so there is just one lepton pair we can compute the MLL for
	parameters: electron_label, muon_label
	return: MLL
	*/ 

	
	if(_NumKinObj[electron_label] + _NumKinObj[muon_label] != 2) return 0.0;


	std::string lep_flavor[2] = {"", ""};
	int         lep_index [2] = { 0,  0}; 

	int j = 0;
	for(int i = 0; i < _NumKinObj[electron_label]; ++i){
		lep_flavor[i] = "electron";
		lep_index [i] = _KinObj[electron_label][i];
		++j;
	}

	for(int i = 0; i < _NumKinObj[muon_label]; ++i){
		lep_flavor[j + i] = "muon";
		lep_index [j + i] = _KinObj[muon_label][i];
	}

	return MLL(lep_flavor[0], lep_index[0], lep_flavor[1], lep_index[1]);	

}


//____________________________________________________________________________
float MPAF::HT(std::string jet_label){
	/*
  	computes HT for a given list of selected jets
  	parameters: jet_label
  	return: HT
  	*/

	float ht = 0;

	for(int i = 0; i < _NumKinObj[jet_label]; ++i) 
		ht += (_JEC == 1 ? _vc -> getF(_jet + "_pt", _KinObj[jet_label][i]) : _vc -> getF(_jet + "_rawPt", _KinObj[jet_label][i]));

	return ht;

}


//____________________________________________________________________________
float MPAF::MLL(std::string lep1_flavor, int lep1_index, std::string lep2_flavor, int lep2_index){
	/*
	computes the invariant mass MLL for a pair of leptons
	parameters: electron_label, muon_label (the labels of the selected electrons and muons)
	return: MLL
	*/

	TLorentzVector lep1, lep2, sum;

	if     (lep1_flavor == "electron") 
		lep1.SetPtEtaPhiM(_vc -> getF(_lep + "_pt", lep1_index), _vc -> getF(_lep + "_eta", lep1_index), _vc -> getF(_lep + "_phi", lep1_index), 0.005);
	else if(lep1_flavor == "muon"    ) 
		lep1.SetPtEtaPhiM(_vc -> getF(_lep + "_pt", lep1_index), _vc -> getF(_lep + "_eta", lep1_index), _vc -> getF(_lep + "_phi", lep1_index), 0.105);
	else
		return 0.;

	if     (lep2_flavor == "electron") 
		lep2.SetPtEtaPhiM(_vc -> getF(_lep + "_pt", lep2_index), _vc -> getF(_lep + "_eta", lep2_index), _vc -> getF(_lep + "_phi", lep2_index), 0.005);
	else if(lep2_flavor == "muon"    ) 
		lep2.SetPtEtaPhiM(_vc -> getF(_lep + "_pt", lep2_index), _vc -> getF(_lep + "_eta", lep2_index), _vc -> getF(_lep + "_phi", lep2_index), 0.105);
	else
		return 0.;

	sum = lep1 + lep2;

	return sum.M();

}


//____________________________________________________________________________
float MPAF::MT(std::string lepton_type, int lepton_iterator){
	/*
  	computes MT for a given lepton of given type (electron or muon)
  	parameters: lepton_type ("electron" or "muon"), lepton_iterator
  	return: MT
  	*/

	TLorentzVector lepton;
	TLorentzVector met;

	if     (lepton_type == "muon"    ) 
		lepton.SetPtEtaPhiM( _vc -> getF(_lep + "_pt", lepton_iterator), _vc -> getF(_lep + "_eta", lepton_iterator), _vc -> getF(_lep + "_phi", lepton_iterator), 0.105);
	else if(lepton_type == "electron") 
		lepton.SetPtEtaPhiM( _vc -> getF(_lep + "_pt", lepton_iterator), _vc -> getF(_lep + "_eta", lepton_iterator), _vc -> getF(_lep + "_phi", lepton_iterator), 0.005);
	else
		return 0.;
	
	met.SetPtEtaPhiM(_vc -> getF("met_pt"), _vc -> getF("met_eta"), _vc -> getF("met_phi"), _vc -> getF("met_mass"));

	float ET_lepton = sqrt(lepton.M2() + lepton.Perp2());

	return sqrt( 2 * (met.Pt() * ET_lepton - lepton.Px() * met.Px() - lepton.Py() * met.Py() ));

}





/*****************************************************************************
******************************************************************************
** INTERFACE TO HISTOGRAM MANAGER                                           **
******************************************************************************
*****************************************************************************/

//____________________________________________________________________________
void MPAF::fill(string var, float valx, float weight) {
	/*
	fills a histogram (var) with a value (valx) that has a weight (weight)
	parameters: var, valx, weight
	return: none
	*/
 
	//if(_unc=="")
	_hm -> fill( var, _inds, valx, weight );
	  //MM : kept for systematic uncertainty future developments
	// else {
	//   if(_uDir==SystUtils::kUp)
	//     fillUnc(var,_unc,valx,weight,"Up");
	//   if(_uDir==SystUtils::kDown)
	//     fillUnc(var,_unc,valx,weight,"Do");
	// }
    
}


//____________________________________________________________________________
void MPAF::fill(string var, float valx, float valy, float weight) {
	/*
	fills a histogram (var) with values (valx, valy) that have a weight (weight)
	parameters: var, valx, valy, weight
	return: none
	*/

	_hm -> fill( var, _inds, valx, valy, weight );

}


//____________________________________________________________________________
void MPAF::fillUnc(string var, string unc, float val, float weight, string dir) {
	/*
	??
	parameters: 
	return: 
	*/

	if(!_isData)
		_hm -> fill( var, unc, val, weight,dir);

}


//____________________________________________________________________________
void MPAF::fillUnc(string var, string unc, float val, float weight, float wup, float wdo) {
	/*
	??
	parameters: 
	return: 
	*/

	_hm -> fill(var, _inds, val, weight );

	if(!_isData ) {
		_hm -> fill( var, unc, val, wup, "Up");
		_hm -> fill( var, unc, val, wdo, "Do");
	}
}





/*****************************************************************************
******************************************************************************
** INTERFACE TO DATABASE MANAGER                                            **
******************************************************************************
*****************************************************************************/

//____________________________________________________________________________
float MPAF::getDBVal(string db, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10) {
	/*
	??
	parameters: 
	return: 
	*/
  
	//if(_unc=="")
	return _dbm -> getDBValue( db, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 );
	//MM : for future systematic uncertainty development
	// else
	//   return applySystDBVar( _vc->_su->getSystInfos(_unc, _uDir), db, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);

}


//____________________________________________________________________________
float MPAF::getDBErrL(string db, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10) {
	/*
	??
	parameters: 
	return: 
	*/

	return _dbm -> getDBErrL( db, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 );

}


//____________________________________________________________________________
float MPAF::getDBErrH(string db, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10) {
	/*
	??
	parameters: 
	return: 
	*/

	return _dbm -> getDBErrH(db, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 );

}


//____________________________________________________________________________
void MPAF::loadDb(string key, string dbfile, string hname) {
	/*
	??
	parameters: 
	return: 
	*/

	// ASCII badtabase per definition
	if(hname == "") 
		_dbm -> loadDb(key, dbfile);
	else
		_dbm -> loadDb(key, dbfile, hname);

}


//____________________________________________________________________________
bool MPAF::makeCut(bool decision, string cName, string type, string eCateg) {
	/*
	??
	parameters: 
	return: 
	*/
  //MM FIXME, option stuff is ugly
	return _au -> makeCut(decision, _SampleName, cName, _EventWeight, type, eCateg, false);

}


//____________________________________________________________________________
void MPAF::counter(string cName, string eCateg) {
	/*
	??
	parameters: 
	return: 
	*/
//MM FIXME, option stuff is ugly+_SampleOption[_SampleName]
	_au -> makeCut(true, _SampleName, cName, _EventWeight, "=", eCateg, false);

}


