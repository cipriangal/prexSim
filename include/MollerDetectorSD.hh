#ifndef MollerDetectorSD_h
#define MollerDetectorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "MollerDetectorHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class MollerDetectorSD : public G4VSensitiveDetector
{

public:
  MollerDetectorSD(G4String name,G4int proc=0);
  virtual ~MollerDetectorSD();

  virtual void Initialize(G4HCofThisEvent*HCE);
  virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  virtual void EndOfEvent(G4HCofThisEvent*HCE);
  inline void SetVolume(G4int ivol)
  { volume_extra = ivol; }
  inline G4int GetVolume() const 
  { return volume_extra; }
private:
  MollerDetectorHitsCollection *hitsCollection;
  G4int HCID;
  G4int volume_extra;
  G4int processPart;

};




#endif

