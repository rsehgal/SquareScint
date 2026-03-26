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
  // searchDir="/home/muon/ExpData/Square_Bar1_Bar2_BG_12Mar/Mu_Slab/";
  std::string searchDir = "/home/rsehgal/shared/SquareScint_DarkRoom/ExpData_DarkRoom/Slab_Cylinder/";
  //    "/Users/rsehgal/Codes/SquareScint/ExpData_DarkRoom/Slab_Cylinder/";
  std::vector<short> locationsVec = {-20, -10, 0, 10, 20};

  std::vector<TH2F*> unitCorrFactorHistVec = GetUnitCorrFactorHistVector(locationsVec);
  std::vector<TH2F*> vecOfCorrHist=GetCorrectionFactorHist(searchDir,locationsVec,unitCorrFactorHistVec);
  std::vector<TH2F*> vecOfCorredHist=GetCorrectionFactorHist(searchDir,locationsVec,vecOfCorrHist);
  TH2F *entriesHist = GetEntriesHist(locationsVec);

  TH2F* uniformityHist_gm_og = vecOfCorrHist[0]; 
  TH2F* uniformityHist_ar_og= vecOfCorrHist[1];

  TH2F* uniformityHist_gm = vecOfCorredHist[0]; 
  TH2F* uniformityHist_ar= vecOfCorredHist[1];

  uniformityHist_gm_og->SetMinimum(0.);
  uniformityHist_gm_og->SetMaximum(1.);
  uniformityHist_ar_og->SetMinimum(0.);
  uniformityHist_ar_og->SetMaximum(1.);
  uniformityHist_gm_og->GetXaxis()->SetNdivisions(5, kFALSE);
  uniformityHist_gm_og->GetYaxis()->SetNdivisions(5, kFALSE);
  uniformityHist_ar_og->GetXaxis()->SetNdivisions(5, kFALSE);
  uniformityHist_ar_og->GetYaxis()->SetNdivisions(5, kFALSE);
  uniformityHist_gm_og->SetMarkerColor(kWhite);
  uniformityHist_gm_og->SetMarkerSize(1.5);
  uniformityHist_ar_og->SetMarkerColor(kWhite);
  uniformityHist_ar_og->SetMarkerSize(1.5);


  uniformityHist_gm->SetMinimum(0.);
  uniformityHist_gm->SetMaximum(1.);
  uniformityHist_ar->SetMinimum(0.);
  uniformityHist_ar->SetMaximum(1.);
  uniformityHist_gm->GetXaxis()->SetNdivisions(5, kFALSE);
  uniformityHist_gm->GetYaxis()->SetNdivisions(5, kFALSE);
  uniformityHist_ar->GetXaxis()->SetNdivisions(5, kFALSE);
  uniformityHist_ar->GetYaxis()->SetNdivisions(5, kFALSE);
  uniformityHist_gm->SetMarkerColor(kWhite);
  uniformityHist_gm->SetMarkerSize(1.5);
  uniformityHist_ar->SetMarkerColor(kWhite);
  uniformityHist_ar->SetMarkerSize(1.5);

  entriesHist->SetMinimum(0.);
  entriesHist->SetMaximum(1.);
  entriesHist->SetMinimum(0.);
  entriesHist->SetMaximum(1.);
  entriesHist->GetXaxis()->SetNdivisions(5, kFALSE);
  entriesHist->GetYaxis()->SetNdivisions(5, kFALSE);
  entriesHist->GetXaxis()->SetNdivisions(5, kFALSE);
  entriesHist->GetYaxis()->SetNdivisions(5, kFALSE);
  entriesHist->SetMarkerColor(kWhite);
  entriesHist->SetMarkerSize(1.5);
  entriesHist->SetMarkerColor(kWhite);
  entriesHist->SetMarkerSize(1.5);

  // TCanvas *can=new TCanvas("Uniformity","Uniformity");

  //TCanvas *can = new TCanvas("Uniformity", "Uniformity", 10, 47, 1545, 700);
  TCanvas *can = new TCanvas("Uniformity", "Uniformity", 10, 10, 2200, 1500);
  can->Divide(3, 2);
  can->cd(1);
  uniformityHist_gm_og->Draw("colz text");
  can->cd(2);
  uniformityHist_gm->Draw("colz text");
  can->cd(3);
  uniformityHist_ar_og->Draw("colz text");
  can->cd(4);
  uniformityHist_ar->Draw("colz text");

  can->cd(5);
  entriesHist->Draw("colz text");

  can->SaveAs("uniformity.eps");
  fApp->Run();
}
