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
#include "Helpers.h"

int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  
  UShort_t qth    = std::atoi(argv[2]); // 410;// 300;
  TH1F *histDecay = GetHist_Exp(argv[1], qth);

  //TF1 *formu = new TF1("decayEqu", "[Amplitude]*exp(-x/[DecayTime]) + [Offset]", 0.06, decayWindow); //Working
  TF1 *formu = new TF1("decayEqu", "[0]*exp(-x/[1]) + [2]*exp(-x/[3])+ [4]", 0.06, decayWindow);
  // TF1 *formu = new TF1("decayEqu", "[Amplitude]*exp(-x/[DecayTime])", 0, 200);
  formu->SetParameters(80, 2.2, 16);
  formu->SetParLimits(0, 0, 1e6);
  //formu->SetParLimits(1, 2.1, 2.6);
  formu->FixParameter(1, 2.19);
  formu->SetParLimits(2, 0, 1e6);
  formu->SetParLimits(3, 1.6, 2.09);
  formu->SetParLimits(4, 0, 1e6);

  histDecay->SetMarkerStyle(8);
  histDecay->Draw("E1 P");
  int status = histDecay->Fit(formu, "RS");
  std::cout <<"=================================" << std::endl;
  std::cout << "Fit STatus : " << status << std::endl;
  std::cout <<"=================================" << std::endl;
  std::cout << "Decay Time : " << formu->GetParameter(1) << " : Qth : " << qth << std::endl;
  // decayVec.push_back(formu->GetParameter(1));
  TFile *f = new TFile("decay.root","UPDATE");
  histDecay->Write();
  f->Close();

  fApp->Run();
  return 0;
}
