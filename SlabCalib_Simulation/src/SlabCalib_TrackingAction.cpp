#include "SlabCalib_TrackingAction.h"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

SlabCalib_TrackingAction::SlabCalib_TrackingAction()  {
}

SlabCalib_TrackingAction::~SlabCalib_TrackingAction() {}

void SlabCalib_TrackingAction::PreUserTrackingAction(const G4Track *track){
std::cout <<"Tracking Action begin....." << std::endl;
}

void SlabCalib_TrackingAction::PostUserTrackingAction(const G4Track *track){
std::cout <<"Tracking Action ends....." << std::endl;
}

