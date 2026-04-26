#include "MuonDecay_PMT_Hit.h"

MuonDecay_PMT_Hit::MuonDecay_PMT_Hit() {}

MuonDecay_PMT_Hit::MuonDecay_PMT_Hit(int chNo, double ts)
    : fChannelNum(chNo), fTimestamp(ts) {}

MuonDecay_PMT_Hit::~MuonDecay_PMT_Hit() {}

int MuonDecay_PMT_Hit::GetChannelNum() const { return fChannelNum; }

double MuonDecay_PMT_Hit::GetTimestamp() const {
  return fTimestamp;
}

void MuonDecay_PMT_Hit::Print() {

  std::cout << "--------------------------------------" << std::endl;
  std::cout << "(" << fChannelNum << " , " << fTimestamp << ")" << std::endl;
}
