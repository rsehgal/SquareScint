#include "MuonDecay_DetectorConstruction.h"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "MuonDecay_SensitiveDetector.h"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
MuonDecay_DetectorConstruction::MuonDecay_DetectorConstruction() {}

MuonDecay_DetectorConstruction::~MuonDecay_DetectorConstruction() {}

G4VPhysicalVolume* MuonDecay_DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");

    //Modify the world volume dimension as required
    G4Box* solidWorld = new G4Box("World", 0.5*m, 0.5*m, 0.5*m);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0);
    
    //TODO : Create your desired detectors here
    
    G4Material* scintMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    G4Box *scint = new G4Box("Scintillator",25*cm,3*cm,25*cm);
    G4LogicalVolume *logicalScintillator = new G4LogicalVolume(scint,scintMat,"LogicalScintillator");
    G4VPhysicalVolume *physicalScintillator = new G4PVPlacement(nullptr,G4ThreeVector(),logicalScintillator,"PhysicalScintillator",logicWorld,false,0,true);
    
    // Logic to Attach sensitive detector to a logical volume
    //MuonDecay_SensitiveDetector* detector = new MuonDecay_SensitiveDetector("SensitiveDetector");
    //G4SDManager::GetSDMpointer()->AddNewDetector(detector);
    //logicWorld->SetSensitiveDetector(detector);

    return physWorld;
}
