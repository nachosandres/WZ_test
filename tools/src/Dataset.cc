#include "src/Dataset.hh"

using namespace std;

ClassImp(Dataset)


//____________________________________________________________________________
Dataset::Dataset():
_chain(0)
{
  _isData = false;
  config("", 1, kTree );
}


//____________________________________________________________________________
Dataset::Dataset(string name):
  _chain(0)
{
  _isData = false;
  config(name, 1, kTree );
}


//____________________________________________________________________________
Dataset::Dataset(string name, int color):
  _chain(0)
{

  _isData = false;
  config(name, color, kHisto);

}


//____________________________________________________________________________
void Dataset::config(string name, int color, int content) {

  _name = name;
  _color = color;
  //_isDataDriven = false;
  _isGhost = false;
  _dsContentType = content;
}


//____________________________________________________________________________
Dataset::~Dataset() {
  delete _chain;
}

//____________________________________________________________________________
void Dataset::freeMemory() {
  delete _chain;
}


//____________________________________________________________________________
void Dataset::addSample(string sfullname, string path, string dir, string objName, string hname, float xSect, float kFact, float lumi, float eqLumi, bool loadH) {
  /*
    adds a sample to the dataset; the sample has a name encoded in sfullname,
    lies in the directory path/dir/, has a tree called objName or a histogram
    called hname, has cross section xSect, k-factor kFact, luminosity lumi
    and equivalent luminosity eqLumi (i.e. corrected for the number of events we're
    looping on)
    parameters: sfullname ("data:sname" or just "sname"), path, dir, objName, hname, xSect, kFact, lumi, eqLumi
    return: none
  */

  string sname, stname, optCat="";
	
  // decode sfullname to get _isData and sname , MM: HAS TO BE REWRITTEN!!!!!!
  if(sfullname.find(":") != (size_t) -1){

    size_t p = sfullname.rfind(":");
    stname = sfullname.substr(p + 1, sfullname.size() - p - 1);

    if(sfullname.find("data") != (size_t) -1) {
      _isData = true;
      size_t pp = sfullname.find(":");
      if(pp!=p) { //means we look at a given category in data!
	optCat = sfullname.substr(pp + 1, p-pp-1);
      }
    }
    else {
      _isData = false;
      optCat=sfullname.substr(0, p);
    }
    
    stname += "_"+optCat;
  }
  else {
    stname =  sfullname;
  }
  sname = stname;
  
  //protection against double loading in the same dataset
  for(size_t is=0;is<_samples.size();is++) {
    if(_samples[is].getName()==sname) {
      return;
    }
  }
	
  //Tree/chain initialisation =========
  if(isTreeType())
    if(_chain==NULL)
      _chain = new TChain(objName.c_str());

  //======
	
  //is Data driven?
  // if(sname.find("DD")!=(size_t)-1 ) {
  //   _isDataDriven=true;
	
  //   Sample s(sname, 0, xSect, kFact, eqLumi);
  //    _samples.push_back(s);
  //    _weights.push_back(1.);
	
  //   return;
  // }
	
  //is from Control Sample?
  _isFromCS=0;

  if(sname.find("CS")!=(size_t)-1 && sname.find("CSA14")==(size_t)-1) {
    _isFromCS=1;
    if(sname.find("CSS")!=(size_t)-1)
      _isFromCS=2;
    if(sname.find("CSC")!=(size_t)-1)
      _isFromCS=3;
    if(sname.find("CSN")!=(size_t)-1)
      _isFromCS=4;
	  
    if(sname.find("OCS")!=(size_t)-1 )
      _isFromCS+=10;
	
    Sample s(sname, 0,0, xSect, kFact, eqLumi);
    _samples.push_back(s);
    _weights.push_back(1.);
	
    return;
  }
	
  //is ghost?
  //cout<<sname<<" ghost? "<<(sname.find("ghost")!=(size_t)-1 )<<endl;
  if(sname.find("ghost")!=(size_t)-1 ) {
    _isGhost =true;
    //cout<<" !!! ghost!!! "<<endl;
    size_t p0=sname.find(" ");
    sname=sname.substr(p0+1,sname.size()-p0-1) ;
  }
	
	
  //Looking for the tree if not data-driven
  int nEvent = 0; //MM: not really needed anymore, kept for now
  string tmpPath=isTreeType()?("data/"+path):"root";
  string tmpFName=isTreeType()?(sname):objName;
  int nProcEvt = (hname=="")?-1:getNProcEvents(tmpPath, dir, tmpFName, hname);
  
  Sample s(sname, nEvent, nProcEvt, xSect, kFact, eqLumi);
  _samples.push_back(s);
  _weights.push_back( s.getLumW() );
  
	
  //tree analysis 
  if(isTreeType()) {
    loadTree(path, dir, sname, objName);
	
    cout<<" Adding "<<sname<<"  to "<<_name
	<<"   :  nEvt "<<_chain->GetEntries()<<" ("<<nProcEvt
	<<" gen) "<<" / w (/pb-1) = "<<s.getLumW()<<endl;
  }
  else {
    if(loadH) { //reading histograms only when needed (disabled for datacards)
      loadHistos(path, dir, objName, hname, optCat);
    }
    
    cout<<" Adding "<<sname<<"  to "<<_name
	<<"   :  nEvt "<<nEvent<<" ("<<nProcEvt
	<<" gen) "<<" / w (/pb-1) = "<<s.getLumW()<<endl;
  }

}

void
Dataset::addFriend(string friendname){
  _friends.push_back(friendname);
}

int
Dataset::getNProcEvents(string path, string dir, string fileName, string hname) {

  string p= string(getenv ("MPAF"))+"/workdir";
  string NameF = p+"/"+path+"/"+dir+"/"+fileName+".root";
  if(path.find(":")!=(size_t)-1) NameF=path+"/"+fileName+".root";
  if(dir.find("psi.ch")!=(size_t)-1)
    NameF="dcap://t3se01.psi.ch:22125/"+dir+"/"+fileName+".root";
  
  TFile* file = TFile::Open( NameF.c_str() );
  
  TH1* htmp = (TH1*)file->Get( hname.c_str() );
  
  int nProc=-1;
  if(htmp) {
    nProc = htmp->Integral(0,1001);
    delete htmp;
  }
  else nProc = -1;

  file->Close();
  delete file;

  return nProc;
}

int
Dataset::getNProcEvents(int evt) {
  //MM fixme, multiple sample handling not considered anymore, still useful?
  return _samples[0].getNProcEvts();

  // for(size_t iv=0;iv<_events.size();iv++) {
  //   if(evt>=_events[iv].first && evt<_events[iv].second) {
  //     return _samples[iv]->getNProcEvts();
  //   }
  // }
  // return -1;

}

// float Dataset::computeWeight(float nEvts, float xSect, float kFact, float lumi, float& eqLumi) {
  
//   float weight=1.;

//   if(eqLumi==-1) { //means that Xsection used for reweigthing
//     eqLumi = nEvts/xSect;
//   }
  
//   weight =(1./eqLumi)*lumi*kFact;
//   return weight;
// }


// string 
// Dataset::findProcess(int evt ) {

//   for(size_t iv=0;iv<_events.size();iv++) {
//     if(evt>=_events[iv].first && evt<_events[iv].second) {
//       return _samples[iv].getName();
//     }
//   }
//   return "";
// }


// float
// Dataset::findWeight(int evt ) {
  
//   for(size_t iv=0;iv<_events.size();iv++) {
//     if(evt>=_events[iv].first && evt<_events[iv].second) {
//       return _weights[iv];
//     }
//   }
//   return 0;
// }


vector<string>
Dataset::getSamples() {
  vector<string> snames;
  for(size_t is=0;is<_samples.size();is++) {
    snames.push_back( _samples[is].getName() );
  }
  return snames;
}



void 
Dataset::loadTree(string path, string dir, string sname, string objName) {
  
  TFile* datafile(nullptr);
  if(dir=="") dir=path;
  string p= string(getenv ("MPAF"))+"/workdir";
  string NameF = p+"/data/"+dir+"/"+sname+".root"; 
  if(path.find(":")!=(size_t)-1) NameF=path+"/"+sname+".root";
  if(dir.find("psi.ch")!=(size_t)-1)
    NameF="dcap://t3se01.psi.ch:22125/"+dir+"/"+sname+".root";

  datafile = TFile::Open(NameF.c_str());
  if(datafile==nullptr) { 
    cout<<" No such file "<<sname<<endl; return;
  }
    
  TTree* tmptree = (TTree*)datafile->Get( objName.c_str() );
 
  if(tmptree != nullptr ) {
    _chain->Add( (NameF+"/"+objName).c_str() ); 

    // adding friend-trees
    for (size_t ft=0; ft<_friends.size(); ft++){
      string NameFr = p+"/data/"+dir+"/"+_friends[ft]+"/evVarFriend_"+sname+".root";
      string name = _friends[ft]+" = sf/t";
      _chain->AddFriend((name).c_str(),(NameFr).c_str());
    } 
    //nEvent = tmptree->GetEntries();
  }
  else
    cout<<" Error no correct tree in "<<sname<<" file "<<endl;

  delete tmptree;
  delete datafile;
 
  //event database

  // bool empty=_events.size()==0;
  // int m=empty?0:_events.back().second;
  // int M=empty?nEvent:_events.back().second+nEvent;
  // pair<int,int> p(m,M);
  // _events.push_back(p);
  
}

void 
Dataset::loadHistos(string path, string dir, string filename, string hname, string optCat) {
  TFile* datafile(nullptr);
  
  string NameF = path+"/"+dir+"/"+filename+".root"; 
  if(path.find(":")!=(size_t)-1) NameF=dir+"/"+filename+".root";
  if(dir.find("psi.ch")!=(size_t)-1)
    NameF="dcap://t3se01.psi.ch:22125/"+dir+"/"+filename+".root";

  datafile = TFile::Open(NameF.c_str());

  //scan the file to retrieve the histograms
  TIter nextkey(datafile->GetListOfKeys());
  TKey *key;
  while ((key = ((TKey*)nextkey()))) {
    TObject* obj = key->ReadObj(); 
    if( obj==nullptr ) continue;
      
    //found a TDirectory which can be a variable
    if( (string)obj->IsA()->GetName()!=("TDirectoryFile") ) continue;
      
    string varName(obj->GetName());
    map<string, TH1*> tmp;

    if(optCat!="") {
      size_t op=varName.find(optCat);
      if(op==string::npos) continue;
      else varName.erase(op,optCat.size());
    }

    TIter nextkeyD( ((TDirectory*)obj)->GetListOfKeys() );
    TKey *keyD;
    while ((keyD = ((TKey*)nextkeyD()))) {
      TObject* objD = keyD->ReadObj(); 
      if( objD==nullptr ) continue;
      
      if( ((string)(objD->IsA()->GetName())).substr(0,2)!=("TH") &&
	  ((string)(objD->IsA()->GetName())).substr(0,2)!=("TP") ) continue;

      //matching the samples needed for the dataset
      string sName( objD->GetName() );
      for(size_t is=0;is<_samples.size();is++) {
	if( sName!=_samples[is].getName() ) continue;
	   
	//histograms and not normalization file
	if(varName=="nEvtProc") continue;
	
	if(_histos[ varName ].size()==0) { //initialization
	  tmp[ sName ] = (TH1*)((TH1*)objD->Clone());
	  _histos[ varName ]= tmp;
	}
	else {
	  _histos[ varName ][ sName ]= (TH1*)((TH1*)objD->Clone());
	}	  

	break;
	
      }
	
    } //loop keyD in directory
      
  } //loop key in file
    
  datafile->Close();
  delete datafile;

}

vector<string> 
Dataset::getObservables() {
  vector<string> names;
  for(map<string,map<string, TH1*> >::const_iterator it=_histos.begin();
      it!=_histos.end();it++) {
    names.push_back( it->first );
  }

  return names;
}

TH1*
Dataset::getHisto(string varName, string sName) {
  return _histos[ varName ][ sName ];
}


int
Dataset::hasSample(string sname) {

  for(size_t is=0;is<_samples.size();is++) {
    if(_samples[is].getName()==sname)
      return is;
  }
  
  return -1;
}

float
Dataset::getWeight(string sname) {
  int is = hasSample(sname);
  if(is==-1) return 0;
  return getWeight(is);
}
