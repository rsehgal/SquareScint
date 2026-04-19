#include "NonSegmented_DetectorConstruction.h"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "NonSegmented_SensitiveDetector.h"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "NonSegmented_PMT_SD.h"
#include "NonSegmented_Slab_SD.h"

NonSegmented_DetectorConstruction::NonSegmented_DetectorConstruction() {}

NonSegmented_DetectorConstruction::~NonSegmented_DetectorConstruction() {}

G4OpticalSurface *NonSegmented_DetectorConstruction::GetOpticalSurface()
{
  // Define optical surface for side reflection
  G4OpticalSurface *surfaceCrystal = new G4OpticalSurface("CrystalSurface");
  surfaceCrystal->SetModel(unified);
  surfaceCrystal->SetType(dielectric_dielectric);
  surfaceCrystal->SetFinish(groundfrontpainted); // or polished

  // Reflective properties
  G4MaterialPropertiesTable *mptSurface = new G4MaterialPropertiesTable();
  const G4int num                       = 2;
  G4double ephoton[num]                 = {1.5 * eV, 3.5 * eV};
  G4double reflectivity[num]            = {1.0, 1.0};
  G4double efficiency[num]              = {1.0, 1.0};
  mptSurface->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
  mptSurface->AddProperty("EFFICIENCY", ephoton, efficiency, num);
  surfaceCrystal->SetMaterialPropertiesTable(mptSurface);
  return surfaceCrystal;
}

G4LogicalVolume *NonSegmented_DetectorConstruction::GetPMT()
{
  G4Material *pmtMat        = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pyrex_Glass");
  G4Box *solidPMT           = new G4Box("PMT", 2.5 * cm, 2.5 * cm, 4 * cm);
  G4LogicalVolume *logicPMT = new G4LogicalVolume(solidPMT, pmtMat, "PMT");

  const int nEntries = 2;
  G4double photonEnergy[nEntries] = {1.5 * eV, 3.5 * eV};

  // Refractive index
  G4double rIndex[nEntries] = {1.9, 1.9};

  G4MaterialPropertiesTable *glassMPT = new G4MaterialPropertiesTable();
  glassMPT->AddProperty("RINDEX", photonEnergy, rIndex, nEntries);
  pmtMat->SetMaterialPropertiesTable(glassMPT);

  return logicPMT;
}

G4OpticalSurface* NonSegmented_DetectorConstruction::GetInterfacingSurface(){
// Define interface between crystal and PMT — allow transmission
  G4OpticalSurface *crystalToPMTSurface = new G4OpticalSurface("CrystalToPMT");
  crystalToPMTSurface->SetType(dielectric_dielectric);
  crystalToPMTSurface->SetModel(unified);
  crystalToPMTSurface->SetFinish(polished);

  // Perfect transmission (no reflectivity)
  const int nEntries = 2;
  G4MaterialPropertiesTable *mptInterface = new G4MaterialPropertiesTable();
  G4double ephoton[nEntries]                 = {1.5 * eV, 3.5 * eV};
  G4double reflectivity_zero[nEntries]         = {1., 1.};
  G4double eff_pmt[nEntries]                   = {0.35,0.35};
  mptInterface->AddProperty("REFLECTIVITY", ephoton, reflectivity_zero, nEntries);
  mptInterface->AddProperty("EFFICIENCY", ephoton, eff_pmt, nEntries);
  crystalToPMTSurface->SetMaterialPropertiesTable(mptInterface);
  return crystalToPMTSurface;
}

void NonSegmented_DetectorConstruction::AttachOpticalProperties(G4Material *scintMat)
{

  const G4int nEntries            = 2;
  G4double photonEnergy[nEntries] = {1.5 * eV, 3.5 * eV};

  // Refractive index
  G4double rIndex[nEntries] = {1.8, 1.8};

  // Absorption length (how far photons travel before being absorbed)
  G4double absorption[nEntries] = {350 * cm, 350 * cm};

  // Scintillation emission spectrum (uniform for simplicity)
  G4double scintSpectrum[nEntries] = {1.0, 1.0};

  G4MaterialPropertiesTable *mptCrystal = new G4MaterialPropertiesTable();
  mptCrystal->AddProperty("RINDEX", photonEnergy, rIndex, nEntries);
  mptCrystal->AddProperty("ABSLENGTH", photonEnergy, absorption, nEntries);
  mptCrystal->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintSpectrum, nEntries);
  mptCrystal->AddProperty("SCINTILLATIONCOMPONENT2", photonEnergy, scintSpectrum, nEntries);
  mptCrystal->AddConstProperty("SCINTILLATIONYIELD", 10000. / MeV);
  mptCrystal->AddConstProperty("RESOLUTIONSCALE", 1.0);
  //mptCrystal->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.1 * ns);
  mptCrystal->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 0.9 * ns);
  //mptCrystal->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10. * ns);
  mptCrystal->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 5 * ns);

  scintMat->SetMaterialPropertiesTable(mptCrystal);
}

G4VPhysicalVolume *NonSegmented_DetectorConstruction::Construct()
{
  G4NistManager *nist  = G4NistManager::Instance();
  G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
//G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");

  // Modify the world volume dimension as required
  G4Box *solidWorld            = new G4Box("World", 1.5 * m, 1.5 * m, 1.5 * m);
  G4LogicalVolume *logicWorld  = new G4LogicalVolume(solidWorld, worldMat, "World");
  G4VPhysicalVolume *physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0);

  G4Material *scintMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  AttachOpticalProperties(scintMat);

  G4OpticalSurface *opticalSurface = GetOpticalSurface();

  // TODO : Create your desired detectors here
#ifdef SCINTBAR
  G4Box *scintSlab                  = new G4Box("ScintSlab", 2.5 * cm, 2.5 * cm, 25 * cm);
#else
  G4Box *scintSlab                  = new G4Box("ScintSlab", 25 * cm, 2.5 * cm, 25 * cm);
#endif
  G4LogicalVolume *scintSlabLogical = new G4LogicalVolume(scintSlab, scintMat, "LogicalScintSlab");
  new G4LogicalSkinSurface("ReflectiveWrapping", scintSlabLogical, opticalSurface);

  G4VPhysicalVolume *physicalSlab = new G4PVPlacement(nullptr, G4ThreeVector(), scintSlabLogical, "PhysicalScintSlab", logicWorld, false, 0, true);

  G4LogicalVolume *pmtLogical = GetPMT();
  G4VPhysicalVolume *physicalPMT1 = new G4PVPlacement(nullptr, G4ThreeVector(0.,0.,29.000000*cm), pmtLogical, "PhysicalPMT", logicWorld, false, 1, true);
  G4VPhysicalVolume *physicalPMT3 = new G4PVPlacement(nullptr, G4ThreeVector(0.,0.,-29.000000*cm), pmtLogical, "PhysicalPMT", logicWorld, false, 3, true);

#ifndef SCINTBAR
  G4RotationMatrix* rotY90 = new G4RotationMatrix();
rotY90->rotateY(90.*deg);

 G4VPhysicalVolume *physicalPMT2 = new G4PVPlacement(rotY90, G4ThreeVector(29.000000*cm,0,0), pmtLogical, "PhysicalPMT", logicWorld, false, 2, true);
  G4VPhysicalVolume *physicalPMT4 = new G4PVPlacement(rotY90, G4ThreeVector(-29.000000*cm,0,0), pmtLogical, "PhysicalPMT", logicWorld, false, 4, true);

#endif
    
  G4OpticalSurface *interfacingSurface = GetInterfacingSurface();
  new G4LogicalBorderSurface("SlabToPMT_Surface1",physicalSlab,physicalPMT1,interfacingSurface);
  new G4LogicalBorderSurface("SlabToPMT_Surface3",physicalSlab,physicalPMT3,interfacingSurface);

#ifndef SCINTBAR
  new G4LogicalBorderSurface("SlabToPMT_Surface2",physicalSlab,physicalPMT2,interfacingSurface);
  new G4LogicalBorderSurface("SlabToPMT_Surface4",physicalSlab,physicalPMT4,interfacingSurface);
#endif

  // Logic to Attach sensitive detector to a logical volume
  //NonSegmented_SensitiveDetector* detector = new NonSegmented_SensitiveDetector("SensitiveDetector");
  NonSegmented_PMT_SD* detector = new NonSegmented_PMT_SD("SensitiveDetectorPMT","PMT");
  G4SDManager::GetSDMpointer()->AddNewDetector(detector);
  pmtLogical->SetSensitiveDetector(detector);

  NonSegmented_Slab_SD* detectorSlab = new NonSegmented_Slab_SD("SensitiveDetectorSlab","Slab");
  G4SDManager::GetSDMpointer()->AddNewDetector(detectorSlab);
  scintSlabLogical->SetSensitiveDetector(detectorSlab);

  return physWorld;
}
