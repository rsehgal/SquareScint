#include "NonSegmented_Slab_SD.h"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4StepPoint.hh"
#include "NonSegmented_Slab_Hit.h"
#include "G4AnalysisManager.hh"
#include <TRandom3.h>

unsigned int NonSegmented_Slab_SD::counter=0;

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
  return eDep;
}

NonSegmented_Slab_SD::NonSegmented_Slab_SD(const G4String &name, const G4String &collName) : G4VSensitiveDetector(name)
{
  collectionName.insert(collName);
  fEDep        = 0.;
  fEDep1274    = 0.;
  gammaCounter = 0;
  trackPrimary = false;
  counter=0;
}

NonSegmented_Slab_SD::~NonSegmented_Slab_SD() {}

void NonSegmented_Slab_SD::Initialize(G4HCofThisEvent *hce)
{
  fSlabHitCollection = new Slab_HitCollection(SensitiveDetectorName, collectionName[0]);
  G4int hcId         = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcId, fSlabHitCollection);
  fEDep        = 0.;
  fEDep1274    = 0.;
  gammaCounter = 0;
  gammaVec.clear();
  energyVec.clear();
  trackPrimary = false;
fTrackEnergyMap.clear();
}

G4bool NonSegmented_Slab_SD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  G4StepPoint *postStepPoint = step->GetPostStepPoint();
  G4Track *track             = step->GetTrack();
  G4double energy            = track->GetKineticEnergy();
  // G4cout << "Detected energy: " << energy / MeV << " MeV" << G4endl;

  // if(track->GetVertexKineticEnergy() > 0.499 && track->GetVertexKineticEnergy() < 0.53)
G4double edep  = step->GetTotalEnergyDeposit();

  if (edep > 0.) {
    G4int trackID = track->GetTrackID();
    // Accumulate energy specifically for this track
    fTrackEnergyMap[trackID] += edep; 
    
    // Maintain your global total if still needed for Ntuple 1
    fEDep += edep; 
  } 

/* {
    fEDep += step->GetTotalEnergyDeposit();
  }*/
  NonSegmented_Slab_Hit *hit = new NonSegmented_Slab_Hit;

  //  if(track->GetParticleDefinition()->GetParticleName()=="gamma")
  /*{
  std::cout << "RAMANSEHGAL : " << track->GetParticleDefinition()->GetParticleName() << " : " << track->GetTrackID() <<
  " : " << track->GetKineticEnergy() << std::endl;
  }*/

  if (track->GetParticleDefinition()->GetParticleName() == "gamma") {
    if (std::find(gammaVec.begin(), gammaVec.end(), track->GetTrackID()) == gammaVec.end()) {
      gammaVec.push_back(track->GetTrackID()); // Only inserts if target wasn't found
      energyVec.push_back(track->GetVertexKineticEnergy());
    }
  }

  if (track->GetParticleDefinition()->GetParticleName() == "e+") {
    track->SetTrackStatus(fStopAndKill);
  }

  if (track->GetTrackID() == 1) {
    /*    if(!trackPrimary){
    std::cout << "PRIMARYRS : " << track->GetParticleDefinition()->GetParticleName() << std::endl;
    trackPrimary=true;
    }*/
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

  // 1. Loop through each track independently to eliminate coincidence summing
  for (auto const& [trackID, energyDeposited] : fTrackEnergyMap) {
    
    if (energyDeposited > 0.001) { // 1 keV threshold
      
      // 2. Apply your resolution smearing to the isolated energy deposit
      double smeared_eDep = SmearSimulationData(energyDeposited);
      double adcChannel   = energyDeposited * 10000 * 0.6320;

      // 3. Fill your primary plotting columns with the SMEARED, isolated data
      analMan->FillNtupleDColumn(2, 10, energyDeposited); // Raw isolated energy
      analMan->FillNtupleDColumn(2, 11, smeared_eDep);     // Smeared isolated energy
      analMan->FillNtupleDColumn(2, 12, adcChannel);
    }
  }
}


#if(0)
void NonSegmented_Slab_SD::EndOfEvent(G4HCofThisEvent *hce)
{
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();

  // Loop over every track that deposited energy during this event
  for (auto const& [trackID, energyDeposited] : fTrackEnergyMap) {
    
    if (energyDeposited > 0.) {
      // Smear the energy of this individual track contribution
      double smeared_eDep = SmearSimulationData(energyDeposited);
      double adcChannel   = energyDeposited * 10000 * 0.6320;

      // Fill Ntuple 1 with individual independent track depositions
      analMan->FillNtupleDColumn(1, 10, energyDeposited); 
      
      // If you are using Ntuple 2 for filtered single photon data:
      if (gammaVec.size() == 1) {
        // Corrected index to 0 to prevent out-of-bounds errors
        //std::cout << "RAMAN : Filling for energy : " << energyVec << std::endl; 
        analMan->FillNtupleDColumn(2, 10, energyDeposited);
        analMan->FillNtupleDColumn(2, 11, smeared_eDep);
        analMan->FillNtupleDColumn(2, 12, adcChannel);
      }
    }
  }
}
#endif
#if(0)

void NonSegmented_Slab_SD::EndOfEvent(G4HCofThisEvent *hce)
{

  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  analMan->FillNtupleDColumn(1, 10, fEDep);

  // double sigma_E = sqrt(0.001658 * fEDep + 0.000314 * fEDep * fEDep);
  // double sigma_E = sqrt(0.001258 * fEDep + 0.000214 * fEDep * fEDep);
  // double smeared_eDep = gRandom->Gaus(fEDep, sigma_E);

  double smeared_eDep = SmearSimulationData(fEDep);
  double adcChannel   = fEDep * 10000 * 0.6320;
  if (gammaVec.size() == 1) {
    counter++;
    //std::cout << "RAMAN : Filling for energy : " << energyVec[1] << " : Counter : " << counter <<  std::endl;
    analMan->FillNtupleDColumn(2, 10, fEDep);
    analMan->FillNtupleDColumn(2, 11, smeared_eDep);
    analMan->FillNtupleDColumn(2, 12, adcChannel);
  }

  /*  if(gammaVec.size()>1)
    std::cout << "RAMAN : EDEP : " <<  fEDep << std::endl;*/

  /*unsigned int entries = fSlabHitCollection->entries();

  for (unsigned int i = 0; i < entries; i++) {
    (*fSlabHitCollection)[i]->Print();
  }*/
}

#endif
