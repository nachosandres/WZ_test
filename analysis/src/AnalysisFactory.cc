
#include "analysis/src/AnalysisFactory.hh"

using namespace std;

//list of analyzers and modules in alphabetical order
#include "analysis/src/csa14exerc.hh"
#include "analysis/src/SSDLBoosted.hh"
#include "analysis/src/synchECO.hh"
#include "analysis/src/synchRA5.hh"
#include "analysis/src/phys14exerc.hh"
#include "analysis/src/SUSY3L.hh"
#include "analysis/src/SUSY3L_sync.hh"
#include "analysis/src/SUSY3L_sync2.hh"
#include "analysis/src/phys14limits.hh"
#include "analysis/src/FakeRatio.hh"

MPAF*
AnalysisFactory::get(const string& analysis, const string& cfg) {
 
// list of analyzers and modules in alphabetical order 
  
  if(analysis=="csa14exerc") {
    return new csa14exerc(cfg);
  }
  if(analysis=="SSDLBoosted") {
    return new SSDLBoosted(cfg);
  }
  if(analysis=="synchECO") {
    return new synchECO(cfg);
  }
  if(analysis=="synchRA5") {
    return new synchRA5(cfg);
  }
  if(analysis=="phys14limits") {
    return new phys14limits(cfg);
  }
  if(analysis=="SUSY3L") {
    return new SUSY3L(cfg);
  }
  if(analysis=="SUSY3L_sync") {
    return new SUSY3L_sync(cfg);
  }
  if(analysis=="SUSY3L_sync2") {
    return new SUSY3L_sync2(cfg);
  }
  if(analysis=="FakeRatio") {
    return new FakeRatio(cfg);
  }
  return 0;
}
