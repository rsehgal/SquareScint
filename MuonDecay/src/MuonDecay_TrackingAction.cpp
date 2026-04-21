#include "MuonDecay_TrackingAction.h"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

MuonDecay_TrackingAction::MuonDecay_TrackingAction()  {
}

MuonDecay_TrackingAction::~MuonDecay_TrackingAction() {}

void MuonDecay_TrackingAction::PreUserTrackingAction(const G4Track *track){
std::cout <<"Tracking Action begin....." << std::endl;
}

void MuonDecay_TrackingAction::PostUserTrackingAction(const G4Track *track){
std::cout <<"Tracking Action ends....." << std::endl;
}

