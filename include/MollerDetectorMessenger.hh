// ****************************************************
// *                                                  *
// * MollerDetectorMessenger to change values of      *
// * collimator positions/sizes.  Temporary solution; *
// * I intend to have a general geometry readin if we *
// * don't move to GEMC soon.                         *
// *                                                  *
// * Added 3-21-2011 by Juliette Mammei               *
// *                                                  *
// ****************************************************

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MollerDetectorMessenger_h
#define MollerDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class MollerDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MollerDetectorMessenger: public G4UImessenger
{
public:
  MollerDetectorMessenger(MollerDetectorConstruction*);
  virtual ~MollerDetectorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    

private:
  MollerDetectorConstruction* myDetector;
  G4UIdirectory*             detDir;

  G4UIcmdWithAString* shieldTypeCmd;
  G4UIcmdWithAString* collimTypeCmd;
  G4UIcmdWithAString* scHoleTypeCmd;
  G4UIcmdWithADoubleAndUnit* shieldThickCmd;
  G4UIcmdWithADoubleAndUnit* shieldRadiusCmd;
  G4UIcmdWithAString* shieldMatCmd;

  G4UIcmdWithADoubleAndUnit* targetCenterCmd;
  G4UIcmdWithADoubleAndUnit* targetLengthCmd;
  G4UIcmdWithAString* targetMatCmd;

  G4UIcmdWithADoubleAndUnit* collimInnerRCmd;

  G4UIcmdWithABool* hallCmd;
  G4UIcmdWithABool* collNoneCmd;
  G4UIcmdWithABool* collOnlyCmd;
  G4UIcmdWithABool* septumCmd;
  G4UIcmdWithAnInteger* NumCollCmd;
  G4UIcmdWithAnInteger* shldConfigCmd;
  G4UIcmdWithAnInteger* processPartCmd;

  G4UIcmdWithAString*         DetGeomFileCmd;
  G4UIcmdWithAString*         WriteGeomFileCmd;
  G4UIcmdWithABool*           ReadGeomFileCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

