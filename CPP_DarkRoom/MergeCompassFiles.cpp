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

std::vector<std::string> GetVecOfFiles(std::string runPrefix = "Capture_Data_BG",
                                       std::string directory = "/home/muon/RemoteWork/DAQ/")
{
  std::vector<std::string> vecOfFiles;
  for (unsigned int i = 0; i < 24; i++) {
    std::string runPrefix2 = "";
    if (i)
      runPrefix2 += runPrefix + "_" + std::to_string(i);
    else
      runPrefix2 = runPrefix;
    std::string filepath = directory + runPrefix2 + "/FILTERED/DataF_" + runPrefix2 + ".root";
    // std::cout << filepath << std::endl;
    vecOfFiles.push_back(filepath);
  }
  return vecOfFiles;
}

int main()
{
  TFile *fout = new TFile("combined_raw.root", "RECREATE");
  fout->cd();
  TTree *ftree = new TTree("Data_F", "Merged TTree"); // nullptr;

  // Data variables
  UShort_t Channel, Board, Energy, EnergyShort;
  ULong64_t Timestamp;
  UInt_t Flags;

  ftree->Branch("Channel", &Channel);
  ftree->Branch("Timestamp", &Timestamp);
  ftree->Branch("Board", &Board);
  ftree->Branch("Energy", &Energy);
  ftree->Branch("EnergyShort", &EnergyShort);
  ftree->Branch("Flags", &Flags);

  ULong64_t cumulativeOffset = 0;
  /*std::vector<const char *> filenameVec = {
      "/home/user/DataF_CFD_Square1_Square2_LongRun_2.root",
      "/home/user/DataF_CFD_Square1_Square2_LongRun_3.root"
  };*/

  std::vector<std::string> filenameVec = GetVecOfFiles();
  for (auto fname : filenameVec) {
    TFile *fp = TFile::Open(fname.c_str(), "READ");
    if (!fp || fp->IsZombie()) {
      std::cerr << "Error opening " << fname << std::endl;
      continue;
    }

    TTree *Data_F = (TTree *)fp->Get("Data_F");
    Data_F->SetBranchAddress("Channel", &Channel);
    Data_F->SetBranchAddress("Timestamp", &Timestamp);
    Data_F->SetBranchAddress("Board", &Board);
    Data_F->SetBranchAddress("Energy", &Energy);
    Data_F->SetBranchAddress("EnergyShort", &EnergyShort);
    Data_F->SetBranchAddress("Flags", &Flags);

    ULong64_t maxTimestampInFile = 0;
    Long64_t nentries            = Data_F->GetEntries();

    for (Long64_t i = 0; i < nentries; i++) {
      Data_F->GetEntry(i);

      if (Timestamp > maxTimestampInFile) maxTimestampInFile = Timestamp;

      // Apply offset
      Timestamp += cumulativeOffset;
      ftree->Fill();
    }

    std::cout << "Processed: " << fname << "\n Added offset: " << cumulativeOffset << std::endl;

    // Update offset for next file
    cumulativeOffset += (maxTimestampInFile + 1);

    fp->Close();
    delete fp; // Clean up memory
  }

  fout->cd();
  ftree->Write();
  fout->Close();
  return 0;
}
