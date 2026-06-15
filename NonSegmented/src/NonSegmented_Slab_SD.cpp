#include "NonSegmented_Slab_SD.h"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4StepPoint.hh"
#include "NonSegmented_Slab_Hit.h"
#include "G4AnalysisManager.hh"
#include <TRandom3.h>
double SmearSimulationData(double eDep)
{
  // Reference parameters from Na-22 prompt edge
  double E0        = 1.062;                   // Reference energy in MeV
  double fwhm_ref  = 0.11;                    // 11% FWHM at reference energy
  double sigma_ref = (fwhm_ref / 2.355) * E0; // Absolute sigma at E0

  double smeared_eDep = eDep;

  if (eDep > 0.001) { // Same threshold as your plot cut
    // 1. Calculate the energy-dependent absolute sigma (MeV)
    // Resolution scales proportional to sqrt(eDep)
    double sigma_E = sigma_ref * sqrt(eDep / E0);

    // 2. Flucluate the energy deposition using a Gaussian distribution
    smeared_eDep = gRandom->Gaus(eDep, sigma_E);
  }
  return smeared_eDep;
}

NonSegmented_Slab_SD::NonSegmented_Slab_SD(const G4String &name, const G4String &collName) : G4VSensitiveDetector(name)
{
  collectionName.insert(collName);
  fEDep = 0.;
}

NonSegmented_Slab_SD::~NonSegmented_Slab_SD() {}

void NonSegmented_Slab_SD::Initialize(G4HCofThisEvent *hce)
{
  fSlabHitCollection = new Slab_HitCollection(SensitiveDetectorName, collectionName[0]);
  G4int hcId         = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcId, fSlabHitCollection);
  fEDep = 0.;
}

G4bool NonSegmented_Slab_SD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  G4StepPoint *postStepPoint = step->GetPostStepPoint();
  G4Track *track             = step->GetTrack();
  G4double energy            = track->GetKineticEnergy();
  // G4cout << "Detected energy: " << energy / MeV << " MeV" << G4endl;
  fEDep += step->GetTotalEnergyDeposit();
  NonSegmented_Slab_Hit *hit = new NonSegmented_Slab_Hit;

  if (track->GetTrackID() == 1) {
    if (postStepPoint->GetStepStatus() == fGeomBoundary) {
      // std::cout << postStepPoint->GetPosition() << std::endl;
      hit->Set(postStepPoint->GetPosition().x(), postStepPoint->GetPosition().z());
      fSlabHitCollection->insert(hit);
    }
  }

  return true;
}

void NonSegmented_Slab_SD::EndOfEvent(G4HCofThisEvent *hce)
{

  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  analMan->FillNtupleDColumn(1, 10, fEDep);

  double sigma_E = sqrt(0.001658 * fEDep + 0.000314 * fEDep * fEDep);
  double smeared_eDep = gRandom->Gaus(fEDep, sigma_E);

  fEDep             = smeared_eDep;//SmearSimulationData(fEDep);
  double adcChannel = fEDep * 10000 * 0.6320;
  analMan->FillNtupleDColumn(2, 10, fEDep);
  analMan->FillNtupleDColumn(2, 11, adcChannel);
  /*unsigned int entries = fSlabHitCollection->entries();

  for (unsigned int i = 0; i < entries; i++) {
    (*fSlabHitCollection)[i]->Print();
  }*/
}
