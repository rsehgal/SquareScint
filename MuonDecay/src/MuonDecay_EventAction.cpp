#include "MuonDecay_EventAction.h"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "MuonDecay_HitCollections.h"
#include <vector>
#include <G4AnalysisManager.hh>
#include "MuonDecay_RunAction.h"
#include "G4RunManager.hh"
#include <set>
MuonDecay_EventAction::MuonDecay_EventAction()
{
  auto baseAction = const_cast<G4UserRunAction *>(G4RunManager::GetRunManager()->GetUserRunAction());
  fRunAction      = static_cast<MuonDecay_RunAction *>(baseAction);
}

MuonDecay_EventAction::~MuonDecay_EventAction() {}

void MuonDecay_EventAction::BeginOfEventAction(const G4Event *event)
{
  // TODO : All the required logic you want to do at the start
  //        of each event
  fVecOfPulses.clear();
}

void MuonDecay_EventAction::EndOfEventAction(const G4Event *event)
{
  // TODO : All the required logic you want to do at the end
  //        of each event
  fVecOfPulses.resize(4);

  G4HCofThisEvent *hce                = event->GetHCofThisEvent();
/*  G4int hcID                          = G4SDManager::GetSDMpointer()->GetCollectionID("PMT_Collection");
  PMT_HitCollection *pmtHitCollection = static_cast<PMT_HitCollection *>(hce->GetHC(hcID));

  if (pmtHitCollection && pmtHitCollection->entries() > 0) {
    for (unsigned int i = 0; i < pmtHitCollection->entries(); i++) {
      MuonDecay_PMT_Hit *hit = (*pmtHitCollection)[i];
      // hit->Print();
      fVecOfPulses[hit->GetChannelNum()].push_back(hit->GetTimestamp());
    }
  }
*/
  G4int hcScintID                         = G4SDManager::GetSDMpointer()->GetCollectionID("Scint_Collection");
  Scint_HitCollection *scintHitCollection = static_cast<Scint_HitCollection *>(hce->GetHC(hcScintID));


  std::set<MuonDecay_Scint_Hit> scintHitSet;
  if (scintHitCollection->entries() > 0) {
    // std::cout << "----------------- RAMAN ---------------------" << std::endl;
    for (unsigned int i = 0; i < scintHitCollection->entries(); i++) {
      MuonDecay_Scint_Hit *scintHit = (*scintHitCollection)[i];
      // scintHit->Print();
      scintHitSet.insert(*scintHit);
    }
  }
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  // Trying to print the hitSet to check for unique elements
  for (const auto &scintHit : scintHitSet) {
    // scintHit.Print();
    G4int eventId = event->GetEventID(); // G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    analMan->FillNtupleSColumn(2, 0, scintHit.fParticleName);
    analMan->FillNtupleSColumn(2, 1, scintHit.fCreatorProcess);
    analMan->FillNtupleIColumn(2, 2, scintHit.fChannelNum);
    analMan->FillNtupleDColumn(2, 3, scintHit.fTimestamp);
    analMan->FillNtupleIColumn(2, 4, scintHit.fTrackId);
    analMan->FillNtupleIColumn(2, 5, scintHit.fParentId);
    analMan->FillNtupleIColumn(2, 6, eventId);
    analMan->AddNtupleRow(2);
  }

  // auto baseAction = G4RunManager::GetRunManager()->GetUserRunAction();
  // auto runAction = static_cast<MuonDecay_RunAction*>(const_cast<G4UserRunAction*>(baseAction));
  for (unsigned int i = 0; i < fVecOfPulses.size(); i++) {
    if (fVecOfPulses[i].size() > 0) {
      analMan->FillNtupleDColumn(0, 0, i);
      fRunAction->FillPulseVector(fVecOfPulses[i]);
      analMan->AddNtupleRow(0);
    }
  }

  /*for (auto pulse : fVecOfPulses) {
    if (pulse.size() > 0) std::cout << "Pulse size : " << pulse.size() << std::endl;
  }*/
}
