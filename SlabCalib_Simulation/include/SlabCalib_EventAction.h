#ifndef SLABCALIB_EVENTACTION_HH
#define SLABCALIB_EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
class SlabCalib_EventAction : public G4UserEventAction {

/*bool fSlabHit;
bool fBar1Hit;
bool fBar2Hit;*/

public:
    SlabCalib_EventAction();
    virtual ~SlabCalib_EventAction();
    virtual void BeginOfEventAction(const G4Event *event);
    virtual void EndOfEventAction(const G4Event *event);
};

#endif
