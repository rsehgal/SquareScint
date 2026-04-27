/*
**	Filename : Test_Cry.cpp
**	2025-03-18
**	username : rsehgal
*/
#include <iostream>
#include "CryInterface.h"
#include "Muon.h"

#ifdef USE_ROOT
#include "TH1F.h"
#include <TApplication.h>
#include <TVector3.h>
#include <TF1.h>
#include <TCanvas.h>
#endif
int main(int argc, char *argv[])
{
  CryInterface *cryInteface = new CryInterface();
  Muon *muon                = cryInteface->SampleMuon();
  muon->Print();

  muon = cryInteface->SampleMuon();
  muon->Print();

  muon = cryInteface->SampleMuon();
  muon->Print();

  #ifdef USE_ROOT
  unsigned int numOfBins = 40;
  TH1F *hist    = new TH1F("Zenith", "Zenith", numOfBins, 0, M_PI / 2.);
  TH1F *histSolidAngleCorrected    = new TH1F("ZenithSolidCorrected", "Solid Angle corrected Zenith Angle ", numOfBins, 0, M_PI / 2.);
  TH1F *histPhi = new TH1F("Azimuthal", "Azimuthal", 200., -1. * M_PI, M_PI);

  unsigned int numOfMuons = 5000000;


  TApplication *fApp        = new TApplication("fApp", NULL, NULL);
  for (unsigned int i = 0; i < numOfMuons; i++) {

    muon = cryInteface->SampleMuon();
    TVector3 ref(0., -1., 0);
    double angle = TVector3(muon->angleX, muon->angleY, muon->angleZ).Angle(ref);
    TVector3 incoming(muon->angleX, muon->angleY, muon->angleZ);
    hist->Fill(angle);
    histPhi->Fill(std::atan(muon->angleZ / muon->angleX));
    // std::cout << (muon->angleY) <<" : " << (incoming.Mag()) << std::endl;
  }

  
  for(unsigned int i = 1 ; i < hist->GetNbinsX()-1 ; i++){
	double binCenter = hist->GetXaxis()->GetBinCenter(i);
	histSolidAngleCorrected->SetBinContent(i,hist->GetBinContent(i)/(std::sin(binCenter)*std::cos(binCenter)));
  } 


  TF1 *cosSqr = new TF1("cosSqr", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", 0., M_PI / 2.);
  TF1 *cosSqr2 = new TF1("cosSqr_SolidAngleCorrected", "[0]*pow(cos(x),[1])", 0., M_PI / 2.);
  hist->Fit(cosSqr);

  hist->Draw("hist");
  cosSqr->Draw("same");

  new TCanvas;
  histSolidAngleCorrected->Fit(cosSqr2);
  histSolidAngleCorrected->Draw("hist");
  cosSqr2->Draw("same");


  new TCanvas;
  histPhi->Draw("hist");

  fApp->Run();
  #endif
  return 0;
}
