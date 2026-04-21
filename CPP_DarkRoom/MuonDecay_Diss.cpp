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
  TApplication *fApp = new TApplication("fApp", NULL, NULL);

  TFile *fout    = new TFile("decay.root", "RECREATE");
  TTree *outTree = new TTree("muonElectron", "A simple Muon decay tree");

  UShort_t qMuon      = 6000;
  UShort_t qElectron  = 6000;
  ULong64_t tMuon     = 0;
  ULong64_t tElectron = 0;
  double delT         = -10.;

  outTree->Branch("qMuon", &qMuon);
  outTree->Branch("qElectron", &qElectron);
  outTree->Branch("tMuon", &tMuon);
  outTree->Branch("tElectron", &tElectron);
  outTree->Branch("delT", &delT);

  TFile *f     = new TFile(argv[1]);
  TTree *ftree = (TTree *)f->Get("ftree");

  gStyle->SetOptFit(111);
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

  Long64_t nentries = ftree->GetEntries();

  Long64_t nbytes   = 0;
  ULong64_t tPrompt = 0;
  ULong64_t tDelay  = 0;

  int decayWindow = 20;

  nbytes += ftree->GetEntry(0);
  ULong64_t pmtTimingArr[4] = {t4, t5, t6, t7};
  UShort_t pmtChargeArr[4]  = {q4, q5, q6, q7};

  tPrompt = t4;
  for (unsigned int j = 1; j < 4; j++) {
    if (pmtTimingArr[j] < tPrompt) tPrompt = pmtTimingArr[j];
  }

  TH1F *histDecay = new TH1F("MuonDecay", "MuonDecay", 200, 0, decayWindow);

  ULong64_t promptIndex    = 0;
  int multipleDelayCounter = 0;

  std::vector<double> hitVec;

  hitVec.clear();

  for (Long64_t i = 1; i < nentries; i++) {
    nbytes += ftree->GetEntry(i);

    ULong64_t pmtTimingArr[4] = {t4, t5, t6, t7};
    UShort_t pmtChargeArr[4]  = {q4, q5, q6, q7};

    tDelay = t4;
    for (unsigned int j = 1; j < 4; j++) {
      if (pmtTimingArr[j] < tDelay) tDelay = pmtTimingArr[j];
    }

    // if ((tDelay - tPrompt) > 700000 && (tDelay - tPrompt) < 20000000) {
    if ((tDelay-tPrompt)>100 && (tDelay - tPrompt) < 20000000) {
      //std::cout << "Delay Found for Prompt Event : " << promptIndex << std::endl;
      multipleDelayCounter++;
      delT = 1. * (1.*tDelay - 1.*tPrompt) / 1000000.;
     
      hitVec.push_back(delT);
    } else {

      promptIndex = i;
      if (hitVec.size() > 1) {
        std::cout << "======================================" << std::endl;
        for (const auto &val : hitVec) {
          std::cout << val << " , ";
        }
        std::cout << std::endl;
      }
      if (multipleDelayCounter > 1) {
        //    std::cout << "============== Multiple Delay Counter : " << multipleDelayCounter << " :: For Event :: " <<
        //    i                  << " ========================" << std::endl;
      }
      if (multipleDelayCounter == 1 && delT > 0.01) {
        // std::cout << "DelT : " << delT << std::endl;
        histDecay->Fill(delT);
      }

      tPrompt              = tDelay;
      multipleDelayCounter = 0;
      hitVec.clear();
    }

    // std::cout << t4 << " : " << t5 << " : " << t6 << " : " << t7 << std::endl;
  }

  histDecay->Draw();
  fApp->Run();
}
