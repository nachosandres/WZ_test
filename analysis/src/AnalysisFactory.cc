#include "analysis/src/AnalysisFactory.hh"

using namespace std;

// list of analyzers and modules in alphabetical order 
#include "analysis/src/SUSYSSDL.hh"
#include "analysis/src/synchECO.hh"
#include "analysis/src/synchRA5.hh"
#include "analysis/src/phys14exerc.hh"
#include "analysis/src/SUSY3L.hh"

MPAF*
AnalysisFactory::get(const string& analysis, const string& cfg) {
   
MPAF*
AnalysisFactory::get(const string& analysis, const string& cfg) {
 
// list of analyzers and modules in alphabetical order 
  
  if(analysis=="SUSYSSDL") {
    return new SUSYSSDL(cfg);
  }
  if(analysis=="synchECO") {
    return new synchECO(cfg);
  }
  if(analysis=="synchRA5") {
    return new synchRA5(cfg);
  }
  if(analysis=="phys14exerc") {
    return new phys14exerc(cfg);
  }

  if(analysis=="SUSY3L") {
    return new SUSY3L(cfg);
  }

  return 0;
}
