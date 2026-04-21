#ifndef MUONDECAY_SENSITIVEDETECTOR_HH
#define MUONDECAY_SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"

class MuonDecay_SensitiveDetector : public G4VSensitiveDetector {
public:
    MuonDecay_SensitiveDetector(const G4String& name);
    virtual ~MuonDecay_SensitiveDetector();

    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
};

#endif
