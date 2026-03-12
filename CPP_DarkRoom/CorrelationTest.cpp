#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <vector>
#include <iostream>
#include <TCanvas.h>
#include <TApplication.h>
#include <TTree.h>
#include <TGraph.h>
#include <TH2F.h>
#include <TProfile.h>
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);

  TCanvas *can = new TCanvas("Canvas", "Canvas");
  can->Divide(3, 2);
  char *filename = argv[1];
  std::cout << "Processing file : " << filename << std::endl;
  TFile *f     = new TFile(filename, "r");
  TTree *ftree = (TTree *)f->Get("ftree");

  UShort_t barId;
  ULong64_t tL;
  ULong64_t tR;
  UShort_t qL;
  UShort_t qR;

  ftree->SetBranchAddress("barId", &barId);
/*
  ftree->SetBranchAddress("qL", &qL);
  ftree->SetBranchAddress("qR", &qR);
  ftree->SetBranchAddress("tL", &tL);
  ftree->SetBranchAddress("tR", &tR);
*/
  ftree->SetBranchAddress("q0", &qL);
  ftree->SetBranchAddress("q1", &qR);
  ftree->SetBranchAddress("t0", &tL);
  ftree->SetBranchAddress("t1", &tR);

  Long64_t nentries = ftree->GetEntries();

  TH1F *hist     = new TH1F("delT", "delT", 250, -25000, 25000);
  TH1F *hist_corr_1     = new TH1F("delT_corr_1", "delT_corr_1", 250, -25000, 25000);
  TH1F *hist_corr_3     = new TH1F("delT_corr_3", "delT_corr_3", 250, -25000, 25000);
  //TH2F *histCorr = new TH2F("delTvsChargeRatio", "delTvsChargeRatio", 100, -10, 10, 250, -25000, 25000);
  TH2F *histCorr = new TH2F("delTvsChargeRatio", "delTvsChargeRatio", 500, 500, 5000, 250, -25000, 25000);
  //TH2F *histCorr_corr = new TH2F("delTvsChargeRatio_corr", "delTvsChargeRatio_corr", 100, -10, 10, 250, -25000, 25000);
  TH2F *histCorr_corr = new TH2F("delTvsChargeRatio_corr", "delTvsChargeRatio_corr", 500, 500, 5000, 250, -25000, 25000);

  //Cs137
  //double a = -2917.;

  //Muon
  double a = -4833.;

  double a0=-672.;
  double a1=-4835;

  double p0=-686.;
  double p1=-5326.;
  double p2=-100.;
  double p3=218.; 

/*  double p0=-686.;
  double p1=-5316.;
  double p2=-395.;
  double p3=142.; 
*/
  Long64_t nbytes = 0;
  for (Long64_t i = 0; i < nentries; i++) {

    nbytes += ftree->GetEntry(i);
    Long64_t delT = tL - tR; // (Long64_t)tL - (Long64_t)tR;

    double chargeLimit = 20;
    if (qL > chargeLimit && qR > chargeLimit && sqrt(qL*qR) >= 500.) {
      //double ratio = log((double)qL / (double)qR);
      //double ratio = sqrt((double)qL*(double)qR);
      double ratio = (double)qL;
      // std::cout << "tL : " << tL << " :: tR : " << tR << " :: DelT : " << delT << std::endl;
      // hist->Fill(delT * 25);
      hist->Fill(delT);
      histCorr->Fill(ratio, delT);

      double deltaTcorr_1 = delT - a1 * ratio;
      double deltaTcorr_3 = delT - p0 - p1 * ratio - p2*std::pow(ratio,2) - p3*std::pow(ratio,3);
      hist_corr_1->Fill(deltaTcorr_1);
      hist_corr_3->Fill(deltaTcorr_3);

      histCorr_corr->Fill(ratio, deltaTcorr_1);
    }
  }

  can->cd(1);
  hist->Draw();

  can->cd(2);
  histCorr->Draw("colz");

  can->cd(3);
  histCorr_corr->Draw("colz");

  can->cd(4);
  hist_corr_1->Draw();

  can->cd(5);
  TProfile *p = histCorr->ProfileX();
  p->SetTitle("ProfileX");
  p->Draw();
  //p->Fit("pol3");//,-2,2);

  can->cd(6);
  hist_corr_3->Draw();

  fApp->Run();
}
