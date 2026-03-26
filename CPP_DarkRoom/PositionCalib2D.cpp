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

int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  gStyle->SetPalette(kRainBow);
  gStyle->SetPaintTextFormat("5.4f");
  gStyle->SetOptStat(0000);

  std::vector<short> locationsVec = {-20, -10, 0, 10, 20};

  std::vector<std::unique_ptr<Data>> vecOfData;

  for (unsigned int i = 0; i < locationsVec.size(); i++) {
    for (unsigned int j = 0; j < locationsVec.size(); j++) {

      std::unique_ptr<Data> dt = GetMeanValues(locationsVec[i], locationsVec[j]);
      vecOfData.emplace_back(std::move(dt));
      // dt->Print(); // This should not work as the unique_ptr is pushed to the vector
    }
  }
/*
  //Available here
  for (const auto &data : vecOfData) {
    data->Print();
  }
*/

  PositionCalibration(vecOfData);

  
  return 0;
}
