#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TApplication.h>

int main(int argc, char *argv[]){

	TApplication *fApp = new TApplication("fApp",NULL,NULL);
	TFile *na = new TFile(argv[1],"r");
	TFile *bg = new TFile(argv[2],"r");

	TH1F *histNa = (TH1F*)na->Get("Q0");
	TH1F *histBg = (TH1F*)bg->Get("Q0");

	int nbins = 16384;//histNa->GetNbinsX();
	int startx = 0;
	int endx = 16384;
	
	TH1F *bgSubtracted = new TH1F("BgSub","BgSub",nbins,startx,endx);//(TH1F*)histNa->Clone();

	for(unsigned int i = 0 ; i < nbins ; i++){
		unsigned int naCount = histNa->GetBinContent(i);
		unsigned int bgCount = histBg->GetBinContent(i);
		int diff = naCount-bgCount;
		if(diff > 0)
		bgSubtracted->SetBinContent(i,diff);

	}

	bgSubtracted->Draw("hist");
	fApp->Run();

	return 0;
}


