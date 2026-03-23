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
#include <TColor.h>
#include <memory>
#include <utility>

class TH1F;
class TF1;
class TGraph;

using D_VecOfVecOfHits = std::vector<std::vector<Hit *>>;
using HitSet = std::set<Hit*,HitPtrCompare>;
using FittedGraph = std::pair<std::unique_ptr<TGraph>, std::unique_ptr<TF1>>;

extern bool EqualSets(const HitSet& s1,const HitSet& s2);
extern bool IsSubset(const HitSet &superSet, const HitSet &subSet);
extern HitSet VecOfHitsToHitSet(std::vector<Hit*> vec);
extern HitSet slab;
extern HitSet topBar;
extern HitSet bottomBar;
extern HitSet slabWithCylinder;
extern HitSet slabWithCylinderAndStilbene;
extern HitSet slabWithStilbene;
extern HitSet slabWithTopBar;
extern HitSet slabWithBottomBar;
//extern HitSet slabWithBottomBar;
extern HitSet slabWithTopAndBottomBar;
extern HitSet OnlyTopAndBottomBar;

//Coincidence of Slab,Cylinder, Bottombar
extern HitSet slabWithCylinderAndBottomBar;



extern std::vector<std::string> GetListOfFiles(std::vector<std::string> searchTokens,
                                        std::string targetDir = "./");
extern TF1* FitRestricted(TH1F* h);
extern std::vector<FittedGraph> GetVectorOfParameterization(std::string searchDir, std::vector<short> locationsVec, std::vector<short> locVec, bool ax=true);
//ROOT colors
extern std::vector<int> color;

extern FittedGraph CreateFittedGraph(std::vector<double> x, std::vector<double> y, std::string dirString="Test") ;
#endif
