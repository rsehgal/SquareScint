#include "NonSegmented_SensitiveDetector.h"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4HCofThisEvent.hh"
#include "algorithm"
#include "G4AnalysisManager.hh"
#include "G4EventManager.hh"
#include "Helpers.h"
NonSegmented_SensitiveDetector::NonSegmented_SensitiveDetector(const G4String &name) : G4VSensitiveDetector(name)
{
  vecOfArrivalTime_PMT.resize(4);
}

NonSegmented_SensitiveDetector::~NonSegmented_SensitiveDetector() {}

void NonSegmented_SensitiveDetector::Initialize(G4HCofThisEvent *hitCollection)
{
  // vecOfArrivalTime_PMT1.clear();
  // vecOfArrivalTime_PMT2.clear();
  // vecOfArrivalTime_PMT3.clear();
  // vecOfArrivalTime_PMT4.clear();

  for (unsigned int i = 0; i < vecOfArrivalTime_PMT.size(); i++) {
    vecOfArrivalTime_PMT[i].clear();
  }
}

G4bool NonSegmented_SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  G4Track *track  = step->GetTrack();
  G4double energy = track->GetKineticEnergy();
  // G4cout << "Detected energy: " << energy / MeV << " MeV" << G4endl;

  // TODO :  Add Whatall information you want to get from steps

  int copyNum = track->GetTouchable()->GetVolume()->GetCopyNo();
  vecOfArrivalTime_PMT[copyNum - 1].push_back(track->GetGlobalTime());
  track->SetTrackStatus(fStopAndKill);

  return true;
}

void NonSegmented_SensitiveDetector::EndOfEvent(G4HCofThisEvent *hitCollection)
{

  unsigned int eventId       = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  for (unsigned int i = 0; i < vecOfArrivalTime_PMT.size(); i++) {

#if(0)
    std::sort(vecOfArrivalTime_PMT[i].begin(), vecOfArrivalTime_PMT[i].end());
    int timeIndex = 0.2 * vecOfArrivalTime_PMT[i].size();
    unsigned int timing = vecOfArrivalTime_PMT[i][timeIndex];   
    std::cout << "Timing : " << timing << std::endl; 
    analMan->FillNtupleDColumn(0, 1, vecOfArrivalTime_PMT[i][timeIndex]);
#else
    double timing = GetTiming(vecOfArrivalTime_PMT[i]);   
    //std::cout << "Timing : " << timing << std::endl; 
    analMan->FillNtupleDColumn(0, 1, timing);

    analMan->FillNtupleDColumn(0, 0, i);
    analMan->FillNtupleDColumn(0, 2, eventId);
    analMan->AddNtupleRow(0);
#endif
    /*std::cout << "========= PMT : " << (i + 1) << " :: Size : " << vecOfArrivalTime_PMT[i].size() << " :: Arrival Time
    using 20% quantile : "<< vecOfArrivalTime_PMT[i][timeIndex]<< " ===========" << std::endl;

    for (unsigned int j = 0; j < vecOfArrivalTime_PMT[i].size(); j++) {
      std::cout << vecOfArrivalTime_PMT[i][j] << ",";
    }
    std::cout << std::endl;*/
  }

  //std::cout << "++++++++++++++++++++++++++++++++++++++" << std::endl;
  bool detectedByAll = true;
  for (unsigned int i = 0; i < vecOfArrivalTime_PMT.size(); i++) {
    //std::cout << "Hit Size : " << vecOfArrivalTime_PMT[i].size() << std::endl;
    detectedByAll &= vecOfArrivalTime_PMT[i].size() > 0;
  }
  if (detectedByAll) {
    //std::cout << "All PMT fired............." << std::endl;

    //int timeIndex   = 0.2 * vecOfArrivalTime_PMT[0].size();
    //double pmt0Time = vecOfArrivalTime_PMT[0][timeIndex];
    double timing0 = GetTiming(vecOfArrivalTime_PMT[0]);
    for (unsigned int i = 1; i < vecOfArrivalTime_PMT.size(); i++) {
      //timeIndex = 0.2 * vecOfArrivalTime_PMT[i].size();
      //analMan->FillNtupleDColumn(1, i - 1, pmt0Time - vecOfArrivalTime_PMT[i][timeIndex]);
      analMan->FillNtupleDColumn(1, i - 1, timing0 - GetTiming(vecOfArrivalTime_PMT[i]));
    }
    analMan->AddNtupleRow(1);
  }
}
