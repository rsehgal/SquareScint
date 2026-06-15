#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QGSP_BERT.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "NonSegmented_DetectorConstruction.h"
#include "NonSegmented_PrimaryGeneratorAction.h"
#include "NonSegmented_SteppingAction.h"

#include "FTFP_BERT_HP.hh"
#include "G4VModularPhysicsList.hh"
#include "G4OpticalPhysics.hh"

#include "NonSegmented_EventAction.h"
#include "NonSegmented_TrackingAction.h"
#include "NonSegmented_RunAction.h"

#include "ctime"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RanecuEngine.h"
#include  "G4RadioactiveDecayPhysics.hh"
#include "G4HadronicParameters.hh"

int main(int argc, char **argv)
{

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());

  // Set time-based seeds
  long seeds[2];
  seeds[0] = time(nullptr);
  seeds[1] = seeds[0] + 37;  // Just to vary the second one
  CLHEP::HepRandom::setTheSeeds(seeds);

  G4UIExecutive *ui = nullptr;
  G4String outfileName;
  int locX = 0;
  int locZ = 0;
  if (argc == 1) {
    ui          = new G4UIExecutive(argc, argv);
    outfileName = "interactive.root";
  } else {
    outfileName = argv[2];
    locX = std::atoi(argv[3]);
    locZ = std::atoi(argv[4]);
  }
  G4RunManager *runManager = new G4RunManager;

  G4OpticalPhysics *opticalPhysics   = new G4OpticalPhysics;
  G4VModularPhysicsList *physicsList = new FTFP_BERT_HP();
//physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics());
//  #define OPTICAL_PHYSICS
  #ifdef OPTICAL_PHYSICS
  physicsList->RegisterPhysics(opticalPhysics);
  #endif

  runManager->SetUserInitialization(new NonSegmented_DetectorConstruction());
  // runManager->SetUserInitialization(new QGSP_BERT);
  runManager->SetUserInitialization(physicsList);
  //runManager->SetUserAction(new NonSegmented_PrimaryGeneratorAction());
  runManager->SetUserAction(new NonSegmented_PrimaryGeneratorAction(locX,locZ));
  runManager->SetUserAction(new NonSegmented_RunAction(outfileName));
  runManager->SetUserAction(new NonSegmented_EventAction());
  //runManager->SetUserAction(new NonSegmented_TrackingAction());
  // runManager->SetUserAction(new NonSegmented_SteppingAction());
    G4HadronicParameters::Instance()->SetTimeThresholdForRadioactiveDecay(1.0e+60 * CLHEP::year);


  G4VisManager *visManager = new G4VisExecutive();
  visManager->Initialize();

  G4UImanager *UImanager = G4UImanager::GetUIpointer();
  if (!ui) {
    // batch mode
    G4String command  = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  } else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;

  return 0;
}
