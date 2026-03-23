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


int main(int argc, char *argv[]) {
  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  gStyle->SetPalette(kRainBow);
  gStyle->SetPaintTextFormat("3.2f");
  // TFile *f           = new TFile(argv[1]);

  TH2F *uniformityHist_gm =
      new TH2F("UniformityHist_GM", "UniformityHist_GM", 5, 0, 5, 5, 0, 5);
  TH2F *uniformityHist_ar =
      new TH2F("UniformityHist_AM", "UniformityHist_AR", 5, 0, 5, 5, 0, 5);

  // std::string
  // searchDir="/home/muon/ExpData/Square_Bar1_Bar2_BG_12Mar/Mu_Slab/";
  std::string searchDir ="/home/rsehgal/shared/SquareScint_DarkRoom/ExpData_DarkRoom/Slab_Cylinder/";
  //    "/Users/rsehgal/Codes/SquareScint/ExpData_DarkRoom/Slab_Cylinder/";
  std::vector<short> locationsVec = {-20, -10, 0, 10, 20};

  for (unsigned int locIdX = 0; locIdX < locationsVec.size(); locIdX++) {
    for (unsigned int locIdY = 0; locIdY < locationsVec.size(); locIdY++) {

      std::string locString = "_" + std::to_string(locationsVec[locIdX]) + "_" +
                              std::to_string(locationsVec[locIdY]);
      std::vector<std::string> searchTokens = {"output", locString};

      std::vector<std::string> vecOfFileNames =
          GetListOfFiles(searchTokens, searchDir);

      if (vecOfFileNames.size() > 0) {

        std::cout <<"Processing File : " << vecOfFileNames[0] << std::endl;
        TFile *f = new TFile((searchDir + vecOfFileNames[0]).c_str());
        TTree *ftree = (TTree *)f->Get("ftree");

        gStyle->SetOptStat(0000);
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

        //     This is the loop skeleton
        //       To read only selected branches, Insert statements like:
        // ftree->SetBranchStatus("*",0);  // disable all branches
        // TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

        Long64_t nentries = ftree->GetEntries();

        Long64_t nbytes = 0;

        TH2F *histCorr =
            new TH2F("HistCorr", "HistCorr", 500, 0, 1000, 200, 0, 200);

        double q_gm = 0.;
        double q_ar = 0.;
        for (Long64_t i = 0; i < nentries; i++) {
          nbytes += ftree->GetEntry(i);
          if (q4 > 0 && q5 > 0 && q6 > 0 && q7 > 0) {
            double avg_log =
                (std::log(q4) + std::log(q5) + std::log(q6) + std::log(q7)) /
                4.;
            q_gm += std::exp(avg_log);
            q_ar += (q4 + q5 + q6 + q7) / 4.;
          }
        }
        q_gm /= nentries;
        q_ar /= nentries;

        std::cout << locIdX << " : " << locIdY << std::endl;
        std::cout << "Q_GM : " << q_gm << " :: Q_AR : " << q_ar << std::endl;

        uniformityHist_gm->SetBinContent(locIdX + 1, locIdY + 1, q_gm);
        uniformityHist_ar->SetBinContent(locIdX + 1, locIdY + 1, q_ar);
      }
    }
  }

  if (uniformityHist_gm->GetMaximum() > 0) {
    uniformityHist_gm->Scale(1.0 / uniformityHist_gm->GetMaximum());
  }
  if (uniformityHist_ar->GetMaximum() > 0) {
    uniformityHist_ar->Scale(1.0 / uniformityHist_ar->GetMaximum());
  }

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

  // TCanvas *can=new TCanvas("Uniformity","Uniformity");

  TCanvas *can = new TCanvas("Uniformity", "Uniformity", 10, 47, 1545, 700);
  can->Divide(2, 1);
  can->cd(1);
  uniformityHist_gm->Draw("colz text");
  can->cd(2);
  uniformityHist_ar->Draw("colz text");
  can->SaveAs("uniformity.eps");
  fApp->Run();
}
