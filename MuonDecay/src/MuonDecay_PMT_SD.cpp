#include "MuonDecay_PMT_SD.h"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "MuonDecay_HitCollections.h"

MuonDecay_PMT_SD::MuonDecay_PMT_SD(const G4String &name,
                                   const G4String &collName)
    : G4VSensitiveDetector(name) {
  collectionName.insert(collName);
}

MuonDecay_PMT_SD::~MuonDecay_PMT_SD() {}

void MuonDecay_PMT_SD::Initialize(G4HCofThisEvent *hce) {
  fPMTHitCollection =
      new PMT_HitCollection(SensitiveDetectorName, collectionName[0]);
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fPMTHitCollection);
}

G4bool MuonDecay_PMT_SD::ProcessHits(G4Step *step, G4TouchableHistory *) {
  G4Track *track = step->GetTrack();
  G4double energy = track->GetKineticEnergy();
  // G4cout << "Detected energy: " << energy / MeV << " MeV" << G4endl;
  int channelNum = track->GetTouchable()->GetVolume()->GetCopyNo();
  double ts = track->GetGlobalTime();
  MuonDecay_PMT_Hit *hit = new MuonDecay_PMT_Hit(channelNum, ts);
  fPMTHitCollection->insert(hit);
  return true;
}

void MuonDecay_PMT_SD::EndOfEvent(G4HCofThisEvent *hce) {
#if(0)
  for (unsigned int i = 0; i < fPMTHitCollection->entries(); i++) {
    MuonDecay_PMT_Hit *hit = (*fPMTHitCollection)[i];
    hit->Print();
  }
#endif
}
