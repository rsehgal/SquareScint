/*
**	Filename : Helpers.cpp
**	2025-08-04
**	username : rsehgal
*/
#include "Helpers.h"
#include <TF1.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <utility>
#include <TLinearFitter.h>
#include <TVectorD.h>
#include <algorithm>
#include <vector>
#include <numeric>
namespace fs = std::filesystem;

HitPtrCompare comp;

std::vector<int> color = {kBlack, kRed, kBlue, kMagenta, kGreen, kCyan};

std::string searchDir = "/home/rsehgal/shared/SquareScint_DarkRoom/ExpData_DarkRoom/Slab_Cylinder/";

HitSet topBar            = {new Hit(0), new Hit(1)};
HitSet bottomBar         = {new Hit(2), new Hit(3)};

HitSet pmt4              = {new Hit(4)};
HitSet pmt5              = {new Hit(5)};
HitSet pmt6              = {new Hit(6)};
HitSet pmt7              = {new Hit(7)};


HitSet slab              = {new Hit(4), new Hit(5), new Hit(6), new Hit(7)};
HitSet slab1              = {new Hit(4), new Hit(5), new Hit(6), new Hit(7)};
HitSet slab2              = {new Hit(0), new Hit(1), new Hit(2), new Hit(3)};
HitSet slabWithTopBar    = {new Hit(0), new Hit(1), new Hit(4), new Hit(5), new Hit(6), new Hit(7)};
HitSet slabWithBottomBar = {new Hit(2), new Hit(3), new Hit(4), new Hit(5), new Hit(6), new Hit(7)};

HitSet slabWithTopAndBottomBar = {new Hit(0), new Hit(1), new Hit(2), new Hit(3),
                                  new Hit(4), new Hit(5), new Hit(6), new Hit(7)};
HitSet bothSlabs = {new Hit(0), new Hit(1), new Hit(2), new Hit(3),
                                  new Hit(4), new Hit(5), new Hit(6), new Hit(7)};

HitSet OnlyTopAndBottomBar = {new Hit(1), new Hit(1), new Hit(2), new Hit(3)};

HitSet slabWithCylinderAndBottomBar = {new Hit(4), new Hit(5), new Hit(6), new Hit(7),
                                       new Hit(2), new Hit(3), new Hit(1)};
HitSet slabWithCylinder             = {new Hit(4), new Hit(5), new Hit(6), new Hit(7), new Hit(0)};
HitSet slabWithStilbene             = {new Hit(4), new Hit(5), new Hit(6), new Hit(7), new Hit(0), new Hit(1)};
HitSet slabWithCylinderAndStilbene  = {new Hit(4), new Hit(5), new Hit(6), new Hit(7), new Hit(0), new Hit(1)};

bool IsSubset(const HitSet &superSet, const HitSet &subSet)
{
  bool b_is_subset_of_a = std::includes(superSet.begin(), superSet.end(), subSet.begin(), subSet.end(), comp);

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

std::vector<std::string> GetListOfFiles(std::vector<std::string> searchTokens, std::string targetDir)
{
  std::vector<std::string> matchingFiles;

  for (const auto &entry : fs::directory_iterator(targetDir)) {
    if (entry.is_regular_file()) {
      std::string filename = entry.path().filename().string();

      bool allTokensFound = true;

      // Check every token; if any are missing, it's not a match
      for (const auto &token : searchTokens) {
        if (filename.find(token) == std::string::npos) {
          allTokensFound = false;
          break; // No need to check other tokens for this file
        }
      }

      if (allTokensFound) {
        // std::cout << "Found match: " << filename << "\n";
        matchingFiles.push_back(filename);
      }
    }
  }
  return matchingFiles;
}

TF1 *FitRestricted(TH1F *h)
{
  // 1. Initial "Guess" Fit
  // This finds the approximate peak position and width
  h->Fit("gaus", "Q"); // "Q" for quiet mode

  TF1 *initialFit = h->GetFunction("gaus");
  double mean     = initialFit->GetParameter(1);
  double sigma    = initialFit->GetParameter(2);

  // 2. Define the Restricted Fit
  // Define a new Gaussian function with a specific range
  double xMin = mean - sigma;
  double xMax = mean + sigma;

  TF1 *finalFit = new TF1("finalFit", "gaus", xMin, xMax);
  finalFit->SetLineColor(kRed);
  finalFit->SetLineWidth(3);

  // 3. Perform the second fit
  // Use "R" to tell ROOT to use the range defined in the TF1
  // Use "+" to keep the previous fit on the canvas if desired
  h->Fit("finalFit", "Q");

  printf("Final Mean: %.2f | Final Sigma: %.2f\n", finalFit->GetParameter(1), finalFit->GetParameter(2));

  return finalFit;
}

std::vector<FittedGraph> GetVectorOfParameterization(std::string searchDir, std::vector<short> locationsVec,
                                                     std::vector<short> locVec, bool ax)
{

  std::vector<FittedGraph> parameterizationVec;
  for (unsigned int locIdX = 0; locIdX < locationsVec.size(); locIdX++) {
    std::vector<double> meanVec;
    std::string dirString;
    for (unsigned int locIdY = 0; locIdY < locVec.size(); locIdY++) {

      // std::string locString = "_" + std::to_string(locationsVec[locId]) + "_"
      // +
      //                       std::to_string(locVec[loc]);

      std::string locString;
      if (ax) {
        locString = "_" + std::to_string(locVec[locIdY]) + "_" + std::to_string(locationsVec[locIdX]);
        dirString = "y_" + std::to_string(locationsVec[locIdX]);
      } else {
        locString = "_" + std::to_string(locationsVec[locIdX]) + "_" + std::to_string(locVec[locIdY]);
        dirString = "x_" + std::to_string(locationsVec[locIdX]);
      }

      std::vector<std::string> searchTokens = {"output", locString};

      std::string histName = "hist_" + locString;
      TH1F *hist           = new TH1F(histName.c_str(), histName.c_str(), 100, -10000, 10000);

      std::vector<std::string> vecOfFileNames = GetListOfFiles(searchTokens, searchDir);

      if (vecOfFileNames.size() > 0) {

        std::cout << "Processing File : " << vecOfFileNames[0] << std::endl;
        TFile *f     = new TFile((searchDir + vecOfFileNames[0]).c_str());
        TTree *ftree = (TTree *)f->Get("ftree");

        // gStyle->SetOptStat(0000);
        //  Declaration of leaves types
        //     vector<int>     qVec;
        UShort_t q0;
        UShort_t q1;
        UShort_t q2;
        UShort_t q3;
        UShort_t q4;
        UShort_t q5;
        UShort_t q6;
        UShort_t q7;
        UShort_t q8;
        ULong64_t t0;
        ULong64_t t1;
        ULong64_t t2;
        ULong64_t t3;
        ULong64_t t4;
        ULong64_t t5;
        ULong64_t t6;
        ULong64_t t7;
        ULong64_t t8;

        // Set branch addresses.
        //   ftree->SetBranchAddress("qVec",&qVec);
        ftree->SetBranchAddress("q0", &q0);
        ftree->SetBranchAddress("q1", &q1);
        ftree->SetBranchAddress("q2", &q2);
        ftree->SetBranchAddress("q3", &q3);
        ftree->SetBranchAddress("q4", &q4);
        ftree->SetBranchAddress("q5", &q5);
        ftree->SetBranchAddress("q6", &q6);
        ftree->SetBranchAddress("q7", &q7);
        ftree->SetBranchAddress("q8", &q8);
        ftree->SetBranchAddress("t0", &t0);
        ftree->SetBranchAddress("t1", &t1);
        ftree->SetBranchAddress("t2", &t2);
        ftree->SetBranchAddress("t3", &t3);
        ftree->SetBranchAddress("t4", &t4);
        ftree->SetBranchAddress("t5", &t5);
        ftree->SetBranchAddress("t6", &t6);
        ftree->SetBranchAddress("t7", &t7);
        ftree->SetBranchAddress("t8", &t8);

        Long64_t nentries = ftree->GetEntries();

        Long64_t nbytes = 0;

        double q_gm = 0.;
        double q_ar = 0.;
        for (Long64_t i = 0; i < nentries; i++) {
          nbytes += ftree->GetEntry(i);
          if (q4 > 0 && q5 > 0 && q6 > 0 && q7 > 0) {
            double avg_log = (std::log(q4) + std::log(q5) + std::log(q6) + std::log(q7)) / 4.;
            q_gm += std::exp(avg_log);
            q_ar += (q4 + q5 + q6 + q7) / 4.;
            Long64_t delT = -100000;
            if (ax)
              delT = t6 - t7;
            else
              delT = t4 - t5;
            // Long64_t delT = t4 - t5;
            hist->Fill(delT);
          }
        }
      }
      hist->Scale(1. / hist->Integral());
      TF1 *fo = FitRestricted(hist);

      meanVec.push_back(fo->GetParameter(1));
    }

    std::vector<double> posVec(locVec.begin(), locVec.end());
    /*std::unique_ptr<TGraph> gr =
        std::make_unique<TGraph>(posVec.size(), &meanVec[0], &posVec[0]);
    TF1 *formu = new TF1("pol3", "pol3",-10000,10000);
    gr->Fit(formu);*/
    parameterizationVec.emplace_back(CreateFittedGraph(meanVec, posVec, dirString));
  }

  return parameterizationVec;
}

FittedGraph CreateFittedGraph(std::vector<double> x, std::vector<double> y, std::string dirString)
{
  std::string grName = "Gr_" + dirString;
  auto gr            = std::make_unique<TGraph>(x.size(), &x[0], &y[0]);
  gr->SetName(grName.c_str());
  gr->SetMarkerStyle(8);

  std::string funcName = "Param_" + dirString;
  auto func            = std::make_unique<TF1>(funcName.c_str(), "pol3", -10000, 10000);

  // Perform the fit
  gr->Fit(func.get(), "Q"); // "Q" for quiet mode

  return {std::move(gr), std::move(func)};
}

std::vector<double> GetMean_GM(std::string filename, std::vector<double> gm_corr_factor)
{
  std::cout << "Processing File : " << filename << std::endl;
  TFile *f     = new TFile(filename.c_str(), "r");
  TTree *ftree = (TTree *)f->Get("ftree");

  // gStyle->SetOptStat(0000);
  //  Declaration of leaves types
  //     vector<int>     qVec;
  UShort_t q0;
  UShort_t q1;
  UShort_t q2;
  UShort_t q3;
  UShort_t q4;
  UShort_t q5;
  UShort_t q6;
  UShort_t q7;
  UShort_t q8;
  ULong64_t t0;
  ULong64_t t1;
  ULong64_t t2;
  ULong64_t t3;
  ULong64_t t4;
  ULong64_t t5;
  ULong64_t t6;
  ULong64_t t7;
  ULong64_t t8;

  // Set branch addresses.
  //   ftree->SetBranchAddress("qVec",&qVec);
  ftree->SetBranchAddress("q0", &q0);
  ftree->SetBranchAddress("q1", &q1);
  ftree->SetBranchAddress("q2", &q2);
  ftree->SetBranchAddress("q3", &q3);
  ftree->SetBranchAddress("q4", &q4);
  ftree->SetBranchAddress("q5", &q5);
  ftree->SetBranchAddress("q6", &q6);
  ftree->SetBranchAddress("q7", &q7);
  ftree->SetBranchAddress("q8", &q8);
  ftree->SetBranchAddress("t0", &t0);
  ftree->SetBranchAddress("t1", &t1);
  ftree->SetBranchAddress("t2", &t2);
  ftree->SetBranchAddress("t3", &t3);
  ftree->SetBranchAddress("t4", &t4);
  ftree->SetBranchAddress("t5", &t5);
  ftree->SetBranchAddress("t6", &t6);
  ftree->SetBranchAddress("t7", &t7);
  ftree->SetBranchAddress("t8", &t8);

  Long64_t nentries = ftree->GetEntries();

  Long64_t nbytes = 0;

  TH2F *histCorr = new TH2F("HistCorr", "HistCorr", 500, 0, 1000, 200, 0, 200);

  double q_gm = 0.;
  double q_ar = 0.;
  for (Long64_t i = 0; i < nentries; i++) {
    nbytes += ftree->GetEntry(i);
    if (q4 > 0 && q5 > 0 && q6 > 0 && q7 > 0) {
      double avg_log = (std::log(q4) + std::log(q5) + std::log(q6) + std::log(q7)) / 4.;
      q_gm += std::exp(avg_log) * gm_corr_factor[0];
      q_ar += ((q4 + q5 + q6 + q7) / 4.) * gm_corr_factor[1];
    }
  }
  q_gm /= nentries;
  q_ar /= nentries;
  std::vector<double> toRet = {q_gm, q_ar};
  // std::cout << locIdX << " : " << locIdY << std::endl;
  std::cout << "Q_GM : " << q_gm << " :: Q_AR : " << q_ar << std::endl;

  return toRet;
}

std::vector<TH2F *> GetUnitCorrFactorHistVector(std::vector<short> locationsVec)
{
  int len                 = locationsVec.size();
  TH2F *uniformityHist_gm = new TH2F("UniformityHist_GM", "UniformityHist_GM", len, 0, len, len, 0, len);
  TH2F *uniformityHist_ar = new TH2F("UniformityHist_AM", "UniformityHist_AR", len, 0, len, len, 0, len);
  for (unsigned int locIdX = 0; locIdX < locationsVec.size(); locIdX++) {
    for (unsigned int locIdY = 0; locIdY < locationsVec.size(); locIdY++) {
      uniformityHist_gm->SetBinContent(locIdX + 1, locIdY + 1, 1.);
      uniformityHist_ar->SetBinContent(locIdX + 1, locIdY + 1, 1.);
    }
  }
  std::vector<TH2F *> unitCorrFactorHistVec = {uniformityHist_gm, uniformityHist_ar};
  return unitCorrFactorHistVec;
}
std::vector<TH2F *> GetCorrectionFactorHist(std::string searchDir, std::vector<short> locationsVec,
                                            std::vector<TH2F *> corrFactorHistVect)
{
  int len                 = locationsVec.size();
  TH2F *uniformityHist_gm = new TH2F("UniformityHist_GM", "UniformityHist_GM", len, 0, len, len, 0, len);
  TH2F *uniformityHist_ar = new TH2F("UniformityHist_AM", "UniformityHist_AR", len, 0, len, len, 0, len);
  for (unsigned int locIdX = 0; locIdX < locationsVec.size(); locIdX++) {
    for (unsigned int locIdY = 0; locIdY < locationsVec.size(); locIdY++) {

      std::string locString = "_" + std::to_string(locationsVec[locIdX]) + "_" + std::to_string(locationsVec[locIdY]);
      std::vector<std::string> searchTokens = {"output", locString};

      std::vector<std::string> vecOfFileNames = GetListOfFiles(searchTokens, searchDir);

      if (vecOfFileNames.size() > 0) {
        std::string filename        = searchDir + vecOfFileNames[0];
        double q_ar                 = 0;
        std::vector<double> corrVec = {1. / corrFactorHistVect[0]->GetBinContent(locIdX + 1, locIdY + 1),
                                       1. / corrFactorHistVect[1]->GetBinContent(locIdX + 1, locIdY + 1)};
        std::vector<double> means   = GetMean_GM(filename, corrVec);
        uniformityHist_gm->SetBinContent(locIdX + 1, locIdY + 1, means[0]);
        uniformityHist_ar->SetBinContent(locIdX + 1, locIdY + 1, means[1]);
      }
    }
  }

  if (uniformityHist_gm->GetMaximum() > 0) {
    uniformityHist_gm->Scale(1.0 / uniformityHist_gm->GetMaximum());
  }
  if (uniformityHist_ar->GetMaximum() > 0) {
    uniformityHist_ar->Scale(1.0 / uniformityHist_ar->GetMaximum());
  }

  std::vector<TH2F *> correctionHistVec = {uniformityHist_gm, uniformityHist_ar};
  return correctionHistVec;
}

TH2F *GetEntriesHist(std::vector<short> locationsVec)
{
  int len           = locationsVec.size();
  TH2F *entriesHist = new TH2F("EntriesHist", "EntriesHist", len, 0, len, len, 0, len);
  for (unsigned int locIdX = 0; locIdX < locationsVec.size(); locIdX++) {
    for (unsigned int locIdY = 0; locIdY < locationsVec.size(); locIdY++) {

      std::string locString = "_" + std::to_string(locationsVec[locIdX]) + "_" + std::to_string(locationsVec[locIdY]);
      std::vector<std::string> searchTokens = {"output", locString};

      std::vector<std::string> vecOfFileNames = GetListOfFiles(searchTokens, searchDir);

      if (vecOfFileNames.size() > 0) {
        std::string filename = searchDir + vecOfFileNames[0];
        TFile *f             = new TFile(filename.c_str(), "r");
        TTree *ftree         = (TTree *)f->Get("ftree");
        ULong64_t t4;
        ftree->SetBranchAddress("t4", &t4);
        ftree->GetEntry(0);
        ULong64_t startTime = t4;

        ftree->GetEntry(ftree->GetEntries() - 1);
        ULong64_t endTime = t4;

        ULong64_t totalTimeInSec = (endTime - startTime) / 1e+12;

        entriesHist->SetBinContent(locIdX + 1, locIdY + 1, (1. * ftree->GetEntries()) / (1. * totalTimeInSec));
        delete ftree;
        f->Close();
      }
    }
  }
  if (entriesHist->GetMaximum() > 0) {
    entriesHist->Scale(1.0 / entriesHist->GetMaximum());
  }

  return entriesHist;
}

std::unique_ptr<Data> GetMeanValues(short xpos, short ypos)
{
  std::string FILENAME = "output_"+std::to_string(xpos)+"_"+std::to_string(ypos)+".root";
  std::string filename = searchDir + FILENAME;
  TFile *f             = new TFile(filename.c_str(), "r");
  TTree *ftree         = (TTree *)f->Get("ftree");

  UShort_t q0;
  UShort_t q1;
  UShort_t q2;
  UShort_t q3;
  UShort_t q4;
  UShort_t q5;
  UShort_t q6;
  UShort_t q7;
  UShort_t q8;
  ULong64_t t0;
  ULong64_t t1;
  ULong64_t t2;
  ULong64_t t3;
  ULong64_t t4;
  ULong64_t t5;
  ULong64_t t6;
  ULong64_t t7;
  ULong64_t t8;

  // Set branch addresses.
  ftree->SetBranchAddress("q0", &q0);
  ftree->SetBranchAddress("q1", &q1);
  ftree->SetBranchAddress("q2", &q2);
  ftree->SetBranchAddress("q3", &q3);
  ftree->SetBranchAddress("q4", &q4);
  ftree->SetBranchAddress("q5", &q5);
  ftree->SetBranchAddress("q6", &q6);
  ftree->SetBranchAddress("q7", &q7);
  ftree->SetBranchAddress("q8", &q8);
  ftree->SetBranchAddress("t0", &t0);
  ftree->SetBranchAddress("t1", &t1);
  ftree->SetBranchAddress("t2", &t2);
  ftree->SetBranchAddress("t3", &t3);
  ftree->SetBranchAddress("t4", &t4);
  ftree->SetBranchAddress("t5", &t5);
  ftree->SetBranchAddress("t6", &t6);
  ftree->SetBranchAddress("t7", &t7);
  ftree->SetBranchAddress("t8", &t8);

  Long64_t nbytes    = 0;
  ULong64_t nentries = ftree->GetEntries();

  std::vector<double>q45_vec;
  std::vector<double>q67_vec;

  for (unsigned int i = 0; i < nentries; i++) {
    nbytes += ftree->GetEntry(i);

    if (q4 > 0. && q5 > 0. && q6 > 0. && q7 > 0.) {

	double ratio45=log((1.*q4)/(1.*q5));
	double ratio67=log((1.*q6)/(1.*q7));
 	q45_vec.emplace_back(ratio45);
 	q67_vec.emplace_back(ratio67);
   
    }
  }

  double sum_45=std::accumulate(q45_vec.begin(),q45_vec.end(),0.);
  sum_45 /= q45_vec.size();
  double sum_67=std::accumulate(q67_vec.begin(),q67_vec.end(),0.);
  sum_67 /= q67_vec.size();

  return std::make_unique<Data>((double)xpos,(double)ypos,sum_45,sum_67);
}

void PositionCalibration(const std::vector<std::unique_ptr<Data>>& vecOfData)
{
  // 1. Initialize the Fitters
  // "Hyp3" specifies a formula with 3 parameters + 1 intercept
  // The formula: y = p0 + p1*X + p2*Y + p3*(X*Y)
  TLinearFitter *fitterX = new TLinearFitter(2, "hyp2");
  TLinearFitter *fitterY = new TLinearFitter(2, "hyp2");

  // 3. Add data points to the fitters
  for (int i = 0; i < vecOfData.size(); ++i) {
    double x_r         = vecOfData[i]->q67;
    double y_r         = vecOfData[i]->q45;
    double interaction = x_r * y_r;

    std::cout <<"(" << vecOfData[i]->xtrue <<","<<vecOfData[i]->ytrue <<") :: " <<  x_r << " : " << y_r << std::endl;

    double input_vars[3] = {x_r, y_r, interaction};

    fitterX->AddPoint(input_vars, vecOfData[i]->xtrue);
    fitterY->AddPoint(input_vars, vecOfData[i]->ytrue);
  }

  // 4. Perform the Fit
  fitterX->EvalRobust();
  fitterY->EvalRobust();
  //fitterX->Eval();
  //fitterY->Eval();

  // 5. Extract Coefficients
  TVectorD coeffsX, coeffsY;
  fitterX->GetParameters(coeffsX);
  fitterY->GetParameters(coeffsY);

  // Results mapping:
  // Index 0: Intercept (a0/b0)
  // Index 1: X_raw coefficient (a1/b1)
  // Index 2: Y_raw coefficient (a2/b2)
  // Index 3: Interaction coefficient (a3/b3)

/*  std::cout << "--- X-Coordinate Coefficients (a) ---" << std::endl;
  // Use brackets [ ] to access the specific double values
  printf("a0 (Offset): %0.4f\na1 (Scale X): %0.4f\na2 (Skew Y): %0.4f\na3 (Interaction): %0.4f\n\n", 
         coeffsX, coeffsX, coeffsX, coeffsX);

  std::cout << "--- Y-Coordinate Coefficients (b) ---" << std::endl;
  printf("b0 (Offset): %0.4f\nb1 (Skew X): %0.4f\nb2 (Scale Y): %0.4f\nb3 (Interaction): %0.4f\n", 
         coeffsY, coeffsY, coeffsY, coeffsY);*/

  coeffsX.Print();
  coeffsY.Print();
  // 6. Cleanup
  delete fitterX;
  delete fitterY;
}



