void enerCalib(char *filename)
{
  std::vector<float> adc   = {50., 60., 100.};
  std::vector<float> comEd = {340., 477., 1060};

  TGraph *gr = new TGraph(adc.size(), &adc[0], &comEd[0]);
  gr->SetMarkerStyle(8);
  TF1 *fit = new TF1("polynom1", "pol1", 40, 120);
  gr->Fit(fit, "R");
  gr->Draw("ap");

  TFile *f      = new TFile(filename);
  TTree *Data_F = nullptr;
  f->GetObject("Data_F", Data_F);

  // Declaration of leaves types
  UShort_t Channel;
  ULong64_t Timestamp;
  UShort_t Energy;

  // Set branch addresses.
  Data_F->SetBranchAddress("Channel", &Channel);
  Data_F->SetBranchAddress("Timestamp", &Timestamp);
  Data_F->SetBranchAddress("Energy", &Energy);

  Long64_t nentries = Data_F->GetEntries();
  Long64_t nbytes   = 0;

  TH1F *hist = new TH1F("Energy","Energy",100,0,4500);
  for (Long64_t i = 0; i < nentries; i++) {
    nbytes += Data_F->GetEntry(i);
    hist->Fill(fit->Eval(Energy));
  }

  TCanvas *canCalib=new TCanvas;
  hist->Draw("hist");

  TLine *edge1 = new TLine(comEd[1], hist->GetMinimum(), comEd[1], hist->GetMaximum());
  TLine *edge2 = new TLine(comEd[2], hist->GetMinimum(), comEd[2], hist->GetMaximum());
  edge1->SetLineColor(kRed);
  edge2->SetLineColor(kRed);
  edge1->SetLineStyle(2);
  edge2->SetLineStyle(2);
  edge1->Draw("same");
  edge2->Draw("same");
  canCalib->SetLogy();
}
