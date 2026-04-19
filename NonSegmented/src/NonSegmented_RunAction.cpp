#include "NonSegmented_RunAction.h"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
NonSegmented_RunAction::NonSegmented_RunAction() {}
NonSegmented_RunAction::NonSegmented_RunAction(G4String outfileName) : fOutfileName(outfileName) {}

NonSegmented_RunAction::~NonSegmented_RunAction() {}

void NonSegmented_RunAction::BeginOfRunAction(const G4Run *)
{
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  //analMan->OpenFile("out.root");
  analMan->OpenFile(fOutfileName);
  // Creating a TTree
  analMan->CreateNtuple("ftree", "A simple tree");
  analMan->CreateNtupleDColumn("pmtNo");
  analMan->CreateNtupleDColumn("arrivalTime");
  analMan->CreateNtupleDColumn("eventNum");
  analMan->FinishNtuple();

  analMan->CreateNtuple("delTTree", "A simple tree storing diff of 2,3&4 Pmt with 1");
  /*analMan->CreateNtupleDColumn("delT_21");
  analMan->CreateNtupleDColumn("delT_31");
  analMan->CreateNtupleDColumn("delT_41");*/
  analMan->CreateNtupleDColumn("t0");
  analMan->CreateNtupleDColumn("t1");
  analMan->CreateNtupleDColumn("t2");
  analMan->CreateNtupleDColumn("t3");
  analMan->CreateNtupleDColumn("q0");
  analMan->CreateNtupleDColumn("q1");
  analMan->CreateNtupleDColumn("q2");
  analMan->CreateNtupleDColumn("q3");
  analMan->CreateNtupleDColumn("x");
  analMan->CreateNtupleDColumn("z");
  analMan->CreateNtupleDColumn("eDep");
  analMan->FinishNtuple();

}

void NonSegmented_RunAction::EndOfRunAction(const G4Run *)
{
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  analMan->Write();
  analMan->CloseFile();
}
