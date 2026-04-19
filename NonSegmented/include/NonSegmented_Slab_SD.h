#ifndef NONSEGMENTED_SLAB_SD_HH
#define NONSEGMENTED_SLAB_SD_HH

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "NonSegmented_HitCollections.h"

class NonSegmented_Slab_SD : public G4VSensitiveDetector {
    Slab_HitCollection *fSlabHitCollection;   
    double fEDep; 
public:
    NonSegmented_Slab_SD(const G4String& name, const G4String &collName);
    virtual ~NonSegmented_Slab_SD();
    virtual void Initialize(G4HCofThisEvent *hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent *hce);
};

#endif
