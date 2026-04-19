#ifndef NONSEGMENTED_PMT_HIT_HH
#define NONSEGMENTED_PMT_HIT_HH

#include "G4VHit.hh"
#include "vector"
#include "G4Allocator.hh"
class NonSegmented_PMT_Hit : public G4VHit {
    unsigned int pmtID;
    double photonArrivalTime;
public:
    NonSegmented_PMT_Hit();
    virtual ~NonSegmented_PMT_Hit();
    unsigned int GetPmtID() const;
    double GetPhotonArrivalTime() const;
    void Set(unsigned int pmtid,double pat);
    void Print();

    void* operator new(size_t);
    void  operator delete(void*);
};


extern G4ThreadLocal G4Allocator<NonSegmented_PMT_Hit>* PMTHitAllocator;

#endif
