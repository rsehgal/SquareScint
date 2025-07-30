#include "Hit.h"
#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "vector"
#include <algorithm>
#include <TApplication.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <set>
// bool SortHits(const std::shared_ptr<Hit> &a, const std::shared_ptr<Hit> &b)
bool SortHits(Hit *a, Hit *b)
{
  return a->Timestamp < b->Timestamp; // Ascending order
}

int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fAPP", NULL, NULL);
  TFile *fp          = new TFile(argv[1], "r");
  TTree *Data_F      = (TTree *)fp->Get("Data_F");

  std::string outfile="output.root";
  if(argc > 2)
	outfile = std::string(argv[2]);

  //TFile *outputFile = new TFile(outfile.c_str(),"RECREATE");

  // Declaration of leaves types
  UShort_t Channel;
  ULong64_t Timestamp;
  UShort_t Board;
  UShort_t Energy;
  UShort_t EnergyShort;
  UInt_t Flags;

  // Set branch addresses.
  Data_F->SetBranchAddress("Channel", &Channel);
  Data_F->SetBranchAddress("Timestamp", &Timestamp);
  Data_F->SetBranchAddress("Board", &Board);
  Data_F->SetBranchAddress("Energy", &Energy);
  Data_F->SetBranchAddress("EnergyShort", &EnergyShort);
  Data_F->SetBranchAddress("Flags", &Flags);

  //     This is the loop skeleton
  //       To read only selected branches, Insert statements like:
  // Data_F->SetBranchStatus("*",0);  // disable all branches
  // TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

  Long64_t nentries = Data_F->GetEntries();

  std::cout << "============Actual number of Entries : " << nentries << " ================" << std::endl;
  Long64_t nbytes = 0;

  Hit h;

  // std::vector<std::shared_ptr<Hit>> vecOfHits;
  std::vector<Hit *> vecOfHits;
  // nentries = 25000;
  int printProgress = 100000;

  if (nentries % 2) nentries -= 1;
  for (Long64_t i = 0; i < nentries; i++) {
    if (!(i % printProgress)) std::cout << "Read " << i << " events...." << std::endl;

    nbytes += Data_F->GetEntry(i);
    h.Set(Channel, Timestamp, Board, Energy, EnergyShort, Flags);
    // h.Print();
    // vecOfHits.push_back(std::make_shared<Hit>(Channel, Timestamp, Board, Energy, EnergyShort, Flags));
    vecOfHits.push_back(new Hit(Channel, Timestamp, Board, Energy, EnergyShort, Flags));
    // vecOfHits[i]->Print();
  }

  std::cout << "Hits vector filled........." << std::endl;
  // TH1F *histPMT  = new TH1F("histPMT", "histPMT", 6, 0, 6);     // ns
  TH1F *histDelT  = new TH1F("DelT", "DelT", 40, -20, 20);     // ns
  TH1F *histQMean = new TH1F("QMean", "QMean", 8192, 0, 8192); // ns
  TH2F *h_dt_q    = new TH2F("h_dt_q", "DeltaT vs Q1; Q1; T1 - T0 (ns)", 1000, 0, 1000, 500, -50, 50);

  TH1F *histDelTSlab = new TH1F("DelTSlab", "DelTSlab", 1000, -20000, 20000); // ns
  TH1F *histQ0       = new TH1F("Q0", "Q0", 512, 0, 8192);                   // ns
  TH1F *histQ1       = new TH1F("Q1", "Q1", 512, 0, 8192);                   // ns
  TH1F *histQ2       = new TH1F("Q2", "Q2", 512, 0, 8192);                   // ns
  TH1F *histQ3       = new TH1F("Q3", "Q3", 512, 0, 8192);                   // ns
  TH1F *histRatio_0_1 = new TH1F("Ratio_0_1","Ratio_0_1",100,0,5);
  TH1F *histRatio_0_2 = new TH1F("Ratio_0_2","Ratio_0_2",100,0,5);
  TH1F *histRatio_0_3 = new TH1F("Ratio_0_3","Ratio_0_3",100,0,5);
  TH1F *histRatio_1_2 = new TH1F("Ratio_1_2","Ratio_1_2",100,0,5);
  TH1F *histRatio_1_3 = new TH1F("Ratio_1_3","Ratio_1_3",100,0,5);
  TH1F *histRatio_2_3 = new TH1F("Ratio_2_3","Ratio_2_3",100,0,5);
  // std::cout << "**************************************" << std::endl;
  std::sort(vecOfHits.begin(), vecOfHits.end(), SortHits);

  std::cout << "Vector size : " << vecOfHits.size() << std::endl;

  // std::vector<std::vector<std::shared_ptr<Hit>>> vecOfVecOfHits;
  std::vector<std::vector<Hit *>> vecOfVecOfHits;

  TH1F *histSize = new TH1F("EventSize", "EventSize", 10, 0, 10);

  ULong64_t start = vecOfHits[0]->Timestamp;
  std::vector<Hit *> vec;
  vec.push_back(vecOfHits[0]);
  for (unsigned int i = 1; i < vecOfHits.size() - 1; i++) {
    if (!(i % printProgress)) std::cout << "Processed " << i << " events...." << std::endl;

    // std::vector<std::shared_ptr<Hit>> vec;
    ULong64_t ts = vecOfHits[i]->Timestamp;
    if ((ts - start) < 20000)
      vec.push_back(vecOfHits[i]);
    else {
      // std::cout << "Inserting the event : Size : " << vec.size() << std::endl;
      std::vector<UShort_t> vecChannels;

      for (unsigned int v = 0; v < vec.size(); v++) {
        vecChannels.push_back(vec[v]->Channel);
      }
      std::set<UShort_t> s(vecChannels.begin(), vecChannels.end());
      if (s.size() == vecChannels.size()) histSize->Fill(vecChannels.size());

      vecOfVecOfHits.push_back(vec);
      start = vecOfHits[i]->Timestamp;
      vec.clear();
      vec.push_back(vecOfHits[i]);
      // i++;
    }
    // vecOfHits[i]->Print();
  }

  std::cout << "Size of vecOfVecOfHits.size : " << vecOfVecOfHits.size() << std::endl;

  ULong64_t sizes[4] = {0, 0, 0, 0};

  for (unsigned int i = 0; i < vecOfVecOfHits.size(); i++) {
    sizes[vecOfVecOfHits[i].size() - 1]++;
  }

  for (unsigned int i = 0; i < 4; i++) {
    std::cout << i << " : " << sizes[i] << std::endl;
  }

  histSize->Draw();

  {

    ULong64_t numEntries = 0;
    for (unsigned int i = 0; i < histSize->GetNbinsX(); i++) {
      if (i > 0) numEntries += ((i - 1) * histSize->GetBinContent(i));
      std::cout << i << " : " << histSize->GetBinContent(i) << std::endl;
    }
    std::cout << "Total Entries from histogram : " << numEntries << std::endl;
  }

  // fp->Close();
  // fApp->Run();
  // return 0 ;
  for (unsigned int i = 0; i < vecOfVecOfHits.size(); i++) {

    bool first  = false;
    bool second = false;
    bool third  = false;
    bool fourth = false;

    if (vecOfVecOfHits[i].size() > 3) {
      // std::cout << "======================================" << std::endl;
      ULong64_t tm0 = 0;
      ULong64_t tm2 = 0;
      UShort_t q0   = 0;
      UShort_t q2   = 0;

      ULong64_t tm1 = 0;
      ULong64_t tm3 = 0;
      UShort_t q1   = 0;
      UShort_t q3   = 0;

      for (unsigned int j = 0; j < vecOfVecOfHits[i].size(); j++) {
        // vecOfVecOfHits[i][j]->Print();
        if (vecOfVecOfHits[i][j]->Channel == 0) {
          tm0 = vecOfVecOfHits[i][j]->Timestamp;
          // std::cout << "Tm0 : " << tm0 << std::endl;
          first = true;
          q0    = vecOfVecOfHits[i][j]->Energy;
        }
        if (vecOfVecOfHits[i][j]->Channel == 1) {
          tm1 = vecOfVecOfHits[i][j]->Timestamp;
          // std::cout << "Tm0 : " << tm0 << std::endl;
          second = true;
          q1     = vecOfVecOfHits[i][j]->Energy;
        }

        if (vecOfVecOfHits[i][j]->Channel == 2) {
          tm2 = vecOfVecOfHits[i][j]->Timestamp;
          // std::cout << "Tm2 : " << tm2 << std::endl;
          third = true;
          q2    = vecOfVecOfHits[i][j]->Energy;
        }
        if (vecOfVecOfHits[i][j]->Channel == 3) {
          tm3 = vecOfVecOfHits[i][j]->Timestamp;
          // std::cout << "Tm0 : " << tm0 << std::endl;
          fourth = true;
          q3     = vecOfVecOfHits[i][j]->Energy;
        }
      }

      // std::cout << "First : " << first << " :: Third : " << third << std::endl;
      if (first && third && second && fourth) {
        // std::cout << tm0 <<" : " << tm2 << std::endl;
        Long64_t delTSlab = static_cast<Long64_t>(tm0) - static_cast<Long64_t>(tm2);
        // std::cout << "DelT : " << delTSlab << std::endl;
        histDelTSlab->Fill(delTSlab);
        h_dt_q->Fill(q0, delTSlab / 1000.);
        // histQMean->Fill(pow(q0*q2*q1*q3,0.25));
        // histQMean->Fill(q0+q2+q1+q3);
        histQMean->Fill(sqrt(sqrt(q0 * q2) * sqrt(q1 * q3)));
        histQ0->Fill(q0);
        histQ1->Fill(q1);
        histQ2->Fill(q2);
        histQ3->Fill(q3);

	histRatio_0_1->Fill(1.*q0/q1);
	histRatio_0_2->Fill(1.*q0/q2);
	histRatio_0_3->Fill(1.*q0/q3);
	histRatio_1_2->Fill(1.*q1/q2);
	histRatio_1_3->Fill(1.*q1/q3);
	histRatio_2_3->Fill(1.*q2/q3);
      }
    }
  }

  histQ0->SetLineColor(1);
  histQ1->SetLineColor(2);
  histQ2->SetLineColor(4);
  histQ3->SetLineColor(8);

  histQ0->SetLineWidth(2);
  histQ1->SetLineWidth(2);
  histQ2->SetLineWidth(2);
  histQ3->SetLineWidth(2);

  histQ0->SetTitle("PMT_0");
  histQ1->SetTitle("PMT_1");
  histQ2->SetTitle("PMT_2");
  histQ3->SetTitle("PMT_3");

  //outputFile->cd();
  //histQ0->Write();
  //histQ1->Write();
  //histQ2->Write();
  //histQ3->Write();

  //outputFile->Close();

  TCanvas *chargeCanvas = new TCanvas("Charge_Q0_Q1_Q2_Q3", "Charge_Q0_Q1_Q2_Q3");
  histQ0->Draw("hist");
  histQ1->Draw("histsames");
  histQ2->Draw("histsames");
  histQ3->Draw("histsames");
  chargeCanvas->BuildLegend();

  new TCanvas;
  histDelTSlab->Draw();

  new TCanvas;
  h_dt_q->Draw();

  new TCanvas;
  histQMean->Draw();

  TCanvas *ratioCan = new TCanvas("ChargeRatio","ChargeRatio");

  histRatio_0_1->SetLineWidth(2);
  histRatio_0_2->SetLineWidth(2);
  histRatio_0_3->SetLineWidth(2);
  histRatio_1_2->SetLineWidth(2);
  histRatio_1_3->SetLineWidth(2);
  histRatio_2_3->SetLineWidth(2);

  histRatio_0_1->SetLineColor(1);
  histRatio_0_2->SetLineColor(2);
  histRatio_0_3->SetLineColor(3);
  histRatio_1_2->SetLineColor(4);
  histRatio_1_3->SetLineColor(6);
  histRatio_2_3->SetLineColor(7);


  histRatio_0_1->SetTitle("C01");
  histRatio_0_2->SetTitle("C02");
  histRatio_0_3->SetTitle("C03");
  histRatio_1_2->SetTitle("C12");
  histRatio_1_3->SetTitle("C13");
  histRatio_2_3->SetTitle("C23");

  histRatio_0_1->Draw("hist");
  histRatio_0_2->Draw("histsames");
  histRatio_0_3->Draw("histsames");
  histRatio_1_2->Draw("histsames");
  histRatio_1_3->Draw("histsames");
  histRatio_2_3->Draw("histsames");
  
  ratioCan->BuildLegend();

  //ULong64_t integral = histQMean->Integral(histQMean->FindBin(500), histQMean->FindBin(4000));

  //std::cout << "Integral : " << integral << std::endl;

  //  return 0;

  /*  new TCanvas("delT","delT");
    histDelT->Draw();
    new TCanvas("qMean","qMean");
    histQMean->Draw();

   */
  //outputFile->Close();
  fApp->Run();
}
