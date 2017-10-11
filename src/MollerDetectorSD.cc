


#include "MollerDetectorSD.hh"
#include "MollerDetectorHit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "TString.h"

#include "G4Track.hh"
#include "G4SDManager.hh"
#include "G4Navigator.hh"
#include "G4VProcess.hh"

MollerDetectorSD::MollerDetectorSD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="hitsColl");
  HCID = -1;
  volume_extra = 0;
}

MollerDetectorSD::~MollerDetectorSD(){;}

void MollerDetectorSD::Initialize(G4HCofThisEvent* HCE)
{
  hitsCollection = new MollerDetectorHitsCollection
                      (SensitiveDetectorName,collectionName[0]); 
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection); }
  HCE->AddHitsCollection(HCID,hitsCollection);

}

G4bool MollerDetectorSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{


  const G4Track* fTrack = aStep->GetTrack();

  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable
    = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4VPhysicalVolume* theMotherPhysical = theTouchable->GetVolume(1); // mother
  G4int copyNo = theMotherPhysical->GetCopyNo();


  G4ThreeVector worldPos = preStepPoint->GetPosition();
  G4ThreeVector vertexPos = fTrack->GetVertexPosition();
  G4ThreeVector momentum = fTrack->GetMomentumDirection();
  G4ThreeVector vertexMomentum = fTrack->GetVertexMomentumDirection();

  /*
  // Energy deposited - from ExN05
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep<=0.) return false;
  if(CellID[copyID]==-1){
    ExN05CalorimeterHit* calHit = new ExN05CalorimeterHit(physVol->GetLogicalVolume());
    calHit->SetEdep( edep );  
  } else {
    (*CalCollection)[CellID[copyID]]->AddEdep( edep );
  }
  */

  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep<0.) return false;

  const G4VProcess* creatorProcess = fTrack->GetCreatorProcess();
  G4String process;
  if (creatorProcess!=0) process = creatorProcess->GetProcessName();

  G4bool ion = (process=="eIoni");

  G4String particle = fTrack->GetDefinition()->GetParticleName();

  // Need to get a number from the volume name somehow...
  //G4String volume = fTrack->GetVolume()->GetName();
  char volumeName[20] = "sphere_det_"; /* volume=100 + i where sphere_det_i Rakitha on Wed May 15 15:28:26 EDT 2013*/
  char volumeNameAlt0[20] = "plane_det_";/* volume= i where plane_det_i */
  char volumeNameAlt1[20] = "target"; /* volume=1000  */
  char volumeNameAlt2[20] = "other_";/* volume=2000 + i where other_i */
  char volumeNameAlt3[20] = "cyl_det_";/* volume=3000 + i where cyl_det_i */
  char volumeNameAlt4[20] = "septum";/* volume=4001  */

  G4int offset =100; /*changed from 0 to 100+i. Rakitha on Wed May 15 15:28:26 EDT 2013 */
  G4int alt0offset = 0;
  G4int alt1offset =1000;
  G4int alt2offset =2000; 
  G4int alt3offset =3000;
  G4int volume=-1000;
  //G4int vol; //not in use
  TString vol_string(fTrack->GetVolume()->GetName().data());
  if (volume_extra!=0) volume = volume_extra; /* if the aux detector name is defined at the geometry file for a sensitive detector.  Rakitha on Wed May 15 15:28:26 EDT 2013 */
  else if ( vol_string.BeginsWith(volumeName) ) { /* sphere_det_  */
    vol_string.Remove(0,11);/* remove 11 starting from 0*/
    volume = offset + vol_string.Atoi(); 
  }
  else if ( vol_string.BeginsWith(volumeNameAlt0) ) { /* plane_det_  */
    vol_string.Remove(0,10);/* remove 10 starting from 0*/
    volume = alt0offset + vol_string.Atoi(); 
    //   G4cout << "volume =" << volume << G4endl;
  }
  else if ( vol_string.BeginsWith(volumeNameAlt1) ) {  /* target  */
    vol_string.Remove(0,6);
    volume = alt1offset + vol_string.Atoi(); 
  }
  else if ( vol_string.BeginsWith(volumeNameAlt2) ) {  /* other_ */
    vol_string.Remove(0,6);
    volume = alt2offset + vol_string.Atoi(); 
    //   G4cout << "volume =" << volume << G4endl;
  }
  else if ( vol_string.BeginsWith(volumeNameAlt3) ) {  /* cyl_det_ */
    vol_string.Remove(0,8);
    volume = alt3offset + vol_string.Atoi(); 
    //   G4cout << "volume =" << volume << G4endl;
  }
  else if ( vol_string.EqualTo(volumeNameAlt4) ) {  /* septum */
    volume = 4001; 
    //   G4cout << "volume =" << volume << G4endl;
  }
  else
    volume=-1000; /* Undefined sensitive detector with different kind of volume name.  Rakitha on Wed May 15 15:28:26 EDT 2013 */

 
  G4int trackID = fTrack->GetTrackID();
  G4int parentID = fTrack->GetParentID();
  G4int pdgID = fTrack->GetDefinition()->GetPDGEncoding();
  G4int trackstatusID = (int)fTrack->GetTrackStatus();

  G4int partType;

  if (particle=="e-") partType=0;
  else if (particle=="e+") partType=1;
  else if (particle=="proton") partType=2;
  else if (particle=="antiproton") partType=3;
  else if (particle=="gamma") partType=4;
  else if (particle=="neutron") partType=5;
  else partType=6;

  G4double kineE = fTrack->GetKineticEnergy();
  G4double kineE0 = fTrack->GetVertexKineticEnergy();

  
  //G4double rad_dist = sqrt(worldPos.x()*worldPos.x() + 
  //worldPos.y()*worldPos.y());

  G4double scat_ang = sqrt(vertexMomentum.x()*vertexMomentum.x() + 
			   vertexMomentum.y()*vertexMomentum.y());

  MollerDetectorHit* newHit = new MollerDetectorHit(copyNo);
    
    
  newHit->SetWorldPos(worldPos);
  newHit->SetVertexPos(vertexPos);
  newHit->SetMomentum(momentum);
  newHit->SetKineticEnergy(kineE);  
  newHit->SetMomentum2(vertexMomentum);
  newHit->SetKineticEnergy2(kineE0);
  newHit->SetScatAngle(scat_ang);
  newHit->SetCreatorProcess(process);
  newHit->SetParticleName(particle);  
  newHit->SetEdep( edep ); ;   
  newHit->SetIon(ion);
  newHit->SetType(partType);  
  newHit->SetVolume(volume);
  //G4cout << "volume =" << volume << G4endl;
  newHit->SetTrackID(trackID);
  newHit->SetParentID(parentID);
  newHit->SetPDG(pdgID);
  newHit->SetTrackStatus(trackstatusID);
  /* 
    G4cout<<"Hit values in ProcessHits:"<<G4endl;

    G4cout<<"particle, volume, trackID, edep"<<G4endl;
    G4cout<<particle<<", "<<volume<<", "<<trackID<<", "<<edep<<G4endl;

    
    //G4cout<<"worldPos, vertexPos, momentum, kineE, vertexMomentum, kineE0, scat_ang, process, particle, ion, partType, volume, trackID, edep"<<G4endl;
    //G4cout<<worldPos<<", "<<vertexPos<<", "<<momentum<<", "<<kineE<<", "<<vertexMomentum<<", "<<kineE0<<", "<<scat_ang<<", "<<process<<", "<<particle<<", "<<ion<<", "<<partType<<", "<<volume<<", "<<trackID<<", "<<edep<<G4endl;
    //G4cout<<worldPos[0]<<", "<<worldPos[1]<<", "<<worldPos[2]<<", "<<vertexPos[0]<<", "<<vertexPos[1]<<", "<<vertexPos[2]<<", "<<momentum[0]<<", "<<momentum[1]<<", "<<momentum[2]<<", "<<kineE<<", "<<vertexMomentum[0]<<", "<<vertexMomentum[1]<<", "<<vertexMomentum[2]<<", "<<kineE0<<", "<<scat_ang<<", "<<process<<", "<<particle<<", "<<ion<<", "<<partType<<", "<<volume<<", "<<trackID<<G4endl;
    
    G4cout<<G4endl;
    */
	if (!(volume == 1006 && edep < 30)){
  	hitsCollection->insert( newHit );}
  
  return true;
}

void MollerDetectorSD::EndOfEvent(G4HCofThisEvent*)
{
}


