

#include "MollerPrimaryGeneratorMessenger.hh"

#include "MollerPrimaryGenAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MollerPrimaryGeneratorMessenger::MollerPrimaryGeneratorMessenger(
                                                                 MollerPrimaryGenAction* MollerGun)
  :MollerAction(MollerGun)
{
  gunDir = new G4UIdirectory("/moller/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");

  GenCmd = new G4UIcmdWithAnInteger("/moller/gun/gen",this);
  GenCmd->SetGuidance("Choose which generator to use.");
  GenCmd->SetGuidance("  Choice : 0. Moller electrons (Dustin) [default]");
  GenCmd->SetGuidance("           1. Elastic ep electrons (Dustin)");
  GenCmd->SetGuidance("           2. Inelastic ep electrons (Dustin)");
  GenCmd->SetGuidance("           3. Optics rays electrons (Moller scattering)");
  GenCmd->SetGuidance("           4. PREx testing");
  GenCmd->SetGuidance("           5. PREx, with raster");
  GenCmd->SetGuidance("           6. PREx, w/o raster");
  GenCmd->SetGuidance("           7. PREx, with realistic raster");
  GenCmd->SetParameterName("gen",true);
  GenCmd->SetDefaultValue(0);
  GenCmd->SetRange("gen>=0 && gen<=7");
  GenCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  seedCmd = new G4UIcmdWithABool("/moller/gun/setConstSeed",this);
  seedCmd->SetGuidance("Set whether to set the random seed to a constant.");
  seedCmd->SetParameterName("fSetSeedConst",true);
  seedCmd->SetDefaultValue(false);

  rasterXcmd = new G4UIcmdWithADoubleAndUnit("/prex/gun/setRasterX",this);
  rasterXcmd->SetGuidance("Set raster X size with unit");
  rasterXcmd->SetDefaultValue(5);
  rasterXcmd->SetDefaultUnit("cm");


  rasterYcmd = new G4UIcmdWithADoubleAndUnit("/prex/gun/setRasterY",this);
  rasterYcmd->SetGuidance("Set raster Y size with unit");
  rasterYcmd->SetDefaultValue(5);
  rasterYcmd->SetDefaultUnit("cm");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MollerPrimaryGeneratorMessenger::~MollerPrimaryGeneratorMessenger()
{
  delete GenCmd;
  delete seedCmd;
  delete gunDir;
  delete rasterXcmd;
  delete rasterYcmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MollerPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == GenCmd ){
    MollerAction->SetGenerator(GenCmd->GetNewIntValue(newValue));
  }else if( command == seedCmd ){
    MollerAction->setSeedValue(seedCmd->GetNewBoolValue(newValue));
  }else if( command == rasterXcmd ){
    MollerAction->SetRasterX(rasterXcmd->GetNewDoubleValue(newValue));
  }else if( command == rasterYcmd ){
    MollerAction->SetRasterY(rasterYcmd->GetNewDoubleValue(newValue));
  }else{
    G4cout<<__PRETTY_FUNCTION__<<"\t"<<__LINE__<<G4endl;
    G4cout<<"Warning! command "<<command<<" not recognized. Continuing!"<<G4endl;
  }
}

G4String MollerPrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand * command)
{
  G4String cv;

  if( command==GenCmd )
    { cv = GenCmd->ConvertToString(MollerAction->GetGenerator()); }

  return cv;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
