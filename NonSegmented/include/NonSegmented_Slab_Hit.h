#ifndef NONSEGMENTED_SLAB_HIT_HH
#define NONSEGMENTED_SLAB_HIT_HH

#include "G4VHit.hh"
#include "G4Allocator.hh"

class NonSegmented_Slab_Hit : public G4VHit {
  double x, z;
  double eDep;
public:
  NonSegmented_Slab_Hit();
  virtual ~NonSegmented_Slab_Hit();
  void Print();
  void Set(double X, double Z);
  void Set(double edep);
  double GetEDep() const;
  double GetX() const;
  double GetZ() const;

  void* operator new(size_t);
  void  operator delete(void*);
};

extern G4ThreadLocal G4Allocator<NonSegmented_Slab_Hit>* SlabHitAllocator;
#endif
