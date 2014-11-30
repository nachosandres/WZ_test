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


#include "analysis/utils/Verbose.hh"




/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR RUNNING THE CODE                                       **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
Verbose::Verbose(VerbosityLevel verbose){
    /*
    constructs the Verbose Class
    parameters: verbose (verbosity level)
    return: none
    */

	
	_StartTime = time(0);
	_WrittenOutput = "STARTING DIlEPTONS\n";

	initialize(verbose);

}


//____________________________________________________________________________
Verbose::~Verbose(){
    /*
    destructs the Verbose Class
    parameters: none
    return: none
    */

}


//____________________________________________________________________________
void Verbose::initialize(VerbosityLevel verbose){ 
	/*
	initializes the Verbose class
    parameters: verbose (verbose level)
	return: none
	*/

	setVerbosityLevel(verbose);
	setErrorMessages();
	setSystemMessages();
	talk(Tools::replace(getSystemMessageByID(0), "__TIMESTAMP__", Tools::getTimestamp()), 1);

}





/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR SETTING PARAMETERS                                     **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void Verbose::setErrorMessages(){
	/*
	sets the vector of error messages by reading them from the corresponding info file
	parameters: none
	return: none
	*/
	
	// CH: this wants to be changed as soon as we agree on how to store error and system messages
	
	//// Since we want to use predefined error messages, we need to make sure the error messages file exists.
	//if(access(error_file_path.c_str(), F_OK && R_OK) != 0){
	//	std::cerr << "The info file containing error messages cannot be found or reading is not permitted." << std::endl;
	//	std::cerr << "Please provide this info file with correct name, location and permission in order to run MPAF." << std::endl;
	//	std::cerr << "Execution terminated!" << std::endl;
	//	exit(1);
	//}

	// CH: this is hard coded now as it is not clear yet, how we do want to handle the verbosity
	// in MPAF; keep the verbosity class? where to store error and system messages?

	_ErrorMessages.push_back("Unexpected error occurred!");
	_ErrorMessages.push_back("Not all resources are provided to the framework. Some could not be found or are faulty. Please check for completeness: info files, config file, input directory (and read permission), output directory (and write permission), afs workspace directory (and write permission). Exiting MPAF.");
	_ErrorMessages.push_back("The given configuration file could not be opened and read. Exiting MPAF.");
	_ErrorMessages.push_back("The user name is missing in the configuration file. Exiting MPAF.");
	_ErrorMessages.push_back("A conflict was detected in the directories that are given in the configuration file. One or more of them do not exist. Exiting MPAF.");
	_ErrorMessages.push_back("The given module list ID is ilegal. Exiting MPAF.");
	_ErrorMessages.push_back("A conflict was detected in the module or sketch IDs that have been provided either in the configuration file or in the definition of the module list. The list may only contain modules or sketches at a time, but not representatives of both. Exiting MPAF.");
	_ErrorMessages.push_back("One or more of the provided data samples could not be found or opened. Please check the file paths given in the configuration file with label 's'. Exiting MPAF."); 

}


//____________________________________________________________________________
void Verbose::setLogFilePath(std::string new_value){
	/*
	sets the file path of the log file
	parameters: new_value
	return: none
	*/

	_LogFilePath = new_value;

}


//____________________________________________________________________________
void Verbose::setNumberOfModules(int number_of_modules_left){
	/*
	sets the number of modules left
	parameters: number_of_modules_left
	return: none
	*/

	_NumberOfModulesLeft = number_of_modules_left;
	_NumberOfAllModules  = number_of_modules_left;

}


//____________________________________________________________________________
void Verbose::setSystemMessages(){
	/*
	sets the vector of system messages by reading them from the corresponding info file
	parameters: none
	return: none
	*/

	// CH: this wants to be changed as soon as we agree on how to store error and system messages

	_SystemMessages.push_back("Execution of Dileptons initialized at __TIMESTAMP__");
	_SystemMessages.push_back("Loading class __CLASS_NAME__");
	_SystemMessages.push_back("Executing function __FUNCTION_NAME__");
	_SystemMessages.push_back("__NUMBER_OF_MODULES_LEFT__ of __NUMBER_OF_MODULES__ modules left to process");
	_SystemMessages.push_back("Processing data sample __SAMPLE_NAME__");
	_SystemMessages.push_back("Execution finished at __TIMESTAMP__ with running time __EXECUTION_TIME__");

}


//____________________________________________________________________________
void Verbose::setVerbosityLevel(VerbosityLevel verbose){
	/*
	sets the verbose of the class to given value
	parameters: verbose (level to be set)
	return: none
	*/

	_Verbose = verbose;

}





/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR GETTING PARAMETERS                                     **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
std::string Verbose::getErrorMessageByID(int error_id){
	/*
	returns the message at a given error ID
	parameters: error_id
	return: message at that ID
	*/

	return _ErrorMessages[error_id];

}


//____________________________________________________________________________
std::vector<std::string> Verbose::getErrorMessages(){
	/*
	returns the vector of all error messages
	parameters: none
	return: kErrorMessages
	*/

	return _ErrorMessages;

}


//____________________________________________________________________________
int Verbose::getNumberOfModulesLeft(){
	/*
	returns the number of modules left
	parameters: none
	return: _NumberOfModulesLeft
	*/

	return _NumberOfModulesLeft;

}

//____________________________________________________________________________
std::string Verbose::getSystemMessageByID(int message_id){
	/*
	returns the message at a given message ID
	parameters: message_id
	return: message at that ID
	*/

	return _SystemMessages[message_id];

}


//____________________________________________________________________________
std::vector<std::string> Verbose::getSystemMessages(){
	/*
	returns the vector of all system messages
	parameters: none
	return: kSystemMessages
	*/

	return _SystemMessages;

}


//____________________________________________________________________________
int Verbose::getVerboseAsInt(){
	/*
	returns the verbose of the class as int
	parameters: none
	return: kVerbose as int
	*/

	return (int) _Verbose;

}






/*****************************************************************************
******************************************************************************
** CLASS MEMBERS FOR USING THE VERBOSE                                      **
******************************************************************************
*****************************************************************************/


//____________________________________________________________________________
void Verbose::Class(std::string class_name){
	/*
  	talks about initializing a new class
  	parameters: class_name
  	return: none
  	*/

	talk(Tools::replace(getSystemMessageByID(1), "__CLASS_NAME__", class_name), 2);

}


//____________________________________________________________________________
void Verbose::error(int error_id){
	/*
  	talks about an error that occurred
  	parameters: error_id
  	return: none
  	*/

	talk(getErrorMessageByID(error_id), 0, true);

}


//____________________________________________________________________________
void Verbose::errorAndExit(int error_id){
	/*
  	talks about a severe error that occurred and exists the execution
  	parametres: error_id
  	return: none
  	*/

	error(error_id);
	exit(1);

}


//____________________________________________________________________________
void Verbose::executionTime(){
	/*
  	talks about the execution time, i.e. the time difference between kStartTime
  	(which is set when this class is initialized, which happens when MPAF
  	is initialized) and now
  	parameters: none
  	return: none
  	*/

	time_t now = time(0);
	double seconds = difftime(now, _StartTime);
	int hours = (int) seconds / 3600;
	int rest_minutes = (int) seconds / 60 - hours * 60;
	int rest_seconds = (int) seconds - hours * 3600 - rest_minutes * 60;

	std::string execution_time = Tools::toStdString(hours) + "h " + Tools::toStdString(rest_minutes) + "m " + Tools::toStdString(rest_seconds) + "s"; 

	talk(Tools::replace(Tools::replace(getSystemMessageByID(5), "__TIMESTAMP__", Tools::getTimestamp()), "__EXECUTION_TIME__", execution_time), 1);

}


//____________________________________________________________________________
void Verbose::function(std::string function_name){
	/*
  	talks about a (member) function being called
  	parameters: function_name
  	return: none
  	*/

	talk(Tools::replace(getSystemMessageByID(2), "__FUNCTION_NAME__", function_name), 2);

}


//____________________________________________________________________________
void Verbose::jobControl(){
	/*
  	this will be job control at some point
  	*/


}


//____________________________________________________________________________
void Verbose::message(int message_id){
	/*
  	talks about system stuff, regular system messages that are not errors
  	parameters: message_id
  	return: none
  	*/

	talk(getSystemMessageByID(message_id), 1);

}


//____________________________________________________________________________
void Verbose::module(){
	/*
  	talks about the number of modules left to process, since this method is called
  	immediately before the calling of another module, we decrease the number of
  	modules left by 1 right afterwards
  	parameters: none
  	return: none
  	*/

	talk(Tools::replace(Tools::replace(getSystemMessageByID(3), "__NUMBER_OF_MODULES_LEFT__", Tools::toStdString(_NumberOfModulesLeft)), "__NUMBER_OF_MODULES__", Tools::toStdString(_NumberOfAllModules)), 1);
	--_NumberOfModulesLeft;

}


//____________________________________________________________________________
void Verbose::print(std::string message, bool print_error){
	/*
  	do the printing, i.e. printing out a message in bash
  	parameters: message, print_error (true if we are printing an error, false otherwise)
  	return: none
  	*/

	if(print_error) std::cerr << ">> " << message << std::endl;
	else            std::cout << ">> " << message << std::endl;

}


//____________________________________________________________________________
void Verbose::sample(std::string sample_name){
	/*
  	talks about a samples being processed
  	parameters: sample_name
  	return: none
  	*/

	talk(Tools::replace(getSystemMessageByID(4), "__SAMPLE_NAME__", sample_name), 1);

}

//____________________________________________________________________________
void Verbose::talk(std::string message, int required_verbose, bool talk_error){
	/*
  	does the talking, i.e. prints and writes a message
  	parameters: message, required_verbose (minimum verbosity level required for printing
  	            the message in the prompt as integer), talk_error (true if the message
  	            is an error message, false otherwise)
  	return: none
  	*/

	if(_Verbose == (VerbosityLevel) required_verbose) 
		print(message, talk_error);

	write(message);

}


//____________________________________________________________________________
void Verbose::write(std::string message){
	/*
  	writes a message to the log file cache
  	parameters: message
  	return: none
  	*/

	_WrittenOutput += message;
	_WrittenOutput += "\n";

}


//____________________________________________________________________________
void Verbose::writeLogFile(){
	/*
  	writes the log file cache to the log file
  	parameters: none
  	return: none
  	*/

	std::ofstream log_file;
	log_file.open(_LogFilePath.c_str());
	log_file << _WrittenOutput;
	log_file << "CLOSING DILEPTONS\n";
	log_file.close();

}




