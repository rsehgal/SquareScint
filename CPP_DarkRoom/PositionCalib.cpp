#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <vector>
#include <iostream>
#include <TCanvas.h>
#include <TApplication.h>
#include <TTree.h>
#include <TGraph.h>
#include "Helpers.h"

int main(int argc, char *argv[])
{
  TApplication *fApp               = new TApplication("fApp", NULL, NULL);
  std::vector<std::string> fileVec = {"Data_-40.root", "Data_-30.root", "Data_-20.root", "Data_-10.root", "Data_0.root",
                                      "Data_10.root",  "Data_20.root",  "Data_30.root",  "Data_40.root"};

/*  std::vector<std::string> fileVec = {"output_-40.root", "output_-30.root", "output_-20.root", "output_0.root",
                                      "output_10.root",  "output_20.root",  "output_30.root",  "output_40.root"};
*/

  //std::string prefix="paired_DataF_Bar1_Bar2_1635_1555_1617_1582_Cs137_";
  std::string prefix="output_";


  TCanvas *can           = new TCanvas("TDC", "TDC");
  std::vector<float> pos = {-40., -30., -20., -10., 0., 10., 20., 30., 40};
  //std::vector<float> pos = {-40., -30., -20.,  0., 10., 20., 30., 40};
  std::vector<float> meanVal;
  can->Divide(2, 2);
  can->cd(1);

  std::vector<Int_t> testDelTVec;
  short testIndex = 8;

  std::vector<int> barIdVec={0};//,1};

  TFile *fcalib = new TFile("calib.root","RECREATE");
  //std::string barPrefix="Bar_";
//  for(unsigned int barInd = 0 ; barInd < barIdVec.size() ; barInd++){
//  std::string barName = barPrefix+std::to_string(barIdVec[barInd]);
  meanVal.clear();
  testDelTVec.clear();
  for (unsigned int fid = 0; fid < pos.size(); fid++) {

    std::string histname = "hist_" + fileVec[fid];
    TH1F *hist           = new TH1F(histname.c_str(), histname.c_str(), 500, -25000, 25000);
    hist->SetDirectory(0);
    // std::string fullFileName="Cs137/"+fileVec[fid];
    //std::string fullFileName = std::string(argv[1]) + "/" + fileVec[fid];
    //std::string fullFileName = prefix+std::to_string((int)pos[fid])+"cm_1min.root";
    std::string fullFileName = prefix+std::to_string((int)pos[fid])+".root";


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

    ULong64_t t2;
    ULong64_t t3;
    UShort_t q2;
    UShort_t q3;


/*    ftree->SetBranchAddress("q2", &qL);
    ftree->SetBranchAddress("q3", &qR);
    ftree->SetBranchAddress("t2", &tL);
    ftree->SetBranchAddress("t3", &tR);

    //probe bar
    ftree->SetBranchAddress("q0", &q2);
    ftree->SetBranchAddress("q1", &q3);

    ftree->SetBranchAddress("t0", &t2);
    ftree->SetBranchAddress("t1", &t3);
*/
    ftree->SetBranchAddress("q0", &qL);
    ftree->SetBranchAddress("q1", &qR);
    ftree->SetBranchAddress("t0", &tL);
    ftree->SetBranchAddress("t1", &tR);

    //probe bar
    ftree->SetBranchAddress("q2", &q2);
    ftree->SetBranchAddress("q3", &q3);

    ftree->SetBranchAddress("t2", &t2);
    ftree->SetBranchAddress("t3", &t3);


    Long64_t nentries = ftree->GetEntries();

    Long64_t nbytes = 0;
    for (Long64_t i = 0; i < nentries; i++) {

        nbytes += ftree->GetEntry(i);
      //if (barId == barIdVec[barInd]) 
      Long64_t probeDelT = t2-t3;
      if(std::sqrt(qL*qR) > 500. ) // && std::sqrt(qL*qR) < 1500.) // && abs(probeDelT)<200)
      {

   
        Long64_t delT = tL-tR;// (Long64_t)tL - (Long64_t)tR;
        //std::cout << "tL : " << tL << " :: tR : " << tR << " :: DelT : " << delT << std::endl;
        //hist->Fill(delT * 25);
        if(abs(probeDelT)<100. && std::sqrt(q2*q3) > 550.)
        hist->Fill(delT);
        if (fid == testIndex) {
        //if (fid == testIndex && std::sqrt(q2*q3) > 500. && abs(probeDelT) < 200.) {
          //testDelTVec.push_back(delT * 25);
          testDelTVec.push_back(delT);
        }
      }
    }

    hist->Scale(1. / hist->Integral());
    TF1 *fo=FitRestricted(hist);
    meanVal.push_back(fo->GetParameter(1));
      hist->Draw("histsame");
  }

  can->cd(2);
  TGraph *gr = new TGraph(pos.size(), &meanVal[0], &pos[0]);
  gr->SetMarkerStyle(8);
  TF1 *pol1 = new TF1("subjectBar", "pol1");
  gr->Fit(pol1);
  gr->Draw("ap");
  
  fcalib->cd();
  gr->Write();
  pol1->Write();

  can->cd(3);
  TH1F *histPos = new TH1F("Position", "Position", 160, -80., 80.);
  for (unsigned int i = 0; i < testDelTVec.size(); i++) {
    //std::cout << testDelTVec[i] << std::endl;
    histPos->Fill(pol1->Eval(testDelTVec[i]));
  }
  histPos->Draw("hist");
  TF1 *posForm = new TF1("gausPos", "gaus");
  histPos->Fit(posForm);
  posForm->Draw("same");



//} //barIndex loop
  //fcalib->Close();
  fApp->Run();
}
