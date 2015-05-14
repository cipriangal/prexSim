

#ifndef RootAnalysis_h
#define RootAnalysis_h 1

#include "TROOT.h"
#include "TFile.h"
#include "TList.h"
#include "TNtuple.h"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "MollerDetectorHit.hh"


class G4Event;
class G4Run;
class G4Step;

class RootAnalysis;
extern RootAnalysis *gRootAnalysis; //global RootAnalysis

class RootAnalysis {

public:

  TFile *hfile;

public:

  RootAnalysis() {
    if (gRootAnalysis)
      delete gRootAnalysis;
    gRootAnalysis = this;

  }

  virtual ~RootAnalysis() {
    if (gRootAnalysis == this)
      gRootAnalysis = (RootAnalysis *)0;
  }

  // G4UserRunAction
  virtual void BeginOfRunAction(const G4Run* /*aRun*/) {;}

  virtual void EndOfRunAction(const G4Run* /*aRun*/) {;}
  
  // G4UserEventAction
  virtual void BeginOfEventAction(const G4Event* /*anEvent*/) {;}
  virtual void EndOfEventAction(const G4Event* /*anEvent*/) {;}
  
  // G4UserSteppingAction
  virtual void UserSteppingAction(const G4Step* /*aStep*/) {;}

  virtual void SetMomentum0(G4double /*E0*/, G4double /*p0*/,
			    G4double /*E0*/, G4double /*p0*/) {
  }
  virtual void SetMomentum1(G4double /*E0*/, G4double /*p0*/,
			    G4double /*E0*/, G4double /*p0*/) {
  }
  virtual void SetMomentum2(G4double /*E0*/, G4double /*p0*/,
			    G4double /*E0*/, G4double /*p0*/) {
  }

  virtual void SetDiffXS(G4double) {
  }
  virtual void SetTotXS(G4double) {
  }
  virtual void SetRate(G4double) {
  }

  virtual void SetProcess(G4int) {
  }

  virtual void AddData(MollerDetectorHit* /*aHit*/) {
  }

  virtual void End() {
  }

};

#endif

