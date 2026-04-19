#ifndef NONSEGMENTED_USERPHYSICSLIST_HH
#define NONSEGMENTED_USERPHYSICSLIST_HH

#include "G4VUserPhysicsList.hh"

class NonSegmented_UserPhysicsList : public G4VUserPhysicsList {
public:
    NonSegmented_UserPhysicsList();
    virtual ~NonSegmented_UserPhysicsList();
    virtual void ConstructParticle(); 
    virtual void ConstructProcess(); 
};

#endif
