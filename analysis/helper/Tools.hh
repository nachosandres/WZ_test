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


#ifndef TOOLS_HH
#define TOOLS_HH

#include <TROOT.h>
#include <TFile.h>
#include <TString.h>
#include <TMath.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>
#include <time.h>
#include <sstream>

#include "analysis/helper/CustomTypes.cc"



namespace Tools {

	// Non-template members

	float AngleAddition(float, float);
	float AngleSubtraction(float, float);
	bool checkDirWritePermission(std::string);
	float dPhi(float, float);
	float dR(float, float, float, float);
	int execCmd(std::string);
	std::vector<std::string> explodeString(std::string, std::string);
	TDirectory * findOrCreateTDir(TString&, TFile*);
	std::string getTimestamp();
	std::string getTimestampDir();
	float interpretCut(TString);
	std::string replace(std::string, std::string, std::string);
	float toFloat(std::string);
	RunMode toRunMode(std::string);
	RunMode toRunMode(TString);
	SampleType toSampleType(std::string);
	std::string toStdString(bool);
	std::string toStdString(int);
	std::string toStdString(TString);
	TString toTString(std::string);
	VerbosityLevel toVerbosityLevel(std::string);



	int countOccurrences(std::string, std::string, size_t = 0, size_t = -1);
	std::string joinStrings(std::vector<std::string>, std::string);
	std::vector<std::string> getColumnFromMatrix(std::vector<std::vector<std::string> >, int = 0);
	std::vector<std::string> getRowFromMatrix(std::vector<std::vector<std::string> >, int = 0);
	int scanStringFormat(std::string, std::string, std::vector<std::string>&);






	float ConvertBoolToFloatAlternatively(bool);
	std::string ConvertBoolToStdString(bool);
	bool ConvertFloatToBoolAlternatively(float);
	std::string ConvertIntToStdString(int);
	std::string ConvertOutputTypeToStdString(OutputType);
	const char* ConvertStdStringToCString(std::string);
	Label ConvertStdStringToLabel(std::string);
	TString ConvertStdStringToTString(std::string);
	const char* ConvertTStringToCString(TString);
	float ConvertTStringToFloat(TString);
	OutputType ConvertTStringToOutputType(TString);
	SampleType ConvertTStringToSampleType(TString);
	std::string ConvertTStringToStdString(TString);
	std::vector<int> ConvertTStringVectorToIntVector(std::vector<TString>);
	TString ExecuteShellScript(TString);
	std::vector<TString> GetColumnFromTStringMatrix(std::vector<std::vector<TString> >, int = 0);
	TString GetUserName();
	void ReplaceAll(std::string&, const std::string, const std::string);


	// Template members

	//____________________________________________________________________________
	template<typename KeyType, typename ValueType> bool findInMapByKey(std::map<KeyType, ValueType> map, KeyType element){
		/*
  		searches through a map looking for a key, returns true if the key is defined, else false
  		parameters: map (the map), element (the key we are looking for)
  		returns: true (if key is in map), false (else)
  		*/

		if(map.find(element) != map.end()) return true;
	
		return false;

	}


	//____________________________________________________________________________
	template<typename VectorType> bool findInVector(std::vector<VectorType> vector, VectorType element){
		/*
		searches through the elements of a vector and returns true if it finds a given one
		parameters: vector (the vector), element (the element we are looking for)
		return: true (if element is in vector), false (else)
		*/
	
		if(std::find(vector.begin(), vector.end(), element) != vector.end()) return true;
		
		return false;
	
	}

	//____________________________________________________________________________
	template<typename ValueType, size_t n> std::vector<ValueType> toVector(const ValueType (&array)[n]){
		/*
		converts an array to a vector, or in other words, initializes a vector with
		an array
		parameters: array
		return: vector
		*/
	
		return std::vector<ValueType>(array, array + n);
	
	}








	//____________________________________________________________________________
	template<typename ValueType> int GetElementIndexInVector(std::vector<ValueType> vector, ValueType element){
		/*
		returns the index of an element in a vector, if it is part of the vector, 
		otherwise it returns the length of the vector
		parameters: vector (the vector), element (the element we are looking for)	
		return: position of element in vector
		*/

		return std::distance(vector.begin(), std::find(vector.begin(), vector.end(), element));

	}


	//____________________________________________________________________________
	template<typename ValueType> int GetElementIndexByObjectName(std::vector<ValueType> vector, TString name){
		/*
		searches through a vector of more complex elements, i.e. elements that may be
		of custom type which has a method called "GetName()", and returns the index
		of the element whose name matches a given one
		parameters: vector (the vector), name (the name)
		return: index of the element in vector, whose name matches name
		*/
		
		for(int i = 0; i < vector.size(); ++i)
			if(vector[i] -> GetName() = name)
				return i;
		
		return -1;
	
	}


	//____________________________________________________________________________
	template<typename ValueType> Label GetElementKeyByObjectName(const std::map<Label, ValueType> map, TString name){
		/*
		searches through a map of more complex elements, i.e. elements that may be
		of custom type which has a method called "GetName()", and returns the key
		of the element whose name matches a given one
		parameters: map (the map), name (the name)
		return: key of the element in map, whose name matches name
		*/
		
		typename std::map<Label, ValueType>::const_iterator i;
		
		for(i = map.begin(); i != map.end(); ++i)
			if(i -> second -> GetName() == name)
				return i->first;
		
		return "";
	
	}

	//____________________________________________________________________________
	// template<typename ValueType> std::map<Label, ValueType> GetSubSetOfMapByKeys(std::map<Label, ValueType> map, Label keys_beginning){
	// 	/*
	// 	selects a subset of a map according to a list of given keys
	// 	parameters: map (the parent map), keys (the vector of keys)
	// 	return: subset of map containing only elements whose keys are in keys
	// 	*/
	
	// 	std::map<Label, ValueType> result;
	// 	typename std::map<Label, ValueType>::iterator i;
		
	// 	for(i = map.begin(); i != map.end(); ++i)
	// 	  if(i -> first(0, (Tstring)(keys_beginning).Length()) == keys_beginning)
	// 			result[i -> first] = map[i -> first];
	
	// 	return result;
	
	// }
	
	
	//___________________________________________________________ _________________
	template<typename ValueType> std::map<Label, ValueType> GetSubSetOfMapByKeys(std::map<Label, ValueType> map, std::vector<Label> keys){
		/*
		selects a subset of a map according to a list of given keys
		parameters: map (the parent map), keys (the vector of keys)
		return: subset of map containing only elements whose keys are in keys
		*/
		
		std::map<Label, ValueType> result;
		typename std::map<Label, ValueType>::iterator i;
		
		for(i = map.begin(); i != map.end(); ++i)
			if(std::find(keys.begin(), keys.end(), i->first) != keys.end())
				result[i -> first] = map[i -> first];
		
		return result;
	
	}


	//____________________________________________________________________________
	template<typename ValueType> std::map<Label, ValueType> GetSubSetOfMapByObjectNames(std::map<Label, ValueType> map, TString names_beginning){
		/*
		selects a subset of a map according to a list of given keys
		parameters: map (the parent map), keys (the vector of keys)
		return: subset of map containing only elements whose keys are in keys
		*/
		
		std::map<Label, ValueType> result;
		typename std::map<Label, ValueType>::iterator i;
		
		for(i = map.begin(); i != map.end(); ++i)
			if(i -> second -> GetName()(0, names_beginning.Length()) == names_beginning)
				result[i -> first] = map[i -> first];
		
		return result;
	
	}
	
	
	//____________________________________________________________________________
	template<typename ValueType> std::map<Label, ValueType> GetSubSetOfMapByObjectNames(std::map<Label, ValueType> map, std::vector<TString> names){
		/*
		selects a subset of a map according to a list of given keys
		parameters: map (the parent map), keys (the vector of keys)
		return: subset of map containing only elements whose keys are in keys
		*/
		
		std::map<Label, ValueType> result;
		typename std::map<Label, ValueType>::iterator i;
		
		for(i = map.begin(); i != map.end(); ++i)
			if(std::find(names.begin(), names.end(), (i -> second) -> GetName()) != names.end())
				result[i -> first] = map[i -> first];

		return result;
	
	}


	//____________________________________________________________________________
	template<typename ValueType> std::vector<ValueType> GetSubSetOfVectorByObjectNames(const std::vector<ValueType> vector, TString names_beginning){
		/*
		selects a subset of a map according to a list of given keys
		parameters: map (the parent map), keys (the vector of keys)
		return: subset of map containing only elements whose keys are in keys
		*/
		
		std::vector<ValueType> result;
		
		for(int i = 0; i < vector.size(); ++i)
			if(vector[i] -> GetName()(0, names_beginning.Length()) == names_beginning)
				result.push_back(vector[i]);
		
		return result;
	
	}
	
	
	//____________________________________________________________________________
	template<typename ValueType> std::vector<ValueType> GetSubSetOfVectorByObjectNames(const std::vector<ValueType> vector, std::vector<TString> names){
		/*
		selects a subset of a map according to a list of given keys
		parameters: map (the parent map), keys (the vector of keys)
		return: subset of map containing only elements whose keys are in keys
		*/
		
		std::vector<ValueType> result;
		
		for(int i = 0; i < vector.size(); ++i)
			if(std::find(names.begin(), names.end(), vector[i] -> GetName()) != names.end())
				result.push_back(vector[i]);
		
		return result;
	
	}


	//____________________________________________________________________________
	template<typename KeyType, typename ValueType> std::vector<KeyType> GetVectorFromMapKeys(const std::map<KeyType, ValueType> map){
		/*
		takes out all keys froma  map discarding all values, i.e. we end up with a vector
		parameters: map (the map)
		return: vector of only keys
		*/
		
		std::vector<KeyType> vector;
		typename std::map<KeyType, ValueType>::const_iterator i;
		
		for(i = map.begin(); i != map.end(); ++i)
			vector.push_back(i->first);
		
		return vector;
	}
	
	
	//____________________________________________________________________________
	template<typename KeyType, typename ValueType> std::vector<ValueType> GetVectorFromMapValues(const std::map<KeyType, ValueType> map){
		/*
		takes out all values from a map discarding all keys, i.e. we end up with a vector
		parameters: map (the map)
		return: vector of only values
		*/
		
		std::vector<ValueType> vector;
		typename std::map<KeyType, ValueType>::const_iterator i;
		
		for(i = map.begin(); i != map.end(); ++i)
			vector.push_back(i->second);
		
		return vector;
		
	}


	//__________________________________________________________________________
	template<typename ValueType> ValueType Maximum(std::vector<ValueType> vector){
		/*
		returns the maximum element of a vector of floats, if size larger than 0,
		otherwise it returns 0.0
		parameters: vector
		return: maximum element
		*/
		
		if(vector.size() == 0) return 0.;
		
		return *std::max_element(vector.begin(), vector.end());
	
	}
	
	
	//__________________________________________________________________________
	template<typename ValueType> ValueType Minimum(std::vector<ValueType> vector){
		/*
		returns the minimum element of a vector of floats, if size larger than 0,
		otherwise it returns 0.0
		parameters: vector
		return: minimum element
		*/
		
		if(vector.size() == 0) return 0.;
		
		return *std::min_element(vector.begin(), vector.end());
	
	}


	//__________________________________________________________________________
	template<typename ValueType> ValueType Sum(std::vector<ValueType> summands){
		/*
		computes the sum over the elements in a vector
		parameters: summands
		return: sum
		*/
		
		float sum = 0.;
		for(int i = 0; i < summands.size(); ++i)
		    sum += summands[i];
		
		return sum;
	
	}


	//____________________________________________________________________________
	template<typename KeyType, typename ValueType> TString PrintContentsOfMap(const std::map<KeyType, ValueType> map, TString delimiter = "\n"){
		/*
  		prints the content (i.e. the key and the value) of a map in a specific
  		way and returns this content as a TString; kind of a dumper for the map
  		parameters: map (the map), delimiter (the delimiter between the elements of the map)
  		return: string containing the content
  		*/

		TString content = "";
		typename std::map<KeyType, ValueType>::const_iterator i;
		
		for(i = map.begin(); i != map.end(); ++i){
			std::ostringstream stream;
			stream << i->first << ": " << i->second << delimiter;
			content += Tools::ConvertStdStringToTString(stream.str());
		}
		
		return content;

	}


	//____________________________________________________________________________
	template<typename ValueType> std::vector<ValueType> RemoveElementFromVector(std::vector<ValueType> vector, ValueType element){
		/*
		remove every element in a vector that matches a given value
		parameters: vector (the vector), element (the given value)
		return: new_vector without element
		*/
		
		std::vector<ValueType> new_vector;

		for(int i = 0; i < vector.size(); ++i)
			if(vector[i] != element) 
				new_vector.push_back(vector[i]);
		
		return new_vector;
	
	}


}

#endif
