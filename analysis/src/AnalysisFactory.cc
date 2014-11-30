#include "analysis/src/AnalysisFactory.hh"

using namespace std;

#include "analysis/src/SUSYSSDL.hh"

MPAF*
AnalysisFactory::get(const string& analysis, const string& cfg) {
  
  if(analysis=="SUSYSSDL") {
    return new SUSYSSDL(cfg);
  }

  return 0;
}
