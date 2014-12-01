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


#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "analysis/src/AnalysisFactory.hh"

using namespace std;

//_____________________________________________________________________________________
void printUsageAndExit(){


  std::cout << "printing usage" << std::endl;
  exit(1);

}


//_____________________________________________________________________________________
int main(int argc, char* argv[]) {
  /*
    main function, which does the whole job
    parameters: argument list given to the executable
    return: none
  */

  // Starting MPAF

  std::cout << ">> STARTING MPAF" << std::endl;



  // Getting Configuration File Path

  std::string cfg = "";
  char ch;

  while ((ch = getopt(argc, argv, "c:h:?")) != -1 ) {
    switch (ch) {
    case 'c': cfg = optarg; break;
    case '?':
    case 'h': printUsageAndExit(); break;
    default : printUsageAndExit();
    }
  }

  // Checking Number of Arguments
  if(argc < 2) printUsageAndExit();
  
  
  // Reading cfg file and get classes to run on
  vector<string> ans;

  MIPar inputVars = Parser::parseFile(cfg);
  for(MIPar::const_iterator it=inputVars.begin(); 
      it!=inputVars.end();it++) {

    if(it->second.type!=Parser::kAN) continue;

    ans.push_back(it->second.val);
    vector<string> opts= it->second.opts;
    for(size_t i=0;i<opts.size();i++)
      ans.push_back( opts[i] );
  }

  
  // Running the Classes
  for(unsigned int i = 0; i < ans.size(); ++i){

    MPAF* ac = AnalysisFactory::get(ans[i], cfg);
    if(ac==nullptr) {
      cout<<"Error, no analysis "<<ans[i]<<" available, please check your config file and the declaration of the analysis"<<endl;
      abort();
    }

    ac->analyze();

    delete ac;
  
  }
  
  // Closing MPAF
  std::cout << std::endl << ">> CLOSING MPAF" << std::endl;

  
  // Return
  return 0;
}

