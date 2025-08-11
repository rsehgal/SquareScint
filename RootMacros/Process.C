void Process(char *filename){
//////////////////////////////////////////////////////////
//   This file has been automatically generated 
//     (Sat Aug  9 13:13:44 2025 by ROOT version6.36.02)
//   from TTree treeML/A Tree for ML
//   found on file: output.root
//////////////////////////////////////////////////////////


//Reset ROOT and connect tree file
TFile *f = new TFile(filename,"r");
TTree *treeML = (TTree*)f->Get("treeML");

//Declaration of leaves types
   vector<int>     qVec;
   UShort_t        q0;
   UShort_t        q1;
   UShort_t        q2;
   UShort_t        q3;
   UShort_t        q4;
   UShort_t        q5;
   UShort_t        q6;
   UShort_t        q7;
   ULong64_t       t0;
   ULong64_t       t1;
   ULong64_t       t2;
   ULong64_t       t3;
   ULong64_t       t4;
   ULong64_t       t5;
   ULong64_t       t6;
   ULong64_t       t7;

   // Set branch addresses.
   treeML->SetBranchAddress("q0",&q0);
   treeML->SetBranchAddress("q1",&q1);
   treeML->SetBranchAddress("q2",&q2);
   treeML->SetBranchAddress("q3",&q3);
   treeML->SetBranchAddress("q4",&q4);
   treeML->SetBranchAddress("q5",&q5);
   treeML->SetBranchAddress("q6",&q6);
   treeML->SetBranchAddress("q7",&q7);
   treeML->SetBranchAddress("t0",&t0);
   treeML->SetBranchAddress("t1",&t1);
   treeML->SetBranchAddress("t2",&t2);
   treeML->SetBranchAddress("t3",&t3);
   treeML->SetBranchAddress("t4",&t4);
   treeML->SetBranchAddress("t5",&t5);
   treeML->SetBranchAddress("t6",&t6);
   treeML->SetBranchAddress("t7",&t7);

//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// treeML->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

   Long64_t nentries = treeML->GetEntries();

   Long64_t nbytes = 0;

   TH1F *hist01 = new TH1F("hist01","hist01",200,-10000,10000);
   TF1 *fitF = new TF1("gaus","gaus");
  for (Long64_t i=0; i<nentries;i++) {
     nbytes += treeML->GetEntry(i);
     Long64_t diff = (t0-t2);
     std::cout << "T01 : " << diff << std::endl;
     hist01->Fill(diff);
  }
  hist01->Draw("hist");
     hist01->Fit(fitF);
     fitF->Draw("same");
}
