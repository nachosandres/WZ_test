#include "core/MPAFDisplay.hh"

ClassImp(MPAFDisplay)

using namespace std;

MPAFDisplay::MPAFDisplay() {

 _hm=new HistoManager();
 _dbm=new DataBaseManager();
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
}


void MPAFDisplay::doStatisticsPlot(){

	vector<float> weights;

	for(size_t id = 0; id < _dsnames.size(); ++id){

		string ids = _dsnames[id];
		Dataset* ds = anConf.getDataset( ids );
		vector<string> samples = ds->getSamples();
		for(size_t is = 0; is < samples.size(); is++)
			weights.push_back(ds -> getWeight(is));
	}

	for(size_t id = 0; id < weights.size(); ++id){
		weights[id] *= anConf.getLumi();
		//cout << "my weight for dataset " << id << " is " << weights[id] << endl;
	}

	vector< pair<string, vector<vector<float> > > > numbers = au.retrieveNumbers( anConf.getDir(), anConf.getObjList(), anConf.getSampleNames(), anConf.getDsNames(), weights);	

	dp.drawStatistics( numbers, anConf.getDSNames() );

}


void MPAFDisplay::doPlot() {

  configure();

  setHistograms();
  dp.setLumi( anConf.getLumi() );

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

}


void
MPAFDisplay::setHistograms() {

  //loop over the datasets, find the histograms, sum them
  // and store them into the HistoManager

  //do not need to reload everything at each iteration, but let's do it for the moment
  //if(!_recompute) return;


  for(size_t ids=0;ids<_dsnames.size();ids++) {
 
	cout << "loading " << _dsnames[ids] << endl; 
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

  vector<string> obss = _hm->getObservables();

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
  anConf.reset();
  _hm->reset();
}

