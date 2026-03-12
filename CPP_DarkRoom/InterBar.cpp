
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <TApplication.h>
#include <TH2F.h>
#include <TRandom3.h>

struct Event {
  UShort_t barId;
  ULong64_t tL;
  ULong64_t tR;
  UShort_t qL;
  UShort_t qR;
  ULong64_t tS;
  Long64_t delT;

  Event() {}

  Event(UShort_t _barId, ULong64_t _tL, ULong64_t _tR, UShort_t _qL, UShort_t _qR)
  {

    barId = _barId;
    tL    = _tL;
    tR    = _tR;
    qL    = _qL;
    qR    = _qR;
    tS    = (tL + tR) / 2.;
    delT = tL-tR;
    /*if (tL < tR)
      tS = tL;
    else
      tS = tR;*/
  }

  void Fill(UShort_t _barId, ULong64_t _tL, ULong64_t _tR, UShort_t _qL, UShort_t _qR)
  {

    barId = _barId;
    tL    = _tL;
    tR    = _tR;
    qL    = _qL;
    qR    = _qR;

    tS = (tL + tR) / 2.;

    delT = tL-tR;
    /*if (tL < tR)
      tS = tL;
    else
      tS = tR;*/
  }

  // Copy Constructor
  Event(const Event &other)
  {
    barId = other.barId;
    tL    = other.tL;
    tR    = other.tR;
    qL    = other.qL;
    qR    = other.qR;
    tS    = other.tS;
    delT    = other.delT;
  }

  // Assignment Operator (The "Equal" Sign)
  Event &operator=(const Event &other)
  {
    if (this != &other) {
      barId = other.barId;
      tL    = other.tL;
      tR    = other.tR;
      qL    = other.qL;
      qR    = other.qR;
      tS    = other.tS;
      delT    = other.delT;
    }
    return *this;
  }
  void Print()
  {
    std::cout << barId << " , " << tL << " , " << tR << " , " << qL << " , " << qR << " , " << tS << std::endl;
  }
};
int main(int argc, char *argv[])
{

  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  TFile *fp          = new TFile(argv[1], "r");
  TTree *ftree       = (TTree *)fp->Get("ftree");

  TRandom3 *rng = new TRandom3(0);

  // Declaration of leaves types
  UShort_t barId;
  ULong64_t tL;
  ULong64_t tR;
  UShort_t qL;
  UShort_t qR;

  // Set branch addresses.
  ftree->SetBranchAddress("barId", &barId);
  ftree->SetBranchAddress("tL", &tL);
  ftree->SetBranchAddress("tR", &tR);
  ftree->SetBranchAddress("qL", &qL);
  ftree->SetBranchAddress("qR", &qR);

  Long64_t nentries = ftree->GetEntries();
  std::cout << "Total number of entries : " << nentries << std::endl;

  Long64_t nbytes = 0;

  // Event startEv;
  // startEv.Fill(barId, tL, tR, qL, qR);

  // Event stopEv;

  TH1F *hist = new TH1F("DelT_InterBar", "DelT_InterBar", 200, -10000, 10000);
  TH1F *histTemp = new TH1F("histTemp", "histTemp", 10, -10000, 10000);
  //TH2F *hist2d = new TH2F("deltblob","deltblob",1000,-10000,10000,100,-10,10);
  TH2F *hist2d = new TH2F("deltblob","deltblob",1000,-60,60,100,-10,10);


  ftree->GetEntry(0);
  Event startEv(barId, tL, tR, qL, qR);
  startEv.Print();

  int sameCounter=0;
  for (Long64_t i = 1; i < nentries - 1; i++) {
    // startEv.Print();
    ftree->GetEntry(i);

    ftree->GetEntry(i);
    Event stopEv(barId, tL, tR, qL, qR);

    // long double delT = (long double)startEv.getMeanTime() - stopEv.getMeanTime();
    Long64_t delT    = (Long64_t)startEv.tS - (Long64_t)stopEv.tS;
    short barId_diff = startEv.barId - stopEv.barId;

    //Long64_t delT;
    if (startEv.barId == 0 && stopEv.barId == 1) {
      delT = (Long64_t)startEv.tS - (Long64_t)stopEv.tS;
    } else if (startEv.barId == 1 && stopEv.barId == 0) {
      delT = (Long64_t)stopEv.tS - (Long64_t)startEv.tS;
    }

    if (std::abs(barId_diff) == 1 && std::abs(delT) < 24000) {
	if(startEv.qL > 500 && startEv.qR > 500)
      hist->Fill(delT);

      std::cout << startEv.delT << " :: " << stopEv.delT << std::endl;

      int startBinNum = histTemp->FindBin(startEv.delT);
      int stopBinNum =  histTemp->FindBin(stopEv.delT);
	std::cout << "startbin : " << startBinNum <<" : : stopBin : "<<  stopBinNum << std::endl;
      
      if (startBinNum==stopBinNum){
double val = rng->Uniform(0, 3);
	sameCounter++;
	//std::cout << "Same bin detected...." << std::endl;
	if(startEv.barId==1)
	hist2d->Fill((double)startEv.delT,0.);
	if(stopEv.barId==1){
	std::cout << "Filling : "<<stopEv.delT<<" , "<<val << std::endl;
	double pos=1.26156+0.00432585*stopEv.delT;
	//hist2d->Fill((double)stopEv.delT,val);
	hist2d->Fill(pos,val);
	}

	}
    }
    startEv = stopEv; // This now works safely and cleanly!
  }
 std::cout << "Total entries with same bins : " << sameCounter << std::endl;
  hist->Draw();

  new TCanvas("blob","blob");
  hist2d->Draw("colz");
  
  fApp->Run();
}
