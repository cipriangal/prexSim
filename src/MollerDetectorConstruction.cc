#include "MollerDetectorConstruction.hh"
#include "MollerDetectorMessenger.hh"

#include "MollerDetectorSD.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "globals.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

#include "G4UniformMagField.hh" 
#include "G4EqMagElectricField.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ChordFinder.hh"
#include "G4PropagatorInField.hh"

#include "G4UImanager.hh"
#include "G4UIcommand.hh"

#include "G4ios.hh"
#include <iostream>
#include "TString.h"


//visual
#include "G4VisAttributes.hh"
#include "G4Colour.hh"



MollerDetectorConstruction::MollerDetectorConstruction()
  :pGlobalMagnetField(0)
{
  G4cout << G4endl <<"Setting Magnetic field " << G4endl ;
  SetGlobalMagneticField();
  DefineMaterials();
  detectorMessenger = new MollerDetectorMessenger(this);
}

MollerDetectorConstruction::~MollerDetectorConstruction()
{
  if (pGlobalMagnetField) delete pGlobalMagnetField;
  delete detectorMessenger;
}
void MollerDetectorConstruction::SetDetectorGeomFile(const G4String& nam) 
{
  G4String fPath = ".";
  detfileName = (fPath+"/"+nam);
}

G4VPhysicalVolume* MollerDetectorConstruction::Construct() 
{


  G4cout << G4endl << "###### Calling MollerDetectorConstruction::Read() " << G4endl << G4endl;

  if (fReadGeoFile){
    G4cout << "Attempting to read geometry file. Geant4 needs to have GDML support" << G4endl;

    fGDMLParser.SetOverlapCheck(true);
    fGDMLParser.Read(detfileName);
    
    worldVolume = fGDMLParser.GetWorldVolume();

  //==========================
  // List auxiliary info
  //==========================

  const G4GDMLAuxMapType* auxmap = fGDMLParser.GetAuxMap();

  G4cout << "Found " << auxmap->size()
         << " volume(s) with auxiliary information."
         << G4endl << G4endl;
  for(G4GDMLAuxMapType::const_iterator
      iter  = auxmap->begin();
      iter != auxmap->end(); iter++) 
  {
    G4cout << "Volume " << ((*iter).first)->GetName()
           << " has the following list of auxiliary information: "<< G4endl;
    for (G4GDMLAuxListType::const_iterator
         vit  = (*iter).second.begin();
         vit != (*iter).second.end(); vit++)
    {
      G4cout << "--> Type: " << (*vit).type
             << " Value: "   << (*vit).value << std::endl;
    }
  }
  G4cout << G4endl<< G4endl;


  //==========================
  // Sensitive detectors
  //==========================
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  char detectorname[200];

  MollerDetectorSD* collimatordetector[100];

  G4int k=0;
  for(G4GDMLAuxMapType::const_iterator
      iter  = auxmap->begin();
      iter != auxmap->end(); iter++)
  {
    G4LogicalVolume* myvol = (*iter).first;
    G4cout << "Volume " << myvol->GetName();
    
    for (G4GDMLAuxListType::const_iterator
	   vit  = (*iter).second.begin();
           vit != (*iter).second.end(); vit++)
    {
      if ((*vit).type == "SensDet")
      {
        G4String det_type = (*vit).value;
	//G4cout << " is a " << det_type <<  G4endl << G4endl;

	snprintf(detectorname,200,"/detector%i",k+1);
	collimatordetector[k] = new MollerDetectorSD(detectorname);

        if (collimatordetector[k] != 0)
        {

          G4cout << "  Creating sensitive detector " << det_type
                 << " for volume " << myvol->GetName()
                 <<  G4endl << G4endl;
	  char volumeNumber[20] = "volumeN"; 
	  TString vol_str(det_type.data());
	  G4int n_vol = -1;
	  if ( vol_str.BeginsWith(volumeNumber) ) {/* volume number set to integer value to be saved in to ntuple */
	    vol_str.Remove(0,7);
	    n_vol =  vol_str.Atoi(); 
	    // G4cout << "volume " << vol_str.Data() << " number overwritten to " << n_vol << G4endl;
	    collimatordetector[k]->SetVolume(n_vol);
	    G4cout << "volume number overwritten to " << n_vol <<" to be accessed in the TNtuple" << G4endl;
	  }
          SDman->AddNewDetector(collimatordetector[k]);
          myvol->SetSensitiveDetector(collimatordetector[k]);
        }
        else
        {
          G4cout << det_type << " sensitive detector type not found" << G4endl;
	}
      }
    }
    k++;
  }

  //==========================
  // Visualization attributes
  //==========================

  G4VisAttributes* motherVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* daughterVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* targetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes* planeDetVisAtt= new G4VisAttributes(G4Colour(0.3,0.8,0.5));
  G4VisAttributes* kryptoVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));//magenta 

  motherVisAtt->SetVisibility(false);

  targetVisAtt->SetVisibility(true);

  daughterVisAtt->SetVisibility(true);
  //  daughterVisAtt->SetForceWireframe (true);

  planeDetVisAtt->SetVisibility(true);
  planeDetVisAtt->SetForceWireframe(true);

  G4VisAttributes* WVisAtt= new G4VisAttributes(G4Colour(0.388,0.345,0.259));
  G4VisAttributes* CW70VisAtt= new G4VisAttributes(G4Colour(0.7,0.7,0.9)); 
  G4VisAttributes* PbVisAtt= new G4VisAttributes(G4Colour(0.6,0.7,0.8));
  G4VisAttributes* FeVisAtt= new G4VisAttributes(G4Colour(0.6,0.7,0.8));
  G4VisAttributes* AlVisAtt= new G4VisAttributes(G4Colour(0.4,0.6,0.4,0.99));
  G4VisAttributes* SSVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  G4VisAttributes* CuVisAtt= new G4VisAttributes(G4Colour(1.0,0.5,0.1));
  G4VisAttributes* SiVisAtt= new G4VisAttributes(G4Colour(0.4,0.4,0.0));
  G4VisAttributes* shieldVisAtt= new G4VisAttributes(G4Colour(0.7,0.0,0.0));
  G4VisAttributes* oringVisAtt= new G4VisAttributes(G4Colour(0.7,0.2,0.2));
  G4VisAttributes* ConcVisAtt= new G4VisAttributes(G4Colour(0.976,0.839,0.620));
 

  WVisAtt->SetVisibility(true);
  CW70VisAtt->SetVisibility(true);
  PbVisAtt->SetVisibility(true);
  FeVisAtt->SetVisibility(true);
  AlVisAtt->SetVisibility(true);
  CuVisAtt->SetVisibility(true);
  SiVisAtt->SetVisibility(true);
  shieldVisAtt->SetVisibility(true);
  oringVisAtt->SetVisibility(true);
  ConcVisAtt->SetVisibility(true);
  /*
  WVisAtt->SetForceWireframe(true);
  PbVisAtt->SetForceWireframe(true);
  FeVisAtt->SetForceWireframe(true);
   */ 
  //AlVisAtt->SetForceWireframe(true);
  /*
  CuVisAtt->SetForceWireframe(true);
  SiVisAtt->SetForceWireframe(true);
  oringVisAtt->SetForceWireframe(true);
  */
  // shieldVisAtt->SetForceWireframe(true);
  worldVolume->GetLogicalVolume()->SetVisAttributes(motherVisAtt);

  
  for(int i=0;i<worldVolume->GetLogicalVolume()->GetNoDaughters();i++)
  {

    worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(daughterVisAtt); 
    
    for (int j=0;j<worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetNoDaughters();j++) {
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(daughterVisAtt);

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Vacuum")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(planeDetVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(worldMaterial);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Tungsten")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(WVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(W);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Concrete")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(ConcVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(concmat);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Shield")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(shieldVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(shieldMaterial);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Water")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(shieldVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(H2O);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("StainlessSteel")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(SSVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(SS);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Iron")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(FeVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(Fe);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("PureLead")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(PbVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(purePb);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Copper")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(CuVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(Cu);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Aluminum")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(AlVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(Al);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Quartz")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(SiVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(Si);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("LiquidDeuterium")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(targetVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(LD2);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Calcium48")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(targetVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(pureCa48);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Calcium40")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(targetVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(pureCa40);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Rubber")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(oringVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(rubberneo);}

      if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName().compare("Kryptonite")==0){
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetVisAttributes(kryptoVisAtt); 
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetMaterial(Kryptonite);
	worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->SetUserLimits( new G4UserLimits(0.0, 0.0, 0.0, DBL_MAX, DBL_MAX) );
      }

    }
    
    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Vacuum")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(planeDetVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(worldMaterial);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Tungsten")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(WVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(W);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("CW70")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(CW70VisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(CW70);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Concrete")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(ConcVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(concmat);}
    
    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Shield")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(shieldVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(shieldMaterial);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Water")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(shieldVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(H2O);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("StainlessSteel")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(SSVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(SS);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Iron")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(FeVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(Fe);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("PureLead")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(PbVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(purePb);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Copper")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(CuVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(Cu);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Aluminum")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(AlVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(Al);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Silicon")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(SiVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(Si);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("LiquidDeuterium")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(targetVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(LD2);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Calcium48")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(targetVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(pureCa48);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Calcium40")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(targetVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(pureCa40);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Rubber")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(oringVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(rubberneo);}

    if (worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetMaterial()->GetName().compare("Kryptonite")==0){
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(kryptoVisAtt); 
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetMaterial(Kryptonite);
      worldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetUserLimits( new G4UserLimits(0.0, 0.0, 0.0, DBL_MAX, DBL_MAX) );
    }
    
  }

  //target_logic->SetVisAttributes(targetVisAtt);

  // TODO


  //==========================
  // Output geometry tree
  //==========================

  //G4cout << G4endl << "Element table: " << G4endl << G4endl;
  //G4cout << *(G4Element::GetElementTable()) << G4endl;

  //G4cout << G4endl << "Material table: " << G4endl << G4endl;
  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  G4cout << G4endl << "Geometry tree: " << G4endl << G4endl;
  DumpGeometricalTree(worldVolume);

  ReadGlobalMagneticField();

  } else {

    worldVolume = ConstructDetector();
  }

  G4cout << G4endl << "###### Leaving MollerDetectorConstruction::Construct() " << G4endl << G4endl;


  return worldVolume;
}

void MollerDetectorConstruction::WriteGeometryFile(const G4String& filename)
{
  G4cout<<" Trying to write out geometry file: Needs GDML Support!!"<<G4endl;
  G4bool appendPointerAddress = true;
  // Note: only change to false if all names are unique
  fGDMLParser.Write(filename,mother_phys,appendPointerAddress);
}

void MollerDetectorConstruction::DumpGeometricalTree(G4VPhysicalVolume* aVolume,G4int depth)
{
  for(int isp=0;isp<depth;isp++)
  { G4cout << "  "; }
  G4cout << aVolume->GetName() << "[" << aVolume->GetCopyNo() << "] "
         << aVolume->GetLogicalVolume()->GetName() << " "
         << aVolume->GetLogicalVolume()->GetNoDaughters() << " "
         << aVolume->GetLogicalVolume()->GetMaterial()->GetName() << " "
	 << G4BestUnit(aVolume->GetLogicalVolume()->GetMass(true),"Mass");
  if(aVolume->GetLogicalVolume()->GetSensitiveDetector())
  {
    G4cout << " " << aVolume->GetLogicalVolume()->GetSensitiveDetector()
                            ->GetFullPathName();
  }
  G4cout << G4endl;
  for(int i=0;i<aVolume->GetLogicalVolume()->GetNoDaughters();i++)
  { DumpGeometricalTree(aVolume->GetLogicalVolume()->GetDaughter(i),depth+1); }
}

void MollerDetectorConstruction::setShieldTypeName(const G4String& nam)
{
  shieldType = nam;
}
void MollerDetectorConstruction::setCollimTypeName(const G4String& nam)
{
  collimType = nam;
}
void MollerDetectorConstruction::setscHoleTypeName(const G4String& nam)
{
  scHoleType = nam;
}
void MollerDetectorConstruction::setShieldMaterial(const G4String& nam)
{
  shieldMat = nam;

  if(shieldMat.compare("polyethylene")==0){
    shieldMaterial = polymat;
  } else if(shieldMat.compare("steel")==0){
    G4cout<<G4endl<<G4endl<<"shield material is "<<shieldMat<<G4endl<<G4endl;
    shieldMaterial = SS;
  } else if(shieldMat.compare("iron")==0){
    shieldMaterial = Fe;
  } else if(shieldMat.compare("concrete")==0){
    shieldMaterial = concmat;
  } else if(shieldMat.compare("borpoly")==0){
    shieldMaterial = borpoly;
  } else if(shieldMat.compare("tungsten")==0){
    shieldMaterial = W;
  } else if(shieldMat.compare("hallC")==0){
    G4cout<<"Hall C material"<<G4endl;
    shieldMaterial = borpoly;
    shieldMaterial_1 = polycrete;
    shieldMaterial_2 = borcrete;
    shieldMaterial_3 = Pb;
  } else if(shieldMat.compare("water")==0){
    shieldMaterial = H2O;
  }
}
void MollerDetectorConstruction::setTargetMaterial(const G4String& nam)
{


  targMat = nam;


  if(targMat.compare("LD2")==0){
    targetMaterial = LD2;
    G4cout<<"Setting target material to LD2"<<G4endl;
  } else if(targMat.compare("Pb")==0){
    //targetMaterial = Pb;
    targetMaterial = purePb;
    G4cout<<"Setting target material to Pb"<<G4endl;
  }

}

void MollerDetectorConstruction::DefineMaterials()
{

  //--------------------------------------------------------------//
  //-----------------------------MATERIALS------------------------//
  //--------------------------------------------------------------//


  G4double a;
  G4int z;
  G4int n;
  G4int natoms;
  G4int nIso;
  G4double abundance;
  G4double density;
  G4double temp = 22.0*kelvin;
  //  G4double press = 22.7*psig;
  // G4double press = 2.116*atmosphere;
  G4int nComponents;
  G4String symbol;
  G4double fractionmass;

  G4NistManager* matman = G4NistManager::Instance();



  //-----Liquid Deuterium---//

  //targetMaterial = matman->FindOrBuildMaterial("G4_Pb");
  //targetMaterial = LD2;

  //check for remol pre-shower scint material
  //scintmat = matman->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  collMaterial   = matman->FindOrBuildMaterial("G4_W");
  worldMaterial  = matman->FindOrBuildMaterial("G4_Galactic");
  scattChamberMaterial  = matman->FindOrBuildMaterial("G4_Al");

  
  rubberneo = matman->FindOrBuildMaterial("G4_RUBBER_NEOPRENE");
  polymat = matman->FindOrBuildMaterial("G4_POLYETHYLENE");
  concmat = matman->FindOrBuildMaterial("G4_CONCRETE");
  bormat = matman->FindOrBuildMaterial("G4_B");

  borpoly = new G4Material("Borated_Polyethylene", density= 1.19*g/cm3, nComponents=2);
  borpoly->AddMaterial(bormat, fractionmass=30.*perCent); 
  borpoly->AddMaterial(polymat, fractionmass=70.*perCent); 
  
  borcrete = new G4Material("Borated_Concrete", density= 2.42*g/cm3, nComponents=2);
  borcrete->AddMaterial(bormat, fractionmass=55.*perCent); 
  borcrete->AddMaterial(concmat, fractionmass=45.*perCent); 

  polycrete = new G4Material("Polycrete", density= 3.75*g/cm3, nComponents=2);
  polycrete->AddMaterial(polymat, fractionmass=40.*perCent); 
  polycrete->AddMaterial(concmat, fractionmass=60.*perCent);
  
  //H =  matman->FindOrBuildMaterial("G4_H");  
  Al = matman->FindOrBuildMaterial("G4_Al");  
  Ar = matman->FindOrBuildMaterial("G4_Ar");
  Mn = matman->FindOrBuildMaterial("G4_Mn");
  C  = matman->FindOrBuildMaterial("G4_C");
  Cu  = matman->FindOrBuildMaterial("G4_Cu");
  Cr = matman->FindOrBuildMaterial("G4_Cr");
  Ni  = matman->FindOrBuildMaterial("G4_Ni");
  Fe  = matman->FindOrBuildMaterial("G4_Fe");
  Si  = matman->FindOrBuildMaterial("G4_Si");
  P  = matman->FindOrBuildMaterial("G4_P");
  S  = matman->FindOrBuildMaterial("G4_S");
  W  = matman->FindOrBuildMaterial("G4_W");
  Pb  = matman->FindOrBuildMaterial("G4_Pb");
  H2O = matman->FindOrBuildMaterial("G4_WATER");
  Air = matman->FindOrBuildMaterial("G4_AIR");

  SS = new G4Material("Stainless_Steel", density= 8000*kg/m3, nComponents=8);
  SS->AddMaterial(Mn, fractionmass=2.000*perCent);
  SS->AddMaterial(C,  fractionmass=0.080*perCent);
  SS->AddMaterial(Cr, fractionmass=19.00*perCent);
  SS->AddMaterial(Ni, fractionmass=9.000*perCent);
  SS->AddMaterial(Fe, fractionmass=68.845*perCent);
  SS->AddMaterial(Si, fractionmass=1.000*perCent);
  SS->AddMaterial(P,  fractionmass=0.045*perCent);
  SS->AddMaterial(S,  fractionmass=0.030*perCent);

   // this material will kill every tracks that touch it                                         
  Kryptonite = new G4Material("Kryptonite", density= 0.00000001*mg/cm3, nComponents=1);
  Kryptonite->AddMaterial(Ar, fractionmass=100.*perCent);

  D  = new G4Isotope("Deuteron", z=1, n=2, a= 2.0141018*g/mole);
  elD = new G4Element("Deuterium",symbol="elD", nIso = 1);
  elD->AddIsotope(D, abundance=100.*perCent);
  LD2 = new G4Material("Liquid_Deuterium", density= 0.169*g/cm3, nComponents=1, kStateLiquid, temp);
  LD2->AddElement(elD, natoms=2); //sum of frac. masses 2 is not 1?

  //-----How pure was the Lead???------//
  
  I_Pb  = new G4Isotope("Lead208", z=82, n=208, a= 207.19*g/mole);
  Pb208 = new G4Element("Lead",symbol="Pb208", nIso = 1);
  Pb208->AddIsotope(I_Pb, abundance=100.*perCent);
  purePb = new G4Material("Pure_Lead", density= 11.35*g/cm3, nComponents=1);
  purePb->AddElement(Pb208, natoms=1); 


  I_Ca48  = new G4Isotope("Calcium_48", z=20, n=48, a= 47.95*g/mole);
  Ca48 = new G4Element("Ca_48",symbol="Ca48", nIso = 1);
  Ca48->AddIsotope(I_Ca48, abundance=100.*perCent);
  pureCa48 = new G4Material("Pure_Calcium48", density= 1.84*g/cm3, nComponents=1);
  pureCa48->AddElement(Ca48, natoms=1); 

  I_Ca40  = new G4Isotope("Calcium_40", z=20, n=40, a= 39.96*g/mole);
  Ca40 = new G4Element("Ca_40",symbol="Ca40", nIso = 1);
  Ca40->AddIsotope(I_Ca40, abundance=100.*perCent);
  pureCa40 = new G4Material("Pure_Calcium40", density= 1.55*g/cm3, nComponents=1);
  pureCa40->AddElement(Ca40, natoms=1); 

  CW70 = new G4Material("CW70", density= 14.18*g/cm3, nComponents=2);
  CW70->AddMaterial(Cu,fractionmass=30.*perCent);
  CW70->AddMaterial(W,fractionmass=70.*perCent);

  septumpipeMaterial  = SS;
  septumMaterial  = Fe;
  floorMaterial = concmat; 
  beamlineMaterial  = Al;

 
  // Print materials defined.
  G4cout << G4endl << "==========The Actual materials defined are : =====" << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  G4cout << G4endl << "==========End of materials table==================" << G4endl << G4endl;

}

G4VPhysicalVolume* MollerDetectorConstruction::ConstructDetector()
{
  std::vector<G4VSensitiveDetector*> detector;

  //--------------------------------------------------------
  // Define the dimensions here:

  //Reference everything to target center so that I can eventually increase the diameter of the Hall
  //and place the target "near" the center...
  
  //------------------------------------
  // World dimensions 
  //G4double half_side_mom = 1500.0*cm;
  //G4double half_len_mom = 1500.0*cm;
  //G4double target_center = -1175.0*cm;
  //  G4double half_side_mom = 2651.7*cm;
  //  G4double half_len_mom = 2651.7*cm;
  G4double half_side_mom = 5000.0*cm;
  G4double half_len_mom = 5000.0*cm;
  //  G4double target_center =fTargetCenter;
  G4double target_center =fTargetCenter;
  G4double prex_target_center = -105.3*cm;  //reference everything to this

  if(target_center!=prex_target_center){
    G4cout<<G4endl<<G4endl<<"Target center is "<<fTargetCenter/cm<<" cm, not PREx center!"<<G4endl<<G4endl;
  }
  //------------------------------------
  // Hall dimensions 
  // G4double radius_hall = 2651.7*cm;
  //G4double height_hall = 2651.7*cm;//??
  //G4double thickness_hall = 1000.*cm;

  //------------------------------------
  // Detector dimensions 
  G4double det_thick = 1*cm;
  G4double det_inner_r = half_len_mom+prex_target_center-det_thick;  //as large as it can be...
  G4double det_outer_r = half_len_mom+prex_target_center;
  //G4double det_inner_r = 325*cm-det_thick;  //as large as it can be...
  //G4double det_outer_r = 325*cm;
  
  //------------------------------------
  // Target dimensions 
  G4double inner_rad = 0.0*cm;
  G4double start_angle = 0.0*deg;
  G4double end_angle = 360.0*deg;
  G4double outer_rad_targ = 5.0*cm;
  //G4double half_len_targ = 0.25*cm;
  G4double half_len_targ = fTargetLength/2;
  //G4double half_len_targ = 0.025*cm; // "Normal" 0.5 mm Pb target
  //G4double half_len_targ = 5.0*cm;
  //G4double half_len_targ = 37.5*cm;
  G4cout<<"Target length is "<<fTargetLength/cm<<" cm"<<G4endl;
  //------------------------------------
  // Scattering chamber dimensions 
  // Estimated from drawings
  G4double thick_sc = 5.08*cm;
  G4double outer_r_sc = 57.15*cm;
  G4double inner_r_sc = outer_r_sc-thick_sc;
  G4double half_height_sc = 119.8655*cm;
  G4double y_cent_sc = 53.94*cm;
  G4double deltaz_targ = 2.77*cm;

  //  G4double r_sc_beam_hole=2.5*cm;
  G4double r_sc_beam_hole=4.102*cm;
  G4double l_sc_beam_hole=outer_r_sc;

  //------------------------------------
  // Collimator dimensions 
  G4double coll_1_halfW = 4.161*cm;  G4double coll_1_halfH = 8.414*cm;  G4double coll_1_halfL = 1.207*cm;
  G4double coll_2_halfW = 5.398*cm;  G4double coll_2_halfH = 8.414*cm;  G4double coll_2_halfL = 2.921*cm;

  G4double coll_3_inner_r = 0*cm;  G4double coll_3_outer_r = 3.073*cm; G4double coll_3_halfL = 3.048*cm;

  //  G4double hole_halfL = 2*(coll_1_halfL+coll_2_halfL+1*cm);
  G4double hole_halfL = 1.001*(coll_1_halfL+coll_2_halfL+coll_3_halfL);

  //Reference is to center of "first" volume in the boolean operation
  G4double coll_us_face = 85.822*cm;
  G4double coll_ds_face = coll_us_face+2*(coll_1_halfL+coll_2_halfL+coll_3_halfL);
  G4double coll_center = prex_target_center+85.822*cm+coll_1_halfL;

  G4double coll_inner_r1=fcollInnerR;
  G4double coll_inner_r2=fcollInnerR;

  G4double coll_angle=atan(fcollInnerR/coll_ds_face);
  G4cout<<"Initializing collimator variables... "<<G4endl;

  if (collimType.compare("cone")==0) {
    //radii won't be quite right because hole_halfL = 1.001*(coll_1_halfL+coll_2_halfL+coll_3_halfL);
    //add to the us and ds faces

    G4cout<<"Using conical collimator, angle = "<<coll_angle/deg<<" degrees!"<<G4endl;
    G4cout<<"At ds face, inner radius = "<<fcollInnerR/cm<<G4endl;

    coll_inner_r1=(coll_us_face-0.001*(coll_1_halfL+coll_2_halfL+coll_3_halfL))*tan(coll_angle);
    coll_inner_r2=(coll_ds_face+0.001*(coll_1_halfL+coll_2_halfL+coll_3_halfL))*tan(coll_angle);

    G4cout<<"us face = "<<coll_us_face/cm<<" , ds face = "<<coll_ds_face/cm<<G4endl;
    G4cout<<"us radius = "<<coll_inner_r1/cm<<" , ds radius = "<<coll_inner_r2/cm<<G4endl;

  }


  //  G4cout<<"Downstream end of collimator at "<<coll_center/cm+<<G4endl; 

  //------------------------------------
  // Septum dimensions 

  G4double sept_halfW=68.58*cm; G4double sept_halfH=38.1*cm; G4double sept_halfL=37.193*cm; 
  G4double sept_center=175.211*cm+prex_target_center; // Seems about right; some space for coils...
  
  G4double rect_hole_center_x = 19.685*cm;  G4double sept_rect_hole_halfW = 15.5575*cm; G4double sept_rect_hole_halfH = 12.5413*cm;


  //------------------------------------
  // Shield hut dimensions 
  G4double hut_halfW = sept_halfW+fShieldThick; G4double hut_halfH = sept_halfH+fShieldThick; G4double hut_halfL = sept_halfL+(sept_center-sept_halfL-(prex_target_center+outer_r_sc-deltaz_targ))/2.;
  G4double hut_hole_halfW = sept_halfW ; G4double hut_hole_halfH = sept_halfH; G4double hut_hole_halfL = hut_halfL-fShieldThick/2;

  G4double hut_center = prex_target_center+outer_r_sc-deltaz_targ+hut_halfL;
  G4double hut_hole_center = prex_target_center+outer_r_sc-deltaz_targ+hut_halfL+fShieldThick/2.;

  //------------------------------------
  // Beamline dimensions 
  //  G4double ri_septum_pipe = 3.0*cm;
  //  G4double ro_septum_pipe = 3.152*cm;
  //  G4double hl_septum_pipe = 53.023*cm;

  // If target center is not -105 cm then no septum pipe? Still use location of center to define rest
  // of beamline...
  G4double ri_septum_pipe = 3.797*cm;
  G4double ro_septum_pipe = 4.102*cm;

  //For now, add bellows to length of septum pipe
  G4double hl_septum_pipe = 68.681*cm;

  // Referenced to the position of the collimator...
 
  G4double z0_septum_pipe = coll_center+coll_1_halfL+2*(coll_2_halfL+coll_3_halfL)+hl_septum_pipe;

  G4double ri_bl[13] = { 4.128*cm, 4.445*cm, 4.763*cm,  5.080*cm, 6.350*cm,   7.620*cm, 10.160*cm, 10.478*cm, 12.700*cm, 15.240*cm,  30.480*cm,  45.720*cm,  45.720*cm};
  //  G4double ro_bl[12] = { 4.280*cm, 4.597*cm, 4.915*cm,  5.232*cm, 6.502*cm,   7.772*cm, 10.312*cm, 10.636*cm, 12.859*cm, 15.399*cm,  30.639*cm,  45.879*cm};
  //G4double hl_bl[12] = {14.009*cm, 9.505*cm, 7.620*cm, 18.514*cm, 11.708*cm, 13.811*cm,  4.724*cm, 47.228*cm, 15.240*cm, 53.102*cm, 275.590*cm, 274.796*cm};
  G4double ro_bl[13] = { 4.432*cm, 4.750*cm, 5.067*cm,  5.385*cm, 6.655*cm,   7.925*cm, 10.478*cm, 10.795*cm, 13.018*cm, 15.558*cm,  30.798*cm,  46.038*cm,  46.038*cm};
  G4double hl_bl[13] = {14.009*cm, 9.505*cm, 7.620*cm, 18.514*cm, 11.708*cm, 13.811*cm, 11.184*cm, 47.228*cm, 15.240*cm, 53.102*cm, 275.590*cm, 782.321*cm, 277.388*cm};

  G4double z0_bl[13];

  G4int max_pipes = 12;

  if (fDrawHall) max_pipes = 13;

  for (G4int i = 0; i<max_pipes;i++){
    if (i==0){
      z0_bl[i]=z0_septum_pipe+hl_septum_pipe+hl_bl[i];
      G4cout<<z0_septum_pipe/cm<<G4endl;
    } else{
      z0_bl[i]=z0_bl[i-1]+hl_bl[i-1]+hl_bl[i];
    }
    G4cout<<z0_bl[i]/cm<<G4endl;
  }

  //------------------------------------
  //Vacuum enclosures
  G4double vac_u_enc_halfW = 1.8301*cm; G4double vac_u_enc_halfH = 5.3289*cm; 
  G4double vac_d_enc_halfW = 5.2413*cm; G4double vac_d_enc_halfH = 11.8250*cm; 
  G4double vac_enc_halfL = 65.2844*cm; G4double vac_enc_z_center = z0_septum_pipe-hl_septum_pipe+vac_enc_halfL;
  G4double vac_enc_yu_center = 0.*cm; 
  G4double vac_enc_thick = 0.635*cm; 

  G4double left_theta = -atan((vac_d_enc_halfW-vac_u_enc_halfW+3.6195*cm)/vac_enc_halfL);
  G4double left_phi = 0;
  G4double right_theta = -left_theta; //?
  G4double right_phi = left_phi;     //?

  G4double vac_enc_xu_center = ro_septum_pipe+vac_u_enc_halfW-vac_enc_halfL*tan(left_theta);

  // Angle wrt y-axis; =0 bc these are actually rectangles??? regardless, alpha_1 has to be equal to alpha_2
  G4double alpha_l_1 = 0.;
  G4double alpha_r_1 = -alpha_l_1;    //?
  G4double alpha_l_2 = alpha_l_1;     //?
  G4double alpha_r_2 = alpha_r_1;     //?

  //------------------------------------
  // Scattering Chamber attachments
  G4double scatt_attach_thick = 2.54*cm ;
  // Need to double check W and H - estimate from enterprise
  // Need to subtract from prex shielding
  G4double scatt_attach_halfW = 22.4055*cm; G4double scatt_attach_halfH = 12.9632*cm; 
  //G4double scatt_attach_halfL = (vac_enc_z_center-vac_enc_halfL-(prex_target_center+outer_r_sc-deltaz_targ))/2; 
  G4double scatt_attach_halfL = (prex_target_center+coll_ds_face-prex_target_center-inner_r_sc+deltaz_targ)/2;
  G4double scatt_attach_center = prex_target_center+coll_ds_face-scatt_attach_halfL;


  //------------------------------------
  // Wall dimensions 
  G4double radius_of_hall = 2651.7*cm;
  G4double wall_halfL = 77.388*cm; G4double wall_halfW = 1000.0*cm; G4double wall_halfH = 360.*cm;
  G4double wall_center = radius_of_hall+wall_halfL;

  //------------------------------------
  // Floor dimensions 
  // Just a box for now; should be a cylindrical plate...
  
  G4double floor_halfL = (wall_center+wall_halfL-(prex_target_center+deltaz_targ-outer_r_sc))/2.; G4double floor_halfW = 1000.0*cm; G4double floor_halfH = 25.0*cm;
  G4double floor_z_center = floor_halfL+(prex_target_center+deltaz_targ-outer_r_sc);
  G4double floor_y_center = -wall_halfH-floor_halfH;

  //------------------------------------
  // Beam dump dimensions 
  // Just a box for now; should be part of a cylindrical wall...
  
  G4double dump_halfL = 200.*cm; G4double dump_halfW = 300.0*cm; G4double dump_halfH = wall_halfH;
  G4double dump_center = radius_of_hall+dump_halfL+2*wall_halfL;

  //  G4double dump_hole_halfL = 161.29*cm-wall_halfL; G4double dump_hole_halfW = 106.408*cm; G4double dump_hole_halfH = 149.938*cm;
  G4double dump_hole_halfL = dump_halfL; G4double dump_hole_halfW = 106.408*cm; G4double dump_hole_halfH = 149.938*cm;
  G4double dump_hole_z_center = dump_center-(dump_halfL-dump_hole_halfL);
  G4double dump_hole_y_center = -43.531*cm;

  G4double dump_ss_enc_halfL = 5.73*cm;  
  //G4double dump_ss_enc_center = dump_center-dump_halfL-2*wall_halfL+60.96*cm;  
  G4double dump_ss_enc_center = dump_center-dump_halfL-2*wall_halfL+5.*cm;  
  G4double dump_ss_enc_hole_radius = ro_bl[11];  

 
  //------ Shielding -----//

  //G4double thick_sc = 9.86*cm;
  G4double inner_r_sc_shld = outer_r_sc;
  G4double outer_r_sc_shld = inner_r_sc_shld+fShieldThick;
  //    G4double half_height_sc = 119.87*cm;
  //    G4double y_cent_sc = 53.94*cm;
  //    G4double deltaz_targ = 2.77*cm;

  G4double box_halfW=69.596*cm; G4double box_halfH=50.8*cm; G4double box_halfL=16.1925*cm; 
  G4double box_center=64.5025*cm+prex_target_center; G4double box_r_1=7.6*cm; G4double box_r_2=10.7*cm; 

  G4double plus_1_halfW=13.2969*cm; G4double plus_1_halfH=50.8*cm; G4double plus_halfL=14.7955*cm;
  G4double plus_2_halfW=69.596*cm; G4double plus_2_halfH=34.8234*cm;
  G4double plus_center=95.4905*cm+prex_target_center; G4double plus_r_1=10.7*cm; G4double plus_r_2=14*cm; 

  //--------------------------------------------------------
  // Define the volumes here:

 
  //------World-----//

  G4Box* mother_solid = new G4Box("boxMother", half_side_mom, half_side_mom, half_len_mom);
  mother_logic = new G4LogicalVolume(mother_solid, worldMaterial, "logicMother", 0);
  mother_phys = new G4PVPlacement(0,G4ThreeVector(), mother_logic, "100", 0, false, 0);

  G4Box* hut_sc_attach_hole = new G4Box("hutScattAttach_hole", 1.0001*scatt_attach_halfW, 1.0001*scatt_attach_halfH, 1.001*hut_halfL);
  G4Box* sc_attach_hole = new G4Box("ScattAttach_hole", 1.0001*24.638/2*cm, 1.0001*17.018/2*cm, 1.001*outer_r_sc);

  if(shieldType.compare("test")!=0){
    //------Target-----//

    G4Tubs* target_solid = new G4Tubs("tubeTarget", inner_rad, outer_rad_targ, half_len_targ, start_angle, end_angle);
    target_logic = new G4LogicalVolume(target_solid, targetMaterial, "logicTarget");
    target_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, target_center), target_logic, "101", mother_logic , false, 0);

    G4cout<<"Target material is ";
    G4cout<<targMat<<G4endl;

    //  if(targMat.compare("LD2")==0){
    if(fDrawCollim) {
      //------ W Collimator -----//
      // Two boxes with cylinder through them and cylinder at downstream end
      G4Box* coll_1 = new G4Box("Wcoll1", coll_1_halfW, coll_1_halfH, coll_1_halfL);
      G4Box* coll_2 = new G4Box("Wcoll2", coll_2_halfW, coll_2_halfH, coll_2_halfL);
      G4Tubs* coll_3 = new G4Tubs("Wcoll3", coll_3_inner_r, coll_3_outer_r, coll_3_halfL, start_angle, end_angle);
      
      G4Cons* coll_hole = new G4Cons("Wcoll_hole", 0*cm, coll_inner_r1, 0*cm, coll_inner_r2, hole_halfL, start_angle, end_angle);
            
      G4ThreeVector zTrans_1(0, 0, coll_1_halfL+coll_2_halfL);
      G4UnionSolid* partial_coll_0 = new G4UnionSolid("pColl0", coll_1, coll_2, 0, zTrans_1);
      
      G4ThreeVector zTrans_2(0, 0, coll_1_halfL+2*coll_2_halfL+coll_3_halfL);
      G4UnionSolid* coll_solid_0 = new G4UnionSolid("WColl_solid_0", partial_coll_0, coll_3, 0, zTrans_2);

      //Centered on collimator "1" not the collimator as a whole...
      //Need to translate to center of whole collimator from center of "1"

      G4ThreeVector zTrans_3(0, 0, coll_2_halfL+coll_3_halfL);
      G4SubtractionSolid* coll_solid = new G4SubtractionSolid("collWSolid", coll_solid_0, coll_hole, 0, zTrans_3);
      
      W_coll_logic = new G4LogicalVolume(coll_solid, collMaterial, "logicWColl", 0);
      W_coll_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, coll_center), W_coll_logic, "103", mother_logic, false, 0);   
   
    } else {
    }
    if(fDrawWholeGeo) {

      //------ Scattering chamber -----//
      // vertical aluminum cylinder with a hole in the side.

      G4Tubs* scatt_cham_main = new G4Tubs("outScattChamber", 0, outer_r_sc, half_height_sc, start_angle, end_angle);
      G4Tubs* scatt_cham_inner = new G4Tubs("innScattChamber", 0, inner_r_sc, half_height_sc-thick_sc, start_angle, end_angle);

      G4Tubs* scatt_cham_hole = new G4Tubs("holeScattChamber", 0, r_sc_beam_hole, l_sc_beam_hole, start_angle, end_angle);
      G4Tubs* scatt_cham_hole2 = new G4Tubs("holeScattChamber2", 0, r_sc_beam_hole, l_sc_beam_hole, start_angle, end_angle);
      
    
      // Need to rotate main, add top, bottom and subtract hole...
      //  G4ThreeVector yTrans_3(0, 0,-(half_height_sc-y_cent_sc));
      G4ThreeVector yTrans_3(0, 0,-y_cent_sc);
      G4ThreeVector scTrans_1(0,-(outer_r_sc-thick_sc/2),-y_cent_sc);
      G4ThreeVector scTrans_2(0,(outer_r_sc-thick_sc/2),-y_cent_sc);

      G4RotationMatrix* xRot = new G4RotationMatrix;  // Rotates Z and Y axes only
      xRot->rotateX(90.*deg);

      G4SubtractionSolid* scatt_w_ends = new G4SubtractionSolid("top+bott", scatt_cham_main, scatt_cham_inner);

      if (scHoleType.compare("round")==0) {
	G4SubtractionSolid* scatt_cham_solid2 = new G4SubtractionSolid("scatt_w_hole2", scatt_w_ends, scatt_cham_hole2, xRot, scTrans_2);
	G4SubtractionSolid* scatt_cham_solid = new G4SubtractionSolid("scatt_w_hole", scatt_cham_solid2, scatt_cham_hole, xRot, scTrans_1);
	scatt_cham_logic = new G4LogicalVolume(scatt_cham_solid, scattChamberMaterial, "logicScattChamber");
	scatt_cham_phys = new G4PVPlacement(xRot,G4ThreeVector(0, y_cent_sc, target_center-deltaz_targ), scatt_cham_logic, "102", mother_logic , false, 0);
      } 

      else if (scHoleType.compare("attachment")==0) { 
	G4SubtractionSolid* scatt_cham_solid2 = new G4SubtractionSolid("scatt_w_hole2", scatt_w_ends, scatt_cham_hole2, xRot, scTrans_2);
	G4SubtractionSolid* scatt_cham_solid = new G4SubtractionSolid("scatt_w_hole", scatt_cham_solid2, sc_attach_hole, xRot, scTrans_1);
	scatt_cham_logic = new G4LogicalVolume(scatt_cham_solid, scattChamberMaterial, "logicScattChamber");
	scatt_cham_phys = new G4PVPlacement(xRot,G4ThreeVector(0, y_cent_sc, target_center-deltaz_targ), scatt_cham_logic, "102", mother_logic , false, 0);
      }
  

      //------ Septum -----//

      // Box of iron with boxes of either air or poly and the vacuum enclosures (cones?)
      if(fDrawSeptum) {

	G4Box* sept_rect_hole = new G4Box("boxSeptum", sept_rect_hole_halfW, sept_rect_hole_halfH, 1.001*sept_halfL);
	G4Tubs* sept_beam_hole = new G4Tubs("bl_name", 0, 1.001*ro_septum_pipe, 1.001*sept_halfL, start_angle, end_angle);

	G4Box* septum_solid_0 = new G4Box("boxSeptum", sept_halfW, sept_halfH, sept_halfL);

	G4ThreeVector xTrans_l(-rect_hole_center_x, 0, 0);
	G4ThreeVector xTrans_r(rect_hole_center_x, 0, 0);

	G4SubtractionSolid* septum_solid_1  = new G4SubtractionSolid("sept_w_hole1", septum_solid_0, sept_rect_hole, 0, xTrans_l);
	G4SubtractionSolid* septum_solid_2  = new G4SubtractionSolid("sept_w_hole2", septum_solid_1, sept_rect_hole, 0, xTrans_r);
	G4SubtractionSolid* septum_solid  = new G4SubtractionSolid("sept_w_holes", septum_solid_2, sept_beam_hole);

	septum_logic = new G4LogicalVolume(septum_solid, septumMaterial, "logicSeptum", 0);
	septum_phys = new G4PVPlacement(0,G4ThreeVector(0,0,sept_center), septum_logic, "104", mother_logic, false, 0);

	//Draw scattering chamber attachment here?  Also subtract from PREx shielding if septum is drawn?
	G4Box* scatt_attach_o = new G4Box("boxScattAttach_o", scatt_attach_halfW, scatt_attach_halfH, scatt_attach_halfL);
	G4Box* scatt_attach_i = new G4Box("boxScattAttach_i", scatt_attach_halfW-scatt_attach_thick, scatt_attach_halfH-scatt_attach_thick, scatt_attach_halfL-scatt_attach_thick);
	G4RotationMatrix* noRot = new G4RotationMatrix;  // Rotates Z and Y axes only
	G4ThreeVector scAttach_translatein(0,0,-scatt_attach_thick);

	G4SubtractionSolid* scatt_attach2  = new G4SubtractionSolid("scattAttach2", scatt_attach_o, scatt_attach_i,noRot,scAttach_translatein);
	G4RotationMatrix* xRot = new G4RotationMatrix;  // Rotates Z and Y axes only
	xRot->rotateX(90.*deg);
	G4ThreeVector scAttach_translate2(0,0,-outer_r_sc-scatt_attach_halfL);
	G4SubtractionSolid* scatt_attach  = new G4SubtractionSolid("scattAttach", scatt_attach2, scatt_cham_main,xRot,scAttach_translate2);
	// commented by Lorenzo in order to compare to gdml geometries. It is difficult, without restiling the hardoced geometry to build the scattering chamber like the one in the gdml file.
	// scatt_attach_logic = new G4LogicalVolume(scatt_attach, beamlineMaterial, "logicScattAttach", 0);
	// scatt_attach_phys = new G4PVPlacement(0,G4ThreeVector(0,0,scatt_attach_center), scatt_attach_logic, "105", mother_logic, false, 0);
	
      }


      //------ Beam Pipe -----//
      // Stainless steel/ Al cylinders?

      char bl_name[200]; 
      char bl_num[200]; 
      G4int bnum=150;
      sprintf(bl_num,"%i",bnum);

      if(targMat.compare("LD2")==0){
      } else {
	G4Tubs* septum_pipe = new G4Tubs("bl_name", ri_septum_pipe, ro_septum_pipe, hl_septum_pipe, start_angle, end_angle);
	septum_pipe_logic = new G4LogicalVolume(septum_pipe, septumpipeMaterial, "logicSeptum", 0);
	septum_pipe_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, z0_septum_pipe), septum_pipe_logic, bl_num, mother_logic, false, 0);
	bnum++;

	G4Trap* vacuum_enclosure_lo = new G4Trap("vacEnclosureLo", vac_enc_halfL, left_theta, left_phi,  
						 vac_u_enc_halfH, vac_u_enc_halfW, vac_u_enc_halfW, alpha_l_1,
						 vac_d_enc_halfH, vac_d_enc_halfW, vac_d_enc_halfW, alpha_l_2);

	G4Trap* vacuum_enclosure_ro = new G4Trap("vacEnclosureRo", vac_enc_halfL, right_theta, right_phi,
						 vac_u_enc_halfH, vac_u_enc_halfW, vac_u_enc_halfW, alpha_r_1,
						 vac_d_enc_halfH, vac_d_enc_halfW, vac_d_enc_halfW, alpha_r_2);

	G4Trap* vacuum_enclosure_li = new G4Trap("vacEnclosureLi", 1.001*vac_enc_halfL, left_theta, left_phi,
						 vac_u_enc_halfH-vac_enc_thick, vac_u_enc_halfW-vac_enc_thick, vac_u_enc_halfW-vac_enc_thick, alpha_l_1,
						 vac_d_enc_halfH-vac_enc_thick, vac_d_enc_halfW-vac_enc_thick, vac_d_enc_halfW-vac_enc_thick, alpha_l_2);

	G4Trap* vacuum_enclosure_ri = new G4Trap("vacEnclosureRi",  1.001*vac_enc_halfL, right_theta, right_phi,
						 vac_u_enc_halfH-vac_enc_thick, vac_u_enc_halfW-vac_enc_thick, vac_u_enc_halfW-vac_enc_thick, alpha_r_1,
						 vac_d_enc_halfH-vac_enc_thick, vac_d_enc_halfW-vac_enc_thick, vac_d_enc_halfW-vac_enc_thick, alpha_r_2);
  
	G4SubtractionSolid* vacuum_enclosure_l = new G4SubtractionSolid("vacEnclosureL",vacuum_enclosure_lo,vacuum_enclosure_li);
	G4SubtractionSolid* vacuum_enclosure_r = new G4SubtractionSolid("vacEnclosureL",vacuum_enclosure_ro,vacuum_enclosure_ri);

	sprintf(bl_num,"%i",bnum);
	vacuum_enclosure_l_logic = new G4LogicalVolume(vacuum_enclosure_l, septumpipeMaterial, "logicVacEncL", 0);
	vacuum_enclosure_l_phys = new G4PVPlacement(0,G4ThreeVector(-vac_enc_xu_center,vac_enc_yu_center,vac_enc_z_center), vacuum_enclosure_l_logic, bl_num, mother_logic, false, 0);
	bnum++;

	sprintf(bl_num,"%i",bnum);
	vacuum_enclosure_r_logic = new G4LogicalVolume(vacuum_enclosure_r, septumpipeMaterial, "logicVacEncR", 0);
	vacuum_enclosure_r_phys = new G4PVPlacement(0,G4ThreeVector(vac_enc_xu_center,vac_enc_yu_center,vac_enc_z_center), vacuum_enclosure_r_logic, bl_num, mother_logic, false, 0);
	bnum++;

      }
      G4Tubs* beam_pipe[13];
      for (int i = 0; i<max_pipes;i++){
	sprintf(bl_name,"beampipe_%i",i);
	sprintf(bl_num,"%i",bnum);
	beam_pipe[i] = new G4Tubs("bl_name", ri_bl[i], ro_bl[i], hl_bl[i], start_angle, end_angle);
	bl_logic[i] = new G4LogicalVolume(beam_pipe[i], beamlineMaterial, "logicBeam", 0);
	bl_phys[i] = new G4PVPlacement(0,G4ThreeVector(0, 0, z0_bl[i]), bl_logic[i], bl_num, mother_logic, false, 0);
	bnum++;
      }


      if (fDrawHall){

	//------ Wall -----//
  
	G4Box* wall_0_solid = new G4Box("boxWall", wall_halfW, wall_halfH, wall_halfL);
	G4Box* wall_hole = new G4Box("boxWallHole", dump_hole_halfW, dump_hole_halfH, 1.001*wall_halfL);

	G4ThreeVector TransWallHole(0, dump_hole_y_center,0.);
	G4SubtractionSolid* wall_solid = new G4SubtractionSolid("dumpTunnelWall", wall_0_solid, wall_hole, 0, TransWallHole);

	wall_logic = new G4LogicalVolume(wall_solid, floorMaterial, "logicWall", 0);
	wall_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, wall_center), wall_logic, "106", mother_logic, false, 0);
  
	//------ Floor -----//
	// should be cylindrical plate covering whole geometry...

	G4Box* floor_solid = new G4Box("boxFloor", floor_halfW, floor_halfH, floor_halfL);
	floor_logic = new G4LogicalVolume(floor_solid, floorMaterial, "logicFloor", 0);
	floor_phys = new G4PVPlacement(0,G4ThreeVector(0, floor_y_center, floor_z_center), floor_logic, "107", mother_logic, false, 0);

	//------ Beam Dump -----//
	// solid box of lead with cylinder partially sunk into it?
  
	G4Box* dump_0_solid = new G4Box("boxDump", dump_halfW, dump_halfH, dump_halfL);
	G4Box* dump_hole = new G4Box("boxDumpHole", dump_hole_halfW, dump_hole_halfH, dump_hole_halfL);

	G4ThreeVector TransDumpHole(0, dump_hole_y_center, dump_hole_z_center-dump_center);
	G4SubtractionSolid* dump_solid = new G4SubtractionSolid("dumpTunnel", dump_0_solid, dump_hole, 0, TransDumpHole);

	dump_logic = new G4LogicalVolume(dump_solid, floorMaterial, "logicDump", 0);
	dump_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, dump_center), dump_logic, "108", mother_logic, false, 0);


	G4Box* dump_ss_0_enc = new G4Box("boxDumpSSEnc", dump_hole_halfW, dump_hole_halfH, dump_ss_enc_halfL);
	G4Cons* dump_ss_enc_hole = new G4Cons("boxDumpHole", 0*cm, dump_ss_enc_hole_radius, 0*cm, dump_ss_enc_hole_radius, 1.001*dump_ss_enc_halfL, 0.*deg, 360.*deg);

	G4ThreeVector TransDumpSSEncHole(0, -dump_hole_y_center, 0);
	G4SubtractionSolid* dump_ss_enc = new G4SubtractionSolid("dumpSSEnc", dump_ss_0_enc, dump_ss_enc_hole, 0, TransDumpSSEncHole);

	ssenc_logic = new G4LogicalVolume(dump_ss_enc, septumpipeMaterial, "logicSSEnc", 0);
	ssenc_phys = new G4PVPlacement(0,G4ThreeVector(0, dump_hole_y_center, dump_ss_enc_center), ssenc_logic, "109", mother_logic, false, 0);
      }

    } else {
      //Don't draw anything but target, collimators and detector (shielding)?
    }

    //------ Shielding -----//
    if(shieldType.compare("none")==0) {

    } else if (shieldType.compare("hut")==0){
      G4cout<<G4endl<<"Beginning shield hut def'n"<<G4endl;

      G4Box* hut_a_solid = new G4Box("boxHut", hut_halfW, hut_halfH, hut_halfL);
      G4Box* hut_hole = new G4Box("boxHutHole", hut_hole_halfW, hut_hole_halfH, hut_hole_halfL);
      //G4Box* hut_sc_attach_hole = new G4Box("hutScattAttach_hole", 1.0001*scatt_attach_halfW, 1.0001*scatt_attach_halfH, 1.0001*hut_halfL);
    
      G4ThreeVector TransHutHole(0, 0, hut_hole_center-hut_center);
      G4SubtractionSolid* hut_b_solid = new G4SubtractionSolid("shldHut", hut_a_solid, hut_hole, 0, TransHutHole);
      G4SubtractionSolid* hut_solid = new G4SubtractionSolid("shldHut", hut_b_solid, hut_sc_attach_hole);
    
      if ((shldConfig==0)||(shldConfig==3)) { // hut with septum shld
	hut_logic = new G4LogicalVolume(hut_solid, shieldMaterial, "logicHut", 0);
	hut_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, hut_center), hut_logic, "110", mother_logic, false, 0);
      }
      if ((shldConfig==3)||(shldConfig==4)||(shldConfig==5)) { // hut with septum shld

	if(fDrawSeptum) {
	  G4cout<<"Drawing septum shield...";

	  G4Box* sept_shld_l_0 = new G4Box("septShld_l_0", 0.9999*sept_rect_hole_halfW, 0.9999*sept_rect_hole_halfH, sept_halfL);
	  G4Trap* hole_for_l_vac_enc = new G4Trap("vacEncHoleL", 1.001*vac_enc_halfL, left_theta, left_phi,  
						  1.0001*vac_u_enc_halfH, 1.0001*vac_u_enc_halfW, 1.0001*vac_u_enc_halfW, alpha_l_1,
						  1.0001*vac_d_enc_halfH, 1.0001*vac_d_enc_halfW, 1.0001*vac_d_enc_halfW, alpha_l_2);
	  G4ThreeVector Trans_septl(rect_hole_center_x-vac_enc_xu_center, 0, vac_enc_z_center-sept_center);
	  G4cout<<rect_hole_center_x<<" - "<<vac_enc_xu_center<<" = "<<rect_hole_center_x-vac_enc_xu_center<<G4endl;
	  G4cout<<vac_enc_z_center<<" - "<<sept_center<<" = "<<vac_enc_z_center-sept_center<<G4endl;
	  G4SubtractionSolid* sept_shld_l = new G4SubtractionSolid("septShld_l",sept_shld_l_0,hole_for_l_vac_enc, 0, Trans_septl);
      
	  G4Box* sept_shld_r_0 = new G4Box("septShld_r_0", 0.999*sept_rect_hole_halfW, 0.999*sept_rect_hole_halfH, sept_halfL);
	  G4Trap* hole_for_r_vac_enc = new G4Trap("vacEncHoleR", 1.0001*vac_enc_halfL, right_theta, right_phi,  
						  1.0001*vac_u_enc_halfH, 1.0001*vac_u_enc_halfW, 1.0001*vac_u_enc_halfW, alpha_l_1,
						  1.0001*vac_d_enc_halfH, 1.0001*vac_d_enc_halfW, 1.0001*vac_d_enc_halfW, alpha_l_2);
      
	  G4ThreeVector Trans_septr(-(rect_hole_center_x-vac_enc_xu_center), 0, vac_enc_z_center-sept_center);
	  G4SubtractionSolid* sept_shld_r = new G4SubtractionSolid("septShld_r",sept_shld_r_0,hole_for_r_vac_enc, 0, Trans_septr);
      
	  sept_shld_l_logic = new G4LogicalVolume(sept_shld_l, shieldMaterial, "logicSeptShldL", 0);
	  sept_shld_l_phys = new G4PVPlacement(0,G4ThreeVector(-rect_hole_center_x,0,sept_center), sept_shld_l_logic, "6", mother_logic, false, 0);
      
	  sept_shld_r_logic = new G4LogicalVolume(sept_shld_r, shieldMaterial, "logicSeptShldR", 0);
	  sept_shld_r_phys = new G4PVPlacement(0,G4ThreeVector(rect_hole_center_x,0,sept_center), sept_shld_r_logic, "7", mother_logic, false, 0);

	} else {
	  G4cerr<<"Shouldn't draw septum shield without septum!"<<G4endl;
	}

      }
      if ((shldConfig==4)||(shldConfig==5)) { // hut with septum shld and sc shld
	G4cout<<"Merging with sc shield...";

	outer_r_sc_shld = inner_r_sc_shld + 20.*cm;

	G4Tubs* scatt_cham_shld = new G4Tubs("outScattChamberShld", inner_r_sc_shld, outer_r_sc_shld, half_height_sc, start_angle, end_angle);
	//G4Box* scatt_attach_hole = new G4Box("boxScattAttach_hole", 1.0001*scatt_attach_halfW, 1.0001*scatt_attach_halfH, 1.0001*scatt_attach_halfL);
	G4Box* scatt_attach_hole = new G4Box("boxScattAttach_hole", 1.0001*scatt_attach_halfW, 1.0001*scatt_attach_halfH, 1.0001*outer_r_sc_shld);
	G4Tubs* scatt_cham_shld_hole = new G4Tubs("holeScattChamberShld", 0, box_r_2, 1.0001*2*outer_r_sc_shld, start_angle, end_angle);
    
	G4ThreeVector zTrans_scshldhole(0, 0, scatt_attach_center-(prex_target_center-deltaz_targ));

	G4ThreeVector yTrans_scshldhole(0, 0, y_cent_sc);
	//G4ThreeVector yTrans_scshld(0, scatt_attach_center-(prex_target_center-deltaz_targ), y_cent_sc);
	G4ThreeVector yTrans_scshld(0, 0, y_cent_sc);
	G4ThreeVector zTrans_scshld(0, y_cent_sc, prex_target_center-deltaz_targ-hut_center);

	G4RotationMatrix* xRotscshld = new G4RotationMatrix;  // Rotates Z and Y axes only
	xRotscshld->rotateX(90.*deg);
	G4RotationMatrix* xRotscshldneg = new G4RotationMatrix;  // Rotates Z and Y axes only
	xRotscshldneg->rotateX(-90.*deg);


	G4UnionSolid* scatt_cham_shld_hole_0 = new G4UnionSolid("scatt_hole", scatt_cham_shld_hole, scatt_attach_hole, 0, zTrans_scshldhole);
	G4SubtractionSolid* scatt_cham_shld_solid = new G4SubtractionSolid("scatt_w_hole", scatt_cham_shld, scatt_cham_shld_hole_0, xRotscshld, yTrans_scshld);

	G4UnionSolid* hut_0_solid  = new G4UnionSolid("shldHut_0", hut_solid, scatt_cham_shld_solid, xRotscshldneg, zTrans_scshld);


	if  (shldConfig==4){

	  hut_logic = new G4LogicalVolume(hut_0_solid, shieldMaterial, "logicHut", 0);
	  hut_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, hut_center), hut_logic, "110", mother_logic, false, 0);

	} else if (shldConfig==5) { // hut with septum shld and sc shld and plus shld
	  G4cout<<" Merging with plus shield...";

	  //G4Box* plusShld_1 = new G4Box("plusShld_1", plus_1_halfW,plus_1_halfH,plus_halfL);
	  //G4Box* plusShld_2 = new G4Box("plusShld_2", plus_2_halfW,plus_2_halfH,plus_halfL);
	  G4Box* plusShld_1 = new G4Box("plusShld_1", plus_1_halfW,sept_halfH,plus_halfL);
	  G4Box* plusShld_2 = new G4Box("plusShld_2", sept_halfW,plus_2_halfH,plus_halfL);
	
	  G4Cons* plusShld_hole = new G4Cons("plusShld_hole", 0*cm, plus_r_1,0*cm, plus_r_2, 1.01*plus_halfL, 0.*deg, 360.*deg);
	
	  G4UnionSolid* plusShld = new G4UnionSolid("plusShld",plusShld_1,plusShld_2);
	  G4SubtractionSolid* plus_shield = new G4SubtractionSolid("plus_shield_w_hole",plusShld,plusShld_hole);
	
	  G4ThreeVector Trans_plusshld(0, 0, hut_center-plus_center);
	
	  plusshld_logic = new G4LogicalVolume(plus_shield, shieldMaterial, "plus_logic_shld");
	  plusshld_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, plus_center), plusshld_logic, "2",mother_logic, false, 0);
	  hut_logic = new G4LogicalVolume(hut_0_solid, shieldMaterial, "logicHut1", 0);
	
	  //G4UnionSolid* hut_1_solid  = new G4UnionSolid("shldHut_1", hut_0_solid, plus_shield, 0, Trans_plusshld);
	  //hut_logic = new G4LogicalVolume(hut_1_solid, shieldMaterial, "logicHut1", 0);
	  hut_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, hut_center), hut_logic, "110", mother_logic, false, 0);
	
	}
      }
      G4cout<<" Done with shield hut def'n"<<G4endl<<G4endl;

    } else if ((shieldType.compare("prex")==0)||(shieldType.compare("prex_sept")==0)){

      G4Box* boxShld = new G4Box("boxShld", box_halfW,box_halfH,box_halfL);
      G4Cons* boxShld_hole = new G4Cons("boxShld_hole", 0*cm, box_r_1,0*cm, box_r_2, 1.01*box_halfL, 0.*deg, 360.*deg);
      G4SubtractionSolid* box_shield_0 = new G4SubtractionSolid("shieldBox",boxShld,boxShld_hole);
 
      G4Box* plusShld_1 = new G4Box("plusShld_1", plus_1_halfW,plus_1_halfH,plus_halfL);
      G4Box* plusShld_2 = new G4Box("plusShld_2", plus_2_halfW,plus_2_halfH,plus_halfL);

      G4Cons* plusShld_hole = new G4Cons("plusShld_hole", 0*cm, plus_r_1,0*cm, plus_r_2, 1.01*plus_halfL, 0.*deg, 360.*deg);

      G4UnionSolid* plusShld = new G4UnionSolid("plusShld",plusShld_1,plusShld_2);
      G4SubtractionSolid* plus_shield = new G4SubtractionSolid("plus_shield_w_hole",plusShld,plusShld_hole);

      G4double boxU_halfL = (sept_center-sept_halfL-(plus_center+plus_halfL))/2.;
      G4double boxU_center = plus_center+plus_halfL+boxU_halfL;
      G4double boxU_r_2=box_r_2+5.*cm;

      G4double boxD_halfL = 10.*cm;
      G4double boxD_center = sept_center+sept_halfL+boxD_halfL;
      G4double boxD_r_1=boxU_r_2+10.*cm;
      G4double boxD_r_2=boxU_r_2+15.*cm;

      if ((shldConfig==1)||(shldConfig==2)){
	//Define box between plus and septum

	G4cout<<"Doing extra shielding"<<G4endl;

	G4Box* boxShldU = new G4Box("boxShldU", box_halfW,box_halfH,boxU_halfL);
	G4Cons* boxShldU_hole = new G4Cons("boxShldU_hole", 0*cm, box_r_2,0*cm, boxU_r_2, 1.01*boxU_halfL, 0.*deg, 360.*deg);
	G4SubtractionSolid* box_shieldU_0 = new G4SubtractionSolid("shieldUBox",boxShldU,boxShldU_hole);
	boxshldU_logic = new G4LogicalVolume(box_shieldU_0, shieldMaterial, "boxU_logic_shld");
	boxshldU_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, boxU_center), boxshldU_logic, "8",mother_logic, false, 0);

	G4cout<<"shield centers (box, plus,boxU): "<<box_center/cm<<", "<<plus_center/cm<<", "<<boxU_center/cm<<G4endl;
	
      }

      if (shldConfig==2) {
	//Define box downstream of septum

	G4cout<<"downstream of septum, too"<<G4endl;

	G4Box* boxShldD = new G4Box("boxShldU", box_halfW,box_halfH,boxD_halfL);
	G4Cons* boxShldD_hole = new G4Cons("boxShldD_hole", 0*cm, boxD_r_1,0*cm, boxD_r_2, 1.01*boxD_halfL, 0.*deg, 360.*deg);
	G4SubtractionSolid* box_shieldD_0 = new G4SubtractionSolid("shieldDBox",boxShldD,boxShldD_hole);
	boxshldD_logic = new G4LogicalVolume(box_shieldD_0, shieldMaterial, "boxD_logic_shld");
	boxshldD_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, boxD_center), boxshldD_logic, "9",mother_logic, false, 0);

      }


      if (fShieldThick>0.){
	G4Tubs* scatt_cham_shld_hole = new G4Tubs("holeScattChamberShld", 0, box_r_2, 1.01*outer_r_sc_shld, start_angle, end_angle);
	G4Tubs* scatt_cham_shld = new G4Tubs("outScattChamberShld", inner_r_sc_shld, outer_r_sc_shld, half_height_sc, start_angle, end_angle);
    
	G4ThreeVector yTrans_4(0, 0,-y_cent_sc);
	G4ThreeVector Trans(0, target_center-deltaz_targ-box_center,-y_cent_sc);
	//G4ThreeVector Trans2(0, target_center-deltaz_targ-box_center-(plus_halfL+box_halfL),-y_cent_sc);
	G4ThreeVector Trans2(0, target_center-deltaz_targ-plus_center,-y_cent_sc);

	//Check center the hole for the scatt chamber attach???
	//G4ThreeVector Trans3(0, target_center-deltaz_targ-box_center,-y_cent_sc);
	G4ThreeVector Trans3(0, target_center-deltaz_targ-scatt_attach_center,-y_cent_sc);
	G4RotationMatrix* xRot2 = new G4RotationMatrix;  // Rotates Z and Y axes only
	xRot2->rotateX(90.*deg);
	G4RotationMatrix* xRot3 = new G4RotationMatrix;  // Rotates Z and Y axes only
	xRot3->rotateX(-90.*deg);

	// Need to add "hole" in plus and box shield for scattering chamber attachment.
	//   - merge box and plus
	//   - subtract outer scattering chamber box...    

	G4Box* scatt_attach_hole = new G4Box("boxScattAttach_hole", 1.0001*scatt_attach_halfW, 1.0001*scatt_attach_halfH, 1.0001*scatt_attach_halfL);
	//G4Box* scatt_attach_hole = new G4Box("boxScattAttach_hole", 1.1*scatt_attach_halfW, 1.1*scatt_attach_halfH, 1.1*scatt_attach_halfL);

	G4SubtractionSolid* scatt_cham_shld_solid_2 = new G4SubtractionSolid("scatt_w_hole", scatt_cham_shld, scatt_cham_shld_hole, xRot2, yTrans_4);
	G4UnionSolid* scatt_cham_shld_solid_1  = new G4UnionSolid("scattBoxShld",scatt_cham_shld_solid_2, box_shield_0,xRot2, Trans);
	G4UnionSolid* scatt_cham_shld_solid_0  = new G4UnionSolid("scattBoxPlusShld",scatt_cham_shld_solid_1, plus_shield, xRot2, Trans2);

	G4SubtractionSolid* scatt_cham_shld_solid  = new G4SubtractionSolid("scattBoxPlusAttachShld",scatt_cham_shld_solid_0, scatt_attach_hole, xRot3, Trans3);

	scatt_cham_shld_logic = new G4LogicalVolume(scatt_cham_shld_solid, shieldMaterial, "logicScattChamberShld");
	scatt_cham_shld_phys = new G4PVPlacement(xRot2,G4ThreeVector(0, y_cent_sc, target_center-deltaz_targ), scatt_cham_shld_logic, "4", mother_logic , false, 0);

      } else {

	plusshld_logic = new G4LogicalVolume(plus_shield, shieldMaterial, "plus_logic_shld");
	plusshld_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, plus_center), plusshld_logic, "2",mother_logic, false, 0);

	boxshld_logic = new G4LogicalVolume(box_shield_0, shieldMaterial, "box_logic_shld");
	boxshld_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, box_center), boxshld_logic, "1",mother_logic, false, 0);


      }
      //----------------------------------------------
      if (shieldType.compare("prex_sept")==0){

	if(fDrawSeptum) {

	  G4Box* sept_shld_l_0 = new G4Box("septShld_l_0", 0.9999*sept_rect_hole_halfW, 0.9999*sept_rect_hole_halfH, sept_halfL);
	  G4Trap* hole_for_l_vac_enc = new G4Trap("vacEncHoleL", 1.001*vac_enc_halfL, left_theta, left_phi,  
						  1.0001*vac_u_enc_halfH, 1.0001*vac_u_enc_halfW, 1.0001*vac_u_enc_halfW, alpha_l_1,
						  1.0001*vac_d_enc_halfH, 1.0001*vac_d_enc_halfW, 1.0001*vac_d_enc_halfW, alpha_l_2);
	  G4ThreeVector Trans_septl(rect_hole_center_x-vac_enc_xu_center, 0, vac_enc_z_center-sept_center);
	  G4cout<<rect_hole_center_x<<" - "<<vac_enc_xu_center<<" = "<<rect_hole_center_x-vac_enc_xu_center<<G4endl;
	  G4cout<<vac_enc_z_center<<" - "<<sept_center<<" = "<<vac_enc_z_center-sept_center<<G4endl;
	  G4SubtractionSolid* sept_shld_l = new G4SubtractionSolid("septShld_l",sept_shld_l_0,hole_for_l_vac_enc, 0, Trans_septl);

	  G4Box* sept_shld_r_0 = new G4Box("septShld_r_0", 0.999*sept_rect_hole_halfW, 0.999*sept_rect_hole_halfH, sept_halfL);
	  G4Trap* hole_for_r_vac_enc = new G4Trap("vacEncHoleR", 1.0001*vac_enc_halfL, right_theta, right_phi,  
						  1.0001*vac_u_enc_halfH, 1.0001*vac_u_enc_halfW, 1.0001*vac_u_enc_halfW, alpha_l_1,
						  1.0001*vac_d_enc_halfH, 1.0001*vac_d_enc_halfW, 1.0001*vac_d_enc_halfW, alpha_l_2);

	  G4ThreeVector Trans_septr(-(rect_hole_center_x-vac_enc_xu_center), 0, vac_enc_z_center-sept_center);
	  G4SubtractionSolid* sept_shld_r = new G4SubtractionSolid("septShld_r",sept_shld_r_0,hole_for_r_vac_enc, 0, Trans_septr);

	  sept_shld_l_logic = new G4LogicalVolume(sept_shld_l, beamlineMaterial, "logicSeptShldL", 0);
	  sept_shld_l_phys = new G4PVPlacement(0,G4ThreeVector(-rect_hole_center_x,0,sept_center), sept_shld_l_logic, "6", mother_logic, false, 0);

	  sept_shld_r_logic = new G4LogicalVolume(sept_shld_r, beamlineMaterial, "logicSeptShldR", 0);
	  sept_shld_r_phys = new G4PVPlacement(0,G4ThreeVector(rect_hole_center_x,0,sept_center), sept_shld_r_logic, "7", mother_logic, false, 0);

	} else {

	  G4double cyl_ri_1 = 8.066*cm; G4double cyl_ro_1 = 9.141*cm;
	  G4double cyl_ri_2 = 8.066*cm; G4double cyl_ro_2 = 20.613*cm;
	
	  G4double cyl_halfL = 37.193*cm; G4double cyl_center = sept_center;
	
	  G4double cyl2_ri_1 = 25.094*cm; G4double cyl2_ro_1 = 39.434*cm;
	  G4double cyl2_ri_2 = 30.471*cm; G4double cyl2_ro_2 = 39.434*cm;
	
	  G4Cons* cyl_shield = new G4Cons("cylShld", cyl_ri_1, cyl_ro_1,cyl_ri_2, cyl_ro_2, cyl_halfL, 0.*deg, 360.*deg);
	  G4Cons* cyl2_shield = new G4Cons("cylShld", cyl2_ri_1, cyl2_ro_1,cyl2_ri_2, cyl2_ro_2, cyl_halfL, 0.*deg, 360.*deg);
	
	  cylshld_logic = new G4LogicalVolume(cyl_shield, shieldMaterial, "cyl_logic_shld");
	  cylshld_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, cyl_center), cylshld_logic, "3",mother_logic, false, 0);
	
	  cylshld2_logic = new G4LogicalVolume(cyl2_shield, shieldMaterial, "cyl2_logic_shld");
	  cylshld2_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, cyl_center), cylshld2_logic, "5",mother_logic, false, 0);
	}

      } else {

	if(fDrawSeptum) {
	} else {
	  //Original cylinder with PREx II geometry
	  G4double cyl_ri_1 = 4.102*cm; G4double cyl_ro_1 = 16.102*cm; G4double cyl_halfL=7.5*cm;
	  G4double cyl_ri_2 = 4.102*cm; G4double cyl_ro_2 = 16.102*cm;
	  G4double cyl_center = 219.*cm+prex_target_center;
      
	  G4Cons* cyl_shield = new G4Cons("cylShld", cyl_ri_1, cyl_ro_1,cyl_ri_2, cyl_ro_2, cyl_halfL, 0.*deg, 360.*deg);
      
	  cylshld_logic = new G4LogicalVolume(cyl_shield, shieldMaterial, "cyl_logic_shld");
	  cylshld_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, cyl_center), cylshld_logic, "3",mother_logic, false, 0);
	}
      }
  

      G4cout<<"Finished shield definition..."<<G4endl;

    } else if (shieldType.compare("combo")==0){

      G4double shld_1_thick = fShieldThick; //should be 10
      G4double shld_2_thick = 2.5*cm;
      G4double shld_3_thick = 2.5*cm;
      G4double shld_inner_r = 15.*cm;
      G4double shld_1_outer_r = shld_inner_r+shld_1_thick;
      G4double shld_2_outer_r = shld_1_outer_r+shld_2_thick;
      G4double shld_3_outer_r = shld_2_outer_r+shld_3_thick;
      //  G4double theta_limit = 1*deg;
      G4double beamhole_r_1 = r_sc_beam_hole;
      G4double beamhole_r_2 = 6.67*cm;
      //  G4double shld_center = prex_target_center;
      G4double shld_center = -12*cm;

      G4Cons* beamline_hole = new G4Cons("beamHole", 0*cm, beamhole_r_1,0*cm, beamhole_r_2, 1.02*shld_3_outer_r, 0.*deg, 360.*deg);

      G4Sphere* sphere_0 = new G4Sphere("shellSheild_1", shld_inner_r, shld_3_outer_r+0.001*cm, 0*deg, 360*deg, 0*deg, 180*deg);

      G4Sphere* sphere_1 = new G4Sphere("shellSheild_1", shld_inner_r, shld_1_outer_r, 0*deg, 360*deg, 0*deg, 180*deg);
      G4SubtractionSolid* pshell_1_shld = new G4SubtractionSolid("shldP_1",sphere_1,beamline_hole);
      sphereshld_1_logic = new G4LogicalVolume(pshell_1_shld, shieldMaterial_1, "sphere_1_logic_det");
      shield_1_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, shld_center), sphereshld_1_logic, "4", mother_logic, false, 0);

      G4Sphere* sphere_2 = new G4Sphere("shellSheild_2", shld_1_outer_r+0.001*cm, shld_2_outer_r, 0*deg, 360*deg, 0*deg, 180*deg);
      G4SubtractionSolid* pshell_2_shld = new G4SubtractionSolid("shldP_2",sphere_2,beamline_hole);
      sphereshld_2_logic = new G4LogicalVolume(pshell_2_shld, shieldMaterial_2, "sphere_2_logic_det");
      shield_2_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, shld_center), sphereshld_2_logic, "5", mother_logic, false, 0);

      G4Sphere* sphere_3 = new G4Sphere("shellSheild_3", shld_2_outer_r+0.001*cm, shld_3_outer_r, 0*deg, 360*deg, 0*deg, 180*deg);
      G4SubtractionSolid* pshell_3_shld = new G4SubtractionSolid("shldP_3",sphere_3,beamline_hole);
      sphereshld_3_logic = new G4LogicalVolume(pshell_3_shld, shieldMaterial_3, "sphere_3_logic_det");
      shield_3_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, shld_center), sphereshld_3_logic, "6", mother_logic, false, 0);

      G4double box_halfW=69.596*cm; G4double box_halfH=50.8*cm; G4double box_halfL=16.1925*cm; 
      G4double box_center=64.5025*cm+prex_target_center; G4double box_r_1=7.6*cm; G4double box_r_2=10.7*cm; 

      G4double plus_1_halfW=13.2969*cm; G4double plus_1_halfH=50.8*cm; G4double plus_halfL=14.7955*cm;
      G4double plus_2_halfW=69.596*cm; G4double plus_2_halfH=34.8234*cm;
      G4double plus_center=95.4905*cm+prex_target_center; G4double plus_r_1=10.7*cm; G4double plus_r_2=14*cm; 

      G4double cyl_ri_1 = 4.102*cm; G4double cyl_ro_1 = 16.102*cm; G4double cyl_halfL=7.5*cm;
      G4double cyl_ri_2 = 4.102*cm; G4double cyl_ro_2 = 16.102*cm;
      G4double cyl_center = 219.*cm+prex_target_center;

      G4cout<<"shield centers"<<shld_center<<" "<<box_center<<" "<<plus_center<<G4endl;
      G4Box* boxShld = new G4Box("boxShld", box_halfW,box_halfH,box_halfL);
      G4Cons* boxShld_hole = new G4Cons("boxShld_hole", 0*cm, box_r_1,0*cm, box_r_2, 1.01*box_halfL, 0.*deg, 360.*deg);

      G4SubtractionSolid* box_shield_0 = new G4SubtractionSolid("shieldBox",boxShld,boxShld_hole);
      G4ThreeVector zTrans_shld_1(0, 0, shld_center-box_center);
      G4SubtractionSolid* box_shield = new G4SubtractionSolid("shieldBox",box_shield_0,sphere_0, 0, zTrans_shld_1);

      G4Box* plusShld_1 = new G4Box("plusShld_1", plus_1_halfW,plus_1_halfH,plus_halfL);
      G4Box* plusShld_2 = new G4Box("plusShld_2", plus_2_halfW,plus_2_halfH,plus_halfL);

      G4Cons* plusShld_hole = new G4Cons("plusShld_hole", 0*cm, plus_r_1,0*cm, plus_r_2, 1.01*plus_halfL, 0.*deg, 360.*deg);

      G4UnionSolid* plusShld = new G4UnionSolid("plusShld",plusShld_1,plusShld_2);
      G4SubtractionSolid* plus_shield_0 = new G4SubtractionSolid("plus_shield_w_hole",plusShld,plusShld_hole);
      G4ThreeVector zTrans_shld_2(0, 0, shld_center-plus_center);
      G4SubtractionSolid* plus_shield = new G4SubtractionSolid("plus_shield_w_hole",plus_shield_0, sphere_0, 0, zTrans_shld_2);

      boxshld_logic = new G4LogicalVolume(box_shield, shieldMaterial, "box_logic_shld");
      boxshld_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, box_center), boxshld_logic, "1",mother_logic, false, 0);
    
      plusshld_logic = new G4LogicalVolume(plus_shield, shieldMaterial, "plus_logic_shld");
      plusshld_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, plus_center), plusshld_logic, "2",mother_logic, false, 0);

      G4Cons* cyl_shield = new G4Cons("cylShld", cyl_ri_1, cyl_ro_1,cyl_ri_2, cyl_ro_2, cyl_halfL, 0.*deg, 360.*deg);

      cylshld_logic = new G4LogicalVolume(cyl_shield, shieldMaterial, "cyl_logic_shld");
      cylshld_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, cyl_center), cylshld_logic, "3",mother_logic, false, 0);

 

    } else if (shieldType.compare("sphere")==0){

      //------  PREx spherical shell shielding -----//
      G4double shld_1_thick = fShieldThick; //should be 10
      G4double shld_2_thick = 2.5*cm;
      G4double shld_3_thick = 2.5*cm;
      G4double shld_inner_r = 15.*cm;
      G4double shld_1_outer_r = shld_inner_r+shld_1_thick;
      G4double shld_2_outer_r = shld_1_outer_r+shld_2_thick;
      G4double shld_3_outer_r = shld_2_outer_r+shld_3_thick;
      //  G4double theta_limit = 1*deg;
      G4double beamhole_r_1 = r_sc_beam_hole;
      G4double beamhole_r_2 = 6.67*cm;
      //  G4double shld_center = prex_target_center;
      G4double shld_center = -12*cm;

      G4Cons* beamline_hole = new G4Cons("beamHole", 0*cm, beamhole_r_1,0*cm, beamhole_r_2, 1.02*shld_3_outer_r, 0.*deg, 360.*deg);

      G4Sphere* sphere_1 = new G4Sphere("shellSheild_1", shld_inner_r, shld_1_outer_r, 0*deg, 360*deg, 0*deg, 180*deg);
      G4SubtractionSolid* pshell_1_shld = new G4SubtractionSolid("shldP_1",sphere_1,beamline_hole);
      sphereshld_1_logic = new G4LogicalVolume(pshell_1_shld, shieldMaterial_1, "sphere_1_logic_det");
      shield_1_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, shld_center), sphereshld_1_logic, "1", mother_logic, false, 0);

      G4Sphere* sphere_2 = new G4Sphere("shellSheild_2", shld_1_outer_r+0.001*cm, shld_2_outer_r, 0*deg, 360*deg, 0*deg, 180*deg);
      G4SubtractionSolid* pshell_2_shld = new G4SubtractionSolid("shldP_2",sphere_2,beamline_hole);
      sphereshld_2_logic = new G4LogicalVolume(pshell_2_shld, shieldMaterial_2, "sphere_2_logic_det");
      shield_2_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, shld_center), sphereshld_2_logic, "2", mother_logic, false, 0);

      G4Sphere* sphere_3 = new G4Sphere("shellSheild_3", shld_2_outer_r+0.001*cm, shld_3_outer_r, 0*deg, 360*deg, 0*deg, 180*deg);
      G4SubtractionSolid* pshell_3_shld = new G4SubtractionSolid("shldP_3",sphere_3,beamline_hole);
      sphereshld_3_logic = new G4LogicalVolume(pshell_3_shld, shieldMaterial_3, "sphere_3_logic_det");
      shield_3_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, shld_center), sphereshld_3_logic, "3", mother_logic, false, 0);

    }
    G4cout<<"Defining detectors..."<<G4endl;
	    
    //------ Generic spherical detector -----//
    G4Sphere* pshell_det = new G4Sphere("shellDet", det_inner_r, det_outer_r, 0*deg, 360*deg, 0*deg, 180*deg);
    spheredet_logic = new G4LogicalVolume(pshell_det, worldMaterial, "sDet_1");
    det_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, prex_target_center), spheredet_logic, "sphere_det_1", mother_logic, false, 0);

    //------ Plane detectors -----//

    G4Box* pDet_1 = new G4Box("pDet_1", half_side_mom, half_side_mom, 0.5*cm);
    G4Box* pDet_2 = new G4Box("pDet_2", half_side_mom, half_side_mom, 0.5*cm);
    G4Box* pDet_3 = new G4Box("pDet_3", half_side_mom, half_side_mom, 0.5*cm);

    plane_det_1_logic = new G4LogicalVolume(pDet_1, worldMaterial, "pDet_1_logic");
    plane_det_1_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, prex_target_center+600.*cm), plane_det_1_logic, "plane_det_1", mother_logic, false, 0);

    plane_det_2_logic = new G4LogicalVolume(pDet_2, worldMaterial, "pDet_1_logic");
    plane_det_2_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, prex_target_center+2300.*cm), plane_det_2_logic, "plane_det_2", mother_logic, false, 0);

    if (NUM_COLS>=4){
      plane_det_3_logic = new G4LogicalVolume(pDet_3, worldMaterial, "pDet_1_logic");
      plane_det_3_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, 2650.*cm), plane_det_3_logic, "plane_det_3", mother_logic, false, 0);
    }

    //===========================================================================================================
    //------  Sensitivity Attributes

    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;

    for (G4int i=0; i<NUM_COLS; i++) {
      SDname = "/detector";
      SDname+=G4UIcommand::ConvertToString((i+1));
      detector.push_back (new MollerDetectorSD(SDname));
      SDman->AddNewDetector(detector[i]);
    }

    spheredet_logic->SetSensitiveDetector(detector[0]);
    plane_det_1_logic->SetSensitiveDetector(detector[1]);
    plane_det_2_logic->SetSensitiveDetector(detector[2]);

    if (NUM_COLS>=4){

      plane_det_3_logic->SetSensitiveDetector(detector[3]);

      if (NUM_COLS>=5){
	if (fDrawCollim) W_coll_logic->SetSensitiveDetector(detector[4]);

	if (shieldType.compare("hut")==0) hut_logic->SetSensitiveDetector(detector[5]);

	if ((shldConfig==3)||(shldConfig==4)||(shldConfig==5)) { // hut with septum shld

	  if(fDrawSeptum) {

	    sept_shld_l_logic->SetSensitiveDetector(detector[6]);
	    sept_shld_r_logic->SetSensitiveDetector(detector[7]);

	  }
	}
      }
    }
  
  
    if(shieldType.compare("none")==0) {
    } else if (shieldType.compare("prex")==0){
      //    bl_logic[11]->SetSensitiveDetector(detector[1]);
    } else if ((shieldType.compare("sphere")==0)&&(NUM_COLS>1)){
    }

    //===========================================================================================================
    //------  Visualization Attributes
    G4cout<<G4endl<<"Defining visualization attributes..."<<G4endl;
  
    G4cout<<"Mother volume"<<G4endl;
    G4VisAttributes* motherVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,0.0));
    motherVisAtt->SetVisibility(false);
    mother_logic->SetVisAttributes(motherVisAtt);

    G4cout<<"Target"<<G4endl;
    G4VisAttributes* targetVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    //  targetVisAtt->SetForceWireframe (true);
    targetVisAtt->SetVisibility(true);
    target_logic->SetVisAttributes(targetVisAtt);

    if(fDrawCollim) {
      G4cout<<"Collimator"<<G4endl;
      G4VisAttributes* collVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
      //collVisAtt->SetForceWireframe (true);
      collVisAtt->SetVisibility(true);
      W_coll_logic->SetVisAttributes(collVisAtt);
    } else {
    }

    if(fDrawWholeGeo) {
      G4cout<<"Whole geo:"<<G4endl;

      G4cout<<"\t Scattering Chamber"<<G4endl;
      G4VisAttributes* scattChamberVisAtt= new G4VisAttributes(G4Colour(0.7,0.7,0.7));
      //scattChamberVisAtt->SetForceWireframe (true);
      scattChamberVisAtt->SetVisibility(true);
      scatt_cham_logic->SetVisAttributes(scattChamberVisAtt);
      
      G4cout<<"\t Scattering Chamber Attachments"<<G4endl;
      G4VisAttributes* scattAttachVisAtt= new G4VisAttributes(G4Colour(0.1,0.1,0.1));
      //scattChamberVisAtt->SetForceWireframe (true);
      //scattAttachVisAtt->SetVisibility(true);
      scattAttachVisAtt->SetVisibility(false);
      //    scatt_attach_logic->SetVisAttributes(scattAttachVisAtt);

      G4cout<<G4endl<<"\t Hall:"<<G4endl;
      if (fDrawHall){
	G4cout<<"\t \t wall, floor, dump"<<G4endl;
	G4VisAttributes* hallVisAtt= new G4VisAttributes(G4Colour(0.9,0.9,0.9));
	//hallVisAtt->SetForceWireframe (true);
	hallVisAtt->SetVisibility(true);
	wall_logic->SetVisAttributes(hallVisAtt);
	floor_logic->SetVisAttributes(hallVisAtt);
	dump_logic->SetVisAttributes(hallVisAtt);
    
	G4cout<<"\t \t Stainless Steel enclosure"<<G4endl;
	//  G4VisAttributes* dumpVisAtt= new G4VisAttributes(G4Colour(0.3,0.3,0.3));
	G4VisAttributes* ssEncVisAtt= new G4VisAttributes(G4Colour(0.,1.0,0.));
	//ssEncVisAtt->SetForceWireframe (true);
	ssEncVisAtt->SetVisibility(true);
	ssenc_logic->SetVisAttributes(ssEncVisAtt);
      }

      //------  Beamline visualization 
      G4cout<<"\t Beamline: number of beampipes is ";
      G4cout<<max_pipes<<" - ";
      for (G4int i = 0; i<max_pipes;i++){
	G4cout<<i<< " ";
	G4VisAttributes* beamVisAtt= new G4VisAttributes(G4Colour(0.7,0.7,0.7));
	//	beamVisAtt->SetForceWireframe (false);
	beamVisAtt->SetVisibility(true);
	//beamVisAtt->SetVisibility(false);
	bl_logic[i]->SetVisAttributes(beamVisAtt);
      }
      G4cout<<G4endl;;

      if(targMat.compare("LD2")==0){
	
      } else {

	//------  Septum visualization 
	if(fDrawSeptum) {
	  G4cout<<"\t Septum"<<G4endl;
	  G4VisAttributes* septumVisAtt = new G4VisAttributes(G4Colour(0.6,0.6,0.6));
	  septumVisAtt->SetVisibility(true);
	  septum_logic->SetVisAttributes(septumVisAtt);

	  G4cout<<"\t Septum pipe"<<G4endl;
	  //G4VisAttributes* septVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
	  G4VisAttributes* septVisAtt= new G4VisAttributes(G4Colour(0.7,0.7,0.7));
	  //	  septVisAtt->SetForceWireframe (false);
	  septVisAtt->SetVisibility(true);
	  //septVisAtt->SetVisibility(false);
	  septum_pipe_logic->SetVisAttributes(septVisAtt);
	  //	  scatt_attach_logic->SetVisAttributes(septVisAtt);
	}
      }
      
      //------  Shield visualization 
      G4cout<<"Shielding:"<<G4endl;
      if(shieldType.compare("none")==0) {
      } else if (shieldType.compare("hut")==0){

	G4cout<<"\t Hut"<<G4endl;
	G4VisAttributes* hutVisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	//	hutVisAtt->SetForceWireframe(true);
	hutVisAtt->SetVisibility(true);
	hut_logic->SetVisAttributes(hutVisAtt);

	if ((shldConfig==3)||(shldConfig==4)||(shldConfig==5)){
	  G4cout<<"\t Septum shield"<<G4endl;
	  G4VisAttributes* septshldVisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	  septshldVisAtt->SetVisibility(true);
	
	  sept_shld_l_logic->SetVisAttributes(septshldVisAtt);
	  sept_shld_r_logic->SetVisAttributes(septshldVisAtt);

	  if (shldConfig==5){
	    G4cout<<"\t Plus shield"<<G4endl;
	    G4VisAttributes* plusshldVisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	    plusshldVisAtt->SetVisibility(true);
	    plusshld_logic->SetVisAttributes(plusshldVisAtt);
	  }

	}
    
      } else if ((shieldType.compare("prex")==0)||(shieldType.compare("prex_sept")==0)){

	G4cout<<"\t Prex shields:"<<G4endl;
	if ((shldConfig==1)||(shldConfig==2)){
	  G4cout<<" \t \t box shield:"<<G4endl;
	  G4VisAttributes* boxshldUVisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	  //boxshldUVisAtt->SetForceWireframe (true);
	  boxshldUVisAtt->SetVisibility(true);
	  boxshldU_logic->SetVisAttributes(boxshldUVisAtt);

	}
	if (shldConfig==2){

	}

	if (fShieldThick>0.){
	  G4cout<<" \t \t Scatt cyl.."<<G4endl;
	  G4VisAttributes* scattshldVisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	  //scattshldVisAtt->SetForceWireframe(true);
	  scattshldVisAtt->SetVisibility(true);
	  scatt_cham_shld_logic->SetVisAttributes(scattshldVisAtt);

	  if ((shldConfig==1)||(shldConfig==2)){
	    G4VisAttributes* boxshldUVisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	    //boxshldUVisAtt->SetForceWireframe (true);
	    boxshldUVisAtt->SetVisibility(true);
	    boxshldU_logic->SetVisAttributes(boxshldUVisAtt);

	  }
	  if (shldConfig==2){
	    G4VisAttributes* boxshldDVisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	    //boxshldDVisAtt->SetForceWireframe (true);
	    boxshldDVisAtt->SetVisibility(true);
	    boxshldD_logic->SetVisAttributes(boxshldDVisAtt);

	  }
	} else {
	  G4VisAttributes* boxshldVisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	  //  shldVisAtt->SetForceWireframe (true);
	  boxshldVisAtt->SetVisibility(true);
	  boxshld_logic->SetVisAttributes(boxshldVisAtt);

	  G4VisAttributes* plusshldVisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	  plusshldVisAtt->SetVisibility(true);
	  plusshld_logic->SetVisAttributes(plusshldVisAtt);

	}
	if (shieldType.compare("prex_sept")==0) {
	  if(fDrawSeptum) {
	    G4VisAttributes* septshldVisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	    septshldVisAtt->SetVisibility(true);
	
	    sept_shld_l_logic->SetVisAttributes(septshldVisAtt);
	    sept_shld_r_logic->SetVisAttributes(septshldVisAtt);
	
	  } else {
	    G4cout<<"cyl1..."<<G4endl;
	    G4VisAttributes* cylshldVisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	    cylshldVisAtt->SetVisibility(true);
	    cylshld_logic->SetVisAttributes(cylshldVisAtt);
	
	    G4cout<<"cyl2..."<<G4endl;
	    cylshld2_logic->SetVisAttributes(cylshldVisAtt);
	
	  }
	}
      } else if (shieldType.compare("combo")==0){
	G4VisAttributes* shld1VisAtt= new G4VisAttributes(G4Colour(0.0,0.9,0.0));
	shld1VisAtt->SetVisibility(true);
	sphereshld_1_logic->SetVisAttributes(shld1VisAtt);
	G4VisAttributes* shld2VisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	shld2VisAtt->SetVisibility(true);
	sphereshld_2_logic->SetVisAttributes(shld2VisAtt);
	G4VisAttributes* shld3VisAtt= new G4VisAttributes(G4Colour(0.0,0.0,0.9));
	shld3VisAtt->SetVisibility(true);
	sphereshld_3_logic->SetVisAttributes(shld3VisAtt);
    
      } else if (shieldType.compare("sphere")==0){
	G4VisAttributes* shld1VisAtt= new G4VisAttributes(G4Colour(0.9,0.0,0.0));
	shld1VisAtt->SetVisibility(true);
	sphereshld_1_logic->SetVisAttributes(shld1VisAtt);
    
	G4VisAttributes* shld2VisAtt= new G4VisAttributes(G4Colour(0.0,0.9,0.0));
	shld2VisAtt->SetVisibility(true);
	sphereshld_2_logic->SetVisAttributes(shld2VisAtt);
    
	G4VisAttributes* shld3VisAtt= new G4VisAttributes(G4Colour(0.0,0.0,0.9));
	shld3VisAtt->SetVisibility(true);
	sphereshld_3_logic->SetVisAttributes(shld3VisAtt);
    
      } 
    } else {
      //Don't draw anything but target, collimators and detector
    }

    //------  Detector visualization 
  
    G4cout<<"Detectors"<<G4endl;
    G4VisAttributes* detVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    detVisAtt->SetForceWireframe (true);
    detVisAtt->SetVisibility(true);

    spheredet_logic->SetVisAttributes(detVisAtt);
    plane_det_1_logic->SetVisAttributes(detVisAtt);
    plane_det_2_logic->SetVisAttributes(detVisAtt);

    if(NUM_COLS==4){
      plane_det_3_logic->SetVisAttributes(detVisAtt);
    }
    G4cout<<"Done with visualization definition!"<<G4endl;

  } else if(shieldType.compare("test")==0){
    //---------------------------------------------------------------------------------------------------
    //Test geometry

    //------  Set up volumes
    G4Tubs* target_solid = new G4Tubs("tubeTarget", 0., fShieldRadius, fShieldThick/2., start_angle, end_angle);
    target_logic = new G4LogicalVolume(target_solid, shieldMaterial, "logicTarget");
    target_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, target_center), target_logic, "101", mother_logic , false, 0);
    G4cout<<G4endl<<G4endl<<"Target: Radius = "<<fShieldRadius<<", Length = "<<fShieldThick<<", Center = "<<target_center<<G4endl<<G4endl;

    /*G4double det1_center=-(fShieldThick/2.+1.*cm);
      G4double det2_center=fShieldThick/2.+1.*cm;*/

    G4double det1_center=-75.*cm;
    G4double det2_center=75.*cm;

    if ((-det1_center<fShieldThick/2.)||(det2_center<fShieldThick/2.)) G4cout<<"Target too thick!!!!!!!!!!!!!!!!"<<G4endl;

    G4Tubs* det1_solid = new G4Tubs("tubeDet1", 0., 505.*cm, 0.05*cm, start_angle, end_angle);
    det1_logic = new G4LogicalVolume(det1_solid, worldMaterial, "logicDet1");
    det1_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, target_center+det1_center), det1_logic, "1", mother_logic , false, 0);

    G4Tubs* det2_solid = new G4Tubs("tubeDet2", 0., 505.*cm, 0.05*cm, start_angle, end_angle);
    det2_logic = new G4LogicalVolume(det2_solid, worldMaterial, "logicDet2");
    det2_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, target_center+det2_center), det2_logic, "2", mother_logic , false, 0);

    //G4Tubs* det3_solid = new G4Tubs("tubeDet3", 506.001*cm, 507.101*cm, (det2_center-det1_center+.1*cm)/2., start_angle, end_angle);
    G4Tubs* det3_solid = new G4Tubs("tubeDet3", 505.0*cm, 505.1*cm, (det2_center-det1_center+.1*cm)/2., start_angle, end_angle);
    det3_logic = new G4LogicalVolume(det3_solid, worldMaterial, "logicDet3");
    det3_phys = new G4PVPlacement(0,G4ThreeVector(0, 0, target_center), det3_logic, "3", mother_logic , false, 0);


    //------  Sensitivity Attributes
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;

    for (int i=0; i<NUM_COLS; i++) {
      SDname = "/detector";
      SDname+=G4UIcommand::ConvertToString((i+1));
      detector.push_back (new MollerDetectorSD(SDname));
      SDman->AddNewDetector(detector[i]);
    }
    
    det1_logic->SetSensitiveDetector(detector[0]);
    det2_logic->SetSensitiveDetector(detector[1]);
    det3_logic->SetSensitiveDetector(detector[2]);



    //------  Visualization Attributes
    G4VisAttributes* motherVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,0.0));
    G4VisAttributes* targetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    G4VisAttributes* det1VisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
    G4VisAttributes* det2VisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    G4VisAttributes* det3VisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0));

    motherVisAtt->SetVisibility(false);
    
    //targetVisAtt->SetForceWireframe (true);
    targetVisAtt->SetVisibility(true);

    if (fDrawWholeGeo){ // Note that draw here just means make targetvisible.
      det1VisAtt->SetForceWireframe(true);
      det2VisAtt->SetForceWireframe(true);
      det3VisAtt->SetForceWireframe(true);
    }
    if (fDrawCollim){ // Note that draw here just means make targetvisible.
      det1VisAtt->SetVisibility(true);
      det2VisAtt->SetVisibility(true);
      det3VisAtt->SetVisibility(true);
      det1VisAtt->SetForceWireframe(true);
      det2VisAtt->SetForceWireframe(true);
      det3VisAtt->SetForceWireframe(true);
    } else {
      det1VisAtt->SetVisibility(false);
      det2VisAtt->SetVisibility(false);
      det3VisAtt->SetVisibility(false);
    }

    mother_logic->SetVisAttributes(motherVisAtt);
    target_logic->SetVisAttributes(targetVisAtt);
    det1_logic->SetVisAttributes(det1VisAtt);
    det2_logic->SetVisAttributes(det2VisAtt);
    det3_logic->SetVisAttributes(det3VisAtt);


    //=================================================================================================
  }

  ReadGlobalMagneticField();

  return mother_phys;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MollerDetectorConstruction::ReadGlobalMagneticField()
{
  mg_field_low = pGlobalMagnetField->ReadLowLimSeptumField();
  mg_field_high = pGlobalMagnetField->ReadHighLimSeptumField();
  mgFieldScaleFactor = pGlobalMagnetField->GetScaleFactor();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MollerDetectorConstruction::SetGlobalMagneticField()   
{

  //--------- Magnetic Field -------------------------------
  
  //============================================
  //  Define the global magnet field Manager
  //============================================
  pGlobalMagnetField = new MollerGlobalMagnetField();

  // Get transportation, field, and propagator  managers
  fGlobalFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4TransportationManager::GetTransportationManager()->GetPropagatorInField()->SetLargestAcceptableStep(25*cm); // when reading MollerMap
  //G4TransportationManager::GetTransportationManager()->GetPropagatorInField()->SetLargestAcceptableStep(0.5*cm);

  fGlobalFieldManager->SetDetectorField(pGlobalMagnetField);
  
  fGlobalEquation = new G4Mag_UsualEqRhs(pGlobalMagnetField);
  
  // taken from one of the Geant4 presentation:
  // - If the field is calculated from a field map, a lower order stepper
  //   is recommended: the less smooth the field is, the lower the order of the
  //   stepper that should be used. For very rough fields one should use 1st order
  //   stepper, for a somewhat smooth field one must choose between 1st and 2nd
  //   order stepper.
  
  //fGlobalStepper  = new G4ClassicalRK4(fGlobalEquation);  // classical 4th order stepper
  //fGlobalStepper  = new G4ExplicitEuler(fGlobalEquation); //           1st order stepper
  //fGlobalStepper  = new G4ImplicitEuler(fGlobalEquation); //           2nd order stepper
  fGlobalStepper  = new G4SimpleRunge(fGlobalEquation);   //           2nd order stepper


  // Provides a driver that talks to the Integrator Stepper, and insures that 
  //   the error is within acceptable bounds.
  G4MagInt_Driver* fGlobalIntgrDriver = new G4MagInt_Driver(1.0e-3*mm, 
							    fGlobalStepper,
							    fGlobalStepper->GetNumberOfVariables());
  
  fGlobalChordFinder = new G4ChordFinder(fGlobalIntgrDriver);
  
  
  
  //       G4bool fieldChangesEnergy = false;
  //       G4FieldManager* pFieldMgr = new G4FieldManager(myField,pChordFinder,FieldChangeEnergy);
  //       LocalLogicalVolume = new G4LogicalVolume(shape, material,"name",pFieldMgr,0,0);
  
  //   // minimal step of 1 mm is default
  //   fMinStep = 0.01*mm ;
  //
  //   fGlobalChordFinder = new G4ChordFinder (pGlobalMagnetField,
  //                                           fMinStep,
  //                                           fGlobalStepper);
  
  fGlobalFieldManager->SetChordFinder(fGlobalChordFinder);
} 
