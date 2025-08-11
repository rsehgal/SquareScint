void gr(){
	std::vector<float> yvec={-20.,-10.,10.,20.};
	std::vector<float> xvec={-2686.,-2080.,1564.,2285.};
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
