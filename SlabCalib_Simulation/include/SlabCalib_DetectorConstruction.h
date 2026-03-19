#ifndef SLABCALIB_DETECTORCONSTRUCTION_HH
#define SLABCALIB_DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

class SlabCalib_DetectorConstruction : public G4VUserDetectorConstruction {
public:
    SlabCalib_DetectorConstruction();
    virtual ~SlabCalib_DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
};

#endif
