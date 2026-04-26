#include "MuonDecay_RunAction.h"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
MuonDecay_RunAction::MuonDecay_RunAction() {}

MuonDecay_RunAction::~MuonDecay_RunAction() {}

void MuonDecay_RunAction::BeginOfRunAction(const G4Run *) {
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  analMan->OpenFile("output.root");
  analMan->CreateNtupleDColumn("Channel");
  analMan->CreateNtupleDColumn("Samples", fPulse);
  analMan->FinishNtuple();
}

void MuonDecay_RunAction::EndOfRunAction(const G4Run *) {
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  analMan->Write();
  analMan->CloseFile();
}
