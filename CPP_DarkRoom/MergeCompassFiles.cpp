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
int main(int argc, char *argv[])
{

  TFile *fout  = new TFile("combined_raw.root", "RECREATE");
  TTree *ftree = nullptr; // new TTree("Data_F", "Combined files");
  UShort_t Channel;
  ULong64_t Timestamp;
  UShort_t Board;
  UShort_t Energy;
  UShort_t EnergyShort;
  UInt_t Flags;

  ULong64_t cumulativeOffset = 0;
  ULong64_t maxTimestamp     = 0;

  // std::vector<char*>
  // filenameVec={"DataF_CFD_Square_LongRun_23Mar.root","DataF_CFD_Square_LongRun_23Mar_1.root","DataF_CFD_Square_LongRun_23Mar_2.root","DataF_CFD_Square_LongRun_23Mar_3.root"};

  std::vector<const char *> filenameVec = {"~/DataF_CFD_Square1_Square2_LongRun_2.root",
                                           "~/DataF_CFD_Square1_Square2_LongRun_3.root"};
  ULong64_t start                       = 0;

  for (unsigned int fid = 0; fid < filenameVec.size(); fid++) {

    maxTimestamp = 0;
    std::cout << "@@@@ Processing File : " << filenameVec[fid] << " @@@@" << std::endl;
    std::cout << "Cumulative Offset : " << cumulativeOffset << std::endl;

    TFile *fp     = new TFile(filenameVec[fid]);
    TTree *Data_F = (TTree *)fp->Get("Data_F");

    // Set branch addresses.
    Data_F->SetBranchAddress("Channel", &Channel);
    Data_F->SetBranchAddress("Timestamp", &Timestamp);
    Data_F->SetBranchAddress("Board", &Board);
    Data_F->SetBranchAddress("Energy", &Energy);
    Data_F->SetBranchAddress("EnergyShort", &EnergyShort);
    Data_F->SetBranchAddress("Flags", &Flags);

    if (!ftree) {
      ftree = Data_F->CloneTree(0);
      ftree->SetDirectory(fout); // Force the tree to live in the output file
    }else{
ftree->CopyAddresses(Data_F);
}

    Long64_t nentries = Data_F->GetEntries();

    Long64_t nbytes = 0;
    for (Long64_t i = 0; i < nentries; i++) {
      nbytes += Data_F->GetEntry(i);
      if (Timestamp > maxTimestamp) maxTimestamp = Timestamp;
      Timestamp += cumulativeOffset;
      ftree->Fill();
    }
    cumulativeOffset += maxTimestamp + 1;
    fp->Close();
  }

  ftree->Write();
  fout->Close();
}
