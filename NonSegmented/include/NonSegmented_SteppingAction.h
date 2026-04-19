#ifndef NONSEGMENTED_STEPPINGACTION_HH
#define NONSEGMENTED_STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"

class NonSegmented_SteppingAction : public G4UserSteppingAction {
public:
    NonSegmented_SteppingAction();
    virtual ~NonSegmented_SteppingAction();

    virtual void UserSteppingAction(const G4Step*);
};

#endif
