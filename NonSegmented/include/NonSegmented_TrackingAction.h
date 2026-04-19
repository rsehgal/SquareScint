#ifndef NONSEGMENTED_TRACKINGACTION_HH
#define NONSEGMENTED_TRACKINGACTION_HH

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"

class NonSegmented_TrackingAction : public G4UserTrackingAction {
public:
    NonSegmented_TrackingAction();
    virtual ~NonSegmented_TrackingAction();
    virtual void PreUserTrackingAction(const G4Track *track);
    virtual void PostUserTrackingAction(const G4Track *track);
};

#endif
