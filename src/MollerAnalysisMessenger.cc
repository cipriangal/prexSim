

#include "MollerAnalysisMessenger.hh"

#include "MollerAnalysis.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MollerAnalysisMessenger::MollerAnalysisMessenger(
                                          MollerAnalysis* MollerFilename)
:MollerAna(MollerFilename)
{

  anaDir = new G4UIdirectory("/moller/analysis/");
  anaDir->SetGuidance("Root filename control");

  rFileCmd = new G4UIcmdWithAString("/moller/ana/rootfilename",this);

  rFileCmd->SetGuidance("Choose the name of the rootile");
  rFileCmd->SetParameterName("rootfilename",true);
  rFileCmd->SetDefaultValue("moller_sim");
  G4String candidateList=("moller_sim" "test");
  //  rFileCmd->SetCandidates(candidateList);

  rFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MollerAnalysisMessenger::~MollerAnalysisMessenger()
{
  delete rFileCmd;
  delete anaDir;
}

void MollerAnalysisMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == rFileCmd )
    { MollerAna->SetRootFileName(newValue);}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

