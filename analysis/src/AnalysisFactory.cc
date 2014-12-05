#include "analysis/src/AnalysisFactory.hh"

using namespace std;

#include "analysis/src/SUSYSSDL.hh"
#include "analysis/src/SUSY3L.hh"

MPAF*
AnalysisFactory::get(const string& analysis, const string& cfg) {
   
  if(analysis=="SUSYSSDL") {
    return new SUSYSSDL(cfg);
  }

  if(analysis=="SUSY3L") {
    return new SUSY3L(cfg);
  }

  return 0;
}
