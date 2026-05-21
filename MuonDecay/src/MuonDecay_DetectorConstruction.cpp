#include "MuonDecay_DetectorConstruction.h"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "MuonDecay_SensitiveDetector.h"
#include "OpticalHelpers.h"
#include "MuonDecay_PMT_SD.h"
#include "MuonDecay_Scint_SD.h"

MuonDecay_DetectorConstruction::MuonDecay_DetectorConstruction() {}

MuonDecay_DetectorConstruction::~MuonDecay_DetectorConstruction() {}

G4VPhysicalVolume *MuonDecay_DetectorConstruction::Construct() {
  G4NistManager *nist = G4NistManager::Instance();
  G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

  // Modify the world volume dimension as required
  G4Box *solidWorld = new G4Box("World", 0.5 * m, 0.5 * m, 0.5 * m);
  G4LogicalVolume *logicWorld =
      new G4LogicalVolume(solidWorld, worldMat, "World");
  G4VPhysicalVolume *physWorld = new G4PVPlacement(
      nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0);

  // TODO : Create your desired detectors here
G4Material *Fe =
      nist->FindOrBuildMaterial("G4_Fe");
G4Material *Al =
      nist->FindOrBuildMaterial("G4_Al");
G4Material *Cu =
      nist->FindOrBuildMaterial("G4_Cu");
G4Material *Pb =
      nist->FindOrBuildMaterial("G4_Pb");

  G4Box *cryPlane = new G4Box("CRYPlane", 25 * cm, 1 * mm, 25 * cm);
  G4LogicalVolume *logicalCRYPlane =
      new G4LogicalVolume(cryPlane,Al, "LogicalCRYPlane");
  G4VPhysicalVolume *physicalCRYPlane =
      new G4PVPlacement(nullptr, G4ThreeVector(0.,25.*cm,0.), logicalCRYPlane,
                        "PhysicalCRYPlane", logicWorld, false, 0, true);



  G4Box *materialSlab = new G4Box("MaterialSlab", 25 * cm, 2 * cm, 25 * cm);
  G4LogicalVolume *logicalMaterialSlab =
      new G4LogicalVolume(materialSlab,Cu, "LogicalMaterialSlab");
/*  G4VPhysicalVolume *physicalMaterialSlab =
      new G4PVPlacement(nullptr, G4ThreeVector(0.,8.*cm,0.), logicalMaterialSlab,
                        "PhysicalMaterialSlab", logicWorld, false, 0, true);
*/


  G4Material *scintMat =
      nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  AttachOpticalProperties(scintMat);

//Top scintillator
G4Box *topScint = new G4Box("TopScintillator", 25 * cm, 0.5 * cm, 25 * cm);
  G4LogicalVolume *logicalTopScintillator =
      new G4LogicalVolume(topScint, scintMat, "LogicalTopScintillator");
  AddReflectiveWrapping(logicalTopScintillator);
/*  G4VPhysicalVolume *physicalTopScintillator =
      new G4PVPlacement(nullptr, G4ThreeVector(0,9.*cm,0.), logicalTopScintillator,
                        "PhysicalTopScintillator", logicWorld, false, 50, true);
*/

  G4Box *scint = new G4Box("Scintillator", 25 * cm, 3 * cm, 25 * cm);
  AttachOpticalProperties(scintMat);
  G4LogicalVolume *logicalScintillator =
      new G4LogicalVolume(scint, scintMat, "LogicalScintillator");
  AddReflectiveWrapping(logicalScintillator);
  G4VPhysicalVolume *physicalScintillator =
      new G4PVPlacement(nullptr, G4ThreeVector(), logicalScintillator,
                        "PhysicalScintillator", logicWorld, false, 51, true);

  G4LogicalVolume *logicalPMT = GetPMT();
  G4VPhysicalVolume *physicalPMT4 =
      new G4PVPlacement(nullptr, G4ThreeVector(0., 0., 29 * cm), logicalPMT,
                        "PhysicalPMT4", logicWorld, false, 4, true);
  G4VPhysicalVolume *physicalPMT5 =
      new G4PVPlacement(nullptr, G4ThreeVector(0., 0., -29 * cm), logicalPMT,
                        "PhysicalPMT5", logicWorld, false, 5, true);

  G4RotationMatrix *rotY90 = new G4RotationMatrix();
  rotY90->rotateY(90. * deg);

  G4VPhysicalVolume *physicalPMT6 =
      new G4PVPlacement(rotY90, G4ThreeVector(29. * cm, 0., 0), logicalPMT,
                        "PhysicalPMT6", logicWorld, false, 6, true);
  G4VPhysicalVolume *physicalPMT7 =
      new G4PVPlacement(rotY90, G4ThreeVector(-29. * cm, 0., 0.), logicalPMT,
                        "PhysicalPMT7", logicWorld, false, 7, true);

  AddOpticalGreaseBetweenVolumes("Scint_PMT4", physicalScintillator,
                                 physicalPMT4);
  AddOpticalGreaseBetweenVolumes("Scint_PMT5", physicalScintillator,
                                 physicalPMT5);
  AddOpticalGreaseBetweenVolumes("Scint_PMT6", physicalScintillator,
                                 physicalPMT6);
  AddOpticalGreaseBetweenVolumes("Scint_PMT7", physicalScintillator,
                                 physicalPMT7);

  // Logic to Attach sensitive detector to a logical volume
  // MuonDecay_SensitiveDetector* detector = new
  // MuonDecay_SensitiveDetector("SensitiveDetector");

  MuonDecay_PMT_SD *detector = new MuonDecay_PMT_SD("PMT","PMT_Collection");
  G4SDManager::GetSDMpointer()->AddNewDetector(detector);
  logicalPMT->SetSensitiveDetector(detector);

  MuonDecay_Scint_SD *detScint=new MuonDecay_Scint_SD("Slab","Scint_Collection");
  G4SDManager::GetSDMpointer()->AddNewDetector(detScint);
  logicalScintillator->SetSensitiveDetector(detScint);
//  logicalTopScintillator->SetSensitiveDetector(detScint);

  return physWorld;
}
