#include "NonSegmented_PMT_SD.h"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "NonSegmented_PMT_Hit.h"
#include "G4AnalysisManager.hh"
#include "Helpers.h"
#include "G4EventManager.hh"
NonSegmented_PMT_SD::NonSegmented_PMT_SD(const G4String &name, const G4String &collName) : G4VSensitiveDetector(name)
{
  collectionName.insert(collName);
}

NonSegmented_PMT_SD::~NonSegmented_PMT_SD() {}

void NonSegmented_PMT_SD::Initialize(G4HCofThisEvent *hce)
{
  fPMTHitCollection = new PMT_HitCollection(SensitiveDetectorName, collectionName[0]);
  G4int hcID        = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fPMTHitCollection);
}

G4bool NonSegmented_PMT_SD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  G4Track *track            = step->GetTrack();
  G4double energy           = track->GetKineticEnergy();
  NonSegmented_PMT_Hit *hit = new NonSegmented_PMT_Hit;
  //G4cout << "Detected energy: " << energy / MeV << " MeV" << G4endl;

  int copyNum   = track->GetTouchable()->GetVolume()->GetCopyNo();
  //double timing = track->GetGlobalTime();
  double timing = track->GetLocalTime();
  //std::cout << "CopyNo : " << copyNum<< " :: Timing : " << timing << std::endl;

  hit->Set(copyNum, timing);
  //hit->Print();
  fPMTHitCollection->insert(hit);
  return true;
}

void NonSegmented_PMT_SD::EndOfEvent(G4HCofThisEvent *hce)
{
#if(0)
  std::vector<std::vector<double>> vecOfPhotonArrivalTime(4);
  // vecOfPhotonArrivalTime.resize(4);
  unsigned int entries = fPMTHitCollection->entries();
  // std::cout << "Entries : " << entries << std::endl;
  for (unsigned int i = 0; i < entries; i++) {
    NonSegmented_PMT_Hit *hit = (*fPMTHitCollection)[i];
    // hit->Print();
    //(*fPMTHitCollection)[i]->Print();
    vecOfPhotonArrivalTime[hit->GetPmtID() - 1].push_back(hit->GetPhotonArrivalTime());
  }

  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  unsigned int eventId       = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  for (unsigned int i = 0; i < vecOfPhotonArrivalTime.size(); i++) {
    double timing = GetTiming(vecOfPhotonArrivalTime[i]);
    analMan->FillNtupleDColumn(0, 1, timing);
    analMan->FillNtupleDColumn(0, 0, i);
    analMan->FillNtupleDColumn(0, 2, eventId);
    analMan->AddNtupleRow(0);
  }

  bool detectedByAll = true;
  for (unsigned int i = 0; i < vecOfPhotonArrivalTime.size(); i++) {
    detectedByAll &= vecOfPhotonArrivalTime[i].size() > 0;
  }
  if (detectedByAll) {
    double timing0 = GetTiming(vecOfPhotonArrivalTime[0]);
    for (unsigned int i = 1; i < vecOfPhotonArrivalTime.size(); i++) {
      analMan->FillNtupleDColumn(1, i - 1, timing0 - GetTiming(vecOfPhotonArrivalTime[i]));
    }
    analMan->AddNtupleRow(1);
  }
#endif
}
