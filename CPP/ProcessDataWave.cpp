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
#include <TArrayS.h>

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

  TFile *outputFile = new TFile(outfile.c_str(),"RECREATE");
  TTree *treeML = new TTree("treeML","A Tree for ML"); 
  std::vector<int> qVec;

  UShort_t q0=0;
  UShort_t q1=0;
  UShort_t q2=0;
  UShort_t q3=0;
  UShort_t q4=0;
  UShort_t q5=0;
  UShort_t q6=0;
  UShort_t q7=0;


  ULong64_t t0=0;
  ULong64_t t1=0;
  ULong64_t t2=0;
  ULong64_t t3=0;
  ULong64_t t4=0;
  ULong64_t t5=0;
  ULong64_t t6=0;
  ULong64_t t7=0;

  TArrayS *samples0=nullptr;
  TArrayS *samples1=nullptr;
  TArrayS *samples2=nullptr;
  TArrayS *samples3=nullptr;
  TArrayS *samples4=nullptr;
  TArrayS *samples5=nullptr;
  TArrayS *samples6=nullptr;
  TArrayS *samples7=nullptr;


  treeML->Branch("qVec",&qVec);
  treeML->Branch("q0",&q0);
  treeML->Branch("q1",&q1);
  treeML->Branch("q2",&q2);
  treeML->Branch("q3",&q3);
  treeML->Branch("q4",&q4);
  treeML->Branch("q5",&q5);
  treeML->Branch("q6",&q6);
  treeML->Branch("q7",&q7);


  treeML->Branch("t0",&t0);
  treeML->Branch("t1",&t1);
  treeML->Branch("t2",&t2);
  treeML->Branch("t3",&t3);
  treeML->Branch("t4",&t4);
  treeML->Branch("t5",&t5);
  treeML->Branch("t6",&t6);
  treeML->Branch("t7",&t7);
 
  treeML->Branch("samples0",&samples0);
  treeML->Branch("samples1",&samples1);
  treeML->Branch("samples2",&samples2);
  treeML->Branch("samples3",&samples3);
  treeML->Branch("samples4",&samples4);
  treeML->Branch("samples5",&samples5);
  treeML->Branch("samples6",&samples6);
  treeML->Branch("samples7",&samples7);
  
  // Declaration of leaves types
  UShort_t Channel;
  ULong64_t Timestamp;
  UShort_t Board;
  UShort_t Energy;
  UShort_t EnergyShort;
  UInt_t Flags;
  TArrayS *Samples=nullptr;

  // Set branch addresses.
  Data_F->SetBranchAddress("Channel", &Channel);
  Data_F->SetBranchAddress("Timestamp", &Timestamp);
  Data_F->SetBranchAddress("Board", &Board);
  Data_F->SetBranchAddress("Energy", &Energy);
  Data_F->SetBranchAddress("EnergyShort", &EnergyShort);
  Data_F->SetBranchAddress("Flags", &Flags);
  Data_F->SetBranchAddress("Samples",&Samples);

  //     This is the loop skeleton
  //       To read only selected branches, Insert statements like:
  // Data_F->SetBranchStatus("*",0);  // disable all branches
  // TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

  Long64_t nentries = Data_F->GetEntries();

  std::cout << "============Actual number of Entries : " << nentries << " ================" << std::endl;
  Long64_t nbytes = 0;

  //Hit h;

  // std::vector<std::shared_ptr<Hit>> vecOfHits;
  std::vector<Hit *> vecOfHits;
  // nentries          = 50000;
  int printProgress = 1000000;

  if (nentries % 2) nentries -= 1;
  for (Long64_t i = 0; i < nentries; i++) {
    if (!(i % printProgress)) std::cout << "Read " << i << " events...." << std::endl;

    nbytes += Data_F->GetEntry(i);
    //h.Set(Channel, Timestamp, Board, Energy, EnergyShort, Flags,Samples);
    // h.Print();
    // vecOfHits.push_back(std::make_shared<Hit>(Channel, Timestamp, Board, Energy, EnergyShort, Flags));
    vecOfHits.push_back(new Hit(Channel, Timestamp, Board, Energy, EnergyShort, Flags,Samples));
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
    if ((ts - start) < 24000)
      vec.push_back(vecOfHits[i]);
    else {
      // std::cout << "Inserting the event : Size : " << vec.size() << std::endl;
      /*
      std::vector<UShort_t> vecChannels;

      {
        // This block is just to fill histogram of size of vec
        for (unsigned int v = 0; v < vec.size(); v++) {
          vecChannels.push_back(vec[v]->Channel);
        }
        std::set<UShort_t> s(vecChannels.begin(), vecChannels.end());
        if (s.size() == vecChannels.size()) histSize->Fill(vecChannels.size());
      }
      */

      vecOfVecOfHits.push_back(vec);
      start = vecOfHits[i]->Timestamp;
      vec.clear();
      vec.push_back(vecOfHits[i]);
      // i++;
    }
    // vecOfHits[i]->Print();
  }

  std::cout << "Size of vecOfVecOfHits.size : " << vecOfVecOfHits.size() << std::endl;

  /*
  for(const auto& vec : vecOfVecOfHits){
	  if (vec.size()==8){
		  std::cout << "Correct event found....." <<std::endl;
	  }
  }
  */

  //return 0;

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

    
    /*std::cout <<"========= " << i  << " ===========" << std::endl;
    for (const auto &hit : hs) {
     hit->Print();
    }*/
    
    
    //if (EqualSets(hs, slabWithTopAndBottomBar)) {
    qVec.clear();
    if (EqualSets(hs, slabWithTopAndBottomBar)) {
    //if (EqualSets(hs, slabWithTopBar)) {
    //if (EqualSets(hs, slabWithBottomBar)) {
    //if (EqualSets(hs, slab)) {
    //  std::cout << "======================================" << std::endl;
	qVec.resize(8);
      for (const auto &hit : hs) {
        // hit->Print();
	if(hit->Channel < 8){
        vecOfHist[hit->Channel]->Fill(hit->Energy);
	qVec[hit->Channel]=hit->Energy;

	if(hit->Channel==0){q0=hit->Energy; t0=hit->Timestamp; samples0=hit->Samples;}  
	if(hit->Channel==1){q1=hit->Energy; t1=hit->Timestamp; samples1=hit->Samples;}
	if(hit->Channel==2){q2=hit->Energy; t2=hit->Timestamp; samples2=hit->Samples;}
	if(hit->Channel==3){q3=hit->Energy; t3=hit->Timestamp; samples3=hit->Samples;}
	if(hit->Channel==4){q4=hit->Energy; t4=hit->Timestamp; samples4=hit->Samples;}
	if(hit->Channel==5){q5=hit->Energy; t5=hit->Timestamp; samples5=hit->Samples;}
	if(hit->Channel==6){q6=hit->Energy; t6=hit->Timestamp; samples6=hit->Samples;}
	if(hit->Channel==7){q7=hit->Energy; t7=hit->Timestamp; samples7=hit->Samples;}

      }
}
	treeML->Fill();
	

    }
  }

  treeML->Write();
  outputFile->Close();

  std::cout << "All Done...................." << std::endl;

/*  ULong64_t maxHEnties = 0;
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
*/
 /* new TCanvas("TopBar","TopBar");
  vecOfHist[4]->Draw("hist");
  vecOfHist[5]->Draw("histsames");*/
  //fApp->Run();
}
