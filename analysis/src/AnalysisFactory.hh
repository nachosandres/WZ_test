#ifndef AnalysisFactory_h
#define AnalysisFactory_h

#include <string>

#include "analysis/core/MPAF.hh"

class AnalysisFactory
{
public:
  //
  // get an analysis object from name
  // 
  static MPAF* get( const std::string &, const std::string&);
 
};

#endif
