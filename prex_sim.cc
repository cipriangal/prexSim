#include "MollerDetectorConstruction.hh"
#include "MollerGlobalMagnetField.hh"
#include "PhysicsList.hh"
#include "MollerPrimaryGenAction.hh"
#include "MollerRunAction.hh"
#include "MollerEventAction.hh"

#include "MollerSteppingVerbose.hh"
#include "MollerSteppingAction.hh"

#include "G4Version.hh"
#if G4VERSION_NUMBER < 1000
#include "G4StepLimiterBuilder.hh"
#else
#include "G4StepLimiterPhysics.hh"
#endif

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "PhysicsListMessenger.hh"

// user interface
#ifdef G4UI_USE_QT
#include "G4UIQt.hh"
#include "G4Qt.hh"
#endif
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif
// visuals
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include <time.h>

#include <fstream>
#include "MollerAnalysis.hh"

int main(int argc, char** argv)
{

  clock_t tStart=clock();	

  // Verbose output class
  //
  G4VSteppingVerbose* verbosity = new MollerSteppingVerbose();
  G4VSteppingVerbose::SetInstance(verbosity);

  // Construct the default Run Manager
  //
  G4RunManager* runManager = new G4RunManager;

  // Mandatory User Initialization classes
  //
  MollerDetectorConstruction* detector = new MollerDetectorConstruction;
  runManager->SetUserInitialization(detector);

  G4PhysListFactory factory;
  G4VModularPhysicsList* phys = 0;
  PhysicsListMessenger* mess = 0;
  G4String physName = "QGSP_BERT_HP";
  G4int verbose=0;
  
  if(factory.IsReferencePhysList(physName)) {
    phys = factory.GetReferencePhysList(physName);
    phys->SetVerboseLevel(verbose);
    
#if G4VERSION_NUMBER < 1000
    phys->RegisterPhysics(new G4StepLimiterBuilder(verbose));
#else
    phys->RegisterPhysics(new G4StepLimiterPhysics());
#endif
  
    mess = new PhysicsListMessenger();
  }
    
  // define physics
  runManager->SetUserInitialization(phys);

  // User Action Classes
  //
  runManager->SetUserAction(new MollerPrimaryGenAction);
  runManager->SetUserAction(new MollerRunAction);
  runManager->SetUserAction(new MollerEventAction);
  runManager->SetUserAction(new MollerSteppingAction);



  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc > 2)   // batch mode
    {
      G4cout << "==========================================================="<< G4endl;
      G4cout << "Batch  mode"<< G4endl;
      G4cout << "==========================================================="<< G4endl;  
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      G4String fileName2 = argv[2];

      UImanager->ApplyCommand(command+fileName);
       // Start analysis
      //
      MollerAnalysis* analysis = new MollerAnalysis(detector);
      //pass the GDML and macro files here
      analysis->AddGDMLFileName(detector->GetDetectorGeomFile());
      analysis->AddMacro_1(fileName);
      analysis->AddMacro_2(fileName2);

      analysis->SetLowLimMagneticShield(detector->GetLowLimMagneticShield());
      analysis->SetHighLimMagneticShield(detector->GetHighLimMagneticShield());

      // Initialize G4 kernel
      //
      runManager->Initialize();
      // Get condition of run from second macro
      //
      UImanager->ApplyCommand(command+fileName2);
       // End analysis
      //
      analysis->End();
      delete analysis;
    }
  if (argc == 2)
    {  // interactive mode : define UI session
      G4cout << "==========================================================="<< G4endl;
      G4cout << "Interactive mode"<< G4endl;
      G4cout << "==========================================================="<< G4endl;   
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      G4VisManager* visManager = new G4VisExecutive;
      visManager->Initialize();
      UImanager->ApplyCommand("/control/macroPath macros"); 
      UImanager->ApplyCommand("/control/execute vis/vis.mac");
#endif
      if (ui->IsGUI()){
	UImanager->ApplyCommand("/control/macroPath macros"); 
	UImanager->ApplyCommand("/control/execute gui.mac");
      }
      ui->SessionStart();
      delete ui;
#endif
#ifdef G4VIS_USE
      delete visManager;
#endif
    } else {
    G4cout << G4endl<< G4endl;
    G4cout << "==========================================================="<< G4endl;
    G4cout << "Please specify the prerun macro, or live with the defaults!"<< G4endl;
    G4cout << "==========================================================="<< G4endl;
    G4cout << G4endl<< G4endl;
  }
 
  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

  delete runManager;
  G4cout<<" Running time[s]: "<< (double) ((clock() - tStart)/CLOCKS_PER_SEC)<<G4endl;
  return 0;
}
