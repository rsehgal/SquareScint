/*
**	Filename : TestDemo.cpp
**	2026-04-06
**	username : rsehgal
*/
#include "SensorData.h"
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TApplication.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <iomanip>
#include "Helpers.h"
int main(int argc, char *argv[])
{

  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  std::vector<SensorData> vecOfSensorData;
  // gStyle->SetLabelAngle(90, "X");
  //  EXample syntax for insertion
  //  vecOfSensorData.emplace_back(1012., 26.5, 2970);

  // Declaration of leaves types
  Char_t temp[8];
  Char_t pressure[8];
  Long64_t ts;

  TFile *f = new TFile(argv[1], "r");
  TTree *T = (TTree *)f->Get("T");
  // Set branch addresses.
  T->SetBranchAddress("temp", temp);
  T->SetBranchAddress("pressure", pressure);
  T->SetBranchAddress("ts", &ts);

  Long64_t nentries = T->GetEntries();

  Long64_t nbytes = 0;

  for (Long64_t i = 0; i < nentries; i++) {
    nbytes += T->GetEntry(i);

    char *end;
    double temp_d     = strtod(temp, &end);
    double pressure_d = strtod(pressure, &end);
    vecOfSensorData.emplace_back(pressure_d, temp_d, (int)ts);
    std::sort(vecOfSensorData.begin(), vecOfSensorData.end(),
              [](const auto &obj1, const auto &obj2) { return obj1.timestamp < obj2.timestamp; });
    // std::cout << val << std::endl;
  }

  std::cout << "Weather File processed ...." << std::endl;

  std::cout << "--------------------------------------" << std::endl;

  auto current_it                 = vecOfSensorData.begin();
  std::vector<double> pressureVec = get_pressure_vec<SensorData>(current_it, vecOfSensorData.end(), 3600);
  current_it                      = vecOfSensorData.begin();
  std::vector<double> timing      = get_time_vec<SensorData>(current_it, vecOfSensorData.end(), 3600);
  // std::vector<double> timing(pressureVec.size());
  // std::iota(timing.begin(), timing.end(), 1);

  //Trying to smooth it.
  std::vector<double> smoothed(pressureVec.size());
  int i = 0; // Transform raw data into smoothed data
  std::transform(pressureVec.begin(), pressureVec.end(), smoothed.begin(),
                 [&](auto val) { return get_3pt_avg<double>(pressureVec, i++); });

  std::cout << "Size of Pressure Vector : " << pressureVec.size() << std::endl;
  //for (const auto val : pressureVec) {
  for (const auto val : smoothed) {
    std::cout << val << " , ";
  }



  std::cout << "--------------------------------------" << std::endl;
  std::cout << "Size of timing vec : " << timing.size() << std::endl;
  for (const auto val : timing) {
    std::cout << std::setprecision(20) << (int)val << " , ";
  }

  TCanvas *can = new TCanvas("Weather", "Weather", 800, 600);

  // 1. Increase the bottom margin BEFORE drawing
  // Vertical labels are tall; 0.2 makes room for them.
  can->SetBottomMargin(0.2);

  //TGraph *gr = new TGraph(timing.size(), &timing[0], &pressureVec[0]);
  TGraph *gr = new TGraph(timing.size(), &timing[0], &smoothed[0]);
  gr->SetMarkerStyle(8);

  // 2. Draw with "A" (Axis) and "P" (Points)
  gr->Draw("ap");

  // 3. Retrieve the axis ONLY after the graph has been drawn
  TAxis *axis = gr->GetXaxis();
  axis->SetTimeDisplay(1);
  axis->SetTimeFormat("%H:%M:%S");
  axis->SetTimeOffset(0, "gmt");
  axis->SetTitle("Time (HH:MM:SS)");

  // 4. Set Vertical Orientation
  axis->SetLabelOffset(0.02); // Space between axis and label
  axis->LabelsOption("v");    // "v" for vertical

  // 5. CRITICAL: Force the canvas to recognize the changes
  can->Modified();
  can->Update();

  fApp->Run();
  return 0;
}
