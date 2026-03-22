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
#include "TGraph.h"
namespace fs = std::filesystem;


int main(int argc, char *argv[]) {
  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  gStyle->SetPalette(kRainBow);
  gStyle->SetPaintTextFormat("3.2f");
  // TFile *f           = new TFile(argv[1]);

  TH2F *hist2d =
      new TH2F("delTMean", "delTMean", 5, 0, 5, 5, 0, 5);
    // std::string
  // searchDir="/home/muon/ExpData/Square_Bar1_Bar2_BG_12Mar/Mu_Slab/";
  std::string searchDir =
      "/Users/rsehgal/Codes/SquareScint/ExpData_DarkRoom/Slab_Cylinder/";
  std::vector<short> locationsVec = {-20, -10, 0, 10, 20};
  std::vector<short> locVec={0};
  
  TCanvas *canDelT = new TCanvas("DelT","DelT");
  canDelT->Divide(2,2);
  canDelT->cd(1);

  std::vector<float> meanVal;

  short testIndex=3;
  std::vector<Long64_t> testData;

  for (unsigned int locId = 0; locId < locationsVec.size(); locId++) {
    for (unsigned int loc = 0; loc < locVec.size(); loc++) {

      //std::string locString = "_" + std::to_string(locationsVec[locId]) + "_" +
        //                      std::to_string(locVec[loc]);
     std::string locString = "_" + std::to_string(locVec[loc]) + "_" +
                              std::to_string(locationsVec[locId]);

      std::vector<std::string> searchTokens = {"output", locString};

      std::string histName="hist_"+locString;
      TH1F *hist = new TH1F(histName.c_str(),histName.c_str(),100,-10000,10000);

      std::vector<std::string> vecOfFileNames =
          GetListOfFiles(searchTokens, searchDir);


      if (vecOfFileNames.size() > 0) {

        std::cout <<"Processing File : " << vecOfFileNames[0] << std::endl;
        TFile *f = new TFile((searchDir + vecOfFileNames[0]).c_str());
        TTree *ftree = (TTree *)f->Get("ftree");

        //gStyle->SetOptStat(0000);
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
  	    //Long64_t delT = t6-t7;
  	    Long64_t delT = t4-t5;
	    hist->Fill(delT);

            if(locId==testIndex){
	testData.push_back(delT);
}
          }
        }
        q_gm /= nentries;
        q_ar /= nentries;

        //std::cout << locId << " : " << loc << std::endl;
        //std::cout << "Q_GM : " << q_gm << " :: Q_AR : " << q_ar << std::endl;

      }
      std::cout << "locID : " << locId << std::endl;

      
      hist->Scale(1./hist->Integral());
      TF1 *fo=FitRestricted(hist);
      meanVal.push_back(fo->GetParameter(1));

      hist->SetLineColor(color[locId]);
      hist->Draw("histsames");
canDelT->Modified();  
  canDelT->Update();

    }
  }

  canDelT->cd(2);
  std::vector<float>locVec_float(locationsVec.begin(),locationsVec.end());
  TGraph *gr=new TGraph(locationsVec.size(),&meanVal[0],&locVec_float[0]);
  gr->SetMarkerStyle(8);
  gr->Draw("ap");

  TF1 *posCalib=new TF1("pol3","pol3");
  gr->Fit(posCalib);


  TH1F *posHist=new TH1F("Pos","Pos",140,-35,35);
  
  for(const auto &data : testData ){
	posHist->Fill(posCalib->Eval(data));
} 

  canDelT->cd(3);
  posHist->Draw("hist");
  
  canDelT->Modified();  
  canDelT->Update();
  fApp->Run();
}
