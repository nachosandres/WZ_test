#include "tools/src/AnaUtils.hh"

using namespace std;

ClassImp(AnaUtils)

AnaUtils::AnaUtils() {

  _kMC=AUtils::kMC;
  _kGlobal=AUtils::kGlobal;

  //MC initialization
  eMap tmp;
  _effMap.push_back( tmp );
  _dsNames.push_back("MC");
  
  _invCut=false;
  _skipCut=false;
  
  _uncSrc="";

  _useAccForEff=false;

  //workflows
  _isWF[ _kGlobal ]=true;
  _nWF=1;
  _curWF=-100;

}

AnaUtils::~AnaUtils() {
}

bool AnaUtils::cbool(bool skip, bool bname) {

  if(!skip)
    return true;
  else{
    if(bname)
      return false;
    else return true;
  }
  
}

string 
AnaUtils::invCut(string type) {
  if(type=="<") {
    return ">=";
  }
  else if(type=="<=") {
    return ">";
  }
  else if( type==">") {
    return "<=";
  }
  else if( type==">=") {
    return "<";
  }
  else if( type=="=") {
    return "!=";
  }
  else if(type=="!=") {
    return "=";
  }
  else if(type=="[]") {
    return "]![";
  }
  else if(type=="][") {
    return "[!]";
  }
  else if(type=="[!]") {
    return "][";
  }
  else if(type=="]![") {
    return "[]";
  }
  else {
    cout<<" Warning cut "<<endl;
    return "=";
  }
} 


void
AnaUtils::setWFEfficiencies(int ids, string cName, float w, bool acc) {
  
  for(unsigned int ic=0;ic<_isWF.size();ic++) {
     if(_isWF[ic])
      setEfficiency(ids, cName, ic , w, acc);
  }
}


void AnaUtils::setEfficiency(int ids, string cName, int iCateg, float w, bool acc) {
  
  //  Acceptance
  if(_useAccForEff)
    if(!_inAcc) w=0;
  
  _itEIMap=_effMap[ ids ][iCateg].find( cName );
  if(_itEIMap==_effMap[ ids ][ iCateg ].end() ) {
    EffST tmp;
    tmp.eff = 0;
    tmp.sumw = 0; tmp.sumw2 = 0;
    tmp.N = 0;    
    tmp.sumwTot = 0; tmp.sumw2Tot = 0;
    tmp.NTot = 0;    
    
    _effMap[ ids ][ iCateg ][ cName ] = tmp;

    bool exists=false;
    for(size_t ic=0;ic<_effNames[ iCateg ].size();ic++)
      if(cName == _effNames[ iCateg ][ic]) exists=true;
    if(!exists)
      _effNames[ iCateg ].push_back( cName );
  }
  
  if(_itEIMap==_effMap[ _kMC ][ iCateg ].end() ) {
    EffST tmp;
    tmp.eff = 0;
    //    tmp.systp = 0; tmp.systm = 0;
    tmp.sumw = 0; tmp.sumw2 = 0;
    tmp.N = 0;    
    tmp.sumwTot = 0; tmp.sumw2Tot = 0;
    tmp.NTot = 0;    

    _effMap[ _kMC ][ iCateg ][ cName ] = tmp;
  }

  setNumbers(ids, cName, iCateg, w, acc);

}


void AnaUtils::setEffFromStat(int ids, string cName, int iCateg, float sw, float esw, int ngen) {
 

  //  Acceptance
  // if(_useAccForEff)
  //   if(!_inAcc) w=0;
  
  _itEIMap=_effMap[ ids ][iCateg].find( cName );
  if(_itEIMap==_effMap[ ids ][ iCateg ].end() ) {
   
    EffST tmp;
    tmp.eff = 0;
    tmp.sumw = 0; tmp.sumw2 = 0;
    tmp.N = 0;    
    tmp.sumwTot = 0; tmp.sumw2Tot = 0;
    tmp.NTot = 0;    
   

   
 
    _effMap[ ids ][ iCateg ][ cName ] = tmp;

    bool exists=false;
    for(size_t ic=0;ic<_effNames[ iCateg ].size();ic++)
      if(cName == _effNames[ iCateg ][ic]) exists=true;

    if(!exists)
      _effNames[ iCateg ].push_back( cName );
  }
  
  if(_itEIMap==_effMap[ _kMC ][ iCateg ].end() ) {
    EffST tmp;
    tmp.eff = 0;
    //    tmp.systp = 0; tmp.systm = 0;
    tmp.sumw = 0; tmp.sumw2 = 0;
    tmp.N = 0;    
    tmp.sumwTot = 0; tmp.sumw2Tot = 0;
    tmp.NTot = 0;    

    _effMap[ _kMC ][ iCateg ][ cName ] = tmp;
  }

  setNumFromStat(ids, cName, iCateg, sw, esw, ngen );

}

void AnaUtils::addDataset(string dsName) {
  
  _dsNames.push_back(dsName);
  eMap tmp;
  _effMap.push_back( tmp );
 
}

void
AnaUtils::init() {

  //first, check if the global instance exists
  // and if not, create it 
  if( _catNames.find( _kGlobal )==_catNames.end()) {
    _catNames[ _kGlobal ] = "global";
    vector<string> vtmp;
    _effNames[ _kGlobal ] = vtmp; 

    for(size_t ids=0;ids<_dsNames.size();ids++) {
      eIMap tmp;
      _effMap[ ids ].push_back( tmp );
    }
  }

}


void
AnaUtils::internalAddCategory(int iCateg, string eCateg) {
  
  _catNames[ iCateg ] = eCateg;
  vector<string> vtmp;
  _effNames[ iCateg ] = vtmp; 
    
  for(size_t ids=0;ids<_dsNames.size();ids++) {
    eIMap tmp;
    _effMap[ ids ].push_back( tmp );
  }

}

void
AnaUtils::addWorkflow(int iCateg, string eCateg) {

  _isWF[ iCateg ] = true;
  internalAddCategory(iCateg, "global_"+eCateg);
  _nWF = _isWF.size();
}


void
AnaUtils::addAutoWorkflow(string eCateg) {

  _isWF[ _nWF ] = false;
  _uncWorkflow[eCateg]=_nWF;
  internalAddCategory(_nWF, "global_"+eCateg);
  _nWF = _isWF.size();
}

void
AnaUtils::addCategory(int iCateg, string eCateg) {

  _isWF[ iCateg ] = false;
  internalAddCategory(iCateg, eCateg);
}

int
AnaUtils::getUncWorkflow(string wf) {
  return _uncWorkflow[wf];
}


void AnaUtils::setSystematics(int ids, string cName, string sName, bool up, bool down, float w) {

  bool accept[2] = {false,false};
  accept[0] = up;
  accept[1] = down;
  
  float val=w;

  // _effMap[ ids ][ 0 ][ cName ].systp+=(accept[0]?val:0);
  // _effMap[ ids ][ 0 ][ cName ].systm+=(accept[1]?val:0);

  _effMap[ ids ][ 0 ][ cName ].systsU[ sName ] +=(accept[0]?val:0);
  _effMap[ ids ][ 0 ][ cName ].systsD[ sName ] +=(accept[1]?val:0);
  
  if(_dsNames[ids]!="data" && _dsNames[ids]!="Data") {
    // _effMap[ _kMC ][ 0 ][ cName ].systp+=(accept[0]?val:0);
    // _effMap[ _kMC ][ 0 ][ cName ].systm+=(accept[1]?val:0);
    _effMap[ _kMC ][ 0 ][ cName ].systsU[ sName ] +=(accept[0]?val:0);
    _effMap[ _kMC ][ 0 ][ cName ].systsD[ sName ] +=(accept[1]?val:0);
  }

}

void
AnaUtils::getYieldSysts(string ds, string lvl) {

  int ids = -1;
  for(size_t i=0;i<_dsNames.size();i++) 
    if(ds==_dsNames[i]) {ids=i; break;}

  if(ids==-1) return;

  // int ilvl=-1;
  // for(size_t i=0;i<_effNames[0].size();i++) {
  //   if(lvl==_effNames[0][i]) {ids=i; break;}
  // }

  // if(ilvl==-1) return;
  
  EffST eST = _effMap[ ids ][ 0 ][ lvl ];


  float central = eST.sumw;
  
  float totUp=0,totDown=0;
  float dU,dD;
  
  map<string,float> rU;
  map<string,float> rD;

  for(map<string,float>::const_iterator it=eST.systsU.begin();
      it != eST.systsU.end(); it++ ) {
    
    string n =  (*it).first;
    dU = it->second - central; //temporary variation variation
    dD = eST.systsD[ n ] - central;
    
    if( dU*dD > 0) { //same sign errors
      rU[ n ] = dU>=0?(dU>dD?(dU*dU):(dD*dD)):0;
      rD[ n ] = dU<0?(dU<dD?(dU*dU):(dD*dD)):0;
      
      totUp += rU[ n ];
      totDown += rD[ n ];
    }
    else { //opposite sign errors
      rU[ n ] = dU>=0?(dU*dU):(dD*dD);
      rD[ n ] = dU<0?(dU*dU):(dD*dD);
      totUp +=rU[ n ];
      totDown +=rD[ n ];
    }

  }

  totUp = sqrt(totUp);
  totDown = sqrt(totDown);

  for(map<string,float>::const_iterator it= rU.begin();it!=rU.end();it++) {
    rU[ it->first ] = sqrt(rU[ it->first ]);
    rD[ it->first ] = sqrt(rD[ it->first ]);    
  }

  cout<<setprecision(2)<<fixed;
  cout<<" ************************************************************* "<<endl;
  cout<<" Detail of uncertainties on ("<<ds<<") yield, at the ("
      <<lvl<<") selection level "<<endl;

  

  cout<<setw(30)<<"   uncertainty source\t"<<"\t"<<" yield "<<endl;
  cout<<setw(30)<<"   statistical\t"<<"\t"<<central<<" +- "<<sqrt(eST.sumw2)
      <<" ( "<<sqrt(eST.sumw2)*100./central<<" %)"<<endl;
  
  for(map<string,float>::const_iterator it= rU.begin();it!=rU.end();it++) {
    string name="   "+it->first+"\t";
    if( fabs((it->second)+(rD[ it->first]) )>0.01*central ) { //asymetric
      cout<<setw(30)<<name<<"\t"<<" + "<<(it->second)*100/central
  	  <<" - "<<rD[ it->first]*100/central<<" %   (+"
  	  <<it->second<<" -"<<rD[ it->first]<<" events)"<<endl;
    }
    else { //symetric
      cout<<setw(30)<<name<<"\t"<<" +- "<<max((it->second),rD[ it->first])*100/central
  	  <<" % (+"<<it->second<<" -"<<rD[ it->first]<<" events)"<<endl;
    }

  }
  cout<<"------------------------------------- "<<endl;
  if(fabs(totUp-totDown)>0.01*central) { //asymetric
    cout<<setw(30)<<"   total"<<"\t"<<" + "<<totUp*100/central
  	<<" - "<<totDown*100/central<<" %   (+"
  	<<totUp<<" -"<<totDown<<" events)"<<endl; 
  }
  else { //symetric
    cout<<setw(30)<<"   total systematics"<<"\t"<<" +- "<<max(totUp,totDown)*100/central
  	<<" % (+"<<totUp<<" -"<<totDown<<" events)"<<endl;
  }


}



void AnaUtils::saveNumbers(string anName, string conName) {

  // testing write permission on output directory
  cout << endl;
  string dirname_ =  (string)(getenv("MPAF")) + "/workdir/stats/" + anName;
  FILE* test = fopen( dirname_.c_str(), "r" );
  if( test == 0 ) {
    string command_ = "mkdir -p " + dirname_; 
    assert( system( command_.c_str() ) == 0 );
  }
  else
    fclose( test );
	
  string ofilename_ = dirname_ + "/" + conName + ".dat";
  test = fopen( ofilename_.c_str(), "r" );
  if( test != 0 )	{
    fclose( test );
    TDatime datime_;
    cout << "File " << ofilename_ << " already exists, save it." << endl;
    string command_ = "mv " + ofilename_ + " " + ofilename_ + "_"; 
    ostringstream os;
    os << datime_.Get();
    command_ += os.str();
    assert( system( command_.c_str() ) == 0 );
  }

  //now storing the output
  ofstream ofile( ofilename_.c_str(), ios::out | ios::trunc );
  if(!ofile) {
    cout << "Error writing log file containing yields " << endl; 
    return;
  }

  vector<int> idxs;
  vector<string> dsNames = prepareDSNames(false, idxs);
 
  // print global efficiencies at the end======================
  vector<int> catNames;
  bool hasGlobEff = false;
  for(map<int, vector<string> >::const_iterator it = _effNames.begin(); it != _effNames.end(); ++it) {
    if( _catNames[ it->first ] == "global") {
      hasGlobEff = true; 
      continue; 
    }
    catNames.push_back( it->first ); 
  }

  if(hasGlobEff)
    catNames.push_back( _kGlobal );

  for(size_t ic = 0; ic < catNames.size(); ++ic) {
    int icat = catNames[ic];
    ofile << "categ\t" << _catNames[icat] << endl;
    bool header = true;

    // to skip the simulation in a first time
    for(size_t ic = 0; ic < _effNames[icat].size(); ++ic) { //cuts

      _itEIMap = _effMap[ _kMC ][icat].find( _effNames[icat][ ic ] );
	
      // out of the loop if no MC
      if(_itEIMap == _effMap[ _kMC ][icat].end() ) 
	break; 

      if(header){
	ofile << "\tselection\tdataset\tyield (unweighted)\tyield error" << endl; //\tnum gen evts
	header = false;
      }
      
      // to skip the simulation summary
      for(size_t id = 0; id < dsNames.size(); ++id) { //datasets
	int ids = idxs[id];

	map<string,EffST>::iterator itm;
	{ // simulation detail
	  // cut name
	  ofile << "\t" << _itEIMap -> first;
	  // dataset name
	  ofile << "\t" << dsNames[id]; 
	  // yields
	  itm = _effMap[ids][icat].find( _itEIMap -> first );
	  
	  if(itm == _effMap[ids][icat].end()) 
	    ofile << "\t 0 0\t 0 ";
	  //ofile << " - ";

	  else {
	    if(itm -> second.sumw > 0.000001 ) { 
	      ostringstream os, os2, os3;
	      os  << fixed << setprecision(4) << itm -> second.sumw;
	      os2 << fixed << setprecision(0) << itm -> second.N;
	      os3 << fixed << setprecision(4) << sqrt(itm -> second.sumw2);
	      
	      ofile << "\t" << os.str() << "\t" << os2.str() << "\t" << os3.str();
	    }
	    else
	      ofile << "\t0\t0\t0 ";
	    						
	    // number of selected events unweighted
	    //ofile << fixed << setprecision(0) << " (" << itm -> second.N << ")" << fixed << setprecision(4);
	  }

	  ofile << endl;
	}
      } // datasets
    } // cuts

    ofile << "endcateg\t" << _catNames[icat] << endl << endl;
  } // categories

  ofile.close();
}


void AnaUtils::printNumbers() {
  
  //move MC at the end
  vector<string> dsNames;
  bool hasData = _dsNames.back()=="data" || _dsNames.back()=="Data";
  size_t nds=_dsNames.size();
  
  for(size_t ids=1;ids<(hasData?nds-1:nds);ids++) {
    dsNames.push_back(_dsNames[ids]);
  }
  //dsNames.push_back(_dsNames[0]); //MC disabled for that function as unweighted yields are printed
  if(hasData)
    dsNames.push_back(_dsNames.back());

  // print global efficiencies at the end======================
  vector<int> catNames;
  bool hasGlobEff=false;
  for(map<int, vector<string> >::const_iterator it=_effNames.begin();
      it!=_effNames.end();it++) {
    
    if(it->first == _kGlobal) {hasGlobEff=true; continue; }
    catNames.push_back(  it->first ); 
  }
  if(hasGlobEff)
    catNames.push_back( _kGlobal );

  for(size_t ic=0;ic<catNames.size();ic++) {
    int icat = catNames[ic];
    
    cout<<endl<<endl;
    cout<<" ===================================================================== "<<endl;
    cout<<" ============= Efficiency category : "<<setw(17)<<_catNames[ icat ]<<" =============== "<<endl;
    cout<<" ===================================================================== "<<endl;

    for(size_t id=0;id<dsNames.size();id++) { //datasets
      string dsName = dsNames[id];
      //but id = 0 is MC
      int ids=id+1;
    
      //need last object for change of weights
      float tmpswtot=-1;
      float tmpNTot=-1;
    
      cout<<endl<< "Begin efficiencies for "<<dsName<<"   "<<_effMap[ids][icat].size()<<"  ********* "<<endl;
    
      string initName;
      for(size_t ic=0;ic<_effNames[ icat ].size();ic++) { //cut
    
	_itEIMap = _effMap[ids][icat].find( _effNames[ icat ][ ic ] );
	if(ic==0) initName = _effNames[ icat ][ ic ];
	if(_itEIMap == _effMap[ids][icat].end() ) continue;
    
	//get total value
	float eff = _itEIMap->second.sumw / _itEIMap->second.sumwTot;
	float error = StatUtils::BinomError( _itEIMap->second.NTot, eff);

	//get High and low values for systematics (if available)
	// float effL = _itEIMap->second.systm / _itEIMap->second.sumwTot;
	// float effH = _itEIMap->second.systp / _itEIMap->second.sumwTot;

	//fix for change of weight
     
	if(_itEIMap->second.sumwTot != tmpswtot && tmpswtot!=-1 ) {
	  eff = _itEIMap->second.sumw / tmpswtot;
	  if(eff>1) eff=1.;
	  error = StatUtils::BinomError( tmpNTot, eff);
	  // effL = _itMap->second.systm /tmpswtot;
	  // effH = _itMap->second.systp /tmpswtot;
	}
         
	//FIXME -> if no systematic called, let remove them
	// if( effL ==0 && effH==0 ) 
	// 	effL = eff;
	// if( effH ==0 && effH==0 )
	// 	effH = eff;
      
	// float systL = fabs(eff-effH); //inverted for the good direction
	// float systH = fabs(eff-effL);

	// cout<<systL<<"   "<<systH<<endl;

	//==========================================================
	//cout<<_itMap->second.sumw<<"   "<<_itEMap->second.begin()->second.sumwTot<<endl;
	float globEff = _itEIMap->second.sumw/_effMap[ids][icat][ initName ].sumwTot;
      
	//FIXME ugly protection against a change of weight
	if(globEff>1) globEff=1;
      
	float globErr = StatUtils::BinomError( _effMap[ids][icat][ initName ].sumwTot, globEff);      
	cout<<setprecision(3)<<fixed;
	cout<<" --> "<<setw(30)<<_itEIMap->first<<"\t  = "<<eff<<" +- "<<error;
	// if( effL !=0 || effH!=0 ) 
	// cout<<" (+ "<<systH<<" - "<<systL;
	cout<<" \t\t "<<_itEIMap->second.sumw<<"  / "
	    <<((tmpswtot==-1)?_itEIMap->second.sumwTot:tmpswtot);
	cout<<"  ("<<_itEIMap->second.N<<" / "<<((tmpNTot==-1)?_itEIMap->second.NTot:tmpNTot)<<")";
	cout<<"  ---> \t = "<<globEff*100<<" +- "<<globErr*100<<endl;
      
	tmpswtot =  _itEIMap->second.sumw;
	tmpNTot = _itEIMap->second.N;

      } //cuts
    
    } //datasets
  
  }//categories
  
  cout<<endl<<endl;

}

int
AnaUtils::getCategId(string categ) {
  int icat=0;
  for(size_t ic=0;ic<_catNames.size();ic++) {
    if(_catNames[ic]==categ) {
      icat = ic;
      break;
    }
  }

  return icat;
}

vector<string> 
AnaUtils::prepareDSNames(bool wMC, vector<int>& idxs) {
  //move MC at the end
  vector<string> dsNames;
  bool hasData = _dsNames.back()=="data" || _dsNames.back()=="Data";
  size_t nds=_dsNames.size();
  
  for(size_t ids=1;ids<(hasData?nds-1:nds);ids++) {
    dsNames.push_back(_dsNames[ids]);
    idxs.push_back(ids);
  }
  if(wMC) {
    dsNames.push_back(_dsNames[0]);
    idxs.push_back(0);
  }
  if(hasData) {
    dsNames.push_back(_dsNames.back());
    idxs.push_back(nds-1);
  }
  
  return dsNames;
}


void
AnaUtils::printTables(string categ) {
  //cout<<" categ = "<<categ<<"   "<<_effNames[ 0 ].size()<<endl;
  int icat= getCategId(categ);
  
  vector<int> idxs;
  vector<string> dsNames = prepareDSNames(true, idxs);
  bool hasData = _dsNames.back()=="data" || _dsNames.back()=="Data";
  
  //Numbers =======================================================
  cout<<endl<<endl;
  //For Latex
  
  bool header=true;
  //start from one to skip the simulation in a first time
  for(size_t ic=0;ic<_effNames[ icat ].size();ic++) { //cuts

    _itEIMap = _effMap[ _kMC ][ icat ].find( _effNames[ icat ][ ic ] );
    if(_itEIMap == _effMap[ _kMC ][ icat ].end() ) break; //out of the loop if no MC

    if(header) {
      cout<<" Cut  "<<fixed<<setprecision(2);
      for(size_t ids=0;ids<dsNames.size();ids++) { //_itEMap

        //_itEMap = _effMap[ ids ].find( dsNames[ ids ] );
        // if(_itEMap->first=="data" || _itEMap->first=="Data") 
        //   cout<<" & MC   "<<"  & "<<_itEMap->first<<"   ";
        // else
        cout<<" & "<<dsNames[ ids ]<<"   ";
      }
      cout<<" \\\\ "<<endl;
      header=false;
    }

    cout<<_effNames[ icat ][ ic ]<<" ";

    //start from one to skip the simulation summary
    for(size_t id=0;id<dsNames.size();id++) { //datasets _itEMap
      int ids=idxs[id];
      
      cout<<" & ";
      { //simulation detail
        _itEIMap=_effMap[ ids ][ icat ].find( _effNames[ icat ][ ic ] );

        if(_itEIMap==_effMap[ ids ][ icat ].end()) 
          cout<<" - ";
        else {
          if(_itEIMap->second.sumw>0.000001 ) {
            cout<<_itEIMap->second.sumw;
            cout<<" $\\pm$ "<<sqrt(_itEIMap->second.sumw2);
          }
          else
            cout<<" - "; 
        }
        
        if(dsNames[id]=="data" || dsNames[id]=="Data" ||
           (!hasData && (dsNames[id]=="MC" || (size_t)ids==dsNames.size() ) ) )
          cout<<" \\\\ "<<endl;
      }
      
    }//datasets
  }//cuts


  cout<<endl<<endl;
  

  //For Visu

  header=true;
  //to skip the simulation in a first time
  for(size_t ic=0;ic<_effNames[icat].size();ic++) { //cuts
    
    _itEIMap = _effMap[ _kMC ][icat].find( _effNames[icat][ ic ] );
    if(_itEIMap == _effMap[_kMC][icat].end() ) break; //out of the loop if no MC

    if(header) {
      cout<<setw(15)<<" Cut                "<<fixed<<setprecision(2)<<"\t";
      for(size_t ids=0;ids<dsNames.size();ids++) {
    	cout<<setw(20)<<dsNames[ids]<<"\t";
      }
      cout<<endl;
      header=false;
    }

    cout<<setw(20)<<_effNames[ icat ][ ic ]<<"\t";

    //to skip the simulation summary
    for(size_t id=0;id<dsNames.size();id++) { //datasets
      int ids = idxs[id];
      { //simulation detail
        _itEIMap=_effMap[ ids ][ icat ].find( _effNames[ icat ][ ic ] );
        if(_itEIMap==_effMap[ ids ][ icat ].end()) 
          cout<<setw(20)<<" - ";
        //cout<<" - ";
        else {
          if(_itEIMap->second.sumw>0.000001 ) {
            
            ostringstream os,os2;
            os <<fixed<<setprecision(2)<<_itEIMap->second.sumw;
            os2 <<fixed<<setprecision(2)<<sqrt(_itEIMap->second.sumw2);
        
            string tmps=os.str()+" +- "+os2.str();
            cout<<setw(20)<<tmps;
          }
          else
            cout<<" - "; 
        }
        
        if(dsNames[id]=="data" || dsNames[id]=="Data" || 
           (!hasData && (dsNames[id]=="MC" || (size_t)ids==dsNames.size() ) ) )  
          cout<<endl;
        else
          cout<<"\t";
      }
      
    }//datasets
  }//cuts

}


int 
AnaUtils::findElement(vector<string> v, string e){

  vector<string>::iterator i = find(v.begin(), v.end(), e);
  if(i == v.end()) return -1;
  return distance( v.begin(), i );

}


vector< pair<string, vector<vector<float> > > >
AnaUtils::retrieveNumbers(string categ, bool mcat, string cname) {

  vector< pair<string, vector<vector<float> > > > onums;

  int cat= getCategId(categ);
  vector<int> idxs;
  vector<string> dsNames = prepareDSNames(true, idxs);

  vector<string> cNames;
  vector<int> catIds;

  //MM some fixme needed
  if(mcat) { //monocateg
    for(map<int, vector<string> >::const_iterator itc=_effNames.begin();
	itc!=_effNames.end();itc++) { //cuts for uni-categ

      string catname = _catNames[ itc->first ];

      //if(itc->first==_kGlobal || catname.find(categ)==(size_t)-1 ) continue;
      //if(catname.find(categ)==(size_t)-1 ) continue;     
      if(catname!=categ) continue;     

      cat= getCategId(catname);
      int ic = -1;
      for(size_t i=0;i<_effNames[cat].size();i++) {
        if(itc->second[i].find(cname) != string::npos) {ic=i; break;}
      } 

      if(ic==-1) {
        cout<<"Error, no such selection name : ["<<cname<<"] in categ "<<categ<<" ("<<catname<<"), please check name"<<endl;
        return onums;
      }
      
      cNames.insert(cNames.begin(), _effNames[cat][ic] );
      catIds.insert(catIds.begin(), itc->first );
    }
  }
  else { //general
    for(size_t ic=0;ic<_effNames[cat].size();ic++) { 
      cNames.push_back( _effNames[cat][ic] );
      catIds.push_back( cat );
    }
  }

 
  //common retrieving of numbers =====================
  for(size_t ic=0;ic<cNames.size();ic++) {
    int icat = catIds[ic];
    string cN = cNames[ ic ];
    
    _itEIMap = _effMap[ _kMC ][ icat ].find( cN );
    
    pair<string, vector<vector<float> > > p;
    vector<vector<float> > v(dsNames.size(),vector<float>(4,0));

    if(mcat) {
      size_t p0= _catNames[icat].find(categ);
      p.first = _catNames[icat].substr(p0+categ.size(), categ.size()-p0-1);
    }
    else
      p.first = _itEIMap->first;
    
    p.second = v;

    //to skip the simulation summary
    for(size_t id=0;id<dsNames.size();id++) { //datasets
      int ids = idxs[id];
      
      { //simulation detail
        _itEIMap=_effMap[ ids ][ icat ].find( cN );
        if(_itEIMap==_effMap[ ids ][ icat ].end() ) {
          p.second[ids][0] = 0.; //no data for this point
        }
        else {
	  //cout<<ic<<"   "<<ids<<"   "<<_itEIMap->second.sumw<<endl;
          p.second[ids][0] = _itEIMap->second.sumw;
          p.second[ids][1] = sqrt(_itEIMap->second.sumw2);
          p.second[ids][2] = 0.;
          p.second[ids][3] = 0.;
        }
      }
     
    }//datasets
   
    onums.push_back( p );
  }//cuts
 
  //protection against empty categories
  if(mcat && onums.size()==0) {
    pair<string, vector<vector<float> > > p;
    for(size_t id=0;id<dsNames.size();id++) { //datasets
      int ids = idxs[id];
    
      p.second[ids][0] = 0.;
      p.second[ids][1] = 0.;
      p.second[ids][2] = 0.;
      p.second[ids][3] = 0.;
    }
    onums.push_back(p);
  }

  return onums;
}

bool
AnaUtils::getDataCardLines(map<string,string>& lines, vector<string> dsNames, string sigName,
			   string categ, string cname, int bin,
			   map<string,vector<string> > intNuisPars) {
  
  vector<pair<string, vector<vector<float> > > > numbers=retrieveNumbers(categ, true, cname);

  // yield lines =================================================================
  float sumBkg=0;
  float sumSig=0;
  float sumData=0;
  
  string procNameLine="";
  string procNumLine="";
  string yieldLine="";
  string dataYield="";
  string binLine="";

  int nBkg=1;
  for(unsigned int ids=1;ids<dsNames.size()+1;ids++) {//0 is MC
    
    ostringstream osB; osB<<bin;  
   
    //cout<<ids<<"    "<<numbers[0].second[ids][0]<<endl;

    if(dsNames[ids-1]!=sigName && dsNames[ids-1].find("sig")==string::npos) {
      sumBkg+= (numbers[0].second[ids][0]==0); //?0.0001:numbers[0].second[ids][0]
      
      binLine += osB.str()+"\t";
      ostringstream os;
      os<<((numbers[0].second[ids][0]==0)?0.0001:numbers[0].second[ids][0]);
      yieldLine += os.str()+"\t";
      procNameLine += dsNames[ids-1]+"\t";
    
      //=======
       ostringstream os2;
       os2<<nBkg;
       procNumLine += os2.str()+"\t";
       nBkg++;

    }
    else if(dsNames[ids-1]==sigName) {

      binLine += osB.str()+"\t";
      sumSig = numbers[0].second[ids][0];
    }
    else if(dsNames[ids-1]=="data") {
      sumData = numbers[0].second[ids][0];
    }
  }
  
  lines[ "bins" ] = binLine;

  //adding the signal =======================
  lines[ "procNames" ] = sigName+"\t"+procNameLine;
  lines[ "procNums" ] = "0\t"+procNumLine;
  
  ostringstream os;
  os<<sumSig;
  lines[ "yields" ] = os.str()+"\t"+yieldLine;
  // =================================================================
  // adding the data =================
  ostringstream os2;
  os2<<sumData;
  lines[ "dataYield" ] = os2.str(); 
  // =================================================================
  // and the nuisance parameters
  
  for(map<string,vector<string> >::const_iterator it=intNuisPars.begin();
      it!=intNuisPars.end(); it++) {
    string line;
    
    for(unsigned int ids=1;ids<dsNames.size()+1;ids++) {
      float vUp = numbers[0].second[ids][0]/numbers[0].second[ids][0]; //MM FIXME
      float vDo = numbers[0].second[ids][0]/numbers[0].second[ids][0];

      ostringstream osU, osD;
      osU<<vUp;
      osD<<vDo;
      
      if( find( it->second.begin(), it->second.end(), dsNames[ids-1] )== it->second.end() ) {

      }
      else {
	if(dsNames[ids-1]!=sigName) {
	  if(vUp==vDo)
	    line+=osU.str()+"\t";
	  else
	    line+=osD.str()+"/"+osU.str()+"\t";
	}
	else {
	  if(vUp==vDo)
	    line=osU.str()+"\t"+line;
	  else
	    line=osD.str()+"/"+osU.str()+"\t"+line;
	}
      }
    }

    //adding the header ==========================
    lines[ "NP_"+it->first ] = it->first+"\tlnN\t"+line;
  }

  if(sumBkg+sumSig==0) return false;
  return true;
}

void
AnaUtils::setSkipCut(vector<string> var, bool invCut) {

  if(!invCut)
    _skipCut=(var.size()!=0);
  if(var.size()!=0)
    _invCut=invCut;
  
  for(size_t i=0;i<var.size();i++)
    _nm1Var.insert(var[i]);
}

void 
AnaUtils::setCurrentWorkflow(int wf) {
  _curWF = wf;
}


void AnaUtils::setNumbers(int ids,string cName, int iCateg, float w, bool acc) {
  


  _effMap[ ids ][ iCateg ][ cName ].NTot +=1;
  _effMap[ ids ][ iCateg ][ cName ].sumwTot +=w;
  _effMap[ ids ][ iCateg ][ cName ].sumw2Tot +=w*w;


  if(_dsNames[ids]!="data" && _dsNames[ids]!="Data" && 
     _dsNames[ids].find("sig")==(size_t)-1) {
    _effMap[ _kMC ][ iCateg ][ cName ].NTot +=1;
    _effMap[ _kMC ][ iCateg ][ cName ].sumwTot +=w;
    _effMap[ _kMC ][ iCateg ][ cName ].sumw2Tot +=w*w;
  }

  if(acc) {
    _effMap[ ids ][ iCateg ][ cName ].N +=1;
    _effMap[ ids ][ iCateg ][ cName ].sumw +=w;
    _effMap[ ids ][ iCateg ][ cName ].sumw2 +=w*w;

    if(_dsNames[ids]!="data" && _dsNames[ids]!="Data" && 
       _dsNames[ids].find("sig")==(size_t)-1) {
      _effMap[ _kMC ][ iCateg ][ cName ].N +=1;
      _effMap[ _kMC ][ iCateg ][ cName ].sumw +=w;
      _effMap[ _kMC ][ iCateg ][ cName ].sumw2 +=w*w;
    }

  }

}


void AnaUtils::setNumFromStat(int ids, string cName, int iCateg, float sw, float esw, int ngen) {
 
  //cout<<ids<<"   "<<iCateg<<"   "<<sw<<"   "<<esw<<endl;

  _effMap[ ids ][ iCateg ][ cName ].NTot +=ngen;
  _effMap[ ids ][ iCateg ][ cName ].sumwTot +=sw;
  _effMap[ ids ][ iCateg ][ cName ].sumw2Tot +=esw*esw;

  if(_dsNames[ids]!="data" && _dsNames[ids]!="Data" && 
     _dsNames[ids].find("sig")==(size_t)-1) {
    _effMap[ _kMC ][ iCateg ][ cName ].NTot +=ngen;
    _effMap[ _kMC ][ iCateg ][ cName ].sumwTot +=sw;
    _effMap[ _kMC ][ iCateg ][ cName ].sumw2Tot +=esw*esw;
  }

  _effMap[ ids ][ iCateg ][ cName ].N +=ngen;
  _effMap[ ids ][ iCateg ][ cName ].sumw +=sw;
  _effMap[ ids ][ iCateg ][ cName ].sumw2 +=esw*esw;

  if(_dsNames[ids]!="data" && _dsNames[ids]!="Data" && 
     _dsNames[ids].find("sig")==(size_t)-1) {
    _effMap[ _kMC ][ iCateg ][ cName ].N +=ngen;
    _effMap[ _kMC ][ iCateg ][ cName ].sumw +=sw;
    _effMap[ _kMC ][ iCateg ][ cName ].sumw2 +=esw*esw;
  }

}


void 
AnaUtils::reset() {

  _effMap.clear();
  _dsNames.clear();
  _catNames.clear();
  _effNames.clear();

  _uncWorkflow.clear();
  
  //MC initialization
  eMap tmp;
  _effMap.push_back( tmp );
  _dsNames[ _kMC ]="MC";

}


void
AnaUtils::setDDCuts(vector<string> ddcuts) {
  
  for(size_t i=0;i<ddcuts.size();i++) {
    _bkgFC.insert( ddcuts[i] );
  }

}



hObs 
AnaUtils::cloneHObs(const hObs* o1) {

  hObs o2;
  for(size_t ih=0;ih<o1->hs.size();ih++) {
    o2.hs.push_back((TH1*)o1->hs[ih]->Clone());
  } 
  
  o2.nBX = o1->nBX;
  o2.nBY = o1->nBY;
  o2.binsX = o1->binsX;
  o2.binsY = o1->binsY;
  
  o2.htype = o1->htype;
  o2.titleX = o1->titleX;
  o2.titleY = o1->titleY;
  o2.name = o1->name;
  o2.type = o1->type;
  
  return o2;
}


//====================================================
//====================================================
