#include "MuonDecay_ActionInitialization.h"
#include "MuonDecay_PrimaryGeneratorAction.h"
#include "MuonDecay_SteppingAction.h"
#include "MuonDecay_TrackingAction.h"
#include "MuonDecay_EventAction.h"
#include "MuonDecay_RunAction.h"

MuonDecay_ActionInitialization::MuonDecay_ActionInitialization() : G4VUserActionInitialization() {}
MuonDecay_ActionInitialization::~MuonDecay_ActionInitialization() {}

void MuonDecay_ActionInitialization::BuildForMaster() const {
    //Mandatory for Multithreading mode
    SetUserAction(new MuonDecay_RunAction());
}

void MuonDecay_ActionInitialization::Build() const {
    //Mandatory
    SetUserAction(new MuonDecay_PrimaryGeneratorAction());

    //Data Management (Recommended)
    //SetUserAction(new MuonDecay_RunAction);
    //SetUserAction(new MuonDecay_EventAction);

    //Granular Control (Optional)
    SetUserAction(new MuonDecay_SteppingAction());
    //SetUserAction(new MuonDecay_TrackingAction());
}
