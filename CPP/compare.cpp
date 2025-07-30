/*
**	Filename : compare.cpp
**	2025-07-30
**	username : rsehgal
*/
#include<iostream>
#include "TH1F.h"
#include "TFile.h"
#include <vector>
#include <string>
#include "TCanvas.h"
#include "TApplication.h"
#include "TLegend.h"
#include "TStyle.h"
/*
** Expect 2 files to be provided at command prompt for comparison
*/
int main(int argc, char *argv[]){

gStyle->SetOptStat(0);
TApplication *fApp = new TApplication("fApp",NULL,NULL);
TFile *file0 = new TFile(argv[1],"r");
TFile *file1 = new TFile(argv[2],"r");

std::vector<TH1F*> vecOfHist_1;
std::vector<TH1F*> vecOfHist_2;


for(unsigned int i = 0 ; i < 4 ; i++){
std::string pmt="Q"+std::to_string(i);
vecOfHist_1.push_back((TH1F*)file0->Get(pmt.c_str()));
vecOfHist_2.push_back((TH1F*)file1->Get(pmt.c_str()));
}

std::cout << vecOfHist_1.size() << " : " << vecOfHist_2.size() << std::endl;

TCanvas *can = new TCanvas("Comparison","Comparison");
can->Divide(2,2);


for(unsigned int i = 0 ; i < vecOfHist_1.size() ; i++){
can->cd(i+1);
gPad->SetLogy(); 

if(i<2){
gPad->SetBottomMargin(0.001); // Reduce bottom margin
    gPad->SetTickx(0);           // Optional: remove X ticks
}else{
gPad->SetTopMargin(0.001);
}


vecOfHist_1[i]->SetLineColor(2);
//vecOfHist_1[i]->SetTitle("Source");
vecOfHist_1[i]->Draw("hist");

vecOfHist_2[i]->SetLineColor(1);
//vecOfHist_2[i]->SetTitle("Background");
vecOfHist_2[i]->Draw("histsame");

TLegend *leg = new TLegend(0.6, 0.7, 0.88, 0.88); // x1,y1,x2,y2 in NDC
leg->AddEntry(vecOfHist_1[i], "Source", "l");
leg->AddEntry(vecOfHist_2[i], "Background", "l");
leg->SetTextSize(0.03);
leg->Draw();
}

//can->BuildLegend();

fApp->Run();
return 0;

}
