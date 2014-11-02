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


#include <TROOT.h>
#include <TString.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "analysis/general/FakeRatio.hh"
#include "analysis/analyzers/SUSYSSDL.hh"



//_____________________________________________________________________________________
void printUsageAndExit(){


	std::cout << "printing usage" << std::endl;
	exit(1);

}


//_____________________________________________________________________________________
std::vector<std::string> getRunOn(std::string config_file){
	/*
	opens and reads the configuration file, analyzes the file content for the variable
	runon and returns its value
	parameters: config_file (the file path of the config file)
	return: RunOn
	*/

	std::vector<std::string> result;
	char buffer[300];
	char symbol_char[1], variable_char[100], value_char[200];
	
	ifstream IN(config_file.c_str());
	if(!IN.is_open()) 
		result.push_back("nothing");
	
	while(IN.getline(buffer, 1000, '\n')){
	
		if(strlen(buffer) == 0) continue;
		if(buffer[0] == '#') continue;	
		if(buffer[0] == ' ') continue;
	
		if(sscanf(buffer, "%s\t%s\t%s", symbol_char, variable_char, value_char) < 3) continue;

		std::string symbol   = symbol_char;
		std::string variable = variable_char;
		std::string value    = value_char;
				
		if(symbol == "n" && variable == "RunOn") 
			result = Tools::explodeString(value, ",");
	}

	return result;

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

	std::string config_file = "";
	char ch;

	while ((ch = getopt(argc, argv, "c:h:?")) != -1 ) {
		switch (ch) {
			case 'c': config_file = optarg; break;
			case '?':
			case 'h': printUsageAndExit(); break;
			default : printUsageAndExit();
		}
	}



	// Checking Number of Arguments

	if(argc < 2) printUsageAndExit();



	// Reading RunOn, i.e. the list of classes to run on

	std::vector<std::string> run_on = getRunOn(config_file);


	
	// Running the Classes
	
	for(unsigned int i = 0; i < run_on.size(); ++i){

		if(run_on[i] == "FakeRatio"){
			FakeRatio* General = new FakeRatio(config_file);
			General -> analyze();
			delete General;
		}
		
		else if(run_on[i] == "SUSYSSDL"){
		        SUSYSSDL* Analyzer = new SUSYSSDL(config_file);
			Analyzer -> analyze();
			delete Analyzer;
		}



	}



	// Closing MPAF

	std::cout << std::endl << ">> CLOSING MPAF" << std::endl;



	// Return

	return 0;

}

