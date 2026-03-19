#ifndef SLABCALIB_USERPHYSICSLIST_HH
#define SLABCALIB_USERPHYSICSLIST_HH

#include "G4VUserPhysicsList.hh"

class SlabCalib_UserPhysicsList : public G4VUserPhysicsList {
public:
    SlabCalib_UserPhysicsList();
    virtual ~SlabCalib_UserPhysicsList();
    virtual void ConstructParticle(); 
    virtual void ConstructProcess(); 
};

#endif
