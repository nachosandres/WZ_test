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

#ifndef VERBOSE_HH
#define VERBOSE_HH

#include "TROOT.h"

#include <time.h>

#include "analysis/utils/CustomTypes.cc"
#include "analysis/utils/Tools.hh"



class Verbose{

public:

	// Member Functions

	Verbose(VerbosityLevel);
	~Verbose();
	void initialize(VerbosityLevel);

	void setErrorMessages();
	void setLogFilePath(std::string);
	void setNumberOfModules(int);
	void setPaths(std::string, std::string);
	void setSystemMessages();
	void setVerbosityLevel(VerbosityLevel);

	std::string getErrorMessageByID(int = 0);
	std::vector<std::string> getErrorMessages();
	int getNumberOfModulesLeft();
	std::string getSystemMessageByID(int = 0);
	std::vector<std::string> getSystemMessages();
	int getVerboseAsInt();

	void Class(std::string);
	void control();
	void error(int = 0);
	void errorAndExit(int = 0);
	void executionTime();
	void function(std::string);
	void jobControl();
	void message(int = 0);
	void module();
	void print(std::string, bool = false);
	void sample(std::string);
	void talk(std::string, int = 0, bool = false);
	void write(std::string);
	void writeLogFile();
	

private:
	
	bool _JobControl;
	std::string _LogFilePath;
	int _NumberOfModulesLeft;
	int _NumberOfAllModules;
	time_t _StartTime;
	VerbosityLevel _Verbose;
	std::string _WrittenOutput;

	std::vector<std::string> _ErrorMessages;
	std::vector<std::string> _SystemMessages;

	
};


#endif
