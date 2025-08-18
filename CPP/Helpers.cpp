/*
**	Filename : Helpers.cpp
**	2025-08-04
**	username : rsehgal
*/
#include <iostream>
#include "Helpers.h"

HitSet slab                    = {new Hit(0), new Hit(1), new Hit(2), new Hit(3)};
HitSet slabWithTopBar          = {new Hit(0), new Hit(1), new Hit(2), new Hit(3), new Hit(4), new Hit(5)};
HitSet slabWithBottomBar          = {new Hit(0), new Hit(1), new Hit(2), new Hit(3), new Hit(6), new Hit(7)};

HitSet slabWithTopAndBottomBar = {new Hit(0), new Hit(1), new Hit(2), new Hit(3),
                                  new Hit(4), new Hit(5), new Hit(6), new Hit(7)};

bool EqualSets(const HitSet &s1, const HitSet &s2)
{
  if (s1.size() != s2.size()) return false;

  auto it1 = s1.begin();
  auto it2 = s2.begin();

  while (it1 != s1.end()) {
    if ((*it1)->Channel != (*it2)->Channel /* || compare other fields if needed */) return false;
    ++it1;
    ++it2;
  }

  return true;
}

HitSet VecOfHitsToHitSet(std::vector<Hit *> vec)
{
  HitSet hs;
  for (const auto &hit : vec) {
    hs.insert(hit);
  }
  return hs;
}
