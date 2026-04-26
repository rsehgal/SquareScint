#ifndef MUONDECAY_PMT_HIT_HH
#define MUONDECAY_PMT_HIT_HH

#include "G4VHit.hh"

class MuonDecay_PMT_Hit : public G4VHit {

    int fChannelNum;
    double fTimestamp;

public:
    MuonDecay_PMT_Hit();
    MuonDecay_PMT_Hit(int chNo, double ts);
    virtual ~MuonDecay_PMT_Hit();
    int GetChannelNum() const;
    double GetTimestamp() const;
    void Print();
};

#endif
