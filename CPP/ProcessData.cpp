#include "Hit.h"
#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "vector"
#include <algorithm>
#include <TApplication.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <set>
#include "Helpers.h"
#include "Histograms.h"
// bool SortHits(const std::shared_ptr<Hit> &a, const std::shared_ptr<Hit> &b)
bool SortHits(Hit *a, Hit *b)
{
  return a->Timestamp < b->Timestamp; // Ascending order
}

int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fAPP", NULL, NULL);
  TFile *fp          = new TFile(argv[1], "r");
  TTree *Data_F      = (TTree *)fp->Get("Data_F");

  std::string outfile = "output.root";
  if (argc > 2) outfile = std::string(argv[2]);

  // TFile *outputFile = new TFile(outfile.c_str(),"RECREATE");

  // Declaration of leaves types
  UShort_t Channel;
  ULong64_t Timestamp;
  UShort_t Board;
  UShort_t Energy;
  UShort_t EnergyShort;
  UInt_t Flags;

  // Set branch addresses.
  Data_F->SetBranchAddress("Channel", &Channel);
  Data_F->SetBranchAddress("Timestamp", &Timestamp);
  Data_F->SetBranchAddress("Board", &Board);
  Data_F->SetBranchAddress("Energy", &Energy);
  Data_F->SetBranchAddress("EnergyShort", &EnergyShort);
  Data_F->SetBranchAddress("Flags", &Flags);

  //     This is the loop skeleton
  //       To read only selected branches, Insert statements like:
  // Data_F->SetBranchStatus("*",0);  // disable all branches
  // TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

  Long64_t nentries = Data_F->GetEntries();

  std::cout << "============Actual number of Entries : " << nentries << " ================" << std::endl;
  Long64_t nbytes = 0;

  Hit h;

  // std::vector<std::shared_ptr<Hit>> vecOfHits;
  std::vector<Hit *> vecOfHits;
  // nentries          = 50000;
  int printProgress = 1000000;

  if (nentries % 2) nentries -= 1;
  for (Long64_t i = 0; i < nentries; i++) {
    if (!(i % printProgress)) std::cout << "Read " << i << " events...." << std::endl;

    nbytes += Data_F->GetEntry(i);
    h.Set(Channel, Timestamp, Board, Energy, EnergyShort, Flags);
    // h.Print();
    // vecOfHits.push_back(std::make_shared<Hit>(Channel, Timestamp, Board, Energy, EnergyShort, Flags));
    vecOfHits.push_back(new Hit(Channel, Timestamp, Board, Energy, EnergyShort, Flags));
    // vecOfHits[i]->Print();
  }

  std::cout << "Hits vector filled........." << std::endl;
  std::sort(vecOfHits.begin(), vecOfHits.end(), SortHits);
  std::cout << "Vector size : " << vecOfHits.size() << std::endl;

  // std::vector<std::vector<std::shared_ptr<Hit>>> vecOfVecOfHits;
  std::vector<std::vector<Hit *>> vecOfVecOfHits;

  TH1F *histSize = new TH1F("EventSize", "EventSize", 10, 0, 10);

  ULong64_t start = vecOfHits[0]->Timestamp;
  std::vector<Hit *> vec;
  vec.push_back(vecOfHits[0]);
  for (unsigned int i = 1; i < vecOfHits.size() - 1; i++) {
    if (!(i % printProgress)) std::cout << "Processed " << i << " events...." << std::endl;

    // std::vector<std::shared_ptr<Hit>> vec;
    ULong64_t ts = vecOfHits[i]->Timestamp;
    if ((ts - start) < 20000)
      vec.push_back(vecOfHits[i]);
    else {
      // std::cout << "Inserting the event : Size : " << vec.size() << std::endl;
      std::vector<UShort_t> vecChannels;

      {
        // This block is just to fill histogram of size of vec
        for (unsigned int v = 0; v < vec.size(); v++) {
          vecChannels.push_back(vec[v]->Channel);
        }
        std::set<UShort_t> s(vecChannels.begin(), vecChannels.end());
        if (s.size() == vecChannels.size()) histSize->Fill(vecChannels.size());
      }

      vecOfVecOfHits.push_back(vec);
      start = vecOfHits[i]->Timestamp;
      vec.clear();
      vec.push_back(vecOfHits[i]);
      // i++;
    }
    // vecOfHits[i]->Print();
  }

  std::cout << "Size of vecOfVecOfHits.size : " << vecOfVecOfHits.size() << std::endl;

  /*ULong64_t sizes[4] = {0, 0, 0, 0};

  for (unsigned int i = 0; i < vecOfVecOfHits.size(); i++) {
    sizes[vecOfVecOfHits[i].size() - 1]++;
  }

  for (unsigned int i = 0; i < 4; i++) {
    std::cout << i << " : " << sizes[i] << std::endl;
  }

  {

    ULong64_t numEntries = 0;
    for (unsigned int i = 0; i < histSize->GetNbinsX(); i++) {
      if (i > 0) numEntries += ((i - 1) * histSize->GetBinContent(i));
      std::cout << i << " : " << histSize->GetBinContent(i) << std::endl;
    }
    std::cout << "Total Entries from histogram : " << numEntries << std::endl;
  }*/

  histQ0->SetLineColor(1);
  histQ1->SetLineColor(2);
  histQ2->SetLineColor(4);
  histQ3->SetLineColor(8);
  histQ4->SetLineColor(6);
  histQ5->SetLineColor(30);
  histQ6->SetLineColor(40);
  histQ7->SetLineColor(46);

  for (unsigned int i = 0; i < vecOfVecOfHits.size(); i++) {
    HitSet hs = VecOfHitsToHitSet(vecOfVecOfHits[i]);

    //if (EqualSets(hs, slabWithTopAndBottomBar)) {
    if (EqualSets(hs, slabWithTopBar)) {
      // std::cout << "======================================" << std::endl;
      for (const auto &hit : hs) {
        // hit->Print();
        vecOfHist[hit->Channel]->Fill(hit->Energy);
      }
    }
  }

  std::cout << "All Done...................." << std::endl;

  ULong64_t maxHEnties = 0;
  short index          = -10;
  short i              = 0;
  for (const auto &hist : vecOfHist) {
    if (hist->GetEntries() > maxHEnties) {
      maxHEnties = hist->GetEntries();
      index      = i;
    }
    i++;
  }
  std::cout << "Index : " << index << " : Has max entries ..." << std::endl;

  vecOfHist[index]->Draw("hist");
  i = 0;
  for (const auto &hist : vecOfHist) {
    if (i != index) {
      hist->Draw("histsames");
    }
    i++;
  }

  new TCanvas("TopBar","TopBar");
  vecOfHist[4]->Draw("hist");
  vecOfHist[5]->Draw("histsames");
  fApp->Run();
}
