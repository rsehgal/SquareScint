//////////////////////////////////////////////////////////
// ROOT macro: Energy spectra + Coincidence (Polynomial Calib)
//////////////////////////////////////////////////////////

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <TH1F.h>
#include <TApplication.h>
#include <filesystem>

using namespace std;

class Event {
public:
  ULong64_t time;
  UShort_t energy;
  UShort_t channel;

  Event(ULong64_t t, UShort_t e, UShort_t c)
  {
    time    = t;
    energy  = e;
    channel = c;
  }

  void Print() { std::cout << time << " , " << energy << " , " << channel << std::endl; }
};
// Create baskets of 24 ns

int counter = 0;

Long64_t GetDelTFromBasket(std::vector<Event> basket)
{
    // Check if we have at least the two channels we need
    ULong64_t tL = 0;
    ULong64_t tR = 0;
    bool foundL = false;
    bool foundR = false;

    for (const auto& ev : basket) {
        if (ev.channel == 0) {
            tL = ev.time;
            foundL = true;
        }
        if (ev.channel == 1) {
            tR = ev.time;
            foundR = true;
        }
    }

    if (foundL && foundR) {
        counter++; // Increment the global counter only when a valid pair is found
        return (Long64_t)(tL - tR);
    }

    return 0; // Default return if channels 0 and 1 weren't both present
}
#if(0)
Long64_t GetDelTFromBasket(std::vector<Event> basket)
{
  if (basket.size() == 3) {
    counter++;
    std::cout << "--------------------------------------" << std::endl;
    // std::cout << "I : " << i << " : Size of basket : " << basket.size() << std::endl;
    ULong64_t tL = 0;
    ULong64_t tR = 0;
    for (unsigned int j = 0; j < basket.size(); j++) {
      if (basket[j].channel == 0) tL = basket[j].time;
      if (basket[j].channel == 1) tR = basket[j].time;
     
    std::cout <<"TL : " << tL <<" :: TR : " << tR << std::endl;
      basket[j].Print();
    }

    std::cout <<"TLL : " << tL <<" :: TRR : " << tR << std::endl;
    if (tL > 0 && tR > 0) {
      return (tL - tR);
    }
return 0;
  }
}
#endif
void CreateBasket(char *filename)
{
  // const char *filename ="DataF_Bar1_Bar2_VerticallySeparated_1702_1633_1690_1650_coinc_24ns_BG_1hr.root";
  const char *treename = "Data_F";

  TFile *f      = new TFile(filename, "READ");
  TTree *Data_F = (TTree *)f->Get(treename);

  if (!Data_F) {
    cout << "ERROR: TTree Data_F not found!" << endl;
    return;
  }

  // Branch variables
  UShort_t Channel;
  ULong64_t Timestamp;
  UShort_t Energy;

  Data_F->SetBranchAddress("Channel", &Channel);
  Data_F->SetBranchAddress("Timestamp", &Timestamp);
  Data_F->SetBranchAddress("Energy", &Energy);

  vector<Event> events;
  vector<Event> basket; // vector of events within 24ns

  Long64_t nentries = Data_F->GetEntries();
  cout << "Total entries : " << nentries << endl;

  for (Long64_t i = 0; i < nentries; i++) {
    Data_F->GetEntry(i);
    events.emplace_back(Timestamp, Energy, Channel);
  }

  // Sort by timestamp
  sort(events.begin(), events.end(), [](const Event &a, const Event &b) { return a.time < b.time; });

  ULong64_t tL = 0;
  ULong64_t tR = 0;
  UShort_t qL  = 0;
  UShort_t qR  = 0;
  UShort_t ch  = 10000;

  std::string finalFileName = "paired_" + std::string(filename);
   
  TH1F *hist = new TH1F("delT","delT",200,-24000,24000);

  // ULong64_t counter = 0;
  ULong64_t time1 = events[0].time;
  for (size_t i = 0; i < events.size() - 1; i++) {
    ULong64_t time2 = events[i].time;

    Long64_t deltaT = time2 - time1;
    if (abs(deltaT) < 24000) {
      //	std::cout << "deltaT : " << deltaT << std::endl;
      if (events[i].channel == 0 || events[i].channel == 1 || events[i].channel == 4)
        basket.emplace_back(events[i].time, events[i].energy, events[i].channel);
    } else {

      //		std::cout << "--------------------------------------" << std::endl;
      /*if (basket.size() == 3) {
        counter++;
        std::cout << "--------------------------------------" << std::endl;
        std::cout << "I : " << i << " : Size of basket : " << basket.size() << std::endl;
        for (unsigned int j = 0; j < basket.size(); j++) {
          basket[j].Print();
        }
      }*/

/*      Long64_t delT = GetDelTFromBasket(basket);
      std::cout << "DelT : " << delT << std::endl;
      hist->Fill(delT);*/
//GetDelTFromBasket(basket);
//     std::cout <<"DDDelt : " << GetDelTFromBasket(basket) << std::endl;

      Long64_t delT = GetDelTFromBasket(basket);
      if(delT !=0 )hist->Fill(delT);
	//GetDelTFromBasket(basket);
      basket.clear();
      time1 = events[i].time;
    }
  }

   hist->Draw();

  std::cout << "Total Number of baskets formed : " << counter << std::endl;
}

void writePairs(char *filename)
{
  // const char *filename ="DataF_Bar1_Bar2_VerticallySeparated_1702_1633_1690_1650_coinc_24ns_BG_1hr.root";
  const char *treename = "Data_F";

  std::filesystem::path p{std::string(filename)};
  std::string justFilename=p.filename().string();

  TFile *f      = new TFile(filename, "READ");
  TTree *Data_F = (TTree *)f->Get(treename);

  if (!Data_F) {
    cout << "ERROR: TTree Data_F not found!" << endl;
    return;
  }

  // Branch variables
  UShort_t Channel;
  ULong64_t Timestamp;
  UShort_t Energy;

  Data_F->SetBranchAddress("Channel", &Channel);
  Data_F->SetBranchAddress("Timestamp", &Timestamp);
  Data_F->SetBranchAddress("Energy", &Energy);

  // Polynomial calibration coefficients
  double a0 = 2.83742;
  double a1 = 0.00520;
  double a2 = 1.85819e-08;
  double a3 = 4.67899e-12;

  vector<Event> events;

  Long64_t nentries = Data_F->GetEntries();
  cout << "Total entries : " << nentries << endl;

  for (Long64_t i = 0; i < nentries; i++) {
    Data_F->GetEntry(i);
    events.emplace_back(Timestamp, Energy, Channel);
  }

  // Sort by timestamp
  sort(events.begin(), events.end(), [](const Event &a, const Event &b) { return a.time < b.time; });

  ULong64_t tL = 0;
  ULong64_t tR = 0;
  UShort_t qL  = 0;
  UShort_t qR  = 0;
  UShort_t ch  = 10000;

  //std::string finalFileName = "paired_" + std::string(filename);
  std::string finalFileName = "paired_" + justFilename;

  TFile *fout    = new TFile(finalFileName.c_str(), "RECREATE");
  TTree *outTree = new TTree("ftree", "Simple Paired Tree");
  outTree->Branch("barId", &ch);
  outTree->Branch("tL", &tL);
  outTree->Branch("tR", &tR);
  outTree->Branch("qL", &qL);
  outTree->Branch("qR", &qR);

  for (size_t i = 0; i < events.size() - 1; i++) {
    tL = 0;
    tR = 0;
    qL = 0;
    qR = 0;
    ch = 10000;

    Long64_t deltaT = events[i].time - events[i + 1].time;
    UShort_t ch1    = events[i].channel;
    UShort_t ch2    = events[i + 1].channel;

    if (abs(deltaT) < 24000) { //}&& ((ch1 == 0 && ch2 == 1) || (ch1 == 1 && ch2 == 0))) {

      bool cond = ch1 < ch2;
      if (cond)
        cond &= !(ch1 % 2);
      else
        cond = !(ch2 % 2);

      if (cond && abs(ch1 - ch2) == 1) {
        // Define x = ΔT with sign convention
        Long64_t x;
        if (ch1 < ch2) {
          x  = events[i].time - events[i + 1].time;
          tL = events[i].time;
          tR = events[i + 1].time;
          qL = events[i].energy;
          qR = events[i + 1].energy;
          ch = events[i].channel / 2;
          outTree->Fill();
          // std::cout << ch1 << " : " << ch2 << " : barID : " << ch << std::endl;
        } else {
          x  = events[i + 1].time - events[i].time;
          tL = events[i + 1].time;
          tR = events[i].time;
          qL = events[i + 1].energy;
          qR = events[i].energy;
          ch = events[i + 1].channel / 2;
          outTree->Fill();
        }
      }
    }
  }

  outTree->Write();
  fout->Close();
}

int main(int argc, char *argv[])
{

//TApplication *fApp = new TApplication("fApp",NULL,NULL);
   writePairs(argv[1]);
  //CreateBasket(argv[1]);

//fApp->Run();
  return 0;
}
