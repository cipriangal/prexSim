

#ifndef MollerDetectorHit_h
#define MollerDetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
//#include "G4ParticleDefinition.hh"  // Include from 'particle+matter'
#include "G4ios.hh"

class G4AttDef;
class G4AttValue;

class MollerDetectorHit : public G4VHit
{
public:
  
  // Constructor/Destrcutor

  MollerDetectorHit();
  MollerDetectorHit(G4int z);
  virtual ~MollerDetectorHit();
  MollerDetectorHit(const MollerDetectorHit &right);
  const MollerDetectorHit& operator=(const MollerDetectorHit &right);
  int operator==(const MollerDetectorHit &right) const;
  
  // Operators
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
  
  
  // Other member functions
  virtual void Draw();
  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;
  virtual void Print();
  
  //G4ParticleDefinition* GetParticleDefinition();
  
  // Get/Set functions
  inline void SetLayerID(G4int z) 
  { layerID = z; }
  inline G4int GetLayerID() const 
  { return layerID; }

  inline void SetTrackID(G4int id) 
  { trackID = id; }
  inline G4int GetTrackID() const 
  { return trackID; }

  inline void SetParentID(G4int id) 
  { parentID = id; }
  inline G4int GetParentID() const 
  { return parentID; }

  inline void SetPDG(G4int id) 
  { pdgID = id; }
  inline G4int GetPDG() const 
  { return pdgID; }

  inline void SetTrackStatus(G4int id) 
  { trackstatusID = id; }
  inline G4int GetTrackStatus() const 
  { return trackstatusID; }

  inline void SetWorldPos(G4ThreeVector xyz) 
  { worldPos = xyz; }
  inline G4ThreeVector GetWorldPos() const 
  { return worldPos; }
  inline void SetVertexPos(G4ThreeVector xyz) 
  { vertexPos = xyz; }
  inline G4ThreeVector GetVertexPos() const 
  { return vertexPos; }

  inline void SetMomentum(G4ThreeVector xyz) 
  { momentum = xyz; }
  inline G4ThreeVector GetMomentum() const 
  { return momentum; }
  inline void SetKineticEnergy(G4double e)
  { kineE = e; }
  inline G4double GetKineticEnergy()
  { return kineE; }
  

  inline void SetMomentum0(G4ThreeVector xyz) 
  { p0 = xyz; }
  inline G4ThreeVector GetMomentum0() const 
  { return p0; }
  inline void SetMomentum1(G4ThreeVector xyz) 
  { p1 = xyz; }
  inline G4ThreeVector GetMomentum1() const 
  { return p1; }
  inline void SetMomentum2(G4ThreeVector xyz) 
  { p2 = xyz; }
  inline G4ThreeVector GetMomentum2() const 
  { return p2; }

  inline void SetKineticEnergy0(G4double e)
  { kineE0 = e; }
  inline G4double GetKineticEnergy0()
  { return kineE0; }
  inline void SetKineticEnergy1(G4double e)
  { kineE1 = e; }
  inline G4double GetKineticEnergy1()
  { return kineE1; }
  inline void SetKineticEnergy2(G4double e)
  { kineE2 = e; }
  inline G4double GetKineticEnergy2()
  { return kineE2; }

  inline void SetScatAngle(G4double ang)
  { scat_ang = ang; }
  inline G4double GetScatAngle()
  { return scat_ang; }

  inline void  SetCreatorProcess(G4String creator)
  { creatorProcess = creator; }
  inline G4String GetCreatorProcess() const 
  { return creatorProcess; }
  inline void  SetParticleName(G4String name)  
  { particleName = name; }
  inline G4String GetParticleName() const 
  { return particleName; }

  inline void SetIon(G4bool b)
  { ion = b; }
  inline G4bool GetIon() const 
  { return ion; }
  inline void SetType(G4int i)
  { partType = i; }
  inline G4int GetType() const 
  { return partType; }
  inline void SetVolume(G4int ivol)
  { volumed = ivol; }
  inline G4int GetVolume() const 
  { return volumed; }

  inline void SetEdep(G4double de)
  { edep = de; };
  inline G4double GetEdep()
  { return edep; };

  
private:
  
  G4int layerID, partType, trackID, volumed, parentID, pdgID, trackstatusID;

  G4double kineE, kineE0, kineE1, kineE2, scat_ang, edep;

  G4ThreeVector worldPos;
  G4ThreeVector vertexPos;

  G4ThreeVector momentum;

  G4ThreeVector p0;
  G4ThreeVector p1;
  G4ThreeVector p2;

  //G4ParticleDefinition* thisParticleDefinition;
  G4String creatorProcess;
  G4String particleName;
  G4bool ion;
  
  //static std::map<G4String,G4AttDef> fAttDefs;
  


};

typedef G4THitsCollection<MollerDetectorHit> MollerDetectorHitsCollection;

extern G4Allocator<MollerDetectorHit> MollerDetectorHitAllocator;

inline void* MollerDetectorHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) MollerDetectorHitAllocator.MallocSingle();
  return aHit;
}

inline void MollerDetectorHit::operator delete(void *aHit)
{
  MollerDetectorHitAllocator.FreeSingle((MollerDetectorHit*) aHit);
}

#endif
