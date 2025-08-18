{
//////////////////////////////////////////////////////////
//   This file has been automatically generated 
//     (Mon Aug 18 16:41:08 2025 by ROOT version6.30/04)
//   from TTree treeML/A Tree for ML
//   found on file: ProcessWaveTest.root
//////////////////////////////////////////////////////////


//Reset ROOT and connect tree file
/*   gROOT->Reset();
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ProcessWaveTest.root");
   if (!f) {
      f = new TFile("ProcessWaveTest.root");
   }
    f->GetObject("treeML",tree);
*/
TFile *f = new TFile("ProcessWaveTest.root","r");
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
   TArrayS         *samples0=nullptr;
   TArrayS         *samples1=nullptr;
   TArrayS         *samples2=nullptr;
   TArrayS         *samples3=nullptr;
   TArrayS         *samples4=nullptr;
   TArrayS         *samples5=nullptr;
   TArrayS         *samples6=nullptr;
   TArrayS         *samples7=nullptr;

   // Set branch addresses.
   //treeML->SetBranchAddress("qVec",&qVec);
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
   treeML->SetBranchAddress("samples0",&samples0);
   treeML->SetBranchAddress("samples1",&samples1);
   treeML->SetBranchAddress("samples2",&samples2);
   treeML->SetBranchAddress("samples3",&samples3);
   treeML->SetBranchAddress("samples4",&samples4);
   treeML->SetBranchAddress("samples5",&samples5);
   treeML->SetBranchAddress("samples6",&samples6);
   treeML->SetBranchAddress("samples7",&samples7);

//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// treeML->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

   Long64_t nentries = treeML->GetEntries();

   Long64_t nbytes = 0;
   for (Long64_t i=0; i<nentries;i++) {
      nbytes += treeML->GetEntry(i);

      std::cout <<"=====================================" << std::endl;
      std::cout << "Size : " << samples0->GetSize() << std::endl; 
      /*for(unsigned int m = 0 ; m < samples0->GetSize() ; m++){
		std::cout << "Value : " << samples0->GetAt(m) <<  " : ";
       } 
	std::cout << std::endl;*/
      Short_t maxVal  = *std::max_element(samples0->GetArray(),samples0->GetArray()+samples0->GetSize());
	std::cout << "MaxValue for Sample 0 : Event Num : " << i << " : " << maxVal << std::endl;
/*
 maxVal  = *std::max_element(samples1->GetArray(),samples1->GetArray()+samples1->GetSize());
	std::cout << "MaxValue for Sample 1 : Event Num : " << i << " : " << maxVal << std::endl;
*/
   }
}
