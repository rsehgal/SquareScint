#include "Helpers.h"
#include "TH1F.h"
#include <vector>

std::vector<double> biasVec={-0.11374400,0.38737635,0.40081115,-0.67444350};
std::vector<double> stdVec={0.79575173,0.76690155,0.74596570,0.76745856};


double GetTiming(std::vector<double> photonArrivalTimeVec)
{

  TH1F *hist = new TH1F("hist", "hist", 1000, 0, 50);
  for (unsigned int i = 0; i < photonArrivalTimeVec.size(); i++) {
    hist->Fill(photonArrivalTimeVec[i]);
  }

  double quantile = 0.10; // 10% quantile
  double qvalue   = 0.0;
  hist->GetQuantiles(1, &qvalue, &quantile);
  delete hist;
  return qvalue;
}
