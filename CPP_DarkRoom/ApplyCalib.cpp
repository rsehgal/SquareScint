/*
**	Filename : ApplyCalib.cpp
**	2026-03-24
**	username : rsehgal
*/
#include <iostream>
#include "Helpers.h"
#include <TF1.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <utility>
#include <TApplication.h>
#include <TStyle.h>
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  gStyle->SetPalette(kRainBow);
  std::cout << "Processing File : " << argv[1] << std::endl;

  TFile *fcalib = new TFile("calib.root", "r");
  TF1 *xcalib   = (TF1 *)fcalib->Get("Param_y_0");
  TF1 *ycalib   = (TF1 *)fcalib->Get("Param_x_0");

  TFile *f     = new TFile(argv[1], "r");
  TTree *ftree = (TTree *)f->Get("ftree");

  // gStyle->SetOptStat(0000);
  //  Declaration of leaves types
  //     vector<int>     qVec;
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

  Long64_t nentries = ftree->GetEntries();

  Long64_t nbytes = 0;

  TH2F *pos2d = new TH2F("position", "position", 50, -25, 25, 50, -25, 25);
  for (Long64_t i = 0; i < nentries; i++) {
    nbytes += ftree->GetEntry(i);
    //Long64_t xdelT = t6 - t7;
    //Long64_t ydelT = t4 - t5;

    double xdelT = -100000; //t6 - t7;
    double ydelT = -100000; //t4 - t5;

    if (q4 > 0 && q5 > 0 && q6 > 0 && q7 > 0) {
      xdelT = log((1. * q6) / (1. * q7));
      ydelT = log((1. * q4) / (1. * q5));
      pos2d->Fill(xcalib->Eval(xdelT), ycalib->Eval(ydelT));
    }
  }

  // pos2d->Draw("CONT");
  pos2d->Smooth(2);
  pos2d->Draw("colz");
  fApp->Run();

  return 0;
}
