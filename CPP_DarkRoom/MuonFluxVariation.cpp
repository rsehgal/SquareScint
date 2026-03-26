#include <TApplication.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TTree.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "Helpers.h"
namespace fs = std::filesystem;

#include <vector>
#include <numeric>
#include <cmath>
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLine.h"

short hours=4;

void PlotNormalizedMuons(const std::vector<int>& counts) {
    int n = counts.size();
    std::vector<double> x(n), y(n), ex(n, 0.0), ey(n);

    // 1. Calculate Mean for Normalization
    double sum = std::accumulate(counts.begin(), counts.end(), 0.0);
    double mean = sum / n;

    for (int i = 0; i < n; ++i) {
        x[i] =hours*( i + 1); // Time bins (every 4 hours)
        
        // 2. Normalize: (Current - Mean) / Mean * 100
        y[i] = ((counts[i] - mean) / mean) * 100.0;
        
        // 3. Statistical Error: sqrt(N) / Mean * 100
        ey[i] = (std::sqrt(counts[i]) / mean) * 100.0;
    }

    // 4. Create ROOT Graph
    TGraphErrors *gr = new TGraphErrors(n, &x[0], &y[0], &ex[0], &ey[0]);
    TCanvas *c1 = new TCanvas("c1", "Normalized Muon Flux", 800, 600);
    
    gr->SetTitle("Relative Muon Flux Variation;Time Bin (4-hour intervals);#Delta I / I_{avg} (%)");
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.2);
    gr->SetLineWidth(2);
    gr->Draw("ALP"); // A=Axes, L=Line, P=Points

    // Add a horizontal line at zero for reference
    TLine *line = new TLine(0.5, 0, n + 0.5, 0);
    line->SetLineStyle(2);
    line->Draw();

    c1->SaveAs("normalized_muon_flux.png");
}
int main(int argc, char *argv[])
{

  TApplication *fApp = new TApplication("fApp",NULL,NULL);
  TFile *f     = new TFile(argv[1], "r");
  TTree *ftree = (TTree *)f->Get("ftree");

  // Declaration of leaves types
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

  //     This is the loop skeleton
  //       To read only selected branches, Insert statements like:
  // ftree->SetBranchStatus("*",0);  // disable all branches
  // TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

  Long64_t nentries = ftree->GetEntries();

  Long64_t nbytes = 0;
  nbytes += ftree->GetEntry(0);
  double avg_log      = (std::log(q4) + std::log(q5) + std::log(q6) + std::log(q7)) / 4.;
  double q_gm         = std::exp(avg_log);
  ULong64_t startTime = t4;
  ULong64_t endTime   = 0;

  ULong64_t counter = 0;

  int timingCounter=0;
  std::vector<int> timingVec;
  std::vector<int> muonVec;

  for (Long64_t i = 1; i < nentries; i++) {
    nbytes += ftree->GetEntry(i);

    double q_gm = 0.;
    if (q4 > 0 && q5 > 0 && q6 > 0 && q7 > 0) {
      double avg_log = (std::log(q4) + std::log(q5) + std::log(q6) + std::log(q7)) / 4.;
      q_gm           = std::exp(avg_log);
      ULong64_t del  = t4 - startTime;
      if (q_gm > 200) {

        if (del < hours*3600e+12)
          {
            counter++;
          }
        else {
	  timingCounter++;
	  timingVec.push_back(timingCounter);
  	  muonVec.push_back(counter);
          startTime = t4;
          std::cout << "DelT : " << del << " :: No. of Muons : " << counter << std::endl;
          counter = 0;
        }
      }
    }
  }

  PlotNormalizedMuons(muonVec);

/*  TGraph *gr=new TGraph(timingVec.size(),&timingVec[0],&muonVec[0]);
  gr->SetMarkerStyle(8);
  gr->Draw("ap");*/
  fApp->Run();
  return 0;
}
