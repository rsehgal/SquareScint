#ifndef MUONDECAY_EVENTACTION_HH
#define MUONDECAY_EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include <vector>

class MuonDecay_RunAction;

class MuonDecay_EventAction : public G4UserEventAction {
    std::vector<std::vector<double>> fVecOfPulses; 
    MuonDecay_RunAction *fRunAction=nullptr;
public:
    MuonDecay_EventAction();
    virtual ~MuonDecay_EventAction();
    virtual void BeginOfEventAction(const G4Event *event);
    virtual void EndOfEventAction(const G4Event *event);
};

#endif
