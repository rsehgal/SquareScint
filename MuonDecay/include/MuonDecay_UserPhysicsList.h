#ifndef _HH
#define _HH

#include "G4VUserPhysicsList.hh"

class MuonDecay_UserPhysicsList : public G4VUserPhysicsList {
public:
    MuonDecay_UserPhysicsList();
    virtual ~MuonDecay_UserPhysicsList();
    virtual void ConstructParticle(); 
    virtual void ConstructProcess(); 
};

#endif
