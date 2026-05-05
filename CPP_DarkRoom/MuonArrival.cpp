#include <TH1F.h>
#include <TFile.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TTree.h>
#include <TStyle.h>
#include <TH2F.h>
#include <TProfile.h>
#include <fstream>
#include <TGraph.h>
#include <algorithm>
#include <numeric>
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);

  TFile *f     = new TFile(argv[1]);
  TTree *ftree = (TTree *)f->Get("ftree");

  gStyle->SetOptFit(111);
  // Declaration of leaves types
  //    vector<int>     qVec;
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

  UShort_t qth = 150;
  std::vector<ULong64_t> tsVec;
  std::vector<UShort_t> qVec;

  std::vector<ULong64_t> arrTimeVec;

  TH1F *hist      = new TH1F("MuonArrival", "MuonArrival", 100, 0, 250);
  Long64_t nbytes = 0;
  for (Long64_t i = 0; i < ftree->GetEntries(); i++) {
    tsVec.clear();
    qVec.clear();
    nbytes += ftree->GetEntry(i);

    qVec.push_back(q4);
    qVec.push_back(q5);
    qVec.push_back(q6);
    qVec.push_back(q7);

    bool result = std::all_of(qVec.begin(), qVec.end(), [qth](UShort_t val) { return val > qth; });
    if (result) {
      tsVec.push_back(t4);
      tsVec.push_back(t5);
      tsVec.push_back(t6);
      tsVec.push_back(t7);
      auto minVal = std::min_element(tsVec.begin(), tsVec.end());
      arrTimeVec.push_back(*minVal);
    }
  }

  std::vector<Long64_t> diffs(arrTimeVec.size());
  std::adjacent_difference(arrTimeVec.begin(), arrTimeVec.end(), diffs.begin());

  for (const auto &val : diffs) {
    hist->Fill(val / 1000000000.); // converted to milliseconds
  }
  std::cout << "Total number of muon event : " << arrTimeVec.size() << std::endl;


  TF1 *formu = new TF1("muarrival","[A]*exp(-x/[T])+[C]",0,250);
  formu->SetParameters(100,25,10);
  hist->Fit(formu,"R");

  hist->SetMarkerStyle(8);  
  hist->Draw("PE1");
  formu->Draw("same");
  fApp->Run();
}
