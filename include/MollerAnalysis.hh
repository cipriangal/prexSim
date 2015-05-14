
/**********************************************************/
/*      This header creates the ROOT TNtuple and          */
/*      initializes the RootAnalysis class.               */
/**********************************************************/



#ifndef MollerAnalysis_h
#define MollerAnalysis_h 1

#include "RootAnalysis.hh"
#include "MollerAnalysisMessenger.hh"
#include "MollerDetectorConstruction.hh"
#include "MollerDetectorHit.hh"
#include "MollerGlobalMagnetField.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4TrackStatus.hh"
#include "G4Material.hh"
#include "G4Types.hh"
#include "TNtuple.h"
#include "G4ThreeVector.hh"

#include <time.h>

#if defined (G4UI_USE_ROOT) || defined (G4UI_BUILD_ROOT_SESSION)
#if !defined (G4ANALYSIS_USE_ROOT)
#define G4ANALYSIS_USE_ROOT 1
#endif /* !defined (G4ANALYSIS_USE_ROOT) */
#endif /* defined (G4UI_USE_ROOT) || defined (G4UI_BUILD_ROOT_SESSION) */

class G4VPhysicalVolume;
class G4Event;
class G4Run;
class G4Track;
class G4Material;
class G4Step;

class TNtuple;
class MollerAnalysisMessenger;
class MollerDetectorConstruction;
class MollerAnalysis : public RootAnalysis {

public:
  MollerAnalysis();
  MollerAnalysis(MollerDetectorConstruction*);
  MollerAnalysis(const MollerAnalysis &right);
  const MollerAnalysis& operator=(const MollerAnalysis &right);
  ~MollerAnalysis();

public:
  // G4UserRunAction
  void BeginOfRunAction(const G4Run*); //used
  void EndOfRunAction(const G4Run*);   //used
  
  // G4UserEventAction
  void BeginOfEventAction(const G4Event*); //used
  void EndOfEventAction(const G4Event*); //used
  
  // G4UserSteppingAction
  void UserSteppingAction(const G4Step*); //used

  void SetRootFileName(const G4String&);

  void SetMomentum0(G4double E0, G4double px, G4double py, G4double pz) {
    kineE0=E0;
    px0=px;
    py0=py;
    pz0=pz;
    theta0 = sqrt(px0*px0 + py0*py0);
  }
  void SetMomentum1(G4double E1, G4double px, G4double py, G4double pz) {
    kineE1=E1;
    px1=px;
    py1=py;
    pz1=pz;
    theta1 = sqrt(px1*px1 + py1*py1);
  }
  void SetMomentum2(G4double E2, G4double px, G4double py, G4double pz) {
    kineE2=E2;
    px2=px;
    py2=py;
    pz2=pz;
    theta2 = sqrt(px2*px2 + py2*py2);
  }

  void SetDiffXS(G4double diffXS_in) {
    diffXS = diffXS_in;
  }

  void SetTotXS(G4double totXS_in) {
    totXS = totXS_in;
  }

  void SetRate(G4double rate_in) {
    rate = rate_in;
  }

  void SetProcess(G4int i) {
    index=i;
  }

  void AddData(MollerDetectorHit *aHit){

  fntup[0] = aHit->GetIon();
  fntup[1] = aHit->GetWorldPos().x();
  fntup[2] = aHit->GetWorldPos().y();
  fntup[3] = aHit->GetWorldPos().z();
  //fntup[4] = (Float_t)x0;
  //fntup[5] = (Float_t)y0;
  //fntup[6] = (Float_t)z0;
  fntup[4] = aHit->GetVertexPos().x();
  fntup[5] = aHit->GetVertexPos().y();
  fntup[6] = aHit->GetVertexPos().z();
  fntup[7] = aHit->GetKineticEnergy();
  fntup[8] = aHit->GetMomentum().x();
  fntup[9] = aHit->GetMomentum().y();
  fntup[10] = aHit->GetMomentum().z();
  
  fntup[11] = (Float_t)kineE0;
  fntup[12] = (Float_t)px0;
  fntup[13] = (Float_t)py0;
  fntup[14] = (Float_t)pz0;
  fntup[15] = (Float_t)kineE1;
  fntup[16] = (Float_t)px1;
  fntup[17] = (Float_t)py1;
  fntup[18] = (Float_t)pz1;
  /*
  fntup[19] = (Float_t)kineE2;
  fntup[20] = (Float_t)px2;
  fntup[21] = (Float_t)py2;
  fntup[22] = (Float_t)pz2;
  */
  fntup[19] = aHit->GetKineticEnergy2();
  fntup[20] = aHit->GetMomentum2().x();
  fntup[21] = aHit->GetMomentum2().y();
  fntup[22] = aHit->GetMomentum2().z();

  fntup[23] = aHit->GetType();
  fntup[24] = (Float_t)aHit->GetVolume();
  fntup[25] = (Float_t)theta0;
  fntup[26] = (Float_t)theta1;
  fntup[27] = (Float_t)theta2;
  fntup[28] = (Float_t)ev_num;
  fntup[29] = (Float_t)splitProcess;
  fntup[30] = (Float_t)event;
  fntup[31] = (Float_t)creatorProcess;
  
  if (!index&&
      (aHit->GetIon()||splitProcess==0)&&
      kineE0>0&&
      aHit->GetType()==0
      ) {
    
    if (aHit->GetIon()) {
      fntup[32] = 2;
      fntup[33] = kineE2;
      fntup[34] = theta2;
    }
    else {
      fntup[32] = 1;
      fntup[33] = kineE1;
      fntup[34] = theta1;
    }
  }
  else if (aHit->GetTrackID()==1){ 
    fntup[32] = 1;
    fntup[33] = kineE1;
    fntup[34] = theta1;
  }
  else if (aHit->GetTrackID()==2){ 
    fntup[32] = 2;
    fntup[33] = kineE2;
    fntup[34] = theta2;
  }
  else { 
    fntup[32] = 0;
    fntup[33] = 0;
    fntup[34] = 0;
  }
  
  fntup[35] = aHit->GetTrackID();
  
  fntup[36] = aHit->GetParentID();
  fntup[37] = aHit->GetPDG();
  fntup[38] = rate; /*Not in use I guess. all values are zero. Rakitha on Fri May 10 14:44:02 EDT 2013*/  
  fntup[39] = aHit->GetEdep();
  fntup[40] = aHit->GetTrackStatus();

  /*
  G4cout<<"Ntuple variables in AddData:"<<G4endl;

  //  for (Int_t i=0;i<40;i++){
  //    G4cout<<fntup[i]<<", ";  
  //}
  G4cout<<" Event,   type,   volume,   track,   edep"<<G4endl;
  G4cout<<fntup[30]<<", "<<fntup[23]<<", "<<fntup[24]<<", "<<fntup[35]<<", "<<fntup[39];
  G4cout<<G4endl;

  */
  ntup->Fill(fntup);
}

  void AddGDMLFileName(const G4String&);
  void AddMacro_1(const G4String&);
  void AddMacro_2(const G4String&);
  void SetLowLimMagneticShield(G4double value) {mg_field_low = value; };
  void SetHighLimMagneticShield(G4double value) { mg_field_high = value; };

  void UpdateSimInputs();

private:

  static const G4bool doDIPS=0;
  static const G4bool doCOLS=0;
  static const G4bool doQUADS=0;

  // When adding additional collimators this number must be increased
  //static const G4int NUM_DETS = 17;
  MollerDetectorConstruction*    myDetector;  //pointer to the geometry
  MollerGlobalMagnetField*      pGlobalMagnetField; // pointer to global magnetic field

  G4int NUM_DETS;

  MollerAnalysisMessenger*  analysisMessenger;   //messenger of this class
  G4String rootfileName;
  G4String GDMLfileName;
  G4String MacrofileName_1;
  G4String MacrofileName_2;
  G4double mg_field_low;
  G4double mg_field_high;

  G4int ev_num, index;
  G4int partType, splitProcess, creatorProcess; 

  TNtuple *ntup;
  Float_t fntup[41];

  std::vector<G4int> hitsCollID;

  G4int verboseLevel;
  
  G4double generator, event;

  G4double kineE0;
  G4double kineE1;
  G4double kineE2;

  G4double px0;
  G4double py0;
  G4double pz0;
  G4double px1;
  G4double py1;
  G4double pz1;
  G4double px2;
  G4double py2;
  G4double pz2;

  G4double theta0;
  G4double theta1;
  G4double theta2;

  G4double x0;
  G4double y0;
  G4double z0;

  G4double rate;
  G4double totXS;
  G4double diffXS;

};


#endif
  


