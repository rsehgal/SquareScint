#ifndef SLABCALIB_SENSITIVEDETECTOR_HH
#define SLABCALIB_SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"

class SlabCalib_SensitiveDetector : public G4VSensitiveDetector {
public:
    SlabCalib_SensitiveDetector(const G4String& name);
    virtual ~SlabCalib_SensitiveDetector();

    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
};

#endif
