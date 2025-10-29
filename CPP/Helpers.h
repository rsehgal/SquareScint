/*
**	Filename : Helpers.h
**	2025-08-04
**	username : rsehgal
*/
#ifndef Helpers_h
#define Helpers_h

#include "Hit.h"
#include <vector>
#include <set>

using D_VecOfVecOfHits = std::vector<std::vector<Hit *>>;
using HitSet = std::set<Hit*,HitPtrCompare>;
extern bool EqualSets(const HitSet& s1,const HitSet& s2);
extern bool IsSubset(const HitSet &superSet, const HitSet &subSet);
extern HitSet VecOfHitsToHitSet(std::vector<Hit*> vec);
extern HitSet slab;
extern HitSet slabWithTopBar;
extern HitSet slabWithBottomBar;
//extern HitSet slabWithBottomBar;
extern HitSet slabWithTopAndBottomBar;
extern HitSet OnlyTopAndBottomBar;

//Coincidence of Slab,Cylinder, Bottombar
extern HitSet slabWithCylinderAndBottomBar;

#endif
