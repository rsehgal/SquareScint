#ifndef MUONDECAY_ACTIONINITIALIZATION_HH
#define MUONDECAY_ACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

class MuonDecay_ActionInitialization : public G4VUserActionInitialization {
public:
    MuonDecay_ActionInitialization();
    virtual ~MuonDecay_ActionInitialization();
    virtual void BuildForMaster() const override;
    virtual void Build() const override;
};
#endif
