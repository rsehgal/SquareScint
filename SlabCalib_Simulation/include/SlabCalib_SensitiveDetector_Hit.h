#ifndef SLABCALIB_SENSITIVEDETECTOR_HIT_HH
#define SLABCALIB_SENSITIVEDETECTOR_HIT_HH

#include "G4VHit.hh"

class SlabCalib_SensitiveDetector_Hit : public G4VHit {
public:
    SlabCalib_SensitiveDetector_Hit();
    virtual ~SlabCalib_SensitiveDetector_Hit();
};

#endif
