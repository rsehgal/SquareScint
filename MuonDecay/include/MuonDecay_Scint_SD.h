#ifndef MUONDECAY_SCINT_SD_HH
#define MUONDECAY_SCINT_SD_HH

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "MuonDecay_HitCollections.h"

class MuonDecay_Scint_SD : public G4VSensitiveDetector {
public:
    Scint_HitCollection *fScintHitCollection;
    static int survivedMuPlus;
    static int survivedMuMinus;
public:
    MuonDecay_Scint_SD(const G4String& name, const G4String &collName);
    virtual ~MuonDecay_Scint_SD();
    virtual void Initialize(G4HCofThisEvent *hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent *hce);
};

#endif
