#include "MollerPrimaryGenAction.hh"
#include "MollerPrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"
#include "globals.hh"
#include "TRandom2.h"

using namespace CLHEP;

MollerPrimaryGenAction::MollerPrimaryGenAction()
  :ebeam(1.05), mp(0.9382796),  rasterX(5*mm), rasterY(5*mm)
{
  eventnumber=0;

  //create a messenger for this class
  gunMessenger = new MollerPrimaryGeneratorMessenger(this);

  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

  particleGun->SetParticleDefinition( G4ParticleTable::GetParticleTable()->FindParticle("e-") );
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(1.05*GeV);
  particleGun->SetParticlePosition(G4ThreeVector(0.*cm, 0.*cm, -1300.*cm));

}

MollerPrimaryGenAction::~MollerPrimaryGenAction()
{
  delete particleGun;
  delete gunMessenger;
}

void MollerPrimaryGenAction::GeneratePrimaries(G4Event* anEvent)
{

  // To add an additional generator:
  // 1. add an extra case statement here
  // 2. create another generator method in a separate file
  // 3. edit the GenCmd by altering the SetGuidance and SetRange method calls
  switch(GeneratorNumber) {
  case 0:
    GeneratePrimaries_moller_dustin(anEvent);
    break;
  case 1:
    GeneratePrimaries_elasticep_dustin(anEvent);
    break;
  case 2:
    GeneratePrimaries_inelasticep_dustin(anEvent);
    break;
  case 3:
    GeneratePrimaries_phasespace_mollers(anEvent);
    break;
    //  case 4:
    //    GeneratePrimaries_phasespace_motts(anEvent);
    //    break;
  case 4:
    GeneratePrimaries_prex(anEvent);
    break;
  case 5:
    GeneratePrimaries_g4raster(anEvent); /* Current setting for PREX simulation. Rakitha Thu May 16 19:04:49 EDT 2013 */
    break;
  case 6:
    GeneratePrimaries_g4(anEvent);
    break;
  case 7:
    GeneratePrimaries_g4Realraster(anEvent);
    break;
  default:
    G4cerr << "Generator number " << GeneratorNumber << " does not exist!\n";
    G4cerr << "Choose which generator to use.\n";
    G4cerr << "  Choice : 0. Moller electrons (Dustin) [default]\n";
    G4cerr << "           1. Elastic ep electrons (Dustin)\n";
    G4cerr << "           2. Inelastic ep electrons (Dustin)\n";
    G4cerr << "           3. Optics rays electrons (Moller scattering)\n";
    G4cerr << "           4. PREx testing\n";
    G4cerr << "           5. PREx, with raster\n";
    G4cerr << "           6. PREx, w/o raster\n";
    G4cerr << "           7. PREx, with realistic raster \n";
    G4cerr << "Running with default.\n";
    GeneratePrimaries_moller_dustin(anEvent);


    break;
  }
}

void MollerPrimaryGenAction::GeneratePrimaries_g4raster(G4Event* anEvent)
{
  G4double vertex_x = (-rasterX/2 + (rasterX*(G4UniformRand())));
  G4double vertex_y = (-rasterY/2 + (rasterY*(G4UniformRand())));

  G4double vertex_z = -1500.*cm;

  particleGun->SetParticlePosition(G4ThreeVector(vertex_x,vertex_y,vertex_z));
  //particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

  particleGun->GeneratePrimaryVertex(anEvent);
  anEvent->GetPrimaryVertex(0)->GetPrimary(0);

}

void MollerPrimaryGenAction::GeneratePrimaries_g4Realraster(G4Event* anEvent)
{
  G4double vertex_x = (-rasterX/2 + (rasterX * (G4UniformRand())));
  G4double vertex_y = (-rasterY/2 + (rasterY * (G4UniformRand())));
  G4double z_0 = 1666.7*cm;

  G4double vertex_z = -110.3*cm - z_0;

  G4double R, unitX,unitY,unitZ;

  R = sqrt(vertex_x*vertex_x + vertex_y*vertex_y + z_0*z_0);
  unitX = vertex_x/R;
  unitY = vertex_y/R;
  unitZ = z_0/R;

  particleGun->SetParticlePosition(G4ThreeVector(0,0,vertex_z));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));

  particleGun->GeneratePrimaryVertex(anEvent);
  anEvent->GetPrimaryVertex(0)->GetPrimary(0);
}

void MollerPrimaryGenAction::GeneratePrimaries_g4(G4Event* anEvent)
{
  G4double vertex_z = -1500.*cm;

  particleGun->SetParticlePosition(G4ThreeVector(0.,0.,vertex_z));
  particleGun->GeneratePrimaryVertex(anEvent);
  anEvent->GetPrimaryVertex(0)->GetPrimary(0);//->SetTrackID(99999);

}
