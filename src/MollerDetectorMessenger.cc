#include "MollerDetectorMessenger.hh"

#include "MollerDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

MollerDetectorMessenger::MollerDetectorMessenger(MollerDetectorConstruction* myDet)
:myDetector(myDet)
{ 

  detDir = new G4UIdirectory("/moller/det/");
  detDir->SetGuidance("detector control.");

  shieldTypeCmd = new G4UIcmdWithAString("/moller/det/setShieldType",this);
  shieldTypeCmd->SetGuidance("Set the shield type.");
  shieldTypeCmd->SetParameterName("shieldType",true);
  shieldTypeCmd->SetDefaultValue("none");
  G4String candidateList=("sphere prex prex_sept test hut combo none");
  shieldTypeCmd->SetCandidates(candidateList);
  shieldTypeCmd->AvailableForStates(G4State_PreInit);

  shieldThickCmd = new G4UIcmdWithADoubleAndUnit("/moller/det/setShieldThick",this);
  shieldThickCmd->SetGuidance("Set the shield thickness.");
  shieldThickCmd->SetParameterName("fShieldThick",true, true);
  shieldThickCmd->SetDefaultValue(10.);
  shieldThickCmd->SetUnitCategory("Length");
  shieldThickCmd->SetDefaultUnit("cm");
  shieldThickCmd->AvailableForStates(G4State_PreInit);

  shieldRadiusCmd = new G4UIcmdWithADoubleAndUnit("/moller/det/setShieldRadius",this);
  shieldRadiusCmd->SetGuidance("Set the shield thickness.");
  shieldRadiusCmd->SetParameterName("fShieldRadius",true, true);
  shieldRadiusCmd->SetDefaultValue(10.);
  shieldRadiusCmd->SetUnitCategory("Length");
  shieldRadiusCmd->SetDefaultUnit("cm");
  shieldRadiusCmd->AvailableForStates(G4State_PreInit);

  shieldMatCmd = new G4UIcmdWithAString("/moller/det/setShieldMaterial",this);
  shieldMatCmd->SetGuidance("Set the shield material.");
  shieldMatCmd->SetParameterName("shieldMat",true);
  shieldMatCmd->SetDefaultValue("concrete");
  candidateList=("concrete polyethylene stainless iron borpoly tungsten steel hallC none water");
  shieldMatCmd->SetCandidates(candidateList);
  shieldMatCmd->AvailableForStates(G4State_PreInit);

  targetCenterCmd = new G4UIcmdWithADoubleAndUnit("/moller/det/setTargetCenter",this);
  targetCenterCmd->SetGuidance("Set the shield center.");
  targetCenterCmd->SetParameterName("fTargetCenter",true, true);
  targetCenterCmd->SetDefaultValue(-105.3);
  targetCenterCmd->SetUnitCategory("Length");
  targetCenterCmd->SetDefaultUnit("cm");
  targetCenterCmd->AvailableForStates(G4State_PreInit);

  targetLengthCmd = new G4UIcmdWithADoubleAndUnit("/moller/det/setTargetLength",this);
  targetLengthCmd->SetGuidance("Set the target length");
  targetLengthCmd->SetParameterName("fTxargetLength",true, true);
  targetLengthCmd->SetDefaultValue(0.05);
  targetLengthCmd->SetUnitCategory("Length");
  targetLengthCmd->SetDefaultUnit("cm");
  targetLengthCmd->AvailableForStates(G4State_PreInit);

  targetMatCmd = new G4UIcmdWithAString("/moller/det/setTargetMaterial",this);
  targetMatCmd->SetGuidance("Set the target material.");
  targetMatCmd->SetParameterName("targetMat",true);
  targetMatCmd->SetDefaultValue("Pb");
  candidateList=("LD2 Pb");
  targetMatCmd->SetCandidates(candidateList);
  targetMatCmd->AvailableForStates(G4State_PreInit);

  collimTypeCmd = new G4UIcmdWithAString("/moller/det/setCollimType",this);
  collimTypeCmd->SetGuidance("Set the collimator hole type.");
  collimTypeCmd->SetParameterName("collType",true);
  collimTypeCmd->SetDefaultValue("tube");
  candidateList=("cone tube none");
  collimTypeCmd->SetCandidates(candidateList);
  collimTypeCmd->AvailableForStates(G4State_PreInit);

  scHoleTypeCmd = new G4UIcmdWithAString("/moller/det/setscHoleType",this);
  scHoleTypeCmd->SetGuidance("Set the scattering chamber hole type.");
  scHoleTypeCmd->SetParameterName("scHoleType",true);
  scHoleTypeCmd->SetDefaultValue("attachment");
  candidateList=("attachment round");
  scHoleTypeCmd->SetCandidates(candidateList);
  scHoleTypeCmd->AvailableForStates(G4State_PreInit);

  collimInnerRCmd = new G4UIcmdWithADoubleAndUnit("/moller/det/setCollInnerR",this);
  collimInnerRCmd->SetGuidance("Set the collimator inner radius");
  collimInnerRCmd->SetParameterName("fcollInnerR",true, true);
  collimInnerRCmd->SetDefaultValue(1.905);
  collimInnerRCmd->SetUnitCategory("Length");
  collimInnerRCmd->SetDefaultUnit("cm");
  collimInnerRCmd->AvailableForStates(G4State_PreInit);

  septumCmd = new G4UIcmdWithABool("/moller/det/setDrawSeptum",this);
  septumCmd->SetGuidance("Set whether to draw the septum.");
  septumCmd->SetParameterName("fDrawSeptum",true);
  septumCmd->SetDefaultValue(true);
  septumCmd->AvailableForStates(G4State_PreInit);

  shldConfigCmd = new G4UIcmdWithAnInteger("/moller/det/setShieldConfig",this);
  shldConfigCmd->SetGuidance("Set the shield confiugration.");
  shldConfigCmd->SetParameterName("shldConfig",false);
  shldConfigCmd->SetDefaultValue(0);
  shldConfigCmd->AvailableForStates(G4State_PreInit);

  processPartCmd = new G4UIcmdWithAnInteger("/moller/det/processPart",this);
  processPartCmd->SetGuidance("0: All 1: neutrons only");
  processPartCmd->SetParameterName("processPart",false);
  processPartCmd->SetDefaultValue(0);
  processPartCmd->AvailableForStates(G4State_PreInit);

  hallCmd = new G4UIcmdWithABool("/moller/det/setDrawHall",this);
  hallCmd->SetGuidance("Set whether to draw the hall.");
  hallCmd->SetParameterName("fDrawHall",true);
  hallCmd->SetDefaultValue(false);
  hallCmd->AvailableForStates(G4State_PreInit);

  collNoneCmd = new G4UIcmdWithABool("/moller/det/setDrawCollim",this);
  collNoneCmd->SetGuidance("Set whether to draw the collimator.");
  collNoneCmd->SetParameterName("fDrawCollim",true);
  collNoneCmd->SetDefaultValue(true);
  collNoneCmd->AvailableForStates(G4State_PreInit);

  collOnlyCmd = new G4UIcmdWithABool("/moller/det/setDrawWholeGeometry",this);
  collOnlyCmd->SetGuidance("Set whether to only draw the target and collimator.");
  collOnlyCmd->SetParameterName("fDrawWholeGeo",true);
  collOnlyCmd->SetDefaultValue(false);
  collOnlyCmd->AvailableForStates(G4State_PreInit);

  NumCollCmd = new G4UIcmdWithAnInteger("/moller/det/setNumberofCollimators",this);
  NumCollCmd->SetGuidance("Set the number of collimators.");
  NumCollCmd->SetParameterName("NUM_COLLS",false);
  NumCollCmd->SetDefaultValue(10);
  NumCollCmd->AvailableForStates(G4State_PreInit);

  DetGeomFileCmd = new G4UIcmdWithAString("/moller/det/setDetectorFileName",this);
  DetGeomFileCmd->SetGuidance("Set the Detector geometry file name.");
  DetGeomFileCmd->SetParameterName("detfileName",true);
  DetGeomFileCmd->SetDefaultValue("geometry/detector_fastradModel/detector.gdml");
  DetGeomFileCmd->AvailableForStates(G4State_PreInit);

  ReadGeomFileCmd = new G4UIcmdWithABool("/moller/det/readGeometryFromFile",this);
  ReadGeomFileCmd->SetGuidance("Set to false to use hard-coded geometry.");
  ReadGeomFileCmd->SetParameterName("fReadGeoFile",true);
  ReadGeomFileCmd->SetDefaultValue(false);
  ReadGeomFileCmd->AvailableForStates(G4State_PreInit);

  WriteGeomFileCmd = new G4UIcmdWithAString("/moller/det/writeGeometry",this);
  WriteGeomFileCmd->SetGuidance("Write the geometry to GDML file.");
  WriteGeomFileCmd->SetParameterName("writeFileName",true);
  WriteGeomFileCmd->SetDefaultValue("moller.gdml");
  WriteGeomFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MollerDetectorMessenger::~MollerDetectorMessenger()
{
  delete shieldTypeCmd;
  delete shieldThickCmd;
  delete shieldRadiusCmd;
  delete shieldMatCmd;
  delete collimTypeCmd;
  delete scHoleTypeCmd;
  delete collimInnerRCmd;
  delete targetCenterCmd;
  delete targetLengthCmd;
  delete targetMatCmd;
  delete hallCmd;
  delete collNoneCmd;
  delete collOnlyCmd;
  delete septumCmd;
  delete NumCollCmd;
  delete shldConfigCmd;
  delete processPartCmd;
  delete DetGeomFileCmd;
  delete ReadGeomFileCmd;
  delete detDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MollerDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if (command == shieldTypeCmd ) 
    { myDetector->setShieldTypeName(newValue);}
  else if ( command == shieldThickCmd ) 
    { myDetector->setShieldThick(shieldThickCmd->GetNewDoubleValue(newValue));}
  else if ( command == shieldRadiusCmd ) 
    { myDetector->setShieldRadius(shieldRadiusCmd->GetNewDoubleValue(newValue));}
  else if (command == shieldMatCmd ) 
    { myDetector->setShieldMaterial(newValue);}
  else if (command == shldConfigCmd ) 
    { myDetector->setShieldConfig(shldConfigCmd ->GetNewIntValue(newValue));}
  else if (command == processPartCmd ) 
    { myDetector->SetProcessParticles(processPartCmd ->GetNewIntValue(newValue));}

  else if ( command == targetCenterCmd ) 
    { myDetector->setTargetCenter(targetCenterCmd->GetNewDoubleValue(newValue));}
  else if ( command == targetLengthCmd ) 
    { myDetector->setTargetLength(targetLengthCmd->GetNewDoubleValue(newValue));}
  else if (command == targetMatCmd ) 
    { myDetector->setTargetMaterial(newValue);}

  else if (command == hallCmd ) 
    { myDetector->setDrawHall(hallCmd->GetNewBoolValue(newValue));}

  else if (command == scHoleTypeCmd ) 
    { myDetector->setscHoleTypeName(newValue);}

  else if (command == collimTypeCmd ) 
    { myDetector->setCollimTypeName(newValue);}
  else if ( command == collimInnerRCmd ) 
    { myDetector->setCollimInnerR(collimInnerRCmd->GetNewDoubleValue(newValue));}
  else if (command == collNoneCmd ) 
    { myDetector->setDrawColl(collNoneCmd->GetNewBoolValue(newValue));}
  else if (command == collOnlyCmd ) 
    { myDetector->setDrawWholeGeo(collOnlyCmd->GetNewBoolValue(newValue));}
  else if (command == septumCmd ) 
    { myDetector->setDrawSeptum(septumCmd->GetNewBoolValue(newValue));}
  else if (command == NumCollCmd ) 
    { myDetector->setNumColl(NumCollCmd ->GetNewIntValue(newValue));}

  else if (command == DetGeomFileCmd)
    { myDetector->SetDetectorGeomFile(newValue); }
  else if (command == ReadGeomFileCmd)
    { myDetector->SetReadGeo(ReadGeomFileCmd->GetNewBoolValue(newValue));}
  else if (command == WriteGeomFileCmd)
    { myDetector->WriteGeometryFile(newValue); }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
