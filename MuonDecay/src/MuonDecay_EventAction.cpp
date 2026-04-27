#include "MuonDecay_EventAction.h"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "MuonDecay_HitCollections.h"
#include <vector>
#include <G4AnalysisManager.hh>
#include "MuonDecay_RunAction.h"
#include "G4RunManager.hh"

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
  G4int hcID                          = G4SDManager::GetSDMpointer()->GetCollectionID("PMT_Collection");
  PMT_HitCollection *pmtHitCollection = static_cast<PMT_HitCollection *>(hce->GetHC(hcID));

  for (unsigned int i = 0; i < pmtHitCollection->entries(); i++) {
    MuonDecay_PMT_Hit *hit = (*pmtHitCollection)[i];
    hit->Print();
    fVecOfPulses[hit->GetChannelNum()].push_back(hit->GetTimestamp());
  }

  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  // auto baseAction = G4RunManager::GetRunManager()->GetUserRunAction();
  // auto runAction = static_cast<MuonDecay_RunAction*>(const_cast<G4UserRunAction*>(baseAction));
  for (unsigned int i = 0; i < fVecOfPulses.size(); i++) {
    if (fVecOfPulses[i].size() > 0) {
      analMan->FillNtupleDColumn(0, 0, i);
      fRunAction->FillPulseVector(fVecOfPulses[i]);
      analMan->AddNtupleRow(0);
    }
  }

  for (auto pulse : fVecOfPulses) {
    if (pulse.size() > 0) std::cout << "Pulse size : " << pulse.size() << std::endl;
  }
}
