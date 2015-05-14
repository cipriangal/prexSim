

#include "MollerEventAction.hh"

#include "MollerDetectorHit.hh"

//#ifdef G4UI_USE_ROOT
#include "RootAnalysis.hh"
//#endif

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"


 
MollerEventAction::MollerEventAction() 
{;}

 
MollerEventAction::~MollerEventAction() 
{;}

 
void MollerEventAction::BeginOfEventAction(const G4Event* anEvent)
{
  gRootAnalysis->BeginOfEventAction(anEvent);
}


void MollerEventAction::EndOfEventAction(const G4Event* anEvent)
{
  gRootAnalysis->EndOfEventAction(anEvent);
}

