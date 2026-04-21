#ifndef MUONDECAY_STEPPINGACTION_HH
#define MUONDECAY_STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"

class MuonDecay_SteppingAction : public G4UserSteppingAction {
public:
    MuonDecay_SteppingAction();
    virtual ~MuonDecay_SteppingAction();

    virtual void UserSteppingAction(const G4Step*);
};

#endif
