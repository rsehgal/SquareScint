#ifndef MUONDECAY_PRIMARYGENERATORACTION_HH
#define MUONDECAY_PRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

class MuonDecay_PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    MuonDecay_PrimaryGeneratorAction();
    virtual ~MuonDecay_PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* anEvent);
public:
    G4ParticleGun *fParticleGun;
};

#endif
