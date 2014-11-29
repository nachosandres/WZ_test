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


#include "analysis/utils/Tools.hh"


 
//____________________________________________________________________________ 
float Tools::AngleAddition(float angle1, float angle2){ 
	/* 
	computes the addition between two angles taking the pi correction into account 
	parameters: angle1, angle2 
	return: the sum of the two 
	*/ 
	
	float result = angle1 + angle2; 
	while(result >   TMath::Pi()) result -= TMath::TwoPi(); 
	while(result <= -TMath::Pi()) result += TMath::TwoPi(); 
	
	return result; 
	 
} 
 
 
//____________________________________________________________________________ 
float Tools::AngleSubtraction(float angle1, float angle2){ 
	/* 
	computes the subtraction between two angles taking the pi correction into account 
	parameters: angle1, angle2 
	return: the difference between the two 
	*/ 
	
	float result = angle1 - angle2; 
	while(result >   TMath::Pi()) result -= TMath::TwoPi(); 
	while(result <= -TMath::Pi()) result += TMath::TwoPi(); 

	return result; 

}


//____________________________________________________________________________
bool Tools::checkDirWritePermission(std::string directory){
	/*
	checks the write permission of a given directory by testing if an empty text file
	can be written to that directory; the file is deletex afterwards
	parameters: directory (path to the directory)
	return: true (if write permissions are granted), false (else)
	*/

	directory += "0.txt";	
	FILE * test_file = fopen(directory.c_str(), "w");
	if(!test_file) return false;
	
	fclose(test_file);
	std::string cmd = "rm " + directory;
	system(cmd.c_str());
	
	return true;
}


//____________________________________________________________________________
float Tools::dPhi(float phi1, float phi2){
	/*
	computes delta Phi between two given azimuthal angles
	parameters: phi1, phi2
	return: delta Phi
	*/
	
	return fabs(Tools::AngleSubtraction(phi1, phi2));

}


//__________________________________________________________________________
float Tools::dR(float eta1, float eta2, float phi1, float phi2){
	/* 
	computes delta R between two particles, whose pseudorapidity and azimuthal
	angles are given
	parameters: eta1, eta2, phi1, phi2
	return: delta R
	*/
	
	float deta = eta1 - eta2;
	float dphi = Tools::dPhi(phi1, phi2);
	
	return sqrt( deta*deta + dphi*dphi );

}


//____________________________________________________________________________
int Tools::execCmd(std::string command){
	/*
	executes a bash command that is given as a std::string
	parameters: command
	return: return of system()
	*/

	return system(command.c_str());

}


//____________________________________________________________________________
std::vector<std::string> Tools::explodeString(const std::string string, std::string delimiter){
	/*
	explodes a std::string into a vector of std::strings by breaking the string at
	every delimiter
	parameters: string (the string to explode), delimiter (the string at whose
	            occurrence the string will be broken)
	return: vector
	*/

	std::vector<std::string> vector;
	std::string element;

	for(std::string::const_iterator i = string.begin(); i != string.end(); ++i){
		element += *i;
	
		if(element.size() >= delimiter.size() && element.substr(element.size() - delimiter.size()) == delimiter){
			vector.push_back(element.substr(0, element.size() - delimiter.size()));
			element = "";
		}
	}

	vector.push_back(element);
	
	return vector;

}


//__________________________________________________________________________
TDirectory* Tools::findOrCreateTDir(TString& dir, TFile* file) {
	/*
  	looks for a directory and returns it if it exists and creates it and then
  	returns it if it does not exist
  	parameters: dir, file
  	return: the TDir
  	*/
	
	// Start from the root of the file
	file->cd();
	// Remove deadly '/'s
	while ( dir.BeginsWith("/") ) dir = dir.Strip(TString::kLeading,'/');
	dir.ReplaceAll("//","/");
	
	// Loop over sub-directories to create (ROOT's mkdir has no -p option...)
	TString cdir(dir);
	while ( cdir.First('/')>0 || cdir.Length()>0 ) {
		// Create new subdirectory
		Ssiz_t index = (cdir.First('/')>0 ? cdir.First('/') : cdir.Length());
		TString subdir = cdir(0,index);
		if ( !gDirectory->GetDirectory(subdir) ) {
			gDirectory->mkdir( subdir.Data() );
		}
		gDirectory->cd(subdir);
		cdir = cdir(index+1,cdir.Length());
	}
	return file->GetDirectory(dir);

}


//____________________________________________________________________________
std::string Tools::getTimestamp(){
	/*
  	returns the current timestamp in format yyyy-mm-dd_hh:mm:ss as std::string
  	paramters: none
  	return: timestamp
  	*/

	char buffer[80];
	time_t timestamp = time(0);
	struct tm tstruct;

	tstruct = *localtime(&timestamp);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d_%X", &tstruct);

	return buffer;

}


//____________________________________________________________________________
std::string Tools::getTimestampDir(){
	/*
  	returns the current timestamp in format yyyymmdd_hhmmss as std::string
  	paramters: none
  	return: timestamp
  	*/

	char buffer[80];
	time_t timestamp = time(0);
	struct tm tstruct;

	tstruct = *localtime(&timestamp);
	strftime(buffer, sizeof(buffer), "%Y%m%d", &tstruct);

	return buffer;

}


//____________________________________________________________________________
float Tools::interpretCut(TString cut){
	/*
  	interprets a cut value defined in the config file such that it can be used
  	as a float
  	parameters: cut
  	return: the float interpretation of the cut
  	*/

	if     (cut == "true" ) return 1;
	else if(cut == "false") return 0;
	else                    return cut.Atof();

}


//____________________________________________________________________________
std::string Tools::replace(std::string string, std::string search, std::string replace){
	/*
  	replaces every occurrence of search by replace in string
  	parameters: string, search, replace
  	return: the modified string
  	*/

	std::string new_string = string;

	for(size_t i = 0; ; i += replace.size()){

		i = new_string.find(search, i);
		if(i == std::string::npos) break;

		new_string.erase (i, search.size());
		new_string.insert(i, replace);

	}	

	return new_string;

}


//____________________________________________________________________________
float Tools::toFloat(std::string value){
	/*
  	converts a std::string to a float
  	parameters: value
  	return: value of type float
  	*/

	if     (value == "true")        
		return 1.0; 
	else if(value == "false")
		return 0.0;
	else 
		return atof(value.c_str());

}


//____________________________________________________________________________
RunMode Tools::toRunMode(std::string value){
	/*
	converts a std::string to a RunMode
	parameters: value
	return: value of type RunMode
	*/

	if     (value == "normal") return normal;
	else if(value == "test"  ) return test;
	else                       return normal;

}


//____________________________________________________________________________
RunMode Tools::toRunMode(TString value){
	/*
	converts a TString to a RunMode
	parameters: value
	return: value of type RunMode
	*/

	return Tools::toRunMode(Tools::toStdString(value));

}


//____________________________________________________________________________
SampleType Tools::toSampleType(std::string value){
	/*
	converts a std::string to a SampleType
	parameters: value
	return: value of type SampleType
	*/

	if(value == "data") return data;
	else                return mc;

}


//____________________________________________________________________________
std::string Tools::toStdString(bool value){
	/*
	converts a boolean variable into a std::string
	parameters: value
	return: "true" (if value is true), "false" (else)
	*/

	if(value == true) return "true";
	else              return "false";

}


//____________________________________________________________________________
std::string Tools::toStdString(int value){
	/*
	converts an integer to a std::string since g++ fails to compile std::to_string 
	parameters: value
	return: value of std::string type
	*/

	char c_string [10];
	snprintf(c_string, sizeof(c_string), "%d", value);
	std::string std_string = std::string(c_string);

	return std_string;	

}


//____________________________________________________________________________
std::string Tools::toStdString(TString value){
	/*
	converts a TString to a std::string
	parameters: value
	return: value of type std::string
	*/

	std::string string = value.Data();
	return string;

}


//____________________________________________________________________________
TString Tools::toTString(std::string value){
	/*
	converts a std::string to a TString
	parameters: value
	return: value of type TString
	*/

	TString new_value = value.c_str();
	return new_value;

}


//____________________________________________________________________________
VerbosityLevel Tools::toVerbosityLevel(std::string value){
	/*
	converts a std::string to a VerbosityLevel
	parameters: value
	return: value of type VerbosityLevel
	*/

	if     (value == "0") return (VerbosityLevel) 0;
	else if(value == "1") return (VerbosityLevel) 1;
	else if(value == "2") return (VerbosityLevel) 2;
	else                  return (VerbosityLevel) 2;

}








//____________________________________________________________________________
int Tools::countOccurrences(std::string haystack, std::string needle, size_t position_start, size_t position_end){
	/*
	counts the total number of occurences of a std::string needle in a std::string haystack
	parameters: haystack (the underlying string), needle (the string whose occurrence
	            in haystack needs to be counted), position_start (the char at which we start
	            to count), position_end (the char at which we stop to cound)
	return: number of occurences
	*/


	if(position_end == (size_t) -1) position_end = haystack.size();
	size_t found_at = haystack.find(needle, position_start);

	if(found_at >= 0 && found_at <= position_end)
		return 1 + Tools::countOccurrences(haystack, needle, haystack.find(needle, position_start) + 1, position_end);
	else
		return 0;

}


////____________________________________________________________________________
//std::vector<std::string> Tools::getColumnFromMatrix(std::vector<std::vector<std::string> > matrix, int column_index){
//	/*
//	takes out a single column from a matrix of std::strings
//	parameters: matrix (the matrix), column_index (the number of the column we take out)
//	return: vector that matches the column
//	*/
//
//	std::vector<std::string> vector;
//
//	for(int i = 0; i < matrix.size(); ++i)
//		vector.push_back(matrix[i][column_index]);
//
//	return vector;
//
//}
//
//
////____________________________________________________________________________
//std::vector<std::string> Tools::getRowFromMatrix(const std::vector<std::vector<std::string> > matrix, int row_index){
//	/*
//	takes out a single row from a matrix of std::strings
//	parameters: matrix (the matrix), row_index (the number of the row we take out)
//	return: vector that matches the row
//	*/
//
//	std::vector<std::string> vector;
//
//	for(int i = 0; i < matrix[row_index].size(); ++i)
//		vector.push_back(matrix[row_index][i]);
//
//	return vector;
//
//}


//____________________________________________________________________________
//std::string Tools::joinStrings(std::vector<std::string> vector, std::string delimiter){
//	/*
//	joins the std::string elements of a vector and creates a single std::string
//	parameters: vector (the vector of std::string elements), delimiter (the
//	            glueing string between the elements)
//	return: the full string
//	*/
//
//	std::string result;
//
//	for(int i = 0; i < vector.size() - 1; ++i){
//		result += vector[i];
//		result += delimiter;
//	}
//	result += vector[vector.size() - 1];
//
//	return result;
//
//}
//
//
////____________________________________________________________________________
//int Tools::scanStringFormat(std::string string, std::string format, std::vector<std::string> &elements){
//	/*
//	implements a dynamic version of sscanf(), i.e. fills a vector of std::strings of arbitrariy 
//	length according to the format
//	parameters: string (the std::string that contains the information), format (std::string that
//	            contains the format), &elements (the vector that hall be filled)
//	returns: number of elements of the vector that have been filled
//	*/
//
//
//	// we exit if the vector size and format do not fit together
//	if(Tools::countOccurrences(format, "%s") != elements.size()) return 0;
//
//
//	// we browser through the string char by char and analyse if the current
//	// delimiter has been found or not, if so, the vector is filled and the
//	// delimiter moves one forward; we exit when we have reached the end
//	std::vector<std::string> delimiters = Tools::explodeString(format, "%s");
//	int delimiter_index = (delimiters[0].size() > 0) ? 0 : 1;
//	int initial_value = delimiter_index;
//	std::string element = "";
//
//	for(Ssiz_t position = 0; position < string.size(); ++position) {
//		element += string.substr(position, 1);
//
//		// end of string reached
//		if(position + 1 == string.size()){
//			elements[delimiter_index - initial_value] = element; //.Strip(TString::kBoth, ' ');
//			return delimiter_index + 1 - initial_value;
//		}
//
//		// delimiter found, i.e. %s is complete; we fill and search for the next delimiter
//		if(delimiter_index < delimiters.size() && delimiters[delimiter_index].size() > 0 && element.substr(element.size() - delimiters[delimiter_index].size(), delimiters[delimiter_index].size()) == delimiters[delimiter_index]){
//			element = element.substr(0, element.size() - delimiters[delimiter_index].size());			
//			elements[delimiter_index - initial_value] = element; //.Strip(TString::kBoth, ' ');
//			element = "";
//			++delimiter_index;
//		}
//
//	}
//
//	return delimiter_index - initial_value;	
//
//}












////____________________________________________________________________________
//float Tools::ConvertBoolToFloatAlternatively(bool value){
//	/*
//	converts a bool to a float in an alternative way - my way
//	ohhhhhh I diiid it myyyyyyy waaaayyy!!!
//	parameters: value
//	return: value of float type
//	*/
//
//	if(value == true) return 1.0;
//	else              return 0.0;
//
//}
//
//
//
////____________________________________________________________________________
//bool Tools::ConvertFloatToBoolAlternatively(float value){
//	/*
//	converts a float to a bool in an alternative way - my way
//	ohhhhhh I diiid it myyyyyyy waaaayyy!!!
//	parameters: value
//	return: value of bool type
//	*/
//
//	if(value == 0.0) return false;
//	else             return true;
//
//}
//
//
//
////____________________________________________________________________________
//std::string Tools::ConvertOutputTypeToStdString(OutputType value){
//	/*
//	converts an OutputType to a std::string
//	parameters: value
//	return: value of OutputType type
//	*/
//
//	if     (value == histogram) return "histogram";
//	else if(value == list     ) return "list";
//	else if(value == text     ) return "text";
//	else if(value == tree     ) return "tree";
//
//	return "";
//
//}
//
//
////____________________________________________________________________________
//const char* Tools::ConvertStdStringToCString(std::string value){
//	/*
//	converts a std::string to a char*
//	parameters: value
//	return: value of type char*
//	*/
//
//	return value.c_str();
//
//}
//
//
////____________________________________________________________________________
//Label Tools::ConvertStdStringToLabel(std::string value){
//	/*
//	converts a std::string to Label
//	parameters: value
//	return: value of type Label
//	*/
//
//	Label new_value = value.c_str();
//	return new_value;
//
//}
//
//
////____________________________________________________________________________
//TString Tools::ConvertStdStringToTString(std::string value){
//	/*
//	converts a std::string to a TString
//	parameters: value
//	return: value of type TString
//	*/
//
//	TString new_value = value.c_str();
//	return new_value;
//
//}
//
//
//
////____________________________________________________________________________
//OutputType Tools::ConvertTStringToOutputType(TString value){
//	/*
//	converts a TString to a OutputType
//	parameters: value
//	return: value of type OutputType
//	*/
//
//	if     (value == "histogram") return histogram;
//	else if(value == "list"     ) return list;
//	else if(value == "text"     ) return text;
//	else if(value == "tree"     ) return tree;
//	else                          return text;
//
//}
//
//
////____________________________________________________________________________
//SampleType Tools::ConvertTStringToSampleType(TString value){
//	/*
//	converts a TString to a SampleType
//	parameters: value
//	return: value of type SampleType
//	*/
//
//	if(value == "data") return data;
//	else                return mc;
//
//}
//
//
////____________________________________________________________________________
//std::string Tools::ConvertTStringToStdString(TString value){
//	/*
//	converts a TString to a std::string
//	parameters: value (TString)
//	return: std::string version of it
//	*/
//
//	std::string new_value = value.Data();
//	return new_value;
//}
//
//
////____________________________________________________________________________
//std::vector<int> Tools::ConvertTStringVectorToIntVector(std::vector<TString> vector){
//	/*
//	converts a vector of TString into a vector of integers
//	parameters: vector (vector of TStrings)
//	return: new_vector (vector of ints)
//	*/
//
//	std::vector<int> new_vector;
//
//	for(int i = 0; i < vector.size(); ++i)
//		new_vector.push_back(vector[i].Atoi());
//
//	return new_vector;
//
//}
//
//
////____________________________________________________________________________
//int Tools::CountTStringOccurrence(TString haystack, TString needle, Ssiz_t position_start, Ssiz_t position_end){
//	/*
//	counts the total number of occurences of a TString needle in a TString haystack
//	parameters: haystack (the underlying string), needle (the string whose occurrence
//	            in haystack needs to be counted), position_start (the char at which we start
//	            to count), position_end (the char at which we stop to cound)
//	return: number of occurences
//	*/
//
//
//	if(position_end == -1) position_end = haystack.Length();
//	Ssiz_t found_at = haystack.Index(needle, position_start);
//
//	if(found_at >= 0 && found_at <= position_end)
//		return 1 + Tools::CountTStringOccurrence(haystack, needle, haystack.Index(needle, position_start) + 1, position_end);
//	else
//		return 0;
//
//}
//
//
////____________________________________________________________________________
//TString Tools::ExecuteShellScript(TString file_path){
//    /*
//    executes a given shell script and returns the piped output
//    parameters: file_path (path to the shell script)
//    return: bash return
//    */
//
//    TString result;
//    char buffer[200];
//    FILE * piped_return;
//
//    piped_return = popen(file_path, "r");
//    if(!piped_return) return "false";
//
//    while(!feof(piped_return)){
//        if(fgets(buffer, 200, piped_return) != NULL)
//            result += buffer;
//    }
//
//    pclose(piped_return);
//    return result;
//
//}
//
//
//
//
////____________________________________________________________________________
//std::vector<TString> Tools::GetColumnFromTStringMatrix(std::vector<std::vector<TString> > matrix, int column_index){
//	/*
//	takes out a single column from a matrix of TStrings
//	parameters: matrix (the matrix), column_index (the number of the column we take out)
//	return: vector that matches the column
//	*/
//
//	std::vector<TString> vector;
//
//	for(int i = 0; i < matrix.size(); ++i)
//		vector.push_back(matrix[i][column_index]);
//
//	return vector;
//
//}
//
//
////____________________________________________________________________________
//std::vector<TString> Tools::GetRowFromTStringMatrix(const std::vector<std::vector<TString> > matrix, int row_index){
//	/*
//	takes out a single row from a matrix of TStrings
//	parameters: matrix (the matrix), row_index (the number of the row we take out)
//	return: vector that matches the row
//	*/
//
//	std::vector<TString> vector;
//
//	for(int i = 0; i < matrix[row_index].size(); ++i)
//		vector.push_back(matrix[row_index][i]);
//
//	return vector;
//
//}
//
//
////____________________________________________________________________________
//TString Tools::GetUserName(){
//	/*
//	returns the user name which is read out of the absolute path of the 
//	executable by assuming that the system runs on a Tier-3 environment, i.e. in
//	a directory structure that starts with /shome/...
//	parameters: none
//	return: user name 
//	*/
//	
//	char path[512];
//	TString result = "";
//	std::vector<TString> exploded;
//
//	ssize_t length = readlink("/proc/self/exe", path, 512);
//
//	if(length > -1){
//		exploded = Tools::ExplodeTString(Tools::ConvertStdStringToTString(path), "/");
//		result = exploded[2];
//	}
//
//	return result;
//
//}
//
//
////____________________________________________________________________________
//std::string Tools::JoinStdString(std::vector<std::string> vector, std::string delimiter){
//	/*
//	joins the std::string elements of a vector and creates a single std::string
//	parameters: vector (the vector of std::string elements), delimiter (the
//	            glueing string between the elements)
//	return: the full string
//	*/
//
//	std::string result;
//
//	for(int i = 0; i < vector.size()-1; ++i){
//		result += vector[0];
//		result += delimiter;
//	}
//	result += vector[vector.size()-1];
//
//	return result;
//
//}
//
//
////____________________________________________________________________________
//TString Tools::JoinTString(std::vector<TString> vector, TString delimiter){
//	/*
//	joins the TString elements of a vector and creates a single TString
//	parameters: vector (the vector of TString elements), delimiter (the
//	            glueing string between the elements)
//	return: the full string
//	*/
//
//	TString result;
//
//	for(int i = 0; i < vector.size()-1; ++i){
//		result += vector[0];
//		result += delimiter;
//	}
//	result += vector[vector.size()-1];
//
//	return result;
//
//}
//
//
////____________________________________________________________________________
//void Tools::ReplaceAll(std::string& string, const std::string look_for, const std::string replace_by){
//	/*
//	replaces all occurrences of find in string by replace
//	parameters: string (the string to be edited), find (the substring to be
//	            searched), replace (the substring to replace find)
//	return: none
//	*/
//
//	size_t position = 0;
//
//	while((position = string.find(look_for, position)) != std::string::npos){
//		string.replace(position, look_for.length(), replace_by);
//		position += replace_by.length();
//	}
//
//}
//
//
////____________________________________________________________________________
//int Tools::ScanTStringFormat(TString string, TString format, std::vector<TString> &elements){
//	/*
//	implements a dynamic version of sscanf(), i.e. fills a vector of TStrings of arbitrariy 
//	length according to the format
//	parameters: string (the TString that contains the information), format (TString that
//	            contains the format), &elements (the vector that hall be filled)
//	returns: number of elements of the vector that have been filled
//	*/
//
//
//	// we exit if the vector size and format do not fit together
//	if(Tools::CountTStringOccurrence(format, "%s") != elements.size()) return 0;
//
//
//	// we browser through the string char by char and analyse if the current
//	// delimiter has been found or not, if so, the vector is filled and the
//	// delimiter moves one forward; we exit when we have reached the end
//	std::vector<TString> delimiters = Tools::ExplodeTString(format, "%s");
//	int delimiter_index = (delimiters[0].Length() > 0) ? 0 : 1;
//	int initial_value = delimiter_index;
//	TString element = "";
//
//	for(Ssiz_t position = 0; position <= string.Length(); ++position) {
//		element += string(position, 1);
//
//		// end of string reached
//		if(position == string.Length()){
//			elements[delimiter_index - initial_value] = element.Strip(TString::kBoth, ' ');
//			return delimiter_index + 1 - initial_value;
//		}
//
//		// delimiter found, i.e. %s is complete; we fill and search for the next delimiter
//		if(delimiter_index < delimiters.size() && delimiters[delimiter_index].Length() > 0 && element(element.Length() - delimiters[delimiter_index].Length(), delimiters[delimiter_index].Length()) == delimiters[delimiter_index]){
//			element = element(0, element.Length() - delimiters[delimiter_index].Length());			
//			elements[delimiter_index - initial_value] = element.Strip(TString::kBoth, ' ');
//			element = "";
//			++delimiter_index;
//		}
//
//	}
//
//	return delimiter_index - initial_value;	
//
//}




