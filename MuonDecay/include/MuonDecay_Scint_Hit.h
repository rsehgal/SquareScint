#ifndef MUONDECAY_SCINT_HIT_HH
#define MUONDECAY_SCINT_HIT_HH

#include "G4VHit.hh"
#include "G4String.hh"

class MuonDecay_Scint_Hit : public G4VHit {
    G4String fParticleName;
    int fChannelNum;
    double fTimestamp;
public:
    MuonDecay_Scint_Hit();
    MuonDecay_Scint_Hit(G4String particle, int chNum, double ts);
    virtual ~MuonDecay_Scint_Hit();
    void Print() const;
};

#endif
