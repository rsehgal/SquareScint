#ifndef SLABCALIB_SD_HH
#define SLABCALIB_SD_HH

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"


class SlabCalib_SD : public G4VSensitiveDetector {
bool fSlabHit;
bool fBar1Hit;
bool fBar2Hit;
G4ThreeVector pos;
public:
    SlabCalib_SD(const G4String& name, const G4String &collName="Test");
    virtual ~SlabCalib_SD();
    virtual void Initialize(G4HCofThisEvent *hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent *hce);
};

#endif
