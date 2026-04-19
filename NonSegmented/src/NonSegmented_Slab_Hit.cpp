#include "NonSegmented_Slab_Hit.h"

G4ThreadLocal G4Allocator<NonSegmented_Slab_Hit>* SlabHitAllocator = nullptr;

void* NonSegmented_Slab_Hit::operator new(size_t)
{
  if (!SlabHitAllocator)
    SlabHitAllocator = new G4Allocator<NonSegmented_Slab_Hit>;
  return (void*) SlabHitAllocator->MallocSingle();
}

void NonSegmented_Slab_Hit::operator delete(void* hit)
{
  SlabHitAllocator->FreeSingle((NonSegmented_Slab_Hit*) hit);
}

NonSegmented_Slab_Hit::NonSegmented_Slab_Hit() : x(0.), z(0.) {}

NonSegmented_Slab_Hit::~NonSegmented_Slab_Hit() {}

void NonSegmented_Slab_Hit::Set(double X, double Z)
{
  x = X;
  z = Z;
  eDep = 0.;
}

double NonSegmented_Slab_Hit::GetX() const
{
  return x;
}

double NonSegmented_Slab_Hit::GetZ() const
{
  return z;
}

void NonSegmented_Slab_Hit::Set(double edep){
  eDep = edep;
}

double NonSegmented_Slab_Hit::GetEDep()const{
  return eDep;
}

void NonSegmented_Slab_Hit::Print()
{
  std::cout << "(" << x << " , " << z << ")" << std::endl;
}
