#include "MuonDecay_Scint_Hit.h"

MuonDecay_Scint_Hit::MuonDecay_Scint_Hit() {}

MuonDecay_Scint_Hit::MuonDecay_Scint_Hit(G4String particle, int chNum, double ts) : fParticleName(particle), fChannelNum(chNum), fTimestamp(ts) {}

MuonDecay_Scint_Hit::~MuonDecay_Scint_Hit() {}

void MuonDecay_Scint_Hit::Print() const
{
  std::cout << "--------------------------------------" << std::endl;
  std::cout << "(" << fParticleName << " , " << fChannelNum <<" , " << fTimestamp << std::endl;
}
