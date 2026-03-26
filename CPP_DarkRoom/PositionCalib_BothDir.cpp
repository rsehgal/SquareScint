#include "Helpers.h"
#include "TGraph.h"
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
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);

  gStyle->SetPalette(kRainBow);
  gStyle->SetPaintTextFormat("3.2f");
  // TFile *f           = new TFile(argv[1]);

  TH2F *hist2d = new TH2F("delTMean", "delTMean", 100, -10000, 10000, 100, -10000, 10000);
  // std::string
  // searchDir="/home/muon/ExpData/Square_Bar1_Bar2_BG_12Mar/Mu_Slab/";
  std::string searchDir = "/home/rsehgal/shared/SquareScint_DarkRoom/ExpData_DarkRoom/Slab_Cylinder/";
  //    "/Users/rsehgal/Codes/SquareScint/ExpData_DarkRoom/Slab_Cylinder/";
  std::vector<short> locationsVec = {-20, -10, 0, 10, 20};
  // std::vector<short> locationsVec = {0};
  std::vector<short> locVec = {-20, -10, 0, 10, 20}; // {0};

  TCanvas *canDelT = new TCanvas("DelT", "DelT");
  canDelT->Divide(2, 1);

  std::vector<float> meanVal;

  short testIndex = 3;
  std::vector<Long64_t> testData;

  TFile *fout = new TFile("calib.root", "RECREATE");
  {
    std::vector<FittedGraph> parameterizationVec = GetVectorOfParameterization(searchDir, locationsVec, locVec, true);
    fout->cd();
    /*parameterizationVec[0].first->Write();
    parameterizationVec[0].second->Write(); */
    for (unsigned int paramId = 0; paramId < parameterizationVec.size(); paramId++) {
      parameterizationVec[paramId].first->Write();
      parameterizationVec[paramId].second->Write();
    }
  }
  {
    std::vector<FittedGraph> parameterizationVec = GetVectorOfParameterization(searchDir, locationsVec, locVec, false);
    fout->cd();
    /*parameterizationVec[0].first->Write();
    parameterizationVec[0].second->Write(); */
    for (unsigned int paramId = 0; paramId < parameterizationVec.size(); paramId++) {
      parameterizationVec[paramId].first->Write();
      parameterizationVec[paramId].second->Write();
    }
  }

  /* canDelT->cd(1);
   parameterizationVec[0].first->Draw("ap");*/
  /*canDelT->Modified();
  canDelT->Update();*/
  fout->Close();
  fApp->Run();
}
