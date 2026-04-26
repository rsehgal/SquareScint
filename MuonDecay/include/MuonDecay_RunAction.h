#ifndef MUONDECAY_RUNACTION_HH
#define MUONDECAY_RUNACTION_HH

class G4Run;

#include "G4UserRunAction.hh"
#include "G4Event.hh"
#include <vector>

class MuonDecay_RunAction : public G4UserRunAction {
    std::vector<double> fPulse;
public:
    MuonDecay_RunAction();
    virtual ~MuonDecay_RunAction();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
};

#endif
