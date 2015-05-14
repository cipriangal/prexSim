


#ifndef MollerSteppingVerbose_h
#define MollerSteppingVerbose_h 1

#include "G4SteppingVerbose.hh"
#include <fstream>

using namespace std;



class MollerSteppingVerbose : public G4SteppingVerbose 
{
 public:
   
  MollerSteppingVerbose();
 ~MollerSteppingVerbose();

  void StepInfo();
  void TrackingStarted();

  ofstream textfile;

};


#endif
