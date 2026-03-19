#ifndef SLABCALIB_PRIMARYGENERATORACTION_HH
#define SLABCALIB_PRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

class CryInterface;

class SlabCalib_PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
#ifdef USE_CRY
    CryInterface *cryInterface;
#endif
public:
    SlabCalib_PrimaryGeneratorAction();
    virtual ~SlabCalib_PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* anEvent);
public:
    G4ParticleGun *fParticleGun;
};

#endif
