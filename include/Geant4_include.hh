//////////////////////////////////////////////////////////
// header Geant4_include.h
//////////////////////////////////////////////////////////

#ifndef __Geant4_include_h
#define __Geant4_include_h

//using namespace std;

//================================================

//================================================

// Geant4 version
#include "G4Version.hh"

// Run related
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UserRunAction.hh"

// material related
#include "G4NistManager.hh"


// touchable related
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"

// step/track related

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4UserSteppingAction.hh"
#include "G4SteppingManager.hh"
#include "G4SteppingVerbose.hh"
#include "G4UnitsTable.hh"

#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4UserTrackingAction.hh"
#include "G4VUserTrackInformation.hh"
#include "G4TrackingManager.hh"
#include "G4TrackVector.hh"

// trajectory related
#include "G4VTrajectory.hh"


// HEP related 
#include "globals.hh"
#include "Randomize.hh"

// material related
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"

// relocation related
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"


// geometry related
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4Cons.hh"
#include "G4Para.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Polyhedra.hh"

// manager related
#include "G4GeometryManager.hh"
#include "G4SDManager.hh"
#include "G4VVisManager.hh"
#include "G4Navigator.hh"

// event related
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4UserEventAction.hh"

#include "G4UserLimits.hh"

// attributes
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

// store related
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4SolidStore.hh"

// detector related
#include "G4VUserDetectorConstruction.hh"
#include "G4VSensitiveDetector.hh"
#include "G4VReadOutGeometry.hh"


// volume related
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
//#include "G4AssembyVolume.hh"


// optical related
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"


#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4VPVParameterisation.hh"
#include "G4PVReplica.hh"


// field related
#include "G4MagneticField.hh"

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
#include "G4PropagatorInField.hh"


#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"


// boolean 
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

// hits related
#include "G4HCofThisEvent.hh"
#include "G4THitsCollection.hh"
#include "G4VHitsCollection.hh"
#include "G4VHit.hh"

// trajectory related
#include "G4Trajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4TrajectoryPoint.hh"

//misc
#include "G4Allocator.hh"

// GUI related
#include "G4UImanager.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"


//====================================================================================
//====================================================================================
//====================================================================================	

// particle related
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"

// physics related
#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4ProcessManager.hh"

#include "G4GammaConversion.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

// Muon Physics
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#if G4VERSION_NUMBER < 1000
#include "G4MuonMinusCaptureAtRest.hh"
#endif

#include "G4UserSpecialCuts.hh"
#include "G4PAIModel.hh"
#include "G4PAIPhotModel.hh"
#include "G4ProductionCuts.hh"
#include "G4Decay.hh"

// Gamma Physics List
#include "G4ComptonScattering.hh"
#include "G4PhotoElectricEffect.hh"

// Boson Physics Models
#if G4VERSION_NUMBER < 1000
#include "G4GammaNuclearReaction.hh"
#endif


//Particle related
#include "G4ParticleTable.hh"

//Particles
#include "G4Proton.hh"

// Leptons
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4TauMinus.hh"
#include "G4TauPlus.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"
#include "G4NeutrinoTau.hh"
#include "G4AntiNeutrinoTau.hh"

// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"

// Mesons
#include "G4PionPlus.hh"

//
// Gamma- and electro-nuclear models and processes
//
#if G4VERSION_NUMBER < 1000
#include "G4ElectroNuclearReaction.hh"
#endif
#include "G4QGSModel.hh"
#include "G4GammaParticipants.hh"
#include "G4QGSMFragmentation.hh"

// Lepton-Nucleon Physics
#include "G4PhotoNuclearProcess.hh"
#include "G4ElectronNuclearProcess.hh"
#include "G4PositronNuclearProcess.hh"

//
// Hadronic processes
//
#include "G4HadronElasticProcess.hh"
#include "G4HadronFissionProcess.hh"
#include "G4KaonPlusInelasticProcess.hh"
#include "G4KaonZeroLInelasticProcess.hh"
#include "G4KaonZeroSInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4AntiNeutronInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"

//
// Hadronic interaction models
// Low energy (E < 20 GeV) part only
//
#if G4VERSION_NUMBER < 1000
#include "G4LElastic.hh"
// #include "G4LEPionPlusInelastic.hh"
// #include "G4LEPionMinusInelastic.hh"
#include "G4LEKaonPlusInelastic.hh"
#include "G4LEKaonMinusInelastic.hh"
#include "G4LEKaonZeroLInelastic.hh"
#include "G4LEKaonZeroSInelastic.hh"
#include "G4LEProtonInelastic.hh"
#include "G4LEAntiProtonInelastic.hh"
#include "G4LENeutronInelastic.hh"
#include "G4LEAntiNeutronInelastic.hh"
#include "G4LELambdaInelastic.hh"
#include "G4LEAntiLambdaInelastic.hh"
#include "G4LEDeuteronInelastic.hh"
#include "G4LETritonInelastic.hh"
#include "G4LEAlphaInelastic.hh"
#endif

//=================
// Neutron Related
// ================

// neutron cross sections
#include "G4NeutronInelasticCrossSection.hh"

// Neutron high-precision models: <20 MeV
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPElasticData.hh"
//
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPInelasticData.hh"
//
#include "G4NeutronHPFission.hh"
#include "G4NeutronHPFissionData.hh"
//
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"

// constructors
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

// processes
#if G4VERSION_NUMBER < 940
#include "G4MultipleScattering.hh"
#else
#include "G4eMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#endif
#include "G4hIonisation.hh"
#include "G4PionPlusInelasticProcess.hh"
#include "G4PionMinusInelasticProcess.hh"
#include "G4KaonMinusInelasticProcess.hh"
#include "G4ProtonInelasticProcess.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4AntiProtonInelasticProcess.hh"
#include "G4LambdaInelasticProcess.hh"
#include "G4AntiLambdaInelasticProcess.hh"
#include "G4SigmaPlusInelasticProcess.hh"
#include "G4SigmaMinusInelasticProcess.hh"
#include "G4AntiSigmaPlusInelasticProcess.hh"
#include "G4AntiSigmaMinusInelasticProcess.hh"
#include "G4XiZeroInelasticProcess.hh"
#include "G4XiMinusInelasticProcess.hh"
#include "G4AntiXiZeroInelasticProcess.hh"
#include "G4AntiXiMinusInelasticProcess.hh"
#include "G4OmegaMinusInelasticProcess.hh"
#include "G4AntiOmegaMinusInelasticProcess.hh"
#if G4VERSION_NUMBER < 1000
#include "G4AntiProtonAnnihilationAtRest.hh"
#include "G4PionMinusAbsorptionAtRest.hh"
#include "G4KaonMinusAbsorption.hh"
#endif
#include "G4AntiNeutronAnnihilationAtRest.hh"

// cross sections
#include "G4PiNuclearCrossSection.hh"
#include "G4ProtonInelasticCrossSection.hh"

// models
#include "G4CascadeInterface.hh"
#if G4VERSION_NUMBER < 1000
#include "G4LEPionPlusInelastic.hh"
#include "G4LEPionMinusInelastic.hh"
#include "G4LCapture.hh"
#include "G4LESigmaPlusInelastic.hh"
#include "G4LESigmaMinusInelastic.hh"
#include "G4LEAntiSigmaPlusInelastic.hh"
#include "G4LEAntiSigmaMinusInelastic.hh"
#include "G4LEXiZeroInelastic.hh"
#include "G4LEXiMinusInelastic.hh"
#include "G4LEAntiXiZeroInelastic.hh"
#include "G4LEAntiXiMinusInelastic.hh"
#include "G4LEOmegaMinusInelastic.hh"
#include "G4LEAntiOmegaMinusInelastic.hh"
#include "G4HEAntiProtonInelastic.hh"
#include "G4HEAntiNeutronInelastic.hh"
#include "G4HELambdaInelastic.hh"
#include "G4HEAntiLambdaInelastic.hh"
#include "G4HESigmaPlusInelastic.hh"
#include "G4HESigmaMinusInelastic.hh"
#include "G4HEAntiSigmaPlusInelastic.hh"
#include "G4HEAntiSigmaMinusInelastic.hh"
#include "G4HEXiZeroInelastic.hh"
#include "G4HEXiMinusInelastic.hh"
#include "G4HEAntiXiZeroInelastic.hh"
#include "G4HEAntiXiMinusInelastic.hh"
#include "G4HEOmegaMinusInelastic.hh"
#include "G4HEAntiOmegaMinusInelastic.hh"
#endif

#include "G4LFission.hh"
#include "G4TheoFSGenerator.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4ExcitationHandler.hh"
#include "G4PreCompoundModel.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4QGSParticipants.hh"



#endif

