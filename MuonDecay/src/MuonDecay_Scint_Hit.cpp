#include "MuonDecay_Scint_Hit.h"

MuonDecay_Scint_Hit::MuonDecay_Scint_Hit() {}

MuonDecay_Scint_Hit::MuonDecay_Scint_Hit(G4String particle, int chNum, double ts)
    : fParticleName(particle), fChannelNum(chNum), fTimestamp(ts), fTrackId(-1)
{
}

MuonDecay_Scint_Hit::MuonDecay_Scint_Hit(G4String particle, int chNum, double ts, G4int trId, G4int parId,
                                         G4String crProcess)
    : fParticleName(particle), fChannelNum(chNum), fTimestamp(ts), fTrackId(trId), fParentId(parId),
      fCreatorProcess(crProcess)
{
}

MuonDecay_Scint_Hit::MuonDecay_Scint_Hit(const MuonDecay_Scint_Hit &obj)
{
  fParticleName   = obj.fParticleName;
  fChannelNum     = obj.fChannelNum;
  fTimestamp      = obj.fTimestamp;
  fTrackId        = obj.fTrackId;
  fParentId       = obj.fParentId;
  fCreatorProcess = obj.fCreatorProcess;
}

bool MuonDecay_Scint_Hit::operator<(const MuonDecay_Scint_Hit &obj) const
{
  if (fChannelNum != obj.fChannelNum) return fChannelNum < obj.fChannelNum;

  // Finally comparing TrackIDs to break the tie
  return fTrackId < obj.fTrackId;
}

MuonDecay_Scint_Hit::~MuonDecay_Scint_Hit() {}

void MuonDecay_Scint_Hit::Print() const
{
  // std::cout << "--------------------------------------" << std::endl;
  std::cout << "RAMAN : (" << fParticleName << " , " << fCreatorProcess << " , " << fChannelNum << " , " << fTimestamp << " , " << fTrackId << " , "
            << fParentId << std::endl;
}
