

#include "MollerSteppingAction.hh"

#include "MollerDetectorConstruction.hh"
#include "RootAnalysis.hh"

#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"


MollerSteppingAction::MollerSteppingAction()		
{ }


MollerSteppingAction::~MollerSteppingAction()
{ }


void MollerSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    /*
    //Now the stepping length is set to zero for Kryptonite metrial and have introduced a new mechanism to properly kill track by depositing the energy into the volume. 
    //Therefore we no longer needs to artificially kill tracks in the UserSteppingAction : Rakitha Tue Jan 27 17:11:54 EST 2015

    /////////////////////////No Longer in Use//////////////////////////////

  G4Track*        track = aStep->GetTrack();
  // Anything passing material "Kryptonite" is killed                                                                                                                          
  if(track->GetMaterial()->GetName() == "Kryptonite")
    {
      track->SetTrackStatus(fStopAndKill);
    }
  gRootAnalysis->UserSteppingAction(aStep);       

    */
  G4Track*        track = aStep->GetTrack();

  if(track->GetMaterial()->GetName() == "VacuumOneWay")
    {
      if(track->GetTrackStatus()==fAlive){
	track->SetTrackStatus(fStopButAlive);
      }
      // if(track->GetTrackStatus()==fStopButAlive){
      //track->SetTrackStatus(fAlive);
      //}
      //}
    }
  gRootAnalysis->UserSteppingAction(aStep);
}




