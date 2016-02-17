//=============================================================================
// 
//   ---------------------------
//  | Doxygen File Information |
//  ---------------------------
/**
 
   \file MollerGlobalMagnetField.hh
   $Revision: 1.2 $	
   $Date: 2005/12/27 19:24:27 $
   \author Klaus Hans Grimm
     
*/
//=============================================================================
// 
//=============================================================================
// 
//   ---------------------------
//  | Doxygen Class Information |
//  ---------------------------
/**
   \class MollerGlobalMagnetField
    
   \brief Set in MollerDetectorConstruction as the field manager.

 MollerGlobalMagnetField::MollerGlobalMagnetField() sets the field map names, initializes 
 the grid after setting the min and max r, z, phi, and reads in the maps.  
 MollerGlobalMagnetField::GetFieldValue() adds the field value for the hybrid and upstream toroids 
 from MollerMagnetFieldMap::GetFieldValue (which does the interpolation) during tracking.

    
 */
//=============================================================================
//
//=============================================================================
//   -----------------------
//  | CVS File Information |
//  -----------------------
// 
//  Last Update:      $Author: grimm $
//  Update Date:      $Date: 2005/12/27 19:24:27 $
//  CVS/RCS Revision: $Revision: 1.2 $
//  Status:           $State: Exp $
// 
// ===================================
//  CVS Revision Log at end of file !!
// ===================================
// 
//=============================================================================

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef MollerGlobalMagnetField_h
#define MollerGlobalMagnetField_h 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// geant4 includes
#include "Geant4_include.hh" 

// user includes
#include "MollerMagnetFieldMap.hh"
#include "MollerGlobalMagnetFieldMessenger.hh"

// root include
#include "TGraph.h"
// user classes
//class MollerGlobalMagnetFieldMessenger;
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
  
  void SetLowLimSeptumField(G4double value) { mg_field_low = value; };
  void SetHighLimSeptumField(G4double value) { mg_field_high = value; };
  void SetScaleFactor(G4double value) {magScaleFactor=value;}
  
  G4double ReadLowLimSeptumField() {return mg_field_low;};
  G4double ReadHighLimSeptumField() {return mg_field_high;};
  G4double GetScaleFactor() {return magScaleFactor;}

  void GetFieldValue( const  G4double Point[4], G4double *Bfield ) const;  
  void WriteMagField();

private:

  //MollerGlobalMagnetFieldMessenger*      fGlobalMagnetFieldMessenger;

  MollerMagnetFieldMap*  fMagneticField_MainMagnet;
  MollerMagnetFieldMap*  fMagneticField_MiniMagnet;
  
  MollerGlobalMagnetFieldMessenger* fieldMessenger;   //messenger of this class

  G4double BFieldScalingFactor_MainMagnet;
  G4double BFieldScalingFactor_MiniMagnet;
  
  G4String upstreamFilename;
  G4String hybridFilename;

  G4double magScaleFactor;
  G4double mg_field_low;
  G4double mg_field_high; 

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
