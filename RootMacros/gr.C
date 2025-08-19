void gr(){
	std::vector<float> yvec={20.,  15.,  10., 5.,  0.,  -5., -10.,-15.,-20.};
	std::vector<float> xvec={2.43,2.058,1.54,0.61,-0.28,-1.127,  -2.12,-2.48, -3.14};
	TGraph *g = new TGraph(xvec.size(),&xvec[0],&yvec[0]);
	g->SetMarkerStyle(4);
	g->Draw("ap");
	TF1 *pol = new TF1("pol3","pol3",-3000,3000);
	g->Fit(pol);

	TFile *f = new TFile("fit.root","RECREATE");
	g->Write();
	pol->Write();
	f->Close();
}
