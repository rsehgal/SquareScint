#include "MuonDecay_RunAction.h"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "MuonDecay_PrimaryGeneratorAction.h"
#include "MuonDecay_Scint_SD.h"

MuonDecay_RunAction::MuonDecay_RunAction() {}

MuonDecay_RunAction::~MuonDecay_RunAction() {}

void MuonDecay_RunAction::BeginOfRunAction(const G4Run *)
{
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  analMan->OpenFile("output.root");
  analMan->CreateNtuple("ftree", "A DAQ tree");
  analMan->CreateNtupleDColumn("Channel");
  analMan->CreateNtupleDColumn("Samples", fPulse);
  analMan->FinishNtuple();

  analMan->CreateNtuple("Decay", "A simple decay tree");
  analMan->CreateNtupleDColumn("delT");
  analMan->FinishNtuple();

  analMan->CreateNtuple("geant","A simple geant tracking tree");
  analMan->CreateNtupleSColumn("particle");
  analMan->CreateNtupleSColumn("creatorProcess");
  analMan->CreateNtupleIColumn("Channel");
  analMan->CreateNtupleDColumn("Timestamp");
  analMan->CreateNtupleIColumn("trackId"); 
  analMan->CreateNtupleIColumn("parentId"); 
  analMan->CreateNtupleIColumn("eventId");
  analMan->FinishNtuple(); 
}

void MuonDecay_RunAction::EndOfRunAction(const G4Run *)
{
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  analMan->Write();
  analMan->CloseFile();

  std::cout << "MuPlus : " << MuonDecay_PrimaryGeneratorAction::muplus << std::endl;
  std::cout << "MuMinu : " << MuonDecay_PrimaryGeneratorAction::muminus << std::endl;

  std::cout << "--------------------------------------" << std::endl;
  std::cout << "Survived Muplus : " << MuonDecay_Scint_SD::survivedMuPlus << std::endl;
  std::cout << "Survived MuMinus : " << MuonDecay_Scint_SD::survivedMuMinus << std::endl;
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
