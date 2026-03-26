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
class TH2F;

struct Data {
  double xtrue;
  double ytrue;
  double q45;
  double q67;

  Data(double xtr, double ytr, double Q45, double Q67) : xtrue(xtr), ytrue(ytr), q45(Q45), q67(Q67) {}

  void Print()
  {
    std::cout << "Pixel Location : (" << xtrue << "," << ytrue << ")" << std::endl;
    std::cout << "Q_Xraw : " << q67 << " :: Q_Yraw : " << q45 << std::endl;
  }
};

using D_VecOfVecOfHits = std::vector<std::vector<Hit *>>;
using HitSet           = std::set<Hit *, HitPtrCompare>;
using FittedGraph      = std::pair<std::unique_ptr<TGraph>, std::unique_ptr<TF1>>;

extern std::string searchDir;
extern bool EqualSets(const HitSet &s1, const HitSet &s2);
extern bool IsSubset(const HitSet &superSet, const HitSet &subSet);
extern HitSet VecOfHitsToHitSet(std::vector<Hit *> vec);
extern HitSet slab;
extern HitSet topBar;
extern HitSet bottomBar;
extern HitSet slabWithCylinder;
extern HitSet slabWithCylinderAndStilbene;
extern HitSet slabWithStilbene;
extern HitSet slabWithTopBar;
extern HitSet slabWithBottomBar;
// extern HitSet slabWithBottomBar;
extern HitSet slabWithTopAndBottomBar;
extern HitSet OnlyTopAndBottomBar;

// Coincidence of Slab,Cylinder, Bottombar
extern HitSet slabWithCylinderAndBottomBar;

extern std::vector<std::string> GetListOfFiles(std::vector<std::string> searchTokens, std::string targetDir = "./");
extern TF1 *FitRestricted(TH1F *h);
extern std::vector<FittedGraph> GetVectorOfParameterization(std::string searchDir, std::vector<short> locationsVec,
                                                            std::vector<short> locVec, bool ax = true);
// ROOT colors
extern std::vector<int> color;

extern FittedGraph CreateFittedGraph(std::vector<double> x, std::vector<double> y, std::string dirString = "Test");

extern std::vector<double> GetMean_GM(std::string filename, std::vector<double> gm_corr_factor = {1., 1.});

extern std::vector<TH2F *> GetCorrectionFactorHist(std::string searchDir, std::vector<short> locationsVec,
                                                   std::vector<TH2F *> corrFactorHistVect);

extern std::vector<TH2F *> GetUnitCorrFactorHistVector(std::vector<short> locationsVec);
extern TH2F *GetEntriesHist(std::vector<short> locationsVec);

extern std::unique_ptr<Data> GetMeanValues(short xpos, short ypos);
extern void PositionCalibration(const std::vector<std::unique_ptr<Data>>& vecOfData);

#endif
