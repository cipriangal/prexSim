#include "MollerPrimaryGenAction.hh"
//#include "MollerPrimaryGenAction_moller.cc"
// #include "MollerPrimaryGenAction_elasticep.cc"
// #include "MollerPrimaryGenAction_inelasticep.cc"
// #include "MollerPrimaryGenAction_phasespace.cc"
// #include "MollerPrimaryGenAction_prex.cc"
//#include "MollerPrimaryGenAction_phasespace_motts.cc"
#include "MollerPrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"
#include "globals.hh"
#include "TRandom2.h"

MollerPrimaryGenAction::MollerPrimaryGenAction() : ebeam(1.05), mp(0.9382796)
{
  TRandom2 *random_num = new TRandom2(0);
  //long particle_seed = (long) random_num->Integer();//this works
  long particle_seed = (long) random_num->GetSeed();//seemed to work
  CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);

  CLHEP::HepRandom::setTheSeed(particle_seed); 
  eventnumber=0;
  
  //create a messenger for this class
  gunMessenger = new MollerPrimaryGeneratorMessenger(this);

  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

  //set particle
  //G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  //  G4ParticleDefinition* particle = G4Proton::ProtonDefinition();
  
  G4cout << "Electron gun created!!\n" << G4endl;

  //  particleGun->SetParticleDefinition(particle);
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
  if(anEvent->GetEventID()==0){
  TRandom2 *random_num = new TRandom2(0);
  long particle_seed = (long) random_num->GetSeed();
  particle_seed += time(0);

  if (!fSetSeedConst) {
    G4cout<<"Setting the seed to a random value for event "<<anEvent->GetEventID()<<G4endl;
    CLHEP::HepRandom::setTheSeed(particle_seed);
    G4cout<<"The seed is: "<<CLHEP::HepRandom::getTheSeed()<<G4endl;
  }
  if (fSetSeedConst) {
    G4cout<<"Setting the seed to a constant value for event "<<anEvent->GetEventID()<<G4endl;
    //CLHEP::HepRandom::setTheSeed(194738798457);  //First event too low angle
    //CLHEP::HepRandom::setTheSeed(734687698678); //First event too high angle
    CLHEP::HepRandom::setTheSeed(94528347598798); 
    G4cout<<"The seed is: "<<CLHEP::HepRandom::getTheSeed()<<G4endl;
  }

  }
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
  //For 5 by 5 mm raster
  G4double vertex_x = (-0.25*cm + (0.5*cm*(G4UniformRand())));
  G4double vertex_y = (-0.25*cm + (0.5*cm*(G4UniformRand())));
  //For 4 by 4 mm raster
  //G4double vertex_x = (-0.2*cm + (0.4*cm*(G4UniformRand())));
  //G4double vertex_y = (-0.2*cm + (0.4*cm*(G4UniformRand())));

  G4double vertex_z = -1500.*cm;

  //G4cout<<"x0 : y0 : z0 "<<vertex_x/cm<<" : "<<vertex_y/cm<<" : "<<vertex_z/cm<<G4endl;

  particleGun->SetParticlePosition(G4ThreeVector(vertex_x,vertex_y,vertex_z));
  //particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

  particleGun->GeneratePrimaryVertex(anEvent);
  anEvent->GetPrimaryVertex(0)->GetPrimary(0);//->SetTrackID(99999);

}

void MollerPrimaryGenAction::GeneratePrimaries_g4Realraster(G4Event* anEvent)
{
  //For 5 by 5 mm raster
  G4double vertex_x = (-0.25*cm + (0.5*cm*(G4UniformRand())));
  G4double vertex_y = (-0.25*cm + (0.5*cm*(G4UniformRand())));
  //For 4 by 4 mm raster
  //G4double vertex_x = (-0.2*cm + (0.4*cm*(G4UniformRand())));
  //G4double vertex_y = (-0.2*cm + (0.4*cm*(G4UniformRand())));
  G4double z_0 = 1666.7*cm;

  G4double vertex_z = -105.3*cm - z_0;

  G4double R, unitX,unitY,unitZ;

  R = sqrt(vertex_x*vertex_x + vertex_y*vertex_y + z_0*z_0);
  unitX = vertex_x/R;
  unitY = vertex_y/R;
  unitZ = z_0/R;

  //G4cout<<"x0 : y0 : z0 "<<vertex_x/cm<<" : "<<vertex_y/cm<<" : "<<vertex_z/cm<<G4endl;

  particleGun->SetParticlePosition(G4ThreeVector(0,0,vertex_z));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(unitX,unitY,unitZ));
  //particleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1.));

  particleGun->GeneratePrimaryVertex(anEvent);
  anEvent->GetPrimaryVertex(0)->GetPrimary(0);//->SetTrackID(99999);

}

void MollerPrimaryGenAction::GeneratePrimaries_g4(G4Event* anEvent)
{
  G4double vertex_z = -1500.*cm;

  particleGun->SetParticlePosition(G4ThreeVector(0.,0.,vertex_z));
  particleGun->GeneratePrimaryVertex(anEvent);
  anEvent->GetPrimaryVertex(0)->GetPrimary(0);//->SetTrackID(99999);

}
