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

  // hybridFileCmd = new G4UIcmdWithAString("/moller/field/setHybridMap",this);
  // hybridFileCmd->SetGuidance("Set the hybrid field map file name.");
  // hybridFileCmd->SetParameterName("hybridFilename",true);
  // hybridFileCmd->SetDefaultValue("blockyHybrid_3.0");
  // //hybridFileCmd->AvailableForStates(G4State_Init);
  // //hybridFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle, G4State_GeomClosed);

  // upstreamFileCmd = new G4UIcmdWithAString("/moller/field/setUpstreamMap",this);
  // upstreamFileCmd->SetGuidance("Set the upstream field map file name.");
  // upstreamFileCmd->SetParameterName("upstreamFilename",true);
  // upstreamFileCmd->SetDefaultValue("blockyUpstream_1.1");
  // //upstreamFileCmd->AvailableForStates(G4State_Init);
  //upstreamFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle, G4State_GeomClosed);

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

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MollerGlobalMagnetFieldMessenger::~MollerGlobalMagnetFieldMessenger()
{
  // delete hybridFileCmd;
  // delete upstreamFileCmd;

  delete fieldDir;
  delete magLowLimCmd;
  delete magHighLimCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MollerGlobalMagnetFieldMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

  if (command ==  magLowLimCmd ) 
    {
      //      G4cout << "Setting lower limit for septum field" << magLowLimCmd->GetNewDoubleValue(newValue) << G4endl;
      setField->SetLowLimSeptumField(magLowLimCmd->GetNewDoubleValue(newValue));
    }
  else if (command == magHighLimCmd ) 
    {
      setField->SetHighLimSeptumField(magHighLimCmd->GetNewDoubleValue(newValue)); 
    }
  // if (command == hybridFileCmd) 
  //   { setField->SetHybridMapFile(newValue); }
  // else if (command == upstreamFileCmd)
  //   { setField->SetUpstreamMapFile(newValue); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
