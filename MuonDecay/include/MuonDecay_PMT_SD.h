#ifndef MUONDECAY_PMT_SD_HH
#define MUONDECAY_PMT_SD_HH


#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "MuonDecay_HitCollections.h"

class MuonDecay_PMT_SD : public G4VSensitiveDetector {

    PMT_HitCollection *fPMTHitCollection;   
public:
    MuonDecay_PMT_SD(const G4String& name, const G4String &collName);
    virtual ~MuonDecay_PMT_SD();
    virtual void Initialize(G4HCofThisEvent *hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent *hce);
};

#endif
