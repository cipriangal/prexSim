

#include "MollerRunAction.hh"
#include "G4Run.hh"
#include "RootAnalysis.hh"
#include "G4RunManager.hh"


MollerRunAction::MollerRunAction()
{}


MollerRunAction::~MollerRunAction()
{}


void MollerRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  gRootAnalysis->BeginOfRunAction(aRun);

}


void MollerRunAction::EndOfRunAction(const G4Run* aRun)
{ 
  gRootAnalysis->EndOfRunAction(aRun);
}




