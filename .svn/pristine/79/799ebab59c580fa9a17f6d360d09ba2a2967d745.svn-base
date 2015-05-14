

#ifndef MollerAnalysisMessenger_h
#define MollerAnalysisMessenger_h 1


#include "G4UImessenger.hh"
#include "globals.hh"

class MollerAnalysis;
class G4UIdirectory;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MollerAnalysisMessenger: public G4UImessenger
{
public:
  MollerAnalysisMessenger(MollerAnalysis*);
  virtual ~MollerAnalysisMessenger();

  void SetNewValue(G4UIcommand*, G4String);
    

private:
  MollerAnalysis*   MollerAna;
  G4UIdirectory*          anaDir; 
  G4UIcmdWithAString*     rFileCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

