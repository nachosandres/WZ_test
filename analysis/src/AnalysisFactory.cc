#include "analysis/src/AnalysisFactory.hh"

using namespace std;

#include "analysis/src/SUSYSSDL.hh"
#include "analysis/src/SSDLBoosted.hh"
#include "analysis/src/phys14exerc.hh"

MPAF*
AnalysisFactory::get(const string& analysis, const string& cfg) {
  
  if(analysis=="SUSYSSDL") {
    return new SUSYSSDL(cfg);
  }
  if(analysis=="SSDLBoosted") {
    return new SSDLBoosted(cfg);
  }
  if(analysis=="phys14exerc") {
    return new phys14exerc(cfg);
  }

  return 0;
}
