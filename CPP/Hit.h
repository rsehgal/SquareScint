/*
**	Filename : Hit.h
**	2025-02-26
**	username : rsehgal
*/
#ifndef Hit_h
#define Hit_h

#include "TROOT.h"
#include <iostream>
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

public:
  Hit() {}

  Hit(UShort_t channel, ULong64_t timestamp, UShort_t board, UShort_t energy, UShort_t energyShort, UInt_t flags)
      : Channel(channel), Timestamp(timestamp), Board(board), Energy(energy), EnergyShort(energyShort), Flags(flags)

  {
 }

  void Set(UShort_t channel, ULong64_t timestamp, UShort_t board, UShort_t energy, UShort_t energyShort, UInt_t flags){
       Channel = channel;
       Timestamp = timestamp;
       Board = board;
       Energy = energy;
       EnergyShort = energyShort;
       Flags=flags;
}

  void Print()
  {
    //std::cout << "=====================================================" << std::endl;
    std::cout << Channel << "\t" << Timestamp << "\t" << Board << "\t" << Energy << "\t" << EnergyShort << "\t" << Flags
              << std::endl;
  }
};

#endif
