#ifndef SLABCALIB_STEPPINGACTION_HH
#define SLABCALIB_STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"

class SlabCalib_SteppingAction : public G4UserSteppingAction {
public:
    SlabCalib_SteppingAction();
    virtual ~SlabCalib_SteppingAction();

    virtual void UserSteppingAction(const G4Step*);
};

#endif
