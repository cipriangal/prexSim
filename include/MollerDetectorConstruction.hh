
/**********************************************************/
/*      This header creates the Detector components and   */
/*      initializes the DetectorConstruction class.       */
/**********************************************************/


#ifndef MollerDetectorConstruction_h
#define MollerDetectorConstruction_h 1

#include "G4GDMLParser.hh"
#include "G4VUserDetectorConstruction.hh"
#include "MollerDetectorMessenger.hh"
#include "MollerGlobalMagnetField.hh"
#include <vector>

using namespace std;

class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VSensitiveDetector;
class G4Material;
class G4Element;
class G4UniformMagField;
class G4QuadrupoleMagField;
class MollerDetectorMessenger;

class MollerDetectorConstruction : public G4VUserDetectorConstruction
{
public:

  MollerDetectorConstruction();
  virtual ~MollerDetectorConstruction();

public: 
  G4VPhysicalVolume* Construct();

  void SetGlobalMagneticField();
  void ReadGlobalMagneticField();

  void setShieldThick (G4double value) { fShieldThick = value; };
  void setShieldRadius (G4double value) { fShieldRadius = value; };
  void setTargetCenter (G4double value) { fTargetCenter = value; };
  void setTargetLength (G4double value) { fTargetLength = value; };
  void setCollimInnerR (G4double value) { fcollInnerR = value; };

  void setDrawWholeGeo (G4bool value) { fDrawWholeGeo = value; 
    G4cout<<"Setting fDrawWholeGeo"<<G4endl;};

  void setDrawHall (G4bool value) { fDrawHall = value; 
    G4cout<<"Setting fDrawHall"<<G4endl;};

  void setDrawColl (G4bool value) { fDrawCollim = value; 
    G4cout<<"Setting fDrawCollim"<<G4endl;};

  void setDrawSeptum (G4bool value) { fDrawSeptum = value; 
    G4cout<<"Setting fDrawSeptum"<<G4endl;};

  void setNumColl(G4int value) { NUM_COLS = value;};

  void setShieldConfig(G4int value) { shldConfig = value;};

  G4int getNumColl() {return NUM_COLS; };

  void setCollimTypeName(const G4String&);
  void setscHoleTypeName(const G4String&);
  void setShieldTypeName(const G4String&);
  void setShieldMaterial(const G4String&);
  void setTargetMaterial(const G4String&);

  void WriteGeometryFile(const G4String& filename);

  void SetDetectorGeomFile(const G4String&);
  void SetReadGeo(G4bool value) {fReadGeoFile  = value;};
  void SetCollimGeomFile(const G4String&);

  G4bool GetReadgeo() {return fReadGeoFile;};
  G4String  GetDetectorGeomFile() {return detfileName;};
  G4double GetLowLimMagneticShield() {return mg_field_low;};
  G4double GetHighLimMagneticShield() {return mg_field_high;};
  G4double GetMagFieldScaleFactor(){return mgFieldScaleFactor;}

  void SetProcessParticles(const G4int val){processPart=val;};
protected:

  //void DefineDipoleFields();
  void DefineQuadFields();

private:
  G4int processPart;

  void DefineMaterials();

  G4Material*  shieldMaterial;
  G4Material*  shieldMaterial_1;
  G4Material*  shieldMaterial_2;
  G4Material*  shieldMaterial_3;
  G4Material*  collMaterial;
  G4Material*  targetMaterial;
  G4Material*  worldMaterial;
  G4Material*  scattChamberMaterial;
  G4Material*  beamlineMaterial;
  G4Material*  septumpipeMaterial;
  G4Material*  septumMaterial;
  G4Material*  floorMaterial;

  //a check for remoll material
  //G4Material*  scintmat;

  //Logical Volumes
  G4LogicalVolume* mother_logic;
  G4LogicalVolume* target_logic;
  G4LogicalVolume* sphereshld_logic;
  G4LogicalVolume* sphereshld_1_logic;
  G4LogicalVolume* sphereshld_2_logic;
  G4LogicalVolume* sphereshld_3_logic;
  G4LogicalVolume* cylshld_logic;
  G4LogicalVolume* cylshld2_logic;
  G4LogicalVolume* boxshld_logic;
  G4LogicalVolume* boxshldU_logic;
  G4LogicalVolume* boxshldD_logic;
  G4LogicalVolume* plusshld_logic;
  G4LogicalVolume* scatt_cham_shld_logic;
  G4LogicalVolume* spheredet_logic;
  G4LogicalVolume* dump_logic;
  G4LogicalVolume* ssenc_logic;
  G4LogicalVolume* hut_logic;
  G4LogicalVolume* wall_logic;
  G4LogicalVolume* floor_logic;
  G4LogicalVolume* W_coll_logic;
  G4LogicalVolume* scatt_cham_logic;
  G4LogicalVolume* septum_pipe_logic;
  G4LogicalVolume* septum_logic;
  G4LogicalVolume* testtube_logic;
  G4LogicalVolume* bl_logic[13];
  G4LogicalVolume* vacuum_enclosure_l_logic;
  G4LogicalVolume* vacuum_enclosure_r_logic;
  G4LogicalVolume* sept_shld_l_logic;
  G4LogicalVolume* sept_shld_r_logic;

  G4LogicalVolume* scatt_attach_logic;
 
  G4LogicalVolume* det1_logic;
  G4LogicalVolume* det2_logic;
  G4LogicalVolume* det3_logic;

  G4LogicalVolume* plane_det_1_logic;
  G4LogicalVolume* plane_det_2_logic;
  G4LogicalVolume* plane_det_3_logic;

  //Physical Volumes
  G4VPhysicalVolume* worldVolume;

  G4VPhysicalVolume* mother_phys;
  G4VPhysicalVolume* target_phys;
  G4VPhysicalVolume* shield_phys;
  G4VPhysicalVolume* shield_1_phys;
  G4VPhysicalVolume* shield_2_phys;
  G4VPhysicalVolume* shield_3_phys;
  G4VPhysicalVolume* cylshld_phys;
  G4VPhysicalVolume* cylshld2_phys;
  G4VPhysicalVolume* boxshld_phys;
  G4VPhysicalVolume* boxshldU_phys;
  G4VPhysicalVolume* boxshldD_phys;
  G4VPhysicalVolume* plusshld_phys;
  G4VPhysicalVolume* scatt_cham_shld_phys;
  G4VPhysicalVolume* det_phys;
  G4VPhysicalVolume* dump_phys;
  G4VPhysicalVolume* hut_phys;
  G4VPhysicalVolume* ssenc_phys;
  G4VPhysicalVolume* wall_phys;
  G4VPhysicalVolume* floor_phys;
  G4VPhysicalVolume* W_coll_phys;
  G4VPhysicalVolume* scatt_cham_phys;
  G4VPhysicalVolume* septum_pipe_phys;
  G4VPhysicalVolume* septum_phys;
  G4VPhysicalVolume* testtube_phys;
  G4VPhysicalVolume* bl_phys[13];
  G4VPhysicalVolume* vacuum_enclosure_l_phys;
  G4VPhysicalVolume* vacuum_enclosure_r_phys;
  G4VPhysicalVolume* scatt_attach_phys;

  G4VPhysicalVolume* sept_shld_l_phys;
  G4VPhysicalVolume* sept_shld_r_phys;

  G4VPhysicalVolume* det1_phys;
  G4VPhysicalVolume* det2_phys;
  G4VPhysicalVolume* det3_phys;

  G4VPhysicalVolume* plane_det_1_phys;
  G4VPhysicalVolume* plane_det_2_phys;
  G4VPhysicalVolume* plane_det_3_phys;


  G4Tubs*             target_solid;   // pointer to the solid Target
  

  G4VPhysicalVolume* ConstructDetector();

  void DumpGeometricalTree(G4VPhysicalVolume* aVolume,G4int depth=0);
  G4GDMLParser fGDMLParser;
  
 

  //MATERIALS


  //Isotope
  G4Isotope* D;
  G4Isotope* I_Pb;
  G4Isotope* I_Ca48;
  G4Isotope* I_Ca40;

  //Elements
  G4Element* elD;
  G4Element* Pb208;
  G4Element* Ca48;
  G4Element* Ca40;

  //Material
  G4Material* Pb;
  G4Material* purePb;
  G4Material* pureCa40;
  G4Material* pureCa48;
  G4Material* LD2;

  G4Material* rubberneo;
  G4Material* polymat;
  G4Material* concmat;
  G4Material* bormat;
  G4Material* borpoly;
  G4Material* borcrete;
  G4Material* polycrete;
  G4Material* W;
  G4Material* CW70;
  G4Material* Al;
  G4Material* Ar;
  G4Material* Mn;
  G4Material* C;
  G4Material* Cu;
  G4Material* Cr;
  G4Material* Ni;
  G4Material* Fe;
  G4Material* Si;
  G4Material* P;
  G4Material* S;
  G4Material* SS;
  G4Material* H2O;
  G4Material* Kryptonite;
  G4Material* Air;
  G4Material* H;
  


  //----------------------
  // global magnet section
  //----------------------
  //
  MollerGlobalMagnetField*      pGlobalMagnetField;

  G4FieldManager*         fGlobalFieldManager;
  G4ChordFinder*          fGlobalChordFinder;
  G4Mag_UsualEqRhs*       fGlobalEquation; 
  G4MagIntegratorStepper* fGlobalStepper;
  
  G4double                fMinStep;


  MollerDetectorMessenger* detectorMessenger;
  G4double fTargetLength;           // Full length of the target
  G4String shieldMat;           
  G4String shieldType;           
  G4String targMat;           
  G4String collimType;
  G4String scHoleType;

  G4int NUM_COLS;
  G4double fShieldThick;
  G4double fShieldRadius;
  G4double fTargetCenter;
  G4double fcollInnerR;
  G4bool fDrawWholeGeo;
  G4bool fDrawCollim;
  G4bool fDrawSeptum;
  G4bool fDrawHall;
  G4int shldConfig;

  G4String detfileName;
  G4String collimfileName;
  G4bool fReadGeoFile;

  G4double mg_field_low;
  G4double mg_field_high;
  G4double mgFieldScaleFactor;
};

#endif
