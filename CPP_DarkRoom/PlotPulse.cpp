#include <TApplication.h>
#include <TArrayS.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TTree.h>
#include <iostream>
#include <vector>
#include <TLine.h>
#include <TAxis.h>

#include <vector>
#include <iostream>
#include <fstream>

//unsigned short preTrigger = 76; //No. of Samples
unsigned short preTrigger = 10;// 32; //No. of Samples
unsigned short timePeriod = 2;  //Time between 2 samples

TLine *GetVerticalLine(TGraph *gr, double xVal)
{
  double yMin  = gr->GetYaxis()->GetXmin();
  double yMax  = gr->GetYaxis()->GetXmax();
  double xMin  = gr->GetXaxis()->GetXmin();
  double xMax  = gr->GetXaxis()->GetXmax();
  TLine *vLine = new TLine(xVal, yMin, xVal, yMax);
  vLine->SetLineColor(kRed);
  return vLine;
}

TLine *GetHorizontalLine(TGraph *gr, double yVal)
{
  double yMin  = gr->GetYaxis()->GetXmin();
  double yMax  = gr->GetYaxis()->GetXmax();
  double xMin  = gr->GetXaxis()->GetXmin();
  double xMax  = gr->GetXaxis()->GetXmax();
  TLine *hLine = new TLine(xMin, yVal, xMax, yVal);
  hLine->SetLineColor(kRed);
  return hLine;
}

std::vector<float> t;
/*struct CFDResult {
    std::vector<float> bipolarPulse;
    float zeroCrossingSample; // Fractional sample index
};

CFDResult CalculateCFD(const std::vector<float>& signal, int delay, float fraction) {
    CFDResult result;
    size_t n = signal.size();
    result.bipolarPulse.assign(n, 0.0f);

    // 1. Generate Bipolar Waveform: B(i) = (Signal[i] * fraction) - Signal[i - delay]
    for (size_t i = delay; i < n; ++i) {
        result.bipolarPulse[i] = (signal[i] * fraction) - signal[i - delay];
    }

    // 2. Find Zero Crossing
    // We look for the transition from positive to negative on the leading edge.
    // Note: Since your pulse is negative-going, the logic depends on polarity.
    result.zeroCrossingSample = -1.0f; // Default if not found

    for (size_t i = delay + 1; i < n; ++i) {
        // Looking for the zero-crossing point on the steep rise/fall
        if (result.bipolarPulse[i-1] > 0 && result.bipolarPulse[i] <= 0) {

            // Linear Interpolation for sub-sample precision:
            // y = mx + c -> solve for y=0
            float y1 = result.bipolarPulse[i-1];
            float y2 = result.bipolarPulse[i];
            float fractionalPart = y1 / (y1 - y2);

            result.zeroCrossingSample = static_cast<float>(i - 1) + fractionalPart;
            break; // Capture the first valid crossing
        }
    }

    return result;
}*/

#include <vector>
#include <cmath>
#include <algorithm>

struct CFDResult {
  std::vector<float> bipolarPulse;
  float zeroCrossingSample;
};

CFDResult CalculateCFD(const std::vector<float> &smoothedSignal, int delay, float fraction, float armingThreshold = 10)
{
  CFDResult result;
  size_t n = smoothedSignal.size();
  result.bipolarPulse.assign(n, 0.0f);

  // 1. Generate Bipolar Waveform
  // Formula: B[i] = (Signal[i] * fraction) - Signal[i - delay]
  for (size_t i = delay; i < n; ++i) {
    result.bipolarPulse[i] = (smoothedSignal[i] * fraction) - smoothedSignal[i - delay];
  }

  // 2. Search for the valid Zero-Crossing
  result.zeroCrossingSample = -1.0f;

  for (size_t i = 1; i < n; ++i) {
    // A: Look for the Negative-to-Positive crossing
    if (result.bipolarPulse[i - 1] < 0 && result.bipolarPulse[i] >= 0) {

      // B: ARMING GATE - Crucial for Negative Pulses
      // Check if the original pulse is deep enough at this moment.
      // Since your pulse is negative, 'smoothedSignal[i]' will be a negative value (e.g., -30).
      // We use the absolute value to compare against your positive arming threshold.
      if (std::abs(smoothedSignal[i]) > armingThreshold) {
        float y1 = result.bipolarPulse[i - 1];
        float y2 = result.bipolarPulse[i];

        // Linear Interpolation for sub-sample precision
        // The distance between samples is 1.0.
        // fractionalPart = distance from (i-1) to the zero point.
        float fractionalPart = (-y1) / (y2 - y1);

        result.zeroCrossingSample = static_cast<float>(i - 1) + fractionalPart;

        // We stop at the first valid crossing that passes the arming gate
        break;
      }
    }
  }
  return result;
}

TGraph *GetGraph(std::vector<float> pulse, char *title = "Sampled Pulse")
{
  TGraph *gr = new TGraph(t.size(), &t[0], &pulse[0]);
  // gr->SetTitle(Form("Pulse %lld;Sample Index;Amplitude", i));
  gr->SetTitle(title);
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(0.5);

  return gr;
}
std::vector<float> BaselineSubtracted(std::vector<float> pulse, int ns)
{
  int n = pulse.size();
  std::vector<float> bs(n);
  int accum = 0;
  for (unsigned int i = 0; i < ns; i++) {
    accum += pulse[i];
  }
  accum /= ns;

  for (int i = 0; i < n; i++) {
    bs[i] = pulse[i] - accum;
  }
  return bs;
}

std::vector<float> MedianFilter(std::vector<float> pulse, int windowSize)
{
  int n = pulse.size();
  std::vector<float> cleaned(n);
  int edge = windowSize / 2;

  for (int i = 0; i < n; i++) {
    std::vector<float> window;
    for (int j = i - edge; j <= i + edge; j++) {
      if (j >= 0 && j < n) window.push_back(pulse[j]);
    }
    // Sort the window to find the median
    std::sort(window.begin(), window.end());
    cleaned[i] = window[window.size() / 2];
  }
  return cleaned;
}

std::vector<float> MovingAverage(std::vector<float> pulse, int ns)
{
  int n = pulse.size();
  std::vector<float> smoothedPulse(n);
  int half = ns / 2;

  for (int i = 0; i < n; i++) {
    float sum = 0;
    int count = 0;
    // Centered window: i - half to i + half
    for (int j = i - half; j <= i + half; j++) {
      if (j >= 0 && j < n) {
        sum += pulse[j];
        count++;
      }
    }
    smoothedPulse[i] = sum / count;
  }
  return smoothedPulse;
}
#if (0)
std::vector<float> MovingAverage(std::vector<float> pulse, unsigned short ns)
{
  int pulseLen = pulse.size();
  if (pulseLen > ns) {

    std::vector<float> smoothedPulse;
    for (unsigned int i = 0; i < pulse.size(); i++) {
      if (i < ns) {
        smoothedPulse.push_back(pulse[i]);
      } else {

        float smoothedValue = 0;
        for (unsigned int j = 0; j < ns; j++) {
          smoothedValue += pulse[i - j];
        }
        smoothedValue /= ns;
        smoothedPulse.push_back(smoothedValue);
      }
    }
    return smoothedPulse;
  } else {
    std::cout << "Pulse size is less than smoothing window size...." << std::endl;
    return pulse;
  }
}
#endif

int main(int argc, char *argv[])
{

  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  TFile *fp          = new TFile(argv[1], "r");

  TTree *Data_F = (TTree *)fp->Get("Data_F");
  // Declaration of leaves types
  UShort_t Channel;
  ULong64_t Timestamp;
  UShort_t Board;
  UShort_t Energy;
  UShort_t EnergyShort;
  UInt_t Flags;
  Int_t Probe;
  TArrayS *Samples;

  // Set branch addresses.
  Data_F->SetBranchAddress("Channel", &Channel);
  Data_F->SetBranchAddress("Timestamp", &Timestamp);
  Data_F->SetBranchAddress("Board", &Board);
  Data_F->SetBranchAddress("Energy", &Energy);
  Data_F->SetBranchAddress("EnergyShort", &EnergyShort);
  Data_F->SetBranchAddress("Flags", &Flags);
  Data_F->SetBranchAddress("Probe", &Probe);
  Data_F->SetBranchAddress("Samples", &Samples);

  //     This is the loop skeleton
  //       To read only selected branches, Insert statements like:
  // Data_F->SetBranchStatus("*",0);  // disable all branches
  // TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

  Long64_t nentries = Data_F->GetEntries();

  // std::vector<float>t;
  // int numOfSamples = 120;
  Data_F->GetEntry(0);
  int numOfSamples = Samples->GetSize();
  std::cout << "Num Of Samples : " << numOfSamples << std::endl;
  for (unsigned int i = 0; i < numOfSamples; i++) {

    t.push_back(timePeriod * i);
  }
  auto pulseNum = std::atoi(argv[2]); // 10;
  std::vector<float> pulse;
  Long64_t nbytes = 0;

   
   nbytes += Data_F->GetEntry(pulseNum);

std::ofstream outfile("pulse.txt");

if (!outfile.is_open()) {
    std::cerr << "Error: Could not open pulse.txt for writing!" << std::endl;
    return 0; // or handle error accordingly
}

for (Int_t j = 0; j < Samples->GetSize(); j++) {
    // 3. Use "\n" instead of std::endl to avoid unnecessary disk flushing
    outfile << j * 2 << "," << Samples->GetAt(j) << "\n";
}

// 4. RAII will close the file automatically, but explicit close is fine
outfile.close();

  for (Long64_t i = 0; i < nentries; i++) {
    nbytes += Data_F->GetEntry(i);

    pulse.clear();
    for (unsigned int j = 0; j < Samples->GetSize(); j++) {
      pulse.push_back(Samples->GetAt(j));
    }

    if (i == pulseNum) {
      // 1. Explicitly create a canvas
      TCanvas *can = new TCanvas("can", "Pulse Plot", 800, 600);
      can->Divide(2, 2);

      std::cout << "Pulse size : " << pulse.size() << std::endl;

      // 2. Create the graph

      // 3. Draw and Force Update
      can->cd(1);
      TGraph *gr = GetGraph(pulse, "Initial Pulse");
      gr->Draw("ACP"); // A=Axes, L=Line, P=Points
      TLine *trigLineRaw = GetVerticalLine(gr,preTrigger*timePeriod);
      trigLineRaw->Draw("same");


      can->cd(2);
      // TGraph *gr2 = GetGraph(MovingAverage(pulse,10),"Smoothed pulse");
      std::vector<float> bs = BaselineSubtracted(MovingAverage(MedianFilter(pulse, 10), 6), 16);
      TGraph *gr2           = GetGraph(bs, "Smoothed pulse");
      // TGraph *gr2 = GetGraph(BaselineSubtracted(pulse,16),"Baseline Subtracted");
      gr2->Draw("ACP"); // A=Axes, L=Line, P=Points
      TLine *trigLine = GetVerticalLine(gr2,preTrigger*timePeriod);
      trigLine->Draw("same");


      can->cd(3);
      CFDResult res = CalculateCFD(bs, 1, 0.3);
      TGraph *gr3   = GetGraph(res.bipolarPulse, "CFD pulse");
      std::cout << "Zero Crossover : " << (res.zeroCrossingSample * timePeriod) << std::endl;
      std::cout << "Coarse Timestamp : " << Timestamp << std::endl;
      std::cout << "====== Assuming PreTrigger is set at  " << (preTrigger*timePeriod) << " ns ===== " << std::endl;
      ULong64_t fineTs = Timestamp + (res.zeroCrossingSample - preTrigger / timePeriod) * timePeriod * 1000.;
      std::cout << "FineTimestamp : " << fineTs << std::endl;
      gr3->Draw("ACP");

      double xVal = res.zeroCrossingSample * timePeriod; // 2.0;

/*
      double yMin = gr3->GetYaxis()->GetXmin();
      double yMax = gr3->GetYaxis()->GetXmax();
      double xMin = gr3->GetXaxis()->GetXmin();
      double xMax = gr3->GetXaxis()->GetXmax();

      TLine *vLine = new TLine(xVal, yMin, xVal, yMax);
      TLine *hLine = new TLine(xMin, 0, xMax, 0);
*/
      TLine *vLine = GetVerticalLine(gr3, xVal);
      TLine *hLine = GetHorizontalLine(gr3, 0.);
      // vLine->SetLineColor(kRed);
      // hLine->SetLineColor(kRed);
      //  vLine->SetLineWidth(2);
      vLine->Draw("same");
      hLine->Draw("same");

      can->Update();
      can->Modified();

      break; // Exit loop to keep this pulse on screen
    }
  }

  fApp->Run();
}
