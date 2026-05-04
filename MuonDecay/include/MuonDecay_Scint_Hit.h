#ifndef MUONDECAY_SCINT_HIT_HH
#define MUONDECAY_SCINT_HIT_HH

#include "G4VHit.hh"
#include "G4String.hh"

class MuonDecay_Scint_Hit : public G4VHit {
public:
    G4String fParticleName;
    int fChannelNum;
    double fTimestamp;
    G4int fTrackId;
    G4int fParentId;
    G4String fCreatorProcess;
public:
    MuonDecay_Scint_Hit();
    MuonDecay_Scint_Hit(G4String particle, int chNum, double ts);
    MuonDecay_Scint_Hit(G4String particle, int chNum, double ts,G4int trId, G4int parId,G4String crProcess);
    MuonDecay_Scint_Hit(const MuonDecay_Scint_Hit &obj);
    virtual ~MuonDecay_Scint_Hit();
    bool operator<(const MuonDecay_Scint_Hit& other) const;
    void Print() const;
};

#endif
