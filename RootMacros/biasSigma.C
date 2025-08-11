std::vector<double> FitIt(TH1F *hist){
	TF1 *fit = new TF1("gaus","gaus");
	hist->Fit(fit,"q");
   	std::cout << "Mean : " << fit->GetParameter(1) <<" : Sigma : " << fit->GetParameter(2) << std::endl;
	std::vector<double> vec;
	vec.push_back(fit->GetParameter(1));
	vec.push_back(fit->GetParameter(2));
	return vec;
}

void biasSigma(char *filename)
{
//////////////////////////////////////////////////////////
//   This file has been automatically generated 
//     (Sun Aug 10 10:01:34 2025 by ROOT version6.36.02)
//   from TTree outml/A simple ML Tree
//   found on file: p10.root
//////////////////////////////////////////////////////////

    TFile *f = new TFile(filename,"r");
    TTree *outml = (TTree*)f->Get("outml");
//Declaration of leaves types
   Long64_t        t01;
   Long64_t        t02;
   Long64_t        t03;
   Long64_t        t12;
   Long64_t        t13;
   Long64_t        t23;
   Double_t        pos;

   // Set branch addresses.
   outml->SetBranchAddress("t01",&t01);
   outml->SetBranchAddress("t02",&t02);
   outml->SetBranchAddress("t03",&t03);
   outml->SetBranchAddress("t12",&t12);
   outml->SetBranchAddress("t13",&t13);
   outml->SetBranchAddress("t23",&t23);
   outml->SetBranchAddress("pos",&pos);

//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// outml->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

   Long64_t nentries = outml->GetEntries();

   TH1F *hist01 = new TH1F("hist01","hist01",200,-10000,10000);
   TH1F *hist02 = new TH1F("hist02","hist02",200,-10000,10000);
   TH1F *hist03 = new TH1F("hist03","hist03",200,-10000,10000);
   TH1F *hist12 = new TH1F("hist12","hist12",200,-10000,10000);
   TH1F *hist13 = new TH1F("hist13","hist13",200,-10000,10000);
   TH1F *hist23 = new TH1F("hist23","hist23",200,-10000,10000);

   Long64_t nbytes = 0;
   for (Long64_t i=0; i<nentries;i++) {
      nbytes += outml->GetEntry(i);

      hist01->Fill(t01);
      hist02->Fill(t02);
      hist03->Fill(t03);
      hist12->Fill(t12);
      hist13->Fill(t13);
      hist23->Fill(t23);
   }

   std::vector<std::vector<double>>vecOfMeanSigma;
vecOfMeanSigma.push_back(FitIt(hist01));
vecOfMeanSigma.push_back(FitIt(hist02));
vecOfMeanSigma.push_back(FitIt(hist03));
vecOfMeanSigma.push_back(FitIt(hist12));
vecOfMeanSigma.push_back(FitIt(hist13));
vecOfMeanSigma.push_back(FitIt(hist23));

std::vector<double> meanVec;
std::vector<double> sigmaVec;
std::vector<double> biasVec;
std::vector<double> resVec;

for(const auto& vec : vecOfMeanSigma){
	std::cout << "=========================" << std::endl;
for(const auto& val : vec){
	std::cout << val << " : ";
}
std::cout << std::endl;
meanVec.push_back(vec[0]);
sigmaVec.push_back(vec[1]);

}

biasVec.push_back(0.25*(meanVec[0]+meanVec[1]+meanVec[2]));
biasVec.push_back(0.25*(-1.*meanVec[0]+meanVec[3]+meanVec[4]));
biasVec.push_back(0.25*(-1.*meanVec[1]-meanVec[3]+meanVec[5]));
biasVec.push_back(0.25*(-1.*meanVec[2]-meanVec[4]-meanVec[5]));

std::cout <<"======== BIAS Vector ==========" << std::endl;
for(const auto& val : biasVec){
	std::cout << val <<" : ";
}

std::cout << std::endl;

std::vector<double> varVec;

for(const auto& val : sigmaVec){
varVec.push_back(val*val);
}

double sum = std::accumulate(varVec.begin(), varVec.end(), 0);
sum /= 3.;

std::vector<double> sumPerPMTVec;

sumPerPMTVec.push_back(varVec[0]+varVec[1]+varVec[2]);
sumPerPMTVec.push_back(varVec[0]+varVec[3]+varVec[4]);
sumPerPMTVec.push_back(varVec[1]+varVec[3]+varVec[5]);
sumPerPMTVec.push_back(varVec[2]+varVec[4]+varVec[5]);

//x0 = (S0 - S_all/3.0)/2.0
std::vector<double> xvec;
for(const auto& sumPerPMT : sumPerPMTVec ){
	xvec.push_back((sumPerPMT-sum)/2.);
}

std::vector<double> finalSigmaVec;
for(const auto& xval : xvec){
	finalSigmaVec.push_back(xval > 0 ? sqrt(xval) : 0);
}

std::cout <<"======== FINAL SIGMA Vector ==========" << std::endl;
for(const auto& val : finalSigmaVec){
	std::cout << val <<" : ";
}

std::cout << std::endl;


   }
