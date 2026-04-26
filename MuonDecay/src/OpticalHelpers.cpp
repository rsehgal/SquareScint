#include "OpticalHelpers.h"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4String.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
G4OpticalSurface *GetOpticalSurface() {
  // Define optical surface for side reflection
  G4OpticalSurface *surfaceCrystal = new G4OpticalSurface("CrystalSurface");
  surfaceCrystal->SetModel(unified);
  surfaceCrystal->SetType(dielectric_dielectric);
  surfaceCrystal->SetFinish(groundfrontpainted); // or polished

  // Reflective properties
  G4MaterialPropertiesTable *mptSurface = new G4MaterialPropertiesTable();
  const G4int num = 2;
  G4double ephoton[num] = {1.5 * eV, 3.5 * eV};
  G4double reflectivity[num] = {1.0, 1.0};
  G4double efficiency[num] = {1.0, 1.0};
  mptSurface->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
  mptSurface->AddProperty("EFFICIENCY", ephoton, efficiency, num);
  surfaceCrystal->SetMaterialPropertiesTable(mptSurface);
  return surfaceCrystal;
}

void AttachOpticalProperties(G4Material *scintMat) {

  const G4int nEntries = 2;
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
  mptCrystal->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy,
                          scintSpectrum, nEntries);
  mptCrystal->AddProperty("SCINTILLATIONCOMPONENT2", photonEnergy,
                          scintSpectrum, nEntries);
  mptCrystal->AddConstProperty("SCINTILLATIONYIELD", 10000. / MeV);
  mptCrystal->AddConstProperty("RESOLUTIONSCALE", 1.0);
  // mptCrystal->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.1 * ns);
  mptCrystal->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 0.9 * ns);
  // mptCrystal->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10. * ns);
  mptCrystal->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 5 * ns);

  scintMat->SetMaterialPropertiesTable(mptCrystal);
}

G4OpticalSurface *GetInterfacingSurface() {
  // Define interface between crystal and PMT — allow transmission
  G4OpticalSurface *crystalToPMTSurface = new G4OpticalSurface("CrystalToPMT");
  crystalToPMTSurface->SetType(dielectric_dielectric);
  crystalToPMTSurface->SetModel(unified);
  crystalToPMTSurface->SetFinish(polished);

  // Perfect transmission (no reflectivity)
  const int nEntries = 2;
  G4MaterialPropertiesTable *mptInterface = new G4MaterialPropertiesTable();
  G4double ephoton[nEntries] = {1.5 * eV, 3.5 * eV};
  G4double reflectivity_zero[nEntries] = {1., 1.};
  G4double eff_pmt[nEntries] = {0.35, 0.35};
  mptInterface->AddProperty("REFLECTIVITY", ephoton, reflectivity_zero,
                            nEntries);
  mptInterface->AddProperty("EFFICIENCY", ephoton, eff_pmt, nEntries);
  crystalToPMTSurface->SetMaterialPropertiesTable(mptInterface);
  return crystalToPMTSurface;
}

void AddReflectiveWrapping(G4LogicalVolume *logVolume) {
  G4OpticalSurface *opticalSurface = GetOpticalSurface();
  new G4LogicalSkinSurface("ReflectiveWrapping", logVolume, opticalSurface);
}

void AddOpticalGreaseBetweenVolumes(G4String name,G4VPhysicalVolume *vol1,
                                    G4VPhysicalVolume *vol2) {
  G4OpticalSurface *interfacingSurface = GetInterfacingSurface();
  new G4LogicalBorderSurface(name, vol1, vol2,
                             interfacingSurface);
}

G4LogicalVolume *GetPMT()
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
