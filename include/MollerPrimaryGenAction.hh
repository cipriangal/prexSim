
#ifndef MollerPrimaryGenAction_h
#define MollerPrimaryGenAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "Randomize.hh"

class G4ParticleGun;
class G4Event;
class MollerPrimaryGeneratorMessenger;


typedef struct {
  double theta;
  double y;
  double xs;
}cms_cross_section_t;

typedef struct {
  double theta;
  double Eprime;
  double xs;
}double_differential_t;

class MollerPrimaryGenAction : public G4VUserPrimaryGeneratorAction
{

public:
  MollerPrimaryGenAction();
  ~MollerPrimaryGenAction();

public:
  void GeneratePrimaries(G4Event* anEvent);
  void GeneratePrimaries_g4raster(G4Event* anEvent);
  void GeneratePrimaries_g4Realraster(G4Event* anEvent);
  void GeneratePrimaries_g4(G4Event* anEvent);
  void GeneratePrimaries_moller_dustin(G4Event* anEvent);
  void GeneratePrimaries_elasticep_dustin(G4Event* anevent);
  void GeneratePrimaries_inelasticep_dustin(G4Event* anevent);
  void GeneratePrimaries_phasespace_mollers(G4Event* anevent);
  void GeneratePrimaries_phasespace_motts(G4Event* anevent);
  void GeneratePrimaries_prex(G4Event* anevent);
  void SetGenerator(G4int val) { GeneratorNumber = val;}
  G4int GetGenerator() {return GeneratorNumber;}

  void SetRasterX(G4double val){rasterX = val;}
  void SetRasterY(G4double val){rasterY = val;}
  void setSeedValue (G4bool value) { fSetSeedConst = value;
    G4cout<<"Setting fSetSeedConst"<<G4endl;};

private:
  G4ParticleGun*                    particleGun;
  MollerPrimaryGeneratorMessenger*  gunMessenger;   //messenger of this class
  G4int                             GeneratorNumber; // Number of generator to use
  G4int                             eventnumber;
  static const G4int nbin = 1000;
  static const G4int nbinIneEP = 100;
  cms_cross_section_t XStable[nbin];
  double_differential_t DDtable[nbinIneEP][nbinIneEP];
  const G4double ebeam;
  const G4double mp;

  G4double rasterX, rasterY;
  G4bool fSetSeedConst;
};

#endif
