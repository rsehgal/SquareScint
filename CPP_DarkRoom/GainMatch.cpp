#include <TH1F.h>;
#include <TCanvas.h>
#include <TApplication.h>
#include <TTree.h>
#include <TFile.h>
#include <vector>

#include <TSpectrum.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLine.h>
#include <iostream>
#include <TGraph.h>
#include <TF1.h>

double AutoFindCompton(TH1F *h)
{
  // 1. Find the Photopeaks automatically
  TSpectrum *spec = new TSpectrum(5);                 // Search for up to 5 peaks
  int nfound      = spec->Search(h, 3, "new", 0.002); // 0.10 is threshold

  double *xpeaks = spec->GetPositionX();
  // Sort peaks by X position to distinguish 511 from 1274
  int indices[nfound];
  TMath::Sort(nfound, xpeaks, indices, false);

  for (int i = nfound - 1; i < nfound; i++) {
    double peak_pos = xpeaks[indices[i]];

    // Skip peaks that are too low in energy (noise/backscatter)
    if (peak_pos < 20) continue;

    // 2. Estimate where the Compton Edge should be
    // For Na22: Edge1 is ~66% of Peak1, Edge2 is ~83% of Peak2
    // We use a wide window (20%) around these estimates
    double edge_estimate = peak_pos * (1.0 - 1.0 / (1.0 + (2.0 * peak_pos / 511.0)));
    // Note: The 511.0 above is a scaling factor if your X-axis is keV.
    // If in channels, a simple 0.7 * peak_pos is a good starting search window.

    double search_min = peak_pos * 0.5;
    double search_max = peak_pos * 0.95;

    // 3. Find steepest descent in that window
    double max_slope = 0;
    double edge_loc  = 0;

    for (int b = h->FindBin(search_min); b <= h->FindBin(search_max); ++b) {
      double slope = (h->GetBinContent(b + 1) - h->GetBinContent(b));
      if (slope < max_slope) {
        max_slope = slope;
        edge_loc  = h->GetBinCenter(b);
      }
    }

    printf("Found Peak at %.2f -> Detected Edge at %.2f\n", peak_pos, edge_loc);
    return peak_pos;

    // Visual Markers
    TLine *l = new TLine(edge_loc, 0, edge_loc, h->GetMaximum() * 0.7);
    l->SetLineColor(kBlue);
    l->SetLineStyle(2);
    l->Draw("same");
  }
}

int main()
{

  TApplication *fApp = new TApplication("fApp",NULL,NULL);

  std::vector<float> voltage = {1500, 1525, 1550, 1575, 1600, 1625, 1650, 1675, 1700};//, 1725, 1750};
  std::string name0        = "hist_0_";
  std::string name1        = "hist_1_";
  std::string name2        = "hist_2_";
  std::string name3        = "hist_3_";

  std::vector<float> pmt0;
  std::vector<float> pmt1;
  std::vector<float> pmt2;
  std::vector<float> pmt3;


  for (unsigned int fid = 0; fid < voltage.size(); fid++) {
    std::string fname = "DataF_Bar1_Bar2_Na22_0cm_gain_matching_" + std::to_string((int)voltage[fid]) + "V.root";
    // char *filename             = "DataF_Bar1_Bar2_Na22_0cm_gain_matching_1550V.root";
    TFile *fp = new TFile(fname.c_str(), "r");

    std::cout << "--------------"<< fname << "------------------------" << std::endl;
    TTree *Data_F = (TTree *)fp->Get("Data_F");

    // for (unsigned int fid = 2; fid < 3; fid++) {
    std::string histname0 = name0 + std::to_string(voltage[fid]);
    std::string histname1 = name1 + std::to_string(voltage[fid]);
    std::string histname2 = name2 + std::to_string(voltage[fid]);
    std::string histname3 = name3 + std::to_string(voltage[fid]);
    TH1F *hist_0          = new TH1F(histname0.c_str(), histname0.c_str(), 200, 0, 200);
    TH1F *hist_1          = new TH1F(histname1.c_str(), histname1.c_str(), 200, 0, 200);
    TH1F *hist_2          = new TH1F(histname2.c_str(), histname2.c_str(), 200, 0, 200);
    TH1F *hist_3          = new TH1F(histname3.c_str(), histname3.c_str(), 200, 0, 200);

    // Declaration of leaves types
    UShort_t Channel;
    ULong64_t Timestamp;
    UShort_t Energy;

    // Set branch addresses.
    Data_F->SetBranchAddress("Channel", &Channel);
    Data_F->SetBranchAddress("Timestamp", &Timestamp);
    Data_F->SetBranchAddress("Energy", &Energy);

    Long64_t nentries = Data_F->GetEntries();

    Long64_t nbytes = 0;
    for (Long64_t i = 0; i < nentries; i++) {
      nbytes += Data_F->GetEntry(i);
      if (Channel == 0) hist_0->Fill(Energy);
      if (Channel == 1) hist_1->Fill(Energy);
      if (Channel == 2) hist_2->Fill(Energy);
      if (Channel == 3) hist_3->Fill(Energy);
    }

    double peakPos_0 = AutoFindCompton(hist_0);
    double peakPos_1 = AutoFindCompton(hist_1);
    double peakPos_2 = AutoFindCompton(hist_2);
    double peakPos_3 = AutoFindCompton(hist_3);
  
    pmt0.push_back(peakPos_0);
    pmt1.push_back(peakPos_1);
    pmt2.push_back(peakPos_2);
    pmt3.push_back(peakPos_3);

    fp->Close();
  }
  // hist->Draw();


  TGraph *gr0 = new TGraph(voltage.size(),&pmt0[0],&voltage[0]);
  TGraph *gr1 = new TGraph(voltage.size(),&pmt1[0],&voltage[0]);
  TGraph *gr2 = new TGraph(voltage.size(),&pmt2[0],&voltage[0]);
  TGraph *gr3 = new TGraph(voltage.size(),&pmt3[0],&voltage[0]);


  gr0->SetMarkerStyle(8);  
  gr1->SetMarkerStyle(8);  
  gr2->SetMarkerStyle(8);  
  gr3->SetMarkerStyle(8);  

  gr0->SetLineColor(1);
  gr1->SetLineColor(2);
  gr2->SetLineColor(4);
  gr3->SetLineColor(6);

  TF1 *fit0=new TF1("pol2","pol2");
  TF1 *fit1=new TF1("pol2","pol2");
  TF1 *fit2=new TF1("pol2","pol2");
  TF1 *fit3=new TF1("pol2","pol2");

  new TCanvas("0","0");
  gr0->Draw("alp");
  gr0->Fit(fit0);
  fit0->Draw("same");
  new TCanvas("1","1");
  gr1->Draw("alp");
  gr1->Fit(fit1);
  fit1->Draw("same");
  new TCanvas("2","2");
  gr2->Draw("alp");
  gr2->Fit(fit2);
  fit2->Draw("same");
  new TCanvas("3","3");
  gr3->Draw("alp");
  gr3->Fit(fit3);
  fit3->Draw("same");

  std::cout << "Voltage of Channel 0 : " << fit0->Eval(60) << std::endl;
  std::cout << "Voltage of Channel 1 : " << fit1->Eval(60) << std::endl;
  std::cout << "Voltage of Channel 2 : " << fit2->Eval(60) << std::endl;
  std::cout << "Voltage of Channel 3 : " << fit3->Eval(60) << std::endl;
  fApp->Run();
}
