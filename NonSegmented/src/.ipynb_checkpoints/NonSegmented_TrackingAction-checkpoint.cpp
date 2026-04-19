#include "NonSegmented_TrackingAction.h"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"

NonSegmented_TrackingAction::NonSegmented_TrackingAction()  {
}

NonSegmented_TrackingAction::~NonSegmented_TrackingAction() {}

void NonSegmented_TrackingAction::PreUserTrackingAction(const G4Track *track){
    
//std::cout <<"Tracking Action begin....." << std::endl;
if(track->GetParticleDefinition()->GetParticleName()=="gamma") 
std::cout << "VolumeName : " << track->GetVolume()->GetName() << " :: ParticleName : " << track->GetParticleDefinition()->GetParticleName() << " :: Energy : " << track->GetKineticEnergy() << " :: CreatorProcess : " << track->GetCreatorProcess()->GetProcessName() <<  std::endl;
}
void NonSegmented_TrackingAction::PostUserTrackingAction(const G4Track *track){
//std::cout <<"Tracking Action ends....." << std::endl;
}

