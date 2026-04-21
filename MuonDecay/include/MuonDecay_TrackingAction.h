#ifndef MUONDECAY_TRACKINGACTION_HH
#define MUONDECAY_TRACKINGACTION_HH

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"

class MuonDecay_TrackingAction : public G4UserTrackingAction {
public:
    MuonDecay_TrackingAction();
    virtual ~MuonDecay_TrackingAction();
    virtual void PreUserTrackingAction(const G4Track *track);
    virtual void PostUserTrackingAction(const G4Track *track);
};

#endif
