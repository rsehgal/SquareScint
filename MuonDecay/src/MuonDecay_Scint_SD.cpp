#include "MuonDecay_Scint_SD.h"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"

MuonDecay_Scint_SD::MuonDecay_Scint_SD(const G4String &name, const G4String &collName) : G4VSensitiveDetector(name)
{
  collectionName.insert(collName);
}

MuonDecay_Scint_SD::~MuonDecay_Scint_SD() {}

void MuonDecay_Scint_SD::Initialize(G4HCofThisEvent *hce)
{
  fScintHitCollection = new Scint_HitCollection(SensitiveDetectorName, collectionName[0]);
  G4int hcId          = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcId, fScintHitCollection);
}

G4bool MuonDecay_Scint_SD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  G4Track *track  = step->GetTrack();
  G4double energy = track->GetKineticEnergy();
  // G4cout << "Detected energy: " << energy / MeV << " MeV" << G4endl;
  G4String particleName = track->GetParticleDefinition()->GetParticleName();

  /*  if (particleName == "mu-" || particleName == "mu+") {
      //if (track->GetTrackStatus() == fStopButAlive || track->GetTrackStatus() == fStopAndKill)
      if (track->GetTrackStatus() == fStopAndKill)
        std::cout <<"ParticleName : " << particleName<< " : ID : " << track->GetTrackID() << " : ParentID : " <<
    track->GetParentID()<<  " : ParticleState : " << track->GetTrackStatus() << " : KE : " << energy << " : Timestamp :
    " << track->GetGlobalTime() << std::endl;
    }*/

  /*  if (particleName == "mu-" && track->GetTrackStatus() == fStopButAlive) {
      G4double t_stop = track->GetGlobalTime();
      std::cout << "SEHGAL MUON STOPPED AT: " << t_stop << " ns"
                << " : TrackID : " << track->GetTrackID() << " : ParentID : " << track->GetParentID() << std::endl;
    }*/

  // If it's the resulting electron moving
  
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();

  if ((particleName == "e-" || particleName == "e+") && track->GetParentID() == 1 &&
      track->GetCurrentStepNumber() == 1) {
    if (energy > 5 * MeV) {
      G4double t_decay = track->GetGlobalTime();
      /*std::cout << "SEHGAL ELECTRON CREATED/MOVING AT: " << t_decay << " ns"
                << " : TrackID : " << track->GetTrackID() << " : ParentID : " << track->GetParentID() << std::endl;*/
     analMan->FillNtupleDColumn(1,0,track->GetGlobalTime()/1000.);
     analMan->AddNtupleRow(1);
    }
  }
  return true;
}

void MuonDecay_Scint_SD::EndOfEvent(G4HCofThisEvent *hce) {}
