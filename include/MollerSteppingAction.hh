

#ifndef MollerSteppingAction_h
#define MollerSteppingAction_h 1

#include "G4UserSteppingAction.hh"

//class MollerDetectorConstruction;
//class MollerEventAction;

class MollerSteppingAction : public G4UserSteppingAction
{
public:
  MollerSteppingAction();
  ~MollerSteppingAction();
  
  void UserSteppingAction(const G4Step*);
  
private:

};


#endif
