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
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("fApp", NULL, NULL);
  TFile *f           = new TFile(argv[1]);
  TTree *ftree       = (TTree *)f->Get("ftree");

  gStyle->SetOptFit(111);
  // Declaration of leaves types
  //    vector<int>     qVec;
  UShort_t q0;
  UShort_t q1;
  UShort_t q2;
  UShort_t q3;
  UShort_t q4;
  UShort_t q5;
  UShort_t q6;
  UShort_t q7;
  UShort_t q8;
  ULong64_t t0;
  ULong64_t t1;
  ULong64_t t2;
  ULong64_t t3;
  ULong64_t t4;
  ULong64_t t5;
  ULong64_t t6;
  ULong64_t t7;
  ULong64_t t8;

  // Set branch addresses.
  //   ftree->SetBranchAddress("qVec",&qVec);
  ftree->SetBranchAddress("q0", &q0);
  ftree->SetBranchAddress("q1", &q1);
  ftree->SetBranchAddress("q2", &q2);
  ftree->SetBranchAddress("q3", &q3);
  ftree->SetBranchAddress("q4", &q4);
  ftree->SetBranchAddress("q5", &q5);
  ftree->SetBranchAddress("q6", &q6);
  ftree->SetBranchAddress("q7", &q7);
  ftree->SetBranchAddress("q8", &q8);
  ftree->SetBranchAddress("t0", &t0);
  ftree->SetBranchAddress("t1", &t1);
  ftree->SetBranchAddress("t2", &t2);
  ftree->SetBranchAddress("t3", &t3);
  ftree->SetBranchAddress("t4", &t4);
  ftree->SetBranchAddress("t5", &t5);
  ftree->SetBranchAddress("t6", &t6);
  ftree->SetBranchAddress("t7", &t7);
  ftree->SetBranchAddress("t8", &t8);

  //     This is the loop skeleton
  //       To read only selected branches, Insert statements like:
  // ftree->SetBranchStatus("*",0);  // disable all branches
  // TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

  Long64_t nentries = ftree->GetEntries();

  Long64_t nbytes = 0;
  bool prompt     = false;
  bool delay      = false;

  ULong64_t tPrompt = 0;
  ULong64_t tDelay  = 0;
  UShort_t qStop    = 0;

  unsigned int decayEventCounter = 0;

  TH2F *histCorr = new TH2F("HistCorr", "HistCorr", 500, 0, 1000, 200, 0, 200);
  // TH2F *histCorr = new TH2F("HistCorr","HistCorr",50,0,0.2,200,0,200);

std::vector<float> thVec;
std::vector<float> decayVec;

// for(unsigned int m = 0 ; m < 50 ; m++)
{
	//UShort_t qth = 100+ 10*m;	
	UShort_t qth = std::atoi(argv[2]) ;//410;// 300;	
thVec.push_back(qth);

  ///TH1F *histDecay = new TH1F("MuonDecay", "MuonDecay", 2000, 0, 200);
  TH1F *histDecay = new TH1F("MuonDecay", "MuonDecay", 200, 0, 50);
  //UShort_t qth    = std::atoi(argv[2]);//0;//400;

  std::ofstream outfile("delT.txt");

  for (Long64_t i = 0; i < nentries; i++) {
    nbytes += ftree->GetEntry(i);

    ULong64_t pmtTimingArr[4] = {t4, t5, t6, t7};
    UShort_t pmtChargeArr[4]  = {q4, q5, q6, q7};

    if (q4 > qth && q5 > qth && q6 > qth && q7 > qth) 
    {
	Long64_t t45=t4-t5;
	Long64_t t67=t6-t7;
      if (!prompt && abs(t45)<1000 && abs(t67) < 1000) {
        tPrompt = t4;
        for (unsigned int j = 1; j < 4; j++) {
          if (pmtTimingArr[j] < tPrompt) tPrompt = pmtTimingArr[j];
        }

        // tPrompt = (t4 + t5 + t6 + t7) / 4.;
        prompt = true;
        delay  = false;

      } else {
	//UShort_t qthe=1000;
        //if (!delay && q4 > qthe && q5 > qthe && q6 > qthe && q7 > qthe) 
	if(!delay)
	{
          tDelay = t4;
          qStop  = q4;
          for (unsigned int j = 1; j < 4; j++) {
            if (pmtTimingArr[j] < tDelay) {
              tDelay = pmtTimingArr[j];
              qStop  = pmtChargeArr[j];
            }
          }

          // tDelay = (t4 + t5 + t6 + t7) / 4.;
          if ((tDelay - tPrompt) < 50000000) {
	    double delT=1.*(tDelay - tPrompt) / 1000000.;
            histDecay->Fill(delT);
	    outfile << delT << std::endl;
	    
            // std::cout << "Decay event found...." << std::endl;
            delay  = true;
            prompt = false;
            decayEventCounter++;
            // histCorr->Fill(1/sqrt(qStop),(tDelay - tPrompt)/1000000);
            histCorr->Fill(qStop, (tDelay - tPrompt) / 1000000);
          } else {
            tPrompt = tDelay;
          }
        }
      }
      // std::cout << t4 << " : " << t5 << " : " << t6 << " : " << t7 << std::endl;
    }
  }
  TF1 *formu = new TF1("decayEqu", "[Amplitude]*exp(-x/[DecayTime]) + [Offset]", 0, 200);
  // TF1 *formu = new TF1("decayEqu", "[Amplitude]*exp(-x/[DecayTime])", 0, 200);
  formu->SetParameters(100, 2.2, 4);

  histDecay->SetMarkerStyle(8);
  histDecay->Draw("E1 P");
  histDecay->Fit(formu,"Q");
  std::cout << "Decay Time : " << formu->GetParameter(1) << " : Qth : " << qth << std::endl;
  decayVec.push_back(formu->GetParameter(1));
  
  TFile *fout = new TFile("decay.root","RECREATE");
  histDecay->Write();
  fout->Close();
}
/*  outfile.close();
  new TCanvas;
  histCorr->Draw("colz");

  new TCanvas;
  TProfile *prof = histCorr->ProfileX();
  prof->Draw();
*/

/*  TGraph *gr =new TGraph(thVec.size(),&thVec[0],&decayVec[0]);
  gr->SetMarkerStyle(8);
  gr->Draw("ap");
*/
  std::cout << "Total number of decay event : " << decayEventCounter << std::endl;

  /*new TCanvas;
  histDecay->SetMarkerStyle(8);
  histDecay->SetMarkerColor(1);
  histDecay->Draw("E1 P");*/

/*  TF1 *formu = new TF1("decayEqu", "[Amplitude]*exp(-x/[DecayTime]) + [Offset]", 0, 40);
  // TF1 *formu = new TF1("decayEqu", "[Amplitude]*exp(-x/[DecayTime])", 0, 200);
  formu->SetParameters(100, 2.2, 0);
  histDecay->Fit(formu);
*/
 
 fApp->Run();
}
