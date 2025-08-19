{
//////////////////////////////////////////////////////////
//   This file has been automatically generated 
//     (Tue Aug 19 14:01:20 2025 by ROOT version6.30/04)
//   from TTree outml/A simple ML Tree
//   found on file: Muon_0_10.root
//////////////////////////////////////////////////////////


//Reset ROOT and connect tree file
/*   gROOT->Reset();
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Muon_0_10.root");
   if (!f) {
      f = new TFile("Muon_0_10.root");
   }
    f->GetObject("outml",tree);
*/

TFile *calib = new TFile("fit.root","r");
TF1 *formula = (TF1*)calib->Get("pol3");
//calib->Close();

TFile *f = new TFile("Muon_0_15.root","r");
TTree *outml = (TTree*)f->Get("outml");


TH1F *hist = new TH1F("hist","hist",100,-50,50);
//Declaration of leaves types
   Double_t        t01;
   Double_t        t02;
   Double_t        t03;
   Double_t        t12;
   Double_t        t13;
   Double_t        t23;
   Double_t        t45;
   Double_t        t67;
   UShort_t        q0;
   UShort_t        q1;
   UShort_t        q2;
   UShort_t        q3;
   UShort_t        q4;
   UShort_t        q5;
   UShort_t        q6;
   UShort_t        q7;
   Short_t         x;
   Short_t         y;

   // Set branch addresses.
   outml->SetBranchAddress("t01",&t01);
   outml->SetBranchAddress("t02",&t02);
   outml->SetBranchAddress("t03",&t03);
   outml->SetBranchAddress("t12",&t12);
   outml->SetBranchAddress("t13",&t13);
   outml->SetBranchAddress("t23",&t23);
   outml->SetBranchAddress("t45",&t45);
   outml->SetBranchAddress("t67",&t67);
   outml->SetBranchAddress("q0",&q0);
   outml->SetBranchAddress("q1",&q1);
   outml->SetBranchAddress("q2",&q2);
   outml->SetBranchAddress("q3",&q3);
   outml->SetBranchAddress("q4",&q4);
   outml->SetBranchAddress("q5",&q5);
   outml->SetBranchAddress("q6",&q6);
   outml->SetBranchAddress("q7",&q7);
   outml->SetBranchAddress("x",&x);
   outml->SetBranchAddress("y",&y);

//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// outml->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

   Long64_t nentries = outml->GetEntries();

   Long64_t nbytes = 0;
   for (Long64_t i=0; i<nentries;i++) {
      nbytes += outml->GetEntry(i);
	hist->Fill(formula->Eval(t02));
   }

   hist->Draw("hist");
}
