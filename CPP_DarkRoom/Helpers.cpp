/*
**	Filename : Helpers.cpp
**	2025-08-04
**	username : rsehgal
*/
#include <iostream>
#include "Helpers.h"
#include <algorithm>
HitPtrCompare comp;

HitSet topBar    =  {new Hit(0), new Hit(1)};
HitSet bottomBar    =  {new Hit(2), new Hit(3)};
HitSet slab              = {new Hit(4), new Hit(5), new Hit(6), new Hit(7)};
HitSet slabWithTopBar    = {new Hit(0), new Hit(1), new Hit(4), new Hit(5), new Hit(6), new Hit(7)};
HitSet slabWithBottomBar = {new Hit(2), new Hit(3), new Hit(4), new Hit(5), new Hit(6), new Hit(7)};

HitSet slabWithTopAndBottomBar = {new Hit(0), new Hit(1), new Hit(2), new Hit(3),
                                  new Hit(4), new Hit(5), new Hit(6), new Hit(7)};

HitSet OnlyTopAndBottomBar = {new Hit(1), new Hit(1), new Hit(2), new Hit(3)};

HitSet slabWithCylinderAndBottomBar = {new Hit(4), new Hit(5), new Hit(6), new Hit(7),
                                       new Hit(2), new Hit(3), new Hit(1)};
HitSet slabWithCylinder = {new Hit(4), new Hit(5), new Hit(6), new Hit(7),
                                       new Hit(0)};


bool IsSubset(const HitSet &superSet, const HitSet &subSet)
{
  bool b_is_subset_of_a = std::includes(superSet.begin(), superSet.end(), 
                                        subSet.begin(), subSet.end(),
				        comp	
  );

  return b_is_subset_of_a;
}

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
