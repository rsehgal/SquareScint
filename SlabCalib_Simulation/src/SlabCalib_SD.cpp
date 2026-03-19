#include "SlabCalib_SD.h"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

SlabCalib_SD::SlabCalib_SD(const G4String &name, const G4String &collName)
    : G4VSensitiveDetector(name), fSlabHit(false), fBar1Hit(false), fBar2Hit(false)
{
  collectionName.insert(collName);
}

SlabCalib_SD::~SlabCalib_SD() {}

void SlabCalib_SD::Initialize(G4HCofThisEvent *hce)
{
  fSlabHit = false;
  fBar1Hit = false;
  fBar2Hit = false;

  
  //std::cout << "============ Initialial Hit State ============" << std::endl;
  //std::cout << "Slab : " << fSlabHit << " : Bar1 : " << fBar1Hit << " : Bar2 : " << fBar2Hit << std::endl;
}

G4bool SlabCalib_SD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  G4Track *track  = step->GetTrack();
  G4double energy = track->GetKineticEnergy();
  // G4cout << "Detected energy: " << energy / MeV << " MeV" << G4endl;
  G4String volumeName   = track->GetTouchable()->GetVolume()->GetName();
  G4String particleName = track->GetParticleDefinition()->GetParticleName();
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  if (particleName == "mu-") {
    if (step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
 pos=step->GetPreStepPoint()->GetPosition();
      //G4cout << "VolumeName : " << volumeName << " : " << pos << G4endl;
      if (volumeName == "PhysicalSlab") fSlabHit = true;
      if (volumeName == "PhysicalBar_1") fBar1Hit = true;
      if (volumeName == "PhysicalBar_2") fBar2Hit = true;
    }
  }
  return true;
}

void SlabCalib_SD::EndOfEvent(G4HCofThisEvent *hce)
{
  G4AnalysisManager *analMan = G4AnalysisManager::Instance();
  if (fSlabHit && fBar1Hit && fBar2Hit) {
    //std::cout << "Slab and Both the bar hitted......" << std::endl;
        analMan->FillNtupleDColumn(0,0,pos.x());
	analMan->FillNtupleDColumn(0,1,pos.y());
	analMan->FillNtupleDColumn(0,2,pos.z());
	analMan->AddNtupleRow(0);
  }
}
