#include "SlabCalib_PrimaryGeneratorAction.h"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#ifdef USE_CRY
#include "CryInterface.h"
#endif

SlabCalib_PrimaryGeneratorAction::SlabCalib_PrimaryGeneratorAction() {
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    //Default we have setup geantino particles
    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("geantino");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleEnergy(1.0 * MeV);
    
#ifdef USE_CRY
    cryInterface = new CryInterface();
#endif    
    // TODO : Add your desired particles
}

SlabCalib_PrimaryGeneratorAction::~SlabCalib_PrimaryGeneratorAction() {
    delete fParticleGun;
}


void SlabCalib_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    
    //TODO :  Logic to change the particle position for each event
    double randX1 = CLHEP::RandFlat::shoot(-1000.,1000.);
    double randZ1 = CLHEP::RandFlat::shoot(-1000.,1000.);
    double randX2 = CLHEP::RandFlat::shoot(-1000.,1000.);
    double randZ2 = CLHEP::RandFlat::shoot(-1000.,1000.);
    
    double ypos=50*cm;

    G4ThreeVector start(randX1,ypos,randZ1);
    G4ThreeVector end(randX2,-1.*ypos,randZ2);

    G4ThreeVector dir(0.,-1.,0.);

#ifdef USE_CRY
    Muon *muon                = cryInterface->SampleMuon();
    fParticleGun->SetParticleEnergy(muon->energy);
    dir.set(muon->angleX,muon->angleY,muon->angleZ);
    dir = dir.unit();
#else
    fParticleGun->SetParticleEnergy(2*GeV);
    dir=(end-start).unit();
#endif


    fParticleGun->SetParticlePosition(start);
    fParticleGun->SetParticleMomentumDirection(dir);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
