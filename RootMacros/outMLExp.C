#include <string>
// void outML(char *filename, char *outfilename, double pos,std::string trName){
void outMLExp(char *filename, char *outfilename, double posx, double posy)
{
  //////////////////////////////////////////////////////////
  //   This file has been automatically generated
  //     (Sat Aug  9 13:56:26 2025 by ROOT version6.36.02)
  //   from TTree treeML/A Tree for ML
  //   found on file: output_20_0.root
  //////////////////////////////////////////////////////////

  // Reset ROOT and connect tree file
  //
  //
  TFile *f       = new TFile(filename, "r");
  char *treeName = "treeML";
  // if(trName!="")
  // treeName = trName.c_string();

  // TTree *treeML = (TTree*)f->Get("treeML");
  TTree *treeML = (TTree *)f->Get(treeName);

  TFile *of     = new TFile(outfilename, "RECREATE");
  TTree *mlTree = new TTree("outml", "A simple ML Tree");

  double t01 = 0;
  double t02 = 0;
  double t03 = 0;
  double t12 = 0;
  double t13 = 0;
  double t23 = 0;
  double t45 = 0;
  double t67 = 0;
  UShort_t Q0;
  UShort_t Q1;
  UShort_t Q2;
  UShort_t Q3;
  UShort_t Q4;
  UShort_t Q5;
  UShort_t Q6;
  UShort_t Q7;
  Short_t x = posx;
  Short_t y = posy;

  //ULong64_t p = pos;

  mlTree->Branch("t01", &t01);
  mlTree->Branch("t02", &t02);
  mlTree->Branch("t03", &t03);
  mlTree->Branch("t12", &t12);
  mlTree->Branch("t13", &t13);
  mlTree->Branch("t23", &t23);
  mlTree->Branch("t45", &t45);
  mlTree->Branch("t67", &t67);
  mlTree->Branch("q0", &Q0);
  mlTree->Branch("q1", &Q1);
  mlTree->Branch("q2", &Q2);
  mlTree->Branch("q3", &Q3);
  mlTree->Branch("q4", &Q4);
  mlTree->Branch("q5", &Q5);
  mlTree->Branch("q6", &Q6);
  mlTree->Branch("q7", &Q7);
  mlTree->Branch("x", &x);
  mlTree->Branch("y", &y);

  // Declaration of leaves types
  vector<int> qVec;
  UShort_t q0;
  UShort_t q1;
  UShort_t q2;
  UShort_t q3;
  UShort_t q4;
  UShort_t q5;
  UShort_t q6;
  UShort_t q7;
  ULong64_t t0;
  ULong64_t t1;
  ULong64_t t2;
  ULong64_t t3;
  ULong64_t t4;
  ULong64_t t5;
  ULong64_t t6;
  ULong64_t t7;

  // Set branch addresses.
  treeML->SetBranchAddress("q0",&q0);
  treeML->SetBranchAddress("q1",&q1);
  treeML->SetBranchAddress("q2",&q2);
  treeML->SetBranchAddress("q3",&q3);
  treeML->SetBranchAddress("q4",&q4);
  treeML->SetBranchAddress("q5",&q5);
  treeML->SetBranchAddress("q6",&q6);
  treeML->SetBranchAddress("q7",&q7);
  treeML->SetBranchAddress("t0", &t0);
  treeML->SetBranchAddress("t1", &t1);
  treeML->SetBranchAddress("t2", &t2);
  treeML->SetBranchAddress("t3", &t3);
  treeML->SetBranchAddress("t4", &t4);
  treeML->SetBranchAddress("t5", &t5);
  treeML->SetBranchAddress("t6", &t6);
  treeML->SetBranchAddress("t7", &t7);

/*
  treeML->SetBranchAddress("q0", &q0);
  treeML->SetBranchAddress("q1", &q1);
  treeML->SetBranchAddress("q2", &q2);
  treeML->SetBranchAddress("q3", &q3);
*/
  /*treeML->SetBranchAddress("t4",&t4);
  treeML->SetBranchAddress("t5",&t5);
  treeML->SetBranchAddress("t6",&t6);
  treeML->SetBranchAddress("t7",&t7);*/

  //     This is the loop skeleton
  //       To read only selected branches, Insert statements like:
  // treeML->SetBranchStatus("*",0);  // disable all branches
  // TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

  Long64_t nentries = treeML->GetEntries();
  TH1F *hist        = new TH1F("hist", "hist", 200, -50, 50);
  Long64_t nbytes   = 0;
  for (Long64_t i = 0; i < nentries; i++) {
    nbytes += treeML->GetEntry(i);
    t01 = ((double)t0 - (double)t1) / 1000.;
    t02 = ((double)t0 - (double)t2) / 1000.;
    t03 = ((double)t0 - (double)t3) / 1000.;
    t12 = ((double)t1 - (double)t2) / 1000.;
    t13 = ((double)t1 - (double)t3) / 1000.;
    t23 = ((double)t2 - (double)t3) / 1000.;
    t45 = ((double)t4 - (double)t5) / 1000.;
    t67 = ((double)t6 - (double)t7) / 1000.;

    Q0 = q0;
    Q1 = q1;
    Q2 = q2;
    Q3 = q3;
    Q4 = q4;
    Q5 = q5;
    Q6 = q6;
    Q7 = q7;

    mlTree->Fill();
  }
  mlTree->Write();

  of->Close();
  f->Close();
}
