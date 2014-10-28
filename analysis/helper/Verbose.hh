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

#include "analysis/helper/CustomTypes.cc"
#include "analysis/helper/Tools.hh"



class Verbose{

public:

	// Member Functions

	Verbose(VerbosityLevel);
	~Verbose();
	void Initialize(VerbosityLevel);

	void SetErrorMessages();
	void SetLogFilePath(std::string);
	void SetNumberOfModules(int);
	void SetPaths(std::string, std::string);
	void SetSystemMessages();
	void SetVerbosityLevel(VerbosityLevel);

	std::string GetErrorMessageByID(int = 0);
	std::vector<std::string> GetErrorMessages();
	int GetNumberOfModulesLeft();
	std::string GetSystemMessageByID(int = 0);
	std::vector<std::string> GetSystemMessages();
	int GetVerboseAsInt();

	void Class(std::string);
	void Control();
	void Error(int = 0);
	void ErrorAndExit(int = 0);
	void ExecutionTime();
	void Function(std::string);
	void JobControl();
	void Message(int = 0);
	void Module();
	void Print(std::string, bool = false);
	void Sample(std::string);
	void Talk(std::string, int = 0, bool = false);
	void Write(std::string);
	void WriteLogFile();
	

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
