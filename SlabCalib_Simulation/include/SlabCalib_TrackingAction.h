#ifndef SLABCALIB_TRACKINGACTION_HH
#define SLABCALIB_TRACKINGACTION_HH

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"

class SlabCalib_TrackingAction : public G4UserTrackingAction {
public:
    SlabCalib_TrackingAction();
    virtual ~SlabCalib_TrackingAction();
    virtual void PreUserTrackingAction(const G4Track *track);
    virtual void PostUserTrackingAction(const G4Track *track);
};

#endif
