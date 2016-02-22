 
#include "MollerAnalysis.hh"
#include "MollerDetectorHit.hh"
#include "RootAnalysis.hh"

#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include "G4RunManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4Track.hh"
#include "G4Material.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4TrackStatus.hh"
#include "G4Step.hh"
#include "G4Types.hh"

#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4TrackStatus.hh"

#include "MollerAnalysisMessenger.hh"
#include "MollerDetectorConstruction.hh"

#include <time.h>
#include <iostream>

#include "TROOT.h"
#include "TNtuple.h"
#include "TApplication.h"
#include "TSystem.h"

#include "TMacro.h"

using namespace std;

MollerAnalysis::MollerAnalysis()
{
  if (gSystem) gSystem->ProcessEvents();
  
  ev_num=0;
  partType=-1;
  splitProcess=-1;
  creatorProcess=-1;
  index=-1;

  //  for (int i=0; i<NUM_DETS; i++) {
  //    hitsCollID[i] = -1;
  //  }

  verboseLevel=0;

  kineE0=0;  kineE1=0;  kineE2=0;

  px0=0;  py0=0;  pz0=0;
  px1=0;  py1=0;  pz1=0;
  px2=0;  py2=0;  pz2=0;

  theta0=0;  theta1=0;  theta2=0;

  x0=0;  y0=0;  z0=0;

  analysisMessenger = new MollerAnalysisMessenger(this);
  //G4cout << "Root file name is "<<rootfileName << G4endl;
}
MollerAnalysis::MollerAnalysis(MollerDetectorConstruction* detector)
:myDetector(detector)
{
  if (gSystem) gSystem->ProcessEvents();

  

  ev_num=0;
  partType=-1;
  splitProcess=-1;
  creatorProcess=-1;
  index=-1;

  //  for (int i=0; i<NUM_DETS; i++) {
  //    hitsCollID[i] = -1;
  //  }

  verboseLevel=0;

  kineE0=0;  kineE1=0;  kineE2=0;

  px0=0;  py0=0;  pz0=0;
  px1=0;  py1=0;  pz1=0;
  px2=0;  py2=0;  pz2=0;

  theta0=0;  theta1=0;  theta2=0;

  x0=0;  y0=0;  z0=0;

  analysisMessenger = new MollerAnalysisMessenger(this);
  //G4cout << "Root file name is "<<rootfileName << G4endl;
}

MollerAnalysis::MollerAnalysis(const MollerAnalysis &right)
  : RootAnalysis()
{
  if (gSystem) gSystem->ProcessEvents();
  
  ev_num=right.ev_num;
  partType=right.partType;
  splitProcess=right.splitProcess;
  creatorProcess=right.creatorProcess;
  index=right.index;
  

  //  for (int i=0; i<NUM_DETS; i++) {
  //    hitsCollID[i]=right.hitsCollID[i];
  //  }
  
  verboseLevel=right.verboseLevel;

  kineE0=right.kineE0;  
  kineE1=right.kineE1;  
  kineE2=right.kineE2;
 
  px0=right.px0;  
  py0=right.py0;  
  pz0=right.pz0;
  
  px1=right.px1;  
  py1=right.py1;  
  pz1=right.pz1;

  px2=right.px2;  
  py2=right.py2;  
  pz2=right.pz2;

  theta0=right.theta0;  
  theta1=right.theta1;  
  theta2=right.theta2;

  x0=right.x0;  
  y0=right.y0;  
  z0=right.z0;

}

const MollerAnalysis& MollerAnalysis::operator=(const MollerAnalysis &right)
{
  if (gSystem) gSystem->ProcessEvents();
  
  ev_num=right.ev_num;
  partType=right.partType;
  splitProcess=right.splitProcess;
  creatorProcess=right.creatorProcess;
  index=right.index;
  
  //  for (int i=0; i<NUM_DETS; i++) {
  //    hitsCollID[i]=right.hitsCollID[i];
  //  }
  
  verboseLevel=right.verboseLevel;

  kineE0=right.kineE0;  
  kineE1=right.kineE1;  
  kineE2=right.kineE2;
  
  px0=right.px0;  
  py0=right.py0;  
  pz0=right.pz0;
  
  px1=right.px1;  
  py1=right.py1;  
  pz1=right.pz1;

  px2=right.px2;  
  py2=right.py2;  
  pz2=right.pz2;

  theta0=right.theta0;  
  theta1=right.theta1;  
  theta2=right.theta2;

  x0=right.x0;  
  y0=right.y0;  
  z0=right.z0;

  return *this;

}

MollerAnalysis::~MollerAnalysis()
{
  delete analysisMessenger;
}
void MollerAnalysis::SetRootFileName(const G4String& nam) 
{
  rootfileName = nam;
}

void MollerAnalysis::AddGDMLFileName(const G4String& nam) 
{
  GDMLfileName = nam;
}

void MollerAnalysis::AddMacro_1(const G4String& nam) 
{
  MacrofileName_1 = nam;
}

void MollerAnalysis::AddMacro_2(const G4String& nam) 
{
  MacrofileName_2 = nam;
}

void MollerAnalysis::UpdateSimInputs()
{
  //adding GDML file to a TList
  FILE *fp = fopen(GDMLfileName,"r");
  char line[200];
  TList * list_gdml = new TList;
  while (fgets(line,80,fp)) {
    list_gdml->Add(new TObjString(line));
  }
  hfile->WriteObject(list_gdml,"GDML_List");

  fclose (fp);
  fp = fopen(MacrofileName_1,"r");
  TList * list_mac_1 = new TList;
  while (fgets(line,80,fp)) {
    list_mac_1->Add(new TObjString(line));
  }
  hfile->WriteObject(list_mac_1,"macro_1_List");

  fclose (fp);
  fp = fopen(MacrofileName_2,"r");
  TList * list_mac_2 = new TList;
  while (fgets(line,80,fp)) {
    list_mac_2->Add(new TObjString(line));
  }
  hfile->WriteObject(list_mac_2,"macro_2_List");
  fclose (fp); 
}

void MollerAnalysis::BeginOfRunAction(const G4Run* /*aRun*/)
{
  if (gSystem) gSystem->ProcessEvents();

  G4cout << "Run Action has been entered\n" << G4endl;

  G4int NUM_COLS = myDetector->getNumColl();

  NUM_DETS = NUM_COLS;
  G4cout<<G4endl<<G4endl<<"NUM_DETS = "<<NUM_DETS<<G4endl<<G4endl;

  G4cout << "Root file name is "<<rootfileName << G4endl;


  // This works:
  G4String filenamechar=(rootfileName+".root");
    
  G4cout << "Root file name is "<<filenamechar << G4endl;   

  hfile = new TFile(filenamechar,"RECREATE","Root/G4 analysis");

  string myntuple_string;
  //                0  1 2 3 4  5  6  7     8  9  10 11     12  13  14  15     16  17  18  19     20  21  22  23   24     25     26     27     28     29      30    31      32  33        34        35    36     37    38   39
  myntuple_string="ion:x:y:z:x0:y0:z0:kineE:px:py:pz:kineE0:px0:py0:pz0:kineE1:px1:py1:pz1:kineE2:px2:py2:pz2:type:volume:theta0:theta1:theta2:ev_num:process:event:creator:hit:kineE_org:theta_org:track:parent:PDGid:rate:Edeposit:trackstatus";

  char *string_ntup = new char[myntuple_string.length()+1];
  strncpy (string_ntup,myntuple_string.c_str(),myntuple_string.length()+1);

  ntup = new TNtuple("geant","G4 Results",(char *)string_ntup);

  //Write GDML and macro files to the root file
  UpdateSimInputs();

  G4cout << "Run Action has been completed\n" << G4endl;
}

void MollerAnalysis::BeginOfEventAction(const G4Event */*anEvent*/)
{
  ev_num++;
  if(ev_num%100000==1) G4cout<<"at event: "<<ev_num<<G4endl;
  
  if (gSystem) gSystem->ProcessEvents();

  G4SDManager * SDman = G4SDManager::GetSDMpointer();

  if((int) hitsCollID.size()==0)   
    {
      //   G4cout<<G4endl<<G4endl<<"Setting hitsCollID"<<G4endl<<G4endl;
     
      for (int i=0; i<NUM_DETS; i++) {
	G4String colNam = "detector";
	colNam+=G4UIcommand::ConvertToString(i+1);
	colNam+="/hitsColl";
	
	G4cout << colNam << G4endl;
	
	hitsCollID.push_back (SDman->GetCollectionID(colNam));
      }      
    }


  gRootAnalysis->SetProcess(1);
}


void MollerAnalysis::UserSteppingAction(const G4Step *aStep)
{
  /** Get data that will go into ROOT tree **/

  G4Track* fTrack = aStep->GetTrack();


  if (fTrack->GetTrackStatus()!=fAlive) { 
 //   printf("track is not 'alive.'\n");
    return; 
  }

  G4int tracking = 1;

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();
  G4Material* material = fTrack->GetMaterial();

  const G4VProcess* originProcess = fTrack->GetCreatorProcess();
  const G4VProcess* stepProcess = thePostPoint->GetProcessDefinedStep();

  G4String process;
  if (originProcess!=0) process = originProcess->GetProcessName();
  G4String process1;
  if (stepProcess!=0) process1 = stepProcess->GetProcessName();


  //  G4String volume = fTrack->GetVolume()->GetName();
  G4String particle = fTrack->GetDefinition()->GetParticleName();


  if (process=="eIoni") creatorProcess=0;
  else if (process=="conv") creatorProcess=1;
  else if (process=="Decay") creatorProcess=2;
  else if (process=="annihil") creatorProcess=3;
  else if (process=="eBrem") creatorProcess=4;
  else if (process=="phot") creatorProcess=5;
  else if (process=="compt") creatorProcess=6;
  else if (process=="Transportation") creatorProcess=7;
  else if (process=="msc") creatorProcess=8;
  else creatorProcess=9;

  //G4bool ion = process=="eIoni";

  // Added by Lorenzo in order to have the process at each step. Before split process was just recorded (see 
  // conditions below after particle== "e-"
  if (process1=="eIoni") splitProcess=0;
  else if (process1=="conv") splitProcess=1;
  else if (process1=="Decay") splitProcess=2;
  else if (process1=="annihil") splitProcess=3;
  else if (process1=="eBrem") splitProcess=4;
  else if (process1=="photo") splitProcess=5;
  else if (process1=="compton") splitProcess=6;
  else if (process1=="Transportation") splitProcess=7;
  else splitProcess=8;
  // finish addition
  
  if ((particle == "e-") 
      && (fTrack->GetVertexKineticEnergy()==1*GeV) 
      && ((thePrePoint->GetKineticEnergy()-
	   thePostPoint->GetKineticEnergy())>0.100*GeV) 
      && (process1=="eIoni"))
    { 
      if (kineE0==0 || kineE1==0) {
	kineE0 = thePrePoint->GetKineticEnergy();
	kineE1 = thePostPoint->GetKineticEnergy();
	px0 = thePrePoint->GetMomentumDirection().x();
	py0 = thePrePoint->GetMomentumDirection().y();
	pz0 = thePrePoint->GetMomentumDirection().z();
	px1 = thePostPoint->GetMomentumDirection().x();
	py1 = thePostPoint->GetMomentumDirection().y();
	pz1 = thePostPoint->GetMomentumDirection().z();
	theta0 = sqrt(px0*px0 + py0*py0);
	theta1 = sqrt(px1*px1 + py1*py1);
	x0 = thePostPoint->GetPosition().x();
	y0 = thePostPoint->GetPosition().y();
	z0 = thePostPoint->GetPosition().z();
      }
      /* The parameter splitProcess is added to ntuple( index-29, process). Rakitha on Fri May 10 14:44:02 EDT 2013 */
      if (process1=="eIoni") splitProcess=0;
      else if (process1=="conv") splitProcess=1;
      else if (process1=="Decay") splitProcess=2;
      else if (process1=="annihil") splitProcess=3;
      else if (process1=="eBrem") splitProcess=4;
      else if (process1=="photo") splitProcess=5;
      else if (process1=="compton") splitProcess=6;
      else if (process1=="Transportation") splitProcess=7;
      else splitProcess=8;
    }
  
  if (tracking==1) { /*parameter ``tracking'' may be a flag. Rakitha on Fri May 10 14:44:02 EDT 2013*/

    G4ThreeVector worldPos = fTrack->GetPosition();
    G4ThreeVector worldMomentum = fTrack->GetMomentumDirection();
    G4ThreeVector vertexPos = fTrack->GetVertexPosition();
    G4ThreeVector vertexMomentum = fTrack->GetVertexMomentumDirection();
    G4int trackID = fTrack->GetTrackID();
    G4int parentID = fTrack->GetParentID();
    G4int pdgID = fTrack->GetDefinition()->GetPDGEncoding();
    G4int trackstatusID = (int)fTrack->GetTrackStatus();


    G4double scat_ang = sqrt(vertexMomentum.x()*vertexMomentum.x() + 
			     vertexMomentum.y()*vertexMomentum.y());
    
    G4double kineE = fTrack->GetKineticEnergy();
    G4double kineE0 = fTrack->GetVertexKineticEnergy();
    
    G4bool ion = (process=="eIoni");
  
    if (particle=="e-") partType=0;
    else if (particle=="e+") partType=1;
    else if (particle=="proton") partType=2;
    else if (particle=="antiproton") partType=3;
    else if (particle=="gamma") partType=4;
    else if (particle=="neutron") partType=5;
    else partType=6;

  }
}

void MollerAnalysis::EndOfEventAction(const G4Event *anEvent)
{
  
  if (gSystem) gSystem->ProcessEvents();
  
  G4HCofThisEvent * HCE = anEvent->GetHCofThisEvent();
  
  event = anEvent->GetEventID();
  
  std::vector<MollerDetectorHitsCollection*> THC;
  
  
  if(HCE) {
    for (int i=0; i<NUM_DETS; i++) {
      THC.push_back ((MollerDetectorHitsCollection*)(HCE->GetHC(hitsCollID[i])));
    }
  }
  //  G4cout<<"Checking for hits..."<<G4endl;
  for (int i=0; i<NUM_DETS; i++) {
    // somehow use: aHit->GetTrackID()==1 and aHit->GetTrackID()==2 with an indicator of first entry for that event/track?
    if(THC[i]){
      int n_hit = THC[i]->entries();

      if (n_hit>0) {
	
	for(int i1=0;i1<n_hit;i1++){
	  MollerDetectorHit* aHit = (*THC[i])[i1];
	  AddData(aHit);
	}
      }
    }
  }
  
  gRootAnalysis->SetMomentum0(0,0,0,0);
  gRootAnalysis->SetMomentum1(0,0,0,0);
  gRootAnalysis->SetMomentum2(0,0,0,0);
  gRootAnalysis->SetProcess(0);
  
}


void MollerAnalysis::EndOfRunAction(const G4Run */*aRun*/)
{
  if (gSystem) gSystem->ProcessEvents();
  
  if (myDetector->GetReadgeo()){
    G4String detfileName = myDetector->GetDetectorGeomFile();
    TMacro addgdml(detfileName.data());
    addgdml.Write();
  }
  G4cout<<"FINAL Number of detectors="<< NUM_DETS << G4endl;
  hfile->Write();
  pGlobalMagnetField = new MollerGlobalMagnetField();
  pGlobalMagnetField->SetLowLimSeptumField(mg_field_low);
  pGlobalMagnetField->SetHighLimSeptumField(mg_field_high);
  pGlobalMagnetField->SetScaleFactor(mgFieldScaleFactor);
  pGlobalMagnetField->WriteMagField();
 
  hfile->Close();
  
}


