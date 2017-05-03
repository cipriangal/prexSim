// ****************************************************
// *                                                  *
// * MollerFieldMessenger to change values of         *
// * field map names and scale factors.               *
// *                                                  *
// * Added 8-31-2011 by Juliette Mammei               *
// *                                                  *
// ****************************************************

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MollerGlobalMagnetFieldMessenger.hh"

#include "MollerGlobalMagnetField.hh"

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MollerGlobalMagnetFieldMessenger::MollerGlobalMagnetFieldMessenger(MollerGlobalMagnetField* myField)
:setField(myField)
{
  fieldDir = new G4UIdirectory("/moller/field/");
  fieldDir->SetGuidance("Magnet field control.");

  magLowLimCmd = new G4UIcmdWithADoubleAndUnit("/moller/field/setLowLim",this);  
  magLowLimCmd->SetGuidance("Set Lower limit for shield in the Septum field.");
  magLowLimCmd->SetParameterName("Mfield_low",true);
  magLowLimCmd->SetDefaultValue(-44.0);
  magLowLimCmd->SetDefaultUnit("cm");
  magLowLimCmd->SetRange("Mfield_low<=-44.0");
  //  magLowLimCmd->AvailableForStates(G4State_Init,G4State_PreInit,G4State_Idle, G4State_GeomClosed,G4State_Quit);

  magHighLimCmd = new G4UIcmdWithADoubleAndUnit("/moller/field/setHighLim",this);  
  magHighLimCmd->SetGuidance("Set Higher limit for shield in the Septum field.");
  magHighLimCmd->SetParameterName("Mfield_high",true);
  magHighLimCmd->SetDefaultValue(44.0);
  magHighLimCmd->SetDefaultUnit("cm");
  magHighLimCmd->SetRange("Mfield_high>=44.0");
  //  magHighLimCmd->AvailableForStates(G4State_Init,G4State_PreInit,G4State_Idle, G4State_GeomClosed,G4State_Quit);

  magScaleFactorCmd = new G4UIcmdWithADouble("/moller/field/setFieldScaleFactor",this);  
  magScaleFactorCmd->SetGuidance("Set magnetic field scale factor.");
  magScaleFactorCmd->SetParameterName("magScaleFactor",true);
  magScaleFactorCmd->SetDefaultValue(1.);

  configurationCmd = new G4UIcmdWithAString("/moller/field/setConfiguration",this);
  configurationCmd->SetGuidance("Possible options: prex1, prex2, crex");
  configurationCmd->SetParameterName("configuration",true);
  configurationCmd->SetDefaultValue("crex");


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MollerGlobalMagnetFieldMessenger::~MollerGlobalMagnetFieldMessenger()
{
  delete fieldDir;
  delete magLowLimCmd;
  delete magHighLimCmd;
  delete configurationCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MollerGlobalMagnetFieldMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

  if (command ==  magLowLimCmd ){
    setField->SetLowLimSeptumField(magLowLimCmd->GetNewDoubleValue(newValue));
  } else if (command == magHighLimCmd ){
    setField->SetHighLimSeptumField(magHighLimCmd->GetNewDoubleValue(newValue)); 
  } else if (command == magScaleFactorCmd ){
    setField->SetScaleFactor(magScaleFactorCmd->GetNewDoubleValue(newValue));
  } else if (command == configurationCmd ){
    setField->SetConfiguration(newValue);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
