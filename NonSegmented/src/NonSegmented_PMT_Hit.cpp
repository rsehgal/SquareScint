#include "NonSegmented_PMT_Hit.h"

G4ThreadLocal G4Allocator<NonSegmented_PMT_Hit>* PMTHitAllocator = nullptr;

void* NonSegmented_PMT_Hit::operator new(size_t)
{
  if (!PMTHitAllocator)
    PMTHitAllocator = new G4Allocator<NonSegmented_PMT_Hit>;
  return (void*) PMTHitAllocator->MallocSingle();
}

void NonSegmented_PMT_Hit::operator delete(void* hit)
{
  PMTHitAllocator->FreeSingle((NonSegmented_PMT_Hit*) hit);
}

NonSegmented_PMT_Hit::NonSegmented_PMT_Hit() {}

NonSegmented_PMT_Hit::~NonSegmented_PMT_Hit() {}

unsigned int NonSegmented_PMT_Hit::GetPmtID() const
{
  return pmtID;
}

double NonSegmented_PMT_Hit::GetPhotonArrivalTime() const
{
  return photonArrivalTime;
}

void NonSegmented_PMT_Hit::Set(unsigned int pmtid, double pat)
{
  pmtID                  = pmtid;
  photonArrivalTime = pat;
}

void NonSegmented_PMT_Hit::Print(){
std::cout << "(" << pmtID <<" , "<< photonArrivalTime << ")" << std::endl;
}
