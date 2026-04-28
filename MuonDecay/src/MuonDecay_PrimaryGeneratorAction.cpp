#include "MuonDecay_PrimaryGeneratorAction.h"

#ifdef USE_CRY
#include "CryInterface.h"
#endif

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Event.hh"

int MuonDecay_PrimaryGeneratorAction::muplus=0;
int MuonDecay_PrimaryGeneratorAction::muminus=0;

MuonDecay_PrimaryGeneratorAction::MuonDecay_PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun     = new G4ParticleGun(n_particle);

  // Default we have setup geantino particles
  G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., -1., 0.));
  fParticleGun->SetParticleEnergy(100.0 * MeV);

#ifdef USE_CRY
  cryInterface = new CryInterface();
#endif

  // TODO : Add your desired particles
}

MuonDecay_PrimaryGeneratorAction::~MuonDecay_PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void MuonDecay_PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{

  // TODO :  Logic to change the particle position for each event
  double randX1 = CLHEP::RandFlat::shoot(-25 * cm, 25 * cm);
  double randZ1 = CLHEP::RandFlat::shoot(-25. * cm, 25. * cm);
  double randX2 = CLHEP::RandFlat::shoot(-25. * cm, 25. * cm);
  double randZ2 = CLHEP::RandFlat::shoot(-25. * cm, 25. * cm);

  double ypos = 25 * cm;

  G4ThreeVector start(randX1, ypos, randZ1);
  G4ThreeVector end(randX2, -1. * ypos, randZ2);

  G4ThreeVector dir(0., -1., 0.);

#ifdef USE_CRY
  Muon *muon                     = cryInterface->SampleMuon();
  //std::cout <<"PDGid : " << muon->pdgid << std::endl;
  if(muon->pdgid == -13) muplus++;
  if(muon->pdgid == 13) muminus++;
  G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle(muon->pdgid);
  fParticleGun->SetParticleDefinition(particle);

  // Setting gun's energy to the energy of muon sampled from CRY
  fParticleGun->SetParticleEnergy(muon->energy);

  /*
  The Hack: Overriding the above high energy to "stoppable" (< 50 MeV)
  This preserves the CRY direction and charge, but forces the decay physics
  and help in getting simulation result quicker
 */
  fParticleGun->SetParticleEnergy(CLHEP::RandFlat::shoot(5.0, 50.0) * MeV);
  // fParticleGun->SetParticleEnergy(20.*MeV);

  dir.set(muon->angleX, muon->angleY, muon->angleZ);
  dir = dir.unit();

#else
  fParticleGun->SetParticleEnergy(20 * MeV);
  dir = (end - start).unit();
#endif

  fParticleGun->SetParticlePosition(start);
  fParticleGun->SetParticleMomentumDirection(dir);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
