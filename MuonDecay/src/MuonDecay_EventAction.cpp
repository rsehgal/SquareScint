#include "MuonDecay_EventAction.h"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "MuonDecay_HitCollections.h"

MuonDecay_EventAction::MuonDecay_EventAction() {}

MuonDecay_EventAction::~MuonDecay_EventAction() {}

void MuonDecay_EventAction::BeginOfEventAction(const G4Event *event) {
  // TODO : All the required logic you want to do at the start
  //        of each event
}

void MuonDecay_EventAction::EndOfEventAction(const G4Event *event) {
  // TODO : All the required logic you want to do at the end
  //        of each event

  G4HCofThisEvent *hce = event->GetHCofThisEvent();
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID("PMT_Collection");
  PMT_HitCollection *pmtHitCollection =
      static_cast<PMT_HitCollection *>(hce->GetHC(hcID));

  for (unsigned int i = 0; i < pmtHitCollection->entries(); i++) {
    MuonDecay_PMT_Hit *hit = (*pmtHitCollection)[i];
    hit->Print();
  }
}
