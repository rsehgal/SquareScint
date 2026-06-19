#ifndef NONSEGMENTED_EVENTACTION_HH
#define NONSEGMENTED_EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "NonSegmented_RunAction.h"
class NonSegmented_EventAction : public G4UserEventAction {

    NonSegmented_RunAction *fRunAction;
public:
    NonSegmented_EventAction();
    virtual ~NonSegmented_EventAction();
    virtual void BeginOfEventAction(const G4Event *event);
    virtual void EndOfEventAction(const G4Event *event);
};

#endif
