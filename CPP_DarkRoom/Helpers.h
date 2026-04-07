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
#include <algorithm>
#include <numeric>
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

extern HitSet pmt4;
extern HitSet pmt5;
extern HitSet pmt6;
extern HitSet pmt7;

extern HitSet slab;
extern HitSet slab1;
extern HitSet slab2;
extern HitSet topBar;
extern HitSet bottomBar;
extern HitSet bothSlabs;
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
extern void PositionCalibration(const std::vector<std::unique_ptr<Data>> &vecOfData);

template<typename T>
inline constexpr auto get_3pt_avg = [](const std::vector<T> &v, int i) {
  int n   = v.size();
  //int p   = (i == 0) ? n - 1 : i - 1;
  int p   = (i == 0) ? i : i - 1;
  //int nxt = (i == n - 1) ? 0 : i + 1;
  int nxt = (i == n - 1) ? i : i + 1;
  return (v[p] + v[i] + v[nxt]) / 3.0;
};

inline constexpr auto get_next_bin_mean = [](auto &it, const auto &end, double window_size) {
  if (it == end) return 0.0;

  // 1. Find the end of the time window using Binary Search (Fast!)
  double limit = (*it) + window_size;
  auto bin_end = std::upper_bound(it, end, limit);

  // 2. Calculate the mean for this specific range
  double sum  = std::accumulate(it, bin_end, 0.0);
  long count  = std::distance(it, bin_end);
  double mean = (count > 0) ? sum / count : 0.0;

  // 3. Update the iterator to the start of the NEXT bin
  it = bin_end;

  return mean;
};

#endif
