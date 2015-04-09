{

  //#include <stdlib.h> 
  //#include <string>
  //#include <map>

  //void rootlogon(){
  //#include <iostream>

  std::cout<<" Loading Libraries "<<std::endl;
  gSystem->Load("$MPAFTools/lib/libMPAFTools.so");
  gSystem->Load("$MPAFDisplay/lib/libMPAFDisplay.so");

  if (!(gInterpreter->IsLoaded("map"))) {
    gInterpreter->ProcessLine("#include <map>");
    gInterpreter->ProcessLine("#include <string>");
  }


  // bool Recompute=1;
  // bool* pr=&Recompute;

  // bool unlock=1;
  // bool* pr2=&unlock;

  //  MPAFDisplay md;

}
