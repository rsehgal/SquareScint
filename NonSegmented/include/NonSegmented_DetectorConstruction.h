#ifndef NONSEGMENTED_DETECTORCONSTRUCTION_HH
#define NONSEGMENTED_DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

class G4Material;
class G4OpticalSurface;
class G4LogicalVolume;

class NonSegmented_DetectorConstruction : public G4VUserDetectorConstruction {
public:
    NonSegmented_DetectorConstruction();
    virtual ~NonSegmented_DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    void AttachOpticalProperties(G4Material *scintMat);
    G4OpticalSurface* GetOpticalSurface();
    G4LogicalVolume* GetPMT();
    G4OpticalSurface* GetInterfacingSurface();
};

#endif
