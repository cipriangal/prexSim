// ****************************************************
// *                                                  *
// * MollerFieldMessenger to change values of         *
// * field map names and scale factors.               *
// *                                                  *
// * Added 8-31-2011 by Juliette Mammei               *
// *                                                  *
// ****************************************************

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MollerGlobalMagnetFieldMessenger_h
#define MollerGlobalMagnetFieldMessenger_h 1

#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"
#include "MollerGlobalMagnetField.hh"
#include "globals.hh"

class MollerGlobalMagnetField;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MollerGlobalMagnetFieldMessenger: public G4UImessenger
{
  public:
    MollerGlobalMagnetFieldMessenger(MollerGlobalMagnetField*);
    ~MollerGlobalMagnetFieldMessenger();
  //    virtual ~MollerFieldMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:

    MollerGlobalMagnetField*    setField;
    G4UIdirectory*              fieldDir;

    G4UIcmdWithAString*         configurationCmd;
    G4UIcmdWithADouble*         magScaleFactorCmd;
    G4UIcmdWithADoubleAndUnit*  magLowLimCmd;
    G4UIcmdWithADoubleAndUnit*  magHighLimCmd;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

