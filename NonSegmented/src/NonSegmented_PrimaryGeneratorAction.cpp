#include "NonSegmented_PrimaryGeneratorAction.h"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4SystemOfUnits.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4IonTable.hh"
#include "G4HadronicParameters.hh"
#include "G4RandomDirection.hh"

/*NonSegmented_PrimaryGeneratorAction::NonSegmented_PrimaryGeneratorAction() {
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    //Default we have setup geantino particles
    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("geantino");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleEnergy(1.0 * MeV);
    
    
    // TODO : Add your desired particles
}*/
NonSegmented_PrimaryGeneratorAction::NonSegmented_PrimaryGeneratorAction(int locX, int locZ):fLocX(locX), fLocZ(locZ) {
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    //Default we have setup geantino particles
    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("geantino");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleEnergy(1.0 * MeV);
    
    
    // TODO : Add your desired particles
}

NonSegmented_PrimaryGeneratorAction::~NonSegmented_PrimaryGeneratorAction() {
    delete fParticleGun;
}


void NonSegmented_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    
    //TODO :  Logic to change the particle position for each event

  double ypos = 45;
//#define Na22
#ifdef Na22
//G4HadronicParameters::Instance()->SetTimeThresholdForRadioactiveDecay(1.0e+60 * CLHEP::year);
ypos=31.5;
G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(11, 22, 0);
fParticleGun->SetParticleDefinition(ion);
fParticleGun->SetParticleEnergy(0*eV);
fParticleGun->SetParticlePosition(G4ThreeVector(fLocX*cm,ypos,fLocZ*cm));
#else
#define RANDOM_POS
#ifdef RANDOM_POS
  //double randX1 = CLHEP::RandFlat::shoot(-250.,250.);
  //double randZ1 = CLHEP::RandFlat::shoot(-250.,250.);

  double randX1 = CLHEP::RandFlat::shoot((fLocX-3)*cm,(fLocX+3)*cm);
  //double randX1 = CLHEP::RandFlat::shoot(-30.,30.);
  double randZ1 = CLHEP::RandFlat::shoot((fLocZ-3)*cm,(fLocZ+3)*cm);
  //double randZ1 = CLHEP::RandFlat::shoot(70.,130.);

  //fParticleGun->SetParticlePosition(G4ThreeVector(randX1,ypos,randZ1));
  fParticleGun->SetParticlePosition(G4ThreeVector(fLocX,ypos,fLocZ));
  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
#endif
#endif
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
