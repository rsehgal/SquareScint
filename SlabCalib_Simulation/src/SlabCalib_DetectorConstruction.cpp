#include "SlabCalib_DetectorConstruction.h"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "SlabCalib_SensitiveDetector.h"
#include "SlabCalib_SD.h"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"
SlabCalib_DetectorConstruction::SlabCalib_DetectorConstruction() {}

SlabCalib_DetectorConstruction::~SlabCalib_DetectorConstruction() {}

G4VPhysicalVolume* SlabCalib_DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");

    //Modify the world volume dimension as required
    G4Box* solidWorld = new G4Box("World", 1*m, 1*m, 1*m);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0);
    
    //TODO : Create your desired detectors here
    G4Box* slab = new G4Box("Slab", 25.*cm, 3.*cm, 25*cm);
    G4LogicalVolume* logicSlab = new G4LogicalVolume(slab, worldMat, "LogicalSlab");
    G4VPhysicalVolume* physicalSlab = new G4PVPlacement(nullptr, G4ThreeVector(), logicSlab, "PhysicalSlab", logicWorld, false, 0,true);

    //Bar
    G4Box* bar = new G4Box("Bar", 3.*cm, 3.*cm, 50*cm);
    G4LogicalVolume* logicBar = new G4LogicalVolume(bar, worldMat, "LogicalBar");
    
    //Top Bar
    G4ThreeVector topBarLoc(0.,7*cm,0);
    G4RotationMatrix *rot = new G4RotationMatrix;
    rot->rotateY(90*degree);
    G4VPhysicalVolume* physicalBar1 = new G4PVPlacement(nullptr, topBarLoc, logicBar, "PhysicalBar_1", logicWorld, false, 0,true);

    //Bottom Bar
    G4ThreeVector bottomBarLoc(0.,-7*cm,0);
    G4VPhysicalVolume* physicalBar2 = new G4PVPlacement(rot, bottomBarLoc, logicBar, "PhysicalBar_2", logicWorld, false, 0,true);


    // Logic to Attach sensitive detector to a logical volume
    SlabCalib_SD* detector = new SlabCalib_SD("SensitiveDetector");
    G4SDManager::GetSDMpointer()->AddNewDetector(detector);
    logicSlab->SetSensitiveDetector(detector);
    logicBar->SetSensitiveDetector(detector);
    

    return physWorld;
}
