

#include "MollerSteppingVerbose.hh"

#include "G4SteppingManager.hh"
#include "G4UnitsTable.hh"

using namespace CLHEP;


MollerSteppingVerbose::MollerSteppingVerbose()
{
  textfile.open("results_geant.txt");
}


MollerSteppingVerbose::~MollerSteppingVerbose()
{
  textfile.close();
}
 

void MollerSteppingVerbose::StepInfo()
{
  CopyState();
  

  G4int prec = G4cout.precision(6);
  G4int parent = fTrack->GetParentID();
  G4int ID = fTrack->GetTrackID();
  G4int step = fTrack->GetCurrentStepNumber();

  G4String volume2 = fTrack->GetVolume()->GetName();
  G4String particle = fTrack->GetDefinition()->GetParticleName();
  
  
  G4StepPoint* thePrePoint = fStep->GetPreStepPoint();
  G4StepPoint* thePostPoint = fStep->GetPostStepPoint();

  const G4VProcess* creatorProcess = fTrack->GetCreatorProcess();
  G4String process;
  if (creatorProcess!=0) process = creatorProcess->GetProcessName();

  const G4VProcess* stepProcess = thePostPoint->GetProcessDefinedStep();
  G4String process1;
  if (stepProcess!=0) process1 = stepProcess->GetProcessName();

  G4int ion = process.contains("eIoni");
  
  G4ThreeVector loc = fTrack->GetPosition();
  G4ThreeVector p_loc = fTrack->GetMomentumDirection();
  G4double kineE = fTrack->GetKineticEnergy();

  G4ThreeVector init = fTrack->GetVertexPosition();

  G4double kineE0 = thePrePoint->GetKineticEnergy();
  G4double kineE1 = thePostPoint->GetKineticEnergy();
  G4double kineE2 = fTrack->GetVertexKineticEnergy();

  G4ThreeVector p0 = thePrePoint->GetMomentumDirection();
  G4ThreeVector p1 = thePostPoint->GetMomentumDirection();
  G4ThreeVector p2 = fTrack->GetVertexMomentumDirection();

  G4double theta2 = sqrt(p2.x()*p2.x() + p2.y()*p2.y());
  

  if( (particle == "e-") &&
      (kineE2==11*GeV) &&
      ((kineE0-kineE1)>1.0*GeV) && 
      (process1=="eIoni")
      ){ 
    
    G4cout << std::setw(8) << particle << ", "
	   << std::setw(9) << parent << ", "
	   << std::setw(9) << ID << ", "
	   << std::setw(9) << kineE0 << ", "
	   << std::setw(9) << kineE1 << ", "
	   << std::setw(9) << kineE2 << ", "
	   << std::setw(9) << volume2 << ", "
	   << std::setw(9) << process << ", "
	   << std::setw(9) << process1 << ", "
	   << std::setw(9) << step << ", "
	   << "  " << G4endl;
    
  }
  
  if( (init.z() > -75.0*cm) &&
      (init.z() < 75.0*cm) &&
      (particle == "e-") &&
      (kineE > 2.75*GeV) && 
      (kineE < 5.5*GeV) &&
      (theta2 > 0.0096295) && 
      (theta2 < 0.016679) && ion  
      ){ 
    
    G4cout << std::setw(8) << particle << ", "
	   << std::setw(9) << parent << ", "
	   << std::setw(9) << ID << ", "
	   << std::setw(9) << kineE0 << ", "
	   << std::setw(9) << kineE1 << ", "
	   << std::setw(9) << kineE2 << ", "
	   << std::setw(9) << volume2 << ", "
	   << std::setw(9) << process << ", "
	   << std::setw(9) << process1 << ", "
	   << std::setw(9) << step << ", "
	   << "  " << G4endl;
  }

  G4cout.precision(prec);
}


void MollerSteppingVerbose::TrackingStarted()
{
  
}


