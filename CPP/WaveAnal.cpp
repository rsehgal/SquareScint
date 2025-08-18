/*
**	Filename : WaveAnal.cpp
**	2025-08-18
**	username : rsehgal
*/
#include<iostream>
#include <TFile.h>
#include <TArrayS.h>
#include <TTree.h>
#include <TApplication.h>
#include <algorithm>
#include <TH1F.h>
int main(int argc, char *argv[]){
TApplication *fApp = new TApplication("fApp",NULL,NULL);
TFile *f = new TFile("/data/WaveAnalysis/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_-20_-20_5min.root");
   TTree *Data_F = (TTree*)f->Get("Data_F");

//Declaration of leaves types
   UShort_t        Channel;
   ULong64_t       Timestamp;
   UShort_t        Board;
   UShort_t        Energy;
   UShort_t        EnergyShort;
   UInt_t          Flags;
   Int_t           Probe;
   TArrayS         *Samples=nullptr;

   // Set branch addresses.
   Data_F->SetBranchAddress("Channel",&Channel);
   Data_F->SetBranchAddress("Timestamp",&Timestamp);
   Data_F->SetBranchAddress("Board",&Board);
   Data_F->SetBranchAddress("Energy",&Energy);
   Data_F->SetBranchAddress("EnergyShort",&EnergyShort);
   Data_F->SetBranchAddress("Flags",&Flags);
   Data_F->SetBranchAddress("Probe",&Probe);
   Data_F->SetBranchAddress("Samples",&Samples);

//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// Data_F->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

   Long64_t nentries = Data_F->GetEntries();

   std::cout << "Nentries : " << nentries << std::endl;
   Long64_t nbytes = 0;

   TH1F *histMax = new TH1F("maxSample","maxSample",500,13000,13500);
   for (Long64_t i=0; i<nentries;i++) {
      nbytes += Data_F->GetEntry(i);

    Short_t maxVal = *std::max_element(Samples->GetArray(), Samples->GetArray() + Samples->GetSize());
    histMax->Fill(maxVal);
    //std::cout << "Maximum value in TArrayS: " << maxVal << std::endl;
   }
histMax->Draw("hist");
fApp->Run();
return 0;

}
