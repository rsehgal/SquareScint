#include <string>
//void outML(char *filename, char *outfilename, double pos,std::string trName){
void outML_2(char *filename, char *outfilename){
//////////////////////////////////////////////////////////
//   This file has been automatically generated 
//     (Sat Aug  9 13:56:26 2025 by ROOT version6.36.02)
//   from TTree treeML/A Tree for ML
//   found on file: output_20_0.root
//////////////////////////////////////////////////////////


//Reset ROOT and connect tree file
//
//
TFile *f = new TFile(filename,"r");
char *treeName ="delTTree";//"" "treeML";
//if(trName!="")
//treeName = trName.c_string();

//TTree *treeML = (TTree*)f->Get("treeML");
TTree *treeML = (TTree*)f->Get(treeName);


TFile *of = new TFile(outfilename,"RECREATE");
TTree *mlTree = new TTree("outml","A simple ML Tree");

double t01 = 0;
double t02 = 0;
double t03 = 0;
double t12 = 0;
double t13 = 0;
double t23 = 0;
double X = 0.;
double Z =0;

mlTree->Branch("t01",&t01);
mlTree->Branch("t02",&t02);
mlTree->Branch("t03",&t03);
mlTree->Branch("t12",&t12);
mlTree->Branch("t13",&t13);
mlTree->Branch("t23",&t23);
mlTree->Branch("x",&X);
mlTree->Branch("z",&Z);

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
   double       t0;
   double       t1;
   double       t2;
   double       t3;
   double       x;
   double       z;
   ULong64_t       t4;
   ULong64_t       t5;
   ULong64_t       t6;
   ULong64_t       t7;

   // Set branch addresses.
   /*treeML->SetBranchAddress("q0",&q0);
   treeML->SetBranchAddress("q1",&q1);
   treeML->SetBranchAddress("q2",&q2);
   treeML->SetBranchAddress("q3",&q3);
   treeML->SetBranchAddress("q4",&q4);
   treeML->SetBranchAddress("q5",&q5);
   treeML->SetBranchAddress("q6",&q6);
   treeML->SetBranchAddress("q7",&q7);*/
   treeML->SetBranchAddress("t0",&t0);
   treeML->SetBranchAddress("t1",&t1);
   treeML->SetBranchAddress("t2",&t2);
   treeML->SetBranchAddress("t3",&t3);
   treeML->SetBranchAddress("x",&x);
   treeML->SetBranchAddress("z",&z);
   /*treeML->SetBranchAddress("t4",&t4);
   treeML->SetBranchAddress("t5",&t5);
   treeML->SetBranchAddress("t6",&t6);
   treeML->SetBranchAddress("t7",&t7);*/

//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// treeML->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

   Long64_t nentries = treeML->GetEntries();
   TH1F *hist = new TH1F("hist","hist",200,-50,50);
   Long64_t nbytes = 0;
   for (Long64_t i=0; i<nentries;i++) {
      nbytes += treeML->GetEntry(i);
      t01 = t0-t1;
      t02 = t0-t2;
      t03 = t0-t3;
      t12 = t1-t2;
      t13 = t1-t3;
      t23 = t2-t3;
      X = x;
      Z = z;
     mlTree->Fill();
   }
mlTree->Write();

of->Close();
   f->Close();
}
