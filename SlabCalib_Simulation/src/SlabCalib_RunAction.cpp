#include "SlabCalib_RunAction.h"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
SlabCalib_RunAction::SlabCalib_RunAction() {}

SlabCalib_RunAction::~SlabCalib_RunAction() {}

void SlabCalib_RunAction::BeginOfRunAction(const G4Run *)
{
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  analMan->OpenFile("simOutput.root");
  analMan->CreateNtuple("ftree", "A simple hit tree");
  analMan->CreateNtupleDColumn("x");
  analMan->CreateNtupleDColumn("y");
  analMan->CreateNtupleDColumn("z");
  analMan->FinishNtuple();
}

void SlabCalib_RunAction::EndOfRunAction(const G4Run *)
{

  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  analMan->Write();
  analMan->CloseFile();
}
