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


#include "VarClass.hh"

using namespace std;





/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR RUNNING THE CODE                                       **
******************************************************************************
*****************************************************************************/


//ClassImp(VarClass)

int VarClass::oC_=10000;
int VarClass::oT_=1000;

//____________________________________________________________________________
VarClass::VarClass() {
  /*
    constructs the VarClass class
    parameters: none
    return: none
  */

  initIds();
  _nextEvent=false;

  _su = new SystUtils();

}


//____________________________________________________________________________
VarClass::~VarClass() {
  /*
    destructs the VarClass class
    parameters: none
    return: none
  */ 

}


//____________________________________________________________________________
void VarClass::reset() {
  /*
    resets all maps and vectors that store the variables
    parameters: none
    return: none
  */

  varmVI.clear();
  varmVUI.clear();
  varmVUL.clear();
  varmVD.clear();
  varmVF.clear();
  varmVB.clear();
  varmVS.clear();
  

  for(itAI=varmAI.begin();itAI!=varmAI.end();itAI++) {
    int i = itAI->first;
    delete [] varmAI[i];
    delete [] varmAUI[i];
    delete [] varmAUL[i];
    delete [] varmAD[i];
    delete [] varmAF[i];
    delete [] varmAB[i];
    delete [] varmAS[i];
  }

  varmAI.clear();
  varmAUI.clear();
  varmAUL.clear();
  varmAD.clear();
  varmAF.clear();
  varmAB.clear();
  varmAS.clear();
	
  varmI.clear();
  varmUI.clear();
  varmUL.clear();
  varmS.clear();
  varmB.clear();
  varmD.clear();
  varmF.clear();
	
  varmTB.clear();
	
  varTypes.clear();
	
  uncmVI.clear();
  uncmVUI.clear();
  uncmVUL.clear();
  uncmVD.clear();
  uncmVF.clear();
	
  uncmI.clear();
  uncmUI.clear();
  uncmUL.clear();
  uncmD.clear();
  uncmF.clear();
  
  cnt_.clear();
  varIds_.clear();
  initIds();

}


//____________________________________________________________________________
bool VarClass::isUsefulVar(string name) {
  /*
    checks, if a variable is defined or not
    parameters: name (the name of the variable)
    return: true (if it is already defined), false (else)
  */ 

  for(size_t i = 0; i < _varnames.size(); ++i)
    if(name == _varnames[i])
      return true;

  return false;

}


//____________________________________________________________________________
void VarClass::registerVar(string name) {
  /*
    adds a variable name to the list of variables that shall be read from the tree
    parameters: name
    return: none
  */

  _varnames.push_back(name);

}

void
VarClass::registerVar(string name, string type) {

  _varnames.push_back(name);
	
  std::pair<string, int> p;
	
  if(type=="VI") { //vector<int>
    p = std::make_pair("vector<int>", -1);
  }
  if(type=="VUI") { //vector<unsigned int>
    p = std::make_pair("vector<unsigned int>", -1);
  }
  if(type=="VUL") { //vector<unsigned int>
    p = std::make_pair("vector<unsigned long>", -1);
  }
  if(type=="VF") { //vector<float>
    p = std::make_pair("vector<float>", -1);
  }
  if(type=="VD") { //vector<double
    p = std::make_pair("vector<double>", -1);
  }
  if(type=="VB") { //vector<bool>
    p = std::make_pair("vector<bool>", -1);
  }
  if(type=="string") { //string
    p = std::make_pair("string", -1);
  }
  if(type=="VS") { //vector<string>
    p = std::make_pair("vector<string>", -1);
  }
  if(type=="TBits") { //TBits
    p = std::make_pair("TBits", -1);
  }
  else if(type=="I") { //int
    p = std::make_pair("", 3);
  }
  else if(type=="UI") { //unsigned int
    p = std::make_pair("", 13);
  }
  else if(type=="UL") { //unsigned long
    p = std::make_pair("", 14);
  }
  else if(type=="F") { //float
    p = std::make_pair("", 5);
  }
  else if(type=="B") { //bool
    p = std::make_pair("", 18);
  }
  else if(type=="D") { //double
    p = std::make_pair("", 8);
  }
  else if(type=="AI") { //int
    p = std::make_pair("arrayI", -1);
  }
  else if(type=="AUI") { //unsigned int
    p = std::make_pair("arrayUI", -1);
  }
  else if(type=="AUL") { //unsigned long
    p = std::make_pair("arrayUL", -1);
  }
  else if(type=="AF") { //float
    p = std::make_pair("arrayF", -1);
  }
  else if(type=="AB") { //bool
    p = std::make_pair("arrayB", -1);
  }
  else if(type=="AD") { //double
    p = std::make_pair("arrayD", -1);
  }
  else if(type=="AS") { //string
    p = std::make_pair("arrayS", -1);
  }


	
  _varTypes[ name ]=p;

}



// //____________________________________________________________________________
 string VarClass::getS(string name, int idx) {
  /*
    returns the value of a string variable
    parameters: name (variable name), idx (index of the element, if any)
    return: the value in the tree entry
  */

   itVId_ = varIds_.find(name);
   if(itVId_ == varIds_.end() )
     cout << " error, no such variable " << name << endl;
   int id = itVId_->second;
   int cType = id/oC_;
   int tType = (id-cType*oC_)/oT_;
   int key = (id-cType*oC_ - tType/oT_);

   if(key!=kString) return "";


   switch(cType) {
   case kScalar: {return varmS[key]; }
   case kVector: {return (*varmVS[key])[idx];}
   case kArray: {return varmAS[key][idx];}
   }
   return 0;

 }


// //____________________________________________________________________________
unsigned int VarClass::getSize(string name) {
  //   /*
  //     returns the size of a variable if it is a vector
  //     parameters: name (the variable)
  //     return: its size
  //   */
  //not finished

   // itVId_ = varIds_.find(name);
   // if(itVId_ == varIds_.end() )
   //   cout << " error, no such variable " << name << endl;
   // int id = itVId_->second;
   // int cType = id/oC_;
   // int tType = (id-cType*oC_)/oT_;
   // int key = (id-cType*oC_ - tType/oT_);

   // if(kScalar==cType) return 0;

  
   // switch(cType) {
   // case kVector: {return (*varmVS[key]).size();}
   // case kArray: {return varmAS[key][idx];}
   // }

   return 0;
}
void VarClass::buildFriendTree(TTree* tree, bool bypass){ 
  TObjLink *lnk = tree->GetListOfFriends()->FirstLink();
  
  while (lnk) {
    TTree *ft = (TTree*) tree->GetFriend(lnk->GetObject()->GetName());

    TObjArray* branches =  ft -> GetListOfBranches();
    string name;
    
    EDataType t;
    TClass* cc;
    string type;
    
    //for arrays
    TObjArray *leaves = ft->GetListOfLeaves();
    TLeaf *leaf;
    TLeaf *leafcount;
    int len;
    
    for(int ib = 0; ib < branches -> GetEntries(); ++ib) {
      len = -1;
      type = "";
      t = (EDataType) -1;

      name = (string)( ((*branches)[ib]) -> GetName());
      ((TBranchSTL*)((*branches)[ib])) -> GetExpectedType(cc,t);	
      
      //determine if it is array
      leaf = (TLeaf*)leaves->UncheckedAt(ib);
      leafcount =leaf->GetLeafCount();
      
      // vector or container 
      if( t == -1 )
	type = (string)(cc -> GetName());
      
      if(leafcount) {
	len = leafcount->GetMaximum();
      }
	
      // failed to find the type of the variable automatically
      // if( type == "" ) {
      //   map<string,std::pair<string, int> >::const_iterator it = _varTypes.find( name );
      //   if( it != _varTypes.end() ) {
      // 	type = it -> second.first;
      // 	t = (EDataType)(it -> second.second);
      // 	//cout << " manual " << type << "  " << t << endl;
      //   }
      // }
      
      // by default, status disabled
      if( !bypass )
	tree -> SetBranchStatus( name.c_str() , 0);
      
      // variable to be registered	
      if( isUsefulVar(name) ) {
	// enable status
	tree -> SetBranchStatus( name.c_str() , 1);
	
	// register branch
	registerBranch(tree, name, type, t, len );
      }
    }
    lnk = lnk->Next(); 
  }
}
//____________________________________________________________________________
void VarClass::buildTree(TTree* tree, bool bypass) {
  /*
    builds the tree, registers all branches that one wants to read
    parameters: tree (the tree), bypass (true, if we do not want to use the
    default where all branches are disabled)
    return: none
  */

  TObjArray* branches =  tree -> GetListOfBranches();
  string name;
	
  EDataType t;
  TClass* cc;
  string type;
	
  //for arrays
  TObjArray *leaves = tree->GetListOfLeaves();
  TLeaf *leaf;
  TLeaf *leafcount;
  int len;
  
  for(int ib = 0; ib < branches -> GetEntries(); ++ib) {

    len = -1;
    type = "";
    t = (EDataType) -1;

    name = (string)( ((*branches)[ib]) -> GetName());
    ((TBranchSTL*)((*branches)[ib])) -> GetExpectedType(cc,t);	
	
    //determine if it is array
    leaf = (TLeaf*)leaves->UncheckedAt(ib);
    leafcount =leaf->GetLeafCount();
    
    // vector or container 
    if( t == -1 )
      type = (string)(cc -> GetName());
	
    if(leafcount) {
      len = leafcount->GetMaximum();
    }
	
    // failed to find the type of the variable automatically
    // if( type == "" ) {
    //   map<string,std::pair<string, int> >::const_iterator it = _varTypes.find( name );
    //   if( it != _varTypes.end() ) {
    // 	type = it -> second.first;
    // 	t = (EDataType)(it -> second.second);
    // 	//cout << " manual " << type << "  " << t << endl;
    //   }
    // }
	
    // by default, status disabled
    if( !bypass )
      tree -> SetBranchStatus( name.c_str() , 0);
	
    // variable to be registered	
    if( isUsefulVar(name) ) {

      // enable status
      tree -> SetBranchStatus( name.c_str() , 1);
		  
      // register branch
      registerBranch(tree, name, type, t, len );
    }
  }
}


//____________________________________________________________________________
void VarClass::registerBranch(TTree* tree, string name, string type, EDataType t, int len) {
  /*
    sets the branch address of a variable (name) with type (type) in the tree (tree)
    parameters: tree, name, type, t ()
    return: none
  */

  int key=0;
 
  // vectors or containers

  if(t==-1) {
    
    // VI: vector<int>
    if( type == "vector<int>" ) {
      if( varIds_.find(name) != varIds_.end() ) {
	cout << " Warning, " << name << " already registered" << endl;
	return;
      }

      setIds( name, kVector, kInt, key);
      varmVI[ key ] = NULL;
      tree -> SetBranchAddress( name.c_str() , &(varmVI[ key ]) );
    }

    // VUI: vector<unsigned int>
    else if( type == "vector<unsigned int>" ) {
      if( varIds_.find(name) != varIds_.end() ) {
	cout << " Warning, " << name << " already registered" << endl;
	return;
      }

      setIds( name, kVector, kUInt, key);
      varmVUI[ key ] = NULL;
      tree -> SetBranchAddress( name.c_str() , &(varmVUI[ key ]) );
    }

    // VUI: vector<unsigned long>
    else if( type == "vector<unsigned long>" ) {
      if( varIds_.find(name) != varIds_.end() ) {
	cout << " Warning, " << name << " already registered" << endl;
	return;
      }

      setIds( name, kVector, kULong, key);
      varmVUL[ key ] = NULL;
      tree->SetBranchAddress( name.c_str() , &(varmVUL[ key ]) );
    }

    // VF: vector<float>
    else if( type == "vector<float>" || type == "Float_t" ) {
      if( varIds_.find(name) != varIds_.end() ) {
	cout << " Warning, " << name << " already registered" << endl;
	return;
      }

      setIds( name, kVector, kFloat, key);
      varmVF[ key ] = NULL;
      tree -> SetBranchAddress( name.c_str() , &(varmVF[ key ]) );
    }

    // VD: vector<double> 
    else if( type == "vector<double>" ) { 
      if( varIds_.find(name) != varIds_.end() ) {
	cout << " Warning, " << name << " already registered" << endl;
	return;
      }

      setIds( name, kVector, kDouble, key);
      varmVD[ key ] = NULL;
      tree->SetBranchAddress( name.c_str() , &(varmVD[ key ]) );
    }

    // VB: vector<bool>
    else if( type == "vector<bool>" ) {		  
      if( varIds_.find(name) != varIds_.end() ) {
	cout << " Warning, " << name << " already registered" << endl;
	return;
      }

      setIds( name, kVector, kBool, key);
      varmVB[ key ] = NULL;
      tree -> SetBranchAddress( name.c_str() , &(varmVB[ key ]) );
    }

    // S: string
    else if( type == "string" ) {  
      if( varIds_.find(name) != varIds_.end() ) {
	cout << " Warning, " << name << " already registered" << endl;
	return;
      }

      setIds( name, kScalar, kString, key);
      varmS[ key ] = "";
      tree -> SetBranchAddress( name.c_str() , &(varmS[ key ]) );  
    }

    // VS: vector<string>
    else if( type == "vector<string>" ) {		
      if( varIds_.find(name) != varIds_.end() ) {
	cout << " Warning, " << name << " already registered" << endl;
	return;
      }

      setIds( name, kVector, kString, key);
      varmVS[ key ] = NULL;
      tree -> SetBranchAddress( name.c_str() , &(varmVS[ key ]) );
    }

    // AS: arrayString
    else if( type == "string" && len!=-1) {
      if( varIds_.find(name) != varIds_.end() ) {
	cout << " Warning, " << name << " already registered" << endl;
	return;
      }

      setIds( name, kArray, kString, key);
      varmAS[ key ] = new string[len];
      tree -> SetBranchAddress( name.c_str() , varmAS[ key ] );
    }

    // TBits
    else if( type == "TBits" ) {	  
      if( varIds_.find(name) != varIds_.end() ) {
	cout << " Warning, " << name << " already registered" << endl;
	return;
      }
      varmTB[ name ] = NULL;
      tree -> SetBranchAddress( name.c_str() , &(varmTB[ name ]) );
    }

  }

  // AI: arrayI
  else if(t==3 && len!=-1) {
    if( varIds_.find(name) != varIds_.end() ) {
      cout << " Warning, " << name << " already registered" << endl;
      return;
    }
    
    setIds( name, kArray, kInt, key);
    varmAI[ key ] = new int[len];
    tree -> SetBranchAddress( name.c_str() , varmAI[ key ] );
  }
		
  // AUI: arrayUI
  else if(t==13 && len!=-1) {
    if( varIds_.find(name) != varIds_.end() ) {
      cout << " Warning, " << name << " already registered" << endl;
      return;
    }

    setIds( name, kArray, kUInt, key);
    varmAUI[ key ] = new unsigned int[len];
    tree -> SetBranchAddress( name.c_str() , varmAUI[ key ] );
  }
  // AF: arrayF
  else if(t==5 && len!=-1) {
    if( varIds_.find(name) != varIds_.end() ) {
      cout << " Warning, " << name << " already registered" << endl;
      return;
    }

    setIds( name, kArray, kFloat, key);
    varmAF[ key ] = new float[len];
    tree -> SetBranchAddress( name.c_str() , varmAF[ key ] );
  }
		
  // AD: arrayD
  else if( t==8 && len!=-1 ) {
    if( varIds_.find(name) != varIds_.end() ) {
      cout << " Warning, " << name << " already registered" << endl;
      return;
    }

    setIds( name, kArray, kDouble, key);
    varmAD[ key ] = new double[len];
    tree->SetBranchAddress( name.c_str() , varmAD[ key ] );
  }

  // AB: arrayB
  else if(  t==18 && len!=-1 ) {
    if( varIds_.find(name) != varIds_.end() ) {
      cout << " Warning, " << name << " already registered" << endl;
      return;
    }

    setIds( name, kArray, kBool, key);
    varmAB[ key ] = new bool[len];
    tree -> SetBranchAddress( name.c_str() , varmAB[ key ] );
  }
		

  // Scalars

  // I: int
  else if(t==3) {  
    if( varIds_.find(name) != varIds_.end() ) {
      cout << " Warning, " << name << " already registered" << endl;
      return;
    }
    
    setIds( name, kScalar, kInt, key);
    varmI[ key ] =0;
    tree -> SetBranchAddress( name.c_str() , &(varmI[ key ]) );
  }

  // UI: unsigned int
  else if( t == 13 ) {
    if( varIds_.find(name) != varIds_.end() ) {
      cout << " Warning, " << name << " already registered" << endl;
      return;
    }

    setIds( name, kScalar, kUInt, key);
    varmUI[ key ] =0;
    tree -> SetBranchAddress( name.c_str() , &(varmUI[ key ]) );
  }

  // UL: unsigned long
  else if( t == 14 ) {  
    if( varIds_.find(name) != varIds_.end() ) {
      cout << " Warning, " << name << " already registered" << endl;
      return;
    }

    setIds( name, kScalar, kULong, key);
    varmUL[ key ] =0;
    tree -> SetBranchAddress( name.c_str() , &(varmUL[ key ]) );
  }

  // F: float
  else if( t == 5 ) {
    if( varIds_.find(name) != varIds_.end() ) {
      cout << " Warning, " << name << " already registered" << endl;
      return;
    }

    setIds( name, kScalar, kFloat, key);
    varmF[ key ] =0.;
    tree -> SetBranchAddress( name.c_str() , &(varmF[ key ]) );
  }

  // B: bool
  else if( t == 18 ) {
    if( varIds_.find(name) != varIds_.end() ) {
      cout << " Warning, " << name << " already registered" << endl;
      return;
    }

    setIds( name, kScalar, kBool, key);
    varmB[ key ] =0;
    tree -> SetBranchAddress( name.c_str() , &(varmB[ key ]) );
  }

  // D: double
  else if( t == 8 ) {
    if( varIds_.find(name) != varIds_.end() ) {
      cout << " Warning, " << name << " already registered" << endl;
      return;
    }

    setIds( name, kScalar, kDouble,key);
    varmD[ key ] =0.;
    tree -> SetBranchAddress( name.c_str() , &(varmD[ key ]) );
  }

}

void 
VarClass::initIds() {

  for(int ic=1;ic<kNConts;ic++) {
    for(int it=1;it<kNTypes;it++) {
      cnt_[ic*oC_+it*oT_]=0;
    }
  }
}

void
VarClass::setIds(string name, int cont, int type, int& id) {
  int cat=cont*oC_ + type*oT_;
  int key=cat + cnt_[cat];
  id = cnt_[cat];
  varIds_[ name ]=key;
  cnt_[cat]++;
}

float 
VarClass::findValue(int id, int idx) {

  int cType = (id/oC_);
  int tType = ((id-cType*oC_)/oT_);
  int key = (id-cType*oC_ - tType*oT_);
  
  switch(cType) {
  case kScalar: {return findSVal(tType, key ); }
  case kVector: {return findVVal(tType, key, idx );}
  case kArray: {return findAVal(tType, key, idx );}
  }
  return 0;
}

float VarClass::findSVal(int tType, int key) {

  switch(tType) {
  case kInt: {return (float)varmI[key];}
  case kUInt: {return (float)varmUI[key];}
  case kULong: {return (float)varmUL[key];}
  case kDouble: {return (float)varmD[key];}
  case kFloat: {return (float)varmF[key];}
    //case kString: {return varmS[key];}
  case kBool: {return (float)varmB[key];}
  }
  return 0;
}

float VarClass::findVVal(int tType, int key, int idx) {

  switch(tType) {
  case kInt: {return (float)((*varmVI[key])[idx]);}
  case kUInt: {return (float)((*varmVUI[key])[idx]);}
  case kULong: {return (float)((*varmVUL[key])[idx]);}
  case kDouble: {return (float)((*varmVD[key])[idx]);}
  case kFloat: {return (float)((*varmVF[key])[idx]);}
    //case kString: {return (*varmVS[key])[idx];}
  case kBool: {return (float)((*varmVB[key])[idx]);}
  }
  return 0;
}

float VarClass::findAVal(int tType, int key, int idx) {

  switch(tType) {
  case kInt: {return (float)(varmAI[key][idx]);}
  case kUInt: {return (float)(varmAUI[key][idx]);}
  case kULong: {return (float)(varmAUL[key][idx]);}
  case kDouble: {return (float)(varmAD[key][idx]);}
  case kFloat: {return (float)(varmAF[key][idx]);}
    //case kString: {return varmAS[key][idx];}
  case kBool: {return (float)(varmAB[key][idx]);}
  }
  return 0;
}


float VarClass::get(string name, int idx) {
  
  itVId_ = varIds_.find(name);
  if(itVId_ == varIds_.end() )
    cout << " error, no such variable " << name << endl;

  return findValue(itVId_->second, idx);
}

//____________________________________________________________________________
// bool VarClass::tryType(string name, string type) {
//   /*
//     checks, whether a variable has the type one expects
//     parameters: name (the variable), type (the expected type; either "S", "B",
//     "UI", "UL", "I", "D", "F", "VS", "VB", "VUI", "VUL", "VI", "VD",
//     "VF")
//     return: true (if the variable is of the expected type), false (else)
//   */


//   // Scalars

//   // string
//   if(type == "S") {
//     itS = varmS.find( name );
//     if( itS != varmS.end() ) return true;
//   }
	
//   // bool
//   else if(type == "B") {
//     itB = varmB.find( name );
//     if( itB != varmB.end() ) return true;
//   }

//   // unsigned int
//   else if(type == "UI") {
//     itUI = varmUI.find( name );
//     if( itUI != varmUI.end() ) return true;
//   }

//   // unsigned long
//   else if(type == "UL") {
//     itUL = varmUL.find( name );
//     if( itUL != varmUL.end() ) return true;
//   }

//   // int
//   else if(type == "I") {
//     itI = varmI.find( name );
//     if( itI != varmI.end() ) return true;
//   }

//   // double
//   else if(type == "D") {
//     itD = varmD.find( name );
//     if( itD != varmD.end() ) return true;
//   }

//   // float
//   else if(type == "F") {
//     itF = varmF.find( name );
//     if( itF != varmF.end() ) return true;
//   }


//   //Vectors

//   // vector<string>
//   else if(type == "VS") {
//     itVS = varmVS.find( name );
//     if( itVS != varmVS.end() ) return true;
//   }

//   // vector<bool>
//   else if(type == "VB") {
//     itVB = varmVB.find( name );
//     if( itVB != varmVB.end() ) return true;
//   }

//   // vector<unsigned int>
//   else if(type == "VUI") {
//     itVUI = varmVUI.find( name );
//     if( itVUI != varmVUI.end() ) return true;
//   }

//   // vector<unsinged long>
//   else if(type == "VUL") {
//     itVUL = varmVUL.find( name );
//     if( itVUL != varmVUL.end() ) return true;
//   }

//   // vector<int>
//   else if(type == "VI") {
//     itVI = varmVI.find( name );
//     if( itVI != varmVI.end() ) return true;
//   }

//   // vector<double>
//   else if(type == "VD") {
//     itVD = varmVD.find( name );
//     if( itVD != varmVD.end() ) return true;
//   }

//   // vector<float>
//   else if(type == "VF") {
//     itVF = varmVF.find( name );
//     if( itVF != varmVF.end() ) return true;
//   }

//   // type not found	
//   cout << "Error for var " << name << endl;
//   return 0;

// }


// const vector<int>& VarClass::getVI(string name) {

//   vector<int> tmp;
//   itVI = varmVI.find( name );

//   if(itVI != varmVI.end() ) 
//     return (*((*itVI).second));
      
//   cout << " error, no such variable " << name << endl;
//   return tmp;

// }

// const vector<unsigned int>& VarClass::getVUI(string name) {

//   vector<unsigned int> tmp;
//   itVUI = varmVUI.find( name );
  
//   if( itVUI != varmVUI.end() ) 
//     return (*((*itVUI).second));

//   cout << " error, no such variable " << name << endl;
//   return tmp;

// }

// const vector<unsigned long>& VarClass::getVUL(string name) {

//   vector<unsigned long> tmp;
//   itVUL = varmVUL.find( name );
  
//   if( itVUL != varmVUL.end() )
//     return (*((*itVUL).second));

//   cout << " error, no such variable " << name << endl;
//   return tmp;

// }

// const vector<bool>& VarClass::getVB(string name) {

//   vector<bool> tmp;
//   itVB = varmVB.find( name );
	  
//   if( itVB != varmVB.end() )
//     return (*((*itVB).second));
	
//   cout << " error, no such variable " << name << endl;
//   return tmp;

// }

// const vector<double>& VarClass::getVD(string name) {

//   vector<double> tmp;
//   itVD = varmVD.find( name );
	  
//   if( itVD != varmVD.end() )
//     return (*((*itVD).second));

//   cout << " error, no such variable " << name << endl;
//   return tmp;

// }


// const vector<float>& VarClass::getVF(string name) {

//   vector<float> tmp;	
//   itVF = varmVF.find( name );
	  
//   if( itVF != varmVF.end() )
//     return (*((*itVF).second));
	
//   cout << " error, no such variable " << name << endl;
//   return tmp;
// }

// const vector<string>& VarClass::getVS(string name) {

//   vector<string> tmp;
//   itVS = varmVS.find( name );
	  
//   if( itVS != varmVS.end() )
//     return (*((*itVS).second));

//   cout << " error, no such variable " << name << endl;
//   return tmp;

// }


/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR SYSTEMATIC UNCERTAINTIES                               **
******************************************************************************
*****************************************************************************/

// //____________________________________________________________________________
void VarClass::applySystVar(string name, int dir, string mvar, float mag, string type) {

  int id = varIds_.find(mvar)->second;
  
  backPortVar(id);

  int cType = (id/oC_);
  int tType = ((id-cType*oC_)/oT_);
  int key = (id-cType*oC_ - tType*oT_);
  
  switch(cType) {
  case kScalar:
    switch(tType) {
    case kInt:    {_su->systOp<int>(name, dir, type, varmI[key], mag); break;}
    case kUInt:   {_su->systOp<unsigned int>(name, dir, type, varmUI[key], mag); break;}
    case kULong:  {_su->systOp<unsigned long>(name, dir, type, varmUL[key], mag); break;}
    case kDouble: {_su->systOp<double>(name, dir, type, varmD[key], mag); break;}
    case kFloat:  {_su->systOp<float>(name, dir, type, varmF[key], mag); break;}
      //   case kBool:   {_su->systOpA<bool>(name, dir, type, varmAB[key], mag); break;}
    }
  case kVector:
    switch(tType) {
    case kInt:    {_su->systOpV<int>(name, dir, type, varmVI[key], mag); break;}
    case kUInt:   {_su->systOpV<unsigned int>(name, dir, type, varmVUI[key], mag); break;}
    case kULong:  {_su->systOpV<unsigned long>(name, dir, type, varmVUL[key], mag); break;}
    case kDouble: {_su->systOpV<double>(name, dir, type, varmVD[key], mag); break;}
    case kFloat:  {_su->systOpV<float>(name, dir, type, varmVF[key], mag); break;}
      //   case kBool:   {_su->systOpA<bool>(name, dir, type, varmAB[key], mag); break;}
    }
  case kArray:
    switch(tType) {
    case kInt:    {_su->systOpA<int>(name, dir, type, varmAI[key], mag); break;}
    case kUInt:   {_su->systOpA<unsigned int>(name, dir, type, varmAUI[key], mag); break;}
    case kULong:  {_su->systOpA<unsigned long>(name, dir, type, varmAUL[key], mag); break;}
    case kDouble: {_su->systOpA<double>(name, dir, type, varmAD[key], mag); break;}
    case kFloat:  {_su->systOpA<float>(name, dir, type, varmAF[key], mag); break;}
      //   case kBool:   {_su->systOpA<bool>(name, dir, type, varmAB[key], mag); break;}
    }
  }

}




//____________________________________________________________________________
void
VarClass::applySystVar(string name, int dir, string mvar, vector<string> vars, string db, string type) {
  
  int id = varIds_.find(mvar)->second;
  backPortVar(id);
  
  vector<vector<float> > vals; //order needed
  for(size_t iv=0;iv<vars.size();iv++) {
    int vid = varIds_.find(vars[iv])->second;
    vector<float> p = getUnivF( vid );
    vals.push_back(p);
  }

  int cType = (id/oC_);
  int tType = ((id-cType*oC_)/oT_);
  int key = (id-cType*oC_ - tType*oT_);
  
  switch(cType) {
  case kScalar:
    switch(tType) {
    case kInt:    {_su->systOp<int>(name, dir, type, varmI[key], db, vals); break;}
    case kUInt:   {_su->systOp<unsigned int>(name, dir, type, varmUI[key], db, vals); break;}
    case kULong:  {_su->systOp<unsigned long>(name, dir, type, varmUL[key], db, vals); break;}
    case kDouble: {_su->systOp<double>(name, dir, type, varmD[key], db, vals); break;}
    case kFloat:  {_su->systOp<float>(name, dir, type, varmF[key], db, vals); break;}
      //   case kBool:   {_su->systOpA<bool>(name, dir, type, varmAB[key], db, vals); break;}
    }
  case kVector:
    switch(tType) {
    case kInt:    {_su->systOpV<int>(name, dir, type, varmVI[key], db, vals); break;}
    case kUInt:   {_su->systOpV<unsigned int>(name, dir, type, varmVUI[key], db, vals); break;}
    case kULong:  {_su->systOpV<unsigned long>(name, dir, type, varmVUL[key], db, vals); break;}
    case kDouble: {_su->systOpV<double>(name, dir, type, varmVD[key], db, vals); break;}
    case kFloat:  {_su->systOpV<float>(name, dir, type, varmVF[key], db, vals); break;}
      //   case kBool:   {_su->systOpA<bool>(name, dir, type, varmAB[key], db, vals); break;}
    }
  case kArray:
    switch(tType) {
    case kInt:    {_su->systOpA<int>(name, dir, type, varmAI[key], db, vals); break;}
    case kUInt:   {_su->systOpA<unsigned int>(name, dir, type, varmAUI[key], db, vals); break;}
    case kULong:  {_su->systOpA<unsigned long>(name, dir, type, varmAUL[key], db, vals); break;}
    case kDouble: {_su->systOpA<double>(name, dir, type, varmAD[key], db, vals); break;}
    case kFloat:  {_su->systOpA<float>(name, dir, type, varmAF[key], db, vals); break;}
      //   case kBool:   {_su->systOpA<bool>(name, dir, type, varmAB[key], db, vals); break;}
    }
  }

}

//____________________________________________________________________________
void
VarClass::backPortVar(int mvar) {


  int cType = (mvar/oC_);
  int tType = ((mvar-cType*oC_)/oT_);
  int key = (mvar-cType*oC_ - tType*oT_);
  
  switch(cType) {
  case kScalar:
    switch(tType) {
    case kInt:    {storeAccess<int>(key, varmI, uncmI); break;}
    case kUInt:   {storeAccess<unsigned int>(key, varmUI, uncmUI); break;}
    case kULong:  {storeAccess<unsigned long>(key, varmUL, uncmUL); break;}
    case kDouble: {storeAccess<double>(key, varmD, uncmD); break;}
    case kFloat:  {storeAccess<float>(key, varmF, uncmF); break;}
      //   case kBool:   {_su->systOpA<bool>(name, dir, type, varmAB[key], db, vals); break;}
    }
  case kVector:
    switch(tType) {
    case kInt:    {storeAccessV<int>(key, varmVI, uncmVI); break;}
    case kUInt:   {storeAccessV<unsigned int>(key, varmVUI, uncmVUI); break;}
    case kULong:  {storeAccessV<unsigned long>(key, varmVUL, uncmVUL); break;}
    case kDouble: {storeAccessV<double>(key, varmVD, uncmVD); break;}
    case kFloat:  {storeAccessV<float>(key, varmVF, uncmVF); break;}
      //   case kBool:   {_su->systOpA<bool>(name, dir, type, varmAB[key], db, vals); break;}
    }
  case kArray:
    switch(tType) {
    case kInt:    {storeAccessA<int>(key, varmAI, uncmAI); break;}
    case kUInt:   {storeAccessA<unsigned int>(key, varmAUI, uncmAUI); break;}
    case kULong:  {storeAccessA<unsigned long>(key, varmAUL, uncmAUL); break;}
    case kDouble: {storeAccessA<double>(key, varmAD, uncmAD); break;}
    case kFloat:  {storeAccessA<float>(key, varmAF, uncmAF); break;}
      //   case kBool:   {_su->systOpA<bool>(name, dir, type, varmAB[key], db, vals); break;}
    }
  }
  
}

//____________________________________________________________________________
void
VarClass::backPortAllVars() {
  
  multiReinit<unsigned int>( varmUI, uncmUI );
  multiReinit<unsigned long>( varmUL, uncmUL );
  multiReinit<int>( varmI, uncmI );
  multiReinit<double>( varmD, uncmD );
  multiReinit<float>( varmF, uncmF );

  multiReinitV<unsigned int>( varmVUI, uncmVUI );
  multiReinitV<unsigned long>( varmVUL, uncmVUL );
  multiReinitV<int>( varmVI, uncmVI );
  multiReinitV<double>( varmVD, uncmVD );
  multiReinitV<float>( varmVF, uncmVF );

  multiReinitA<unsigned int>( varmAUI, uncmVUI );
  multiReinitA<unsigned long>( varmAUL, uncmVUL );
  multiReinitA<int>( varmAI, uncmVI );
  multiReinitA<double>( varmAD, uncmVD );
  multiReinitA<float>( varmAF, uncmVF );

}


vector<float>
VarClass::getUnivF(int id) {
  
  vector<float> vf;

  int cType = (id/oC_);
  int tType = ((id-cType*oC_)/oT_);
  int key = (id-cType*oC_ - tType*oT_);

  switch(cType) {
  case kScalar: {vf.push_back( findSVal(tType, key) ); break;}
  case kVector: { 
    switch(tType) {
    case kInt:    { vf = convertVal<int>( (*varmVI[key]) ); break;}
    case kUInt:   { vf = convertVal<unsigned int>( (*varmVUI[key]) ); break;}
    case kULong:  { vf = convertVal<unsigned long>( (*varmVUL[key]) ); break;}
    case kDouble: { vf = convertVal<double>( (*varmVD[key]) ); break;}
    case kFloat:  { vf = convertVal<float>( (*varmVF[key]) ); break;}
    }
  }
  case kArray: {
    switch(tType) {
    case kInt:    { vf = convertVal<int>(varmAI[key]); break;}
    case kUInt:   { vf = convertVal<unsigned int>(varmAUI[key]); break;}
    case kULong:  { vf = convertVal<unsigned long>(varmAUL[key]); break;}
    case kDouble: { vf = convertVal<double>(varmAD[key]); break;}
    case kFloat:  { vf = convertVal<float>(varmAF[key]); break;}
    }
  }
  }
    
  return vf;
}


//____________________________________________________________________________
// string VarClass::getType(string mvar) {
//   /*
//     returns the type as string (abbreviation) of a variable
//     parameters: mvar (the variable)
//     return: the type ("UI", "UL", "I", "D", "F", "VUI", "VUL", "VI", "VD", "VF")
//   */

//   if( tryType(mvar,"UI" ) ) return "UI";
//   if( tryType(mvar,"UL" ) ) return "UL";
//   if( tryType(mvar,"I"  ) ) return "I";
//   if( tryType(mvar,"D"  ) ) return "D";
//   if( tryType(mvar,"F"  ) ) return "F";
	
//   if( tryType(mvar,"VUI") ) return "VUI";
//   if( tryType(mvar,"VUL") ) return "VUL";
//   if( tryType(mvar,"VI" ) ) return "VI";
//   if( tryType(mvar,"VD" ) ) return "VD";
//   if( tryType(mvar,"VF" ) ) return "VF";
	
//   return "raté";
  
// }


