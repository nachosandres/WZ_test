#ifndef UncertaintyTest_HH
#define UncertaintyTest_HH

#include "analysis/core/MPAF.hh"

class UncertaintyTest: public MPAF {

public:

  // Member Functions

  UncertaintyTest(std::string);
  virtual ~UncertaintyTest();


private:

  void initialize();

  void run();
  void defineOutput();
  void modifyWeight();
  void writeOutput();

  void modifySkimming();


private: 

  //counter categories, 0 is ALWAYS global (even if not specified later
  enum {kGlobal=0};
  
};


#endif
