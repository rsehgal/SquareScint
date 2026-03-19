#ifndef SLABCALIB_RUNACTION_HH
#define SLABCALIB_RUNACTION_HH

class G4Run;

#include "G4UserRunAction.hh"
#include "G4Event.hh"
class SlabCalib_RunAction : public G4UserRunAction {
public:
    SlabCalib_RunAction();
    virtual ~SlabCalib_RunAction();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
};

#endif
