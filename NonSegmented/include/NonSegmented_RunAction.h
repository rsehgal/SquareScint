#ifndef NONSEGMENTED_RUNACTION_HH
#define NONSEGMENTED_RUNACTION_HH

class G4Run;

#include "G4UserRunAction.hh"
#include "G4Event.hh"
#include <vector>
class NonSegmented_RunAction : public G4UserRunAction {
    G4String fOutfileName;
    std::vector<double> fPulse;
public:
    NonSegmented_RunAction();
    NonSegmented_RunAction(G4String outfileName);
    virtual ~NonSegmented_RunAction();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    void FillPulse(std::vector<double> pulse);
};

#endif
