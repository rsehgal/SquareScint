#include <TApplication.h>
#include <TArrayS.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TLine.h>
#include <TTree.h>
#include <iostream>
#include <vector>

#include <iostream>
#include <vector>

unsigned short preTrigger = 10; // 32-10
unsigned short timePeriod = 2;

std::vector<float> t;

#include <algorithm>
#include <cmath>
#include <vector>

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
      // Since your pulse is negative, 'smoothedSignal[i]' will be a negative
      // value (e.g., -30). We use the absolute value to compare against your
      // positive arming threshold.
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
  TTree *Data_F      = (TTree *)fp->Get("Data_F");
  // Declaration of leaves types
  UShort_t Channel;
  ULong64_t Timestamp;
  UShort_t Board;
  UShort_t Energy;
  UShort_t EnergyShort;
  UInt_t Flags;
  Int_t Probe;
  TArrayS *Samples=nullptr;

  // Set branch addresses.
  Data_F->SetBranchAddress("Channel", &Channel);
  Data_F->SetBranchAddress("Timestamp", &Timestamp);
  Data_F->SetBranchAddress("Board", &Board);
  Data_F->SetBranchAddress("Energy", &Energy);
  Data_F->SetBranchAddress("EnergyShort", &EnergyShort);
  Data_F->SetBranchAddress("Flags", &Flags);
  Data_F->SetBranchAddress("Probe", &Probe);
  Data_F->SetBranchAddress("Samples", &Samples);

  TFile *outFile = new TFile("cfd.root", "RECREATE");
  TTree *Data    = new TTree("Data_F", "A Simple CFD tree");
  UShort_t Channel_out;
  ULong64_t Timestamp_out;
  UShort_t Board_out;
  UShort_t Energy_out;
  UShort_t EnergyShort_out;
  UInt_t Flags_out;
  Int_t Probe_out;
  ULong64_t fineTime;

  Data->Branch("Channel", &Channel_out);
  Data->Branch("Timestamp", &Timestamp_out);
  Data->Branch("Board", &Board_out);
  Data->Branch("Energy", &Energy_out);
  Data->Branch("EnergyShort", &EnergyShort_out);
  Data->Branch("Flags", &Flags_out);
  Data->Branch("Probe", &Probe_out);
  Data->Branch("FineTime", &fineTime);

  //     This is the loop skeleton
  //       To read only selected branches, Insert statements like:
  // Data_F->SetBranchStatus("*",0);  // disable all branches
  // TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

  Long64_t nentries = Data_F->GetEntries();
  std::cout << "Num of Entries : " << nentries << std::endl;
  // std::vector<float>t;
  Data_F->GetEntry(0);
  // int numOfSamples = 120;
  int numOfSamples = Samples->GetSize();

  for (unsigned int i = 0; i < numOfSamples; i++) {

    t.push_back(timePeriod * i);
  }
  // auto pulseNum = std::atoi(argv[2]); // 10;
  std::vector<float> pulse;
  Long64_t nbytes = 0;
  ULong64_t counter=0;

  auto numEv=10000;
  auto interruptLoop=100000;
  for (Long64_t i = 0; i < nentries; i++) {

    if(!(i%numEv))
	std::cout << "Processed : " << i << " events ...." << std::endl;

    if(i > interruptLoop)
	break;

    nbytes += Data_F->GetEntry(i);
    counter++;

    Channel_out     = Channel;
    Board_out       = Board;
    Energy_out      = Energy;
    EnergyShort_out = EnergyShort;
    Flags_out       = Flags;
    Probe_out       = Probe;

    pulse.clear();
    for (unsigned int j = 0; j < Samples->GetSize(); j++) {
      pulse.push_back(Samples->GetAt(j));
    }

    /*if (i == pulseNum) {
      std::cout << "Pulse size : " << pulse.size() << std::endl;
*/

    std::vector<float> bs = BaselineSubtracted(MovingAverage(MedianFilter(pulse, 10), 6), 16);

    CFDResult res = CalculateCFD(bs, 1, 0.3);
    /*unsigned short preTrigger = 32;
    unsigned short timePeriod = 1;*/
    fineTime=(res.zeroCrossingSample - preTrigger) * timePeriod * 1000.;
    ULong64_t fineTs = Timestamp + fineTime;
    // if(res.zeroCrossingSample >= 0)
    {
      Timestamp_out = fineTs;
      Data->Fill();
    }

    /*std::cout << "============================================" << std::endl;
    std::cout << "Zero crossing Sample : " << res.zeroCrossingSample << std::endl;
    std::cout << "preTrigger : " << preTrigger << std::endl;
    std::cout << "FineTime : " << fineTime << std::endl;*/
    //}
  }
  std::cout <<"Counter : " << counter << std::endl;

  Data->Write();
  outFile->Close();
  // fp->Close();
  // fApp->Run();
  // return 0;
}
