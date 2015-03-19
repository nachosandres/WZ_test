#include "core/MPAFDisplay.hh"

ClassImp(MPAFDisplay)

using namespace std;

MPAFDisplay::MPAFDisplay() {

  _hm=new HistoManager();
  _dbm=new DataBaseManager();
  _au=new AnaUtils();
  _recompute=true;
}

MPAFDisplay::~MPAFDisplay() {
  reset();
  delete _hm;
}


void
MPAFDisplay::configure() {
  _dsnames = anConf.getDSNames();
 
  //_au->setDDCuts(anConf.getDDCuts() );
  
  _hm->configAnalysis(_dsnames );
  
  for(size_t ids=0;ids<_dsnames.size();ids++) {
    dp.configure(_dsnames[ids], 
		 anConf.getDataset(_dsnames[ids])->getColor(),
		 anConf.getDataset(_dsnames[ids])->isGhost() );
    //Default, weight=1 for anyone
    dp.setWeight( _dsnames[ids], 1. );
  }

}

void
MPAFDisplay::reset() {
  _hm->reset();
  dp.reset();
  anConf.reset(); 
  //  _au->reset();
}


void
MPAFDisplay::makeDataCard(string dirname, string categ, string cname){

  bool mcat=false;
  if(cname!="") mcat=true;

  vector< pair<string, vector<vector<float> > > > numbers = _au->retrieveNumbers(categ, mcat, cname);

  vector<string> dsNames = anConf.getDSNames();
  dsNames.insert(dsNames.begin(), "MC");

  dp.makeDataCard( numbers, dsNames, dirname );
}



void 
MPAFDisplay::drawStatistics(string categ, string cname){

  bool mcat=false;
  if(cname!="") mcat=true;

  vector< pair<string, vector<vector<float> > > > numbers = _au->retrieveNumbers(categ, mcat, cname);	

  vector<string> dsNames = anConf.getDSNames();
  dsNames.insert(dsNames.begin(), "MC");

  dp.drawStatistics( numbers, dsNames );

}


void
MPAFDisplay::getStatistics(string categ) {
  _au->printTables(categ);
}


void 
MPAFDisplay::setNumbers() {
  if(!_recompute) return;

  for(size_t id=0;id<_dsnames.size();id++) {
    _au->addDataset(_dsnames[id]);
  }
  
  vector<string> statFiles = anConf.getObjList();
  string ctag = "";
  size_t bl = 0;
  size_t bh = 0;
  
  int icat=1; //0 for global

  for(int i=(int)(statFiles.size())-1; i>=0; i--) {
    
    if(statFiles.size()>1 && i!=0)
      ctag = findDiff(statFiles[0], statFiles[i],'_',bl,bh);
    if(statFiles.size()>1 && i==0)
      ctag = statFiles[0].substr(bl,bh-bl);

    if(ctag.size()>4 && ctag.substr(ctag.size()-4) == ".dat")
      ctag.erase(ctag.size()-4,4);

    readStatFile( statFiles[i], ctag, icat);
  }

}

void
MPAFDisplay::readStatFile(string filename, string ctag, int& icat) {
 
 
  if(filename=="") return;
  _au->init();
  
  string ndb = filename;
  ifstream fDb( ndb.c_str(), ios::in );

  map<pair<string,string>, bool > fVal;

  if(fDb)  {
    string line;
    
    string categ;
    string cname;
    string sname;
    string dsname;
    float yield, eyield;
    int gen;
    int ids;
  
    while(getline(fDb, line)) {
	
      istringstream iss(line);
      vector<string> tks;
      copy(istream_iterator<string>(iss),
           istream_iterator<string>(),
           back_inserter<vector<string> >(tks));

 
      if(tks.size()==0 || line == "") continue; 
      
      if(tks[0]=="categ") {
        categ="";
        for(size_t i=1;i<tks.size();i++) //prevent from spaces in categ names
          categ +=tks[i];
     
        categ+=ctag;
      
        if(categ!="global") {
          _au->addCategory(icat, categ);
        } 
        else {
          if(ctag=="") icat=0; //comes in last so do not mess the reading
        }
      }
      else if(tks[0]=="endcateg") { //fill the maps
        icat++;
      }
      else if(tks[0]=="selection") continue;
 
      else {
 
        size_t n=tks.size()-4;
        cname="";
        for(size_t i=0;i<n;i++)
          cname += tks[i]+" ";
          
        sname = tks[n];
 
        Dataset* ds=anConf.findDS( sname );
        ids=-1;

        if(ds==nullptr) continue;

        for(size_t id=0;id<_dsnames.size();id++ ) {
          if(_dsnames[id]==ds->getName() ) {
            ids = id+1;//because MC is 0
            break;
          }
        }
     
        float w = ds->getWeight(sname)*anConf.getLumi();
   
        yield  = atof( tks[n+1].c_str() ) *w ;
		//cout << "adding " << atof( tks[n+1].c_str() ) << " for " << ds->getName() <<","<<cname<<","<<icat<< endl;   
        gen = atoi( tks[n+2].substr(1,tks[3].size()-1).c_str() );
        eyield = atof( tks[n+3].c_str() ) *w;

        pair<string,string> p(ds->getName(), cname+sname);
        if(fVal.find(p)==fVal.end() ) {
          fVal[p]=true;
          _au->setEffFromStat(ids,cname,icat,yield,eyield,gen);
        }
      
      }
	
    }
    fDb.close();
  }
  else {
    cout<<"Warning, statistics file "<<filename<<" not loaded"<<endl;
  }


}


void
MPAFDisplay::prepareDisplay(){

  configure();
  dp.setLumi( anConf.getLumi() );
  setNumbers();

}


void
MPAFDisplay::doPlot() {

  //configure();

  setHistograms();

  //dp.setLumi( anConf.getLumi() );
  //setNumbers();

  //See if a fit is needed for the normalization
  //ugly....
  string fitVar=dp.getFitVar();
  if(fitVar!="") {
    const hObs* wobs = _hm->getHObs( fitVar );
    if( wobs!=NULL)
      dp.initWeights(wobs); 
    else
      cout<<" Error, no observable of name : "<<fitVar<<" for fitting "<<endl;
  }
  
  //Find the observables and draw them
  //could be done in a better way...
  vector<string> obs_ = dp.getObservables();
  vector<const hObs*> Obs_;
  vector<vector<systM> > systs_;
  
  for(size_t io = 0; io < obs_.size(); ++io) {
    _currentObs = obs_[io];

    if( _hm->getHObs( obs_[io] )==NULL) {
      cout<<" Error, no observable of name : "<<obs_[io]<<endl;
      continue;
    }

    Obs_.push_back( _hm->getHObs( obs_[io] ) );
    systs_.push_back( _hm->getSystObs( obs_[io] ) );
  }  
  
  if(Obs_.size()!=0) {
    dp.setSystematicsUnc( systs_ );
    dp.plotDistributions( Obs_ );
  }

  _recompute=false;

}


void
MPAFDisplay::setHistograms() {

  //loop over the datasets, find the histograms, sum them
  // and store them into the HistoManager

  //do not need to reload everything at each iteration, but let's do it for the moment
  if(!_recompute) return;

  for(size_t ids=0;ids<_dsnames.size();ids++) {
    
    _ids = _dsnames[ids];
    _inds = ids;

    Dataset* ds=anConf.getDataset( _ids );
    string tmpDs= _ids;

    vector<string> obss = ds->getObservables();
    for(size_t io=0;io<obss.size();io++) {
      TH1* htmp(0);
      vector<string> samples= ds->getSamples();
      for(size_t is=0;is<samples.size(); is++) {
	if(is==0) {
	  htmp = ds->getHisto( obss[io], samples[is] );
	  htmp->Scale( ds->getWeight(is) );
	}
	else {
	  htmp->Add( ds->getHisto( obss[io], samples[is] ), 
		     ds->getWeight(is) );
	}
      } 

      _hm->copyHisto( obss[io] , _inds, htmp );

      delete htmp;
    
    }

  }//datasets

  //keep for later ideas for uncertainties
  // TH1* htmp(0); 
  // for(size_t ih=0;ih<ah.getNHistos();ih++) {
  //   htmp = ah.getHistogram(ih);
    
  //   _inds = anConf.findChan( htmp->GetName() );
    
  //   bool isUnc=((string)htmp->GetName()).find("Unc")!=(size_t)-1;
  //   if(isUnc) _hm->copyHisto( ah.getVar(), -10, htmp, ah.getXtitle() );
  //   if(_inds==-1) continue;
  //   _hm->copyHisto( ah.getVar(), _inds, htmp, ah.getXtitle() );
    
  // }
  // delete htmp;

  //  _recompute=false;

}


void
MPAFDisplay::drawRatio(string o1, string o2 ) {

  vector<const hObs*> Obs_;
  // vector<vector<systM> > systs_;
  
  Obs_.push_back( _hm->getHObs( o1 ) );
  Obs_.push_back( _hm->getHObs( o2 ) );

  if( Obs_[0]==NULL) {
    cout<<" Error, no observable of name : "<<o1<<endl;
    return;
  }
  if( Obs_[1]==NULL) {
    cout<<" Error, no observable of name : "<<o2<<endl;
    return;
  }

  // systs_.push_back( _hm->getSystObs( o1 ) );
  // systs_.push_back( _hm->getSystObs( o2 ) );
  
  //dp.setSystematicsUnc( systs_ );
  dp.ratioObservables( Obs_ );

}


void
MPAFDisplay::drawResiduals(string o1) {
  const hObs* Obs_;
  vector<vector<systM> > systs_;

  Obs_ =  _hm->getHObs( o1 );
  systs_.push_back( _hm->getSystObs( o1 ) );
  dp.setSystematicsUnc( systs_ );
  dp.residualData( Obs_ );
}

void
MPAFDisplay::drawSignificance(string o1) {
  const hObs* Obs_;
  vector<vector<systM> > systs_;

  Obs_ =  _hm->getHObs( o1 );

  if( Obs_==NULL) {
    cout<<" Error, no observable of name : "<<o1<<endl;
    return;
  }
  
  dp.showSignificance( Obs_ );
}

void
MPAFDisplay::drawCumulativePlots(string o1) {
  const hObs* Obs_;
  vector<vector<systM> > systs_;

  Obs_ =  _hm->getHObs( o1 );

  if( Obs_==NULL) {
    cout<<" Error, no observable of name : "<<o1<<endl;
    return;
  }
  
  dp.drawCumulativeHistos( Obs_ );
}

void
MPAFDisplay::drawEfficiencies(string o1) {
  const hObs* Obs_;
  
  Obs_ =  _hm->getHObs( o1 );

  if( Obs_==NULL) {
    cout<<" Error, no observable of name : "<<o1<<endl;
    return;
  }
  
  dp.drawEfficiency( Obs_ );
}


void
MPAFDisplay::drawROCcurves(string o1) {
  const hObs* Obs_;
  
  Obs_ =  _hm->getHObs( o1 );

  if( Obs_==NULL) {
    cout<<" Error, no observable of name : "<<o1<<endl;
    return;
  }
  
  dp.drawROCCurves( Obs_ );
}


void
MPAFDisplay::multiROCcurves() {
  vector<const hObs*> Obs_;
  vector<string> obs_ = dp.getObservables();
  
  for(size_t io=0;io<obs_.size();io++) {
    if( _hm->getHObs( obs_[io] )==NULL) {
      cout<<" Error, no observable of name : "<<obs_[io]<<endl;
      continue;
    }
    
    Obs_.push_back( _hm->getHObs( obs_[io] ) );
  }  
  
  dp.compaROCCurves( Obs_ );
}

void
MPAFDisplay::saveHistos(string o1) {
  const hObs* Obs_;
  Obs_ =  _hm->getHObs( o1 );

  if( Obs_==NULL) {
    cout<<" Error, no observable of name : "<<o1<<endl;
    return;
  }
   
  dp.saveHistos(o1, Obs_);

}

void
MPAFDisplay::saveDataMCRatio(string fname,string hname) {
  dp.saveDMCRWeight(fname,hname);
}


void
MPAFDisplay::savePlot(string path, string advname) {
  _hm->savePlots(path, dp.getCanvas(), advname);
  //save
}

void
MPAFDisplay::producePlots(string path) {
  
  gROOT->SetBatch(kTRUE);

  //vector<string> obss = _hm->getObservables();
  vector<string> obss = dp.getAutoVars();

  for(size_t is=0;is<obss.size();is++) {

    refresh();
    dp.setObservables( obss[is] );
    doPlot();
    savePlot(path);
  }
  gROOT->SetBatch(kFALSE);
}

void
MPAFDisplay::drawDetailSyst(bool cumul) {
  vector<vector<systM> > systs_;
  systs_.push_back( _hm->getSystObs( _currentObs ) );
  dp.setSystematicsUnc( systs_ );
  dp.drawDetailSystematics( cumul );
}

void
MPAFDisplay::getIntegral(float x1, float x2, float y1, float y2) {
  dp.getIntegral(x1, x2, y1, y2);
}

void
MPAFDisplay::refresh() {
  dp.reset();
  // anConf.reset();
  // _hm->reset();
}



vector<string> 
MPAFDisplay::split(const string& s, char delim) {

  vector<string> elems;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    if (!item.empty())
      elems.push_back(item);
  }
  return elems;
}

string
MPAFDisplay::findDiff(const string& s1, const string& s2,
		      char delim, size_t& bl, size_t& bh) {

  vector<string> sel1 = split(s1, delim);
  vector<string> sel2 = split(s2, delim);

  if(sel1.size() != sel2.size() ) {
    bl = 0;
    bh = sel1.size()-1;
    return s2;
  }

  string diff="";
  for(size_t ie=0;ie<sel1.size();ie++) {
    if(sel1[ie]==sel2[ie]) continue;

    diff += sel2[ie];
    bl = s1.find(sel1[ie]);
    bh = bl + sel1[ie].size();
  }
  
  return diff;
}
