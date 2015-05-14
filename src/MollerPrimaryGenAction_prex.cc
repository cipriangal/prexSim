#include "G4ParticleGun.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Event.hh"
#include "globals.hh"

#include "MollerPrimaryGenAction.hh"
#include "RootAnalysis.hh"

void MollerPrimaryGenAction::GeneratePrimaries_prex(G4Event* anEvent)
{
  //----------------------
  // GenerateEvent
  //----------------------
  
  double initial_x = 0.0;//*cm;
  double initial_y = 0.0;//*cm;
  double initial_z = -1300.0;//*cm;

  double initial_px = 0.0;//*GeV;
  double initial_py = 0.0;//*GeV;
  double initial_pz = 0.0;//*GeV;

  double Evertex = ebeam;

  initial_px = 0.0;
  initial_py = 0.0;
  initial_pz = Evertex; // GeV


  G4ParticleDefinition* particle = G4Electron::ElectronDefinition();
  //G4ParticleDefinition* particle = G4Proton::ProtonDefinition();
  //We will only throw the electron for now

  //  G4cout << "Seeing PREx Generator!!\n" << G4endl;
  //  G4cout << "Beam energy is " << ebeam<<G4endl;

  
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(initial_px/Evertex,initial_py/Evertex,initial_pz/Evertex));
  //  particleGun->SetParticleEnergy(Evertex*1000);
  particleGun->SetParticleEnergy(Evertex*1000);
  particleGun->SetParticlePosition(G4ThreeVector(initial_x*10, initial_y*10, initial_z*10));
  particleGun->GeneratePrimaryVertex(anEvent);
  
  gRootAnalysis->SetMomentum0(Evertex*1000,initial_px,initial_py,initial_pz);
  gRootAnalysis->SetMomentum1(Evertex*1000,initial_px,initial_py,initial_pz);

  
  /*
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(ebeam);
  particleGun->SetParticlePosition(G4ThreeVector(0.*cm, 0.*cm, -125.*cm));
  */

  //anEvent->SetEventID(0);
  anEvent->GetPrimaryVertex(0)->GetPrimary(0);//->SetTrackID(99999);

}
