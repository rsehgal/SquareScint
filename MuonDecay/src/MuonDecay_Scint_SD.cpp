#include "MuonDecay_Scint_SD.h"
#include "MuonDecay_Scint_Hit.h"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"
#include "G4VProcess.hh"

int MuonDecay_Scint_SD::survivedMuPlus  = 0;
int MuonDecay_Scint_SD::survivedMuMinus = 0;

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

  G4AnalysisManager *analMan = G4AnalysisManager::Instance();


  G4String creatorProcess = "";
  if (track->GetCreatorProcess()) {
    creatorProcess = track->GetCreatorProcess()->GetProcessName();
    fScintHitCollection->insert(new MuonDecay_Scint_Hit(particleName, track->GetTouchable()->GetVolume()->GetCopyNo(),
                                                        track->GetGlobalTime(), track->GetTrackID(),
                                                        track->GetParentID(), creatorProcess));
  }

  if ((particleName == "e-" || particleName == "e+") && track->GetParentID() == 1 &&
      track->GetCurrentStepNumber() == 1) {
    if (energy > 5 * MeV) {
      G4double t_decay = track->GetGlobalTime();
      // G4double t_decay = track->GetLocalTime();
      /* std::cout << "SEHGAL ELECTRON CREATED/MOVING AT: " << t_decay << " ns"
                 << " : TrackID : " << track->GetTrackID() << " : ParentID : " << track->GetParentID() << std::endl;*/
      analMan->FillNtupleDColumn(1, 0, track->GetGlobalTime() / 1000.);
      // analMan->FillNtupleDColumn(1,0,t_decay);
      analMan->AddNtupleRow(1);

      if (particleName == "e-") survivedMuMinus++;
      if (particleName == "e+") survivedMuPlus++;
    }
  }
  return true;
}

void MuonDecay_Scint_SD::EndOfEvent(G4HCofThisEvent *hce) {}
