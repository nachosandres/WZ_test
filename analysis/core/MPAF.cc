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

  _verbose->executionTime();
  _verbose->writeLogFile();

  delete _verbose;
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
  
  _verbose = new Verbose((VerbosityLevel) 0);
  _verbose->Class("MPAF");

  _vc  = new VarClass();
  _hm  = new HistoManager();
  _dbm = new DataBaseManager();
  _au  = new AnaUtils();
  
  _fullSkim=true;
  _skim=false;
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
  for(unsigned int i=0; i<_datasets.size(); ++i){
		
    // open file
    _sampleName = _datasets[i]->getName();
    _inds = i;
    _isData = _datasets[i]->isPPcolDataset();

    _verbose->sample(_sampleName);
		
    _vc->reset();
    _vc->buildTree( _datasets[i]->getTree() , _skim&&_fullSkim );
		
    //prepare skimmed file and tree
    if(_skim) {
      initSkimming();
    }
	
    // loop over entries
    unsigned int nEvts = _datasets[i]->getNEvents();
    if(_nEvtMax!=-1) nEvts =  min(_nEvtMax+_nSkip,nEvts);
    
    cout<<" Starting processing dataset : "<<_sampleName<<"  (running on "<<nEvts<<" events)"<<endl;

    boost::progress_display show_progress( nEvts );
    for(_ie = _nSkip; _ie < nEvts; ++_ie) {
      ++show_progress;
      stw.Start();
		  
      //MM : preparation for uncertainty variation over one variable
      // keeping line for future development
      // _vc->applySystVar( _vc->_su->getSystInfos(_unc, _uDir) );

      _weight = 1.;

      // get tree entry, i.e. load branches
      _datasets[i]->getTree()->GetEntry(_ie);
			
      // get event weight, PU reweight it if needed 
      modifyWeight();
		
      // do something at every entry	
      run();

      timeWall+=stw.RealTime();
      timeCPU+=stw.CpuTime();
      nE++;
    }

    //write skimmed file
    if(_skim) {
      finalizeSkimming();
    }
		
    //cleaning memory
    _datasets[i]->freeMemory();
  }


  cout<<" +++ Execution frequency +++ "<<endl;
  cout<<"   Real time = "<<timeWall/nE<<" s/evt "<<"->"<<nE/timeWall<<" Hz"<<endl;
  cout<<"   CPU time = "<<timeCPU/nE<<" s/evt "<<"->"<<nE/timeCPU<<" Hz"<<endl<<endl;

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

  // check directories
  //if(_inputPath.size()>0 && (access(_inputPath.c_str(), 0) != 0 || _inputPath.substr(_inputPath.size() - 1) != "/")) _verbose->errorAndExit(4);
  //if(_outputPath.size()>0 && (access(_outputPath.c_str(), 0) != 0 || _outputPath.substr(_outputPath.size() - 1) != "/")) _verbose->errorAndExit(4);

  // check write permission in output folder
  //if(!Tools::checkDirWritePermission(_outputPath)) _verbose->ErrorAndExit(1);
}


//____________________________________________________________________________
void MPAF::createOutputStructure(){
  /*
    creates the entire output structure in the local output folder
    parameters: none
    return: none
  */

  // Tools::execCmd("mkdir " + _outputPath + "MPAF");
  
  // _verbose->setLogFilePath(_outputPath + "MPAF/log.out");

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
void MPAF::loadConfigurationFile(std::string cfg){
  /*
    loads the configuration file and reads the configuration variables
    parameters: configuratoin_file (path to the configuration file)
    return: none
  */

  _inputVars = Parser::parseFile(cfg);

  string tName;

  for(MIPar::const_iterator it=_inputVars.begin(); 
      it!=_inputVars.end();it++) {

    if(it->second.type==Parser::kDir) {
      _inputPath=it->second.val;
    }
    if(it->second.type==Parser::kVbs) {
      // int vLvl=Tools::toVerbosityLevel(it->second.val);
      // _verbose->setVerbosityLevel(vLvl);
    }
    if(it->second.type==Parser::kSkim) {
      _skim    = true;
      if(it->second.val=="Limited") _fullSkim=false;
    }
    if(it->second.type==Parser::kNMax) {
      _nEvtMax = atoi(it->second.val.c_str());
    }
    if(it->second.type==Parser::kSkip) {
      _nSkip = atoi(it->second.val.c_str());
    }
    if(it->second.type==Parser::kTree) {
      tName = it->second.val;
    }
    
  }

  //datasets
  
  for(MIPar::const_iterator it=_inputVars.begin(); 
      it!=_inputVars.end();it++) {
    
    if(it->second.type!=Parser::kDS) continue;
    
    string dsName=it->second.val;
    string dirName="";
    vector<string> opts= it->second.opts;
    if(opts.size()!=0) {
      for(size_t i=0;i<opts.size();i++) {
        if(opts[i].substr(0,4)=="dir:")
          dirName=opts[i].substr(4, opts[i].size()-4 );
      }
    }
    _datasets.push_back(new Dataset(dsName));
	cout << it->second.val << ":"<< _inputPath << ":" << dirName << ":" << tName << endl; 
    _datasets.back()->addSample(it->second.val, _inputPath, dirName, tName, "", 1.0, 1.0, 1.0, 1.0);
    _au->addDataset( dsName );
  }

}


string
MPAF::getCfgVarS(string n) {
  return _inputVars[n].opts[0];
}

int
MPAF::getCfgVarI(string n) {
  return atoi(getCfgVarS(n).c_str() );
}

float
MPAF::getCfgVarF(string n) {
  return atof(getCfgVarS(n).c_str() );
}

// //_____________________________________________________________________________________
// void MPAF::setAllModules(std::vector<std::string> all_modules){
//   /*
//     sets the list of all modules the daughter class inhabits, i.e. the list of all
//     modules that could possibly be executed
//     parameters: all_modules
//     return: none 
//   */

//   _AllModules = all_modules;

// }


//____________________________________________________________________________
void MPAF::setConfigName(std::string cfg){
  /*
    saves the name of the configuration
    parameters: configuration_file
    return: none
  */

  size_t p = cfg.find_last_of("/");
  size_t q = cfg.find(".cfg");

  _cfgName = cfg.substr(p+1, q-p-1);

}


//____________________________________________________________________________
void MPAF::startExecution(std::string cfg){
  /*
    starts MPAF, i.e. loads the configuration file, checks the configuration,
    loads and sets some variables and prepares the output structure
    parameters: configuration_file (path to the configuration file)
    return: none
  */

  setConfigName(cfg);
  loadConfigurationFile(cfg);
  //checkConfiguration(); // CH: uncomment again as soon as config file and things are fixed
  //createOutputStructure();

  std::vector<std::string> dsNames;
  for(unsigned int i=0; i<_datasets.size(); ++i) {
    dsNames.push_back(_datasets[i]->getName());
  }

  _hm->configAnalysis( dsNames );

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
  _hm->fill( var, _inds, valx, weight );
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

  _hm->fill( var, _inds, valx, valy, weight );

}


//____________________________________________________________________________
void MPAF::fillUnc(string var, string unc, float val, float weight, string dir) {
  /*
    ??
    parameters: 
    return: 
  */

  if(!_isData)
    _hm->fill( var, unc, val, weight,dir);

}


//____________________________________________________________________________
void MPAF::fillUnc(string var, string unc, float val, float weight, float wup, float wdo) {
  /*
    ??
    parameters: 
    return: 
  */

  _hm->fill(var, _inds, val, weight );

  if(!_isData ) {
    _hm->fill( var, unc, val, wup, "Up");
    _hm->fill( var, unc, val, wdo, "Do");
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
  return _dbm->getDBValue( db, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 );
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

  return _dbm->getDBErrL( db, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 );

}


//____________________________________________________________________________
float MPAF::getDBErrH(string db, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10) {
  /*
    ??
    parameters: 
    return: 
  */

  return _dbm->getDBErrH(db, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 );

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
    _dbm->loadDb(key, dbfile);
  else
    _dbm->loadDb(key, dbfile, hname);

}


//____________________________________________________________________________
bool MPAF::makeCut(bool decision, string cName, string type, int eCateg) {
  /*
    ??
    parameters: 
    return: 
  */
  //MM FIXME, option stuff is ugly
  //_SampleName
  return _au->makeCut(decision, _inds , cName, _weight, type, eCateg, false);

}


//____________________________________________________________________________
void MPAF::counter(string cName, int eCateg) {
  /*
    ??
    parameters: 
    return: 
  */
  //MM FIXME, option stuff is ugly+_SampleOption[_SampleName]
  // _SampleName
  _au->makeCut(true, _inds , cName, _weight, "=", eCateg, false);

}



// skimming functions ======================================
void MPAF::modifySkimming() {
}

void MPAF::initSkimming() {
  
  string opath = string(getenv ("MPAF"))+"/workdir/skims";
  FILE* test = fopen( opath.c_str(), "r" );
  if( test == 0 ) {
    string command_ = "mkdir -p " + opath; 
    assert( system( command_.c_str() ) == 0 );
  }
  else
    fclose( test );

  _oFile = new TFile( (opath+"/"+_sampleName+"_skim.root").c_str(),"RECREATE");
  _datasets[_inds]->getTree()->LoadTree(0);
  if(_fullSkim) {
    _skimTree = (TTree*)_datasets[_inds]->getTree()->CloneTree(0);
    _hnSkim =new TH1I( "nEvtProc", "nEvtProc", 1, 0, 1);
    _hnSkim->SetBinContent(1,_datasets[_inds]->getNProcEvent(0) );
  }
  else {
    TString name = _datasets[_inds]->getTree()->GetName();
    _skimTree = new TTree( name, name );
  }
  _skimTree->SetDirectory( _oFile );
  
  modifySkimming();
    
}

void MPAF::finalizeSkimming() {

  _oFile->cd();
  _skimTree->Write();
  if(_hnSkim)
    _hnSkim->Write();
  _oFile->Write();
  _oFile->Close();
}
