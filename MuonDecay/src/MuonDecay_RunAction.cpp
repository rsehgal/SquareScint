#include "MuonDecay_RunAction.h"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
MuonDecay_RunAction::MuonDecay_RunAction() {}

MuonDecay_RunAction::~MuonDecay_RunAction() {}

void MuonDecay_RunAction::BeginOfRunAction(const G4Run *)
{
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  analMan->OpenFile("output.root");
  analMan->CreateNtuple("ftree","A DAQ tree");
  analMan->CreateNtupleDColumn("Channel");
  analMan->CreateNtupleDColumn("Samples", fPulse);
  analMan->FinishNtuple();

  analMan->CreateNtuple("Decay","A simple decay tree");
  analMan->CreateNtupleDColumn("delT");
  analMan->FinishNtuple();
}

void MuonDecay_RunAction::EndOfRunAction(const G4Run *)
{
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  analMan->Write();
  analMan->CloseFile();
}

std::vector<double> MuonDecay_RunAction::GetPulseVector() const
{
  return fPulse;
}

void MuonDecay_RunAction::FillPulseVector(std::vector<double> pulse)
{
  fPulse.clear();
  for (auto sample : pulse) {
    fPulse.push_back(sample);
  }
}
