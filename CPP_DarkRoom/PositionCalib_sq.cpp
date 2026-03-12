#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <vector>
#include <iostream>
#include <TCanvas.h>
#include <TApplication.h>
#include <TTree.h>
#include <TGraph.h>
int main(int argc, char *argv[])
{
  TApplication *fApp               = new TApplication("fApp", NULL, NULL);
  std::vector<std::string> fileVec = {"Data_-20.root", "Data_-15.root", "Data_-10.root", "Data_-5.root", "Data_0.root",
                                      "Data_5.root",   "Data_10.root",  "Data_15.root",  "Data_20.root"};

  TCanvas *can           = new TCanvas("TDC", "TDC");
  std::vector<float> pos = {-20., -15., -10., -5., 0., 5., 10., 15., 20};
  std::vector<float> meanVal;
  can->Divide(2, 2);
  can->cd(1);

  std::vector<Int_t> testDelTVec;
  short testIndex = 4;

  for (unsigned int fid = 0; fid < fileVec.size(); fid++) {

    std::string histname = "hist_" + fileVec[fid];
    //TH1F *hist           = new TH1F(histname.c_str(), histname.c_str(), 500, -25000, 25000);
    TH1F *hist           = new TH1F(histname.c_str(), histname.c_str(), 100,-10,10);
    hist->SetDirectory(0);
    // std::string fullFileName="Cs137/"+fileVec[fid];
    std::string fullFileName = std::string(argv[1]) + "/" + fileVec[fid];
    const char *filename     = fullFileName.c_str();

    std::cout << "Processing file : " << filename << std::endl;
    TFile *f     = new TFile(filename, "r");
    TTree *ftree = (TTree *)f->Get("ftree");

    /*// Declaration of leaves types
    Int_t tL;
    Int_t tR;
    Int_t delT;
*/
    UShort_t barId;
    ULong64_t tL;
    ULong64_t tR;
    UShort_t qL;
    UShort_t qR;

    // Set branch addresses.
    ftree->SetBranchAddress("barId", &barId);
    ftree->SetBranchAddress("tL", &tL);
    ftree->SetBranchAddress("tR", &tR);
    ftree->SetBranchAddress("qL", &qL);
    ftree->SetBranchAddress("qR", &qR);
    // ftree->SetBranchAddress("delT", &delT);

    //     This is the loop skeleton
    //       To read only selected branches, Insert statements like:
    // ftree->SetBranchStatus("*",0);  // disable all branches
    // TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

    Long64_t nentries = ftree->GetEntries();

    Long64_t delT   = -5000000;
    Long64_t nbytes = 0;
    for (Long64_t i = 0; i < nentries; i++) {

      nbytes += ftree->GetEntry(i);
      if (barId == 2) {
        // hist->Fill(delT * 25);
        delT = log((1.*qL)/(1.*qR));//tL - tR;
        std::cout << "DelT : " << delT << std::endl;
        hist->Fill(delT);
        if (fid == testIndex) {
          // testDelTVec.push_back(delT * 25);
          testDelTVec.push_back(delT);
        }
      }
    }

    TF1 *fo = new TF1("gaus", "gaus");
    hist->Scale(1. / hist->Integral());
    hist->Fit(fo);
    std::cout << "Mean : " << fo->GetParameter(1) << std::endl;
    meanVal.push_back(fo->GetParameter(1));
    hist->Draw("PLC histsame");
    f->Close();
  }

  can->cd(1);
  can->Update();
  can->Modified();

  can->cd(2);
  TGraph *gr = new TGraph(pos.size(), &meanVal[0], &pos[0]);
  gr->SetMarkerStyle(8);
  TF1 *pol1 = new TF1("pol3", "pol3");
  gr->Fit(pol1);
  gr->Draw("ap");

  can->cd(3);
  TH1F *histPos = new TH1F("Position", "Position", 160, -80., 80.);
  for (unsigned int i = 0; i < testDelTVec.size(); i++) {
    histPos->Fill(pol1->Eval(testDelTVec[i]));
  }
  TF1 *posForm = new TF1("gausPos", "gaus");
  histPos->Fit(posForm);
  histPos->Draw("hist");
  posForm->Draw("same");

  fApp->Run();
}
