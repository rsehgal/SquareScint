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
  // TApplication *fApp = new TApplication("fApp", NULL, NULL);
  TFile *f      = new TFile(argv[1]);
  TTree *Data_F = (TTree *)f->Get("Data_F");

  ULong64_t counter[4] = {0, 0, 0, 0};

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

  Long64_t nentries = Data_F->GetEntries();

  Long64_t nbytes = 0;

  ULong64_t startTime = 0;
  ULong64_t endTime   = 0;
  for (Long64_t i = 0; i < nentries; i++) {
    nbytes += Data_F->GetEntry(i);
    if (i == 0) startTime = Timestamp;
    if (i == (nentries - 1)) endTime = Timestamp;
    counter[Channel - 4]++;
  }

  ULong64_t totalTime = (endTime - startTime) / 1e+12;
  std::cout << "--------- Total Time -----------" << std::endl;
  std::cout << (totalTime/3600) <<" hours : "<< ((totalTime%3600)/60) << " mins" << std::endl;
  std::cout << "--------- Single Count -----------" << std::endl;
  std::cout << counter[0] << " , " << counter[1] << " , " << counter[2] << " , " << counter[3] << std::endl;
  double rate[4] = {0, 0, 0, 0};
  std::cout << "--------- Rate -----------" << std::endl;
  for (unsigned int i=0 ; i< 4 ; i++) {
    rate[i] = counter[i] / totalTime;
  }
  std::cout << rate[0] << " , " << rate[1] << " , " << rate[2] << " , " << rate[3] << std::endl;

  ULong64_t coincWindow = 50e-9;
  long double accCoincRate = 4*rate[0]*rate[1]*rate[2]*rate[3]*std::pow(coincWindow,3);
  
  std::cout << "--------- Accidental coincident Rate -----------" << std::endl;
  std::cout << accCoincRate << std::endl;
  // fApp->Run();
}
