/*
**	Filename : Hit.h
**	2025-02-26
**	username : rsehgal
*/
#ifndef Hit_h
#define Hit_h

#include "TROOT.h"
#include <iostream>
#include <TArrayS.h>
/*bool SortHits(const std::shared_ptr<Hit>& a, const std::shared_ptr<Hit>& b) {
    return a->Timestamp < b->Timestamp; // Ascending order
}*/



class Hit {
public:
  UShort_t Channel;
  ULong64_t Timestamp;
  UShort_t Board;
  UShort_t Energy;
  UShort_t EnergyShort;
  UInt_t Flags;
  TArrayS *Samples;

public:
  Hit() {}
  Hit(UShort_t channel):Channel(channel),Timestamp(0),Board(0),Energy(0),EnergyShort(0),Flags(0){
Samples = nullptr;
}

  Hit(UShort_t channel, ULong64_t timestamp, UShort_t board, UShort_t energy, UShort_t energyShort, UInt_t flags, TArrayS *samples=nullptr)
      : Channel(channel), Timestamp(timestamp), Board(board), Energy(energy), EnergyShort(energyShort), Flags(flags)

  {
if(samples){
Samples = new TArrayS;//(samples->GetSize());
samples->Copy(*Samples);
}

/*for(unsigned int i = 0 ; i < samples->GetSize() ; i++){
//Samples->SetAt(i,samples->GetAt(i));
}*/
//Samples = samples;
  }

  void Set(UShort_t channel, ULong64_t timestamp, UShort_t board, UShort_t energy, UShort_t energyShort, UInt_t flags,TArrayS *samples=nullptr)
  {
    Channel     = channel;
    Timestamp   = timestamp;
    Board       = board;
    Energy      = energy;
    EnergyShort = energyShort;
    Flags       = flags;
    Samples = samples;
  }

  void Print()
  {
    // std::cout << "=====================================================" << std::endl;
    std::cout << Channel << "\t" << Timestamp << "\t" << Board << "\t" << Energy << "\t" << EnergyShort << "\t" << Flags
              << std::endl;
  
    for(unsigned int i = 0 ; i < Samples->GetSize() ; i++){
	std::cout << Samples->GetAt(i) << " : ";
    } 
std::cout << std::endl;
  }

  bool operator<(const Hit *other) const { return Channel < other->Channel; }

}
;
struct HitPtrCompare {
    bool operator()(const Hit* a, const Hit* b) const {
        return a->Channel < b->Channel;  // or use full comparison with std::tie
    }
};

#endif
