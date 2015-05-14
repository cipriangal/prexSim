

#ifndef MollerPrimaryGeneratorMessenger_h
#define MollerPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class MollerPrimaryGenAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MollerPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  MollerPrimaryGeneratorMessenger(MollerPrimaryGenAction*);
  virtual ~MollerPrimaryGeneratorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
  G4String GetCurrentValue(G4UIcommand * command);

private:
  MollerPrimaryGenAction*   MollerAction;
  G4UIdirectory*            gunDir; 
  G4UIcmdWithAnInteger*     GenCmd;
  G4UIcmdWithABool* seedCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

