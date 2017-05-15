//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef MollerGlobalMagnetField_h
#define MollerGlobalMagnetField_h
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// geant4 includes
#include "Geant4_include.hh"

// user includes
#include "MollerMagnetFieldMap.hh"
#include "MollerGlobalMagnetFieldMessenger.hh"

class MollerMagnetFieldMap;
class MollerGlobalMagnetFieldMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class MollerGlobalMagnetField : public G4MagneticField
{

public:

  MollerGlobalMagnetField();
  ~MollerGlobalMagnetField();


  void SetHybridMapFile(const G4String&);
  void SetUpstreamMapFile(const G4String&);

  void SetConfiguration(const G4String);
  void SetLowLimSeptumField(G4double value) { mg_field_low = value; };
  void SetHighLimSeptumField(G4double value) { mg_field_high = value; };
  void SetScaleFactor(G4double value) {magScaleFactor=value;}
  void SetQ1fringeValue(G4bool val){addQ1fringe=val;}

  G4double ReadLowLimSeptumField() {return mg_field_low;};
  G4double ReadHighLimSeptumField() {return mg_field_high;};
  G4double GetScaleFactor() {return magScaleFactor;}

  void GetFieldValue( const  G4double Point[4], G4double *Bfield ) const;
  void GetFieldValuePREX2( const  G4double Point[4], G4double *Bfield ) const;
  void GetFieldValueQ1( const  G4double Point[4], G4double *Bfield ) const;
  void GetFieldValueCREX( const  G4double Point[4], G4double *Bfield ) const;
  void WriteMagField();

private:

  G4double interpolate(std::vector<double> xV,
		       std::vector<double> yV,
		       G4double val)const;

  MollerMagnetFieldMap*  fMagneticField_MainMagnet;
  MollerMagnetFieldMap*  fMagneticField_MiniMagnet;

  MollerGlobalMagnetFieldMessenger* fieldMessenger;   //messenger of this class

  G4double BFieldScalingFactor_MainMagnet;
  G4double BFieldScalingFactor_MiniMagnet;

  G4String upstreamFilename;
  G4String hybridFilename;
  G4String configuration;

  G4bool addQ1fringe;

  G4double magScaleFactor;
  G4double mg_field_low;
  G4double mg_field_high;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
