/*
**	Filename : resiAll.C
**	2026-05-19
**	username : rsehgal
*/
#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TF1.h>
#include <vector>
#include <TString.h>
#include <string>
#include <TLegend.h>
#include <TPaveText.h>
#include <TVirtualFitter.h>
#include <TMatrixDSym.h>
#include <TStyle.h>
#include <TRandom.h>
#include "Helpers.h"
#include <TApplication.h>
const double XMIN  = 0.05; // fit start (_s)
const double XMAX  = 15.0; // fit end   (_s)
const int NBINS    = 200;  // histogram bins
const double T_CUT = 50.0; // timestamp cut   (ns)
const int CHANNEL  = 51;

const double HIST_MIN = 0.;   // histogram start (_s)
const double HIST_MAX = 15.0; // histogram end   (_s)

double fstartX = 0.1;         // fit start (_s)
double fendX   = decayWindow; // fit end   (_s)

TH1F *GetHist_Simulation(char *filename, char *single, char *mu)
{
  std::cout << "Processin file : " << filename << std::endl;
  TFile *f  = new TFile(filename, "r");
  TTree *tr = (TTree *)f->Get("geant");
  if (!tr) {
    std::cout << "ERROR: Tree 'geant' not found!" << std::endl;
    return nullptr;
  }

  // ── Configuration ──────────────────────────────────────────
  TH1F *hist = nullptr;
  /*  TString cut = Form(
        "Timestamp > %.1f && Channel==%d "
        "&& creatorProcess==\"Decay\" "
        "&& (particle==\"e+\" || particle==\"e-\")",
        T_CUT, CHANNEL);*/

  TString cut = "";
  if (std::string(single) == "single") {

    if (std::string(mu) == "mu-") {
      hist =
          new TH1F("hist", "(b) #mu- Decay Time Spectrum;Decay Time (#mus);Counts per bin", NBINS, HIST_MIN, HIST_MAX);
      cut = Form("Timestamp > %.1f && Channel==%d "
                 "&& (creatorProcess==\"Decay\" || creatorProcess==\"muMinusCaptureAtRest\")"
                 "&& (particle==\"e-\" || particle==\"e-\")",
                 T_CUT, CHANNEL);
    }

    if (std::string(mu) == "mu+") {
      hist =
          new TH1F("hist", "(a) #mu+ Decay Time Spectrum;Decay Time (#mus);Counts per bin", NBINS, HIST_MIN, HIST_MAX);
      cut = Form("Timestamp > %.1f && Channel==%d "
                 "&& (creatorProcess==\"Decay\" || creatorProcess==\"muMinusCaptureAtRest\")"
                 "&& (particle==\"e+\" || particle==\"e+\")",
                 T_CUT, CHANNEL);
    }
  } else if (std::string(single) == "combine") {
    hist = new TH1F("hist", "Combined Decay Time Spectrum;Decay Time (#mus);Counts per bin", NBINS, HIST_MIN, HIST_MAX);
    cut  = Form("Timestamp > %.1f && Channel==%d "
                 "&& (creatorProcess==\"Decay\" || creatorProcess==\"muMinusCaptureAtRest\")"
                 "&& (particle==\"e+\" || particle==\"e-\")",
                T_CUT, CHANNEL);
  } else {
    hist = new TH1F("hist", "Combined Decay Time Spectrum;Decay Time (#mus);Counts per bin", NBINS, HIST_MIN, HIST_MAX);
    cut  = Form("Timestamp > %.1f && Channel==%d "
                 "&& (creatorProcess==\"Decay\" || creatorProcess==\"muMinusCaptureAtRest\")"
                 "&& (particle==\"e+\" || particle==\"e-\")",
                T_CUT, CHANNEL);
  }

  tr->Draw("Timestamp/1000.>>hist", cut);
  return hist;
}

// The main program is written actuall for simulation
// Now trying to adapt it for exp data.

int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  gStyle->SetOptStat(0000);
  gRandom->SetSeed(42);
  char *filename = argv[1];
  char *single   = argv[2];
  char *mu       = argv[3];
  char *type     = argv[4];
  // For Simulated data
  // TH1F *h      = GetHist_Simulation(filename, single, mu);

  // For Exp data
  TH1F *h = GetHist_Exp(filename);

  TF1 *fitFunc = nullptr;

  if (std::string(single) == "decay" && std::string(type) == "exp") fstartX = 0.7;

  if (std::string(single) == "single" || std::string(single) == "combine" || std::string(single) == "decay") {
    fitFunc = new TF1("fitFunc", "[0]*exp(-x/[1]) + [2]", fstartX, fendX);
    // fitFunc->SetParameters(20000, 2.19, 1);
    double hmax = h->GetMaximum();
    fitFunc->SetParameters(hmax, 2.19, h->GetBinContent(h->GetNbinsX()));
    fitFunc->SetParLimits(0, 0, 1e6);
    fitFunc->SetParLimits(1, 2.0, 2.3); // ← this is what's missing!
    fitFunc->SetParLimits(2, 0, 1e4);

    /*fitFunc->SetParLimits(0, 0, 1e7);   // N+
    fitFunc->SetParLimits(1, 2.1, 2.3); // tau+ — above 2.1
   */
    fitFunc->SetParLimits(2, 0, 1e7); // N-
    fitFunc->SetParNames("N_{+}", "#tau_{+}", "B");

  } else {
    fitFunc = new TF1("fitFunc", "[0]*exp(-x/[1]) + [2]*exp(-x/[3]) + [4]", fstartX, fendX);
    fitFunc->SetParameters(20000, 2.197, 17000, 2.04, 1);
    fitFunc->SetParLimits(0, 0, 1e7);   // N+
    //fitFunc->SetParLimits(1, 2.1, 2.6); // tau+ — above 2.1
    fitFunc->FixParameter(1, 2.19); // tau+ — above 2.1
    fitFunc->SetParLimits(2, 0, 1e7);   // N-
    fitFunc->SetParLimits(3, 1.9, 2.1);   // N-
    fitFunc->SetParLimits(4, 0, 1e7);   // B

    fitFunc->SetParNames("N_{+}", "#tau_{+}", "N_{-}", "#tau_{-}", "B");
    // fitFunc->FixParameter(1, 2.1969);
  }
  double n_plus        = 0;
  double n_plus_err    = 0;
  double n_minus       = 0;
  double n_minus_err   = 0;
  double tau_plus      = 0;
  double tau_plus_err  = 0;
  double tau_minus     = 0;
  double tau_minus_err = 0;
  double chi2          = 0;
  int ndf              = 0;
  double n             = 0;
  double n_err         = 0;
  double b             = 0;
  double b_err         = 0;

  double tau      = 0;
  double tau_err  = 0;
  TF1 *comp_plus  = nullptr;
  TF1 *comp_minus = nullptr;
  if (std::string(single) != "single" && std::string(single) != "combine" && std::string(single) != "decay") {
    std::cout << "Doing Double Exp fit...." << std::endl;
    TFitResultPtr fitResult = h->Fit(fitFunc, "RS");

    //====================
    TVirtualFitter *fitter = TVirtualFitter::GetFitter();
    TMatrixDSym *cov       = new TMatrixDSym(fitFunc->GetNpar());

    // Fill covariance matrix
    for (int i = 0; i < fitFunc->GetNpar(); i++)
      for (int j = 0; j < fitFunc->GetNpar(); j++)
        (*cov)(i, j) = fitter->GetCovarianceMatrixElement(i, j);

    // Compute correlation coefficient between N+ (p0) and N- (p2)
    int i = 0; // N+ is parameter 0
    int j = 2; // N- is parameter 2

    double cov_ij  = (*cov)(i, j);
    double sigma_i = fitFunc->GetParError(i); // __{N+}
    double sigma_j = fitFunc->GetParError(j); // __{N-}

    double rho = cov_ij / (sigma_i * sigma_j);

    std::cout << "Covariance cov(N+,N-)     = " << cov_ij << std::endl;
    std::cout << "sigma(N+)                 = " << sigma_i << std::endl;
    std::cout << "sigma(N-)                 = " << sigma_j << std::endl;
    std::cout << "rho(N+,N-)                = " << rho << std::endl;

    //=====================

    // Extract fit parameters
    n_plus        = fitFunc->GetParameter(0);
    n_plus_err    = fitFunc->GetParError(0);
    n_minus       = fitFunc->GetParameter(2);
    n_minus_err   = fitFunc->GetParError(2);
    tau_plus      = fitFunc->GetParameter(1);
    tau_plus_err  = fitFunc->GetParError(1);
    tau_minus     = fitFunc->GetParameter(3);
    tau_minus_err = fitFunc->GetParError(3);
    chi2          = fitFunc->GetChisquare();
    ndf           = fitFunc->GetNDF();
    b             = fitFunc->GetParameter(4);
    b_err         = fitFunc->GetParError(4);

    // Individual components for plotting
    comp_plus = new TF1("comp_plus", "[0]*exp(-x/[1])", fstartX, fendX);
    comp_plus->SetParameters(fitFunc->GetParameter(0), fitFunc->GetParameter(1));
    comp_plus->SetLineColor(kBlue);
    comp_plus->SetLineStyle(2); // dashed

    comp_minus = new TF1("comp_minus", "[0]*exp(-x/[1])", fstartX, fendX);
    comp_minus->SetParameters(fitFunc->GetParameter(2), fitFunc->GetParameter(3));
    comp_minus->SetLineColor(kGreen + 2);
    comp_minus->SetLineStyle(2); // dashed
  } else {

    std::cout << "@@@@@@@@@ Doing single Exp fit... @@@@@@@@@@" << std::endl;

    TFitResultPtr fitResult = h->Fit(fitFunc, "RS");
    // std::cout <<"====== Second Fit Done ======" << std::endl;
    n       = fitFunc->GetParameter(0);
    n_err   = fitFunc->GetParError(0);
    tau     = fitFunc->GetParameter(1);
    tau_err = fitFunc->GetParError(1);
    chi2    = fitFunc->GetChisquare();
    ndf     = fitFunc->GetNDF();
    b       = fitFunc->GetParameter(2);
    b_err   = fitFunc->GetParError(2);
  }
  // =============================================
  // STEP 4 — Create canvas with two pads
  // =============================================
  TCanvas *c = new TCanvas("c", "Decay Fit", 800, 900);
  c->SetFillColor(0);

  // Histogram formatting
  h->SetMarkerStyle(20);
  h->SetMarkerSize(0.7);
  h->SetMarkerColor(kBlack);
  h->SetLineColor(kBlack);

  // Axis formatting
  h->GetYaxis()->SetTitle("Counts / 50 ns");
  h->GetYaxis()->SetTitleSize(0.055);
  h->GetYaxis()->SetTitleOffset(1.0);
  h->GetYaxis()->SetLabelSize(0.05);
  //  h->GetXaxis()->SetLabelSize(0.5); // hide x labels on upper pad

  h->Draw("EP");

  if (std::string(single) != "single" && std::string(single) != "combine") {
    // Draw fit and components
    //comp_plus->Draw("same");
    //comp_minus->Draw("same");
    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(2);
    fitFunc->Draw("same");

    // Legend
    TLegend *leg = new TLegend(0.55, 0.60, 0.92, 0.88);
    leg->SetBorderSize(1);
    leg->SetFillColor(0);
    leg->SetTextSize(0.04);
    leg->AddEntry(h, "Data", "EP");
    leg->AddEntry(fitFunc, "Total fit", "L");
    leg->AddEntry(comp_plus, "#mu^{+} component", "L");
    leg->AddEntry(comp_minus, "#mu^{-} component", "L");
    leg->Draw();

    // Fit result text box
    TPaveText *pt = new TPaveText(0.13, 0.12, 0.55, 0.42, "NDC");
    pt->SetFillColor(0);
    pt->SetTextFont(42);
    pt->SetBorderSize(1);
    pt->SetTextSize(0.04);
    pt->SetTextAlign(12);
    pt->AddText("N(t) = N_{#mu+} e^{-t/#tau_{#mu+}} +N_{#mu-} e^{-t/#tau_{#mu}}+ B");
    pt->AddText(Form("N_{+} = %.4f #pm %.4f", n_plus, n_plus_err));
    pt->AddText(Form("N_{-} = %.4f #pm %.4f", n_minus, n_minus_err));
    pt->AddText(Form("#tau_{+} = %.4f #pm %.4f #mus", tau_plus, tau_plus_err));
    pt->AddText(Form("#tau_{-} = %.4f #pm %.4f #mus", tau_minus, tau_minus_err));
    pt->AddText(Form("B = %.4f #pm %.4f #mus", b, b_err));
    pt->AddText(Form("#chi^{2}/NDF = %.1f / %d", chi2, ndf));
    // pt->AddText(Form("#chi^{2}/NDF = %.3f", chi2 / (double)ndf));
    pt->Draw();
  } else {

    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(2);
    fitFunc->Draw("same");

    // Legend
    TLegend *leg = new TLegend(0.55, 0.60, 0.92, 0.88);
    leg->SetBorderSize(1);
    leg->SetFillColor(0);
    leg->SetTextSize(0.04);
    leg->AddEntry(h, "Data", "EP");
    leg->AddEntry(fitFunc, "Total fit", "L");
    leg->Draw();

    TPaveText *pt = new TPaveText(0.13, 0.12, 0.55, 0.42, "NDC");
    pt->SetFillColor(0);
    pt->SetTextFont(42);
    pt->SetBorderSize(1);
    pt->SetTextSize(0.04);
    pt->SetTextAlign(12);
    if (std::string(mu) == "mu+") {
      pt->AddText("N_{#mu+}(t) = A_{#mu+} e^{-t/#tau_{#mu+}} + B_{#mu+}");
      pt->AddText(Form("A_{#mu+} = %.4f #pm %.4f", n, n_err));
      pt->AddText(Form("#tau_{#mu+} = %.4f #pm %.4f #mus", tau, tau_err));
      pt->AddText(Form("B_{#mu+} = %.4f #pm %.4f #mus", b, b_err));
      pt->AddText(Form("#chi^{2}/NDF = %.1f / %d", chi2, ndf));
    }
    if (std::string(mu) == "mu-") {
      pt->AddText("N_{#mu-}(t) = A_{#mu-} e^{-t/#tau_{#mu-}} + B_{#mu-}");
      pt->AddText(Form("A_{#mu-} = %.4f #pm %.4f", n, n_err));
      pt->AddText(Form("#tau_{#mu-} = %.4f #pm %.4f #mus", tau, tau_err));
      pt->AddText(Form("B_{#mu-} = %.4f #pm %.4f #mus", b, b_err));
      pt->AddText(Form("#chi^{2}/NDF = %.1f / %d", chi2, ndf));
    }
    if (std::string(single) == "combine") {
      pt->AddText("N(t) = N_{0} e^{-t/#tau_{comb}} + B");
      pt->AddText(Form("N_{0} = %.4f #pm %.4f", n, n_err));
      pt->AddText(Form("#tau_{comb} = %.4f #pm %.4f #mus", tau, tau_err));
      pt->AddText(Form("B = %.4f #pm %.4f #mus", b, b_err));
      pt->AddText(Form("#chi^{2}/NDF = %.1f / %d", chi2, ndf));
    }

    // pt->AddText(Form("#chi^{2}/NDF = %.3f", chi2 / (double)ndf));
    pt->Draw();
  }

  if (std::string(single) == "single") {
    std::cout << "\n========= FIT RESULTS =========" << std::endl;
    std::cout << "N  = " << n << " +/- " << n_err << std::endl;
    std::cout << "tau  = " << tau << " +/- " << tau_err << " us" << std::endl;

  } else {
    std::cout << "\n========= FIT RESULTS =========" << std::endl;
    std::cout << "N+  = " << n_plus << " +/- " << n_plus_err << std::endl;
    std::cout << "N-  = " << n_minus << " +/- " << n_minus_err << std::endl;
    std::cout << "tau+  = " << tau_plus << " +/- " << tau_plus_err << " us" << std::endl;
    std::cout << "tau-  = " << tau_minus << " +/- " << tau_minus_err << " us" << std::endl;
    std::cout << "chi2/NDF = " << chi2 << " / " << ndf << " = " << chi2 / (double)ndf << std::endl;
  }

  fApp->Run();
}
