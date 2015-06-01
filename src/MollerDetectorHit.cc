#include "MollerDetectorHit.hh"
#include "G4AttCheck.hh"

#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"

G4Allocator<MollerDetectorHit> MollerDetectorHitAllocator;

MollerDetectorHit::MollerDetectorHit()
{
  layerID = -1;
  trackID = -1;

  //  worldPos = 0;
  //  vertexPos = 0;

  kineE=0;
  kineE0=0;
  
  creatorProcess = "";
  particleName = "";
  ion = 0;
}

MollerDetectorHit::MollerDetectorHit(G4int z)
{
  layerID = z;
  trackID = -1;

  //  worldPos = 0;
  //  vertexPos = 0;

  kineE=0;
  kineE0=0;

  creatorProcess = "";
  particleName = "";
  ion = 0;
}

MollerDetectorHit::~MollerDetectorHit()
{;}

MollerDetectorHit::MollerDetectorHit(const MollerDetectorHit &right)
  : G4VHit()
{

  layerID = right.layerID;
  trackID = right.trackID;
  worldPos = right.worldPos;
  vertexPos = right.vertexPos;

  kineE=right.kineE;
  kineE0=right.kineE0;

  creatorProcess = right.creatorProcess;
  particleName = right.particleName;
  ion = right.ion;
}

const MollerDetectorHit& MollerDetectorHit::operator=(const MollerDetectorHit &right)
{

  layerID = right.layerID;
  trackID = right.trackID;
  worldPos = right.worldPos;
  vertexPos = right.vertexPos;

  kineE=right.kineE;
  kineE0=right.kineE0;

  creatorProcess = right.creatorProcess;
  particleName = right.particleName;
  ion = right.ion;

  return *this;
}

int MollerDetectorHit::operator==(const MollerDetectorHit &/*right*/) const
{
  return  0;
}

//std::map<G4String,G4AttDef> MollerDetectorHit::fAttDefs;

void MollerDetectorHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(worldPos);
    circle.SetScreenSize(0.04);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

const std::map<G4String,G4AttDef>* MollerDetectorHit::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("MollerDetectorHit",isNew);

  if (isNew) {
    G4String HitType("HitType");
    (*store)[HitType] = G4AttDef(HitType,"Hit Type","Physics","","G4String");

    G4String ID("ID");
    (*store)[ID] = G4AttDef(ID,"ID","Physics","","G4int");

    G4String trackID("trackID");
    (*store)[trackID] = G4AttDef(trackID,"trackID","Physics","","G4int");

    G4String Pos("Pos");
    (*store)[Pos] = G4AttDef(Pos, "Position",
		      "Physics","G4BestUnit","G4ThreeVector");

    G4String Creator("Creator");
    (*store)[Creator] = G4AttDef(Creator, "CreatorProcess",
				 "Physics","","G4String");

    G4String Name("Name");
    (*store)[Name] = G4AttDef(Name, "ParticleName",
		      "Physics","","G4String");

    G4String Ion("Ion");
    (*store)[Ion] = G4AttDef(Ion, "IsMoller",
    "Physics","","G4bool");
  }
  return store;
}

std::vector<G4AttValue>* MollerDetectorHit::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("HitType","MolleDetectorHit",""));

  values->push_back
    (G4AttValue("ID",G4UIcommand::ConvertToString(layerID),""));

  values->push_back
    (G4AttValue("trackID",G4UIcommand::ConvertToString(trackID),""));

  values->push_back
    (G4AttValue("Pos",G4BestUnit(worldPos,"Length"),""));

  values->push_back
    (G4AttValue("Creator",creatorProcess,""));

  values->push_back
    (G4AttValue("Name",particleName,""));

  values->push_back
    (G4AttValue("Ion",ion,""));

  return values;

}

void MollerDetectorHit::Print()
{  
  G4cout << "  Layer[" << layerID << "] : Particle type: " << particleName 
	 << " created by " << creatorProcess << " \nat (x,y,z) " 
	 << vertexPos.x() << ", " << vertexPos.y() << ", " << vertexPos.z()  
	 << " with KineE0= " << kineE0/CLHEP::GeV << "[GeV]" 
	 << " hit detector \nat (x,y,z) " << worldPos.x()
         << ", " << worldPos.y() << ", " << worldPos.z() 
	 << " with KineE= " << kineE/CLHEP::GeV << "[GeV]\n" 
	 << G4endl;
}


