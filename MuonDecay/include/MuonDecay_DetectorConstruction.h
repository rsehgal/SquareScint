#ifndef MUONDECAY_DETECTORCONSTRUCTION_HH
#define MUONDECAY_DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

class MuonDecay_DetectorConstruction : public G4VUserDetectorConstruction {
public:
    MuonDecay_DetectorConstruction();
    virtual ~MuonDecay_DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
};

#endif
