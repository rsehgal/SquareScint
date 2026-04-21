#include "MuonDecay_SteppingAction.h"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
MuonDecay_SteppingAction::MuonDecay_SteppingAction() {}

MuonDecay_SteppingAction::~MuonDecay_SteppingAction() {}

void MuonDecay_SteppingAction::UserSteppingAction(const G4Step *step) {
  G4Track *track = step->GetTrack();
  G4double energy = track->GetKineticEnergy();

  G4StepPoint *postStepPoint = step->GetPostStepPoint();
  const G4VProcess *processStep = postStepPoint->GetProcessDefinedStep();

  if (track->GetCreatorProcess())
    G4cout << "TrackID : " << track->GetTrackID()
           << " : ParentID : " << track->GetParentID()
           << " : Creator Process : "
           << track->GetCreatorProcess()->GetProcessName()
           << " :: Particle : " << track->GetDefinition()->GetParticleName()
           << " :: EDep : " << step->GetTotalEnergyDeposit()
           << " :: ProcessStep : " << processStep->GetProcessName()
           << " :: KE : " << track->GetKineticEnergy()
           << " :: GlobalTime : " << track->GetGlobalTime() << G4endl;
  else
    G4cout << "TrackID : " << track->GetTrackID()
           << " : ParentID : " << track->GetParentID()
           << " : Creator Process : " << " SYSTEM"
           << " :: Particle : " << track->GetDefinition()->GetParticleName()
           << " :: EDep : " << step->GetTotalEnergyDeposit()
           << " :: ProcessStep : " << processStep->GetProcessName()
           << " :: KE : " << track->GetKineticEnergy()
           << " :: GlobalTime : " << track->GetGlobalTime() << G4endl;
}
