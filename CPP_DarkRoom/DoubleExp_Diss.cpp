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
#include <TFitResult.h>
using namespace std;

const double XMIN  = 0.05; // fit start (_s)
const double XMAX  = 15.0; // fit end   (_s)
const int NBINS    = 200;  // histogram bins
const double T_CUT = 50.0; // timestamp cut   (ns)
const int CHANNEL  = 51;

const double HIST_MIN = 0.;   // histogram start (_s)
const double HIST_MAX = 15.0; // histogram end   (_s)

double fstartX = 0.1;         // fit start (_s)
double fendX   = decayWindow; // fit end   (_s)

void fit_real_data(TH1F *h)
{

  // =============================================
  // Get histogram properties
  // =============================================
  double maxContent   = h->GetMaximum();
  double totalEntries = h->GetEntries();

  cout << "Max bin content = " << maxContent << endl;
  cout << "Total entries   = " << totalEntries << endl;

  // =============================================
  // Step 1 — Estimate background from late bins
  // t > 10 us: signal negligible
  // =============================================
  TF1 *bkg = new TF1("bkg", "[0]", 10.0, 15.0);
  bkg->SetParameter(0, 10.0); // initial guess
  h->Fit(bkg, "RQ");
  double B_est = max(0.0, bkg->GetParameter(0));
  cout << "Estimated background = " << B_est << endl;

  // =============================================
  // Step 2 — Single exponential with limits
  // =============================================
  TF1 *single = new TF1("single", "[0]*exp(-x/[1]) + [2]", 0.5, 10.0);

  // Set initial parameters
  // N0 ~ max bin content - background
  double N0_est = maxContent - B_est;

  single->SetParameters(N0_est, 2.1, B_est);

  // CRITICAL — set parameter limits
  single->SetParLimits(0, 0, 1e8);        // N0 > 0
  single->SetParLimits(1, 1.5, 2.8);      // tau physical range
  single->SetParLimits(2, 0, maxContent); // B >= 0

  cout << "\nInitial parameters:" << endl;
  cout << "N0 = " << N0_est << endl;
  cout << "tau = 2.1" << endl;
  cout << "B = " << B_est << endl;

  // Fit with logging
  TFitResultPtr result = h->Fit(single, "RS");

  cout << "\n===== SINGLE EXP FIT =====" << endl;
  cout << "Status = " << result->Status() << endl;
  cout << "N0  = " << single->GetParameter(0) << " +/- " << single->GetParError(0) << endl;
  cout << "tau = " << single->GetParameter(1) << " +/- " << single->GetParError(1) << endl;
  cout << "B   = " << single->GetParameter(2) << " +/- " << single->GetParError(2) << endl;
  cout << "chi2/NDF = " << single->GetChisquare() << "/" << single->GetNDF() << " = "
       << single->GetChisquare() / single->GetNDF() << endl;
}

void scan_tmin(TH1F *h)
{

  std::cout << "=============== Start time scan started ====================" << std::endl;
  double tmins[] = {0.2, 0.3, 0.5, 0.8, 1.0, 1.5, 2.0};
  int n          = 7;

  cout << "\n=== tmin SCAN ===" << endl;
  cout << Form("%-8s %-12s %-12s %-12s %-10s", "tmin", "tau(us)", "B", "chi2/NDF", "status") << endl;

  for (int i = 0; i < n; i++) {

    TF1 *f = new TF1(Form("f_%d", i), "[0]*exp(-x/[1]) + [2]", tmins[i], decayWindow);

    // Estimate N0 from histogram at tmin
    int bin       = h->FindBin(tmins[i]);
    double N0_est = h->GetBinContent(bin) - 61.5;

    f->SetParameters(max(N0_est, 100.0), 2.1, 61.5);
    f->SetParLimits(0, 0, 1e6);
    f->SetParLimits(1, 1.5, 2.8);
    f->SetParLimits(2, 0, 200);

    TFitResultPtr r = h->Fit(f, "RSQ");

    cout << Form("%-8.2f %-12.4f %-12.4f %-12.4f %-10d", tmins[i], f->GetParameter(1), f->GetParameter(2),
                 f->GetChisquare() / f->GetNDF(), r->Status())
         << endl;

    delete f;
  }
}
void check_background(TH1F *h)
{

  // Fit background in late time region
  // If truly flat, chi2/NDF should be ~1

  std::cout << "================= Background nature detection started =====================" << std::endl;
  TF1 *flatBkg = new TF1("flatBkg", "[0]", 8.0, decayWindow);
  h->Fit(flatBkg, "R");

  cout << "Background fit (8-" << decayWindow << " us):" << endl;
  cout << "B = " << flatBkg->GetParameter(0) << " +/- " << flatBkg->GetParError(0) << endl;
  cout << "chi2/NDF = " << flatBkg->GetChisquare() / flatBkg->GetNDF() << endl;

  // If chi2/NDF >> 1 then background is not flat
  // This could mean:
  // - Long-lived radioactive contamination
  // - Cosmic ray muons arriving during gate
  // - Electronic noise with time structure

  // Also try linear background
  TF1 *linBkg = new TF1("linBkg", "[0]+[1]*x", 8.0, decayWindow);
  h->Fit(linBkg, "R");
  cout << "\nLinear background fit:" << endl;
  cout << "a = " << linBkg->GetParameter(0) << endl;
  cout << "b = " << linBkg->GetParameter(1) << endl;
  cout << "chi2/NDF = " << linBkg->GetChisquare() / linBkg->GetNDF() << endl;
}

void fit_double_real(TH1F *h){

    // Fix background first
    TF1 *bkg = new TF1("bkg", "[0]", 10.0, 15.0);
    h->Fit(bkg, "RQ");
    double B_fixed = bkg->GetParameter(0);

    // Double exponential from tmin = 1.5 us
    TF1 *f = new TF1("f",
        "[0]*exp(-x/[1]) + [2]*exp(-x/[3]) + [4]",
        1.5, 15.0);

    // Initial parameters
    // Total N ~ 784 at tmin=0.5
    // At tmin=1.5 the amplitude will be lower
    // Estimate from single exp result
    double N_total = 700 * exp(-1.5/2.375);

    f->SetParameters(
        0.55*N_total, 2.19,   // N+, tau+
        0.45*N_total, 2.03,   // N-, tau-
        B_fixed               // B fixed
    );

    // Physical limits
    f->SetParLimits(0, 0,    1e6);
    f->SetParLimits(1, 2.10, 2.40);  // tau+
    f->SetParLimits(2, 0,    1e6);
    f->SetParLimits(3, 1.90, 2.15);  // tau-
    f->FixParameter(4, B_fixed);

    TFitResultPtr r = h->Fit(f, "RS");

    cout << "\n=== DOUBLE EXP FIT (tmin=1.5) ===" << endl;
    cout << "Status = " << r->Status() << endl;
    cout << "tau+  = " << f->GetParameter(1)
         << " +/- " << f->GetParError(1) << endl;
    cout << "tau-  = " << f->GetParameter(3)
         << " +/- " << f->GetParError(3) << endl;
    cout << "R     = " << f->GetParameter(0)/
                          f->GetParameter(2) << endl;
    cout << "rho   = " << r->Correlation(0,2) << endl;
    cout << "chi2/NDF = "
         << f->GetChisquare()/f->GetNDF() << endl;
}

void fit_double_real_v2(TH1F *h)
{

  // Fix background first
  TF1 *bkg = new TF1("bkg", "[0]", 10.0, 15.0);
  h->Fit(bkg, "RQ");
  double B_fixed = bkg->GetParameter(0);

  // Double exponential from tmin = 1.5 us
  //TF1 *f = new TF1("f", "[0]*exp(-x/[1]) + [2]*exp(-x/[3]) + [4]", 0.7, 15.0);
  TF1 *f = new TF1("f", "1.22*[1]*exp(-x/[0]) + [1]*exp(-x/[2]) + [3]", 0.7, 15.0);

  // Initial parameters
  // Total N ~ 784 at tmin=0.5
  // At tmin=1.5 the amplitude will be lower
  // Estimate from single exp result
  double N_total = 700 * exp(-1.5 / 2.375);

/*  f->SetParameters(0.55 * N_total, 2.19, // N+, tau+
                   0.45 * N_total, 2.03, // N-, tau-
                   B_fixed               // B fixed
*/
 f->SetParameters(2.19, // N+, tau+
                   0.45 * N_total, 2.03, // N-, tau-
                   B_fixed               // B fixed

  );

  // Physical limits
/*  f->SetParLimits(0, 0, 1e6);
  f->SetParLimits(1, 2.10, 2.40); // tau+
  f->SetParLimits(2, 0, 1e6);
  f->SetParLimits(3, 1.90, 2.15); // tau-
  f->FixParameter(4, B_fixed);
*/
  f->SetParLimits(0, 2.10, 2.40); // tau+
  f->SetParLimits(1, 0, 1e6);
  f->SetParLimits(2, 1.90, 2.1); // tau-
  f->FixParameter(3, B_fixed);


  TFitResultPtr r = h->Fit(f, "RS");

/*  cout << "\n=== DOUBLE EXP FIT (tmin=1.5) ===" << endl;
  cout << "Status = " << r->Status() << endl;
  cout << "tau+  = " << f->GetParameter(1) << " +/- " << f->GetParError(1) << endl;
  cout << "tau-  = " << f->GetParameter(3) << " +/- " << f->GetParError(3) << endl;
  cout << "R     = " << f->GetParameter(0) / f->GetParameter(2) << endl;
  cout << "rho   = " << r->Correlation(0, 2) << endl;
  cout << "chi2/NDF = " << f->GetChisquare() / f->GetNDF() << endl;*/
}

void fit_double_fixed_tau(TH1F *h){

    TF1 *f = new TF1("f",
        "[0]*exp(-x/[1]) + [2]*exp(-x/[3]) + [4]",
        1.5, 15.0);

    f->SetParameters(350, 2.19, 300, 2.027, 57.0);

    // Fix tau- at theoretical value
    f->FixParameter(3, 2.027);  // tau- fixed
    f->FixParameter(4, 57.0);   // B fixed

    // Only fit N+, tau+, N-
    f->SetParLimits(0, 0,    1e6);
    f->SetParLimits(1, 2.10, 2.35);
    f->SetParLimits(2, 0,    1e6);

    TFitResultPtr r = h->Fit(f, "RS");

    cout << "\n=== FIXED tau- FIT ===" << endl;
    cout << "tau+ = " << f->GetParameter(1)
         << " +/- " << f->GetParError(1) << endl;
    cout << "tau- = 2.027 (fixed)" << endl;
    cout << "R    = " << f->GetParameter(0)/
                         f->GetParameter(2) << endl;
    cout << "rho(N+,N-) = " 
         << r->Correlation(0,2) << endl;
    cout << "chi2/NDF = "
         << f->GetChisquare()/f->GetNDF() << endl;
}
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  gStyle->SetOptStat(0000);
  gRandom->SetSeed(42);
  char *filename = argv[1];
  UShort_t qth   = std::atoi(argv[2]);
  // For Exp data
  TH1F *h = GetHist_Exp(filename, qth);

  // fit_real_data(h);
  scan_tmin(h);

  std::cout << "======================================" << std::endl;

  check_background(h);

  std::cout << "=============== FITTING DOUBLE REAL =======================" << std::endl;
  fit_double_real(h);
  std::cout << "=============== FITTING DOUBLE FIXED TAU  =======================" << std::endl;
  fit_double_fixed_tau(h);


  fApp->Run();

  return 0;

  // Recommended fit procedure for real data
  /*  TF1 *fitSingle = new TF1("fitSingle", "[0]*exp(-x/[1]) + [2]", fstartX, fendX); // choose carefully

    // Key parameters to optimise:
    // tmin: start beyond prompt region
    //       typically 200-500 ns for real data
    // tmax: where statistics become poor
    //       typically 8-10 μs

    // Try different tmin values
    double tmins[] = {0.2, 0.3, 0.5, 0.8, 1.0};
    for (auto tmin : tmins) {
      fitSingle->SetRange(tmin, fendX);
      h->Fit(fitSingle, "R");
      cout << "tmin=" << tmin << " tau=" << fitSingle->GetParameter(1) << " B=" << fitSingle->GetParameter(2) << endl;
    }
    fApp->Run();*/
}
